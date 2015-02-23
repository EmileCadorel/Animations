#pragma once



#include "Singleton.hh"
#include "XML.hh"
#include "Direct3D.hh"
#include "Timer.hh"
#include "Camera.hh"
#include "Shader.hh"
#include "Font.hh"
#include "Input.hh"
#include "M3DLoader.hh"
#include "Mesh.hh"
#include "WorldInfo.hh"
#include "Light.hh"

#include <string>
#include <sstream>
#include <Windows.h>

namespace O3D {


	class Window : public Singleton<Window> {
	public:

		friend Singleton<Window>;

		/*
			@brief Creer la fenetre
			@param const char * le nom du fichier de config
			@return bool
		*/
		bool init(const char * confFile);

		/*
			@brief Initialise les shaders et les mesh
			@param void
			@return void
		*/
		void initWorld();


		/*
			@brief Retourne l'instance de hwnd
			@param void
			@return HWND *
		*/
		HWND * getHWND();


		/*
			@brief Retourne les input
			@param void
			@return Input &
		*/
		Input & getInput();



		/*
			@brief Affiche une frame
			@param void
			@return void
		*/
		void frame();

		/*
			@brief Fais avancer le timer
			@param void
			@return void
		*/
		void tick();


		/*
			@brief Recupere les signaux
			@param void
			@return void
		*/
		void getMsg();


		/*
			@brief calcule le framerate
			@param void
			@return void
		*/
		void calculeFrameStats();


		/*
			@brief Fait un rendu vers une texture de type shadow
			@param void
			@return void
		*/
		void renderToShadowMap();


		/*
			@brief Ferme la fenetre
			@param void
			@return void
		*/
		void kill();


		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);


		~Window();

	private:

		Window() {}
		Window(const Window &) {}
		Window & operator=(const Window &){}


	private:

		/*
			@brief Initialise la fenetre
			@param void
			@return bool
		*/
		bool initWindow();

		/*
			@brief ferme la fenetre
			@param void
			@return void
		*/
		void killWindow();


	private:

		HWND m_hwnd;
		HINSTANCE m_hinstance;
		XML m_file;

		int m_w, m_h;

		std::string m_name;
		Timer m_timer;
		Shader m_shader, m_toTex, m_shadow;
		Camera m_camera;
		float m_angle;
		Font m_font;
		int nb_fps;
		Input m_input;
		int color[3];
		Mesh<SkinnedVertex> m_mesh;
		SkinnedData data;
	};
}

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
