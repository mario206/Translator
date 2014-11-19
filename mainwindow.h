#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <selectedtext.h>
#include <translator.h>
#include <form.h>
#include <MyGlobalShortcut/MyGlobalShortCut.h>
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void noticeWindow();
    void timerEvent(QTimerEvent *);
public slots:

    // 有新的剪贴板内容
    void newselectedtext(QString str);
    // 新的单词查询完成
    void translateFinished(Words word);
    // 划词开关
    void activate();
private:
    Ui::MainWindow *ui;
    SelectedText* m_text;
    QLabel* notice;
    int m_timer;


};

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QObject* pare,QString str);
    void run();
private:
    QString word;
signals:
    void translateFinished(Words word);

};

#endif // MAINWINDOW_H
