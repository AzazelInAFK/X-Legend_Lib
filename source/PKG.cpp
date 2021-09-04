/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PKG.h"
#include "zlib-1.2.11/zlib.h"

std::string Seraph::PKG::PKGName() {
	std::string buffer;
	buffer.assign("pkg");
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << Header.pkgNum;
	buffer.append(oss.str());
	buffer.append(".pkg");
	return buffer;
}

bool Seraph::PKG::EncodeFile() {
	std::string FilePath;
	FilePath.assign(Header.FilePath);
	FilePath.append(Header.FileName);
	std::ifstream File(FilePath, std::ios::binary);
	if (!File.is_open())
		return Log(FilePath + " was not found, or not possible to open! This file is necessary in the repack proccess.");
	std::string Data;
	File.seekg(0, std::ios::end);
	Data.reserve(File.tellg());
	File.seekg(0, std::ios::beg);
	Data.assign(std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
	Sz = compressBound(Data.size());
	Packed = new unsigned char[Sz];
	int err = compress2((Bytef*)Packed, (uLongf*)&Sz, (Bytef*)Data.c_str(), Data.size(),1);
	if (err != Z_OK)
		return Log("Failed to compress data to file.");
	Data.clear();
	File.close();
	return true;
}

bool Seraph::PKG::DecodeFile() {
	unsigned char* packed = new unsigned char[Header.SizePacked];
	std::string FileName{ PKGName() };
	std::ifstream Data(FileName, std::ios::binary);
	if (!Data.is_open())
		return Log(FileName + " is missing or was not possible to open.");
	Data.seekg(Header.Offset, std::ios::beg);
	Data.read((char*)packed, Header.SizePacked);
	Data.close();
	Unpacked = new unsigned char[Header.SizeOriginal];
	uncompress((Bytef*)Unpacked, (uLongf*)&Header.SizeOriginal, (Bytef*)packed, (uLongf)Header.SizePacked);
	delete packed;
	packed = nullptr;
	return true;
}

bool Seraph::PKG::Encrypt() {
	if (!EncodeFile())
		return Seraph::Error::Throw();
	if (!WritePacked())
		return Seraph::Error::Throw();
	Reset();
	return true;
}

bool Seraph::PKG::Decrypt() {
	if (!DecodeFile())
		return Seraph::Error::Throw();
	Seraph::RecurseDirCreate(Header.FilePath);
	if (!WriteUnpacked())
		return Seraph::Error::Throw();
	Reset();
	return true;
}

bool Seraph::PKG::WritePacked() {
	std::string FilePath;
	FilePath.assign(PKGName());
	std::cout << "Write: " << Header.FileName << " in " << FilePath << "\n\n";
	std::fstream File;
	File.open(FilePath, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
	if (!File.is_open())
		return Log("Failed to create packed file: " + FilePath + " in: " + PKGName());
	File.write((char*)Packed, Sz);
	File.close();
	return true;
}

bool Seraph::PKG::WriteUnpacked() {
	std::string FullFilePath;
	FullFilePath.assign(Header.FilePath);
	FullFilePath.append(Header.FileName);
	std::cout << "Write: " << Header.FileName << "\n\n";
	std::ofstream Output(FullFilePath, std::ios::binary);
	if (!Output.is_open())
		return Log("Failed to create " + FullFilePath);
	Output.write((char*)Unpacked, Header.SizeOriginal);
	Output.close();
	return true;
}

void Seraph::PKG::Reset() {
	ZeroMemory(&Header, sizeof(FileHeader));
	delete Unpacked;
	Unpacked = nullptr;
	delete Packed;
	Packed = nullptr;
}

bool Seraph::PKG::Load(bool Unpack) {
	char TempDir[MAX_PATH];
	int result = GetModuleFileNameA(NULL, TempDir, MAX_PATH);
	IDXFile.open("pkg.idx", std::ios::binary);
	if (!IDXFile.is_open())
		return Log("pkg.idx is missing or was not possible to open.");
	IDXFile.read((char*)&Signature, sizeof(IdxSignature));
	while (!IDXFile.eof()) {
		IDXFile.read((char*)&Header, sizeof(FileHeader));
		if (Unpack)
			Decrypt();
		else
			Encrypt();
	}
	IDXFile.close();
	return true;
}
