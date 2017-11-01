#include <vector>
#include <stack>
#include "Quadrado.hpp"

enum class TrocaReflexao {NAO_TROCA,TROCA_X,TROCA_Y,TROCA_XY};

class Grid{
	private:
		int altura,
			largura,
			tamanhoQuadrados,
			xInicial,
			yInicial;

		double corLinha[3];

		std::vector< std::vector<Quadrado> > quadrados;
		std::vector< std::vector< std::vector <double> > > frameBuffer;

	public:
		Grid(int altura,int largura,int tamanhoQuadrados,int xInicial,int yInicial,double corLinha[3]);
		Grid(const Grid &);
		void criaLinhaQuadrados(int xGrid,int xReal);
		void pintaQuadrado(int xGrid, int yGrid);
		void pintaQuadrado(const Quadrado &);
		void pintaFrameBuffer(double cor[],int x,int y);
		
		int * mapCoordenadaGridParaReal(int x, int y);
		int * mapCoordenadaRealParaGrid(int x, int y);


		const Quadrado & getQuadrado(int xGrid, int yGrid);


		static void troca(int &, int &);
		static std::stack<TrocaReflexao> reflexao(std::pair<int,int> &p1,std::pair<int,int> &p2);
		static void reflexaoInversa(std::stack<TrocaReflexao> tiposTroca,int &x, int &y);
		static std::vector< std::pair<int,int> > bresenham(int x1, int y1,int x2, int y2);

};

