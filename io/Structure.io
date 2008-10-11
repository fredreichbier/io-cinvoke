CInvoke Structure do(
    newSlot("library", Library)
    newSlot("typeString", "{")
    init := method(
	    self ptr := method(
		    Pointer toType(self)
	    )
	)
)
