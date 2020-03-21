#include "IntervalMap.h"
#include <gtest/gtest.h>
#include <limits>

template <class TMap>
void RangeEqualsTo(const TMap &map,
                   const typename TMap::kType &begin,
                   const typename TMap::kType &end,
                   const typename TMap::vType &val)
{
    for (auto i = begin; i < end - 1; i++)
    {
        ASSERT_EQ(map[i], val) << "i = " << (int)i;
    }
}

const char initialValue = 'A'; 

class IntervalMapTest : public ::testing::Test
{
public:
    IntervalMapTest() : m_map(initialValue)
    {
    }

protected:
    void TearDown() override
    {
        if (::testing::Test::HasFailure())
        {
            auto min = std::numeric_limits<Key>::min();
            auto max = std::numeric_limits<Key>::max();
            std::cout << "min : " << (int)min << ", max : " << (int)max << std::endl;
            for (auto i = min; i < max; i++)
            {
                std::cout << (int)i << ':' << m_map[i] << ' ';
            }
        }
    }
    IntervalMap<uint8_t, char> m_map;
    using Key = decltype(m_map)::kType;
    using Value = decltype(m_map)::vType;
};

TEST_F(IntervalMapTest, FilledWithInitialValue)
{
    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  std::numeric_limits<Key>::max(),
                  initialValue);
}

TEST_F(IntervalMapTest, SqueezeThrough)
{
    const Key insertBegin = 10, insertEnd = 20;
    Value insertVal = 'B';
    m_map.assign(insertBegin, insertEnd, insertVal);

    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  insertBegin,
                  initialValue);
    RangeEqualsTo(m_map,
                  insertBegin,
                  insertEnd,
                  'B');
    RangeEqualsTo(m_map,
                  insertEnd,
                  std::numeric_limits<Key>::max(),
                  initialValue);
}