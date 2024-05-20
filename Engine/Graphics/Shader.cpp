#include "pch.h"
#include "Graphics/Shader.h"

Shader::Shader(fs::path file,const std::string& entry,const std::string& target) {

	if (!fs::exists(file)) {
		throw std::runtime_error("Shader file not found");
	}
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

		//std::ofstream out{ binpath , std::ios::binary };
		//SIZE_T size = m_shaderBlob->GetBufferSize();
		//out.write(reinterpret_cast<char*>(std::addressof(size)), sizeof(SIZE_T));
		//out.write(reinterpret_cast<char*>(m_shaderBlob->GetBufferPointer()), m_shaderBlob->GetBufferSize());
		
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ShadingModel::ShadingModel(){
	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy
	

	m_staticSamplers[0] = pointWrap;
	m_staticSamplers[1] = pointClamp;
	m_staticSamplers[2] = linearWrap;
	m_staticSamplers[3] = linearClamp;
	m_staticSamplers[4] = anisotropicWrap;
	m_staticSamplers[5] = anisotropicClamp;

}

ShadingModel::~ShadingModel()
{
}

void ShadingModel::BuildRootSignatrue(const D3D12_ROOT_SIGNATURE_DESC* rootSignatrueDesc){
	ComPtr<ID3D10Blob> errorBlob{};

	auto hr = ::D3D12SerializeRootSignature(rootSignatrueDesc, D3D_ROOT_SIGNATURE_VERSION_1, m_serializedRootSignature.GetAddressOf(), errorBlob.GetAddressOf());

	if (FAILED(hr)) {
		OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
		throw std::runtime_error{ "Build Root Sigantrue Error!\n" };
		PostQuitMessage(0);
	}
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


DefaultShading::DefaultShading(ID3D12Device* device, ID3D12GraphicsCommandList* commandList) : ShadingModel(){


	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.NumStaticSamplers = m_staticSamplers.size();
	desc.pStaticSamplers = m_staticSamplers.data();

	CD3DX12_ROOT_PARAMETER rootparameters[3]{};

	D3D12_DESCRIPTOR_RANGE ranges[2]{};

	ranges[0].BaseShaderRegister = 0;
	ranges[0].NumDescriptors = 1;
	ranges[0].OffsetInDescriptorsFromTableStart = 0;
	ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

	ranges[1].BaseShaderRegister = 2;
	ranges[1].NumDescriptors = 1;
	ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	
	rootparameters[0].InitAsConstants(16, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX); // World Matrix ( b0 )
	rootparameters[1].InitAsConstants(16, 1, 0, D3D12_SHADER_VISIBILITY_VERTEX); // View * Projection Matrix ( b1 ) 
	rootparameters[2].InitAsDescriptorTable(1, std::addressof(ranges[0]), D3D12_SHADER_VISIBILITY_PIXEL); // Material Descriptor Table [ Material Diffuse ( t0 ) , Material Constants ( b2 ) ]	 

	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	desc.NumParameters = _countof(rootparameters);
	desc.pParameters = rootparameters;


	D3D12_INPUT_LAYOUT_DESC inputlayout{};
	D3D12_INPUT_ELEMENT_DESC inputelements[4]{};

	inputelements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputelements[1] = { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputelements[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputelements[3] = { "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	inputlayout.NumElements = _countof(inputelements);
	inputlayout.pInputElementDescs = inputelements;

	ShadingModel::BuildRootSignatrue(std::addressof(desc));		
	CheckFailed(
		device->CreateRootSignature(0, m_serializedRootSignature->GetBufferPointer(), m_serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(m_rootSignatrue.GetAddressOf())));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.pRootSignature = m_rootSignatrue.Get();
	pipelineDesc.InputLayout = inputlayout;
	pipelineDesc.NodeMask = 0;
	pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT{});
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC{CD3DX12_DEFAULT{}};
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC{ CD3DX12_DEFAULT{} };
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.SampleDesc.Count = 1;
	pipelineDesc.SampleDesc.Quality = 0;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	Shader VS{ L"./Shaders//Default.hlsl", "VS_Main", "vs_5_1" };
	Shader PS{ L"./Shaders//Default.hlsl", "PS_Main", "ps_5_1" };

	pipelineDesc.VS = VS.GetByteCode();
	pipelineDesc.PS = PS.GetByteCode();

	CheckFailed(device->CreateGraphicsPipelineState(std::addressof(pipelineDesc), IID_PPV_ARGS(m_pipelineState.GetAddressOf())));

}

DefaultShading::~DefaultShading(){

}


void DefaultShading::Bind(ID3D12GraphicsCommandList* commandlist){
	commandlist->SetPipelineState(m_pipelineState.Get());
}

extern UINT msaaq;

DefaultInstancingShading::DefaultInstancingShading(ID3D12Device* device,ID3D12GraphicsCommandList* commandList){

	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.NumStaticSamplers = m_staticSamplers.size();
	desc.pStaticSamplers = m_staticSamplers.data();

	CD3DX12_ROOT_PARAMETER rootparameters[3]{};

	D3D12_DESCRIPTOR_RANGE ranges[2]{};

	ranges[0].BaseShaderRegister = 0;
	ranges[0].NumDescriptors = 1;
	ranges[0].OffsetInDescriptorsFromTableStart = 0;
	ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

	ranges[1].BaseShaderRegister = 2;
	ranges[1].NumDescriptors = 1;
	ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

	rootparameters[0].InitAsConstants(16, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX); // World Matrix ( b0 )
	rootparameters[1].InitAsConstants(16, 1, 0, D3D12_SHADER_VISIBILITY_VERTEX); // View * Projection Matrix ( b1 ) 
	rootparameters[2].InitAsDescriptorTable(1, std::addressof(ranges[0]), D3D12_SHADER_VISIBILITY_PIXEL); // Material Descriptor Table [ Material Diffuse ( t0 ) , Material Constants ( b2 ) ]	 

	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	desc.NumParameters = _countof(rootparameters);
	desc.pParameters = rootparameters;

	ShadingModel::BuildRootSignatrue(std::addressof(desc));
	CheckFailed(
		device->CreateRootSignature(0, m_serializedRootSignature->GetBufferPointer(), m_serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(m_rootSignatrue.GetAddressOf())));


	D3D12_INPUT_LAYOUT_DESC inputlayout{};
	D3D12_INPUT_ELEMENT_DESC inputelements[3]{};

	inputelements[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputelements[1] = { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputelements[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	
	
	//// 인스턴싱 데이터, 행렬은 R32G32B32A32 가 4개 있어야 4x4 행렬이 완성되므로, 이처럼 InputLayout 을 구성해야 한다. 
	//inputelements[4] = 
	//{ "WORLDMATRIX",0,DXGI_FORMAT_R32G32B32A32_FLOAT,1,0,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1 };
	//inputelements[5] = 
	//{ "WORLDMATRIX",1,DXGI_FORMAT_R32G32B32A32_FLOAT,1,16,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1 };
	//inputelements[6] = 
	//{ "WORLDMATRIX",2,DXGI_FORMAT_R32G32B32A32_FLOAT,1,32,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1 };
	//inputelements[7] = 
	//{ "WORLDMATRIX",3,DXGI_FORMAT_R32G32B32A32_FLOAT,1,48,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1 };



	inputlayout.NumElements = _countof(inputelements);
	inputlayout.pInputElementDescs = inputelements;


	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	
	pipelineDesc.pRootSignature = m_rootSignatrue.Get();
	pipelineDesc.InputLayout = inputlayout;
	pipelineDesc.NodeMask = 0;
	pipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT{});
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC{ CD3DX12_DEFAULT{} };
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC{ CD3DX12_DEFAULT{} };
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.SampleDesc.Count = 4;
	pipelineDesc.SampleDesc.Quality = msaaq - 1;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	Shader VS{ L"./Shaders//Default.hlsl", "VS_Main", "vs_5_1" };
	Shader PS{ L"./Shaders//Default.hlsl", "PS_Main", "ps_5_1" };

	pipelineDesc.VS = VS.GetByteCode();
	pipelineDesc.PS = PS.GetByteCode();

	CheckFailed(device->CreateGraphicsPipelineState(std::addressof(pipelineDesc), IID_PPV_ARGS(m_pipelineState.GetAddressOf())));


}

DefaultInstancingShading::~DefaultInstancingShading()
{
}

void DefaultInstancingShading::Bind(ID3D12GraphicsCommandList* commandList){
	commandList->SetGraphicsRootSignature(m_rootSignatrue.Get());
	commandList->SetPipelineState(m_pipelineState.Get());
}
