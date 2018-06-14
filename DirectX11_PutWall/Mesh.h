#pragma once

//物の生成の主に使うデータを作成　継承元になる
class Mesh
{
public:
				Float3 position = Float3(0.0f, 0.0f, 0.0f);//生成した物体の座標
				Float3 angles = Float3(0.0f, 0.0f, 0.0f);//生成した物体の角度
				Float3 scale = Float3(1.0f, 1.0f, 1.0f);//生成した物体の大きさを設定
				Float3 axis = Float3(0.0f, 0.0f, 0.0f);//基本座標からのずれを設定
				std::vector<Vertex> vertices;
				std::vector<int> indices;

				Mesh();
			
				~Mesh() {};

				Material& GetMaterial()
				{
								return material;
				}
				void Apply();
			
				void Draw();

protected:
				struct Constant
				{
								DirectX::XMMATRIX world;
				};

				Material material;
				Constant constant;
				ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
				ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;

				void CreateTriangle(Float3 vertices1, Float3 vertices2, Float3 vertices3,
								bool souldClear = true, Float2 uv = Float2(1.0f, 1.0f), Float2 numUV = Float2(0.0f, 0.0f));
				//板を生成している
				//uvが1.0以下になるとuvの始まる箇所がおかしくなるのでお勧めしない
				//numUVは指定した画像の左上座標をとるのでほしい場所 - 1 座標が合う
				void CreatePlane(Float2 uv = Float2(1.0f, 1.0f), Float2 numUv = Float2(0.0f, 0.0f),
								Float2 size = Float2(0.5f, 0.5f), Float3 offset = Float3(0.0f, 0.0f, 0.0f),
								bool souldClear = true,
								Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),
								Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),
								Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f));
};
class Plane : public Mesh
{
public:
				Plane()
				{
								uv = Float2(0.0f, 0.0f);
				};

				void SetTexture(Texture* tex, Float2 uv = Float2(0.0f, 0.0f))
				{
								material.SetTexture(0, tex);
								if (uv.x < 0.0f || uv.y < 0.0f)
								{
												return;
								}
								this->uv = uv;
				}

				//板の作成　ただあくまでテクスチャを設定した後に行う処理、間違えるべからず
				void Create(Float2 numUv = Float2(1.0f, 1.0f))
				{
								if (numUv.x > uv.x || numUv.y > uv.y)
								{
												//何もない場所を描画することになる
												//それを良しとするのであればここはコメントアウト
												//ただ非推奨である
												return;
								}
								CreatePlane(this->uv, numUv);
				}
				//2Dのテクスチャを描画するときに使用する
				void DrawSprite();

private:
				Float2 uv;
};

class Cube : public Mesh
{
public:
				Cube() 
				{
				};
				//テクスチャの描画する位置を変えるときはこの値を変更すること
				UvData uvData;

				void SetTexture(Texture* tex)
				{
								material.SetTexture(0, tex);
				}

				void Create(int normal,Float2 size = Float2(0.5f,0.5f))
				{
								CreateCube(normal);
								Apply();
				}
private:
				//normalには1か-1しか入れてはいけない
				void CreateCube(int normal = 1, Float2 size = Float2(0.5f, 0.5f), bool souldClear = true);
};


//今回は使わなかった
class Sphere : public Mesh
{
public:
				Sphere()
				{
				};
				UvData uvData;

				void SetTexture(Texture* tex)
				{
								material.SetTexture(0, tex);
				}

				void Create()
				{
								CreateSphere();
								Apply();
				}
private:
				void CreateSphere(bool souldClear = true, int heightRaito = 30, int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0), Float3 EndAngle = Float3(360, 360, 360));
};

class Tube : public Mesh
{
public:
				Tube()
				{
				};

				UvData uvData;

				void SetTexture(Texture* tex)
				{
								material.SetTexture(0, tex);
				}

				void Create(Texture* tex)
				{
								material.SetTexture(0, tex);
								CreateTube();
								Apply();
				}
private:
				void CreateTube(bool souldClear = true, int heightRaito = 30, int widthRaito = 30,
								Float3 StartAngle = Float3(0, 0, 0), Float3 EndAngle = Float3(360, 360, 360));
};
