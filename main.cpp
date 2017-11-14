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
bool janela = false;
std::vector<std::pair<int, int>> pontosJanela;

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

void limpaVariaveisGlobiais(bool limpaGrid){
	janela = false;

	if (limpaGrid)
		grid->limpa();

	quadradosSelecionados.clear();
	lados.clear();
	pontosJanela.clear();	
}

void apertaTecla(unsigned char key, int x, int y){

	if (key == 'c')
		return limpaVariaveisGlobiais(true);

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

		limpaVariaveisGlobiais(false);
	}

}

void mouse(int btn, int state , int x , int y){

	if (state == GLUT_DOWN){

		int *coordenadaGrid = grid->mapCoordenadaRealParaGrid(x, y);

		if (modo == '2'){
			std::cout << "Preenchimento recursivo! " << std::endl;
			std::cout << "Cor ponto:  " << corPonto[0] << ',' << corPonto[1] << ',' << corPonto[2] << std::endl;

			int *coordenadasGrid = grid->mapCoordenadaRealParaGrid(x, y);
			int xGrid = coordenadasGrid[0], yGrid = coordenadasGrid[1];
			std::cout <<"COR" << grid->getCorFrameBuffer(xGrid,yGrid).data()[0] << std::endl;
			grid->preenchimentoRecursivo(xGrid, yGrid , corPonto, grid->getCorFrameBuffer(xGrid,yGrid).data());
			return;
		}


		grid->pintaFrameBuffer(corPonto, coordenadaGrid[0], coordenadaGrid[1]);
		grid->pintaQuadrado(coordenadaGrid[0], coordenadaGrid[1]);
		quadradosSelecionados.push_back(std::make_pair(coordenadaGrid[0], coordenadaGrid[1]));
		

		if (quadradosSelecionados.size() == 2){
			std::pair<int, int> &primeiroPonto = quadradosSelecionados[0], &segundoPonto = quadradosSelecionados[1];

			std::cout << "insere lados!" << std::endl;
			lados.push_back(std::make_pair(primeiroPonto,segundoPonto));

			if (modo == '4'){
				if (!janela){
					//pinta a janela
					//===============================================================================================
					std::pair<int, int> p1 = primeiroPonto, p2 = segundoPonto,
										p3, p4;

					int xMin = p1.first, xMax = p2.first, yMin = p1.second, yMax = p2.second;

					p3 = std::make_pair(xMax, yMin);
					p4 = std::make_pair(xMin, yMax);

					pontosJanela.push_back(p1);
					pontosJanela.push_back(p2);
					pontosJanela.push_back(p3);
					pontosJanela.push_back(p4);

					int *coordenadaGrid = grid->mapCoordenadaRealParaGrid(x, y);

					//pinta a janela
					grid->pintaLinha(p1, p3);
					grid->pintaLinha(p2, p4);

					//reseta valores dos pontos porque são modificados nas chamadas de 'Grid::pintaLinha()'
					p1 = primeiroPonto;
					p2 = segundoPonto;
					p3 = std::make_pair(xMax, yMin);
					p4 = std::make_pair(xMin, yMax);

					grid->pintaLinha(p1, p4);
					grid->pintaLinha(p2, p3);
					//===============================================================================================


					janela = true;
					quadradosSelecionados.clear();
					return ;
				}
				else{
					std::cout << "Já pintou janela!" << std::endl;
					std::pair<std::pair<int, int>, std::pair<int, int>> novosPontos;
					std::pair<int, int> p1 = pontosJanela[0],
										p2 = pontosJanela[1],
										p3 = pontosJanela[2],
										p4 = pontosJanela[3],
										pontoForaTeste = std::make_pair(-1,-1);

					int xMin = p1.first,
						yMin = p1.second,
						xMax = p2.first,
						yMax = p2.second;

					novosPontos = grid->cohenSutherland(primeiroPonto, segundoPonto, xMin, xMax, yMin, yMax);

					std::cout << "Novos pontos: " << std::endl;
					std::cout << novosPontos.first.first << ',' << novosPontos.first.second << std::endl;
					std::cout << novosPontos.second.first << ',' << novosPontos.second.second << std::endl << std::endl;


					//pinta pontos do lado de preto. Caso eles estejam fora, assim não vão ser pintados
					double corPreta[3] = {0.0,0.0,0.0};
					Quadrado qP1 = grid->getQuadrado(primeiroPonto.first,primeiroPonto.second),
							 qP2 = grid->getQuadrado(segundoPonto.first,segundoPonto.second);

					qP1.setCor(corPreta);
					qP1.pinta();
					qP2.setCor(corPreta);
					qP2.pinta();
					//atualiza frame buffer com os pontos apagados
					grid->pintaFrameBuffer(corPreta, primeiroPonto.first, primeiroPonto.second);
					grid->pintaFrameBuffer(corPreta, segundoPonto.first, segundoPonto.second);

					primeiroPonto = novosPontos.first;
					segundoPonto = novosPontos.second;

					if (primeiroPonto == pontoForaTeste && segundoPonto == pontoForaTeste){
						std::cout << "Ta fora!" << std::endl;
						quadradosSelecionados.clear();
						return;
					}

				}
			}

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


	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(apertaTecla);
	glutMainLoop();
}

