#include "Options.h"

namespace futils
{
	String Options::createSaveDirectory() const
	{
		
	}

	Options::Options()
	{
		if (!loadSaveDirectory())
		{
			resetOptions();
		}
	}

	void Options::resetOptions()
	{
		m_SaveDirectory = createSaveDirectory();
		
		m_IsKeyloggerEnabled = false;
		m_IsExtraClipEnabled = false;
		m_TimersEnabled = false;

		if (!saveOptions())
		{
			System::Windows::Forms::MessageBox::Show("The Options couldn't be saved!");
		}
	}
}
