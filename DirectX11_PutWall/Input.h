class Input
{
public:
				Input()
				{
								App::Initialize();
								Update();
				}
				~Input()
				{
				}
				bool GetKey(int keyCode) const
				{
								return keyState[keyCode] & 0x80;
				}
				bool GetKeyDown(int keyCode) const
				{
								return (keyState[keyCode] & 0x80) &&
												!(preKeyState[keyCode] & 0x80);
				}
				bool GetKeyUp(int keyCode) const
				{
								return !(keyState[keyCode] & 0x80) &&
												(preKeyState[keyCode] & 0x80);
				}
				Float2 GetMousePosition() const
				{
								return mousePosition;
				}
				void SetMousePosition(float x, float y);
			
				void SetShowCursor(bool isShowCursor);
	
				void Update();
			
private:
				Float2 mousePosition;
				BYTE preKeyState[256];
				BYTE keyState[256];
				bool isShowCursor = true;
};