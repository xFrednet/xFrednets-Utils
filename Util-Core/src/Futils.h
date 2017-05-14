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

typedef unsigned int		uint;
typedef std::string			String;

#define XML_BOOL_SAVE(x)			((x) ? "true" : "false")
#define XML_BOOL_LOAD(x)			((x == "true") ? true : false)

#define XML_UINT_SAVE(x)			(System::UInt32(x).ToString())
#define XML_UINT_LOAD(x)			System::UInt32::Parse(x)

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
}
