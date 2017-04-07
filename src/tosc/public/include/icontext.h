#ifndef CCONTEXT_H
#define CCONTEXT_H

#include "frame_global.h"

class FRAME_EXPORT IContext
{
public:
	IContext();
	virtual ~IContext();

	/* 系统信息 */
	virtual QString userId() const = 0;
};

#endif
