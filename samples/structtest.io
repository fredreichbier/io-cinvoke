CInvoke

lib := Library clone setName("./libstructtest.so")

MyStruct := Structure clone setLibrary(lib) addMember("a", Types SInt) addMember("b", Types CString) finish
#MyStruct := Structure clone setLibrary(lib) addMember("a", Types SInt) finish

testme := Function clone setLibrary(lib) setName("testme") setArgumentTypes(list(MyStruct ptr)) setReturnType(Types SInt)

inst := StructureInstance setLibrary(lib) setStructure(MyStruct) setValue("a", 6) setValue("b", "Hello")
testme call(Pointer clone toType(MyStruct) setValue(inst)) println
