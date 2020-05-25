#ifndef TRIGGERMENUITEM_H
#define TRIGGERMENUITEM_H

#include "labelmouseevent.h"

class TriggerMenuItem : public LabelMouseEvent
{
public:
    TriggerMenuItem(QWidget* parent = 0);
    TriggerMenuItem(QString text, QWidget* parent = 0);

private:
    void initWindow();

};

#endif // TRIGGERMENUITEM_H
