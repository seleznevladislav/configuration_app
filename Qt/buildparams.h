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
	// TODO: 1) ����������� ����� ���� ��������� ������ 
	//struct testConfigParams
	//{
	//	std::string name; // �������� �������� ���� 455 4.5 32
	//	int lengthK; // �������� �� �������
	//	int length0; //  �������� �� �������
	//	int LENGTH; // �������� �� �������
	//};
}