#include "pch.h"
#include "Device.h"

Device::Device(){
#if defined(DEBUG) || defined(_DEBUG)
    {
        CheckFailed(::D3D12GetDebugInterface(IID_PPV_ARGS(m_debug.GetAddressOf())));
        m_debug->EnableDebugLayer();
        OutputDebugString(_T("DebugLayer Enabled\n"));
    }
#endif 
    CheckFailed(::CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_factory.GetAddressOf())));

    HRESULT HardwareResult = ::D3D12CreateDevice(nullptr, m_directXFeatureLevel, IID_PPV_ARGS(m_device.GetAddressOf()));

    if (FAILED(HardwareResult)) {
        ComPtr<IDXGIAdapter> WarpAdapter{};
        CheckFailed(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&WarpAdapter)));
        CheckFailed(::D3D12CreateDevice(WarpAdapter.Get(), m_directXFeatureLevel, IID_PPV_ARGS(m_device.GetAddressOf())));
    }

}

Device::~Device()
{
}

// 터미널 만들고 
void Device::LogAdapter()
{
}
