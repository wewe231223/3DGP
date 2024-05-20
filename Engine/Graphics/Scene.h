#pragma once 



class Scene {
public:
	Scene(ID3D12Device* device,ID3D12GraphicsCommandList* commandList);
	virtual ~Scene();
private:
	std::unique_ptr<class ShadingModel> m_defaultShadingModel{ nullptr };
	std::unique_ptr<class Mesh> m_mesh{ nullptr };
public:
	void Render(ID3D12GraphicsCommandList* commandList);
};


/*
* 
* 어떤 그리기 방법 ( 파이프라인 상태 ) 를 하나로 묶자.. 
* 
* Shader 과 Root Signature 그리고 이들을 모두 필요로 하는 Pipeline State Object 
* 여기에 입력 레이아웃까지.. 
* 
* 이것들을 
* 
* 
*/