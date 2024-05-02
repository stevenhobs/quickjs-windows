# QuickJS Windows

This project is to build **qjs.exe** and **qjsc.exe** on Windows.

## Feature

- CMake with ninja-build tool and mingw64 toolchain are enough. Do not need Msys2 environment.
- Both support shared and static lib build.
- qjsc.exe with MinGW toolchain. You can also use C module for your qjs exe.

## Usage

## Build

1. Install tools

- mingw64 gcc > 8.1.0 <https://github.com/niXman/mingw-builds-binaries/releases>
- CMake > 3.6.0 <https://github.com/Kitware/CMake/releases>
- Ninja > 1.10.0 <https://github.com/ninja-build/ninja/releases>

  > Ensure these bin-folders in the **PATH** environment variety.

2. Clone this repo  `git clone --depth=1 https://github.com/stevenhobs/quickjs-windows.git`
3. Open the terminal from the repo position
4. build commands

   ```powershell
   cd quickjs-windows

   # check cmake tool
   where.exe cmake
   # check ninja
   where.exe ninja
   # check gcc
   where.exe gcc

   # pull required repositories
   git submodule update --init --depth=1

   # modify configuration about qjs
   notepad CMakeLists.txt

   # generate cmake build cache
   cmake -B build -DCC=gcc -G Ninja

   # build targets
   cmake --build build -v

   # install targets
   cmake --install build
   ```

5. copy the target lib and exe files
   ```powershell
   # Open File Explorer
   exlorer.exe .\install
   ```
   choose files about quickjs. Usually they are qjs.exe,qjsc.exe and libquckjs.dll(if you build shared lib)
   > Note that some mingw64 sdk have shared stdlib dlls at the _bin_ folder of MinGW installed directory.
   > If you want to share your build targets with others. You should also take the copy of these dll files.
   > For example: `libgcc_s_seh-1.dll`, `libwinpthread-1.dll`

## LICENSE

`MIT`
