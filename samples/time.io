CInvoke

libc := Library clone setName("libc.so.6")
time_t := Types Long clone
time := Function clone setLibrary(libc) setName("time") setArgumentTypes(list(time_t ptr)) setReturnType(time_t)

time call println
dest := 0
time call(dest)
dest println
