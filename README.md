# ncconv

Utility program for converting meteorological data sets to the NetCDF format.


## Usage

```shell
./ncconv -i <input-path> -o <output-path>
```

For the input file, currently only `.ctl` [GrADS files](http://cola.gmu.edu/grads/gadoc/descriptorfile.html) are
supported. The output file should end with `.nc`.


## Building and running the programm

### Linux

There are two ways to build the program on Linux systems.
- Using the system package manager to install the dependencies (tested: apt on Ubuntu, pacman on Arch Linux).
- Using [vcpkg](https://github.com/microsoft/vcpkg) to install the dependencies.

The script `build.sh` in the project root directory can be used to build the project. If no arguments are passed, the
dependencies are installed using the system package manager. When calling the script as `./build.sh --vcpkg`, vcpkg is
used instead. The build scripts will also launch the program after successfully building it. If you wish to build the
program manually, instructions can be found in the directory `docs/compilation`.

Below, more information concerning different Linux distributions tested can be found.

#### Arch Linux

Arch Linux and its derivative Manjaro are fully supported using both build modes (package manager and vcpkg).

#### Ubuntu 18.04, 20.04 & 22.04

Ubuntu 22.04 is fully supported.

Please note that Ubuntu 18.04 and 20.04 ship a too old version of CMake, which causes the build process to fail.
In this case, CMake needs to be upgraded manually beforehand using the steps at https://apt.kitware.com/.

#### Other Linux Distributions

If you are using a different Linux distribution and face difficulties when building the program, please feel free to
open a [bug report](https://github.com/chrismile/ncconv/issues).


### Windows

There are two ways to build the program on Windows.
- Using [vcpkg](https://github.com/microsoft/vcpkg) to install the dependencies. The program can then be compiled using
  [Microsoft Visual Studio](https://visualstudio.microsoft.com/vs/).
- Using [MSYS2](https://www.msys2.org/) to install the dependencies and compile the program using MinGW.

In the project folder, a script called `build-msvc.bat` can be found automating this build process using vcpkg and
Visual Studio. It is recommended to run the script using the `Developer PowerShell for VS 2022` (or VS 2019 depending on
your Visual Studio version). The build script will also launch the program after successfully building it.
Building the program is regularly tested on Windows 10 and 11 with Microsoft Visual Studio 2019 and 2022.

The script `build.sh` in the project root directory can also be used to alternatively build the program using
MSYS2/MinGW on Windows. For this, it should be run from a MSYS2 shell.

If you wish to build the program manually using Visual Studio and vcpkg, or using MSYS2, instructions can be found in
the directory `docs/compilation`.
