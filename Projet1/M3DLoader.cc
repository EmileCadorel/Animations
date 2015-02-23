#include "M3DLoader.hh"
using namespace std;


namespace O3D {

	bool M3DLoader::loadM3D(const char * name, Mesh<SkinnedVertex> & mesh, SkinnedData & data) {
		ifstream fin(name);

		UINT numMaterials, numVertices, numTriangles, numBones, numAnimClips;
		std::string ignore;

		if (fin.is_open()) {
			fin >> ignore >> ignore;
			fin >> numMaterials >> ignore;
			fin >> numVertices >> ignore;
			fin >> numTriangles >> ignore;
			fin >> numBones >> ignore;
			fin >> numAnimClips;


			std::vector<XMFLOAT4X4> boneOffsets;
			std::vector<int> boneIndexToParentIndex;
			std::map<std::string, AnimationClip> animations;


			readMaterial(fin, numMaterials, mesh.getMaterials());
			readSubsetTable(fin, numMaterials, mesh.getSubset());
			readSkinnedVertices(fin, numVertices, mesh.getVertices());
			readTriangles(fin, numTriangles, mesh.getIndices());
			readBoneOffsets(fin, numBones, boneOffsets);
			readBoneHierarchy(fin, numBones, boneIndexToParentIndex);
			readAnimationClips(fin, numBones, numAnimClips, animations);

			data.set(boneIndexToParentIndex, boneOffsets, animations);

			fin.close();
			return true;
		}
		return false;
	}



	bool M3DLoader::loadM3D(const char * name, Mesh<Vertex> & mesh) {
		ifstream fin(name);

		UINT numMaterials, numVertices, numTriangles, numBones, numAnimClips;
		std::string ignore;

		if (fin.is_open()) {
			fin >> ignore >> ignore;
			fin >> numMaterials >> ignore;
			fin >> numVertices >> ignore;
			fin >> numTriangles >> ignore;
			fin >> numBones >> ignore;
			fin >> numAnimClips;

			readMaterial(fin, numMaterials, mesh.getMaterials());
			readSubsetTable(fin, numMaterials, mesh.getSubset());
			readVertices(fin, numVertices, mesh.getVertices());
			readTriangles(fin, numTriangles, mesh.getIndices());

			fin.close();
			return true;
		}
		return false;
	}



	void M3DLoader::readMaterial(ifstream & fin, UINT num, std::map<int, Material> & maters) {
		string ignore;
		float ig;
		fin >> ignore;
		for (int i = 0; (int)i < num; i++) {
			auto & mat = maters[i];
			fin >> ignore >> mat.ambiant.x >> mat.ambiant.y >> mat.ambiant.z;
			fin >> ignore >> mat.diffuse.x >> mat.diffuse.y >> mat.diffuse.z;
			fin >> ignore >> mat.specular.x >> mat.specular.y >> mat.specular.z;
			fin >> ignore >> mat.specPower;
			fin >> ignore >> ig >> ig >> ig; //reflection
			fin >> ignore >> mat.alpha;
			fin >> ignore >> ignore; //shader name
			fin >> ignore >> mat.tex_name;
			fin >> ignore >> ignore; // normal tex
		}
	}


	void M3DLoader::readSubsetTable(ifstream & fin, UINT num, std::map<int, Subset> & sub) {
		string ignore;
		fin >> ignore;
		for (int i = 0; i < (int)num; i++) {
			auto & subset = sub[i];
			fin >> ignore >> subset.ID;
			fin >> ignore >> subset.VertexStart;
			fin >> ignore >> subset.VertexCount;
			fin >> ignore >> subset.FaceStart;
			fin >> ignore >> subset.FaceCount;
		}
	}


	void M3DLoader::readVertices(std::ifstream& fin, UINT numVertices, std::vector<Vertex>& vertices) {
		std::string ignore;
		vertices.resize(numVertices);
		float ig;
		fin >> ignore;
		for (UINT i = 0; i < numVertices; ++i)
		{
			fin >> ignore >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
			fin >> ignore >> ig >> ig >> ig >> ig;
			fin >> ignore >> vertices[i].normals.x >> vertices[i].normals.y >> vertices[i].normals.z;
			fin >> ignore >> vertices[i].tex.x >> vertices[i].tex.y;
		}
	}


	void M3DLoader::readSkinnedVertices(ifstream & fin, UINT num, std::vector<SkinnedVertex> & verts) {
		string ignore;
		verts.resize(num);
		int boneIndice[4];
		float weight[4];
		float ig;

		fin >> ignore;
		for (int i = 0; i < (int)num; i++) {
			fin >> ignore >> verts[i].position.x >> verts[i].position.y >> verts[i].position.z;
			fin >> ignore >> ig >> ig >> ig >> ig; // tangent
			fin >> ignore >> verts[i].normals.x >> verts[i].normals.y >> verts[i].normals.z;
			fin >> ignore >> verts[i].tex.x >> verts[i].tex.y;
			fin >> ignore >> weight[0] >> weight[1] >> weight[2] >> weight[3];
			fin >> ignore >> boneIndice[0] >> boneIndice[1] >> boneIndice[2] >> boneIndice[3];

			verts[i].weights.x = weight[0];
			verts[i].weights.y = weight[1];
			verts[i].weights.z = weight[2];

			verts[i].indices.x = boneIndice[0];
			verts[i].indices.y = boneIndice[1];
			verts[i].indices.z = boneIndice[2];
			verts[i].indices.w = boneIndice[3];
		}
	}

	void M3DLoader::readTriangles(ifstream & fin, UINT num, vector<unsigned int> & indices) {
		string ignore;
		indices.resize(num * 3);

		fin >> ignore;
		for (int i = 0; i < (int)num; i++) {
			fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
		}
	}

	void M3DLoader::readBoneOffsets(ifstream & fin, UINT num, vector<XMFLOAT4X4> & offsets) {
		string ignore;
		offsets.resize(num);

		fin >> ignore;
		for (int i = 0; i < num; i++) {
			fin >> ignore >>
				offsets[i](0, 0) >> offsets[i](0, 1) >> offsets[i](0, 2) >> offsets[i](0, 3) >>
				offsets[i](1, 0) >> offsets[i](0, 1) >> offsets[i](1, 2) >> offsets[i](1, 3) >>
				offsets[i](2, 0) >> offsets[i](2, 1) >> offsets[i](2, 2) >> offsets[i](2, 3) >>
				offsets[i](3, 0) >> offsets[i](3, 1) >> offsets[i](3, 2) >> offsets[i](3, 3);
		}

	}

	void M3DLoader::readBoneHierarchy(ifstream & fin, UINT num, vector<int>& boneIndices) {
		string ignore;
		boneIndices.resize(num);
		fin >> ignore;
		for (int i = 0; i < num; i++) {
			fin >> ignore >> boneIndices[i];
		}
	}



	void M3DLoader::readAnimationClips(ifstream & fin, UINT numBone, UINT numAnim, map<string, AnimationClip> & animations) {
		string ignore;
		fin >> ignore;

		for (int i = 0; i < (int)numAnim; i++) {
			string clipName;
			fin >> ignore >> clipName;
			fin >> ignore;
			
			AnimationClip clip;
			clip.m_boneAnimations.resize(numBone);
			for (int j = 0; j < (int)numBone; j++) {
				readBoneKeyframes(fin, numBone, clip.m_boneAnimations[j]);
			}
			fin >> ignore;
			animations[clipName] = clip;
		}
	}


	void M3DLoader::readBoneKeyframes(ifstream & fin, UINT num, BoneAnimation & anim) {
		string ignore;
		UINT numKeyFrame;
		fin >> ignore >> ignore >> numKeyFrame;
		fin >> ignore; // {
		anim.m_keys.resize(numKeyFrame);
		for (int i = 0; i < (int)numKeyFrame; i++) {
			float t = 0.0f;

			XMFLOAT3 p(0.0f, 0.0f, 0.0f);
			XMFLOAT3 s(1.0f, 1.0f, 1.0f);
			XMFLOAT4 q(0.0f, 0.0f, 0.0f, 1.0f);

			fin >> ignore >> t;
			fin >> ignore >> p.x >> p.y >> p.z;
			fin >> ignore >> s.x >> s.y >> s.z;
			fin >> ignore >> q.x >> q.y >> q.z >> q.w;

			anim.m_keys[i].timePos = t;
			anim.m_keys[i].rotationQuat = q;
			anim.m_keys[i].translation = p;
			anim.m_keys[i].scale = s;

		}
		fin >> ignore; // }
	}



}