#pragma once
#include <Graphics/Components/ColorComponent.h>
#include <Time/Time.h>

class OpacityAnimation
{
public:
	OpacityAnimation(fe::ColorComponent* colorComponent, float time, float opacityFrom, float OpacityTo, float delay = 0);

	void Run();
	bool IsFinished();


	void Reset();
	void Update();

private:
	fe::ColorComponent* colorComponent;
	float totalTime;
	float currentTime;
	float delay;

	bool isRunning;
	bool isFinished;

	float from;
	float to;
};

