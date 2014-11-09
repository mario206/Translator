#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString str2="<font face=\"Times New Roman\"> θ  ð  ʃ  ʒ  tʃ  dʒ  ŋ ɪ æ  ɔ  ʌ   ʊ  ə  ɛ ɑ: ʊ: ɔ eɪ  ɑɪ  ɔɪ  əʊ  oʊ  ɑʊ ɪə  eə  ʊə  ɪə</font>";
    qDebug() << str2 << endl;
    ui->label->setText(str2);
    m_text = new SelectedText();
    m_translator = new Translator();

    connect(m_text,SIGNAL(newtextselected(QString)),this,SLOT(newselectedtext(QString)));
   // connect(m_translator,SIGNAL(translateFinished(Words)),this,SLOT(translateFinished(Words)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newselectedtext(QString str)
{
    m_translator->translateWord(str);
}

void MainWindow::translateFinished(Words)
{

}
