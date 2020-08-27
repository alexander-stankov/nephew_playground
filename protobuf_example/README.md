## Protobuf test project ##

### Installing prerequisites ###

This project depends only on [Google Protocol Buffers](https://developers.google.com/protocol-buffers/).\
To install it for your desired platform use the steps below:

**On Debian Linux**
``` shell
sudo apt update
sudo apt install libprotobuf-dev
```

**On Windows**\
On Windows the best way to install protobuf is by using vcpkg.\
Installation instructions for vcpkg on Windows can be found at
[Quick Start Windows](https://github.com/Microsoft/vcpkg#quick-start-windows)

After you have installed vcpkg use the following command to install protobuf
``` shell
vcpkg install protobuf protobuf:x64-windows
```

### Building and Running the project ###
After you have installed the prerequisites for the project follow the steps below to build and run it.

**Linux**

``` shell
mkdir build && cd build
cmake ../
make -jN #where N is the number of jobs you want to use
./protobuf_example #run the project
```

**Windows**\
It is important to note the you will need the path to the vcpkg installation from the previous steps
``` shell
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]\scripts\buildsystems\vcpkg.cmake ../
cmake --build .
cd Debug # or Release depending on the build mode
protobuf_example.exe #run the project
```
