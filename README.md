# X-Legend Library
## Overview
This library contains functions to work with different X-Legend files. Currently supported are FSM, SMP and PKG files.\
For each file type there is a seperate class with members to deal with the actual files and their data.\
To use the functions you can create an object of the desired class in the namespace `Seraph`.

## File types
### FSM
To read and parse data from a FSM file into memory:
```c++
Seraph::FSM myFSMObject;
myFSMObject.Load("MyFSMFile.fsm");
```
To convert the terrain's heightmap data into a bitmap file for easier understanding:
```c++
myFSMObject.ExportHeightMapAsBMP("BMPFile");
```
To convert the object's data and optionally the terrain heightmap data into a text file for easier understanding:
```c++
myFSMObject.AsASCIIFile(false); // without heightmap
myFSMObject.AsASCIIFile(true);  // with heightmap
```
For future updates, it is planned that this library will support modifying and creating FSM files.

### SMP
To read and parse data from a SMP file into memory:
```c++
Seraph::SMP mySMPObject;
mySMPObject.Load("MySMPFile.smp");
```
For future updates, it is planned that this library will support modifying and creating SMP files.

### PKG
To read and parse data from 'pkg.idx' into memory:
```c++
Seraph::PKG myPKGObject;
myPKGObject.Load(false);
```
If this function parameter is set to true, the function will use the files in the folder to repack them into the original PKG files.\
Otherwise, if this function parameter is set to false, the function will instead unpack all the files from the game into the current folder.

Please notice that this PKG tool is a demo version and it is not yet 100% trustable.
