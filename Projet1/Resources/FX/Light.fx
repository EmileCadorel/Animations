#include "Resources/FX/lightHelper.fx"

matrix world;
matrix view;
matrix projection;
float3 ambiant;
float3 eyePos;

Texture2D shaderTexture;


Material mater;
Light light;


SamplerState SampleType
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Border;
	AddressV = Border;
};

struct VertexInputType
{
	float4 position : POSITION;
	float3 normal : NORMALS;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

PixelInputType LightVertexShader(VertexInputType input) {
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.tex = input.tex;
	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	return output;
}

float4 LightPixelShader(PixelInputType input) : SV_Target
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	lightDir = -light.pos;
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f) {
		color += (float4(mater.diffuse, 1.0f) * lightIntensity);
	}

	color = saturate(color);
	color = color * textureColor;

	return color;
}

technique10 LightTechnique
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, LightVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, LightPixelShader()));
		SetGeometryShader(NULL);
	}
}
