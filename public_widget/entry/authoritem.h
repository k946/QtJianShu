#ifndef AuthorItem_H
#define AuthorItem_H

#include "widget_entry.h"
#include "labelmouseevent.h"
#include <QLabel>
#include "myimage.h"

class AuthorItem : public Widget_Entry
{
public:
    AuthorItem(QWidget* parent = 0);
    void update(std::map<QString, QString> &);

private:
    void iniWindow();
    void initVariable();
    void iniConnect();

private:
    QLabel* avatar;
    QLabel* name;
    LabelMouseEvent* following;
    LabelMouseEvent* followers;
    LabelMouseEvent* articles;
    QLabel* words;
    QLabel* likes;
    QLabel* property;

    MyImage myImage;

};

#endif // AuthorItem_H
