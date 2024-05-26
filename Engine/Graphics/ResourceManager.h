#pragma once 


namespace Resource {
	class Mesh {
	public:
		Mesh();
		~Mesh();
	private:
		ComPtr<ID3D12Resource> m_positionDefault{ nullptr };
		ComPtr<ID3D12Resource> m_positionUpload{ nullptr };

		ComPtr<ID3D12Resource> m_normalDefault{ nullptr };
		ComPtr<ID3D12Resource> m_normalUpload{ nullptr };

		ComPtr<ID3D12Resource> m_uvDefault{ nullptr };
		ComPtr<ID3D12Resource> m_uvUpload{ nullptr };

		ComPtr<ID3D12Resource> m_indexDefault{ nullptr };
		ComPtr<ID3D12Resource> m_indexUpload{ nullptr };

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView{};
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView{};

		UINT32 m_indexCount{};
	};
	
	
	
	// Vertex 를 표준화할까? 
	class ResourceManager {
	public:
		ResourceManager();
		~ResourceManager();
	public:
		// 어느 하나의 속성이 없는 경우는 똥값을 채워야하나? 
		ComPtr<ID3D12Resource> m_positionBuffer{ nullptr };
		ComPtr<ID3D12Resource> m_normalBuffer{ nullptr };
		ComPtr<ID3D12Resource> m_uvBuffer{ nullptr };

		ComPtr<ID3D12Resource> m_indexBuffer{ nullptr };



	};
	
}