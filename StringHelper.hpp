#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#include <string>

inline std::string wstring2utf8string(const wchar_t* input, size_t m_encode = CP_UTF8) {
    int size = WideCharToMultiByte(m_encode, 0, input, wcslen(input), 0, 0,
        nullptr, nullptr);
    std::string result(size, 0);
    if (size) {
        WideCharToMultiByte(m_encode, 0, input, wcslen(input), &result[0], size,
            nullptr, nullptr);
    }

    return result;
}

inline std::wstring utf8string2wstring(const std::string& input) {
    size_t nLen = input.length();
    if (nLen == 0)
        return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)input.c_str(), (int)nLen, NULL, 0);
    if (size <= 0)
        return L"";

    std::wstring wstr;
    wstr.resize(size + 1);

    int nResult = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)input.c_str(), (int)nLen, (LPWSTR)&wstr[0], size);
    if (nResult == 0) {
        return L"";
    }

    wstr.resize(nResult);
    return wstr;
}

inline std::string wstring2string(const wchar_t* input, size_t mEncode = CP_UTF8) {
    int size = WideCharToMultiByte(mEncode, 0, input, wcslen(input), 0, 0,
        nullptr, nullptr);
    std::string result(size, 0);
    if (size) {
        WideCharToMultiByte(mEncode, 0, input, wcslen(input), &result[0], size,
            nullptr, nullptr);
    }

    return result;
}

inline std::wstring string2wstring(const std::string& input, size_t mEncode = CP_UTF8) {
    size_t nLen = input.length();
    if (nLen == 0)
        return L"";
    int size = MultiByteToWideChar(mEncode, 0, (LPCSTR)input.c_str(), (int)nLen, NULL, 0);
    if (size <= 0)
        return L"";

    std::wstring wstr;
    wstr.resize(size + 1);

    int nResult = MultiByteToWideChar(mEncode, 0, (LPCSTR)input.c_str(), (int)nLen, (LPWSTR)&wstr[0], size);
    if (nResult == 0) {
        return L"";
    }

    wstr.resize(nResult);
    return wstr;
}
#endif // _WIN32:check is windows or not
