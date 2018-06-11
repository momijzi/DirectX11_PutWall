#pragma once
class Texture
{
public:
				Texture()
				{
								App::Initialize();
				}

				Texture(const wchar_t* const filePath)
				{
								App::Initialize();
								Load(filePath);
				}
				~Texture()
				{
				}

				#pragma region Uv
				struct UvData
				{
								Float2 uv;			//�摜������ 1���番����-1
								Float2 numUv[6];	//���������ꍇ�ǂ���`�悷�邩(�l�p�̏ꍇ��6�ʈႤ�e�N�X�`�����g���\�������邽�ߔz��6)

																									//������
								UvData()
								{
												uv = Float2(1.0f, 1.0f);
												for (int i = 0; i < 6; i++)
												{
																numUv[i] = Float2(0.0f, 0.0f);
												}
								}
				};
				//�����������������ĕ�����̑傫��������
				void SetDivide(Float2 uv);
				
				//6�ʑS���̃f�[�^��ύX����
				void SetNumUvAll(Float2 numUv);				

				//�O�ʂ�uv�ݒ�
				void SetNumUvFront(Float2 numUV)
				{
								uvData.numUv[0] = numUV;
				}
				//��ʂ�uv�ݒ�
				void SetNumUvBack(Float2 numUV)
				{
								uvData.numUv[1] = numUV;
				}
				//���ʂ�uv�ݒ�
				void SetNumUvLeft(Float2 numUV)
				{
								uvData.numUv[2] = numUV;
				}
				//�O�ʂ�uv�ݒ�
				void SetNumUvRight(Float2 numUV)
				{
								uvData.numUv[3] = numUV;
				}
				//��ʂ�uv�ݒ�
				void SetNumUvUp(Float2 numUV)
				{
								uvData.numUv[4] = numUV;
				}
				//���ʂ�uv�ݒ�
				void SetNumUvDown(Float2 numUV)
				{
								uvData.numUv[5] = numUV;
				}

				UvData GetUvData()
				{
								return uvData;
				}

				#pragma endregion
				//���[�h
				void Load(const wchar_t* const filePath);
				
				//�Z�b�g
				void Attach(int slot);

private:
				UvData uvData;

				ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
				ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
				ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};