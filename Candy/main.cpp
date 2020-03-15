#include <vector>
#include <iostream>
#include <iomanip>

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
    for (int i = 0, exi = 0; i < vec.size(); i++)
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

void GetExtremums(std::vector<Data> &extremums, std::vector<int> &eliminates, const std::vector<int> &elements)
{
    if (elements.empty())
        return;
    if (elements.size() == 1)
    {
        extremums.push_back(Data(Extremum::Min, elements[0], 0));
        return;
    }
    Direction lastDirection = GetDirection(elements.at(0), elements.at(1));
    if (lastDirection == Direction::Falling)
    {
        extremums.push_back(Data(Extremum::Max, elements[0], 0));
    }
    else if (lastDirection == Direction::Rising)
    {
        extremums.push_back(Data(Extremum::Min, elements[0], 0));
    }
    for (int i = 1; i < elements.size() - 1; i++)
    {
        auto direction = GetDirection(elements.at(i), elements.at(i + 1));
        switch (getChange(lastDirection, direction))
        {
        case Extremum::Min:
            extremums.push_back(Data(Extremum::Min, elements[i], i));
            break;
        case Extremum::Max:
            extremums.push_back(Data(Extremum::Max, elements[i], i));
            break;
        case Extremum::None:
            if (direction == Direction::Plain)
            {
                eliminates.push_back(i);
            }
            break;
        }

        lastDirection = direction;
    }
    if (lastDirection == Direction::Falling)
    {
        extremums.push_back(Data(Extremum::Min, elements[elements.size() - 1], elements.size() - 1));
    }
    else if (lastDirection == Direction::Rising)
    {
        extremums.push_back(Data(Extremum::Max, elements[elements.size() - 1], elements.size() - 1));
    }
}

int GetNthProgressionSum(int n)
{
    return (n + 1) * (n / 2.0);
}

int main()
{
    std::vector<int> ratings = {5, 4, 6, 2, 2, 6, 7, 5, 6, 5, 6, 7, 7, 7, 6, 5, 4, 4, 4, 3, 6, 4, 2, 1, 4, 5, 6, 7, 0, 6};
    std::vector<int> eliminates;
    std::vector<Data> extremums;
    GetExtremums(extremums, eliminates, ratings);
    show(ratings, extremums, eliminates);
    std::cout << std::endl;
    return 0;
}