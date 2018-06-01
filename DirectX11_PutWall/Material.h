class Material
{
public:
				Material()
				{
								Initialize();
				}
				Material(const wchar_t* const filePath)
				{
								Initialize();
								Create(filePath);
				}
				~Material()
				{

				}
				void SetBuffer(int slot, void* cbuffer, size_t size);
				
				void SetTexture(int slot, Texture* texture)
				{
								textures[slot] = texture;
				}
				void Attach();
				
				void Create(const wchar_t* const filePath);
			
private:
				struct ConstantBuffer
				{
								void* ptr = nullptr;
								ATL::CComPtr<ID3D11Buffer> buffer = nullptr;
				};

				ConstantBuffer constantBuffer[10];
				Texture* textures[10];
				ATL::CComPtr<ID3D11VertexShader> vertexShader = nullptr;
				ATL::CComPtr<ID3D11PixelShader> pixelShader = nullptr;
				ATL::CComPtr<ID3D11InputLayout> inputLayout = nullptr;

				void Initialize();
	
				void CompileShader(const wchar_t* const filePath,
								const char* const entryPoint,const char* const shaderModel, ID3DBlob** out);
};