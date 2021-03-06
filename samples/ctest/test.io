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
lib test_callback(cb ptr)
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
# Here's a little convenience again. It's possible to write the verbose one:
#struct setValue("a", 30)
# Or:
struct a := 30

struct setValue("b", "Hello C!")

# If you don't use `lib newFunction`, you have to
# explicitly call your function.
# The C library should now print the correct `a` and
# `b` member.
writeln("That should be 31: " .. test_struct call(struct ptr))

# That's a convenience method, used by `lib newFunction`, too.
test_struct_lazy := test_struct asMethod
writeln("That should be 31: " .. test_struct_lazy(struct ptr))

# We are able to receive structs from C, too.

lib newFunction("create_struct", list(Types Int), MyStruct Ptr)
inst := lib create_struct(123) value # it returns a pointer, so we have to get the value.
# It's now possible to get the values:
# The verbose one
writeln("A new structure instance arrived: " .. inst getValue("a"))
# The lazy one
writeln("A new structure instance arrived: " .. inst a)

inst b println

lib newFunction("t2", list(Types Int, Types CString), Types Void)
lib t2(33, "zwei");
