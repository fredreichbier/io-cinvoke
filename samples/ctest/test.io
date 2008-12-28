# That's a little example on using CInvoke.
# It can be useful to read `testlib.c` first.
# To run that example, compile `testlib.c` to
# a shared library using `make`. Then start 
# `test.io`.

CInvoke

# First, we load the library.

lib := Library clone setName("./libtestlib.so")

# Let's test callbacks!

cb := Callback clone do( 
    setLibrary(lib)
    setArgumentTypes(list(Types SInt))
    setReturnType(Types SInt)
)

cb setMessage(cb, message(io_callback))
cb io_callback := method(a,
    writeln("This should be 123: ", a)
    456
)

# There is a convenience method in Library:
lib newFunction("test_callback", list(Callback),   Types Void)
#               ^^ name          ^^ argument types ^^ return type

# Now you can call it like a regular method.
lib test_callback(Pointer clone toType(cb) setValue(cb))
# Desired output:
# 
# This should be 123: 123
# That should be 456: 456

# Let's test structure passing!
MyStruct := Structure clone do (
    setLibrary(lib)
    addMember("a", Types SInt)
    addMember("b", Types CString)
    finish
)

# That's the old way of function definition.
test_struct := Function clone do(
    setLibrary(lib)
    setName("test_struct")
    setArgumentTypes(list(MyStruct Ptr))
    setReturnType(Types SInt)
)

# Let's make a structure instance. That's not very elegant at the moment.
struct := StructureInstance clone setLibrary(lib) setStructure(MyStruct)
struct setValue("a", 30)
struct setValue("b", "Hello C!")

# If you don't use `lib newFunction`, you have to
# explicitly call your function.
# The C library should now print the correct `a` and
# `b` member.
writeln("That should be 31: " .. test_struct call(struct ptr))

# That's a convenience method, used by `lib newFunction`, too.
test_struct_lazy := test_struct asMethod
writeln("That should be 31: " .. test_struct_lazy(struct ptr))

# We will be able to receive structs from C, too.
# TODO.

lib newFunction("create_struct", list(), MyStruct Ptr)
lib create_struct value println

