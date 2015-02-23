#pragma once

#include "Shader.hh"
#include <D3D10.h>
#include <D3DX10.h>


namespace O3D  {

	class Light {
	public:

		/*
			@brief Passe les informations du monde dans le shader
			@param Shader & le shader qui va afficher
			@return void
		*/
		void passThrowShader(Shader & shader);

		/*
			@brief Genere la matrice de vue
			@param void
			@return void
		*/
		void generateView();

		/*
			@brief Genere la matrice de projection
			@param void
			@return void
		*/
		void generateProjection();


		/*
			@brief Genere la matrice de projection de facon ortho
			@param void
			@return void
		*/
		void generateOrthoProjection(D3DXVECTOR2);


		/*
			@brief Genere une matrice de vue
			@param void
			@return D3DXMATRIX
		*/
		D3DXMATRIX getView();

		/*
			@brief Genere une matrice de projection
			@param void
			@return D3DXMATRIX
		*/
		D3DXMATRIX getProjection();

		
		D3DXVECTOR3 pos;
		D3DXVECTOR3 dir;
		D3DXVECTOR3 look;
		float spotPower;
		float range;
		D3DXMATRIX view;
		D3DXMATRIX projection;

		struct PassableLight {
			D3DXVECTOR3 pos;
			float pad;
			D3DXVECTOR3 dir;
			float spotPower;
			float range;
		};

	};

}