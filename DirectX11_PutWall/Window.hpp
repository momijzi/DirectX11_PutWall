class Window
{
public:
				Window()
				{
								App::Initialize();

								HINSTANCE instance = GetModuleHandleW(nullptr);

								//={}�ŏ�����
								WNDCLASSW windowClass = {};
								windowClass.lpfnWndProc = ProceedMessage;
								windowClass.hInstance = instance;
								//static_cast�@C++�ł̃L���X�g
								windowClass.hCursor = static_cast<HCURSOR>(LoadImageW(nullptr,
												MAKEINTRESOURCEW(OCR_NORMAL),
												IMAGE_CURSOR, 0, 0,
												LR_DEFAULTSIZE | LR_SHARED));
								windowClass.lpszClassName = App::NAME;
								RegisterClassW(&windowClass);

								//�E�B���h�E�֌W
								handle = CreateWindowW(App::NAME, App::NAME, WS_OVERLAPPEDWINDOW,
												0, 0, 0, 0, nullptr, nullptr, instance, nullptr);

								SetSize(640, 480);

								ShowWindow(handle, SW_SHOWNORMAL);
				}
				~Window()
				{
				}
				HWND GetHandle() const
				{
								return handle;
				}
				DirectX::XMINT2 GetSize() const
				{
								RECT clientRect = {};
								GetClientRect(handle, &clientRect);

								return DirectX::XMINT2(
												clientRect.right - clientRect.left,
												clientRect.bottom - clientRect.top
								);
				}
				void SetSize(int width, int height)
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
				bool Update()
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

private:
				HWND handle;

				//�E�B���h�E�v���V�[�W��
				static LRESULT CALLBACK ProceedMessage
				(HWND window, UINT message, WPARAM w, LPARAM l)
				{
								if (message == WM_DESTROY)
								{
												PostQuitMessage(0);
								}
								return DefWindowProcW(window, message, w, l);
				}
};