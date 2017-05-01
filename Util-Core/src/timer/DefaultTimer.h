#pragma once

#include "..\Futils.h"

#include "Timer.h"

namespace futils
{
	class DefaultTimer : public Timer
	{
		time_t m_StartTime;
	public:
		DefaultTimer(String name, String saveData);
		DefaultTimer(String name, time_t startTime = time(0));

		time_t getTime() const override;
		String getSaveString() const override;

		TIMER_TYPE getType() const override;
	};
	
}

