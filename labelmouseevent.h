#ifndef LABELMOUSEEVENT_H
#define LABELMOUSEEVENT_H

#include <QObject>
#include <QLabel>

class LabelMouseEvent : public QLabel
{
    Q_OBJECT
public:
    explicit LabelMouseEvent(QWidget* parent = 0);
    LabelMouseEvent(const QString& title, QWidget* parent = 0);
    void setLink(const QString& link);
    QString getLink();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);


signals:
    void sigClicked();
    void sigEnter();
    void sigLeave();
    
private:
    QString link;
    
};

#endif // LABELMOUSEEVENT_H
