#include "dlg_qdialog_derived_class.h"
#include "ui_dlg_qdialog_derived_class.h"

#include <QDir>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>

dlg_qdialog_derived_class::dlg_qdialog_derived_class(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qdialog_derived_class)
{
    ui->setupUi(this);
}

dlg_qdialog_derived_class::~dlg_qdialog_derived_class()
{
    delete ui;
}

QString dlg_qdialog_derived_class::get_qstrCaption() const
{
    return ui->cbCaption->currentText();
}

QString dlg_qdialog_derived_class::get_qstrDir() const
{
    QString qstrDir = ui->cbDir->currentText();
    if(qstrDir == QString::fromLocal8Bit("(QDir::currentPath())"))
    {
        qstrDir = QDir::currentPath();
    }
    return qstrDir;
}

QString dlg_qdialog_derived_class::get_qstrFilter() const
{
    return ui->cbFilter->currentText();
}

QColor dlg_qdialog_derived_class::get_qcolorInitial() const
{
    if(ui->cbColorInitial->currentText() == QString::fromLocal8Bit("(Qt::white)"))
    {
        return QColor(Qt::white);
    }
    return QColor();
}

QString dlg_qdialog_derived_class::get_qstrTitle() const
{
    return ui->cbTitle->currentText();
}

QFont dlg_qdialog_derived_class::get_qfontInitial() const
{
    if(ui->cbFontInitial->currentText() == QString::fromLocal8Bit("QFont(\"Times\", 12)"))
    {
        return QFont(QString::fromLocal8Bit("Times"), 12);
    }

    return QFont();
}

QString dlg_qdialog_derived_class::get_qstrLabel() const
{
    return ui->cbLabel->currentText();
}

QString dlg_qdialog_derived_class::get_qstrText() const
{
    return ui->cbText->currentText();
}

int dlg_qdialog_derived_class::get_iValue() const
{
    return ui->cbValue->currentText().toInt();
}

int dlg_qdialog_derived_class::get_iMin() const
{
    return ui->cbMin->currentText().toInt();
}

int dlg_qdialog_derived_class::get_iMax() const
{
    return ui->cbMax->currentText().toInt();
}

int dlg_qdialog_derived_class::get_iStep() const
{
    return ui->cbStep->currentText().toInt();
}

double dlg_qdialog_derived_class::get_dValue() const
{
    return ui->cbDoubleValue->currentText().toDouble();
}

double dlg_qdialog_derived_class::get_dMin() const
{
    return ui->cbDoubleMin->currentText().toDouble();
}

double dlg_qdialog_derived_class::get_dMax() const
{
    return ui->cbDoubleMax->currentText().toDouble();
}

int dlg_qdialog_derived_class::get_iDecimals() const
{
    return ui->cbDecimals->currentText().toInt();
}

QStringList dlg_qdialog_derived_class::get_qstringlistItems() const
{
    QStringList result;
    for(int i = 0; i < ui->lwItems->count(); ++i)
    {
        result << ui->lwItems->item(i)->text();
    }
    return result;
}

int dlg_qdialog_derived_class::get_iCurrent() const
{
    return ui->cbCurrent->currentText().toInt();
}

bool dlg_qdialog_derived_class::get_bEditable() const
{
    if(ui->cbEditable->currentText() == QString::fromLocal8Bit("true"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int dlg_qdialog_derived_class::get_iButton0() const
{
    if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::NoButton"))
    {
        return QMessageBox::NoButton;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::Ok"))
    {
        return QMessageBox::Ok;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::Cancel"))
    {
        return QMessageBox::Cancel;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::Yes"))
    {
        return QMessageBox::Yes;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::No"))
    {
        return QMessageBox::No;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::Abort"))
    {
        return QMessageBox::Abort;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::Retry"))
    {
        return QMessageBox::Retry;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::Ignore"))
    {
        return QMessageBox::Ignore;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::YesAll"))
    {
        return QMessageBox::YesAll;
    }
    else if(ui->cbButton0->currentText() == QString::fromLocal8Bit("QMessageBox::NoAll"))
    {
        return QMessageBox::NoAll;
    }
    else
    {
        return -1;
    }
}

int dlg_qdialog_derived_class::get_iButton1() const
{
    if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::NoButton"))
    {
        return QMessageBox::NoButton;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::Ok"))
    {
        return QMessageBox::Ok;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::Cancel"))
    {
        return QMessageBox::Cancel;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::Yes"))
    {
        return QMessageBox::Yes;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::No"))
    {
        return QMessageBox::No;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::Abort"))
    {
        return QMessageBox::Abort;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::Retry"))
    {
        return QMessageBox::Retry;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::Ignore"))
    {
        return QMessageBox::Ignore;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::YesAll"))
    {
        return QMessageBox::YesAll;
    }
    else if(ui->cbButton1->currentText() == QString::fromLocal8Bit("QMessageBox::NoAll"))
    {
        return QMessageBox::NoAll;
    }
    else
    {
        return -1;
    }
}

int dlg_qdialog_derived_class::get_iButton2() const
{
    if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::NoButton"))
    {
        return QMessageBox::NoButton;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::Ok"))
    {
        return QMessageBox::Ok;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::Cancel"))
    {
        return QMessageBox::Cancel;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::Yes"))
    {
        return QMessageBox::Yes;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::No"))
    {
        return QMessageBox::No;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::Abort"))
    {
        return QMessageBox::Abort;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::Retry"))
    {
        return QMessageBox::Retry;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::Ignore"))
    {
        return QMessageBox::Ignore;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::YesAll"))
    {
        return QMessageBox::YesAll;
    }
    else if(ui->cbButton2->currentText() == QString::fromLocal8Bit("QMessageBox::NoAll"))
    {
        return QMessageBox::NoAll;
    }
    else
    {
        return -1;
    }
}

QString dlg_qdialog_derived_class::get_qstrButton0Text() const
{
    return ui->cbButton0Text->currentText();
}

QString dlg_qdialog_derived_class::get_qstrButton1Text() const
{
    return ui->cbButton1Text->currentText();
}

QString dlg_qdialog_derived_class::get_qstrButton2Text() const
{
    return ui->cbButton2Text->currentText();
}

int dlg_qdialog_derived_class::get_iDefaultButtonNumber() const
{
    return ui->cbDefaultButtonNumber->currentText().toInt();
}

int dlg_qdialog_derived_class::get_iEscapeButtonNumber() const
{
    return ui->cbEscapeButtonNumber->currentText().toInt();
}

QMessageBox::StandardButtons dlg_qdialog_derived_class::get_standardbuttonButtons() const
{
    QString standardbuttonButtons = ui->cbButtons->currentText();
    if(standardbuttonButtons == QString::fromLocal8Bit("QMessageBox::Ok"))
    {
        return QMessageBox::Ok;
    }
    else if(standardbuttonButtons == QString::fromLocal8Bit("QMessageBox::Ok | QMessageBox::Cancel"))
    {
        return QMessageBox::Ok | QMessageBox::Cancel;
    }
    else if(standardbuttonButtons == QString::fromLocal8Bit("QMessageBox::Yes | QMessageBox::No"))
    {
        return QMessageBox::Yes | QMessageBox::No;
    }
    else if(standardbuttonButtons == QString::fromLocal8Bit("QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel"))
    {
        return QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
    }
    else
    {
        return QMessageBox::NoButton;
    }
}

QMessageBox::StandardButton dlg_qdialog_derived_class::get_standardbuttonDefaultButton() const
{
    if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::NoButton"))
    {
        return QMessageBox::NoButton;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::Ok"))
    {
        return QMessageBox::Ok;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::Cancel"))
    {
        return QMessageBox::Cancel;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::Yes"))
    {
        return QMessageBox::Yes;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::No"))
    {
        return QMessageBox::No;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::Abort"))
    {
        return QMessageBox::Abort;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::Retry"))
    {
        return QMessageBox::Retry;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::Ignore"))
    {
        return QMessageBox::Ignore;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::YesAll"))
    {
        return QMessageBox::YesAll;
    }
    else if(ui->cbDefaultButton->currentText() == QString::fromLocal8Bit("QMessageBox::NoAll"))
    {
        return QMessageBox::NoAll;
    }
    else
    {
        return QMessageBox::NoButton;
    }
}

void dlg_qdialog_derived_class::on_pbGetOpenFileName_clicked()
{
    QString qstrCaption = get_qstrCaption();
    QString qstrDir = get_qstrDir();
    QString qstrFilter = get_qstrFilter();

    QString result = QFileDialog::getOpenFileName(Q_NULLPTR, qstrCaption, qstrDir, qstrFilter, Q_NULLPTR, QFileDialog::Options());

    ui->leGetOpenFileNameResult->setText(result);

    //QFileDialog::getOpenFileName()函数需要传递3个字符串型参数，分别如下
    //1.对话框标题，这里设置为"选择一个文件"
    //2.初始化目录，打开对话框时的初始目录，这里用QDir::currentPath()获取应用程序当前目录
    //3.文件过滤器，设置选择不同后缀的文件，可以设置多组文件，如：
    //QString filter = "文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)";
    //每组文件之间用两个分号隔开，同一组内不同后缀之间用空格隔开
    //QFileDialog::getOpenFileName()函数返回的是选择文件的带路径的完整文件名，如果在对话框里取消选择，则返回字符串为空
}

void dlg_qdialog_derived_class::on_pbGetOpenFileNames_clicked()
{
    QString qstrCaption = get_qstrCaption();
    QString qstrDir = get_qstrDir();
    QString qstrFilter = get_qstrFilter();

    QStringList result = QFileDialog::getOpenFileNames(Q_NULLPTR, qstrCaption, qstrDir, qstrFilter, Q_NULLPTR, QFileDialog::Options());

    ui->lwGetOpenFileNamesResult->clear();
    for(auto const& aResult : result)
    {
        ui->lwGetOpenFileNamesResult->addItem(aResult);
    }
}

void dlg_qdialog_derived_class::on_pbGetExistingDirectory_clicked()
{
    QString qstrCaption = get_qstrCaption();
    QString qstrDir = get_qstrDir();

    QString result = QFileDialog::getExistingDirectory(Q_NULLPTR, qstrCaption, qstrDir, QFileDialog::ShowDirsOnly);

    ui->leGetExistingDirectoryResult->setText(result);

    //一般用QFileDialog::ShowDirsOnly，表示对话框中只显示目录
}

void dlg_qdialog_derived_class::on_pbGetSaveFileName_clicked()
{
    QString qstrCaption = get_qstrCaption();
    QString qstrDir = get_qstrDir();
    QString qstrFilter = get_qstrFilter();

    QString result = QFileDialog::getSaveFileName(Q_NULLPTR, qstrCaption, qstrDir, qstrFilter, Q_NULLPTR, QFileDialog::Options());

    ui->leGetSaveFileNameResult->setText(result);

    //选择保存文件名
    //选择一个保存文件，使用静态函数QFileDialog::getSaveFileName()，传递的参数与getOpenFileName()函数相同。只是在调用
    //getSaveFileName()函数时，若选择的是一个已经存在的文件，会提示是否覆盖原有的文件。如果提示覆盖，会返回为选择的文件，但
    //是并不会对文件进行实质操作，对文件的删除操作需要在选择文件之后自定编码实现。
    //如下面的代码，即使选择覆盖文件，由于代码里没有实质地覆盖原来的文件，也不会对选择的文件造成任何影响
}

void dlg_qdialog_derived_class::on_pbGetColor_clicked()
{
    QColor qcolorInitial = get_qcolorInitial();
    QString qstrTitle = get_qstrTitle();

    QColor result = QColorDialog::getColor(qcolorInitial, Q_NULLPTR, qstrTitle, QColorDialog::ColorDialogOptions());
    if(!result.isValid())
    {
        return;
    }

    ui->leGetColorResult->setText(result.name(QColor::HexArgb));

    QPalette palette = ui->leGetColorResult->palette();
    palette.setColor(QPalette::Background, result);
    ui->leGetColorResult->setPalette(palette);

    //若在颜色对话框里取消选择，则返回的颜色值无效，通过QColor::isValid()函数来判断返回是否有效
}

void dlg_qdialog_derived_class::on_pbGetFont_clicked()
{
    QFont qfontInitial = get_qfontInitial();
    QString qstrTitle = get_qstrTitle();

    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, qfontInitial, Q_NULLPTR, qstrTitle, QFontDialog::FontDialogOptions());
    if(ok)
    {
        ui->leGetFontResult->setText(font.toString());
        ui->leGetFontResult->setFont(font);
    }

    //getFont()返回一个字体变量，但是QFont没有类似于isValid()的函数来判断有效性，所以在调用getFont()函数时以引用方式传递一个逻辑变量
    //ok，调用后通过判断ok是否为true来判断字体选择是否有效
}

void dlg_qdialog_derived_class::on_pbGetText_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrLabel = get_qstrLabel();
    QString qstrText = get_qstrText();

    bool ok = false;
    QString result = QInputDialog::getText(this, qstrTitle, qstrLabel, QLineEdit::Normal, qstrText, &ok, Qt::WindowFlags(), Qt::ImhNone);
    if(ok && !result.isEmpty())
    {
        ui->leGetTextResult->setText(result);
    }

    //其中编辑框响应模式是枚举类型QLineEdit::EchoMode，他控制编辑框上文字的显示方式，正常情况下选择QLineEdit::Normal；如果是输入
    //密码，选择QLineEdit::Password
}

void dlg_qdialog_derived_class::on_pbGetInt_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrLabel = get_qstrLabel();
    int iValue = get_iValue();
    int iMin = get_iMin();
    int iMax = get_iMax();
    int iStep = get_iStep();

    bool ok = false;
    int result = QInputDialog::getInt(this, qstrTitle, qstrLabel, iValue, iMin, iMax, iStep, &ok, Qt::WindowFlags());
    if(ok)
    {
        ui->leGetIntResult->setText(QString::number(result));
    }
}

void dlg_qdialog_derived_class::on_pbGetDouble_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrLabel = get_qstrLabel();
    double dValue = get_dValue();
    double dMin = get_dMin();
    double dMax = get_dMax();
    int iDecimals = get_iDecimals();//小数点位数

    bool ok = false;
    double result = QInputDialog::getDouble(this, qstrTitle, qstrLabel, dValue, dMin, dMax, iDecimals, &ok, Qt::WindowFlags());
    if(ok)
    {
        ui->leGetDoubleResult->setText(QString::number(result));
    }
}

void dlg_qdialog_derived_class::on_pbGetItem_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrLabel = get_qstrLabel();
    QStringList qstringlistItems = get_qstringlistItems();
    int iCurrent = get_iCurrent();
    bool bEditable = get_bEditable();

    bool ok;
    QString result = QInputDialog::getItem(this, qstrTitle, qstrLabel, qstringlistItems, iCurrent, bEditable, &ok, Qt::WindowFlags(), Qt::ImhNone);
    if(ok)
    {
        ui->leGetItemResult->setText(result);
    }

}

void dlg_qdialog_derived_class::on_pbInformation_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrText = get_qstrText();
    int iButton0 = get_iButton0();
    int iButton1 = get_iButton1();
    int iButton2 = get_iButton2();

    int result = QMessageBox::information(this, qstrTitle, qstrText, iButton0, iButton1, iButton2);

    if(result == QMessageBox::Ok)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::Ok"));
    }
    else if(result == QMessageBox::Cancel)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::Cancel"));
    }
    else if(result == QMessageBox::Yes)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::Yes"));
    }
    else if(result == QMessageBox::No)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::No"));
    }
    else if(result == QMessageBox::Abort)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::Abort"));
    }
    else if(result == QMessageBox::Retry)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::Retry"));
    }
    else if(result == QMessageBox::Ignore)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::Ignore"));
    }
    else if(result == QMessageBox::YesAll)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::YesAll"));
    }
    else if(result == QMessageBox::NoAll)
    {
        ui->leInformationResult->setText(QString::fromLocal8Bit("QMessageBox::NoAll"));
    }
}

void dlg_qdialog_derived_class::on_pbInformationQString_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrText = get_qstrText();
    QString qstrButton0Text = get_qstrButton0Text();
    QString qstrButton1Text = get_qstrButton1Text();
    QString qstrButton2Text = get_qstrButton2Text();
    int iDefaultButtonNumber = get_iDefaultButtonNumber();
    int iEscapeButtonNumber = get_iEscapeButtonNumber();

    int result = QMessageBox::information(this, qstrTitle, qstrText, qstrButton0Text, qstrButton1Text, qstrButton2Text, iDefaultButtonNumber, iEscapeButtonNumber);

    if(result == QMessageBox::Ok)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::Ok"));
    }
    else if(result == QMessageBox::Cancel)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::Cancel"));
    }
    else if(result == QMessageBox::Yes)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::Yes"));
    }
    else if(result == QMessageBox::No)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::No"));
    }
    else if(result == QMessageBox::Abort)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::Abort"));
    }
    else if(result == QMessageBox::Retry)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::Retry"));
    }
    else if(result == QMessageBox::Ignore)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::Ignore"));
    }
    else if(result == QMessageBox::YesAll)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::YesAll"));
    }
    else if(result == QMessageBox::NoAll)
    {
        ui->leInformationQStringResult->setText(QString::fromLocal8Bit("QMessageBox::NoAll"));
    }
}

void dlg_qdialog_derived_class::on_pbInformationStandardButton_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrText = get_qstrText();
    QMessageBox::StandardButtons standardbuttonButtons = get_standardbuttonButtons();
    QMessageBox::StandardButton standardbuttonDefaultButton = get_standardbuttonDefaultButton();

    QMessageBox::StandardButton result =  QMessageBox::information(this, qstrTitle, qstrText, standardbuttonButtons, standardbuttonDefaultButton);

    if(result == QMessageBox::Ok)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::Ok"));
    }
    else if(result == QMessageBox::Cancel)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::Cancel"));
    }
    else if(result == QMessageBox::Yes)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::Yes"));
    }
    else if(result == QMessageBox::No)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::No"));
    }
    else if(result == QMessageBox::Abort)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::Abort"));
    }
    else if(result == QMessageBox::Retry)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::Retry"));
    }
    else if(result == QMessageBox::Ignore)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::Ignore"));
    }
    else if(result == QMessageBox::YesAll)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::YesAll"));
    }
    else if(result == QMessageBox::NoAll)
    {
        ui->leInformationStandardButtonResult->setText(QString::fromLocal8Bit("QMessageBox::NoAll"));
    }
}

void dlg_qdialog_derived_class::on_pbAbout_clicked()
{
    QString qstrTitle = get_qstrTitle();
    QString qstrText = get_qstrText();

    QMessageBox::about(this, qstrTitle, qstrText);
}

void dlg_qdialog_derived_class::on_pbAboutQt_clicked()
{
    QString qstrTitle = get_qstrTitle();

    QMessageBox::aboutQt(this, qstrTitle);
}
