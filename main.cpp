#include <iostream>
#include <vector>
#include <utility>
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

		// int * coordenadaGrid = mapCoordenadaRealParaGrid(x,y);
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
	std::cout << "aasjdaosdjapods" << std::endl;
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (LARGURA, ALTURA);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Computacao Grafica");
	init2D(0.0,0.0,0.0);

	grid = new Grid(ALTURA,LARGURA,TAMANHO_QUADRADO,ESPACO_GRID,ESPACO_GRID,corPonto);

	std::cout << "depois" << std::endl;



	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
}