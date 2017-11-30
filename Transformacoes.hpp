#include <cmath>
#include "Util.hpp"


namespace Transformacoes{

	enum class TipoTransformacao{
		ROTACAO,
		ESCALA
	};

	std::vector<std::vector<double>> getMatRotacao(int ang){
		std::vector<std::vector<double>> matRotacao = {
			{cos(ang),-sin(ang)},
			{sin(ang),cos(ang)}
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

	std::vector<std::vector<double>> transforma(std::vector<std::vector<double>> pontos,TipoTransformacao tipo){

		std::vector<std::vector<double>> matrizEscolhida;

		switch(tipo){
			case TipoTransformacao::ROTACAO:
				matrizEscolhida = getMatRotacao();
				break;
			case TipoTransformacao::ESCALA:
				matrizEscolhida = getMatEscala();
				break;
			
		}

		return Util::multiplicaMatriz(matrizEscolhida,pontos);
	}


}
