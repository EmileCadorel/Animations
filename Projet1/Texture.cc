#include "Texture.hh"
#include "Window.hh"

namespace O3D {

	Texture::Texture() {
		m_texture = NULL;
	}


	bool Texture::initialize(ID3D10Device * device, std::string filename) {
		HRESULT result = D3DX10CreateShaderResourceViewFromFile(device, filename.c_str(), NULL, NULL, &m_texture, NULL);
		if (FAILED(result)) {
			MessageBox(*Singleton<Window>::getInstance().getHWND(), (std::string("fail to load texture :\n") + filename).c_str(), "error", MB_OK);
			return false;
		}
		return true;
	}

	void Texture::passThrowShader(Shader & shader) {
		shader["shaderTexture"]->AsShaderResource()->SetResource(m_texture);
	}


	ID3D10ShaderResourceView * Texture::getTexture() {
		return m_texture;
	}


	void Texture::shutdown() {
		if (m_texture != NULL) {
			m_texture->Release();
			m_texture = NULL;
		}
	}

	Texture::~Texture()	{
	}
}
