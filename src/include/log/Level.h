#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>

class Level
{
public:
    enum Type
    {
        FATAL,
        ERROR,
        WARN,
        NOTICE,
        INFO,
        DEBUG,
        MAX,
    };

    Level(Level::Type level = INFO);

    std::string Str() const;

    const Level& operator = (Level::Type level);
    const Level& operator = (const Level &other);

    bool operator == (const Level &other) const;
    bool operator != (const Level &other) const;

    bool operator <= (const Level &other) const;
    bool operator >= (const Level &other) const;

    bool operator < (const Level &other) const;
    bool operator > (const Level &other) const;

private:
    Level::Type        m_level;

    static const char* s_LevelStr[Level::Type::MAX];
};


#endif //__LEVEL_H__
