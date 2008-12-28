//metadoc TrivalObject copyright Your Name Here
//metadoc TrivalObject license BSD revised
//metadoc TrivalObject category Example
/*metadoc TrivalObject description
Describe your addon here.
*/

#include "IoState.h"
#include "IoMap.h"
#include "IoCInvokeStructureInstance.h"
#include "IoCInvokeLibrary.h"
#include "IoCInvokeDataType.h"

#define DATA(self) ((IoCInvokeStructureInstanceData *)(IoObject_dataPointer(self)))

// docDependsOn("CInvokeDataType")

// _tag makes an IoTag for the bookkeeping of names and methods for this proto
IoTag *IoCInvokeStructureInstance_newTag(void *state)
{
        // first allocate a new IoTag
        IoTag *tag = IoTag_newWithName_("StructureInstance");

        // record this tag as belonging to this VM
        IoTag_state_(tag, state);

        // give the tag pointers to the _free, _mark and _rawClone functions we'll need to use
        IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoCInvokeStructureInstance_free);
        IoTag_markFunc_(tag, (IoTagMarkFunc *)IoCInvokeStructureInstance_mark);
        IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoCInvokeStructureInstance_rawClone);
        return tag;
}

// _proto creates the first-ever instance of the prototype 
IoObject *IoCInvokeStructureInstance_proto(void *state)
{
        // First we allocate a new IoObject
        IoCInvokeStructureInstance *self = IoObject_new(state);

        // Then tag it
        IoObject_tag_(self, IoCInvokeStructureInstance_newTag(state));
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeStructureInstanceData)));
        
	// then register this proto generator
        IoState_registerProtoWithFunc_(state, self, IoCInvokeStructureInstance_proto);

        // and finally, define the table of methods this proto supports
        // we just have one method here, returnSelf, then terminate the array
        // with NULLs
        {
                IoMethodTable methodTable[] = {
			        {"setValue", IoCInvokeStructureInstance_setValue},
			        {"setLibrary", IoCInvokeStructureInstance_setLibrary},
                    {"getValue", IoCInvokeStructureInstance_getValue},
                	{NULL, NULL},
                };
                IoObject_addMethodTable_(self, methodTable);
        }

        return self;
}

// _rawClone clones the existing proto passed as the only argument
IoObject *IoCInvokeStructureInstance_rawClone(IoCInvokeStructureInstance *proto)
{
        IoObject *self = IoObject_rawClonePrimitive(proto);
	IoObject_setDataPointer_(self, calloc(1, sizeof(IoCInvokeStructureInstanceData)));

        return self;
}

// _new creates a new object from this prototype
IoObject *IoCInvokeStructureInstance_new(void *state)
{
        IoObject *proto = IoState_protoWithInitFunction_(state, IoCInvokeStructureInstance_proto);
        return IOCLONE(proto);
}

// _mark is called when this proto is marked for garbage collection
// If this proto kept references to any other IoObjects, it should call their mark() methods as well.
IoObject *IoCInvokeStructureInstance_mark(IoCInvokeStructureInstance* self)
{
        return self;
}

// _free defines any cleanup or deallocation code to run when the object gets garbage collected
void IoCInvokeStructureInstance_free(IoCInvokeStructureInstance *self)
{
	if(DATA(self)->context) {
		cinv_structure_delete_instance(DATA(self)->context, DATA(self)->instance);
	}
	free(DATA(self));
}

IoObject *IoCInvokeStructureInstance_setValue(IoCInvokeStructureInstance *self, IoObject *locals, IoMessage *m)
{
	CInvContext* context = DATA(self)->context;
	CInvStructure* structure = IoCInvokeStructure_getStructure_(IoObject_getSlot_(self, IOSYMBOL("structure")));
    if(!DATA(self)->instance) {
		DATA(self)->instance = cinv_structure_create_instance(context, structure);
	}
	char* name = IoMessage_locals_cStringArgAt_(m, locals, 0);
	void* value = IoCInvokeDataType_ValuePointerFromObject_(IoMessage_locals_valueArgAt_(m, locals, 1));
	if(!cinv_structure_instance_setvalue(context, structure, DATA(self)->instance, name, value)) {
		printf("Something went wrong with StructureInstance setValue\n");
	};
	return self;
}

IoObject *IoCInvokeStructureInstance_getValue(IoCInvokeStructureInstance *self, IoObject *locals, IoMessage *m) {
 	CInvContext* context = DATA(self)->context;
	CInvStructure* structure = IoCInvokeStructure_getStructure_(IoObject_getSlot_(self, IOSYMBOL("structure")));
    if(!DATA(self)->instance) {
		DATA(self)->instance = cinv_structure_create_instance(context, structure); // ... is that intelligent?
	}
	char* name = IoMessage_locals_cStringArgAt_(m, locals, 0);
	void* value = cinv_structure_instance_getvalue(context, structure, DATA(self)->instance, name);
    IoObject *memberType = IoMap_rawAt(IoObject_getSlot_(
                IoObject_getSlot_(self, IOSYMBOL("structure")), IOSYMBOL("memberTypes")), 
                IOSYMBOL(name));
    IoObject *io_value = IoCInvokeDataType_objectFromData_(memberType, value);
    return io_value;
}

IoObject *IoCInvokeStructureInstance_setLibrary(IoCInvokeStructureInstance *self, IoObject *locals, IoMessage *m)
{
	IoObject* lib = IoMessage_locals_valueArgAt_(m, locals, 0);
	DATA(self)->context = IoCInvokeLibrary_getContext_((IoCInvokeLibrary*)lib);
	return self;
}

void* IoCInvokeStructureInstance_valuePointer(IoCInvokeStructureInstance* self) {
	return DATA(self)->instance;
}

IoObject *IoCInvokeStructureInstance_objectFromData_(IoObject *structure, void *data) {
    IoObject *self = IoCInvokeStructureInstance_new(((IoState *)(IoObject_tag(structure)->state))); // because IOSTATE depends on `self`
    DATA(self)->context = getGlobalContext();
    DATA(self)->instance = data;
    IoObject_setSlot_to_(self, IOSYMBOL("structure"), structure);
    return self;
}
