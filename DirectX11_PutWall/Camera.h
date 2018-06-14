#pragma once
class Camera
{
public:
				Float3 position;
				Float3 angles;
				float clearColor[4];
				bool isDepthTest;
				//�ݒ肷�����
				Camera(Float4 color = Float4(1.0f, 1.0f, 1.0f, 1.0f));
				~Camera() {}

				//�J�����̎���̌��� 3D�Ȃ��̕���2D�Ȃ牺���g��
				void SetPerspective();
				void SetOrthographic(float size = 1.0f);
				//�J�����̑Ώۂ̌��� 
				void SetCameraInverse();//�J�������_�Ȃ̂ŃJ������angle���g�p
				void SetCameraLookAt(Float3 playerPos);//�J��������ΏۂɎ��_��������̂őΏۂ�position���K�v

				void Update(bool dimensionFlag = true,bool cameraView = false, Float3 playerPos = Float3(0.0f,0.0f,0.0f));
private:
				struct Constant
				{
								DirectX::XMMATRIX view;
								DirectX::XMMATRIX projection;
				};
				Constant constant;
				float fieldOfView;//����
				float nearClip;//�N���b�s���O�ʂ̋ߖ�
				float farClip;//�N���b�s���O�ʂ̉���

				ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
				ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
				ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

				void Create();
};