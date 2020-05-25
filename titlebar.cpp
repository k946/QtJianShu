#include "titlebar.h"
#include <QStackedWidget>
#include "global.h"

//调用WIN API需要用到的头文件与库
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    initWindow();
    initConnect();
}

void TitleBar::setWindowTitle(const QString &title)
{
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setText(title);
}

void TitleBar::addStackedWidget(QStackedWidget *stack, QWidget *sub)
{
    StackedPair t;
    t.stack = stack;
    t.sub = sub;
    this->stack.push(t);

    m_back->setVisible(true);
}

void TitleBar::clearStackedWidget(QStackedWidget *s)
{
    if(s == 0){
        /*如果参数值为0则将元素全部清除*/
        while( !stack.empty()){
            stack.pop();
        }
    }else{
        /*参数值不为0，则清除s对应的层的上方的所有界面，包括s对应的层*/
        bool hasS = false;    //检测堆栈中是否有s
        std::stack<StackedPair> temp;     //保存弹出的StackPair,防止没有s时将所有界面清除

        /*将所有s之上包括s在内的stackedWidget弹出*/
        while( !stack.empty()){
           StackedPair t = stack.top();
           if(t.stack == s){
               /*达到s所在的层*/
               hasS = true;
               /*继续清除s*/
               while( !stack.empty() ){
                   StackedPair t = stack.top();
                   if(t.stack != s){
                       break;
                   }
                   stack.pop();
               }
               break;
           }
           stack.pop();
           temp.push(t);
        }

        /*不存在s，将界面全部压回stack*/
        if(!hasS){
            while(!temp.empty()){
                StackedPair t = temp.top();
                stack.push(t);
                temp.pop();
            }
        }
    }

    if(stack.empty()){
        m_back->setVisible(false);
    }

}

/*设置图标*/
void TitleBar::setTitleBarIcon(const QString &iconPath)
{
    QPixmap map(iconPath);

    /*删除原始图标*/
    for(int i = 0; i < m_iconLabel->children().size(); ++i){
        if(m_iconLabel->children().at(i)->objectName() == "icon"){
            m_iconLabel->children().at(i)->deleteLater();
            break;
        }
    }

    m_iconLabel->setPixmap(map);
}

void TitleBar::initWindow()
{

    /*标题栏按钮样式*/
    QString sty = "QPushButton{border:0px;}"
                  "QPushButton:hover{"
                  "background:rgb(237,245,253)};";

    /*顶层布局*/
    QHBoxLayout* topHLayout = new QHBoxLayout(this);
    topHLayout->setMargin(0);
    topHLayout->setSpacing(0);

    /*返回上一个界面按钮*/
    m_back = new QPushButton(this);
    m_back->setFixedWidth(90);
    m_back->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    GLOBAL::setIcon( QChar(0xf060), m_back, "", 9, Qt::AlignCenter);
    m_back->setStyleSheet(sty);
    m_back->setVisible(false);
    topHLayout->addWidget(m_back);


    /*图标*/
    m_iconLabel = new QLabel(this);
    m_iconLabel->setStyleSheet("background:transparent;");
    GLOBAL::setIcon(QChar(0xf14e), m_iconLabel, "rgb(122,220,237)", 12, Qt::AlignVCenter);
    m_iconLabel->setFixedSize(40, 40);
    topHLayout->addSpacing(20);
    topHLayout->addWidget(m_iconLabel);


    /*标题*/
    m_titleLabel = new QLabel(this);
    m_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_titleLabel->setText("标题");
    topHLayout->addSpacing(20);
    topHLayout->addWidget(m_titleLabel);
    topHLayout->addStretch(0);


    /*最小化按钮*/
    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setFixedWidth(90);
    m_minimizeButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    GLOBAL::setIcon( QChar(0xf2d1), m_minimizeButton, "", 9, Qt::AlignHCenter, 5);
    m_minimizeButton->setStyleSheet(sty);
    topHLayout->addWidget(m_minimizeButton);


    /*最大化按钮*/
    m_maximizeButton = new QPushButton(this);
    m_maximizeButton->setFixedWidth(90);
    m_maximizeButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    GLOBAL::setIcon( QChar(0xf2d2), m_maximizeButton, "", 9, Qt::AlignCenter);
    m_maximizeButton->setStyleSheet(sty);
    topHLayout->addWidget(m_maximizeButton);

    /*关闭按钮*/
    m_closeButton = new QPushButton(this);
    m_closeButton->setFixedWidth(90);
    m_closeButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    GLOBAL::setIcon( QChar(0xf410), m_closeButton, "", 9, Qt::AlignCenter);
    m_closeButton->setStyleSheet(sty);
    topHLayout->addWidget(m_closeButton);

    /*问题：无法设置背景色*/
    setStyleSheet("TitleBar{background:rgb(219,230,244);"
                        "border-top:4px solid rgb(103,203,224);}");

    /*问题：无法设置高度*/
}

void TitleBar::initConnect()
{

    connect(m_minimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_maximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_closeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_back, SIGNAL(clicked(bool)), this, SLOT(sltClickedBack()));
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_maximizeButton->click();
}

/*窗口移动*/
void TitleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
    event->ignore();
#else
#endif

}

/*事件过滤器
**用于设置标题、设置图标、最大最小化、返回上一个界面
*/
bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{

    if(obj->objectName() == "stackedWidget" || obj->objectName() == "stackWidget"){
        QStackedWidget* s = (QStackedWidget*)obj;
        if(event->type() == QEvent::Hide ){
            /*stackedWidget隐藏时，删除其保存的返回界面,同时设置stackedWidget显示第一个界面*/
            s->setCurrentIndex(0);
            clearStackedWidget(s);
        }else if( event->type() == QEvent::Close ){
            /*stackedWidget关闭时，删除其保存的返回界面*/
            clearStackedWidget(s);
        }
    }

    switch(event->type())
    {
        //设置标题
        case QEvent::WindowTitleChange:
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget)
            {
                m_titleLabel->setText(pWidget->windowTitle());
                return true;
            }
        }
        //设置图标
        case QEvent::WindowIconChange:
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget)
            {
                QIcon icon = pWidget->windowIcon();
                m_iconLabel->setPixmap(icon.pixmap(m_iconLabel->size()));
                return true;
            }
        }
        // 窗口状态变化、窗口大小变化
        case QEvent::WindowStateChange:
        case QEvent::Resize:
            updateMaximize();
            return true;
    }

    return QWidget::eventFilter(obj, event);
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_maximizeButton->setToolTip(tr("Restore"));
            m_maximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            m_maximizeButton->setProperty("maximizeProperty", "maximize");
            m_maximizeButton->setToolTip(tr("Maximize"));
        }

        m_maximizeButton->setStyle(QApplication::style());
    }
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == m_minimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_maximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_closeButton)
        {
            pWindow->close();
        }
        else if(pButton == m_back){

        }
    }
}

void TitleBar::sltClickedBack()
{
    if( !this->stack.empty() ){
        StackedPair t = this->stack.top();
        t.stack->setCurrentWidget(t.sub);
        this->stack.pop();
    }

    if(this->stack.empty() ){
        m_back->setVisible(false);
    }

}
