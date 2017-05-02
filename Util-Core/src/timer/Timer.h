#pragma once

#include "..\Futils.h"

namespace futils
{
	String TimeToString(const time_t& time);

	class Timer
	{
		String m_Name;
		time_t m_StartTime;
	public:
		Timer(String name, time_t startTime = time(0));
		Timer(String name, String saveData);

		time_t getTime() const;
		String getSaveString() const;
		
		String getName() const;
		String getInfoString() const;
	};

}
/*
 * String Loader::LoadFile(String file)
	{
		using namespace std;

		String content;
		fstream fileStream;
		fileStream.open(file);

		//error check
		if (!fileStream.is_open())
		{
			ZA_ERROR("Could not open: ", file);
			return "";
		}

		//loading file
		String line;
		while (!fileStream.eof())
		{
			getline(fileStream, line);
			content += line + "\n";
		}

		return content;
	}
 */