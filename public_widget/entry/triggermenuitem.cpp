#include "triggermenuitem.h"

TriggerMenuItem::TriggerMenuItem(QWidget *parent) : LabelMouseEvent(parent)
{
    initWindow();
}

TriggerMenuItem::TriggerMenuItem(QString text, QWidget *parent) :
    LabelMouseEvent(text, parent)
{
    initWindow();
}

void TriggerMenuItem::initWindow()
{
    setMargin(0);

    setStyleSheet("LabelMouseEvent{"
                            "color:#969696;}"
                            "QLabel:hover{"
                            "border-bottom:4px solid;}"
                            "QLabel:hover{"
                            "color:#646464;}"
                            );

    setAlignment(Qt::AlignVCenter);
    setIndent(90);
    setFont( QFont("黑体", 12, 87) );

    int wordNumber = text().size();
    int width = 200 + (wordNumber - 2) * 40;
    setFixedSize(width,80);
}

