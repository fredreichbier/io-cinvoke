/* CInvoke - An Io interface to C
   Copyright (c) 2008 Friedrich Weber
   heavily based on CFFI, which is
   Copyright (c) 2006 Trevor Fancher. All rights reserved
   All code licensed under the New BSD license.
 */

CInvoke do(
	Pointer do(
		init := method(nil)

		ptr := method(
			if(self isType,
				toType(self)
			,
				self proto ptr
			)
		)

		isType := method(
			self hasLocalSlot("pointedToType")
		)

		typeString := method(
			"^" .. self pointedToType typeString
		)
	)
)
