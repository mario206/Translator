#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>

class Words {

public:
    Words(bool, QVector<QString>,QString,QString, QString);
    bool isWord;
    QVector<QString> explains;
    QString phonetic;
    QString query;
    QString voice_url;
};

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


signals:
    void translateFinished(Words);
public slots:
};


#endif // TRANSLATOR_H
