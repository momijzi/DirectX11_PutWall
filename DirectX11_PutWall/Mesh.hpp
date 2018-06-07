class Mesh
{
public:
				//CUBEのINは内側法線　OUTは外側法線
				enum CreateMode { NON, TRIANGLE, PLANE, CUBEIN, CUBEOUT, SPHERE };
				Float3 position;
				Float3 angles;
				Float3 scale;
				std::vector<Vertex> vertices;
				std::vector<int> indices;

				Mesh()
				{
								App::Initialize();

								position = Float3(0.0f, 0.0f, 0.0f);
								angles = Float3(0.0f, 0.0f, 0.0f);
								scale = Float3(1.0f, 1.0f, 1.0f);

								material = Material(L"Shader.hlsl");
				}
				~Mesh()
				{

				}

				//現状の問題
				//生成する物体を変えたいがテクスチャを変えた時UV比率が違ったらだめなのでどうするか
				//方法１．テクスチャを変える際事前に現在描画しているデータを持っておく
				//そしてテクスチャを適応させたデータをもう一度作成する
				//てかこれしかない気が。。。。

				//データ作成　テクスチャのUVはここまでに指定しておく
				//テクスチャを変えるときは再度この関数を呼ぶ必要がある
				void CreateData(Texture* tex,CreateMode mode,Float2 size = Float2(0.5f,0.5f))
				{
								material.SetTexture(0, tex);

								switch (mode)
								{
												case TRIANGLE:
																CreateTriangle();
																break;
												case PLANE:
																CreatePlane(size);
																break;
												case CUBEIN:
																CreateCube(tex->GetUvData(),-1);
																break;
												case CUBEOUT:
																CreateCube(tex->GetUvData(),1);
																break;
												case SPHERE:
																//CreateSphere();
																break;
								}
								Apply();
				}
				//テクスチャを特に設定しないトライアングル生成
				void CreateTriangle()
				{
								vertices.clear();

								vertices.push_back({ Float3(0.0f, 1.0f, 0.0f),Float3(1.0f,0.0f,0.0f),Float2(0.5f,0.0f) });
								vertices.push_back({ Float3(1.0f,-1.0f, 0.0f),Float3(0.0f,1.0f,0.0f),Float2(1.0f,1.0f) });
								vertices.push_back({ Float3(-1.0f,-1.0f,0.0f),Float3(0.0f,0.0f,1.0f),Float2(0.0f,1.0f) });
				}
				//球体生成用の三角形生成
				void CreateTriangle(
								Float3 vertices1,
								Float3 vertices2,
								Float3 vertices3,
								bool souldClear = true,
								Float2 uv = Float2(1.0f, 1.0f),
								Float2 numUV = Float2(0.0f, 0.0f)
				)
				{
								if (souldClear)
								{
												vertices.clear();
								}

								vertices.push_back(Vertex(vertices1, Float3(-1.0f, 0.0f, 0.0f), Float2((numUV.x + 0.5f)*(1 / uv.x), numUV.y*(1 / uv.y))));
								vertices.push_back(Vertex(vertices2, Float3(0.0f, -1.0f, 0.0f), Float2((numUV.x + 1.0f)*(1 / uv.x), numUV.y*(1 / uv.y))));
								vertices.push_back(Vertex(vertices3, Float3(0.0f, 0.0f, -1.0f), Float2(numUV.x*(1 / uv.x), (numUV.y + 1.0f)*(1 / uv.y))));
				}

				//板を生成している
				//uvが1.0以下になるとuvの始まる箇所がおかしくなるのでお勧めしない
				//numUVは指定した画像の左上座標をとるのでほしい場所 - 1 座標が合う
				void CreatePlane(Float2 size = Float2(0.5f, 0.5f),Float3 offset = Float3(0.0f, 0.0f, 0.0f),
								bool souldClear = true,Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
								Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f),
								Float2 uv = Float2(1.0f,1.0f),Float2 numUv = Float2(0.0f,0.0f))
				{
								if (souldClear)
								{
												vertices.clear();
												indices.clear();
								}

								leftDirection = DirectX::XMVector3Normalize(leftDirection);
								upDirection = DirectX::XMVector3Normalize(upDirection);
								forwardDirection = DirectX::XMVector3Normalize(forwardDirection);

								vertices.push_back({ leftDirection*-size.x + upDirection*size.y + offset,
									-forwardDirection ,Float2(numUv * (1 / uv)) });
								vertices.push_back({ leftDirection*size.x + upDirection*size.y + offset,
									-forwardDirection ,Float2((numUv.x + 1) * (1 / uv.x), numUv.y *(1 / uv.y)) });
								vertices.push_back({ leftDirection*-size.x + upDirection*-size.y + offset,
									-forwardDirection ,Float2(numUv.x * (1 / uv.x), (numUv.y + 1) *(1 / uv.y)) });
								vertices.push_back({ leftDirection*size.x + upDirection*-size.y + offset,
									-forwardDirection ,Float2((numUv + 1) * (1 / uv)) });

								size_t indexOffset = vertices.size() - 4;
								indices.push_back(indexOffset + 0);
								indices.push_back(indexOffset + 1);
								indices.push_back(indexOffset + 2);
								indices.push_back(indexOffset + 3);
								indices.push_back(indexOffset + 2);
								indices.push_back(indexOffset + 1);
				}
				//normalには1か-1しか入れてはいけない
				void CreateCube(Texture::UvData uvData,int normal = 1,Float2 size = Float2(0.5f, 0.5f),bool souldClear = true)
				{
								if (souldClear)
								{
												vertices.clear();
												indices.clear();
								}
								//前
								CreatePlane(size, Float3(0.0f, 0.0f, -0.5f), false,
												Float3(1.0f, 0.0f, 0.0f) * normal,		//x ここに-1かけてやると法線が逆転する
												Float3(0.0f, 1.0f, 0.0f),											//y
												Float3(0.0f, 0.0f, 1.0f),											//z
												uvData.uv, uvData.numUv[0]);												//uv
								//後
								CreatePlane(size, Float3(0.0f, 0.0f, 0.5f), false,
												Float3(-1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 1.0f, 0.0f),
												Float3(0.0f, 0.0f, -1.0f),
												uvData.uv, uvData.numUv[1]);
								//左
								CreatePlane(size, Float3(0.5f, 0.0f, 0.0f), false,
												Float3(0.0f, 0.0f, 1.0f)* normal, 
												Float3(0.0f, 1.0f, 0.0f), 
												Float3(-1.0f, 0.0f, 0.0f),
												uvData.uv, uvData.numUv[2]);
								//右
								CreatePlane(size, Float3(-0.5f, 0.0f, 0.0f), false,
												Float3(0.0f, 0.0f, -1.0f)* normal,
												Float3(0.0f, 1.0f, 0.0f),
												Float3(1.0f, 0.0f, 0.0f),
												uvData.uv, uvData.numUv[3]);
								//上
								CreatePlane(size, Float3(0.0f, 0.5f, 0.0f), false,
												Float3(1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 0.0f, 1.0f),
												Float3(0.0f, -1.0f, 0.0f),
												uvData.uv, uvData.numUv[4]);
								//下
								CreatePlane(size, Float3(0.0f, -0.5f, 0.0f), false,
												Float3(1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 0.0f, -1.0f),
												Float3(0.0f, 1.0f, 0.0f),
												uvData.uv, uvData.numUv[5]);
				}
				//球体の生成
				void  CreateSphere(
								Texture::UvData uvData,
								bool souldClear = true,
								int heightRaito = 30,			//何度で分割するか ※90の約数でないとあかんです・・
								int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0),	//開始角指定用の引数,半球などの作成時に使用する
								Float3 EndAngle = Float3(360, 360, 360)			//終了角指定用の引数,半球などの作成時に使用する
				)
				{
								if (souldClear)
								{
												vertices.clear();
												indices.clear();
								}

								float heightStart = StartAngle.y / heightRaito;
								float widthStart = StartAngle.x / widthRaito;

								//何回分割するか
								float heightSplit = EndAngle.y / heightRaito;
								float widthSplit = EndAngle.x / widthRaito;

								for (float n = heightSplit / 4 + heightStart / 2; n < (heightSplit * 3) / 4; n++)
								{
												for (float i = widthStart; i < widthSplit; i++)
												{
																CreateTriangle(
																				Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))), -(sin(DirectX::XMConvertToRadians((n)* heightRaito))), sin(DirectX::XMConvertToRadians(i * widthRaito))* (cos(DirectX::XMConvertToRadians(n * heightRaito))))*0.5f,
																				Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)) * (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))), -(sin(DirectX::XMConvertToRadians((n + 1) * heightRaito))), sin(DirectX::XMConvertToRadians(i * widthRaito))* (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))))*0.5f,
																				Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))), -(sin(DirectX::XMConvertToRadians((n)* heightRaito))), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))))*0.5f,
																				false
																);

																CreateTriangle(
																				Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))), -(sin(DirectX::XMConvertToRadians((n)* heightRaito))), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))))*0.5f,
																				Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)) * (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))), -(sin(DirectX::XMConvertToRadians((n + 1) * heightRaito))), sin(DirectX::XMConvertToRadians(i * widthRaito)) * (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))))*0.5f,
																				Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))), -(sin(DirectX::XMConvertToRadians((n + 1) * heightRaito))), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))))*0.5f,
																				false
																);
												}
								}
				}

				//筒状のオブジェクトを作成する
				void CreateTube(
								Texture::UvData uvData,
								bool souldClear = true,
								int heightRaito = 30,					//何度で分割するか 90の約数で頼む
								int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0),	//開始角指定用の引数,半球などの作成時に使用する
								Float3 EndAngle = Float3(360, 360, 360)			//終了角指定用の引数,半球などの作成時に使用する
				)
				{
								if (souldClear)
								{
												vertices.clear();
												indices.clear();
								}

								float heightStart = StartAngle.y / heightRaito;
								float widthStart = StartAngle.x / widthRaito;

								float heightSplit = EndAngle.y / heightRaito;
								float widthSplit = EndAngle.x / widthRaito;

								for (float n = heightSplit / 4 + heightStart / 2; n < (heightSplit * 3) / 4; n++)
								{
												for (float i = widthStart; i < widthSplit; i++)
												{
																if ((int)heightSplit != (int)heightStart)
																{
																				CreateTriangle(
																								Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*(n - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians(i * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*((n + 1) - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians(i * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*(n - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)))*0.5f,
																								false,
																								uvData.uv,
																								uvData.numUv[0]
																				);

																				CreateTriangle(
																								Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*(n - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*((n + 1) - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians(i * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*((n + 1) - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)))*0.5f,
																								false,
																								uvData.uv,
																								uvData.numUv[0]
																				);
																}
												}

												//内側
												for (float i = widthStart; i < widthSplit; i++)
												{
																if ((int)heightSplit != (int)heightStart)
																{
																				CreateTriangle(
																								Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*(n - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians(i * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*(n - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*((n + 1) - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians(i * widthRaito)))*0.5f,
																								false,
																								uvData.uv,
																								uvData.numUv[0]
																				);

																				CreateTriangle(
																								Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*(n - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*((n + 1) - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)))*0.5f,
																								Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)), (float)1 / ((heightSplit - heightStart) / 4)*((n + 1) - (heightSplit - heightStart) / 2), sin(DirectX::XMConvertToRadians(i * widthRaito)))*0.5f,
																								false,
																								uvData.uv,
																								uvData.numUv[0]
																				);
																}
												}
								}
				}


				Material& GetMaterial()
				{
								return material;
				}
				void Apply()
				{
								vertexBuffer.Release();
								if (vertices.size() > 0)
								{
												D3D11_BUFFER_DESC vertexBufferDesc = {};
												vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
												vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
												vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
												vertexBufferDesc.CPUAccessFlags = 0;
												vertexBufferDesc.MiscFlags = 0;
												D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
												vertexSubresourceData.pSysMem = vertices.data();

												App::GetGraohicsDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData,
																&vertexBuffer);
								}

								indexBuffer.Release();
								if (indices.size() > 0)
								{
												D3D11_BUFFER_DESC indexBufferDesc = {};
												indexBufferDesc.ByteWidth = sizeof(int) * indices.size();
												indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
												indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
												indexBufferDesc.CPUAccessFlags = 0;
												indexBufferDesc.MiscFlags = 0;
												D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
												indexSubresourceData.pSysMem = indices.data();

												App::GetGraohicsDevice().CreateBuffer(&indexBufferDesc, &indexSubresourceData,
																&indexBuffer);
								}
								material.SetBuffer(1, &constant, sizeof(Constant));
				}
				void Draw(Float3 postion = Float3(0.0f, 0.0f, 0.0f), Float3 angles = Float3(0.0f,0.0f,0.0f))
				{
								if (vertexBuffer == nullptr)
												return;

								constant.world = DirectX::XMMatrixTranspose(
												DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)*
												DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z))*
												DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x))*
												DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y))*
												DirectX::XMMatrixTranslation(position.x, position.y, position.z)
								);

								material.Attach();

								UINT stride = sizeof(Vertex);
								UINT offset = 0;
								App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p,
												&stride, &offset);

								if (indexBuffer == nullptr)
								{
												App::GetGraphicsContext().Draw(vertices.size(), 0);
								}
								else
								{
												App::GetGraphicsContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
												App::GetGraphicsContext().DrawIndexed(indices.size(), 0, 0);
								}
				}

private:
				struct Constant
				{
								DirectX::XMMATRIX world;
				};

				Material material;
				Constant constant;
				ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
				ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;
};
