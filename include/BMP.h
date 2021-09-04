/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Error.h"

namespace Seraph {
	class BMP {
		BITMAPFILEHEADER Header;
		BITMAPINFOHEADER Info;
		//char* Data;
	public:
		bool Export(std::string FilePath, int Width, int Height, char* Data);
	};
}
