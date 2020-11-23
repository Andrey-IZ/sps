#ifndef IDINFOWIDGET_H
#define IDINFOWIDGET_H

#include <QWidget>
#include "../evap/evapmessageservice.h"
#include "../evap-protocol/interfaces/evapinterface.h"

namespace Ui {
class IdInfoWidget;
}

class IdInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IdInfoWidget(EvapInterface *evapInterface, EvapMessageService *ms, QWidget *parent = 0);
    ~IdInfoWidget();

signals:
    void doubleClicked();
    void clicked();

public slots:
    void updateIdInfo();

private:
    Ui::IdInfoWidget *ui;
    EvapInterface *_evapInterface;
    EvapMessageService *_ms;

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
};

#endif // IDINFOWIDGET_H
