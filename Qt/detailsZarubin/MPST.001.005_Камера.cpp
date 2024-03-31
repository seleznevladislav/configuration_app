#include "../BuildMathModel.h"

using namespace BuildMathModel;

const int startX = 75;
const int startY = 30;
const double DEG_TO_RAD = M_PI / 180.0;

//SolidSPtr HolyHole1(SolidSPtr* previus, int holes, double DiametrCircle, double radius) {
//    // Именователь граней для построения тела с помощью булевой операции 
//    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);
//    MbBooleanOperationParams parameters(bo_Union, true, operBoolNames);
//
//    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
//    MbBooleanFlags flagsBool;
//    flagsBool.InitBoolean(true);
//    flagsBool.SetMergingFaces(true);
//    flagsBool.SetMergingEdges(true);
//
//    double ANG1 = 0 * DEG_TO_RAD;
//
//    SolidSPtr boltCyl;
//
//    SpacePointsVector boltCylPnts;
//
//    boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 0));
//    boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 100));
//    boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1) + radius, DiametrCircle / 2 * sin(ANG1), 0));
//
//
//    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);
//
//    SolidSPtr news;
//    ::BooleanResult(*previus, cm_Copy, boltCyl, cm_Copy,
//        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), news);
//
//    for (int i = 1; i <= holes; i++) {
//
//
//        ANG1 = 360 / holes * i * DEG_TO_RAD;
//
//        //double ANG2 = 22.5 + 45 * 1 * DEG_TO_RAD;
//
//        SolidSPtr boltCyl;
//
//        SpacePointsVector boltCylPnts;
//
//        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 0));
//        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 100));
//        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1) + radius, DiametrCircle / 2 * sin(ANG1), 0));
//
//        SolidSPtr buffer;
//
//        ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);
//
//        ::BooleanResult(news, cm_Copy, boltCyl, cm_Copy,
//            MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), buffer);
//
//        news = buffer;
//
//    }
//
//
//
//    return news;
//}

//void CreateSketcher1(RPArray<MbContour>& _arrContours)
//{
//    SArray<MbCartPoint> arrPnts(100);
//    arrPnts.Add(MbCartPoint(0, 400));
//    arrPnts.Add(MbCartPoint(0, 386));
//    arrPnts.Add(MbCartPoint(0, -80)); //центр дуги большой 0
//    arrPnts.Add(MbCartPoint(-273.123119, 297.570870));
//    arrPnts.Add(MbCartPoint(-233.648485, 243));//центр дуги маленькой 0
//    arrPnts.Add(MbCartPoint(-301, 243));
//    arrPnts.Add(MbCartPoint(-301, 10));
//    arrPnts.Add(MbCartPoint(-321, 10));
//    arrPnts.Add(MbCartPoint(-321, 0));
//    arrPnts.Add(MbCartPoint(-400, 0));
//    arrPnts.Add(MbCartPoint(-400, 40));
//    arrPnts.Add(MbCartPoint(-340, 40));
//    arrPnts.Add(MbCartPoint(-315, 65));
//    arrPnts.Add(MbCartPoint(-315, 243));//13
//    arrPnts.Add(MbCartPoint(-233.648485, 243));//центр дуги маленькой 14
//    arrPnts.Add(MbCartPoint(-281.328534, 308.914201));
//    arrPnts.Add(MbCartPoint(0, -80)); //центр дуги большой 14
//    arrPnts.Add(MbCartPoint(0, 400));
//
//
//    const double RADIUSB1 = 466;
//    const double RADIUSB2 = 480;
//
//    const double RADIUSM1 = 67.351515;
//    const double RADIUSM2 = 81.351515;
//
//    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
//
//    MbArc* pArc1 = new MbArc(arrPnts[2], RADIUSB1, arrPnts[1], arrPnts[3], 1);
//    MbArc* pArc2 = new MbArc(arrPnts[4], RADIUSM1, arrPnts[3], arrPnts[5], 1);
//
//
//    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[5], arrPnts[6]);
//    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[6], arrPnts[7]);
//    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[7], arrPnts[8]);
//    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[8], arrPnts[9]);
//    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[9], arrPnts[10]);
//    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[10], arrPnts[11]);
//    MbLineSegment* pLine8 = new MbLineSegment(arrPnts[11], arrPnts[12]);
//    MbLineSegment* pLine9 = new MbLineSegment(arrPnts[12], arrPnts[13]);
//
//    MbArc* pArc3 = new MbArc(arrPnts[14], RADIUSM2, arrPnts[13], arrPnts[15], -1);
//    MbArc* pArc4 = new MbArc(arrPnts[16], RADIUSB2, arrPnts[15], arrPnts[17], -1);
//
//
//    MbContour* pContour = new MbContour();
//
//    pContour->AddSegment(pLine1);
//    pContour->AddSegment(pArc1);
//    pContour->AddSegment(pArc2);
//    pContour->AddSegment(pLine2);
//    pContour->AddSegment(pLine3);
//    pContour->AddSegment(pLine4);
//    pContour->AddSegment(pLine5);
//    pContour->AddSegment(pLine6);
//    pContour->AddSegment(pLine7);
//    pContour->AddSegment(pLine8);
//    pContour->AddSegment(pLine9);
//    pContour->AddSegment(pArc3);
//    pContour->AddSegment(pArc4);
//
//    _arrContours.push_back(pContour);
//}

//void CreateSketcher2(RPArray<MbContour>& _arrContours)
//{
//    SArray<MbCartPoint> arrPnts(100);
//    arrPnts.Add(MbCartPoint(0, 386));
//    arrPnts.Add(MbCartPoint(0, -80)); //центр дуги большой 0
//    arrPnts.Add(MbCartPoint(-273.123119, 297.570870));
//    arrPnts.Add(MbCartPoint(-233.648485, 243));//центр дуги маленькой 0
//    arrPnts.Add(MbCartPoint(-301, 243));
//    arrPnts.Add(MbCartPoint(-301, 10));
//    arrPnts.Add(MbCartPoint(301, 10));
//    arrPnts.Add(MbCartPoint(301, 243));//7
//    arrPnts.Add(MbCartPoint(233.648485, 243));//центр дуги маленькой 0
//    arrPnts.Add(MbCartPoint(273.123119, 297.570870));
//    arrPnts.Add(MbCartPoint(0, -80)); //центр дуги большой 0
//
//
//    const double RADIUSB1 = 466;
//    const double RADIUSM1 = 67.351515;
//
//
//    MbArc* pArc1 = new MbArc(arrPnts[1], RADIUSB1, arrPnts[0], arrPnts[2], 1);
//    MbArc* pArc2 = new MbArc(arrPnts[3], RADIUSM1, arrPnts[2], arrPnts[4], 1);
//
//
//    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[4], arrPnts[5]);
//    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[5], arrPnts[6]);
//    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[6], arrPnts[7]);
//
//    MbArc* pArc3 = new MbArc(arrPnts[8], RADIUSM1, arrPnts[7], arrPnts[9], 1);
//    MbArc* pArc4 = new MbArc(arrPnts[10], RADIUSB1, arrPnts[9], arrPnts[0], 1);
//
//
//    MbContour* pContour = new MbContour();
//
//    pContour->AddSegment(pArc1);
//    pContour->AddSegment(pArc2);
//    pContour->AddSegment(pLine1);
//    pContour->AddSegment(pLine2);
//    pContour->AddSegment(pLine3);
//    pContour->AddSegment(pArc3);
//    pContour->AddSegment(pArc4);
//
//    _arrContours.push_back(pContour);
//}

//SPtr<MbSolid> ParametricModelCreator::Zarubincreate_005_kamera()
//{
//    MbPlacement3D pl;
//
//    // Создание образующей для тела выдавливания
//    RPArray<MbContour> arrContours;
//    CreateSketcher1(arrContours);
//
//    //Плоскость
//    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
//    MbSweptData sweptData(*pPlaneXY, arrContours);
//
//    //Угол Вращения
//    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);
//
//    // Именователи для операции построения тела вращения и для контуров образующей
//    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
//    PArray<MbSNameMaker> cNames(0, 1, false);
//
//    // Ось вращения для построения тела:
//    MbAxis3D axis(pl.GetAxisZ());
//
//    // Вызов функции-утилиты для построения твердого тела вращения
//    MbSolid* pSolid = nullptr;
//    MbResultType res = ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);
//
//    //ВЫДАВЛИВАНИЕ
//    RPArray<MbContour> arrContours1;
//    CreateSketcher2(arrContours1);
//
//    MbSweptData sweptData1(*pPlaneXY, arrContours1);
//
//    MbVector3D dirX(1, 0, 0);
//    MbVector3D dirY(0, 1, 0);
//    MbVector3D dirZ(0, 0, 1);
//
//    MbSolid* pSolid1 = nullptr;
//
//    MbResultType res1 = ::ExtrusionSolid(sweptData1, dirX, NULL, NULL, false, ExtrusionValues(0, 10), operNames, cNames, pSolid1);
//
//    //ОБЪЕДИНЕНИЕ
//
//    SolidSPtr Solid(pSolid);
//    SolidSPtr Solid1(pSolid1);
//    SolidSPtr Solid2 = nullptr;
//
//    // Именователь граней для построения тела с помощью булевой операции 
//    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);
//    MbBooleanOperationParams parameters(bo_Union, true, operBoolNames);
//
//    MbResultType res2 = ::BooleanResult(Solid, cm_Copy, Solid1, cm_Copy, parameters, Solid2);
//
//    //ОТВЕРСТИЯ
//
//    //Параметры
//    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
//
//    const double radius = 24.0 / 2;
//    const double Dcircle = 370.0 * 2;
//    const int holes = 32;
//
//
//    SolidSPtr result = HolyHole1(&Solid2, holes, Dcircle, radius);
//
//    return result;
//}