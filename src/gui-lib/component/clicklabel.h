#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>

class ClickLabel : public QLabel
{
    Q_OBJECT
public:
    ClickLabel(QWidget* parent = 0);

signals:
    void clicked();
public slots:

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *);
};

#endif // CLICKLABEL_H
