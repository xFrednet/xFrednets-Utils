#pragma once

#include "Timer.h"

namespace futils
{
	class CountdownTimer : public Timer
	{
	private:
		time_t m_TargetTime;
	public:
		CountdownTimer(String name, String saveData);
		CountdownTimer(String name, time_t targetTime);

		time_t getTime() const override;
		String getSaveString() const override;

		TIMER_TYPE getType() const override;
	};
}
