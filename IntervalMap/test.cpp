#include "IntervalMap.h"
#include <gtest/gtest.h>
#include <limits>

using K = uint8_t;

template <class TMap>
bool RangeEqualsTo(const TMap &map,
                   const typename TMap::kType &begin,
                   const typename TMap::kType &end,
                   const typename TMap::vType &val)
{
    for (auto i = begin; i < end; i++)
    {
        if (map[i] != val)
            return false;
    }
    return true;
}

TEST(IntervalMap, FilledWithInitialValue)
{
    IntervalMap<K, char> map('A');
    ASSERT_TRUE(
        RangeEqualsTo(map,
                      std::numeric_limits<K>::min(),
                      std::numeric_limits<K>::max(),
                      'A'));
}

TEST(IntervalMap, SqueezeThrough)
{
    char initialVal = 'A';
    IntervalMap<K, decltype(initialVal)> map(initialVal);

    const K insertBegin = 10, insertEnd = 20;
    const decltype(initialVal) insertVal = 'B';
    map.assign(insertBegin, insertEnd, insertVal);

    ASSERT_TRUE(
        RangeEqualsTo(map,
                      std::numeric_limits<K>::min(),
                      insertBegin,
                      initialVal));
    ASSERT_TRUE(
        RangeEqualsTo(map,
                      insertBegin,
                      insertEnd,
                      insertVal));
    ASSERT_TRUE(
        RangeEqualsTo(map,
                      insertEnd,
                      std::numeric_limits<K>::max(),
                      initialVal));
}