# terminal-2048

Clone of the game [2048](https://en.wikipedia.org/wiki/2048_(video_game)), played in terminal

Writen in C++, using [ncurses](https://invisible-island.net/ncurses/) as the TUI library

## Features 
 - Different board sizes - up to 100 rows and 100 columns
 - Changing the size of the single tile
 - Terminal colors 

## Why i made it
I am learning C++ and i tought that making 2084 would be a fun and interesting project to make.

Making this project helped me to drastically improve my C++ skills.



## How to build and run
This project uses cmake as its build system

Currently supported is only Linux.

### Build
~~~ shell
git clone https://github.com/zalesyc/terminal-2048.git && cd terminal-2048
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
~~~
### Run

Executable is created in the build directory, run it with:
~~~ shell
./terminal-2048
~~~

