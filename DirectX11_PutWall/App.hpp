#pragma once

#define OEMRESOURCE//無いとエラーが出る

#include <memory>
#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wincodec.h>
#include<fbxsdk.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

//初期設定の自動化
#if defined(_WIN64)
#define A "x64/"
#else
#define A "x86/"
#endif

#if defined(_DEBUG)
#define B "debug/"
#else
#define B "release/"
#endif

#if defined(_DLL)
#define C "libfbxsdk-md.lib"
#else
#define C "libfbxsdk-mt.lib"
#endif

#pragma comment(lib,A B C)
#undef A
#undef B
#undef C
//-------------------------------------
#define MAIN() APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)

#include "Utility.hpp"

class App final
{
public:
				#include "Window.h"
				#include "Graphics.h"
				#include "Input.h"
				#include "Timer.h"
				static constexpr wchar_t* NAME = L"DirectX11";

				App() = delete;
				static bool Refresh()
				{
								GetGraphics().Update();
								GetInput().Update();
								GetTimer().Update();
								return GetWindow().Update();
				}
				static void Initialize()
				{
								static bool isInitialized = false;

								if (!isInitialized)
								{
												CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
												_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
												isInitialized = true;
								}
				}
				//ウィンドウ関係
				static HWND GetWindowHandle()
				{
								return GetWindow().GetHandle();
				}
				static DirectX::XMINT2 GetWindowSize()
				{
								return GetWindow().GetSize();
				}
				static void SetWindowSize(int width, int height)
				{
								GetWindow().SetSize(width, height);
				}
				//グラフィック関係
				static ID3D11Device& GetGraohicsDevice()
				{
								return GetGraphics().GetDevice();
				}
				static ID3D11DeviceContext& GetGraphicsContext()
				{
								return GetGraphics().GetContext();
				}
				static IDXGISwapChain& GetSwapChain()
				{
								return GetGraphics().GetSwapChain();
				}
				//入力関係
				static bool GetKey(int VK_CODE)
				{
								return GetInput().GetKey(VK_CODE);
				}
				static bool GetKeyDown(int VK_CODE)
				{
								return GetInput().GetKeyDown(VK_CODE);
				}
				static bool GetKeyUp(int VK_CODE)
				{
								return GetInput().GetKeyUp(VK_CODE);
				}
				static Float2 GetMousePosition()
				{
								return GetInput().GetMousePosition();
				}
				static void SetMousePosition(float x, float y)
				{
								GetInput().SetMousePosition(x, y);
				}
				static void SetShowCursor(bool isShowCursor)
				{
								GetInput().SetShowCursor(isShowCursor);
				}
				//タイム関係
				static float GetTime()
				{
								return GetTimer().GetTime();
				}
				static float GetDeltaTime()
				{
								return GetTimer().GetDeltaTime();
				}
				static int GetFrameRate()
				{
								return GetTimer().GetFrameRate();
				}
private:
				static Window& GetWindow()
				{
								static std::unique_ptr<Window> window(new Window());
								return *window.get();
				}
				static Graphics& GetGraphics()
				{
								static std::unique_ptr<Graphics> graphics(new Graphics());
								return *graphics.get();
				}
				static Input& GetInput()
				{
								static std::unique_ptr<Input> input(new Input());
								return *input.get();
				}
				static Timer& GetTimer()
				{
								static std::unique_ptr<Timer> timer(new Timer());
								return *timer.get();
				}
};

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "Model.h"
#include "Move.hpp"
#include "Wall.h"
#include "Player.h"