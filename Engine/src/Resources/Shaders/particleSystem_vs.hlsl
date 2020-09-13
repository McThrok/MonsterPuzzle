cbuffer cbView : register(b0) //Vertex Shader constant buffer slot 1
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix invViewMatrix;
	matrix projMatrix;
};

struct VSInput
{
	float3 pos : POSITION;
	float age : TEXCOORD0;
	float angle : TEXCOORD1;
	float size : TEXCOORD2;
	float ttl : TEXCOORD3;
	int version : TEXCOORD4;
};

struct GSInput
{
	float4 pos : POSITION;
	float age : TEXCOORD0;
	float angle : TEXCOORD1;
	float size : TEXCOORD2;
	float ttl : TEXCOORD3;
	int version : TEXCOORD4;
};

GSInput main(VSInput i)
{
	GSInput o = (GSInput)0;
	o.pos = float4(i.pos, 1.0f);
	o.pos = mul(viewMatrix, o.pos);
	o.age = i.age;
	o.angle = i.angle;
	o.size = i.size;
	o.ttl = i.ttl;
	o.version = i.version;

	return o;
}