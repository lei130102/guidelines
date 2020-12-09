#include "widget_hostwindow.h"
#include "ui_widget_hostwindow.h"

#include <hostwindow_manager.h>

#include <QStyleFactory>
#include <QResizeEvent>

widget_hostwindow::widget_hostwindow(hostwindow_manager* manager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_hostwindow),
    m_manager(manager)
{
    ui->setupUi(this);

    //Set Qt Host Window Icon
    QIcon icon(":/images/icons/qt-log.ico");
    setWindowIcon(icon);

	//去掉标题栏
	//setWindowFlags(Qt::CustomizeWindowHint); //界面上方有一个很大的边框
	setWindowFlags(Qt::FramelessWindowHint); //这个不能鼠标缩放窗口


    //Set Qt Host Window Style
    QApplication::setStyle(QStyleFactory::create("WPF_HOST"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Window, QColor(30, 30, 30));
    qApp->setPalette(p);

	isLeftPressDown = false;
	dir = NONE;
	// 追踪鼠标
	this->setMouseTracking(true);
}

widget_hostwindow::~widget_hostwindow()
{
    delete ui;
}

void widget_hostwindow::resizeEvent(QResizeEvent* evt)
{
    //Prevent White Flicking Edges
    m_manager->hostwindow()->update();
    m_manager->hostwindow()->repaint();

    //Getting New Size
    m_manager->set_hostwindow_size(evt->size());
    MoveWindow(m_manager->wpf_hwnd(), 0, 0, m_manager->hostwindow_size().width(), m_manager->hostwindow_size().height(), FALSE);
}

void widget_hostwindow::judgeRegionSetCursor(const QPoint& currentPoint)
{
	// 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());

	int x = currentPoint.x();
	int y = currentPoint.y();

	if (tl.x() + Padding >= x && tl.x() <= x && tl.y() + Padding >= y && tl.y() <= y) {
		// 左上角
		dir = LEFTTOP;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
	}
	else if (x >= rb.x() - Padding && x <= rb.x() && y >= rb.y() - Padding && y <= rb.y()) {
		// 右下角
		dir = RIGHTBOTTOM;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x <= tl.x() + Padding && x >= tl.x() && y >= rb.y() - Padding && y <= rb.y()) {
		//左下角
		dir = LEFTBOTTOM;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x <= rb.x() && x >= rb.x() - Padding && y >= tl.y() && y <= tl.y() + Padding) {
		// 右上角
		dir = RIGHTTOP;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x <= tl.x() + Padding && x >= tl.x()) {
		// 左边
		dir = LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (x <= rb.x() && x >= rb.x() - Padding) {
		// 右边
		dir = RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (y >= tl.y() && y <= tl.y() + Padding) {
		// 上边
		dir = UP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (y <= rb.y() && y >= rb.y() - Padding) {
		// 下边
		dir = DOWN;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else {
		// 默认
		dir = NONE;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}

void widget_hostwindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		isLeftPressDown = false;
		if (dir != NONE) {
			this->releaseMouse();
			this->setCursor(QCursor(Qt::ArrowCursor));
		}
	}
}
void widget_hostwindow::mousePressEvent(QMouseEvent *event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		isLeftPressDown = true;
		if (dir != NONE) {
			this->mouseGrabber();
		}
		else {
			dragPosition = event->globalPos() - this->frameGeometry().topLeft();
		}
		break;
	default:
		QWidget::mousePressEvent(event);
	}
}
void widget_hostwindow::mouseMoveEvent(QMouseEvent *event)
{
	QPoint gloPoint = event->globalPos();
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());

	if (!isLeftPressDown) {
		this->judgeRegionSetCursor(gloPoint);
	}
	else {

		if (dir != NONE) {
			QRect rMove(tl, rb);

			switch (dir) {
			case LEFT:
				if (rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				break;
			case RIGHT:
				rMove.setWidth(gloPoint.x() - tl.x());
				break;
			case UP:
				if (rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case DOWN:
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			case LEFTTOP:
				if (rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				if (rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case RIGHTTOP:
				rMove.setWidth(gloPoint.x() - tl.x());
				rMove.setY(gloPoint.y());
				break;
			case LEFTBOTTOM:
				rMove.setX(gloPoint.x());
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			case RIGHTBOTTOM:
				rMove.setWidth(gloPoint.x() - tl.x());
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			default:
				break;
			}
			this->setGeometry(rMove);
		}
		else {
			move(event->globalPos() - dragPosition);
			event->accept();
		}
	}
	QWidget::mouseMoveEvent(event);
}
