#include"App.hpp"

void Model::Load(const wchar_t* const filePath)
{
				std::unique_ptr<FbxManager, FbxManagerDeleter> manager(FbxManager::Create());

				size_t length = wcslen(filePath) + 1;//wcslenはstringの文字数を返す関数　+１はなんか必須ぽい
				std::unique_ptr<char[]> charFilePath(new char[length]);
				wcstombs_s// ワイド文字のシーケンスを対応するマルチバイト文字のシーケンスに変換します
				(
								nullptr,//変換された文字数
								charFilePath.get(),//変換された文字列が生成されるバッファのアドレス
								length,//サイズのバイト数
								filePath,//変換さえれるワイド文字の文字列へのポインタ
								_TRUNCATE//使用するロケール
				);
				std::unique_ptr<FbxImporter, FbxImporterDeleter> importer
				(
								FbxImporter::Create
								(
												manager.get(),	//格納したい場所のアドレスをセット
												""				//格納したいchar型のデータ
								)
				);
				importer->Initialize
				(
								charFilePath.get(),	//アクセスするファイル
								-1,					//フォーマット　初期化ぽい　とりあえず-１で良きかな
								manager->GetIOSettings()//クライアントIOSettingsが指定されていない場合、デフォルトのIOSettingsが作成されます
				);
				std::unique_ptr<FbxScene, FbxSceneDeleter> scene(FbxScene::Create(manager.get(), ""));
				importer->Import(scene.get());

				FbxGeometryConverter converter(manager.get());
				converter.Triangulate
				(
								scene.get(),//メッシュを三角分割するために反復するシーン
								true		//うーんよくわからん　true以外がわかんね
				);

				SearchNode(scene.get(), scene->GetRootNode());
}

void Model::Draw()
{
				frame++;
				frame %= animations[animIndex].frames.size();

				for (int i = 0; i < animations[animIndex].frames[frame].bones.size(); i++)
				{
								constant.bone[i] = DirectX::XMMatrixTranspose(
												animations[animIndex].frames[frame].bones[i]
								);
				}

				for (int i = 0; i < meshes.size(); i++)
				{
								meshes[i]->position = position;
								meshes[i]->angles = angles;
								meshes[i]->scale = scale;

								meshes[i]->Draw();
				}
}

void Model::Initialize()
{
				App::Initialize();

				position = Float3(0.0f, 0.0f, 0.0f);
				angles = Float3(0.0f, 0.0f, 0.0f);
				scale = Float3(1.0f, 1.0f, 1.0f);
}

void Model::SearchNode(FbxScene* scene, FbxNode* node)
{
				LoadNode(scene, node);

				for (int i = 0; i < node->GetChildCount(); i++)
				{
								SearchNode(scene, node->GetChild(i));
				}
}

void Model::LoadNode(FbxScene* scene, FbxNode* node)
{
				if (!node->GetVisibility())
								return;

				FbxNodeAttribute* attribute = node->GetNodeAttribute();

				if (attribute == nullptr)
								return;

				if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
				{
								LoadMesh(scene, node->GetMesh());
				}
}

bool Model::IsOptimized(FbxMesh* mesh)
{
				//indexとvertexのどちらを使えば効率がいいか最適化
				if (mesh->GetElementNormal() != nullptr)
				{
								if (mesh->GetElementNormal()->GetMappingMode() !=
												FbxLayerElement::EMappingMode::eByControlPoint)
												return false;
				}
				if (mesh->GetElementUV() != nullptr)
				{
								if (mesh->GetElementUV()->GetMappingMode() !=
												FbxLayerElement::EMappingMode::eByControlPoint)
												return false;
				}
				return true;
}

void Model::LoadMesh(FbxScene* scene, FbxMesh* mesh)
{
				std::unique_ptr<Mesh> item(new Mesh());
				item->vertices.clear();
				item->indices.clear();
				bool isOptimized = IsOptimized(mesh);

				if (isOptimized)
								LoadMeshWithControlPoint(mesh, item.get());
				else
								LoadMeshWithPolygonVertex(mesh, item.get());

				//アニメーション追加----------------------------------------------
				LoadAnim(scene, mesh, item.get(), isOptimized);

				//----------------------------------------------------------------
				item->Apply();
				item->GetMaterial().Create(L"Shader2.hlsl");
				item->GetMaterial().SetBuffer(2, &constant, sizeof(Constant));
				meshes.push_back(std::move(item));//ここでポインタのデータをメッシュに渡す
}

void Model::LoadMeshWithControlPoint(FbxMesh* mesh, Mesh* item)
{
				item->vertices.resize(mesh->GetControlPointsCount());

				for (int i = 0; i < mesh->GetControlPointsCount(); i++)
				{
								FbxVector4 position = mesh->GetControlPointAt(i);
								item->vertices[i].position = Float3(position[0], position[1], position[2]);

								if (mesh->GetElementNormal() != nullptr)
								{
												FbxVector4 normal = mesh->GetElementNormal()->GetDirectArray().GetAt(i);
												item->vertices[i].normal = Float3(normal.mData[0], normal.mData[1], normal.mData[2]);
								}
								if (mesh->GetElementUV() != nullptr)
								{
												FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(i);
												item->vertices[i].uv = Float2(uv.mData[0], 1.0 - uv.mData[1]);
								}
				}

				item->indices.resize(mesh->GetPolygonVertexCount());

				for (int i = 0; i < mesh->GetPolygonVertexCount(); i++)
				{
								item->indices[i] = mesh->GetPolygonVertices()[i];
				}
}

void Model::LoadMeshWithPolygonVertex(FbxMesh* mesh, Mesh* item)
{
				for (int i = 0; i < mesh->GetPolygonCount(); i++)
				{
								for (int j = 0; j < mesh->GetPolygonSize(i); j++)
								{
												Vertex vertex;

												int v = mesh->GetPolygonVertex(i, j);
												FbxVector4 position = mesh->GetControlPoints()[v];
												vertex.position = Float3(position.mData[0], position.mData[1], position.mData[2]);

												FbxVector4 normal;
												mesh->GetPolygonVertexNormal(i, j, normal);
												vertex.normal = Float3(normal.mData[0], normal.mData[1], normal.mData[2]);

												FbxStringList uvNameList;
												mesh->GetUVSetNames(uvNameList);
												FbxVector2 uv;
												bool isMapped;
												if (mesh->GetElementUVCount() > 0)
																mesh->GetPolygonVertexUV(i, j, uvNameList[0], uv, isMapped);

												vertex.uv = Float2(uv.mData[0], 1.0f - uv.mData[1]);

												item->vertices.push_back(vertex);
								}
				}
}

void Model::LoadAnim(FbxScene* scene, FbxMesh* mesh, Mesh* item, bool isOptimized)
{
				std::vector<std::vector<int>> controlPointIndices;
				if (!isOptimized)
				{
								controlPointIndices.resize(mesh->GetControlPointsCount());
								for (int i = 0; i < mesh->GetPolygonVertexCount(); i++)
								{
												controlPointIndices[mesh->GetPolygonVertices()[i]].push_back(i);
								}
				}

				FbxArray<FbxString*> animStackList;
				scene->FillAnimStackNameArray(animStackList);
				printf("anim %d\n", animStackList.Size());

				animations.resize(animStackList.Size());

				for (int i = 0; i < animStackList.Size(); i++)
				{
								FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(
												animStackList[i]->Buffer()
												);
								scene->SetCurrentAnimationStack(animStack);

								LoadSkin(mesh, animStack, i, item, controlPointIndices, isOptimized);
				}
}

void Model::LoadSkin(FbxMesh* mesh, FbxAnimStack* animStack, int animNum, Mesh* item,
				std::vector<std::vector<int>>& controlPointIndices, bool isOptimized)
{
				FbxTime start = animStack->GetLocalTimeSpan().GetStart();
				FbxTime stop = animStack->GetLocalTimeSpan().GetStop();

				float length = (stop.GetMilliSeconds() - start.GetMilliSeconds()) / 1000.0f;
				int frameCount = (int)(length*60.0f);
				animations[animNum].frames.resize(frameCount);

				printf("deformer %d\n", mesh->GetDeformerCount(FbxDeformer::eSkin));

				for (int i = 0; i < mesh->GetDeformerCount(FbxDeformer::eSkin); i++)
				{
								LoadBones(mesh, animNum, i, frameCount, item, start, controlPointIndices, isOptimized);
				}
}

void Model::LoadBones(FbxMesh* mesh, int animNum, int skinNum, int frameCount, Mesh* item, FbxTime start,
				std::vector<std::vector<int>>& controlPointIndices, bool isOptimized)
{
				FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(
								skinNum, FbxDeformer::eSkin
				);
				FbxTime time;
				for (int i = 0; i < frameCount; i++)
				{
								time.SetMilliSeconds((i / 60.0f * 1000.0f) + start.GetMilliSeconds());

								FbxMatrix globalPosition = mesh->GetNode()->
												EvaluateGlobalTransform(time);
								FbxVector4 translation = mesh->GetNode()->
												GetGeometricTranslation(FbxNode::eSourcePivot);
								FbxVector4 rotation = mesh->GetNode()->
												GetGeometricRotation(FbxNode::eSourcePivot);
								FbxVector4 scaling = mesh->GetNode()->
												GetGeometricScaling(FbxNode::eSourcePivot);
								FbxAMatrix geometryOffset = FbxAMatrix(
												translation, rotation, scaling);

								animations[animNum].frames[i].bones.resize(
												skin->GetClusterCount()
								);

								for (int j = 0; j < skin->GetClusterCount(); j++)
								{
												FbxCluster* cluster = skin->GetCluster(j);

												FbxMatrix vertexTransformMatrix;
												FbxAMatrix referenceGlobalInitPosition;
												FbxAMatrix clusterGlobalInitPosition;
												FbxMatrix clusterGlobalCurrentPosition;
												FbxMatrix clusterRelativeInitPosition;
												FbxMatrix clusterRelativeCurrentPositionInverse;

												cluster->GetTransformMatrix(referenceGlobalInitPosition);

												referenceGlobalInitPosition *= geometryOffset;

												cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

												clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(time);

												clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() *referenceGlobalInitPosition;

												clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;

												vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

												animations[animNum].frames[i].bones[j] = FbxMatrixToXMMatrix(vertexTransformMatrix);

												if (isOptimized)
																AddBlendInControlPoint(j, cluster, item);
												else
																AddBlendInPolygonVertex(j, cluster, item, controlPointIndices);
								}
				}
}

void Model::AddBlendInControlPoint(int blendIndex, FbxCluster* cluster, Mesh* item)
{
				for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++)
				{
								int index = cluster->GetControlPointIndices()[i];
								float weight = (float)cluster->GetControlPointWeights()[i];

								SearchBlendIndex(index, blendIndex, weight, item);
				}
}

void Model::AddBlendInPolygonVertex(int blendIndex, FbxCluster* cluster, Mesh* item,
				std::vector<std::vector<int>>& controlPointIndices)
{
				for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++)
				{
								int index = cluster->GetControlPointIndices()[i];
								float weight = (float)cluster->GetControlPointWeights()[i];

								for (int v = 0; v < controlPointIndices[index].size(); v++)
								{
												SearchBlendIndex(controlPointIndices[index][v], blendIndex, weight, item);
								}
				}
}

void Model::SearchBlendIndex(int vertexIndex, int blendIndex, float blendWeight, Mesh* item)
{
				for (int i = 0; i < 8; i++)
				{
								if (item->vertices[vertexIndex].blendIndices[i] == blendIndex)
								{
												return;
								}
								if (item->vertices[vertexIndex].blendIndices[i] < 999)
								{
												continue;
								}
								item->vertices[vertexIndex].blendIndices[i] = blendIndex;
								item->vertices[vertexIndex].blendWeights[i] = blendWeight;
								return;
				}
}

DirectX::XMMATRIX Model::FbxMatrixToXMMatrix(FbxMatrix source)
{
				DirectX::XMMATRIX dest;
				for (int x = 0; x < 4; x++)
				{
								for (int y = 0; y < 4; y++)
								{
												dest.r[x].m128_f32[y] = (float)source.mData[x][y];
								}
				}
				return dest;
}
