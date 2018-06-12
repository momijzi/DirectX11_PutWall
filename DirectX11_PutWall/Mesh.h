#pragma once

//���̐����̎�Ɏg���f�[�^���쐬�@�p�����ɂȂ�
class Mesh
{
public:
				Float3 position = Float3(0.0f, 0.0f, 0.0f);//�����������̂̍��W
				Float3 angles = Float3(0.0f, 0.0f, 0.0f);//�����������̂̊p�x
				Float3 scale = Float3(1.0f, 1.0f, 1.0f);//�����������̂̑傫����ݒ�
				Float3 axis = Float3(0.0f, 0.0f, 0.0f);//��{���W����̂����ݒ�
				std::vector<Vertex> vertices;
				std::vector<int> indices;

				Mesh();
			
				~Mesh() {};

				Material& GetMaterial()
				{
								return material;
				}
				void Apply();
			
				void Draw();

protected:
				struct Constant
				{
								DirectX::XMMATRIX world;
				};

				Material material;
				Constant constant;
				ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
				ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;

				void CreateTriangle(Float3 vertices1, Float3 vertices2, Float3 vertices3,
								bool souldClear = true, Float2 uv = Float2(1.0f, 1.0f), Float2 numUV = Float2(0.0f, 0.0f));
				//�𐶐����Ă���
				//uv��1.0�ȉ��ɂȂ��uv�̎n�܂�ӏ������������Ȃ�̂ł����߂��Ȃ�
				//numUV�͎w�肵���摜�̍�����W���Ƃ�̂łق����ꏊ - 1 ���W������
				void CreatePlane(Float2 uv = Float2(1.0f, 1.0f), Float2 numUv = Float2(0.0f, 0.0f),
								Float2 size = Float2(0.5f, 0.5f), Float3 offset = Float3(0.0f, 0.0f, 0.0f),
								bool souldClear = true,
								Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
								Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),
								Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f));
};
class Plane : public Mesh
{
public:
				Plane()
				{
				};
				void Create(Texture* tex)
				{
								material.SetTexture(0, tex);
								CreatePlane(tex->GetUvData().uv,tex->GetUvData().numUv[0]);
								Apply();
				}
				//2D�̃e�N�X�`����`�悷��Ƃ��Ɏg�p����
				void DrawSprite();
};

class Cube : public Mesh
{
public:
				Cube() 
				{
				};
				void Create(Texture* tex, int normal,Float2 size = Float2(0.5f,0.5f))
				{
								material.SetTexture(0, tex);
								CreateCube(tex->GetUvData(), normal);
								Apply();
				}
private:
				//normal�ɂ�1��-1��������Ă͂����Ȃ�
				void CreateCube(Texture::UvData uvData, int normal = 1, Float2 size = Float2(0.5f, 0.5f), bool souldClear = true);
};

class Sphere : public Mesh
{
public:
				Sphere()
				{
				};

				void Create(Texture* tex)
				{
								material.SetTexture(0, tex);
								CreateSphere(tex->GetUvData());
								Apply();
				}
private:
				void CreateSphere(Texture::UvData uvData, bool souldClear = true, int heightRaito = 30, int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0), Float3 EndAngle = Float3(360, 360, 360));
};

class Tube : public Mesh
{
public:
				Tube()
				{
				};
				void Create(Texture* tex)
				{
								material.SetTexture(0, tex);
								CreateTube(tex->GetUvData());
								Apply();
				}
private:
				void CreateTube(Texture::UvData uvData, bool souldClear = true, int heightRaito = 30, int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0), Float3 EndAngle = Float3(360, 360, 360));
};
