#include "BuildMathModel.h"

using namespace c3d;
using namespace std;
using namespace BuildMathModel;

// ������������� ����������� ���������� ������ - �����������
int ParametricModelCreator::colorScheme = 0;
int ParametricModelCreator::variantsConf = 0;
int ParametricModelCreator::variantsConfB = 0;

MbModel* ParametricModelCreator::CreatePneymocylinderModel(ConfigParams params)
{
	/*ParametricModelCreator::colorScheme = params.colorScheme;
	ParametricModelCreator::variantsConf = params.variantsConf;
	ParametricModelCreator::variantsConfB = params.variantsConfB;*/
	MbModel* model = new MbModel();
	auto pneumocylinderAssembly = CreatePneumocylinderAssembly(params);
	model->AddItem(*pneumocylinderAssembly);
	return model;
}

//MbModel* ParametricModelCreator::CreatePneymocylinderModelZarubin(BuildParamsZarubin params)
//{
//	ParametricModelCreator::colorScheme = params.colorScheme;
//	ParametricModelCreator::variantsConf = params.variantsConf;
//	ParametricModelCreator::variantsConfB = params.variantsConfB;
//	MbModel* model = new MbModel();
//	auto pneumocylinderAssembly = CreateTTOR(params);
//	model->AddItem(*pneumocylinderAssembly);
//	return model;
//}