#ifndef CCONTEXT_H
#define CCONTEXT_H

#include "frame_global.h"

class FRAME_EXPORT IContext
{
public:
	IContext();
	virtual ~IContext();

	/* ϵͳ��Ϣ */
	virtual int userId() const = 0;
};

#endif
