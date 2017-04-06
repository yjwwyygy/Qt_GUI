#ifndef FRAME_GLOBAL_H
#define FRAME_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef FRAME_LIB
# define FRAME_EXPORT Q_DECL_EXPORT
#else
# define FRAME_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAME_GLOBAL_H
