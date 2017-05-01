#pragma once

#include <KeyLogger.h>

#include <windows.h>
#include <iostream>
#include <time.h>
#include <string>
#include <list>
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
}
