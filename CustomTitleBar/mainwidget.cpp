#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    initUI();
}

void MainWidget::initUI()
{
    // 设置主窗体无边框，注意函数名为 setWindowFlags ，不要少写了最好的 s 。
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_nBorderWidth = 5;
    m_pMovieLable = new QLabel(this);
    m_pTitleBar = new TitleBar(this);
    // 安装事件过滤器
    this->installEventFilter(m_pTitleBar);

    // 设置主窗体
    this->resize(960, 640);
    this->setMinimumSize(300, 200);
    this->setWindowIcon(QIcon(":/image/image/icon.png"));
    this->setWindowTitle("rouot");

    // 动图播放器
    m_pMovie = new QMovie(":/image/image/background.gif");
//    m_pMovieLable->setScaledContents(true);
    m_pMovieLable->setMovie(m_pMovie);
    m_pMovie->start();
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    m_pTitleBar->setGeometry(0, 0, width(), 30);
    m_pMovieLable->setGeometry(0, 30, width(), height()-30);

    event->ignore();
}

bool MainWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        // 计算鼠标在窗体中的位置坐标
        int xPos = GET_X_LPARAM(param->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(param->lParam) - this->frameGeometry().y();

        // 鼠标区域位于窗体边框，进行缩放。可以模拟窗体边框缩放效果
        if(xPos < m_nBorderWidth && yPos<m_nBorderWidth)                    //左上角
            *result = HTTOPLEFT;
        else if(xPos>=width()-m_nBorderWidth&&yPos<m_nBorderWidth)          //右上角
            *result = HTTOPRIGHT;
        else if(xPos<m_nBorderWidth&&yPos>=height()-m_nBorderWidth)         //左下角
            *result = HTBOTTOMLEFT;
        else if(xPos>=width()-m_nBorderWidth&&yPos>=height()-m_nBorderWidth)//右下角
            *result = HTBOTTOMRIGHT;
        else if(xPos < m_nBorderWidth)                                     //左边
            *result = HTLEFT;
        else if(xPos>=width()-m_nBorderWidth)                              //右边
            *result = HTRIGHT;
        else if(yPos<m_nBorderWidth)                                       //上边
            *result = HTTOP;
        else if(yPos>=height()-m_nBorderWidth)                             //下边
            *result = HTBOTTOM;
        else              //其他部分不做处理，返回false，留给其他事件处理器处理
            return false;

        return true;
    }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

MainWidget::~MainWidget()
{
    delete ui;
}
