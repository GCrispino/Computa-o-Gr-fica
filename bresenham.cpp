#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include <GL/glut.h>
#include <GL/glu.h>

#define ALTURA 800
#define LARGURA 800
#define ESPACO_GRID 5
#define TAMANHO_QUADRADO 30


std::vector< std::vector< std::vector <double> > > frameBuffer(ALTURA);

bool firstClick = true;
int pontoOrigem[2];
std::vector< std::pair<int,int> > quadradosSelecionados;

enum class TrocaReflexao {NAO_TROCA,TROCA_X,TROCA_Y,TROCA_XY};

void initFrameBuffer(std::vector< std::vector< std::vector <double> > > &frameBuffer){
	for (auto &linha: frameBuffer)
		for (unsigned int j = 0;j < LARGURA;++j)
			linha.push_back(std::vector<double> {0,0,0});
}

void pintaFrameBuffer(std::vector< std::vector< std::vector <double> > > &frameBuffer, double *cor, int x,int y){
	frameBuffer[x][y][0] = cor[0];
	frameBuffer[x][y][1] = cor[1];
	frameBuffer[x][y][2] = cor[2];
}

void troca(int &x,int &y){
	int aux = x;
	x = y;
	y = aux;
}

std::stack<TrocaReflexao> reflexao(std::pair<int,int> &p1,std::pair<int,int> &p2){
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

void reflexaoInversa(std::stack<TrocaReflexao> tiposTroca,int &x, int &y){
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

//mapea coordenada do grid para coordenada real do monitor
int * mapCoordenadaGridParaReal(int x, int y){
	int * coordenadaReal = new int[2];

	coordenadaReal[0] = x * TAMANHO_QUADRADO + ESPACO_GRID;
	coordenadaReal[1] = y * TAMANHO_QUADRADO + ESPACO_GRID;

	return coordenadaReal;
}

//mapea coordenada real do monitor para coordenada do grid
int * mapCoordenadaRealParaGrid(int x, int y){
	int * coordenadaGrid = new int[2];

	coordenadaGrid[0] = (x - ESPACO_GRID) / TAMANHO_QUADRADO;
	coordenadaGrid[1] = (y - ESPACO_GRID) / TAMANHO_QUADRADO;

	return coordenadaGrid;
}

void desenhaQuadrado(double cor[3],int x,int y,int tamanhoQuadrado){
	glColor3dv(cor);
	glBegin(GL_POINTS);
		for (int i = 0;i < tamanhoQuadrado;++i){
			glVertex2i(x + i,y);
			glVertex2i(x + i,y + tamanhoQuadrado);
			glVertex2i(x,y + i);
			glVertex2i(x + tamanhoQuadrado,y + i);
		}
	glEnd();
}


void encheQuadrado(double cor[3],int x,int y){
	int * coordenadaReal = mapCoordenadaGridParaReal(x,y);
	int xReal = coordenadaReal[0], yReal = coordenadaReal[1];

	glColor3dv(cor);
	glBegin(GL_POINTS);
		for (int i = xReal + 1;i < xReal + TAMANHO_QUADRADO;++i)
			for (int j = yReal + 1;j < yReal + TAMANHO_QUADRADO;++j)
				glVertex2i(i,j);
	glEnd();

	glFlush();
}

std::vector< std::pair<int,int> > bresenham(int x1, int y1,int x2, int y2){

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

void desenhaLinhaQuadrados(double cor[3],int x,int y,int tamanhoQuadrado){
	for (int i = x;i + tamanhoQuadrado< LARGURA;i += tamanhoQuadrado)
		desenhaQuadrado(cor,i,y,tamanhoQuadrado);
}

void criaGrid(int tamanhoQuadrado,double cor[3]){
	for (int y = ESPACO_GRID;y + tamanhoQuadrado< ALTURA;y += tamanhoQuadrado)
		desenhaLinhaQuadrados(cor,ESPACO_GRID,y,tamanhoQuadrado);
}

void init2D(float r, float g, float b)
{
	initFrameBuffer(frameBuffer);

	glClearColor(r,g,b,0.0);  
	glMatrixMode (GL_PROJECTION);
	glOrtho( 0, LARGURA, ALTURA, 0, 0, 1 );

	glMatrixMode (GL_MODELVIEW);	

	glClear(GL_COLOR_BUFFER_BIT);
}

void display(void)
{
	// glClear(GL_COLOR_BUFFER_BIT);
	// glColor3f(1.0, 0.0, 0.0);

	// glBegin(GL_POINTS);
	// for(int i = 0; i < 400; i++)
	// {
	// 	glVertex2i(i,100);
	// }
	// glEnd();

	// //draw a line
	// glBegin(GL_LINES);
	// 	glVertex2i(10,10);
	// 	glVertex2i(200,100);
	// glEnd();

	glFlush();
}

void desenhaPonto(double cor[3],int x, int y){
	glColor3f(cor[0], cor[1], cor[2]);
	glPointSize(20);
		glBegin(GL_POINTS);
			glVertex2i(x,y);
		glEnd();
}

void mouse(int btn, int state , int x , int y){
	if (state == GLUT_DOWN){
		double cor[] = {1.0,0.0,0.0};

		int * coordenadaGrid = mapCoordenadaRealParaGrid(x,y);

		pintaFrameBuffer(frameBuffer, cor, coordenadaGrid[0], coordenadaGrid[1]);
		encheQuadrado(cor,coordenadaGrid[0],coordenadaGrid[1]);

		quadradosSelecionados.push_back(std::make_pair(coordenadaGrid[0],coordenadaGrid[1]));

		if (quadradosSelecionados.size() == 2){
			std::vector< std::pair<int,int> > pontos;
			std::pair<int,int> &primeiroPonto = quadradosSelecionados[0],&segundoPonto = quadradosSelecionados[1];
			TrocaReflexao tipoTroca;
			std::stack<TrocaReflexao> tiposTroca;


			std::cout << "Primeiro ponto: " << primeiroPonto.first << ',' << primeiroPonto.second << std::endl;
			std::cout << "Segundo ponto: " << segundoPonto.first << ',' << segundoPonto.second << std::endl;
			std::cout << std::endl;



			tiposTroca = reflexao(primeiroPonto,segundoPonto);
			pontos = bresenham(primeiroPonto.first,primeiroPonto.second,segundoPonto.first,segundoPonto.second);

			std::cout << "Depois da reflexão: " << std::endl;
			std::cout << "Primeiro ponto: " << primeiroPonto.first << ',' << primeiroPonto.second << std::endl;
			std::cout << "Segundo ponto: " << segundoPonto.first << ',' << segundoPonto.second << std::endl;
			std::cout << std::endl;
			


			std::cout << "Depois da reflexão inversa: ";
			//reflete cada ponto e os pinta
			for (unsigned int i = 0;i < pontos.size();++i){
				std::pair<int,int> &ponto = pontos[i];
				reflexaoInversa(tiposTroca,ponto.first,ponto.second);
				std::cout << "Ponto: " << ponto.first << ',' << ponto.second << " ";
				encheQuadrado(cor,ponto.first,ponto.second);

				pintaFrameBuffer(frameBuffer, cor, ponto.first,ponto.second);
			}
			std::cout << std::endl << std::endl;


			quadradosSelecionados.clear();
		}

	}
}

int main(int argc,char *argv[])
{
	double corPonto[] = {1.0,0.0,0.0};

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (LARGURA, ALTURA);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Computacao Grafica");
	init2D(0.0,0.0,0.0);
	criaGrid(TAMANHO_QUADRADO,corPonto);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
}