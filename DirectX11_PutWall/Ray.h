#pragma once
class Ray
{
public:
				Ray();
				~Ray();

				void UpDate(Float3 playerPosition,Float3 blockPosition);
				void Create(Float2 numUv = Float2(0.0f, 0.0f));
				void Draw();
private:
				Sphere ray;
				Texture rayTex;
				Float3 startPos;
				Float3 movePos;
				bool drawFlag = false;
};