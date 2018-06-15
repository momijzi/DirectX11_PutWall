#pragma once

class UIData
{
public:
				UIData();
				~UIData() {};
				
				void CreateStateUi(Float2 numUv = Float2(0.0f, 0.0f));
				void CreateNumberUi(Float2 numUv = Float2(0.0f, 0.0f));

				void DrawStateUi();
				//引数は残りの押し出せる量
				void DrawNumberUi(Float2 playerPushLength);

				void Draw(Float2 playerPushLength);
private:
				Camera uiCamera;
				
				Texture gameStateTex;
				Plane gameStateUi;
				Texture numberTex;
				Plane numberUi;
};

