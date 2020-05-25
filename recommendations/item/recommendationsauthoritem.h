#ifndef AUTHORITEM_H
#define AUTHORITEM_H

#include "widget_entry.h"
#include "labelmouseevent.h"
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include "myimage.h"

class RecommendationsAuthorItem : public Widget_Entry
{
    Q_OBJECT
public:
    RecommendationsAuthorItem(QWidget* parent = 0);
    void update(std::map<QString, QString> &) override;

private:
    void initWindow();
    void initVariabel();
    void initConnect();

signals:
    void sigLoadArticle(QString );
    void sigLoadAuthor(QString );

private:
    QLabel* avatar;
    QLabel* name;
    QLabel* sex;
    QLabel* description;
    Widget_Entry* infoItem;

    LabelMouseEvent* follow;


    LabelMouseEvent* article1;
    LabelMouseEvent* article2;
    LabelMouseEvent* article3;

    MyImage image;
    QGraphicsDropShadowEffect *shadow;

};



#endif // AUTHORITEM_H
