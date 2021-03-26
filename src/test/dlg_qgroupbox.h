#ifndef DLG_QGROUPBOX_H
#define DLG_QGROUPBOX_H

#include <QDialog>

namespace Ui {
class dlg_qgroupbox;
}

class dlg_qgroupbox : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qgroupbox(QWidget *parent = 0);
    ~dlg_qgroupbox();

private slots:
    void on_pbInsertGroupBox_clicked();
    void on_pbInsertToGroupBoxRight_clicked();
    void on_pbInsertToGroupBoxWrong_clicked();
    void on_pbInsertWithLayout_clicked();

private:
    Ui::dlg_qgroupbox *ui;
};

#endif // DLG_QGROUPBOX_H
