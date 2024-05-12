class CommandQueue {
public:
	CommandQueue(ID3D12Device* device);
	~CommandQueue();
private:
	ComPtr<ID3D12CommandQueue> m_commandQueue{ nullptr };
	ComPtr<ID3D12Fence> m_fence{ nullptr };
	HANDLE m_syncHandle{ nullptr };
	UINT64 m_fenceValue{ 0 };
public:
	ID3D12CommandQueue* GetCommandQueue() const { return m_commandQueue.Get(); }
	void Execute(ID3D12CommandList** commandList,UINT numofcommandlists);
	void SingleSync();
};