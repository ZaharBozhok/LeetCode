#include "IntervalMap.h"
#include <gtest/gtest.h>
#include <limits>

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
            ShowMapFromTill(m_map, std::numeric_limits<Key>::min(), 41);
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
    Value insertVal = '*';
    m_map.assign(insertBegin, insertEnd, insertVal);

    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  insertBegin,
                  initialValue);
    RangeEqualsTo(m_map,
                  insertBegin,
                  insertEnd,
                  insertVal);
    RangeEqualsTo(m_map,
                  insertEnd,
                  std::numeric_limits<Key>::max(),
                  initialValue);
}

TEST_F(IntervalMapTest, SqueezeThroughLeft)
{
    const Key insertBegin = std::numeric_limits<Key>::min(), insertEnd = 10;
    Value insertVal = '*';
    m_map.assign(insertBegin, insertEnd, insertVal);

    RangeEqualsTo(m_map,
                  insertBegin,
                  insertEnd,
                  insertVal);
    RangeEqualsTo(m_map,
                  insertEnd,
                  std::numeric_limits<Key>::max(),
                  initialValue);
}

TEST_F(IntervalMapTest, SqueezeThroughRight)
{
    const Key insertBegin = 10, insertEnd = std::numeric_limits<Key>::max();
    Value insertVal = '*';
    m_map.assign(insertBegin, insertEnd, insertVal);

    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  insertBegin,
                  initialValue);
    RangeEqualsTo(m_map,
                  insertEnd,
                  std::numeric_limits<Key>::max(),
                  insertVal);
}

TEST_F(IntervalMapTest, SqueezeThroughThrough)
{
    m_map.assign(10, 20, '*');
    m_map.assign(13, 17, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 13,'*');
    RangeEqualsTo(m_map, 13, 17,'X');
    RangeEqualsTo(m_map, 17, 20,'*');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(),initialValue);
}

TEST_F(IntervalMapTest, ReversedParametersDoNothing)
{
    m_map.assign(20, 10, '*');

    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  std::numeric_limits<Key>::max(),
                  initialValue);
}

TEST_F(IntervalMapTest, InsertPoint)
{
    Value insertVal = '*';
    m_map.assign(1, 2, insertVal);
    RangeEqualsTo(m_map, 1, 2, insertVal);
}

/* It should have tested full range filling, but it is impossible using this scheme [a,b) */
TEST_F(IntervalMapTest, DISABLED_FullBomb)
{
    Value insertVal = '*';
    m_map.assign(std::numeric_limits<Key>::min(), std::numeric_limits<Key>::max(), insertVal);
    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  std::numeric_limits<Key>::max(),
                  insertVal);
    std::cout << m_map[std::numeric_limits<Key>::max()] << std::endl;
}