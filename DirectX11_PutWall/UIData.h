#pragma once

class UIData
{
public:
				UIData();
				~UIData() {};

				Plane stateUi;
				
				void Draw();

private:
				Camera uiCamera;
				
};

