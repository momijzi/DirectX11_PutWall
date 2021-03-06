#include"App.hpp"

Camera::Camera(Float4 color)
{
				App::Initialize();

				position = Float3(0.0f, 0.0f, 0.0f);
				angles = Float3(0.0f, 0.0f, 0.0f);
				clearColor[0] = color.x;
				clearColor[1] = color.y;
				clearColor[2] = color.z;
				clearColor[3] = color.w;
			 isDepthTest = true;
				fieldOfView = 60.0f;
				nearClip = 0.1f;
				farClip = 1000.0f;
				Create();
}

//カメラの設定//3D
void Camera::SetPerspective()
{
				constant.projection = DirectX::XMMatrixTranspose
				(
								DirectX::XMMatrixPerspectiveFovLH
								(
												DirectX::XMConvertToRadians(fieldOfView),//視野角
												App::GetWindowSize().x / (float)App::GetWindowSize().y,//ビュー空間のアスペクト比X:Y
												nearClip,			//クリッピング面の近面
												farClip					//遠面
								)
				);
}
//基本的に1.0fでいいと思われる
void Camera::SetOrthographic(float size)
{
				constant.projection = DirectX::XMMatrixTranspose
				(
								DirectX::XMMatrixOrthographicLH
								(
												App::GetWindowSize().x * size, App::GetWindowSize().y * size, nearClip, farClip
								)
				);
}

void Camera::SetCameraInverse()
{
				constant.view = DirectX::XMMatrixTranspose(
								DirectX::XMMatrixInverse(
												nullptr,
												DirectX::XMMatrixRotationX(
																DirectX::XMConvertToRadians(angles.x)
												)*
												DirectX::XMMatrixRotationY(
																DirectX::XMConvertToRadians(angles.y)
												)*
												DirectX::XMMatrixRotationZ(
																DirectX::XMConvertToRadians(angles.z)
												)*
												DirectX::XMMatrixTranslation(
																position.x, position.y, position.z
												)
								)
				);
}

void Camera::SetCameraLookAt(Float3 playerPos)
{
				constant.view = DirectX::XMMatrixTranspose
				(
								DirectX::XMMatrixLookAtLH(position, playerPos, Float3(0.0f, 1.0f, 0.0f))
				);
}

void Camera::Update(bool dimensionFlag,bool cameraView, Float3 playerPos)
{
				if (cameraView)
				{
								SetCameraLookAt(playerPos);
				}
				else
				{
								SetCameraInverse();
				}

				//このカメラは3Dを映すのかそれとも2Dを映すのかの判定
				if (dimensionFlag)
				{
								SetPerspective();
				}
				else
				{
								SetOrthographic(1.0f);
				}

				App::GetGraphicsContext().UpdateSubresource(
								constantBuffer, 0, nullptr, &constant, 0, 0
				);

				//頂点シェーダ
				App::GetGraphicsContext().VSSetConstantBuffers(
								0, 1, &constantBuffer.p
				);

				//ハルシェーダ
				App::GetGraphicsContext().HSSetConstantBuffers(
								0, 1, &constantBuffer.p
				);

				//ドメインシェーダ
				App::GetGraphicsContext().DSSetConstantBuffers(
								0, 1, &constantBuffer.p
				);

				//ジオメトリシェーダ
				App::GetGraphicsContext().GSSetConstantBuffers(
								0, 1, &constantBuffer.p
				);

				//ピクセルシェーダ
				App::GetGraphicsContext().PSSetConstantBuffers(
								0, 1, &constantBuffer.p
				);

				//このカメラが二番目の時画面の更新を行わないようにする
				if (dimensionFlag)
				{
								App::GetGraphicsContext().ClearRenderTargetView(renderTargetView, clearColor);
				}
				if (isDepthTest)
				{
								App::GetGraphicsContext().ClearDepthStencilView(
												depthStencilView,
												D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
												1.0f, 0);
								App::GetGraphicsContext().OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
				}
				else
				{
								App::GetGraphicsContext().OMSetRenderTargets(1, &renderTargetView.p, nullptr);
				}
}

void Camera::Create()
{
				ATL::CComPtr<ID3D11Texture2D> renderTexture = nullptr;
				App::GetSwapChain().GetBuffer(0, __uuidof(ID3D11Texture2D),
								reinterpret_cast<void**>(&renderTexture));

				App::GetGraohicsDevice().CreateRenderTargetView(renderTexture, nullptr, &renderTargetView);

				ATL::CComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;

				D3D11_TEXTURE2D_DESC depthStencilTextureDesc = {};
				depthStencilTextureDesc.Width = App::GetWindowSize().x;
				depthStencilTextureDesc.Height = App::GetWindowSize().y;
				depthStencilTextureDesc.MipLevels = 1;
				depthStencilTextureDesc.ArraySize = 1;
				depthStencilTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
				depthStencilTextureDesc.SampleDesc.Count = 1;
				depthStencilTextureDesc.SampleDesc.Quality = 0;
				depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
				depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

				App::GetGraohicsDevice().CreateTexture2D(
								&depthStencilTextureDesc,
								nullptr,
								&depthStencilTexture
				);

				D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
				depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

				App::GetGraohicsDevice().CreateDepthStencilView(
								depthStencilTexture,
								&depthStencilViewDesc,
								&depthStencilView
				);

				constantBuffer.Release();
				D3D11_BUFFER_DESC constantBufferDesc = {};
				constantBufferDesc.ByteWidth = sizeof(Constant);
				constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				App::GetGraohicsDevice().CreateBuffer(
								&constantBufferDesc,
								nullptr,
								&constantBuffer
				);
}
