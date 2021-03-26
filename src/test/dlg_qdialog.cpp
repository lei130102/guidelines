#include "dlg_qdialog.h"
#include "ui_dlg_qdialog.h"

#include <QMessageBox>

#include <boost/locale.hpp>

#include <dlg_value.h>

dlg_qdialog::dlg_qdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qdialog)
{
    ui->setupUi(this);

    //可以通过parentWidget()获得父QWidget
    QWidget* pParentWidget = parentWidget();
}

dlg_qdialog::~dlg_qdialog()
{
    delete ui;
}

void dlg_qdialog::on_pbDlgValueExec_clicked()
{
    //new一个对话框对象，并指定父QWidget，这样他就默认定位在顶级父QWidget的上面居中位置，并且共享父QWidget的任务栏条目
    dlg_value* dlgValue = new dlg_value(this);

    Qt::WindowFlags windowFlags = dlgValue->windowFlags();
    //固定对话框大小
    windowFlags |= Qt::MSWindowsFixedSizeDialogHint;
    //去掉对话框右上角的问号，只保留关闭
    windowFlags &= ~Qt::WindowContextHelpButtonHint;
    dlgValue->setWindowFlags(windowFlags);

    //自定义对话框数据初始化
    dlgValue->setValue(ui->leValueExec->text());

    //模态方式显示对话框
    int ret = dlgValue->exec();
    if(ret == QDialog::Accepted)
    {
        ui->leValueExec->setText(dlgValue->getValue());
    }

    //delete对话框对象
    delete dlgValue;//这里会调用对话框对象的析构函数

    //注意：不能设置setAttribute(Qt::WA_DeleteOnClose)，一关闭对话框就删除对话框对象会导致无法获取对话框中的值
}

void dlg_qdialog::on_pbDlgValueExecLongTime_clicked()
{
    //一个dlg_value指针，可以是局部静态变量、全局变量、dlg_value的静态成员变量或者本单例类的非静态成员变量等等
    //具体是什么以使用方便为准
    static dlg_value* g_dlgValue = nullptr;//为了指针所指对象的生命周期

    //当指针是初始状态时，new一个对话框对象，并指定父QWidget
    if(g_dlgValue == nullptr)
    {
        g_dlgValue = new dlg_value(this);
    }

    Qt::WindowFlags windowFlags = g_dlgValue->windowFlags();
    //固定对话框大小
    windowFlags |= Qt::MSWindowsFixedSizeDialogHint;
    //去掉对话框右上角的问号，只保留关闭
    windowFlags &= ~Qt::WindowContextHelpButtonHint;
    g_dlgValue->setWindowFlags(windowFlags);

    //自定义对话框数据初始化
    g_dlgValue->setValue(ui->leValueExecLongTime->text());

    //模态方式显示对话框
    int ret = g_dlgValue->exec();
    if(ret == QDialog::Accepted)
    {
        ui->leValueExecLongTime->setText(g_dlgValue->getValue());
    }

    //不再通过delete g_dlgValue;在此时销毁，而是通过父QWidget销毁

    //注意：不能设置setAttribute(Qt::WA_DeleteOnClose)，一关闭对话框就删除对话框对象会导致无法获取对话框中的值
}

void dlg_qdialog::on_pbDlgValueShow_clicked()
{
    //防止同时创建多个dlg_value对象
    ui->pbDlgValueShow->setEnabled(false);

    //一个dlg_value指针，可以是局部静态变量、全局变量、dlg_value的静态成员变量或者本单例类的非静态成员变量等等
    //具体是什么以使用方便为准
    //注意不能使用局部非静态变量，因为当dlg_value对象销毁后，还需要通知相关类(这里的通知方式就是判断指针是否为空)
    static dlg_value* g_dlgValue = nullptr;//为了指针的生命周期

    g_dlgValue = new dlg_value(this);

    //对话框关闭时自动删除，针对关闭对话框后，不需要从对话框对象中获取值的情况，不过也不是绝对的，因为可以利用closeEvent中的信号
    g_dlgValue->setAttribute(Qt::WA_DeleteOnClose);

    Qt::WindowFlags windowFlags = g_dlgValue->windowFlags();
    //固定对话框大小
    windowFlags |= Qt::MSWindowsFixedSizeDialogHint;
    //去掉对话框右上角的问号，只保留关闭
    windowFlags &= ~Qt::WindowContextHelpButtonHint;
    //针对非模态显示对话框，将对话框显示始终在最上方
    windowFlags |= Qt::WindowStaysOnTopHint;
    g_dlgValue->setWindowFlags(windowFlags);

    //这里就不叫初始化，因为两个窗口可以同时操作
    connect(ui->leValueShow, &QLineEdit::textChanged, [](QString const& qstr){
        if(g_dlgValue != nullptr)
        {
            g_dlgValue->setValue(qstr);
        }
    });
    connect(g_dlgValue, &dlg_value::valueChanged, [this](QString const& qstr){
        ui->leValueShow->setText(qstr);
    });

    //
    connect(g_dlgValue, &dlg_value::shown, [this](){

        QMessageBox::information(this, QString(), QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("dlg_qdialog中的connect(dlgValue, &dlg_value::shown, 处理)", "gbk")), QMessageBox::Ok);

    });

    //点击“确定”，不会调用dlg_value::closeEvent(QCloseEvent* event)
    //点击“取消”，不会调用dlg_value::closeEvent(QCloseEvent* event)
    //点击“×”，会调用dlg_value::closeEvent(QCloseEvent* event)，但不会触发QDialog::finished信号
    connect(g_dlgValue, &dlg_value::closed,[this](){

        QMessageBox::information(this, QString(), QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("dlg_qdialog中的connect(dlgValue, &dlg_value::closed, 处理)", "gbk")), QMessageBox::Ok);

    });

    //点击“确定”，会触发QDialog::finished信号
    //点击“取消”，会触发QDialog::finished信号
    //点击“×”，不会触发QDialog::finished信号，但会调用dlg_value::closeEvent(QCloseEvent* event)
    connect(g_dlgValue, &QDialog::finished, [this](int result){

        QMessageBox::information(this, QString(), QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("dlg_qdialog中的connect(dlgValue, &QDialog::finished, 处理)", "gbk")), QMessageBox::Ok);

        if(result == QDialog::Accepted)
        {
            ui->leValueShow->setText(g_dlgValue->getValue());
        }
    });

    //因为dlgValue->setAttribute(Qt::WA_DeleteOnClose);
    //点击“确定”，点击“取消”，点击“×”，都会调用dlg_value的析构函数
    connect(g_dlgValue, &dlg_value::destroyed, [this](){

        QMessageBox::information(this, QString(), QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("dlg_qdialog中的connect(dlgValue, &dlg_value::destroyed, 处理)", "gbk")), QMessageBox::Ok);

        ui->pbDlgValueShow->setEnabled(true);

        g_dlgValue = nullptr;//注意！dlg_value对象被销毁必须通知相关类指向dlg_value对象的指针为nullptr
    });


    //非模态方式显示对话框
    g_dlgValue->show();//不会阻塞
}
