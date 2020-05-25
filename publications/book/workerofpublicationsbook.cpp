#include "workerofpublicationsbook.h"

WorkerOfPublicationsBook::WorkerOfPublicationsBook(QObject *parent) : QObject(parent)
{
    initConnect();
}

void WorkerOfPublicationsBook::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread(t);
    decodeThread.moveToThread(t);
}

void WorkerOfPublicationsBook::initConnect()
{
    //开始HTTP请求
    connect(this, SIGNAL(sigStartHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)),
            &httpThread, SLOT(sltHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)), Qt::QueuedConnection);

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) );

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodePublcationsBook(std::shared_ptr<QByteArray>)));

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));
}

void WorkerOfPublicationsBook::sltGetHttpRequestResult(std::shared_ptr<QByteArray> content)
{
    emit sigStartDecode(content);
}

void WorkerOfPublicationsBook::sltGetDecodeResult(std::vector<std::map<QString, QString> > & result)
{

    emit sigSendResult(result);
}

void WorkerOfPublicationsBook::work()
{

    std::map<QString, QString> header;

    header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
    header["Accept"] =  "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3";
    header["Accept-Language"] =  "zh-CN,zh;q=0.9";

    emit sigStartHttpRequest("https://www.jianshu.com/publications?",
                             "get", "" ,
                             header);
}
