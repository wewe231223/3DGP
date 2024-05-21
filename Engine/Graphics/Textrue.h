#pragma once 


class Texture {
public:
	Texture(ID3D12Device* device,ID3D12GraphicsCommandList* commandList,const fs::path& filePath);
	~Texture();
private:
	DirectX::ScratchImage m_image{};

	ComPtr<ID3D12Resource> m_defaultHeap{ nullptr };
	ComPtr<ID3D12Resource> m_uploadHeap{ nullptr };

	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesciption{};
public:
	void CreateSRV(ID3D12Device* device,D3D12_CPU_DESCRIPTOR_HANDLE handle);
};