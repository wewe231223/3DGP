#pragma once

class Engine {
public:
	Engine(struct HINSTANCE__* hInstance,struct HWND__* hWnd);
	~Engine();
private:
	struct HINSTANCE__*						m_hInstance{ nullptr };
	struct HWND__*							m_hWnd{ nullptr };
private:
	RECT									m_clientRect{};
	std::unique_ptr<class Device>			m_device{ nullptr };
	std::unique_ptr<class SwapChain>		m_swapChain{ nullptr };
	std::unique_ptr<class CommandQueue>		m_commandQueue{ nullptr };
	std::unique_ptr<class CommandList>		m_resourceCommandList{ nullptr };
	std::unique_ptr<class CommandList>		m_renderCommandList{ nullptr };
	std::unique_ptr<class DepthStencil>		m_depthStencil{ nullptr };
private:
	std::unique_ptr<class Timer>			m_timer{ nullptr };
	std::unique_ptr<class Input>			m_input{ nullptr };
private:
	std::unique_ptr<class Scene>			m_scene{ nullptr };
public:
	void Update();
	void Render();
	void Resize();
};