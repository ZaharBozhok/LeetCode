#include "IntervalMap.h"
#include "IntervalMapTestUtils.h"
#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include <iomanip>
#include <limits>
#include <sstream>

const char initialValue = '-';
#define initialValueM "-"

class IntervalMapTest : public ::testing::Test
{
public:
    IntervalMapTest() : m_map(initialValue)
    {
    }

protected:
    void TearDown() override
    {
        ASSERT_FALSE(HasDoubles(m_map.getMap()));
        if (::testing::Test::HasFailure())
        {
            ShowMapFromTill(m_map, MIN(), MAX());
            std::cout << "Underlying map : " << std::endl;
            ShowUnderlyingMap(m_map.getMap());
        }
    }
    interval_map<TestNumeric, char> m_map;
    using Key = decltype(m_map)::kType;
    using Value = decltype(m_map)::vType;
    Key MIN() const { return std::numeric_limits<Key>::min(); }
    Key MAX() const { return std::numeric_limits<Key>::max(); }
};

TEST_F(IntervalMapTest, FilledWithInitialValue)
{
    RangeEqualsTo(m_map, MIN(), MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValue));
}

TEST_F(IntervalMapTest, SqueezeThrough)
{
    m_map.assign(10, 20, '*');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, SqueezeThroughLeft)
{
    m_map.assign(MIN(), 10, '*');

    RangeEqualsTo(m_map, MIN(), 10, '*');
    RangeEqualsTo(m_map, 10, MAX(), initialValue);

    ASSERT_TRUE(MapEqSequence(m_map.getMap(), "*" initialValueM));
}

/* Unfortunately useless */
TEST_F(IntervalMapTest, SqueezeThroughRight)
{
    m_map.assign(10, MAX(), '*');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, MAX(), '*');
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, SqueezeThroughThrough)
{
    m_map.assign(10, 20, '*');
    m_map.assign(13, 17, 'X');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 13, '*');
    RangeEqualsTo(m_map, 13, 17, 'X');
    RangeEqualsTo(m_map, 17, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*X*" initialValueM));
}

TEST_F(IntervalMapTest, ReversedParametersDoNothing)
{
    m_map.assign(20, 10, '*');

    RangeEqualsTo(m_map, MIN(), MAX(), initialValue);

    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM));
}

TEST_F(IntervalMapTest, InsertPoint)
{
    Value insertVal = '*';
    m_map.assign(1, 2, insertVal);
    RangeEqualsTo(m_map, 1, 2, insertVal);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, InsertEmptyPoint)
{
    m_map.assign(2, 2, '*');
    RangeEqualsTo(m_map, MIN(), MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValue));
}

TEST_F(IntervalMapTest, Neighbours)
{
    m_map.assign(10, 15, 'L');
    m_map.assign(20, 25, 'R');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 15, 'L');
    RangeEqualsTo(m_map, 15, 19, initialValue);
    RangeEqualsTo(m_map, 20, 25, 'R');
    RangeEqualsTo(m_map, 25, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "L" initialValueM "R" initialValueM));
}

TEST_F(IntervalMapTest, CloseNeighbours)
{
    m_map.assign(10, 15, 'L');
    m_map.assign(15, 20, 'R');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 15, 'L');
    RangeEqualsTo(m_map, 15, 20, 'R');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "LR" initialValueM));
}

TEST_F(IntervalMapTest, FullElimination)
{
    m_map.assign(10, 20, '*');
    m_map.assign(9, 21, 'X');

    RangeEqualsTo(m_map, MIN(), 9, initialValue);
    RangeEqualsTo(m_map, 9, 21, 'X');
    RangeEqualsTo(m_map, 21, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X" initialValueM));
}

TEST_F(IntervalMapTest, PreciseFullElimination)
{
    m_map.assign(10, 20, '*');
    m_map.assign(10, 20, 'X');

    RangeEqualsTo(m_map, MIN(), 9, initialValue);
    RangeEqualsTo(m_map, 10, 20, 'X');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X" initialValueM));
}

TEST_F(IntervalMapTest, PartialEliminationLeft)
{
    m_map.assign(10, 20, '*');
    m_map.assign(15, 25, 'X');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 15, '*');
    RangeEqualsTo(m_map, 15, 25, 'X');
    RangeEqualsTo(m_map, 25, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*X" initialValueM));
}

TEST_F(IntervalMapTest, PartialEliminationRight)
{
    m_map.assign(10, 20, '*');
    m_map.assign(5, 15, 'X');

    RangeEqualsTo(m_map, MIN(), 5, initialValue);
    RangeEqualsTo(m_map, 15, 20, '*');
    RangeEqualsTo(m_map, 5, 15, 'X');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X*" initialValueM));
}

TEST_F(IntervalMapTest, PartialEliminationOfCloseNeighbours)
{
    m_map.assign(10, 20, '*');
    m_map.assign(20, 30, '#');
    m_map.assign(15, 25, 'X');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 15, '*');
    RangeEqualsTo(m_map, 15, 25, 'X');
    RangeEqualsTo(m_map, 25, 30, '#');
    RangeEqualsTo(m_map, 30, MAX(), initialValue);

    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*X#" initialValueM));
}

TEST_F(IntervalMapTest, PartialEliminationOfNeighbours)
{
    m_map.assign(10, 18, '*');
    m_map.assign(22, 30, '#');
    m_map.assign(15, 25, 'X');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 15, '*');
    RangeEqualsTo(m_map, 15, 25, 'X');
    RangeEqualsTo(m_map, 25, 30, '#');
    RangeEqualsTo(m_map, 30, MAX(), initialValue);

    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*X#" initialValueM));
}

TEST_F(IntervalMapTest, PreciseRampage)
{
    m_map.assign(5, 10, 'A');
    m_map.assign(10, 15, 'B');
    m_map.assign(17, 23, 'C');
    m_map.assign(25, 30, 'D');
    m_map.assign(30, 35, 'E');
    m_map.assign(5, 35, 'X');

    RangeEqualsTo(m_map, MIN(), 5, initialValue);
    RangeEqualsTo(m_map, 5, 35, 'X');
    RangeEqualsTo(m_map, 35, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X" initialValueM));
}

TEST_F(IntervalMapTest, Rampage)
{
    m_map.assign(5, 10, 'A');
    m_map.assign(10, 15, 'B');
    m_map.assign(17, 23, 'C');
    m_map.assign(25, 30, 'D');
    m_map.assign(30, 35, 'E');
    m_map.assign(4, 36, 'X');

    RangeEqualsTo(m_map, MIN(), 4, initialValue);
    RangeEqualsTo(m_map, 4, 36, 'X');
    RangeEqualsTo(m_map, 36, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X" initialValueM));
}

TEST_F(IntervalMapTest, Absorption)
{
    m_map.assign(10, 20, initialValue);
    RangeEqualsTo(m_map, MIN(), MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValue));
}

TEST_F(IntervalMapTest, AbsorptionInSmallerRange)
{
    m_map.assign(10, 30, '*');
    m_map.assign(15, 25, '*');
    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 30, '*');
    RangeEqualsTo(m_map, 30, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingLeft)
{
    m_map.assign(15, 20, '*');
    m_map.assign(10, 17, '*');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingRight)
{
    m_map.assign(10, 17, '*');
    m_map.assign(15, 20, '*');

    RangeEqualsTo(m_map, MIN(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingNeighboursLeft)
{
    m_map.assign(15, 20, '*');
    m_map.assign(10, 15, '*');

    RangeEqualsTo(m_map, 0, 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingNeighboursRight)
{
    m_map.assign(10, 15, '*');
    m_map.assign(15, 20, '*');

    RangeEqualsTo(m_map, 0, 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingAndCollapsingNeighboursLeft)
{
    m_map.assign(15, 20, '*');
    m_map.assign(10, 15, 'V');
    m_map.assign(5, 15, '*');

    RangeEqualsTo(m_map, 0, 5, initialValue);
    RangeEqualsTo(m_map, 5, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingAndCollapsingNeighboursRight)
{
    m_map.assign(5, 10, '*');
    m_map.assign(10, 15, 'V');
    m_map.assign(10, 20, '*');

    RangeEqualsTo(m_map, 0, 5, initialValue);
    RangeEqualsTo(m_map, 5, 20, '*');
    RangeEqualsTo(m_map, 20, MAX(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, FailedRandom1)
{
    m_map.assign(0, 25, 'B');
    m_map.assign(4, 8, 'B');
    m_map.assign(5, 8, 'E');
    m_map.assign(25, 31, 'C');
    m_map.assign(14, 27, 'F');
    m_map.assign(6, 39, 'A');
    m_map.assign(15, 18, 'C');
    m_map.assign(7, 23, 'E');
    m_map.assign(16, 23, 'B');
    m_map.assign(5, 34, 'B');

    RangeEqualsTo(m_map, 0, 34, 'B');
    RangeEqualsTo(m_map, 34, 39, 'A');
    RangeEqualsTo(m_map, 39, MAX(), initialValue);

    ASSERT_TRUE(MapEqSequence(m_map.getMap(), "BA" initialValueM));
}

TEST_F(IntervalMapTest, FailedRandom2)
{
    m_map.assign(38, 39, 'B');
    m_map.assign(12, 39, 'F');
    m_map.assign(26, 38, 'C');
    m_map.assign(18, 39, 'C');
    m_map.assign(11, 35, 'F');
    m_map.assign(24, 30, 'A');
    m_map.assign(1, 28, 'D');
    m_map.assign(4, 32, 'C');
    m_map.assign(20, 39, 'D');
    m_map.assign(20, 35, 'C');
    m_map.assign(12, 14, 'C');
    m_map.assign(11, 15, 'F');
}

TEST_F(IntervalMapTest, FailedRandom3)
{
    m_map.assign(8, 25, 'E');
    m_map.assign(17, 24, 'B');
    m_map.assign(5, 10, 'B');
    m_map.assign(15, 20, 'A');
    m_map.assign(23, 23, 'C');
    m_map.assign(16, 26, 'A');
    m_map.assign(16, 29, 'A');
    m_map.assign(24, 37, 'A');
    m_map.assign(15, 22, 'E');
    m_map.assign(7, 8, 'F');
}

TEST_F(IntervalMapTest, FailedRandom4)
{
    m_map.assign(7, 26, 'F');
    m_map.assign(20, 36, 'C');
    m_map.assign(3, 23, 'C');
    m_map.assign(36, 39, 'A');
    m_map.assign(36, 38, 'C');
    m_map.assign(20, 22, 'C');
    m_map.assign(21, 26, 'A');
}

/* Just random tests, shouldn't fail :) */
TEST_F(IntervalMapTest, Random)
{
    std::srand(std::time(0));
    int max = 40;
    int valMax = 'G' - 'A';
    struct Range
    {
        Range(Key begin, Key end, Value val) : begin(begin), end(end), val(val) {}
        Key begin;
        Key end;
        Value val;
    };
    std::vector<Range> ranges;
    std::vector<std::string> outputs;
    for (size_t i = 0; i < std::rand() % max; i++)
    {
        std::stringstream ss;
        auto begin = std::rand() % max;
        auto end = std::rand() % max;
        if (begin > end)
            std::swap(begin, end);
        auto newVal = 'A' + std::rand() % valMax;
        ranges.push_back(Range(begin, end, newVal));
        ss << std::endl
           << i + 1 << ". begin : " << begin << ", end : " << end << ", newVal : " << (char)newVal << std::endl;
        m_map.assign(begin, end, newVal);
        ShowMapFromTill(m_map, 0, 41, ss);
        outputs.push_back(ss.str());
    }
    auto doubles = HasDoubles(m_map.getMap());
    if (doubles == true)
    {
        for (const auto &output : outputs)
        {
            std::cout << output;
        }
        std::cout << "TEST_F(IntervalMapTest, FailedRandom)" << std::endl
                  << "{" << std::endl;
        for (const auto &range : ranges)
        {
            std::cout << "    m_map.assign(" << range.begin << ", " << range.end << ", '" << range.val << "');" << std::endl;
        }
        std::cout << std::endl
                  << "}" << std::endl;
    }
    ASSERT_FALSE(doubles);
}

/* It should have tested full range filling, but it is impossible using this filling [a,b) */
TEST_F(IntervalMapTest, DISABLED_FullBomb)
{
    Value insertVal = '*';
    m_map.assign(MIN(), MAX(), insertVal);
    RangeEqualsTo(m_map,
                  MIN(),
                  MAX(),
                  insertVal);
    std::cout << m_map[MAX()] << std::endl;
}