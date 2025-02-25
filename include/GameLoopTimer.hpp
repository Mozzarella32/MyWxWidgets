#pragma once

#include "pch.hpp"

class GameLoopTimer {
private:
	wxWindow* Parent;

	std::thread TimerThread;
	bool Running = false;

	std::mutex FinishedLoopMutex;
	std::condition_variable FinishedLoop_cv;
	bool FinishedLoop = true;

	std::deque<std::chrono::nanoseconds> frameTimes;
	double TargetUPS = 60.0;
	size_t maxFrameSamples = 60;
	std::chrono::high_resolution_clock::duration TargetFrameDuration = std::chrono::nanoseconds(int(1000000000.0 / TargetUPS));

	std::chrono::high_resolution_clock::time_point NextTime;
	std::chrono::high_resolution_clock::time_point LastTime;

	double NextSleep = 0;

	struct Info {
		double FPS;
		std::chrono::high_resolution_clock::duration Sleep;
	};

	Info info{};
public:

	using GameLoopFunktion = std::function<void(void)>;

private:

	GameLoopFunktion Loop;

	void Work();

	void OnTimer();

public:

	GameLoopTimer(wxWindow* Parent, GameLoopFunktion Loop);

	void Start();

	void Stop();

	void SetFPS(double FPS);

	void SetSamples(size_t Sampels);

	const Info& GetInfo()const;

	~GameLoopTimer();
};