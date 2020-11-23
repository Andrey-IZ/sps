#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <functional>
#include "../../plugins/src/basecustomwidget.h"

class MY_WIDGET_EXPORT TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = 0);

    void hideTab(int numTab);
    int revealTab(int numTab);
    void addTab(QWidget *widget, const QString &s);
    void setTabText(int index, const QString &text);

private:
    QVector<QPair<QWidget*, QString> > m_tabs;
    QVector<bool> m_hiddenTabs;

    // QTabWidget interface
protected:
    virtual void tabInserted(int index);
};

#endif // TABWIDGET_H
