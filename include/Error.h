/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.										   //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		   //
//I take no responsaiblity over this code or how it is used.						   //
//This code was tested on Grand Fantasia files, it may or may not work on AK, EE or another X-Legend game. //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Windows.h>
#include <direct.h>
#include <string>
#include <sstream>
#include <fstream>

static HRESULT hr;

#define Log(Msg) Seraph::Error::_Log(Msg, __FILE__, __LINE__)

namespace Seraph {
	void RecurseDirCreate(std::string FilePath);
	namespace Error {
		static unsigned short m_ErrorCount;
		static std::stringstream m_ErrorMessage;

		void Console(float Val);
		bool _Log(std::string Msg, std::string File, int Line);
		bool Throw();
	};
}
