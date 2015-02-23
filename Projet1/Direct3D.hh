#pragma once


#include <D3D10.h>
#include <D3DX10.h>
#include <string>

#include "XML.hh"
#include "Singleton.hh"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")

namespace O3D {

	typedef unsigned int uint;



	class Direct3D : public Singleton<Direct3D> {
	public:

		friend Singleton<Direct3D>;

		/*
			@brief Initialize une device DirectX
			@param const char * le nom du fichier de configuration
			@return bool reussite ou echec
			*/
		bool init(const char * configFile, HWND * hwnd);


		/*
			@brief Demarre une scene, cad efface la fenetre
			@param D3DXVECTOR4 la couleur pour effacer
			@return void
			*/
		void beginScene(D3DXVECTOR4 color);


		/*
			@brief Met fin a la scene, change de buffer et affiche le dessin
			@param void
			@return void
			*/
		void endScene();


		/*
			@brief Retourne la device
			@param void
			@return ID3D10Device * la device
			*/
		ID3D10Device * getDevice();

		/*
			@brief Recupere les informations de la carte graphique, les retourne par reference
			@param std::string & le nom de la carte
			@param int & la memoire de la carte
			@return void
			*/
		void getVideoCardInfo(std::string & cardName, int & memory);


		/*
			@brief Remet le dessin sur la bonne target de sortie
			@param void
			@return void
		*/
		void resetOMTarget();


		/*
			@brief Reactive le test de profondeur
			@param void
			@return void
		*/
		void turnZBufferOn();


		/*
			@brief Ferme le device et la rend inutilisable sans nouvelle init
			@param void
			@return void
			*/
		void kill();


		/*
			@brief kill();
			*/
		~Direct3D();

	private:


		Direct3D() {}
		Direct3D(const Direct3D &) {}
		Direct3D & operator=(const Direct3D &) {}
		/*
			@brief Initialise les differents element de la classe
			@param void
			@return void
			*/
		HRESULT initSwapChain();
		HRESULT initRenderTargetView();
		HRESULT initDepthStencil();
		HRESULT initRasterState();
		HRESULT initViewPort();
		HRESULT initDepthStencilState();



	private:


		bool m_vsync_enabled;
		int m_videoCard_memory;
		char m_videoCard_name[128];

		D3D10_VIEWPORT m_viewport;
		IDXGISwapChain * m_swapChain;
		ID3D10Device * m_device;
		ID3D10RenderTargetView * m_renderTargetView;
		ID3D10Texture2D * m_depthStencilBuffer;
		ID3D10DepthStencilState * m_depthStencilState;
		ID3D10DepthStencilView * m_depthStencilView;
		ID3D10RasterizerState* m_rasterState;

		ID3D10DepthStencilState* m_depthDisabledStencilState;
		HWND * m_hwnd;


		XML m_file;
	};

};