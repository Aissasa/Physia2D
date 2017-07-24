#include "pch.h"
#include "Stopwatch.h"

using namespace std;
using namespace sf;
using namespace glm;

namespace Testbed
{
	/************************************************************/
	Stopwatch::Stopwatch() : mLastFrameTime(0)
	{
		mClock.restart();
	}

	/************************************************************/
	void Stopwatch::Reset()
	{
		mClock.restart();
		mLastFrameTime = 0;
	}

	/************************************************************/
	float32_t Stopwatch::DelayInSeconds() const
	{
		return mClock.getElapsedTime().asSeconds();
	}

	/************************************************************/
	int32_t Stopwatch::DelayInMilliseconds() const
	{
		return mClock.getElapsedTime().asMilliseconds();
	}

	/************************************************************/
	float32_t Stopwatch::DelayFromLastFrameInSeconds(const bool resetLastFrameTime)
	{
		float32_t delay = mClock.getElapsedTime().asSeconds() - mLastFrameTime;
		if (resetLastFrameTime)
		{
			ResetLastFrameTime();
		}
		return delay;
	}

	/************************************************************/
	int32_t Stopwatch::DelayFromLastFrameInMilliseconds(const bool resetLastFrameTime)
	{
		int32_t delay = static_cast<int32_t>(mClock.getElapsedTime().asMilliseconds() - mLastFrameTime / 1000.0f);
		if (resetLastFrameTime)
		{
			ResetLastFrameTime();
		}
		return delay;
	}

	/************************************************************/
	void Stopwatch::ResetLastFrameTime()
	{
		mLastFrameTime = mClock.getElapsedTime().asSeconds();
	}
}