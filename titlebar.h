#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPoint>
#include <QPixmap>
#include <QString>
#include <QStackedWidget>
#include <stack>

class StackedPair{
public:
    QStackedWidget* stack;
    QWidget* sub;
};


class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = NULL);
    void setWindowTitle(const QString& title);
    void setTitleBarIcon(const QString &iconPath);

    /*添加返回按钮返回的界面*/
    void addStackedWidget(QStackedWidget* stack, QWidget* sub);

    /*清除返回界面*/
    void clearStackedWidget(QStackedWidget* s);


private:
    void initWindow();
    void initConnect();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    /**
     * @brief 事件过滤处理器
     * @param obj，参数
     * @param event，参数，事件
     * @return 成功返回true，失败返回false
     * @note 设置标题、图标
     */
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void updateMaximize();

protected slots:
    void onClicked();
    void sltClickedBack();

private:
    std::stack< StackedPair > stack;
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;

    QPushButton* m_back;
    QPushButton* m_minimizeButton;
    QPushButton* m_maximizeButton;
    QPushButton* m_closeButton;
    QPoint m_start;//起始点
    QPoint m_end;//结束点

};

#endif // TITLEBAR_H
