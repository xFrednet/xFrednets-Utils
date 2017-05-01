#include "Futils.h"
#include "timer\Timer.h"
#include "timer\CountdownTimer.h"

using namespace std;
using namespace futils;
using namespace System::Xml;
using namespace System::IO;
using namespace System::Text;

/* //////////////////////////////////////////////////////////////////////////////// */
// // Values //
/* //////////////////////////////////////////////////////////////////////////////// */
#define EX_CLASS_NAME				"xFrednet's utils"
#define PASSWORD_MANAGER_EXE_NAME	"PasswordManager.exe"

//general
bool contineLoop;

//system tray
#define ID_TRAY_APP_ICON			1001
#define ID_TRAY_MENU_TIMER			1002
#define ID_TRAY_MENU_PASSWORDS		1003
#define ID_TRAY_MENU_OPTIONS		1004
#define ID_TRAY_MENU_EXIT			1005
#define WM_TRAY						(WM_USER + 1)
HMENU				tray_hmenu_;
uint				tray_creationID_;
NOTIFYICONDATA		tray_data_;
char				tray_tooltip[128] = "xFrednet's utils\nClick on this icon for more info.";

//timers
#define ID_TIMER_CREATE				2000
#define ID_TIMER_LIST				2001
#define TIMER_SAVE_FILE				"Timers.xml"
list<Timer*>		timer_list_;
HMENU				timer_hmenu_;

//clipboard
#define CLIP_SLOT_COUNT				10
#define CLIP_BUTTON_HEIGHT			20
#define CLIP_BUTTON_WIDTH			100
#define CLIP_BUTTON_BASE_ID			100
HWND								clip_menu_buttons_[CLIP_SLOT_COUNT];
typedef struct CLIP_SLOT_ {
	uint Size;
	void* Data;
} CLIP_SLOT;
bool								clip_slots_blocked = false;
CLIP_SLOT							clip_slots_[CLIP_SLOT_COUNT];

//window
#define WINDOW_WIDTH				CLIP_BUTTON_WIDTH + 300
#define WINDOW_HEIGHT				CLIP_BUTTON_HEIGHT * CLIP_SLOT_COUNT
#define WINDOW_STYLE				(WS_POPUP | WS_THICKFRAME)
HWND hwnd_;
HWND forground_hwnd_;
int hwnd_border_width_;
int hwnd_border_height_;

//XML
#define XML_OBJ_TIMERS		"Timers"
#define XML_ATR_NAME		"name"

//key logger
KeyLogger logger_;

/* //////////////////////////////////////////////////////////////////////////////// */
// // Util //
/* //////////////////////////////////////////////////////////////////////////////// */

void StartProcess(string file)
{
	//C: || D: || [...]
	if (file.at(1) != ':')
	{
		char cbuf[100] = { 0 };
		int i = GetModuleFileName(NULL, cbuf, 100);
		if (!i)
			return;
		string strBuf(cbuf);
		file = string(strBuf).substr(0, strBuf.find_last_of("\\/") + 1) + file;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process. 
	if (!CreateProcess(file.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

/* //////////////////////////////////////////////////////////////////////////////// */
// // Clipboard //
/* //////////////////////////////////////////////////////////////////////////////// */
void Clip_DrawContent()
{
	CLIP_SLOT clip;

	if (clip_slots_blocked)
		return;
	clip_slots_blocked = true;

	//getting data
	if (!OpenClipboard(nullptr))
		return;

	HGLOBAL clipData = GetClipboardData(CF_TEXT);
	if (clipData)
	{
		clip.Size = GlobalSize(clipData);
		clip.Data = malloc(clip.Size);
		memcpy(clip.Data, clipData, clip.Size);
		GlobalUnlock(clipData);
	} else
	{
		clip.Size = 0;
		clip.Data = nullptr;
	}
	CloseClipboard();
	if (clip.Data)
	{
		//saving data
		if (clip_slots_[CLIP_SLOT_COUNT - 1].Data)
			free(clip_slots_[CLIP_SLOT_COUNT - 1].Data);
		for (uint i = CLIP_SLOT_COUNT - 1; i > 0; i--)
			clip_slots_[i] = clip_slots_[i - 1];//memcpy_s() is unavailable because, the are in the same space
		clip_slots_[0] = clip;

		for (uint i = 0; i < CLIP_SLOT_COUNT; i++)
			if (clip_slots_[i].Data)
				cout << i << " " << (char*)clip_slots_[i].Data << endl;
	}
	clip_slots_blocked = false;
}
void Clip_PushContent(uint index)
{
	if (index > CLIP_SLOT_COUNT || clip_slots_blocked)
		return;
	CLIP_SLOT slot = clip_slots_[index];
	if (!slot.Data)
		return;
	clip_slots_blocked = true;

	{
		OpenClipboard(NULL);
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, slot.Size);
		if (!hg)
		{
			CloseClipboard();
			clip_slots_blocked = false;
			return;
		}
		memcpy(GlobalLock(hg), slot.Data, slot.Size);
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
	}

	clip_slots_blocked = false;
}
char* Clip_GetContent(uint index)
{
	if (clip_slots_blocked)
		return "NULL<Blocked>";
	if (index >= CLIP_SLOT_COUNT)
		return "NULL<Out of bounds>";
	if (!clip_slots_[index].Data)
		return "NULL";

	return (char*)clip_slots_[index].Data;

}
void Clip_OpenMenu()
{
	POINT pos;
	forground_hwnd_ = GetForegroundWindow();
	GetCursorPos(&pos);
	SetWindowPos(hwnd_, HWND_TOP, pos.x - hwnd_border_width_, pos.y - hwnd_border_height_, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	HDC hdc = GetDC(hwnd_);

	RECT rect;
	rect.left = CLIP_BUTTON_WIDTH;
	rect.right = WINDOW_WIDTH - CLIP_BUTTON_WIDTH;

	for (uint i = 0; i < CLIP_SLOT_COUNT; i++)
	{
		rect.top = CLIP_BUTTON_HEIGHT * i;
		rect.bottom = CLIP_BUTTON_HEIGHT * (i + 1);

		char* text = Clip_GetContent(i);
		DrawTextEx(hdc, text, strlen(text), &rect, DT_LEFT | DT_TOP, NULL);
	}
	ReleaseDC(hwnd_, hdc);
	UpdateWindow(hwnd_);
}
void Clip_CopyKeyPressed(bool newPress)
{
	if (newPress)
		Clip_DrawContent();
}
void Clip_MenuKeyPressed(bool newPress)
{
	if (newPress)
		Clip_OpenMenu();

}

/* //////////////////////////////////////////////////////////////////////////////// */
// // Timer //
/* //////////////////////////////////////////////////////////////////////////////// */
void Timer_UpdateMenu()
{
	uint i = 0;
	for (Timer* timer : timer_list_)
		ModifyMenu(timer_hmenu_, i++, MF_BYPOSITION, ID_TIMER_LIST + i, timer->getInfoString().c_str());
	
}
void Timer_Add(Timer* timer)
{
	timer_list_.push_back(timer);
	uint size = timer_list_.size();
	InsertMenu(timer_hmenu_, size - 1, MF_BYPOSITION | MF_STRING, ID_TIMER_LIST + size, timer->getInfoString().c_str());
}
void Timer_Save(String file)
{
	XmlDocument^ xml = gcnew XmlDocument();
	xml->AppendChild(xml->CreateXmlDeclaration("1.0", "UTF-8", nullptr));

	XmlElement^ xmlTimers = xml->CreateElement(XML_OBJ_TIMERS);
	for (Timer* timer : timer_list_)
	{
		XmlElement^ xmlTimer = xml->CreateElement(to_CS_String(GetTimerTypeName(timer->getType())));
		xmlTimer->SetAttribute(XML_ATR_NAME, to_CS_String(timer->getName()));
		xmlTimer->InnerText = to_CS_String(timer->getSaveString());

		xmlTimers->AppendChild(xmlTimer);
	}

	xml->AppendChild(xmlTimers);
	xml->Save(gcnew StreamWriter(to_CS_String(file), false, Encoding::UTF8));
}
void Timer_Load(String file) {
	
	System::String^ xmlString;
	try
	{
		xmlString = File::ReadAllText(to_CS_String(file), Encoding::UTF8);
	} catch (FileNotFoundException^ e)
	{
		xmlString = "";
	}

	if (xmlString->Length != 0)
	{
		XmlDocument^ xml = gcnew XmlDocument();
		xml->LoadXml(xmlString);

		XmlNode^ xmlChild;
		XmlElement^ xmlTimer;
		for (int ci = 0; ci < xml->ChildNodes->Count; ci++)
		{
			xmlChild = xml->ChildNodes->Item(ci);
			if (xmlChild->Name == XML_OBJ_TIMERS)
			{
				for (int ti = 0; ti < xmlChild->ChildNodes->Count; ti++)
				{
					xmlTimer = (XmlElement^)xmlChild->ChildNodes->Item(ti);
					Timer_Add( CreateTimer(
						GetTimerType(to_CPP_String(xmlTimer->Name)),
						to_CPP_String(xmlTimer->GetAttribute(XML_ATR_NAME)),
						to_CPP_String(xmlTimer->InnerText))
					);
				}
			}
		}
		cout << "Timer_Load: " << file.c_str() << " successfully" << endl;
	} else
	{
		cout << "Timer_Load: " << file.c_str() << " couldn't be loaded" << endl;
	}
}

/* //////////////////////////////////////////////////////////////////////////////// */
// // Window //
/* //////////////////////////////////////////////////////////////////////////////// */
LRESULT CALLBACK WindowProc(HWND hwnd, uint msg, WPARAM wp, LPARAM lp);

bool InitWindow()
{
	tray_creationID_ = RegisterWindowMessage("SystemTrayCreation");

	/* ********************************************************* */
	// * WNDCLASSEX *
	/* ********************************************************* */
	WNDCLASSEX wClass;
	memset(&wClass, 0, sizeof(WNDCLASSEX));

	wClass.lpszClassName = EX_CLASS_NAME;
	wClass.lpfnWndProc = WindowProc;
	wClass.style = CS_DBLCLKS;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
	if (!RegisterClassEx(&wClass))
		return false;

	/* ********************************************************* */
	// * Hwnd *
	/* ********************************************************* */
	RECT size = { 0, 0, LONG(WINDOW_WIDTH), LONG(WINDOW_HEIGHT) };
	AdjustWindowRect(&size, WINDOW_STYLE, false);
	hwnd_border_width_ = size.left;
	hwnd_border_height_ = size.top;
	hwnd_ = CreateWindowEx(0,
		EX_CLASS_NAME, EX_CLASS_NAME,
		WINDOW_STYLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		size.right - size.left,
		size.bottom - size.top,
		HWND_DESKTOP, NULL, NULL, NULL);
	if (!hwnd_)
		return false;

	ShowWindow(hwnd_, SW_HIDE);

	/* ********************************************************* */
	// * buttons *
	/* ********************************************************* */
	for (uint i = 0; i < CLIP_SLOT_COUNT; i++)
	{
		CreateWindow(
			"BUTTON",  // Predefined class; Unicode assumed 
			(string("Slot ") + to_string(i)).c_str(),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			0,         // x position 
			0 + i * CLIP_BUTTON_HEIGHT,         // y position 
			CLIP_BUTTON_WIDTH,        // Button width
			CLIP_BUTTON_HEIGHT,        // Button height
			hwnd_,     // Parent window
			(HMENU)(CLIP_BUTTON_BASE_ID + i),       // No menu.
			(HINSTANCE)GetWindowLong(hwnd_, GWL_HINSTANCE),
			NULL);
	}

	/* ********************************************************* */
	// * init tray_data *
	/* ********************************************************* */
	memset(&tray_data_, 0, sizeof(NOTIFYICONDATA));

	tray_data_.cbSize = sizeof(NOTIFYICONDATA);
	tray_data_.hWnd = hwnd_;
	tray_data_.uID = ID_TRAY_APP_ICON;
	tray_data_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	tray_data_.uCallbackMessage = WM_TRAY;
	tray_data_.hIcon = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	strncpy_s(tray_data_.szTip, tray_tooltip, sizeof(tray_data_.szTip));

	Shell_NotifyIcon(NIM_ADD, &tray_data_);

	/* ********************************************************* */
	// * timer menu *
	/* ********************************************************* */

	timer_hmenu_ = CreatePopupMenu();
	AppendMenu(timer_hmenu_, MF_SEPARATOR, 0, "");
	AppendMenu(timer_hmenu_, MF_STRING, ID_TIMER_CREATE, "Create Timer");

	/* ********************************************************* */
	// * tray menu *
	/* ********************************************************* */
	// Timer
	// ------------
	// Passwords
	// ------------
	// Options
	// Exit

	tray_hmenu_ = CreatePopupMenu();
	AppendMenu(tray_hmenu_, MF_STRING | MF_POPUP, (UINT_PTR)timer_hmenu_, "Timer");
	AppendMenu(tray_hmenu_, MF_SEPARATOR, 0, "");
	AppendMenu(tray_hmenu_, MF_STRING, ID_TRAY_MENU_PASSWORDS, "Passwords");
	AppendMenu(tray_hmenu_, MF_SEPARATOR, 0, "");
	AppendMenu(tray_hmenu_, MF_STRING, ID_TRAY_MENU_OPTIONS, "Options");
	AppendMenu(tray_hmenu_, MF_STRING, ID_TRAY_MENU_EXIT, "Exit :(");

	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, uint msg, WPARAM wp, LPARAM lp)
{
	if (msg == tray_creationID_)
		return 0;

	UINT clicked = 0;
	switch (msg)
	{
	case WM_ACTIVATE:
		break;
	case WM_CREATE:
		break;
	case WM_SYSCOMMAND:

		switch (wp & 0xFFF0)
		{
		case SC_MINIMIZE:
		case SC_CLOSE:
			ShowWindow(hwnd_, SW_HIDE);
			return 0;
		case BS_PUSHBUTTON:
			break;
		default:
			break;
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wp) >= CLIP_BUTTON_BASE_ID && LOWORD(wp) < (CLIP_BUTTON_BASE_ID + CLIP_SLOT_COUNT))
		{
			cout << (LOWORD(wp) - CLIP_BUTTON_BASE_ID) << endl;
			Clip_PushContent((LOWORD(wp) - CLIP_BUTTON_BASE_ID));
			ShowWindow(hwnd_, SW_HIDE);
			SetForegroundWindow(forground_hwnd_);
		}

		break;
	case WM_TRAY:

		if (lp == WM_RBUTTONDOWN)
		{
			// Get current mouse position.
			POINT curPoint;
			GetCursorPos(&curPoint);

			Timer_UpdateMenu();
			clicked = TrackPopupMenu(tray_hmenu_, TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTBUTTON, curPoint.x, curPoint.y, 0, hwnd, NULL);

			SendMessage(hwnd, WM_NULL, 0, 0); // send message to window to make sure the menu goes away.
			switch (clicked)
			{
			case ID_TRAY_MENU_PASSWORDS:
				StartProcess(PASSWORD_MANAGER_EXE_NAME);
				break;
			case ID_TRAY_MENU_EXIT:
				// quit the application.
				contineLoop = false;
				PostQuitMessage(0);
				break;
			default:
				cout << "> WindowProc > WM_TRAY > clicked > " << clicked << endl;
				break;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

/* //////////////////////////////////////////////////////////////////////////////// */
// // Main //
/* //////////////////////////////////////////////////////////////////////////////// */
MSG msg_;
int main()
{
	
	//
	//init
	//
	if (!InitWindow())
		return -1;
	
	logger_.addKeyCombo(KeyCombo({ VK_LCONTROL, 'C' }, Clip_CopyKeyPressed));
	logger_.addKeyCombo(KeyCombo({ VK_LCONTROL, VK_ALT, 'V' }, Clip_MenuKeyPressed));
	Timer_Load(TIMER_SAVE_FILE);

	//
	// loop
	//
	contineLoop = true;
	while (contineLoop)
	{
		WaitMessage();
		logger_.update();
		if (PeekMessage(&msg_, hwnd_, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg_);
			DispatchMessage(&msg_);
			if (msg_.message == WM_QUIT)
				break;
		}
	}
	//
	// Cleanup
	//
	Shell_NotifyIcon(NIM_DELETE, &tray_data_);
	clip_slots_blocked = true;
	for (uint i = 0; i < CLIP_SLOT_COUNT; i++)
		if (clip_slots_[i].Data)
			free(clip_slots_[i].Data);
	clip_slots_blocked = false;

	Timer_Save(TIMER_SAVE_FILE);
	
	return 0;
}