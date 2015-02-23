#pragma once

#include <map>
#include "Direct3D.hh"
#include <xnamath.h>


#include <vector>

namespace O3D {

	struct KeyFrame {

		KeyFrame();


		float timePos;
		XMFLOAT3 translation;
		XMFLOAT3 scale;
		XMFLOAT4 rotationQuat;
	};


	struct BoneAnimation {

		/*
			@brief Retourne le temps de la premiere frame
			@param void
			@return float
		*/
		float getStartTime() const;

		/*
			@brief Retourne le temps de la derniere frame
			@param void
			@return float
		*/
		float getEndTime() const;

		/*
			@brief Calcule une matrice interpoler entre 2 frame
			@param float  le temps
			@param XMFLOAT4X4 & la matrice a generer
			@return void
		*/
		void interpolate(float t, XMFLOAT4X4 & M) const;


		std::vector<KeyFrame> m_keys;

	};



	struct AnimationClip {

		/*
			@brief Retourne le temps de la premiere frame
			@param void
			@return float
		*/
		float getClipStartTime() const;

		/*
			@brief Retourne le temps de la derniere frame
			@param void
			@return float
		*/
		float getClipEndTime() const;

		/*
			@breif Retourne la liste des transformation des os
			@param float le temps
			@param std::vector<XMFLOAT4X4> & la liste a generer
			@return void
		*/
		void interpolate(float t, std::vector<XMFLOAT4X4> & boneTransforms) const;


		std::vector<BoneAnimation> m_boneAnimations;

	};



	class SkinnedData {
	public:


		UINT BoneCount() const;

		/*
			@brief retourne le temps de la premiere frame de l'animation
			@param const char * le nom de l'animation
			@return float
		*/
		float getClipStartTime(const char * name) const;

		/*
			@brief retourne le temps de la derniere frame de l'animation
			@param const char * le nom de l'animation
			@return float
		*/
		float getClipEndTime(const char * name) const;

		/*
			@brief Initialise les informations de tout ca
			@param std::vecto<int> & la hierarchie
			@param std::vector<XMFLOAT4X4> & les position des os
			@param std::map<std::string, AnimationClip>& les animations
			@return void
		*/
		void set(std::vector<int> & boneHier, std::vector<XMFLOAT4X4> & boneOffsets, std::map<std::string, AnimationClip> & animations);

		/*
			@brief Recupere une frame avec toute les matrices des os bien comme il faut
			@param const char * le nom de l'animation
			@param float timePos le temps pour le choix de la frame
			@param std::vector<XMFLOAT4X4> & le transformation des os a set
			@return void
		*/
		void getFinalTransformation(const char * name, float timepos, std::vector<D3DXMATRIX> & finalTrans) const;


	private:

		std::vector<int> m_boneHier;
		std::vector<XMFLOAT4X4> m_boneOffsets;
		std::map<std::string, AnimationClip> m_animations;

	};


}

