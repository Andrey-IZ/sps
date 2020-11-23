#include "tabsmartwidget.h"
#include "ui_tabsmartwidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QTableView>
#include <algorithm>

TabSmartWidget::TabSmartWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::TabSmartWidget), isExpanded(false)
{
    ui->setupUi(this);
    m_curSize = m_size = this->size();

    tabBar()->installEventFilter(this);
    listTabIndex.insert(0);
    this->removeTab(0);
    this->removeTab(1);
}

void TabSmartWidget::init(QSize hidenSize)
{
    this->resize(hidenSize);
    m_curSize = this->size();
}

TabSmartWidget::~TabSmartWidget()
{
    delete ui;
}

int TabSmartWidget::getTabHeight() const
{
    return tabHeight;
}

void TabSmartWidget::setTabHeight(int value)
{
    tabHeight = value;
}

int TabSmartWidget::getTabWidth() const
{
    return tabWidth;
}

void TabSmartWidget::setTabWidth(int value)
{
    tabWidth = value;
}

bool TabSmartWidget::getIsExpanded() const
{
    return isExpanded;
}

void TabSmartWidget::setIsExpanded(bool value)
{
    isExpanded = value;
}

bool TabSmartWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == tabBar() && e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        int index = tabBar()->tabAt(mouseEvent->pos());
        if (this->currentIndex() == index)
            revealWidget();
        else
            this->setCurrentIndex(index);
        return true;
    }
    return QTabWidget::eventFilter(o, e);
}

bool TabSmartWidget::setWidget(int index, QWidget *widget, QString text, QSize widgetSize, QIcon icon)
{
    QWidget* tab = new QWidget();
    QVBoxLayout* l = new QVBoxLayout(tab);
    l->setSpacing(6);
    l->setContentsMargins(5, 5, 5, 5);
    widget->setParent(tab);
    l->addWidget(widget);

    int max = *std::max_element(listTabIndex.begin(), listTabIndex.end());

    if (listTabIndex.contains(index))   // it means that it replaces the tab content
    {
        this->setCurrentIndex(index);
        int ind = addTab(tab, icon, text);
        removeTab(ind - 1);
    }
    else if(index == max + 1)    // just append it as new tab widget
    {
        this->setCurrentIndex(max);
        addTab(tab, icon, text);
    }
    else
        return false;

    if (widgetSize.isValid())
        m_size = widgetSize;
    else
        m_size = widget->size();
    listTabIndex.insert(index);
    return true;
}

void TabSmartWidget::revealWidget()
{
    if (isExpanded)
    {
        QSize t_s = tabBar()->size();
        this->resize(t_s);
        m_curSize = this->size();
    }
    else
    {
        this->resize(m_size);
        m_curSize = m_size;
    }
    isExpanded = !isExpanded;
    emit expanded(isExpanded);
}

QSize TabSmartWidget::sizeHint() const
{
    return m_curSize;
}

QSize TabSmartWidget::minimumSizeHint() const
{
    QSize m_s = tabBar()->size();
    return QSize(m_s);
}
