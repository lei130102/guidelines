#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <hostwindow_manager.h>
#include <widget_hostwindow.h>

hostwindow_manager manager;

typedef HWND(*create_userinterface_func)();
create_userinterface_func create_userinterface = nullptr;
typedef void(*display_userinterface_func)();
display_userinterface_func display_userinterface = nullptr;
typedef void(*destroy_userinterface_func)();
destroy_userinterface_func destroy_userinterface = nullptr;

LRESULT CALLBACK hostwindow_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_CLOSE:
        manager.set_hostwindow_is_running(false);
        destroy_userinterface();
        manager.hostwindow()->close();
        break;
    }
    return CallWindowProc(manager.hostwindow_wndproc(), hwnd, uMsg, wParam, lParam);
}

BOOST_AUTO_TEST_SUITE(s_hostwindow)

BOOST_AUTO_TEST_CASE(c_hostwindow)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    widget_hostwindow hostwindow(&manager);
    manager.set_hostwindow(&hostwindow);
    manager.set_hostwindow_hwnd((HWND)hostwindow.winId());
    manager.set_hostwindow_wndproc((WNDPROC)SetWindowLongPtr(manager.hostwindow_hwnd(), WAPI_GWL_WNDPROC, (LONG_PTR)&hostwindow_callback));

    ////
    //Loading dotNet UI Library
    HMODULE dotNetGUILibrary = LoadLibrary("Sample.dll");
    create_userinterface = (create_userinterface_func)GetProcAddress(dotNetGUILibrary, "CreateUserInterface");
    display_userinterface = (display_userinterface_func)GetProcAddress(dotNetGUILibrary, "DisplayUserInterface");
    destroy_userinterface = (destroy_userinterface_func)GetProcAddress(dotNetGUILibrary, "DestroyUserInterface");
    //creating .Net GUI
    if(create_userinterface != nullptr)
    {
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        manager.set_wpf_hwnd(create_userinterface());
        if(manager.wpf_hwnd() != nullptr)
        {
            display_userinterface();
        }
    }

    hostwindow.show();

    manager.set_hostwindow_is_running(true);
    MSG loop_message;
    while(GetMessage(&loop_message, NULL, 0, 0) > 0 && manager.hostwindow_is_running())
    {
        TranslateMessage(&loop_message);
        DispatchMessage(&loop_message);

        //Process Qt Events [Optimal]
        QApplication::processEvents();
    }

	FreeLibrary(dotNetGUILibrary);
	Sleep(500);
	exit(1);
}

BOOST_AUTO_TEST_SUITE_END()
