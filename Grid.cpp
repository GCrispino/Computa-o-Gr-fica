#include <iostream>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <GL/glut.h>
#include "Grid.hpp"
#include "Transformacoes.hpp"
// #include "Janela.hpp"

//AJEITAR IMPLEMENTAÇÃO DO FRAMEBUFFER -> NÃO DEVE SER [ALTURA][LARGURA]
//FUNÇÃO criaLinhaQuadrado PODE RETORNAR LINHA
//ADICIONAR CHECAGEM SE PONTO FORA DO GRID FOR ACESSADO

Grid::Grid(int altura, int largura,int tamanhoQuadrados,int xInicial,int yInicial,double corLinha[3])
:altura(altura),largura(largura),tamanhoQuadrados(tamanhoQuadrados),xInicial(xInicial),yInicial(yInicial){
	this->corLinha[0] = corLinha[0];
	this->corLinha[1] = corLinha[1];
	this->corLinha[2] = corLinha[2];

	//Inicialização do frame buffer
	for (unsigned int i = 0;i < this->largura;++i){
		std::vector< std::vector<double> > linha;
		for (unsigned int j = 0;j < this->altura;++j)
			linha.push_back(std::vector<double> {0.0,0.0,0.0});
		this->frameBuffer.push_back(linha);
	}

	std::cout << std::endl;
	for (int xGrid = 0,xReal = this->yInicial;xReal + this->tamanhoQuadrados < this->altura;xReal += this->tamanhoQuadrados,++xGrid){
		std::vector<Quadrado> linha;
		this->quadrados.push_back(linha);
		this->criaLinhaQuadrados(xGrid,xReal);
	}


}

Grid::Grid(const Grid &g)
:altura(g.altura),largura(g.largura),tamanhoQuadrados(g.tamanhoQuadrados),xInicial(g.xInicial),yInicial(g.yInicial){
	this->corLinha[0] = g.corLinha[0];
	this->corLinha[1] = g.corLinha[1];
	this->corLinha[2] = g.corLinha[2];
}


void Grid::criaLinhaQuadrados(int xGrid,int xReal){

	for (int yReal = this->xInicial;yReal + this->tamanhoQuadrados < this->largura; yReal += this->tamanhoQuadrados){
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

std::vector<std::pair<int, int>> Grid::pintaLinha(std::pair<int, int> &primeiroPonto, std::pair<int, int> &segundoPonto)
{
	std::vector<std::pair<int, int>> pontos;
	TrocaReflexao tipoTroca;
	std::stack<TrocaReflexao> tiposTroca;

	tiposTroca = Grid::reflexao(primeiroPonto, segundoPonto);
	pontos = Grid::bresenham(primeiroPonto.first, primeiroPonto.second, segundoPonto.first, segundoPonto.second);


	//reflete cada ponto e os pinta
	for (unsigned int i = 0; i < pontos.size(); ++i){
		std::pair<int, int> &ponto = pontos[i];
		Grid::reflexaoInversa(tiposTroca, ponto.first, ponto.second);
		this->pintaQuadrado(ponto.first, ponto.second);

		this->pintaFrameBuffer(this->corLinha, ponto.first, ponto.second);
	}

	return pontos;
}

std::vector<std::pair<int, int>> Grid::pintaCirculo(std::pair<int, int> &centro, int raio){
	std::vector<std::pair<int, int>>
		pontos = Grid::pontoMedioCirculo(centro, raio);

	//pinta os pontos
	for (const auto &ponto: pontos){
		this->pintaQuadrado(ponto.first, ponto.second);

		this->pintaFrameBuffer(this->corLinha, ponto.first, ponto.second);
	}

	return pontos;
}

//MUDAR ISSO AQUI DEPOIS PARA PEGAR OS PONTOS DA BORDA E PINTAR DIRETO!
void Grid::pintaJanela(const Janela &janela){
	double vermelho[] = {1.0,0.0,0.0};

	// std::cout << "Pontos!!!" << std::endl;
	// for (auto & ponto: janela.getPontosBorda()){
	// 	// std::cout << ponto.first << ',' << ponto.second << std::endl;
	//  	this->pintaQuadrado(ponto.first,ponto.second);
	// 	this->pintaFrameBuffer(vermelho,ponto.first,ponto.second);
	// }
	
	std::cout << std::endl << std::endl;

	std::pair<int, int> p1, p2,p3, p4;
	int xMin = janela.getXMin(),yMin = janela.getYMin(),
		xMax = janela.getXMax(),yMax = janela.getYMax();

	

	p1 = std::make_pair(xMin, yMin);
	p2 = std::make_pair(xMax, yMax);
	p3 = std::make_pair(xMax, yMin);
	p4 = std::make_pair(xMin, yMax);

	//pinta a janela
	this->pintaLinha(p1, p3);
	this->pintaLinha(p2, p4);

	//reseta valores dos pontos porque são modificados nas chamadas de 'Grid::pintaLinha()'
	p1 = std::make_pair(xMin, yMin);
	p2 = std::make_pair(xMax, yMax);
	p3 = std::make_pair(xMax, yMin);
	p4 = std::make_pair(xMin, yMax);

	this->pintaLinha(p1, p4);
	this->pintaLinha(p2, p3);
}

void Grid::pintaFrameBuffer(const double cor[],int x,int y){
	this->frameBuffer[x][y][0] = cor[0];
	this->frameBuffer[x][y][1] = cor[1];
	this->frameBuffer[x][y][2] = cor[2];
}

void Grid::apagaPontos(const std::vector<std::pair<int, int>> &pontos){
	double preto[] = {0.0, 0.0, 0.0};

	// std::cout << "APaga pontos!!!" << std::endl;
	for (const auto &ponto: pontos){
		unsigned int x = ponto.first,y = ponto.second;
		std::cout << "oi" << std::endl;
		std::cout << "apaga ponto: " << x << ',' << y << std::endl;
		if (x < 0 || y < 0 || x >= this->frameBuffer.size() || y >= this->frameBuffer[0].size())
			continue;
		if (!this->mesmaCor(preto, this->frameBuffer[x][y].data())){
			std::cout << "apaga ponto: " << x << ',' << y << std::endl;
			Quadrado &q = this->getQuadrado(x, y);
			//muda a cor do quadrado para preto apenas para pintar de volta para preto
			q.setCor(preto);
			q.pinta();
			//depois volta para a cor que estava antes
			q.setCor(this->corLinha);

			this->pintaFrameBuffer(preto, x, y);
		}
	}
}

void Grid::limpa(){
	double preto[] = {0.0,0.0,0.0};

	for (unsigned int x = 0;x < this->largura / this->tamanhoQuadrados;++x)
		for (unsigned int y = 0;y < this->altura / this->tamanhoQuadrados;++y)
			if (!this->mesmaCor(preto,this->frameBuffer[x][y].data())){
				Quadrado &q = this->getQuadrado(x,y);
				//muda a cor do quadrado para preto apenas para pintar de volta para preto
				q.setCor(preto);
				q.pinta();
				//depois volta para a cor que estava antes
				q.setCor(this->corLinha);

				this->pintaFrameBuffer(preto,x,y);
			}

}

Quadrado & Grid::getQuadrado(int xGrid, int yGrid){
	return this->quadrados[xGrid][yGrid];
}

const std::vector<double> Grid::getCorFrameBuffer(int x, int y){
	return this->frameBuffer[x][y];
}


bool Grid::mesmaCor(const double *c1,const double *c2){
	return c1[0] == c2[0] && c1[1] == c2[1] && c1[2] == c2[2];
}

std::stack<TrocaReflexao> Grid::reflexao(std::pair<int,int> &p1,std::pair<int,int> &p2){
	int &x1 = p1.first, &y1 = p1.second, &x2 = p2.first, &y2 = p2.second;
	double m = (double)(y2 - y1) / (x2 - x1);
	std::stack<TrocaReflexao> tiposTroca;

	// std::cout << "m: " << m << std::endl;

	if (m > 1 || m < -1){
		Util::troca(x1,y1);
		Util::troca(x2, y2);
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
		// std::cout << "Reflexao inversa: ponto: " << x << "," << y << std::endl;
		TrocaReflexao tipoTroca = tiposTroca.top();
		tiposTroca.pop();
		switch (tipoTroca){
			case TrocaReflexao::TROCA_XY:
				Util::troca(x,y);
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

	// std::cout << "bresenham: ";
	while(x1 <= x2){
		pontos.push_back(std::make_pair(x1,y1));
		// std::cout << x1 <<"," << y1 << " ";

		if (e >= 0){
			++y1;
			--e;
		}
		
		++x1;

		e += m;

	}

	// std::cout << std::endl;

	return pontos;
}

std::vector<std::pair<int, int>> Grid::pontoMedioCirculo(std::pair<int, int> &centro, int r){
	std::vector<std::pair<int, int>> pontos;

	int
		xCentro = centro.first,
		yCentro = centro.second;

	pontos.push_back(std::make_pair(xCentro + r,yCentro));
	pontos.push_back(std::make_pair(xCentro, yCentro + r));
	pontos.push_back(std::make_pair(xCentro - r, yCentro));
	pontos.push_back(std::make_pair(xCentro,yCentro - r));


	int
		x = 0,
		y = r,
		p = 1 - r;

	while (x < y){
		++x;

		if (p < 0)
			p += 2 * x + 3;
		else{
			--y;
			p += 2 * x - 2 * y + 5;
		}

		std::pair<int, int> pontosQuadrantes[] = {
			std::make_pair(xCentro + x, yCentro + y),
			std::make_pair(xCentro + y, yCentro + x),
			std::make_pair(xCentro + y, yCentro - x),
			std::make_pair(xCentro + x, yCentro - y),
			std::make_pair(xCentro - x, yCentro - y),
			std::make_pair(xCentro - y, yCentro - x),
			std::make_pair(xCentro - y, yCentro + x),
			std::make_pair(xCentro - x, yCentro + y),
		};

		pontos.insert(pontos.end(), pontosQuadrantes, pontosQuadrantes + 8);
	}

	return pontos;
}

std::pair<int, int> Grid::getPontoYMin(const std::pair<std::pair<int, int>, std::pair<int, int>> &lado){
	const std::pair<int,int> &p1 = lado.first,&p2 = lado.second;

	if (p1.second < p2.second)
		return p1;
	else
		return p2;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>
	Grid::getLados(int yVarredura, const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &lados, std::map<std::pair<int, int>, bool> &ladosMinMax)
{
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> ladosInterseccaoPonto;

	int yMinGlobal = (this->altura / this->tamanhoQuadrados) + 1, yMaxGlobal = 0,
		xYMinGlobal,xYMaxGlobal;

	std::cout << "yMinGlobal inicial: " << yMinGlobal << std::endl;
	std::cout << "yMaxGlobal inicial: " << yMaxGlobal << std::endl;

	for (const std::pair<std::pair<int, int>, std::pair<int, int>> &lado : lados){
		int yMin,yMax,xYMin,xYMax;

		if (lado.first.second < lado.second.second){
			yMin = lado.first.second;
			xYMin = lado.first.first;
			yMax = lado.second.second;
			xYMax = lado.second.first;
		}
		else{
			yMin = lado.second.second;
			xYMin = lado.second.first;
			yMax = lado.first.second;
			xYMax = lado.first.first;
		}

		if (yMin < yMinGlobal){
			yMinGlobal = yMin;
			xYMinGlobal = xYMin;
		}

		if (yMax > yMaxGlobal){
			yMaxGlobal = yMax;
			xYMaxGlobal = xYMax;
		}

		if (yMin <= yVarredura && yVarredura <= yMax)
			ladosInterseccaoPonto.push_back(lado);
	}

	ladosMinMax[std::make_pair(xYMinGlobal,yMinGlobal)] = true;
	ladosMinMax[std::make_pair(xYMaxGlobal,yMaxGlobal)] = true;


	return ladosInterseccaoPonto;
}

void Grid::preenchimentoVarredura(const std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > &lados){
	//Obtém os pontos das arestas que serão usados no preechimento depois
	std::vector<std::pair<int, int>> pontosParaPintar;

	int yMax = Grid::mapCoordenadaRealParaGrid(0,this->altura)[1];
	std::map<std::pair<int,int>,bool> ladosMinMax; //retorna true se o ponto usado como chave é um máximo ou mínimo
	
	for (int yVarredura = 0; yVarredura < yMax; ++yVarredura){

		//obtém lados que possuem interseccao com o ponto atual
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> ladosInterseccaoPonto = this->getLados(yVarredura, lados,ladosMinMax);

		for (const std::pair<std::pair<int, int>, std::pair<int, int>> &lado: ladosInterseccaoPonto){
			std::pair<int,int> pontoYMin = this->getPontoYMin(lado);

			int x1 = lado.first.first, x2 = lado.second.first, xYMin = pontoYMin.first,
				y1 = lado.first.second, y2 = lado.second.second, yMin = pontoYMin.second;
			double m = (double)(y2 - y1) / (x2 - x1);

			//se lado for horizontal(m == 0), não guarda os pontos referentes a ele e pinta os seus pontos que podem não estar pintados
			if (m == 0){
				continue;
			}

			int xInterseccao = (1 / m) *  (yVarredura - yMin) + xYMin;
			
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


	//Preenche a figura a partir dos pontos obtidos
	int yAtual = pontosParaPintar[0].second;
	std::vector<int> coordenadasX_YAtual;

	for (unsigned int i = 0; i < pontosParaPintar.size(); ++i){
		std::cout << "i - Ponto atual: " << pontosParaPintar[i].first << ',' << pontosParaPintar[i].second << std::endl;
		if (pontosParaPintar[i].second != yAtual || i == pontosParaPintar.size() - 1){
			if (i == pontosParaPintar.size() - 1)
				coordenadasX_YAtual.push_back(pontosParaPintar[i].first);

			std::cout << "Entrou! yAtual = " << yAtual << ", novo y = " << pontosParaPintar[i].second << std::endl;

			// const std::pair<int,int> &p1 = pontosParaPintar[i],&p2 = pontosParaPintar[i + npontosParaPintarY];
			const int xInicial = coordenadasX_YAtual[0], xFinal = coordenadasX_YAtual.back();
			// unsigned int y = pInicial.second;

			// std::cout << "\tponto 1: " << pInicial.first << ',' << pInicial.second << std::endl;
			// std::cout << "\tponto 2: " << p2.first << ',' << p2.second << std::endl;
			std::cout << "\txInicial: " << xInicial << std::endl;
			std::cout << "\txFinal: " << xFinal << std::endl;

			bool dentro = false;
			std::cout << "pontos: " << std::endl;

			//pinta de xInicial até xFinal
			//começa pintando xInicial e xFinal
			this->pintaQuadrado(xInicial, yAtual);
			this->pintaFrameBuffer(this->corLinha,xInicial,yAtual);
			this->pintaQuadrado(xFinal, yAtual);
			this->pintaFrameBuffer(this->corLinha,xFinal,yAtual);

			for (unsigned int x = xInicial; x <= xFinal; ++x){
				//checa se coordenada x atual pertence ao vector coordenadasX_YAtual
				std::cout << "x atual = " << x << std::endl;
				std::cout << dentro << std::endl;
				
				//OU se número de ocorrências do x examinado é ímpar.
				bool isPontoMaxMin = ladosMinMax[std::make_pair(x,yAtual)]; //falta ajeitar isso - indica se ponto é máximo ou mínimo
				const int nOcorrenciasPonto = std::count(coordenadasX_YAtual.begin(), coordenadasX_YAtual.end(), x);
				if ((nOcorrenciasPonto == 2 && !isPontoMaxMin) || nOcorrenciasPonto % 2 != 0){
					//Se for, modifica o estado da variável 'dentro'
					dentro = !dentro;
				}
				else
					std::cout << "\tponto : " << x << ',' << yAtual << std::endl;

				if (dentro){
					this->pintaQuadrado(x, yAtual);
					this->pintaFrameBuffer(this->corLinha,x,yAtual);
				}
			}

			//limpa o vector e atualiza o valor do yAtual
			coordenadasX_YAtual.clear();
			yAtual = pontosParaPintar[i].second;
		}

		coordenadasX_YAtual.push_back(pontosParaPintar[i].first);
	}

	std::cout << std::endl;
	std::cout << "Numero de pontos de interseccao: " << pontosParaPintar.size() << std::endl;

}

void Grid::preenchimentoRecursivo(int x, int y,const double *corPonto,const double *corInicial){
	if (x > (this->largura / this->tamanhoQuadrados) || x < 0 || y > (this->altura / this->tamanhoQuadrados) || y < 0){
		// std::cout << "Vai acabar!" << std::endl;
		return ;
	}
	
	//se não tiver pintado com uma das cores, pinta
	if (
		mesmaCor(this->frameBuffer[x][y].data(), corInicial)
		&& x < ((this->largura - this->xInicial) / this->tamanhoQuadrados) && x >= 0 && y < ((this->altura - this->yInicial) / this->tamanhoQuadrados) && y >= 0
	){

		//pinta o restante
		this->pintaFrameBuffer(corPonto,x,y);
		this->pintaQuadrado(x,y);

		// if (x < (this->largura / this->tamanhoQuadrados) - 1 && x >= 0 && y < (this->altura / this->tamanhoQuadrados) - 1 && y >= 0){
			this->preenchimentoRecursivo(x + 1, y, corPonto, corInicial);
			this->preenchimentoRecursivo(x,y + 1,corPonto,corInicial);
			this->preenchimentoRecursivo(x - 1,y,corPonto,corInicial);
			this->preenchimentoRecursivo(x,y - 1,corPonto,corInicial);
		// }
	}
}

std::vector<std::pair<int, int>> Grid::translacao(std::vector<std::vector<double>> &matPontos, int fatorX, int fatorY){
	std::vector<std::pair<int, int>> pontosTransladados;

	for (int i = 0; i < matPontos[0].size(); ++i){
		int x = std::round(matPontos[0][i]),
			y = std::round(matPontos[1][i]),
			novoX = x + fatorX,
			novoY = y + fatorY;

		pontosTransladados.push_back(std::make_pair(novoX, novoY));
	}

	return pontosTransladados;
}

std::vector<std::pair<int, int>> Grid::translacao(Janela &j, int fatorX, int fatorY){

	std::vector<std::pair<int, int>> pontosDentroJanela = j.getPontosDentro(),
		pontosTransladados;

	std::vector<std::vector<double>> matPontosATransladar(j.getMatrizPontosDentro().size());

	for (int i = 0; i < pontosDentroJanela.size(); ++i){
		auto &ponto = pontosDentroJanela[i];
		int x = ponto.first,
			y = ponto.second;
		const double corPreta[] = {0.0, 0.0, 0.0}, *corPonto = this->getCorFrameBuffer(x, y).data();

		if (!Grid::mesmaCor(corPreta, corPonto)){
			matPontosATransladar[0].push_back(x);
			matPontosATransladar[1].push_back(y);
		}

	}

	return this->translacao(matPontosATransladar,fatorX,fatorY);
}

std::vector<std::pair<int, int>> Grid::rotacao(std::vector<std::vector<double>> &matPontos, double angulo, int fatorTranslacaoX, int fatorTranslacaoY)
{
	std::vector<std::vector<double>> matPontosTransladados,matPontosTransladadosRotacionados;
	std::vector<std::pair<int,int>> pontosTransladados,pontosRotacionadosFinal;

	pontosTransladados = this->translacao(matPontos,-fatorTranslacaoX,-fatorTranslacaoY);
	matPontosTransladados = Transformacoes::convertePontosParaMatPontos(pontosTransladados);

	std::cout << "Fatores translação: " << fatorTranslacaoX << ',' << fatorTranslacaoY << std::endl;

	matPontosTransladadosRotacionados = Util::multiplicaMatriz(Transformacoes::getMatRotacao(angulo), matPontosTransladados);

	// Util::imprimeMatriz(matPontosTransladadosRotacionados);

	pontosRotacionadosFinal = this->translacao(matPontosTransladadosRotacionados, fatorTranslacaoX, fatorTranslacaoY);

	return pontosRotacionadosFinal;

	// for (int i = 0;i < matPontos[0].size();++i){
	// 	int x = std::round(matPontosRotacionados[0][i]), 
	// 		y = std::round(matPontosRotacionados[1][i]);

	// 	pontosRotacionados[i] = std::make_pair(x,y);
	// }

	// return pontosRotacionados;

}


std::vector<std::pair<int, int>> Grid::rotacao(Janela &j,double angulo){
	std::vector<std::vector<double>> matPontosDentroJanela = j.getMatrizPontosBorda(),//pontos dentro da janela
	// std::vector<std::vector<double>> matPontosDentroJanela = j.getMatrizPontosDentro(),//pontos dentro da janela
									 matPontosARotacionar(matPontosDentroJanela.size()),
									 matPontosRotacionados;

	//TRANSFORMAR ISSO EM UMA FUNÇÃO
	for (unsigned int i = 0;i < matPontosDentroJanela[0].size();++i){
		int x = matPontosDentroJanela[0][i], y = matPontosDentroJanela[1][i];

		const double corPreta[] = {0.0, 0.0, 0.0}, *corPonto = this->getCorFrameBuffer(x,y).data();

		if (!Grid::mesmaCor(corPreta, corPonto)){
			matPontosARotacionar[0].push_back(x);
			matPontosARotacionar[1].push_back(y);
		}
	}

	int fatorTranslacaoX = j.getXMin(),fatorTranslacaoY = j.getYMin();

	return this->rotacao(matPontosARotacionar, angulo, fatorTranslacaoX, fatorTranslacaoY);
}

std::vector<std::pair<int, int>> Grid::escala(std::vector<std::vector<double>> &matPontos, double fatorX, double fatorY, int fatorTranslacaoX, int fatorTranslacaoY){
	std::vector<std::vector<double>> matPontosTransladados,matPontosTransladadosEscalados;
	std::vector<std::pair<int, int>> pontosTransladados,pontosEscaladosFinal;

	pontosTransladados = this->translacao(matPontos, -fatorTranslacaoX, -fatorTranslacaoY);
	matPontosTransladados = Transformacoes::convertePontosParaMatPontos(pontosTransladados);

	matPontosTransladadosEscalados = Util::multiplicaMatriz(Transformacoes::getMatEscala(fatorX, fatorY), matPontosTransladados);

	pontosEscaladosFinal = this->translacao(matPontosTransladadosEscalados, fatorTranslacaoX, fatorTranslacaoY);

	return pontosEscaladosFinal;

	
}

std::vector<std::pair<int, int>> Grid::escala(Janela &j, double fatorX, double fatorY){
	std::vector<std::vector<double>> matPontosDentroJanela = j.getMatrizTodosPontos(),
	// std::vector<std::vector<double>> matPontosDentroJanela = j.getMatrizPontosDentro(),
									 matPontosAEscalar(matPontosDentroJanela.size()),
									 matPontosEscalados;

	for (unsigned int i = 0; i < matPontosDentroJanela[0].size(); ++i){

		int x = matPontosDentroJanela[0][i], y = matPontosDentroJanela[1][i];

		const double corPreta[] = {0.0, 0.0, 0.0}, *corPonto = this->getCorFrameBuffer(x, y).data();

		if (!Grid::mesmaCor(corPreta, corPonto)){
			matPontosAEscalar[0].push_back(x);
			matPontosAEscalar[1].push_back(y);
		}
	}

	int fatorTranslacaoX = j.getXMin(), fatorTranslacaoY = j.getYMin();

	return this->escala(matPontosAEscalar,fatorX,fatorY,fatorTranslacaoX,fatorTranslacaoX);
}