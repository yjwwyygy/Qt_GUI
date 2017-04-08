#ifndef CLI_TEST_GLOBAL_H
#define CLI_TEST_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef CLI_TEST_LIB
# define CLI_TEST_EXPORT Q_DECL_EXPORT
#else
# define CLI_TEST_EXPORT Q_DECL_IMPORT
#endif

#endif // CLI_TEST_GLOBAL_H
