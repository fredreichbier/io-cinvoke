/* CInvoke - An Io interface to C
   Copyright (c) 2006 Trevor Fancher. All rights reserved.
   All code licensed under the New BSD license.
 */

// docDependsOn("IoDataType.c")

#include "IoCInvokePointer.h"
#include "IoCInvokeDataType.h"
#include "IoMap.h"
#include "IoSeq.h"
#include "IoNumber.h"
#include "UArray.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // exit

#define DATA(self) ((IoCInvokePointerData *)(IoObject_dataPointer(self)))

IoTag *IoCInvokePointer_newTag(void *state)
{
	IoTag *tag = IoTag_newWithName_("Pointer");
	IoTag_state_(tag, state);
	IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoCInvokePointer_free);
	IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoCInvokePointer_rawClone);
	return tag;
}

IoCInvokePointer *IoCInvokePointer_proto(void *state)
{
	IoObject *self = IoCInvokeDataType_new(state);
	IoObject_tag_(self, IoCInvokePointer_newTag(state));

	IoObject_setSlot_to_(self, IOSYMBOL("pointers"), IoMap_new(state));

	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokePointerData)));
	DATA(self)->ptr = NULL;

	IoState_registerProtoWithFunc_(state, self, IoCInvokePointer_proto);

	{
		IoMethodTable methodTable[] = {
			{"address", IoCInvokePointer_address},
			{"value", IoCInvokePointer_value},
			{"setValue", IoCInvokePointer_setValue},
			{"toType", IoCInvokePointer_toType},
			{NULL, NULL},
		};
		IoObject_addMethodTable_(self, methodTable);
	}

	return self;
}

IoCInvokePointer *IoCInvokePointer_rawClone(IoCInvokePointer *proto)
{
	IoObject *self = IoObject_rawClonePrimitive(proto);
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokePointerData)));
	return self;
}

IoCInvokePointer *IoCInvokePointer_new(void *state)
{
	IoObject *proto = IoState_protoWithInitFunction_(state, IoCInvokePointer_proto);
	return IOCLONE(proto);
}

void IoCInvokePointer_free(IoCInvokePointer *self)
{
	IoCInvokePointerData *data = DATA(self);

	free(data);
}

/* ---------------------------------------------------------------- */

IoCInvokePointer *IoCInvokePointer_ToType_(IoObject *type)
{
	IoObject *pointer, *self;
	IoMap *pointers;
	IoSymbol *key;

	// this is a hack so macros relying on self will work
	self = type;

	pointers = IoObject_getSlot_(IoState_protoWithInitFunction_(IOSTATE, IoCInvokePointer_proto), IOSYMBOL("pointers"));
	key = IoState_on_doCString_withLabel_(IOSTATE, type, "uniqueHexId", "IoCInvokePointer_ToType_");

	pointer = IoMap_rawAt(pointers, key);
	if (!pointer)
	{
		// create new pointer and add to cache
		pointer = IoCInvokePointer_new(IOSTATE);
		IoObject_setSlot_to_(pointer, IOSYMBOL("pointedToType"), type);

		IoMap_rawAtPut(pointers, key, pointer);
	}

	return pointer;
}

IoObject *IoCInvokePointer_address(IoCInvokePointer *self, IoObject *locals, IoMessage *m)
{
	char str[64] = {0};

	snprintf(str, 64, "%p", DATA(self)->ptr);
	return IOSYMBOL(str);
}

IoObject *IoCInvokePointer_value(IoCInvokePointer *self, IoObject *locals, IoMessage *m)
{
	IoObject *pointedToType;
	IoCInvokePointer *pointer;
	char *typeString, *cp, c;

	if (DATA(self)->ptr == NULL)
	{
		IoState_error_(IOSTATE, m, "attempt to dereference NULL pointer");
		return IONIL(self);
	}

	typeString = CSTRING(IoState_on_doCString_withLabel_(IOSTATE, self, "typeString", "IoCInvokePointer_value"));

	pointedToType = IoObject_getSlot_(self, IOSYMBOL("pointedToType"));
	if (ISCInvokePointer(pointedToType))
	{
		// we are a pointer to a pointer, so return a new Pointer
		// that points to the address of our ptr dereferenced

		cp = strrchr(typeString, '^');
		switch (c = *(++cp))
		{

#define IoCInvokePointer_value_SET_DATA_PTR(cType) \
	pointer = IOCLONE(IoCInvokePointer_ToType_(IoObject_getSlot_(pointedToType, IOSYMBOL("pointedToType")))); \
	DATA(pointer)->ptr = *((cType **)(DATA(self)->ptr)); break

			case 'c':
				IoCInvokePointer_value_SET_DATA_PTR(char);
			case 'C':
				IoCInvokePointer_value_SET_DATA_PTR(unsigned char);
			case 's':
				IoCInvokePointer_value_SET_DATA_PTR(short);
			case 'S':
				IoCInvokePointer_value_SET_DATA_PTR(unsigned short);
			case 'i':
				IoCInvokePointer_value_SET_DATA_PTR(int);
			case 'I':
				IoCInvokePointer_value_SET_DATA_PTR(unsigned int);
			case 'l':
				IoCInvokePointer_value_SET_DATA_PTR(long);
			case 'L':
				IoCInvokePointer_value_SET_DATA_PTR(unsigned long);
			case 'f':
				IoCInvokePointer_value_SET_DATA_PTR(float);
			case 'd':
				IoCInvokePointer_value_SET_DATA_PTR(double);

#undef IoCInvokePointer_value_SET_DATA_PTR

			case 'v':
				IoState_error_(IOSTATE, m, "attempt to dereference a void pointer");
				return IONIL(self);
			case '{':
			case '(':
				IoState_error_(IOSTATE, m, "structs and unions are not yet supported by CInvoke");
				return IONIL(self);
			default:
				IoState_error_(IOSTATE, m, "unknown character '%c' in Pointer typeString", c);
				return IONIL(self);
			}
			return pointer;
	}
	else if(ISCInvokeDataType(pointedToType))
	{
		return IoCInvokeDataType_objectFromData_(pointedToType, DATA(self)->ptr);
	}
	else
	{
		IoState_error_(IOSTATE, m, "attempt to dereference Pointer object whose pointedToType was neither a Pointer or a DataType.");
		return IONIL(self);
	}
}

IoObject *IoCInvokePointer_setValue(IoCInvokePointer *self, IoObject *locals, IoMessage *m)
{
	DATA(self)->ptr = IoCInvokeDataType_ValuePointerFromObject_(IoMessage_locals_valueArgAt_(m, locals, 0));

	return self;
}

IoObject *IoCInvokePointer_toType(IoCInvokePointer *self, IoObject *locals, IoMessage *m)
{
	IoObject *type;

	type = IoMessage_locals_valueArgAt_(m, locals, 0);
	return IoCInvokePointer_ToType_(type);
}

void *IoCInvokePointer_valuePointer(IoCInvokePointer *self)
{
	return &(DATA(self)->ptr);
}
