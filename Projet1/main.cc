#include <Windows.h>
#include <iostream>

#include "Application.hh"

using namespace std;

#pragma warning(disable : 4005)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	O3D::Application app("Resources/conf/test.conf");
	app.run();

	return 0;
}
