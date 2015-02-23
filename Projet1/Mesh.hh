#pragma once


#include "Direct3D.hh"
#include "Texture.hh"
#include "Material.hh"
#include <xnamath.h>

#include <vector>
#include <map>

namespace O3D {

	struct ColorVertex {
		D3DXVECTOR3 position;
		D3DXVECTOR3 color;
	};

	struct Vertex {
		D3DXVECTOR3 position;
		D3DXVECTOR3 normals;
		D3DXVECTOR2 tex;
	};

	struct SkinnedVertex {
		XMFLOAT3 position;
		XMFLOAT3 normals;
		XMFLOAT2 tex;
		XMFLOAT3 weights;
		XMFLOAT4 indices;
	};


	struct Subset {
		UINT ID;
		UINT VertexStart;
		UINT VertexCount;
		UINT FaceStart;
		UINT FaceCount;
	};



	template <typename TVertex>
	class Mesh {
	public:

		Mesh() {
			m_vboId = NULL;
		}

		/*
			@brief Initialise les vertexBuffer et les indexBuffer
			@param void
			@return void
		*/
		void init(){
			D3D10_BUFFER_DESC vertexBufferDesc;
			D3D10_SUBRESOURCE_DATA vertexData;
			HRESULT result;

			Direct3D & device = Singleton<Direct3D>::getInstance();

			vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(TVertex)* m_vertices.size();
			vertexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexData.pSysMem = m_vertices.data();

			result = device.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vboId);

			
			D3D10_BUFFER_DESC indexBufferDesc;
			D3D10_SUBRESOURCE_DATA indexData;

			indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned int)* m_indices.size();
			indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexData.pSysMem = m_indices.data();

			result = device.getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_iboId);

			for (auto it : m_maters) {

				if (it.second.tex_name != "") {
					std::string tex = (std::string("Resources/Textures/") + it.second.tex_name);
					m_textures[it.first].initialize(device.getDevice(), tex);
				}
			}

		}

		/*
			@brief Affiche un subset identifier par le nom de son materiel
			@param const char * le nom du subset
			@param D3D10_PRIMITIVE_TOPOLOGY (option) la topologie
			@return void
		*/
		void drawSubset(int name, D3D10_PRIMITIVE_TOPOLOGY topo = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
			ID3D10Device * device = Singleton<Direct3D>::getInstance().getDevice();
			auto sub = m_subsetTable.find(name);
			if (sub != m_subsetTable.end()) {
				unsigned int offset, stride;
				stride = sizeof(TVertex);
				offset = 0;
				device->IASetVertexBuffers(0, 1, &m_vboId, &stride, &offset);
				device->IASetIndexBuffer(m_iboId, DXGI_FORMAT_R32_UINT, 0);
				device->IASetPrimitiveTopology(topo);
			}
		}


		void passThrowShader(Shader & shader) {
			for (auto it : m_subsetTable) {
				m_textures[it.first].passThrowShader(shader);
				m_maters[it.first].passThrowShader(shader);
				drawSubset(it.first);
				shader.render(it.second.FaceCount * 3, it.second.FaceStart * 3);
			}
		}

		/*
			@brief Retourne l'ensemble de vertex
			@param void
			@return std::vector<TVertex> & 
		*/
		std::vector<TVertex> & getVertices()  {
			return m_vertices;
		}


		/*
			@brief Retourne l'ensemble de indices
			@param void
			@return std::vector<unsigned int> &
		*/
		std::vector<unsigned int> & getIndices()  {
			return m_indices;
		}

		/*
			@brief Retourne l'ensemble des subsets
			@param void
			@return std::map<std::string, Subset> & 
		*/
		std::map<int, Subset> & getSubset() {
			return m_subsetTable;
		}

		/*
			@brief Retourne l'ensemble des material
			@param void
			@return std::map<std::string, Material> &
		*/
		std::map<int, Material> & getMaterials() {
			return m_maters;
		}

		/*
			@brief Retourne l'ensemble de texture
			@param void
			@return std::map<std::string, Texture> &
		*/
		std::map < int, Texture> & getTextures() {
			return m_textures;
		}

		/*
			@brief Efface toute le donnees
			@param void
			@return void
		*/
		void clear() {
		
			if (m_iboId != NULL) {
				m_iboId->Release();
				m_iboId = NULL;
			}

			if (m_vboId != NULL) {
				m_vboId->Release();
				m_vboId = NULL;
			}

			m_indices.clear();
			m_vertices.clear();
			m_maters.clear();
			m_textures.clear();
		}

		/*
			@brief clear();
		*/
		Mesh::~Mesh() {
			clear();
		}

	private:

		std::vector<TVertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::map<int, Subset> m_subsetTable;

		ID3D10Buffer * m_vboId;
		ID3D10Buffer * m_iboId;

		std::map<int, Material> m_maters;
		std::map<int, Texture> m_textures;
	};

}