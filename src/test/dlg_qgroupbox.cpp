#include "dlg_qgroupbox.h"
#include "ui_dlg_qgroupbox.h"

#include <QVBoxLayout>
#include <QLabel>

dlg_qgroupbox::dlg_qgroupbox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qgroupbox)
{
    ui->setupUi(this);
}

dlg_qgroupbox::~dlg_qgroupbox()
{
    delete ui;
}

void dlg_qgroupbox::on_pbInsertGroupBox_clicked()
{
    QGroupBox* pGroupBox = new QGroupBox(ui->gbGlobalInsert);

    QVBoxLayout* pLayout = new QVBoxLayout;

    QLabel* pLabel1 = new QLabel(ui->gbInsert);
    pLabel1->setText("label1");
    pLayout->addWidget(pLabel1);
    QLabel* pLabel2 = new QLabel(ui->gbInsert);
    pLabel2->setText("label2");
    pLayout->addWidget(pLabel2);
    pLayout->addStretch(1);

    pGroupBox->setLayout(pLayout);

    ui->gbGlobalInsert->layout()->addWidget(pGroupBox);
}

void dlg_qgroupbox::on_pbInsertToGroupBoxRight_clicked()
{
    //QLayout* pLayoutInsert = ui->gbInsert->layout();
    //pLayoutInsert为空指针

    QVBoxLayout* pLayout = new QVBoxLayout;

    QLabel* pLabel1 = new QLabel(ui->gbInsert);
    pLabel1->setText("label1");
    pLayout->addWidget(pLabel1);
    QLabel* pLabel2 = new QLabel(ui->gbInsert);
    pLabel2->setText("label2");
    pLayout->addWidget(pLabel2);
    pLayout->addStretch(1);

    ui->gbInsert->setLayout(pLayout);
}

void dlg_qgroupbox::on_pbInsertToGroupBoxWrong_clicked()
{
//	//有问题代码，从网上找的，待删
//    QWidget* groupBoxChild = new QWidget(ui->gbInsert);

//    //设置其几何形状与父QWidget保持一致
//    QRect rectGroupBox = ui->gbInsert->geometry();
//    rectGroupBox.setX(0);
//    rectGroupBox.setY(0);
//    groupBoxChild->setGeometry(rectGroupBox);//注意，这里的rectGroupBox是相对于父QWidget而言的
//    groupBoxChild->setWindowTitle("groupBoxChild");

//    QVBoxLayout* pLayout = new QVBoxLayout;

//    QLabel* pLabel1 = new QLabel(groupBoxChild);//注意是groupBoxChild
//    pLabel1->setText("label1");
//    pLayout->addWidget(pLabel1);
//    QLabel* pLabel2 = new QLabel(groupBoxChild);//注意是groupBoxChild
//    pLabel2->setText("label2");
//    pLayout->addWidget(pLabel2);
//    pLayout->addStretch(1);

//    groupBoxChild->setLayout(pLayout);//注意是groupBoxChild

//    groupBoxChild->show();//更新布局

//    //注意groupBoxChild与QGroupBox之间没有Layout
}

//删除QLayout下的所有子
void clearLayoutAllChild(QLayout* pLayout)
{
    if(!pLayout)
    {
        return;
    }

    QLayoutItem* item = nullptr;
    while((item = pLayout->takeAt(0)) != nullptr)
    {
        QWidget* pChildWidget = item->widget();//这里可以选择性清理
        if(pChildWidget)
        {
            pChildWidget->setParent(nullptr);
        }

        QLayout* pChildLayout = item->layout();
        if(pChildLayout)
        {
            clearLayoutAllChild(pChildLayout);
        }

        delete item;
    }
}

//删除QLayout下的除了某某QWidget对象之外的子
//...

void dlg_qgroupbox::on_pbInsertWithLayout_clicked()
{
    //先删除
    QVBoxLayout* pLayoutInsertWithLayout = dynamic_cast<QVBoxLayout*>(ui->gbInsertWithLayout->layout());
    if(pLayoutInsertWithLayout == nullptr)
    {
        return;
    }

    clearLayoutAllChild(pLayoutInsertWithLayout);

    //再添加
    QLabel* pLabel1 = new QLabel(ui->gbInsertWithLayout);
    pLabel1->setText("label1");
    pLayoutInsertWithLayout->addWidget(pLabel1);
    QLabel* pLabel2 = new QLabel(ui->gbInsertWithLayout);
    pLabel2->setText("label2");
    pLayoutInsertWithLayout->addWidget(pLabel2);
    pLayoutInsertWithLayout->addStretch(1);
}
