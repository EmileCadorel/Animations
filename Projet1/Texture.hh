#pragma once

#include <iostream>
#include <D3D10.h>
#include <D3DX10.h>
#include "Shader.hh"

namespace O3D {

	class Texture {
	public:

		/*
			\brief Met la resources a NULL
			*/
		Texture();

		/*
			\brief charge la texture
			\param ID3D10Device * la device de l'application
			\param const char * le chemin de la texture
			\return bool si la chargement a reussi
			*/
		bool initialize(ID3D10Device * device, std::string);

		/*
			\brief retourne le pointeur vers la texture
			\param void
			return ID3D10ShaderResourceView * la texture
			*/
		ID3D10ShaderResourceView * getTexture();


		/*
			@brief Passe la texture dans le shader
			@param Shader & 
			@return void
		*/
		void passThrowShader(Shader & shader);


		/*
			\brief libere la texture
		*/
		void shutdown();

		/*
			\brief ne fait rien
			*/
		~Texture();

	private:

		ID3D10ShaderResourceView * m_texture;

	};

}