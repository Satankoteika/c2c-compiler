
# C2C Compiler

This repository contains the source code for 2cc, a compiler designed with the basic implementation of the C2C language in mind. 2cc is specifically designed for compiling programs written in the c2c programming language.

The 2cc compiler has been designed to provide high performance and efficiency while maintaining simplicity and usability. With 2cc, you can easily compile your programs written in the c2c programming language, getting executable files optimized for your target platform.

This repository also includes the с2с Standard Language Library (lib2c) and crt(crt2c) for с2с, making it a central resource for с2с development.

c2c language - https://github.com/c2c-team/c2c-draft

## Supports Platform

Below is a table of platforms and architectures for which the 2cc compiler can generate code

| Platform | x86 | x64 | ARM | ARM64 |
|----------|-----|-----|-----|-------|
| Windows  | No | No | No  | No   |
| Linux    | No | No | No | No   |
| macOS    | No  | No | No  | No    |
| Android  | No  | No  | No | No   |
| iOS      | No  | No  | No  | No   |

## Building the 2cc compiler

If you want to build a 2cc compiler from source code, here are a few steps you need to follow:

1. Install dependencies: Make sure you have all the necessary tools and libraries installed to build 2cc. In particular, you will need CMake, a C++ compiler that supports C++20, and Git.
2. Clone the repository: Clone the 2cc repository to your local computer using the command:

```bash
git clone https://github.com/c2c-team/c2c-compiler
```
3. Create a directory for the build: Go to the repository directory and create a directory for the build. For example:

```bash
cd 2cc
mkdir build
cd build
```

4. Run CMake: Run CMake to generate files for the build. For example:
```bash
cmake ..
```
## Usage/Examples
A simple example of compiling a program written in c2c using 2cc:
```bash
ctc main.2c -o main.o
```


## Authors
C2C Team
- [@lofinaf](https://github.com/Lofinaf)
- [@satankoteika](https://github.com/Satankoteika)


## License

[MIT License](https://choosealicense.com/licenses/mit/)

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)

## Contributing

Contributions are always welcome!

See `contributing.md` for ways to get started

