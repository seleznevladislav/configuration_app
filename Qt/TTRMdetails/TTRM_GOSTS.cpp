#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void createPolygonSketch(RPArray<MbContour>& _arrContours, int assortmentInnerTubes, double rate)
{
    const float radius = assortmentInnerTubes / 2;
    const float x = radius + rate;
    const float y = (sqrt(3) * x) / 2;

    SArray<MbCartPoint> arrPnts(6);

    arrPnts.Add(MbCartPoint(x / 2, y));
    arrPnts.Add(MbCartPoint(x, 0));
    arrPnts.Add(MbCartPoint(x / 2, -y));
    arrPnts.Add(MbCartPoint(-(x / 2), -y));
    arrPnts.Add(MbCartPoint(-x, 0));
    arrPnts.Add(MbCartPoint(-(x / 2), y));

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[5], arrPnts[0]);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);

    _arrContours.push_back(pContour);
}

void createFlnageScrewSketch(RPArray<MbContour>& _arrContours)
{

    SArray<MbCartPoint> arrPnts(6);

    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint(0, 35));
    arrPnts.Add(MbCartPoint(4, 35));
    arrPnts.Add(MbCartPoint(5, 34));
    arrPnts.Add(MbCartPoint(5, 1.5));
    arrPnts.Add(MbCartPoint(3.5, 0));


    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[5], arrPnts[0]);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);

    _arrContours.push_back(pContour);
}

void createFlnageESketch(RPArray<MbContour>& _arrContours, double diametrY)
{
    const float radius = diametrY / 2;

    SArray<MbCartPoint> arrPnts(8);

    arrPnts.Add(MbCartPoint(radius + 0.5, 0));
    arrPnts.Add(MbCartPoint(radius + 0.5, 12));
    arrPnts.Add(MbCartPoint(radius + 0.5 + 2, 14));
    arrPnts.Add(MbCartPoint(radius + 32.5, 14));
    arrPnts.Add(MbCartPoint(radius + 32.5, 4));
    arrPnts.Add(MbCartPoint(radius + 9.0, 4));
    arrPnts.Add(MbCartPoint(radius + 9.0, 0));

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[6], arrPnts[0]);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);

    _arrContours.push_back(pContour);
}

void createFlnageFSketch(RPArray<MbContour>& _arrContours, double diametrY)
{
    const float radius = diametrY / 2;

    SArray<MbCartPoint> arrPnts(10);

    arrPnts.Add(MbCartPoint(radius + 9.5, 0));
    arrPnts.Add(MbCartPoint(radius + 9.5, 3));
    arrPnts.Add(MbCartPoint(radius + 0.5, 3));
    arrPnts.Add(MbCartPoint(radius + 0.5, 3 + 9));
    arrPnts.Add(MbCartPoint(radius + 0.5, 3 + 9));
    arrPnts.Add(MbCartPoint(radius + 0.5 + 2, 3 + 9 + 2));
    arrPnts.Add(MbCartPoint(radius + 32.5, 3 + 9 + 2));
    arrPnts.Add(MbCartPoint(radius + 32.5, 2));
    arrPnts.Add(MbCartPoint(radius + 14.5, 2));
    arrPnts.Add(MbCartPoint(radius + 12.5, 0));


    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[6], arrPnts[7]);
    MbLineSegment* pLine8 = new MbLineSegment(arrPnts[7], arrPnts[8]);
    MbLineSegment* pLine9 = new MbLineSegment(arrPnts[8], arrPnts[9]);
    MbLineSegment* pLine10 = new MbLineSegment(arrPnts[9], arrPnts[0]);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);
    pContour->AddSegment(pLine8);
    pContour->AddSegment(pLine9);
    pContour->AddSegment(pLine10);

    _arrContours.push_back(pContour);
}


SPtr<MbSolid> ParametricModelCreator::buildFlangeE(double diametrY)
{
    const double radius = diametrY / 2;

    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    createFlnageESketch(arrContours, diametrY);

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneYZ, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Ось вращения для построения тела:
    MbAxis3D axis(pl.GetAxisZ());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid;

    ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr boltCyl, boltCyl2, boltCyl3, boltCyl4;
    SolidSPtr mergeBoltCyl, mergeBoltCyl2, mergeBoltCyl3, result;

    SpacePointsVector boltCylPnts;

    boltCylPnts.push_back(MbCartPoint3D(radius + 20, 0, 0));
    boltCylPnts.push_back(MbCartPoint3D(radius + 20, 0, 20));
    boltCylPnts.push_back(MbCartPoint3D(radius + 20 + 5.5, 0, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);

    SpacePointsVector boltCylPnts2;

    boltCylPnts2.push_back(MbCartPoint3D(-(radius + 20), 0, 0));
    boltCylPnts2.push_back(MbCartPoint3D(-(radius + 20), 0, 20));
    boltCylPnts2.push_back(MbCartPoint3D(-(radius + 20 + 5.5), 0, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts2, blockNames), boltCyl2);

    SpacePointsVector boltCylPnts3;

    boltCylPnts3.push_back(MbCartPoint3D(0, -(radius + 20), 0));
    boltCylPnts3.push_back(MbCartPoint3D(0, -(radius + 20), 20));
    boltCylPnts3.push_back(MbCartPoint3D(0, -(radius + 20 + 5.5), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts3, blockNames), boltCyl3);

    SpacePointsVector boltCylPnts4;

    boltCylPnts4.push_back(MbCartPoint3D(0, radius + 20, 0));
    boltCylPnts4.push_back(MbCartPoint3D(0, radius + 20, 20));
    boltCylPnts4.push_back(MbCartPoint3D(0, radius + 20 + 5.5, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts4, blockNames), boltCyl4);

    SolidSPtr Solid(pSolid);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    ::BooleanResult(Solid, cm_Copy, boltCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl);

    ::BooleanResult(mergeBoltCyl, cm_Copy, boltCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl2);

    ::BooleanResult(mergeBoltCyl2, cm_Copy, boltCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl3);

    ::BooleanResult(mergeBoltCyl3, cm_Copy, boltCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), result);

    return result;
}

SPtr<MbSolid> ParametricModelCreator::buildFlangeF(double diametrY)
{
    const double radius = diametrY / 2;

    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    createFlnageFSketch(arrContours, diametrY);

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneYZ, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Ось вращения для построения тела:
    MbAxis3D axis(pl.GetAxisZ());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid;

    ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr boltCyl, boltCyl2, boltCyl3, boltCyl4;
    SolidSPtr mergeBoltCyl, mergeBoltCyl2, mergeBoltCyl3, result;

    SpacePointsVector boltCylPnts;

    boltCylPnts.push_back(MbCartPoint3D(radius + 20, 0, 0));
    boltCylPnts.push_back(MbCartPoint3D(radius + 20, 0, 20));
    boltCylPnts.push_back(MbCartPoint3D(radius + 20 + 5.5, 0, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);

    SpacePointsVector boltCylPnts2;

    boltCylPnts2.push_back(MbCartPoint3D(-(radius + 20), 0, 0));
    boltCylPnts2.push_back(MbCartPoint3D(-(radius + 20), 0, 20));
    boltCylPnts2.push_back(MbCartPoint3D(-(radius + 20 + 5.5), 0, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts2, blockNames), boltCyl2);

    SpacePointsVector boltCylPnts3;

    boltCylPnts3.push_back(MbCartPoint3D(0, -(radius + 20), 0));
    boltCylPnts3.push_back(MbCartPoint3D(0, -(radius + 20), 20));
    boltCylPnts3.push_back(MbCartPoint3D(0, -(radius + 20 + 5.5), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts3, blockNames), boltCyl3);

    SpacePointsVector boltCylPnts4;

    boltCylPnts4.push_back(MbCartPoint3D(0, radius + 20, 0));
    boltCylPnts4.push_back(MbCartPoint3D(0, radius + 20, 20));
    boltCylPnts4.push_back(MbCartPoint3D(0, radius + 20 + 5.5, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts4, blockNames), boltCyl4);

    SolidSPtr Solid(pSolid);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    ::BooleanResult(Solid, cm_Copy, boltCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl);

    ::BooleanResult(mergeBoltCyl, cm_Copy, boltCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl2);

    ::BooleanResult(mergeBoltCyl2, cm_Copy, boltCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl3);

    ::BooleanResult(mergeBoltCyl3, cm_Copy, boltCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), result);

    return result;
}

SPtr<MbSolid> ParametricModelCreator::buildFlangeScrew35()
{
    //       // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    createFlnageScrewSketch(arrContours);

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneYZ, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Ось вращения для построения тела:
    MbAxis3D axis(pl.GetAxisZ());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid;

    ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr screwBlock;
    SolidSPtr result;

    SpacePointsVector screwBlockPnts;

    screwBlockPnts.push_back(MbCartPoint3D(0.8, 5, 35 - 3));
    screwBlockPnts.push_back(MbCartPoint3D(0.8, -5, 35 - 3));
    screwBlockPnts.push_back(MbCartPoint3D(0.8, 0, 35));
    screwBlockPnts.push_back(MbCartPoint3D(-0.8, 0, 35 - 1));

    ::ElementarySolid(MbElementarySolidParams(et_Block, screwBlockPnts, blockNames), screwBlock);


    SolidSPtr Solid(pSolid);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    ::BooleanResult(Solid, cm_Copy, screwBlock, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), result);

    return result;
}

void createWasherSketch(RPArray<MbContour>& _arrContours, double radius)
{
    SArray<MbCartPoint> arrPnts(8);

    arrPnts.Add(MbCartPoint(radius + 0.5, 0));
    arrPnts.Add(MbCartPoint(radius, 0.5));
    arrPnts.Add(MbCartPoint(radius, radius - 0.5));
    arrPnts.Add(MbCartPoint(radius + 0.5, radius));
    arrPnts.Add(MbCartPoint(radius + 2.6, radius));
    arrPnts.Add(MbCartPoint(radius + 6, radius - 1));
    arrPnts.Add(MbCartPoint(radius + 6, 1));
    arrPnts.Add(MbCartPoint(radius + 2.6, 0));

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[6], arrPnts[7]);
    MbLineSegment* pLine8 = new MbLineSegment(arrPnts[7], arrPnts[0]);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);
    pContour->AddSegment(pLine8);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::buildWasher10()
{
    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    createWasherSketch(arrContours, 5.0);

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneYZ, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Ось вращения для построения тела:
    MbAxis3D axis(pl.GetAxisZ());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid;

    ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours2;
    createPolygonSketch(arrContours2, 10, 4.2);

    //Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbSweptData sweptData2(*pPlaneXY, arrContours2);

    MbVector3D dir(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 1));

    const double HEIGHT_FORWARD = 5, HEIGHT_BACKWARD = 0;

    ExtrusionValues extrusionParams(HEIGHT_FORWARD, HEIGHT_BACKWARD);

    MbSolid* pPart1;

    ::ExtrusionSolid(sweptData2, dir, NULL, NULL, false, extrusionParams, operNames, cNames, pPart1);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr pMainSolid(pSolid), pPolygonSolid(pPart1), mergeSolid;

    ::BooleanResult(pMainSolid, cm_Copy, pPolygonSolid, cm_Copy,
        MbBooleanOperationParams(bo_Intersect, flagsBool, operBoolNames), mergeSolid);

    return mergeSolid;
}

void createScrew55(RPArray<MbContour>& _arrContours)
{
    SArray<MbCartPoint> arrPnts(10);

    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint(0, 61.5));
    arrPnts.Add(MbCartPoint(8, 61.5));
    arrPnts.Add(MbCartPoint(9.2, 60.5));
    arrPnts.Add(MbCartPoint(9.2, 55.5));
    arrPnts.Add(MbCartPoint(8, 55.5));
    arrPnts.Add(MbCartPoint(8, 55));
    arrPnts.Add(MbCartPoint(5, 55));
    arrPnts.Add(MbCartPoint(5, 1));
    arrPnts.Add(MbCartPoint(4, 0));

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[6], arrPnts[7]);
    MbLineSegment* pLine8 = new MbLineSegment(arrPnts[7], arrPnts[8]);
    MbLineSegment* pLine9 = new MbLineSegment(arrPnts[8], arrPnts[9]);
    MbLineSegment* pLine10 = new MbLineSegment(arrPnts[9], arrPnts[0]);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);
    pContour->AddSegment(pLine8);
    pContour->AddSegment(pLine9);
    pContour->AddSegment(pLine10);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::buildScrew55()
{
    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    createScrew55(arrContours);

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneYZ, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Ось вращения для построения тела:
    MbAxis3D axis(pl.GetAxisZ());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid;

    ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours2;
    createPolygonSketch(arrContours2, 10, 4.3);

    //Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbSweptData sweptData2(*pPlaneXY, arrContours2);

    MbVector3D dir(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 1));

    const double HEIGHT_FORWARD = 65, HEIGHT_BACKWARD = 0;

    ExtrusionValues extrusionParams(HEIGHT_FORWARD, HEIGHT_BACKWARD);

    MbSolid* pPart1;

    ::ExtrusionSolid(sweptData2, dir, NULL, NULL, false, extrusionParams, operNames, cNames, pPart1);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr pMainSolid(pSolid), pPolygonSolid(pPart1), mergeSolid;

    ::BooleanResult(pMainSolid, cm_Copy, pPolygonSolid, cm_Copy,
        MbBooleanOperationParams(bo_Intersect, flagsBool, operBoolNames), mergeSolid);

    return mergeSolid;
}