# Hacklib #

Hacklib is a C++ library for building applications that run as a shared library in another application. It provides general purpose functionality like pattern scanning, hooking and laying out foreign classes. Additionally it contains some D3D drawing facilities and a high performance 3D-capable transparent overlay.

## Features ##

### Main.h ###

A helper / framework for implementing a DLL that runs by itself after it is loaded into the target process.


```
#!c++

class MyMain : public hl::Main
{
public
    bool init() override
    {
        // Your init code here. Return true to loop on step member function afterwards.
        return false;
    }
};

hl::StaticInit<MyMain> g_main;
```


### WindowOverlay.h ###

A high performance 3D-capable transparent overlay for Windows. Will follow a target window and always draw on top of it with the ability have transparency on the overlay. Can be rendered to with D3D. Requires the compositiing window manager introduced with Windows Vista (always active since Windows 8).

For an example see Drawer.h section.

### Drawer.h ###

Wrapper for drawing with D3D in a resource-safe C++ way.


```
#!c++

hl::Drawer drawer;
hl::WindowOverlay overlay;

if (overlay.create() != hl::WindowOverlay::Error::Success)
    return false;

auto pDev = overlay.getDev();
overlay.registerResetHandlers(std::bind(&hl::Drawer::OnLostDevice, &drawer), std::bind(&hl::Drawer::OnResetDevice, &drawer));
drawer.SetDevice(pDev);

while (pDev->BeginScene() == D3D_OK)
{
    overlay.clearRenderTarget();
    drawer.DrawCircle(100, 100, 30, 0xaacc6633);
    pDev->EndScene();
    pDev->Present(0, 0, 0, 0);
}
```


### Hooker.h ###

Implements various hooking methods like simple JMP redirection, virtual table hooks and vectored exception handler hooking.

### PatternScanner.h ###

Provides pattern scanning techniques like masked search strings or search by referenced strings in the code of the target process.


```
#!c++

uintptr_t MapIdSig = hl::FindPattern("\00\x00\x08\x00\x89\x0d", "xxxxxx");

hl::PatternScanner scanner;
auto results = scanner.find({
    "ViewAdvanceDevice",
    "ViewAdvanceAgentSelect",
    "ViewAdvanceAgentView",
    "ViewAdvanceWorldView",
    "character->IsAlive() || (character->IsDowned() && character->IsInWater())"
});

void *pAgentSelectionCtx = *(void**)(hl::FollowRelativeAddress(results[1] + 0xa) + 0x1);
```


### Injector.h ###

A way to forcibly get your shared library into the target process.

### ConsoleEx.h ###

A high performance Windows console that accepts input and output simultaneously.

### D3DDeviceFetcher.h ###

Finds the D3D device used for rendering by the host application in a generic way.


```
#!c++

auto pDev = hl::D3DDeviceFetcher::GetD3D9Device();
```


### ForeignClass.h ###

Helper class for accessing a foreign class dynamically at runtime, including doing virtual member function calls.


```
#!c++

void *ptr = 0x12345678;
hl::ForeignClass obj = ptr;
int result = obj.call<int>(0x18, "Hello World", 3.14f, 42);
double value = obj.get<double>(0x6c);
obj.set(0x70, value+7);
```


### ImplementMember.h ###

Macros for declaring a foreign class statically in a type-safe, const-safe and simply conventient manner.


```
#!c++

class CPlayer
{
    // Declare virtual table function by offset
    IMPLVTFUNC(double, someFunc, 0x1c, int, id, double, speed);
    // Declare virtual table function by ordinal
    IMPLVTFUNC_OR(void, boop, 3);
    // Declare member variable by offset
    IMPLMEMBER(int, Id, 0x10);
    // Declare member variable by relative offset
    IMPLMEMBER_REL(D3DXVECTOR3, Pos, 0x8, Id);
    IMPLMEMBER_REL(float, Hp, 0, Pos);
};
```


### Utility ###

These are not really related to the topic of this library, but might often be used in a program built from this library.

Rng.h:

```
#!c++

hl::Rng rng;

while (true)
{
    // From 0 to 99
    int a = rng.nextInt(100);
    // From 7.1 to 10.3
    auto b = rng.nextReal(7.1, 10.3);
}
```

Timer.h:

```
#!c++

hl::Timer t;
// Some computation.
std::cout << t.diff() << std::endl;
t.reset();
// Another computation.
std::cout << t.diff() << std::endl;
```


## Dependencies ##

Hacklib is written in modern C++ and requires a recent compiler like Visual Studio 2013, GCC 4.8 or Clang 3.3 with C++11 support enabled.

The project is using CMake and CMake 2.8.11.2 or newer is required to build.

## How to build ##

* Run CMake
* Build the project that was generated by CMake

## Contribute ##

Please use the issue tracker and submit pull requests to contribute.