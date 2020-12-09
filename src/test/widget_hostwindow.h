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
	bool isLeftPressDown;  // �ж�����Ƿ���
	QPoint dragPosition;   // �����ƶ��϶�ʱ��Ҫ��ס�ĵ� 
						   // ���ڴ�С�ı�ʱ����¼�ı䷽��
	Direction dir;
};

#endif // WIDGET_HOSTWINDOW_H
