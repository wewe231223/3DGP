#include "pch.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil(ID3D12Device* device, DXGI_FORMAT format) : m_format(format){
	D3D12_DESCRIPTOR_HEAP_DESC DepthStencilDescriptorDesc{};
	::ZeroMemory(&DepthStencilDescriptorDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	DepthStencilDescriptorDesc.NumDescriptors = 1;
	DepthStencilDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DepthStencilDescriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DepthStencilDescriptorDesc.NodeMask = NULL;

	CheckFailed(device->CreateDescriptorHeap(&DepthStencilDescriptorDesc, IID_PPV_ARGS(m_depthstencilHeap.GetAddressOf())));

	m_optimizeClearValue = D3D12_CLEAR_VALUE{ m_format,1.f,0.f };

}

DepthStencil::~DepthStencil()
{
}

void DepthStencil::Resize(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, RECT& clientRect,UINT msaa4xQuality){
	m_depthstencil.Reset();

	D3D12_RESOURCE_DESC DepthStencilDesc;
	DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	DepthStencilDesc.Alignment = 0;
	DepthStencilDesc.Width = static_cast<UINT>(clientRect.right - clientRect.left) * 4;
	DepthStencilDesc.Height = static_cast<UINT>(clientRect.bottom - clientRect.top) * 4;
	DepthStencilDesc.DepthOrArraySize = 1;
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.Format = m_format;
	DepthStencilDesc.SampleDesc.Count = msaa4xQuality == 0 ? 1 : 4;
	DepthStencilDesc.SampleDesc.Quality = msaa4xQuality - 1;
	DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	CD3DX12_HEAP_PROPERTIES HeapProp{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };

	CheckFailed(device->CreateCommittedResource(
		&HeapProp,
		D3D12_HEAP_FLAG_NONE,
		&DepthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&m_optimizeClearValue,
		IID_PPV_ARGS(m_depthstencil.GetAddressOf())
	));

	device->CreateDepthStencilView(m_depthstencil.Get(), nullptr, m_depthstencilHeap->GetCPUDescriptorHandleForHeapStart());


	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_depthstencil.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);

	commandlist->ResourceBarrier(1, &barrier);
}

D3D12_CPU_DESCRIPTOR_HANDLE DepthStencil::GetDescriptorHeapAddr() const {
	return m_depthstencilHeap->GetCPUDescriptorHandleForHeapStart();
}
