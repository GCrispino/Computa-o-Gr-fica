#include <cmath>
#include "Util.hpp"


namespace Transformacoes{

	enum class TipoTransformacao{
		ROTACAO,
		ESCALA
	};

	std::vector<std::vector<double>> convertePontosParaMatPontos(std::vector<std::pair<int,int>> &pontos){
		std::vector<std::vector<double>> matPontos(2);

		for (auto &ponto: pontos){
			matPontos[0].push_back(ponto.first);
			matPontos[1].push_back(ponto.second);
		}

		return matPontos;
	}


	std::vector<std::vector<double>> getMatRotacao(double ang){
		double angRad = ang * Util::PI / 180.0;

		std::vector<std::vector<double>> matRotacao = {
			{cos(angRad),-sin(angRad)},
			{sin(angRad),cos(angRad)}
		};

		return matRotacao;
	}

	std::vector<std::vector<double>> getMatEscala(double fatorX, double fatorY){
		std::vector<std::vector<double>> matEscala = {
			{fatorX, 0},
			{0, fatorY}
		};

		return matEscala;
	}

	// std::vector<std::vector<double>> transforma(std::vector<std::vector<double>> pontos,TipoTransformacao tipo, double argMatriz){

	// 	std::vector<std::vector<double>> matrizEscolhida;

	// 	switch(tipo){
	// 		case TipoTransformacao::ROTACAO:
	// 			matrizEscolhida = getMatRotacao(argMatriz);
	// 			break;
	// 		case TipoTransformacao::ESCALA:
	// 			matrizEscolhida = getMatEscala(argMatriz);
	// 			break;
			
	// 	}

	// 	return Util::multiplicaMatriz(matrizEscolhida,pontos);
	// }


}
