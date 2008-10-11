/* CInvoke - An Io interface to C
   Copyright (c) 2006 Trevor Fancher. All rights reserved.
   All code licensed under the New BSD license.
 */

#ifndef IoCInvokeStructure_DEFINED
#define IoCInvokeStructure_DEFINED 1

#include "IoObject.h"
#include "List.h"
#include <cinvoke.h>

#define ISCInvokeStructure(self) IoObject_hasCloneFunc_(self, (IoTagCloneFunc *)IoCInvokeStructure_rawClone)

typedef IoObject IoCInvokeStructure;

typedef struct IoCInvokeStructure
{
	CInvStructure* structure;
	List* types;
} IoCInvokeStructureData;

IoTag *IoCInvokeStructure_newTag(void *state);
IoCInvokeStructure *IoCInvokeStructure_proto(void *state);
IoCInvokeStructure *IoCInvokeStructure_rawClone(IoCInvokeStructure *self);
void IoCInvokeStructure_free(IoCInvokeStructure *self);

IoObject *IoCInvokeStructure_addMember(IoCInvokeStructure *self, IoObject *locals, IoMessage *m);
IoObject* IoCInvokeStructure_finish(IoCInvokeStructure* self, IoObject* locals, IoMessage* m);
CInvStructure* IoCInvokeStructure_getStructure_(IoCInvokeStructure* self);

#endif
