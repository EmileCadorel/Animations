#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <sstream>

#include "Mesh.hh"

namespace O3D {

	class ObjLoader {

	public:
		ObjLoader();

		void import(const char * filename);
		void to_mesh(Mesh<Vertex> & mesh);

		~ObjLoader();


		struct int3 {
			int x, y, z;
		};

		struct Indices {
			std::vector<int3> vert;
			std::vector<int3> tex;
			std::vector<int3> norm;
		};


		std::vector<D3DXVECTOR3> vertices;
		std::vector<D3DXVECTOR2> texture;
		std::vector<D3DXVECTOR3> Normal;

		struct Obj {
			std::map<std::string, std::map<std::string, Indices>> indices;

			std::string name;
		};

		void read_file(FILE * file);
		void read_mtl(const char * file_name);
		void to_mesh(const std::string & name, Obj & obj, Mesh<Vertex> & mesh);
		void addVertToMesh(Mesh<Vertex> & mesh, int ind, Vertex & vert, const char * name);

		std::map<std::string, Obj> mesh;
		std::map<std::string, Material> mat;

		std::string racine;

	};

}