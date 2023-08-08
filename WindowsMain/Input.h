#pragma once
#include <array>

// #. Input
//	다양한 디바이스를 처리하기 위해서 서로 다른 루틴이 필요하며 각 장치의 특성을 이해해야 한다.
//	가령 마우스는 상하좌우 커서의 움직임 정도를 파악해야 하며, 키보드는 각 키의 상태를, 게임 패드는 아날로그 입력과 진동의 특성
//	을 파악해야 한다. 게임 별로 사용되는 레이싱 휠이나 플라이트 스틱의 경우는 개별적인 처리가 추가로 필요하다.
// #. DirectInput
//	정확한 데이터 수집
//		=> 입력 장치의 모든 상태( 진동 : Force Feedback 포함 )를 알 수 있다.
//		=> 앱이 비활성 상태( 백그라운드 상태 )일 때도 입력 장치의 값을 취득 가능
//	커스터마이징이 쉬움
//		=> 액션 매핑을 사용하여 장치의 데이터를 게임의 액션으로 연동이 가능
// #. XInput
//	마이크로 소프트의 콘솔 XBox와 PC의 경계를 허물려는 시도 중 대표적인 라이브러리
//		=> XBOX Controller는 PC와 완벽히 호환되는 유/무선 컨트롤러 이다.
//		=> 아날로그 스틱 2개 및 여러 디지털 버튼과 아날로그 트리거를 가지고 있어서 다양한 조작이 가능하다.
//		=> Force Feedback을 위한 2개의 모터를 제어
//		=> 헤드셋을 사용해 음성을 녹음하고 전송

namespace MyUtil
{
	class Input
	{	// 키보드, 마우스
		std::array<bool, 256> mKeys;
		int mCursorX, mCursorY;

	public:
		void Initialize();
		// 어떤 버튼을 누렀는지
		void SetKeyDown(unsigned int key);
		void SetKeyUp(unsigned int key);
		void SetCursor(int x, int y);

		bool IsKeyDown(unsigned int key);
		void GetCursorPosition(int& x, int& y);
	};
}