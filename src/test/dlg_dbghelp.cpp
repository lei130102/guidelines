#include "dlg_dbghelp.h"
#include "ui_dlg_dbghelp.h"

dlg_dbghelp::dlg_dbghelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_dbghelp)
{
    ui->setupUi(this);
}

dlg_dbghelp::~dlg_dbghelp()
{
    delete ui;
}

char* dlg_dbghelp::get_pszName() const
{
    return strdup(ui->cbName->currentText().toUtf8().data());
}

DWORD dlg_dbghelp::get_dwordFlag() const
{
    QString dwordFlag = ui->cbFlag->currentText();
    if(dwordFlag == QString::fromUtf8("UNDNAME_COMPLETE"))
    {
        return UNDNAME_COMPLETE;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_LEADING_UNDERSCORES"))
    {
        return UNDNAME_NO_LEADING_UNDERSCORES;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_MS_KEYWORDS"))
    {
        return UNDNAME_NO_MS_KEYWORDS;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_FUNCTION_RETURNS"))
    {
        return UNDNAME_NO_FUNCTION_RETURNS;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_ALLOCATION_MODEL"))
    {
        return UNDNAME_NO_ALLOCATION_MODEL;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_ALLOCATION_LANGUAGE"))
    {
        return UNDNAME_NO_ALLOCATION_LANGUAGE;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_MS_THISTYPE"))
    {
        return UNDNAME_NO_MS_THISTYPE;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_CV_THISTYPE"))
    {
        return UNDNAME_NO_CV_THISTYPE;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_THISTYPE"))
    {
        return UNDNAME_NO_THISTYPE;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_ACCESS_SPECIFIERS"))
    {
        return UNDNAME_NO_ACCESS_SPECIFIERS;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_THROW_SIGNATURES"))
    {
        return UNDNAME_NO_THROW_SIGNATURES;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_MEMBER_TYPE"))
    {
        return UNDNAME_NO_MEMBER_TYPE;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_RETURN_UDT_MODEL"))
    {
        return UNDNAME_NO_RETURN_UDT_MODEL;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_32_BIT_DECODE"))
    {
        return UNDNAME_32_BIT_DECODE;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NAME_ONLY"))
    {
        return UNDNAME_NAME_ONLY;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_ARGUMENTS"))
    {
        return UNDNAME_NO_ARGUMENTS;
    }
    else if(dwordFlag == QString::fromUtf8("UNDNAME_NO_SPECIAL_SYMS"))
    {
        return UNDNAME_NO_SPECIAL_SYMS;
    }
    else
    {
        return -1;
    }
}

void dlg_dbghelp::on_pbUnDecorateSymbolName_clicked()
{
    char* pszName = get_pszName();
    DWORD dwordFlag = get_dwordFlag();

    DWORD size = 0;
    if((size = UnDecorateSymbolName(pszName, nullptr, 0, dwordFlag)) == 0)
    {
        //GetLastError
        return;
    }
    char* pszOutputString = static_cast<char*>(calloc(size + 1, sizeof(char)));
    UnDecorateSymbolName(pszName, pszOutputString, size, dwordFlag);

    ui->leUnDecorateSymbolNameResult->setText(QString::fromUtf8(pszOutputString));

    free(pszOutputString);
    free(pszName);
}
