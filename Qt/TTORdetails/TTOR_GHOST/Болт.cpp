#include "../../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

// ГОСТ 7798-70
// Исполнение 1

struct GOST_7798_70
{
    double d;      // 1 - Номинальный диаметр (Главный параметр)
    double K;      // 2 - Высота головки
    double l;      // 3 - Высота стержня
    double b;      // 4 - Высота резьбы
    double S;      // 5 - Размер ключа
    double hw;     // 6 - Высота порога
    double dw;     // 7 - Диаметр порога
    bool recomend; // 8 - Является ли параметр рекомендуемым
    double step;     // 9 - Шаг резьбы
};

std::vector<GOST_7798_70> configsBolt = {
    //1     2    3    4   5    6    7
    {6,   4,    25,  18, 10, 0.6, 8.7  , true, 1},
    {8,   5.3,  30,  22, 13, 0.6, 11.5 , true, 1},
    {10,  6.4,  35,  26, 16, 0.6, 14.5 , true, 1.25},
    {12,  7.5,  40,  30, 18, 0.6, 16.5 , true, 1.25},
    {14,  8.8,  45,  34, 21, 0.6, 19.2 ,false, 1.5},
    {16,  10,   50,  38, 24, 0.8, 22   , true, 1.5},
    {18,  12,   55,  42, 27, 0.8, 24.8 ,false, 1.5},
    {20,  12.5, 60,  46, 30, 0.8, 27.7 , true, 1.5},
    {22,  14,   65,  50, 34, 0.8, 31.4 ,false, 1.5},
    {24,  15,   75,  54, 36, 0.8, 33.2 , true, 2},
    {27,  17,   80,  58, 41, 0.8, 38   ,false, 2},
    {30,  18.7, 85,  68, 46, 0.8, 42.7 , true, 2},
    {36,  22.5, 90,  72, 55, 0.8, 51.1 , true, 3},
    {42,  26,   95,  76, 65, 0.8, 59.9 , true, 3},
    {48,  30,   100, 80, 75, 0.8, 69.34, true, 3},
};


void SketchForSixCorner(RPArray<MbContour>& _arrContours, double S) {

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

void SketchForRotate(RPArray<MbContour>& _arrContours, int index, double d, double dw, double hw, double l)
{
    SArray<MbCartPoint> arrPnts(6);
    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint(dw / 2, 0));
    arrPnts.Add(MbCartPoint(dw / 2, hw));
    arrPnts.Add(MbCartPoint(d / 2, hw));
    arrPnts.Add(MbCartPoint(d / 2, l));
    arrPnts.Add(MbCartPoint(0, l));


    MbLineSegment* pLine0 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[5], arrPnts[0]);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine0);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);

    _arrContours.push_back(pContour);
}

void SketchForRotateTriangle(RPArray<MbContour>& _arrContours, double S, double K)
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

void CreateSketchForPath(RPArray<MbContour>& _arrContours, double d, double step)
{
    //создаем умный массив из 4 точек
    SArray<MbCartPoint>arrPnts(3);
    step = step - 0.1;
    //рассчитываем координты точек по которым построим эскиз сечения и добавляем точки в массив
    arrPnts.Add(MbCartPoint(d / 2, 0));
    arrPnts.Add(MbCartPoint(d / 2, step));
    arrPnts.Add(MbCartPoint(d / 2 - step, step / 2));

    //соединяем точки линиями
    MbLineSegment* pS1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pS2 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pS3 = new MbLineSegment(arrPnts[2], arrPnts[0]);

    //создаем контур
    MbContour* pContour = new MbContour();

    //добавляем созданные линии в контур
    pContour->AddSegment(pS1);
    pContour->AddSegment(pS2);
    pContour->AddSegment(pS3);

    //добавляем контур в массив контуров
    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::BoltGostTTOR(double diam, bool simpleModeOn)
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

    const double d = configsBolt[index].d;
    const double K = configsBolt[index].K;
    const double l = configsBolt[index].l;
    const double b = configsBolt[index].b;
    const double S = configsBolt[index].S;
    const double hw = configsBolt[index].hw;
    const double dw = configsBolt[index].dw;
    const bool recomend = configsBolt[index].recomend;
    const double step = configsBolt[index].step;

    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Для Филета
    MbSNameMaker filletNames(ct_FilletSolid, MbSNameMaker::i_SideNone, 0);
    SmoothValues params;
    params.distance1 = (d - dw) / 2;
    params.distance2 = (d - dw) / 2;
    params.form = st_Fillet;
    params.conic = 0;
    params.prolong = false;
    params.smoothCorner = SmoothValues::ec_uniform;
    params.keepCant = ts_negative;
    params.strict = true;

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));

    //ВЫДАВЛИВАНИЕ
    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    SketchForSixCorner(arrContours, S);

    MbSweptData sweptData1(*pPlaneYZ, arrContours);

    MbVector3D dirX(1, 0, 0);
    MbVector3D dirY(0, 1, 0);
    MbVector3D dirZ(0, 0, 1);

    MbSolid* pSolid1 = nullptr;

    MbResultType res1 = ::ExtrusionSolid(sweptData1, dirX, NULL, NULL, false, ExtrusionValues(0, K), operNames, cNames, pSolid1);

    RPArray<MbContour> arrContours1;
    RPArray<MbContour> arrContours2;
    RPArray<MbContour> arrContours3;

    SketchForRotate(arrContours1, index, d, dw, hw, l);
    SketchForRotateTriangle(arrContours2, S, K);
    CreateSketchForPath(arrContours3, d, step);
    ////Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(1, 0, 0));
    MbPlane* pPlanetest = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(1, 0, 0));
    MbSweptData sweptData(*pPlaneXY, arrContours1);
    MbSweptData sweptDataRev(*pPlaneXY, arrContours2);
    MbSweptData sweptDataRevSwift(*pPlanetest, arrContours3);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Ось вращения для построения тела:
    MbAxis3D axiX(pl.GetAxisX());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid = nullptr;
    MbSolid* pSolid32 = nullptr;
    MbSolid* pSolidTest = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axiX, revParms, operNames, cNames, pSolid);
    MbResultType res1321312 = ::RevolutionSolid(sweptDataRev, axiX, revParms, operNames, cNames, pSolid32);

    SPtr<MbConeSpiral> spiral(MbConeSpiral::Create(MbCartPoint3D(0, 0, 0), MbCartPoint3D(b, 0, 0), MbCartPoint3D(0, d, 0), step, false));

    // Объект с параметрами операции построения тела заметания
    EvolutionValues paramsREf;

    // Служебные объекты-именователи для вызова геометрической операции
    MbSNameMaker operName321s(ct_CurveEvolutionSolid, MbSNameMaker::i_SideNone, 0);
    MbSNameMaker cNames1(ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> contourNames(1, 0, false);
    contourNames.Add(&cNames1);
    MbSNameMaker splineNames(ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0);


    SolidSPtr Solid(pSolid1);
    SolidSPtr Solid1(pSolid);
    SolidSPtr Solid2(pSolid32);


    // Объединение
    SolidSPtr SolidRes1 = nullptr;
    SolidSPtr SolidRes2 = nullptr;
    SolidSPtr SolidRes3 = nullptr;

    // Именователь граней для построения тела с помощью булевой операции 
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);
    MbBooleanOperationParams parametersUnion(bo_Union, true, operBoolNames);
    MbBooleanOperationParams parametersDif(bo_Difference, true, operBoolNames);




    ::BooleanResult(Solid, cm_Copy, Solid1, cm_Copy, parametersUnion, SolidRes1);
    ::BooleanResult(SolidRes1, cm_Copy, Solid2, cm_Copy, parametersDif, SolidRes2);

    int rebro = 4;
    // Скругление
    if (!simpleModeOn) {
        MbResultType resko = ::EvolutionSolid(sweptDataRevSwift, *spiral, paramsREf, operName321s, contourNames, splineNames, pSolidTest);
        pSolidTest->Move(MbVector3D(l - b, 0, 0));

        SolidSPtr Solid3(pSolidTest);
        ::BooleanResult(SolidRes2, cm_Copy, Solid3, cm_Copy, parametersDif, SolidRes3);
    }
    else {
        SolidRes3 = SolidRes2;
        rebro = 23;
    }

    RPArray<MbCurveEdge> allEdges;
    SolidRes3->GetEdges(allEdges);

    c3d::EdgesSPtrVector initCurves;
    initCurves.clear();
    initCurves.emplace_back(allEdges[rebro]);

    FacesSPtrVector initBounds;
    SolidSPtr filletBody;

    ::FilletSolid(*(MbSolid*)SolidRes3, cm_Copy, MbShellFilletValues(initCurves, initBounds, params, filletNames), filletBody);

    return filletBody;
}