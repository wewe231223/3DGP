#pragma once

// 사실상 디스크립터 힙은 유일할 것이다. 아닌가? 

// 서로 다른 힙을 테이블로 입히기 위해서는
//

//
// 루트 시그니쳐에는 무분별하게 나열되어있는 descriptor heap 을 어떻게 해석할 것인지 
// 들어있다.
// 
// 따라서 이는 루트 시그니쳐와 무관할 것이다. 
// 
// 일단 기본적인 루트 시그니쳐의 틀을 잡자 
// 
// 발생되는 문제 ==> 모든 물체를 다 인스턴싱으로 그릴 것인가? 
// 
// 인스턴싱할 객체와 인스턴싱하지 않을 객체를 분리해야한다 
// 
// 
// Instancing 을 하기 위해서는 먼저 모든 객체의 인스턴싱 데이터를 받고 그려야 한다. 
// 
// Instancing 을 하지 않으려면 그냥 Root Parameter 2 에 있는 Root Descriptor CBV 에 자기 리소스 바인딩하고 그리면 된다. 
// 
// 
// 
// 텍스쳐의 경우... 
// 
// 먼저 모든 텍스쳐 이미지는 Descriptor Table 에 바인딩 되어야 한다. 
// 
// 이러한 Descriptor Table 에는
// 
// Shding Model 에서 이와 같은 구조를 따라야 한다. 
// Root Parameter 1 ==> Buffer Per Pass 
// Root Parameter 2 ==> Buffer Per Object 
// Root Parameter 3 ==> Material Table 
// 
// 텍스쳐를 생성할 때, 적당한 위치에 생성하고 내보낼 것인가
// 
// 테이블을 생성할 때 총 Descriptor 의 개수를 알아야한다 -> 즉 바인딩돨 모든 텍스쳐의 수를 알아야 한다. 
// 
// 모든 텍스쳐 이미지가 하나의 테이블로 올라간다는 보장도 없는 법이다........ 
// 
// 하지만 파이프라인에 바인딩될 SRV heap 은 유일하다 ( 즉 material table 은 유일하다 ) 
// 
// 
// 
// 메쉬 오브젝트 + material 오브젝트 = Game Object
// 
// 
// 모든 리소스는 여하 불문하고 id3d12resource 를 가진다.
// 
// 
// 
//


// 모든 리소스들을 받아놓고 마지막에 한번에 쭉 만들어야 한다.
// 근데 바인딩은 어떻게? 
class DescriptorTable {
public:
	DescriptorTable(ID3D12Device* device);
	~DescriptorTable();
private:
	ComPtr<ID3D12DescriptorHeap> m_descriptorHeap{ nullptr };

};