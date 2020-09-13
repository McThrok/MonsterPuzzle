cbuffer perObjectBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix invViewMatrix;
	matrix projMatrix;
}; 

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;

	//o.pos = mul(worldMatrix, float4(i.pos, 1.0f));
	//o.pos = mul(viewMatrix, o.pos);
	//o.pos = mul(projMatrix, o.pos);


	float3 worldPos = mul(worldMatrix, float4(i.pos, 1.0f)).xyz;

	o.pos = mul(viewMatrix, float4(worldPos, 1.0f));
	o.pos = mul(projMatrix, o.pos);
	o.tex = i.tex;

	return o;
}