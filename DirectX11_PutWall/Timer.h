#pragma once

class Timer
{
public:
	Timer();
	~Timer() {};
	float GetTime()const { return time; }
	float GetDeltaTime()const { return deltaTime; }
	int GetFrameRate()const { return frameRate; }
	void Update();
private:
	float time;
	float deltaTime;
	int frameRate;
	float second;
	int frameCount;
	LARGE_INTEGER preCount;
	LARGE_INTEGER frequency;
	
	LARGE_INTEGER GetCounter()const;
	LARGE_INTEGER GetCountFrequency()const;
};