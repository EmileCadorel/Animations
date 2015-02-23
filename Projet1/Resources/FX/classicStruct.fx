
struct Material {
	float3 ambiant;
	float3 diffuse;
	float3 specular;
	float specPower;
	float alpha;
};

struct Light {
	float3 pos;
	float3 dir;
	float spotPower;
	float range;
};