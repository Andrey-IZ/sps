#ifndef EVAP_GLOBAL_H
#define EVAP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EVAP_LIBRARY)
#  define EVAPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EVAPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EVAP_GLOBAL_H
