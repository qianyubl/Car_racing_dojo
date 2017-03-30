#pragma once

class Team
{
public:
    Team(int p_id) : m_id(p_id)
    {

    }

    bool operator==(const Team& p_rhs) const
    {
        return m_id == p_rhs.m_id;
    }

private:
    int m_id;
};
