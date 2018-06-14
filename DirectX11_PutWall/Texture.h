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

				//���[�h
				void Load(const wchar_t* const filePath);
				
				//�Z�b�g
				void Attach(int slot);

private:

				ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
				ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
				ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};