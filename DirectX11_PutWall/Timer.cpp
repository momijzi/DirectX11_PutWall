#include "App.hpp"

App::Timer::Timer()
{
				App::Initialize();

				time = 0.0f;
				deltaTime = 0.0f;
				frameRate = 0;
				second = 0.0f;
				frameCount = 0;
				preCount = GetCounter();
				frequency = GetCountFrequency();
}

void App::Timer::Update()
{
				LARGE_INTEGER count = GetCounter();
				deltaTime = (static_cast<float>(count.QuadPart - preCount.QuadPart) / frequency.QuadPart);
				preCount = GetCounter();

				time += deltaTime;

				frameCount++;
				second += deltaTime;
				if (second >= 1.0f)
				{
								frameRate = frameCount;
								frameCount = 0;
								second -= 1.0f;
				}
}

LARGE_INTEGER App::Timer::GetCounter()const
{
				LARGE_INTEGER counter;
				QueryPerformanceCounter(&counter);
				return counter;
}

LARGE_INTEGER App::Timer::GetCountFrequency()const
{
				LARGE_INTEGER frequency;
				QueryPerformanceFrequency(&frequency);
				return frequency;
}