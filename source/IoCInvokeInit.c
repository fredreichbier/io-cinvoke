#include "IoState.h"
#include "IoObject.h"

IoObject *IoCInvokeLibrary_proto(void *state);
IoObject *IoCInvokeDataType_proto(void *state);
IoObject *IoCInvokeStructureInstance_proto(void *state);
IoObject *IoCInvokePointer_proto(void *state);
IoObject *IoCInvokeStructure_proto(void *state);
IoObject *IoCInvokeFunction_proto(void *state);

void IoCInvokeInit(IoObject *context)
{
	IoState *self = IoObject_state((IoObject *)context);

	IoObject_setSlot_to_(context, SIOSYMBOL("CInvokeLibrary"), IoCInvokeLibrary_proto(self));

	IoObject_setSlot_to_(context, SIOSYMBOL("CInvokeDataType"), IoCInvokeDataType_proto(self));

	IoObject_setSlot_to_(context, SIOSYMBOL("CInvokeStructureInstance"), IoCInvokeStructureInstance_proto(self));

	IoObject_setSlot_to_(context, SIOSYMBOL("CInvokePointer"), IoCInvokePointer_proto(self));

	IoObject_setSlot_to_(context, SIOSYMBOL("CInvokeStructure"), IoCInvokeStructure_proto(self));

	IoObject_setSlot_to_(context, SIOSYMBOL("CInvokeFunction"), IoCInvokeFunction_proto(self));

}
