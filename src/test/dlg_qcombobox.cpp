#include "dlg_qcombobox.h"
#include "ui_dlg_qcombobox.h"

#include <QMessageBox>

#include <boost/locale.hpp>

dlg_qcombobox::dlg_qcombobox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qcombobox)
{
    ui->setupUi(this);
}

dlg_qcombobox::~dlg_qcombobox()
{
    delete ui;
}

void dlg_qcombobox::on_pbComboBoxInitQString_clicked()
{
    std::vector<std::tuple<QIcon, QString, int>> data{
        std::make_tuple(QIcon(":/images/icons/aim.ico"), QString("displayText0"), 0),
        std::make_tuple(QIcon(":/images/icons/aim.ico"), QString("displayText1"), 1),
        std::make_tuple(QIcon(":/images/icons/aim.ico"), QString("displayText2"), 2),
        std::make_tuple(QIcon(":/images/icons/aim.ico"), QString("displayText3"), 3),
        std::make_tuple(QIcon(":/images/icons/aim.ico"), QString("displayText4"), 4),
        std::make_tuple(QIcon(":/images/icons/aim.ico"), QString("displayText5"), 5)
    };

    ui->cbInitQString->clear();

    for(auto const& elem : data)
    {
        //ui->cbInitQString->addItem(std::get<0>(elem), std::get<1>(elem), std::get<2>(elem));
        //icon是std::get<0>(elem)
        //userData是std::get<2>(elem)，对应的是Qt::UserRole
        //icon和userData不是必需的



        ////或者



        ui->cbInitQString->addItem(std::get<1>(elem));
        //注意第一次调用时，ui->cbInitQString->currentIndex()从-1变为0，之后一直保持0

        int index = ui->cbInitQString->count() - 1;
        ui->cbInitQString->setItemIcon(index, std::get<0>(elem));
        ui->cbInitQString->setItemData(index, std::get<2>(elem), Qt::UserRole);
    }

    //因为两个成员函数信号名一样，所以以这种方式调用需要转函数类型
    //成员函数类型为void(QComboBox::*)(int)
    connect(ui->cbInitQString, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index){

        //int index = ui->cbInitQString->currentIndex();
        QString text = ui->cbInitQString->currentText();

        //略

    });

    ////或者

    //成员函数类型为void(QComboBox::*)(QString const&)
    connect(ui->cbInitQString, static_cast<void(QComboBox::*)(QString const&)>(&QComboBox::currentIndexChanged), [this](QString const& text){

        int index = ui->cbInitQString->currentIndex();
        //QString text = ui->cbInitQString->currentText();

        ui->leIndex->setText(QString::number(index));
        ui->leIcon->setText(ui->cbInitQString->itemIcon(index).name());
        ui->leText->setText(ui->cbInitQString->itemText(index));
        ui->leUserData->setText(ui->cbInitQString->itemData(index).toString());

    });
}

void dlg_qcombobox::on_pbBlockSignalInit_clicked()
{
    //第二次调用本函数时，因为下面的程序代码clear，导致触发QComboBox::currentIndexChanged信号，所以做好加上blockSignal

    ui->cbBlockSignal->blockSignals(true);
    ui->cbBlockSignal->clear();
    ui->cbBlockSignal->addItem("t1");
    ui->cbBlockSignal->addItem("t2");
    ui->cbBlockSignal->addItem("t3");
    ui->cbBlockSignal->blockSignals(false);

    connect(ui->cbBlockSignal, static_cast<void(QComboBox::*)(QString const&)>(&QComboBox::currentIndexChanged), [this](QString const& text){

        QMessageBox::warning(this, QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("注意", "gbk")),
                                   QString::fromStdWString(boost::locale::conv::to_utf<wchar_t>("ui->cbBlockSignal触发QComboBox::currentIndexChanged信号", "gbk")), QMessageBox::Ok);

    });
}

void dlg_qcombobox::on_pbBlockSignalFalse_clicked()
{
    int count = ui->cbBlockSignal->count();
    int index = ui->cbBlockSignal->currentIndex();
    if(++index > (count - 1))
    {
        index = 0;
    }
    ui->cbBlockSignal->setCurrentIndex(index);
}

void dlg_qcombobox::on_pbBlockSignalTrue_clicked()
{
    int count = ui->cbBlockSignal->count();
    int index = ui->cbBlockSignal->currentIndex();
    if(++index > (count - 1))
    {
        index = 0;
    }
    ui->cbBlockSignal->blockSignals(true);
    ui->cbBlockSignal->setCurrentIndex(index);
    ui->cbBlockSignal->blockSignals(false);
}

//setCurrentText函数
