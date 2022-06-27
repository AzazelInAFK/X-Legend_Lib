/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FSM.h"

bool Seraph::FSM::AsASCIIFile(bool WriteHeightmap) {
	File out;
	out.open("FSMOut.txt", std::ios::in);
	Assert(InMemory);
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
	Assert(Img.Import(FilePath));
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
		Assert(BMP.Export(FilePath, m_HeightMapHeader[Layer].Width, m_HeightMapHeader[Layer].Height, transfData));
	}
	return true;
}

bool Seraph::FSM::ImportObject(float X, float Y, float Z, float Rotation, float Scale, const char* Name) {
	int iSize = strlen(Name);

	if (iSize > STRINGSZ)
		return false;
	ObjectType Buffer;
	Buffer.X = X;
	Buffer.Y = Y;
	Buffer.Z = Z;
	Buffer.Rotation = Rotation;
	Buffer.Scale = Scale;
	for (int i{ 0 }; i < iSize; i++)
		Buffer.Name[i] = Name[i];
	for (int i{ iSize }; i < STRINGSZ; i++)
		Buffer.Name[i] = '\0';

	ImportedObj.push_back(Buffer);

	return true;
}

bool Seraph::FSM::Save(std::string OutputPath) {
	File Out;
	Assert(Out.open(OutputPath, std::ios::out | std::ios::binary));
	Assert(Out.write(&m_FSMHeader));
	for (int Layer{ 0 }; Layer < m_FSMHeader.HeightmapCount; Layer++) {
		Assert(Out.write(&m_HeightMapHeader[Layer]));
		Assert(Out.write(HeightMap[Layer], m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height));
		Assert(Out.write(&m_TextureLayerHeader[Layer]));
		Assert(Out.write(TextureLayer[Layer], m_TextureLayerHeader[Layer].MaterialCount));
	}
	m_ObjectHeader.ObjectCount += ImportedObj.size();
	m_ObjectHeader.LinearSz += ImportedObj.size() * sizeof(ObjectType);
	Assert(Out.write(&m_ObjectHeader));
	Assert(Out.write(Object, m_ObjectHeader.ObjectCount - ImportedObj.size()));
	Assert(Out.write(ImportedObj.data(), ImportedObj.size()));
	return true;
}

bool Seraph::FSM::Load(std::string FilePath) {
	Assert (!FilePath.empty());
	m_FilePath.assign(FilePath);
	Assert(m_Istr.open(FilePath, std::ios::in | std::ios::binary));
	Assert(m_Istr.read(&m_FSMHeader));
	m_HeightMapHeader = new HeightMapHeader[m_FSMHeader.HeightmapCount];
	m_TextureLayerHeader = new TextureLayerHeader[m_FSMHeader.HeightmapCount];
	HeightMap = new HeightMapType * [m_FSMHeader.HeightmapCount];
	TextureLayer = new TextureLayerType * [m_FSMHeader.HeightmapCount];
	for (int Layer{ 0 }; Layer < m_FSMHeader.HeightmapCount; Layer++) { // Terrain routine:
		Assert(m_Istr.read(&m_HeightMapHeader[Layer]));
		HeightMap[Layer] = new HeightMapType[m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height];
		Assert(m_Istr.read(HeightMap[Layer], m_HeightMapHeader[Layer].Width * m_HeightMapHeader[Layer].Height));
		Assert(m_Istr.read(&m_TextureLayerHeader[Layer]));
		TextureLayer[Layer] = new TextureLayerType[m_TextureLayerHeader[Layer].MaterialCount];
		Assert(m_Istr.read(TextureLayer[Layer], m_TextureLayerHeader[Layer].MaterialCount));
	}
	Assert(m_Istr.read(&m_ObjectHeader));
	Object = new ObjectType[m_ObjectHeader.ObjectCount];
	Assert(m_Istr.read(Object, m_ObjectHeader.ObjectCount));
	m_Istr.close();
	InMemory = true;
	return true;
}

long Seraph::FSM::GetHeightMapCount() {
	return m_FSMHeader.HeightmapCount;
}

long Seraph::FSM::GetWidthAt(int HeightMapID) {
	return m_HeightMapHeader[HeightMapID].Width;
}

long Seraph::FSM::GetHeightAt(int HeightMapID) {
	return m_HeightMapHeader[HeightMapID].Height;
}

long Seraph::FSM::GetScaleAt(int HeightMapID) {
	return m_HeightMapHeader[HeightMapID].Scale;
}

long Seraph::FSM::GetMaterialCountAt(int TextureLayerID) {
	return m_TextureLayerHeader[TextureLayerID].MaterialCount;
}

long Seraph::FSM::GetObjectCount() {
	return m_ObjectHeader.ObjectCount;
}

void Seraph::FSM::Shutdown() {
	if (!InMemory)
		return;
	m_Istr.close();
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
