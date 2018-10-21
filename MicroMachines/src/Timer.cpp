#include "Timer.h"

float Timer::deltaTime = 0;
float Timer::elapsedTime = 0;
bool Timer::isPaused = false;
std::clock_t Timer::m_CurrentTime;
std::clock_t Timer::m_DeltaTime;

Timer::Timer() {}

void Timer::Start() {
	Timer::Tick();
}

void Timer::Tick() {
	if (isPaused)
		return;

	auto temp = clock();
	m_DeltaTime = temp - m_CurrentTime;
	m_CurrentTime = temp;
	deltaTime = ((float)m_DeltaTime) / CLOCKS_PER_SEC;
	elapsedTime += deltaTime;
}

void Timer::Pause() {
	isPaused = !isPaused;
	Tick();
	m_DeltaTime = 0;
	deltaTime = 0;
}

void Timer::Unpause() {
	isPaused = false;
}

