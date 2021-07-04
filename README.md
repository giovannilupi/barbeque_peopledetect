# OpenCV Peopledetect sample ported to BarbequeRTRM

The aim of this project is to port the sample "peopledetect" from the
OpenCV library to the Barbeque Run-Time Resource Manager
(BarbequeRTRM).

# Installation

The file ```doc/report_bbque_opencv.pdf``` contains detailed
installation instructions and lists the required dependencies.

# Compilation

In the following examples, BOSP is installed in
`/home/user/Work/BOSP`.

Configure the BOSP directory: edit CMakeLists.txt and set
```
set(BOSP_SYSROOT "/home/user/Work/BOSP/out/usr")
set(CMAKE_INSTALL_PREFIX "/home/user/Work/BOSP/out")
```
according to your installation of BOSP.

Then build and install the application:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

# Running the program

Run BOSP
```
. ~/Work/BOSP/out/etc/bbque/bosp_init.env
```
From the BOSP shell, start the daemon and run the application (change the path
according to your installation):
```
[BOSPShell ~] \> bbque-startd
[sudo] password: xxxxxxxx
peopledetect --video=/home/user/Work/BOSP-devel/peopledetect/peopledetect.bbque/resources/sample2-scaled.mp4
```

# Compiling the documentation

The documentation (report_bbque_opencv.pdf) is already precompiled in the ```doc``` directory.

To recompile it just run ```make``` in the ```doc``` directory (Latex is needed.)

