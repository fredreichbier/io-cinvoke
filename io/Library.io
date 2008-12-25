/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

CInvoke Library do(
	newSlot("name")

	newFunction := method(name, arguments, rettype,
	    func := Function clone
	    func setLibrary(self)
	    func setName(name)
	    func setArgumentTypes(arguments)
	    func setReturnType(rettype)
	    self setSlot(name, func asMethod)
	    self
	)
)
