#pragma once 



class Scene {
public:
	Scene(ID3D12Device* device,ID3D12GraphicsCommandList* commandList,std::shared_ptr<class Window_info> windowInfo);
	virtual ~Scene();
private:
	std::unique_ptr<class Camera> m_camera{ nullptr };
	std::unique_ptr<class ShadingModel> m_defaultShadingModel{ nullptr };
	std::unique_ptr<class Mesh> m_mesh{ nullptr };
	std::unique_ptr<class Texture> m_tex{ nullptr };
	ComPtr<ID3D12DescriptorHeap> descheap{ nullptr };
public:
	void Render(ID3D12GraphicsCommandList* commandList);
	void Update(class Input* input,float timeElapsed);
};


/*
* 
* � �׸��� ��� ( ���������� ���� ) �� �ϳ��� ����.. 
* 
* Shader �� Root Signature �׸��� �̵��� ��� �ʿ�� �ϴ� Pipeline State Object 
* ���⿡ �Է� ���̾ƿ�����.. 
* 
* �̰͵��� 
* 
* 
*/