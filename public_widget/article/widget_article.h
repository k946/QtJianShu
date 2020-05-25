#ifndef WIDGET_ARTICLE_H
#define WIDGET_ARTICLE_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include "workerofarticle.h"
#include "../../../day0/Image/myimage.h"

class Widget_Article : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_Article(QWidget *parent = nullptr);
    void loadArticle(const QString& url);

private:
    void initWindow();
    void initConnect();
    void initVariable();
    void clearWidget();

signals:

protected slots:
    void sltGetResult(std::vector<std::map<QString, QString> > data);

private:
    QScrollArea* scrollArea;
    QWidget* scrollWidget;
    QVBoxLayout* scrollVLayout;
    WorkerOfArticle worker;
};

#endif // WIDGET_ARTICLE_H
