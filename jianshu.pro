#-------------------------------------------------
#
# Project created by QtCreator 2020-04-20T10:38:38
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jianshu
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    decode.cpp \
    httprequest.cpp \
        main.cpp \
    recommendations/item/recommendationsauthoritem.cpp \
    recommendations/widget_recommendationsauthor.cpp \
    recommendations/workerofrecommendationsauthor.cpp \
        widget.cpp \
    search/widget_search.cpp \
    jsoncpp/dist/jsoncpp.cpp \
    search/workerofsearch.cpp \
    widget_entry.cpp \
    homepage/widget_homepage.cpp \
    homepage/workerofhomepage.cpp \
    htmlcxx/html/Extensions.cc \
    htmlcxx/html/Node.cc \
    htmlcxx/html/ParserDom.cc \
    htmlcxx/html/ParserSax.cc \
    htmlcxx/html/Uri.cc \
    htmlcxx/html/utils.cc \
    ../../day0/Image/myimage.cpp \
    books/widget_books.cpp \
    books/workerofbooks.cpp \
    global.cpp \
    books/widget_entrybooks.cpp \
    labelmouseevent.cpp \
    publications/author/widget_entrypublicationsauthor.cpp \
    publications/widget_publications.cpp \
    publications/book/widget_entrypublicationsbook.cpp \
    publications/author/workerofpublicationsauthor.cpp \
    publications/book/workerofpublicationsbook.cpp \
    author/widget_authorinfo.cpp \
    author/workerofauthorinfo.cpp \
    titlebar.cpp \
    public_widget/article/widget_article.cpp \
    public_widget/article/workerofarticle.cpp \
    shcool/widget_shcool.cpp \
    public_widget/entry/triggermenuitem.cpp \
    shcool/workerofshcool.cpp \
    public_widget/entry/colletionitem.cpp \
    public_widget/entry/articleitem.cpp \
    public_widget/entry/authoritem.cpp



HEADERS += \
    decode.h \
    httprequest.h \
    recommendations/item/recommendationsauthoritem.h \
    recommendations/widget_recommendationsauthor.h \
    recommendations/workerofrecommendationsauthor.h \
        widget.h \
    search/widget_search.h \
    jsoncpp/dist/json/json.h \
    jsoncpp/dist/json/json-forwards.h \
    search/workerofsearch.h \
    widget_entry.h \
    homepage/widget_homepage.h \
    homepage/workerofhomepage.h \
    htmlcxx/html/ci_string.h \
    htmlcxx/html/debug.h \
    htmlcxx/html/Extensions.h \
    htmlcxx/html/Node.h \
    htmlcxx/html/ParserDom.h \
    htmlcxx/html/ParserSax.h \
    htmlcxx/html/tld.h \
    htmlcxx/html/tree.h \
    htmlcxx/html/Uri.h \
    htmlcxx/html/utils.h \
    htmlcxx/html/wincstring.h \
    ../../day0/Image/myimage.h \
    books/widget_books.h \
    books/workerofbooks.h \
    global.h \
    books/widget_entrybooks.h \
    labelmouseevent.h \
    publications/widget_publications.h \
    publications/author/widget_entrypublicationsauthor.h \
    publications/book/widget_entrypublicationsbook.h \
    publications/author/workerofpublicationsauthor.h \
    publications/book/workerofpublicationsbook.h \
    author/widget_authorinfo.h \
    author/workerofauthorinfo.h \
    titlebar.h \
    public_widget/article/widget_article.h \
    public_widget/article/workerofarticle.h \
    shcool/widget_shcool.h \
    public_widget/entry/triggermenuitem.h \
    shcool/workerofshcool.h \
    public_widget/entry/collectionitem.h \
    public_widget/entry/articleitem.h \
    public_widget/entry/authoritem.h



FORMS += \
        widget.ui \
    search\widget_search.ui \
    homepage\widget_homepage.ui \
    public_widget\entry\articleitem.ui



LIBS += -LD:/K/QT/blackhouse/jianshu/jianshu -lhtmlcxx

RESOURCES += \
    icon.qrc

