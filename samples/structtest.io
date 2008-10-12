CInvoke

lib := Library clone setName("./libstructtest.so")

MyStruct := Structure clone setLibrary(lib) addMember("a", Types SInt) addMember("b", Types CString) finish
#MyStruct := Structure clone setLibrary(lib) addMember("a", Types SInt) finish

testme := Function clone setLibrary(lib) setName("testme") setArgumentTypes(list(MyStruct ptr)) setReturnType(Types SInt)

cb := Callback clone setLibrary(lib) setArgumentTypes(list(Types SInt)) setReturnType(Types Void)
cb setMessage(cb, message(yeh))
cb yeh := method(a,
    writeln("YAY, IT WORKS, this should be 123: ", a)
)
test_callback := Function clone setLibrary(lib) setName("test_callback") setArgumentTypes(list(cb)) setReturnType(Types Void) 
//inst := StructureInstance setLibrary(lib) setStructure(MyStruct) setValue("a", 6) setValue("b", "Hello")
//testme call(Pointer clone toType(MyStruct) setValue(inst)) println
test_callback call(Pointer clone toType(cb) setValue(cb))
