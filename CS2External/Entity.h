class Entity {
public:
    Entity(Memory& mem, uintptr_t address) : m_memory(mem), m_address(address) {}

    int GetHealth() const {
        return m_memory.Read<int>(m_address + Offsets::Entity::m_iHealth);
    }

    int GetTeam() const {
        return m_memory.Read<int>(m_address + Offsets::Entity::m_iTeamNum);
    }

    Vector3 GetPosition() const {
        return m_memory.Read<Vector3>(m_address + Offsets::Entity::m_vecOrigin);
    }

    std::string GetPlayerName() const {
        char buffer[32];
        m_memory.ReadArray(m_address + Offsets::Entity::m_szPlayerName, buffer, sizeof(buffer));
        buffer[31] = '\0';
        return std::string(buffer);
    }

    bool IsValid() const {
        return m_address != 0;
    }

private:
    Memory& m_memory;
    uintptr_t m_address;
};