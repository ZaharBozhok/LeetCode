#include <gtest/gtest.h>
#include "IntervalMapTestUtils.h"

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