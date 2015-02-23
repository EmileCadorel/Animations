#pragma once

#include <map>


namespace O3D {

	typedef unsigned int uint;

	class Input {
	public:
		Input();

		/*
			@brief Renvoi le bool pour la modification
			@param unsigned int le numero a modifier
			@return bool &
		*/
		bool & operator[](uint param);

		/*
			@brief Renvoi le bool pour la consultation
			@param unsigned int 
			@return bool
		*/
		bool operator[](uint param) const;



		~Input();

	private:

		std::map<uint, bool> m_inputs;

	};

}