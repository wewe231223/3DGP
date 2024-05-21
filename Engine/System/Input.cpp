#include "pch.h"
#include "System/Input.h"

Input::Input(HWND hWnd, HINSTANCE hInstance) {

	// DirectInput 객체 생성 
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, nullptr);

	// 키보드 디바이스 생성 
	m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoardDevice, nullptr);
	m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoardDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);
	while (m_pKeyBoardDevice->Acquire() == DIERR_INPUTLOST);

	// 마우스 디바이스 생성 
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, nullptr);
	m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
	m_pMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
}

Input::~Input() {
	if (m_pMouseDevice) {
		m_pMouseDevice->Unacquire();
		m_pMouseDevice->Release();
		m_pMouseDevice = NULL;
	}

	if (m_pKeyBoardDevice) {
		m_pKeyBoardDevice->Unacquire();
		m_pKeyBoardDevice->Release();
		m_pKeyBoardDevice = NULL;
	}

	if (m_pDirectInput) {
		m_pDirectInput->Release();
		m_pDirectInput = NULL;
	}
}

void Input::Update() {
	std::array<BYTE, 256> Keyboard{};
	DIMOUSESTATE Mouse{};

	if (FAILED(m_pKeyBoardDevice->GetDeviceState(256, Keyboard.data()))) {
		while (m_pKeyBoardDevice->Acquire() == DIERR_INPUTLOST);
	}

	if (FAILED(m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &Mouse))) {
		while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
	}

	for (auto i = 0; i < 256; ++i) {
		if (Keyboard[i] & 0x80) {
			if (m_prevKeyboardState[i] == KEY_STATE::NONE || m_prevKeyboardState[i] == KEY_STATE::RELEASE) {
				m_prevKeyboardState[i] = KEY_STATE::DOWN;
			}
			else if (m_prevKeyboardState[i] == KEY_STATE::DOWN) {
				m_prevKeyboardState[i] = KEY_STATE::PRESS;
			}
		}
		else {
			if (m_prevKeyboardState[i] == KEY_STATE::PRESS || m_prevKeyboardState[i] == KEY_STATE::DOWN) {
				m_prevKeyboardState[i] = KEY_STATE::RELEASE;
			}
			else if (m_prevKeyboardState[i] == KEY_STATE::RELEASE) {
				m_prevKeyboardState[i] = KEY_STATE::NONE;
			}
		}
	}

	/*
	* 0 = Left Mouse Button
	* 1 = Right Mouse Button
	* 2 = Middle Mouse Button
	* 3 = Side Mouse Button( 하지만 이 키는 마우스별로 인식되는 버튼이 다를 수 있다. 따라서 이 키는 되도록 사용하지 말 것 )
	*/
	for (UINT i = 0; i < 4; ++i) {
		if (Mouse.rgbButtons[i] & 0x80) {
			if (m_prevMouseState[i] == KEY_STATE::NONE || m_prevMouseState[i] == KEY_STATE::RELEASE) {
				m_prevMouseState[i] = KEY_STATE::DOWN;
			}
			else if (m_prevMouseState[i] == KEY_STATE::DOWN) {
				m_prevMouseState[i] = KEY_STATE::PRESS;
			}
		}
		else {
			if (m_prevMouseState[i] == KEY_STATE::PRESS || m_prevMouseState[i] == KEY_STATE::DOWN) {
				m_prevMouseState[i] = KEY_STATE::RELEASE;
			}
			else if (m_prevMouseState[i] == KEY_STATE::RELEASE) {
				m_prevMouseState[i] = KEY_STATE::NONE;
			}
		}
	}

	m_deltaMouse.x = Mouse.lX;
	m_deltaMouse.y = Mouse.lY;
}

KEY_STATE Input::GetKeyState(DWORD key) const {
	return m_prevKeyboardState[key];
}

KEY_STATE Input::GetMouseState(DWORD key) const {
	return m_prevMouseState[key];
}

LONG Input::GetDeltaMouseX() const {
	return m_deltaMouse.x;
}

LONG Input::GetDeltaMouseY() const {
	return m_deltaMouse.y;
}
