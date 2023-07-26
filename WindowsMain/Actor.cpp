#include "Actor.h"
#include "BitmapManager.h"

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) :
	mpFramework{ pFramework }, mX{}, mY{}, mOpacity{ 1.0f }
{
	mpBitmap = BitmapManager::Instance().LoadBitmap(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) :
	Actor(pFramework, filename)
{
	this->mX = x;
	this->mY = y;
	this->mOpacity = opacity;
}

Actor::~Actor()
{

}

void Actor::Draw(float x, float y, float opacity)
{
	auto pRenderTarget = mpFramework->GetRenderTarget();

	if (pRenderTarget != nullptr)
	{
		auto size{ mpBitmap->GetPixelSize() };
		D2D1_RECT_F rect{
			x,y,
			static_cast<float>(x + size.width),
			static_cast<float>(y + size.height)
		};

		pRenderTarget->DrawBitmap(
			mpBitmap,
			rect,
			opacity
		);
	}
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}