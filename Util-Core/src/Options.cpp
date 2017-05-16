#include "Options.h"

#include <vector>

using namespace std;
using namespace System::IO;
using namespace System::Text;
using namespace System::Xml;

#define CPP_ELEMENT_STRING(x)		std::string(#x)
#define CS_ELEMENT_STRING(x)		to_CS_String(CPP_ELEMENT_STRING(x))

#define XML_OPTIONS_NAMESPACE		"Options"


namespace futils
{
	String Options::createSaveDirectory() const
	{
		char buffer[256];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buffer)))
		{
			return String(buffer) + "\\Futils\\";
		} else
		{
			if (GetModuleFileName(NULL, buffer, 256))
			{
				String dir(buffer);
				return dir.substr(0, dir.find_last_of("//"));
			} else
			{
				System::Windows::Forms::MessageBox::Show("The save directory file couldn't be opened and no directory could be selected.");
				return "";
			}
		}
		;
	}

	Options::Options()
	{
		if (loadOptions())
		{
			std::cout << "Options: Options loaded successfully" << std::endl;
			printInfo();
		} else
		{
			m_SaveDirectory = createSaveDirectory();
			System::Windows::Forms::MessageBox::Show("The Options couldn't be loaded!");
			resetOptions();
		}
		printInfo();
	}

	void Options::printInfo() const
	{
		cout << "Options: {" << endl;
		cout << "	m_IsKeyloggerEnabled	:" << ((m_IsKeyloggerEnabled) ? "true" : "false") << endl;
		cout << "	m_IsExtraClipEnabled	:" << ((m_IsExtraClipEnabled) ? "true" : "false") << endl;
		cout << "	m_ExtraClipShortcut		:" << m_ExtraClipShortcut.getString() << endl;
		cout << "	m_TimersEnabled			:" << ((m_TimersEnabled) ? "true" : "false") << endl;
		cout << "	m_SaveDirectory:		:" << m_SaveDirectory.c_str() << endl;
		cout << "}" << endl;
	}

	void Options::resetOptions()
	{
		m_IsKeyloggerEnabled = false;
		m_IsExtraClipEnabled = false;
		m_ExtraClipShortcut = KeyCombo({ VK_LCONTROL, VK_ALT, 'V' });
		m_TimersEnabled = false;

		if (!saveOptions())
		{
			System::Windows::Forms::MessageBox::Show("The Options couldn't be saved!");
		}
	}
	bool Options::loadSaveDirectory()
	{
		fstream file;
		file.open(SAVE_DIRECTROY_FILE_NAME);

		if (file.is_open())
		{
			getline(file, m_SaveDirectory);
			file.close();
			if (m_SaveDirectory.length() > 0)
				return true;
		}
		return false;
	}
	bool Options::setSaveDirectory(String directory)
	{
		FILE* file = fopen(SAVE_DIRECTROY_FILE_NAME, "w");
		if (file)
		{
			fprintf_s(file, "%s", directory.c_str());
			fclose(file);
			m_SaveDirectory = directory;
			return true;
		}
		return false;
	}

	//XML
	XmlElement^ saveKeyCombo(KeyCombo combo, XmlElement^ xmlObj, XmlDocument^ xml)
	{
		vector<unsigned> keys = combo.getKeys();
		XmlElement^ xmlKey;
		for (uint i : keys)
		{
			xmlKey = xml->CreateElement("Key");
			xmlKey->InnerText = XML_UINT_SAVE(i);
			xmlObj->AppendChild(xmlKey);
		}
		return xmlObj;
	}
	KeyCombo loadKeyCombo(XmlElement^ xmlObj)
	{
		vector<unsigned> keys;
		uint count = xmlObj->ChildNodes->Count;
		XmlElement^ xmlKey;
		for (uint i = 0; i < count; i++)
		{
			xmlKey = (XmlElement^)xmlObj->ChildNodes->Item(i);
			if (xmlKey->Name == "Key")
				keys.push_back(XML_UINT_LOAD(xmlKey->InnerText));
		}

		return KeyCombo(keys);
	}

	bool Options::loadOptions(String file)
	{
		HKEY key;
		if (RegCreateKeyEx(HKEY_CURRENT_USER,
			REG_SAVE_DIR,
			NULL,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&key,
			NULL) != ERROR_SUCCESS) 
		{
			cout << "Options: The Options couldn't be loaded!" << endl;
			return false;
		}
		DWORD dataSize;
		char data[256];
		//keylogger
		dataSize = 1;
		if (RegQueryValueEx(key, REG_KEY_KEYLOGGER_ENABLED, NULL, NULL, (BYTE*)&m_IsKeyloggerEnabled, &dataSize) != ERROR_SUCCESS)
			cout << "Options: The keylogger state couldn't be saved." << endl;

		//extra clip
		dataSize = 1;
		if (RegQueryValueEx(key, REG_KEY_EXTRACLIP_ENABLED, NULL, NULL, (BYTE*)&m_IsExtraClipEnabled, &dataSize) != ERROR_SUCCESS)
			cout << "Options: The ExtraClip state couldn't be saved." << endl;
		dataSize = 256;
		if (RegQueryValueEx(key, REG_KEY_EXTRACLIP_SHORCUT, NULL, NULL, (BYTE*)data, &dataSize) == ERROR_SUCCESS)
		{
			std::vector<unsigned> shortcut(dataSize / sizeof(unsigned));
			memcpy(&shortcut[0], data, dataSize);
			m_ExtraClipShortcut.setKeys(shortcut);
		} else
		{
			cout << "Options: The keylogger shortcut couldn't be saved." << endl;
		}

		//timers
		dataSize = 1;
		if (RegQueryValueEx(key, REG_KEY_TIMERS_ENABLED, NULL, NULL, (BYTE*)&m_TimersEnabled, &dataSize) != ERROR_SUCCESS)
			cout << "Options: The Timer state couldn't be saved." << endl;

		//save directory
		dataSize = 256;
		memset(data, 0, dataSize);
		if (RegQueryValueEx(key, REG_KEY_SAVEDIR, NULL, NULL, (BYTE*)data, &dataSize) == ERROR_SUCCESS)
			m_SaveDirectory = String(data);
		else
			cout << "Options: The save directory couldn't be saved." << endl;

		RegCloseKey(key);
		return true;
	}
	bool Options::saveOptions(String file) const
	{
		HKEY key;
		if (RegCreateKeyEx(HKEY_CURRENT_USER,
			REG_SAVE_DIR,
			NULL,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&key, 
			NULL) != ERROR_SUCCESS)
		{
			cout << "Options: The Options couldn't be saved!" << endl;
			return false;
		}
		
		//keylogger
		if (RegSetValueEx(key, REG_KEY_KEYLOGGER_ENABLED, NULL, REG_BINARY, (BYTE*)&m_IsKeyloggerEnabled, 1) != ERROR_SUCCESS)
			cout << "Options: The keylogger state couldn't be saved." << endl;

		//extra clip
		if (RegSetValueEx(key, REG_KEY_EXTRACLIP_ENABLED, NULL, REG_BINARY, (BYTE*)&m_IsExtraClipEnabled, 1) != ERROR_SUCCESS)
			cout << "Options: The ExtraClip state couldn't be saved." << endl;
		std::vector<unsigned> shortcut = m_ExtraClipShortcut.getKeys();
		if ((shortcut.size() == 0) || RegSetValueEx(key, REG_KEY_EXTRACLIP_SHORCUT, NULL, REG_BINARY, (BYTE*)&shortcut[0], sizeof(unsigned) * shortcut.size()) != ERROR_SUCCESS)
			cout << "Options: The keylogger shortcut couldn't be saved." << endl;
		
		//timers
		if (RegSetValueEx(key, REG_KEY_TIMERS_ENABLED, NULL, REG_BINARY, (BYTE*)&m_TimersEnabled, 1) != ERROR_SUCCESS)
			cout << "Options: The Timer state couldn't be saved." << endl;

		//save directory
		if (RegSetValueEx(key, REG_KEY_SAVEDIR, NULL, REG_SZ, (BYTE*)m_SaveDirectory.c_str(), m_SaveDirectory.length() + 1) != ERROR_SUCCESS)
			cout << "Options: The save directory couldn't be saved." << endl;

		RegCloseKey(key);
		cout << "Options: The Options have been saved." << endl;
		return true;
	}

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

	//
	// IO stuff
	//
	void Options::insureSaveDirExistence() const
	{
		if (Directory::Exists(to_CS_String(m_SaveDirectory)))
		{
			Directory::CreateDirectory(to_CS_String(m_SaveDirectory));
		}
	}
	System::String^ Options::loadStringFromFile(String fileName) const
	{
		try
		{
			return File::ReadAllText(to_CS_String(m_SaveDirectory + fileName), Encoding::UTF8);
		} catch (System::Exception^ e)
		{
			return "";
		}
	}
	bool Options::saveStringToFile(String fileName, String content) const
	{
		if (content.length() == 0)
			return false;

		try
		{
			insureSaveDirExistence();
			
			StreamWriter^ writer = gcnew StreamWriter(to_CS_String(m_SaveDirectory + fileName), false, Encoding::UTF8);
			writer->Write(to_CS_String(content));
			writer->Close();
			
			return true;
		} catch (System::Exception^ e)
		{
			return false;
		}
	}
	bool Options::saveXmlToFile(String fileName, System::Xml::XmlDocument^ xml) const
	{
		try
		{
			insureSaveDirExistence();
			xml->Save(gcnew StreamWriter(to_CS_String(m_SaveDirectory + fileName), false, Encoding::UTF8));
			return true;
		} catch (System::Exception^ e)
		{
			return false;
		}
	}
}
