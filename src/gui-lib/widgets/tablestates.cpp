#include "tablestates.h"

#include <QVBoxLayout>

TableStates::TableStates(QWidget *parent) : TableWidget(parent)
{
    setupUI();
}

TableStates::~TableStates() {}

void TableStates::setupUI()
{
    this->clear();
    this->setColumnCount(3);
    this->setRowCount(1);

    this->setCellWidget(0,0, new LedLabel("test"));
    this->setCellWidget(0,1, new LedLabel("test2"));
    this->setCellWidget(0,2, new LedLabel("test3"));
}

void TableStates::initTableStateView(QStringList rowsHeaders, QStringList colsHeaders,
                                     QList<QList<QString> >listLabels)
{
    QList<QList<QWidget*> >listWidgets;

    for (int row = 0; row < listLabels.count(); ++row)
    {
        QList<QWidget*> listCols;
        for (int col = 0; col < listLabels.at(row).count(); ++col)
        {
            LedLabel* ll = new LedLabel(listLabels.at(row).at(col));
            ll->setStylesheetOn("background-color: green;");
            ll->setStylesheetOff("background-color: red;");
            ll->setObjectName("labelInsideCell");

            listCols.append(ll);
        }
        listWidgets.append(listCols);
    }
    initTableView(rowsHeaders, colsHeaders, listWidgets);
}

LedLabel * TableStates::cellLedLabel(int idRow, int idCol) const
{
    return qobject_cast<LedLabel*>(this->cellWidget(idRow, idCol));
}

void TableStates::setCellStatus(int idRow, int idCol, LedLabel::StateType status)
{
    LedLabel* ll = cellLedLabel(idRow, idCol);
    if(ll == NULL)
        return;
    ll->setState(status);
    this->selectColumn(idCol);
}

void TableStates::setCellValue(int idRow, int idCol, QString value)
{
    LedLabel* ll = cellLedLabel(idRow, idCol);
    if(ll == NULL)
        return;
    ll->setText(value);
    this->selectColumn(idCol);
}

void TableStates::setAllCellStylesheet(QString stylesheetOn,
                                       QString stylesheetOff,
                                       QString stylesheetDisable)
{
    for (int row = 0; row < this->rowCount(); ++row)  {
        for (int col = 0; col < this->columnCount(); ++col) {
            if (!stylesheetOn.isEmpty())
                cellLedLabel(row, col)->setStylesheetOn(stylesheetOn);
            if (!stylesheetOff.isEmpty())
                cellLedLabel(row, col)->setStylesheetOff(stylesheetOff);
            if (!stylesheetDisable.isEmpty())
                cellLedLabel(row, col)->setStylesheetDisable(stylesheetDisable);
        }
    }
}

void TableStates::setAllCellStatus(LedLabel::StateType state)
{
    for (int row = 0; row < this->rowCount(); ++row)  {
        for (int col = 0; col < this->columnCount(); ++col) {
            cellLedLabel(row, col)->setState(state);
        }
    }
}

void TableStates::setCellStatus(int idRow, int idCol, bool status)
{
    LedLabel* ll = cellLedLabel(idRow, idCol);
    if(ll == NULL)
        return;
    ll->setLedOn(status);
    this->selectColumn(idCol);
}
