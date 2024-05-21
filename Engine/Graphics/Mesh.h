#pragma once 


class Mesh {
public:
	Mesh();
	~Mesh();
private:
	ComPtr<ID3D12Resource> m_vertexDefault{ nullptr };
	ComPtr<ID3D12Resource> m_vertexUpload{ nullptr };

	ComPtr<ID3D12Resource> m_indexDefault{ nullptr };
	ComPtr<ID3D12Resource> m_indexUpload{ nullptr };

	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView{};
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView{};

	UINT32 m_indexCount{};

public:
	template<typename Vertex, typename Index = UINT32>
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const fs::path& filePath);

	void Render(ID3D12GraphicsCommandList* commandList);
	void Render(ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW instanceView);

private:
	void CreateBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, 
		ComPtr<ID3D12Resource>& defaultBuffer, ComPtr<ID3D12Resource>& uploadBuffer,
		void* data,UINT64 size);

};

template<typename Vertex, typename Index>
inline void Mesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const fs::path& filePath){
	std::ifstream in{ filePath, std::ios::binary };
	if (!in) {
		throw std::runtime_error{ "Failed To Open File: " + filePath.string() };
		PostQuitMessage(0);
	}

	size_t numOfVertex;
	size_t numOfIndex;

	in.read(reinterpret_cast<char*>(std::addressof(numOfVertex)), sizeof(size_t));
	std::unique_ptr<Vertex[]> Vertices = std::make_unique<Vertex[]>(numOfVertex);
	in.read(reinterpret_cast<char*>(Vertices.get()), sizeof(Vertex) * numOfVertex);

	in.read(reinterpret_cast<char*>(std::addressof(numOfIndex)), sizeof(size_t));
	std::unique_ptr<Index[]> Indices = std::make_unique<Index[]>(numOfIndex);
	in.read(reinterpret_cast<char*>(Indices.get()), sizeof(Index) * numOfIndex);




	CreateBuffer(device, commandList, m_vertexDefault, m_vertexUpload, Vertices.get(), static_cast<UINT64>(sizeof(Vertex) * numOfVertex));
	CreateBuffer(device, commandList, m_indexDefault, m_indexUpload, Indices.get(), static_cast<UINT64>(sizeof(Index) * numOfIndex));

	m_vertexBufferView.BufferLocation = m_vertexDefault->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(Vertex) * numOfVertex);
	m_vertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(Vertex));

	m_indexBufferView.BufferLocation = m_indexDefault->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = static_cast<UINT>(sizeof(Index) * numOfIndex);



	if (std::is_same_v<Index, UINT16>)
		m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	else if (std::is_same_v<Index, UINT32>)
		m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	else
		throw std::runtime_error{ "Not Supported Type\n" };
	
	m_indexCount = static_cast<UINT32>(numOfIndex);
	
}
