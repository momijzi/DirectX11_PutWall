class Texture
{
public:
				Float2 uv;			//�摜������ 1���番����-1
				Float2 numUV[6];	//���������ꍇ�ǂ���`�悷�邩(�l�p)

				Texture()
				{
								App::Initialize();
				}

				Texture(const wchar_t* const filePath);
			
				~Texture()
				{
				}

				//�����������������ĕ�����̑傫��������
				void SetDivide(Float2 uv);

				//�O�ʓ���uv�ɐݒ�
				void SetAll(Float2 numUV);
				
				//��Ɖ��ȊO��uv�ݒ�
				void SetAround(Float2 numUV);
				
				//�O�ʂ�uv�ݒ�
				void SetFront(Float2 numUV);

				//��ʂ�uv�ݒ�
				void SetBack(Float2 numUV);

				//���ʂ�uv�ݒ�
				void SetLeft(Float2 numUV);

				//�O�ʂ�uv�ݒ�
				void SetRight(Float2 numUV);

				//��ʂ�uv�ݒ�
				void SetUp(Float2 numUV);
			
				//���ʂ�uv�ݒ�
				void SetDown(Float2 numUV);
				
				//���[�h
				void Load(const wchar_t* const filePath);
				
				//�Z�b�g
				void Attach(int slot);

private:
				ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
				ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
				ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};