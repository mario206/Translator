#include "translator.h"
#include <qDebug>
#include <QTextCodec>

Translator::Translator(QObject *parent, QString str)
    :QThread(parent),text(str)
{

}

void Translator::run()
{
    translateWord();
    // 单词查询失败，则改为句子查询
    connect(this,SIGNAL(failedToTranslateWord()),this,SLOT(translateSentences()));
}
void Translator::translateSentences()
{
    // URL Encode
    QTextCodec * codecGB2312 = QTextCodec::codecForName("utf-8");
    QByteArray byteArrayGB2312 = codecGB2312->fromUnicode(text);
    QByteArray byteArrayPercentEncoded = byteArrayGB2312.toPercentEncoding();
    QString query = QString(byteArrayPercentEncoded);
    QString query_url = "http://fanyi.youdao.com/translate?keyfrom=deskdict.main&dogVersion=1.0&ue=utf8&i=" + query + "&doctype=json&type=AUTO&xmlVersion=1.6&client=deskdict&id=02f0a95cf7ea6800e&vendor=unknown&in=YoudaoDict&appVer=6.2.54.2064&appZengqiang=0&abTest=&smartresult=dict&smartresult=rule";

    QUrl url(query_url);
    QNetworkRequest request(url);

//    request.setRawHeader(request.setRawHeader(QByteArray("Accept"), QByteArray("*/*")));
//    request.setRawHeader(request.setRawHeader(QByteArray("Cookie"), QByteArray("DESKDICT_VENDOR=unknown; OUTFOX_SEARCH_USER_ID=-318915942@121.8.210.21; JSESSIONID=abcHn8mvx40cke4BTCLNu")));
//    request.setRawHeader(request.setRawHeader(QByteArray("Accept-Encoding"), QByteArray("gzip")));
//    request.setRawHeader(request.setRawHeader(QByteArray("User-Agent"), QByteArray("Youdao Desktop Dict (Windows 6.2.9200)")));



    s_reply = m_net_manager.get(QNetworkRequest(url));
    connect(s_reply,SIGNAL(finished()),this,SLOT(querySentencesFinished()));



}

void Translator::queryWordFinished()
{

    m_result = m_reply->readAll();
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_result.toUtf8(), &error);
    qDebug() << m_result;
    // 返回出错
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "error.error != QJsonParseError::NoError" << endl;

        // 改为句子查询
        emit failedToTranslateWord();

        return;
    }
    // 返回出错
    if (!jsonDocument.isObject()) {
        qDebug() << "!jsonDocument.isObject()" << endl;

        // 改为句子查询
        emit failedToTranslateWord();
        return ;
    }

    QVariantMap result = jsonDocument.toVariant().toMap();
    // 查询出现错误
    if(result["errorCode"] != 0) {
        qDebug() << "查询出错内容" << endl;


        // 改为句子查询
        emit failedToTranslateWord();

        return;
    }
    if(result.find("basic") == result.end()) {
        //qDebug() << "查不到所选内容" << endl;

        // 改为句子查询
        emit failedToTranslateWord();

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

    emit translateWordFinished(word);
}





void Translator::translateWord()
{
    QString API_url = "http://fanyi.youdao.com/openapi.do?keyfrom=milutranslator&key=469683201&type=data&doctype=json&version=1.1&q=";
    QString query_url = API_url + text;
    QUrl url(query_url);
    m_reply = m_net_manager.get(QNetworkRequest(url));
    connect(m_reply,SIGNAL(finished()),this,SLOT(queryWordFinished()));
}


void Translator::querySentencesFinished()
{
    m_result = s_reply->readAll();

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
        qDebug() << "查询句子出错" << endl;

        return;
    }
    if(result.find("translateResult") == result.end()) {
        //qDebug() << "查不到所选内容" << endl;
        return;
    }

    QString explain;
    QVariantList lis = QVariant(result["translateResult"].toList()).toList();


    foreach (QVariant lis,result["translateResult"].toList()) {
        foreach(QVariant li,lis.toList())
        {
            QVariantMap map = li.toMap();
            explain += map["tgt"].toString();
        }
    }
    qDebug() << "explain :" << explain << endl;
    if(!(explain.toLower() == text.toLower()))
        emit translateSentencesFinished(explain);

}


