/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <sys/stat.h>
#include <iomanip>

#include "Error.h"
#include "File.h"

namespace Seraph {
    class PKG {
    private:
        typedef unsigned int uint;

        struct IdxSignature
        {
            uint Dummy[0x41];
            char Signature[0xC];
            uint Unknown2;
            uint Unknown3;
            uint Unknown4;
            uint Unknown5;
            uint Unknown6;
        } Signature;

        struct FileHeader
        {
            uint FileID;
            uint Offset;
            uint Unk4;
            uint SizePacked;
            uint Unk6;
            uint Unk7;
            uint Unk8;
            uint Unk9;
            time_t PackTime;
            time_t OpenTime;
            time_t ChangeTime;
            uint SizeOriginal;
            char FileName[0x104];
            char FilePath[0x104];
            uint Unk1;
            uint pkgNum;
            uint FileCRC;
        } Header;
        File IDXFile;
        File IDXFileOut;
        std::string CurrentDir;
        unsigned char* Unpacked;
        unsigned char* Packed;
        int Sz;
    private:
        void Reset();
        std::string PKGName();
        bool EncodeFile();
        bool DecodeFile();
        bool Encrypt();
        bool Decrypt();
        bool WritePacked();
        bool WriteUnpacked();
    public:
        bool Load(bool Unpack = false);
    };
}
