#pragma once

class Engine {
public:
	Engine(struct HINSTANCE__* hInstance,struct HWND__* hWnd);
	~Engine();
private:
	struct HINSTANCE__* m_hInstance{ nullptr };
	struct HWND__* m_hWnd{ nullptr };

public:
	

};