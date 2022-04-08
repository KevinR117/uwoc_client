# Client UWOC

## Usage

This client application is supposed to be running on a distant machine

It contains a client that is sending video to the distant server. It is also
waiting for frames received from the server (to show it into a window)

## Build and execute

To compile this project, you need to change directory to build directory by `cd build`

Then remove the directory content when you are compiling it for the first time with typing
`rm -rf *`
Be carefull to be in the build directory before doing this

Type `cmake ..`

Finally type `make -j2`

An executable file will be created, so you can execute it typing `./uwoc_client`