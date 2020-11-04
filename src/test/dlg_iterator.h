#ifndef DLG_ITERATOR_H
#define DLG_ITERATOR_H

#include <QDialog>

namespace Ui {
class dlg_iterator;
}

class dlg_iterator : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_iterator(QWidget *parent = 0);
    ~dlg_iterator();

private slots:
    void on_pbNumericRangeCopy_clicked();
    void on_pbNumericRangeAccumulate_clicked();
    void on_pbNumericRangeFor_clicked();

private:
    Ui::dlg_iterator *ui;
};

#endif // DLG_ITERATOR_H
