#include "Timer.h"

using namespace std;

namespace futils
{
	/* //////////////////////////////////////////////////////////////////////////////// */
	// // Class //
	/* //////////////////////////////////////////////////////////////////////////////// */
	Timer::Timer(String name, time_t startTime)
		: m_Name(name),
		m_StartTime(startTime)
	{
	}
	Timer::Timer(String name, String saveData)
		: m_Name(name),
		m_StartTime((time_t)atoll(saveData.c_str()))
	{
	}

	time_t Timer::getTime() const
	{
		return time(0) - m_StartTime;
	}
	String Timer::getSaveString() const
	{
		return std::to_string(m_StartTime);
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
}
