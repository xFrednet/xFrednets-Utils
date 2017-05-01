#include "CountdownTimer.h"

using namespace std;
namespace futils
{
	CountdownTimer::CountdownTimer(String name, String saveData)
		: CountdownTimer(name, (time_t)atoll(saveData.c_str()))
	{
		cout << saveData.c_str() << " " << atoll(saveData.c_str()) << endl;
	}
	CountdownTimer::CountdownTimer(String name, time_t targetTime)
		: Timer(name),
		m_TargetTime(targetTime)
	{
	}

	time_t CountdownTimer::getTime() const
	{
		return m_TargetTime - time(0);
	}
	String CountdownTimer::getSaveString() const
	{
		return std::to_string(m_TargetTime);
	}

	TIMER_TYPE CountdownTimer::getType() const
	{
		return TIMER_TYPE_COUNTDOWN;
	}
}

