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
* � �׸��� ��� ( ���������� ���� ) �� �ϳ��� ����.. 
* 
* Shader �� Root Signature �׸��� �̵��� ��� �ʿ�� �ϴ� Pipeline State Object 
* ���⿡ �Է� ���̾ƿ�����.. 
* 
* �̰͵��� 
* 
* 
*/