#include "Font.hh"

namespace O3D {

	Font::Font() : m_font(NULL) {
	}


	void Font::init(const char * fontName, int size, D3DXCOLOR color) {
		D3DX10_FONT_DESC FontDesc;
		FontDesc.Height = size;
		FontDesc.Width = 0;
		FontDesc.Weight = 0;
		FontDesc.MipLevels = 1;
		FontDesc.Italic = false;
		FontDesc.CharSet = OUT_DEFAULT_PRECIS;
		FontDesc.Quality = DEFAULT_QUALITY;
		FontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(FontDesc.FaceName, fontName);

		ID3D10Device * device = Singleton<Direct3D>::getInstance().getDevice();
		D3DX10CreateFontIndirect(device, &FontDesc, &m_font);

		m_size = size;
		m_color = color;
	}

	void Font::draw(const char * text, D3DXVECTOR2 pos) {
		if (m_font) {
			RECT fontRect = { pos.x, pos.y, 0, 0 };
			m_font->DrawText(0, text, -1, &fontRect, DT_NOCLIP, m_color);
		}
	}


	Font::~Font() {
		if (m_font) {
			m_font->Release();
		}
	}



}