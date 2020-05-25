#ifndef HttpRequest_H
#define HttpRequest_H

/*
*   负责执行HTTP请求的线程
*/

#include <QThread>
#include <QNetworkAccessManager>    //加载网络请求头文件
#include <QNetworkReply>
#include <QNetworkRequest>      //加载发送请求头文件
#include <QSslSocket>
#include <map>
#include <QFile>
#include <memory>

class HttpRequest : public QThread
{
    Q_OBJECT

public:
    HttpRequest();
    void moveToThread(QThread*);


signals:
    void sigSendResult( std::shared_ptr< QByteArray>);

public slots:
    void sltProcessFinished();    //处理HTTP请求结果

    /*请求图片*/
    void sltHttpRequest(const QString& url);

    //所有
    void sltHttpRequest(const QString& url, const QString& method,
                        const QString& p , const std::map<QString, QString>& header);

    void sltHttpRequestSearch(QString url, QString method, QString p);   //搜索
    void sltHttpRequestHomePage(QString url, QString method, QString p); //主页


public:
    QNetworkAccessManager* manage;      //HTTP请求
    QNetworkReply *reply;
};


#endif // HttpRequest_H
