#pragma once

class UIData
{
public:
				UIData();
				~UIData() {};
				
				void CreateStateUi(Float2 numUv = Float2(0.0f, 0.0f));
				void CreateNumberUi(Float2 numUv = Float2(0.0f, 0.0f));

				//UIの描画
				//現在の状態の描画、デバックで使用
				void DrawStateUi();
				//引数は残りの押し出せる量
				void DrawNumberUi(Float2 playerPushLength);
				//上の描画を一括で行うまとめたもの
				void Draw(Float2 playerPushLength);
private:
				//2Dデータを描画するカメラ
				Camera uiCamera;
				
				Texture gameStateTex;
				Plane gameStateUi;
				Texture numberTex;
				Plane numberUi;
};

