#include <math.h>

#include <QPainter>
#include <QGradient>
#include <QPaintDevice>
#include <QTimer>

#include "led.h"

Led::Led(QWidget* parent) :
	QWidget(parent),
	diameter_(5),
    m_colorOn(QColor("lime")),
    m_colorOff(QColor("red")),
    m_colorWait(QColor("yellow")),
	alignment_(Qt::AlignCenter),
    state_(Led::OFF),
	flashRate_(200),
	flashing_(false)
{
	timer_ = new QTimer(this);
	connect(timer_, SIGNAL(timeout()), this, SLOT(toggleState()));

    setDiameter(diameter_);
}

double Led::diameter() const
{
	return diameter_;
}

void Led::setDiameter(double diameter)
{
	diameter_ = diameter;

	pixX_ = round(double(height())/heightMM());
	pixY_ = round(double(width())/widthMM());

	diamX_ = diameter_*pixX_;
	diamY_ = diameter_*pixY_;

	update();
}


QColor Led::colorOn() const
{
    return m_colorOn;
}

void Led::setColorOn(const QColor& color)
{
    m_colorOn = color;
	update();
}

Qt::Alignment Led::alignment() const
{
	return alignment_;
}

void Led::setAlignment(Qt::Alignment alignment)
{
	alignment_ = alignment;

	update();
}

void Led::setFlashRate(int rate)
{
	flashRate_ = rate;
	update();
}

void Led::setFlashing(bool flashing)
{
	flashing_ = flashing;
	update();
}

void Led::startFlashing()
{
	setFlashing(true);
}

void Led::stopFlashing()
{
	setFlashing(false);
}

void Led::setState(bool state)
{
    if (state)
        state_ = Led::ON;
    else
        state_ = Led::OFF;
    update();
}

void Led::setState(Led::TStates state)
{
    state_ = state;
}

void Led::toggleState()
{
    if (state_ == Led::ON)
    {
        state_ = Led::OFF;
        update();
        return;
    }
    if (state_ == Led::OFF)
    {
        state_ = Led::ON;
        update();
        return;
    }
}

QColor Led::colorWait() const
{
    return m_colorWait;
}

void Led::setColorWait(const QColor &colorWait)
{
    m_colorWait = colorWait;
}

//int Led::heightForWidth(int width) const
//{
//	return width;
//}

//QSize Led::sizeHint() const
//{
//	return QSize(diamX_, diamY_);
//}

//QSize Led::minimumSizeHint() const
//{
//	return QSize(diamX_, diamY_);
//}

void Led::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

	QPainter p(this);

	QRect geo = geometry();
	int width = geo.width();
	int height = geo.height();

	int x=0, y=0;
	if ( alignment_ & Qt::AlignLeft )
		x = 0;
	else if ( alignment_ & Qt::AlignRight )
		x = width-diamX_;
	else if ( alignment_ & Qt::AlignHCenter )
		x = (width-diamX_)/2;
	else if ( alignment_ & Qt::AlignJustify )
		x = 0;

	if ( alignment_ & Qt::AlignTop )
		y = 0;
	else if ( alignment_ & Qt::AlignBottom )
		y = height-diamY_;
	else if ( alignment_ & Qt::AlignVCenter )
		y = (height-diamY_)/2;

	QRadialGradient g(x+diamX_/2, y+diamY_/2, diamX_*0.4,
		diamX_*0.4, diamY_*0.4);

	g.setColorAt(0, Qt::white);
    switch (state_)
    {
        case Led::ON:
            g.setColorAt(1, m_colorOn);
            break;
        case Led::OFF:
            g.setColorAt(1, m_colorOff);
            break;
        case Led::WAIT:
            g.setColorAt(1, m_colorWait);
    }
	QBrush brush(g);

    p.setPen(m_colorOn);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setBrush(brush);
	p.drawEllipse(x, y, diamX_-1, diamY_-1);
    p.setPen(Qt::black);
    p.drawEllipse(x, y, diamX_, diamY_);

	if ( flashRate_ > 0 && flashing_ )
		timer_->start(flashRate_);
	else
		timer_->stop();
}

QColor Led::colorOff() const
{
    return m_colorOff;
}

void Led::setColorOff(const QColor &colorOff)
{
    m_colorOff = colorOff;
    update();
}

bool Led::state() const
{
    if (state_ == Led::ON)
        return true;
    else
        return false;
}

bool Led::isFlashing() const
{
    return flashing_;
}

int Led::flashRate() const
{
    return flashRate_;
}
