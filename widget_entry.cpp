#include "widget_entry.h"
#include <QLabel>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QThread>

Widget_Entry::Widget_Entry(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
}

void Widget_Entry::setLink(const QString & link)
{
    this->link = link;
}

QString Widget_Entry::getLink()
{
    return link;
}

bool Widget_Entry::event(QEvent *event)
{
    QWidget::event(event);
}

void Widget_Entry::mouseReleaseEvent(QMouseEvent * event)
{
    if( event->button() == Qt::LeftButton){
        emit sigClicked();
    }
}

void Widget_Entry::enterEvent(QEvent *)
{
    emit sigEnter();
}

void Widget_Entry::leaveEvent(QEvent *)
{
    emit sigLeave();
}


