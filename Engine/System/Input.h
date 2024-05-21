class Input {
public:
	Input(HWND hWnd, HINSTANCE hInstance);
	~Input();
private:
	IDirectInput8* m_pDirectInput{ nullptr };
	IDirectInputDevice8* m_pKeyBoardDevice{ nullptr };
	IDirectInputDevice8* m_pMouseDevice{ nullptr };

	std::array<KEY_STATE, 256> m_prevKeyboardState{ KEY_STATE::NONE, };
	std::array<KEY_STATE, 4> m_prevMouseState{ KEY_STATE::NONE };


	POINT m_deltaMouse{};

public:
	void Update();
	KEY_STATE GetKeyState(DWORD key) const;
	KEY_STATE GetMouseState(DWORD key) const;
	LONG GetDeltaMouseX() const;
	LONG GetDeltaMouseY() const;
};

constexpr DWORD DIM_LEFT = 0x00;
constexpr DWORD DIM_RIGHT = 0x01;

