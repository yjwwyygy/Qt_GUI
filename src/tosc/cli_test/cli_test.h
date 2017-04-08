#ifndef CLI_TEST_H
#define CLI_TEST_H

#include "cli_test_global.h"
#include "iviewpage.h"
#include "icontext.h"

extern "C"
{
	CLI_TEST_EXPORT IViewPage* LoadBizModule(IContext* pContext);
};

#endif // CLI_TEST_H
