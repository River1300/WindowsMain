#pragma once

// #. 비트맵 매니저 : "이미지를 불러오고 비트맵을 생성하는 것"과 "화면에 그리는 것"을 분리
//		=> 파일 이름을 받아서 ID2D1Bitmap 객체를 생성
//			=> 처음 생성하는 것이라면 내부에 ID2D1Bitmap 객체를 생성하고 그 포인터를 반환
//			=> 이미 존재하는 것이라면 해당 ID2D1Bitmap 객체를 반환
//		=> 내부 관리를 위한 목록 필요
//			=> ID2D1Bitmap 집합
//			=> 파일 이름으로 검색( 중복 불가능 )
#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h>
#include <map>
#include <string>

// #. final : 해당 클래스로부터 더이상 자식을 만들 수 없음
class BitmapManager final
{
public:
	// #. 싱글턴 패턴
	//		=> static을 활용하여 생성자가 여러번 호출되어도 하나만 생성되게 만든다.
	//		=> 인스턴스를 반환하는 함수를 만든다.
	//			=> 내부에 static 변수로 클래스 인스턴스를 만들고 자신을 반환한다.
	//		=> 클래스의 멤버함수를 호출하려면 인스턴스가 필요 -> static 함수로 만들어야 한다.
	static BitmapManager& Instance()
	{
		static BitmapManager instance;
		return instance;
	}

private:
	// #. 싱글턴은 단 하나의 인스턴스만 존재해야만 한다.
	//		=> 기본 생성자, 복사 생성자, 복사 대입 생성자를 숨겨버린다.
	BitmapManager() {}
	BitmapManager(const BitmapManager&) {}
	void operator =(const BitmapManager&) {}

public:
	~BitmapManager() {}

private:
	ID2D1HwndRenderTarget* mpRenderTarget{};	// 이미지를 불러온 후 컨버팅할 때 사용하는 렌더 타겟
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};

	// #. map은 [key, value]형태의 쌍으로 되어 있다. 이미지를 관리하기 위해서 <파일명, 비트맵> 형태의 쌍이 필요
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources;

private:
	// 내부 함수로 코덱을 만들고 컨버팅하여 ID2D1Bitmap을 생성해 주는 함수
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);

public:
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();
	// 맵에 데이터가 있는지 없는지 확인하고 관리하는 기능을 하는 함수
	//		=> 로딩을 했는지 안했는지 알 수 있게 파일명을 매개변수로 받아서 확인한다.
	ID2D1Bitmap* LoadBitmap(std::wstring filename);
};