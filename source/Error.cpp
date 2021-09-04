#include "Error.h"

void Seraph::RecurseDirCreate(std::string FilePath) {
	struct stat buffer;
	const std::string Dir{ FilePath };
	size_t index1{ 0 }, index2{ Dir.find('\\',index1) };
	std::string Exists;
	do {
		std::string RecurseDir;
		RecurseDir.assign(Exists);
		RecurseDir.append(Dir.substr(index1, index2 - index1));
		if (stat(RecurseDir.c_str(), &buffer))
			_mkdir(RecurseDir.c_str());
		index1 = index2;
		index2 = Dir.find('\\', ++index2);
		Exists.assign(RecurseDir);
	} while (index2 != -1);
}

bool Seraph::Error::_Log(std::string Msg,std::string File,int Line) {
	m_ErrorMessage << File << ", Line: " << Line << "\n\n\n";
	m_ErrorMessage << m_ErrorCount++ << ": " + Msg << std::endl;
	return false;
}

bool Seraph::Error::Throw() {
	if (m_ErrorMessage.str().empty()) {
		m_ErrorMessage << "Invalid throw, unknown error. " << m_ErrorCount << std::endl;
		MessageBoxA(NULL, m_ErrorMessage.str().c_str(), "Seraph", MB_ICONEXCLAMATION | MB_OK);
		return true;
	}
	if (!m_ErrorMessage.str().empty())
		MessageBoxA(NULL, m_ErrorMessage.str().c_str(), "Seraph : ERROR", MB_ICONEXCLAMATION | MB_OK);
	m_ErrorMessage.str(std::string());
	return false;
}

void Seraph::Error::Console(float val) {
	std::ostringstream buff;
	buff << val << std::endl;
	OutputDebugStringA(buff.str().c_str());
}