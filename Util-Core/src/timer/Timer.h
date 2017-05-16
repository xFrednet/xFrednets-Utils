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
/*
void Options::setAutostart(bool enabled)
{
	HKEY key;
	if (RegOpenKeyEx(
		HKEY_CURRENT_USER, 
		"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_ALL_ACCESS,
		&key) == ERROR_SUCCESS)
	{
		if (enabled)
		{
			char buffer[256];
			memset(buffer, 0, 256);
			if (GetModuleFileName(NULL, &buffer[1], 256 - 2)) {
				buffer[0] = '\"';
				buffer[strlen(buffer)] = '\"';
				if (!RegSetValueEx(key, REG_AUTOSTART_KEY_NAME, NULL, REG_SZ, (BYTE*)buffer, strlen(buffer) + 1) == ERROR_SUCCESS)
					cout << "The value couldn't be set" << endl;
			}
		} else
		{
			if (!RegDeleteValue(key, REG_AUTOSTART_KEY_NAME) == ERROR_SUCCESS)
				cout << "Key couldn't be deleted" << endl;
		}
		RegCloseKey(key);
	} else
	{
		cout << "key couldn't be opened" << endl;
	}
}

if (RegOpenKeyEx(
HKEY_CURRENT_USER,
REG_SAVE_DIR,
0,
KEY_ALL_ACCESS,
&key) == ERROR_SUCCESS)
else
 */
