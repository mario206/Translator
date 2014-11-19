#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <words.h>
#include <QLabel>
#include <QPushButton>
#include <QEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    static Form* Instance();
    void setWord(Words Word);
    void show();
    void timerEvent(QTimerEvent *);
private:
    Form();
    Form(QWidget* parent,Words word);
    ~Form();
    void InitWindow();
    void leaveEvent(QEvent *);
    Words m_word;
    int m_timerId;
    static bool hadInit;

    QVBoxLayout* up;
    QVBoxLayout* down;
private:

    // 下载语音
    QNetworkAccessManager m_net_manager;
    QNetworkReply* m_reply;
    QString m_result;

public slots:
    void getVoice();
public:

private:
    Ui::Form *ui;
    static Form* lastWindow;

};

#endif // FORM_H
