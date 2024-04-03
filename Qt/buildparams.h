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

		double ttDiam;         // 2  - ������� ������������� �����
		double ttThickness;    // 3  - ������� ������������� �����
		double ktDiam;         // 4  - ������� ��������� �����
		double ktThickness;    // 5  - ������� ��������� �����
		double lK;             // 6  - ����� ���������� �����
		double L;              // 7  - ������� �������� ��������
		double dV;             // 8  - ������� ������
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
	// TODO: 1) ����������� ����� ���� ��������� ������ 
	//struct testConfigParams
	//{
	//	std::string name; // �������� �������� ���� 455 4.5 32
	//	int lengthK; // �������� �� �������
	//	int length0; //  �������� �� �������
	//	int LENGTH; // �������� �� �������
	//};
}