#ifndef FOCUSINSELECTLINEEDIT_H
#define FOCUSINSELECTLINEEDIT_H

#include <QLineEdit>

class FocusInSelectLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    FocusInSelectLineEdit(QWidget* parent = Q_NULLPTR);

private:
    void mousePressEvent(QMouseEvent* event);
    void focusInEvent(QFocusEvent* event);

private:
    bool bfocusIn;
};

#endif // FOCUSINSELECTLINEEDIT_H
