#include "ObjLoader.hh"
using namespace std;

#pragma warning(disable : 4996)

namespace O3D {

	ObjLoader::ObjLoader() {
	}


	void ObjLoader::import(const char * filename) {
		FILE * file = fopen(filename, "r");
		auto name = string(filename);
		auto pos = name.find_last_of("/");
		racine = name.substr(0, pos + 1);
		read_file(file);
		fclose(file);
	}

	void ObjLoader::to_mesh(Mesh<Vertex> & _mesh) {
		int i = 0;
		for (auto it : mesh) {
			to_mesh(it.first, it.second, _mesh);
			i++;
		}
	}

	void ObjLoader::read_mtl(const char * filename) {
	
		FILE * file = fopen((racine + string(filename)).c_str(), "r");
		string line_type = " ", current_mtl;
		char line[256], mtl[256];
		Material * mater = NULL;
		char ignore[256];
		while (line_type != "\0") {
			fscanf(file, "%s", line);
			line_type = line;
			strcpy(line, "");
			if (line_type == "#" || line_type == "illum" || line_type == "Ni") {
				fgets(ignore, 256, file);
			}
			else if (line_type == "d") {
				fscanf(file, "%f", &mater->alpha);
			}
			else if (line_type == "newmtl") {
				fscanf(file, "%s", mtl);
				current_mtl = mtl;
				mater = &mat[mtl];
			}
			else if (line_type == "Kd") {
				fscanf(file, "%f %f %f", &mater->diffuse.x, &mater->diffuse.y, &mater->diffuse.z);
			}
			else if (line_type == "Ka") {
				fscanf(file, "%f %f %f", &mater->ambiant.x, &mater->ambiant.y, &mater->ambiant.z);
			}
			else if (line_type == "Ks") {
				fscanf(file, "%f %f %f", &mater->specular.x, &mater->specular.y, &mater->specular.z);
			}
			else if (line_type == "Ns") {
				fscanf(file, "%f", &mater->specPower);
			}
			else if (line_type == "map_Kd") {
				char filename[256];
				fgets(filename, 256, file);
				string name = filename;
				mater->tex_name = name.substr(1, name.length() - 2);
			}
			else if (line_type == "map_bump") {
				char filename[256];
				fgets(filename, 256, file);
				string name = filename;
				//mater->bump_name = name.substr(1, name.length() - 2);
			}
			else {
				break;
			}
		}
	}


	void ObjLoader::read_file(FILE * file) {
		char type[256], mtl[256], current_obj[256], current_mat[256], current_bone[256];
		char ignore[256];
		string line_type = "p";
		Obj * obj = NULL;
		Indices * ind;
		if (file == NULL) {
			return;
		}
		while (line_type != "\0") {
			fscanf(file, "%s", type);
			line_type = type;
			strcpy(type, "");
			if (line_type != "\0") {
				if (line_type == "#" || line_type == "s") {
					fgets(ignore, 256, file);
				}
				else if (line_type == "mtllib") {
					fscanf(file, "%s", mtl);
					read_mtl(mtl);
				}
				else if (line_type == "o") {
					fscanf(file, "%s", current_obj);
					mesh[current_obj].name = current_obj;
					obj = &mesh[current_obj];
				}
				else if (line_type == "g") {
					fscanf(file, "%s", current_bone);
					//ind = &obj->indices[current_mat][current_bone];
				}
				else if (line_type == "v") {
					D3DXVECTOR3 v;
					fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
					v.z *= -1.0f;
					vertices.push_back(v);
				}
				else if (line_type == "vn") {
					D3DXVECTOR3 n;
					fscanf(file, "%f %f %f", &n.x, &n.y, &n.z);
					Normal.push_back(n);
				}
				else if (line_type == "vt") {
					D3DXVECTOR2 t;
					fscanf(file, "%f %f", &t.x, &t.y);
					t.y = 1.0f - t.y;
					texture.push_back(t);
				}
				else if (line_type == "usemtl") {
					fscanf(file, "%s", current_mat);
					ind = &obj->indices[current_mat][current_bone];
				}
				else if (line_type == "f") {
					//on pars du principe qu'il y a les tex et les normal
					int3 vert, tex, norm;
					fscanf(file, "%i/%i/%i", &vert.x, &tex.x, &norm.x);
					fscanf(file, "%i/%i/%i", &vert.y, &tex.y, &norm.y);
					fscanf(file, "%i/%i/%i", &vert.z, &tex.z, &norm.z);
					ind->vert.push_back(vert);
					ind->tex.push_back(tex);
					ind->norm.push_back(norm);
				}
				else {
					break;
				}
			}
		}
	}


	void ObjLoader::addVertToMesh(Mesh<Vertex> & mesh, int index, Vertex & vert, const char * name) {
		if (index < (int)mesh.getVertices().size()) {
			Vertex & vert_ = mesh.getVertices()[index];
			if (vert.position == vert_.position && vert.tex == vert_.tex && vert.normals == vert_.normals) {
				mesh.getIndices().push_back(index);
			}
			else {
				mesh.getVertices().push_back(vert);
				mesh.getIndices().push_back(mesh.getVertices().size() - 1);
			}
		}
		else {
			mesh.getVertices().push_back(vert);
			mesh.getIndices().push_back(mesh.getVertices().size() - 1);
		}

	}



	void ObjLoader::to_mesh(const std::string & obj_name, Obj & obj, Mesh<Vertex> & out) {
		int boneIndex = 0;
		int id = 0;
		int sub = 0, deb_sub = 0;
		for (auto mat : mesh[obj_name].indices) {
			deb_sub = sub;
			for (auto bone : mat.second) {
				for (int i = 0; i < (int)bone.second.vert.size(); i++) {
					D3DXVECTOR3 pos, pos2, pos3;
					pos = vertices[bone.second.vert[i].x - 1];
					pos2 = vertices[bone.second.vert[i].y - 1];
					pos3 = vertices[bone.second.vert[i].z - 1];

					D3DXVECTOR2 tex, tex2, tex3;
					tex = texture[bone.second.tex[i].x - 1];
					tex2 = texture[bone.second.tex[i].y - 1];
					tex3 = texture[bone.second.tex[i].z - 1];


					D3DXVECTOR3 norm, norm2, norm3;
					norm = Normal[bone.second.norm[i].x - 1];
					norm2 = Normal[bone.second.norm[i].y - 1];
					norm3 = Normal[bone.second.norm[i].z - 1];


					Vertex vert1 = { pos3, norm3, tex3 }, vert2 = { pos2, norm2, tex2 }, vert3 = { pos, norm, tex };
					addVertToMesh(out, bone.second.norm[i].z - 1, vert1, mat.first.c_str());
					addVertToMesh(out, bone.second.norm[i].y - 1, vert2, mat.first.c_str());
					addVertToMesh(out, bone.second.norm[i].x - 1, vert3, mat.first.c_str());
					sub++;
				}
				boneIndex++;
			}
			out.getMaterials()[id] = (this->mat[mat.first]);
			out.getSubset()[id] = { id, 0, 0, deb_sub, sub - deb_sub};
			id++;
		}
	}

	ObjLoader::~ObjLoader() {
	}

}