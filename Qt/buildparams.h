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

	struct BuildParamsForHeatExchangerTTOR
	{
		std::string name;

		double ttDiam;         // 2  - Диаметр теплообменных труб
		double ttThickness;    // 3  - Толщина теплообменных труб
		double ktDiam;         // 4  - Диаметр кожуховых труб
		double ktThickness;    // 5  - Толщина кожуховых труб
		double lK;             // 6  - Длина кожуховых труб
		double L;              // 7  - Общая длина аппарата примерно * (Значение не используется в рассчетах)
		double dV;             // 8  - Диаметр камеры
		double dU;             // 9  - Диаметр фланца соединительного
		double H;              // 10 - Высота фланца соединительного
		double H1;             // 11 - Высота опоры от центра (H1 * 2 = целая высота)
		double l0;             // 12 - Расстояние между опорами
		double l1;             // 13 - Расстояние между фланцем боковым и фланцем соединительным
		double l2;             // 14 - Расстояние между фланцем соединительным и опорой
		double l3;             // 15 - Длина камеры
		double t;              // 16 - Расстояние между трубами
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


	struct ConfigParams_IP
	{
		std::string name;
		int Dv_Kzh;
		double L_Base;//длина кожуха
		double s;//толщина стенки
		double p;//давление
		int DKr;//внутренний диаметр камеры

		//расстояния для фланцев внизу кожух:
		int l0;
		//	int l = 350;
		int l2;
		int l3;
		//для фланцев
		double Dn;
		double Dy;
	};

	struct ConfigParams_IU
	{
		std::string name;
		double p = 1.6;
		int diam = 800;
		double Length = 5395.;
		int D_Kam = 500;
		int Kam_L = 562;
		int S = 20;
		int l = 350;
		int l2 = 1750;
		int l3 = 6390;
		//float CylS;
		//float EllS;
		//float PerS;

		//float napr;
		//float C;
	};

	struct date_fluidProperties{
		std::string name; // 0 - Наименование
		double p;         // 1 - Кинематический коэффициент вязкости
		double c;		  // 2 - Коэффициент теплопроводности,
		double laymbda;	  // 3 - Коэффициент линейного расширения вещества
	};
}