#include "Shader.hh"

namespace O3D {

	Shader::Shader() {
		m_effect = NULL;
		m_technique = NULL;
		m_layout = NULL;
	}


	bool Shader::init(const char * filename, HWND hwnd) {
		m_file.load(filename);
		const Node & shad = m_file.getNode("Shader");
		bool compile = initShader(Singleton<Direct3D>::getInstance().getDevice(), hwnd, shad.get_Value("File").c_str());
		if (!compile) { 
			m_file.clear();
			return false; 
		}

		bool vars = initVars();
		if (!vars) { 
			m_file.clear();
			return false; 
		}

		m_file.clear();

		return true;
	}


	bool Shader::initShader(ID3D10Device * device, HWND hwnd, const char * filename) {
		HRESULT result = S_OK;
		ID3D10Blob * error = NULL;
		unsigned int numElements;
		D3D10_PASS_DESC passDesc;


		result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS,	0, device, NULL, NULL, &m_effect, &error, NULL);

		if (FAILED(result)) {
			if (error) {
				compilationErrorMsg(error, hwnd, filename);
			}
			else {
				MessageBox(hwnd, filename, "Missing Shader file", MB_OK);
			}
			return false;
		}
		m_technique = m_effect->GetTechniqueByName(m_file.getNode("Tech").get_Value("name").c_str());

		std::vector<D3D10_INPUT_ELEMENT_DESC> polygonLayout;
		const Node & layout = m_file.getNode("Layout");
		for (int i = 0; i < (int)layout.m_childs.size(); i++) {
			std::string * name = new std::string(layout.m_childs[i]->get_Value("name"));
			if (i == 0) {
				polygonLayout.push_back({ name->c_str() , 0,
					DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 });
			}
			else {
				polygonLayout.push_back({ name->c_str(), 0,
					DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 });
			}
		}


		numElements = polygonLayout.size();
		result = m_technique->GetPassByIndex(0)->GetDesc(&passDesc);
		if (FAILED(result)) { return false; }

		result = device->CreateInputLayout(polygonLayout.data(), numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_layout);
		if (FAILED(result)) { return false; }
		return true;
	}



	bool Shader::initVars() {
		const Node & variable = m_file.getNode("Variables");
		for (auto & it : variable.m_childs) {
			std::string type = it->get_Value("type");
			std::string name = it->get_Value("name");
			if (type == "mat") {
				m_vars[name] = m_effect->GetVariableByName(name.c_str())->AsMatrix();
			}
			else if (type == "vec") {
				m_vars[name] = m_effect->GetVariableByName(name.c_str())->AsVector();
			}
			else if (type == "scalar") {
				m_vars[name] = m_effect->GetVariableByName(name.c_str())->AsScalar();
			}
			else if (type == "resource") {
				m_vars[name] = m_effect->GetVariableByName(name.c_str())->AsShaderResource();
			}
			else {
				m_vars[name] = m_effect->GetVariableByName(name.c_str());
			}
			if (FAILED(m_vars[name]->IsValid())) { return false; }
		}
		return true;
	}


	void Shader::render(unsigned int nb, unsigned int faceStart) {
		D3D10_TECHNIQUE_DESC techDesc;
		ID3D10Device * device = Singleton<Direct3D>::getInstance().getDevice();

		device->IASetInputLayout(m_layout);
		m_technique->GetDesc(&techDesc);
		for (int i = 0; i < techDesc.Passes; i++) {
			m_technique->GetPassByIndex(i)->Apply(0);
			device->DrawIndexed(nb, faceStart, 0);
		}
	}



	void Shader::compilationErrorMsg(ID3D10Blob * errorMessage, HWND hwnd, const char * filename) {
		char* compileErrors;
		unsigned long bufferSize, i;
		std::ofstream fout;

		compileErrors = (char*)(errorMessage->GetBufferPointer());
		bufferSize = errorMessage->GetBufferSize();
		fout.open("OUTS/shader-error.txt");
		for (i = 0; i<bufferSize; i++) {
			fout << compileErrors[i];
		}
		fout.close();

		errorMessage->Release();
		errorMessage = 0;
		MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", filename, MB_OK);
	}



	ID3D10EffectVariable * Shader::operator[](const char * name) {
		auto & it = m_vars.find(name);
		if (it != m_vars.end()) {
			return it->second;
		} else {
			return NULL;
		}
	}


	Shader::~Shader() {
	}
}
