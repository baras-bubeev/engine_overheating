#include <iostream>
#include <thread>
#include <mutex>
#include "engine.hpp"

// Кусочно-линейная интерполяция
double	interpolation(double x, std::vector<double> X, std::vector<double> Y)
{
	int		size = X.size();
	int		i = 1;
	double	a0, a1, F;

	if (size < 2 || x < X[0] || x > X[size - 1])
	{
		std::cout << "Error\n";
		exit(1);
	}
	while (i < size && x > X[i])
		i++;
	a1 = (Y[i] - Y[i - 1]) / (X[i] - X[i - 1]);
	a0 = Y[i - 1] - (a1 * X[i - 1]);
	F = a0 + (a1 * x);
	return (F);
}

// void	cool(Engine& engine, std::mutex& tEng_mutex)
// {
//	double	t;
//
// 	while (true)
// 	{
//		t = engine.VcCalc();
// 		tEng_mutex.lock();
// 		engine.set_tEngine(t);
// 		tEng_mutex.unlock();
// 	}
// }

// void	warm(Engine& engine, std::mutex& tEng_mutex)
// {
//	double	t;
//
// 	while (true)
// 	{
//		t = engine.VhCalc();
// 		tEng_mutex.lock();
// 		engine.set_tEngine(t);
// 		tEng_mutex.unlock();
// 	}
// }

void	start(Engine& engine)
{
	// v - скорость варщения коленвала, m - крутящий момент
	// Tmax - температура перегрева, tEngine - температура двигателя
	double	v = 0, m, Tmax, tEngine;
	int		time = 0;

	Tmax = engine.get_T();
	tEngine = engine.get_tEngine();
	
	// цикл работы двигателя, одна итерация == 1 сек.
	while (tEngine <= Tmax)
	{
		m = interpolation(v, engine.get_Varr(), engine.get_Marr());	// интераполяция табл. значений крутящего момента
		v += engine.aCalc(m);										// увеличение скорости вращения коленвала
		engine.set_M(m);
		engine.set_V(v);
		tEngine += engine.VhCalc() + engine.VcCalc();				// изменение температуры двигателя
		engine.set_tEngine(tEngine);

		time++;
	}
	std::cout << "Overheating time: " << time << " seconds\n";
}

int	main()
{
	double		tEnv;		// температура окружающей среды
	Engine		engine;
	// std::mutex	tEng_mutex;

	while (true)
	{
		std::cout << "Please, enter the ambient temperature: ";
		std::cin >> tEnv;
		if (std::cin.fail())		// проверка корректности ввода
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Error: Incorrect input\n";
		}
		else break;
	}
	engine.set_tEnv(tEnv);
	start(std::ref(engine));
	// std::thread	run(start, std::ref(engine));
	// std::thread warming(cool, std::ref(engine), std::ref(tEng_mutex));
	// std::thread cooling(warm, std::ref(engine), std::ref(tEng_mutex));
	// run.join();
	// warming.detach();
	// cooling.detach();

	return (0);
}

// Закомментированные участки кода - попытка сделать через многопоточность
// в итоге пришел к выводу, что многопоточность в этой задаче ни к чему
// т.к. нагрев и охлаждение меняют один и тот же объект, из-за чего
// появляется необходимость ставить мьютексы, в результате этого потоки выполняются
// последовательно => многопоточность теряет смысл