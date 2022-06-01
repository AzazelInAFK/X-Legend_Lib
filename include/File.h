/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <fstream>
#include <iostream>

#include "Error.h"

namespace Seraph {
	class File : public std::fstream {
	public:
		inline bool open(const std::string& _Str, std::ios_base::openmode _Mode);
		inline bool open(const char* _Filename, std::ios_base::openmode _Mode);

		template<typename T>
		bool read(T* pData, size_t szNumElements = 1) {
			std::ios_base::iostate state = std::fstream::read((char*)pData, sizeof(T) * szNumElements).rdstate();
			AssertC(std::fstream::gcount() == sizeof(T) * szNumElements && state == std::fstream::ios_base::goodbit, state);
			return true;
		}
		template<typename T>
		bool write(T* pData, size_t szNumElements = 1) {
			std::ios_base::iostate state = std::fstream::write((char*)pData, sizeof(T) * szNumElements).rdstate();
			AssertC(state == std::fstream::ios_base::goodbit, state);
			return true;
		}
		template<typename T>
		T get(){
			T Data;
			assert(read(&Data));
			return Data;
		};
	};
}

inline bool Seraph::File::open(const std::string& _Str, std::ios_base::openmode _Mode) {
	std::fstream::open(_Str, _Mode);
	AssertCM(std::fstream::is_open(), std::fstream::rdstate(), _Str.c_str());
	return true;
}

inline bool Seraph::File::open(const char* _Filename, std::ios::openmode _Mode) {
	std::fstream::open(_Filename, _Mode);
	AssertCM(std::fstream::is_open(), std::fstream::rdstate(), _Filename);
	return true;
}
