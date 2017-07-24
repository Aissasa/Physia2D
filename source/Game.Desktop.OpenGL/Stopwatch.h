#pragma once

#include <mat3x2.hpp>
#include <SFML/System/Clock.hpp>

namespace Testbed
{
	/** Stopwatch used to calculate the time taken by a certain operation.
	*/
	class Stopwatch
	{

	public:

		/** Stopwatch constructor.
		*/
		Stopwatch();

		/** Stopwatch destructor.
		*/
		~Stopwatch() = default;

		/** Stopwatch copy constructor.
		*/
		Stopwatch(const Stopwatch & rhs) = default;

		/** Stopwatch copy assignment operator.
		*/
		Stopwatch& operator=(const Stopwatch& rhs) = default;

		/** Resets the stopwatch to the current time.
		*/
		void Reset();

		/** Returns the delay between the start time and the current time in seconds.
		* @return Delay from the start time in seconds.
		*/
		glm::float32_t DelayInSeconds() const;

		/** Returns the delay between the start time and the current time in milliseconds.
		* @return Delay from the start time in milliseconds.
		*/
		glm::int32_t DelayInMilliseconds() const;

		/** Returns the delay between the start time and the last frame time in seconds.
		* @return Delay from the last frame time in seconds.
		*/
		glm::float32_t DelayFromLastFrameInSeconds(const bool resetLastFrameTime = true);

		/** Returns the delay between the start time and the last frame time in milliseconds.
		* @return Delay from the last frame time in milliseconds.
		*/
		glm::int32_t DelayFromLastFrameInMilliseconds(const bool resetLastFrameTime = true);

		/** Resets the last frame time to the current time.
		*/
		void ResetLastFrameTime();

	private:

		sf::Clock mClock;
		glm::float32_t mLastFrameTime;

	};
}