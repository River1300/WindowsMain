#pragma once

#include <memory>
#include <list>
#include "D2DFramework.h"
#include "Actor.h"

// #. 임의의 위치에 40마리의 벌레를 그려본다.
//		=> 동적 관리 + 자유롭게 삽입/삭제하는 std::list를 사용
//				=> 원소들이 다양한 곳에서 사용되므로 shared_ptr사용

class BuggyHouse : public D2DFramework
{
	std::shared_ptr<Actor> mspBackground;
	std::list<std::shared_ptr<Actor>> mBugList;

public:
	virtual HRESULT Initialize(
		HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example",
		UINT width = 1024, UINT height = 768) override;

	virtual void Release() override;
	virtual void Render() override;
};