#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ArrayView;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QSlider;
class QPlainTextEdit;
class QLabel;
class QTimer;

// 主窗口:上方输入表单 + 中间可视化画布 + 下方结果/日志。
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onLoad();       // 读取输入,准备动画
    void onSolveAll();   // 直接显示全部配对
    void onStepNext();   // 单步前进
    void onStepPrev();   // 单步后退
    void onPlayPause();  // 播放 / 暂停
    void onReset();      // 回到起点
    void onTick();       // 定时器:自动播放下一步

private:
    void refreshLog();
    void setPlaying(bool playing);
    void updateButtons();

    // 输入区
    QLineEdit* numbersEdit_ = nullptr;
    QSpinBox* targetSpin_ = nullptr;
    QPushButton* loadBtn_ = nullptr;
    QPushButton* solveAllBtn_ = nullptr;

    // 动画控制
    QPushButton* prevBtn_ = nullptr;
    QPushButton* nextBtn_ = nullptr;
    QPushButton* playBtn_ = nullptr;
    QPushButton* resetBtn_ = nullptr;
    QSlider* speedSlider_ = nullptr;

    // 展示区
    ArrayView* view_ = nullptr;
    QPlainTextEdit* log_ = nullptr;
    QLabel* statusLabel_ = nullptr;

    QTimer* timer_ = nullptr;
    bool playing_ = false;
    bool loaded_ = false;
};

#endif  // MAINWINDOW_H
