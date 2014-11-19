#pragma once
#include <QApplication>
#include <QAbstractNativeEventFilter>
#include <windows.h>
#include <QObject>
#include <MyGlobalShortcut/MyWinEventFilter.h>
#include <QKeySequence>
#include <QHash>
class MyGlobalShortCut;


class MyGlobalShortCut : public QObject
{
    Q_OBJECT
public:
    MyGlobalShortCut();
    MyGlobalShortCut(QObject* app);
    MyGlobalShortCut(QString key,QObject* app);
    MyGlobalShortCut(QKeySequence key,QObject* app);
    ~MyGlobalShortCut();
    void activateShortcut();
    bool registerHotKey();
    bool unregisterHotKey();
    QHash<QPair<quint32, quint32>, MyGlobalShortCut*> shortcuts;
private:
    QApplication *m_app;
    MyWinEventFilter *m_filter;
    QKeySequence m_key;
    Qt::Key key;
    Qt::KeyboardModifiers mods;
    static quint32 nativeKeycode(Qt::Key keycode);
    static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
signals:
    void activated();

};
