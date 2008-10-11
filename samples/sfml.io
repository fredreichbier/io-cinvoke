CFFI

SFML := Object clone do (
    Types := CFFI Types clone do (
    
    )
    _functions := Map clone

    initi := method(
	self lib := Library clone setName("/usr/lib/libcsfml-window.so")
	appendProto(SFML Types)

	_functions do(

	    add := method(name, retType, argTypeList,
		atPut(name, Function clone setLibrary(SFML lib) setName(name) setReturnType(retType) setArgumentTypes(argTypeList)) 
	    )
	    
//	    add("sfWindow_Create", Long, list(CString))
	)
	self sfVideoMode := Structure clone setLibrary(lib) addMember("Width", SInt) addMember("Height", SInt) addMember("BitsPerPixel", SInt) finish
    )

    forward := method(
    m := Message clone setName("call") setArguments(call message arguments)
    m doInContext(_functions at(call message name))
    )
)

SFML initi
vm := StructureInstance clone setLibrary(SFML lib) setStructure(SFML sfVideoMode) setValue("Width", 640)
create := SFML sfWindow_Create
