#include "../../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;


//ГОСТ 5915-70
//Исполнение 1

struct GOST_5915_70
{
    double d;       // 1 - Диаметр резьбы крепежного изделия (Главный параметр)
    double s;       // 2 - Размер под ключ
    double dw;      // 3 - Диаметр описанной окружности
    double m;       // 4 - высота
};

std::vector<GOST_5915_70> configsGaika = {
    //1     2    3    4   
    {6,  10, 9.0 , 5.2},
    {8,  13, 11.7, 6.8},
    {10, 16, 14.5, 8.4},
    {12, 18, 16.5, 10.8},
    {14, 21, 19.2, 12.8},
    {16, 24, 22.0, 14.8},
    {18, 27, 24.8, 16.4},
    {20, 30, 27.7, 18.0},
    {22, 34, 31.4, 19.8},
    {24, 36, 33.2, 21.5},
    {27, 41, 38.0, 23.6},
    {30, 46, 42.7, 25.6},
    {36, 55, 51.1, 31.0},
    {42, 65, 59.9, 34.0},
    {48, 75, 69.4, 38.0},
};


void SketchForSixCorner1(RPArray<MbContour>& _arrContours, double S) {

    // Вычисление радиуса шестигранника
    const double radius = S / sqrt(3);
    const double alpha = 2 * M_PI / 6;

    SArray<MbCartPoint> arrPnts(6);
    MbContour* pContour = new MbContour();

    for (int i = 0; i < 6; i++) {
        double x = radius * cos(i * alpha);
        double y = radius * sin(i * alpha);
        arrPnts.Add(MbCartPoint(x, y));
    }

    for (int i = 0; i < 6; i++) {
        int lastIndex = i + 1;

        if (i == 5) {
            lastIndex = 0;
        }

        MbLineSegment* pLine = new MbLineSegment(arrPnts[i], arrPnts[lastIndex]);
        pContour->AddSegment(pLine);
    }


    _arrContours.push_back(pContour);
}

void SketchForRotateTriangle1(RPArray<MbContour>& _arrContours, double S, double K)
{
    // Вычисление радиуса шестигранника
    const double radius = (S + 2) / sqrt(3);

    double x = radius * cos(M_PI / 6);
    double y = radius * sin(M_PI / 6);
    //TOZO: теорему косинусо надо для 3 точки
    SArray<MbCartPoint> arrPnts(6);
    arrPnts.Add(MbCartPoint((0.95 * S) / 2, -K));
    arrPnts.Add(MbCartPoint(radius, -K));
    arrPnts.Add(MbCartPoint(radius, -K + K / 3));

    MbLineSegment* pLine0 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[2], arrPnts[0]);
    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine0);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);

    _arrContours.push_back(pContour);
}

void SketchForRotateTriangle3(RPArray<MbContour>& _arrContours, double S, double K)
{
    // Вычисление радиуса шестигранника
    const double radius = (S + 2) / sqrt(3);

    double x = radius * cos(M_PI / 6);
    double y = radius * sin(M_PI / 6);
    //TOZO: теорему косинусо надо для 3 точки
    SArray<MbCartPoint> arrPnts(6);
    arrPnts.Add(MbCartPoint((0.95 * S) / 2, 0));
    arrPnts.Add(MbCartPoint(radius, 0));
    arrPnts.Add(MbCartPoint(radius, 0 - K / 3));

    MbLineSegment* pLine0 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[2], arrPnts[0]);
    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine0);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::NutBoltGostTTOR(double diam)
{
    int index = 0;

    if (diam == 6) {
        index = 0;
    }
    else if (diam == 8) {
        index = 1;
    }
    else if (diam == 10) {
        index = 2;
    }
    else if (diam == 12) {
        index = 3;
    }
    else if (diam == 14) {
        index = 4;
    }
    else if (diam == 16) {
        index = 5;
    }
    else if (diam == 18) {
        index = 6;
    }
    else if (diam == 20) {
        index = 7;
    }
    else if (diam == 22) {
        index = 8;
    }
    else if (diam == 24) {
        index = 9;
    }
    else if (diam == 27) {
        index = 10;
    }
    else if (diam == 30) {
        index = 11;
    }
    else if (diam == 36) {
        index = 12;
    }
    else if (diam == 42) {
        index = 13;
    }
    else if (diam == 48) {
        index = 14;
    }

    const double d = configsGaika[index].d;
    const double s = configsGaika[index].s;
    const double dw = configsGaika[index].dw;
    const double m = configsGaika[index].m;

    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);


    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));

    //ВЫДАВЛИВАНИЕ
    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    RPArray<MbContour> arrContours2;
    RPArray<MbContour> arrContours3;
    SketchForSixCorner1(arrContours, s);

    MbSweptData sweptData1(*pPlaneYZ, arrContours);

    MbVector3D dirX(1, 0, 0);
    MbVector3D dirY(0, 1, 0);
    MbVector3D dirZ(0, 0, 1);

    MbSolid* pSolid1 = nullptr;

    MbResultType res1 = ::ExtrusionSolid(sweptData1, dirX, NULL, NULL, false, ExtrusionValues(0, m), operNames, cNames, pSolid1);

    SketchForRotateTriangle1(arrContours2, s, m);
    SketchForRotateTriangle3(arrContours3, s, m);
    ////Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(1, 0, 0));
    MbSweptData sweptDataRev(*pPlaneXY, arrContours2);
    MbSweptData sweptDataRev1(*pPlaneXY, arrContours3);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Ось вращения для построения тела:
    MbAxis3D axiX(pl.GetAxisX());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid32 = nullptr;
    MbSolid* pSolid33 = nullptr;
    MbResultType res1321312 = ::RevolutionSolid(sweptDataRev, axiX, revParms, operNames, cNames, pSolid32);
    MbResultType res1123 = ::RevolutionSolid(sweptDataRev1, axiX, revParms, operNames, cNames, pSolid33);

    SolidSPtr Solid1(pSolid1);
    SolidSPtr Solid2(pSolid32);
    SolidSPtr Solid3(pSolid33);

    // Объединение
    SolidSPtr SolidRes1 = nullptr;
    SolidSPtr SolidRes2 = nullptr;
    SolidSPtr SolidRes3 = nullptr;

    // Именователь граней для построения тела с помощью булевой операции 
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);
    MbBooleanOperationParams parametersUnion(bo_Union, true, operBoolNames);
    MbBooleanOperationParams parametersDif(bo_Difference, true, operBoolNames);

    ::BooleanResult(Solid1, cm_Copy, Solid2, cm_Copy, parametersDif, SolidRes1);
    ::BooleanResult(SolidRes1, cm_Copy, Solid3, cm_Copy, parametersDif, SolidRes2);

    SolidSPtr outerPipe;

    SpacePointsVector bigRadiusCylPnts;
    bigRadiusCylPnts.push_back(MbCartPoint3D(0, 0, 0));
    bigRadiusCylPnts.push_back(MbCartPoint3D(-m, 0, 0));
    bigRadiusCylPnts.push_back(MbCartPoint3D(0, d / 2, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPnts, blockNames), outerPipe);

    ::BooleanResult(SolidRes2, cm_Copy, outerPipe, cm_Copy, parametersDif, SolidRes3);

    return SolidRes3;
}