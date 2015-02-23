#include "WorldInfo.hh"

namespace O3D {

	void WorldInfo::passThrowShader(Shader & shader) {
		shader["world"]->AsMatrix()->SetMatrix(world);
		shader["projection"]->AsMatrix()->SetMatrix(projection);
		shader["view"]->AsMatrix()->SetMatrix(view);

		if (shader["eyePos"]) 
			shader["eyePos"]->AsVector()->SetFloatVector((float*)&eyePos);
		if (shader["ambiant"])
			shader["ambiant"]->AsVector()->SetFloatVector((float*)&ambiant);
		if (shader["shadowColor"])
			shader["shadowColor"]->AsVector()->SetFloatVector((float*)&shadowColor);
	}

}
