#pragma once

#include <D3D10.h>
#include <D3DX10.h>
#include "Shader.hh"

namespace O3D {

	class WorldInfo {
	public:
		
		/*
			@brief Passe les informations du monde dans le shader
			@param Shader & le shader qui va afficher
			@return void
		*/
		void passThrowShader(Shader & shader);


		D3DXMATRIX world;
		D3DXMATRIX projection;
		D3DXMATRIX view;
		D3DXVECTOR3 eyePos;
		D3DXVECTOR3 ambiant;
		D3DXVECTOR3 shadowColor;
	};

}