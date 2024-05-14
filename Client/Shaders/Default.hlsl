struct VS_IN
{
    float3 POSITION : POSITION;
    float3 NORMAL : NORMAL;
    float2 TEXCOORD : TEXCOORD;
    float4 COLOR : COLOR;
};

struct VS_OUT
{
    float4 Pos : SV_Position;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};

VS_OUT VS_Main(VS_IN Input)
{
    VS_OUT output = (VS_OUT) 0;
    
    output.Pos = float4(Input.POSITION, 1.f);
    output.Color = Input.COLOR;
    output.Normal = Input.NORMAL;
    output.Tex = Input.TEXCOORD;
    
    return output;
}