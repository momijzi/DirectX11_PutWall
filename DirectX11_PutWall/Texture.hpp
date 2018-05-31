class Texture
{
public:
				Float2 uv;			//画像分割数 1から分割数-1
				Float2 numUV[6];	//分割した場合どこを描画するか(四角)

				Texture()
				{
								App::Initialize();
				}

				Texture(const wchar_t* const filePath)
				{
								App::Initialize();
								uv = Float2(1.0f, 1.0f);
								for (int i = 0; i < 6; i++)
								{
												numUV[i] = Float2(0.0f, 0.0f);
								}
								Load(filePath);
				}

				~Texture()
				{

				}

				//引数分だけ分割して分割一つの大きさを入れる
				void SetDivide(Float2 uv)
				{
								if (uv.x > 0 && uv.y > 0)
								{
												this->uv = uv;
								}
				}
				//前面同じuvに設定
				void SetAll(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												for (int i = 0; i < 6; i++)
												{
																this->numUV[i] = numUV;
												}
								}
				}
				//上と下以外のuv設定
				void SetAround(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												for (int i = 0; i < 4; i++)
												{
																this->numUV[i] = numUV;
												}
								}
				}
				//前面のuv設定
				void SetFront(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												this->numUV[0] = numUV;
								}								
				}
				//後面のuv設定
				void SetBack(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												this->numUV[1] = numUV;
								}
				}
				//左面のuv設定
				void SetLeft(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												this->numUV[2] = numUV;
								}
				}
				//前面のuv設定
				void SetRight(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												this->numUV[3] = numUV;
								}
				}
				//上面のuv設定
				void SetUp(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												this->numUV[4] = numUV;
								}
				}
				//下面のuv設定
				void SetDown(Float2 numUV)
				{
								if (numUV.x >= 0 && numUV.y >= 0)
								{
												this->numUV[5] = numUV;
								}
				}

				void Load(const wchar_t* const filePath)
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
				}

				void Attach(int slot)
				{
								App::GetGraphicsContext().PSSetShaderResources(
												slot, 1, &shaderResourceView.p
								);
								App::GetGraphicsContext().PSSetSamplers(
												slot, 1, &samplerState.p
								);
				}

private:
				ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
				ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
				ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};