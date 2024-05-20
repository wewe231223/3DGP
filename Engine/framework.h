#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// C 런타임 헤더 파일입니다.
#include <tchar.h>

// C++ 런타임 헤더 파일입니다.
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <variant>
#include <type_traits>
#include <fstream>
#include <algorithm>
#include <regex>
#include <iostream>
#include <unordered_map>
#include <functional>


namespace fs = std::filesystem;

#ifdef UNICODE
namespace std {
	using tstring = std::wstring;
}
#else 
namespace std{
	using tstring = std::string;
}
#endif

