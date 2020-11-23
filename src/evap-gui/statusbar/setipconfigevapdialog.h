#ifndef SETIPCONFIGEVAPDIALOG_H
#define SETIPCONFIGEVAPDIALOG_H

#include <QDialog>
#include "../evap-protocol/interfaces/evaptcp.h"

namespace Ui {
class SetIpConfigEvapDialog;
}

class SetIpConfigEvapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetIpConfigEvapDialog(EvapTCP* evapTcp, QWidget *parent = 0);
    ~SetIpConfigEvapDialog();

private slots:
    void on_pushButtonSend_clicked();

private:
    Ui::SetIpConfigEvapDialog *ui;
    EvapTCP* evapTcp;
};

#endif // SETIPCONFIGEVAPDIALOG_H
