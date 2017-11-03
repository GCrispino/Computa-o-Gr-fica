#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "Grid.hpp"

//AJEITAR pintaQuadrado -> PODE SER REDUZIDA
//FUNÇÃO criaLinhaQuadrado PODE RETORNAR LINHA
//ADICIONAR CHECAGEM SE PONTO FORA DO GRID FOR ACESSADO
//FAZER FUNÇÃO pintaLinha()
//antes do algoritmo de preenchimento, pintar os lados usando bresenham

Grid::Grid(int altura, int largura,int tamanhoQuadrados,int xInicial,int yInicial,double corLinha[3])
:altura(altura),largura(largura),tamanhoQuadrados(tamanhoQuadrados),xInicial(xInicial),yInicial(yInicial){
	this->corLinha[0] = corLinha[0];
	this->corLinha[1] = corLinha[1];
	this->corLinha[2] = corLinha[2];

	//Inicialização do frame buffer
	// for (auto &linha: this->frameBuffer)
	for (unsigned int i = 0;i < this->largura;++i){
		std::vector< std::vector<double> > linha;
		for (unsigned int j = 0;j < this->altura;++j)
			linha.push_back(std::vector<double> {0.0,0.0,0.0});
		this->frameBuffer.push_back(linha);
	}

	for (int xGrid = 0,xReal = this->yInicial;xReal + this->tamanhoQuadrados < this->altura;xReal += this->tamanhoQuadrados,++xGrid){
		std::vector<Quadrado> linha;
		// this->quadrados.push_back(this->criaLinhaQuadrados(xGrid,xReal));
		this->quadrados.push_back(linha);
		this->criaLinhaQuadrados(xGrid,xReal);
	}
	std::cout << "Acabou!" << std::endl;

}

Grid::Grid(const Grid &g)
:altura(g.altura),largura(g.largura),tamanhoQuadrados(g.tamanhoQuadrados),xInicial(g.xInicial),yInicial(g.yInicial){
	this->corLinha[0] = g.corLinha[0];
	this->corLinha[1] = g.corLinha[1];
	this->corLinha[2] = g.corLinha[2];
}


void Grid::criaLinhaQuadrados(int xGrid,int xReal){

	for (int yReal = this->xInicial;yReal + this->tamanhoQuadrados < this->largura; yReal += this->tamanhoQuadrados){
		// std::cout << "Quadrado " << x << "," << yGrid << std::endl;
		this->quadrados[xGrid].push_back(Quadrado(xReal,yReal,this->tamanhoQuadrados,this->corLinha));
	}
}

void Grid::pintaQuadrado(int xGrid, int yGrid){
	const Quadrado q = this->getQuadrado(xGrid,yGrid);

	this->pintaQuadrado(q);
}

void Grid::pintaQuadrado(const Quadrado &q){
	q.pinta();
}

void Grid::pintaFrameBuffer(double cor[],int x,int y){
	this->frameBuffer[x][y][0] = cor[0];
	this->frameBuffer[x][y][1] = cor[1];
	this->frameBuffer[x][y][2] = cor[2];
}


const Quadrado & Grid::getQuadrado(int xGrid, int yGrid){
	return this->quadrados[xGrid][yGrid];
}

void Grid::troca(int &x, int &y){
	int aux = x;
	x = y;
	y = aux;
}

std::stack<TrocaReflexao> Grid::reflexao(std::pair<int,int> &p1,std::pair<int,int> &p2){
	int &x1 = p1.first, &y1 = p1.second, &x2 = p2.first, &y2 = p2.second;
	double m = (double)(y2 - y1) / (x2 - x1);
	std::stack<TrocaReflexao> tiposTroca;

	std::cout << "m: " << m << std::endl;

	if (m > 1 || m < -1){
		troca(x1,y1);
		troca(x2,y2);
		tiposTroca.push(TrocaReflexao::TROCA_XY);
	}
	if (x1 > x2){
		x1 = -x1;
		x2 = -x2;
		tiposTroca.push(TrocaReflexao::TROCA_X);
	}
	if (y1 > y2){
		y1 = -y1;
		y2 = -y2;
		tiposTroca.push(TrocaReflexao::TROCA_Y);
	}

	return tiposTroca;
}


void Grid::reflexaoInversa(std::stack<TrocaReflexao> tiposTroca,int &x, int &y){
	while(!tiposTroca.empty()){
		std::cout << "Reflexao inversa: ponto: " << x << "," << y << std::endl;
		TrocaReflexao tipoTroca = tiposTroca.top();
		tiposTroca.pop();
		switch (tipoTroca){
			case TrocaReflexao::TROCA_XY:
				troca(x,y);
				break;
			case TrocaReflexao::TROCA_X:
				x = -x;
				break;
			case TrocaReflexao::TROCA_Y:
				y = -y;
				break;
			default:
				return;
		}
	}
}


int * Grid::mapCoordenadaGridParaReal(int x, int y){
	int * coordenadaReal = new int[2];
	
	coordenadaReal[0] = x * this->tamanhoQuadrados + this->xInicial;
	coordenadaReal[1] = y * this->tamanhoQuadrados + this->yInicial;
	
	return coordenadaReal;
}


int * Grid::mapCoordenadaRealParaGrid(int x, int y){
	int * coordenadaGrid = new int[2];
	
		coordenadaGrid[0] = (x - this->xInicial) / this->tamanhoQuadrados;
		coordenadaGrid[1] = (y - this->yInicial) / this->tamanhoQuadrados;
		
		return coordenadaGrid;
	}

std::vector< std::pair<int,int> > Grid::bresenham(int x1, int y1,int x2, int y2){
	
	std::vector< std::pair<int,int> > pontos;
	double cor[] = {1.0,0.0,0.0};

	double m = (double)(y2 - y1) / (x2 - x1);
	double e = m - 0.5f;

	std::cout << "bresenham: ";
	while(x1 <= x2){
		pontos.push_back(std::make_pair(x1,y1));
		std::cout << x1 <<"," << y1 << " ";

		if (e >= 0){
			++y1;
			--e;
		}
		
		++x1;

		e += m;

	}

	std::cout << std::endl;

	return pontos;
}

std::pair<int, int> Grid::getPontoYMin(const std::pair<std::pair<int, int>, std::pair<int, int>> &lado){
	const std::pair<int,int> &p1 = lado.first,&p2 = lado.second;

	if (p1.second < p2.second)
		return p1;
	else
		return p2;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Grid::getLados(int yVarredura, const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &lados){
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> ladosInterseccaoPonto;

	for (const std::pair<std::pair<int, int>, std::pair<int, int>> &lado : lados){
		int yMin,yMax;

		if (lado.first.second < lado.second.second){
			yMin = lado.first.second;
			yMax = lado.second.second;
		}
		else{
			yMin = lado.second.second;
			yMax = lado.first.second;
		}

		if (yMin <= yVarredura && yVarredura <= yMax)
			ladosInterseccaoPonto.push_back(lado);
	}

	return ladosInterseccaoPonto;
}

std::vector<std::pair<int, int>> Grid::preenchimentoVarredura(const std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > &lados){
	std::vector<std::pair<int, int>> pontosParaPintar;

	int yMax = Grid::mapCoordenadaRealParaGrid(0,this->altura)[1];

	for (int yVarredura = 0; yVarredura < yMax; ++yVarredura){
		//obtém lados que possuem interseccao com o ponto atual
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> ladosInterseccaoPonto = this->getLados(yVarredura, lados);

		std::cout << "Y varredura: " << yVarredura << std::endl;
		std::cout << "Pontos de intersecção: " << std::endl;
		for (const std::pair<std::pair<int, int>, std::pair<int, int>> &lado: ladosInterseccaoPonto){
			std::pair<int,int> pontoYMin = this->getPontoYMin(lado);

			int x1 = lado.first.first, x2 = lado.second.first, xYMin = pontoYMin.first,
				y1 = lado.first.second, y2 = lado.second.second, yMin = pontoYMin.second;
			double m = (double)(y2 - y1) / (x2 - x1);

			if (m == 0)//ignora se lado for horizontal
				continue;

			int xInterseccao = (1 / m) *  (yVarredura - yMin) + xYMin;
			std::cout << "\tx1 = " << x1 << " y1 = " << y1 << ", x2 = " << x2 << ", y2 = " << y2 << std::endl;
			std::cout << "\tm = " << m << std::endl;
			std::cout << "\tyMin = " << yMin << std::endl;
			std::cout << "\txYMin = " << xYMin << std::endl;

			std::cout << "\tlado: " << std::endl;

			std::cout << "\t\tPrimeiro ponto: " << lado.first.first << ',' << lado.first.second << std::endl;
			std::cout << "\t\tSegundo ponto: " << lado.second.first << ',' << lado.second.second << std::endl;

			std::cout << "\txInterseccao = " << xInterseccao << std::endl;
			pontosParaPintar.push_back(std::make_pair(xInterseccao,yVarredura));
		}

	}

	std::sort(pontosParaPintar.begin(), pontosParaPintar.end(), [](const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
		unsigned int x1 = p1.first, y1 = p1.second,
					 x2 = p2.first, y2 = p2.second;

		if (y1 == y2)
			return x1 < x2;
		return y1 < y2;
	});

	return pontosParaPintar;
}
