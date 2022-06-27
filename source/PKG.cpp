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
	File File;
	Assert(File.open(FilePath, std::ios::in | std::ios::binary));
	std::string Data;
	File.seekg(0, std::ios::end);
	Data.reserve(File.tellg());
	File.seekg(0, std::ios::beg);
	Data.assign(std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
	Sz = compressBound(Data.size());
	Header.SizeOriginal = Data.size();
	Packed = new unsigned char[Sz];
	int err = compress2((Bytef*)Packed, (uLongf*)&Sz, (Bytef*)Data.c_str(), Data.size(),1);
	Assert(err == Z_OK);
	Header.SizePacked = Sz;
	std::ifstream checkpos(PKGName(),std::ios::binary);
	if (checkpos.is_open()) {
		checkpos.seekg(0,std::ios::end);
		Header.Offset = checkpos.tellg();
		checkpos.close();
	}
	Assert(IDXFileOut.write((char*)&Header, sizeof(FileHeader)));
	Data.clear();
	File.close();
	return true;
}

bool Seraph::PKG::DecodeFile() {
	unsigned char* packed = new unsigned char[Header.SizePacked];
	std::string FileName{ PKGName() };
	File Data;
	Assert(Data.open(FileName, std::ios::in | std::ios::binary));
	Data.seekg(Header.Offset, std::ios::beg);
	Assert(Data.read(packed, Header.SizePacked));
	Data.close();
	Unpacked = new unsigned char[Header.SizeOriginal];
	uncompress((Bytef*)Unpacked, (uLongf*)&Header.SizeOriginal, (Bytef*)packed, (uLongf)Header.SizePacked);
	delete[] packed;
	packed = nullptr;
	return true;
}

bool Seraph::PKG::Encrypt() {
	Assert(EncodeFile());
	Assert(WritePacked());
	Reset();
	return true;
}

#include <direct.h> // mkdir

void RecurseDirCreate(std::string FilePath) {
	struct stat buffer;
	const std::string Dir{ FilePath };
	size_t index1{ 0 }, index2{ Dir.find('\\',index1) };
	std::string Exists;
	do {
		std::string RecurseDir;
		RecurseDir.assign(Exists);
		RecurseDir.append(Dir.substr(index1, index2 - index1));
		if (stat(RecurseDir.c_str(), &buffer))
			_mkdir(RecurseDir.c_str());
		index1 = index2;
		index2 = Dir.find('\\', ++index2);
		Exists.assign(RecurseDir);
	} while (index2 != -1);
}

bool Seraph::PKG::Decrypt() {
	Assert(DecodeFile());
	RecurseDirCreate(Header.FilePath);
	Assert(WriteUnpacked());
	Reset();
	return true;
}

bool Seraph::PKG::WritePacked() {
	std::string FilePath;
	FilePath.assign(PKGName());
	std::cout << "Write: " << Header.FileName << " in " << FilePath << "\n\n";
	File File;
	Assert(File.open(FilePath, std::ios::in | std::ios::out | std::ios::binary | std::ios::app));
	Assert(File.write(Packed, Sz));
	File.close();
	return true;
}

bool Seraph::PKG::WriteUnpacked() {
	std::string FullFilePath;
	FullFilePath.assign(Header.FilePath);
	FullFilePath.append(Header.FileName);
	std::cout << "Write: " << Header.FileName << "\n\n";
	File file;
	Assert(file.open(FullFilePath, std::ios::out | std::ios::binary));
	Assert(file.write(Unpacked, Header.SizeOriginal));
	file.close();
	return true;
}

void Seraph::PKG::Reset() {
	ZeroMemory(&Header, sizeof(FileHeader));
	delete[] Unpacked;
	Unpacked = nullptr;
	delete[] Packed;
	Packed = nullptr;
}

bool Seraph::PKG::Load(bool Unpack) {
	char TempDir[MAX_PATH];
	int result = GetModuleFileNameA(NULL, TempDir, MAX_PATH);
	IDXFile.open("pkg.idx", std::ios::in | std::ios::binary);
	IDXFile.read(&Signature);
	if (!Unpack) {
		Assert(IDXFileOut.open("tempdata", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc));
		Assert(IDXFileOut.write((char*)&Signature, sizeof(IdxSignature)));
	}
	while (!IDXFile.eof()) {
		Assert(IDXFile.read(&Header));
		if (Unpack) {
			Assert(Decrypt());
		}
		else {
			Assert(Encrypt());
		}
	}
	IDXFile.close();
	if (!Unpack) {
		File File;
		File.open("pkg.idx", std::ios::binary);
		char* Data;
		int datasz;
		IDXFileOut.seekg(0,std::ios::end);
		datasz = IDXFileOut.tellg();
		Data = new char[datasz];
		IDXFileOut.seekg(std::ios::beg);
		Assert(IDXFileOut.read(Data, datasz));
		Assert(File.write(Data, datasz));
		File.close();
		IDXFileOut.close();
	}
	return true;
}
