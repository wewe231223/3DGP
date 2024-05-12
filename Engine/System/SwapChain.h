#pragma once

/*

SampleDesc.Count = 1 및 SampleDesc.Quality = 0을 사용하여 평소와 같이 SwapChain을 생성합니다. ( o ) 
평소와 같이 SwapChain RTV를 생성합니다. 

필요한 샘플 수와 품질을 사용하여 또 다른 RenderTarget 텍스처 + 뷰를 만듭니다.
동일한 수와 품질로 PipelineStateObject를 생성합니다(이는 깊이 스텐실 버퍼에도 적용됩니다).
장면을 렌더링하려면 SwapChain이 아닌 렌더 타겟을 사용하세요.

화면에 콘텐츠를 표시할 준비가 되면 SwapChain이 아닌 렌더 타겟을 "D3D12_RESOURCE_STATE_RESOLVE_SOURCE" 상태로 전환하고 실제 SwapChain 렌더 타겟을 "D3D12_RESOURCE_STATE_RESOLVE_DEST"로 전환합니다.
올바른 소스/대상을 사용하여 명령 목록에서 ResolveSubresource를 호출하십시오.
SwapChain 렌더 타겟을 D3D12_RESOURCE_STATE_PRESENT로 전환하면 완료됩니다.

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