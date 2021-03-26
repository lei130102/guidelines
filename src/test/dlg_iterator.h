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
    void on_pbVectorArrayStringIterator_clicked();
    void on_pbReverseIterator_clicked();
    void on_pbIteratorToReverseIterator_clicked();
    void on_pbReverseIteratorToIterator_clicked();
    void on_pbBackInsertIterator_clicked();
    void on_pbBackInsertIteratorGenerate_n_clicked();
    void on_pbBackInsertIteratorCopy_clicked();
    void on_pbFrontInsertIterator_clicked();
    void on_pbInsertIterator_clicked();
    void on_pbOstreamIterator_clicked();
    void on_pbIstreamIterator_clicked();
    void on_pbOstreamIteratorIstreamIterator_clicked();
    void on_pbMoveIterator_clicked();
    void on_pbMakeMoveIterator_clicked();
    void on_pbOfstreamOstreamIterator_clicked();
    void on_pbIfstreamIstreamIterator_clicked();
    void on_pbIfstreamIstreambufIterator_clicked();

private:
    Ui::dlg_iterator *ui;
};

#endif // DLG_ITERATOR_H
