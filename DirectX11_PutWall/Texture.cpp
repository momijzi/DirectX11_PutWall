#include"App.hpp"

void Texture::SetDivide(Float2 uv)
{
				if (uv.x < 0 && uv.y < 0)
				{
								//��O����
				}
				uvData.uv = uv;
}

void Texture::SetNumUvAll(Float2 numUv)
{
				if (numUv.x < 0.0f || numUv.x < uvData.uv.x ||
								numUv.y < 0.0f || numUv.y < uvData.uv.y)
				{
								//��O����
				}
				for (int i = 0; i < 6; i++)
				{
								uvData.numUv[i] = numUv;
				}
}

void Texture::Load(const wchar_t* const filePath)
{
				ATL::CComPtr<IWICImagingFactory> factory = nullptr;
				factory.CoCreateInstance(
								CLSID_WICImagingFactory,
								nullptr,
								CLSCTX_INPROC_SERVER
				);

				ATL::CComPtr<IWICBitmapDecoder> decoder = nullptr;
				factory->CreateDecoderFromFilename(
								filePath,
								0,
								GENERIC_READ,
								WICDecodeMetadataCacheOnDemand,
								&decoder
				);

				ATL::CComPtr<IWICBitmapFrameDecode> frame = nullptr;
				decoder->GetFrame(0, &frame);

				UINT width, height;
				frame->GetSize(&width, &height);

				WICPixelFormatGUID pixelFormat = {};
				frame->GetPixelFormat(&pixelFormat);

				std::unique_ptr<BYTE[]> buffer(
								new BYTE[width*height * 4]
				);

				if (pixelFormat != GUID_WICPixelFormat32bppRGBA)
				{
								ATL::CComPtr<IWICFormatConverter> converter = nullptr;
								factory->CreateFormatConverter(&converter);

								converter->Initialize(
												frame,
												GUID_WICPixelFormat32bppRGBA,
												WICBitmapDitherTypeErrorDiffusion,
												0, 0,
												WICBitmapPaletteTypeCustom
								);

								converter->CopyPixels(
												0,
												width * 4,
												width*height * 4,
												buffer.get()
								);
				}
				else
				{
								frame->CopyPixels(
												0,
												width * 4,
												width*height * 4,
												buffer.get()
								);
				}

				D3D11_TEXTURE2D_DESC textureDesc = {};
				textureDesc.Width = width;
				textureDesc.Height = height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

				D3D11_SUBRESOURCE_DATA subresourceData = {};
				subresourceData.pSysMem = buffer.get();
				subresourceData.SysMemPitch = width * 4;
				subresourceData.SysMemSlicePitch = width * height * 4;

				texture.Release();
				App::GetGraohicsDevice().CreateTexture2D(
								&textureDesc,
								&subresourceData,
								&texture
				);

				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
				shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				shaderResourceViewDesc.ViewDimension =
								D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResourceViewDesc.Texture2D.MipLevels = 1;

				shaderResourceView.Release();
				App::GetGraohicsDevice().CreateShaderResourceView(
								texture,
								&shaderResourceViewDesc,
								&shaderResourceView
				);

				D3D11_SAMPLER_DESC samplerDesc = {};
				samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.MipLODBias = 0.0f;
				samplerDesc.MaxAnisotropy = 1;
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				samplerDesc.MinLOD = 0.0f;
				samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

				samplerState.Release();
				App::GetGraohicsDevice().CreateSamplerState(
								&samplerDesc,
								&samplerState
				);

				D3D11_BLEND_DESC blendStateDesc = {};
				blendStateDesc.AlphaToCoverageEnable = false;
				blendStateDesc.IndependentBlendEnable = false;
				for (int i = 0; i < 8; i++)
				{
								blendStateDesc.RenderTarget[i].BlendEnable = true;
								blendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
								blendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
								blendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
								blendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
								blendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
								blendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
								blendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				}

				App::GetGraohicsDevice().CreateBlendState(&blendStateDesc, &blendState);

				float blendFactor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO ,D3D11_BLEND_ZERO ,D3D11_BLEND_ZERO };
				App::GetGraphicsContext().OMSetBlendState(blendState, blendFactor, 0xffffffff);
}

void Texture::Attach(int slot)
{
				App::GetGraphicsContext().PSSetShaderResources(
								slot, 1, &shaderResourceView.p
				);
				App::GetGraphicsContext().PSSetSamplers(
								slot, 1, &samplerState.p
				);
}
