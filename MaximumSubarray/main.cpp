#include <iostream>
#include <vector>
using namespace std;

int maxSubArray(vector<int> &nums)
{
     int acc = nums[0];
        int maxSum = nums[0];
        for(int i=1; i<nums.size(); i++)
        {
            auto& curr = nums[i];
            auto newSum = acc + curr;
            if (newSum > curr)
                acc = newSum;
            else
                acc = curr;
            if (acc > maxSum)
                maxSum = acc;
        }
        return maxSum;
}
int main()
{
    vector<int> vec = {-2,1,-3,4,-1,2,1-5,4};
    std::cout << "res : " << maxSubArray(vec) << std::endl;
    return 0;
}