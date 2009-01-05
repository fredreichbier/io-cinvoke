CInvoke

sdl := Library clone do(
    setName("libSDL.so")
)

sdl SDL_Rect := Structure clone do(
    setLibrary(sdl)
    addMember("x", Types Int) 
    addMember("y", Types Int)
    addMember("w", Types UInt)
    addMember("h", Types UInt)
    finish
)

sdl SDL_Surface := Structure clone do(
    setLibrary(sdl)

    addMember("flags", Types ULong) # UInt32?
    addMember("format", Types Ptr)
    addMember("w", Types Int)
    addMember("h", Types Int)
    addMember("pitch", Types UInt) # UInt16?
    addMember("pixels", Types Ptr)
    addMember("clip_rect", sdl SDL_Rect)
    addMember("refcount", Types Int)
    finish
)

sdl do(
    newFunction("SDL_Init", list(Types UInt), Types Int)
    newFunction("SDL_Quit", list(), Types Void)

    newFunction("SDL_SetVideoMode", list(Types Int, Types Int, Types Int, Types UInt), SDL_Surface Ptr)
    # SDL_LoadBMP is a macro. workaround follows.
    #define SDL_LoadBMP(file)	SDL_LoadBMP_RW(SDL_RWFromFile(file, "rb"), 1)
    newFunction("SDL_LoadBMP_RW", list(Types Ptr, Types Int), SDL_Surface Ptr)
    newFunction("SDL_RWFromFile", list(Types CString, Types CString), Types Ptr)
    SDL_LoadBMP := method(filename,
        rwops := SDL_RWFromFile(filename, "rb")
        SDL_LoadBMP_RW(rwops, 1)
    )

    newFunction("SDL_FreeSurface", list(SDL_Surface Ptr), Types Void)
    newFunction("SDL_DisplayFormat", list(SDL_Surface Ptr), SDL_Surface Ptr)

    # SDL_BlitSurface is a macro pointing to SDL_UpperBlit.
    newFunction("SDL_UpperBlit", list(SDL_Surface Ptr, SDL_Rect Ptr, SDL_Surface Ptr, SDL_Rect Ptr), Types Int)

    newFunction("SDL_Flip", list(SDL_Surface Ptr), Types Int)
    newFunction("SDL_Delay", list(Types UInt), Types Void)

)

sdl SDL_Init(32)
screen := sdl SDL_SetVideoMode(640, 480, 16, 1073741824) # SDL_DOUBLEBUF
temp := sdl SDL_LoadBMP("sample.bmp")
image := sdl SDL_DisplayFormat(temp)
sdl SDL_FreeSurface(temp)

/*
# The following doesn't work ... :/
src := sdl SDL_Rect instance do(
    x := 0
    y := 0
    w := image value w
    h := image value h
)

dest := sdl SDL_Rect instance do(
    x := 0  
    y := 0
    w := image value w
    h := image value h
)

sdl SDL_UpperBlit(image, src ptr, screen, dest ptr)
*/

sdl SDL_UpperBlit(image, nil, screen, nil)
sdl SDL_Flip(screen)

sdl SDL_Delay(2000)
sdl SDL_Quit
