#include "../../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

//ГОСТ 11371-78
//Исполнение 1

struct GOST_11371_78
{
    double d;       // 1 - Диаметр резьбы крепежного изделия (Главный параметр)
    double d1;      // 2 - Номинальный диаметр
    double d2;      // 3 - Внешний диаметр
    double s;       // 4 - Высота стержня
};

std::vector<GOST_11371_78> configsWasher = {
    //1     2    3    4   
    {6,   6.6,  12,  1.6},
    {8,   9.0,  16,  1.6},
    {10,  11.0, 20,  2.0},
    {12,  13.5, 24,  2.5},
    {14,  15.5, 28,  2.5},
    {16,  17.5, 30,  3.0},
    {18,  20.0, 34,  3.0},
    {20,  22.0, 37,  3.0},
    {22,  24.0, 39,  3.0},
    {24,  26.0, 44,  4.0},
    {27,  30.0, 50,  4.0},
    {30,  33.0, 56,  4.0},
    {36,  39.0, 66,  5.0},
    {42,  45.0, 78,  7.0},
    {48,  52.0, 92,  8.0},
};

void SketchForRotate(RPArray<MbContour>& _arrContours, double d1, double d2, double s)
{
    SArray<MbCartPoint> arrPnts(4);
    arrPnts.Add(MbCartPoint(d1 / 2, 0));
    arrPnts.Add(MbCartPoint(d1 / 2, s));
    arrPnts.Add(MbCartPoint(d2 / 2, s));
    arrPnts.Add(MbCartPoint(d2 / 2, 0));


    MbLineSegment* pLine0 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[3], arrPnts[0]);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine0);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::WasherGostTTOR(double diam)
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

    const double d1 = configsWasher[index].d1;
    const double d2 = configsWasher[index].d2;
    const double s = configsWasher[index].s;

    //ВЫДАВЛИВАНИЕ
    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    SketchForRotate(arrContours, d1, d2, s);


    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    ////Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(1, 0, 0));
    MbSweptData sweptData(*pPlaneXY, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Ось вращения для построения тела:
    MbAxis3D axiX(pl.GetAxisX());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axiX, revParms, operNames, cNames, pSolid);


    SolidSPtr Solid(pSolid);

    return Solid;
}