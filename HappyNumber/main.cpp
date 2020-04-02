#include <iostream>
#include <unordered_map>
#include <iostream>
using namespace std;

class Solution {
public:
    int getSumOfSquares(int n)
    {
        int sumOfSquares = 0;
        while(n)
        {
            sumOfSquares+=(n%10)*(n%10);
            n/=10;
        }
        return sumOfSquares;
    }
    bool isHappy(int n) {
        std::unordered_map<int, int> alreadyChecked;
        while(true)
        {
            if (n == 1)
            {
                return true;
            }
            auto it = alreadyChecked.find(n);
            if (it == alreadyChecked.cend())
            {
                alreadyChecked.insert(std::pair(n,n));
            }
            else
            {
                return false;
            }
            n = getSumOfSquares(n);
        }
        return false;
    }
};

int main()
{
    Solution solution;
    //solution.isHappy(1);
    std::cout << (solution.isHappy(7) == true ? "Happy number" : "Not happy number" ) << std::endl;
    std::cout << (solution.isHappy(6) == true ? "Happy number" : "Not happy number" ) << std::endl;
    //solution.isHappy(19);
    //solution.i{sHappy(49);
    //solution.isHappy(97);
    //solution.isHappy(130);
    //solution.isHappy(10);
    return 0;
}