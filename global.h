#ifndef GLOBAL_H
#define GLOBAL_H

#include <QThread>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QNetworkAccessManager>

class TitleBar;
#include "titlebar.h"

class GLOBAL
{
public:
    static TitleBar* titleBar;
    static QThread* thread;
    static QNetworkAccessManager*  http;


    //设置图标
    static void setIcon(const QChar &id, QWidget *w, const QString &color, int size, Qt::Alignment align = Qt::AlignLeft, int position = 0)
    {
        //加载图标库
        static int fontID = QFontDatabase::addApplicationFont(":/icon/C:/Users/user/Desktop/fontawesome-free-5.13.0-web/webfonts/fa-solid-900.ttf");
        static QString fontName = QFontDatabase::applicationFontFamilies(fontID).at(0);
        static QFont font = QFont(fontName);
        font.setPointSize(size);   //图标大小

        QLabel* icon = new QLabel(w);
        icon->setObjectName("icon");
        icon->setFont( font );
        icon->setText( id );    //要使用的图标的编码
        icon->setAlignment(align);


        if(align == Qt::AlignLeft || align == Qt::AlignHCenter || align == Qt::AlignRight ){
            if(position > 0){
                //水平方向改变距离
                icon->setContentsMargins(position, 0, 0, 0);
            }else{
                icon->setContentsMargins(0, 0, -position, 0);
            }

        }else{
            if(position > 0){
                //垂直方向改变距离
                icon->setContentsMargins(0, position, 0, 0);
            }else{
                icon->setContentsMargins(0, 0, 0, -position);
            }
        }

        icon->setStyleSheet("QLabel#icon{background:transparent;color:" + color + ";}");   //设置图标颜色

        QHBoxLayout* h = new QHBoxLayout(w);
        h->insertWidget( 0, icon );
        h->setMargin(0);

    }

    //修改图标颜色
    static void changeIconColor(QWidget *w,const QString&color){

        for(int i = 0; i < w->children().size(); ++i){
            if(w->children().at(i)->objectName() == "icon"){
                ((QLabel*)(w->children().at(i)))->setStyleSheet("QLabel#icon{background:transparent;color:" + color + ";}");
                break;
            }
        }
    }

    /*鼠标位于图标上方时的颜色*/
    static void hoverIconColor(QWidget *w,const QString&color){

        for(int i = 0; i < w->children().size(); ++i){
            if(w->children().at(i)->objectName() == "icon"){
                QLabel* icon = (QLabel*)(w->children().at(i));
                QString sty = icon->styleSheet() + "QLabel#icon:hover{ background:transparent;color:" + color + ";}";
                icon->setStyleSheet(sty);
                break;
            }
        }
    }
};

#endif // GLOBAL_H
