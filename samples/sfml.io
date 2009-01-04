CInvoke

system := Library clone do(
    setName("/usr/lib/libcsfml-system.so")
    explicitLoad
)

window := Library clone do(
    setName("/usr/lib/libcsfml-window.so")
    explicitLoad
)

graphics := Library clone do(
    setName("/usr/lib/libcsfml-graphics.so")
    explicitLoad

    newFunction("sfRenderWindow_Create", list(Types UInt, Types UInt, Types UInt, /* mode */
                Types CString /* title */, Types ULong /* style */,
                Types UInt, Types UInt, Types UInt /* settings */),
                Types Ptr)
)

render_window := graphics sfRenderWindow_Create(800, 600, 32, "Hello World!", 0, 24, 8, 0)
