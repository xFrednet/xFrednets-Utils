#pragma once

#include "Futils.h"

#define SAVE_DIRECTROY_FILE_NAME			"saveDir.txt"
#define OPTION_SAVE_FILE_NAME				"options.xml"

#define REG_AUTOSTART_KEY_NAME			"FUtils"

namespace futils
{
	class Options
	{
	private:
		bool m_IsKeyloggerEnabled;

		//clipboard
		bool m_IsExtraClipEnabled;
		KeyCombo m_ExtraClipShortcut;

		//timer
		bool m_TimersEnabled;

		//save directory
		String m_SaveDirectory;

		String createSaveDirectory() const;
	public:
		Options();

		void printInfo() const;

		void resetOptions();
		bool loadSaveDirectory();
		bool setSaveDirectory(String directory);
		bool loadOptions(String file = OPTION_SAVE_FILE_NAME);
		bool saveOptions(String file = OPTION_SAVE_FILE_NAME) const;
		
		//
		// IO stuff
		//
		void insureSaveDirExistence() const;
		System::String^ loadStringFromFile(String fileName) const;
		bool saveStringToFile(String fileName, String content) const;
		bool saveXmlToFile(String fileName, System::Xml::XmlDocument^ xml) const;


		void setAutostart(bool enabled);
		bool isAutoStartEnabled();

		//
		// Getters
		//
		inline bool isKeyloggerEnabled() const
		{
			return m_IsKeyloggerEnabled;
		}

		//clip
		inline bool isExtraetClipEnabled() const
		{
			return m_IsExtraClipEnabled;
		}
		inline KeyCombo getExtraClipShortcut() const
		{
			return m_ExtraClipShortcut;
		}

		//timer
		inline bool areTimersEnabled() const
		{
			return m_TimersEnabled;
		}

		//save Directory
		inline String getSaveDir() const
		{
			return m_SaveDirectory;
		}

	};
}
