#pragma once
#include "Futils.h"

//clipboard
#define CLIP_DEFAULT_SLOT_COUNT		10
#define CLIP_INFO_WIDTH				500
#define CLIP_BUTTON_WIDTH			100
#define CLIP_BUTTON_HEIGHT			20
#define CLIP_MIN_ID					3000
#define CLIP_MAX_ID					3999
#define CLIP_BUTTON_BASE_ID			CLIP_MIN_ID


namespace futils
{
	typedef struct CLIP_SLOT_ {
		uint Size;
		void* Data;
	} CLIP_SLOT;

	HWND							clip_menu_hwnd_;
	HWND							clip_last_focus_hwnd_;
	uint							clip_menu_border_width_;
	uint							clip_menu_border_height_;
	bool							clip_slots_blocked_;
	CLIP_SLOT*						clip_slots_;
	uint							clip_slot_count_;

	void Clip_Init(HWND hwnd, uint slotCount = CLIP_DEFAULT_SLOT_COUNT);
	void Clip_Terminate();

	//content
	void Clip_DrawContent();
	void Clip_PushContent(uint index);
	char* Clip_GetContent(uint index);

	void Clip_OpenMenu();
	void Clip_MenuCallback(uint id);
}

