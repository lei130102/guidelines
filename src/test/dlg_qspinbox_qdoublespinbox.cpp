#include "dlg_qspinbox_qdoublespinbox.h"
#include "ui_dlg_qspinbox_qdoublespinbox.h"

dlg_qspinbox_qdoublespinbox::dlg_qspinbox_qdoublespinbox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qspinbox_qdoublespinbox)
{
    ui->setupUi(this);
}

dlg_qspinbox_qdoublespinbox::~dlg_qspinbox_qdoublespinbox()
{
    delete ui;
}

void dlg_qspinbox_qdoublespinbox::on_pbSpinBox_clicked()
{
    //在使用QSpinBox和QDoubleSpinBox读取和设置数值时，无需做字符串与数值之间的转换，也无需做进制的转换，
    //其显示效果(前缀、后缀、进制和小数位数)在设置好之后就自动按照效果进行显示，这对于数值的输入输出是非常方便的

    ui->sbCodeSet->setPrefix(QString("Dec"));
    ui->sbCodeSet->setSuffix(QString("kg"));

    ui->sbCodeSet->setDisplayIntegerBase(10);//十进制 QSpinBox特有属性
    ui->sbCodeSet->setMinimum(0);
    ui->sbCodeSet->setMaximum(100);
    ui->sbCodeSet->setSingleStep(1);

    ui->sbCodeSet->setValue(4);//频繁使用

    connect(ui->sbCodeSet, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int i){
        ui->leSpinBox->setText(QString::number(i));
    });

    connect(ui->sbCodeSet, static_cast<void(QSpinBox::*)(QString const& text)>(&QSpinBox::valueChanged), [this](QString const& text){
        ui->leSpinBoxText->setText(text);
    });
}

void dlg_qspinbox_qdoublespinbox::on_pbDoubleSpinBox_clicked()
{
    ui->dsbCodeSet->setPrefix(QString("Dec"));
    ui->dsbCodeSet->setSuffix(QString("kg"));

    ui->dsbCodeSet->setDecimals(2);//小数点精度   QDoubleSpinBox特有属性
    ui->dsbCodeSet->setMinimum(0);
    ui->dsbCodeSet->setMaximum(100);
    ui->dsbCodeSet->setSingleStep(1);

    ui->dsbCodeSet->setValue(4);//频繁使用

    connect(ui->dsbCodeSet, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this](double d){
        ui->leDoubleSpinBox->setText(QString::number(d));
    });

    connect(ui->dsbCodeSet, static_cast<void(QDoubleSpinBox::*)(QString const& text)>(&QDoubleSpinBox::valueChanged), [this](QString const& text){
        ui->leDoubleSpinBoxText->setText(text);
    });
}
