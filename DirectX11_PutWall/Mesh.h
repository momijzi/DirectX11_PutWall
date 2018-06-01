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

				void CreateTriangle();				

				//�𐶐����Ă���
				//uv��1.0�ȉ��ɂȂ��uv�̎n�܂�ӏ������������Ȃ�̂ł����߂��Ȃ�
				//numUV�͎w�肵���摜�̍�����W���Ƃ�̂łق����ꏊ - 1 ���W������
				void CreatePlane(Float2 size = Float2(0.5f, 0.5f),
								Float3 offset = Float3(0.0f, 0.0f, 0.0f),	bool souldClear = true, 
								Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
								Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),
								Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f),
								Float2 uv = Float2(1.0f, 1.0f), Float2 numUV = Float2(0.0f, 0.0f));
				
				//normal�ɂ�1��-1��������Ă͂����Ȃ�
				void CreateCube(Texture* tex, int normal = 1,
								Float2 size = Float2(0.5f, 0.5f),bool souldClear = true);

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
