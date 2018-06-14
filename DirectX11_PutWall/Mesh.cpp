#include"App.hpp"

Mesh::Mesh()
{
				App::Initialize();

				position = Float3(0.0f, 0.0f, 0.0f);
				angles = Float3(0.0f, 0.0f, 0.0f);
				scale = Float3(1.0f, 1.0f, 1.0f);
				axis = Float3(0.0f, 0.0f, 0.0f);

				material = Material(L"Shader.hlsl");
}
//���̐����p�̎O�p�`����
void Mesh::CreateTriangle(Float3 vertices1, Float3 vertices2, Float3 vertices3,
				bool souldClear, Float2 uv, Float2 numUv)
{
				if (souldClear)
				{
								vertices.clear();
				}

				vertices.push_back(Vertex(vertices1, Float3(-1.0f, 0.0f, 0.0f), Float2((numUv.x + 0.5f)*(1 / uv.x), numUv.y*(1 / uv.y))));
				vertices.push_back(Vertex(vertices2, Float3(0.0f, -1.0f, 0.0f), Float2((numUv.x + 1.0f)*(1 / uv.x), numUv.y*(1 / uv.y))));
				vertices.push_back(Vertex(vertices3, Float3(0.0f, 0.0f, -1.0f), Float2(numUv.x*(1 / uv.x), (numUv.y + 1.0f)*(1 / uv.y))));
}

void Mesh::CreatePlane(Float2 uv, Float2 numUv,
				Float2 size, Float3 offset, bool souldClear,
				Float3 leftDirection, Float3 upDirection, Float3 forwardDirection)
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

void Mesh::Apply()
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

void Mesh::Draw()
{
				if (vertexBuffer == nullptr)
								return;

				constant.world = DirectX::XMMatrixTranspose(
								DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)*
								DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x))*
								DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y))*
								DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z))*
								DirectX::XMMatrixTranslation(position.x, position.y, position.z));

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

void Plane::DrawSprite()
{
				if (vertexBuffer == nullptr)
								return;
				//���ꑽ��axis�����ƈӖ����Ȃ��Ǝv���A�A�A
				constant.world = DirectX::XMMatrixTranspose(
								DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)*
								DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x))*
								DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y))*
								DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z))*
								DirectX::XMMatrixTranslation(position.x, position.y, position.z));

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

void Cube::CreateCube(Texture::UvData uvData, int normal, Float2 size, bool souldClear)
{
				if (souldClear)
				{
								vertices.clear();
								indices.clear();
				}
				//�O
				CreatePlane(uvData.uv, uvData.numUv[0],//uv
								size, Float3(0.0f, 0.0f, -0.5f), false,
								Float3(1.0f, 0.0f, 0.0f) * normal,	//x ������-1�����Ă��Ɩ@�����t�]����
								Float3(0.0f, 1.0f, 0.0f),										//y
								Float3(0.0f, 0.0f, 1.0f));									//z

																																											//��
				CreatePlane(uvData.uv, uvData.numUv[1],
								size, Float3(0.0f, 0.0f, 0.5f), false,
								Float3(-1.0f, 0.0f, 0.0f)* normal,
								Float3(0.0f, 1.0f, 0.0f),
								Float3(0.0f, 0.0f, -1.0f));
				//��
				CreatePlane(uvData.uv, uvData.numUv[2],
								size, Float3(0.5f, 0.0f, 0.0f), false,
								Float3(0.0f, 0.0f, 1.0f)* normal,
								Float3(0.0f, 1.0f, 0.0f),
								Float3(-1.0f, 0.0f, 0.0f));
				//�E
				CreatePlane(uvData.uv, uvData.numUv[3],
								size, Float3(-0.5f, 0.0f, 0.0f), false,
								Float3(0.0f, 0.0f, -1.0f)* normal,
								Float3(0.0f, 1.0f, 0.0f),
								Float3(1.0f, 0.0f, 0.0f));
				//��
				CreatePlane(uvData.uv, uvData.numUv[4],
								size, Float3(0.0f, 0.5f, 0.0f), false,
								Float3(1.0f, 0.0f, 0.0f)* normal,
								Float3(0.0f, 0.0f, 1.0f),
								Float3(0.0f, -1.0f, 0.0f));
				//��
				CreatePlane(uvData.uv, uvData.numUv[5],
								size, Float3(0.0f, -0.5f, 0.0f), false,
								Float3(1.0f, 0.0f, 0.0f)* normal,
								Float3(0.0f, 0.0f, -1.0f),
								Float3(0.0f, 1.0f, 0.0f));
}

//����̃I�u�W�F�N�g���쐬����
void Tube::CreateTube(Texture::UvData uvData, bool souldClear,
				int heightRaito, int widthRaito,				//���x�ŕ������邩 90�̖񐔂ŗ���
				Float3 StartAngle,	//�J�n�p�w��p�̈���,�����Ȃǂ̍쐬���Ɏg�p����
				Float3 EndAngle)//�I���p�w��p�̈���,�����Ȃǂ̍쐬���Ɏg�p����
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

								//����
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

//���̂̐���
void Sphere::CreateSphere(Texture::UvData uvData, bool souldClear,
				int heightRaito, int widthRaito,//���x�ŕ������邩 ��90�̖񐔂łȂ��Ƃ�����ł��E�E
				Float3 StartAngle,	//�J�n�p�w��p�̈���,�����Ȃǂ̍쐬���Ɏg�p����
				Float3 EndAngle)			//�I���p�w��p�̈���,�����Ȃǂ̍쐬���Ɏg�p����
{
				if (souldClear)
				{
								vertices.clear();
								indices.clear();
				}

				float heightStart = StartAngle.y / heightRaito;
				float widthStart = StartAngle.x / widthRaito;

				//���񕪊����邩
				float heightSplit = EndAngle.y / heightRaito;
				float widthSplit = EndAngle.x / widthRaito;

				for (float n = heightSplit / 4 + heightStart / 2; n < (heightSplit * 3) / 4; n++)
				{
								for (float i = widthStart; i < widthSplit; i++)
								{
												CreateTriangle(
																Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))),
																				-(sin(DirectX::XMConvertToRadians((n)* heightRaito))),
																				sin(DirectX::XMConvertToRadians(i * widthRaito))* (cos(DirectX::XMConvertToRadians(n * heightRaito))))*0.5f,

																Float3(cos(DirectX::XMConvertToRadians(i * widthRaito)) * (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))),
																				-(sin(DirectX::XMConvertToRadians((n + 1) * heightRaito))),
																				sin(DirectX::XMConvertToRadians(i * widthRaito))* (cos(DirectX::XMConvertToRadians((n + 1) * heightRaito))))*0.5f,

																Float3(cos(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))),
																				-(sin(DirectX::XMConvertToRadians((n)* heightRaito))),
																				sin(DirectX::XMConvertToRadians((i + 1) * widthRaito)) * (cos(DirectX::XMConvertToRadians(n * heightRaito))))*0.5f,
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
