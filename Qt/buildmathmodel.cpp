#include "BuildMathModel.h"

using namespace c3d;
using namespace std;
using namespace BuildMathModel;

// Инициализирую статическую переменную класса - обязательно
int ParametricModelCreator::colorScheme = 0;
int ParametricModelCreator::variantsConf = 0;
int ParametricModelCreator::variantsConfB = 0;
int ParametricModelCreator::assemblyHeight = 0;
int ParametricModelCreator::assemblyHeightTTOR = 0;

MbModel* ParametricModelCreator::CreatePneymocylinderModelTTRM(ConfigParams params, int configurationQuantity)
{
	MbModel* model = new MbModel();
	auto pneumocylinderAssembly = CreatePneumocylinderAssembly(params, configurationQuantity);
	model->AddItem(*pneumocylinderAssembly);
	return model;
}

MbModel* ParametricModelCreator::CreatePneymocylinderModelZarubin(BuildParamsForHeatExchangerTTOR params, int configurationQuantity)
{
	MbModel* model = new MbModel();
	auto pneumocylinderAssembly = CreateTTOR(params, configurationQuantity); // TOZO hideStandartDetails
	model->AddItem(*pneumocylinderAssembly);
	return model;
}

MbModel* ParametricModelCreator::CreatePneymocylinderModelFukina(ConfigParams_IP params)
{
	MbModel* model = new MbModel();
	auto pneumocylinderAssembly = CreateIP(params);
	model->AddItem(*pneumocylinderAssembly);
	return model;
}

MbModel* ParametricModelCreator::CreatePneymocylinderModelVasinkina(ConfigParams_IU params)
{
	MbModel* model = new MbModel();
	auto pneumocylinderAssembly = CreateIU(params);
	model->AddItem(*pneumocylinderAssembly);
	return model;
}