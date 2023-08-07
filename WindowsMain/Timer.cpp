#include <Windows.h>
#include "Timer.h"

namespace MyUtil
{
	Timer::Timer() : mdSecondsPerCount{ 0.0 }, mdDeltaTime{ -1.0 }, mfScale{ 1.f }, mllBaseTime{ 0 },
		mllPausedTime{ 0 }, mllStopTime{ 0 }, mllPrevTime{ 0 }, mllcurrTime{ 0 }, mbStopped{ false }
	{
		long long countsPerSec;		// 1초에 몇번?
		// 1 마이크로 초 = 백만, 1초에 몇번 반복하는지
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		mdSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	}
	void Timer::Start()
	{
		long long currTime;
		// 컴퓨터가 켜진 이후에 얼만큼 일하였는지
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllBaseTime = currTime;
		mllPrevTime = mllBaseTime;
		mllStopTime = 0;
		mbStopped = false;
	}
	void Timer::Stop()
	{
		if (!mbStopped)
		{
			long long currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			mllStopTime = currTime;
			mbStopped = true;
		}
	}
	void Timer::Resume()
	{
		long long startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		if (mbStopped)
		{
			mllPausedTime += (startTime - mllStopTime);

			mllPrevTime = startTime;
			mllStopTime = 0;
			mbStopped = false;
		}
	}
	void Timer::Update()
	{
		// 게임이 업데이트 될 때마다 매번 호출되는 함수
		//	=> 타이머가 멈춰있으면 시간의 변화가 없으니 델타 타임을 0으로 지정하고 종료
		//	=> 델타 타임을 초 단위로 변경해 저장
		//	=> 이전 시간을 저장
		//	=> 델타 타임이 비정상적일 경우 0으로 맞춰 둔다.
		if (mbStopped)
		{
			mdDeltaTime = 0.0f;
			return;
		}

		long long currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllcurrTime = currTime;
		mdDeltaTime = (mllcurrTime - mllPrevTime) * mdSecondsPerCount;

		mllPrevTime = mllcurrTime;

		if (mdDeltaTime < 0.0)
		{
			mdDeltaTime = 0.0;
		}
	}
	float Timer::TotalTime() const
	{
		//	전체 시간을 구해온다.
		//	=> Pause 상태
		//		=> Pause가 시작된 시간과 앱이 구동된 시간의 차이가 전체 시간
		//	=> 일반 상태
		//		=> 현시간과 시작 시간의 차이가 전체 시간
		//	=> 멈춰 있던 시간을 빼줘야 게임 플레이 시간

		if (mbStopped)
		{
			return static_cast<float>(
				((mllStopTime - mllPausedTime) - mllBaseTime) * mdSecondsPerCount
				);
		}
		else
		{
			return static_cast<float>(
				((mllcurrTime - mllPausedTime) - mllBaseTime) * mdSecondsPerCount
				);
		}
	}
	float Timer::DeltaTime() const
	{
		// 델타 타임을 구하고 스케일 값을 곱해서 보정

		return static_cast<float>(mdDeltaTime * mfScale);
	}
	void Timer::SetScale(float scale)
	{
		// 스케일을 지정

		mfScale = scale;
	}
}
