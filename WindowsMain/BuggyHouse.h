#pragma once

#include <memory>
#include <list>
#include "D2DFramework.h"
#include "Actor.h"

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

	void CheckBugs();	// 벌레를 클릭 했는지 처리하는 함수
};