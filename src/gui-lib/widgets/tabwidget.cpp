#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent):
    QTabWidget(parent)
{
}

void TabWidget::hideTab(int numTab)
{
    if(numTab >= 0 && m_hiddenTabs.count() > numTab && !m_hiddenTabs.at(numTab))
    {
        int num = -1;
        for (int i = 0; i <= numTab; ++i)
        {
            if(!m_hiddenTabs[i])
                num ++;
        }

        this->removeTab(num);
        m_hiddenTabs[numTab] = true;
    }
}

int TabWidget::revealTab(int numTab)
{
    int index;
    if(numTab >= 0 && m_hiddenTabs.count() > numTab && m_hiddenTabs.at(numTab))
    {
        m_hiddenTabs[numTab] = false;

        auto i = numTab - 1;
        if (i < 0) i = 0;

        index = this->insertTab(i, m_tabs.at(numTab).first, m_tabs.at(numTab).second);
    }
    return index;
}

void TabWidget::addTab(QWidget *widget, const QString &s)
{
    m_tabs.append(QPair<QWidget*, QString>(widget, s));
    QTabWidget::addTab(widget, s);
    m_hiddenTabs.append(false);
}

void TabWidget::setTabText(int index, const QString &text)
{
    if(index >= 0 && m_hiddenTabs.count() > index)
        m_tabs[index].second = text;
    QTabWidget::setTabText(index, text);
}

void TabWidget::tabInserted(int index)
{
    QTabWidget::tabInserted(index);
}
