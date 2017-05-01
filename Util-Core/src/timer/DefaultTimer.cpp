#include "DefaultTimer.h"

namespace futils
{
	DefaultTimer::DefaultTimer(String name, String saveData)
		: DefaultTimer(name, (time_t)atoll(saveData.c_str()))
	{
	}
	DefaultTimer::DefaultTimer(String name, time_t startTime)
		: Timer(name),
		m_StartTime(startTime)
	{
		
	}

	time_t DefaultTimer::getTime() const
	{
		return time(0) - m_StartTime;
	}
	String DefaultTimer::getSaveString() const
	{
		return std::to_string(m_StartTime);
	}

	TIMER_TYPE DefaultTimer::getType() const
	{
		return TIMER_TYPE_DEFAULT;
	}
}
