#include "pch.h"
#include "Graphics/GraphicsPipeline.h"
#include "Graphics/Shader.h"


void GraphicsPipeline::AttachShader(const Shader* shader, ShaderType type){
	switch (type)
	{
	case ShaderType::VertexShader:
		m_pipelineDesc.VS = shader->GetByteCode();
		break;
	case ShaderType::GeometryShader:
		m_pipelineDesc.GS = shader->GetByteCode();
		break;
	case ShaderType::HullShader:
		m_pipelineDesc.HS = shader->GetByteCode();
		break;
	case ShaderType::DomainShader:
		m_pipelineDesc.DS = shader->GetByteCode();
		break;
	case ShaderType::PixelShader:
		m_pipelineDesc.PS = shader->GetByteCode();
		break;
	case ShaderType::ShaderTypeCount:
		throw std::runtime_error{ "Undefined Behavior.\n" };
		break;
	default:
		break;
	}
}

ID3D12PipelineState* GraphicsPipeline::GetPipelineState() const noexcept {
	return m_pipelineState ? m_pipelineState.Get() : nullptr;
}

