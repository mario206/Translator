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

class Translator : public QThread
{
    Q_OBJECT
public:
    Translator(QObject*,QString);
    void run();
private:
    // 查询单词
    void translateWord();

private slots:
    // 查询单词有结果
    void queryWordFinished();
    // 单词查询失败后当成句子查询
    void translateSentences();
    // 查询句子有结果
    void querySentencesFinished();
signals:
    // 单词，句子查询成功（对外信号）
    void translateWordFinished(Words word);
    void translateSentencesFinished(QString);
    // 单词查询失败（对内信号）
    void failedToTranslateWord();



private:
    QString text;
    QNetworkAccessManager m_net_manager;
    QNetworkReply* m_reply;
    QNetworkReply* s_reply;
    QString m_result;

};


#endif // TRANSLATOR_H
