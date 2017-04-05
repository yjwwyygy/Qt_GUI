#ifndef CCONTEXT_H
#define CCONTEXT_H

#include "frame_global.h"

class FRAME_EXPORT CContext
{
public:
	CContext();
	virtual ~CContext();

	/* 系统信息 */
	virtual int userId() const = 0;
};

#endif
