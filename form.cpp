#include "form.h"
#include "ui_form.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <qdebug.h>
#include <QMediaPlayer>

bool Form::hadInit = false;
Form* Form::lastWindow = NULL;

Form *Form::Instance()
{
    // 保证只有一个窗口存在
    Form* w = new Form();
    if(lastWindow != NULL)
        lastWindow->hide(); 
    lastWindow = w;
    return w;
}

void Form::setWord(Words Word)
{
    m_word = Word;
    InitWordWindow();
}

void Form::setSentences(QString text)
{
    sentences = text;
    InitSentencesWindow();
}

void Form::show()
{
    m_timerId = startTimer(500);
   this->setVisible(true);
}



Form::Form()
{
    this->setWindowIcon(QIcon(":/ico.png"));

}

Form::Form(QWidget *parent, Words word)
    :QWidget(parent),ui(new Ui::Form)
{

    m_word = word;
    //ui->setupUi(this);

//    ui->word->setText(m_word.query);
//    ui->phonetic->setText("[" + m_word.phonetic + "]");
//    QString explain = "";
//    for(auto i : m_word.explains)
//    {
//        explain += i;
//        explain += "\n";
//    }
//    ui->explains->setText(explain);
      InitWordWindow();
}

Form::~Form()
{
    delete ui;
}

void Form::InitWordWindow()
{

    QVBoxLayout* up = new QVBoxLayout;
    QHBoxLayout* h = new QHBoxLayout;
    QLabel* word = new QLabel(this);
    word->setText(m_word.query);
    word->setStyleSheet("color:#094;font-family: Verdana,Geneva,sans-serif;font-size:20px;font-weight:bold;line-height: 1.25em;");

    QLabel* phonetic = new QLabel(this);
    phonetic->setText("[" + m_word.phonetic + "]");

    QPushButton* btn = new QPushButton(this);
    connect(btn,SIGNAL(clicked()),this,SLOT(getVoice()));



    //btn->setGeometry(btn->pos().x(),btn->pos().y(),41,41);
    btn->setStyleSheet("QPushButton {border-image: url(:/green.png);width:32px;height:32px;} QPushButton:hover { border-image: url(:/pink.png)};");
   // btn->resize(40,80);
    //btn->resize(128,128);



    h->addWidget(phonetic);
    h->addWidget(btn,0,Qt::AlignLeft);
   // h->addWidget(btn);
    phonetic->setStyleSheet("font-family: 'lucida sans unicode',arial,sans-serif;font-weight: bold;font-size: 14px;color: #094;");

    up->addWidget(word,0,Qt::AlignLeft);

    up->addLayout(h);
    //up->addWidget(phonetic);

    //word->setAttribute(Qt::WA_TranslucentBackground);
    //phonetic->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout* down = new QVBoxLayout;
    for(auto i : m_word.explains)
    {
        QLabel* left  = new QLabel(this);
        QLabel* right = new QLabel(this);
        QHBoxLayout* hl = new QHBoxLayout;

        int dot = i.indexOf(".");
        QString type = i.mid(0,dot + 1);
        QString explain = i.mid(dot+1,i.length() - dot - 1);

        left->setText(type);
        right->setText(explain);

        left->setAlignment(Qt::AlignLeft);
        right->setAlignment(Qt::AlignLeft);

        left->setStyleSheet("font-family: Georgia,serif;font-size: 18px;font-weight: bold;color: #094;");
        right->setStyleSheet("font-family: Georgia,serif;font-weight: bold;font-size: 18px;color: #094;");
        right->setWordWrap(true);

        //left->setAttribute(Qt::WA_TranslucentBackground);
        //right->setAttribute(Qt::WA_TranslucentBackground);


        hl->addWidget(left);
        hl->addWidget(right,Qt::AlignLeft);
        down->addLayout(hl);
    }

    //up->addLayout(h);

    up->addLayout(down);
   // up->setContentsMargins(0,0,0,0);
    //up->setContentsMargins(1,1,1,1);
    this->setLayout(up);
    setWindowOpacity(0.85);
    this->setMaximumWidth(600);
    this->setStyleSheet("background-color: rgb(255, 255, 255);");
    //this ->setAttribute(Qt::WA_TranslucentBackground);    
}

void Form::InitSentencesWindow()
{
    QVBoxLayout* up = new QVBoxLayout;
    QLabel* word = new QLabel(this);
    word->setMaximumWidth(600);
    word->setWordWrap(true);
    word->setText(sentences);
    word->setStyleSheet("color:#094;font-family: Verdana,Geneva,sans-serif;font-size:20px;font-weight:bold;line-height: 1.25em;");
    up->addWidget(word,Qt::AlignCenter);
    this->setLayout(up);
    setWindowOpacity(0.85);
    //this->setMaximumWidth(600);
    this->setStyleSheet("background-color: rgb(255, 255, 255);");
}

void Form::leaveEvent(QEvent *)
{
    this->close();
}


void Form::getVoice()
{
    QMediaPlayer* player = new QMediaPlayer();

    QUrl url(m_word.voice_url);

    player->setMedia(url);
    player->setVolume(100);
    player->play();

    m_reply = m_net_manager.get(QNetworkRequest(url));
    connect(m_reply,SIGNAL(finished()),this,SLOT(downloadFinished()));
}



void Form::timerEvent(QTimerEvent *)
{
    QPoint point = QCursor::pos();
    QRect rect = this->geometry();
//    qDebug() << "point:" << point << endl;
//    qDebug() << "rect:" << rect << endl;

    // 鼠标在窗体上面
    if(rect.y() > point.y())
    {
//        qDebug() << "鼠标在窗体上面";
        rect.adjust(0,-100,0,0);
//        qDebug() << point;
//        qDebug() << rect;
    }
    // 鼠标在窗体下面
    else
    {
//        qDebug() << "鼠标在窗体下面";
        rect.adjust(0,300,0,0);
//        qDebug() << point;
//        qDebug() << rect;
    }

    if(!rect.contains(point))
    {
        killTimer(m_timerId);
        this->hide();
    }


}
