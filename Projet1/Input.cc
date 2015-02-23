#include "Input.hh"

namespace O3D {
	Input::Input() {
	}

	bool & Input::operator[](uint param) {
		return m_inputs[param];
	}

	bool Input::operator[](unsigned int param) const {
		auto it = m_inputs.find(param);
		if (it == m_inputs.end()) {
			return false;
		}
		else {
			return true;
		}
	}

	Input::~Input()	{
	}
}
