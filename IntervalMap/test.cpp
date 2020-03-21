#include <gtest/gtest.h>
#include <limits>
#include "IntervalMap.h"

using K = uint8_t;

TEST(IntervalMap, FilledWithInitialValue)
{
    char value = 'A';
    IntervalMap<K, decltype(value)> map(value);

    for(auto i=std::numeric_limits<K>::min(); i<std::numeric_limits<K>::max(); i++)
    {
        ASSERT_EQ(map[i], value);
    }
}