#pragma once 


class CommandList {
public:
	CommandList(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE commandListType = D3D12_COMMAND_LIST_TYPE_DIRECT);
	~CommandList();
private:
	ComPtr<ID3D12CommandAllocator> m_allocator{ nullptr };
	ComPtr<ID3D12GraphicsCommandList> m_commandlist{ nullptr };
public:
	void Open();
	void Close();
	ID3D12GraphicsCommandList* GetCommandList() const { return m_commandlist.Get(); }
};

