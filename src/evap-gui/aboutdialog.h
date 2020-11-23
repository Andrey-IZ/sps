#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "../evap-protocol/interfaces/evapinterface.h"
#include "../evap/evapmessageservice.h"

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(EvapInterface *evapInterface, EvapMessageService *ms,
                         QWidget *parent = 0);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
    EvapInterface *_evapInterface;
    EvapMessageService *_ms;

    void init();
    void enableUI(bool isEnabled);
};

#endif // ABOUTDIALOG_H
