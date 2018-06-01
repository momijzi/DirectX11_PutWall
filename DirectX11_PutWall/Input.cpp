#include"App.hpp"

void App::Input::SetMousePosition(float x, float y)
{
				if (GetActiveWindow() != App::GetWindowHandle())
								return;

				POINT point = {};
				point.x = static_cast<int>(x) + App::GetWindowSize().x / 2;
				point.y = static_cast<int>(-y) + App::GetWindowSize().y / 2;
				ClientToScreen(App::GetWindowHandle(), &point);
				SetCursorPos(point.x, point.y);

				mousePosition.x = x;
				mousePosition.y = y;
}

void App::Input::SetShowCursor(bool isShowCursor)
{
				if (this->isShowCursor == isShowCursor)
								return;

				this->isShowCursor = isShowCursor;
				ShowCursor(isShowCursor);
}

void App::Input::Update()
{
				POINT point = {};
				GetCursorPos(&point);
				ScreenToClient(App::GetWindowHandle(), &point);

				mousePosition.x = static_cast<float>(
								point.x - App::GetWindowSize().x / 2);
				mousePosition.y = static_cast<float>(
								-point.y + App::GetWindowSize().y / 2);

				for (int i = 0; i < 256; i++)
				{
								preKeyState[i] = keyState[i];
				}
				GetKeyboardState(keyState);
}

