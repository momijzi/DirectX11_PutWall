#pragma once
class Window
{
public:
				Window();
				~Window() {};
				HWND GetHandle() const
				{
								return handle;
				}
				DirectX::XMINT2 GetSize() const;
				void SetSize(int width, int height);

				bool Update();

private:
				HWND handle;

				//ウィンドウプロシージャ
				static LRESULT CALLBACK ProceedMessage(HWND window, UINT message, WPARAM w, LPARAM l)
				{
								if (message == WM_DESTROY)
								{
												PostQuitMessage(0);
								}
								return DefWindowProcW(window, message, w, l);
				}
};