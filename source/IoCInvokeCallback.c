//metadoc TrivalObject copyright Your Name Here
//metadoc TrivalObject license BSD revised
//metadoc TrivalObject category Example
/*metadoc TrivalObject description
Describe your addon here.
*/

#include "IoState.h"
#include "IoList.h"
#include "IoCInvokeCallback.h"
#include "IoCInvokeDataType.h"
#include "IoCInvokeLibrary.h"

#define DATA(self) ((IoCInvokeCallbackData *)(IoObject_dataPointer(self)))

// _tag makes an IoTag for the bookkeeping of names and methods for this proto
IoTag *IoCInvokeCallback_newTag(void *state)
{
        // first allocate a new IoTag
        IoTag *tag = IoTag_newWithName_("Callback");

        // record this tag as belonging to this VM
        IoTag_state_(tag, state);

        // give the tag pointers to the _free, _mark and _rawClone functions we'll need to use
        IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoCInvokeCallback_free);
        IoTag_markFunc_(tag, (IoTagMarkFunc *)IoCInvokeCallback_mark);
        IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoCInvokeCallback_rawClone);
        return tag;
}

// _proto creates the first-ever instance of the prototype 
IoObject *IoCInvokeCallback_proto(void *state)
{
        // First we allocate a new IoObject
        IoCInvokeCallback *self = IoObject_new(state);

        // Then tag it
        IoObject_tag_(self, IoCInvokeCallback_newTag(state));

        // then register this proto generator
        IoState_registerProtoWithFunc_(state, self, IoCInvokeCallback_proto);

	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeCallbackData)));
        // and finally, define the table of methods this proto supports
        // we just have one method here, returnSelf, then terminate the array
        // with NULLs
        {
                IoMethodTable methodTable[] = {
                {"returnSelf", IoCInvokeCallback_returnSelf},
		{"setMessage", IoCInvokeCallback_setMessage},
                {NULL, NULL},
                };
                IoObject_addMethodTable_(self, methodTable);
        }

        return self;
}

// _rawClone clones the existing proto passed as the only argument
IoObject *IoCInvokeCallback_rawClone(IoCInvokeCallback *proto)
{
        IoObject *self = IoObject_rawClonePrimitive(proto);
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeCallbackData)));
	DATA(self)->callback = NULL;
        return self;
}

// _new creates a new object from this prototype
IoObject *IoCInvokeCallback_new(void *state)
{
        IoObject *proto = IoState_protoWithInitFunction_(state, IoCInvokeCallback_proto);
        return IOCLONE(proto);
}

// _mark is called when this proto is marked for garbage collection
// If this proto kept references to any other IoObjects, it should call their mark() methods as well.
IoObject *IoCInvokeCallback_mark(IoCInvokeCallback* self)
{
        return self;
}

// _free defines any cleanup or deallocation code to run when the object gets garbage collected
void IoCInvokeCallback_free(IoCInvokeCallback *self)
{
        // free dynamically allocated data and do any cleanup
	free(DATA(self));
}

// This is the one method we define, which does nothing but return self.
IoObject *IoCInvokeCallback_returnSelf(IoCInvokeCallback *self, IoObject *locals, IoMessage *m)
{
        // A method should always return an IoObject*
        // Per Io style guidelines, it's preferred to return self when possible.
        return self;
}

void IoCInvokeCallback_callback(CInvFunction* function, void* parameters[], void* returnout, void* ud) {
	IoCInvokeCallbackData* data = (IoCInvokeCallbackData*)ud;
	IoMessage* msg = IoMessage_deepCopyOf_(data->message);
	
	IoObject* self = data->target;
	IoObject *funRetTypeObject;
	List *funArgTypeObjects;
	int i, funArgCount;

	funArgTypeObjects = IoList_rawList(IoObject_getSlot_(self, IOSYMBOL("argumentTypes")));
	funArgCount = (int)List_size(funArgTypeObjects);
	funRetTypeObject = IoObject_getSlot_(self, IOSYMBOL("returnType"));
	//printf("yeh, callback\n");
	for (i = 0; i < funArgCount; i++)
	{
		IoObject* o = IoCInvokeDataType_objectFromData_(List_at_(funArgTypeObjects, i), parameters[i]);
		IoMessage_addCachedArg_(msg, o);
	}
	IoObject* result = IoMessage_locals_performOn_(msg, data->locals, self);
	// pass the result to the C function
	size_t mysize = getSizeFromSymbol(IoCInvokeDataType_cinvType(funRetTypeObject));
	void* ptr = IoCInvokeDataType_ValuePointerFromObject_(result);
	// TODO: *That* looks ugly. Can it be better?
	memmove(returnout, ptr, mysize);
	// TODO: memory management stuff. That is probably a memory lag.
}

IoObject* IoCInvokeCallback_setMessage(IoCInvokeCallback* self, IoObject* locals, IoMessage* m) {
        DATA(self)->target = IoMessage_locals_valueArgAt_(m, locals, 0);
        DATA(self)->message = IoMessage_locals_messageArgAt_(m, locals, 1);
        DATA(self)->locals = locals;
	return self;
}

IoObject *IoCInvokeCallback_setCallback(IoCInvokeCallback *self)
{
	// create the function interface
	IoCInvokeLibrary *library;
	char* funArgTypes;
	char funRetType[2];
	CInvFunction *funInterface;
	int funArgCount, i;
	IoObject *funRetTypeObject, *o;
	List *funArgTypeObjects;
	library = IoObject_getSlot_(self, IOSYMBOL("library"));

	CInvContext* context = ((IoCInvokeLibraryData *)(IoObject_dataPointer(library)))->context;
	DATA(self)->context = context;

	library = IoObject_getSlot_(self, IOSYMBOL("library"));

	funArgTypeObjects = IoList_rawList(IoObject_getSlot_(self, IOSYMBOL("argumentTypes")));
	funRetTypeObject = IoObject_getSlot_(self, IOSYMBOL("returnType"));

	funArgCount = (int)List_size(funArgTypeObjects);
	funArgTypes = calloc(funArgCount, sizeof(char));

	for (i = 0; i < funArgCount; i++)
	{
		o = List_at_(funArgTypeObjects, i);
		funArgTypes[i] = IoCInvokeDataType_cinvType(o);
	}
	funRetType[0] = IoCInvokeDataType_cinvType(funRetTypeObject);
	funRetType[1] = '\0';

    //printf("[Callback] FunArgTypes: %s; FunRetType: %s\n", funArgTypes, funRetType);
	
	if(funRetType[0] == 0) {
		funInterface = cinv_function_create(context, CINV_CC_DEFAULT, funRetType+1, funArgTypes);
	} else {
		funInterface = cinv_function_create(context, CINV_CC_DEFAULT, funRetType, funArgTypes);
	}
	if (!funInterface)
	{
		printf("\n\nUh oh.  Something went wrong in IoCInvokeFunction_call.\n\n");
		free(funArgTypes);
		return IONIL(self);
	}

	// create the callback object
	//
	CInvCallback* cb;
	cb = cinv_callback_create(context, funInterface, DATA(self), IoCInvokeCallback_callback);
	if(!cb) {
		printf("something went wrong with the callback.");
		free(funArgTypes);
		return IONIL(self);
	}
	DATA(self)->callback = cb;
	free(funArgTypes);

	return self;
}

void* IoCInvokeCallback_valuePointer(IoObject* self) {
	if(!DATA(self)->callback) {
		IoCInvokeCallback_setCallback(self);
	}
    if(!DATA(self)->entrypoint) {
        DATA(self)->entrypoint = cinv_callback_getentrypoint(DATA(self)->context, DATA(self)->callback);
    }
    //printf("[Callback] My callback: %d\n", &DATA(self)->callback);
    return &(DATA(self)->entrypoint);
}

