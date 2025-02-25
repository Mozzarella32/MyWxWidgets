#include "pch.hpp"
#include "GameLoopTimer.hpp"

#include "../submodules/Instrumentor/Instrumentor.hpp"

void GameLoopTimer::Work() {
	PROFILE_FUNKTION;

	FinishedLoop = true;
	LastTime = std::chrono::high_resolution_clock::now();
	NextTime = std::chrono::high_resolution_clock::now();
	while (Running) {
		OnTimer();
	}
}

void GameLoopTimer::OnTimer() {
	PROFILE_FUNKTION;

	using namespace std::chrono;

	FinishedLoop = false;
	Parent->CallAfter([this, LoopCopy = Loop]() {
		LoopCopy();
		FinishedLoop = true;
		FinishedLoop_cv.notify_all();
		});

	std::unique_lock ul_cv(FinishedLoopMutex);
	FinishedLoop_cv.wait(ul_cv, [this]() {
		return FinishedLoop | !Running;
		});

	if (!Running)return;

	auto CurrentTime = high_resolution_clock::now();
	auto Elapsed = CurrentTime - LastTime;

	if (NextTime - CurrentTime < -10s) {
		//NextTime = CurrentTime - 10s;
		//Elapsed = TargetFrameDuration;
		//frameTimes.clear();
	}

	frameTimes.push_back(Elapsed);
	while (frameTimes.size() > maxFrameSamples)
	{
		frameTimes.pop_front();
	}

	LastTime = CurrentTime;

	high_resolution_clock::duration AverageFrameTime{};
	for (auto frameTime : frameTimes)
	{
		AverageFrameTime += frameTime;
	}
	AverageFrameTime /= frameTimes.size();

	NextTime += TargetFrameDuration;

	info.FPS = ((double)high_resolution_clock::duration(1000000000).count() / (double)AverageFrameTime.count());
	info.Sleep = NextTime - CurrentTime;

	std::this_thread::sleep_until(NextTime);
}

GameLoopTimer::GameLoopTimer(wxWindow* Parent, GameLoopFunktion Loop)
	: Parent(Parent), Loop(Loop) {}

void GameLoopTimer::Start() {
	PROFILE_FUNKTION;

	if (Running)return;
	/*	Timer.StartOnce(0);*/
	TimerThread = std::thread([this]() {Work(); });
	Running = true;
}

void GameLoopTimer::Stop() {
	PROFILE_FUNKTION;

	if (!Running)return;
	Running = false;
	FinishedLoop_cv.notify_all();
	if (TimerThread.joinable()) {
		TimerThread.join();
	}
}

void GameLoopTimer::SetFPS(double FPS) {
	TargetUPS = FPS;
	TargetFrameDuration = std::chrono::nanoseconds(int(1000000000.0 / TargetUPS));
}

void GameLoopTimer::SetSamples(size_t Sampels) {
	maxFrameSamples = Sampels;
}

const GameLoopTimer::Info& GameLoopTimer::GetInfo() const {
	return info;
}

GameLoopTimer::~GameLoopTimer() {
	Stop();
}
