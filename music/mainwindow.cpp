#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer;
    playerlist = new QMediaPlaylist;



//    for(int i=0;i<playerlist->mediaCount();i++)
//    {
//        ui->listWidget->addItem(playerlist->media(i).canonicalUrl().url());
//    }

    player->setPlaylist(playerlist);

    player->playlist()->setPlaybackMode(playerlist->CurrentItemOnce);

    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(getduration(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(playButtonChange(QMediaPlayer::State)));
    connect(player, SIGNAL(currentMediaChanged(const QMediaContent)), this, SLOT(nowPlaying(const QMediaContent)));
    connect(playerlist, SIGNAL(mediaInserted(int, int)), this, SLOT(showMediaList(int, int)));

    ui->verticalSlider->setMinimum(0);
    ui->verticalSlider->setMaximum(100);
    ui->verticalSlider->setSingleStep(1);
    ui->verticalSlider->setValue(100);

//    ui->listWidget->setAcceptDrops(true);
    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::traverseFolder(QString path)
{
    //判断路径是否存在
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }

    //获取所选文件类型过滤器
    QStringList filters;
    filters<<QString("*.flac")<<QString("*.wav")<<QString("*.mp3")<<QString("*.ape");

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(path,
        filters,
        QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);

    QStringList string_list;

    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        playerlist->addMedia(QUrl::fromLocalFile(absolute_file_path));
    }
}

QString MainWindow::settime(qint64 playtime)
{
    qint64 hour, minute, second;

    qint64 time = playtime / 1000;  //获得的 playtime 时间是以毫秒为单位的

    hour = time / 3600;
    minute = (time - hour * 3600) / 60;
    second = time - hour * 3600 - minute * 60;

    if(minute < 10)
    {
        if(second < 10)
        {
            return QString("0%1:0%2").arg(minute).arg(second);
        }
        return QString("0%1:%2").arg(minute).arg(second);
    }
    return QString("%1:%2").arg(minute).arg(second);  //把int型转化为string类型后再设置为label的text
}

void MainWindow::getduration(qint64 playtime)
{
    ui->label->setText(settime(playtime));

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(int(playtime));
    ui->horizontalSlider->setSingleStep(1);
}

void MainWindow::positionChanged(qint64 playtime)
{
    ui->label_2->setText(settime(playtime));
    ui->horizontalSlider->setValue(int(playtime));
}

void MainWindow::on_pushButton_clicked()
{
    if(player->state() == player->PlayingState)
    {
        player->pause();
    }
    else
    {
        player->play();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    player->stop();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    player->setVolume(value);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(value > player->position() + 1000)
    {
        player->setPosition(value);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    player->playlist()->previous();
}

void MainWindow::on_pushButton_4_clicked()
{
    player->playlist()->next();
}

void MainWindow::on_pushButton_5_clicked()
{
    int mod = ui->pushButton_5->text().toInt();
    if(mod == 4)
    {
        mod = -1;
    }
    mod++;
    ui->pushButton_5->setText(QString::number(mod));

    switch (mod) {
    case 0: //单曲，播完停止
        player->playlist()->setPlaybackMode(playerlist->CurrentItemOnce);
        break;
    case 1: //单曲，循环
        player->playlist()->setPlaybackMode(playerlist->CurrentItemInLoop);
        break;
    case 2: //列表，播完停止
        player->playlist()->setPlaybackMode(playerlist->Sequential);
        break;
    case 3: //列表，循环
        player->playlist()->setPlaybackMode(playerlist->Loop);
        break;
    case 4: //随机
        player->playlist()->setPlaybackMode(playerlist->Random);
        break;
    default:
        break;
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    playerlist->setCurrentIndex(ui->listWidget->row(item));
    player->play();
}

void MainWindow::playButtonChange(QMediaPlayer::State state)
{
    switch (state) {
    case player->PlayingState:
        ui->pushButton->setText("||");
        break;
    case player->PausedState:
        ui->pushButton->setText(">");
        break;
    case player->StoppedState:
        ui->pushButton->setText(">");
        break;
    }
}

void MainWindow::nowPlaying(const QMediaContent &content)
{
    ui->label_4->setText(content.canonicalUrl().url().split("/").last());

}

void MainWindow::showMediaList(int start, int end)
{
    while (start <= end) {
        QString fileName = playerlist->media(start).canonicalUrl().url().split("/").last();
        ui->listWidget->addItem(fileName);
        start++;
    }
}

//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
//    foreach(QString url, event->mimeData()->formats())
//    {
//        ui->listWidget->addItem(url);
//    }
    //如果为文件，则支持拖放
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

//当用户放下这个文件后，就会触发dropEvent事件
void MainWindow::dropEvent(QDropEvent *event)
{
    //注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
    //如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
    QList<QUrl> urls = event->mimeData()->urls();

    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls)
    {
        QString file_path = url.toLocalFile();

        QFileInfo fileInfo(file_path);

        if(fileInfo.isDir())
        {
            traverseFolder(file_path);
            continue;
        }

        QString file_name = fileInfo.fileName();
        QString file_extend = fileInfo.suffix();

        if(file_extend == "flac" || file_extend == "wav" || file_extend == "mp3" || file_extend == "ape")
        {
//            ui->listWidget->addItem(file_name);
            playerlist->addMedia(QUrl::fromLocalFile(file_path));
        }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if(ui->listWidget->isVisible())
    {
        ui->listWidget->setVisible(false);
        ui->widget->setGeometry(0,height()-120, width(), 100);
        ui->pushButton_6->setText("A");
    }
    else
    {
        ui->listWidget->setVisible(true);
        ui->listWidget->setGeometry(0,100,width(),height()-120);
        ui->widget->setGeometry(0,0, width(), 100);
        ui->pushButton_6->setText("V");
    }
}
