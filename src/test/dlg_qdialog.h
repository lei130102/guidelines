#ifndef DLG_QDIALOG_H
#define DLG_QDIALOG_H

#include <QDialog>

namespace Ui {
class dlg_qdialog;
}

class dlg_qdialog : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qdialog(QWidget *parent = 0);
    ~dlg_qdialog();


private slots:
    void on_pbDlgValueExec_clicked();
    void on_pbDlgValueExecLongTime_clicked();
    void on_pbDlgValueShow_clicked();

private:
    Ui::dlg_qdialog *ui;
};

#endif // DLG_QDIALOG_H
