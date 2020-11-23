#ifndef UTILSFORGUI_H
#define UTILSFORGUI_H

#include <QMainWindow>
#include <QStatusBar>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QSharedMemory>
#include <QHash>
#include "gui-lib_global.h"

class GUILIBSHARED_EXPORT UtilsForGui
{
    
public:
    UtilsForGui();

    enum HltStyle
    {
        StyleWidgetOn,      ///< Indexes a state of widget's as ready one to work
        StyleWidgetOff,     ///< Indexes a state of widget's as not ready one to work
        StyleWidgetWaiting, ///< Indexes a state of widget's as a waiting one for result
        StyleWidgetClear    ///< Indexes a state of widget's as a normal view
    };

    enum TypeMessage
    {
        MsgNormal,  ///< Indexes a state of widget's \a normal view message
        MsgError,   ///< Indexes a state of widget's \a error view message
        MsgWarning, ///< Indexes a state of widget's \a warning view message
        MsgInfo,    ///< Indexes a state of widget's \a informaive view message
        MsgClear,   ///< Indexes a state for \a clearing message widget style
    };

    static void SetReadOnly(QWidget* widget);
    static void SetWidgetInWaiting(QWidget* widget, bool isWait);
    static void SetWidgetStyleHlt(QWidget *widget, HltStyle style);
    static void MoveWindowToCenter(QWidget *w);
    static void ShowMessageInfo(QStatusBar *statusBar, QString text,
                                TypeMessage typeMsg, int delay=0);
    static void ResetRusLocale();
    static QString ByteArrayToHexString(const QByteArray ba);
    static QByteArray HexStringToByteArray(QString str);
    static void setWidgetStyleHlt(QWidget *label, HltStyle style);
    static bool SetSingleInstance(QString key, QSharedMemory &sharedMemory);
    static void clearStyleTheRestWidgets(QObject *ob, QPushButton *except);
    static void setTheWidgetStyles(QObject* ob, QPushButton* except, HltStyle style);
    template<typename T>
    static QHash<T, int> getObjectListByPattern(QString patternNameRx, QString patternTitleRx, QObject *container);
    static quint64 getFlagSetFromCheckboxKit(QString patternNameRx, QString patternTitleRx, QObject *container);
    static QVector<bool> getFlagSortedCheckboxKit(QString patternNameRx, QString patternTitleRx,
                                                  QObject *container);
    static void setEnabledAllWidgets(QWidget *container, bool isEnabled,
                                     bool includingContainer = true);
};

#endif // UTILSFORGUI_H
