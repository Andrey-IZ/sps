#include "utilsforgui.h"
#include <QDesktopWidget>
#include <QObject>
#include <QTextCodec>
#include <QSystemSemaphore>
#include <QCheckBox>
#include <QSpinBox>
#include <QDebug>

UtilsForGui::UtilsForGui()
{

}

/*!
 * \brief Sets the \a widget as readonly
 * \param widget given widget
 */
void UtilsForGui::SetReadOnly(QWidget *widget)
{
    widget->setAttribute(Qt::WA_TransparentForMouseEvents);
    widget->setFocusPolicy(Qt::NoFocus);
}

/*!
 * \brief Make the \a widget highlighted as waited
 * \param widget given widget
 * \param isWait condition to manage state
 */
void UtilsForGui::SetWidgetInWaiting(QWidget *widget, bool isWait)
{
    widget->setDisabled(isWait);
}
/**
 * @brief Changes an widget's appearance
 * @param widget current widget
 * @param style kind of the widget state
 */
void UtilsForGui::SetWidgetStyleHlt(QWidget *widget, HltStyle style)
{
    switch (style)
    {
        case UtilsForGui::StyleWidgetOn:
            widget->setStyleSheet(
                "background-color: rgb(0, 225, 0); color: black;");
            break;

        case UtilsForGui::StyleWidgetOff:
            widget->setStyleSheet(
                "background-color: rgb(220, 0, 0); color: black;");
            break;

        case UtilsForGui::StyleWidgetWaiting:
            widget->setStyleSheet(
                "background-color: rgb(247, 247, 0); color: black;"); // подсветить жёлтым цветом
            break;

        case UtilsForGui::StyleWidgetClear:
            widget->setStyleSheet("");  // отключить стили
            break;

        default:
            break;
    }
}

/*!
 * \brief Changes an label's appearance
 * \param label current label (or sensor led)
 * \param style kind of the widget state
 */
void UtilsForGui::setWidgetStyleHlt(QWidget *label, HltStyle style)
{
    switch (style)
    {
        case UtilsForGui::StyleWidgetOn:
            label->setEnabled(true);
            label->setStyleSheet("background-color: rgb(0, 255, 0);"
                                 "border: 2px solid #3873d9;"
                                 "color: black;");
            break;
        case UtilsForGui::StyleWidgetOff:
            label->setEnabled(true);
            label->setStyleSheet("background-color: rgb(255, 0, 0);"
                                  "border: 2px solid #3873d9;"
                                 "color: white;");
            break;
        case UtilsForGui::StyleWidgetWaiting:
            label->setDisabled(true);
            break;
        case UtilsForGui::StyleWidgetClear:
            label->setEnabled(true);
            label->setStyleSheet("");
            break;
    }
}

/*!
 * \brief Moves the widget to center of the screen
 * \param w given widget
 */
void UtilsForGui::MoveWindowToCenter(QWidget *w)
{
    QRect frect = w->frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    w->move(frect.topLeft());
}

/*!
 * \brief Shows text notificaton in statusbar with highlight
 * \param statusBar used StatusBar
 * \param text  text message
 * \param typeMsg used highlights (\ref UtilsForGui::TypeMessage)
 * \param delay the number of seconds to keeps the message in \a statusBar.
 * \note If the parameter \a delay is not used the message keep forever.
 */
void UtilsForGui::ShowMessageInfo(QStatusBar *statusBar, QString text, TypeMessage typeMsg, int delay)
{
//    QString text = QObject::tr(text_str.toStdString().c_str());
    if (statusBar!= NULL)
    {
        switch (typeMsg)
        {
            case MsgNormal:
                statusBar->setStyleSheet("Color: black");
                statusBar->showMessage(text, 2000);
                break;

            case MsgError:
                statusBar->setStyleSheet("color: rgb(255, 0, 0);");
                statusBar->showMessage(QObject::tr("Ошибка: ") + text, delay);
                break;

            case MsgWarning:
                statusBar->setStyleSheet("background-color: rgb(255, 235, 15);"
                                           "color: black;");    // жёлтый
                statusBar->showMessage(QObject::tr(" ") + text, delay);
                break;

            case MsgInfo:
                statusBar->setStyleSheet("color: rgb(0, 0, 0);");
                statusBar->showMessage(QObject::tr("Инфо: ") + text, delay);
                break;
            case MsgClear:
                statusBar->setStyleSheet("");
                statusBar->showMessage("");
                break;

            default:
                break;
        }
    }
}

/*!
 * \brief Converts QByteArray into Hexadecimal QString
 * \param ba given QByteArray
 * \return the hex string
 */
QString UtilsForGui::ByteArrayToHexString(const QByteArray ba)
{
    QString resHex;
    int size = ba.size();
    for (int i = 0; i < size; ++i)
        resHex.append(QString::number(quint8(ba.at(i)), 16).toUpper().
                      rightJustified(2, '0')).append(" ");
    return resHex.remove(resHex.count() - 1, 1);
}

/*!
 * \brief Converts Hexadecimal string into QbyteArray
 * \param str given string
 * \return translated QByteArray
 */
QByteArray UtilsForGui::HexStringToByteArray(QString str)
{
    return QByteArray::fromHex(str.replace(QRegExp("[^A-F0-9]"), "").toLatin1());
}

/*!
 * Sets a russian codecs in the app.
 * \note In Windows codecs set into CP1251. In Linux sets into UTF-8
 * \warning You have to use \a tr() function to type message encoded as russian
 */
void UtilsForGui::ResetRusLocale()
{
    setlocale(LC_CTYPE, "Russian");
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
#ifdef Q_WS_WIN
    codec = QTextCodec::codecForName("CP1251");
#endif
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
}

/*!
 * \brief Forbids to launch a duplicate of the app
 * \param key any string to set something kinda a salt
 * \param sharedMemory a reference for QSharedMemory object
 * \return true if the copy of app had been started, otherwise false
 * \note this version can be used in Windows also.
 */
bool UtilsForGui::SetSingleInstance(QString key, QSharedMemory &sharedMemory)
{
    QSystemSemaphore semaphore("semaphore" + key, 1);  // создаём семафор
    semaphore.acquire();                              // Поднимаем семафор, запрещая другим экземплярам работать с разделяемой памятью

    #ifndef Q_OS_WIN32
       // в linux/unix разделяемая память не освобождается при аварийном завершении приложения,
       // поэтому необходимо избавиться от данного мусора
       QSharedMemory nix_fix_shared_memory("_sharedMem" + key);
       if(nix_fix_shared_memory.attach()){
           nix_fix_shared_memory.detach();
       }
    #endif

    sharedMemory.setKey("_sharedMem" + key);  // Создаём экземпляр разделяемой памяти
    bool is_running;                               // переменную для проверки уже запущенного приложения
    if (sharedMemory.attach()){ // пытаемся присоединить экземпляр разделяемой памяти
                               // к уже существующему сегменту
       is_running = true;      // Если успешно, то определяем, что уже есть запущенный экземпляр
    }else{
       is_running = !sharedMemory.create(1); // В противном случае выделяем 1 байт памяти
//       is_running = false;     // И определяем, что других экземпляров не запущено
    }

    semaphore.release();        // Опускаем семафор

    return is_running;
}

/*!
 * \brief Sets the widget style for an array ones
 * \param ob - widget container
 * \param except widget which skips the changings
 * \param style (\ref UtilsForGui::HltStyle)
  */
void UtilsForGui::setTheWidgetStyles(QObject* ob, QPushButton* except, HltStyle style)
{
    foreach (QObject* obj, ob->children())
    {
        if (obj == except) continue;
        QPushButton* pb = qobject_cast<QPushButton*>(obj);
        if (pb != NULL && pb != except)
                UtilsForGui::SetWidgetStyleHlt(pb, style);
        else if (obj->children().count() > 0)
                    clearStyleTheRestWidgets(obj, except);
    }
}

/*!
 * \brief Clears the style for the buttons array apart one
 * \param ob - widgets container
 * \param except - button which skips the changings
 */
void  UtilsForGui::clearStyleTheRestWidgets(QObject* ob, QPushButton* except)
{
    setTheWidgetStyles(ob, except, UtilsForGui::StyleWidgetClear);
}

template<typename T>
inline QHash<T, int> UtilsForGui::getObjectListByPattern(QString patternNameRx,
                                                      QString patternTitleRx,
                                                      QObject *container)
{
    QRegExp rxName(patternNameRx);
    QRegExp rxTitle(patternTitleRx);
    QRegExp *rx;
    QHash<T, int> listH;

    foreach (T w, container->findChildren<T>())
    {
        bool status;
        if (!patternNameRx.isEmpty())
        {
            status = rxName.indexIn(w->objectName()) != -1;
            rx = &rxName;
        }
        else if (!patternTitleRx.isEmpty())
        {
            status = rxTitle.indexIn(w->text()) != -1;
            rx = &rxTitle;
        }
        else
            return listH;

        if (status)
        {
            QStringList list = rx->capturedTexts();
            if(list.count() > 1)
            {
                listH.insert(w, list[1].toInt());
            }
        }
    }
    return listH;
}

quint64 UtilsForGui::getFlagSetFromCheckboxKit(QString patternNameRx, QString patternTitleRx,
                                               QObject *container)
{
    quint64 flags = 0;

    QHash<QCheckBox*, int>  hash = getObjectListByPattern<QCheckBox*>(patternNameRx, patternTitleRx, container);

    QList<int> svchList = hash.values();
    qSort(svchList.begin(), svchList.end(), qLess<int>());
    foreach (int offset, svchList)
    {
        QCheckBox* cb = hash.key(offset);
        int state = cb->isChecked() ? 1 : 0;

        flags |= state << (offset-1);
    }
    return flags;
}

QVector<bool> UtilsForGui::getFlagSortedCheckboxKit(QString patternNameRx, QString patternTitleRx,
                                               QObject *container)
{
    QHash<QCheckBox*, int>  hash = getObjectListByPattern<QCheckBox*>(patternNameRx, patternTitleRx, container);

    QList<int> svchList = hash.values();
    qSort(svchList.begin(), svchList.end(), qLess<int>());
    QVector<bool> flags;
    foreach (auto val, svchList)
        flags.append(hash.key(val)->isChecked());

    return flags;
}

void UtilsForGui::setEnabledAllWidgets(QWidget* container, bool isEnabled,
                                       bool includingContainer)
{
    if (includingContainer)
        container->setEnabled(isEnabled);
    foreach (QWidget* w, container->findChildren<QWidget*>())
    {
        w->setEnabled(isEnabled);
    }
}
