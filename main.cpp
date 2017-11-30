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
Janela * janela = nullptr,*janelaSelecao = nullptr;
std::vector<std::pair<int, int>> pontosJanela,tempPontosSelecao;

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

void reshape(int width,int height){}

void limpaVariaveisGlobais(bool limpaGrid){
	delete janela;
	janela = nullptr;

	delete janelaSelecao;
	janelaSelecao = nullptr;

	if (limpaGrid)
		grid->limpa();

	quadradosSelecionados.clear();
	lados.clear();
	pontosJanela.clear();

	glFlush();
	
}

void apertaTecla(unsigned char key, int x, int y){

	if (key == 'c')
		return limpaVariaveisGlobais(true);

	if (modo == '3' && key == 'p'){
		std::cout << "Preenche!" << std::endl;

		if (!lados.empty()){
			grid->preenchimentoVarredura(lados);
			std::cout << "Terminou de preencher!" << std::endl;
			lados.clear();
			glFlush();
		}
	}

	if (modo != key){
		modo = key;

		return limpaVariaveisGlobais(false);
	}

}

void mouseMovimento(int x , int y){

	std::vector<std::pair<int,int>> linhaDesenhada;
	int *coordGrid = grid->mapCoordenadaRealParaGrid(x, y),
		xMaxGrid = LARGURA / TAMANHO_QUADRADO - 1,
		yMaxGrid = ALTURA / TAMANHO_QUADRADO - 1;


	if (coordGrid[0] < 0 || coordGrid[1] < 0 || coordGrid[0] > xMaxGrid || coordGrid[1] > yMaxGrid)
		return;

	std::pair<int, int> p1 = std::make_pair(quadradosSelecionados[0].first, quadradosSelecionados[0].second),
						p2 = std::make_pair(coordGrid[0], coordGrid[1]),
						p3,
						p4;
	if (modo == '5'){
		int xMin = p1.first, xMax = p2.first, yMin = p1.second, yMax = p2.second;

		if (!janelaSelecao)
			janelaSelecao = new Janela(xMin, yMin, xMax, yMax);
		else{

			std::cout << janelaSelecao->getXMin() << ',' << janelaSelecao->getYMin() << ' ';
			std::cout << janelaSelecao->getXMax() << ',' << janelaSelecao->getYMax() << std::endl;

			grid->apagaPontos(janelaSelecao->getPontosDentro());
			grid->apagaPontos(janelaSelecao->getPontosBorda());

			janelaSelecao->setXMax(xMax);
			janelaSelecao->setYMax(yMax);
		}

		grid->pintaJanela(*janelaSelecao);

		std::cout << "nPontosDentro: " << janelaSelecao->getPontosDentro().size() << std::endl;
		// grid->apagaPontos(janelaSelecao->getPontosDentro());

	}

	glFlush();
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
					//instancia a janela
					janela = new Janela(xMin,yMin,xMax,yMax);
					grid->pintaJanela(*janela);

					quadradosSelecionados.clear();
					return ;
				}
				else{
					//Já pintou janela
					int xMin = janela->getXMin(), yMin = janela->getYMin(),
						xMax = janela->getXMax(), yMax = janela->getYMax();
					std::pair<std::pair<int, int>, std::pair<int, int>> novosPontos;
			
					std::pair<int, int> p1 = std::make_pair(xMin, yMin),
										p2 = std::make_pair(xMax, yMax),
										p3 = std::make_pair(xMax, yMin),
										p4 = std::make_pair(xMin, yMax),
										pontoForaTeste = std::make_pair(-1, -1);



					novosPontos = janela->cohenSutherland(primeiroPonto, segundoPonto);

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

	glFlush();
}
int main(int argc,char *argv[]){

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (LARGURA, ALTURA);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Computacao Grafica");
	init2D(0.0,0.0,0.0);

	grid = new Grid(ALTURA,LARGURA,TAMANHO_QUADRADO,ESPACO_GRID,ESPACO_GRID,corPonto);


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMovimento);
	glutKeyboardFunc(apertaTecla);
	glutMainLoop();
}

