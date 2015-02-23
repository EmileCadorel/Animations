#include "Resources/FX/lightHelper.fx"

matrix world;
matrix view;
matrix projection;
float3 ambiant;
float3 eyePos;

Light light;
Material mater;

Texture2D shaderTexture;

float4x4 boneTransforms[96];
uint nbBones;

SamplerState SampleType {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Border;
	AddressV = Border;
};

struct VS_IN {
	float3 pos : POSITION;
	float3 normal : NORMALS;
	float2 tex : TEXCOORD;
	float3 weights : WEIGHTS;
	float4 boneIndices : BONEINDICES;
};

struct VS_OUT {
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};


VS_OUT VertShader(VS_IN vIn) {

	VS_OUT vOut;

	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = vIn.weights.x;
	weights[1] = vIn.weights.y;
	weights[2] = vIn.weights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 2; i++) {
		posL += weights[i] * mul(float4(vIn.pos, 1.0f), boneTransforms[vIn.boneIndices[i]]).xyz;
		normalL += weights[i] * mul(vIn.normal, (float3x3)boneTransforms[vIn.boneIndices[i]]);
	}

	vOut.posW = mul(float4(posL, 1.0f), world).xyz;
	vOut.normal = mul(normalL, (float3x3)world);
	vOut.posH = mul(float4(vOut.posW, 1.0f), view);
	vOut.posH = mul(vOut.posH, projection);
	vOut.tex = vIn.tex;
	return vOut;
}

float4 PixShader(VS_OUT pIn) : SV_Target{
	pIn.normal = normalize(pIn.normal);

	float4 texCo = shaderTexture.Sample(SampleType, pIn.tex);

	BasicPixel pix = { pIn.posW, pIn.normal };

	float4 litColor = pointLight(mater, light, pix, ambiant, eyePos);
	litColor *= texCo;
	litColor.a = mater.alpha;
	return litColor;
}


technique10 SkinningTech {
	pass P0 {
		SetVertexShader(CompileShader(vs_4_0, VertShader()));
		SetPixelShader(CompileShader(ps_4_0, PixShader()));
		SetGeometryShader(NULL);
	}
}
