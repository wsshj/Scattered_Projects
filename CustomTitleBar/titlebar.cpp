#include "titlebar.h"
#include "ui_titlebar.h"

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);

    initUI();
}

void TitleBar::initUI()
{
    // 定义
    m_pIconLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);
    m_pMinimizeButton = new QPushButton(this);
    m_pMaximizeButton = new QPushButton(this);
    m_pCloseButton = new QPushButton(this);

    // 设置 objectName 供外部调用
    m_pTitleLabel->setObjectName("whiteLabel");
    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");

    // 设置工具提示
    m_pMinimizeButton->setToolTip("最小化");
    m_pMaximizeButton->setToolTip("最大化");
    m_pCloseButton->setToolTip("关闭");

    // 设置标题属性
    m_pTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 设置按键图标
    m_pMinimizeButton->setIcon(QIcon(":/image/image/min.png"));
    m_pMaximizeButton->setIcon(QIcon(":/image/image/max.png"));
    m_pCloseButton->setIcon(QIcon(":/image/image/close.png"));

    // 设置按键图标尺寸
    m_pMinimizeButton->setIconSize(QSize(25, 25));
    m_pMaximizeButton->setIconSize(QSize(25, 25));
    m_pCloseButton->setIconSize(QSize(25, 25));

    // 去除按键边框
    m_pMinimizeButton->setStyleSheet("border:none;");
    m_pMaximizeButton->setStyleSheet("border:none;");
    m_pCloseButton->setStyleSheet("border:none;");

    // 创建画布填充背景色
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(150,150,150));
    setAutoFillBackground(true);
    setPalette(pal);

    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

void TitleBar::resizeEvent(QResizeEvent *event)
{
    m_pIconLabel->setGeometry(0, 0, 30, 30);
    m_pTitleLabel->setGeometry(35, 0, 130, 30);
    m_pMinimizeButton->setGeometry(width()-90, 0, 30, 30);
    m_pMaximizeButton->setGeometry(width()-60, 0, 30, 30);
    m_pCloseButton->setGeometry(width()-30, 0, 30, 30);

    event->ignore();
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close();
        }
    }
}

//进行界面的拖动  [一般情况下，是界面随着标题栏的移动而移动，所以我们将事件写在标题栏中比较合理]
void TitleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
            SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
    event->ignore(); //没有实质性的作用，只是用来允许event可以不使用，用来避免编译器警告
#else
#endif
}

//双击标题栏进行界面的最大化/还原
void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit m_pMaximizeButton->clicked();
    }
}

//使用事件过滤器监听标题栏所在的窗体，所以当窗体标题、图标等信息发生改变时，标题栏也应该随之改变
bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) //判断发生事件的类型
    {
    case QEvent::WindowTitleChange: //窗口标题改变事件
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj); //获得发生事件的窗口对象
        if (pWidget)
        {
            //窗体标题改变，则标题栏标题也随之改变
            m_pTitleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    break;
    case QEvent::WindowIconChange: //窗口图标改变事件
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            //窗体图标改变，则标题栏图标也随之改变
            QIcon icon = pWidget->windowIcon();
            m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
            return true;
        }
    }
    break;
    case QEvent::Resize:
    {
        updateMaximize(); //最大化/还原
    }
    break;
    default:
        return QWidget::eventFilter(obj, event);
    }

    return QWidget::eventFilter(obj, event);
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_pMaximizeButton->setToolTip(tr("还原"));
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            m_pMaximizeButton->setToolTip(tr("最大化"));
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
        }

        m_pMaximizeButton->setStyle(QApplication::style());
    }
}

TitleBar::~TitleBar()
{
    delete ui;
}
