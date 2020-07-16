#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "common.h"
#include "titlebar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

private:
    // 标题栏
    TitleBar *m_pTitleBar;

    // 模拟窗体边框的宽度
    int m_nBorderWidth;

    // 背景动图播放器
    QMovie *m_pMovie;

    // 放动图播放器的标签
    QLabel *m_pMovieLable;

    // 界面初始化
    void initUI();

protected:
    // 监听窗体缩放事件
    void resizeEvent(QResizeEvent *event);

    // 用来获取windows的事件处理
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

};
#endif // MAINWIDGET_H
