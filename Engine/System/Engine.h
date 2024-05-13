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
	std::unique_ptr<class CommandList>		m_commandList{ nullptr };
	std::unique_ptr<class DepthStencil>		m_depthStencil{ nullptr };
private:
	std::unique_ptr<class Timer>			m_timer{ nullptr };
public:
	void Update();
	void Render();
	void Resize();
	LRESULT ProcessWindowMessage(UINT Msg, WPARAM Wparam, LPARAM Lparam);
};