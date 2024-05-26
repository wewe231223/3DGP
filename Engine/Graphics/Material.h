
class Material {
public:
	Material(ID3D12Device* device,D3D12_CPU_DESCRIPTOR_HANDLE& descriptorHeapHandle,std::initializer_list<std::shared_ptr<class Texture>> textures);
	~Material();
private:
	ComPtr<ID3D12Resource> m_materialConstantBuffer{ nullptr };
	class MaterialConstants {
	public:
		DirectX::XMFLOAT4 diffuseAlbedo{ 0.f,0.f,0.f,0.f };
		DirectX::XMFLOAT3 fresnelR0{ 0.f,0.f,0.f };
		float roughness{ 0.f };
		DirectX::XMFLOAT4X4 matTransform;
	};

	constexpr static UINT MaterialConstantsSize = ( sizeof(MaterialConstants) + 255 ) & ~255;

	D3D12_CPU_DESCRIPTOR_HANDLE m_srvHandle{};
public:
	MaterialConstants m_materialConstants{};
public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() const { return m_srvHandle; }
};


/////////////////////////////////////////////
//
//	
//  ------------
// | Material |
//
// 
/////////////////////////////////////////////

