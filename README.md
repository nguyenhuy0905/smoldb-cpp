# Cmake template (2.0)

> A rewrite of this CMake template, trimming away some "bloat"

## What's this

- A somewhat minimalist CMake project configuration.

## Disclaimer

- Limited testing was done for MSVC (Visual Studio). While it is expected that
this works out of the box for Visual Studio, in reality, it may not be so simple.

## What's included

- Integration with multiple tools:
  - Formatters & linters:
    - clang-format
    - clang-tidy
    - cmake-format
  - Unit test (GTest), fuzz test (libFuzzer)
  - Sanitizers (ASan, UBSan, MSan, TSan)
  - Download dependencies with conan
  - ccache
  - lld
- Convenient commands in [the Makefile](./Makefile)
- Basic CPack configuration

## How to use

- If you want to use conan, first, set up conan [by Reading The Friendly Manual](https://docs.conan.io/2/installation.html):
- Then install packages,

```bash
conan install <project source dir> --build=missing -s build_type=<your build type>
```

- Then generate the CMake

```bash
# --preset conan-release if building release
cmake -B <your build dir> --preset conan-debug \
    -DCMAKE_CXX_COMPILER=<the compiler> \
    -DCMAKE_C_COMPILER=<the compiler> \
    -G <generator> \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

## What to change

- At the very least, the project name.
  - If you know how to use `sed`, this is extremely easy:

  ```sh
  #!/bin/sh

  for cmake_file in `find . -name '*.cmake' -or -name 'CMakeLists.txt'`; do
    sed -i "s/myproj/<your project name>/g" $cmake_file
  done
  ```

- If this is a top-level project, and you want these features:
  - [The install configuration](./cmake/InstallConfig.cmake)
  - [The packing configuration](./cmake/PackConfig.cmake)

- If you use other dependencies:
  - [The conanfile](./conanfile.py)

## Coming soon

- Google benchmark integration
