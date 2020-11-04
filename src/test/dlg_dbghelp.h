#ifndef DLG_DBGHELP_H
#define DLG_DBGHELP_H

#include <QDialog>

#include <windows.h>
#include <DbgHelp.h>

namespace Ui {
class dlg_dbghelp;
}

class dlg_dbghelp : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_dbghelp(QWidget *parent = 0);
    ~dlg_dbghelp();

private slots:
    void on_pbUnDecorateSymbolName_clicked();

private:
    char* get_pszName() const;
    DWORD get_dwordFlag() const;

private:
    Ui::dlg_dbghelp *ui;
};

#endif // DLG_DBGHELP_H
