#pragma once
#define CASE_ITEMNAME std::uint32_t(1111)

namespace BuildMathModel {
	struct BuildParams
	{
		double diam = 50.0;
		double length = 265;
		int colorScheme = 1;
		int variantsConf = 1;
		int variantsConfB = 1;
	};

	struct BuildParamsZarubin
	{
		std::string name;

		double ttDiam;         // 2  - Диаметр Теплообменные трубы
		double ttThickness;    // 3  - Толщина Теплообменные трубы
		double ktDiam;         // 4  - Диаметр Кожуховые трубы
		double ktThickness;    // 5  - Толщина Кожуховые трубы
		double lK;             // 6  - Длина Калжуховые трубы
		double L;              // 7  - Диаметр Аппарата примерно
		double dV;             // 8  - Диаметр камеры
		double dU;             // 9  - 
		double H;              // 10 - 
		double H1;             // 11 - 
		double l0;             // 12 - 
		double l1;             // 13 - 
		double l2;             // 14 - 
		double l3;             // 15 - 
		double t;              // 16 - 
	};
	struct ConfigParams
	{
		std::string name;

		double assortmentInnerTubes;    // 1 - Диаметр теплообменной трубы
		double thicknessInnerTubes;     // 2 - Толщина теплообменной трубы
		double assortmentOuterTubes;    // 3 - Диаметр кожуховой трубы
		double thicknessOuterTubes;     // 4 - Толщина кожуховой трубы
		double assortmentCamera;        // 5 - Диаметр камер  
		double thicknessCamera;         // 6 - Толщина стенок камер
		
		double lengthK;                 // 7 - Длина кожуховой трубы
		double length0;                 // 8 - Расстояние между опорами
		double LENGTH;                  // 9 - Общая длина аппарата

		double diametrY;                // 10 - Диаметр входной/выходной трубы
		double t;                       // 11 - Расстояние между трубами

		double length1;                 // 12 - Расстояние между камерой распределительной и опорой
		double length2;                 // 13 - Длинна распределительной камеры
		double length3;                 // 14 - Длинна поворотной камеры
	};

	// TODO: 1) Прописываем здесь свою структуру данных 
	//struct testConfigParams
	//{
	//	std::string name; // Название аппарата ТТРМ 455 4.5 32
	//	int lengthK; // Параметр из таблицы
	//	int length0; //  Параметр из таблицы
	//	int LENGTH; // Параметр из таблицы
	//};
}