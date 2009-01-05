/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

#include "IoCInvokeLibrary.h"
#include <string.h>
#include "IoSeq.h"

#define DATA(self) ((IoCInvokeLibraryData *)(IoObject_dataPointer(self)))

IoTag *IoCInvokeLibrary_newTag(void *state)
{
	IoTag *tag = IoTag_newWithName_("Library");
	IoTag_state_(tag, state);
	IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoCInvokeLibrary_free);
	IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoCInvokeLibrary_rawClone);
	return tag;
}

IoCInvokeLibrary *IoCInvokeLibrary_proto(void *state)
{
	IoObject *self = IoObject_new(state);
	IoObject_tag_(self, IoCInvokeLibrary_newTag(state));

	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeLibraryData)));
	DATA(self)->context = NULL;
	DATA(self)->library = NULL;

	IoState_registerProtoWithFunc_(state, self, IoCInvokeLibrary_proto);

	{
		IoMethodTable methodTable[] = {
            {"explicitLoad", IoCInvokeLibrary_explicitLoad},
			{NULL, NULL},
		};
		IoObject_addMethodTable_(self, methodTable);
	}
	return self;
}

IoCInvokeLibrary *IoCInvokeLibrary_rawClone(IoCInvokeLibrary *proto)
{
	IoObject *self = IoObject_rawClonePrimitive(proto);
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeLibraryData)));
	return self;
}

void IoCInvokeLibrary_free(IoCInvokeLibrary *self)
{
	CInvLibrary *library = DATA(self)->library;

	if (library)
	{
		cinv_library_delete(DATA(self)->context, library);
		library = NULL;
	}
	/*if (DATA(self)->context) {
		cinv_context_delete(DATA(self)->context);
	}*/

	free(DATA(self));
}

/* ---------------------------------------------------------------- */

void *IoCInvokeLibrary_rawGetFunctionPointer_(IoCInvokeLibrary *self, const char *name)
{
    if(!DATA(self)->library) {
        IoCInvokeLibrary_load(self);
    }
	void *ep = cinv_library_load_entrypoint(DATA(self)->context, DATA(self)->library, name);
    if(!ep) {
        IoState_error_(IOSTATE, NULL, "There is no function named like '%s' (\
entry point is NULL) - maybe it's a macro?", name);
    }
    return ep;
}

void IoCInvokeLibrary_load(IoCInvokeLibrary *self) {
    IoCInvokeLibrary_getContext_(self);
    
//    printf("[Library] Name: %s; Context: %d\n", CSTRING(IoObject_getSlot_(self, IOSYMBOL("name"))), DATA(self)->context);
    if (!DATA(self)->library)
	{
		const char *name = CSTRING(IoObject_getSlot_(self, IOSYMBOL("name")));

		DATA(self)->library = cinv_library_create(DATA(self)->context, name);
		if(!DATA(self)->library) {
			printf("Library not loaded: %s\n", cinv_context_geterrormsg(DATA(self)->context));
		}
	}

//    printf("Done - %d.\n", DATA(self)->library);
}

IoObject *IoCInvokeLibrary_explicitLoad(IoCInvokeLibrary *self, IoObject *locals, IoMessage *m) {
    IoCInvokeLibrary_load(self);
    return self;
}

CInvContext* IoCInvokeLibrary_getContext_(IoCInvokeLibrary* self) {
	//printf("[Library] _getContext_ called!\n");
    if (!(DATA(self)->context)) { 
		DATA(self)->context = getGlobalContext();
	}
    return (DATA(self)->context);
}

CInvContext *getGlobalContext() {
    static CInvContext *ctx = 0;
    if(ctx == 0) ctx = cinv_context_create();
    return ctx;
}
