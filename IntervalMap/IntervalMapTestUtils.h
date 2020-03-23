#ifndef INTERVALMAPTESTTUILS_H
#define INTERVALMAPTESTTUILS_H

#include "IntervalMap.h"
#include <gtest/gtest.h>

template <class TMap>
void RangeEqualsTo(const TMap &map,
                   const typename TMap::kType &begin,
                   const typename TMap::kType &end,
                   const typename TMap::vType &val)
{
    for (auto i = begin; i < end; i++)
    {
        ASSERT_EQ(map[i], val) << "i = " << (int)i;
    }
}

template <class TMap>
void ShowUnderlyingMap(const TMap& map, std::ostream& stream = std::cout)
{
    for(const auto& elem : map)
    {
        stream << "[" << std::setw(3) << (int)elem.first << "]";
        stream << " = " << elem.second << std::endl;
    }
}

template<class Map>
void ShowMapFromTill(const Map& map, const typename Map::kType& keyBegin, const typename Map::kType& keyEnd, std::ostream& stream = std::cout)
{
  for(auto i=keyBegin; i<keyEnd; i++)
  {
    stream << std::setw(2) << map[i] << ' ';
  }
  stream << std::endl;
  for(auto i=keyBegin; i<keyEnd; i++)
  {
    stream << std::setw(2) << (int)i << ' ';
  }
  stream << std::endl;
}

template <class TMap, class TArr>
bool MapEqSequence(const TMap& map, const TArr& arr)
{
    if(map.size() != arr.size())
    {
        return false;
    }
    auto mapIt = map.cbegin();
    auto arrIt = arr.cbegin();
    /* sizes are equal, can safely iterate through one of them */
    for(; mapIt != map.cend(); mapIt++, arrIt++)
    {
        if(mapIt->second != *arrIt)
        {
            return false;
        }
    }
    return true;
}

template <class TMap>
bool MapEqSequence(const TMap& map, const char* str)
{
    return MapEqSequence(map, std::string(str));
}

template <class TMap>
bool MapEqSequence(const TMap& map, const char& ch)
{
    return MapEqSequence(map, std::string("") + ch);
}

template <class TMap>
bool HasDoubles(const TMap& map)
{
    auto it = map.cbegin();
    auto val = it->second;
    while(++it != map.cend())
    {
        if(val == it->second)
        {
            return true;
        }
        val = it->second;
    }
    return false;
}

#endif //INTERVALMAPTESTTUILS_H