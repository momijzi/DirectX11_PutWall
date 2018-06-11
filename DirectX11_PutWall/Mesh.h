#pragma once
class Mesh
{
public:
				//CUBE��IN�͓����@���@OUT�͊O���@��
				enum CreateMode{NON,TRIANGLE,PLANE,CUBEIN,CUBEOUT,SPHERE};
				Float3 position;
				Float3 angles;
				Float3 scale;
				std::vector<Vertex> vertices;
				std::vector<int> indices;

				Mesh();
			
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
				void CreateData(Texture* tex, CreateMode mode);

				void CreateTriangle(Float3 vertices1, Float3 vertices2, Float3 vertices3,
								bool souldClear = true, Float2 uv = Float2(1.0f, 1.0f), Float2 numUV = Float2(0.0f, 0.0f));


				//�𐶐����Ă���
				//uv��1.0�ȉ��ɂȂ��uv�̎n�܂�ӏ������������Ȃ�̂ł����߂��Ȃ�
				//numUV�͎w�肵���摜�̍�����W���Ƃ�̂łق����ꏊ - 1 ���W������
				void CreatePlane(Float2 size = Float2(0.5f, 0.5f),
								Float3 offset = Float3(0.0f, 0.0f, 0.0f),	bool souldClear = true, 
								Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
								Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),
								Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f),
								Float2 uv = Float2(1.0f, 1.0f), Float2 numUv = Float2(0.0f, 0.0f));
				
				//normal�ɂ�1��-1��������Ă͂����Ȃ�
				void CreateCube(Texture::UvData uvData, int normal = 1, Float2 size = Float2(0.5f, 0.5f), bool souldClear = true);

				void CreateSphere(Texture::UvData uvData, bool souldClear = true,int heightRaito = 30, int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0),Float3 EndAngle = Float3(360, 360, 360));

				void CreateTube(Texture::UvData uvData, bool souldClear = true,	int heightRaito = 30, int widthRaito = 30,		
								Float3 StartAngle = Float3(0, 0, 0),	Float3 EndAngle = Float3(360, 360, 360));


				Material& GetMaterial()
				{
								return material;
				}
				void Apply();
			
				void Draw();

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
