#ifndef TABSMARTWIDGET_H
#define TABSMARTWIDGET_H

#include <QSet>
#include <QList>
#include <QTabWidget>
#include "../gui-lib_global.h"

namespace Ui {
class TabSmartWidget;
}

class GUILIBSHARED_EXPORT TabSmartWidget : public QTabWidget
{
    Q_OBJECT

    Q_PROPERTY(bool isExpanded READ getIsExpanded WRITE setIsExpanded)
    Q_PROPERTY(int tabWidth READ getTabWidth WRITE setTabWidth)
    Q_PROPERTY(int tabHeight READ getTabHeight WRITE setTabHeight)

public:
    explicit TabSmartWidget(QWidget *parent = 0);
    ~TabSmartWidget();
    void init(QSize hidenSize);

signals:
    void expanded(bool isExpanded);

private:
    Ui::TabSmartWidget *ui;
    QSize m_size, m_curSize;
    QSet<int> listTabIndex;
    bool isExpanded;
    int tabWidth;
    int tabHeight;

    // QObject interface
    void revealWidget();

public:
    virtual bool eventFilter(QObject *o, QEvent *e);
    bool setWidget(int index, QWidget* widget, QString text, QSize widgetSize = QSize(), QIcon icon = QIcon());

    // QWidget interface
public:
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    bool getIsExpanded() const;
    void setIsExpanded(bool value);
    int getTabWidth() const;
    void setTabWidth(int value);
    int getTabHeight() const;
    void setTabHeight(int value);
};

#endif // TABSMARTWIDGET_H
