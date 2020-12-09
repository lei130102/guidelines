#ifndef DLG_QSQLQUERY_H
#define DLG_QSQLQUERY_H

#include <QDialog>

namespace Ui {
class dlg_qsqlquery;
}

class dlg_qsqlquery : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qsqlquery(QWidget *parent = 0);
    ~dlg_qsqlquery();

    QString get_qstrType() const;
    QString get_qstrHostName() const;
    QString get_qstrDatabaseName() const;
    QString get_qstrUserName() const;
    QString get_qstrPassword() const;

private slots:
    void on_pbSelect_clicked();
    void on_pbSelectIntoContainer_clicked();
    void on_pbInsert_clicked();
    void on_pbInsertPart_clicked();
    void on_pbDelete_clicked();

private:
    Ui::dlg_qsqlquery *ui;
};

#endif // DLG_QSQLQUERY_H
