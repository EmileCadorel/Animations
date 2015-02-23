#pragma once

template <class T>
class Singleton {

public:

	static T & getInstance() {
		return instance;
	}

protected:

	void operator=(const Singleton<T> &) {}

	static T instance;

};

template <class T>
T Singleton<T>::instance;