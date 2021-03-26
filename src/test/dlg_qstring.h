#ifndef DLG_QSTRING_H
#define DLG_QSTRING_H

#include <QDialog>

namespace Ui {
class dlg_qstring;
}

class dlg_qstring : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qstring(QWidget *parent = 0);
    ~dlg_qstring();

private slots:
    void on_pbAppend_clicked();
    void on_pbPrepend_clicked();
    void on_pbInsert_clicked();
    void on_pbLeft_clicked();
    void on_pbMid_clicked();
    void on_pbRight_clicked();
    void on_pbToUpper_clicked();
    void on_pbToLower_clicked();
    void on_pbSection_clicked();
    void on_pbSimplified_clicked();
    void on_pbTrimmed_clicked();
    void on_pbCount_clicked();
    void on_pbSize_clicked();
    void on_pbLength_clicked();
    void on_pbCountQstr_clicked();
    void on_pbIndexOf_clicked();
    void on_pbLastIndexOf_clicked();
    void on_pbStartsWith_clicked();
    void on_pbEndsWith_clicked();
    void on_pbContains_clicked();
    void on_pbIsEmpty_clicked();
    void on_pbIsNull_clicked();
    void on_pbSplit_clicked();
    void on_pbToInt_clicked();
    void on_pbToDouble_clicked();
    void on_pbNumber_clicked();
    void on_pbASprintf_clicked();
    void on_pbSetNum_clicked();
    void on_pbSprintf_clicked();

private:
    Ui::dlg_qstring *ui;
};

#endif // DLG_QSTRING_H
