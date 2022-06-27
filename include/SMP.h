/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <string>

#include "Error.h"
#include "BMP.h"
#include "File.h"

namespace Seraph {
	class SMP {
		short SignatureSz;
		char* Signature;
		short LinkSz;
		char* Link;
		//long unk;
		struct Header {
			short Width, Height;
			long LinearSz;
		}m_Header;
		char* Data;

		File In;
		File Out;
	public:
		bool Save(std::string OutputPath);
		bool Load(std::string FilePath);
		bool ExportAsBMP(std::string OutputPath);
		void RemoveCollision();
		void Shutdown();
	};
}
