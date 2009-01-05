/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

// docDependsOn("CInvokeStructure")

#include "IoCInvokeStructure.h"
#include "IoCInvokeLibrary.h"
#include "IoCInvokeDataType.h"
#include "IoMap.h"
#include <cinvoke.h>

#define DATA(self) ((IoCInvokeStructureData *)(IoObject_dataPointer(self)))

IoTag *IoCInvokeStructure_newTag(void *state)
{
	IoTag *tag = IoTag_newWithName_("Structure");
	IoTag_state_(tag, state);
	IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoCInvokeStructure_free);
	IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoCInvokeStructure_rawClone);
	return tag;
}

IoCInvokeStructure *IoCInvokeStructure_proto(void *state)
{
	IoObject *self = IoObject_new(state);
	IoObject_tag_(self, IoCInvokeStructure_newTag(state));

	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeStructureData)));
	DATA(self)->types = List_new();
	IoState_registerProtoWithFunc_(state, self, IoCInvokeStructure_proto);

	{
		IoMethodTable methodTable[] = {
			{"addMember", IoCInvokeStructure_addMember},
			{"finish", IoCInvokeStructure_finish},
			{NULL, NULL},
		};
		IoObject_addMethodTable_(self, methodTable);
	}

	return self;
}

IoCInvokeStructure *IoCInvokeStructure_rawClone(IoCInvokeStructure *proto)
{
	IoObject *self = IoObject_rawClonePrimitive(proto);
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeStructureData)));
    IoObject_setSlot_to_(self, IOSYMBOL("memberTypes"), IoMap_new(IOSTATE));
	return self;
}

void IoCInvokeStructure_free(IoCInvokeStructure *self)
{
	if(DATA(self)->structure) {
		free(DATA(self)->structure);
	}
	if(DATA(self)->types) {
		List_free(DATA(self)->types);
	}
	free(DATA(self));
}

/* format: addMember(name, type) */
IoObject* IoCInvokeStructure_addMember(IoCInvokeStructure* self, IoObject* locals, IoMessage* m) {
	CInvContext* context = IoCInvokeLibrary_getContext_(IoObject_getSlot_(self, IOSYMBOL("library")));
	if(!(DATA(self)->structure)) {
		DATA(self)->structure = cinv_structure_create(context);
	}
	char* name = IoMessage_locals_cStringArgAt_(m, locals, 0);
    IoObject *typeobj = IoMessage_locals_valueArgAt_(m, locals, 1);
    
    if(ISCInvokeStructure(typeobj)) { /* adding a structure member! */
        if(!cinv_structure_addmember_struct(context, DATA(self)->structure, name, DATA(typeobj)->structure)) {
            printf("error with adding struct member\n");
        }    
    } else { /* hopefully adding a DataType */
        cinv_type_t type = IoCInvokeDataType_cinvType_t(typeobj);
        if(!cinv_structure_addmember_value(context, DATA(self)->structure, name, type)) {
            printf("error with adding value member\n");
        }
    }

    // append it to the `memberTypes` Map
    IoObject *memberTypes = IoObject_getSlot_(self, IOSYMBOL("memberTypes"));
    IoMap_rawAtPut(memberTypes, IoMessage_locals_valueArgAt_(m, locals, 0), IoMessage_locals_valueArgAt_(m, locals, 1));

	return self;
}

IoObject* IoCInvokeStructure_finish(IoCInvokeStructure* self, IoObject* locals, IoMessage* m) {
	CInvContext* context = IoCInvokeLibrary_getContext_(IoObject_getSlot_(self, IOSYMBOL("library")));
	if(!(DATA(self)->structure)) {
		DATA(self)->structure = cinv_structure_create(context);
	}
	cinv_structure_finish(context, DATA(self)->structure);

	return self;
}

CInvStructure* IoCInvokeStructure_getStructure_(IoCInvokeStructure* self) {
	return DATA(self)->structure;
}
