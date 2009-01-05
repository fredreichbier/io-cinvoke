/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

CInvoke Function do(
	newSlot("library", Library)
	newSlot("name", "")
	newSlot("returnType", "")
	newSlot("argumentTypes", "")

    asMethod := method(
        bl := block(
            call message setName("call") asMessageWithEvaluatedArgs(call sender) doInContext(self)
        ) 
        bl setScope(self) 
        bl setIsActivatable(true)
    )
)
