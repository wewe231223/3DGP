#include "pch.h"
#include "Material.h"

#pragma region External	
#include "Graphics/Textrue.h"
#pragma endregion

Material::Material(ID3D12Device* device,D3D12_CPU_DESCRIPTOR_HANDLE& descriptorHeapHandle,std::initializer_list<std::shared_ptr<class Texture>> textures){

	auto heapIncreasement = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_srvHandle = descriptorHeapHandle;

	for (const auto& tex : textures) {
		tex->CreateSRV(device, descriptorHeapHandle);
		descriptorHeapHandle.ptr += heapIncreasement;
	}

	
	CD3DX12_HEAP_PROPERTIES heapProperties{ D3D12_HEAP_TYPE_UPLOAD };
	CD3DX12_RESOURCE_DESC resourceDesc{ CD3DX12_RESOURCE_DESC::Buffer(MaterialConstantsSize) };

	CheckFailed(device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_materialConstantBuffer.GetAddressOf())
	));


	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = m_materialConstantBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = MaterialConstantsSize;


	device->CreateConstantBufferView(&cbvDesc, descriptorHeapHandle);
}

Material::~Material()
{
}
