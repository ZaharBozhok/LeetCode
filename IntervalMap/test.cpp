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
    /* `i` just a counter */
    for(size_t i = 0; mapIt != map.cend(); mapIt++, arrIt++)
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


TEST(MapEqSequence, FalseOnDifferentSequencesSizes)
{
    std::map<int, char> m = {
        {0, 'a'},
        {1, 'b'},
        {2, 'c'}
    };
    ASSERT_FALSE(MapEqSequence(m, std::string("")));
}

TEST(MapEqSequence, FalseOnDifferentSequences)
{
    std::map<int, char> m = {
        {0, 'a'},
        {1, 'b'},
        {2, 'c'}
    };
    ASSERT_FALSE(MapEqSequence(m, std::string("qwe")));
}

TEST(MapEqSequence, TrueOnEqSequences)
{
    std::map<int, char> m = {
        {0, 'a'},
        {1, 'b'},
        {2, 'c'}
    };
    ASSERT_TRUE(MapEqSequence(m, std::string("abc")));
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


TEST(HasDoubles, TrueOnDoublesFound)
{
    std::map<int, char> m;
    m[1] = 'a';
    m[2] = 'b';
    m[3] = 'c';
    m[4] = 'b';
    m[5] = 'd';
    m[6] = 'd';
    ASSERT_TRUE(HasDoubles(m));
}

TEST(HasDoubles, FalseOnNoDoubles)
{
    std::map<int, char> m;
    m[1] = 'a';
    m[2] = 'b';
    m[3] = 'c';
    m[4] = 'b';
    m[5] = 'd';
    ASSERT_FALSE(HasDoubles(m));
}

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
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValue));
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
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
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

    ASSERT_TRUE(MapEqSequence(m_map.getMap(), "*" initialValueM));
}

/* Unfortunately useless */
TEST_F(IntervalMapTest, DISABLED_SqueezeThroughRight)
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
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
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
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*X*" initialValueM));
}

TEST_F(IntervalMapTest, ReversedParametersDoNothing)
{
    m_map.assign(20, 10, '*');

    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  std::numeric_limits<Key>::max(),
                  initialValue);
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
    RangeEqualsTo(m_map,
                  std::numeric_limits<Key>::min(),
                  std::numeric_limits<Key>::max(),
                  initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValue));
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
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "L" initialValueM "R" initialValueM));
}

TEST_F(IntervalMapTest, CloseNeighbours)
{
    m_map.assign(10, 15, 'L');
    m_map.assign(15, 20, 'R');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 15, 'L');
    RangeEqualsTo(m_map, 15, 20, 'R');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(),initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "LR" initialValueM));
}

TEST_F(IntervalMapTest, FullElimination)
{
    m_map.assign(10, 20, '*');
    m_map.assign(9, 21, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 9, initialValue);
    RangeEqualsTo(m_map, 9, 21,'X');
    RangeEqualsTo(m_map, 21, std::numeric_limits<Key>::max(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X" initialValueM));
}

TEST_F(IntervalMapTest, PreciseFullElimination)
{
    m_map.assign(10, 20, '*');
    m_map.assign(10, 20, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 9 ,initialValue);
    RangeEqualsTo(m_map, 10, 20,'X');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(),initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X" initialValueM));
}

TEST_F(IntervalMapTest, PartialEliminationLeft)
{
    m_map.assign(10, 20, '*');
    m_map.assign(15, 25, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10 ,initialValue);
    RangeEqualsTo(m_map, 10, 15, '*');
    RangeEqualsTo(m_map, 15, 25,'X');
    RangeEqualsTo(m_map, 25, std::numeric_limits<Key>::max(),initialValue);   
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*X" initialValueM));
}

TEST_F(IntervalMapTest, PartialEliminationRight)
{
    m_map.assign(10, 20, '*');
    m_map.assign(5, 15, 'X');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 5 ,initialValue);
    RangeEqualsTo(m_map, 15, 20, '*');
    RangeEqualsTo(m_map, 5, 15,'X');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(),initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X*" initialValueM));
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

    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*X#" initialValueM));
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

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 5 ,initialValue);
    RangeEqualsTo(m_map, 5, 35, 'X');
    RangeEqualsTo(m_map, 35, std::numeric_limits<Key>::max(),initialValue);
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

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 4 ,initialValue);
    RangeEqualsTo(m_map, 4, 36, 'X');
    RangeEqualsTo(m_map, 36, std::numeric_limits<Key>::max(),initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "X" initialValueM));
}

TEST_F(IntervalMapTest, Absorption)
{
    m_map.assign(10, 20, initialValue);
    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), std::numeric_limits<Key>::max(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValue));
}

TEST_F(IntervalMapTest, AbsorptionInSmallerRange)
{
    m_map.assign(10, 30, '*');
    m_map.assign(15, 25, '*');
    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 30, '*');
    RangeEqualsTo(m_map, 30, std::numeric_limits<Key>::max(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingLeft)
{
    m_map.assign(15, 20, '*');
    m_map.assign(10, 17, '*');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingRight)
{
    m_map.assign(10, 17, '*');
    m_map.assign(15, 20, '*');

    RangeEqualsTo(m_map, std::numeric_limits<Key>::min(), 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingNeighboursLeft)
{
    m_map.assign(15, 20, '*');
    m_map.assign(10, 15, '*');

    RangeEqualsTo(m_map, 0, 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, MergingNeighboursRight)
{
    m_map.assign(10, 15, '*');
    m_map.assign(15, 20, '*');

    RangeEqualsTo(m_map, 0, 10, initialValue);
    RangeEqualsTo(m_map, 10, 20, '*');
    RangeEqualsTo(m_map, 20, std::numeric_limits<Key>::max(), initialValue);
    ASSERT_TRUE(MapEqSequence(m_map.getMap(), initialValueM "*" initialValueM));
}

TEST_F(IntervalMapTest, DISABLED_MergingAndCollapsingNeighboursLeft)
{
}

TEST_F(IntervalMapTest, DISABLED_MergingAndCollapsingNeighboursRight)
{
}

#include <cstdlib>
#include <ctime>

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
    RangeEqualsTo(m_map, 39, std::numeric_limits<Key>::max(), initialValue);

    ASSERT_TRUE(MapEqSequence(m_map.getMap(),  "BA" initialValueM));
}

TEST_F(IntervalMapTest, DISABLED_FailedRandom2)
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

TEST_F(IntervalMapTest, DISABLED_FailedRandom3)
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

TEST_F(IntervalMapTest, DISABLED_FailedRandom4)
{
    m_map.assign(7, 26, 'F');
    m_map.assign(20, 36, 'C');
    m_map.assign(3, 23, 'C');
    m_map.assign(36, 39, 'A');
    m_map.assign(36, 38, 'C');
    m_map.assign(20, 22, 'C');
    m_map.assign(21, 26, 'A');
}

/* Just random tests */
TEST_F(IntervalMapTest, DISABLED_Random)
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
    for(size_t i=0; i<std::rand()%max; i++)
    {
        auto begin = std::rand() % max;
        auto end = std::rand() % max;
        if (begin > end)
            std::swap(begin, end);
        auto newVal = 'A' + std::rand() % valMax;
        ranges.push_back(Range(begin, end, newVal));
        std::cout << std::endl << i+1 << ". begin : " << begin << ", end : " << end << ", newVal : " << (char)newVal << std::endl;
        m_map.assign(begin, end, newVal);
        ShowMapFromTill(m_map, 0, 41);
    }
    auto doubles = HasDoubles(m_map.getMap());
    if (doubles == true)
    {
        std::cout << "TEST_F(IntervalMapTest, FailedRandom)" << std::endl << "{" << std::endl;
        for(const auto& range : ranges)
        {
            std::cout << "    m_map.assign(" << (int)range.begin << ", " << (int)range.end << ", '" << (char)range.val << "');" << std::endl; 
        }
        std::cout << std::endl << "}" << std::endl;
    }
    ASSERT_FALSE(doubles);
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