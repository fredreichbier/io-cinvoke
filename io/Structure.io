CInvoke Structure do(
    newSlot("library", Library)
    newSlot("typeString", "{")

    Ptr := method(
        Pointer toType(self)
    )

    instance := method(
        StructureInstance clone setStructure(self)
    )
)
