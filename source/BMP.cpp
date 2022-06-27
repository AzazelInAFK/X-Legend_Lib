/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BMP.h"

bool Seraph::BMP::Import(std::string FilePath) {
	std::string MemoryErr{ "Unexpected end of file." };
	File File;
	Assert(File.open(FilePath, std::ios::in | std::ios::binary));
	Assert(File.read(&Header));
	Assert(File.read(&Info));
	Data = new char[Info.biWidth * Info.biHeight*4];
	Assert(File.read(Data, Info.biWidth * Info.biHeight * 4));
	return true;
}

bool Seraph::BMP::Export(std::string FilePath, int Width, int Height, char* Data) {
	File File;
	Assert(File.open(FilePath, std::ios::out | std::ios::binary));
	Header.bfType = ('B' << 0) | ('M' << 8);
	Header.bfReserved1 = Header.bfReserved2 = 0;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	Header.bfSize = Header.bfOffBits + (Width * Height * 32);
	Info.biSize = sizeof(BITMAPINFOHEADER);
	Info.biWidth = Width;
	Info.biHeight = Height;
	Info.biPlanes = 1;
	Info.biBitCount = sizeof(char) * 32;
	Info.biCompression = false;
	Info.biSizeImage = false;
	Info.biXPelsPerMeter = Info.biYPelsPerMeter = 0;
	Info.biClrUsed = false;
	Info.biClrImportant = false;
	Assert(File.write(&Header));
	Assert(File.write(&Info));
	Assert(File.write(Data, Width * Height * 4));
	File.close();
	return true;
}
