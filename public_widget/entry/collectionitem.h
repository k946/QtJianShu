#ifndef WIDGET_ITEMCOLLECTION_H
#define WIDGET_ITEMCOLLECTION_H

#include <QWidget>
#include <QLabel>
#include "widget_entry.h"
#include "myimage.h"

class CollectionItem : public Widget_Entry
{
    Q_OBJECT
public:
    explicit CollectionItem(QWidget *parent = nullptr, double sizeRatio = 1);
    void update(std::map<QString, QString> &);

private:
    void initWindow();
    void initVariable();

private:
    QLabel* img;
    QLabel* title;
    QLabel* info;

    MyImage myImage;

    double sizeRatio = 1;
};

#endif // WIDGET_ITEMCOLLECTION_H
