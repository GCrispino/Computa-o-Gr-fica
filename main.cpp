#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <GL/glut.h>
#include <GL/glu.h>
#include "Grid.hpp"

#define ALTURA 800
#define LARGURA 800
#define ESPACO_GRID 5
#define TAMANHO_QUADRADO 30

double corPonto[] = {1.0,0.0,0.0};
Grid *grid;

std::vector< std::pair<int,int> > quadradosSelecionados;


void init2D(float r, float g, float b)
{
	glClearColor(r,g,b,0.0);  
	glMatrixMode (GL_PROJECTION);
	glOrtho( 0, LARGURA, ALTURA, 0, 0, 1 );

	glMatrixMode (GL_MODELVIEW);	

	glClear(GL_COLOR_BUFFER_BIT);
}

void display(void)
{
	glFlush();
}

void mouse(int btn, int state , int x , int y){
	if (state == GLUT_DOWN){
		double cor[] = {1.0,0.0,0.0};

		int * coordenadaGrid = grid->mapCoordenadaRealParaGrid(x,y);
		
		grid->pintaFrameBuffer(cor, coordenadaGrid[0], coordenadaGrid[1]);
		grid->pintaQuadrado(coordenadaGrid[0],coordenadaGrid[1]);

		quadradosSelecionados.push_back(std::make_pair(coordenadaGrid[0],coordenadaGrid[1]));

		if (quadradosSelecionados.size() == 2){
			std::vector< std::pair<int,int> > pontos;
			std::pair<int,int> &primeiroPonto = quadradosSelecionados[0],&segundoPonto = quadradosSelecionados[1];
			TrocaReflexao tipoTroca;
			std::stack<TrocaReflexao> tiposTroca;


			std::cout << "Primeiro ponto: " << primeiroPonto.first << ',' << primeiroPonto.second << std::endl;
			std::cout << "Segundo ponto: " << segundoPonto.first << ',' << segundoPonto.second << std::endl;
			std::cout << std::endl;


			tiposTroca = Grid::reflexao(primeiroPonto,segundoPonto);
			pontos = Grid::bresenham(primeiroPonto.first,primeiroPonto.second,segundoPonto.first,segundoPonto.second);

			std::cout << "Depois da reflexão: " << std::endl;
			std::cout << "Primeiro ponto: " << primeiroPonto.first << ',' << primeiroPonto.second << std::endl;
			std::cout << "Segundo ponto: " << segundoPonto.first << ',' << segundoPonto.second << std::endl;
			std::cout << std::endl;
			


			std::cout << "Depois da reflexão inversa: ";
			//reflete cada ponto e os pinta
			for (unsigned int i = 0;i < pontos.size();++i){
				std::pair<int,int> &ponto = pontos[i];
				Grid::reflexaoInversa(tiposTroca,ponto.first,ponto.second);
				std::cout << "Ponto: " << ponto.first << ',' << ponto.second << " ";
				grid->pintaQuadrado(ponto.first,ponto.second);

				grid->pintaFrameBuffer(cor, ponto.first,ponto.second);
			}
			std::cout << std::endl << std::endl;


			quadradosSelecionados.clear();
		}

	}
}

int main(int argc,char *argv[])
{
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> lados = {
		// {{5, 23}, {12, 0}},
		// // {{5, 23}, {15, 13}},
		// {{5,23},{20,7}},
		// // {{12, 0}, {15, 13}},
		// {{12, 0}, {23, 22}},
		// // {{15, 13}, {20, 7}},
		// // {{15, 13}, {23, 22}},
		// {{20, 7}, {23, 22}}

		// {{12,4},{12,14}},
		// {{12,4},{22,5}},
		// {{12,14},{22,13}},
		// {{22,5},{22,13}}

		{{3,12},{6,4}},
		{{3,12},{8,23}},
		{{6,4},{11,10}},
		{{8,23},{11,10}}/*,
		{{},{}},
		{{},{}},
		{{},{}},
		{{},{}},
		{{},{}},
		{{},{}},*/
	},outrosLados;
	std::vector<std::pair<int,int>> pontos;
	

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (LARGURA, ALTURA);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Computacao Grafica");
	init2D(0.0,0.0,0.0);

	grid = new Grid(ALTURA,LARGURA,TAMANHO_QUADRADO,ESPACO_GRID,ESPACO_GRID,corPonto);

	for (auto &lado : lados){
		grid->pintaQuadrado(lado.first.first,lado.first.second);
		grid->pintaQuadrado(lado.second.first,lado.second.second);
	}

	pontos = grid->preenchimentoVarredura(lados);

	int yAtual = pontos[0].second;
	std::vector<int> coordenadasX_YAtual;

	for (unsigned int i = 0;i < pontos.size();++i){
		std::cout << "i - Ponto atual: " << pontos[i].first << ',' << pontos[i].second << std::endl;
		if (pontos[i].second != yAtual || i == pontos.size() - 1){
			if (i == pontos.size() - 1)
				coordenadasX_YAtual.push_back(pontos[i].first);

			std::cout << "Entrou! yAtual = " << yAtual << ", novo y = " << pontos[i].second << std::endl;

			// const std::pair<int,int> &p1 = pontos[i],&p2 = pontos[i + nPontosY];
			const int xInicial = coordenadasX_YAtual[0], xFinal = coordenadasX_YAtual.back();
			// unsigned int y = pInicial.second;
			
			// std::cout << "\tponto 1: " << pInicial.first << ',' << pInicial.second << std::endl;
			// std::cout << "\tponto 2: " << p2.first << ',' << p2.second << std::endl;
			std::cout << "\txInicial: " << xInicial << std::endl;
			std::cout << "\txFinal: " << xFinal << std::endl;
			
			bool dentro = false;
			std::cout << "Pontos: " << std::endl;
			//pinta de xInicial até xFinal
			//começa pintando xInicial e xFinal
			grid->pintaQuadrado(xInicial, yAtual);
			grid->pintaQuadrado(xFinal, yAtual);

			for (unsigned int x = xInicial;x <= xFinal;++x){
				//checa se coordenada x atual pertence ao vector coordenadasX_YAtual
				std::cout << "x atual = " << x << std::endl;
				std::cout << dentro << std::endl;
				//checa se número de ocorrências do x examinado é ímpar.
				//Se for, modifica o estado da variável 'dentro'
				if (std::count(coordenadasX_YAtual.begin(), coordenadasX_YAtual.end(),x) % 2 != 0)
					dentro = !dentro;
				else
					std::cout << "\tponto : " << x << ',' << yAtual << std::endl;
	
				if (dentro)
					grid->pintaQuadrado(x, yAtual);
			}

			//limpa o vector e atualiza o valor do yAtual
			coordenadasX_YAtual.clear();
			yAtual = pontos[i].second;
		}
		
		coordenadasX_YAtual.push_back(pontos[i].first);
	}

	std::cout << std::endl;
	std::cout << "Numero de pontos de interseccao: " << pontos.size() << std::endl;

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
}

