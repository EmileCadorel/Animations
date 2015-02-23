#include "Material.hh"

namespace O3D {

	void Material::passThrowShader(Shader & shader) {
		Passable pass = { ambiant, 0.0f, diffuse, 0.0f, specular, specPower, alpha };
		if (shader["mater"]) {
			shader["mater"]->SetRawValue(&pass, 0, sizeof(Passable));
		}
	}

}