#include "BMP.h"

bool Seraph::BMP::Export(std::string FilePath, int Width, int Height, char* Data) {
	std::ofstream File;
	File.open(FilePath, std::ios::binary);
	if (!File.is_open())
		return Log("Failed to create file bmp file to export BMP data.\n" + FilePath);
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
	File.write((char*)&Header, sizeof(BITMAPFILEHEADER));
	File.write((char*)&Info, sizeof(BITMAPINFOHEADER));
	File.write(Data, Width * Height * 4);
	File.close();
	return true;
}