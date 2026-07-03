#include "two_sum.h"

#include <unordered_map>

namespace algo {

std::vector<std::pair<int, int>> two_sum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, std::vector<int>> seen;  // 值 -> 迄今出现过的所有下标
    std::vector<std::pair<int, int>> result;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) {
            // it->second 里的下标都比 i 小,逐个配对
            for (int j : it->second) {
                result.emplace_back(j, i);
            }
        }
        seen[nums[i]].push_back(i);
    }

    return result;
}

std::vector<Step> two_sum_trace(const std::vector<int>& nums, int target) {
    std::unordered_map<int, std::vector<int>> seen;
    std::vector<Step> steps;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        Step step;
        step.i = i;
        step.need = target - nums[i];

        auto it = seen.find(step.need);
        if (it != seen.end()) {
            step.matched = it->second;  // 本步找到的所有搭档下标
        }
        steps.push_back(step);

        seen[nums[i]].push_back(i);
    }

    return steps;
}

}  // namespace algo
