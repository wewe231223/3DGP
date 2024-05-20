#include "pch.h"
#include "Graphics/Scene.h"

#pragma region External 
#include "Graphics/Shader.h"
#include "Graphics/Textrue.h"
#include "Graphics/Mesh.h"

#pragma endregion 

Scene::Scene(ID3D12Device* device,ID3D12GraphicsCommandList* commandList){
	m_defaultShadingModel = std::make_unique<DefaultInstancingShading>(device,commandList);

	// ���� ���Ǵ� �Դ� 
	// 1. ���ҽ� �����ϱ�
	// 2. ���ҽ� ���ε��ϱ� 
	// 3. Matreial ����� 

	m_mesh = std::make_unique<Mesh>();
	m_mesh->Initialize<DefaultInstancingShading::Vertex>(device, commandList, L"./Resources/woodenBox.bin");

	// Texture tex{ device,commandList,"./Resources/wooden_box.tga" };
	//m_tex = std::make_unique<Texture>(device, commandList, "./Resources/wooden_box.tga");

}

Scene::~Scene()
{
}

void Scene::Render(ID3D12GraphicsCommandList* commandList){
	m_defaultShadingModel->Bind(commandList);


	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(10.f, 10.f, -50.f, 0.f), DirectX::XMVectorSet(1.f, 1.f, 0.f, 0.f), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.f), 1920.f / 1080.f, 0.01f, 100.0f);

	DirectX::XMMATRIX viewproj = DirectX::XMMatrixMultiply(view, proj);

	DirectX::XMFLOAT4X4 mat{};
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(viewproj));
	commandList->SetGraphicsRoot32BitConstants(1, 16, &mat, 0);


	m_mesh->Render(commandList);
}
