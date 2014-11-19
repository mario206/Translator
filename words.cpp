#include "words.h"

Words::Words()
{

}

Words::Words(bool t_isword, QVector<QString> t_explains, QString t_phonetic,QString t_query, QString t_voice_url)
    :isWord(t_isword),explains(t_explains),phonetic(t_phonetic),query(t_query),voice_url(t_voice_url)
{
    // 首字母大写
    query = t_query.mid(0,1).toUpper() + t_query.mid(1,query.length()-1);
}
