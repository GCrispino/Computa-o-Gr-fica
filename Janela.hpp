#include <iostream>
#include <vector>

class Janela{
	private:
		int xMin,yMin,xMax,yMax;
		std::vector<std::pair<int,int>> pontosBorda;
		std::vector<std::pair<int,int>> pontosDentro;

	public:
		Janela(int xMin, int yMin, int xMax, int yMax);
//		Janela(const std::pair<int, int> &p1, const std::pair<int, int> &p2);

		std::pair<std::pair<int, int>, std::pair<int, int>>
			cohenSutherland(const std::pair<int,int> &p1,const std::pair<int, int> &p2);

		std::string getCodigoCS(const std::pair<int,int> &p);
		std::pair<unsigned int, unsigned int> 
			getDiffBitCS(const std::string &c1Bin, const std::string &c2Bin);
		std::pair<std::pair<int, int>, std::pair<int, int>>
			getPontoInterseccaoCS(std::pair < unsigned int, unsigned int> infoDiffBit,
							  const std::pair<const std::pair<int, int> &, const std::string &> &p1Info,
							  const std::pair<const std::pair<int, int> &, const std::string &> &p2Info);

		void atualizaPontos();

		bool isPontoDentro(const std::pair<int,int> &);

		std::vector<std::pair<int, int>> getPontosBorda() const;
		std::vector<std::vector<double>> getMatrizPontosBorda();
		std::vector<std::pair<int, int>> getPontosDentro() const;
		std::vector<std::vector<double>> getMatrizPontosDentro();
		std::vector<std::vector<double>> getMatrizTodosPontos();
		int getXMin() const;
		int getYMin() const;
		int getXMax() const;
		int getYMax() const;

		void setXMin(int xMin);
		void setYMin(int yMin);
		void setXMax(int xMax);
		void setYMax(int yMax);
};