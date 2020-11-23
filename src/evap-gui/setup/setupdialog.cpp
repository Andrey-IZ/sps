#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *insideWidget, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog), _widget(insideWidget)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(_widget);
    setWindowTitle(title);
}

SetupDialog::~SetupDialog()
{
    delete ui;
}
