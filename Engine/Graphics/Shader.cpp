#include "pch.h"
#include "Graphics/Shader.h"

Shader::Shader(fs::path file,const std::string& entry,const std::string& target) {


	fs::path binpath = file;
	binpath.replace_extension(".bin");

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	if (file.extension() == ".bin") {
		std::ifstream in{ file,std::ios::binary };
		SIZE_T size{};
		in.read(reinterpret_cast<char*>(std::addressof(size)), sizeof(SIZE_T));
		::D3DCreateBlob(size, m_shaderBlob.GetAddressOf());
		in.read(reinterpret_cast<char*>(m_shaderBlob->GetBufferPointer()), size);

	} else if (file.extension() == ".hlsl") {
		ComPtr<ID3DBlob> errorBlob;
		HRESULT hr = D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), target.c_str(), compileFlags, 0, m_shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		
		
		if (errorBlob != nullptr) {
			OutputDebugStringA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		CheckFailed(hr);

		std::ofstream out{ binpath , std::ios::binary };
		SIZE_T size = m_shaderBlob->GetBufferSize();
		out.write(reinterpret_cast<char*>(std::addressof(size)), sizeof(SIZE_T));
		out.write(reinterpret_cast<char*>(m_shaderBlob->GetBufferPointer()), m_shaderBlob->GetBufferSize());
		
	}
	else {
		throw std::runtime_error("Unsupported shader file type");
	}
}

Shader::~Shader()
{
}

D3D12_SHADER_BYTECODE Shader::GetByteCode() const {
	return D3D12_SHADER_BYTECODE{m_shaderBlob->GetBufferPointer(),m_shaderBlob->GetBufferSize()};
}
