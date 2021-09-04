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