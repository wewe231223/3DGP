#pragma once 



class RootSignature {
public:
	RootSignature();
	virtual ~RootSignature();
protected:
	ComPtr<ID3D12RootSignature> m_rootSignature{ nullptr };
	ComPtr<ID3D10Blob> m_serializedRootSignature{ nullptr };
	D3D12_ROOT_SIGNATURE_DESC m_rootSignatrueDesc{};

	std::vector<CD3DX12_ROOT_PARAMETER> m_rootParameters{};
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> m_staticSamplers{};
public:
	ID3D12RootSignature* GetRootSignatrue() const noexcept;
};


//////////////////////////////////////////////
//
// |<---CBV---->|<-----SRV----->|<-----UAV------>|
//  
// ���� ��� ���� 
// 
// ��Ʈ �ñ״��� ��Ʈ�� 
// 
// 1. ���� * �� ��� ( 16�� )
// 2. ���� ��� ( 16�� ) 
// 3. �ؽ���( Material ) Descriptor Table ( 1�� )
// 
// 3. Material Descriptor Table �� ���Ͽ� 
// 
// ����. Stone, Wood, Steel �� �� ���� Material �� �ִٰ� �������� 
// 
// ���� Material �� ������ �ؽ��ĸ� ������ �ִ�.
// 
// �̷� ������ Descriptor Table �� �������� 
// |<---------Stone Texture---------><-----Stone CBV----->|<---------Wood Texture---------><-----Wood CBV----->|<---------Steel Texture---------><-----Steel CBV----->|
// 0													 40													   80								
// 
// 
// Root Signatrue �� ���̴��� ���ӵǴ� ��ü�̴�.. 
// 
// ���̴��� ���� �׳� ��Ʈ �ñ״��ĸ� ���ӽ�ų�� ==> Shading Model �� ������� 
//  
//