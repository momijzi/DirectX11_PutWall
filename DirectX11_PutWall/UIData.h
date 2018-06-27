#pragma once

class UIData
{
public:
				UIData();
				~UIData() {};
				
				void CreateNumberUi(Float2 numUv = Float2(0.0f, 0.0f));
				void CreateCutinUi(Float2 numUv = Float2(0.0f, 0.0f));
				//UIの描画
				//現在の状態の描画、デバックで使用
				void DrawTitleUi();
				//引数は残りの押し出せる量
				void DrawNumberUi(Float2 playerPushLength, int drowLimitCount);
				//カットインで描画するもの
				bool Cutin(int num);
				//上の描画を一括で行うまとめたもの
				void Draw(int gameState, Float2 playerPushLength,int drowLimitCount);
private:
				//2Dデータを描画するカメラ
				Camera uiCamera;
				
				Texture gameTitleTex;
				Plane gameTitleUi;
				Texture numberTex;
				Plane numberUi;
				Texture cutinTex;
				Plane cutinUi;

				float countTime;
};

