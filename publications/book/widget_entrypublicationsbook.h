#ifndef WIDGET_ENTRYPUBLICATIONSBOOK_H
#define WIDGET_ENTRYPUBLICATIONSBOOK_H

#include "widget_entry.h"
#include <QLabel>
#include "labelmouseevent.h"
#include "myimage.h"

class Widget_EntryPublicationsBook : public Widget_Entry
{
public:
    Widget_EntryPublicationsBook(QWidget* parent = 0);
    ~Widget_EntryPublicationsBook();
    void update(std::map<QString, QString> &);

private:
    void initWindow();
    void initConnect();
    void initVariable();

private:
    QLabel* img;
    QLabel* name;
    QLabel* author;
    QLabel* intros;
    QLabel* price;

    MyImage myImage;
};

#endif // WIDGET_ENTRYPUBLICATIONSBOOK_H
