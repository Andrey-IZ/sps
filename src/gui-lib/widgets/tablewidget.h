#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QStandardItemModel>
#include <QTableWidget>


class TableWidget: public QTableWidget
{
    Q_OBJECT
//    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)

public:
    TableWidget(QWidget *parent = 0);
    ~TableWidget();

    void initTableView(QStringList rowsHeaders, QStringList colsHeaders, QList<QList<QWidget *> > listWidgets);

signals:

public slots:
private:
//    QTableWidget* m_tableView;
    QStandardItemModel* m_model;
    void setupUI();
};

#endif // TABLEWIDGET_H
