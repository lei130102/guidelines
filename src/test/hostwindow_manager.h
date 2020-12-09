#ifndef HOSTWINDOW_MANAGER_H
#define HOSTWINDOW_MANAGER_H

#include <QSize>
#include <QWidget>

#include <Windows.h>
#define WAPI_GWL_WNDPROC (-4)

class hostwindow_manager
{
public:
    hostwindow_manager();

    QWidget* hostwindow() const;
    void set_hostwindow(QWidget* hostwindow);

    HWND hostwindow_hwnd() const;
    void set_hostwindow_hwnd(HWND hostwindow_hwnd);

    QSize hostwindow_size() const;
    void set_hostwindow_size(QSize hostwindow_size);

    bool hostwindow_is_running() const;
    void set_hostwindow_is_running(bool hostwindow_is_running);

    WNDPROC hostwindow_wndproc() const;
    void set_hostwindow_wndproc(WNDPROC hostwindow_wndproc);

    HWND wpf_hwnd() const;
    void set_wpf_hwnd(HWND wpf_hwnd);

private:
    QWidget* m_hostwindow;
    HWND m_hostwindow_hwnd;
    QSize m_hostwindow_size;
    bool m_hostwindow_is_running;
    WNDPROC m_hostwindow_wndproc;

    HWND m_wpf_hwnd;
};

#endif // HOSTWINDOW_MANAGER_H
