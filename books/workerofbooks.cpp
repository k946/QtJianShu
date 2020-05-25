#include "workerofbooks.h"
#include "global.h"

WorkerOfBooks::WorkerOfBooks(QObject *parent) :
    QObject(parent)
{
    initVariable();
    initConnect();

}

void WorkerOfBooks::work(int page)
{

    QString url = "https://www.jianshu.com/asimov/books?";
    QString p = "page=" + QString::number(page) + "&count=12&category_id=284";
    std::map<QString, QString> header;

    header["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";
    header["Accept"] =  "text/html, */*; q=0.01";
    header["Accept-Language"] =  "zh-CN,zh;q=0.9";

    emit sigStartHttpRequest(url, "get", p, header);
}

void WorkerOfBooks::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread(t);
    decodeThread.moveToThread(t);
}

void WorkerOfBooks::initConnect()
{
    //开始HTTP请求
    connect(this, SIGNAL(sigStartHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)),
            &httpThread, SLOT(sltHttpRequest(const QString &, const QString &, const QString &, const std::map<QString, QString> &)), Qt::QueuedConnection);

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) );

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodeBooks(std::shared_ptr<QByteArray>)));

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));
}

void WorkerOfBooks::initVariable()
{

}

void WorkerOfBooks::sltGetHttpRequestResult(std::shared_ptr<QByteArray> content)
{
    emit sigStartDecode( content );
}

void WorkerOfBooks::sltGetDecodeResult(std::vector<std::map<QString, QString> >& result)
{
    emit sigSendResult(result);
}
