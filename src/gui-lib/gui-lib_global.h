#ifndef GUILIB_GLOBAL_H
#define GUILIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GUILIB_LIBRARY)
#  define GUILIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GUILIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GUILIB_GLOBAL_H
