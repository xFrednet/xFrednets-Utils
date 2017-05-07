#include "Options.h"

#include <vector>

using namespace std;
using namespace System::IO;
using namespace System::Text;
using namespace System::Xml;

#define CPP_ELEMENT_STRING(x)		std::string(#x)
#define CS_ELEMENT_STRING(x)		to_CS_String(CPP_ELEMENT_STRING(x))

#define XML_OPTIONS_NAMESPACE		"Options"

#define XML_BOOL_SAVE(x)			((x) ? "true" : "false")
#define XML_BOOL_LOAD(x)			((x == "true") ? true : false)

#define XML_UINT_SAVE(x)			(System::UInt32(x).ToString())
#define XML_UINT_LOAD(x)			System::UInt32::Parse(x)


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
		if (loadSaveDirectory())
		{
			if (!loadOptions())
			{
				System::Windows::Forms::MessageBox::Show("The Options couldn't be loaded!");
				resetOptions();
			} else
			{
				std::cout << "Options: Options loaded successfully" << std::endl;
			}
		} else
		{
			m_SaveDirectory = createSaveDirectory();
			resetOptions(); // => save Option
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
		System::String^ xmlString = loadStringFromFile(file);
		if (xmlString->Length > 0)
		{
			//init
			XmlDocument^ xml = gcnew XmlDocument();
			xml->LoadXml(xmlString);

			//loop
			XmlNode^ xmlMainChild;
			uint mainCount = xml->ChildNodes->Count;
			XmlElement^ xmlChild;
			uint count;
			for (uint mi = 0; mi < mainCount; mi++)
			{
				xmlMainChild = xml->ChildNodes->Item(mi);
				if (xmlMainChild->Name != XML_OPTIONS_NAMESPACE)
					continue;

				count = xmlMainChild->ChildNodes->Count;
				for (uint i = 0; i < count; i++)
				{
					xmlChild = (XmlElement^)xmlMainChild->ChildNodes->Item(i);
					cout << "XML: "<< to_CPP_String(xmlChild->Name).c_str() << " " << to_CPP_String(xmlChild->InnerText).c_str() << endl;
					//
					// Keylogger
					//
					if (xmlChild->Name == CS_ELEMENT_STRING(m_IsKeyloggerEnabled))
						m_IsKeyloggerEnabled = XML_BOOL_LOAD(xmlChild->InnerText);
					//
					//extra clip
					//
					if (xmlChild->Name == CS_ELEMENT_STRING(m_IsExtraClipEnabled))
						m_IsExtraClipEnabled = XML_BOOL_LOAD(xmlChild->InnerText);
					if (xmlChild->Name == CS_ELEMENT_STRING(m_ExtraClipShortcut))
						m_ExtraClipShortcut = loadKeyCombo(xmlChild);
					//
					//timers
					//
					if (xmlChild->Name == CS_ELEMENT_STRING(m_TimersEnabled))
						m_TimersEnabled = XML_BOOL_LOAD(xmlChild->InnerText);
					//
					//default
					//
					else
						cout << "Unknown save: " << to_CPP_String(xmlChild->InnerText).c_str() << endl;
				}
			}
			return true;
		}
		return false;
	}
	bool Options::saveOptions(String file) const
	{
		XmlDocument^ xml = gcnew XmlDocument();
		xml->AppendChild(xml->CreateXmlDeclaration("1.0", "UTF-8", nullptr));

		XmlElement^ xmlOptions = xml->CreateElement(XML_OPTIONS_NAMESPACE);
		XmlElement^ xmlOption;
		{
			//m_IsKeyloggerEnabled
			xmlOption = xml->CreateElement(CS_ELEMENT_STRING(m_IsKeyloggerEnabled));
			xmlOption->InnerText = XML_BOOL_SAVE(m_IsKeyloggerEnabled);
			xmlOptions->AppendChild(xmlOption);

			//m_IsExtraClipEnabled
			xmlOption = xml->CreateElement(CS_ELEMENT_STRING(m_IsExtraClipEnabled));
			xmlOption->InnerText = XML_BOOL_SAVE(m_IsExtraClipEnabled);
			xmlOptions->AppendChild(xmlOption);
			//m_ExtraClipShortcut
			xmlOptions->AppendChild(saveKeyCombo(m_ExtraClipShortcut, xml->CreateElement(CS_ELEMENT_STRING(m_ExtraClipShortcut)), xml));

			//m_TimersEnabled
			xmlOption = xml->CreateElement(CS_ELEMENT_STRING(m_TimersEnabled));
			xmlOption->InnerText = XML_BOOL_SAVE(m_TimersEnabled);
			xmlOptions->AppendChild(xmlOption);
		}
		
		xml->AppendChild(xmlOptions);
		
		return saveXmlToFile(OPTION_SAVE_FILE_NAME, xml);
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
