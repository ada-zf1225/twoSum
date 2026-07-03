#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QWidget>
#include <utility>
#include <vector>

#include "two_sum.h"

// 负责绘制数组方块、当前扫描位置、哈希表着色以及配对连线的画布。
class ArrayView : public QWidget {
    Q_OBJECT
public:
    explicit ArrayView(QWidget* parent = nullptr);

    // 载入一组新数据并重置动画状态。
    void setData(const std::vector<int>& nums, int target,
                 const std::vector<algo::Step>& steps);

    // 把动画推进到第 stepIndex 步(-1 表示尚未开始)。
    void showStep(int stepIndex);

    // 直接展示全部配对结果(不做动画)。
    void showAll();

    int stepCount() const { return static_cast<int>(steps_.size()); }
    int currentStep() const { return current_; }

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize minimumSizeHint() const override { return QSize(400, 220); }

private:
    std::vector<int> nums_;
    int target_ = 0;
    std::vector<algo::Step> steps_;
    int current_ = -1;                             // 当前步(-1 未开始)
    std::vector<std::pair<int, int>> foundPairs_;  // 已经找到的配对(累积)
};

#endif  // VISUALIZER_H
