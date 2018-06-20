#include"App.hpp"

App::Window::Window()
{
				App::Initialize();

				HINSTANCE instance = GetModuleHandleW(nullptr);

				//={}で初期化
				WNDCLASSW windowClass = {};
				windowClass.lpfnWndProc = ProceedMessage;
				windowClass.hInstance = instance;
				//static_cast　C++でのキャスト
				windowClass.hCursor = static_cast<HCURSOR>(LoadImageW(nullptr,
								MAKEINTRESOURCEW(OCR_NORMAL),
								IMAGE_CURSOR, 0, 0,
								LR_DEFAULTSIZE | LR_SHARED));
				windowClass.lpszClassName = App::NAME;
				RegisterClassW(&windowClass);

				//ウィンドウ関係
				handle = CreateWindowW(App::NAME, App::NAME, WS_OVERLAPPEDWINDOW,
								0, 0, 0, 0, nullptr, nullptr, instance, nullptr);

				SetSize(960,640);

				ShowWindow(handle, SW_SHOWNORMAL);
}

DirectX::XMINT2 App::Window::GetSize() const
{
				RECT clientRect = {};
				GetClientRect(handle, &clientRect);

				return DirectX::XMINT2(
								clientRect.right - clientRect.left,
								clientRect.bottom - clientRect.top
				);
}

void App::Window::SetSize(int width, int height)
{
				RECT windowRect = {};
				RECT clientRect = {};
				GetWindowRect(handle, &windowRect);
				GetClientRect(handle, &clientRect);
				int w = (windowRect.right - windowRect.left) -
								(clientRect.right - clientRect.left) + width;
				int h = (windowRect.bottom - windowRect.top) -
								(clientRect.bottom - clientRect.top) + height;
				int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
				int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
				SetWindowPos(handle, nullptr, x, y, w, h, SWP_FRAMECHANGED);
}

bool App::Window::Update()
{
				MSG msg = {};

				while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
				{
								if (msg.message == WM_QUIT)
												return false;

								TranslateMessage(&msg);
								DispatchMessageW(&msg);
				}
				return true;
}