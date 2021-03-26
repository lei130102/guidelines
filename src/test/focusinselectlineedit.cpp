#include "focusinselectlineedit.h"

FocusInSelectLineEdit::FocusInSelectLineEdit(QWidget* parent)
    :QLineEdit(parent),bfocusIn(false)
{

}

void FocusInSelectLineEdit::mousePressEvent(QMouseEvent* event)
{
    if(bfocusIn)
    {
        //...

        bfocusIn = false;//重置
    }
    else
    {
        QLineEdit::mousePressEvent(event);//防止冲掉FocusInSelectLineEdit::focusInEvent中的设置

        //...
    }
}

void FocusInSelectLineEdit::focusInEvent(QFocusEvent* event)
{
    bfocusIn = true;

    QLineEdit::focusInEvent(event);

    QString qstr = text();
    setSelection(0, qstr.length());
}
