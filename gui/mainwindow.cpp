#include "mainwindow.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSlider>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "two_sum.h"
#include "visualizer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("两数之和 · 可视化"));

    auto* central = new QWidget(this);
    auto* root = new QVBoxLayout(central);

    // ---------- 输入区 ----------
    auto* inputBox = new QGroupBox(QStringLiteral("输入"), central);
    auto* inputLay = new QHBoxLayout(inputBox);
    inputLay->addWidget(new QLabel(QStringLiteral("数组(空格分隔):")));
    numbersEdit_ = new QLineEdit(QStringLiteral("2 7 11 15 3 6"), inputBox);
    inputLay->addWidget(numbersEdit_, 1);
    inputLay->addWidget(new QLabel(QStringLiteral("target:")));
    targetSpin_ = new QSpinBox(inputBox);
    targetSpin_->setRange(-1000000, 1000000);
    targetSpin_->setValue(9);
    inputLay->addWidget(targetSpin_);
    loadBtn_ = new QPushButton(QStringLiteral("载入"), inputBox);
    solveAllBtn_ = new QPushButton(QStringLiteral("直接求解全部"), inputBox);
    inputLay->addWidget(loadBtn_);
    inputLay->addWidget(solveAllBtn_);
    root->addWidget(inputBox);

    // ---------- 可视化画布 ----------
    view_ = new ArrayView(central);
    root->addWidget(view_, 1);

    // ---------- 动画控制 ----------
    auto* ctrlBox = new QGroupBox(QStringLiteral("动画控制"), central);
    auto* ctrlLay = new QHBoxLayout(ctrlBox);
    resetBtn_ = new QPushButton(QStringLiteral("⏮ 重置"), ctrlBox);
    prevBtn_ = new QPushButton(QStringLiteral("◀ 上一步"), ctrlBox);
    playBtn_ = new QPushButton(QStringLiteral("▶ 播放"), ctrlBox);
    nextBtn_ = new QPushButton(QStringLiteral("下一步 ▶"), ctrlBox);
    ctrlLay->addWidget(resetBtn_);
    ctrlLay->addWidget(prevBtn_);
    ctrlLay->addWidget(playBtn_);
    ctrlLay->addWidget(nextBtn_);
    ctrlLay->addSpacing(16);
    ctrlLay->addWidget(new QLabel(QStringLiteral("慢")));
    speedSlider_ = new QSlider(Qt::Horizontal, ctrlBox);
    speedSlider_->setRange(200, 2000);  // 毫秒/步
    speedSlider_->setValue(800);
    speedSlider_->setInvertedAppearance(true);  // 右=快
    ctrlLay->addWidget(speedSlider_, 1);
    ctrlLay->addWidget(new QLabel(QStringLiteral("快")));
    root->addWidget(ctrlBox);

    // ---------- 结果 / 日志 ----------
    statusLabel_ = new QLabel(QStringLiteral("就绪。"), central);
    root->addWidget(statusLabel_);
    log_ = new QPlainTextEdit(central);
    log_->setReadOnly(true);
    log_->setMaximumHeight(140);
    root->addWidget(log_);

    setCentralWidget(central);
    resize(860, 620);

    timer_ = new QTimer(this);

    connect(loadBtn_, &QPushButton::clicked, this, &MainWindow::onLoad);
    connect(solveAllBtn_, &QPushButton::clicked, this, &MainWindow::onSolveAll);
    connect(nextBtn_, &QPushButton::clicked, this, &MainWindow::onStepNext);
    connect(prevBtn_, &QPushButton::clicked, this, &MainWindow::onStepPrev);
    connect(playBtn_, &QPushButton::clicked, this, &MainWindow::onPlayPause);
    connect(resetBtn_, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(timer_, &QTimer::timeout, this, &MainWindow::onTick);

    updateButtons();
}

// 从输入框解析出数组;非法 token 跳过。
static std::vector<int> parseNumbers(const QString& text) {
    std::vector<int> out;
    const QStringList parts =
        text.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
    for (const QString& t : parts) {
        bool ok = false;
        int v = t.toInt(&ok);
        if (ok) out.push_back(v);
    }
    return out;
}

void MainWindow::onLoad() {
    setPlaying(false);
    std::vector<int> nums = parseNumbers(numbersEdit_->text());
    int target = targetSpin_->value();
    if (nums.empty()) {
        statusLabel_->setText(QStringLiteral("⚠ 没解析到任何数字。"));
        loaded_ = false;
        updateButtons();
        return;
    }
    auto steps = algo::two_sum_trace(nums, target);
    view_->setData(nums, target, steps);
    view_->showStep(-1);
    loaded_ = true;
    log_->clear();
    statusLabel_->setText(
        QStringLiteral("已载入 %1 个数字,共 %2 步。点「下一步」或「播放」。")
            .arg(nums.size())
            .arg(steps.size()));
    updateButtons();
}

void MainWindow::onSolveAll() {
    onLoad();
    if (!loaded_) return;
    setPlaying(false);
    view_->showAll();
    refreshLog();
    statusLabel_->setText(QStringLiteral("已直接展示全部结果。"));
    updateButtons();
}

void MainWindow::onStepNext() {
    if (!loaded_) return;
    if (view_->currentStep() + 1 >= view_->stepCount()) {
        setPlaying(false);
        return;
    }
    view_->showStep(view_->currentStep() + 1);
    refreshLog();
    updateButtons();
}

void MainWindow::onStepPrev() {
    if (!loaded_) return;
    view_->showStep(view_->currentStep() - 1);
    refreshLog();
    updateButtons();
}

void MainWindow::onReset() {
    if (!loaded_) return;
    setPlaying(false);
    view_->showStep(-1);
    refreshLog();
    updateButtons();
}

void MainWindow::onPlayPause() {
    if (!loaded_) return;
    if (playing_) {
        setPlaying(false);
    } else {
        if (view_->currentStep() + 1 >= view_->stepCount())
            view_->showStep(-1);  // 已到末尾则从头播
        setPlaying(true);
    }
}

void MainWindow::onTick() {
    if (view_->currentStep() + 1 >= view_->stepCount()) {
        setPlaying(false);
        return;
    }
    onStepNext();
}

void MainWindow::setPlaying(bool playing) {
    playing_ = playing;
    if (playing) {
        timer_->start(speedSlider_->value());
        playBtn_->setText(QStringLiteral("⏸ 暂停"));
    } else {
        timer_->stop();
        playBtn_->setText(QStringLiteral("▶ 播放"));
    }
}

void MainWindow::refreshLog() {
    // 累积到当前步为止的所有配对
    QStringList lines;
    // 直接向画布索要:通过重放 steps 复原(画布内部已算好,这里简单重算展示)
    // 为保持简单,重新问算法要 trace 并累积到当前步。
    // （画布与日志各自计算,数据源一致,不会不一致。）
    lines << QStringLiteral("=== 已找到的配对 (下标对) ===");
    // 借助画布当前步来展示
    int cur = view_->currentStep();
    // 重新计算配对:与画布逻辑一致
    // 注意:这里无法直接拿到画布内部 vector,故重新跑一遍 trace。
    // 为此需要输入,重新解析。
    std::vector<int> nums = parseNumbers(numbersEdit_->text());
    int target = targetSpin_->value();
    auto steps = algo::two_sum_trace(nums, target);
    int count = 0;
    for (int s = 0; s <= cur && s < static_cast<int>(steps.size()); ++s) {
        for (int j : steps[s].matched) {
            lines << QStringLiteral("  [%1, %2]   (%3 + %4 = %5)")
                         .arg(j)
                         .arg(steps[s].i)
                         .arg(nums[j])
                         .arg(nums[steps[s].i])
                         .arg(target);
            ++count;
        }
    }
    if (count == 0)
        lines << QStringLiteral("  (暂无)");
    else
        lines << QStringLiteral("小计:%1 组").arg(count);
    log_->setPlainText(lines.join('\n'));
}

void MainWindow::updateButtons() {
    bool has = loaded_;
    int cur = view_ ? view_->currentStep() : -1;
    int total = view_ ? view_->stepCount() : 0;
    prevBtn_->setEnabled(has && cur >= 0);
    nextBtn_->setEnabled(has && cur + 1 < total);
    playBtn_->setEnabled(has && total > 0);
    resetBtn_->setEnabled(has && cur >= 0);
}
