#include "radioswitcher.h"
#include <QHBoxLayout>

RadioSwitcher::RadioSwitcher(QWidget *parent) :
    QWidget(parent),
    m_viewOnlytLeftLabel(false)
{
    initUI();

    connect(m_label_Left, SIGNAL(clicked()), this, SLOT(leftClick()));
    connect(m_label_Right, SIGNAL(clicked()), this, SLOT(rightClick()));
    connect(m_sliderSwitcher, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
    connect(m_sliderSwitcher, SIGNAL(sliderPressed()), this, SLOT(on_sliderSwitcher_sliderPressed()));

    m_isLeftOption = false;

    m_selectionStyle = "font: bold;"
                        "background: lime;"
                        "border:1px solid black;"
                        "border-radius: 7px;";
    m_disselected = "border:1px solid black;"
                    "font: bold;"
                    "border-radius: 7px;";
//                    "background: red;";
    setIsLeftOption(true);
}

void RadioSwitcher::initUI()
{
//    this->setObjectName(QString::fromUtf8("RadioSwitcher"));
    this->setMinimumSize(QSize(0, 44));
    this->setMaximumSize(QSize(300, 44));
    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
//    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    m_label_Left = new ClickLabel(this);
//    m_label_Left->setObjectName(QString::fromUtf8("m_label_Left"));
    m_label_Left->setMinimumSize(QSize(15, 0));
    m_label_Left->setCursor(QCursor(Qt::PointingHandCursor));
    m_label_Left->setText("a");
    m_label_Left->setStyleSheet(QString::fromUtf8("border:1px solid black;\n"
"border-radius: 7px;"));
    m_label_Left->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    horizontalLayout->addWidget(m_label_Left);

    m_sliderSwitcher = new QSlider(this);
//    m_sliderSwitcher->setObjectName(QString::fromUtf8("sliderSwitcher"));
    m_sliderSwitcher->setMinimumSize(QSize(50, 26));
    m_sliderSwitcher->setCursor(QCursor(Qt::PointingHandCursor));
    m_sliderSwitcher->setMinimum(0);
    m_sliderSwitcher->setMaximum(1);
    m_sliderSwitcher->setPageStep(1);
    m_sliderSwitcher->setOrientation(Qt::Horizontal);
    m_sliderSwitcher->setTickPosition(QSlider::NoTicks);

    horizontalLayout->addWidget(m_sliderSwitcher);

    m_label_Right = new ClickLabel(this);
    m_label_Right->setObjectName(QString::fromUtf8("label_Right"));
    m_label_Right->setMinimumSize(QSize(15, 0));
    m_label_Right->setText("b");
    QFont font;
//    font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
    font.setPointSize(8);
    font.setBold(false);
    font.setItalic(false);
    font.setWeight(50);
    m_label_Right->setFont(font);
    m_label_Right->setCursor(QCursor(Qt::PointingHandCursor));
//    m_label_Right->setStyleSheet(QString::fromUtf8("border:1px solid black;\nborder-radius: 7px;"));
    m_label_Right->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    horizontalLayout->addWidget(m_label_Right);
}

void RadioSwitcher::sliderChanged(int value)
{
    setIsLeftOption(!((bool) value));
}

void RadioSwitcher::leftClick()
{
    setIsLeftOption(true);
}

void RadioSwitcher::rightClick()
{
    setIsLeftOption(false);
}

bool RadioSwitcher::isLeftOption()
{
    return m_isLeftOption;
}

void RadioSwitcher::setIsLeftOption(bool isLeftOption)
{
    if(!m_sliderSwitcher->isEnabled())
        m_sliderSwitcher->setEnabled(true);
    else  if(m_isLeftOption == isLeftOption)
        return;

    m_isLeftOption = isLeftOption;
    if (m_isLeftOption)
    {
        m_sliderSwitcher->setSliderPosition(0);
        m_label_Left->setStyleSheet(m_selectionStyle);
        m_label_Right->setStyleSheet(m_disselected);
    }
    else
    {
        m_sliderSwitcher->setSliderPosition(1);
        m_label_Right->setStyleSheet(m_selectionStyle);
        m_label_Left->setStyleSheet(m_disselected);
    }
    emit switchChanged(m_isLeftOption);
}

void RadioSwitcher::setDisabled()
{
    m_sliderSwitcher->setEnabled(false);
    m_label_Left->setDisabled(true);
    m_label_Right->setDisabled(true);
}

QString RadioSwitcher::labelLeft()
{
    return m_label_Left->text();
}

void RadioSwitcher::setLabelLeft(QString labelLeft)
{
    m_label_Left->setText(labelLeft);
    m_label_Left->setVisible(true);
}

QString RadioSwitcher::labelRight()
{
    return m_label_Right->text();
}

void RadioSwitcher::setLabelRight(QString labelRight)
{
    m_label_Right->setText(labelRight);
    m_label_Right->setVisible(true);
}

void RadioSwitcher::setLabels(QString leftText, QString rightText)
{
    setLabelLeft(leftText);
    setLabelRight(rightText);
}

bool RadioSwitcher::viewOnlytLeft()
{
    return m_viewOnlytLeftLabel;
}

void RadioSwitcher::setViewOnlytLeftLabel(QString leftText)
{
    m_label_Left->setText(leftText);
    m_label_Right->setVisible(false);
}

void RadioSwitcher::setViewOnlytLeft(bool isViewOnlyLeft)
{
    if(isViewOnlyLeft)
        setViewOnlytLeftLabel(this->labelLeft());
    else
        setLabels(labelLeft(), labelRight());
    update();
//        setLabels(labelLeft(), labelRight());
}

bool RadioSwitcher::isDisabled()
{
    return m_sliderSwitcher->isEnabled();
}

void RadioSwitcher::setIsDisabled(bool isDisabled)
{
    if(isDisabled)
        setDisabled();
    else
        setIsLeftOption(m_isLeftOption);
}

void RadioSwitcher::on_sliderSwitcher_sliderPressed()
{
    setIsLeftOption(!isLeftOption());
}
