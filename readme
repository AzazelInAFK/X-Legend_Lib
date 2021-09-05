This code currently supports FSM, SMP and PKG files.
The library is separate between multiple files for each format supported, each with their respective class with members to deal with the actual files and their data.
To use it you just need to create a object of the desired object under the namespace 'Seraph', eg.: Seraph::PKG;

FSM:

'Seraph::FSM::Load'-> Reads and parses data from a FSM file into memory.
'Seraph::FSM::ExportHeightMapAsBMP'-> Converts the terrain's heightmap data into a bitmap file for easier understanding.
'Seraph::FSM::AsASCIIFile'-> Converts the object's data and optionally the terrain heightmap data into a text file for easier understanding.
I'm planning to add support for modifying FSM data through here and also the possibility of creating a personalized FSM file.

SMP:

'Seraph::SMP::Load'-> Reads and parses data from a SMP file into memory.
I'm planning to add support for modifying SMP data through here and also the possibility of creating a personalized SMP file.

PKG:

'Seraph::PKG::Load'-> Reads and parses data from 'pkg.idx' into memory. (Note: In case you set this function parameter to true, it will use the files on your folder to repack
them to the original .pkg files, notice this is a demo version and is not 100% trustable. If you use the default parameter value, it will instead unpack all the files from your
game into the same foulder you're running your program.
Even though this section is not entirely necessary because of the existance of the aeria games script which can also export and reimport data from pkg files, in my opnion this 
library is best due to the script being outdated and not always working mainly on newer files from the games.
