Using uC/OS-II with mbed on ARM Cortex M3/M4
============================================

Introduction
------------
This is the source code repository for a simple application using  
uC/OS-II with an mbed board (FRDM-K64F).

Build Instructions
------------------

1. Ensure that you have installed mbed-cli
   (https://os.mbed.com/docs/v5.9/tools/arm-mbed-cli.html) and the GCC_ARM
   tools
   (https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads). 

1. Create a new mbed OS2 directory for the application
   
     ```sh
     $ mbed new blinky-mbed-ucos-ii --mbedlib
     ```

1. Set the current working directory to your application directory

     ```sh
     $ cd blinky-mbed-ucos-ii
     ```

1. Clone the source repository into a directory called `src`

     ```sh
     $ git clone https://github.com/davidkendall/blinky-mbed-ucos-ii-src src
     ```

1. Compile the program

     ```sh
     $ mbed compile -t GCC_ARM -m K64F
     ```

1. Load and run, e.g.

     ```sh
     $ cp BUILD/K64F/GCC_ARM/blinky-mbed-ucos-ii.bin /media/mbed
     ```
   assuming that your board is mounted as a drive at `/media/mbed`. It's also 
   possible to use [pyocd](https://github.com/mbedmicro/pyOCD) and gdb for load
   and debug.


