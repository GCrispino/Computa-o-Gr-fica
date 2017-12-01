#include <iostream>
#include <vector>

namespace Util{

	const double PI = 3.14159265;

	template <typename T>
	void troca(T &x, T &y){
		T aux = x;
		x = y;
		y = aux;
	}
	
	std::vector<std::vector<double>> multiplicaMatriz(const std::vector<std::vector<double>> &m1, const std::vector<std::vector<double>> &m2){
		unsigned int nLinhasM1 = m1.size(), nColunasM1 = m1[0].size();
		unsigned int nLinhasM2 = m2.size(), nColunasM2 = m2[0].size();

		if (nColunasM1 != nLinhasM2)
			return std::vector<std::vector<double>>{};

		std::vector<std::vector<double>> mResult(nLinhasM1);

		for (unsigned int i = 0; i < nLinhasM1; ++i){
			for (unsigned int k = 0; k < nColunasM2; ++k){

				double soma = 0.0;
				for (unsigned int j = 0; j < nColunasM1; ++j)
					soma += m1[i][j] * m2[j][k];

				mResult[i].push_back(soma);
			}
		}

		return mResult;
	}

	void imprimeMatriz(const std::vector<std::vector<double>> &matriz)	{
		for (auto &linha : matriz){
			for (float coluna : linha)
				std::cout << coluna << " ";

			std::cout << std::endl;
		}
	}
}