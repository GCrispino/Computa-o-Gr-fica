#include "Quadrado.hpp"
#include <iostream>
#include <GL/glut.h>


Quadrado::Quadrado(int xReal,int yReal,int tamanho,double cor[3]):xReal(xReal),yReal(yReal),tamanho(tamanho){
	this->cor[0] = cor[0];
	this->cor[1] = cor[1];
	this->cor[2] = cor[2];

	glColor3dv(this->cor);
	glBegin(GL_POINTS);
		for (int i = 0;i < this->tamanho;++i){
			glVertex2i(this->xReal + i,this->yReal);
			glVertex2i(this->xReal + i,this->yReal + this->tamanho);
			glVertex2i(this->xReal,this->yReal + i);
			glVertex2i(this->xReal + this->tamanho,this->yReal + i);
		}
	glEnd();

}

Quadrado::Quadrado(const Quadrado &q)
:xReal(q.xReal),yReal(q.yReal),tamanho(q.tamanho){
	this->cor[0] = q.cor[0];
	this->cor[1] = q.cor[1];
	this->cor[2] = q.cor[2];
}

void Quadrado::pinta() const{
	glColor3dv(cor);
	glBegin(GL_POINTS);
		for (int i = this->xReal + 1;i < this->xReal + this->tamanho;++i)
			for (int j = this->yReal + 1;j < this->yReal + this->tamanho;++j)
				glVertex2i(i,j);
	glEnd();

}

const int Quadrado::getXReal() const{
	return this->xReal;
}

const int Quadrado::getYReal() const{
	return this->yReal;
}

const double * Quadrado::getCor() const{
	return this->cor;
}

void Quadrado::setCor(const double * cor){
	this->cor[0] = cor[0];
	this->cor[1] = cor[1];
	this->cor[2] = cor[2];
}