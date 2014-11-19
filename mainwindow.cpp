#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <form.h>
#include <QCursor>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = 0;
    notice = new QLabel();
    //QString str2="<font face=\"Times New Roman\"> θ  ð  ʃ  ʒ  tʃ  dʒ  ŋ ɪ æ  ɔ  ʌ   ʊ  ə  ɛ ɑ: ʊ: ɔ eɪ  ɑɪ  ɔɪ  əʊ  oʊ  ɑʊ ɪə  eə  ʊə  ɪə</font>";
    //qDebug() << str2 << endl;
    //ui->label->setText(str2);
    m_text = new SelectedText();
    connect(m_text,SIGNAL(newtextselected(QString)),this,SLOT(newselectedtext(QString)));

    MyGlobalShortCut* shortcut = new MyGlobalShortCut("Alt+2",this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(activate()));
   // ui->pushButton->show();



}
void MainWindow::activate()
{
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



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::noticeWindow()
{
    QString str = "开启划词翻译";
    if(m_text->working == true)
        str = "关闭划词翻译";

    notice->setText(str);
    notice->setStyleSheet("color:#094;font-family: Verdana,Geneva,sans-serif;font-size:50px;font-weight:bold;line-height: 1.25em;");
    notice ->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    notice ->setAttribute(Qt::WA_TranslucentBackground);
    notice ->show();

    m_timer = startTimer(500);

}
void MainWindow::timerEvent(QTimerEvent *)
{
    notice->hide();
    killTimer(m_timer);
}

void MainWindow::newselectedtext(QString str)
{


    MyThread* thread = new MyThread(this,str);

    connect(thread,SIGNAL(translateFinished(Words)),this,SLOT(translateFinished(Words)));
    thread->run();

}

void MainWindow::translateFinished(Words word)
{
    qDebug() << word.explains << endl;
    Form* window = Form::Instance();
    window->setWord(word);
    window -> setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    QPoint point = QCursor::pos();
    window -> show();
    window->setGeometry(point.x()-window->width()/2,point.y() + 20,window->width(),window->height());

    //connect(window,SIGNAL(getVoice(QString)),this,SLOT(playVoice(QString)));

}



MyThread::MyThread(QObject *pare, QString str)
    :QThread(pare),word(str)
{

}

void MyThread::run()
{

    Translator* translator = new Translator();
    translator -> translateWord(word);
    connect(translator,SIGNAL(translateFinished(Words)),this,SIGNAL(translateFinished(Words)));
}


