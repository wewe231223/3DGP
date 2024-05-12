#pragma once

class DepthStencil {
public:
	DepthStencil(ID3D12Device* device, DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT);
	~DepthStencil();
private:
	ComPtr<ID3D12Resource> m_depthstencil{ nullptr };
	ComPtr<ID3D12DescriptorHeap> m_depthstencilHeap{ nullptr };
	DXGI_FORMAT m_format{};
	D3D12_CLEAR_VALUE m_optimizeClearValue{};
public:
	void Resize(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist,RECT& clientRect,UINT msaa4xQuality);
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHeapAddr() const;
};