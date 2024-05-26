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
	
	
	
	// Vertex �� ǥ��ȭ�ұ�? 
	class ResourceManager {
	public:
		ResourceManager();
		~ResourceManager();
	public:
		// ��� �ϳ��� �Ӽ��� ���� ���� �˰��� ä�����ϳ�? 
		ComPtr<ID3D12Resource> m_positionBuffer{ nullptr };
		ComPtr<ID3D12Resource> m_normalBuffer{ nullptr };
		ComPtr<ID3D12Resource> m_uvBuffer{ nullptr };

		ComPtr<ID3D12Resource> m_indexBuffer{ nullptr };



	};
	
}