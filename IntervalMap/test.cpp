#include "IntervalMap.h"
#include <gtest/gtest.h>
#include <limits>
#include <iomanip>

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
void ShowUnderlyingMap(const TMap& map)
{
    for(const auto& elem : map)
    {
        std::cout << "[" << std::setw(3) << (int)elem.first << "]";
        std::cout << " = " << elem.second << std::endl;
    }
}

template <class TMap>
void AssertUnderlyingMapSizeEq(const TMap& map, const size_t& size)
{
    ASSERT_EQ(map.getMap().size(), size);
}

const char initialValue = '-';

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
            std::cout << "Underlying map : " << std::endl;
            ShowUnderlyingMap(m_map.getMap());
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
    AssertUnderlyingMapSizeEq(m_map, 1);
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
    AssertUnderlyingMapSizeEq(m_map, 3);        
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

    AssertUnderlyingMapSizeEq(m_map, 2);
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
    AssertUnderlyingMapSizeEq(m_map, 3);
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
    AssertUnderlyingMapSizeEq(m_map, 5);
}

TEST_F(IntervalMapTest, ReversedParametersDoNothing)
{
    m_map.assign(20, 10, '*');

    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  std::numeric_limits<Key>::max(),
                  initialValue);
    AssertUnderlyingMapSizeEq(m_map, 1);
}

TEST_F(IntervalMapTest, InsertPoint)
{
    Value insertVal = '*';
    m_map.assign(1, 2, insertVal);
    RangeEqualsTo(m_map, 1, 2, insertVal);
    AssertUnderlyingMapSizeEq(m_map, 3);
}

TEST_F(IntervalMapTest, Neighbours)
{
    m_map.assign(10, 15, 'L');
    m_map.assign(20, 25, 'R');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 15, 'L');
    RangeEqualsTo(m_map, 15, 19, initialValue);
    RangeEqualsTo(m_map, 20, 25, 'R');
    RangeEqualsTo(m_map, 25, std::numeric_limits<Key>::max(),initialValue);
    AssertUnderlyingMapSizeEq(m_map, 5);
}

TEST_F(IntervalMapTest, CloseNeighbours)
{
    m_map.assign(10, 15, 'L');
    m_map.assign(15, 20, 'R');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 15, 'L');
    RangeEqualsTo(m_map, 15, 20, 'R');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(),initialValue);
    AssertUnderlyingMapSizeEq(m_map, 4);
}

TEST_F(IntervalMapTest, FullElimination)
{
    m_map.assign(10, 20, '*');
    m_map.assign(9, 21, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 9, initialValue);
    RangeEqualsTo(m_map, 9, 21,'X');
    RangeEqualsTo(m_map, 21, std::numeric_limits<Key>::max(), initialValue);
    AssertUnderlyingMapSizeEq(m_map, 3);
}

TEST_F(IntervalMapTest, PreciseFullElimination)
{
    m_map.assign(10, 20, '*');
    m_map.assign(10, 20, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 9 ,initialValue);
    RangeEqualsTo(m_map, 10, 20,'X');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(),initialValue);
    AssertUnderlyingMapSizeEq(m_map, 3);
}

TEST_F(IntervalMapTest, PartialEliminationLeft)
{
    m_map.assign(10, 20, '*');
    m_map.assign(15, 25, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 15, '*');
    RangeEqualsTo(m_map, 15, 25,'X');
    RangeEqualsTo(m_map, 25, std::numeric_limits<Key>::max(),initialValue);   
    AssertUnderlyingMapSizeEq(m_map, 4);
}

TEST_F(IntervalMapTest, PartialEliminationRight)
{
    m_map.assign(10, 20, '*');
    m_map.assign(5, 15, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 5 ,initialValue);
    RangeEqualsTo(m_map, 15, 20, '*');
    RangeEqualsTo(m_map, 5, 15,'X');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(),initialValue);
    AssertUnderlyingMapSizeEq(m_map, 4);
}

TEST_F(IntervalMapTest, PartialEliminationOfCloseNeighbours)
{
    m_map.assign(10, 20, '*');
    m_map.assign(20, 30, '#');
    m_map.assign(15, 25, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 15, '*');
    RangeEqualsTo(m_map, 15, 25, 'X');
    RangeEqualsTo(m_map, 25, 30, '#');
    RangeEqualsTo(m_map, 30, std::numeric_limits<Key>::max(),initialValue);

    AssertUnderlyingMapSizeEq(m_map, 5);
}

TEST_F(IntervalMapTest, PartialEliminationOfNeighbours)
{
    m_map.assign(10, 18, '*');
    m_map.assign(22, 30, '#');
    m_map.assign(15, 25, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 15, '*');
    RangeEqualsTo(m_map, 15, 25, 'X');
    RangeEqualsTo(m_map, 25, 30, '#');
    RangeEqualsTo(m_map, 30, std::numeric_limits<Key>::max(),initialValue);

    AssertUnderlyingMapSizeEq(m_map, 5);
}

TEST_F(IntervalMapTest, PreciseRampage)
{
    m_map.assign(5, 10, 'A');
    m_map.assign(10, 15, 'B');
    m_map.assign(17, 23, 'C');
    m_map.assign(25, 30, 'D');
    m_map.assign(30, 35, 'E');
    m_map.assign(5, 35, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 5 ,initialValue);
    RangeEqualsTo(m_map, 5, 35, 'X');
    RangeEqualsTo(m_map, 35, std::numeric_limits<Key>::max(),initialValue);
    AssertUnderlyingMapSizeEq(m_map, 3);
}

TEST_F(IntervalMapTest, Rampage)
{
    m_map.assign(5, 10, 'A');
    m_map.assign(10, 15, 'B');
    m_map.assign(17, 23, 'C');
    m_map.assign(25, 30, 'D');
    m_map.assign(30, 35, 'E');
    m_map.assign(4, 36, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 4 ,initialValue);
    RangeEqualsTo(m_map, 4, 36, 'X');
    RangeEqualsTo(m_map, 36, std::numeric_limits<Key>::max(),initialValue);
    AssertUnderlyingMapSizeEq(m_map, 3);
}

TEST_F(IntervalMapTest, Absorption)
{
    m_map.assign(10, 20, initialValue);
    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), std::numeric_limits<Key>::max(), initialValue);
    ASSERT_EQ(m_map.getMap().size(), 1);
}

/* Just random tests */
TEST_F(IntervalMapTest, DISABLED_Random)
{
    m_map.assign(0,1,'1');
    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), std::numeric_limits<Key>::max() ,initialValue);
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