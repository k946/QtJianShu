#ifndef WIDGET_ENTRYPUBLICATIONS_H
#define WIDGET_ENTRYPUBLICATIONS_H

#include <QWidget>
#include "widget_entry.h"
#include <QLabel>
#include "labelmouseevent.h"
#include "myimage.h"


class Widget_EntryPublicationsAuthor : public Widget_Entry
{
public:
    Widget_EntryPublicationsAuthor(QWidget* parent = 0);
    ~Widget_EntryPublicationsAuthor();
    void update(std::map<QString, QString> &);

private:
    void initWindow();
    void initConnect();
    void initVariable();

private:
    QLabel* avatar;
    QLabel* name;
    QLabel* intro;

    LabelMouseEvent* keyWork;

    LabelMouseEvent* article1;
    LabelMouseEvent* article2;
    
    QLabel* authorpage;

    MyImage myImage;
};

#endif // WIDGET_ENTRYPUBLICATIONS_H
