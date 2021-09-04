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