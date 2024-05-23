#include "pch.h"
#include "Graphics/Scene.h"

#pragma region External 
#include "Graphics/Shader.h"
#include "Graphics/Textrue.h"
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "System/Input.h"

#pragma endregion 

Scene::Scene(ID3D12Device* device,ID3D12GraphicsCommandList* commandList, std::shared_ptr<Window_info> windowInfo){
	m_defaultShadingModel = std::make_unique<DefaultInstancingShading>(device,commandList);

	// 이제 거의다 왔다 
	// 1. 리소스 생성하기
	// 2. 리소스 바인딩하기 
	// 3. Matreial 만들기 

	m_camera = std::make_unique<Camera>();

	m_mesh = std::make_unique<Mesh>();
	m_mesh->Initialize<DefaultInstancingShading::Vertex>(device, commandList, L"./Resources/cube_low.bin");

	// Texture tex{ device,commandList,"./Resources/wooden_box.tga" };
	m_tex = std::make_unique<Texture>(device, commandList, "./Resources/cube_low_None_AlbedoTransparency.png");


	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 2;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(descheap.GetAddressOf()));


	m_tex->CreateSRV(device, descheap->GetCPUDescriptorHandleForHeapStart());



}

Scene::~Scene()
{
}

void Scene::Render(ID3D12GraphicsCommandList* commandList){
	m_defaultShadingModel->Bind(commandList);
	
	commandList->SetDescriptorHeaps(1, descheap.GetAddressOf());
	commandList->SetGraphicsRootDescriptorTable(2, descheap->GetGPUDescriptorHandleForHeapStart());


	DirectX::XMMATRIX view = m_camera->GetViewMatrix();
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.f), 1920.f / 1080.f, 0.01f, 1000.0f);

	DirectX::XMMATRIX viewproj = DirectX::XMMatrixMultiply(view, proj);

	DirectX::XMFLOAT4X4 mat{};
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(viewproj));
	commandList->SetGraphicsRoot32BitConstants(1, 16, &mat, 0);

	DefaultInstancingShading::InstanceData instancedata[10][10][10]{};

	for (auto i = 0; i < 10; ++i) {
		for (auto j = 0; j < 10; ++j) {
			for (auto k = 0; k < 10; ++k) {
				DirectX::XMStoreFloat4x4(&instancedata[i][j][k].world,
					DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(i * 10.f, j * 10.f, k * 10.f))

				);
			}
		}
	}

	memcpy(reinterpret_cast<DefaultInstancingShading*>(m_defaultShadingModel.get())->GetInstanceDataAddr(), &instancedata, sizeof(DefaultInstancingShading::InstanceData) * 1000);


	m_mesh->Render(commandList, reinterpret_cast<DefaultInstancingShading*>(m_defaultShadingModel.get())->GetInstanceView());
}

void Scene::Update(Input* input, float timeElapsed){
	if (input->GetMouseState(DIM_RIGHT) == KEY_STATE::PRESS) {
		m_camera->FocusUpdate(static_cast<float>(input->GetDeltaMouseX()),static_cast<float>(input->GetDeltaMouseY()));
	}
	m_camera->PositionUpdate(input);
	m_camera->UpdateViewMatrix();
}
