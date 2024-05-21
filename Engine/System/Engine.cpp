#include "pch.h"
#include "System/Engine.h"


#pragma region External
#include "System/Device.h"
#include "System/SwapChain.h"
#include "System/DepthStencil.h"
#include "System/CommandList.h"
#include "System/CommandQueue.h"
#include "System/Timer.h"
#include "System/Input.h"
#include "Graphics/Scene.h"
#pragma endregion 


Engine::Engine(HINSTANCE__* hInstance, HWND__* hWnd) : m_hInstance(hInstance), m_hWnd(hWnd) {



	m_device = std::make_unique<Device>();
	m_commandQueue = std::make_unique<CommandQueue>(m_device->GetDevice());
	m_swapChain = std::make_unique<SwapChain>(hWnd, m_device->GetFactory(), m_device->GetDevice(), m_commandQueue->GetCommandQueue());
	m_resourceCommandList = std::make_unique<CommandList>(m_device->GetDevice());
	m_renderCommandList = std::make_unique<CommandList>(m_device->GetDevice());
	m_depthStencil = std::make_unique<DepthStencil>(m_device->GetDevice());
	m_timer = std::make_unique<Timer>(m_hWnd);
	m_input = std::make_unique<Input>(m_hWnd, m_hInstance);


	::GetClientRect(m_hWnd, std::addressof(m_clientRect));

	m_resourceCommandList->Open();

	m_swapChain->Resize(m_device->GetDevice(),m_resourceCommandList->GetCommandList(), m_clientRect);
	m_depthStencil->Resize(m_device->GetDevice(), m_resourceCommandList->GetCommandList(), m_clientRect, m_swapChain->GetMsaaQualityLevel());

	m_scene = std::make_unique<Scene>(m_device->GetDevice(), m_resourceCommandList->GetCommandList());

	m_resourceCommandList->Close();
	ID3D12CommandList* commandlists[]{ m_resourceCommandList->GetCommandList() };
	m_commandQueue->Execute(commandlists, _countof(commandlists));
	m_commandQueue->SingleSync();


	m_timer->Start();



}

Engine::~Engine() {

}

void Engine::Update(){
	m_timer->Update();
	m_input->Update();
	m_scene->Update(m_input.get(), m_timer->GetTimeElapsed());
}

void Engine::Render(){
	m_renderCommandList->Open();

	m_renderCommandList->GetCommandList()->ClearRenderTargetView(m_swapChain->GetCurrBackBufferView(), DirectX::Colors::Black, 0, nullptr);
	m_renderCommandList->GetCommandList()->ClearDepthStencilView(m_depthStencil->GetDescriptorHeapAddr(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_swapChain->OMSetRenderTareget(m_renderCommandList->GetCommandList(),m_depthStencil->GetDescriptorHeapAddr());

	D3D12_VIEWPORT viewport{};
	viewport.Height = static_cast<float>(m_clientRect.bottom - m_clientRect.top);
	viewport.Width = static_cast<float>(m_clientRect.right - m_clientRect.left);
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = static_cast<float>(m_clientRect.left);
	viewport.TopLeftY = static_cast<float>(m_clientRect.top);
	
	D3D12_RECT scissorRect{};
	scissorRect.left = m_clientRect.left;
	scissorRect.top = m_clientRect.top;
	scissorRect.right = m_clientRect.right;
	scissorRect.bottom = m_clientRect.bottom;
	
	m_renderCommandList->GetCommandList()->RSSetViewports(1, &viewport);
	m_renderCommandList->GetCommandList()->RSSetScissorRects(1, &scissorRect);

	// �� �ؿ� ���� �׷��� �Ѵ�. 
	m_scene->Render(m_renderCommandList->GetCommandList());

	m_swapChain->ResolveRenderTarget(m_renderCommandList->GetCommandList());
	m_renderCommandList->Close();
	ID3D12CommandList* commandlists[] { m_renderCommandList->GetCommandList()};
	m_commandQueue->Execute(commandlists, _countof(commandlists));
	m_swapChain->Present();
	m_commandQueue->SingleSync();
}

void Engine::Resize(){
	m_commandQueue->SingleSync();
	m_resourceCommandList->Open();
	::GetClientRect(m_hWnd, std::addressof(m_clientRect));
	m_swapChain->Resize(m_device->GetDevice(), m_resourceCommandList->GetCommandList(), m_clientRect);
	m_depthStencil->Resize(m_device->GetDevice(), m_resourceCommandList->GetCommandList(), m_clientRect, m_swapChain->GetMsaaQualityLevel());
	m_resourceCommandList->Close();
	m_commandQueue->SingleSync();
}

