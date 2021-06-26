/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.																				   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".				   //
//I take no responsaiblity over this code or how it is used.											   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <bitset>
#include <Windows.h>

class FSM {
	int Offset = 0x20; // 32
private:
	std::ifstream File;
	struct Position {
		float X, Y, Z;
	};
	struct ObjInfo {
		Position Coords;
		float RotRad, RotDeg;
		float Scale;
		std::string Name;
	};
	std::vector<ObjInfo> m_ObjData;
	struct Mesh {
		float* HeightMap;
		short Width, Height;
		long LinearSz;
		std::string* Material;
		std::string* AlphaMap;
	};
	std::vector<Mesh> m_MeshData;
private:
	bool LoadAREA(std::string FilePath); // Not used!
	bool LogError(std::string ErrorDesc);
	bool BatchProcessHM(unsigned short Position);
	void BatchProcessTL(unsigned short Position);
	bool BatchProcessOBJ();

	bool Skip(int Count);

	float LittleEndian32f();
	std::string GetName();
public:
	bool LoadFSM(std::string FilePath);

	int GetGroundCount();
	int GetObjCount();
	const Mesh& GetGroundDataAt(unsigned short MeshIndex);
	const ObjInfo& GetObjDataAt(unsigned short ObjIndex);
};
