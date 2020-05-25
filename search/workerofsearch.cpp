#include "workerofsearch.h"
#include <QMetaType>
#include <global.h>

typedef std::map<std::string,std::string> MAP_STDS2STDS;
typedef std::string STDS;


WorkerOfSearch::WorkerOfSearch(QObject *parent)
{
    setParent(parent);

    //开始HTTP请求
    connect(this, SIGNAL(sigStartHttpRequest(QString, QString, QString)),
            &httpThread, SLOT(sltHttpRequestSearch(QString, QString, QString)), Qt::QueuedConnection );

    //获取HTTP响应
    connect(&httpThread, SIGNAL(sigSendResult(std::shared_ptr<QByteArray>)),
            this, SLOT(sltGetHttpRequestResult(std::shared_ptr<QByteArray>)) );

    //开始Json解析
    connect(this, SIGNAL(sigStartDecode(std::shared_ptr<QByteArray>)),
            &decodeThread, SLOT(sltDecodeSearchNote(std::shared_ptr<QByteArray>)));

    //获取Json解析的结果
    connect(&decodeThread, SIGNAL(sigSendResult(std::vector< std::map<QString, QString> >&)),
            this, SLOT(sltGetDecodeResult(std::vector< std::map<QString, QString> >&)));

}

void WorkerOfSearch::moveToThread(QThread *t)
{
    QObject::moveToThread(t);
    httpThread.moveToThread( t ); //改变子线程依附，让槽函数在子线程中执行
    decodeThread.moveToThread( t ); //改变子线程依附，让槽函数在子线程中执行
}


//获取HTTP请求的结果，结果为Json数据，然后开始解析Json
void WorkerOfSearch::sltGetHttpRequestResult(std::shared_ptr<QByteArray> content)
{
    emit sigStartDecode(content);  //子线程开始解析json
}


//获取Json解析的结果，然后返回给给widget_search显示
void WorkerOfSearch::sltGetDecodeResult(std::vector<std::map<QString, QString> > &result)
{
    emit sigSendResult(result);  //结果返回给雇主：widget_search
}


void WorkerOfSearch::work(const QString& q,const QString& page,
                            const QString& type, const QString& order_by, const QString& time_range)
{

    QString param = "";
    param += "q=" + q;
    param += "&page=" + page;
    param += "&type=" + type;
    param += "&order_by=" + order_by;
    param += "&time_range=" + time_range;
    emit sigStartHttpRequest("https://www.jianshu.com/search/do?", "post", param); //子线程接收，开始HTTP请求
}
