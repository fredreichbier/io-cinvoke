//metadoc copyright Your Name Here 2008

// don't forget the macro guard
#ifndef IOCInvokeCallback_DEFINED
#define IOCInvokeCallback_DEFINED 1

#include "IoObject.h"
#include <cinvoke.h>

// define a macro that can check whether an IoObject is of our type by checking whether it holds a pointer to our clone function
#define ISCInvokeCallback(self) IoObject_hasCloneFunc_(self, (IoTagCloneFunc *)IoCInvokeCallback_rawClone)

// declare a C type for ourselves
typedef IoObject IoCInvokeCallback;

typedef struct IoCInvokeCallback {
	CInvContext* context;
	CInvCallback* callback;
	
	IoObject* target;
	IoObject* locals;
	IoMessage* message;
} IoCInvokeCallbackData;

// define the requisite functions
IoTag *IoCInvokeCallback_newTag(void *state);
IoObject *IoCInvokeCallback_proto(void *state);
IoObject *IoCInvokeCallback_rawClone(IoCInvokeCallback *self);
IoObject *IoCInvokeCallback_mark(IoCInvokeCallback *self);
void IoCInvokeCallback_free(IoCInvokeCallback *self);

// define our custom C functions
IoObject *IoCInvokeCallback_returnSelf(IoCInvokeCallback *self, IoObject *locals, IoMessage *m);
IoObject *IoCInvokeCallback_setCallback(IoCInvokeCallback *self);
void* IoCInvokeCallback_valuePointer(IoObject* self);
IoObject* IoCInvokeCallback_setMessage(IoCInvokeCallback* self, IoObject* locals, IoMessage* m);

#endif

