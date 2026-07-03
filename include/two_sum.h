#ifndef TWO_SUM_H
#define TWO_SUM_H

#include <utility>
#include <vector>

namespace algo {

// 返回所有满足条件的下标对 (i, j),其中 i < j 且 nums[i] + nums[j] == target。
// 纯算法,不做任何输入输出。平均时间复杂度 O(n + k),k 为配对数量。
std::vector<std::pair<int, int>> two_sum(const std::vector<int>& nums, int target);

// 算法执行过程中的一步,供可视化回放使用。
struct Step {
    int i;                     // 当前扫描到的下标
    int need;                  // 正在寻找的搭档值 = target - nums[i]
    std::vector<int> matched;  // 本步在哈希表里找到的搭档下标(都 < i)
};

// 与 two_sum 相同的逻辑,但把每一步扫描记录下来。
// 下标 0..step.i-1 都已在哈希表中;step.matched 是本步新产生的配对的另一半。
std::vector<Step> two_sum_trace(const std::vector<int>& nums, int target);

}  // namespace algo

#endif  // TWO_SUM_H
