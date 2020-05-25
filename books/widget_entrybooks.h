#ifndef WIDGET_ENTRYBOOKS_H
#define WIDGET_ENTRYBOOKS_H

#include "widget_entry.h"
#include "labelmouseevent.h"
#include "myimage.h"

class Widget_EntryBooks : public Widget_Entry
{
public:
    Widget_EntryBooks(QWidget* parent = 0);
    virtual void update(std::map<QString, QString>& data);

private:
    void initWindow();
    void initConnect();
    void initVariable();

private:
    QLabel* image;
    LabelMouseEvent* title;
    LabelMouseEvent* nickname;
    QLabel* headImage;
    QLabel* readNumber;

    QString user_url;
    QString book_url;

    MyImage myImage1;
    MyImage myImage2;
};

#endif // WIDGET_ENTRYBOOKS_H
