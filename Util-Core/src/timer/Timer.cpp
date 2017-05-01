#include "Timer.h"

#include "DefaultTimer.h"
#include "CountdownTimer.h"

using namespace std;

namespace futils
{
	/* //////////////////////////////////////////////////////////////////////////////// */
	// // Class //
	/* //////////////////////////////////////////////////////////////////////////////// */
	Timer::Timer(String name)
		: m_Name(name)
	{
	}

	String Timer::getName() const
	{
		return m_Name;
	}
	String Timer::getInfoString() const
	{
		return getName() + ": " + TimeToString(getTime());
	}

	/* //////////////////////////////////////////////////////////////////////////////// */
	// // Util //
	/* //////////////////////////////////////////////////////////////////////////////// */
	String TimeToString(const time_t& seconds)
	{
		//the time is saved in seconds
		long minutes = (long)(abs(seconds) / 60);
		long hours = minutes / 60;
		return
			String((seconds >= 0) ? "" : "-") + 
			String(((hours / 24) != 0) ? (to_string(hours / 24) + "d ") : "") +
			String(((hours) != 0     ) ? (to_string(hours % 24) + "h ") : "") +
			to_string(minutes % 60) + "min " +
			to_string(abs(seconds) % 60) + "sec";
	}
	String GetTimerTypeName(TIMER_TYPE type)
	{
		switch (type)
		{
		case TIMER_TYPE_DEFAULT:
			return "Timer";
		case TIMER_TYPE_COUNTDOWN:
			return "CountdownTimer";
		default:
			return "Timer";
		}
	}
	TIMER_TYPE GetTimerType(String typeName)
	{
		if (typeName == "Timer")
			return TIMER_TYPE_DEFAULT;
		if (typeName == "CountdownTimer")
			return TIMER_TYPE_COUNTDOWN;
		return TIMER_TYPE_DEFAULT;
	}
	Timer* CreateTimer(TIMER_TYPE type, String name, String saveData)
	{
		switch (type)
		{
		case TIMER_TYPE_DEFAULT:
			return new DefaultTimer(name, saveData);
		case TIMER_TYPE_COUNTDOWN:
			return new CountdownTimer(name, saveData);
		default:
			return new DefaultTimer(name, saveData);
		}
	}
}
