#include "Overlay.h"
#include "ESP.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

ID3D11Device* Overlay::device = nullptr;
ID3D11DeviceContext* Overlay::context = nullptr;
IDXGISwapChain* Overlay::swapChain = nullptr;
ID3D11RenderTargetView* Overlay::renderTarget = nullptr;
ID3D11VertexShader* Overlay::vertexShader = nullptr;
ID3D11PixelShader* Overlay::pixelShader = nullptr;
ID3D11InputLayout* Overlay::inputLayout = nullptr;
ID3D11Buffer* Overlay::vertexBuffer = nullptr;
ID3D11Buffer* Overlay::fontBuffer = nullptr;
HWND Overlay::hwnd = nullptr;
Memory* Overlay::g_memory = nullptr;
int Overlay::width = 0;
int Overlay::height = 0;

const char* vertexShaderCode = R"(
struct VS_INPUT {
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.pos = float4(input.pos.xy, 0.0f, 1.0f);
    output.color = input.color;
    return output;
}
)";

const char* pixelShaderCode = R"(
struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_Target {
    return input.color;
}
)";

bool Overlay::InitShaders() {
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    if (FAILED(D3DCompile(vertexShaderCode, strlen(vertexShaderCode), nullptr, nullptr, nullptr,
        "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vsBlob, &errorBlob))) {
        return false;
    }

    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
    vsBlob->Release();

    ID3DBlob* psBlob = nullptr;
    if (FAILED(D3DCompile(pixelShaderCode, strlen(pixelShaderCode), nullptr, nullptr, nullptr,
        "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &psBlob, &errorBlob))) {
        return false;
    }

    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
    psBlob->Release();

    return true;
}

bool Overlay::CreateBuffers() {
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(Vertex) * 1024; // Размер буфера для вершин
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(device->CreateBuffer(&bufferDesc, nullptr, &vertexBuffer))) {
        return false;
    }

    bufferDesc.ByteWidth = sizeof(Vertex) * 2048; // Для текста
    if (FAILED(device->CreateBuffer(&bufferDesc, nullptr, &fontBuffer))) {
        return false;
    }

    return true;
}

bool Overlay::Init() {
    HWND gameWindow = FindWindowW(L"SDL_app", L"Counter-Strike 2");
    if (!gameWindow) return false;

    RECT gameRect;
    GetClientRect(gameWindow, &gameRect);
    width = gameRect.right - gameRect.left;
    height = gameRect.bottom - gameRect.top;

    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"DX11Overlay";
    RegisterClassEx(&wc);

    POINT point = { gameRect.left, gameRect.top };
    ClientToScreen(gameWindow, &point);

    hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        L"DX11Overlay", L"Overlay",
        WS_POPUP,
        point.x, point.y, width, height,
        nullptr, nullptr,
        GetModuleHandle(nullptr), nullptr
    );

    if (!hwnd) return false;

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 2;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &scd, &swapChain, &device, &featureLevel, &context
    );

    ID3D11Texture2D* backBuffer;
    if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer))) {
        return false;
    }

    device->CreateRenderTargetView(backBuffer, nullptr, &renderTarget);
    backBuffer->Release();

    if (!InitShaders() || !CreateBuffers()) {
        Shutdown();
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    return true;
}

void Overlay::Render() {
    if (!device || !context || !swapChain || !g_memory) return;

    HWND gameWindow = FindWindowW(L"SDL_app", L"Counter-Strike 2");
    if (gameWindow) {
        RECT gameRect;
        GetClientRect(gameWindow, &gameRect);
        POINT point = { gameRect.left, gameRect.top };
        ClientToScreen(gameWindow, &point);
        SetWindowPos(hwnd, HWND_TOPMOST, point.x, point.y,
            gameRect.right - gameRect.left,
            gameRect.bottom - gameRect.top,
            SWP_NOACTIVATE);
    }

    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context->ClearRenderTargetView(renderTarget, clearColor);
    context->OMSetRenderTargets(1, &renderTarget, nullptr);

    context->IASetInputLayout(inputLayout);
    context->VSSetShader(vertexShader, nullptr, 0);
    context->PSSetShader(pixelShader, nullptr, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    ESP::RenderDX11(*g_memory, device, context, vertexBuffer, fontBuffer, width, height);

    swapChain->Present(1, 0);
}

void Overlay::Shutdown() {
    if (fontBuffer) fontBuffer->Release();
    if (vertexBuffer) vertexBuffer->Release();
    if (inputLayout) inputLayout->Release();
    if (pixelShader) pixelShader->Release();
    if (vertexShader) vertexShader->Release();
    if (renderTarget) renderTarget->Release();
    if (swapChain) swapChain->Release();
    if (context) context->Release();
    if (device) device->Release();
    if (hwnd) DestroyWindow(hwnd);

    fontBuffer = nullptr;
    vertexBuffer = nullptr;
    inputLayout = nullptr;
    pixelShader = nullptr;
    vertexShader = nullptr;
    renderTarget = nullptr;
    swapChain = nullptr;
    context = nullptr;
    device = nullptr;
    hwnd = nullptr;
}