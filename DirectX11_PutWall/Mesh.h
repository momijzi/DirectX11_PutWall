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
								uv = Float2(0.0f, 0.0f);
				};

				void SetTexture(Texture* tex, Float2 uv = Float2(0.0f, 0.0f))
				{
								material.SetTexture(0, tex);
								if (uv.x < 0.0f || uv.y < 0.0f)
								{
												return;
								}
								this->uv = uv;
				}

				//�̍쐬�@���������܂Ńe�N�X�`����ݒ肵����ɍs�������A�ԈႦ��ׂ��炸
				void Create(Float2 numUv = Float2(1.0f, 1.0f))
				{
								if (numUv.x > uv.x || numUv.y > uv.y)
								{
												//�����Ȃ��ꏊ��`�悷�邱�ƂɂȂ�
												//�����ǂ��Ƃ���̂ł���΂����̓R�����g�A�E�g
												//�����񐄏��ł���
												return;
								}
								CreatePlane(this->uv, numUv);
				}
				//2D�̃e�N�X�`����`�悷��Ƃ��Ɏg�p����
				void DrawSprite();

private:
				Float2 uv;
};

class Cube : public Mesh
{
public:
				Cube() 
				{
				};
				//�e�N�X�`���̕`�悷��ʒu��ς���Ƃ��͂��̒l��ύX���邱��
				UvData uvData;

				void SetTexture(Texture* tex)
				{
								material.SetTexture(0, tex);
				}

				void Create(int normal,Float2 size = Float2(0.5f,0.5f))
				{
								CreateCube(normal);
								Apply();
				}
private:
				//normal�ɂ�1��-1��������Ă͂����Ȃ�
				void CreateCube(int normal = 1, Float2 size = Float2(0.5f, 0.5f), bool souldClear = true);
};


//����͎g��Ȃ�����
class Sphere : public Mesh
{
public:
				Sphere()
				{
				};
				UvData uvData;

				void SetTexture(Texture* tex)
				{
								material.SetTexture(0, tex);
				}

				void Create()
				{
								CreateSphere();
								Apply();
				}
private:
				void CreateSphere(bool souldClear = true, int heightRaito = 30, int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0), Float3 EndAngle = Float3(360, 360, 360));
};

class Tube : public Mesh
{
public:
				Tube()
				{
				};

				UvData uvData;

				void SetTexture(Texture* tex)
				{
								material.SetTexture(0, tex);
				}

				void Create(Texture* tex)
				{
								material.SetTexture(0, tex);
								CreateTube();
								Apply();
				}
private:
				void CreateTube(bool souldClear = true, int heightRaito = 30, int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0), Float3 EndAngle = Float3(360, 360, 360));
};
