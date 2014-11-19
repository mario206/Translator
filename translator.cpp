#include "translator.h"
#include <qDebug>

Translator::Translator(QObject *parent) :
    QObject(parent)
{

}

bool Translator::translateWord(QString word)
{
    QString API_url = "http://fanyi.youdao.com/openapi.do?keyfrom=milutranslator&key=469683201&type=data&doctype=json&version=1.1&q=";
    QString query_url = API_url + word;
    QUrl url(query_url);
    m_reply = m_net_manager.get(QNetworkRequest(url));
    connect(m_reply,SIGNAL(finished()),this,SLOT(queryFinished()));
    return true;
}

bool Translator::translateSentences(QString sentences)
{

}

void Translator::queryFinished()
{

    m_result = m_reply->readAll();
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_result.toUtf8(), &error);
    qDebug() << m_result;
    // 返回出错
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "error.error != QJsonParseError::NoError" << endl;
        return;
    }
    // 返回出错
    if (!jsonDocument.isObject()) {
        qDebug() << "!jsonDocument.isObject()" << endl;

        return ;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();
    // 查询出现错误
    if(result["errorCode"] != 0) {
        qDebug() << "查询出错内容" << endl;

        return;
    }
    if(result.find("basic") == result.end()) {
        qDebug() << "查不到所选内容" << endl;

        return;
    }
    QVector<QString> explains;
    // 1.解释
    // n. 绝对；绝对事物",
    QVariantMap basic = result["basic"].toMap();
    foreach (QVariant explain,basic["explains"].toList()) {
        //qDebug() << explain.toString() << endl;
        explains.push_back(explain.toString());
    }
    // 2.音标
    QString phonetic = basic["phonetic"].toString();
    //qDebug() <<  phonetic << endl;
    // 3.所查单词
    QString query = result["query"].toString();

    //qDebug() << query << endl;
    // 4.语音文件url
    QString voice_url = "http://dict.youdao.com/dictvoice?audio=" + query + "&keyfrom=deskdict.stroke&client=deskdict&id=537bacce025530026a26e561b9d9edcdd&vendor=youdao.fanyiindex&in=YoudaoDict_youdao.fanyiindex&appVer=5.4.46.5554&appZengqiang=0";

    Words word(true,explains,phonetic,query,voice_url);

    emit translateFinished(word);


}

void Translator::translateWord()
{
    translateWord(word);
}


