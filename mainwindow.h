#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <selectedtext.h>
#include <translator.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void newselectedtext(QString str);
    void translateFinished(Words word);
private:
    Ui::MainWindow *ui;
    SelectedText* m_text;
    Translator*   m_translator;

};

#endif // MAINWINDOW_H
