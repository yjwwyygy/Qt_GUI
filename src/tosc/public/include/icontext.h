#ifndef CCONTEXT_H
#define CCONTEXT_H

#include "frame_global.h"

class IContext;
extern IContext* gContext;

class FRAME_EXPORT IContext
{
public:
	IContext();
	virtual ~IContext();
};

#endif
