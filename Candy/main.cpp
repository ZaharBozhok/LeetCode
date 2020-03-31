#include <iomanip>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

enum Direction
{
    Rising,
    Plain,
    Falling
};

enum Extremum
{
    Min,
    Max,
    None
};

struct Data
{
    Data(Extremum ext, int value, int index)
        : left(nullptr), right(nullptr), set(false), ext(ext), value(value), index(index)
    {
    }
    Data *left;
    Data *right;
    bool set;
    Extremum ext;
    int value;
    int index;
};

Direction GetDirection(int first, int second)
{
    if (first == second)
        return Direction::Plain;
    if (first < second)
        return Direction::Rising;
    return Direction::Falling;
}

void show(const std::vector<int> &vec, std::vector<Data> extremums, const std::vector<int> &eliminates)
{
    auto maxRate = *std::max_element(vec.cbegin(), vec.cend());
    std::vector<std::vector<char>> board(maxRate + 1, std::vector<char>(vec.size(), '-'));
    for (int coli = 0; coli < vec.size(); coli++)
    {
        for (int rowi = 0; rowi < vec.at(coli) + 1; rowi++)
        {
            board[rowi][coli] = 'O';
        }
    }
    for (auto it = board.crbegin(); it != board.crend(); it++)
    {
        std::cout << maxRate--;
        for (auto jt = it->cbegin(); jt != it->cend(); jt++)
        {
            std::cout << "--" << *jt;
        }
        std::cout << std::endl;
    }
    std::cout << ' ';
    for (int i = 0; i < vec.size(); i++)
    {
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl
              << ' ';
    for (int i = 0, exi = 0; i < vec.size() && exi < extremums.size(); i++)
    {
        char symbol = ' ';
        if (i == extremums[exi].index)
        {
            if (extremums[exi].ext == Extremum::Min)
                symbol = 'v';
            else
                symbol = '^';
            exi++;
        }
        std::cout << std::setw(3) << symbol;
    }
    std::cout << std::endl
              << ' ';
    for (int i = 0, xi = 0; i < vec.size() && xi < eliminates.size(); i++)
    {
        char symbol = ' ';
        if (i == eliminates.at(xi))
        {
            symbol = 'X';
            xi++;
        }
        std::cout << std::setw(3) << symbol;
    }
    std::cout << std::endl;
}

Extremum getChange(const Direction &last, const Direction &curr)
{
    if (((last == Direction::Falling || last == Direction::Plain) && curr == Direction::Rising) ||
        (last == Direction::Falling && curr == Direction::Plain))
    {
        return Extremum::Min;
    }
    if (((last == Direction::Rising || last == Direction::Plain) && curr == Direction::Falling) ||
        (last == Direction::Rising && curr == Direction::Plain))
    {
        return Extremum::Max;
    }
    return Extremum::None;
}

int GetNthProgressionSum(int n)
{
    if (0 == n)
        return 0;
    if (1 == n)
        return 1;
    return (n + 1) * (n / 2.0);
}

void GetExtremums(std::vector<Data> &extremums, std::vector<int> &eliminates, const std::vector<int> &elements)
{
    if (elements.empty())
        return;
    int candies = 0;
    if (elements.size() == 1)
    {
        eliminates.push_back(0);
        candies = 1;
        return;
    }
    int lastMin = -1;
    int lastMax = -1;
    int mins = 0;
    Direction lastDirection = GetDirection(elements.at(0), elements.at(1));
    switch(lastDirection)
    {
        case Direction::Falling:
        {
            extremums.push_back(Data(Extremum::Max, elements[0], 0));
            lastMax = 0;
        }
        break;
        case Direction::Rising:
        {
            extremums.push_back(Data(Extremum::Min, elements[0], 0));
            lastMin = 0;
            mins++;
            candies++;
        }
        break;
        case Direction::Plain:
        {
            eliminates.push_back(0);
            candies++;
        }
        break;
    }
    
    for (int i = 1; i < elements.size() - 1; i++)
    {
        auto direction = GetDirection(elements.at(i), elements.at(i + 1));
        switch (getChange(lastDirection, direction))
        {
        case Extremum::Min:
        {
            extremums.push_back(Data(Extremum::Min, elements[i], i));
            mins++;
            auto &newMin = i;
            if (lastMax != -1)
            {
                if (lastMin != -1)
                {
                    auto leftSlopeLen = lastMax - lastMin;
                    auto rightSlopeLen = newMin - lastMax;
                    if (rightSlopeLen > leftSlopeLen)
                    {
                        leftSlopeLen--;
                    }
                    else
                    {
                        rightSlopeLen--;
                    }
                    auto candiesR = GetNthProgressionSum(rightSlopeLen + 1) - 1;
                    auto candiesL = GetNthProgressionSum(leftSlopeLen + 1) - 1;
                    candies += candiesR + candiesL;
                }
                else
                {
                    auto slope = newMin - lastMax;
                    candies += GetNthProgressionSum(slope + 1) - 1;
                }
            }
            lastMin = newMin;
            lastMax = -1;
            candies++;
        }
        break;
        case Extremum::Max:
        {
            extremums.push_back(Data(Extremum::Max, elements[i], i));
            lastMax = i;
        }
        break;
        case Extremum::None:
            if (direction == Direction::Plain)
            {
                if (lastMax != -1 && lastMin != -1)
                {
                    auto slope = lastMax - lastMin;
                    candies += GetNthProgressionSum(slope + 1) - 1;
                }
                lastMax = -1;
                lastMin = -1;
                candies++;
                eliminates.push_back(i);
            }
            break;
        }
        lastDirection = direction;
    }
    auto i = elements.size() - 1;
    switch (lastDirection)
    {
        case Direction::Falling:
        {
            extremums.push_back(Data(Extremum::Min, elements[i], i));
            auto &newMin = i;
            auto slope = newMin - lastMax;
            candies += GetNthProgressionSum(slope);
        }
        break;
        case Direction::Rising:
        {
            extremums.push_back(Data(Extremum::Max, elements[i], i));
            auto &newMax = i;
            auto slope = newMax - lastMin;
            candies += GetNthProgressionSum(slope + 1) - 1;
        }
        break;
        case Direction::Plain:
            eliminates.push_back(elements.size() - 1);
            if (lastMax != -1)
            {
                auto slope = lastMax - lastMin;
                candies += GetNthProgressionSum(slope + 1) - 1;
            }
            candies++;
        break;
    }
}

int main()
{
    std::vector<int> ratings = {1,0,2,2};
    //std::vector<int> ratings = {5};
    std::vector<int> eliminates;
    std::vector<Data> extremums;
    GetExtremums(extremums, eliminates, ratings);
    show(ratings, extremums, eliminates);
    std::cout << std::endl;
    return 0;
}