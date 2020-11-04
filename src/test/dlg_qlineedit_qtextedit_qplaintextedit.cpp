#include "dlg_qlineedit_qtextedit_qplaintextedit.h"
#include "ui_dlg_qlineedit_qtextedit_qplaintextedit.h"

#include <QIntValidator>

#include <QDoubleValidator>

#include <QRegExp>
#include <QRegExpValidator>

#include <QScrollBar>

dlg_qlineedit_qtextedit_qplaintextedit::dlg_qlineedit_qtextedit_qplaintextedit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qlineedit_qtextedit_qplaintextedit)
{
    ui->setupUi(this);

}

dlg_qlineedit_qtextedit_qplaintextedit::~dlg_qlineedit_qtextedit_qplaintextedit()
{
    delete ui;
}

int dlg_qlineedit_qtextedit_qplaintextedit::get_iMinimum() const
{
    return ui->cbMinimum->currentText().toInt();
}

int dlg_qlineedit_qtextedit_qplaintextedit::get_iMaximum() const
{
    return ui->cbMaximum->currentText().toInt();
}

double dlg_qlineedit_qtextedit_qplaintextedit::get_dBottom() const
{
    return ui->cbBottom->currentText().toDouble();
}

double dlg_qlineedit_qtextedit_qplaintextedit::get_dTop() const
{
    return ui->cbTop->currentText().toDouble();
}

int dlg_qlineedit_qtextedit_qplaintextedit::get_iDecimals() const
{
    return ui->cbDecimals->currentText().toInt();
}

int dlg_qlineedit_qtextedit_qplaintextedit::get_iMaxLength() const
{
    return ui->cbMaxLength->currentText().toInt();
}

QLineEdit::EchoMode dlg_qlineedit_qtextedit_qplaintextedit::get_echomode() const
{
    QString echomode = ui->cbEchoMode->currentText();
    if(echomode == "QLineEdit::Normal")
    {
        return QLineEdit::Normal;
    }
    else if(echomode == "QLineEdit::NoEcho")
    {
        return QLineEdit::NoEcho;
    }
    else if(echomode == "QLineEdit::Password")
    {
        return QLineEdit::Password;
    }
    else if(echomode == "QLineEdit::PasswordEchoOnEdit")
    {
        return QLineEdit::PasswordEchoOnEdit;
    }
    else
    {
        return QLineEdit::Normal;
    }
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbValidatorInt_clicked()
{
    int iMinimum = get_iMinimum();
    int iMaximum = get_iMaximum();

    ui->leValidatorInt->setValidator(new QIntValidator(iMinimum, iMaximum, this));
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbValidatorDouble_clicked()
{
    double dBottom = get_dBottom();
    double dTop = get_dTop();
    int iDecimals = get_iDecimals();

    ui->leValidatorDouble->setValidator(new QDoubleValidator(dBottom, dTop, iDecimals, this));
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbValidatorLength_clicked()
{
    int iMaxLength = get_iMaxLength();
    ui->leValidatorLength->setMaxLength(iMaxLength);
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbValidatorNumber_clicked()
{
    QRegExp re("^[0-9]+$");
    QValidator* validator = new QRegExpValidator(re, ui->leValidatorNumber);
    ui->leValidatorNumber->setValidator(validator);
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbValidatorInt180_clicked()
{
    QRegExp re("(^-?180$)|(^-?1[0-7]\\d$)|(^-?[1-9]\\d$)|(^-?[1-9]$)|^0$");
    QValidator* validator = new QRegExpValidator(re, ui->leValidatorInt180);
    ui->leValidatorInt180->setValidator(validator);
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbValidatorDouble180_clicked()
{
    QRegExp re("^-?(180|1?[0-7]?\\d(\\.\\d+)?)$");
    QValidator* validator = new QRegExpValidator(re, ui->leValidatorDouble180);
    ui->leValidatorDouble180->setValidator(validator);
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbValidatorDouble180_4_clicked()
{
    QRegExp re("^-?(180|1?[0-7]?\\d(\\.\\d{1,4})?)$");
    QValidator* validator= new QRegExpValidator(re, ui->leValidatorDouble180_4);
    ui->leValidatorDouble180_4->setValidator(validator);
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbReadOnly_clicked()
{
    ui->leReadOnly->setText("read only");
    ui->leReadOnly->setReadOnly(true);
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbEchoMode_clicked()
{
    QLineEdit::EchoMode echomode = get_echomode();
    ui->leEchoMode->setEchoMode(echomode);
}

void dlg_qlineedit_qtextedit_qplaintextedit::on_pbTWTextEdit_clicked()
{
    ui->twTextEdit->setRowCount(2);
    ui->twTextEdit->setColumnCount(2);

    QTextEdit* pTextEdit = new QTextEdit;//用于setCellWidget，所以不需要指定父QWidget
    pTextEdit->setReadOnly(true);
    pTextEdit->setText("qwertyuiopasdfghjklzxcvbnm1234567890");

    //固定宽度
    pTextEdit->setFixedWidth(100);

    //文本高度
    QTextDocument* pTextDocument = pTextEdit->document();
    pTextDocument->adjustSize();
    int height = pTextDocument->size().height();//注意如果是以setStyleSheet方式设置的字体，那么获取的高度会偏小

    //隐藏滚动条
    pTextEdit->verticalScrollBar()->hide();

    //取消边框
    pTextEdit->setStyleSheet("QTextEdit{border:none;}");

    ui->twTextEdit->setCellWidget(0, 0, pTextEdit);
    ui->twTextEdit->setRowHeight(0, height * 1.3);//1.3为了有个余量
}

