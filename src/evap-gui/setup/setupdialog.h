#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *insideWidget, QString title, QWidget *parent = 0);
    ~SetupDialog();

private:
    Ui::SetupDialog *ui;
    QWidget *_widget;
};

#endif // SETUPDIALOG_H
