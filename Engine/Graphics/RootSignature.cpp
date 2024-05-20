#include "pch.h"
#include "Graphics/RootSignature.h"

ID3D12RootSignature* RootSignature::GetRootSignatrue() const noexcept {
	return m_rootSignature ? m_rootSignature.Get() : nullptr;
}
