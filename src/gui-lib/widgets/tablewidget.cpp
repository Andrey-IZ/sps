#include "tablewidget.h"

#include <QVBoxLayout>
#include <QHeaderView>


TableWidget::TableWidget(QWidget *parent):
    QTableWidget(parent)
{
    setupUI();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);

}

TableWidget::~TableWidget()
{
//    delete m_tableView;
}

void TableWidget::setupUI()
{
//    QVBoxLayout* verticalLayout = new QVBoxLayout(this);
//    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
//    m_tableView = new QTableView(tableWidget);
    this->setObjectName(QString::fromUtf8("tableView"));
    this->setMinimumSize(QSize(50, 30));
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setTextElideMode(Qt::ElideMiddle);
    this->setGridStyle(Qt::DashLine);
//    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setProperty("showSortIndicator", QVariant(false));

//    verticalLayout->addWidget(m_tableView);
//    m_model = new QStandardItemModel(this);
//    m_tableView->setModel(m_model);
}

void TableWidget::initTableView(QStringList rowsHeaders, QStringList colsHeaders, QList<QList<QWidget*> >listWidgets)
{
    this->clear();
    this->setColumnCount(listWidgets.at(0).count());
    this->setRowCount(listWidgets.count());

//    m_tableView->setModel(m_model);

    for (int row = 0; row < listWidgets.count(); ++row)
    {
        for (int col = 0; col < listWidgets.at(0).count(); ++col)
        {
            this->setCellWidget(row, col, listWidgets.at(row).at(col));
//            m_model->setData(m_model->index(row, col), false, Qt::UserRole);
//            m_tableView->setIndexWidget(m_model->index(row, col),listWidgets.at(col).at(row));
        }
    }

    this->setHorizontalHeaderLabels(colsHeaders);
    this->setVerticalHeaderLabels(rowsHeaders);
}
