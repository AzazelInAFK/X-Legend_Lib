/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <vector>
#include <iomanip>

#include "Error.h"
#include "BMP.h"
#include "File.h"

#define STRINGSZ 0x28

namespace Seraph {
	class FSM { // File info:
	public:
		struct CoreHeader {
			char Signature[0x54];
			long HeightmapCount;
		} m_FSMHeader;
		struct HeightMapHeader {
			char Signature[4];
			unsigned long LinearSz;
			unsigned long Width;
			unsigned long Height;
			float Scale;
		} *m_HeightMapHeader;
		struct TextureLayerHeader {
			char Signature[4];
			unsigned long LinearSz;
			unsigned long MaterialCount;
		} *m_TextureLayerHeader;
		struct ObjectHeader {
			char Signature[4];
			unsigned long LinearSz;
			unsigned long ObjectCount;
		} m_ObjectHeader;
		struct HeightMapType {
			float HeightMap;
			unsigned char RGBA[4];
		}**HeightMap;
		struct TextureLayerType {
			char Material[STRINGSZ];
			char AlphaMap[STRINGSZ];
		}**TextureLayer;
		struct ObjectType {
			float X, Y, Z;
			float Rotation;
			float Scale;
			char Name[STRINGSZ];
		}*Object;
		
	protected:
		std::string m_FilePath;

		File m_Istr;
		bool InMemory{ false };
	private:
		std::vector<ObjectType> ImportedObj; // Extra objects, merged in the output file.
	public:
		bool Save(std::string OutputPath);
		bool Load(std::string FilePath);
		bool ExportHeightMapAsBMP(std::string FilePath);
		bool AsASCIIFile(bool WriteHeightmap = false);
		bool ImportHeightMapFromBMP(std::string FilePath, long HeightMapSlotID);
		bool ImportObject(float X, float Y, float Z, float Rotation, float Scale, const char* Name);
		void Shutdown();

	public: // Getters
		long GetHeightMapCount();
		long GetWidthAt(int HeightMapID);
		long GetHeightAt(int HeightMapID);
		long GetScaleAt(int HeightMapID);
		long GetMaterialCountAt(int TextureLayerID);
		long GetObjectCount();
	};
}
