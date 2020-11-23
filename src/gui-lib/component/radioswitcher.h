#ifndef RADIOSWITCHER_H
#define RADIOSWITCHER_H

#include <component/clicklabel.h>
#include <QSlider>

class RadioSwitcher : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool isLeftOption READ isLeftOption WRITE setIsLeftOption)
    Q_PROPERTY(QString labelLeft READ labelLeft WRITE setLabelLeft)
    Q_PROPERTY(QString labelRight READ labelRight WRITE setLabelRight)
    Q_PROPERTY(bool viewOnlytLeft READ viewOnlytLeft WRITE setViewOnlytLeft)
    Q_PROPERTY(bool isDisabled READ isDisabled WRITE setIsDisabled)

public:
    RadioSwitcher(QWidget *parent = 0);

    bool isLeftOption();
    void setIsLeftOption(bool isLeftOption);
    void setDisabled();

    QString labelLeft();
    void setLabelLeft(QString labelLeft);
    QString labelRight();
    void setLabelRight(QString labelRight);
    void setLabels(QString leftText, QString rightText);
    bool viewOnlytLeft();
    void setViewOnlytLeftLabel(QString leftText);
    void setViewOnlytLeft(bool isViewOnlyLeft);
    bool isDisabled();
    void setIsDisabled(bool isDisabled);


signals:
    void switchChanged(bool isLeftOption);

protected:
    QSlider* m_sliderSwitcher;
    ClickLabel* m_label_Right;
    ClickLabel* m_label_Left;
    bool m_isLeftOption;
    QString m_selectionStyle;
    QString m_disselected;
    bool m_viewOnlytLeftLabel;

private slots:
    void leftClick();
    void rightClick();
    void sliderChanged(int value);
    void on_sliderSwitcher_sliderPressed();

private:
        void initUI();
};

#endif // RADIOSWITCHER_H
