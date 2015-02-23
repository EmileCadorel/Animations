#include "Application.hh"

namespace O3D {

	Application::Application(const char * confFile) : m_window(Singleton<Window>::getInstance()),
													  m_d3d(Singleton<Direct3D>::getInstance()) {
		m_window.init(confFile);
		m_d3d.init(confFile, m_window.getHWND());

		m_window.initWorld();
	
	}

	void Application::run() {
		bool finish = false;
		while (!finish) {
			m_window.getMsg();
			if (m_window.getInput()[VK_ESCAPE]) {
				break;
			}
			else {
				m_window.tick();
				m_window.calculeFrameStats();
				m_window.frame();
			}
		}
	}



	Application::~Application()	{
	}

}
