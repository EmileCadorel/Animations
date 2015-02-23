#pragma once

#include "XML.hh"
#include "Direct3D.hh"

#include <D3DX10.h>

#include <map>
#include <vector>

namespace O3D{

	class Shader {
	public:

		Shader();
		
		/*
			@brief Initialise un shader a partir d'un fichier XML de configuration
			@param const char * le nom du fichier XML
			@return bool
		*/
		bool init(const char * confFile, HWND hwnd);


		/*
			@brief Affiche des elements
			@param unsigned int le nombre d'index
			@return void
		*/
		void render(unsigned int nb, unsigned int faceStart);

		/*
			@brief Retourne la variable dont le nom est passe en param
			@param const char * le nom de la variable
			@return ID3D10EffectVariable * la variable, ou NULL dans le cas ou elle existe pas
		*/
		ID3D10EffectVariable * operator[](const char * name);


		Shader::~Shader();

	private:

		/*
			@brief Initialise le shader
			@param ID3D10Device * 
			@param HWND
			@param const char *
		*/
		bool initShader(ID3D10Device * device, HWND hwnd, const char * filename);

		/*
			@brief Init les variables du shader
			@brief elles sont reference dans le fichier XML
			@param void
			@return void
		*/
		bool initVars();


		/*
			@brief Ecris les erreur de compilations dans un fichier
			@param ID3D10Blob * blob
			@param HWND
			@param const char * le nom du fichier de shader "incompilable"
			@return void
		*/
		void compilationErrorMsg(ID3D10Blob * blob, HWND hwnd, const char * filename);


	private:


		ID3D10Effect * m_effect;
		ID3D10EffectTechnique * m_technique;
		ID3D10InputLayout * m_layout;

		std::map<std::string, ID3D10EffectVariable *> m_vars;


		XML m_file;

	};

}
