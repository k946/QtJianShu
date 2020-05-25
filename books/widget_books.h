#ifndef WIDGET_BOOKS_H
#define WIDGET_BOOKS_H

#include <QPushButton>
#include <QScrollArea>
#include <QWidget>
#include "widget_entry.h"
#include "workerofbooks.h"

class Widget_Books : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_Books(QWidget *parent = nullptr);

private:
    void initConnect();
    void initVariable();
    void initWindow();

signals:


public slots:
    void sltGetResult( std::vector< std::map<QString, QString> > result);
    void sltNextpage();
    void sltPrevpage();

private:
    int page = 1;
    QPushButton* pushButton_nextpage;
    QPushButton* pushButton_prevpage;
    QScrollArea* scrollArea;
    Widget_Entry* entries[12];
    WorkerOfBooks worker;

};

#endif // WIDGET_BOOKS_H
