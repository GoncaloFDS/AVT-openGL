#pragma once

#include <ctime>

struct Timer {

	Timer();
	
	static float deltaTime;
	static float elapsedTime;

	static void Tick();
	static void Pause();
	static void Unpause();

private:
	static std::clock_t m_CurrentTime;
	static std::clock_t m_DeltaTime;
	static bool isPaused;
	
};

