#pragma once 

class Device {
public:
	Device();
	~Device();
private:
	ComPtr<IDXGIFactory4> m_factory{ nullptr };
	ComPtr<ID3D12Device> m_device{ nullptr };
#if defined(DEBUG) || defined(_DEBUG)
	ComPtr<ID3D12Debug> m_debug{ nullptr };
#endif
	D3D_FEATURE_LEVEL m_directXFeatureLevel{ D3D_FEATURE_LEVEL_11_0 };
public:
	ID3D12Device* GetDevice() const {
		return m_device ? m_device.Get() : nullptr;
	}

	IDXGIFactory4* GetFactory() const {
		return m_factory ? m_factory.Get() : nullptr;
	}
private:
	void LogAdapter();

};