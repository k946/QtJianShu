#include "httprequest.h"
#include "global.h"

HttpRequest::HttpRequest()
{
    manage = GLOBAL::http;
}



void HttpRequest::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
}


//处理HTTP响应
void HttpRequest::sltProcessFinished()
{

    QByteArray *httpRequstResult = new QByteArray();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); //HTTP应答状态码

    if(200 == statusCode){
        QByteArray data = reply->readLine();
        while(!data.isEmpty()){
            httpRequstResult->append( data );
            data = reply->readLine();
        }

        emit sigSendResult( std::make_shared<QByteArray>(*httpRequstResult) );
        delete httpRequstResult;

    }else{
        qDebug() << "status code=" << statusCode;
    }
    reply->deleteLater();
}



//一般用于请求图片
void HttpRequest::sltHttpRequest(const QString &url)
{
    //qDebug() << this->thread()->currentThreadId() <<"   HttpRequest: " << url ;
    QNetworkRequest req;        //http请求
    QSslConfiguration config;   //SSL，用于进行HTTPS请求

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);       //SSL版本必须为 QSsl::TlsV1SslV3
    req.setSslConfiguration(config);

    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);  //状态码为301时，继续跟踪请求
    req.setUrl( url );
    reply =  manage->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(sltProcessFinished()));
}


//适用于任何搜索
void HttpRequest::sltHttpRequest(const QString &url, const QString &method, const QString &p, const std::map<QString, QString> &header)
{
    QNetworkRequest req;        //http请求
    QSslConfiguration config;   //SSL，用于进行HTTPS请求

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);       //SSL版本必须为 QSsl::TlsV1SslV3
    req.setSslConfiguration(config);

    std::map<QString, QString>::const_iterator it = header.begin();
    std::map<QString, QString>::const_iterator end = header.end();

    //设置头部
    for(;it != end; ++it){
       req.setRawHeader( it->first.toStdString().data(), it->second.toStdString().data() );
    }

    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);  //状态码为301时，继续跟踪请求

    if( "post" == method){
        req.setUrl( url + p );
        //qDebug() << this->thread()->currentThreadId() << "   post: " << req.url().toString();
        reply = manage->post(req, p.toStdString().data() );
    }else{
        req.setUrl( url + p );
        //qDebug() << this->thread()->currentThreadId() << "   get: " << req.url().toString();
        reply = manage->get(req);
    }

    connect(reply, SIGNAL(finished()), this, SLOT(sltProcessFinished()));
}



//简书-搜索
void HttpRequest::sltHttpRequestSearch(QString url, QString method, QString p)
{
    QNetworkRequest req;        //http请求
    QSslConfiguration config;   //SSL，用于进行HTTPS请求

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);       //SSL版本必须为 QSsl::TlsV1SslV3
    req.setSslConfiguration(config);

    //设置头部
    req.setRawHeader("Content-Type","aplication/json");
    req.setRawHeader("Accept", "application/json");

    req.setRawHeader("Accept-Language", "zh-CN,zh;q=0.9");

    req.setRawHeader("Cookie", "__yadk_uid=PmMoZSiMufOl3kWqQIj9oR9JrqVxpXhw; _ga=GA1.2.1949481127.1583371337; __gads=ID=606e33502bd1e3dc:T=1583371335:S=ALNI_Mbwoa27nz6D72xg6qauqb0JakkDaQ; _gid=GA1.2.1777375671.1587117956; read_mode=day; default_font=font2; locale=zh-CN; _m7e_session_core=1a04342bae2e6d3117219be6e22064b0; sensorsdata2015jssdkcross=%7B%22distinct_id%22%3A%2216e45ae1c7d46c-0a3f91d5c558e-3a65420e-1247616-16e45ae1c7e1e9%22%2C%22%24device_id%22%3A%2216e45ae1c7d46c-0a3f91d5c558e-3a65420e-1247616-16e45ae1c7e1e9%22%2C%22props%22%3A%7B%22%24latest_referrer%22%3A%22https%3A%2F%2Fwww.baidu.com%2Flink%22%2C%22%24latest_traffic_source_type%22%3A%22%E8%87%AA%E7%84%B6%E6%90%9C%E7%B4%A2%E6%B5%81%E9%87%8F%22%2C%22%24latest_search_keyword%22%3A%22%E6%9C%AA%E5%8F%96%E5%88%B0%E5%80%BC%22%7D%7D; Hm_lvt_0c0e9d9b1e7d617b3e6842e85b9fb068=1587213355,1587213355,1587213355,1587213656; signin_redirect=https%3A%2F%2Fwww.jianshu.com%2Fsearch%3Fq%3Dios%26page%3D1%26type%3Dnote");

    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);  //状态码为301时，继续跟踪请求

    if( "post" == method){
        req.setUrl( url + p );
        reply = manage->post(req, p.toStdString().data() );
    }else{
        req.setUrl( url + p );
        reply = manage->get(req);
    }

   connect(reply, SIGNAL(finished()), this, SLOT(sltProcessFinished()));
}

//首页
void HttpRequest::sltHttpRequestHomePage(QString url, QString method, QString p)
{
    QNetworkRequest req;        //http请求
    QSslConfiguration config;   //SSL，用于进行HTTPS请求

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);       //SSL版本必须为 QSsl::TlsV1SslV3
    req.setSslConfiguration(config);

    //设置头部
    req.setRawHeader("Content-Type","application/x-www-form-urlencoded; charset=UTF-8");
    req.setRawHeader("Accept", "text/html, */*; q=0.01");

    req.setRawHeader("Accept-Language", "zh-CN,zh;q=0.9");

    req.setRawHeader("X-CSRF-Token", "S+Odng+Y2/3Djzk5+Z9E6T1CxSPLKioeuEPqcqP1mlfVv/h1pfsGh1Jd700iKm72YXnMklMhXFN5T6KrqEgOBw==" );
    req.setRawHeader("X-INFINITESCROLL", "true" );
    req.setRawHeader("X-Requested-With", "XMLHttpRequest" );

    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);  //状态码为301时，继续跟踪请求

    if( "post" == method){
        req.setUrl( url );
        reply = manage->post(req, p.toStdString().data());

    }else{
        req.setUrl( url + p );
        reply = manage->get(req);
    }

    connect(reply, SIGNAL(finished()), this, SLOT(sltProcessFinished()));
}


