//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.											//
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".			//
//I take no responsaiblity over this code or how it is used.							//
//This code was tested on a few Eden Eternal files, it is meant to be used in newer games, if you're modifying  //
//Grand Fantasia or any other older game, use "FSM.h" instead of this one.                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class FSM2 {
protected:
	char m_acSignature[0x54];
	unsigned long m_ulHeightMaps;

	struct HeightMapHeader {
		char m_acSignature[0x4];
		unsigned long m_ulBytes;
		unsigned long m_ulWidth;
		unsigned long m_ulHeight;
		float m_fScale;
		unsigned long m_ulNumCells;
	};

	struct CellHeader {
		unsigned long m_ulWidth;
		unsigned long m_ulHeight;
		float m_fScale;
		unsigned long m_ulUnknown2;
	};

	struct VertexData {
		float m_afPosition[3];
		float m_afNormal[3];
		float m_afColor[4];
	};

	struct Texture {
		bool m_bUsed;
		char m_acName[0x44];
		float m_fUnknown;
	};

	struct DarkMap {
		char m_acName[0x1D];
		Texture m_aTextures[5];
	};

	struct Cell {
		CellHeader m_Header;
		VertexData** m_ppVertices;
		DarkMap m_DarkMap;
	};

	HeightMapHeader m_HMHeader;
	Cell* m_pCells;

	struct ObjectCoreHeader {
		char m_acSignature[0x4];
		unsigned long m_ulNumObjects;
		unsigned long m_ulNumInstancedObjects;
	};

	struct ObjectInstanceHeader {
		char m_acSignature[0x4];
		unsigned long m_ulBytes;
		unsigned long m_ulNumInstances;
	};

	struct ObjectHeader {
		char m_acSignature[0x4];
		unsigned long m_ulBytes;
		unsigned long m_ulNumObjects;
	};

	struct InstancedObject {
		unsigned long m_ulID;
		char m_acName[0x28];
	};

	struct InstanceData {
		unsigned long m_ulUnknown1;
		float m_afPosition[3];
		float m_fRotation;
		unsigned long m_ulUnknown2;
		unsigned long m_ulUnknown3;
		float m_fScale;
	};

	struct ObjectData {
		float m_afPosition[3];
		unsigned long m_aulUnknown[8];
		float m_fRotation;
		float m_fScale;
		char m_acName[0x28];
	};

	ObjectCoreHeader m_ObjCoreHeader;
	ObjectInstanceHeader m_ObjInstanceHeader;
	ObjectHeader m_ObjHeader;
	InstancedObject* m_pObjInstances;
	InstanceData* m_pInstances;
	ObjectData* m_pObjects;

public:
	bool Load(const char* pcFileName);
};
