#ifndef DLG_QLINEEDIT_QTEXTEDIT_QPLAINTEXTEDIT_H
#define DLG_QLINEEDIT_QTEXTEDIT_QPLAINTEXTEDIT_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class dlg_qlineedit_qtextedit_qplaintextedit;
}

class dlg_qlineedit_qtextedit_qplaintextedit : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qlineedit_qtextedit_qplaintextedit(QWidget *parent = 0);
    ~dlg_qlineedit_qtextedit_qplaintextedit();

    int get_iMinimum() const;
    int get_iMaximum() const;
    double get_dBottom() const;
    double get_dTop() const;
    int get_iDecimals() const;
    int get_iMaxLength() const;
    QLineEdit::EchoMode get_echomode() const;

private slots:
    void on_pbValidatorInt_clicked();
    void on_pbValidatorDouble_clicked();
    void on_pbValidatorLength_clicked();
    void on_pbValidatorNumber_clicked();
    void on_pbValidatorInt180_clicked();
    void on_pbValidatorDouble180_clicked();
    void on_pbValidatorDouble180_4_clicked();
    void on_pbReadOnly_clicked();
    void on_pbEchoMode_clicked();
    void on_pbTWTextEdit_clicked();

private:
    Ui::dlg_qlineedit_qtextedit_qplaintextedit *ui;
};

#endif // DLG_QLINEEDIT_QTEXTEDIT_QPLAINTEXTEDIT_H
