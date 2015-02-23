#include "Camera.hh"
using namespace std;

Camera::Camera() {
}


Camera::Camera(int x, int y, std::string type) {
	m_screen_x = x;
	m_screen_y = y;
	m_cibleX = 0.0f;
	m_cibleY = 0.0f;
	m_cibleZ = 0.0f;
	rayon = 2.0f;
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, D3DX_PI / 4.0f, (float)x/ (float)y, 0.1f, 1000.0f);	
	m_precise = 0.2f;
	m_teta = 60;
	m_phi = 0;
	m_type = type;
	m_vitesse = 5.0f/60.0f; // on est sur du 60 fps, on fait du 5 metre seconde
	POINT p;
	GetCursorPos(&p);
	anc_pos.x = p.x;
	anc_pos.y = p.y;
	Render();
	
}

void Camera::init(int x, int y, const char * type) {
	m_screen_x = x;
	m_screen_y = y;
	m_cibleX = 0.0f;
	m_cibleY = 30.0f;
	m_cibleZ = 0.0f;
	rayon = 120.0f;
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, D3DX_PI / 4.0f, (float)x / (float)y, 0.1f, 1000.0f);
	m_precise = 0.2f;
	m_teta = 60;
	m_phi = 0;
	m_type = type;
	m_vitesse = 5.0f / 60.0f; // on est sur du 60 fps, on fait du 5 metre seconde
	POINT p;
	GetCursorPos(&p);
	anc_pos.x = p.x;
	anc_pos.y = p.y;
	Render();
}

void Camera::set_cible(const D3DXVECTOR3 & position) {
	m_cibleX = position.x;
	m_cibleY = position.y;
	m_cibleZ = position.z;
}


void Camera::set_pos(const D3DXVECTOR3 & position) {
	m_positionX = position.x;
	m_positionY = position.y;
	m_positionZ = position.z;
}

int Camera::GetTeta() {
	return m_phi;
}


D3DXVECTOR3 Camera::getCible() const {
	return D3DXVECTOR3(m_cibleX, m_cibleY, m_cibleZ);
}

void Camera::updatePos() {
	POINT p;
	GetCursorPos(&p);
	anc_pos.x = p.x;
	anc_pos.y = p.y;
}


void Camera::Render() {

	POINT p;
	GetCursorPos(&p);
	int posX = anc_pos.x - p.x;
	int posY = anc_pos.y - p.y;
	if (p.x < 50) {
		p.x = m_screen_x;
	}
	if (p.x > m_screen_x - 10) {
		p.x = 60;
	}
	if (p.y < 50) {
		p.y = m_screen_y;
	}
	if (p.y > m_screen_y - 10) {
		p.y = 60;
	}

	anc_pos.x = p.x;
	anc_pos.y = p.y;
	
	SetCursorPos(p.x, p.y);

	if (m_type == "TPS") {
		m_phi = (m_phi + (int)(m_precise * posX)) % 360;
		m_teta = (m_teta - (int)(m_precise * posY)) % 360;
	}
	else {
		m_phi = (m_phi - (int)(m_precise * posX)) % 360;
		m_teta = (m_teta - (int)(m_precise * posY)) % 360;
	}
	
		if (m_teta < 10) {
			m_teta = 10;
		}
		else if (m_teta > 179) {
			m_teta = 179;
		}
	

	if (m_type == "TPS") {
		m_positionX = m_cibleX + rayon * sin(m_teta * M_PI / 180.0) * sin(m_phi * M_PI / 180.0);
		m_positionY = m_cibleY + rayon * cos(m_teta* M_PI / 180.0);
		m_positionZ = m_cibleZ + rayon * sin(m_teta * M_PI / 180.0) * cos(m_phi* M_PI / 180.0);
	}
	else if(m_type == "FPS") {
		m_cibleX = m_positionX + rayon * sin(m_teta * M_PI / 180.0) * sin(m_phi * M_PI / 180.0);
		m_cibleY = m_positionY + rayon * cos(m_teta* M_PI / 180.0);
		m_cibleZ = m_positionZ + rayon * sin(m_teta * M_PI / 180.0) * cos(m_phi* M_PI / 180.0);
	}

	D3DXMatrixLookAtLH(&m_viewMatrix,
		&D3DXVECTOR3(m_positionX, m_positionY, m_positionZ),    // the camera position
		&D3DXVECTOR3(m_cibleX, m_cibleY, m_cibleZ),      // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
}


D3DXVECTOR3 Camera::getPosition() const {
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

void Camera::zoom() {
	if (rayon > 0.1f) {
		rayon -= 0.1f;
	}
}

void Camera::dezoom() {
	rayon += 0.1f;
}

void Camera::up() {
	m_cibleY += 0.1f;
}

void Camera::down() {
	m_cibleY -= 0.1f;
}


void Camera::set_type(string type) {
	m_type = type;
}

Camera::~Camera() {
}
