#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include "Error.h"

namespace Seraph {
	class SMP {
		short SignatureSz;
		char* Signature;
		short LinkSz;
		char* Link;
		struct Header {
			int Unknown;
			short Width;
			short Height;
			int Unknown2;
		} m_Header;

		unsigned int FullSz;
		unsigned char* Data;
		std::ifstream File;
	private:
		bool SecurityCheck();
	public:
		bool Load(std::string FilePath);

		unsigned char* GetData();
		unsigned int GetDataSize();

		short GetWidth();
		short GetHeight();
		std::string GetRelatedScene();
	};
}