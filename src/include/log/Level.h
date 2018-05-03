#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>

class Level
{
public:
    enum LevelType
    {
        LEVEL_FATAL,
        LEVEL_ERROR,
        LEVEL_WARN,
        LEVEL_NOTICE,
        LEVEL_INFO,
        LEVEL_DEBUG,
        LEVEL_MAX,
    };

    Level(LevelType level = LEVEL_INFO);

    std::string Str() const;

    const Level& operator= (LevelType level);

    const Level& operator= (const Level &other);

    bool operator == (const Level &other) const;

private:
    LevelType          m_level;

    static const char* s_LevelStr[LEVEL_MAX];
};


#endif //__LEVEL_H__
