class Mesh
{
public:
				//CUBE��IN�͓����@���@OUT�͊O���@��
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

				//����̖��
				//�������镨�̂�ς��������e�N�X�`����ς�����UV�䗦��������炾�߂Ȃ̂łǂ����邩
				//���@�P�D�e�N�X�`����ς���ێ��O�Ɍ��ݕ`�悵�Ă���f�[�^�������Ă���
				//�����ăe�N�X�`����K���������f�[�^��������x�쐬����
				//�Ă����ꂵ���Ȃ��C���B�B�B�B

				//�f�[�^�쐬�@�e�N�X�`����UV�͂����܂łɎw�肵�Ă���
				//�e�N�X�`����ς���Ƃ��͍ēx���̊֐����ĂԕK�v������
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

				//�𐶐����Ă���
				//uv��1.0�ȉ��ɂȂ��uv�̎n�܂�ӏ������������Ȃ�̂ł����߂��Ȃ�
				//numUV�͎w�肵���摜�̍�����W���Ƃ�̂łق����ꏊ - 1 ���W������
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
				//normal�ɂ�1��-1��������Ă͂����Ȃ�
				void CreateCube(Texture* tex,int normal = 1,Float2 size = Float2(0.5f, 0.5f),bool souldClear = true)
				{
								if (souldClear)
								{
												vertices.clear();
												indices.clear();
								}
								//�O
								CreatePlane(size, Float3(0.0f, 0.0f, -0.5f), false,
												Float3(1.0f, 0.0f, 0.0f) * normal,		//x ������-1�����Ă��Ɩ@�����t�]����
												Float3(0.0f, 1.0f, 0.0f),											//y
												Float3(0.0f, 0.0f, 1.0f),											//z
												tex->uv, tex->numUV[0]);												//uv
								//��
								CreatePlane(size, Float3(0.0f, 0.0f, 0.5f), false,
												Float3(-1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 1.0f, 0.0f),
												Float3(0.0f, 0.0f, -1.0f),
												tex->uv, tex->numUV[1]);
								//��
								CreatePlane(size, Float3(0.5f, 0.0f, 0.0f), false,
												Float3(0.0f, 0.0f, 1.0f)* normal, 
												Float3(0.0f, 1.0f, 0.0f), 
												Float3(-1.0f, 0.0f, 0.0f),
												tex->uv, tex->numUV[2]);
								//�E
								CreatePlane(size, Float3(-0.5f, 0.0f, 0.0f), false,
												Float3(0.0f, 0.0f, -1.0f)* normal,
												Float3(0.0f, 1.0f, 0.0f),
												Float3(1.0f, 0.0f, 0.0f),
												tex->uv, tex->numUV[3]);
								//��
								CreatePlane(size, Float3(0.0f, 0.5f, 0.0f), false,
												Float3(1.0f, 0.0f, 0.0f)* normal,
												Float3(0.0f, 0.0f, 1.0f),
												Float3(0.0f, -1.0f, 0.0f),
												tex->uv, tex->numUV[4]);
								//��
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
