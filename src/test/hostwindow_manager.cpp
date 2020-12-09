#include "hostwindow_manager.h"

hostwindow_manager::hostwindow_manager():
    m_hostwindow(nullptr),
    m_hostwindow_hwnd(NULL),
    m_hostwindow_is_running(false),
    m_hostwindow_wndproc(NULL),
    m_wpf_hwnd(NULL)
{

}

QWidget* hostwindow_manager::hostwindow() const
{
    return m_hostwindow;
}

void hostwindow_manager::set_hostwindow(QWidget* hostwindow)
{
    m_hostwindow = hostwindow;
}

HWND hostwindow_manager::hostwindow_hwnd() const
{
    return m_hostwindow_hwnd;
}

void hostwindow_manager::set_hostwindow_hwnd(HWND hostwindow_hwnd)
{
    m_hostwindow_hwnd = hostwindow_hwnd;
}

QSize hostwindow_manager::hostwindow_size() const
{
    return m_hostwindow_size;
}

void hostwindow_manager::set_hostwindow_size(QSize hostwindow_size)
{
    m_hostwindow_size = hostwindow_size;
}

bool hostwindow_manager::hostwindow_is_running() const
{
    return m_hostwindow_is_running;
}

void hostwindow_manager::set_hostwindow_is_running(bool hostwindow_is_running)
{
    m_hostwindow_is_running = hostwindow_is_running;
}

WNDPROC hostwindow_manager::hostwindow_wndproc() const
{
    return m_hostwindow_wndproc;
}

void hostwindow_manager::set_hostwindow_wndproc(WNDPROC hostwindow_wndproc)
{
    m_hostwindow_wndproc = hostwindow_wndproc;
}

HWND hostwindow_manager::wpf_hwnd() const
{
    return m_wpf_hwnd;
}

void hostwindow_manager::set_wpf_hwnd(HWND wpf_hwnd)
{
    m_wpf_hwnd = wpf_hwnd;

    if(m_wpf_hwnd != nullptr)
    {
        //Set WPF Window to a Child Control
        SetWindowLong(m_wpf_hwnd, GWL_STYLE, WS_CHILD);

        //Get your host client area rect
        m_hostwindow_size = m_hostwindow->size();
        int window_width = m_hostwindow_size.width();
        int window_height = m_hostwindow_size.height();

        //Set WPF Control Order, Size and Position
        MoveWindow(m_wpf_hwnd, 0, 0, window_width, window_height, TRUE);
        SetWindowPos(m_wpf_hwnd, HWND_TOP, 0, 0, window_width, window_height, SWP_NOMOVE);

        //Set WPF as A Child to Host Window
        SetParent(m_wpf_hwnd, m_hostwindow_hwnd);

        ShowWindow(m_wpf_hwnd, SW_RESTORE);
    }
}


