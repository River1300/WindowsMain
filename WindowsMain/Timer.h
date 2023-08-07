#pragma once

namespace MyUtil
{
	// #. Timer Ŭ���� : �ð��� ����ؼ� ��Ÿ Ÿ�� ������ ó���ߴ� �۾��� ���� �ð� ������ ��Ÿ Ÿ���� ���Ѵ�.
	//		=> �������� �÷��� Ÿ�� �˸�
	class Timer
	{
	public:
		Timer();

	public:
		void		Start();
		void		Stop();
		void		Resume();
		void		Update();	// �ð� ������ ���ؼ� GameLoop���� ��� �ҷ��־�� �Ѵ�.
		float		TotalTime() const;	// �ð��� �޴� Getter
		float		DeltaTime() const;	// ��Ÿ �ð��� �޴� Getter
		void		SetScale(float scale);	// �ð��� ���ο��� || �н�Ʈ��� ó���� �Լ�

	private:
		// ����ũ�� ������ ������ ū ���� ���� ������ 8����Ʈ ���� ���
		double		mdSecondsPerCount;
		double		mdDeltaTime;
		float		mfScale;

		long long	mllBaseTime;	// ���� �ð����� ������ ó�� ���� �Ǿ��� ���� �ð�
		long long	mllPausedTime;	// ���� �־��� �ð��� ����
		long long	mllStopTime;	// ���� �ð��� ����
		long long	mllPrevTime;	// ���� �ð�
		long long	mllcurrTime;	// ���� �ð�

		bool		mbStopped;
	};
}
