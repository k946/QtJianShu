#ifndef WIDGET_PUBLICATIONS_H
#define WIDGET_PUBLICATIONS_H

#include <QWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include "author/widget_entrypublicationsauthor.h"
#include "book/widget_entrypublicationsbook.h"
#include "labelmouseevent.h"
#include "author/workerofpublicationsauthor.h"
#include "book/workerofpublicationsbook.h"
#include "author/widget_authorinfo.h"

class Widget_Publications : public QWidget
{
    Q_OBJECT
public:
    explicit Widget_Publications(QWidget *parent = nullptr);

private:
    void initConnect();
    void initVariable();
    void initWindow();
    void initWindow(int novel, int it, int career, int culture, int skill, int collection);

signals:
    void sigLoadAuthorInfo(const QUrl& url);

public slots:
    void sltGetResultAuthor( std::vector< std::map<QString, QString> > result);
    void sltGetResultBook( std::vector< std::map<QString, QString> > result);

    void sltOnClickedAuthor();

private:
    QStackedWidget* stackWidget;
    QScrollArea* scrollArea;    //滑动窗口
    Widget_AuthorInfo* authorInfoWidget = 0;

    //推荐作者
    LabelMouseEvent* page_change;
    LabelMouseEvent* authorsBlockLabel;
    QWidget* authorsBlockWidget;
    Widget_EntryPublicationsAuthor* authors[4];

    //小说、故事
    LabelMouseEvent* novelBlockLabel;
    QWidget* novelBlockWidget;

    //互联网、科普
    LabelMouseEvent* itBlockLabel;
    QWidget* itBlockWidget;

    LabelMouseEvent* careerBlockLabel;
    QWidget* careerBlockWidget;

    LabelMouseEvent* cultureBlockLabel;
    QWidget* cultureBlockWidget;

    LabelMouseEvent* skillBlockLabel;
    QWidget* skillBlockWidget;

    LabelMouseEvent* collectionBlockLabel;
    QWidget* collectionBlockWidget;

    std::vector< Widget_Entry* > books;

    WorkerOfPublicationsAuthor authorWorker;
    WorkerOfPublicationsBook bookWorker;

    int authorsPage = 1;
    bool finishInit = false;
};

#endif // WIDGET_PUBLICATIONS_H
