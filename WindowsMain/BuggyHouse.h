#pragma once
#include <list>
#include <memory>
#include "D2DFramework.h"
#include "Actor.h"

// #. Vector : 연속된 공간 + 끝에서 삽입/삭제가 용이
// #. List : 삽입/삭제가 자유로움

class BuggyHouse : public D2DFramework
{
	std::shared_ptr<Actor> mspBackground;
	std::list<std::shared_ptr<Actor>> mBugList;

public:
	HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"BuggyHouse",
		UINT width = 1024, UINT height = 768) override;
	void Release() override;
	void Render() override;

	void CheckBugs();
};