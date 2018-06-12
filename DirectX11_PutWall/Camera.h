#pragma once
class Camera
{
public:
				Float3 position;
				Float3 angles;
				float clearColor[4];
				bool isDepthTest;
				//�ݒ肷�����
				Camera(bool dimensionFlag = true,Float4 color = Float4(1.0f, 1.0f, 1.0f, 1.0f));
				~Camera() {}

				void SetPerspective();
				void SetOrthographic(float size = 1.0f);

				void Update();
private:
				struct Constant
				{
								DirectX::XMMATRIX view;
								DirectX::XMMATRIX projection;
				};
				Constant constant;
				bool dimensionFlag;//���̃J�������񎟌����O�������ǂ����̃t���O
				float fieldOfView;//����
				float nearClip;//�N���b�s���O�ʂ̋ߖ�
				float farClip;//�N���b�s���O�ʂ̉���

				ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
				ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
				ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

				void Create();
};