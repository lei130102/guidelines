using RGiesecke.DllExport;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Interop;

//1.类库里可以创建WPF窗体，虽然模板中没有

//2.修改WPF窗体XAML中的参数
//WindowStyle="None" ResizeMode="NoResize"

namespace Sample
{
    public class UIBridge
    {
        public static MainWindow mainwindow_ui;
        public static Thread gui_thread;
        public static IntPtr mainwindow_handle = IntPtr.Zero;

        [DllExport]
        public static IntPtr CreateUserInterface() //Multi-Threaded Version
        {
            gui_thread = new Thread(() =>
            {
                mainwindow_ui = new MainWindow()
                {
                    Opacity = 0,
                    Width = 0,
                    Height = 0
                };
                mainwindow_ui.Show();
                mainwindow_handle = new WindowInteropHelper(mainwindow_ui).Handle;
                System.Windows.Threading.Dispatcher.Run();
            });
            gui_thread.SetApartmentState(ApartmentState.STA);//STA Thread Initialization
            gui_thread.Start();
            while (mainwindow_handle == IntPtr.Zero) { }
            return mainwindow_handle;
        }

        [DllExport]
        static public void DisplayUserInterface()//Multi-Threaded Version
        {
            try
            {
                mainwindow_ui.Opacity = 1;
            }
            catch// Can't Access to UI Thread, So Dispatching
            {
                mainwindow_ui.Dispatcher.BeginInvoke((Action)(() =>
                {
                    mainwindow_ui.Opacity = 1;
                }));
            }
        }

        [DllExport]
        static public void DestroyUserInterface() //Multi-Thread Version
        {
            try
            {
                mainwindow_ui.Close();
            }
            catch//Can't Access to UI Thread, So Dispatching
            {
                mainwindow_ui.Dispatcher.BeginInvoke((Action)(() =>
                {
                    mainwindow_ui.Close();
                }));
            }
        }
    }
}
