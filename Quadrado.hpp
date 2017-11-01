

class Quadrado{
	private:
		int xReal,
			yReal,
			tamanho;
		double cor[3];

	public:
		Quadrado(int xReal,int yReal,int tamanho,double cor[3]);
		Quadrado(const Quadrado &);

		void pinta() const;

		const int getXReal() const;
		const int getYReal() const;

		const double * getCor() const;
		void setCor(const double *);
};
