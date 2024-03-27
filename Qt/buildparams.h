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
		double diam = 50.0;
		double length = 265;
		int colorScheme = 1;
		int variantsConf = 1;
		int variantsConfB = 1;

		double l0 = 3000.0;
		double distanceRezhetka = 500.0;
		double distanceTubesKozhux = (-1) * (distanceRezhetka + 90.0);
		double distanceTubesTeploobmen = -660.0;
	};
	struct ConfigParams
	{
		std::string name;
		int lengthK;
		int length0;
		int LENGTH;
	};
}