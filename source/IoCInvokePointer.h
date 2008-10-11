/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

#ifndef IoCInvokePointer_DEFINED
#define IoCInvokePointer_DEFINED 1

#include "IoObject.h"

#define ISCInvokePointer(self) IoObject_hasCloneFunc_(self, (IoTagCloneFunc *)IoCInvokePointer_rawClone)

typedef IoObject IoCInvokePointer;

typedef struct IoCInvokePointer
{
	void *ptr;
} IoCInvokePointerData;

IoTag *IoCInvokePointer_newTag(void *state);
IoCInvokePointer *IoCInvokePointer_proto(void *state);
IoCInvokePointer *IoCInvokePointer_rawClone(IoCInvokePointer *self);
void IoCInvokePointer_free(IoCInvokePointer *self);

IoCInvokePointer *IoCInvokePointer_ToType_(IoObject *type);

IoCInvokePointer *IoCInvokePointer_new(void *state);

IoObject *IoCInvokePointer_address(IoCInvokePointer *self, IoObject *locals, IoMessage *m);
IoObject *IoCInvokePointer_value(IoCInvokePointer *self, IoObject *locals, IoMessage *m);
IoObject *IoCInvokePointer_setValue(IoCInvokePointer *self, IoObject *locals, IoMessage *m);
IoObject *IoCInvokePointer_toType(IoCInvokePointer *self, IoObject *locals, IoMessage *m);

void *IoCInvokePointer_valuePointer(IoCInvokePointer *self);

#endif
