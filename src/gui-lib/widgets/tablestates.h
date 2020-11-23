#ifndef TABLESTATES_H
#define TABLESTATES_H

#include "tablewidget.h"
#include "../component/ledlabel.h"

class GUILIBSHARED_EXPORT TableStates : public TableWidget
{
    Q_OBJECT

//    Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount)

    void setupUI();
public:
    TableStates(QWidget *parent = 0);
    ~TableStates();

    void initTableStateView(QStringList rowsHeaders, QStringList colsHeaders,
                            QList<QList<QString> > listLabels);
    void setCellStatus(int idRow, int idCol, bool status);
    void setCellStatus(int idRow, int idCol, LedLabel::StateType status);
    void setCellValue(int idRow, int idCol, QString value);

    LedLabel * cellLedLabel(int idRow, int idCol) const;
    void setAllCellStatus(LedLabel::StateType state);
    void setAllCellStylesheet(QString stylesheetOn = "", QString stylesheetOff= "",
                              QString stylesheetDisable ="");
signals:

public slots:

private:
};

#endif // TABLESTATES_H
