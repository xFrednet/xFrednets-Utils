#include "Futils.h"
#include "timer\Timer.h"
#include "timer\TimerCreator.h"
#include "Options.h"
#include "ExtraClip.h"


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
bool options_continue_loop_;
String options_save_directory_;

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
#define ID_TIMER_MIN_ID				2000
#define ID_TIMER_MAX_ID				2999
#define ID_TIMER_CREATE				2000
#define ID_TIMER_REMOVE				2001
#define ID_TIMER_LIST				2002
#define TIMER_SAVE_FILE				"Timers.xml"
list<Timer>			timer_list_;
HMENU				timer_hmenu_;
bool				timer_remove_mode_;

//window
#define WINDOW_WIDTH				500
#define WINDOW_HEIGHT				300
#define WINDOW_STYLE				(WS_POPUP | WS_THICKFRAME)
HWND hwnd_;

//XML
#define XML_OBJ_TIMERS		"Timers"
#define XML_ATR_NAME		"name"

//Objects
Options options_;
KeyLogger* logger_;

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
// // Timer //
/* //////////////////////////////////////////////////////////////////////////////// */
void Timer_Save(String file = TIMER_SAVE_FILE);
void Timer_Add(Timer timer)
{
	timer_list_.push_back(timer);
	uint size = timer_list_.size();
	InsertMenu(timer_hmenu_, size - 1, MF_BYPOSITION | MF_STRING, ID_TIMER_LIST + size, timer.getInfoString().c_str());
}
void Timer_Create()
{
	Timer t = TimerCreator::CreateTimer();
	if (t.getName().length() != 0)
	{
		Timer_Add(t);
		Timer_Save();
	}
}
void Timer_Save(String file)
{
	XmlDocument^ xml = gcnew XmlDocument();
	xml->AppendChild(xml->CreateXmlDeclaration("1.0", "UTF-8", nullptr));

	XmlElement^ xmlTimers = xml->CreateElement(XML_OBJ_TIMERS);
	for (Timer timer : timer_list_)
	{
		XmlElement^ xmlTimer = xml->CreateElement("Timer");
		xmlTimer->SetAttribute(XML_ATR_NAME, to_CS_String(timer.getName()));
		xmlTimer->InnerText = to_CS_String(timer.getSaveString());

		xmlTimers->AppendChild(xmlTimer);
	}

	xml->AppendChild(xmlTimers);
	try
	{
		Directory::CreateDirectory(Path::GetDirectoryName(to_CS_String(options_save_directory_ + file)));
		xml->Save(gcnew StreamWriter(to_CS_String(options_save_directory_ + file), false, Encoding::UTF8));
	} catch (System::Exception^ e)
	{
		System::Windows::Forms::MessageBox::Show("The Timers couldn't be saved do to some Error... Sorry");
	}
}
void Timer_Load(String file = TIMER_SAVE_FILE)
{
	System::String^ xmlString;
	try
	{
		xmlString = File::ReadAllText(to_CS_String(options_save_directory_ + file), Encoding::UTF8);
	} catch (System::Exception^ e) {
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
					Timer_Add( Timer(
						to_CPP_String(xmlTimer->GetAttribute(XML_ATR_NAME)),
						to_CPP_String(xmlTimer->InnerText))
					);
				}
			}
		}
		cout << "Timer_Load: Loaded " << file.c_str() << " successfully" << endl;
	} else
	{
		cout << "Timer_Load: " << file.c_str() << " couldn't be loaded" << endl;
		Timer_Save();
	}
}
void Timer_UpdateMenu()
{
	uint i = 0;
	if (timer_remove_mode_)
	{
		ModifyMenu(timer_hmenu_, ID_TIMER_REMOVE, MF_BYCOMMAND, ID_TIMER_REMOVE, "Remove Timer");
		timer_remove_mode_ = false;
	}
	for (Timer timer : timer_list_)
		ModifyMenu(timer_hmenu_, i++, MF_BYPOSITION | MF_GRAYED, ID_TIMER_LIST + i, timer.getInfoString().c_str());
}
bool Timer_MenuClicked(int clickedItemID)
{
	uint i = 0;
	switch(clickedItemID)
	{
	case ID_TIMER_CREATE:
		Timer_Create();
		break;
	case ID_TIMER_REMOVE:
		if (timer_remove_mode_)
		{
			ModifyMenu(timer_hmenu_, ID_TIMER_REMOVE, MF_BYCOMMAND, ID_TIMER_REMOVE, "Confirm");
			for (Timer timer : timer_list_)
				ModifyMenu(timer_hmenu_, i++, MF_BYPOSITION | MF_GRAYED, ID_TIMER_LIST + i, timer.getInfoString().c_str());
			timer_remove_mode_ = false;
		} else
		{
			ModifyMenu(timer_hmenu_, ID_TIMER_REMOVE, MF_BYCOMMAND, ID_TIMER_REMOVE, "Remove Timer");
			for (Timer timer : timer_list_)
				ModifyMenu(timer_hmenu_, i++, MF_BYPOSITION, ID_TIMER_LIST + i, timer.getInfoString().c_str());
			timer_remove_mode_ = true;
		}
		return true;
	default:
		if (timer_remove_mode_ && clickedItemID >= ID_TIMER_LIST)
		{
			for (Timer timer : timer_list_)
				if (ID_TIMER_LIST + i == clickedItemID)
					ModifyMenu(timer_hmenu_, i++, MF_BYPOSITION | MF_CHECKED, ID_TIMER_LIST + i, timer.getInfoString().c_str());
				else
					ModifyMenu(timer_hmenu_, i++, MF_BYPOSITION | MF_UNCHECKED, ID_TIMER_LIST + i, timer.getInfoString().c_str());
		}
	}

	return false;
}
void Timer_Init()
{
	if (!options_.areTimersEnabled())
		return;
	
	timer_hmenu_ = CreatePopupMenu();
	AppendMenu(timer_hmenu_, MF_SEPARATOR, 0, "");
	AppendMenu(timer_hmenu_, MF_STRING, ID_TIMER_CREATE, "Create Timer");
	AppendMenu(timer_hmenu_, MF_STRING, ID_TIMER_REMOVE, "Remove");

	Timer_Load();
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
	// * tray menu *
	/* ********************************************************* */
	// Timer
	// ------------
	// Passwords
	// ------------
	// Options
	// Exit

	tray_hmenu_ = CreatePopupMenu();
	if (timer_hmenu_)
	{
		AppendMenu(tray_hmenu_, MF_STRING | MF_POPUP, (UINT_PTR)timer_hmenu_, "Timer");
		AppendMenu(tray_hmenu_, MF_SEPARATOR, 0, "");
	}
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
	case WM_KILLFOCUS:
		if (!IsChild(hwnd_, (HWND)wp))
			ShowWindow(hwnd_, SW_HIDE);
		
		break;
	case WM_SYSCOMMAND:

		switch (wp & 0xFFF0)
		{
		case SC_MINIMIZE:
		case SC_CLOSE:
			ShowWindow(hwnd_, SW_HIDE);
			return 0;
		default:
			break;
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wp) >= CLIP_MIN_ID && LOWORD(wp) <= CLIP_MAX_ID)
			Clip_MenuCallback(LOWORD(wp));

		break;
	case WM_TRAY:

		if (lp == WM_RBUTTONDOWN)
		{
			// Get current mouse position.
			POINT curPoint;
			GetCursorPos(&curPoint);

			Timer_UpdateMenu();
			clicked = TrackPopupMenu(tray_hmenu_, TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTBUTTON, curPoint.x, curPoint.y, 0, hwnd, NULL);
			if (clicked >= ID_TIMER_MIN_ID && clicked <= ID_TIMER_MAX_ID)
				if (Timer_MenuClicked(clicked))
					break;

			SendMessage(hwnd, WM_NULL, 0, 0); // send message to window to make sure the menu goes away.
			switch (clicked)
			{
			case ID_TRAY_MENU_PASSWORDS:
				StartProcess(PASSWORD_MANAGER_EXE_NAME);
				break;
			case ID_TRAY_MENU_EXIT:
				// quit the application.
				options_continue_loop_ = false;
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

	if (options_.areTimersEnabled())
		Timer_Init();
	if (options_.isKeyloggerEnabled())
		logger_ = new KeyLogger();
	//
	//init
	//
	if (!InitWindow())
		return -1;
	
	if (options_.isExtraetClipEnabled())
	{
		Clip_Init(hwnd_);
		Clip_Load(options_.getSaveDir());
		if (logger_) {
			logger_->addKeyCombo(KeyCombo({ VK_LCONTROL, 'C' }, Clip_CopyKeyPressed));
			logger_->addKeyCombo(KeyCombo({ VK_LCONTROL, VK_ALT, 'V' }, Clip_MenuKeyPressed));
		}
	}

	//
	// loop
	//
	options_continue_loop_ = true;
	while (options_continue_loop_)
	{
		WaitMessage();
		if (logger_)
			logger_->update();
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
	
	// Clip
	Clip_Save(options_.getSaveDir());
	Clip_Terminate();
	
	//window
	Shell_NotifyIcon(NIM_DELETE, &tray_data_);
	DestroyMenu(timer_hmenu_);
	DestroyMenu(tray_hmenu_);
	DestroyWindow(hwnd_);

	//logger
	if (logger_)
		delete logger_;

	return 0;
}
