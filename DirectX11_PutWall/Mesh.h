#pragma once
class Mesh
{
public:
				//CUBEのINは内側法線　OUTは外側法線
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

				//現状の問題
				//生成する物体を変えたいがテクスチャを変えた時UV比率が違ったらだめなのでどうするか
				//方法１．テクスチャを変える際事前に現在描画しているデータを持っておく
				//そしてテクスチャを適応させたデータをもう一度作成する
				//てかこれしかない気が。。。。

				//データ作成　テクスチャのUVはここまでに指定しておく
				//テクスチャを変えるときは再度この関数を呼ぶ必要がある
				void CreateData(Texture* tex, CreateMode mode);

				void CreateTriangle(Float3 vertices1, Float3 vertices2, Float3 vertices3,
								bool souldClear = true, Float2 uv = Float2(1.0f, 1.0f), Float2 numUV = Float2(0.0f, 0.0f));


				//板を生成している
				//uvが1.0以下になるとuvの始まる箇所がおかしくなるのでお勧めしない
				//numUVは指定した画像の左上座標をとるのでほしい場所 - 1 座標が合う
				void CreatePlane(Float2 size = Float2(0.5f, 0.5f),
								Float3 offset = Float3(0.0f, 0.0f, 0.0f),	bool souldClear = true, 
								Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
								Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),
								Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f),
								Float2 uv = Float2(1.0f, 1.0f), Float2 numUv = Float2(0.0f, 0.0f));
				
				//normalには1か-1しか入れてはいけない
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
