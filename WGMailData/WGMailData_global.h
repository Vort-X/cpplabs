#ifndef WGMAILDATA_GLOBAL_H
#define WGMAILDATA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WGMAILDATA_LIBRARY)
#  define WGMAILDATA_EXPORT Q_DECL_EXPORT
#else
#  define WGMAILDATA_EXPORT Q_DECL_IMPORT
#endif

#endif // WGMAILDATA_GLOBAL_H