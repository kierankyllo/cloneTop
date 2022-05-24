# cloneTop

cloneTop is a htop-like system monitor application for Linux. This project was completed as part of the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

![System Monitor](images/monitor.png)

## ncurses
The GUI for this program is provided by the ncurses library. [ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal.

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions To Build The Project

1. Clone the project repository: `git clone https://github.com/kierankyllo/cloneTop.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`

![Starting System Monitor](images/starting_monitor.png)
