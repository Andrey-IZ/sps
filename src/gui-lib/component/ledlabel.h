#ifndef LEDLABEL_H
#define LEDLABEL_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include "../gui-lib_global.h"

class GUILIBSHARED_EXPORT LedLabel : public QWidget
{
    Q_OBJECT
    Q_ENUMS( StateType )


    void destructor();

public:
    explicit LedLabel(QWidget *parent = 0);
    LedLabel(QString text, QWidget *parent = 0);
    ~LedLabel();

    enum StateType
    {
        Off = 0, On = 1, Disabled = 2
    };

    Qt::Alignment alignment() const;

    void setTitle(const QString title);
    QString title();

    void setLabelWidth(int width);

    void setAlignment(Qt::Alignment alignment);
    void setAlignmentTitle(Qt::Alignment alignment);

    QString stylesheetOn() const;
    void setStylesheetOn(const QString &stylesheetOn);

    StateType state() const;
    void setState(StateType state);

    void setLedOn(bool isOn);
    bool isOn();

    QString stylesheetOff() const;
    void setStylesheetOff(const QString &stylesheetOff);

    QString text() const;
    void setText(const QString &text);

    int fontSize() const;
    void setFontSize(int fontSize);

    QString stylesheetDisable() const;
    void setStylesheetDisable(const QString &stylesheetDisable);

    void setStylesheetState(StateType state, const QString &stylesheet);

    void initLayout(bool isHorizontal = true);

    void setDisabled(bool isDisabled);
    void setEnabled(bool isEnabled);

    void setToolTip(QString text);

signals:
    void clicked(bool isOn);

protected:
    QBoxLayout *mainLayout;
    Qt::Alignment m_alignment;
    QString m_ssOn;
    QString m_ssOff;
    QString m_ssDisable;
    StateType m_state;
    QString m_text;
    QString m_textTitle;
    int m_fontSize;
    bool m_isOn;
    bool m_isHorizontal;
    QLabel *m_label;
    QLabel *m_labelTitle;

    void init(QString text, QString title, bool isHorizontal);
    
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *);

    // QWidget interface
protected:
    virtual void enabledChange(bool isEnabled);
};

#endif // LEDLABEL_H
