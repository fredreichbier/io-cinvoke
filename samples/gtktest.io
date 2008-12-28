CInvoke

ev := Callback clone setArgumentTypes(list(Types Ptr, Types Ptr, Types Ptr)) setReturnType(Types SInt)

libgtk := Library clone do (
    setName("libgtk-x11-2.0.so")
    newFunction("gtk_init", list(Types UInt ptr, Types CString ptr), Types Void)
    newFunction("gtk_window_new", list(Types UInt), Types Ptr)
    newFunction("gtk_window_set_title", list(Types Ptr, Types CString), Types Void)
    newFunction("gtk_widget_show", list(Types Ptr), Types Void)
    newFunction("gtk_main", list(), Types Void)
    newFunction("gtk_main_quit", list(), Types Void)
)

libgobject := Library clone do (
    setName("libgobject-2.0.so")
    newFunction("g_signal_connect_data", list(Types Ptr, Types CString, Callback Ptr, Types Ptr, Types Ptr, Types UInt), Types Long)
)

#libgtk gtk_init
libgtk gtk_init(0, "") 
win := libgtk gtk_window_new(0)
libgtk gtk_window_set_title(win, "Hello world!")
libgtk gtk_widget_show(win)
ev setLibrary(libgobject) setMessage(ev, message(destroyed))

ev destroyed := method(
    "Io Handler invoked!" println
#    libgtk gtk_main_quit # that crashes.
    return(0)
)

libgobject g_signal_connect_data(win, "delete-event", ev ptr, nil, nil, 0) println
libgtk gtk_main
