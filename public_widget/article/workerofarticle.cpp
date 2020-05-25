#include "workerofarticle.h"

WorkerOfArticle::WorkerOfArticle(QObject *parent) : QObject(parent)
{
    initVariable();
    initConnect();
}

void WorkerOfArticle::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread(t);
    decodeThread.moveToThread(t);
}

void WorkerOfArticle::work(const QUrl& url)
{
    std::map<QString, QString> header;

    header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
    header["Accept"] =  "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3";
    header["Accept-Language"] =  "zh-CN,zh;q=0.9";


    //header["X-CSRF-Token"] = "1VPEgvarJC2D22wCQmP+E+P1NzdCfUEecpKJGiYvL42DneDZdC77mUXiXGl5gEukQWdYHagyvB/baw805DdAQw==";
    //header["X-INFINITESCROLL"] = "true";
    //header["If-None-Match"] = "W\/\"16c7d94c4846279b581ba34806b075e0\"";
    //header["X-Requested-With"] = "XMLHttpRequest";

    emit sigStartHttpRequest(url.toString(), "get", "", header);
}

void WorkerOfArticle::initConnect()
{
    //开始HTTP请求
    connect(this, SIGNAL(sigStartHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)),
            &httpThread, SLOT(sltHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)), Qt::QueuedConnection);

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) , Qt::QueuedConnection);

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodeArticle(std::shared_ptr<QByteArray>)), Qt::QueuedConnection);

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));
}

void WorkerOfArticle::initVariable()
{

}

void WorkerOfArticle::sltGetHttpRequestResult(std::shared_ptr<QByteArray> content)
{
    emit sigStartDecode( content );
}

void WorkerOfArticle::sltGetDecodeResult(std::vector<std::map<QString, QString> >& result)
{
    emit sigSendResult(result);
}
