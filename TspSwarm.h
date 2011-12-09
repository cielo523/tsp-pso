#pragma once

#include "stdafx.h"

#include "Particle.h"
#include "Features.h"
#include "Graph.h"
#include "Algo.h"

namespace WMH {
	namespace PSO {
		/** Klasa reprezentujaca roj */
		class TspSwarm : public iAlgo {
			friend class Particle;
			/** Maksymalna liczba iteracji ktora nie poprawia globalnego wyniku */
			int NOCHANGE_MAX;
			/** Zablokowany - jakis problem trzeba rozwiazywac */
			TspSwarm() {}
			/** Czas obliczen w ms */
			DWORD computationTime;
		protected:
			/** Graf dla ktorego przeprowadzane sa obliczenia */
			Graph*	g;
			/** List czastek */
			std::vector<Particle> particles;
			/** Najlepsze znalezione rozwiazanie (globalne) */
			Features	best;
			/** Miara dopasowania najlepszego rozwiazania */
			float		bestFit;
			/** Liczba iteracji ktora nie poprawila wyniku */
			int			noChange;
		public:

			/** Tworzy roj rozwiazujacy TSP dla grafu g */
			TspSwarm(Graph* g, unsigned int particlesCount, int maxNoChange = 1000);
			
			/** Rozpoczyna obliczenia */
			void compute();
			
			/** Zwraca koszt najlepszego rozwiazania */
			inline float getBestCost() const
			{
				return bestFit;
			}
			
			/** Zwraca najlepsze rozwiazanie */
			inline std::vector<int> getBestSolution() const
			{
				return best.toPermut();
			}
			
			/** Zwraca nazwe algorytmu */
			inline const char* getAlgorithmName() const
			{
				return "PSO\t\t";
			}

			/** Zwraca czas obliczen w ms */
			inline DWORD getComputationTime() const
			{
				return computationTime;
			}
		};
	}; // namespace PSO
}; // namespace TSP