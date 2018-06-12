#pragma once
class Camera
{
public:
				Float3 position;
				Float3 angles;
				float clearColor[4];
				bool isDepthTest;
				//設定するもの
				Camera(bool dimensionFlag = true,Float4 color = Float4(1.0f, 1.0f, 1.0f, 1.0f));
				~Camera() {}

				void SetPerspective();
				void SetOrthographic(float size = 1.0f);

				void Update();
private:
				struct Constant
				{
								DirectX::XMMATRIX view;
								DirectX::XMMATRIX projection;
				};
				Constant constant;
				bool dimensionFlag;//このカメラが二次元か三次元かどうかのフラグ
				float fieldOfView;//視野
				float nearClip;//クリッピング面の近面
				float farClip;//クリッピング面の遠面

				ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
				ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
				ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

				void Create();
};