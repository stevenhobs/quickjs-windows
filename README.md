# QuickJS Windows

This project is to build **qjs.exe** and **qjsc.exe** on Windows.

## Feature

- CMake build tool and mingw64 toolchain are enough. Do not need Msys2 environment.
- Both support shared and static lib build.
- qjsc.exe with MinGW toolchain. You can also use C module for your qjs exe.

## Usage

## Build

1. Install tools

- CMake > 3.6.0
- mingw64 gcc > 8.1.0

  > Ensure you have the **make.exe** or **ninja.exe** tool in the PATH enviroment

2. Clone this repo
3. Open the terminal from the repo position
4. build commands

   ```powershell
   cd quickjs-windows

   # check cmake tool
   where.exe cmake
   # check gcc
   where.exe gcc

   # pull required repositories
   git submodule update --depth 1

   # modify configuration about qjs
   notepad CMakeLists.txt

   # generate cmake build cache
   cmake -B build -DCC=gcc -G Ninja
   # If you use Make tool, use this:
   # cmake -B build -DCC=gcc -G "Unix Makefiles"
   cmake --build build -DCMAKE_BUILD_TYPE=Release

   # install
   cmake install build
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
