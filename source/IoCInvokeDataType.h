/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

#ifndef IoCInvokeDataType_DEFINED
#define IoCInvokeDataType_DEFINED 1

#include "IoObject.h"
#include <cinvoke.h>

#define ISCInvokeDataType(self) IoObject_hasCloneFunc_(self, (IoTagCloneFunc *)IoCInvokeDataType_rawClone)

typedef IoObject IoCInvokeDataType;

typedef struct IoCInvokeDataType
{
	union
	{
	          char c;
	 unsigned char C;
	         short s;
	unsigned short S;
	           int i;
	  unsigned int I;
	          long l;
	 unsigned long L;
	         float f;
	        double d;
	       char *str;
	} type;
	int needToFreeStr;
} IoCInvokeDataTypeData;

IoTag *IoCInvokeDataType_newTag(void *state);
IoCInvokeDataType *IoCInvokeDataType_proto(void *state);
IoCInvokeDataType *IoCInvokeDataType_rawClone(IoCInvokeDataType *self);
IoCInvokeDataType *IoCInvokeDataType_new(void *state);
void IoCInvokeDataType_free(IoCInvokeDataType *self);

void *IoCInvokeDataType_ValuePointerFromObject_(IoObject *o);

IoCInvokeDataType *IoCInvokeDataType_ptr(IoCInvokeDataType *self, IoObject *locals, IoMessage *m);
IoCInvokeDataType *IoCInvokeDataType_value(IoCInvokeDataType *self, IoObject *locals, IoMessage *m);
IoCInvokeDataType *IoCInvokeDataType_setValue(IoCInvokeDataType *self, IoObject *locals, IoMessage *m);

char IoCInvokeDataType_cinvType(IoCInvokeDataType *self);
IoObject *IoCInvokeDataType_objectFromData_(IoCInvokeDataType *self, void *data);
void *IoCInvokeDataType_valuePointer(IoCInvokeDataType *self);
size_t getSizeFromSymbol(char symbol);
cinv_type_t IoCInvokeDataType_cinvType_t(IoCInvokeDataType *self);

#endif
