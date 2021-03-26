#ifndef DLG_QSPINBOX_QDOUBLESPINBOX_H
#define DLG_QSPINBOX_QDOUBLESPINBOX_H

#include <QDialog>

namespace Ui {
class dlg_qspinbox_qdoublespinbox;
}

class dlg_qspinbox_qdoublespinbox : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qspinbox_qdoublespinbox(QWidget *parent = 0);
    ~dlg_qspinbox_qdoublespinbox();

private slots:
    void on_pbSpinBox_clicked();
    void on_pbDoubleSpinBox_clicked();

private:
    Ui::dlg_qspinbox_qdoublespinbox *ui;
};

#endif // DLG_QSPINBOX_QDOUBLESPINBOX_H
