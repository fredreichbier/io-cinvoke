Callback do (
    newSlot("library")
    newSlot("argumentTypes")
    newSlot("returnType")
    newSlot("typeString", "^")

    Ptr := method(
        Pointer clone toType(self)
    )

    init := method(
        self ptr := method(
            Ptr setValue(self)
        )
        resend
    )
)
