#include "pch.h"
#include "System/Engine.h"


#pragma region External
#include "System/Device.h"
#include "System/SwapChain.h"
#include "System/DepthStencil.h"
#include "System/CommandList.h"
#include "System/CommandQueue.h"
#include "System/Timer.h"
#pragma endregion 

#include "Graphics/Shader.h"

Engine::Engine(HINSTANCE__* hInstance, HWND__* hWnd) : m_hInstance(hInstance), m_hWnd(hWnd) {
	m_device = std::make_unique<Device>();
	m_commandQueue = std::make_unique<CommandQueue>(m_device->GetDevice());
	m_swapChain = std::make_unique<SwapChain>(hWnd, m_device->GetFactory(), m_device->GetDevice(), m_commandQueue->GetCommandQueue());
	m_commandList = std::make_unique<CommandList>(m_device->GetDevice());
	m_depthStencil = std::make_unique<DepthStencil>(m_device->GetDevice());
	m_timer = std::make_unique<Timer>(m_hWnd);


	::GetClientRect(m_hWnd, std::addressof(m_clientRect));

	m_commandList->Open();

	m_swapChain->Resize(m_device->GetDevice(),m_commandList->GetCommandList(), m_clientRect);
	m_depthStencil->Resize(m_device->GetDevice(), m_commandList->GetCommandList(), m_clientRect, m_swapChain->GetMsaaQualityLevel());

	m_commandList->Close();
	ID3D12CommandList* commandlists[]{ m_commandList->GetCommandList() };
	m_commandQueue->Execute(commandlists, _countof(commandlists));
	m_commandQueue->SingleSync();
	m_timer->Start();

	Shader sh{ "./Shaders/Default.hlsl","VS_Main","vs_5_0"};

}

Engine::~Engine() {

}

void Engine::Update(){
	m_timer->Update();
}

void Engine::Render(){
	m_commandList->Open();

	m_commandList->GetCommandList()->ClearRenderTargetView(m_swapChain->GetCurrBackBufferView(), DirectX::Colors::LightSteelBlue, 0, nullptr);
	m_commandList->GetCommandList()->ClearDepthStencilView(m_depthStencil->GetDescriptorHeapAddr(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_swapChain->OMSetRenderTareget(m_commandList->GetCommandList(),m_depthStencil->GetDescriptorHeapAddr());
	m_swapChain->ResolveRenderTarget(m_commandList->GetCommandList());

	// ÀÌ ¹Ø¿¡ ¹º°¡ ±×·Á¾ß ÇÑ´Ù. 


	m_commandList->Close();
	ID3D12CommandList* commandlists[] {m_commandList->GetCommandList()};
	m_commandQueue->Execute(commandlists, _countof(commandlists));
	m_swapChain->Present();
	m_commandQueue->SingleSync();
}

void Engine::Resize(){
	m_commandQueue->SingleSync();
	m_commandList->Open();
	::GetClientRect(m_hWnd, std::addressof(m_clientRect));
	m_swapChain->Resize(m_device->GetDevice(), m_commandList->GetCommandList(), m_clientRect);
	m_depthStencil->Resize(m_device->GetDevice(), m_commandList->GetCommandList(), m_clientRect, m_swapChain->GetMsaaQualityLevel());
	m_commandList->Close();
	m_commandQueue->SingleSync();
}

LRESULT Engine::ProcessWindowMessage(UINT Msg, WPARAM Wparam, LPARAM Lparam){
	if (m_hWnd){
		switch (Msg)
		{
		case WM_SIZE:
		
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(m_hWnd, Msg, Wparam, Lparam);
		}
	}
	return LRESULT();
}
