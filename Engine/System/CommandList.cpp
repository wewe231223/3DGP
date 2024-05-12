#include "pch.h"
#include "CommandList.h"

CommandList::CommandList(ID3D12Device* device,D3D12_COMMAND_LIST_TYPE commandListType){
	device->CreateCommandAllocator(commandListType, IID_PPV_ARGS(m_allocator.GetAddressOf()));
	device->CreateCommandList(0, commandListType, m_allocator.Get(), nullptr, IID_PPV_ARGS(m_commandlist.GetAddressOf()));

	CommandList::Close();
}

CommandList::~CommandList()
{
}

void CommandList::Open(){
	CheckFailed(m_allocator->Reset());
	CheckFailed(m_commandlist->Reset(m_allocator.Get(), nullptr));
}

void CommandList::Close(){
	CheckFailed(m_commandlist->Close());
}
