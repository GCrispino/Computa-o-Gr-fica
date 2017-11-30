#include <iostream>
#include <utility>
#include "Janela.hpp"

Janela::Janela(int xMin, int yMin, int xMax, int yMax)
:xMin(xMin),yMin(yMin),xMax(xMax),yMax(yMax){
	// this->pontosBorda = std::vector<std::pair<int, int>>();
	// this->pontosDentro = std::vector<std::pair<int, int>>();

	this->atualizaPontos();

	/*for (int x = xMin;x <= xMax;++x){
		this->pontosBorda.push_back(std::make_pair(x,yMin));
		this->pontosBorda.push_back(std::make_pair(x,yMax));
	}

	for (int y = yMin + 1;y <= yMax - 1;++y){
		this->pontosBorda.push_back(std::make_pair(xMin, y));
		this->pontosBorda.push_back(std::make_pair(xMax, y));
	}

	for (int x = xMin + 1;x <= xMax - 1;++x)
		for (int y = yMin + 1;y <= yMax - 1;++y)
			this->pontosDentro.push_back(std::make_pair(x,y));*/
			
}

// Janela::Janela(const std::pair<int, int> &p1, const std::pair<int, int> &p2){}

std::pair<std::pair<int, int>, std::pair<int, int>>
Janela::cohenSutherland(const std::pair<int, int> &p1, const std::pair<int, int> &p2)
{
	/**
		 * ALGORITMO
		 * 
		 * obtém códigos para os pontos p1 e p2 - FEITO
		 * verifica se os pontos estão dentro ou fora
		 *	se ambos estiverem dentro, retorna os pontos p1 e p2 
		 * 	se ambos estiverem fora, retorna null
		 * 	se estiver um dentro e um fora:
		 * 		calcula intersecção 
		 * 		verifica qual ponto está dentro e qual está fora
		 * 		chama a função recursivamente com a intersecção calculada
		*/

	std::string c1Bin = this->getCodigoCS(p1),
				c2Bin = this->getCodigoCS(p2);
	unsigned int c1Dec = std::strtoull(c1Bin.data(), NULL, 2),
				 c2Dec = std::strtoull(c2Bin.data(), NULL, 2);
	bool totalDentro = !(c1Dec | c2Dec),
		 totalFora = (c1Dec & c2Dec);

	std::cout << "cohenSutherland()!" << std::endl;
	std::cout << "P1: " << p1.first << ',' << p1.second << std::endl;
	std::cout << "P2: " << p2.first << ',' << p2.second << std::endl
			  << std::endl;

	std::cout << c1Bin << ',' << c1Dec << '\t' << c2Bin << ',' << c2Dec << std::endl;
	std::cout << totalDentro << ',' << totalFora << std::endl;

	if (totalDentro)
	{
		//Retorna os pontos que estão dentro
		return std::make_pair(p1, p2);
	}
	else if (totalFora)
	{
		return std::make_pair(std::make_pair(-1, -1), std::make_pair(-1, -1));
	}
	else
	{
		/*encontra a parte do plano de corte onde está a intersecção
			 *	faz isso pegando o bit em que há a primeira diferença(menos significativo ao mais significativo) entre os códigos 
			 *		isso é feito até que os dois pontos estiverem dentro da janela
			 * encontra a intersecção
			 * chama a função para os pontos atualizados com a intersecção
			 */
		std::cout << "Terceiro caso" << std::endl;

		std::pair<unsigned int, unsigned int> infoDiffBit = this->getDiffBitCS(c1Bin, c2Bin);
		std::pair<const std::pair<int, int> &, const std::string &>
			p1Info = std::make_pair(p1, c1Bin),
			p2Info = std::make_pair(p2, c2Bin);

		//pega o ponto original da reta e o novo ponto de interseccao com uma das linhas da janela de corte
		std::pair<std::pair<int, int>, std::pair<int, int>>
			pontosAtualizados = this->getPontoInterseccaoCS(infoDiffBit, p1Info, p2Info);

		std::pair<int, int> novoP1 = pontosAtualizados.first, novoP2 = pontosAtualizados.second;

		return this->cohenSutherland(novoP1, novoP2);
	}
}

std::string Janela::getCodigoCS(const std::pair<int, int> &p)
{
	char codigo[4];
	char *const cima = codigo,
				*const baixo = codigo + 1,
				*const direita = codigo + 2,
				*const esquerda = codigo + 3; //os bits
	const int x = p.first, y = p.second;

	*cima = y > this->yMax ? '1' : '0';
	*baixo = y < this->yMin ? '1' : '0';
	*direita = x > this->xMax ? '1' : '0';
	*esquerda = x < this->xMin ? '1' : '0';

	*(codigo + 4) = '\0'; //caractere de fim de string

	return std::string(codigo);
}

std::pair<unsigned int, unsigned int> Janela::getDiffBitCS(const std::string &c1Bin, const std::string &c2Bin)
{
	for (unsigned int i = 0; i < 4; ++i)
		if (c1Bin[i] != c2Bin[i])
		{
			unsigned int pontoEscolhido = c1Bin[i] == '1' ? 0 : 1;
			return std::make_pair(i, pontoEscolhido);
		}
}

std::pair<std::pair<int, int>, std::pair<int, int>>
Janela::getPontoInterseccaoCS(
	std::pair<unsigned int, unsigned int> infoDiffBit,
	const std::pair<const std::pair<int, int> &, const std::string &> &p1Info,
	const std::pair<const std::pair<int, int> &, const std::string &> &p2Info)
{

	const std::pair<int, int> &p1 = p1Info.first, &p2 = p2Info.first;
	const std::string &c1Bin = p1Info.second, &c2Bin = p2Info.second;
	const unsigned int c1Dec = std::strtoull(c1Bin.data(), NULL, 2), c2Dec = std::strtoull(c2Bin.data(), NULL, 2);

	unsigned int diffBit = infoDiffBit.first, iPontoEscolhido = infoDiffBit.second;

	std::cout << "c1dec: " << c1Dec << ", c2dec: " << c2Dec << std::endl;

	//escolhe o ponto que está fora. Caso ambos estejam fora, escolhe o primeiro
	const std::pair<int, int> &pontoEscolhido = iPontoEscolhido == 0 ? p1 : p2,
							  &outroPonto = (pontoEscolhido == p1) ? p2 : p1;

	const int xPontoEscolhido = pontoEscolhido.first,
			  yPontoEscolhido = pontoEscolhido.second,
			  xOutroPonto = outroPonto.first,
			  yOutroPonto = outroPonto.second;

	// std::cout << "Ponto escolhido: " << xPontoEscolhido << ',' << yPontoEscolhido << std::endl;
	// std::cout << "Outro ponto: " << xOutroPonto << ',' << yOutroPonto << std::endl;
	// std::cout << std::endl;

	// std::cout << "Calculos: " << std::endl;
	double x, y;
	double m = (double)(yPontoEscolhido - yOutroPonto) / (xPontoEscolhido - xOutroPonto);

	// std::cout << "yPontoEscolhido - yOutroPonto: " << yPontoEscolhido - yOutroPonto << std::endl;
	// std::cout << "xPontoEscolhido - xOutroPonto: " << xPontoEscolhido - xOutroPonto << std::endl;
	// std::cout << "m: " << m << std::endl;
	// std::cout << "diffBit: " << diffBit << std::endl;
	switch (diffBit)
	{
	case 0: //CIMA
		x = xPontoEscolhido - (yPontoEscolhido - this->yMax) / m;
		y = this->yMax;
		break;
	case 1: //BAIXO
		x = xPontoEscolhido - (yPontoEscolhido - this->yMin) / m;
		y = this->yMin;
		break;
	case 2: //DIREITA
		x = this->xMax;
		y = yPontoEscolhido - (xPontoEscolhido - this->xMax) * m;
		break;
	case 3: //ESQUERDA
		x = this->xMin;
		y = yPontoEscolhido - (xPontoEscolhido - this->xMin) * m;
		break;
	}

	// std::cout << "Depois Calculos. " << std::endl;

	if (pontoEscolhido == p1)
		return std::make_pair(std::make_pair(x, y), outroPonto);
	else
		return std::make_pair(outroPonto, std::make_pair(x, y));
}

void Janela::atualizaPontos(){
	for (int x = xMin; x <= xMax; ++x){
		this->pontosBorda.push_back(std::make_pair(x, yMin));
		this->pontosBorda.push_back(std::make_pair(x, yMax));
	}

	for (int y = yMin + 1; y <= yMax - 1; ++y){
		this->pontosBorda.push_back(std::make_pair(xMin, y));
		this->pontosBorda.push_back(std::make_pair(xMax, y));
	}

	for (int x = xMin + 1; x <= xMax - 1; ++x)
		for (int y = yMin + 1; y <= yMax - 1; ++y)
			this->pontosDentro.push_back(std::make_pair(x, y));
}

std::vector< std::pair<int, int> > Janela::getPontosBorda() const{
	return this->pontosBorda;
}

std::vector< std::pair<int, int> > Janela::getPontosDentro() const{
	return this->pontosDentro;
}

int Janela::getXMin() const{
	return this->xMin;
}

int Janela::getYMin() const{
	return this->yMin;
}

int Janela::getXMax() const{
	return this->xMax;
}

int Janela::getYMax() const{
	return this->yMax;
}

void Janela::setXMin(int xMin){
	this->xMin = xMin;
	this->atualizaPontos();
}

void Janela::setYMin(int yMin){
	this->yMin = yMin;
	this->atualizaPontos();
}

void Janela::setXMax(int xMax){
	this->xMax = xMax;
	this->atualizaPontos();
}
void Janela::setYMax(int yMax){
	this->yMax = yMax;
	this->atualizaPontos();
}