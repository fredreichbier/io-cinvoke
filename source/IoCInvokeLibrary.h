/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

#ifndef IoCInvokeLibrary_DEFINED
#define IoCInvokeLibrary_DEFINED 1

#include "IoObject.h"
#include <cinvoke.h>

#define ISCInvokeLibrary(self) IoObject_hasCloneFunc_(self, (IoTagCloneFunc *)IoCInvokeLibrary_rawClone)

typedef IoObject IoCInvokeLibrary;

typedef struct IoCInvokeLibrary
{
	CInvContext* context;
	CInvLibrary* library;
} IoCInvokeLibraryData;

IoTag *IoCInvokeLibrary_newTag(void *state);
IoCInvokeLibrary *IoCInvokeLibrary_proto(void *state);
IoCInvokeLibrary *IoCInvokeLibrary_rawClone(IoCInvokeLibrary *self);
void IoCInvokeLibrary_free(IoCInvokeLibrary *self);

void *IoCInvokeLibrary_rawGetFunctionPointer_(IoCInvokeLibrary *self, const char *name);
CInvContext* IoCInvokeLibrary_getContext_(IoCInvokeLibrary* self);

#endif
