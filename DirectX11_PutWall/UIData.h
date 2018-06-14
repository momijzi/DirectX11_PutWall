#pragma once

class UIData
{
public:
				UIData();
				~UIData() {};
				
				void CreateStateUi(Float2 numUv);
				
				void Draw();

private:
				Camera uiCamera;
				
				Texture gameStateTex;
				Plane stateUi;
};

