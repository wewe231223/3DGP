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