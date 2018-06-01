class Graphics
{
public:
				Graphics();
				~Graphics()
				{
				}
				ID3D11Device& GetDevice()const
				{
								return *device;
				}
				ID3D11DeviceContext& GetContext() const
				{
								return *context;
				}
				IDXGISwapChain& GetSwapChain() const
				{
								return *swapChain;
				}
				void Update()
				{
								swapChain->Present(1, 0);
				}

private:
				struct Constant
				{
								DirectX::XMMATRIX view;
								DirectX::XMMATRIX projection;
				};
				Constant constant;

				ATL::CComPtr<ID3D11Device> device = nullptr;
				ATL::CComPtr<IDXGISwapChain> swapChain = nullptr;
				ATL::CComPtr<ID3D11DeviceContext> context = nullptr;
};