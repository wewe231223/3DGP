#include "pch.h"
#include "Graphics/Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Render(ID3D12GraphicsCommandList* commandList){
	commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	commandList->IASetIndexBuffer(&m_indexBufferView);
	commandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}

void Mesh::Render(ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW instanceView){
	D3D12_VERTEX_BUFFER_VIEW views[] = { m_vertexBufferView, instanceView };
	commandList->IASetVertexBuffers(0, _countof(views), views);
	commandList->IASetIndexBuffer(&m_indexBufferView);
	commandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->DrawIndexedInstanced(m_indexCount, 1000, 0, 0, 0);
}

void Mesh::CreateBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ComPtr<ID3D12Resource>& defaultBuffer,ComPtr<ID3D12Resource>& uploadBuffer, void* data, UINT64 size){
	CD3DX12_HEAP_PROPERTIES DefaultHeapProperties{ D3D12_HEAP_TYPE_DEFAULT };
	CD3DX12_HEAP_PROPERTIES UploadHeapProperties{ D3D12_HEAP_TYPE_UPLOAD };

	CD3DX12_RESOURCE_DESC BufferDesc{ CD3DX12_RESOURCE_DESC::Buffer(size) };

	CheckFailed(device->CreateCommittedResource(
		std::addressof(DefaultHeapProperties),
		D3D12_HEAP_FLAG_NONE,
		std::addressof(BufferDesc),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf())
	));

	CheckFailed(device->CreateCommittedResource(
		std::addressof(UploadHeapProperties),
		D3D12_HEAP_FLAG_NONE,
		std::addressof(BufferDesc),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf())
	));

	D3D12_SUBRESOURCE_DATA SubResourceData{};
	SubResourceData.pData = data;
	SubResourceData.RowPitch = size;
	SubResourceData.SlicePitch = SubResourceData.RowPitch;

	CD3DX12_RESOURCE_BARRIER ResourceBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_COPY_DEST)};
	commandList->ResourceBarrier(1, &ResourceBarrier);

	UpdateSubresources<1>(commandList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, std::addressof(SubResourceData));

	ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	commandList->ResourceBarrier(1,&ResourceBarrier);

}
