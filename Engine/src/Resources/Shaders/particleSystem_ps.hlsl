Texture2D colorMap : register(t0);
Texture2D opacityMap : register(t1);
SamplerState colorSampler : register(s0);

cbuffer perObjectBuffer : register(b0)
{
	float4 color;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 tex1: TEXCOORD0;
	float2 tex2: TEXCOORD1;
};

float4 main(PSInput i) : SV_TARGET
{
	float4 col = colorMap.Sample(colorSampler, i.tex1);
	float4 opacity = opacityMap.Sample(colorSampler, i.tex2);
	col *= col;
	col.a *= opacity.a;

	if (col.a == 0.0f)
		discard;

	return col;
};