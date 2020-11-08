# FrOS

## Description

Based on 
Carlos Fenollosas [OS tutorial](https://github.com/cfenollosa/os-tutorial), additionally using the following resources:

- [Writing a Simple Operating System — from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
- [OS Dev Wiki](https://wiki.osdev.org/Meaty_Skeleton)
- [JamesM's Kernel Development Tutorials](https://web.archive.org/web/20160412174753/http://www.jamesmolloy.co.uk/tutorial_html/index.html)
- [BrokenThorn Entertainment's Operating System Development Series](http://www.brokenthorn.com/Resources/OSDevIndex.html)

## Setup

### Install Assembler and Emulator

```bash
brew install qemu nasm
```

### Install Kernel Compiler

https://github.com/lordmilko/i686-elf-tools#mac-os-x

```
brew install i386-elf-binutils
brew install i386-elf-gcc
brew install i386-elf-gdb
```

```
export PATH="/usr/local/Cellar/x86_64-elf-binutils/<version>/bin/:/usr/local/Cellar/x86_64-elf-gcc/<version>/bin/:/usr/local/Cellar/i386-elf-gdb/<version>/bin:$PATH"
```

## Usage

### Boot

```
make run
```

### Debug

```
make debug
```

In GDB shell:

- Set breakpoint at function (e.g. `main`): `b main`
- Start execution: `c`
- Jump to next instruction: `n`
- Print variable: `p <variable_name>`
