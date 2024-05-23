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
		std::string name;      // 1  - ������������
		double ttDiam;         // 2  - ������� ������������� ����
		double ttThickness;    // 3  - ������� ������������� ����
		double ktDiam;         // 4  - ������� ��������� ����
		double ktThickness;    // 5  - ������� ��������� ����
		double lK;             // 6  - ����� ��������� ����
		double L;              // 7  - ����� ����� �������� �������� * (�������� �� ������������ � ���������)
		double dV;             // 8  - ������� ������
		double dU;             // 9  - ������� ������ ���������������
		double H;              // 10 - ������ ������ ���������������
		double H1;             // 11 - ������ ����� �� ������ (H1 * 2 = ����� ������)
		double l0;             // 12 - ���������� ����� �������
		double l1;             // 13 - ���������� ����� ������� ������� � ������� ��������������
		double l2;             // 14 - ���������� ����� ������� �������������� � ������
		double l3;             // 15 - ����� ������
		double t;              // 16 - ���������� ����� �������
		double Lt;             // 17 - ����� ������������� ���� (L-l3-t+40(����� ������))
		bool turnOnStandart;   // 18 - �������� ���
		bool simpleMode;       // 19 - ���������� �����
	};
	struct ConfigParams
	{
		std::string name;

		double assortmentInnerTubes;    // 1 - ������� ������������� �����
		double thicknessInnerTubes;     // 2 - ������� ������������� �����
		double assortmentOuterTubes;    // 3 - ������� ��������� �����
		double thicknessOuterTubes;     // 4 - ������� ��������� �����
		double assortmentCamera;        // 5 - ������� �����  
		double thicknessCamera;         // 6 - ������� ������ �����
		
		double lengthK;                 // 7 - ����� ��������� �����
		double length0;                 // 8 - ���������� ����� �������
		double LENGTH;                  // 9 - ����� ����� ��������

		double diametrY;                // 10 - ������� �������/�������� �����
		double t;                       // 11 - ���������� ����� �������

		double length1;                 // 12 - ���������� ����� ������� ����������������� � ������
		double length2;                 // 13 - ������ ����������������� ������
		double length3;                 // 14 - ������ ���������� ������
	};


	struct ConfigParams_IP
	{
		std::string name;
		int Dvne; //������� ������� ������
		double L_Base;//����� ������
		double s;//������� ������
		double p;//��������
		int DKr;//���������� ������� ������

		//���������� ��� ������� ����� �����:
		/*int l0 = 460;
		int l = 350;
		int l2 = 1750;
		int l3 = 6390;*/


		//��� �������
		double Dn;
		double Dy;

	};

	struct ConfigParams_IU
	{
		std::string name;
		int diamVne = 840;
		double Length = 5395.;
		int S = 20;
		double pressure = 1.6;
		int diam = 800;

		int D_KamVne = 540;
		int D_Kam = 500;
		int Kam_L = 562;

		double p = 1.6;


		//float CylS;
		//float EllS;
		//float PerS;

		//float napr;
		//float C;

		//���������� ��� ������� ����� �����:
		/*int l0 = 460;
		int l = 350;
		int l2 = 1750;
		int l3 = 6390;*/
	};

	struct data_fluidProperties {
		std::string name;     // 0 - ������������
		double p;             // 1 - ���������
		double c;		      // 2 - ����������� ���������������� (���/ ��*C)
		double laymbda;	      // 3 - ����������� ��������� ���������� �������� (��/ �*C)
		double u_viscocity;	  // 4 - ����������� ������������ �������� (���� �� � ����, ���� ��� �������������, ���� 0 ) (��/�)
		double viscocity;	  // 5 - ����������� �������������� �������� (���� �� � ����, ���� ��� �������������, ���� 0 ) (�2/�)
		double capacity;	  // 6 - �����������
		double Pr;			  // 7 - �����������
	};

	struct data_materialProperties {
		std::string name;				// 0 - ������������
		double tMaterial;               // 1 - ��� ����� ����������� ������������ ����������������
		double laymbdaMateral;		    // 2 - ����������� ���������������� (��/ m*c)
	};
	
	struct data_tubeProperties {
		double d1_diam;				// 0 - ������� ����� ������
		double d2_diam;				// 1 - ������� ����� �������
		double L_length;			// 4 - ����� �����
	};
}