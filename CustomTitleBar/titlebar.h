#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

#include "common.h"

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

private:
    Ui::TitleBar *ui;

private:
    QLabel *m_pIconLabel;
    QLabel *m_pTitleLabel;
    QPushButton *m_pMinimizeButton;
    QPushButton *m_pMaximizeButton;
    QPushButton *m_pCloseButton;

    // 界面初始化
    void initUI();

    // 最大化/还原
    void updateMaximize();

protected:
    // 监听窗体缩放事件
    void resizeEvent(QResizeEvent *event);

    // 进行界面的拖动
    void mousePressEvent(QMouseEvent *event);

    // 窗体最大化与还原
    void mouseDoubleClickEvent(QMouseEvent *event);

    // 事件过滤器，监听主窗体标题与图标，主窗体修改，则根据其做出调整
    bool eventFilter(QObject *obj, QEvent *event);

public:

private slots:
    // 进行最小化、最大化/还原、关闭操作
    void onClicked();
};

#endif // TITLEBAR_H
