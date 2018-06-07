class Texture
{
public:
				Texture()
				{
								App::Initialize();
				}

				Texture(const wchar_t* const filePath)
				{
								App::Initialize();
							
								Load(filePath);
				}

				~Texture()
				{

				}

				#pragma region Uv
				struct UvData
				{
								Float2 uv;			//画像分割数 1から分割数-1
								Float2 numUv[6];	//分割した場合どこを描画するか(四角の場合に6面違うテクスチャを使う可能性があるため配列6)

								//初期化
								UvData()
								{
												uv = Float2(1.0f, 1.0f);
												for (int i = 0; i < 6; i++)
												{
																numUv[i] = Float2(0.0f, 0.0f);
												}
								}
				};
				//引数分だけ分割して分割一つの大きさを入れる
				void SetDivide(Float2 uv)
				{
								if (uv.x < 0 && uv.y < 0)
								{
												//例外処理
								}
								uvData.uv = uv;
				}
				void SetNumUvAll(Float2 numUv)
				{
								if (numUv.x < 0.0f || numUv.x < uvData.uv.x ||
												numUv.y < 0.0f || numUv.y < uvData.uv.y)
								{
												//例外処理
								}
								for (int i = 0; i < 6; i++)
								{
												uvData.numUv[i] = numUv;
								}
				}

				//前面のuv設定
				void SetNumUvFront(Float2 numUV)
				{
								uvData.numUv[0] = numUV;
				}
				//後面のuv設定
				void SetNumUvBack(Float2 numUV)
				{
								uvData.numUv[1] = numUV;
				}
				//左面のuv設定
				void SetNumUvLeft(Float2 numUV)
				{
								uvData.numUv[2] = numUV;
				}
				//前面のuv設定
				void SetNumUvRight(Float2 numUV)
				{
								uvData.numUv[3] = numUV;
				}
				//上面のuv設定
				void SetNumUvUp(Float2 numUV)
				{
								uvData.numUv[4] = numUV;
				}
				//下面のuv設定
				void SetNumUvDown(Float2 numUV)
				{
								uvData.numUv[5] = numUV;
				}

				UvData GetUvData()
				{
								return uvData;
				}

				#pragma endregion

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
				UvData uvData;
			


				ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
				ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
				ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};