#pragma once


#include <D3DX10.h>
#include "Shader.hh"

namespace O3D {
	class Material {
	public:

		/*
			@brief Passe le material dans un shader
			@param Shader & 
			@return void
		*/
		void passThrowShader(Shader & shader);


		
		D3DXVECTOR3 ambiant;
		D3DXVECTOR3 diffuse;
		D3DXVECTOR3 specular;
		float specPower;
		float alpha;
		std::string tex_name;

		struct Passable {
			D3DXVECTOR3 ambiant;
			float pad1;
			D3DXVECTOR3 diffuse;
			float pad2;
			D3DXVECTOR3 specular;
			float specPower;
			float alpha;
		};

	};
}

