#pragma once
#include <list>
#include <memory>
#include "D2DFramework.h"
#include "Actor.h"

// #. Vector : ���ӵ� ���� + ������ ����/������ ����
// #. List : ����/������ �����ο�

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