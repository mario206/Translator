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
    void translateWordFinished(Words word);
    // 新的句子查询完成
    void translateSentencesFinished(QString);
    // 划词开关
    void activate();

private:
    void adjustWindowPos(Form* window);
private:
    Ui::MainWindow *ui;
    SelectedText* m_text;
    QLabel* notice;
    int m_timer;
    bool onNotice;


};
#endif

