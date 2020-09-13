cbuffer cbProj : register(b0) 
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix invViewMatrix;
	matrix projMatrix;
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

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 tex1: TEXCOORD0;
	float2 tex2: TEXCOORD1;
};

static const float TimeToLive = 4.0f;

[maxvertexcount(4)]
void main(point GSInput inArray[1], inout TriangleStream<PSInput> ostream)
{
	GSInput i = inArray[0];
	float sina, cosa;
	sincos(i.angle, sina, cosa);
	float dx = (cosa - sina) * 0.5 * i.size;
	float dy = (cosa + sina) * 0.5 * i.size;
	PSInput o = (PSInput)0;

	o.tex1.y = 0.5f;
	o.tex1.x = i.age / i.ttl;

	float2 offset = float2(0, 0);
	if (i.version == 1 || i.version == 3)
		offset.x += 0.5;
	if (i.version == 2 || i.version == 3)
		offset.y += 0.5;

	// TODO : 2.28 Initialize 4 vertices to make a bilboard and append them to the ostream

	o.pos = i.pos;
	o.pos += float4(-dy, dx, 0, 0);
	o.pos = mul(projMatrix, o.pos);
	o.tex2 = float2(0.5, 0.5) + offset;
	ostream.Append(o);

	o.pos = i.pos;
	o.pos += float4(dx, dy, 0, 0);
	o.pos = mul(projMatrix, o.pos);
	o.tex2 = float2(0.5, 0) + offset;
	ostream.Append(o);

	o.pos = i.pos;
	o.pos += float4(-dx, -dy, 0, 0);
	o.pos = mul(projMatrix, o.pos);
	o.tex2 = float2(0, 0.5) + offset;
	ostream.Append(o);

	o.pos = i.pos;
	o.pos += float4(dy, -dx, 0, 0);
	o.pos = mul(projMatrix, o.pos);
	o.tex2 = float2(0, 0) + offset;
	ostream.Append(o);

	ostream.RestartStrip();
}