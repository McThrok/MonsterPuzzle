#pragma once
#include <chrono>

namespace fe
{
	class Timer
	{
	public:
		Timer();

		double GetMilisecondsElapsed();
		bool Start();
		bool Stop();
		void Restart();

	private:
		bool isrunning = false;
#ifdef _WIN32
		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> stop;
#else
		std::chrono::time_point<std::chrono::system_clock> start;
		std::chrono::time_point<std::chrono::system_clock> stop;
#endif
	};
}