#pragma once

#include "Mesh.hh"
#include "Animation.hh"

#include <fstream>

namespace O3D {

	struct M3DMaterial {
		Material mater;
		bool alphaClip;
		std::string EffectTypeName;
		std::string DiffuseMapName;
		std::string NormalMapName;
	};


	class M3DLoader {
	public:


		/*
			@brief Charge un Model M3D dans un mesh de type normal
			@param Mesh<SkinnedVertex> & le mesh
			@return bool
		*/
		bool loadM3D(const char * name, Mesh<Vertex> & mesh);

		/*
			@brief Charge un Model M3D dans un mesh de type skinned
			@param Mesh<SkinnedVertex> & le mesh
			@param SkinnedData & les information d'os a remplir
			@return bool
		*/
		bool loadM3D(const char * name, Mesh<SkinnedVertex> & mesh, SkinnedData & data);

	private:
		
		/*
			@brief Charge tout les materiaux qui sont dans le fichier
			@param std::ifstream & le fichier
			@param UINT le nombre de materiaux
			@param std::vector<M3DMaterial> & le materiaux a remplir
			@return void
		*/
		void readMaterial(std::ifstream & fin, UINT nbMaters, std::map<int, Material> & mats);

		/*
			@brief Lis les informations sur les subsets
			@param std::ifstream & le fichier
			@param UINT le nombre de subset
			@param std::vector<Subset> & 
			@return void
		*/
		void readSubsetTable(std::ifstream& fin, UINT numSubsets, std::map<int, Subset>& subsets);

		/*
		@brief Lis les informations sur les vertices
		@param std::ifstream & le fichier
		@param UINT le nombre de vertices
		@param std::vector<Vertex> &
		@return void
		*/
		void readVertices(std::ifstream& fin, UINT numVertices, std::vector<Vertex> & vertices);


		/*
			@brief Lis les informations sur les vertices
			@param std::ifstream & le fichier
			@param UINT le nombre de vertices
			@param std::vector<SkinnedVertex> &
			@return void
		*/
		void readSkinnedVertices(std::ifstream& fin, UINT numVertices, std::vector<SkinnedVertex> & vertices);
		

		/*
			@brief Lis les informations sur les triangles
			@param std::ifstream & le fichier
			@param UINT le nombre de triangles
			@param std::vector<unsigned int> &
			@return void
		*/
		void readTriangles(std::ifstream& fin, UINT numTriangles, std::vector<unsigned int>& indices);
		

		/*
			@brief Lis les informations sur les os
			@param std::ifstream & le fichier
			@param UINT le nombre d'os
			@param std::vector<XMFLOAT4X4> &
			@return void
		*/
		void readBoneOffsets(std::ifstream& fin, UINT numBones, std::vector<XMFLOAT4X4>& boneOffsets);
		

		/*
			@brief Lis les informations sur la hierarchie des os
			@param std::ifstream & le fichier
			@param UINT le nombre d'os
			@param std::vector<int> &
			@return void
		*/
		void readBoneHierarchy(std::ifstream& fin, UINT numBones, std::vector<int>& boneIndexToParentIndex);
		
		
		/*
			@brief Lis les differentes animations stocker dans le fichier
			@param std::ifstream & le fichier
			@param UINT le nombre d'os
			@param UINT le nombre d'animation
			@param std::map<std::string, AnimationClip>& les animations a remplir
			@return void
		*/
		void readAnimationClips(std::ifstream& fin, UINT numBones, UINT numAnimationClips, std::map<std::string, AnimationClip>& animations);
		

		/*
			@brief Lis les keyFrame de tous les os
			@param std::ifstream & le fichier
			@param UINT le nombre d'os
			@param BoneAnimation & l'animation d'os a remplir
			@return void
		*/
		void readBoneKeyframes(std::ifstream& fin, UINT numBones, BoneAnimation& boneAnimation);
		

	};

}


