#ifndef WIDGET_ENTRYHOMEPAGE_H
#define WIDGET_ENTRYHOMEPAGE_H

#include <QWidget>
#include "widget_entry.h"
#include <QThread>
#include "labelmouseevent.h"
#include "../../../day0/Image/myimage.h"
#include <QHBoxLayout>

namespace Ui {
class ArticleItem;
}

class ArticleItem : public Widget_Entry
{
    Q_OBJECT

public:
    explicit ArticleItem(QWidget *parent = 0);
    ~ArticleItem();
    virtual void update(std::map<QString, QString>& data);

private:
    void initWindow();
    void initVariable();
    void initConnect();

signals:
    void sigLoadArticle(QString url);
    void sigLoadAuthor(QString url);

protected slots:
    void sltClickedTitle();
    void sltClickedAuthor();

private:
    Ui::ArticleItem *ui;
    LabelMouseEvent* nickname = 0;
    QLabel* clicks = 0;
    QLabel* time = 0;
    LabelMouseEvent* comments = 0;
    QLabel* likes = 0;
    QLabel* diamonds = 0;
    QLabel* money = 0;
    QLabel* paid = 0;

    QHBoxLayout* bottomHLayout;
    MyImage image;

};

#endif // WIDGET_ENTRYHOMEPAGE_H
