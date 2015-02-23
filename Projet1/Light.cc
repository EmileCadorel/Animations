#include "Light.hh"


namespace O3D {

	void Light::passThrowShader(Shader & shader) {
		PassableLight pass = { pos, 0.0f, dir, spotPower, range };
		auto it = shader["light"];
		if (it) {
			it->SetRawValue(&pass, 0, sizeof(PassableLight));
		}
		it = shader["lview"];
		if (it) {
			it->AsMatrix()->SetMatrix((float*)&view);
		}
		it = shader["lprojection"];
		if (it) {
			it->AsMatrix()->SetMatrix((float*)&projection);
		}
	}

	void Light::generateView() {
		D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
		D3DXMatrixLookAtLH(&view, &pos, &look, &up);
	}

	D3DXMATRIX Light::getView() {		
		return view;
	}


	void Light::generateProjection() {
		float fieldOfView, screenAspect;
		fieldOfView = (float)D3DX_PI / 4.0f;
		screenAspect = 1.0f;

		D3DXMatrixPerspectiveFovLH(&projection, fieldOfView, screenAspect, 2.0f, 1000.0f);
	}


	void Light::generateOrthoProjection(D3DXVECTOR2 size) {
		float fieldOfView, screenAspect;

		D3DXMatrixOrthoLH(&projection, size.x, size.y, 0.1f, 100.0f);
	}


	D3DXMATRIX Light::getProjection() {
		return projection;
	}

}