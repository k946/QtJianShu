#ifndef WIDGET_ENTRY_H
#define WIDGET_ENTRY_H

#include <QObject>
#include <QWidget>
#include <map>
#include <QFont>

class Widget_Entry : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_Entry(QWidget *parent = nullptr);
    virtual void update(std::map<QString, QString>& ){}
    void setLink(const QString&);
    QString getLink();

    bool event(QEvent *event);
    void mouseReleaseEvent(QMouseEvent* );
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

signals:
    void sigClicked();
    void sigEnter();
    void sigLeave();

private:
    QString link;
};

#endif // WIDGET_ENTRY_H
