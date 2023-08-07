#pragma once

namespace MyUtil
{
	// #. Timer 클래스 : 시간을 계산해서 델타 타임 이전에 처리했던 작업과 현재 시간 사이의 델타 타임을 구한다.
	//		=> 유저에게 플레이 타임 알림
	class Timer
	{
	public:
		Timer();

	public:
		void		Start();
		void		Stop();
		void		Resume();
		void		Update();	// 시간 측정을 위해서 GameLoop에서 계속 불러주어야 한다.
		float		TotalTime() const;	// 시간을 받는 Getter
		float		DeltaTime() const;	// 델타 시간을 받는 Getter
		void		SetScale(float scale);	// 시간의 슬로우모션 || 패스트모션 처리용 함수

	private:
		// 마이크로 세컨드 단위로 큰 값이 들어가기 때문에 8바이트 변수 사용
		double		mdSecondsPerCount;
		double		mdDeltaTime;
		float		mfScale;

		long long	mllBaseTime;	// 기준 시간으로 게임이 처음 구동 되었을 때의 시간
		long long	mllPausedTime;	// 멈춰 있었던 시간을 저장
		long long	mllStopTime;	// 멈춘 시간을 저장
		long long	mllPrevTime;	// 이전 시간
		long long	mllcurrTime;	// 현재 시간

		bool		mbStopped;
	};
}
