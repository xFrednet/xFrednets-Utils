#pragma once

#include "..\Futils.h"

namespace futils
{
	class Timer;
	
	typedef enum TIMER_TYPE_
	{
		TIMER_TYPE_DEFAULT   = 0,
		TIMER_TYPE_COUNTDOWN = 1
	} TIMER_TYPE;

	String TimeToString(const time_t& time);

	String GetTimerTypeName(TIMER_TYPE type);
	TIMER_TYPE GetTimerType(String typeName);
	Timer* CreateTimer(TIMER_TYPE type, String name, String saveData);

	class Timer
	{
		String m_Name;
	protected:
		Timer(String name);
	public:
		virtual time_t getTime() const = 0;
		virtual String getSaveString() const = 0;
		
		String getName() const;
		virtual String getInfoString() const;

		virtual TIMER_TYPE getType() const = 0;
	};

}
