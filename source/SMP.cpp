/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SMP.h"

unsigned int Seraph::SMP::GetDataSize() {
	return FullSz;
}

unsigned char* Seraph::SMP::GetData() {
	return Data;
}

short Seraph::SMP::GetWidth() {
	return m_Header.Width;
}

short Seraph::SMP::GetHeight() {
	return m_Header.Height;
}

std::string Seraph::SMP::GetRelatedScene() {
	return std::string(Link);
}

bool Seraph::SMP::SecurityCheck() {
	if (strncmp(Signature, "SMP2.0", SignatureSz) != 0)
		return false;
	return true;
}

bool Seraph::SMP::Load(std::string FilePath) {
	File.open(FilePath, std::ios::binary);
	if (!File.is_open())
		return Log("Failed to open " + FilePath);

	File.read((char*)&SignatureSz, sizeof(short));
	Signature = new char[SignatureSz+1];
	File.read(Signature, SignatureSz);

	if (!SecurityCheck())
		return Log("This is not a SMP file.");

	Signature[SignatureSz] = '\0';

	File.read((char*)&LinkSz, sizeof(short));
	Link = new char[LinkSz+1];
	File.read(Link, LinkSz);

	Link[LinkSz] = '\0';

	File.read((char*)&m_Header, sizeof(Header));

	unsigned int FullSz = m_Header.Width * m_Header.Height;
	Data = new unsigned char[FullSz];

	File.read((char*)Data, FullSz);

	return true;
}
