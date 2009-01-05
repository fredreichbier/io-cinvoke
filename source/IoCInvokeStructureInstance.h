//metadoc copyright Your Name Here 2008

// don't forget the macro guard
#ifndef IOStructureInstance_DEFINED
#define IOStructureInstance_DEFINED 1

#include "IoObject.h"
#include "IoCInvokeStructure.h"

// define a macro that can check whether an IoObject is of our type by checking whether it holds a pointer to our clone function
#define ISCInvokeStructureInstance(self) IoObject_hasCloneFunc_(self, (IoTagCloneFunc *)IoCInvokeStructureInstance_rawClone)

// declare a C type for ourselves
typedef IoObject IoCInvokeStructureInstance;

typedef struct IoCInvokeStructureInstance {
//	CInvStructure* structure;
	void* instance;
} IoCInvokeStructureInstanceData;

// define the requisite functions
IoTag *IoCInvokeStructureInstance_newTag(void *state);
IoObject *IoCInvokeStructureInstance_proto(void *state);
IoObject *IoCInvokeStructureInstance_rawClone(IoCInvokeStructureInstance *self);
IoObject *IoCInvokeStructureInstance_mark(IoCInvokeStructureInstance *self);
void IoCInvokeStructureInstance_free(IoCInvokeStructureInstance *self);

// define our custom C functions
IoObject *IoCInvokeStructureInstance_setValue(IoCInvokeStructureInstance *self, IoObject *locals, IoMessage *m);
IoObject *IoCInvokeStructureInstance_getValue(IoCInvokeStructureInstance *self, IoObject *locals, IoMessage *m);
IoObject *IoCInvokeStructureInstance_setLibrary(IoCInvokeStructureInstance *self, IoObject *locals, IoMessage *m);
void* IoCInvokeStructureInstance_valuePointer(IoCInvokeStructureInstance* self);
IoObject *IoCInvokeStructureInstance_objectFromData_(IoObject *structure, void *data);

#endif

