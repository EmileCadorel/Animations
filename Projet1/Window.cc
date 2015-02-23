#include "Window.hh"
#include "ObjLoader.hh"
#include <ctime>

namespace O3D {

	bool KeyDown(int vk_code){ return (GetAsyncKeyState(vk_code) & 0x8000) ? true : false; }

	bool Window::init(const char * file) {
		m_file.load(file);
		bool re = initWindow();
		const Node & node = m_file.getNode("screen");
		int x = std::stoi(node.get_Value("width")), y = std::stoi(node.get_Value("height"));
		m_w = x; m_h = y;
		m_camera.init(x, y, "TPS");
		m_file.clear();

		return re;
	}


	void Window::initWorld() {
		m_timer.reset();

		if (!m_shader.init("Resources/FX-XML/skinning.xml", m_hwnd)) {
			MessageBox(m_hwnd, "Le shader a foire", "erreur", MB_OK);
			throw m_shader;
		}


		//on charge le modele ca prend 3 plombe
		//ca charge un mesh de type <SkinnedMesh>
		//en gros c'est position, normal, texture et indice pour les os
		//on charge aussi les information du squelette, elle vont dans data
		//ici on a une animation 'Take1' animer sur 75 frame avec 58 os
		//j'ai pas de modele plus simple ^^
		M3DLoader loader;
		loader.loadM3D("Resources/modeles/soldier.m3d", m_mesh, data);
		m_mesh.init();
	}

	/*init de dx pas important*/
	bool Window::initWindow() {
		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		const Node & node = m_file.getNode("screen"), node2 = m_file.getNode("cursor");

		int posX = std::stoi(node.get_Value("x")), posY = std::stoi(node.get_Value("y"));
		int w = std::stoi(node.get_Value("width")), h = std::stoi(node.get_Value("height"));
		
		RECT rc = { posX, posY, w, h };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		m_hinstance = GetModuleHandle(NULL);
		m_name = "Engine DX10";
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_name.c_str();
		wc.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wc);

		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_name.c_str(), m_name.c_str(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_BORDER, posX, posY, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, m_hinstance, NULL);
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);


		ShowCursor(std::stoi(node2.get_Value("show")));
		return true;
	}

	
	HWND * Window::getHWND() {
		return &m_hwnd;
	}

	Input & Window::getInput() {
		return m_input;
	}

	void Window::tick() {
		m_timer.tick();
	}

	//affichage dans la fenetre
	void Window::frame() {
		//on efface la fenetre et on affiche du blanc
		Singleton<Direct3D>::getInstance().beginScene({ 1.0f, 1.0f, 1.0f, 1.0f });
		
		static float t = 0.0f;

		if (KeyDown(VK_ADD)) {
			m_camera.dezoom();
		}

		WorldInfo info;
		m_camera.Render();
		D3DXMatrixIsIdentity(&info.world);
		info.view = m_camera.m_viewMatrix;
		info.projection = m_camera.m_projectionMatrix;
		info.ambiant = { 0.2f, 0.2f, 0.2f };
		info.eyePos = m_camera.getPosition();
		
		Light l;
		l.pos = { 1.0f, 0.0f, 0.0f };
		l.spotPower = 2.0f;
		std::vector<D3DXMATRIX> trans;


		//////
		////// On cree les transformations des os
		//////cf: Animation.cc
		data.getFinalTransformation("Take1", t, trans);
		
		//on les envoi au shader
		//cf Resources/FX/Skinning.fx
		HRESULT res = m_shader["boneTransforms"]->AsMatrix()->SetMatrixArray((float*)trans.data(), 0, trans.size());
		m_shader["nbBones"]->AsScalar()->SetInt(trans.size());
		

		/*lumiere et monde, pas important ca fonctionne ca*/
		l.passThrowShader(m_shader);
		info.passThrowShader(m_shader);

		//on affiche la modele
		m_mesh.passThrowShader(m_shader);
		

		//fin du rendu

		Singleton<Direct3D>::getInstance().endScene();
	}


	void Window::calculeFrameStats() {
		static int frame_count = 0;
		static float time_elapsed = 0.0f;

		frame_count++;
		if (m_timer.totalTime() - time_elapsed > 1.0f) {
			float fps = (float)frame_count;
			nb_fps = fps;
			std::ostringstream outs;
			outs << "FPS : " << fps;
			SetWindowText(m_hwnd, outs.str().c_str());
			frame_count = 0;
			time_elapsed += 1.0f;
		}
	}


	void Window::renderToShadowMap() {
		Singleton<Direct3D>::getInstance().resetOMTarget();
	}

	void Window::getMsg() {
		MSG msg;
		bool done = false, result;

		ZeroMemory(&msg, sizeof(MSG));
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}



	LRESULT CALLBACK Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
		switch (umsg) {
		
		case WM_KEYDOWN: {
							 m_input[(uint)wparam] = true;
							 return 0;
		}
		case WM_KEYUP: {
						   m_input[(uint)wparam] = false;
						   return 0;
		}
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}


	void Window::kill() {
		killWindow();
	}

	void Window::killWindow() {
		ShowCursor(true);
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;

		UnregisterClass(m_name.c_str(), m_hinstance);
		m_hinstance = NULL;
	}

	Window::~Window() {
		killWindow();
	}
}


static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage)
	{
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
					   return 0;
	}
	case WM_CLOSE:
	{
					 PostQuitMessage(0);
					 return 0;
	}
	default:
	{
			//envoi des message a la fenetre
			   return Singleton<O3D::Window>::getInstance().MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
