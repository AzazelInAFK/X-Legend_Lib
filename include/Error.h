#pragma once

#include <Windows.h>
#include <comdef.h>
#include <sstream>

static HRESULT g_shr;

#ifdef _DEBUG
#define AssertC(Expr, Code) \
    if(!Err(#Expr, Expr, __FILE__, __LINE__, Code)) return false
#define Assert(Expr) \
    if(!Err(#Expr, Expr, __FILE__, __LINE__)) return false
#define AssertCM(Expr, Code, Message) \
    if(!Err(#Expr, Expr, __FILE__, __LINE__, Code, Message)) return false
#define AssertM(Expr, Message) \
    if(!Err(#Expr, Expr, __FILE__, __LINE__, UINT32_MAX, Message)) return false
#else
#define AssertC(Expr, Code) \
    if(!Err(Expr, Code)) return false
#define Assert(Expr) \
    if(!Err(Expr)) return false
#define AssertM(Expr, Message) \
    if(!Err(Expr, Code, UINT32_MAX, Message)) return false
#define AssertCM(Expr, Code, Message) \
    if(!Err(Expr, Code, Message)) return false
#endif


static bool Err(bool expr, HRESULT hr = UINT32_MAX, const char* pMsg = nullptr)
{
    if (!expr)
    {
        std::wstringstream wcErrDesc;
        wcErrDesc << "Assert failed." << std::endl;
        if (pMsg)
            wcErrDesc << "Error description:\t\t" << pMsg << std::endl;
        if (hr != UINT32_MAX) {
            _com_error errbuffer(hr);
            wcErrDesc << L"Code:\t" << errbuffer.ErrorMessage() << "\n\n";
        }
        MessageBoxW(nullptr, wcErrDesc.str().c_str(), nullptr, 0);
    }
    return expr;
}

static bool Err(const char* expr_str, bool expr, const char* file, int line, HRESULT hr = UINT32_MAX, const char* pMsg = nullptr)
{
    if (!expr)
    {
        std::wstringstream wcErrDesc;
        wcErrDesc << "Assert failed." << std::endl;
        if (pMsg)
            wcErrDesc << "Error description:\t" << pMsg << std::endl;
        if (hr != UINT32_MAX) {
            _com_error errbuffer(hr);
            wcErrDesc << "Code:\t\t" << errbuffer.ErrorMessage() << "\n\n";
        }
        wcErrDesc   << "Expression:\t" << expr_str << "\n"
                    << "Source:\t\t" << file << ", line " << line << "\n";
        if (MessageBoxW(nullptr, wcErrDesc.str().c_str(), nullptr, MB_OKCANCEL) == IDCANCEL)
            return true;
    }
    return expr;
}
