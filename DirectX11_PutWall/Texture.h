class Texture
{
public:
				Float2 uv;			//画像分割数 1から分割数-1
				Float2 numUV[6];	//分割した場合どこを描画するか(四角)

				Texture()
				{
								App::Initialize();
				}

				Texture(const wchar_t* const filePath);
			
				~Texture()
				{
				}

				//引数分だけ分割して分割一つの大きさを入れる
				void SetDivide(Float2 uv);

				//前面同じuvに設定
				void SetAll(Float2 numUV);
				
				//上と下以外のuv設定
				void SetAround(Float2 numUV);
				
				//前面のuv設定
				void SetFront(Float2 numUV);

				//後面のuv設定
				void SetBack(Float2 numUV);

				//左面のuv設定
				void SetLeft(Float2 numUV);

				//前面のuv設定
				void SetRight(Float2 numUV);

				//上面のuv設定
				void SetUp(Float2 numUV);
			
				//下面のuv設定
				void SetDown(Float2 numUV);
				
				//ロード
				void Load(const wchar_t* const filePath);
				
				//セット
				void Attach(int slot);

private:
				ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
				ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
				ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};