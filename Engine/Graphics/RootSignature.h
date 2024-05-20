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
// 월드 행렬 버퍼 
// 
// 루트 시그니쳐 엔트리 
// 
// 1. 투영 * 뷰 행렬 ( 16개 )
// 2. 월드 행렬 ( 16개 ) 
// 3. 텍스쳐( Material ) Descriptor Table ( 1개 )
// 
// 3. Material Descriptor Table 에 대하여 
// 
// 가정. Stone, Wood, Steel 의 세 가지 Material 이 있다고 가정하자 
// 
// 각각 Material 은 각자의 텍스쳐를 가지고 있다.
// 
// 이런 식으로 Descriptor Table 을 구성하자 
// |<---------Stone Texture---------><-----Stone CBV----->|<---------Wood Texture---------><-----Wood CBV----->|<---------Steel Texture---------><-----Steel CBV----->|
// 0													 40													   80								
// 
// 
// Root Signatrue 은 셰이더에 종속되는 객체이다.. 
// 
// 셰이더에 따라 그냥 루트 시그니쳐를 종속시킬까 ==> Shading Model 을 사용하자 
//  
//