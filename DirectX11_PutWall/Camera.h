#pragma once

class Camera
{
public:
				Float3 position;
				Float3 angles;
				float clearColor[4] = { 1.0f,0.0f,1.0f,1.0f };
				bool isDepthTest;

				Camera();
				~Camera() {}
				void Update();
private:
				struct Constant
				{
								DirectX::XMMATRIX view;
								DirectX::XMMATRIX projection;
				};

				Constant constant;
				ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
				ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
				ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

				void Create();
};