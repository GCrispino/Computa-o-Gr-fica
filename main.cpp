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
std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> lados;

unsigned char modo = '1';

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

void apertaTecla(unsigned char key, int x, int y){

	if (key == 'c'){
		grid->limpa();
		return;
	}

	if (modo == '3' && key == 'p'){
		std::cout << "Preenche!" << std::endl;

		if (!lados.empty()){
			grid->preenchimentoVarredura(lados);
			std::cout << "Terminou de preencher!" << std::endl;
			lados.clear();
		}
	}

	if (modo != key){
		modo = key;
		quadradosSelecionados.clear();
		lados.clear();
	}

}

void mouse(int btn, int state , int x , int y){
	if (state == GLUT_DOWN){

		if (modo == '2'){
			std::cout << "Preenchimento recursivo! " << std::endl;
			std::cout << "Cor ponto:  " << corPonto[0] << ',' << corPonto[1] << ',' << corPonto[2] << std::endl;

			int *coordenadasGrid = grid->mapCoordenadaRealParaGrid(x, y);
			grid->preenchimentoRecursivo(coordenadasGrid[0], coordenadasGrid[1], corPonto, corPonto);
			return;
		}

		int *coordenadaGrid = grid->mapCoordenadaRealParaGrid(x, y);

		grid->pintaFrameBuffer(corPonto, coordenadaGrid[0], coordenadaGrid[1]);
		grid->pintaQuadrado(coordenadaGrid[0], coordenadaGrid[1]);

		quadradosSelecionados.push_back(std::make_pair(coordenadaGrid[0], coordenadaGrid[1]));
		

		if (quadradosSelecionados.size() == 2){
			std::pair<int, int> &primeiroPonto = quadradosSelecionados[0], &segundoPonto = quadradosSelecionados[1];

			if (modo == '3')
				lados.push_back(std::make_pair(primeiroPonto,segundoPonto));

			grid->pintaLinha(primeiroPonto, segundoPonto);

			quadradosSelecionados.clear();
		}

	}
}

int main(int argc,char *argv[]){
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

		// {{3,12},{6,4}},
		// {{3,12},{8,23}},
		// {{6,4},{11,10}},
		// {{8,23},{11,10}}
	},outrosLados;
	std::vector<std::pair<int,int>> pontos;
	

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (LARGURA, ALTURA);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Computacao Grafica");
	init2D(0.0,0.0,0.0);

	grid = new Grid(ALTURA,LARGURA,TAMANHO_QUADRADO,ESPACO_GRID,ESPACO_GRID,corPonto);

	// for (auto &lado : lados){
	// 	grid->pintaQuadrado(lado.first.first,lado.first.second);
	// 	grid->pintaQuadrado(lado.second.first,lado.second.second);
	// }

	// pontos = grid->preenchimentoVarredura(lados);

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(apertaTecla);
	glutMainLoop();
}

