CFFI

libgtk := Library clone do (
    setName("libgtk-x11-2.0.so")
    newFunction("gtk_init", list(Types UInt ptr, Types CString ptr), Types Void)
    newFunction("gtk_window_new", list(Types UInt), Types Ptr)
    newFunction("gtk_window_set_title", list(Types Ptr, Types CString), Types Void)
    newFunction("gtk_widget_show", list(Types Ptr), Types Void)
    newFunction("gtk_main", list(Types Ptr), Types Void)
)

libgtk gtk_init call(0, "") 
win := libgtk gtk_window_new call(0)
libgtk gtk_window_set_title call(win, "Hello world!")
libgtk gtk_widget_show call(win)
libgtk gtk_main call
