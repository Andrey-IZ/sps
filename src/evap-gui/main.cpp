#include "mainwindow.h"
#include <QApplication>
#include "../gui-lib/utilsforgui.h"
#include "settings.h"
#include "QFile"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QApplication a(argc, argv);
    UtilsForGui::ResetRusLocale();

#ifndef QT_NO_STYLE
    // custom stylesheet
    QFile styleFile(":/style.qss");
//    QFile styleFile(qApp->applicationDirPath().append("/../rc/ui/style.qss"));
    if( styleFile.open(QFile::ReadOnly) )
    {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
    }
    else
        qDebug() << "Warning: Style can't be loaded.";
#endif

    MainWindow w;
    w.show();

    return a.exec();
}
