#include "widget.h"
#include "ui_widget.h"
#include "global.h"

//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#endif

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    initWindow();

    homePageWidget = new Widget_HomePage(this);
    ui->stackedWidget->addWidget(homePageWidget);
    ui->stackedWidget->setCurrentWidget(homePageWidget);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initWindow()
{

    setWindowFlag(Qt::FramelessWindowHint,true);

    /*设置事件过滤器，当Widget有事件触发时，会先调用titleBar的eventFilter方法*/
    installEventFilter(ui->titleBar);

    ui->titleBar->setStyleSheet("background:rgb(219,230,244);");
    ui->titleBar->setFixedHeight(50);
    setWindowTitle("简书");
    GLOBAL::titleBar = ui->titleBar;

    /*m_nBorder表示鼠标位于边框缩放范围的宽度*/
    m_nBorderWidth = 50;

    GLOBAL::setIcon( QChar(0xf14e), ui->pushButton_HomePage, "", 15 , Qt::AlignCenter);
    GLOBAL::setIcon( QChar(0xf02d), ui->pushButton_Books, "", 15 , Qt::AlignCenter);
    GLOBAL::setIcon( QChar(0xf002), ui->pushButton_Search, "", 13 , Qt::AlignCenter);
    GLOBAL::setIcon( QChar(0xf687), ui->pushButton_Publications, "", 15 , Qt::AlignCenter);
    GLOBAL::setIcon( QChar(0xf549), ui->pushButton_School, "", 15 , Qt::AlignCenter);
}

void Widget::clearSubWidget()
{
    int widgetNumber = ui->stackedWidget->count();
    for(int i = 0; i < widgetNumber; ++i){
        QWidget* w = ui->stackedWidget->currentWidget();

        homePageWidget = 0;    //首页
        searchWidget = 0;        //搜索
        booksWidget = 0;          //简书优选
        publicationWidget = 0; //简书版权
        shcoolWidget = 0;        //简书大学堂
        recommendationsAuthor = 0; //推荐作者

        w->deleteLater();
    }
}

//nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();


        // 如果鼠标位于子控件上，则不进行处理
        if (childAt(nX, nY) != nullptr){
            return QWidget::nativeEvent(eventType, message, result);
        }

        *result = HTCAPTION;
        qDebug() << nX << nY;
        // 鼠标区域位于窗体边框，进行缩放
        if ((nX > 0) && (nX < m_nBorderWidth))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorderWidth)){
            *result = HTTOP;
        }

        if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
                && (nY < m_nBorderWidth))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorderWidth)
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

//搜索
void Widget::on_pushButton_Search_clicked()
{
    clearSubWidget();
    QString searchContent = ui->lineEdit_SearchContent->text();

    if(0 == searchWidget){
        searchWidget = new Widget_Search(this);
        ui->stackedWidget->addWidget(searchWidget);
    }

    ui->stackedWidget->setCurrentWidget(searchWidget);
    searchWidget->search(searchContent, "1");
}

//首页
void Widget::on_pushButton_HomePage_clicked()
{
    clearSubWidget();

    if( 0 == homePageWidget){
        homePageWidget = new Widget_HomePage(this);
        ui->stackedWidget->addWidget(homePageWidget);
    }
    ui->stackedWidget->setCurrentWidget(homePageWidget);
}

//优选连载
void Widget::on_pushButton_Books_clicked()
{
    clearSubWidget();
    if(0 == booksWidget){
        booksWidget = new Widget_Books(this);
        ui->stackedWidget->addWidget(booksWidget);
    }
    ui->stackedWidget->setCurrentWidget(booksWidget);
}

//优选版权
void Widget::on_pushButton_Publications_clicked()
{
    clearSubWidget();
    if(0 == publicationWidget){
        publicationWidget = new Widget_Publications(this);
        ui->stackedWidget->addWidget(publicationWidget);
    }
    ui->stackedWidget->setCurrentWidget(publicationWidget);
}


//简书大学堂
void Widget::on_pushButton_School_clicked()
{
    clearSubWidget();
    if(0 == shcoolWidget){
        shcoolWidget = new Widget_Shcool(this);
        ui->stackedWidget->addWidget(shcoolWidget);
    }
    ui->stackedWidget->setCurrentWidget(shcoolWidget);
}

void Widget::on_pushButton_author_clicked()
{
    clearSubWidget();
    if(0 == recommendationsAuthor){
        recommendationsAuthor = new Widget_RecommendationsAuthor(this);
        ui->stackedWidget->addWidget(recommendationsAuthor);
    }
    ui->stackedWidget->setCurrentWidget(recommendationsAuthor);
}
