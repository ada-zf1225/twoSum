#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "two_sum.h"

using algo::two_sum;

TEST(TwoSum, BasicPair) {
    std::vector<int> nums{2, 7, 11, 15};
    auto res = two_sum(nums, 9);
    ASSERT_EQ(res.size(), 1u);
    EXPECT_EQ(res[0], std::make_pair(0, 1));
}

TEST(TwoSum, NoPair) {
    std::vector<int> nums{1, 2, 3};
    EXPECT_TRUE(two_sum(nums, 100).empty());
}

TEST(TwoSum, ReturnedIndicesAreAscending) {
    std::vector<int> nums{3, 2, 4};
    auto res = two_sum(nums, 6);
    ASSERT_EQ(res.size(), 1u);
    EXPECT_LT(res[0].first, res[0].second);
    EXPECT_EQ(res[0], std::make_pair(1, 2));
}

TEST(TwoSum, MultiplePairsWithDuplicates) {
    std::vector<int> nums{3, 3, 4, 3};
    auto res = two_sum(nums, 6);
    // 期望配对: (0,1), (0,3), (1,3)
    EXPECT_EQ(res.size(), 3u);
}

TEST(TwoSum, EmptyInput) {
    std::vector<int> nums;
    EXPECT_TRUE(two_sum(nums, 0).empty());
}

TEST(TwoSum, NegativeNumbers) {
    std::vector<int> nums{-1, -2, -3, -4};
    auto res = two_sum(nums, -5);
    // (-1)+(-4)=-5 -> (0,3);  (-2)+(-3)=-5 -> (1,2)
    EXPECT_EQ(res.size(), 2u);
}

TEST(TwoSumTrace, StepsMatchArrayLengthAndMatches) {
    std::vector<int> nums{3, 2, 4};
    auto steps = algo::two_sum_trace(nums, 6);
    ASSERT_EQ(steps.size(), nums.size());  // 每个元素一步
    EXPECT_EQ(steps[0].i, 0);
    EXPECT_TRUE(steps[0].matched.empty());  // 3: 找 3,还没存过
    EXPECT_TRUE(steps[1].matched.empty());  // 2: 找 4,没有
    ASSERT_EQ(steps[2].matched.size(), 1u); // 4: 找 2,命中下标 1
    EXPECT_EQ(steps[2].matched[0], 1);
}

TEST(TwoSumTrace, TraceAgreesWithTwoSum) {
    std::vector<int> nums{3, 3, 4, 3};
    auto steps = algo::two_sum_trace(nums, 6);
    size_t pairCount = 0;
    for (const auto& s : steps) pairCount += s.matched.size();
    EXPECT_EQ(pairCount, two_sum(nums, 6).size());  // 轨迹产生的配对数应与结果一致
}
