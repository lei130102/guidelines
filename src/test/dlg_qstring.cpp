#include "dlg_qstring.h"
#include "ui_dlg_qstring.h"

#include <QString>
#include <QStringList>
#include <QListWidget>
#include <QObject>

dlg_qstring::dlg_qstring(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_qstring)
{
    ui->setupUi(this);
}

dlg_qstring::~dlg_qstring()
{
    delete ui;
}

void dlg_qstring::on_pbAppend_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstring2 = ui->cbQString2->currentText();

    QString result = qstring1.append(qstring2);

    ui->leAppendResult->setText(result);
}

void dlg_qstring::on_pbPrepend_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstring2 = ui->cbQString2->currentText();

    QString result = qstring1.prepend(qstring2);

    ui->lePrependResult->setText(result);
}

void dlg_qstring::on_pbInsert_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstring2 = ui->cbQString2->currentText();
    int iPosition = ui->cbiPosition->currentText().toInt();

    QString result = qstring1.insert(iPosition, qstring2);

    ui->leInsertResult->setText(result);
}

void dlg_qstring::on_pbLeft_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    int iN = ui->cbiN->currentText().toInt();

    QString result = qstring1.left(iN);

    ui->leLeftResult->setText(result);
}

void dlg_qstring::on_pbMid_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    int iPosition = ui->cbiPosition->currentText().toInt();
    int iN = ui->cbiN->currentText().toInt();

    QString result = qstring1.mid(iPosition, iN);

    ui->leMidResult->setText(result);
}

void dlg_qstring::on_pbRight_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    int iN = ui->cbiN->currentText().toInt();

    QString result = qstring1.right(iN);

    ui->leRightResult->setText(result);
}

void dlg_qstring::on_pbToUpper_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    QString result = qstring1.toUpper();

    ui->leToUpperResult->setText(result);
}

void dlg_qstring::on_pbToLower_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    QString result = qstring1.toLower();

    ui->leToLowerResult->setText(result);
}

void dlg_qstring::on_pbSection_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstrSep = ui->cbqstrSep->currentText();

    int iStart = ui->cbiStart->currentText().toInt();
    int iEnd = ui->cbiEnd->currentText().toInt();

    QString::SectionFlags sectionFlag;
    if(ui->cbsectionFlag->currentText() == "QString::SectionDefault")
    {
        sectionFlag = QString::SectionDefault;
    }
    else if(ui->cbsectionFlag->currentText() == "QString::SectionSkipEmpty")
    {
        sectionFlag = QString::SectionSkipEmpty;
    }
    else if(ui->cbsectionFlag->currentText() == "QString::SectionIncludeLeadingSep")
    {
        sectionFlag = QString::SectionIncludeLeadingSep;
    }
    else if(ui->cbsectionFlag->currentText() == "QString::SectionIncludeTrailingSep")
    {
        sectionFlag = QString::SectionIncludeTrailingSep;
    }
    else if(ui->cbsectionFlag->currentText() == "QString::SectionCaseInsensitiveSeps")
    {
        sectionFlag = QString::SectionCaseInsensitiveSeps;
    }

    QString result = qstring1.section(qstrSep, iStart, iEnd, sectionFlag);

    ui->leSectionResult->setText(result);
}

void dlg_qstring::on_pbSimplified_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    QString result = qstring1.simplified();

    ui->leSimplifiedResult->setText(result);
}

void dlg_qstring::on_pbTrimmed_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    QString result = qstring1.trimmed();

    ui->leTrimmedResult->setText(result);
}

void dlg_qstring::on_pbCount_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    int result = qstring1.count();

    ui->leCountResult->setText(QString::number(result));
}

void dlg_qstring::on_pbSize_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    int result = qstring1.size();

    ui->leSizeResult->setText(QString::number(result));
}

void dlg_qstring::on_pbLength_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    int result = qstring1.length();

    ui->leLengthResult->setText(QString::number(result));
}

void dlg_qstring::on_pbCountQstr_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    QString qstring2 = ui->cbQString2->currentText();

    Qt::CaseSensitivity caseSensitivity;
    if(ui->cbcaseSensitivity->currentText() == "Qt::CaseSensitive")
    {
        caseSensitivity = Qt::CaseSensitive;
    }
    else if(ui->cbcaseSensitivity->currentText() == "Qt::CaseInsensitive")
    {
        caseSensitivity = Qt::CaseInsensitive;
    }

    int result = qstring1.count(qstring2, caseSensitivity);

    ui->leCountQstrResult->setText(QString::number(result));
}

void dlg_qstring::on_pbIndexOf_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    QString qstring2 = ui->cbQString2->currentText();

    int iFrom = ui->cbiFrom->currentText().toInt();

    Qt::CaseSensitivity caseSensitivity;
    if(ui->cbcaseSensitivity->currentText() == "Qt::CaseSensitive")
    {
        caseSensitivity = Qt::CaseSensitive;
    }
    else if(ui->cbcaseSensitivity->currentText() == "Qt::CaseInsensitive")
    {
        caseSensitivity = Qt::CaseInsensitive;
    }

    int result = qstring1.indexOf(qstring2, iFrom, caseSensitivity);

    ui->leIndexOfResult->setText(QString::number(result));
}

void dlg_qstring::on_pbLastIndexOf_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    QString qstring2 = ui->cbQString2->currentText();

    int iLastFrom = ui->cbiLastFrom->currentText().toInt();

    Qt::CaseSensitivity caseSensitivity;
    if(ui->cbcaseSensitivity->currentText() == "Qt::CaseSensitive")
    {
        caseSensitivity = Qt::CaseSensitive;
    }
    else if(ui->cbcaseSensitivity->currentText() == "Qt::CaseInsensitive")
    {
        caseSensitivity = Qt::CaseInsensitive;
    }

    int result = qstring1.lastIndexOf(qstring2, iLastFrom, caseSensitivity);

    ui->leLastIndexOfResult->setText(QString::number(result));
}

void dlg_qstring::on_pbStartsWith_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstring2 = ui->cbQString2->currentText();

    bool result = qstring1.startsWith(qstring2);

    ui->leStartsWithResult->setText(result?"true":"false");
}

void dlg_qstring::on_pbEndsWith_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstring2 = ui->cbQString2->currentText();

    bool result = qstring1.endsWith(qstring2);

    ui->leEndsWithResult->setText(result?"true":"false");

}

void dlg_qstring::on_pbContains_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstring2 = ui->cbQString2->currentText();

    Qt::CaseSensitivity caseSensitivity;
    if(ui->cbcaseSensitivity->currentText() == "Qt::CaseSensitive")
    {
        caseSensitivity = Qt::CaseSensitive;
    }
    else if(ui->cbcaseSensitivity->currentText() == "Qt::CaseInsensitive")
    {
        caseSensitivity = Qt::CaseInsensitive;
    }

    bool result = qstring1.contains(qstring2, caseSensitivity);

    ui->leContainsResult->setText(result?"true":"false");
}

void dlg_qstring::on_pbIsEmpty_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    bool result = qstring1.isEmpty();

    ui->leIsEmptyResult->setText(result?"true":"false");
}

void dlg_qstring::on_pbIsNull_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();

    bool result = qstring1.isNull();

    ui->leIsNullResult->setText(result?"true":"false");
}

void dlg_qstring::on_pbSplit_clicked()
{
    QString qstring1 = ui->cbQString1->currentText();
    QString qstrSep = ui->cbqstrSep->currentText();

    QString::SplitBehavior splitBehavior;
    if(ui->cbsplitBehavior->currentText() == "QString::KeepEmptyParts")
    {
        splitBehavior = QString::KeepEmptyParts;
    }
    else if(ui->cbsplitBehavior->currentText() == "QString::SkipEmptyParts")
    {
        splitBehavior = QString::SkipEmptyParts;
    }

    Qt::CaseSensitivity caseSensitivity;
    if(ui->cbcaseSensitivity->currentText() == "Qt::CaseSensitive")
    {
        caseSensitivity = Qt::CaseSensitive;
    }
    else if(ui->cbcaseSensitivity->currentText() == "Qt::CaseInsensitive")
    {
        caseSensitivity = Qt::CaseInsensitive;
    }

    QStringList result = qstring1.split(qstrSep, splitBehavior, caseSensitivity);

    ui->lwSplitResult->clear();
    for(auto iter = result.begin(); iter != result.end(); ++iter)
    {
        ui->lwSplitResult->addItem(*iter);
    }
}







