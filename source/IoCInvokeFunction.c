/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

// docDependsOn("CInvokeStructure")

#include "IoCInvokeFunction.h"
#include "IoCInvokeLibrary.h"
#include "IoCInvokeDataType.h"
#include "IoState.h"
#include "IoNumber.h"
#include "IoList.h"
#include "List.h"

#define DATA(self) ((IoCInvokeFunctionData *)(IoObject_dataPointer(self)))

IoTag *IoCInvokeFunction_newTag(void *state)
{
	IoTag *tag = IoTag_newWithName_("Function");
	IoTag_state_(tag, state);
	IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoCInvokeFunction_free);
	IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoCInvokeFunction_rawClone);
	return tag;
}

IoCInvokeFunction *IoCInvokeFunction_proto(void *state)
{
	IoObject *self = IoObject_new(state);
	IoObject_tag_(self, IoCInvokeFunction_newTag(state));

	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeFunctionData)));

	IoState_registerProtoWithFunc_(state, self, IoCInvokeFunction_proto);

	{
		IoMethodTable methodTable[] = {
			{"call", IoCInvokeFunction_call},
			{NULL, NULL},
		};
		IoObject_addMethodTable_(self, methodTable);
	}

	return self;
}

IoCInvokeFunction *IoCInvokeFunction_rawClone(IoCInvokeFunction *proto)
{
	IoObject *self = IoObject_rawClonePrimitive(proto);
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeFunctionData)));
	return self;
}

void IoCInvokeFunction_free(IoCInvokeFunction *self)
{
	free(DATA(self));
}

/* ---------------------------------------------------------------- */

IoObject *IoCInvokeFunction_call(IoCInvokeFunction *self, IoObject *locals, IoMessage *m)
{
	IoCInvokeLibrary *library;
	const char *funName;
	void *funPointer, **funArgVals, *funRetVal;
	char* funArgTypes;
	char funRetType[2];
	CInvFunction *funInterface;
	int funArgCount, i;
	cinv_status_t status;
	IoObject *returnValAsObj, *funRetTypeObject, *o;
	List *funArgTypeObjects;
	

	library = IoObject_getSlot_(self, IOSYMBOL("library"));
	funName = CSTRING(IoObject_getSlot_(self, IOSYMBOL("name")));
	funPointer = IoCInvokeLibrary_rawGetFunctionPointer_(library, funName);

	CInvContext* context = ((IoCInvokeLibraryData *)(IoObject_dataPointer(library)))->context;

	funArgTypeObjects = IoList_rawList(IoObject_getSlot_(self, IOSYMBOL("argumentTypes")));
	funRetTypeObject = IoObject_getSlot_(self, IOSYMBOL("returnType"));

	funArgCount = (int)List_size(funArgTypeObjects);
	funArgTypes = calloc(funArgCount, sizeof(char));

	for (i = 0; i < funArgCount; i++)
	{
		o = List_at_(funArgTypeObjects, i);
		funArgTypes[i] = IoCInvokeDataType_cinvType(o);
	}
	printf("Funargtypes: '%s'\n", funArgTypes);
	funRetType[0] = IoCInvokeDataType_cinvType(funRetTypeObject);
	funRetType[1] = '\0';
	
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

	funArgVals = calloc(funArgCount, sizeof(void *));
	funRetVal = calloc(1, getSizeFromSymbol(funRetType[0]));
	IoState_pushCollectorPause(IOSTATE);
	{
		for (i = 0; i < funArgCount; i++)
		{
			o = IoMessage_locals_valueArgAt_(m, locals, i);
			funArgVals[i] = IoCInvokeDataType_ValuePointerFromObject_(o);
			printf("yee, current: %p\n", funArgVals[i]);
		}
		status = cinv_function_invoke(context, funInterface, funPointer, funRetVal, funArgVals);
		if(!status) {
			printf("Something went wrong with invoking the function.\n");
			free(funArgTypes);
			free(funArgVals);
			free(funRetVal);
			return IONIL(self);
		}
		returnValAsObj = IoCInvokeDataType_objectFromData_(funRetTypeObject, funRetVal);
	}
	IoState_popCollectorPause(IOSTATE);

	free(funArgTypes);
	free(funArgVals);
	free(funRetVal);
	free(funInterface);

	return returnValAsObj;
}
