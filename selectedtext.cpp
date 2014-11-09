#include "selectedtext.h"
#include <qDebug>

SelectedText* SelectedText::pthis = NULL;

SelectedText::SelectedText(QObject *parent) :
    QObject(parent)
{
    pthis = this;
    // 初始化变量
    m_move = false;
    // 初始化剪贴板
    m_board = QApplication::clipboard();
    // 安装钩子
    installHook();

}

SelectedText::~SelectedText()
{
    uninstallHook();
}

int SelectedText::mouseProc(int ncode, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;
    switch(wParam)
    {

    // 左键按下
    case WM_LBUTTONDOWN:
        pthis->m_down_pos = p->pt;
        pthis->m_move = false;

        break;
    // 鼠标移动
    case WM_MOUSEMOVE:
        pthis->m_move = true;

        break;
    // 左键弹起，交给
    case WM_LBUTTONUP:
        pthis->m_up_pos = p->pt;
        pthis->checkSelectedText();
        pthis->m_move = false;

        break;
    }
    return CallNextHookEx(pthis->MyHook,ncode,wParam,lParam);
}



void SelectedText::installHook()
{
    MyHook = SetWindowsHookEx(WH_MOUSE_LL,
            (HOOKPROC)&SelectedText::mouseProc,   // 回调函数地址
            GetModuleHandle(NULL),
            NULL);
}

void SelectedText::uninstallHook()
{
    // 卸载钩子
    UnhookWindowsHookEx(MyHook);

}

void SelectedText::checkSelectedText()
{



    if(pthis->m_move) {
        qDebug() << "检测中" << endl;
        // 获取左键弹起时光标所在窗口句柄
        m_hwnd = WindowFromPoint(m_up_pos);
        if(!m_hwnd) {
             qDebug() << "获取不到窗口句柄" << endl;
             return;
        }
        // 输出窗口句柄
        //qDebug() << (int)hwnd;




        // 备份剪贴板信息
        m_lastString = m_board->text();
        //拷贝一份原剪贴板信息
        m_old_data = copyMimeData(m_board->mimeData());
        connect(m_board,SIGNAL(dataChanged()),this,SLOT(textCopyFinished()));

        // 发送拷贝请求
        SetForegroundWindow(m_hwnd);

        keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event('C', 0x2E, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event('C', 0x2E, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }

}

void SelectedText::textCopyFinished()
{
    disconnect(m_board,SIGNAL(dataChanged()),this,SLOT(textCopyFinished()));
    QString str = m_board->text();

   // TCHAR buffer[256];
    //GetWindowText(hwnd,buffer,256);

   // QString title = QString::fromWCharArray(buffer);
    if(!str.isEmpty() && str != m_lastString)
    {
        qDebug() << str << endl;
        emit newtextselected(str);
    }
    else
    {
         SendMessage(m_hwnd,WM_COPY,0,0);
         str = m_board->text();
         if(!str.isEmpty() && str != m_lastString) {
             qDebug() << str << endl;
             emit newtextselected(str);
         }
    }
  // 恢复剪贴板信息
    m_board->setMimeData(m_old_data);


}

QMimeData *SelectedText::copyMimeData(const QMimeData *mimeReference)
{
    QMimeData * mimeCopy = new QMimeData();

        foreach(QString format, mimeReference->formats())
        {
            // Retrieving data
            QByteArray data = mimeReference->data(format);
            // Checking for custom MIME types
            if(format.startsWith("application/x-qt"))
            {
                // Retrieving true format name
                int indexBegin = format.indexOf('"') + 1;
                int indexEnd = format.indexOf('"', indexBegin);
                format = format.mid(indexBegin, indexEnd - indexBegin);
            }
            mimeCopy->setData(format, data);
        }

        return mimeCopy;
}
