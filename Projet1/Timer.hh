#pragma once

#include <windows.h>


namespace O3D {


	class Timer	{
	public:
		Timer();

		/*
			@brief recupere le temps sans les pause
			@param void
			@return float
		*/
		float time() const;

		/*
			@brief Rcupere le temps passer entre 2 tick
			@param void
			@return float
		*/
		float deltaTime() const;

		/*
			@brief Reinitialise le compteur
			@param void
			@return void
		*/
		void reset();

		/*
			@brief lance le compteur
			@param void
			@return void
		*/
		void start();


		/*
			@brief met le compteur en pause
			@param void
			@return void
		*/
		void stop();

		/*
			@brief met a jour le compteur
			@param void
			@return void
		*/
		void tick();


		/*
			@brief Retourne le temps entre le dernier tick et le reset-start
			@param void
			@return float
		*/
		float totalTime() const;


		~Timer();


	private:

		double m_second_per_count;
		double m_delta_time;

		double m_base_time;
		double m_pause_time;
		double m_stop_time;
		double m_prev_time;
		double m_curr_time;

		bool m_stop;
	};


}
