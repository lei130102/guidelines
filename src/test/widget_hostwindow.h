#ifndef WIDGET_HOSTWINDOW_H
#define WIDGET_HOSTWINDOW_H

#include <QWidget>

enum Direction {
	UP, DOWN, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE
};

class hostwindow_manager;

namespace Ui {
class widget_hostwindow;
}

class widget_hostwindow : public QWidget
{
    Q_OBJECT

public:
    explicit widget_hostwindow(hostwindow_manager* manager, QWidget *parent = 0);
    ~widget_hostwindow();

    void resizeEvent(QResizeEvent* evt);

protected:
	void mouseMoveEvent(QMouseEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;

private:
	void judgeRegionSetCursor(const QPoint&);


private:
    Ui::widget_hostwindow *ui;

    hostwindow_manager* m_manager;

	const int Padding = 2;
	bool isLeftPressDown;  // 判断左键是否按下
	QPoint dragPosition;   // 窗口移动拖动时需要记住的点 
						   // 窗口大小改变时，记录改变方向
	Direction dir;
};

#endif // WIDGET_HOSTWINDOW_H
