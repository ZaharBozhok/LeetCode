#ifndef INTERVALMAPTESTTUILS_H
#define INTERVALMAPTESTTUILS_H

#include "IntervalMap.h"
#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include <iomanip>
#include <limits>
#include <sstream>

/* utils shouldn't include gtest, so maybe rewrite on bool returning but...*/
template <class TMap>
void RangeEqualsTo(const TMap &map,
                   const typename TMap::kType &begin,
                   const typename TMap::kType &end,
                   const typename TMap::vType &val)
{
    for (int i = (int)begin; i < (int)end; i++)
    {
        ASSERT_EQ(map[i], val) << "i = " << (int)i;
    }
}

template <class TMap>
void ShowUnderlyingMap(const TMap &map, std::ostream &stream = std::cout)
{
    for (const auto &elem : map)
    {
        stream << "[" << std::setw(3) << (int)elem.first << "]";
        stream << " = " << elem.second << std::endl;
    }
}

template <class Map>
void ShowMapFromTill(const Map &map,
                     const typename Map::kType &keyBegin = std::numeric_limits<typename Map::kType>::min(),
                     const typename Map::kType &keyEnd = std::numeric_limits<typename Map::kType>::max(),
                     std::ostream &stream = std::cout)
{
    for (int i = (int)keyBegin; i < (int)keyEnd; i++)
    {
        stream << std::setw(2) << map[i] << ' ';
    }
    stream << std::endl;
    for (int i = (int)keyBegin; i < (int)keyEnd; i++)
    {
        stream << std::setw(2) << i << ' ';
    }
    stream << std::endl;
}

template <class TMap, class TArr>
bool MapEqSequence(const TMap &map, const TArr &arr)
{
    if (map.size() != arr.size())
    {
        return false;
    }
    auto mapIt = map.cbegin();
    auto arrIt = arr.cbegin();
    /* sizes are equal, can safely iterate through one of them */
    for (; mapIt != map.cend(); mapIt++, arrIt++)
    {
        if (mapIt->second != *arrIt)
        {
            return false;
        }
    }
    return true;
}

template <class TMap>
bool MapEqSequence(const TMap &map, const char *str)
{
    return MapEqSequence(map, std::string(str));
}

template <class TMap>
bool MapEqSequence(const TMap &map, const char &ch)
{
    return MapEqSequence(map, std::string("") + ch);
}

template <class TMap>
bool HasDoubles(const TMap &map)
{
    auto it = map.cbegin();
    auto val = it->second;
    while (++it != map.cend())
    {
        if (val == it->second)
        {
            return true;
        }
        val = it->second;
    }
    return false;
}

const int TestKeyMin = 0;
const int TestKeyMax = 41;

/* To provide the only functional specified in task */
class TestKey
{
public:
    TestKey() : m_val(int()) {}
    TestKey(const int &val) : m_val(val) {}
    explicit operator int() const { return m_val; }
    bool operator!=(const TestKey& val) = delete;
    bool operator>(const TestKey& val) = delete;
    bool operator>=(const TestKey& val) = delete;
    bool operator<=(const TestKey& val) = delete;
    bool operator<(const TestKey &num) const
    {
        return m_val < num.m_val;
    }
    friend std::ostream &operator<<(std::ostream &os, const TestKey &num)
    {
        os << num.m_val;
        return os;
    }
private:
    int m_val;
};

namespace std
{
template <>
class numeric_limits<TestKey>
{
public:
    static TestKey min() { return TestKey(TestKeyMin); };
    static TestKey max() { return TestKey(TestKeyMax); };
};
} // namespace std

/* To provide the only functional specified in task */
class TestValue
{
public:
    //TestValue() : m_val(char()) {}
    TestValue(const char& val) : m_val(val) {}
    operator char() const {return m_val;}
    bool operator==(const TestValue& val)
    {
        return m_val == val.m_val;
    }
    bool operator!=(const TestValue& val) = delete;
    bool operator>(const TestValue& val) = delete;
    bool operator<(const TestValue& val) = delete;
    bool operator>=(const TestValue& val) = delete;
    bool operator<=(const TestValue& val) = delete;
    friend std::ostream &operator<<(std::ostream &os, const TestValue &val)
    {
        os << val.m_val;
        return os;
    }
private:
    char m_val;
};

#endif //INTERVALMAPTESTTUILS_H