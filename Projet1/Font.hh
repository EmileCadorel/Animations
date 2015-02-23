#pragma once

#include "Direct3D.hh"

namespace O3D {

	class Font	{
	public:
		
		Font();

		/*
			@brief Creer une police 
			@param const char * le nom de la police
			@param int la taille de la police
			@return void
		*/
		void init(const char * fontName, int size, D3DXCOLOR color);

		/*
			@brief Ecris du texte a une position
			@param const char * le texte a ecrise
			@param D3DXVECTOR2 la position
			@return void
		*/
		void draw(const char * text, D3DXVECTOR2 pos);


		~Font();

	private:

		ID3DX10Font * m_font;
		int m_size;
		D3DXCOLOR m_color;
	};

}
