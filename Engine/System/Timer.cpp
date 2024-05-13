#include "pch.h"
#include "System/Timer.h"

Timer::Timer(HWND hWnd) : m_hWnd(hWnd){
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(std::addressof(m_performanceFrequencyPerSec)));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(std::addressof(m_lastPerformanceCounter)));
	m_timeScale = 1.0 / static_cast<double>(m_performanceFrequencyPerSec);

	m_basePerformanceCounter = m_lastPerformanceCounter;
}

Timer::~Timer(){

}

void Timer::Update(float LockFPS){
	if (m_stopped){
		m_timeElapsed = 0.0f;
		return;
	}
	float TimeElapsed{};

	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(std::addressof(m_currentPerformanceCounter)));
	TimeElapsed = static_cast<float>((m_currentPerformanceCounter - m_lastPerformanceCounter) * m_timeScale);

	if (LockFPS > 0.0f) {
		while (TimeElapsed < (1.0f / LockFPS)){
			::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_currentPerformanceCounter));
			TimeElapsed = static_cast<float>((m_currentPerformanceCounter - m_lastPerformanceCounter) * m_timeScale);
		}
	}

	m_lastPerformanceCounter = m_currentPerformanceCounter;

	if (fabsf(TimeElapsed - m_timeElapsed) < 1.0f){
		::memmove(std::addressof(m_frameTime[1]), m_frameTime.data(), (MAX_SAMPLE_COUNT - 1) * sizeof(float));
		m_frameTime[0] = TimeElapsed;
		if (m_sampleCount < MAX_SAMPLE_COUNT) m_sampleCount++;
	}

	m_framePerSecond++;
	m_fpsTimeElapsed += TimeElapsed;
	if (m_fpsTimeElapsed > 1.0f) {
		m_currentFrameRate = m_framePerSecond;
		m_framePerSecond = 0;
		m_fpsTimeElapsed = 0.0f;

		if(m_hWnd)::SetWindowText(m_hWnd, std::tstring(_T("FPS : ") + std::to_wstring(m_currentFrameRate)).c_str());
	}

	m_timeElapsed = 0.0f;
	for (ULONG i = 0; i < m_sampleCount; i++) m_timeElapsed += m_frameTime[i];
	if (m_sampleCount > 0) m_timeElapsed /= m_sampleCount;
}

void Timer::Start(){
	__int64 PerformanceCounter;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(std::addressof(PerformanceCounter)));
	if (m_stopped) {
		m_pausedPerformanceCounter += (PerformanceCounter - m_stopPerformanceCounter);
		m_lastPerformanceCounter = PerformanceCounter;
		m_stopPerformanceCounter = 0;
		m_stopped = false;
	}
}

void Timer::Stop(){
	if (!m_stopped) {
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(std::addressof(m_stopPerformanceCounter)));
		m_stopped = true;
	}
}

void Timer::Reset(){
	__int64 PerformanceCounter;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(std::addressof(PerformanceCounter)));
	m_basePerformanceCounter = PerformanceCounter;
	m_lastPerformanceCounter = PerformanceCounter;
	m_stopPerformanceCounter = 0;
	m_stopped = false;
}

float Timer::GetTimeElapsed(){
	return m_timeElapsed;
}

float Timer::GetTotalTime(){
	return m_stopped ? static_cast<float>(((m_stopPerformanceCounter - m_pausedPerformanceCounter) - m_basePerformanceCounter) * m_timeScale) :
		static_cast<float>(((m_currentPerformanceCounter - m_pausedPerformanceCounter) - m_basePerformanceCounter) * m_timeScale);
}

