#pragma once
/*
* This is a KeyLogger for Windows. While this program can potentially
* be used for nefarious purposes, it was written for educational and recreational
* purposes only and the author does not endorse illegal use.
*/

#include <functional>
#include <vector>
#include <string>
#include <windows.h> // macros

#pragma warning(push)
#pragma warning(disable: 4251)

#ifndef KEY_STATE_SIZE
#	define KEY_STATE_SIZE 0xff
#endif

#if 1
#	define KEYLOGGER_API __declspec(dllexport)
#else
#	define KEYLOGGER_API __declspec(dllimport)
#endif

KEYLOGGER_API typedef unsigned char byte;
KEYLOGGER_API typedef std::function<void(unsigned key, bool newPress, byte flags)> KeyLoggerCallback;
KEYLOGGER_API typedef std::function<void(bool newPress)> KeyComboCallback;

/* //////////////////////////////////////////////////////////////////////////////// */
// // Util //
/* //////////////////////////////////////////////////////////////////////////////// */
KEYLOGGER_API std::string KeyToText(unsigned key, byte flags = 0);

#define FLAG_SHIFT_MASK			0x01
#define FLAG_CONTROL_MASK		0x02
#define FLAG_ALT_MASK			0x04
#define FLAG_CAPS_MASK			0x08
#define FLAG_WIN_MASK			0x10

#define IS_FLAG_SET(flags, mask) ((flags | mask) == flags)

/* //////////////////////////////////////////////////////////////////////////////// */
// // KeyCombo //
/* //////////////////////////////////////////////////////////////////////////////// */
class KeyLogger;
class KEYLOGGER_API KeyCombo
{
private:
	friend class KeyLogger;
private:
	KeyComboCallback m_CallBack;
	
	std::vector<unsigned> m_Keys;
	
	void sortKeys();
	void callCallBack(bool newPress);
public:
	KeyCombo();
	KeyCombo(std::vector<unsigned> keys, KeyComboCallback callBack = KeyComboCallback());
	~KeyCombo();

	void setCallBack(KeyComboCallback callBack);

	std::string getString() const;
	bool operator==(const KeyCombo& combo) const;
};

/* //////////////////////////////////////////////////////////////////////////////// */
// // KeyLogger //
/* //////////////////////////////////////////////////////////////////////////////// */
class KEYLOGGER_API KeyLogger
{
private:
	std::vector<KeyLoggerCallback> m_CallBacks;
	std::vector<KeyCombo> m_KeyCombos;
	bool m_KeyStates[0xff];
	byte m_Flags;

public:
	KeyLogger();
	~KeyLogger();

	void keyEvent(unsigned key, bool pressed);

	void addCallBack(const KeyLoggerCallback& callback);
	void addKeyCombo(const KeyCombo& combo);
	KeyCombo getCurrentCombo();

	void startCallBackLoop() const;
	void update() const;
};

#define VK_ENTER	VK_RETURN
#define VK_ALT		VK_LMENU
#define VK_ALT_GR	VK_RMENU

#undef VK_CONTROL
#undef VK_SHIFT
#undef VK_WIN

#pragma warning(pop)
