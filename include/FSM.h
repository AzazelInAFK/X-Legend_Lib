#pragma once

#include "Error.h"
#include "BMP.h"

#include <vector>
#include <fstream>
#include <iomanip>

#define STRINGSZ 0x28

namespace Seraph {
	class FSM { // File info:
		struct FSMHeader {
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
	private: // File data:
		struct HeightMapType {
			float HeightMap;
			float Dummy;
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

		bool InMemory{ false };
	public:
		bool Load(std::string FilePath);
		bool ExportHeightMapAsBMP(std::string FilePath);
		bool AsASCIIFile(bool WriteHeightmap = false);
	};
}