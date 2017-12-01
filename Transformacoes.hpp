#include <cmath>
#include "Util.hpp"


namespace Transformacoes{

	enum class TipoTransformacao{
		ROTACAO,
		ESCALA
	};

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
