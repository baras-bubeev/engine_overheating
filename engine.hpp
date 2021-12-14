#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <vector>
#include <cmath>

class Engine
{
private:

	double				_I;		// момент инерции двигателя (кг*м^2)
	std::vector<double>	_Marr;	/* зависимость крутящего момента (Н*м)	*/
	std::vector<double>	_Varr;	/* от скорости вращения коленвала		*/
	double				_T;		// температура перегрева
	double				_Hm;	// коэффициент зависимости скорости нагрева от крутящего момента
	double				_Hv;	// коэффициент зависимости скорости нагрева от скорости вращения коленвала
	double				_C;		// коэффициент зависимости скорости охлаждения от температуры двигателя и окружающей среды

	double	_tEnv;		// температура окружающей среды
	double	_tEngine;	// температура двигателя
	double	_M;			//крутящий момент
	double	_V;			// скорость вращения коленвала
	// double	_a;		// ускорение коленвала a = M / I
	// double	_Vh;	// скорость нагрева двигателя Vh = M * Hm + V^2 * Hv
	// double	_Vc;	// скорость охлаждения двигателя Vc = C * (tEnv - tEngine)

public:

	Engine();
	~Engine();

	void	set_tEnv(double t){	_tEnv = t;	_tEngine = t;}		// инициализация температур среды и двигателя
	void	set_tEngine(double t){	_tEngine = t;}				// изменение температуры двигателя
	void	set_V(double v){	_V = v;}
	void	set_M(double m){	_M = m;}
	double	get_T(){	return (_T);}
	double	get_tEngine(){	return (_tEngine);}
	double	VhCalc(){	return (_M * _Hm + pow(_V, 2) * _Hv);}	// расчет скорости нагрева двигателя
	double	VcCalc(){	return (_C * (_tEnv - _tEngine));}		// расчет скорости охлаждения двигателя
	double	aCalc(double M){	return (M / _I);}				// расчет ускорения коленвала

	std::vector<double>	get_Varr(){	return (_Varr);}
	std::vector<double>	get_Marr(){	return (_Marr);}
};

Engine::Engine()
{
	_I = 10;
	_Marr = { 20, 75, 100, 105, 75, 0 };
	_Varr = { 0, 75, 150, 200, 250, 300 };
	_T = 110;
	_Hm = 0.01;
	_Hv = 0.0001;
	_C = 0.1;
}

Engine::~Engine()
{
}


#endif