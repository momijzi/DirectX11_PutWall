#pragma once
class Model
{
public:
				Float3 position;
				Float3 angles;
				Float3 scale;

				Model()
				{
								Initialize();
				}
				Model(const wchar_t* const filePath)
				{
								Initialize();
								Load(filePath);
				}
				~Model()
				{
				}
				void Load(const wchar_t* const filePath);
			
				//モデルの描画
				void Draw();
				
private:

				struct FbxManagerDeleter
				{
								void operator()(FbxManager* fbxManager) const
								{
												fbxManager->Destroy();
								}
				};
				struct FbxImporterDeleter
				{
								void operator()(FbxImporter* fbxImporter) const
								{
												fbxImporter->Destroy();
								}
				};
				struct FbxSceneDeleter
				{
								void operator()(FbxScene* fbxScene) const
								{
												fbxScene->Destroy();
								}
				};

				std::vector<std::unique_ptr<Mesh>> meshes;

				struct Constant
				{
								DirectX::XMMATRIX bone[200];
				};
				Constant constant;

				struct Animation
				{
								struct Frame
								{
												std::vector<DirectX::XMMATRIX> bones;
								};
								std::vector<Frame> frames;
				};
				std::vector<std::unique_ptr<Mesh>> meshs;
				std::vector<Animation> animations;
				int animIndex = 0;
				int frame = 0;
				//初期化
				void Initialize();
				
				void SearchNode(FbxScene* scene, FbxNode* node);
				
				void LoadNode(FbxScene* scene, FbxNode* node);
				
				bool IsOptimized(FbxMesh* mesh);
				
				void LoadMesh(FbxScene* scene, FbxMesh* mesh);
				
				//indexbuffer使用
				void LoadMeshWithControlPoint(FbxMesh* mesh, Mesh* item);
				
				//vertexbufferの使用
				void LoadMeshWithPolygonVertex(FbxMesh* mesh, Mesh* item);

				//アニメーションのロード
				void LoadAnim(FbxScene* scene, FbxMesh* mesh, Mesh* item, bool isOptimized);
				
				//スキンのロード
				void LoadSkin(FbxMesh* mesh, FbxAnimStack* animStack, int animNum, Mesh* item,
								std::vector<std::vector<int>>& controlPointIndices, bool isOptimized);
				//骨組みのロード
				void LoadBones(FbxMesh* mesh, int animNum, int skinNum, int frameCount, Mesh* item, FbxTime start,
								std::vector<std::vector<int>>& controlPointIndices, bool isOptimized);
				//
				void AddBlendInControlPoint(int blendIndex, FbxCluster* cluster, Mesh* item);
			
				//
				void AddBlendInPolygonVertex(int blendIndex, FbxCluster* cluster, Mesh* item,
								std::vector<std::vector<int>>& controlPointIndices);

				//
				void SearchBlendIndex(int vertexIndex, int blendIndex, float blendWeight, Mesh* item);
			
				//FbxMatrixをXMMATRIXに変換する関数
				static DirectX::XMMATRIX FbxMatrixToXMMatrix(FbxMatrix source);
};