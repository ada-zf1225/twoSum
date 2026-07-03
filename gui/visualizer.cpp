 #include "visualizer.h"

#include <QPainter>
#include <QPainterPath>
#include <algorithm>

ArrayView::ArrayView(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(240);
}

void ArrayView::setData(const std::vector<int>& nums, int target,
                        const std::vector<algo::Step>& steps) {
    nums_ = nums;
    target_ = target;
    steps_ = steps;
    current_ = -1;
    foundPairs_.clear();
    update();
}

void ArrayView::showStep(int stepIndex) {
    // 重新累积到 stepIndex 为止的所有配对
    foundPairs_.clear();
    int clamped = std::max(-1, std::min(stepIndex, stepCount() - 1));
    for (int s = 0; s <= clamped; ++s) {
        const auto& st = steps_[s];
        for (int j : st.matched) {
            foundPairs_.emplace_back(j, st.i);
        }
    }
    current_ = clamped;
    update();
}

void ArrayView::showAll() {
    showStep(stepCount() - 1);
}

void ArrayView::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.fillRect(rect(), QColor(0x1e, 0x1e, 0x2e));

    if (nums_.empty()) {
        p.setPen(QColor(0x9a, 0x9a, 0xb2));
        p.drawText(rect(), Qt::AlignCenter,
                   QStringLiteral("点击「载入」开始，方块=元素，橙色=当前扫描位"));
        return;
    }

    const int n = static_cast<int>(nums_.size());
    const int margin = 24;
    const int gap = 12;
    const int boxTop = 120;
    const int boxH = 56;
    int avail = width() - 2 * margin - (n - 1) * gap;
    int boxW = std::max(28, std::min(72, avail / std::max(1, n)));
    int totalW = n * boxW + (n - 1) * gap;
    int startX = (width() - totalW) / 2;

    auto boxRect = [&](int idx) {
        int x = startX + idx * (boxW + gap);
        return QRect(x, boxTop, boxW, boxH);
    };

    const algo::Step* cur = (current_ >= 0 && current_ < stepCount())
                                ? &steps_[current_]
                                : nullptr;

    // 颜色定义
    const QColor cProcessed(0x45, 0x47, 0x7a);  // 已入哈希表
    const QColor cCurrent(0xf5, 0x9e, 0x0b);    // 当前扫描
    const QColor cMatched(0x22, 0xc5, 0x5e);    // 本步匹配到的搭档
    const QColor cIdle(0x31, 0x33, 0x50);       // 尚未处理
    const QColor cText(0xf5, 0xf5, 0xfa);

    std::vector<bool> isMatched(n, false);
    if (cur) {
        for (int j : cur->matched) {
            if (j >= 0 && j < n) isMatched[j] = true;
        }
    }

    // 先画已确认的配对连线(在方块下方用弧线连接)
    for (const auto& pr : foundPairs_) {
        QRect a = boxRect(pr.first);
        QRect b = boxRect(pr.second);
        QPointF pa(a.center().x(), a.bottom());
        QPointF pb(b.center().x(), b.bottom());
        double midX = (pa.x() + pb.x()) / 2.0;
        double depth = boxTop + boxH + 30 + std::abs(pb.x() - pa.x()) * 0.12;
        QPainterPath path;
        path.moveTo(pa);
        path.cubicTo(QPointF(pa.x(), depth), QPointF(pb.x(), depth), pb);
        p.setPen(QPen(QColor(0x22, 0xc5, 0x5e, 200), 2));
        p.drawPath(path);
    }

    // 画方块
    QFont valFont = p.font();
    valFont.setPointSize(13);
    valFont.setBold(true);
    QFont idxFont = p.font();
    idxFont.setPointSize(9);

    for (int i = 0; i < n; ++i) {
        QRect r = boxRect(i);
        QColor fill = cIdle;
        if (cur) {
            if (i == cur->i)
                fill = cCurrent;
            else if (isMatched[i])
                fill = cMatched;
            else if (i < cur->i)
                fill = cProcessed;
        }
        p.setPen(Qt::NoPen);
        p.setBrush(fill);
        p.drawRoundedRect(r, 8, 8);

        p.setPen(cText);
        p.setFont(valFont);
        p.drawText(r, Qt::AlignCenter, QString::number(nums_[i]));

        p.setFont(idxFont);
        p.setPen(QColor(0xb0, 0xb0, 0xc8));
        p.drawText(QRect(r.left(), r.bottom() + 2, r.width(), 16),
                   Qt::AlignCenter, QString::number(i));
    }

    // 顶部状态文字
    p.setFont(idxFont);
    p.setPen(cText);
    QString top;
    if (cur) {
        top = QStringLiteral("第 %1/%2 步  |  i=%3, nums[i]=%4, 找搭档 target-nums[i]=%5")
                  .arg(current_ + 1)
                  .arg(stepCount())
                  .arg(cur->i)
                  .arg(nums_[cur->i])
                  .arg(cur->need);
        if (!cur->matched.empty()) {
            QStringList js;
            for (int j : cur->matched) js << QString::number(j);
            top += QStringLiteral("  →  找到！下标 %1").arg(js.join(", "));
        } else {
            top += QStringLiteral("  →  没找到，存入哈希表");
        }
    } else {
        top = QStringLiteral("target = %1   （尚未开始，点「单步」或「播放」）").arg(target_);
    }
    p.drawText(QRect(0, 12, width(), 40), Qt::AlignCenter | Qt::TextWordWrap, top);
}
