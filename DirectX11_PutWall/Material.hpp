class Material
{
public:
				Material()
				{
								Initialize();
				}
				Material(const wchar_t* const filePath)
				{
								Initialize();
								Create(filePath);
				}
				~Material()
				{

				}
				void SetBuffer(int slot, void* cbuffer, size_t size)
				{
								constantBuffer[slot].ptr = cbuffer;

								D3D11_BUFFER_DESC constantBufferDesc = {};
								constantBufferDesc.ByteWidth = size;
								constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
								constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

								constantBuffer[slot].buffer.Release();
								App::GetGraohicsDevice().CreateBuffer(
												&constantBufferDesc,
												nullptr,
												&constantBuffer[slot].buffer
								);
				}
				void SetTexture(int slot, Texture* texture)
				{
								textures[slot] = texture;
				}
				void Attach()
				{
								for (int i = 0; i < 10; i++)
								{
												if (constantBuffer[i].ptr != nullptr)
												{
																App::GetGraphicsContext().UpdateSubresource(constantBuffer[i].buffer, 0,
																				nullptr, constantBuffer[i].ptr, 0, 0);
																App::GetGraphicsContext().VSSetConstantBuffers(i, 1, &constantBuffer[i].buffer.p);
																App::GetGraphicsContext().HSSetConstantBuffers(i, 1, &constantBuffer[i].buffer.p);
																App::GetGraphicsContext().DSSetConstantBuffers(i, 1, &constantBuffer[i].buffer.p);
																App::GetGraphicsContext().GSSetConstantBuffers(i, 1, &constantBuffer[i].buffer.p);
																App::GetGraphicsContext().PSSetConstantBuffers(i, 1, &constantBuffer[i].buffer.p);
												}
								}

								for (int i = 0; i < 10; i++)
								{
												if (textures[i] != nullptr)
												{
																textures[i]->Attach(i);
												}
								}

								if (vertexShader != nullptr)
												App::GetGraphicsContext().VSSetShader(vertexShader, nullptr, 0);

								if (pixelShader != nullptr)
												App::GetGraphicsContext().PSSetShader(pixelShader, nullptr, 0);

								if (inputLayout != nullptr)
												App::GetGraphicsContext().IASetInputLayout(inputLayout);
				}
				void Create(const wchar_t* const filePath)
				{
								ATL::CComPtr<ID3DBlob> vertexShaderBolb = nullptr;
								CompileShader(filePath, "VS", "vs_5_0", &vertexShaderBolb);

								vertexShader.Release();
								App::GetGraohicsDevice().CreateVertexShader(
												vertexShaderBolb->GetBufferPointer(),
												vertexShaderBolb->GetBufferSize(),
												nullptr,
												&vertexShader
								);

								ATL::CComPtr<ID3DBlob> pixelShaderBolb = nullptr;
								CompileShader(filePath, "PS", "ps_5_0", &pixelShaderBolb);

								pixelShader.Release();
								App::GetGraohicsDevice().CreatePixelShader(
												pixelShaderBolb->GetBufferPointer(),
												pixelShaderBolb->GetBufferSize(),
												nullptr,
												&pixelShader
								);

								std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
								{
									{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
									{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
									{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
									{ "BLENDINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0 },
									{ "BLENDINDICES",1,DXGI_FORMAT_R32G32B32A32_UINT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0 },
									{ "BLENDWEIGHT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,64,D3D11_INPUT_PER_VERTEX_DATA,0 },
									{ "BLENDWEIGHT",1,DXGI_FORMAT_R32G32B32A32_FLOAT,0,80,D3D11_INPUT_PER_VERTEX_DATA,0 },
								};

								inputLayout.Release();
								App::GetGraohicsDevice().CreateInputLayout(
												inputElementDesc.data(),
												inputElementDesc.size(),
												vertexShaderBolb->GetBufferPointer(),
												vertexShaderBolb->GetBufferSize(),
												&inputLayout
								);
				}

private:
				struct ConstantBuffer
				{
								void* ptr = nullptr;
								ATL::CComPtr<ID3D11Buffer> buffer = nullptr;
				};

				ConstantBuffer constantBuffer[10];
				Texture* textures[10];
				ATL::CComPtr<ID3D11VertexShader> vertexShader = nullptr;
				ATL::CComPtr<ID3D11PixelShader> pixelShader = nullptr;
				ATL::CComPtr<ID3D11InputLayout> inputLayout = nullptr;

				void Initialize()
				{
								App::Initialize();

								for (int i = 0; i < 10; i++)
								{
												textures[i] = nullptr;
								}
				}

				void CompileShader(const wchar_t* const filePath, const char* const entryPoint,
								const char* const shaderModel, ID3DBlob** out)
				{
								//シェーダーのエラー検出用
								DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
								//デバック時のみ実行するコード
								#if defined(DEBUG)||defined(_DEBUG)
								flags |= D3DCOMPILE_DEBUG;
								#endif

								ATL::CComPtr<ID3DBlob> error = nullptr;
								D3DCompileFromFile(
												filePath,
												nullptr,
												D3D_COMPILE_STANDARD_FILE_INCLUDE,
												entryPoint,
												shaderModel,
												flags, 0, out, &error);

								if (error != nullptr)
								{
												OutputDebugStringA((char*)error->GetBufferPointer());
												MessageBoxA(App::GetWindowHandle(),
																(char*)error->GetBufferPointer(),
																"Shader Error", MB_OK);
								}
				}
};