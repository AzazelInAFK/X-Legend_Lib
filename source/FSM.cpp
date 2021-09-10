/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FSM.h"

bool Seraph::FSM::AsASCIIFile(bool WriteHeightmap) {
	std::ofstream out("FSMOut.txt");
	if (!out.is_open() || !InMemory)
		return Log("Impossible to create output file and/or write ascii version of this file.(FSMOut.txt)");
	out << "Objects in this file:\n";
	for (int ObjectID{ 0 }; ObjectID < m_ObjectHeader.ObjectCount; ObjectID++) {
		out << "Name: ";
		out.write(Object[ObjectID].Name, STRINGSZ);
		out << std::endl << "Position: X(" << std::to_string(Object[ObjectID].X) << "), Y(" << std::to_string(Object[ObjectID].Y) << "), Z(" << std::to_string(Object[ObjectID].Z) << ").\n";
		out << "Rotation(Radian): " << Object[ObjectID].Rotation << "\nRotation(Degree): " << (Object[ObjectID].Rotation * 180.0f) / 3.14159f << std::endl;
		out << "Scale: " << std::to_string(Object[ObjectID].Scale) << std::endl;
	}
	if (WriteHeightmap) {
		for (int Layer{ 0 }; Layer < m_FSMHeader.HeightmapCount; Layer++) {
			out << "Heightmap #" << Layer << ":\n";
			for (int Y{ 0 }; Y < m_HeightMapHeader[Layer].Height; Y++)
				for (int X{ 0 }; X < m_HeightMapHeader[Layer].Width; X++)
					out << Y << ',' << X << ':' << HeightMap[Layer][X + (Y * m_HeightMapHeader[Layer].Width)].HeightMap << std::endl;
		}
	}
	out.close();
	return true;
}

bool Seraph::FSM::ImportHeightMapFromBMP(std::string FilePath, long HeightMapSlotID){ // CAUTION: THIS IS A DEMO AND PROBABLY HAVE FLAWS.
	if (HeightMapSlotID >= m_FSMHeader.HeightmapCount)
		return false;
	BMP Img;
	if (!Img.Import(FilePath))
		return Log("Failed to import bmp file as heightmap.");
	m_HeightMapHeader[HeightMapSlotID].Width = Img.Info.biWidth;
	m_HeightMapHeader[HeightMapSlotID].Height = Img.Info.biHeight;
	m_HeightMapHeader[HeightMapSlotID].LinearSz = Img.Info.biWidth * Img.Info.biHeight * sizeof(HeightMapType);
	delete HeightMap[HeightMapSlotID];
	HeightMap[HeightMapSlotID] = nullptr;
	HeightMap[HeightMapSlotID] = new HeightMapType[m_HeightMapHeader[HeightMapSlotID].Width * m_HeightMapHeader[HeightMapSlotID].Height];
	for (int i{ 0 }; i < Img.Info.biWidth * Img.Info.biHeight; i++)
		HeightMap[HeightMapSlotID][i].HeightMap = Img.Data[i * 4]; // precision issue from char to float.
	return true;
}

bool Seraph::FSM::ExportHeightMapAsBMP(std::string FilePath) {
	std::string Original{ FilePath };
	for (int Layer{ 0 }; Layer < m_FSMHeader.HeightmapCount; Layer++) {
		FilePath.assign(Original);
		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << Layer;
		FilePath.append(oss.str());
		FilePath.append(".bmp");
		Seraph::BMP BMP;
		char* transfData = new char[m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height * 4];
		for (int i{ 0 }; i < m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height; i++)
			for (int j{ 0 };j<4;j++)
			transfData[i*4+j] = HeightMap[Layer][i].HeightMap;
		if (!BMP.Export(FilePath, m_HeightMapHeader[Layer].Width, m_HeightMapHeader[Layer].Height, transfData))
			return Log("Failed to convert and export heightmap to BMP.");
	}
	return true;
}

bool Seraph::FSM::ImportObject(float X, float Y, float Z, float Rotation, float Scale, char* Name, long NameSz) {
	if (NameSz > STRINGSZ)
		return false;
	ObjectType Buffer;
	Buffer.X = X;
	Buffer.Y = Y;
	Buffer.Z = Z;
	Buffer.Rotation = Rotation;
	Buffer.Scale = Scale;
	for (int i{ 0 }; i < NameSz; i++)
		Buffer.Name[i] = Name[i];
	for (int i{ NameSz }; i < STRINGSZ; i++)
		Buffer.Name[i] = '\0';

	ImportedObj.push_back(Buffer);

	return true;
}

bool Seraph::FSM::Save(std::string OutputPath) {
	std::ofstream Out;
	Out.open(OutputPath, std::ios::binary);
	if (!Out.is_open())
		return Log("Failed to save FSM file in " + OutputPath);
	Out.write((char*)&m_FSMHeader, sizeof(CoreHeader));
	for (int Layer{ 0 }; Layer < m_FSMHeader.HeightmapCount; Layer++) {
		Out.write((char*)&m_HeightMapHeader[Layer], sizeof(HeightMapHeader));
		for (int Point{ 0 }; Point < m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height; Point++)
			Out.write((char*)&HeightMap[Layer][Point], sizeof(HeightMapType));
		Out.write((char*)&m_TextureLayerHeader[Layer], sizeof(TextureLayerHeader));
		for (int Material{ 0 }; Material < m_TextureLayerHeader[Layer].MaterialCount; Material++)
			Out.write((char*)&TextureLayer[Layer][Material], sizeof(TextureLayerType));
	}
	m_ObjectHeader.ObjectCount += ImportedObj.size();
	m_ObjectHeader.LinearSz += ImportedObj.size() * sizeof(ObjectType);
	Out.write((char*)&m_ObjectHeader, sizeof(ObjectHeader));
	for (int ObjectID{ 0 }; ObjectID < m_ObjectHeader.ObjectCount; ObjectID++)
		Out.write((char*)&Object[ObjectID], sizeof(ObjectType));
	for (int ObjectID{ 0 }; ObjectID < ImportedObj.size(); ObjectID++) // extra imported object data.
		Out.write((char*)&ImportedObj[ObjectID], sizeof(ObjectType));
	return true;
}

bool Seraph::FSM::Load(std::string FilePath) {
	std::string MemoryErr = "Unexpected end of file.";
	In.open(FilePath, std::ios::binary);
	if (!In.is_open())
		return Log("Failed to open/read specified file: " + FilePath);
	In.read((char*)&m_FSMHeader, sizeof(CoreHeader));
	if (In.gcount() != sizeof(CoreHeader))
		return Log(MemoryErr);
	m_HeightMapHeader = new HeightMapHeader[m_FSMHeader.HeightmapCount];
	m_TextureLayerHeader = new TextureLayerHeader[m_FSMHeader.HeightmapCount];
	HeightMap = new HeightMapType * [m_FSMHeader.HeightmapCount];
	TextureLayer = new TextureLayerType * [m_FSMHeader.HeightmapCount];
	for (int Layer{ 0 }; Layer < m_FSMHeader.HeightmapCount; Layer++) { // Terrain routine:
		In.read((char*)&m_HeightMapHeader[Layer], sizeof(HeightMapHeader));
		if (In.gcount() != sizeof(HeightMapHeader))
			return Log(MemoryErr);
		HeightMap[Layer] = new HeightMapType[m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height];
		for (int Point{ 0 }; Point < (m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height); Point++) {
			In.read((char*)&HeightMap[Layer][Point], sizeof(HeightMapType));
			if (In.gcount() != sizeof(HeightMapType))
				return Log(MemoryErr);
		}
		In.read((char*)&m_TextureLayerHeader[Layer], sizeof(TextureLayerHeader));
		if (In.gcount() != sizeof(TextureLayerHeader))
			return Log(MemoryErr);
		TextureLayer[Layer] = new TextureLayerType[m_TextureLayerHeader[Layer].MaterialCount];
		for (int Material{ 0 }; Material < m_TextureLayerHeader[Layer].MaterialCount; Material++) {
			In.read((char*)&TextureLayer[Layer][Material], sizeof(TextureLayerType));
			if (In.gcount() != sizeof(TextureLayerType))
				return Log(MemoryErr);
		}
	}
	In.read((char*)&m_ObjectHeader, sizeof(ObjectHeader));
	if (In.gcount() != sizeof(ObjectHeader))
		return Log(MemoryErr);
	Object = new ObjectType[m_ObjectHeader.ObjectCount];
	for (int ObjectID{ 0 }; ObjectID < m_ObjectHeader.ObjectCount; ObjectID++) { // Object routine:
		In.read((char*)&Object[ObjectID], sizeof(ObjectType));
		if (In.gcount() != sizeof(ObjectType))
			return Log(MemoryErr);
	}
	In.close();
	InMemory = true;
	return true;
}

void Seraph::FSM::Shutdown() {
	if (!InMemory)
		return;
	memset(&m_FSMHeader, 0, sizeof(CoreHeader));
	memset(&m_ObjectHeader, 0, sizeof(ObjectHeader));
	delete m_HeightMapHeader;
	delete m_TextureLayerHeader;
	m_HeightMapHeader = nullptr;
	m_TextureLayerHeader = nullptr;
	delete Object;
	delete[] HeightMap;
	delete[] TextureLayer;
	Object = nullptr;
	HeightMap = nullptr;
	TextureLayer = nullptr;
}
