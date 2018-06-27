#include "App.hpp"

#define RadianToDegree 180/ 3.1415926535

Ray::Ray()
{
				App::Initialize();
				rayTex.Load(L"texture/test.png");
				rayTex.SetDivide(Float2(1.0f, 1.0f));
				Create();
}

Ray::~Ray()
{

}

void Ray::Create(Float2 numUv)
{
				rayTex.SetNumUvAll(numUv);
				ray.Create(&rayTex);
}

void Ray::Draw()
{
				if (drawFlag)
				{
								Float3 delta = movePos - startPos;

								ray.position = startPos + (Float3)DirectX::XMVector3Normalize(delta) / 2;
								ray.Draw();
								drawFlag = false;
				}
}

void Ray::UpDate(Float3 playerPosition, Float3 blockPosition)
{
				ray.scale = 0.02f;
				startPos = playerPosition;
				movePos = blockPosition;
				drawFlag = true;
}