/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.																				   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".				   //
//I take no responsaiblity over this code or how it is used.											   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FSM.h"

bool FSM::Skip(int Count) {
	for (int i{ 0 }; (i < Count) && (!File.eof()); i++)
		File.get();
	return !File.eof();
}

float FSM::LittleEndian32f() {
	float Buffer;
	char Arr[]{
		File.get(),File.get(),File.get(),File.get()
	};
	memcpy(&Buffer, Arr, sizeof(float));

	return Buffer;
}

std::string FSM::GetName() {
	std::string Name;
	if (!File.eof()) {
		while (Name.find('\0', 0) == -1)
			Name.push_back(File.get());
		Name.pop_back();
	}
	return Name;
}

bool FSM::LogError(std::string ErrorDesc) {
	if (!ErrorDesc.empty()) {
		MessageBoxA(GetConsoleWindow(), ErrorDesc.c_str(), NULL, MB_ICONEXCLAMATION);
	}

	return false;
}

bool FSM::BatchProcessHM(unsigned short Position) {
	if ((m_MeshData[Position].Width < 1) || (m_MeshData[Position].Height < 1) || (m_MeshData[Position].LinearSz < 1))
		return LogError("Invalid header data.");
	m_MeshData[Position].HeightMap = new float[m_MeshData[Position].Width * m_MeshData[Position].Height];
	for (int i{ 0 }; i < m_MeshData[Position].Width * m_MeshData[Position].Height; i++) {
		File.read((char*)&m_MeshData[Position].HeightMap[i], sizeof(float));
		if (!Skip(sizeof(float)))
			return LogError("Unexpected end of file.");
	}
	if (!Skip(m_MeshData[Position].LinearSz - (m_MeshData[Position].Width * m_MeshData[Position].Height * 8)))
		return LogError("Unexpect end of file.");
	return true;
}

void FSM::BatchProcessTL(unsigned short Position) {
	int BinSz{};
	File.read((char*)&BinSz, sizeof(int));
	int ObjCount{};
	File.read((char*)&ObjCount, sizeof(int));
	m_MeshData[Position].Material = new std::string[ObjCount];
	m_MeshData[Position].AlphaMap = new std::string[ObjCount];
	int StrSz = 0;
	for (int i{ 0 }, j{ 0 }, index{ 0 }; index < ObjCount * 2; index++) {
		if (!(index % 2))
			StrSz = m_MeshData[Position].Material[i++].assign(GetName()).size()+1;
		else if (ObjCount > 1)
			StrSz = m_MeshData[Position].AlphaMap[j++].assign(GetName()).size()+1;
		if (ObjCount > 1)
			Skip(Offset + 8 - StrSz);
	}
	if (ObjCount == 1)
		Skip(BinSz - 12 - StrSz);
}

bool FSM::BatchProcessOBJ() {
	Skip(sizeof(long long)); // I don't understand this.
	float tempRot;
	while (!File.eof()) {
		m_ObjData.push_back(ObjInfo{ Position{LittleEndian32f(),LittleEndian32f(),LittleEndian32f()}, // POSITION
										tempRot = LittleEndian32f(),tempRot / 0.0174533f, // ROTATION
										LittleEndian32f(), // SCALE
										GetName() }); // NAME
		if (!Skip(Offset + (8 - m_ObjData[m_ObjData.size() - 1].Name.size()-1))) return false;
	}
	return true;
}

bool FSM::LoadFSM(std::string FilePath) {
	if (FilePath.empty()) return false;
	File.open(FilePath, std::ios::binary);
	if (!File.is_open())
		return LogError(FilePath + " was not found or was not possible to open.");
	char TempStr[4];
	unsigned int TempVal{};
	File.read(TempStr, 4);
	if (strncmp(TempStr, "FSM ", 4) != 0)
		return LogError(FilePath + " is not a FSM file!");
	Skip(0x50);
	File.read((char*)&TempVal, sizeof(int));
	m_MeshData.resize(TempVal);

	int Position = 0;

	while (!File.eof()) {
		File.read(TempStr, 4);
		if (strncmp(TempStr, "HM", 2) != 0) {
			if (strncmp(TempStr, "OBJ ", 4) == 0) {
				BatchProcessOBJ();
				break;
			}
			else
				return LogError("Impossible to load object information.");
		}
		File.read((char*)&m_MeshData[Position].LinearSz, sizeof(long));
		File.read((char*)&m_MeshData[Position].Width, sizeof(int));
		File.read((char*)&m_MeshData[Position].Height, sizeof(int));
		Skip(sizeof(int));
		if (!BatchProcessHM(Position))
			return false;
		File.read(TempStr, 4);
		if (strncmp(TempStr, "TL", 2) == 0)
			BatchProcessTL(Position);
		else
			return LogError("Invalid or corrupt data.");
		Position++;
	}
	return true;
}

int FSM::GetGroundCount() {
	return m_MeshData.size();
}

int FSM::GetObjCount() {
	return m_ObjData.size();
}

const FSM::Mesh& FSM::GetGroundDataAt(unsigned short MeshIndex) {
	return m_MeshData[MeshIndex];
}

const FSM::ObjInfo& FSM::GetObjDataAt(unsigned short ObjIndex) {
	return m_ObjData[ObjIndex];
}
