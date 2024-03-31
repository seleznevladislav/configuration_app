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
		int lengthK;
		int length0;
		int LENGTH;
		int length1;
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