#include "ledlabel.h"

LedLabel::LedLabel(QWidget *parent) : QWidget(parent),
    mainLayout(NULL), m_isHorizontal(true), m_label(NULL), m_labelTitle(NULL)
{
    init("Led", "", true);
}

LedLabel::LedLabel(QString text, QWidget *parent): QWidget(parent),
    mainLayout(NULL), m_isHorizontal(true), m_label(NULL), m_labelTitle(NULL)
{
    init(text, "", true);
}

void LedLabel::destructor()
{
    delete mainLayout;
    delete m_label;
    delete m_labelTitle;
}

LedLabel::~LedLabel()
{
    destructor();
}

void LedLabel::init(QString text, QString title, bool isHorizontal)
{
    if (isHorizontal)
        mainLayout = new QHBoxLayout(this);
    else
        mainLayout = new QVBoxLayout(this);

    m_label = new QLabel(this);
    m_labelTitle = new QLabel(this);
    m_labelTitle->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    m_labelTitle->setMargin(0);
    m_textTitle = title;
    m_labelTitle->setText(title);
    if (title.isEmpty())
        m_labelTitle->setVisible(false);

    mainLayout->addWidget(m_labelTitle);
    mainLayout->addWidget(m_label);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);

    setAlignment(Qt::AlignCenter);
    setStylesheetOn("background-color: lightgreen; border: 1px solid blue;");
    setStylesheetOff("background-color: red; border: 1px solid blue;");
    setStylesheetDisable("background-color: lightgray; border: 1px solid darkgray; color:darkgray;");
    setState(LedLabel::Off);
    setFontSize(10);
    setText(text);

    setWindowTitle(tr("Led label"));
    resize(70, 30);
}

void LedLabel::initLayout(bool isHorizontal)
{
    if (m_isHorizontal == isHorizontal)
        return;
    m_isHorizontal = isHorizontal;
    destructor();
    init(m_text, m_textTitle, isHorizontal);
}

void LedLabel::setDisabled(bool isDisabled)
{
    if (isDisabled)
        setState(Off);
    else
        setState(Disabled);
}

void LedLabel::setEnabled(bool isEnabled)
{
    setDisabled(!isEnabled);
}

void LedLabel::setToolTip(QString text)
{
    m_label->setToolTip(text);
}

Qt::Alignment LedLabel::alignment() const
{
    return m_label->alignment();
}

void LedLabel::setTitle(const QString title)
{
    m_labelTitle->setVisible(true);
    m_textTitle = title;
    m_labelTitle->setText(title);
}

QString LedLabel::title()
{
    return m_labelTitle->text();
}

void LedLabel::setLabelWidth(int width)
{
    m_label->setMinimumWidth(width);
}

void LedLabel::setAlignment(Qt::Alignment alignment)
{
    m_label->setAlignment(alignment);
}

void LedLabel::setAlignmentTitle(Qt::Alignment alignment)
{
    m_labelTitle->setAlignment(alignment);
}

QString LedLabel::stylesheetOn() const
{
    return m_ssOn;
}

void LedLabel::setStylesheetOn(const QString &stylesheetOn)
{
    m_ssOn = stylesheetOn;
    setState(m_state);
}

void LedLabel::setStylesheetState(StateType state, const QString &stylesheet)
{
    switch (state)
    {
        case LedLabel::Off:
            m_ssOff = stylesheet;
            break;
        case LedLabel::On:
            m_ssOn = stylesheet;
            break;
        case LedLabel::Disabled:
            m_ssDisable = stylesheet;
            break;
    }
    m_label->setStyleSheet(stylesheet);
}

LedLabel::StateType LedLabel::state() const
{
    return m_state;
}

void LedLabel::setState(StateType state)
{
    m_state = state;
    switch (state)
    {
    case LedLabel::On:
        m_label->setStyleSheet(m_ssOn);
        m_isOn = true;
        break;
    case LedLabel::Off:
        m_label->setStyleSheet(m_ssOff);
        m_isOn = false;
        break;
    case LedLabel::Disabled:
        m_label->setStyleSheet(m_ssDisable);
        m_isOn = false;
        break;
    default:
        break;
    }
}

QString LedLabel::stylesheetOff() const
{
    return m_ssOff;
}

void LedLabel::setStylesheetOff(const QString &stylesheetOff)
{
    m_ssOff = stylesheetOff;
    setState(m_state);
}

QString LedLabel::text() const
{
    return m_text;
}

void LedLabel::setText(const QString &text)
{
    m_text = text;
    m_label->setText(text);
}

int LedLabel::fontSize() const
{
    return m_fontSize;
}

void LedLabel::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
    QFont font(m_label->font());
    font.setPointSize(fontSize);
    m_label->setFont(font);
}

QString LedLabel::stylesheetDisable() const
{
    return m_ssDisable;
}

void LedLabel::setStylesheetDisable(const QString &ssDisable)
{
    m_ssDisable = ssDisable;
    setState(m_state);
}

void LedLabel::setLedOn(bool isOn)
{
    m_isOn = isOn;
    if(m_isOn)
        setState(LedLabel::On);
    else
        setState(LedLabel::Off);
}

bool LedLabel::isOn()
{
    return m_isOn;
}

void LedLabel::mousePressEvent(QMouseEvent *)
{
    emit clicked(isOn());
}


void LedLabel::enabledChange(bool isEnabled)
{
    setEnabled(isEnabled);
}
