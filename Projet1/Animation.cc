#include "Animation.hh"


/*
	Pour tout les XM... cf doc xnamath.h
*/

namespace O3D {

	using namespace std;

	KeyFrame::KeyFrame() : 	timePos(0.0f), 
							translation(0.0f, 0.0f, 0.0f), 
							scale(1.0f, 1.0f, 1.0f), 
							rotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}



	float BoneAnimation::getStartTime() const {
		return m_keys.front().timePos;
	}


	float BoneAnimation::getEndTime() const {
		return m_keys.back().timePos;
	}

	//interpolation entre le deux frame d'un os
	void BoneAnimation::interpolate(float t, XMFLOAT4X4 & M) const {
		if (t <= m_keys.front().timePos) {
			XMVECTOR S = XMLoadFloat3(&m_keys.front().scale);
			XMVECTOR P = XMLoadFloat3(&m_keys.front().translation);
			XMVECTOR Q = XMLoadFloat4(&m_keys.front().rotationQuat);

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
		}
		else if (t >= m_keys.back().timePos) {
			XMVECTOR S = XMLoadFloat3(&m_keys.back().scale);
			XMVECTOR P = XMLoadFloat3(&m_keys.back().translation);
			XMVECTOR Q = XMLoadFloat4(&m_keys.back().rotationQuat);

			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
		}
		//la vrai interpolation
		else {
			for (int i = 0; i < (int)m_keys.size() - 1; i++) {
				if (t >= m_keys[i].timePos && t <= m_keys[i + 1].timePos) {
					float lerpPercent = (t - m_keys[i].timePos) / (m_keys[i + 1].timePos - m_keys[i].timePos);
					
					XMVECTOR S0 = XMLoadFloat3(&m_keys[i].scale);
					XMVECTOR S1 = XMLoadFloat3(&m_keys[i + 1].scale);

					XMVECTOR p0 = XMLoadFloat3(&m_keys[i].translation);
					XMVECTOR p1 = XMLoadFloat3(&m_keys[i + 1].translation);

					XMVECTOR q0 = XMLoadFloat4(&m_keys[i].rotationQuat);
					XMVECTOR q1 = XMLoadFloat4(&m_keys[i + 1].rotationQuat);

					XMVECTOR S = XMVectorLerp(S0, S1, lerpPercent);
					XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
					XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

					XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
					XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
					
					break;
				}
			}
		}
	}


	float AnimationClip::getClipStartTime() const {
		float t = FLT_MAX;
		for (int i = 0; i < (int)m_boneAnimations.size(); i++) {
			t = min(t, m_boneAnimations[i].getStartTime());
		}
		return t;
	}

	float AnimationClip::getClipEndTime() const {
		float t = 0.0f;
		for (int i = 0; i < (int)m_boneAnimations.size(); i++) {
			t = max(t, m_boneAnimations[i].getEndTime());
		}
		return t;
	}

	//on calcule la transformation de tout les os de l'animation
	void AnimationClip::interpolate(float t, vector<XMFLOAT4X4> & boneTrans) const {
		for (int i = 0; i < (int)m_boneAnimations.size(); i++) {
			m_boneAnimations[i].interpolate(t, boneTrans[i]);
		}
	}

	UINT SkinnedData::BoneCount() const {
		return m_boneHier.size();
	}


	float SkinnedData::getClipStartTime(const char * name) const {
		auto clip = m_animations.find(name);
		if (clip != m_animations.end()) {
			return clip->second.getClipStartTime();
		}
		else {
			return FLT_MAX;
		}
	}


	float SkinnedData::getClipEndTime(const char * name) const {
		auto clip = m_animations.find(name);
		if (clip != m_animations.end()) {
			return clip->second.getClipEndTime();
		}
		else {
			return 0.0f;
		}
	}

	//au chargement, on enregistre les informations d'animations
	void SkinnedData::set(vector<int> & boneHier, vector<XMFLOAT4X4> & boneOff, map<string, AnimationClip> & animations) {
		m_boneHier = boneHier;
		m_boneOffsets = boneOff;
		m_animations = animations;
	}

	void copy4x4toD3D(XMFLOAT4X4 & x4, D3DXMATRIX & d3d) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				d3d(i, j) = x4(i, j);
			}
		}
	}

	//la fonction est appele a chaque frame
	void SkinnedData::getFinalTransformation(const char * name, float timePos, vector<D3DXMATRIX> & finalTrans) const {
		
		UINT numBones = m_boneOffsets.size();
		finalTrans.resize(numBones);
		std::vector<XMFLOAT4X4> toParentTransforms(numBones);
		auto clip = m_animations.find(name);
		clip->second.interpolate(timePos, toParentTransforms);

		std::vector<XMFLOAT4X4> toRootTransforms(numBones);

		toRootTransforms[0] = toParentTransforms[0];

		for (UINT i = 1; i < numBones; ++i)
		{
			XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[i]);

			int parentIndex = m_boneHier[i];
			XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

			XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

			XMStoreFloat4x4(&toRootTransforms[i], toRoot);
		}

		for (UINT i = 0; i < numBones; ++i)
		{
			XMMATRIX offset = XMLoadFloat4x4(&m_boneOffsets[i]);
			XMMATRIX toRoot = XMLoadFloat4x4(&toRootTransforms[i]);
			XMFLOAT4X4 tmp;
			XMStoreFloat4x4(&tmp, XMMatrixMultiply(offset, toRoot));

			//pour passer dans le shader 
			//on a besoin que la matrice soit du type D3DXMATRIX
			copy4x4toD3D(tmp, finalTrans[i]);
		}
	}


}