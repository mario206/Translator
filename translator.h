#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include <QThread>
#include <words.h>

class Translator : public QObject
{
    Q_OBJECT
public:
    explicit Translator(QObject *parent = 0);
    bool translateWord(QString word);
    bool translateSentences(QString sentences);
    QNetworkAccessManager m_net_manager;
    QNetworkReply* m_reply;
    QString m_result;

public slots:
    void queryFinished();
    void translateWord();

signals:
    void translateFinished(Words);
public:
    QString word;
};


#endif // TRANSLATOR_H
