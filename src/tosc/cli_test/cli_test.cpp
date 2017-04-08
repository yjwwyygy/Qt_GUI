#include "cli_test.h"
#include "testpage.h"

IContext* gContext = nullptr;

extern "C"
{
	IViewPage* LoadBizModule(IContext* pContext)
	{
		gContext = pContext;
		return new TestPage(nullptr);
	}
};
