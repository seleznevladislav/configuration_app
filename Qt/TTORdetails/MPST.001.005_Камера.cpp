#include "../BuildMathModel.h"

using namespace BuildMathModel;

const int startX = 75;
const int startY = 30;
const double DEG_TO_RAD = M_PI / 180.0;

SolidSPtr HolyHole112(SolidSPtr* previus, int holes, double DiametrCircle, double radius, double depth) {
    // Именователь граней для построения тела с помощью булевой операции 
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);
    MbBooleanOperationParams parameters(bo_Union, true, operBoolNames);

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    double ANG1 = 0 * DEG_TO_RAD;

    SolidSPtr boltCyl;

    SpacePointsVector boltCylPnts;

    boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 0));
    boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), depth));
    boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1) + radius, DiametrCircle / 2 * sin(ANG1), 0));


    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);

    SolidSPtr news;
    ::BooleanResult(*previus, cm_Copy, boltCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), news);

    for (int i = 1; i <= holes; i++) {


        ANG1 = 360 / holes * i * DEG_TO_RAD;

        //double ANG2 = 22.5 + 45 * 1 * DEG_TO_RAD;

        SolidSPtr boltCyl;

        SpacePointsVector boltCylPnts;

        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 0));
        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), depth));
        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1) + radius, DiametrCircle / 2 * sin(ANG1), 0));

        SolidSPtr buffer;

        ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);

        ::BooleanResult(news, cm_Copy, boltCyl, cm_Copy,
            MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), buffer);

        news = buffer;

    }



    return news;
}

void CreateSketchForCapPart(RPArray<MbContour>& _arrContours, double l3, double Db, double Dm, double Dpaz, double Hb, double Hm, double Hpaz, double Thickness)
{
    // расчет длины камеры l3 - длина 
    const double Rasstoyanie = l3 - 157; // 243 мм значение для увеличения длины камеры

    double param1 = 157;
    double RADIUSB2 = 480;

    double RADIUSM1 = 67.351515;
    double RADIUSM2 = 81.351515;
    double hardcodePont1 = 54.570870;
    double hardcodePont2 = 65.914201;
    double hardcodePont3x = 273.123119;
    double hardcodePont4x = 281.328534;
    double hardcodePont5center = 233.648485;

    if (l3 > 400) {
        param1 = 257;
        RADIUSB2 = 580;

        RADIUSM1 = 172.834320;
        RADIUSM2 = 186.834320;

        hardcodePont1 = 141.989720;
        hardcodePont2 = 153.491233;
        hardcodePont3x = 322.708166;
        hardcodePont4x = 330.690346;
        hardcodePont5center = 224.16568;
    }

    SArray<MbCartPoint> arrPnts(20);
    arrPnts.Add(MbCartPoint(0, param1)); // 0
    arrPnts.Add(MbCartPoint(0, param1 - Thickness));
    arrPnts.Add(MbCartPoint(0, -(RADIUSB2 - param1))); //центр дуги большой 0
    arrPnts.Add(MbCartPoint(-hardcodePont3x, hardcodePont1)); // 3
    arrPnts.Add(MbCartPoint(-hardcodePont5center, 0));//центр дуги маленькой 0
    arrPnts.Add(MbCartPoint(-Dm, 0));
    arrPnts.Add(MbCartPoint(-Dm, -Rasstoyanie + Hm));
    arrPnts.Add(MbCartPoint(-Dpaz, -Rasstoyanie + Hm));
    arrPnts.Add(MbCartPoint(-Dpaz, -Rasstoyanie));
    arrPnts.Add(MbCartPoint(-Db, -Rasstoyanie));
    arrPnts.Add(MbCartPoint(-Db, -Rasstoyanie + Hb));
    arrPnts.Add(MbCartPoint(-(Dm + Hpaz + Thickness), -Rasstoyanie + Hb)); // 25мм это фаска
    arrPnts.Add(MbCartPoint(-(Dm + Thickness), -Rasstoyanie + Hb + Hpaz)); // 12
    arrPnts.Add(MbCartPoint(-(Dm + Thickness), 0));// 13
    arrPnts.Add(MbCartPoint(-hardcodePont5center, 0));//центр дуги маленькой 14
    arrPnts.Add(MbCartPoint(-hardcodePont4x, hardcodePont2));
    arrPnts.Add(MbCartPoint(0, -(RADIUSB2 - param1))); // Центр дуги большой 16
    arrPnts.Add(MbCartPoint(0, param1)); // 17

    const double RADIUSB1 = RADIUSB2 - Thickness;

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);

    MbArc* pArc1 = new MbArc(arrPnts[2], RADIUSB1, arrPnts[1], arrPnts[3], 1);
    MbArc* pArc2 = new MbArc(arrPnts[4], RADIUSM1, arrPnts[3], arrPnts[5], 1);


    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[6], arrPnts[7]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[7], arrPnts[8]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[8], arrPnts[9]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[9], arrPnts[10]);
    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[10], arrPnts[11]);
    MbLineSegment* pLine8 = new MbLineSegment(arrPnts[11], arrPnts[12]);
    MbLineSegment* pLine9 = new MbLineSegment(arrPnts[12], arrPnts[13]);

    MbArc* pArc3 = new MbArc(arrPnts[14], RADIUSM2, arrPnts[13], arrPnts[15], -1);
    MbArc* pArc4 = new MbArc(arrPnts[16], RADIUSB2, arrPnts[15], arrPnts[17], -1);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pArc1);
    pContour->AddSegment(pArc2);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);
    pContour->AddSegment(pLine8);
    pContour->AddSegment(pLine9);
    pContour->AddSegment(pArc3);
    pContour->AddSegment(pArc4);

    _arrContours.push_back(pContour);


    //viewManager->AddObject(Style(1, LIGHTRED), pContour);
}


void CreateSketchForWall(RPArray<MbContour>& _arrContours, double l3, double Db, double Dm, double Dpaz, double Hb, double Hm, double Hpaz, double Thickness)
{
    // расчет длины камеры l3 - длина 
    const double Rasstoyanie = l3 - 157; // 243 мм значение для увеличения длины камеры

    double param1 = 157;
    double RADIUSB2 = 480;

    double RADIUSM1 = 67.351515;
    double RADIUSM2 = 81.351515;
    double hardcodePont1 = 54.570870;
    double hardcodePont2 = 65.914201;
    double hardcodePont3x = 273.123119;
    double hardcodePont4x = 281.328534;
    double hardcodePont5center = 233.648485;

    if (l3 > 400) {
        param1 = 257;
        RADIUSB2 = 580;

        RADIUSM1 = 172.834320;
        RADIUSM2 = 186.834320;

        hardcodePont1 = 141.989720;
        hardcodePont2 = 153.491233;
        hardcodePont3x = 322.708166;
        hardcodePont4x = 330.690346;
        hardcodePont5center = 224.16568;
    }

    SArray<MbCartPoint> arrPnts(12);
    arrPnts.Add(MbCartPoint(0, param1 - Thickness));
    arrPnts.Add(MbCartPoint(0, -(RADIUSB2 - param1))); //центр дуги большой 0
    arrPnts.Add(MbCartPoint(-hardcodePont3x, hardcodePont1)); // 3
    arrPnts.Add(MbCartPoint(-hardcodePont5center, 0));//центр дуги маленькой 0
    arrPnts.Add(MbCartPoint(-Dm, 0));
    arrPnts.Add(MbCartPoint(-Dm, -Rasstoyanie + Hm));
    arrPnts.Add(MbCartPoint(Dm, -Rasstoyanie + Hm));
    arrPnts.Add(MbCartPoint(Dm, 0));//7
    arrPnts.Add(MbCartPoint(hardcodePont5center, 0));//центр дуги маленькой 0
    arrPnts.Add(MbCartPoint(hardcodePont3x, hardcodePont1));
    arrPnts.Add(MbCartPoint(0, -(RADIUSB2 - param1))); //центр дуги большой 0

    const double RADIUSB1 = RADIUSB2 - Thickness;

    MbArc* pArc1 = new MbArc(arrPnts[1], RADIUSB1, arrPnts[0], arrPnts[2], 1);
    MbArc* pArc2 = new MbArc(arrPnts[3], RADIUSM1, arrPnts[2], arrPnts[4], 1);


    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[6], arrPnts[7]);

    MbArc* pArc3 = new MbArc(arrPnts[8], RADIUSM1, arrPnts[7], arrPnts[9], 1);
    MbArc* pArc4 = new MbArc(arrPnts[10], RADIUSB1, arrPnts[9], arrPnts[0], 1);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pArc1);
    pContour->AddSegment(pArc2);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pArc3);
    pContour->AddSegment(pArc4);

    _arrContours.push_back(pContour);
    //viewManager->AddObject(Style(1, BLACK), pContour);
}

struct mpst001005
{
    double Db;         // 1 Диаметр внешний 
    double Dm;         // 2 Диаметр внутренний
    double Dpaz;       // 3 Диаметр для прокладки
    double Hb;         // 4 Высота плюшки
    double Hm;         // 5 Высота паза
    double Hpaz;       // 6 Высота фаска
    double Thickness;  // 7 Толщина стенки
};

std::vector<mpst001005> parametersFor005 = {
    //1     2    3   4   5   6   7 
    {400,  301, 321, 40, 10, 25, 14},
    {510,  397, 421, 60, 14, 25, 14},
};

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_005_kamera(double ktDiam, double ktThickness, double l3)
{
    int index = 0;
    double Dcircle = 370.0 * 2;

    if (l3 <= 400) { 
        index = 0;
    }
    else if (l3 > 400) {
        index = 1;
        Dcircle = 470.0 * 2;
    }

    double ANG1 = 180 * DEG_TO_RAD;

    const double dTruba = ktDiam;

    const double Db = parametersFor005[index].Db;
    const double Dm = parametersFor005[index].Dm;
    const double Dpaz = parametersFor005[index].Dpaz;
    const double Hb = parametersFor005[index].Hb;
    const double Hm = parametersFor005[index].Hm;
    const double Hpaz = parametersFor005[index].Hpaz;
    const double Thickness = parametersFor005[index].Thickness;

    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    CreateSketchForCapPart(arrContours, l3, Db, Dm, Dpaz, Hb, Hm, Hpaz, Thickness);

    //Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneXY, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Ось вращения для построения тела:
    MbAxis3D axis(pl.GetAxisZ());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    //ВЫДАВЛИВАНИЕ
    RPArray<MbContour> arrContours1;
    CreateSketchForWall(arrContours1, l3, Db, Dm, Dpaz, Hb, Hm, Hpaz, Thickness);

    MbSweptData sweptData1(*pPlaneXY, arrContours1);

    MbVector3D dirX(1, 0, 0);
    MbVector3D dirY(0, 1, 0);
    MbVector3D dirZ(0, 0, 1);

    MbSolid* pSolid1 = nullptr;

    MbResultType res1 = ::ExtrusionSolid(sweptData1, dirX, NULL, NULL, false, ExtrusionValues(0, 10), operNames, cNames, pSolid1);

    //ОБЪЕДИНЕНИЕ

    SolidSPtr Solid(pSolid);
    SolidSPtr Solid1(pSolid1);
    SolidSPtr Solid2 = nullptr;

    // Именователь граней для построения тела с помощью булевой операции 
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);
    MbBooleanOperationParams parameters(bo_Union, true, operBoolNames);

    MbResultType res2 = ::BooleanResult(Solid, cm_Copy, Solid1, cm_Copy, parameters, Solid2);

    //ОТВЕРСТИЯ

    //Параметры
    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);

    const double radius = 24.0 / 2;
    const int holes = 32;

    const double depth = -l3;


    SolidSPtr result = HolyHole112(&Solid2, holes, Dcircle, radius, depth);

    return result;
}