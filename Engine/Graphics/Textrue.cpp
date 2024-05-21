#include "pch.h"
#include "Graphics/Textrue.h"

Texture::Texture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const fs::path& filePath) {

	std::wstring ext = filePath.extension();

	if (ext == L".dds" || ext == L".DDS")
		DirectX::LoadFromDDSFile(filePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, m_image);
	else if (ext == L".tga" || ext == L".TGA")
		DirectX::LoadFromTGAFile(filePath.c_str(), nullptr, m_image);
	else // png, jpg, jpeg, bmp
		DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_image);


	CheckFailed(DirectX::CreateTexture(device, m_image.GetMetadata(), m_defaultHeap.GetAddressOf()));

	std::vector<D3D12_SUBRESOURCE_DATA> subResources{};

	CheckFailed(DirectX::PrepareUpload(
		device,
		m_image.GetImages(),
		m_image.GetImageCount(),
		m_image.GetMetadata(),
		subResources));


	const UINT64 bufferSize{ ::GetRequiredIntermediateSize(m_defaultHeap.Get(),0,static_cast<UINT32>(subResources.size())) };

	D3D12_HEAP_PROPERTIES HeapProperties{ CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_UPLOAD} };
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	CheckFailed(
		device->CreateCommittedResource(
			std::addressof(HeapProperties),
			D3D12_HEAP_FLAG_NONE,
			std::addressof(desc),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_uploadHeap.GetAddressOf())
		)
	);


	::UpdateSubresources(
		commandList,
		m_defaultHeap.Get(),
		m_uploadHeap.Get(),
		0, 0,
		static_cast<UINT>(subResources.size()),
		subResources.data()
	);

	m_srvDesciption.Format = m_image.GetMetadata().format;
	m_srvDesciption.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	m_srvDesciption.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_srvDesciption.Texture2D.MipLevels = 1;
}

Texture::~Texture(){

}

void Texture::CreateSRV(ID3D12Device* device,D3D12_CPU_DESCRIPTOR_HANDLE handle){
	device->CreateShaderResourceView(m_defaultHeap.Get(), &m_srvDesciption, handle);
}

