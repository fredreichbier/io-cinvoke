/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

// docDependsOn("CInvokeLibrary")
//
#include "IoCInvokeDataType.h"
#include "IoCInvokePointer.h"
#include "IoCInvokeCallback.h"
#include "IoCInvokeStructureInstance.h"
#include "IoSeq.h"
#include "IoNumber.h"
#include "IoObject.h"
#include "IoState.h"
#include <stdlib.h>
#include <string.h>
#include <cinvoke.h>

#define DATA(self) ((IoCInvokeDataTypeData *)(IoObject_dataPointer(self)))

void *IoCInvokeDataType_null = NULL;

IoTag *IoCInvokeDataType_newTag(void *state)
{
	IoTag *tag = IoTag_newWithName_("DataType");
	IoTag_state_(tag, state);
	IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoCInvokeDataType_free);
	IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoCInvokeDataType_rawClone);
	return tag;
}

IoCInvokeDataType *IoCInvokeDataType_proto(void *state)
{
	IoObject *self = IoObject_new(state);
	IoObject_tag_(self, IoCInvokeDataType_newTag(state));

	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeDataTypeData)));
	DATA(self)->needToFreeStr = 0;

	IoState_registerProtoWithFunc_(state, self, IoCInvokeDataType_proto);

	{
		IoMethodTable methodTable[] = {
			{"value", IoCInvokeDataType_value},
			{"setValue", IoCInvokeDataType_setValue},
			{NULL, NULL},
		};
		IoObject_addMethodTable_(self, methodTable);
	}

	return self;
}

IoCInvokeDataType *IoCInvokeDataType_rawClone(IoCInvokeDataType *proto)
{
	IoObject *self = IoObject_rawClonePrimitive(proto);
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeDataTypeData)));
	return self;
}

IoCInvokeDataType *IoCInvokeDataType_new(void *state)
{
	IoObject *proto = IoState_protoWithInitFunction_(state, IoCInvokeDataType_proto);
	return IOCLONE(proto);
}

void IoCInvokeDataType_free(IoCInvokeDataType *self)
{
	IoCInvokeDataTypeData *data;

	data = DATA(self);
	if (data->needToFreeStr)
	{
		free(data->type.str);
		data->needToFreeStr = 0;
	}
	free(DATA(self));
}

/* ---------------------------------------------------------------- */

void *IoCInvokeDataType_ValuePointerFromObject_(IoObject *o)
{
	IoObject *self, *number;

	// this is a hack so macros relying on self will work
	self = o;
    //printf("[DataType] INVOKED, and it's a ...");
	if (ISNUMBER(o))
	{
//		number = IoState_doCString_(IoObject_state(o), "CInvoke Types Double clone");
		number = IoState_doCString_(IoObject_state(o), "CInvoke Types SInt clone");

//		DATA(number)->type.d = IoObject_dataDouble(o);
		DATA(number)->type.i = IoNumber_asInt(o); // TODO: so, why? Double should be the proper way :s
        //printf("number\n");
		return IoCInvokeDataType_ValuePointerFromObject_(number);
	}
	else if (ISSEQ(o))
	{
        //printf("seq\n");
		return (char **)IoObject_dataPointer(o);
	}
	else if (ISNIL(o))
	{
        //printf("nil\n");
		return &IoCInvokeDataType_null;
	}
	else if(ISCInvokePointer(o))
	{
        //printf("ptr\n");
		return IoCInvokePointer_valuePointer(o);
	}
	else if(ISCInvokeDataType(o))
	{
        //printf("datatype\n");
		return IoCInvokeDataType_valuePointer(o);
	}
	else if(ISCInvokeStructureInstance(o)) {
        //printf("struct\n");
		return IoCInvokeStructureInstance_valuePointer(o);
	}
	else if(ISCInvokeCallback(o)) {
        //printf("Callback value.\n");
		return IoCInvokeCallback_valuePointer(o);
	}
	else {
        //printf("WTF\n");
		return NULL;
	}
}

IoCInvokeDataType *IoCInvokeDataType_value(IoCInvokeDataType *self, IoObject *locals, IoMessage *m)
{
	return IoCInvokeDataType_objectFromData_(self, &(DATA(self)->type));
}

IoCInvokeDataType *IoCInvokeDataType_setValue(IoCInvokeDataType *self, IoObject *locals, IoMessage *m)
{
	char c, *cp;
	IoCInvokeDataTypeData *data;
	void *value;

	data = DATA(self);
	if (data->needToFreeStr)
	{
		free(data->type.str);
		data->needToFreeStr = 0;
	}

	value = IoCInvokeDataType_ValuePointerFromObject_(IoMessage_locals_valueArgAt_(m, locals, 0));
	//printf("_setValue: '%s'\n",  CSTRING(IoState_on_doCString_withLabel_(IOSTATE, self, "typeString", "IoCInvokeDataType_setValue")));
	switch (c = CSTRING(IoState_on_doCString_withLabel_(IOSTATE, self, "typeString", "IoCInvokeDataType_setValue"))[0])
	{
		case 'c':
			data->type.C = **((char **)value); break;
		case 'C':
			data->type.C = (unsigned char)**((char **)value); break;
		case 's':
			data->type.s = (short)(*(double *)value); break;
		case 'S':
			data->type.S = (unsigned short)(*(double *)value); break;
		case 'i':
			data->type.i = (int)(*(double *)value); break;
		case 'I':
			data->type.I = (unsigned int)(*(double *)value); break;
		case 'l':
			data->type.l = (long)(*(double *)value); break;
		case 'L':
			data->type.L = (unsigned long)(*(double *)value); break;
		case 'f':
			data->type.f = (float)(*(double *)value); break;
		case 'd':
			data->type.d = *(double *)value; break;
		case '*':
			cp = *(char **)value;
			data->type.str = malloc(strlen(cp) + 1);
			data->needToFreeStr = 1;
			strcpy(data->type.str, cp);
			break;

		case 'v':
			IoState_error_(IOSTATE, NULL, "attempt to setValue on void DataType");
			return IONIL(self);

		default:
			IoState_error_(IOSTATE, NULL, "unknown character '%c' in typeString", c);
			return IONIL(self);
	}
	return self;
}

IoObject *IoCInvokeDataType_objectFromData_(IoCInvokeDataType *self, void *data)
{
	char *typeString, c;
	IoCInvokePointer *pointer;

	typeString = CSTRING(IoState_on_doCString_withLabel_(IOSTATE, self, "typeString", "IoCInvokeDataType_objectFromData_"));
	//printf("_objectFromData: '%s'\n", typeString);

	switch (c = typeString[0])
	{
		case 'c':
		case 'C':
			return IoSeq_newWithCString_length_(IOSTATE, (char *)data, 1);
		case 's':
			return IONUMBER(((double)(*((short *)data))));
		case 'S':
			return IONUMBER(((double)(*((unsigned short *)data))));
		case 'i':
			return IONUMBER(((double)(*((int *)data))));
		case 'I':
			return IONUMBER(((double)(*((unsigned int *)data))));
		case 'l':
			return IONUMBER(((double)(*((long *)data))));
		case 'L':
			return IONUMBER(((double)(*((unsigned long *)data))));
		case 'f':
			return IONUMBER(((double)(*((float *)data))));
		case 'd':
			return IONUMBER((*((double *)data)));
		case 'v':
			return IONIL(self);

		case '*':
			if (*(char **)data)
			{
				return IoSeq_newWithCString_(IOSTATE, *(char **)data);
			}
			else
			{
				return IoSeq_new(IOSTATE);
			}

		case '^':
			pointer = IOCLONE(self);
			((IoCInvokePointerData *)(IoObject_dataPointer(pointer)))->ptr = *(void **)data;
			return pointer;

		case '{':
		case '(':
			IoState_error_(IOSTATE, NULL, "structs and unions are not yet supported by CInvoke");
			return IONIL(self);

		default:
			IoState_error_(IOSTATE, NULL, "unknown character '%c' in typeString", c);
			return IONIL(self);
	}
}

char IoCInvokeDataType_cinvType(IoCInvokeDataType *self)
{
	char *typeString, c;

	typeString = CSTRING(IoState_on_doCString_withLabel_(IOSTATE, self, "typeString", "IoCInvokeDataType_cinvType"));

	if (strlen(typeString) < 1)
	{
		return (char)0;
	}
	//printf("Typestring is '%s'\n", typeString);

	switch (c = typeString[0])
	{
		case 'c':
		case 'C':
			return 'c';
		case 's':
		case 'S':
			return 's';
		case 'i':
		case 'I':
			return 'i';
		case 'l':
		case 'L':
			return 'l'; // TODO: long long?
		case 'f':
			return 'f';
		case 'd':
			return 'd';
		case 'v':
//			IoState_error_(IOSTATE, NULL, "v got. no idea what's that.");
//			IOASSERT(0, "v got");
			return (char)0;
		case '*':
		case '^':
			return 'p';
		case '{':
			IoState_error_(IOSTATE, NULL, "passing structs is not yet supported by CInvoke. Try to pass a pointer?");
			return (char)0;
		case '(':
			IoState_error_(IOSTATE, NULL, "unions are not yet supported by CInvoke");
			return (char)0;

		default:
			IoState_error_(IOSTATE, NULL, "unknown character '%c' in typeString", c);
			return (char)0;
	}
}

cinv_type_t IoCInvokeDataType_cinvType_t(IoCInvokeDataType *self)
{
	char *typeString, c;

	typeString = CSTRING(IoState_on_doCString_withLabel_(IOSTATE, self, "typeString", "IoCInvokeDataType_cinvType"));

	if (strlen(typeString) < 1)
	{
		return (char)0;
	}

	//printf("CinvType_t typestring: '%s'\n", typeString);

	switch (c = typeString[0])
	{
		case 'c':
		case 'C':
			return CINV_T_CHAR;
		case 's':
		case 'S':
			return CINV_T_SHORT;
		case 'i':
		case 'I':
			return CINV_T_INT;
		case 'l':
		case 'L':
			return CINV_T_LONG; // TODO: long long?
		case 'f':
			return CINV_T_FLOAT;
		case 'd':
			return CINV_T_DOUBLE;
		case 'v':
			IoState_error_(IOSTATE, NULL, "v got. no idea what's that.");
//			IOASSERT(0, "v got");
			return (char)0;
		case '*':
		case '^':
			return CINV_T_PTR;

		//case '{':

		case '(':
			IoState_error_(IOSTATE, NULL, "unions are not yet supported by CInvoke");
			return (char)0;

		default:
			IoState_error_(IOSTATE, NULL, "unknown character '%c' in typeString t", c);
			return (char)0;
	}
}

void *IoCInvokeDataType_valuePointer(IoCInvokeDataType *self)
{
	char c, *typeString;
	IoCInvokeDataTypeData *data;

	typeString = CSTRING(IoState_on_doCString_withLabel_(IOSTATE, self, "typeString", "IoCInvokeDataType_valuePointer"));
	data = DATA(self);
	//printf("_valuePointer: '%s'\n", typeString);
	switch (c = typeString[0])
	{
		case 'c': return &(data->type.c);
		case 'C': return &(data->type.C);
		case 's': return &(data->type.s);
		case 'S': return &(data->type.S);
		case 'i': return &(data->type.i);
		case 'I': return &(data->type.I);
		case 'l': return &(data->type.l);
		case 'L': return &(data->type.L);
		case 'f': return &(data->type.f);
		case 'd': return &(data->type.d);
		case '^':
		case '*': return &(data->type.str);
		case 'v':
			IoState_error_(IOSTATE, NULL, "atempt to get data pointer from Void type");
			return NULL;

		default:
			IoState_error_(IOSTATE, NULL, "unknown character '%c' in typeString", c);
			return NULL;
	}
}

size_t getSizeFromSymbol(char symbol) {
	switch(symbol) {
		case 'c': return sizeof(char);
		case 'C': return sizeof(unsigned char);
		case 's': return sizeof(short);
		case 'S': return sizeof(unsigned short);
		case 'i': return sizeof(int);
		case 'I': return sizeof(unsigned int);
		case 'l': return sizeof(long);
		case 'L': return sizeof(unsigned long);
		case 'f': return sizeof(float);
		case 'd': return sizeof(double);
		case 'p': return sizeof(void*);
		case '\0': return 0;
		default:
			printf("unknown character '%c' to get size from\n", symbol);
			return 0;
	}
}

/*
switch (c)
{
	case 'c':
	case 'C':
	case 's':
	case 'S':
	case 'i':
	case 'I':
	case 'l':
	case 'L':
	case 'f':
	case 'd':
	case 'v':
	case '*':
	case '^':
	case '{':
	case '(':

	default:
		IoState_error_(IOSTATE, NULL, "unknown character '%c' in typeString", c);
		return NULL;
}
*/
