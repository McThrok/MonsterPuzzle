cbuffer perObjectBuffer : register(b0)
{
	float4 color;
};
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

Texture2D tex : TEXTURE: register(t0);
SamplerState samp : SAMPLER: register(s0);

float4 main(PS_INPUT i) : SV_TARGET
{
	float4 texColor = tex.Sample(samp, i.tex);
	texColor *= color;
	return texColor;
}