#include"App.hpp"

Mesh::Mesh()
{
				App::Initialize();

				position = Float3(0.0f, 0.0f, 0.0f);
				angles = Float3(0.0f, 0.0f, 0.0f);
				scale = Float3(1.0f, 1.0f, 1.0f);

				material = Material(L"Shader.hlsl");
}

void Mesh::CreateData(Texture* tex, CreateMode mode)
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
												CreateCube(tex, -1);
												break;
								case CUBEOUT:
												CreateCube(tex, 1);
												break;
								default:
												return;
												break;
				}
				Apply();
}

void Mesh::CreateTriangle()
{
				vertices.clear();

				vertices.push_back({ Float3(0.0f, 1.0f, 0.0f),Float3(1.0f,0.0f,0.0f),Float2(0.5f,0.0f) });
				vertices.push_back({ Float3(1.0f,-1.0f, 0.0f),Float3(0.0f,1.0f,0.0f),Float2(1.0f,1.0f) });
				vertices.push_back({ Float3(-1.0f,-1.0f,0.0f),Float3(0.0f,0.0f,1.0f),Float2(0.0f,1.0f) });
}

void Mesh::CreatePlane(
				Float2 size , Float3 offset,bool souldClear,Float3 leftDirection,
				Float3 upDirection,Float3 forwardDirection,
				Float2 uv,Float2 numUV)
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

void Mesh::CreateCube(Texture* tex, int normal, Float2 size, bool souldClear)
{
				if (souldClear)
				{
								vertices.clear();
								indices.clear();
				}
				//‘O
				CreatePlane(size, Float3(0.0f, 0.0f, -0.5f), false,
								Float3(1.0f, 0.0f, 0.0f) * normal,		//x ‚±‚±‚É-1‚©‚¯‚Ä‚â‚é‚Æ–@ü‚ª‹t“]‚·‚é
								Float3(0.0f, 1.0f, 0.0f),											//y
								Float3(0.0f, 0.0f, 1.0f),											//z
								tex->uv, tex->numUV[0]);												//uv
																																												//Œã
				CreatePlane(size, Float3(0.0f, 0.0f, 0.5f), false,
								Float3(-1.0f, 0.0f, 0.0f)* normal,
								Float3(0.0f, 1.0f, 0.0f),
								Float3(0.0f, 0.0f, -1.0f),
								tex->uv, tex->numUV[1]);
				//¶
				CreatePlane(size, Float3(0.5f, 0.0f, 0.0f), false,
								Float3(0.0f, 0.0f, 1.0f)* normal,
								Float3(0.0f, 1.0f, 0.0f),
								Float3(-1.0f, 0.0f, 0.0f),
								tex->uv, tex->numUV[2]);
				//‰E
				CreatePlane(size, Float3(-0.5f, 0.0f, 0.0f), false,
								Float3(0.0f, 0.0f, -1.0f)* normal,
								Float3(0.0f, 1.0f, 0.0f),
								Float3(1.0f, 0.0f, 0.0f),
								tex->uv, tex->numUV[3]);
				//ã
				CreatePlane(size, Float3(0.0f, 0.5f, 0.0f), false,
								Float3(1.0f, 0.0f, 0.0f)* normal,
								Float3(0.0f, 0.0f, 1.0f),
								Float3(0.0f, -1.0f, 0.0f),
								tex->uv, tex->numUV[4]);
				//‰º
				CreatePlane(size, Float3(0.0f, -0.5f, 0.0f), false,
								Float3(1.0f, 0.0f, 0.0f)* normal,
								Float3(0.0f, 0.0f, -1.0f),
								Float3(0.0f, 1.0f, 0.0f),
								tex->uv, tex->numUV[5]);
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
