#pragma once

#include <KeyLogger.h>

#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <list>
#include <shlwapi.h>
#include "shlobj.h"
#pragma comment(lib,"shlwapi.lib")
#include <msclr\marshal_cppstd.h>
#using <mscorlib.dll>

typedef unsigned int uint;
typedef std::string String;

namespace futils
{
	static System::String^ to_CS_String(std::string stdString)
	{
		return msclr::interop::marshal_as<System::String^>(stdString);
	}
	static String to_CPP_String(System::String^ csString)
	{
		return msclr::interop::marshal_as<String>(csString);
	}

	String String_Replace(String string, String oldStr, String newStr)
	{
		if (string.empty() || oldStr.empty())
			return string;

		size_t offset = 0;
		size_t pos;

		while ((pos = string.find(oldStr, offset)) != string.npos) {
			string.replace(pos, oldStr.size(), newStr);
			offset = pos + newStr.size();
		}

		return string;
	}
}
