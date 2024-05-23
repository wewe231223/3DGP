#include "pch.h"
#include "SwapChain.h"

UINT msaaq = 0;

SwapChain::SwapChain(HWND hWnd,IDXGIFactory4* factory,ID3D12Device* device,ID3D12CommandQueue* commandQueue,DXGI_FORMAT format){
	m_swapChain.Reset();

	m_format = format;
	m_hWnd = hWnd;

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaa4xState{};
	msaa4xState.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msaa4xState.Format = m_format;
	msaa4xState.NumQualityLevels = 0;
	msaa4xState.SampleCount = 4;

	device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaa4xState, sizeof(msaa4xState));

	// 0 이라면 지원 안함, 0 이상이라면 m_massQuality -1 의 수준 지원 
	m_msaaQuality = msaa4xState.NumQualityLevels;
	msaaq = m_msaaQuality;

	RECT r{};
	::GetClientRect(m_hWnd, &r);

	DXGI_SWAP_CHAIN_DESC SwapChainDesc{};

	SwapChainDesc.BufferDesc.Width = static_cast<UINT>(r.right - r.left);
	SwapChainDesc.BufferDesc.Height = static_cast<UINT>(r.bottom - r.top);
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Format = m_format;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = SwapChain::BUFFER_COUNT;
	SwapChainDesc.OutputWindow = m_hWnd;
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	CheckFailed(factory->CreateSwapChain(commandQueue, &SwapChainDesc, m_swapChain.GetAddressOf()));


	D3D12_DESCRIPTOR_HEAP_DESC RenderTargetViewDesc{};
	::ZeroMemory(&RenderTargetViewDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	RenderTargetViewDesc.NumDescriptors = SwapChain::BUFFER_COUNT;
	RenderTargetViewDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RenderTargetViewDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RenderTargetViewDesc.NodeMask = 0;

	CheckFailed(device->CreateDescriptorHeap(&RenderTargetViewDesc, IID_PPV_ARGS(m_presentRenderTargetDescriptorHeap.GetAddressOf())));
	CheckFailed(device->CreateDescriptorHeap(&RenderTargetViewDesc, IID_PPV_ARGS(m_msaa4xRenderTargetDescriptorHeap.GetAddressOf())));
	
	m_presentRenderTargetDescriptorHeap->SetName(_T("PresentRenderTargetDescriptorHeap"));
	m_msaa4xRenderTargetDescriptorHeap->SetName(_T("Msaa4xRenderTargetDescriptorHeap"));
	
	m_rtDescriptorIncresement = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

SwapChain::~SwapChain(){

}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetCurrBackBufferView() const {
	return CD3DX12_CPU_DESCRIPTOR_HANDLE{m_msaa4xRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
	static_cast<INT>(m_currBackbufferIndex),
	m_rtDescriptorIncresement
	};
}

void SwapChain::Resize(ID3D12Device* device, ID3D12CommandList* commandlist, RECT& clientrect) {



	for (auto i = 0; i < SwapChain::BUFFER_COUNT; ++i) {
		m_presentbackBuffers[i].Reset();
		m_msaa4xBackBuffers[i].Reset();
	}



	CheckFailed(m_swapChain->ResizeBuffers(SwapChain::BUFFER_COUNT, static_cast<UINT>(clientrect.right - clientrect.left), static_cast<UINT>(clientrect.bottom - clientrect.top), m_format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));



	CD3DX12_CPU_DESCRIPTOR_HANDLE RtvHandle{ m_presentRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };



	for (auto i = 0; i < SwapChain::BUFFER_COUNT; ++i) {
		CheckFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_presentbackBuffers[i].GetAddressOf())));
		device->CreateRenderTargetView(m_presentbackBuffers[i].Get(), nullptr, RtvHandle);
		RtvHandle.Offset(1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}




	CD3DX12_RESOURCE_DESC MsaaBackBufferDesc{ CD3DX12_RESOURCE_DESC::Tex2D(m_format, static_cast<UINT>(clientrect.right - clientrect.left), static_cast<UINT>(clientrect.bottom - clientrect.top), 1, 1, 4, m_msaaQuality - 1, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) };

	D3D12_CLEAR_VALUE ClearValue{};
	ClearValue.Format = m_format;
	ClearValue.Color[0] = DirectX::Colors::Black[0];
	ClearValue.Color[1] = DirectX::Colors::Black[1];
	ClearValue.Color[2] = DirectX::Colors::Black[2];
	ClearValue.Color[3] = DirectX::Colors::Black[3];

	CD3DX12_HEAP_PROPERTIES DefaultHeapProperties{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };
	RtvHandle = m_msaa4xRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_RENDER_TARGET_VIEW_DESC rtvdesc{};
	rtvdesc.Format = m_format;
	rtvdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	

	for(auto i = 0; i < SwapChain::BUFFER_COUNT; ++i){
		CheckFailed(device->CreateCommittedResource(
			&DefaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&MsaaBackBufferDesc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&ClearValue,
			IID_PPV_ARGS(m_msaa4xBackBuffers[i].GetAddressOf())));

		device->CreateRenderTargetView(m_msaa4xBackBuffers[i].Get(),nullptr, RtvHandle);
		
		RtvHandle.Offset(1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	
}

void SwapChain::OMSetRenderTareget(ID3D12GraphicsCommandList* commandlist,D3D12_CPU_DESCRIPTOR_HANDLE depthStencil){
	CD3DX12_CPU_DESCRIPTOR_HANDLE RtvHandle{ m_msaa4xRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),static_cast<int>(m_currBackbufferIndex),m_rtDescriptorIncresement};
	commandlist->OMSetRenderTargets(1, &RtvHandle, true, &depthStencil);
}

void SwapChain::ResolveRenderTarget(ID3D12GraphicsCommandList* commandlist){
	CD3DX12_RESOURCE_BARRIER barriers[] = {
	CD3DX12_RESOURCE_BARRIER::Transition(m_presentbackBuffers[m_currBackbufferIndex].Get(),D3D12_RESOURCE_STATE_PRESENT,D3D12_RESOURCE_STATE_RESOLVE_DEST),
	CD3DX12_RESOURCE_BARRIER::Transition(m_msaa4xBackBuffers[m_currBackbufferIndex].Get(),D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_RESOLVE_SOURCE)
	};
	commandlist->ResourceBarrier(_countof(barriers), barriers);

	commandlist->ResolveSubresource(m_presentbackBuffers[m_currBackbufferIndex].Get(), 0, m_msaa4xBackBuffers[m_currBackbufferIndex].Get(), 0, m_format);

	barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(m_presentbackBuffers[m_currBackbufferIndex].Get(), D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_PRESENT);
	barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(m_msaa4xBackBuffers[m_currBackbufferIndex].Get(), D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	commandlist->ResourceBarrier(_countof(barriers), barriers);

}



void SwapChain::Present(){
	CheckFailed(m_swapChain->Present(0, 0));
	m_currBackbufferIndex = ( m_currBackbufferIndex + 1 ) % SwapChain::BUFFER_COUNT;
}
