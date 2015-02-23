#include "Resources/FX/classicStruct.fx"


struct BasicPixel {
	float3 position;
	float3 normal;
};


float4 pointLight(Material mater, Light light, BasicPixel pix, float3 ambiant, float3 eyePos) {
	float4 litColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 lightVec = pix.position - light.pos;
	float d = length(lightVec);
	float4 spec;
	float3 reflection;
	litColor = float4(ambiant, 1.0f);

	lightVec = lightVec / d;
	float lightIntensity = saturate(dot(lightVec, pix.normal));
	if (lightIntensity > 0.0f) {
		litColor += float4(mater.diffuse, 1.0f) * light.spotPower * lightIntensity;
		litColor = saturate(litColor);
		float3 toEye = normalize(eyePos - pix.position);
		reflection = normalize(2 * lightIntensity * (pix.normal - lightVec));
		spec = pow(saturate(dot(reflection, toEye)), mater.specPower);
	}

	litColor = saturate(litColor + (spec * float4(mater.specular, 1.0f)));
	return litColor;
}

