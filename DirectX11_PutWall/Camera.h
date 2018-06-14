#pragma once
class Camera
{
public:
				Float3 position;
				Float3 angles;
				float clearColor[4];
				bool isDepthTest;
				//設定するもの
				Camera(Float4 color = Float4(1.0f, 1.0f, 1.0f, 1.0f));
				~Camera() {}

				//カメラの視野の見方 3Dなら上の方を2Dなら下を使う
				void SetPerspective();
				void SetOrthographic(float size = 1.0f);
				//カメラの対象の見方 
				void SetCameraInverse();//カメラ視点なのでカメラのangleを使用
				void SetCameraLookAt(Float3 playerPos);//カメラから対象に視点を向けるので対象のpositionが必要

				void Update(bool dimensionFlag = true,bool cameraView = false, Float3 playerPos = Float3(0.0f,0.0f,0.0f));
private:
				struct Constant
				{
								DirectX::XMMATRIX view;
								DirectX::XMMATRIX projection;
				};
				Constant constant;
				float fieldOfView;//視野
				float nearClip;//クリッピング面の近面
				float farClip;//クリッピング面の遠面

				ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
				ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
				ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

				void Create();
};