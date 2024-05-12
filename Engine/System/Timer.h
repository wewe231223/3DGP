#pragma once 

constexpr unsigned long MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

class Timer
{
public:
	Timer(HWND hWnd = nullptr);
	virtual ~Timer();

	void Update(float fLockFPS = 0.0f);
	void Start();
	void Stop();
	void Reset();

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	float GetTimeElapsed();
	float GetTotalTime();

private:
	double							m_timeScale{};
	float							m_timeElapsed{};

	__int64							m_basePerformanceCounter{};
	__int64							m_pausedPerformanceCounter{ 0 };
	__int64							m_stopPerformanceCounter{ 0 };
	__int64							m_currentPerformanceCounter{};
	__int64							m_lastPerformanceCounter{};

	__int64							m_performanceFrequencyPerSec{};

	std::array<float, MAX_SAMPLE_COUNT> m_frameTime{};
	ULONG							m_sampleCount{ 0 };

	unsigned long					m_currentFrameRate{ 0 };
	unsigned long					m_framePerSecond{ 0 };
	float							m_fpsTimeElapsed{ 0.f };

	bool							m_stopped;

	HWND							m_hWnd{ nullptr };
};
