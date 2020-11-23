#ifndef LED_H
#define LED_H

#include <QWidget>

class Led : public QWidget
{
	Q_OBJECT

    Q_PROPERTY(double diameter READ diameter WRITE setDiameter)
    Q_PROPERTY(QColor colorOn READ colorOn WRITE setColorOn)
    Q_PROPERTY(QColor colorOff READ colorOff WRITE setColorOff)
	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool state READ state WRITE setState)
    Q_PROPERTY(bool isflashing READ isFlashing WRITE setFlashing)
	Q_PROPERTY(int flashRate READ flashRate WRITE setFlashRate)

public:
    enum TStates {OFF = 0, ON = 1, WAIT = 3};

    Led(QWidget* parent=0);

    double diameter() const;
    void setDiameter(double diameter);

    QColor colorOn() const;
    void setColorOn(const QColor& colorOn);

    QColor colorOff() const;
    void setColorOff(const QColor &colorOff);

    QColor colorWait() const;
    void setColorWait(const QColor &colorWait);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    bool state() const;

    bool isFlashing() const;
    
    int flashRate() const;
    void setState(bool state);
    void setState(TStates state);
    void setFlashing(bool flashing);
    void setFlashRate(int rate);
    void startFlashing();
    void stopFlashing();

public slots:
    void toggleState();

public:
//	int heightForWidth(int width) const;
//	QSize sizeHint() const;
//	QSize minimumSizeHint() const;


protected:
    double diameter_;
    QColor m_colorOn;
    QColor m_colorOff;
    QColor m_colorWait;
    Qt::Alignment alignment_;
    TStates state_;
    int flashRate_;
    bool flashing_;

    void paintEvent(QPaintEvent* event);

    //
    // Pixels per mm for x and y...
    //
    int pixX_, pixY_;

    //
    // Scaled values for x and y diameter.
    //
    int diamX_, diamY_;

    QRadialGradient gradient_;
    QTimer* timer_;
};

#endif
