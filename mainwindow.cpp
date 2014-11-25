#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <form.h>
#include <QCursor>
#include "QDesktopWidget"
#include <QMovie>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = 0;
    onNotice = false;

    //qDebug() << str2 << endl;
    //ui->label->setText(str2);
    m_text = new SelectedText();
    connect(m_text,SIGNAL(newtextselected(QString)),this,SLOT(newselectedtext(QString)));

    MyGlobalShortCut* shortcut = new MyGlobalShortCut("Alt+2",this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(activate()));
    // 手动开启
    activate();
    activate();




}
void MainWindow::activate()
{
    if(onNotice)
        return;

    if(m_text->working)
    {
        noticeWindow();
        m_text->uninstallHook();
    }
    else
    {
        noticeWindow();
        m_text->installHook();
    }

}

void MainWindow::adjustWindowPos(Form *window)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->screenGeometry();
    // 若窗体超出屏幕，进行调整
    QPoint point = QCursor::pos();
    int maxW = deskRect.width();
    int maxH = deskRect.height();

    int x = point.x()-window->width()/2;
    int y = point.y() + 20;
    int w = window->width();
    int h = window->height();


    if(x < 0)
    {
        x = 0;
    }

    if(x + w > maxW)
    {
        x = maxW - w;
    }
    if(y + h > maxH)
    {
        y = point.y() - h - 10;
    }

   window->setGeometry(x,y,w,h);


}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::noticeWindow()
{
//    QString str;
//    str = str.fromUtf8("开启划词翻译");
//    if(m_text->working == true)
//        str = str.fromUtf8("关闭划词翻译");

//    notice->setText(str);
//    notice->setStyleSheet("color:#094;font-family: Verdana,Geneva,sans-serif;font-size:50px;font-weight:bold;line-height: 1.25em;");
//    notice ->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
//    notice ->setAttribute(Qt::WA_TranslucentBackground);
//    notice ->show();
//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    QRect deskRect = desktopWidget->screenGeometry();
//    int cenX = deskRect.width()/2;
//    int cenY = deskRect.height() * 0.4;
//    notice->setGeometry(cenX - notice->width()/2,cenY - notice->height()/2,notice->width(),notice->height());
//    m_timer = startTimer(1000);

    notice = new QLabel();

    onNotice = true;

    QMovie *movie;
    if(m_text->working)
        movie = new QMovie(":/off.gif");
    else
        movie = new QMovie(":/on.gif");
    notice->setMovie(movie);


    notice ->setAttribute(Qt::WA_TranslucentBackground);
    notice ->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    notice->resize(200,200);
    notice->show();

    movie->start();
    m_timer = startTimer(1000);

}
void MainWindow::timerEvent(QTimerEvent *)
{

    notice->hide();
    killTimer(m_timer);
    onNotice = false;
}

void MainWindow::newselectedtext(QString str)
{

    Translator* translator = new Translator(this,str);
    connect(translator,SIGNAL(translateWordFinished(Words)),this,SLOT(translateWordFinished(Words)));
    connect(translator,SIGNAL(translateSentencesFinished(QString)),this,SLOT(translateSentencesFinished(QString)));
    translator->run();

}



void MainWindow::translateWordFinished(Words word)
{
    //qDebug() << "第一个查询窗口" << endl;
    //qDebug() << word.explains << endl;
    Form* window = Form::Instance();
    window->setWord(word);
    window -> setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    window -> show();
    adjustWindowPos(window);
}

void MainWindow::translateSentencesFinished(QString sentences)
{
    Form* window = Form::Instance();
    window->setSentences(sentences);
    window -> setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    window -> show();
    adjustWindowPos(window);

}


