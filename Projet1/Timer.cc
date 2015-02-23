#include "Timer.hh"

namespace O3D {

	Timer::Timer() : m_second_per_count(0.0), 
					 m_delta_time(0.0), 
					 m_base_time(0.0), 
					 m_pause_time(0.0), 
					 m_prev_time(0.0), 
					 m_curr_time(0.0), 
					 m_stop(false)	{
		__int64 count_per_sec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&count_per_sec);
		m_second_per_count = 1.0f / (double)count_per_sec;
	}


	void Timer::tick() {
		if (m_stop) {
			m_delta_time = 0.0;
		}
		else {
			__int64 curr_time;
			QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);
			m_curr_time = (double)curr_time;
			m_delta_time = (m_curr_time - m_prev_time) * m_second_per_count;
			m_prev_time = m_curr_time;

			if (m_delta_time < 0.0) {
				m_delta_time = 0.0;
			}
		}
	}


	float Timer::deltaTime() const {
		return (float)m_delta_time;
	}


	void Timer::reset() {
		__int64 curr_time;
		QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);
		m_base_time = (double)curr_time;
		m_prev_time = (double)curr_time;
		m_stop_time = 0.0;
		m_stop = false;
	}


	void Timer::start() {
		__int64 start_time;
		QueryPerformanceCounter((LARGE_INTEGER*)&start_time);

		if (m_stop) {
			m_pause_time += (start_time - m_stop_time);
			m_prev_time = start_time;
			m_stop_time = 0;
			m_stop = false;
		}
	}

	void Timer::stop() {
		if (!m_stop) {
			__int64 curr_time;
			QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);

			m_stop_time = curr_time;
			m_stop = true;
		}
	}


	float Timer::totalTime() const {
		if (m_stop) {
			return (float)(((m_stop_time - m_pause_time) - m_base_time) * m_second_per_count);
		}
		else {
			return (float)(((m_curr_time - m_pause_time) - m_base_time) * m_second_per_count);
		}
	}


	Timer::~Timer()	{
	}
}
