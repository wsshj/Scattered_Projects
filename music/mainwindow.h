#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QDir>
#include <QDirIterator>
#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QMimeData>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QMediaPlayer *player;
    QMediaPlaylist *playerlist;
    QList<QMediaContent> List;

    QString settime(qint64 playtime);
    void traverseFolder(QString path);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_verticalSlider_valueChanged(int value);

    void getduration(qint64 playtime);

    void positionChanged(qint64 playtime);

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void playButtonChange(QMediaPlayer::State state);

    void nowPlaying(const QMediaContent &content);

    void showMediaList(int start, int end);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
