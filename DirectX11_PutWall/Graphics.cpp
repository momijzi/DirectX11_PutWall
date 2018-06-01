#include"App.hpp"

App::Graphics::Graphics()
{
				App::Initialize();

				std::vector<D3D_DRIVER_TYPE> driverTypes
				{
								D3D_DRIVER_TYPE_HARDWARE,
								D3D_DRIVER_TYPE_WARP,
								D3D_DRIVER_TYPE_REFERENCE,
								D3D_DRIVER_TYPE_SOFTWARE,
				};

				std::vector<D3D_FEATURE_LEVEL> featureLevels
				{
								D3D_FEATURE_LEVEL_11_0,
								D3D_FEATURE_LEVEL_10_1,
								D3D_FEATURE_LEVEL_10_0,
				};

				UINT flags = 0;
				#if defined(DEBUG) || defined(_DEBUG)
				flags |= D3D11_CREATE_DEVICE_DEBUG;
				#endif

				//スワップチェイン
				DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
				swapChainDesc.BufferDesc.Width = App::GetWindowSize().x;
				swapChainDesc.BufferDesc.Height = App::GetWindowSize().y;
				swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
				swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
				swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				swapChainDesc.SampleDesc.Count = 1;
				swapChainDesc.SampleDesc.Quality = 0;
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.BufferCount = 1;
				swapChainDesc.OutputWindow = App::GetWindowHandle();
				swapChainDesc.Windowed = true;
				swapChainDesc.Flags = 0;

				for (size_t i = 0; i < driverTypes.size(); i++)
				{
								HRESULT r =
												D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i],
																nullptr, flags, featureLevels.data(), featureLevels.size(),
																D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device,
																nullptr, &context);

								if (SUCCEEDED(r))
												break;
				}

				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//カメラの範囲
				D3D11_VIEWPORT viewPort = {};
				viewPort.TopLeftX = 0.0f;
				viewPort.TopLeftY = 0.0f;
				viewPort.Width = static_cast<float>(App::GetWindowSize().x);
				viewPort.Height = static_cast<float>(App::GetWindowSize().y);
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.0f;
				context->RSSetViewports(1, &viewPort);
}