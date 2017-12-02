#include <vector>
#include <map>
#include <stack>
#include "Quadrado.hpp"
#include "Janela.hpp"

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
		std::vector<std::pair<int, int>> pintaLinha(std::pair<int, int> &primeiroPonto, std::pair<int, int> &segundoPonto);
		void pintaJanela(const Janela &);
		void pintaFrameBuffer(const double cor[],int x,int y);
		void apagaPontos(const std::vector<std::pair<int,int>> &pontos);
		void limpa();
		
		int * mapCoordenadaGridParaReal(int x, int y);
		int * mapCoordenadaRealParaGrid(int x, int y);


		Quadrado & getQuadrado(int xGrid, int yGrid);
		const std::vector<double> getCorFrameBuffer(int x, int y);

		static std::pair<int, int> getPontoYMin(const std::pair<std::pair<int, int>, std::pair<int, int>> &lado);
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> 
			getLados(int yVarredura, const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &lados, std::map<std::pair<int, int>, bool> &ladosMinMax);
		void preenchimentoVarredura(const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &lados);
		void preenchimentoRecursivo(int x, int y, const double * corPonto,const double * corInicial);

		std::vector<std::pair<int, int>> translacao(std::vector<std::vector<double>> &pontos, int fatorX, int fatorY);
		std::vector<std::pair<int, int>> translacao(Janela &j, int fatorX, int fatorY);
		std::vector<std::pair<int, int>> rotacao(std::vector<std::vector<double>> &pontos, double angulo, int fatorTranslacaoX, int fatorTranslacaoY);
		std::vector<std::pair<int, int>> rotacao(Janela &j, double angulo);
		std::vector<std::pair<int, int>> escala(std::vector<std::vector<double>> &pontos, double fatorX, double fatorY, int fatorTranslacaoX, int fatorTranslacaoY);
		std::vector<std::pair<int, int>> escala(Janela &j, double fatorX, double fatorY);

		static bool mesmaCor(const double *,const double *);
		static std::stack<TrocaReflexao> reflexao(std::pair<int,int> &p1,std::pair<int,int> &p2);
		static void reflexaoInversa(std::stack<TrocaReflexao> tiposTroca,int &x, int &y);
		static std::vector<std::pair<int, int>> bresenham(int x1, int y1, int x2, int y2);
};

