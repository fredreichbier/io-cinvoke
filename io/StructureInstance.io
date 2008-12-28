CInvoke StructureInstance do(
    newSlot("structure", Structure)

    ptr := method(
        structure Ptr setValue(self)
    )

    init := method(
        self setSlot := method(key, value,
            if(self structure hasSlot("memberTypes"),
                if(self structure memberTypes hasKey(key), # is a structure instance value
                    setValue(key, value)
                    ,
                    resend
                )
                ,
                resend
            )
        )
    )

    forward := method(
        # get a value lazily
        if(self structure hasSlot("memberTypes"),
            key := call message name
            if(self structure memberTypes hasKey(key), # is a structure instance value
                return(getValue(key))
                ,
                resend
            )
            ,
            resend
        )
    )
)
