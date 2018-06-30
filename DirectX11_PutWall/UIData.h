#pragma once

class UIData
{
public:
				UIData();
				~UIData(){};
				
				void CreateCutinUi(Float2 numUv = Float2(0.0f, 0.0f));
				
				//UIの描画
				//現在の状態の描画、デバックで使用
				void DrawTitleUi();
				//引数は残りの押し出せる量
				void DrawNumberUi(float num, Float3 pos);
				//カットインで描画するものまたカットインの時間取得
				bool Cutin(int num);
				float GetCountTime() { return countTime; }
				//左右にプレイヤーの押し出せるブロック量
				//真ん中に沈むまでのリミット
				void DrawGaugeBackUi(Float2 numUv, int sign, int num, bool turn);
				//ゲームオーバー時の描画
				void DrawOver(bool turn);
				//上の描画を一括で行うまとめたもの
				void Draw(int gameState, Float2 playerPushLength,int drowLimitCount, bool turn);
private:
				//2Dデータを描画するカメラ
				Camera uiCamera;
				
				Texture gameTitleTex;
				Plane gameTitleUi;

				Texture numberTex;
				Plane numberUi;

				Texture cutinTex;
				Plane cutinUi;

				Texture gaugeBackTex;
				Plane gaugeBackUi;

				Texture overTex;
				Plane overUi;
				float countTime;
};

