#include <Windows.h>
#include "Timer.h"

namespace MyUtil
{
	Timer::Timer() : mdSecondsPerCount{ 0.0 }, mdDeltaTime{ -1.0 }, mfScale{ 1.f }, mllBaseTime{ 0 },
		mllPausedTime{ 0 }, mllStopTime{ 0 }, mllPrevTime{ 0 }, mllcurrTime{ 0 }, mbStopped{ false }
	{
		long long countsPerSec;		// 1�ʿ� ���?
		// 1 ����ũ�� �� = �鸸, 1�ʿ� ��� �ݺ��ϴ���
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		mdSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	}
	void Timer::Start()
	{
		long long currTime;
		// ��ǻ�Ͱ� ���� ���Ŀ� ��ŭ ���Ͽ�����
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
		// ������ ������Ʈ �� ������ �Ź� ȣ��Ǵ� �Լ�
		//	=> Ÿ�̸Ӱ� ���������� �ð��� ��ȭ�� ������ ��Ÿ Ÿ���� 0���� �����ϰ� ����
		//	=> ��Ÿ Ÿ���� �� ������ ������ ����
		//	=> ���� �ð��� ����
		//	=> ��Ÿ Ÿ���� ���������� ��� 0���� ���� �д�.
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
		//	��ü �ð��� ���ؿ´�.
		//	=> Pause ����
		//		=> Pause�� ���۵� �ð��� ���� ������ �ð��� ���̰� ��ü �ð�
		//	=> �Ϲ� ����
		//		=> ���ð��� ���� �ð��� ���̰� ��ü �ð�
		//	=> ���� �ִ� �ð��� ����� ���� �÷��� �ð�

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
		// ��Ÿ Ÿ���� ���ϰ� ������ ���� ���ؼ� ����

		return static_cast<float>(mdDeltaTime * mfScale);
	}
	void Timer::SetScale(float scale)
	{
		// �������� ����

		mfScale = scale;
	}
}
