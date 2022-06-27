/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SMP.h"

bool Seraph::SMP::Save(std::string OutputPath) {
	Assert(Out.open(OutputPath, std::ios::out | std::ios::binary));
	Assert(Out.write(&SignatureSz));
	Out << Signature;
	Assert(Out.write((char*)&LinkSz, sizeof(short)));
	Out << Link;
	Assert(Out.write("\0\0\0\0", 4)); // unknown, put zeros in there
	Assert(Out.write((char*)&m_Header, sizeof(Header)));
	Assert(Out.write(Data, m_Header.LinearSz));
	Out.close();
	return true;
}

bool Seraph::SMP::Load(std::string FilePath) {
	Assert(In.open(FilePath, std::ios::in | std::ios::binary));
	Assert(In.read(&SignatureSz));
	Signature = new char[SignatureSz + 1];
	Assert(In.read(Signature, SignatureSz));
	Signature[SignatureSz] = '\0';
	Assert(In.read(&LinkSz));
	Link = new char[LinkSz + 1];
	Assert(In.read(Link, LinkSz));
	Link[LinkSz] = '\0';
	In.seekg(sizeof(long), std::ios::cur); // jump unknown data
	Assert(In.read(&m_Header));
	Data = new char[m_Header.LinearSz];
	Assert(In.read(Data, m_Header.LinearSz));
	In.close();
	return true;
}

bool Seraph::SMP::ExportAsBMP(std::string OutputPath) {
	BMP Image;
	char* imgData = new char[m_Header.LinearSz * 4];
	for (int Pos{ 0 }; Pos < m_Header.LinearSz; Pos++)
		for (int RGBA{ 0 }; RGBA < 4; RGBA++)
			imgData[Pos * 4 + RGBA] = Data[Pos];
	Assert(Image.Export(OutputPath, m_Header.Width, m_Header.Height, imgData));
	return true;
}

void Seraph::SMP::RemoveCollision() {
	memset((void*)Data, 0, m_Header.LinearSz);
}

void Seraph::SMP::Shutdown() {
	RemoveCollision();
	memset(&m_Header, 0, sizeof(Header));
	LinkSz = SignatureSz = 0;
	delete Signature;
	delete Link;
	Link = Signature = nullptr;
}
