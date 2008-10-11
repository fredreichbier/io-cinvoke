/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

#ifndef IoCInvokeFunction_DEFINED
#define IoCInvokeFunction_DEFINED 1

#include "IoObject.h"
#include <cinvoke.h>

#define ISCInvokeFunction(self) IoObject_hasCloneFunc_(self, (IoTagCloneFunc *)IoCInvokeFunction_rawClone)

typedef IoObject IoCInvokeFunction;

typedef struct IoCInvokeFunction
{
	CInvFunction interface;
} IoCInvokeFunctionData;

IoTag *IoCInvokeFunction_newTag(void *state);
IoCInvokeFunction *IoCInvokeFunction_proto(void *state);
IoCInvokeFunction *IoCInvokeFunction_rawClone(IoCInvokeFunction *self);
void IoCInvokeFunction_free(IoCInvokeFunction *self);

IoObject *IoCInvokeFunction_call(IoCInvokeFunction *self, IoObject *locals, IoMessage *m);

#endif
