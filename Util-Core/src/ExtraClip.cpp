#include "ExtraClip.h"

using namespace std;

namespace futils
{
	HWND							clip_menu_hwnd_			= nullptr;
	HWND							clip_last_focus_hwnd_	= nullptr;
	int								clip_menu_border_width_;
	int								clip_menu_border_height_;
	bool							clip_slots_blocked_		= true;
	CLIP_SLOT*						clip_slots_				= nullptr;
	uint							clip_slot_count_		= 0;

	String String_Replace(String string, String oldStr, String newStr)
	{
		if (string.empty() || oldStr.empty())
			return string;

		size_t offset = 0;
		size_t pos;

		while ((pos = string.find(oldStr, offset)) != string.npos) {
			string.replace(pos, oldStr.size(), newStr);
			offset = pos + newStr.size();
		}

		return string;
	}
	inline RECT Clip_GetMenuSize()
	{
		if (clip_slot_count_)
			return {0, 0, CLIP_BUTTON_WIDTH + CLIP_INFO_WIDTH, (LONG)(CLIP_BUTTON_HEIGHT * clip_slot_count_)};
		
		cout << "Error Clip_GetMenuSize was called with clip_slot_count_ == 0"  << endl;
		return {0, 0, 0, 0};
	}

	void Clip_Init(HWND hwnd, uint slotCount)
	{
		if (clip_slots_)
			return;
		
		//slots
		clip_slot_count_ = slotCount;
		clip_slots_ = (CLIP_SLOT*)malloc(sizeof(CLIP_SLOT) * clip_slot_count_);
		memset(clip_slots_, 0, sizeof(CLIP_SLOT) * clip_slot_count_);

		//hwnd
		{
			clip_menu_hwnd_ = hwnd;
			
			DWORD hwndStyle = (DWORD)GetWindowLong(hwnd, GWL_STYLE);
			RECT size = Clip_GetMenuSize();
			AdjustWindowRect(&size, hwndStyle, false);

			clip_menu_border_width_ = size.left;
			clip_menu_border_height_ = size.top;
			SetWindowPos(hwnd, 0, 0, 0, size.right - size.left, size.bottom - size.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			for (uint i = 0; i < clip_slot_count_; i++)
			{
				CreateWindow(
					"BUTTON",									// Predefined class; Unicode assumed 
					(string("Slot ") + to_string(i)).c_str(),	// Button text 
					WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,	// Styles 
					0,											// x position 
					0 + i * CLIP_BUTTON_HEIGHT,					// y position 
					CLIP_BUTTON_WIDTH,							// Button width
					CLIP_BUTTON_HEIGHT,							// Button height
					hwnd,										// Parent window
					(HMENU)(CLIP_BUTTON_BASE_ID + i),			// ID
					(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					NULL);
			}
		}

		//blocked
		clip_slots_blocked_ = false;
	}
	void Clip_Terminate()
	{
		if (!clip_slots_)
			return;

		clip_slots_blocked_ = true;

		//free
		for (uint i = 0; i < clip_slot_count_; i++)
			if (clip_slots_[i].Data)
				free(clip_slots_[i].Data);
		
		free(clip_slots_);

		clip_slot_count_ = 0;
	}

	void Clip_DrawContent()
	{
		CLIP_SLOT clip;

		if (clip_slots_blocked_)
			return;
		clip_slots_blocked_ = true;

		clip.Size = 0;
		clip.Data = nullptr;

		//getting data
		if (OpenClipboard(nullptr))
		{
			HGLOBAL clipData = GetClipboardData(CF_TEXT);
			if (clipData) {
				clip.Size = GlobalSize(clipData);
				clip.Data = malloc(clip.Size);
				memcpy(clip.Data, clipData, clip.Size);
				GlobalUnlock(clipData);
			}
			CloseClipboard();
		}

		if (clip.Data && (clip.Size != clip_slots_[0].Size || memcmp(clip.Data, clip_slots_[0].Data, clip.Size))) {
			//saving data
			if (clip_slots_[clip_slot_count_ - 1].Data)
				free(clip_slots_[clip_slot_count_ - 1].Data);
			for (uint i = clip_slot_count_ - 1; i > 0; i--)
				clip_slots_[i] = clip_slots_[i - 1];//memcpy_s() is unavailable because, the are in the same space
			clip_slots_[0] = clip;
		}
		clip_slots_blocked_ = false;
	}
	void Clip_PushContent(uint index)
	{
		if (index >= clip_slot_count_ || clip_slots_blocked_)
			return;
		CLIP_SLOT slot = clip_slots_[index];
		if (!slot.Data)
			return;
		clip_slots_blocked_ = true;

		if (OpenClipboard(nullptr))
		{
			EmptyClipboard();
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, slot.Size);
			if (!hg) {
				CloseClipboard();
				clip_slots_blocked_ = false;
				return;
			}
			memcpy(GlobalLock(hg), slot.Data, slot.Size);
			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
			CloseClipboard();
			GlobalFree(hg);
		}

		clip_slots_blocked_ = false;
	}
	char* Clip_GetContent(uint index)
	{
		if (clip_slots_blocked_)
			return "NULL<Blocked>";
		if (index >= clip_slot_count_)
			return "NULL<Out of bounds>";
		if (clip_slots_[index].Data)
			return (char*)clip_slots_[index].Data;

		return "NULL";
	}

	void Clip_OpenMenu()
	{
		clip_last_focus_hwnd_ = GetForegroundWindow();
		
		//position
		POINT pos;
		GetCursorPos(&pos);
		SetWindowPos(clip_menu_hwnd_, HWND_TOP,
			pos.x - clip_menu_border_width_, pos.y - clip_menu_border_height_, 0, 0,
			SWP_NOSIZE | SWP_SHOWWINDOW);

		//draw Text
		String text;
		RECT hdcArea = Clip_GetMenuSize();
		hdcArea.left = CLIP_BUTTON_WIDTH;

		HDC hdc = GetDC(clip_menu_hwnd_);

		for (uint i = 0; i < clip_slot_count_; i++)
		{
			hdcArea.top = CLIP_BUTTON_HEIGHT * i;
			hdcArea.bottom = (CLIP_BUTTON_HEIGHT + 1) * i;

			//test
			text = String_Replace(Clip_GetContent(i), "\r\n", "[\\n]");
			text = String_Replace(text, "\n", "[\\n]");
			text = String_Replace(text, "\r", "[\\n]");
			text = String_Replace(text, "\t", "[\\t]");

			DrawTextEx(hdc, (char*)text.c_str(), text.length(), &hdcArea, DT_LEFT | DT_TOP, NULL);
		}
		
		ReleaseDC(clip_menu_hwnd_, hdc);
		UpdateWindow(clip_menu_hwnd_);
		SetForegroundWindow(clip_menu_hwnd_);
	}
	void Clip_MenuCallback(uint id)
	{
		if (id <= CLIP_MIN_ID && id <= CLIP_MAX_ID)
			return;

		uint index = id - CLIP_BUTTON_BASE_ID;
		Clip_PushContent(index);
		ShowWindow(clip_menu_hwnd_, SW_HIDE);
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
}
