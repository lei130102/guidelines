#include "dlg_value.h"
#include "ui_dlg_value.h"

#include <QMessageBox>
#include <QCloseEvent>

#include <boost/locale.hpp>

dlg_value::dlg_value(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_value)
{
    ui->setupUi(this);

    connect(ui->leValue, &QLineEdit::textChanged, this, &dlg_value::valueChanged);
}

dlg_value::~dlg_value()
{
    QMessageBox::information(this, QString(), QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("调用dlgvalue::~dlgvalue()", "gbk")), QMessageBox::Ok);

    emit destroyed();

    delete ui;
}

void dlg_value::setValue(QString const& value)
{
    ui->leValue->setText(value);
}

QString dlg_value::getValue() const
{
    return ui->leValue->text();
}

void dlg_value::showEvent(QShowEvent* event)
{
    QMessageBox::information(this, QString(), QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("调用dlg_value::showEvent(QShowEvent* event)", "gbk")), QMessageBox::Ok);

    emit shown();
}

void dlg_value::closeEvent(QCloseEvent* event)
{
    QMessageBox::information(this, QString(), QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("调用dlg_value::closeEvent(QCloseEvent* event)", "gbk")), QMessageBox::Ok);

    //可以在这里根据情况组织，单击“×”关闭对话框
    if(false)
    {
        event->ignore();
    }
    else
    {
        event->accept();

        emit closed();
    }
}
