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