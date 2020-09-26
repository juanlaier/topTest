#include "ctopcontrol.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <QDebug>

CTopControl::CTopControl(QObject *parent) : QObject(parent)
{

}


QStringList CTopControl::RefreshProgressList()
{
    QStringList list;
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot != INVALID_HANDLE_VALUE){
        PROCESSENTRY32 process = {sizeof(PROCESSENTRY32)};
        BOOL bRet = Process32First(hSnapshot, &process);
        while(bRet){
            list.append(QString::fromStdWString(process.szExeFile));
            bRet = Process32Next(hSnapshot, &process);
        }
    }
    return list;
}

bool is_top_level_window(HWND hwnd) {
    if (!IsWindow(hwnd)) return false;

    DWORD dw_style = GetWindowLongPtr(hwnd, GWL_STYLE);
    DWORD dw_exstyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

    DWORD includes = WS_CHILDWINDOW;
    DWORD excludes = WS_VISIBLE /*| WS_MINIMIZEBOX*/;

    if ((dw_style & includes) != 0 || (dw_style & excludes) != excludes)
        return false;

    includes = WS_EX_TOOLWINDOW | WS_EX_NOREDIRECTIONBITMAP;
    excludes = 0;

    if ((dw_exstyle & includes) != 0 || (dw_exstyle & excludes) != excludes)
        return false;

    if (dw_style & WS_POPUP) {
        if (GetParent(hwnd) != NULL) {
            excludes = WS_EX_OVERLAPPEDWINDOW;
            if ((dw_exstyle & excludes) != excludes) {
                return false;
            }
        }
    }
    return true;
}

BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam) {
    QStringList arrWindows;
    if (is_top_level_window(hwnd)) {
        char buff[1024] = { 0 };
        buff[GetWindowTextA(hwnd, buff, _countof(buff))] = '\0';
        DWORD processId = 0;
        GetWindowThreadProcessId(hwnd, &processId);
        printf("%08X - %08X - %s\n", processId, hwnd, buff);
        arrWindows.append(buff);
    }
    return 1;
}

QStringList CTopControl::refreshHWNDList()
{
    HDESK hDesk = GetThreadDesktop(GetCurrentThreadId());
    EnumDesktopWindows(hDesk, [](HWND hWnd, LPARAM lParam)->BOOL{
        CTopControl* pThis = (CTopControl*)lParam;
        QString strWindow;
        if (is_top_level_window(hWnd)) {
            char buff[1024] = { 0 };
            buff[GetWindowTextA(hWnd, buff, _countof(buff))] = '\0';
            DWORD processId = 0;
            GetWindowThreadProcessId(hWnd, &processId);
            printf("%08X - %08X - %s\n", processId, hWnd, buff);
            strWindow = QString::fromLocal8Bit(buff);
            pThis->_arrWindows.append(QPair<QString, int64_t>(strWindow, (int64_t)hWnd));
        }
        return TRUE;
    }, (LPARAM)this);

    return arrWindowsName();
}

void CTopControl::setWindowOnTop(int index, bool bOnTop)
{

    if(index >= 0 && index < _arrWindows.length()){
        for(auto i : _arrWindows){
            HWND hWnd = (HWND)i.second;
            ::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
        }

        HWND hWnd = (HWND)_arrWindows.at(index).second;
        SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE);
    }
}


QStringList CTopControl::arrWindowsName()
{
    QStringList list;
    for(auto i : _arrWindows){
        list << i.first;
    }
    return list;
}

void CTopControl::setWindowVisible(bool bVisible)
{
}

void CTopControl::printStringList(QStringList arrTexts)
{
}
