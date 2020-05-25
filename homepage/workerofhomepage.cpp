#include "workerofhomepage.h"
#include "global.h"

WorkerOfHomePage::WorkerOfHomePage(QObject *parent) : QObject(parent)
{
    setParent(parent);

    initConnect();
}

WorkerOfHomePage::~WorkerOfHomePage()
{

}

void WorkerOfHomePage::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread( GLOBAL::thread ); //改变子线程依附，让槽函数在子线程中执行
    decodeThread.moveToThread( GLOBAL::thread ); //改变子线程依附，让槽函数在子线程中执行
}

void WorkerOfHomePage::initConnect()
{
    //开始HTTP请求
    connect(this, sigStartHttpRequest,
            &httpThread, HttpRequest::sltHttpRequestHomePage, Qt::QueuedConnection);

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) );

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodeHomePage(std::shared_ptr<QByteArray>)));

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));

}


void WorkerOfHomePage::sltGetHttpRequestResult(std::shared_ptr<QByteArray> pData){
    emit sigStartDecode(pData);

}

void WorkerOfHomePage::sltGetDecodeResult(std::vector< std::map<QString, QString> >& result){

    emit sigSendResult(result);

    //保存文章的id，搜索时需要用到
    for(int i = 0; i < result.size(); ++i){
        id.push_back(result[i]["id"]);
    }
}

void WorkerOfHomePage::search(const QString &page)
{
    QString param = "";
    param += "page=" + page;

    //将已获取的文章的id加入参数列表，否则会重复获取文章，导致最终只能显示前7篇文章
    for(int i = 0; i < id.size() ;++i){
        param += "&seen_snote_ids%5B%5D=" + id[i];
    }

    //不同的页需要使用不同的url进行请求，但是参数是一样的
    if( page.toInt() > 3 ){
        emit sigStartHttpRequest("https://www.jianshu.com/trending_notes?", "post", param);
    }else{
        emit sigStartHttpRequest("https://www.jianshu.com?", "get", param);
    }
}



