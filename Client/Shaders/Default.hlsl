struct VS_IN
{
    float3 POSITION : POSITION;
    float3 NORMAL : NORMAL;
    float2 TEXCOORD : TEXCOORD;
 //   matrix World : WORLDMATRIX;
 //   uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 Pos : SV_Position;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;

};



SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

cbuffer cbPerObject : register(b0) {
    matrix World;
}

cbuffer cbPerPass : register(b1)
{
    matrix ViewProj;
}



Texture2D Diffuse : register(t0);
cbuffer cbMaterial : register(b2){
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
    float4x4 gMatTransform;
};


VS_OUT VS_Main(VS_IN Input)
{
    VS_OUT output = (VS_OUT) 0;
    
    output.Pos = mul(float4(Input.POSITION, 1.0f), ViewProj);
    output.Normal = Input.NORMAL;
    output.Tex = Input.TEXCOORD;
   
    return output;
}

float4 PS_Main(VS_OUT Input) : SV_Target
{
    return float4(0.5f, 0.5f, 0.5f, 1.f);
}