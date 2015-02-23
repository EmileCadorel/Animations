#pragma once

#include "Window.hh"
#include "Direct3D.hh"
#include "Timer.hh"

#include <sstream>

namespace O3D {

	class Application {
	public:

		Application(const char * confFile);
		
		/*
			@brief Boucle principale
			@param void
			@return void
		*/
		void run();


		~Application();

	private:

		Window & m_window;
		Direct3D & m_d3d;
		Timer m_timer;
	};

}