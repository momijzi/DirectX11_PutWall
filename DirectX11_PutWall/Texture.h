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
								Float2 uv;			//画像分割数 1から分割数-1
								Float2 numUv[6];	//分割した場合どこを描画するか(四角の場合に6面違うテクスチャを使う可能性があるため配列6)

																									//初期化
								UvData()
								{
												uv = Float2(1.0f, 1.0f);
												for (int i = 0; i < 6; i++)
												{
																numUv[i] = Float2(0.0f, 0.0f);
												}
								}
				};
				//引数分だけ分割して分割一つの大きさを入れる
				void SetDivide(Float2 uv);

				//6面全部のデータを変更する
				void SetNumUvAll(Float2 numUv);

				//前面のuv設定
				void SetNumUv(Float2 numUV,int num)
				{
								uvData.numUv[num] = numUV;
				}
				
				UvData GetUvData()
				{
								return uvData;
				}

				#pragma endregion
				//ロード
				void Load(const wchar_t* const filePath);

				//セット
				void Attach(int slot);

private:
				UvData uvData;

				ATL::CComPtr<ID3D11Texture2D> texture = nullptr;
				ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
				ATL::CComPtr<ID3D11SamplerState> samplerState = nullptr;
};