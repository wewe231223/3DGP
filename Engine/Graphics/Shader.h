#pragma once 

class Shader {
public:
	Shader(fs::path file, const std::string& entry, const std::string& target);
	virtual ~Shader();
private:
	ComPtr<ID3D10Blob> m_shaderBlob{ nullptr };
public:
	D3D12_SHADER_BYTECODE GetByteCode() const;
};


class ShadingModel{
public:
	ShadingModel();
	virtual ~ShadingModel();
protected: // Root Signatrue  
	ComPtr<ID3D12RootSignature> m_rootSignatrue{ nullptr };
	ComPtr<ID3D10Blob> m_serializedRootSignature{ nullptr };
protected: // PSO 
	ComPtr<ID3D12PipelineState>  m_pipelineState{ nullptr };
protected:
	std::array<CD3DX12_STATIC_SAMPLER_DESC, 6> m_staticSamplers{};
public:
	virtual void Bind(ID3D12GraphicsCommandList* commandlist) PURE;
protected:
	void BuildRootSignatrue(const D3D12_ROOT_SIGNATURE_DESC* rootSignatrueDesc);
};


class DefaultShading final : public ShadingModel {
public:
	DefaultShading(ID3D12Device* device,ID3D12GraphicsCommandList* commandList);
	virtual ~DefaultShading();
public:
	virtual void Bind(ID3D12GraphicsCommandList* commandList) override;
public:
	class Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT4 color;
	};
	static const UINT TableRootIndex = 2;
};


class DefaultInstancingShading final : public ShadingModel {
public:
	DefaultInstancingShading(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual ~DefaultInstancingShading();
public:
	virtual void Bind(ID3D12GraphicsCommandList* commandList) override;
public:
	class Vertex {
	public:
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};
	static const UINT TableRootIndex = 2;

};