#pragma once
#include <d3dx10math.h>
#include <iostream>

#define M_PI 3.14159265358979323846

class Camera {
public:

	Camera();

	Camera(int, int, std::string type);
	~Camera();

	void init(int, int, const char * type);


	void Render();
	
	void set_cible(const D3DXVECTOR3 &);
	void set_pos(const D3DXVECTOR3 &);
	int GetTeta();

	void set_type(std::string type);
	
	void updatePos();

	void zoom();
	void dezoom();

	void up();
	void down();
	
	D3DXVECTOR3 getCible() const;
	D3DXVECTOR3 getPosition() const;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;


private:
	int m_phi, m_teta;
	float m_precise;
	int m_screen_x, m_screen_y;

	float m_positionX, m_positionY, m_positionZ;
	float m_cibleX, m_cibleY, m_cibleZ;
	float m_upX, m_upY, m_upZ;
	float rayon;

	std::string m_type;
	float m_vitesse;


	D3DXVECTOR2 anc_pos;
};
