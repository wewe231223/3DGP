#include "pch.h"
#include "CommandQueue.h"

CommandQueue::CommandQueue(ID3D12Device* device){
	D3D12_COMMAND_QUEUE_DESC CommandDesc{};
	CommandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CommandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	CheckFailed(device->CreateCommandQueue(&CommandDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf())));

	m_syncHandle = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	CheckFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf())));
}

CommandQueue::~CommandQueue(){


}

void CommandQueue::Execute(ID3D12CommandList** commandList, UINT numofcommandlists){
	m_commandQueue->ExecuteCommandLists(numofcommandlists, commandList);
}

void CommandQueue::SingleSync(){
	m_fenceValue++;
	CheckFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValue));

	if (m_fence->GetCompletedValue() < m_fenceValue) {
		CheckFailed(m_fence->SetEventOnCompletion(m_fenceValue, m_syncHandle));
		::WaitForSingleObject(m_syncHandle, INFINITE);
	}

}
