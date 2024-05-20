#pragma once 


class GraphicsPipeline {
public:
	GraphicsPipeline();
	virtual ~GraphicsPipeline();
private:
	ComPtr<ID3D12PipelineState> m_pipelineState{ nullptr };
private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_pipelineDesc{ nullptr };
public:
	void AttachShader(const class Shader* shader,ShaderType type);
	ID3D12PipelineState* GetPipelineState() const noexcept;
};

