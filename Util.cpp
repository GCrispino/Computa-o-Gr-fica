// #include "Util.hpp"

template <typename T>
void Util::troca(T &x, T &y)
{
	T aux = x;
	x = y;
	y = aux;
}