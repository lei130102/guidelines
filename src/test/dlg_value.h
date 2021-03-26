#ifndef DLG_VALUE_H
#define DLG_VALUE_H

#include <QDialog>

namespace Ui {
class dlg_value;
}

class dlg_value : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_value(QWidget *parent = 0);
    ~dlg_value();

    void setValue(QString const& value);
    QString getValue() const;

signals:
    void shown();
    void closed();
    void destroyed();
    void valueChanged(QString const& qstr);

protected:
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    Ui::dlg_value *ui;
};

#endif // DLG_VALUE_H
