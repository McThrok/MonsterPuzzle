#include "pch.h"
#include "Timer.h"

namespace fe
{
	Timer::Timer()
	{
		start = std::chrono::high_resolution_clock::now();
		stop = std::chrono::high_resolution_clock::now();
	}

	double Timer::GetMilisecondsElapsed()
	{
		if (isrunning)
		{
			auto elapsed = std::chrono::duration<double, std::milli>(
				std::chrono::high_resolution_clock::now() - start);
			return elapsed.count();
		}
		else
		{
			auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
			return elapsed.count();
		}
	}

	bool Timer::Start()
	{
		if (isrunning)
		{
			return false;
		}
		else
		{
			start = std::chrono::high_resolution_clock::now();
			isrunning = true;
			return true;
		}
	}

	bool Timer::Stop()
	{
		if (!isrunning)
			return false;
		else
		{
			stop = std::chrono::high_resolution_clock::now();
			isrunning = false;
			return true;
		}
	}

	void Timer::Restart()
	{
		isrunning = true;
		start = std::chrono::high_resolution_clock::now();
	}
}
