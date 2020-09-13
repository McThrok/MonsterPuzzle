#include "OpacityAnimation.h"

OpacityAnimation::OpacityAnimation(fe::ColorComponent* colorComponent, float time, float opacityFrom, float OpacityTo, float delay)
{
	this->totalTime = time;
	this->from = opacityFrom;
	this->to = OpacityTo;
	this->colorComponent = colorComponent;
	this->delay = delay;

	Reset();
}

void OpacityAnimation::Run()
{
	if (isRunning == true)
		return;

	isFinished = false;
	isRunning = true;
	currentTime = -delay;
}

bool OpacityAnimation::IsFinished()
{
	return isFinished;
}

void OpacityAnimation::Reset()
{
	isRunning = false;
	isFinished = false;
}

void OpacityAnimation::Update()
{
	if (!isRunning)
		return;

	currentTime += fe::Time::deltaTime;
	currentTime = min(totalTime, currentTime);
	if (currentTime == totalTime)
	{
		isFinished = true;
		isRunning = false;
	}

	if (currentTime >= 0)
		colorComponent->Color.w = from + (to - from) * currentTime / totalTime;
}
