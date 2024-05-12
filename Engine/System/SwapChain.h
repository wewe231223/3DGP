#pragma once

/*

SampleDesc.Count = 1 �� SampleDesc.Quality = 0�� ����Ͽ� ��ҿ� ���� SwapChain�� �����մϴ�. ( o ) 
��ҿ� ���� SwapChain RTV�� �����մϴ�. 

�ʿ��� ���� ���� ǰ���� ����Ͽ� �� �ٸ� RenderTarget �ؽ�ó + �並 ����ϴ�.
������ ���� ǰ���� PipelineStateObject�� �����մϴ�(�̴� ���� ���ٽ� ���ۿ��� ����˴ϴ�).
����� �������Ϸ��� SwapChain�� �ƴ� ���� Ÿ���� ����ϼ���.

ȭ�鿡 �������� ǥ���� �غ� �Ǹ� SwapChain�� �ƴ� ���� Ÿ���� "D3D12_RESOURCE_STATE_RESOLVE_SOURCE" ���·� ��ȯ�ϰ� ���� SwapChain ���� Ÿ���� "D3D12_RESOURCE_STATE_RESOLVE_DEST"�� ��ȯ�մϴ�.
�ùٸ� �ҽ�/����� ����Ͽ� ��� ��Ͽ��� ResolveSubresource�� ȣ���Ͻʽÿ�.
SwapChain ���� Ÿ���� D3D12_RESOURCE_STATE_PRESENT�� ��ȯ�ϸ� �Ϸ�˴ϴ�.

*/


class SwapChain {
public:
	SwapChain(HWND hWnd, IDXGIFactory4* factory, ID3D12Device* device, ID3D12CommandQueue* commandQueue, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~SwapChain();
public:
	static constexpr int BUFFER_COUNT{ 2 };
private:
	ComPtr<IDXGISwapChain> m_swapChain{ nullptr };
	std::array<ComPtr<ID3D12Resource>, SwapChain::BUFFER_COUNT> m_presentbackBuffers{};
	std::array<ComPtr<ID3D12Resource>,SwapChain::BUFFER_COUNT> m_msaa4xBackBuffers{};

	ComPtr<ID3D12DescriptorHeap> m_presentRenderTargetDescriptorHeap{ nullptr };
	ComPtr<ID3D12DescriptorHeap> m_msaa4xRenderTargetDescriptorHeap{ nullptr };

	HWND m_hWnd{};
	DXGI_FORMAT m_format{};
	UINT m_msaaQuality{ 0 };

	UINT m_currBackbufferIndex{ 0 };

	UINT m_rtDescriptorIncresement{ 0 };
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrBackBufferView() const;
	UINT GetMsaaQualityLevel() const { return m_msaaQuality; }
	void Resize(ID3D12Device* device, ID3D12CommandList* commandlist,RECT& clientRect);
	void OMSetRenderTareget(ID3D12GraphicsCommandList* commandlist,D3D12_CPU_DESCRIPTOR_HANDLE depthStencil);
	void ResolveRenderTarget(ID3D12GraphicsCommandList* commandlist);
	void Present();
};