#include "labelmouseevent.h"
#include <QMouseEvent>
#include <QDebug>
#include <QLayout>

LabelMouseEvent::LabelMouseEvent(QWidget *parent):QLabel(parent)
{

}

LabelMouseEvent::LabelMouseEvent(const QString &title, QWidget *parent):QLabel(title, parent )
{

}

void LabelMouseEvent::setLink(const QString &link)
{
    this->link = link;
}

QString LabelMouseEvent::getLink()
{
    return link;
}

void LabelMouseEvent::mouseReleaseEvent(QMouseEvent *event)
{
    //QLabel::mouseReleaseEvent(event);
    if( event->button() == Qt::LeftButton){
        emit sigClicked();
    }
}

void LabelMouseEvent::enterEvent(QEvent * e)
{
    QLabel::enterEvent(e);
    emit sigEnter();
}

void LabelMouseEvent::leaveEvent(QEvent *e)
{
    QLabel::leaveEvent(e);
    emit sigLeave();
}



