#pragma once

#include <ctime>

struct Timer {

	Timer();
	
	static void Start();
	static float deltaTime;
	static float elapsedTime;

	static void Tick();
	static void TooglePause();
	static void Unpause();
	static void Pause();

private:
	static std::clock_t m_CurrentTime;
	static std::clock_t m_DeltaTime;
	static bool isPaused;
	
};

