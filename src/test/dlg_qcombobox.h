#ifndef DLG_QCOMBOBOX_H
#define DLG_QCOMBOBOX_H

#include <QDialog>

namespace Ui {
class dlg_qcombobox;
}

class dlg_qcombobox : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qcombobox(QWidget *parent = 0);
    ~dlg_qcombobox();

private slots:
    void on_pbComboBoxInitQString_clicked();
    void on_pbBlockSignalInit_clicked();
    void on_pbBlockSignalFalse_clicked();
    void on_pbBlockSignalTrue_clicked();

private:
    Ui::dlg_qcombobox *ui;
};

#endif // DLG_QCOMBOBOX_H
