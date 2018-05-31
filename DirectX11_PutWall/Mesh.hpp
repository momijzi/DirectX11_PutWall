class Mesh
{
public:
				//CUBEのINは内側法線　OUTは外側法線
				enum CreateMode{NON,TRIANGLE,PLANE,CUBEIN,CUBEOUT};
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
				void CreateData(Texture* tex,CreateMode mode)
				{
								material.SetTexture(0, tex);

								switch (mode)
								{
												case TRIANGLE:
																CreateTriangle();
																break;
												case PLANE:
																CreatePlane();
																break;
												case CUBEIN:
																CreateCube(tex,-1);
																break;
												case CUBEOUT:
																CreateCube(tex,1);
																break;
												default:
																return;
																break;
								}
								Apply();
				}

				void CreateTriangle()
				{
								vertices.clear();

								vertices.push_back({ Float3(0.0f, 1.0f, 0.0f),Float3(1.0f,0.0f,0.0f),Float2(0.5f,0.0f) });
								vertices.push_back({ Float3(1.0f,-1.0f, 0.0f),Float3(0.0f,1.0f,0.0f),Float2(1.0f,1.0f) });
								vertices.push_back({ Float3(-1.0f,-1.0f,0.0f),Float3(0.0f,0.0f,1.0f),Float2(0.0f,1.0f) });
				}

				//板を生成している
				//uvが1.0以下になるとuvの始まる箇所がおかしくなるのでお勧めしない
				//numUVは指定した画像の左上座標をとるのでほしい場所 - 1 座標が合う
				void CreatePlane(Float2 size = Float2(0.5f, 0.5f),Float3 offset = Float3(0.0f, 0.0f, 0.0f),
								bool souldClear = true,Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
								Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f),
								Float2 uv = Float2(1.0f,1.0f),Float2 numUV = Float2(0.0f,0.0f))
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
									-forwardDirection ,Float2(numUV * (1 / uv)) });
								vertices.push_back({ leftDirection*size.x + upDirection*size.y + offset,
									-forwardDirection ,Float2((numUV.x + 1) * (1 / uv.x), numUV.y *(1 / uv.y)) });
								vertices.push_back({ leftDirection*-size.x + upDirection*-size.y + offset,
									-forwardDirection ,Float2(numUV.x * (1 / uv.x), (numUV.y + 1) *(1 / uv.y)) });
								vertices.push_back({ leftDirection*size.x + upDirection*-size.y + offset,
									-forwardDirection ,Float2((numUV + 1) * (1 / uv)) });

								size_t indexOffset = vertices.size() - 4;
								indices.push_back(indexOffset + 0);
								indices.push_back(indexOffset + 1);
								indices.push_back(indexOffset + 2);
								indices.push_back(indexOffset + 3);
								indices.push_back(indexOffset + 2);
								indices.push_back(indexOffset + 1);
				}
				//normalには1か-1しか入れてはいけない
				void CreateCube(Texture* tex,int normal = 1,Float2 size = Float2(0.5f, 0.5f),bool souldClear = true)
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
												tex->uv, tex->numUV[0]);												//uv
								//後
								CreatePlane(size, Float3(0.0f, 0.0f, 0.5f), false,
												Float3(-1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 1.0f, 0.0f),
												Float3(0.0f, 0.0f, -1.0f),
												tex->uv, tex->numUV[1]);
								//左
								CreatePlane(size, Float3(0.5f, 0.0f, 0.0f), false,
												Float3(0.0f, 0.0f, 1.0f)* normal, 
												Float3(0.0f, 1.0f, 0.0f), 
												Float3(-1.0f, 0.0f, 0.0f),
												tex->uv, tex->numUV[2]);
								//右
								CreatePlane(size, Float3(-0.5f, 0.0f, 0.0f), false,
												Float3(0.0f, 0.0f, -1.0f)* normal,
												Float3(0.0f, 1.0f, 0.0f),
												Float3(1.0f, 0.0f, 0.0f),
												tex->uv, tex->numUV[3]);
								//上
								CreatePlane(size, Float3(0.0f, 0.5f, 0.0f), false,
												Float3(1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 0.0f, 1.0f),
												Float3(0.0f, -1.0f, 0.0f),
												tex->uv, tex->numUV[4]);
								//下
								CreatePlane(size, Float3(0.0f, -0.5f, 0.0f), false,
												Float3(1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 0.0f, -1.0f),
												Float3(0.0f, 1.0f, 0.0f),
												tex->uv, tex->numUV[5]);
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
				void Draw()
				{
								if (vertexBuffer == nullptr)
												return;

								constant.world = DirectX::XMMatrixTranspose(
												DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)*
												DirectX::XMMatrixRotationX(angles.x)*
												DirectX::XMMatrixRotationY(angles.y)*
											 DirectX::XMMatrixRotationZ(angles.z)*
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
