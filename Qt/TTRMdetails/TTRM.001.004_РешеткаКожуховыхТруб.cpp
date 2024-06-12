#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void createMainSolidSketch(RPArray<MbContour>& _arrContours, double length2, double assortmentCamera)
{
    SArray<MbCartPoint> arrPnts(7);

    arrPnts.Add(MbCartPoint(0, length2));
    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint((assortmentCamera + 96) / 2, 0));
    arrPnts.Add(MbCartPoint((assortmentCamera + 96) / 2, 22));
    arrPnts.Add(MbCartPoint((assortmentCamera + 96) / 2 - 40, 22));
    arrPnts.Add(MbCartPoint(assortmentCamera / 2, 60));
    arrPnts.Add(MbCartPoint(assortmentCamera / 2, length2 - 22));
    arrPnts.Add(MbCartPoint(assortmentCamera / 2 - 23.5, length2));

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

void createYZWall(RPArray<MbContour>& _arrContours, double length2, double assortmentCamera, double thicknessCamera)
{
    SArray<MbCartPoint> wallPnts(6);

    wallPnts.Add(MbCartPoint(assortmentCamera / 2 - thicknessCamera, 0));
    wallPnts.Add(MbCartPoint(assortmentCamera / 2 - thicknessCamera, length2 - 22 * 2));
    wallPnts.Add(MbCartPoint(assortmentCamera / 2 - thicknessCamera - 23, length2 - 22));
    wallPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera - 23), length2 - 22));
    wallPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera), length2 - 22 * 2));
    wallPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera), 0));


    MbLineSegment* pLine = new MbLineSegment(wallPnts[0], wallPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(wallPnts[1], wallPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(wallPnts[2], wallPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(wallPnts[3], wallPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(wallPnts[4], wallPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(wallPnts[5], wallPnts[0]);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);

    _arrContours.push_back(pContour);
}

void createXZWall(RPArray<MbContour>& _arrContours, double length2, double assortmentCamera, double thicknessCamera)
{
    SArray<MbCartPoint> wallPnts(6);

    wallPnts.Add(MbCartPoint(0, length2 - 22));
    wallPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera - 27), length2 - 22));
    wallPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera), length2 - 22 * 2));
    wallPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera), 0));
    wallPnts.Add(MbCartPoint(0, 0));

    MbLineSegment* pLine = new MbLineSegment(wallPnts[0], wallPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(wallPnts[1], wallPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(wallPnts[2], wallPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(wallPnts[3], wallPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(wallPnts[4], wallPnts[0]);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::createOuterPipesGrid_004(double length2, double diametrY, double thickness, double t, double assortmentInnerTubes, double assortmentCamera, double thicknessCamera)
{
    const double DEG_TO_RAD = M_PI / 180.0;

    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrMainContours;
    createMainSolidSketch(arrMainContours, length2, assortmentCamera);

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));

    MbSweptData sweptData(*pPlaneYZ, arrMainContours);

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

    MbSNameMaker namesElSolid(ct_ElementarySolid, MbSNameMaker::i_SideNone, 0);
    SolidSPtr pCylSolid;
    SpacePointsVector cylPnts;

    cylPnts.push_back(MbCartPoint3D(-50, -183, 127));
    cylPnts.push_back(MbCartPoint3D(-50, 183, 127));
    cylPnts.push_back(MbCartPoint3D(-50, -183, 127 + diametrY / 2));

    // Построение трубы
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, cylPnts, namesElSolid), pCylSolid);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr pMergeSolid, pMainSolid(pSolid);

    // Первое объеденение
    ::BooleanResult(pMainSolid, cm_Copy, pCylSolid, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), pMergeSolid);

    ////ВЫДАВЛИВАНИЕ
    RPArray<MbContour> arrContours1;
    createYZWall(arrContours1, length2, assortmentCamera, thicknessCamera);

    MbSweptData sweptDataInnerCyl(*pPlaneYZ, arrContours1);
    MbSolid* pInner;

    ::RevolutionSolid(sweptDataInnerCyl, axis, revParms, operNames, cNames, pInner);

    SolidSPtr pInnerSolid(pInner), pDifferentSolid;
    ::BooleanResult(pMergeSolid, cm_Copy, pInnerSolid, cm_Copy, 
        MbBooleanOperationParams(bo_Difference, false, operBoolNames), pDifferentSolid);

    MbVector3D dirX(1, 0, 0);
    MbVector3D dirY(0, 1, 0);
    MbVector3D dirZ(0, 0, 1);

    MbSolid* pWallYZ;
    MbSolid* pWallXZ;

    // Главная стенка YZ
    ::ExtrusionSolid(sweptDataInnerCyl, dirX, NULL, NULL, false, ExtrusionValues(0, 10), operNames, cNames, pWallYZ);

    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));
    RPArray<MbContour> arrContours2;
    createXZWall(arrContours2, length2, assortmentCamera, thicknessCamera);
    MbSweptData sweptDataSmallWall(*pPlaneXZ, arrContours2);

    // Главная стенка XZ
    ::ExtrusionSolid(sweptDataSmallWall, dirY, NULL, NULL, false, ExtrusionValues(0, 10), operNames, cNames, pWallXZ);

    SolidSPtr SolidWallYZ(pWallYZ), SolidWallXZ(pWallXZ), mergeSolid = nullptr, mergeSolidWithWalls = nullptr;

    MbBooleanOperationParams parameters(bo_Union, true, operBoolNames);

    ::BooleanResult(pDifferentSolid, cm_Copy, SolidWallYZ, cm_Copy, parameters, mergeSolid);
    ::BooleanResult(mergeSolid, cm_Copy, SolidWallXZ, cm_Copy, parameters, mergeSolidWithWalls);

    SolidSPtr pCylSmallTube1Solid, pCylSmallTube2Solid, pCylSmallTube3Solid, pCylSmallTube4Solid;
    SpacePointsVector smallCyl1Pnts, smallCyl2Pnts, smallCyl3Pnts, smallCyl4Pnts;

    const double halfOfT = t / 2;
    const double radiusOfInnerTube = assortmentInnerTubes / 2;

    smallCyl1Pnts.push_back(MbCartPoint3D(halfOfT, halfOfT, length2 - 22));
    smallCyl1Pnts.push_back(MbCartPoint3D(halfOfT, halfOfT, length2));
    smallCyl1Pnts.push_back(MbCartPoint3D(halfOfT, halfOfT + radiusOfInnerTube, length2 - 22));

    smallCyl2Pnts.push_back(MbCartPoint3D(-halfOfT, halfOfT, length2 - 22));
    smallCyl2Pnts.push_back(MbCartPoint3D(-halfOfT, halfOfT, length2));
    smallCyl2Pnts.push_back(MbCartPoint3D(-halfOfT, halfOfT + radiusOfInnerTube, length2 - 22));

    smallCyl3Pnts.push_back(MbCartPoint3D(halfOfT, -halfOfT, length2 - 22));
    smallCyl3Pnts.push_back(MbCartPoint3D(halfOfT, -halfOfT, length2));
    smallCyl3Pnts.push_back(MbCartPoint3D(halfOfT, -(halfOfT + radiusOfInnerTube), length2 - 22));

    smallCyl4Pnts.push_back(MbCartPoint3D(-halfOfT, -halfOfT, length2 - 22));
    smallCyl4Pnts.push_back(MbCartPoint3D(-halfOfT, -halfOfT, length2));
    smallCyl4Pnts.push_back(MbCartPoint3D(-halfOfT, -(halfOfT + radiusOfInnerTube), length2 - 22));

    // Построение трубы
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCyl1Pnts, namesElSolid), pCylSmallTube1Solid);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCyl2Pnts, namesElSolid), pCylSmallTube2Solid);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCyl3Pnts, namesElSolid), pCylSmallTube3Solid);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCyl4Pnts, namesElSolid), pCylSmallTube4Solid);

    SolidSPtr pCylMergeSolid, pCylMergeSolid2, pCylMergeSolid3, pCylMergeSolid4;

    ::BooleanResult(mergeSolidWithWalls, cm_Copy, pCylSmallTube1Solid, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), pCylMergeSolid);
    ::BooleanResult(pCylMergeSolid, cm_Copy, pCylSmallTube2Solid, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), pCylMergeSolid2);
    ::BooleanResult(pCylMergeSolid2, cm_Copy, pCylSmallTube3Solid, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), pCylMergeSolid3);
    ::BooleanResult(pCylMergeSolid3, cm_Copy, pCylSmallTube4Solid, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), pCylMergeSolid4);

    SolidSPtr extractHole, extractHole2;
    SolidSPtr extractHoleMergeSolid, extractHoleMergeSolid2;

    SpacePointsVector extractCylPnts;
    SpacePointsVector extractCylPnts2;

    extractCylPnts.push_back(MbCartPoint3D(-50, 50, 127));
    extractCylPnts.push_back(MbCartPoint3D(-50, 183, 127));
    extractCylPnts.push_back(MbCartPoint3D(-50, 50, 127 + (diametrY / 2 - thickness)));

    extractCylPnts2.push_back(MbCartPoint3D(-50, -50, 127));
    extractCylPnts2.push_back(MbCartPoint3D(-50, -183, 127));
    extractCylPnts2.push_back(MbCartPoint3D(-50, -50, 127 + (diametrY / 2 - thickness)));

    // Построение трубы
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, extractCylPnts, namesElSolid), extractHole);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, extractCylPnts2, namesElSolid), extractHole2);

    ::BooleanResult(pCylMergeSolid4, cm_Copy, extractHole, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), extractHoleMergeSolid);
    ::BooleanResult(extractHoleMergeSolid, cm_Copy, extractHole2, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), extractHoleMergeSolid2);

    //
    SolidSPtr resultSolid, boltCyl, boltCyl2, boltCyl3, boltCyl4, boltCyl5, boltCyl6, boltCyl7, boltCyl8;
    SolidSPtr mergeBoltCyl, mergeBoltCyl2, mergeBoltCyl3, mergeBoltCyl4, mergeBoltCyl5, mergeBoltCyl6, mergeBoltCyl7;

    const int angle = 22.5;

    double boltAngle = angle * DEG_TO_RAD;
    double boltAngle2 = (angle + 45 * 1) * DEG_TO_RAD;
    double boltAngle3 = (angle + 45 * 2) * DEG_TO_RAD;
    double boltAngle4 = (angle + 45 * 3) * DEG_TO_RAD;
    double boltAngle5 = (angle + 45 * 4) * DEG_TO_RAD;
    double boltAngle6 = (angle + 45 * 5) * DEG_TO_RAD;
    double boltAngle7 = (angle + 45 * 6) * DEG_TO_RAD;
    double boltAngle8 = (angle + 45 * 7) * DEG_TO_RAD;

    SpacePointsVector boltCylPnts;

    boltCylPnts.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle), (assortmentCamera / 2 + 30.5) * sin(boltAngle), 0));
    boltCylPnts.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle), (assortmentCamera / 2 + 30.5) * sin(boltAngle), 22.5));
    boltCylPnts.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, operNames), boltCyl);

    SpacePointsVector boltCylPnts2;

    boltCylPnts2.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle2), (assortmentCamera / 2 + 30.5) * sin(boltAngle2), 0));
    boltCylPnts2.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle2), (assortmentCamera / 2 + 30.5) * sin(boltAngle2), 22.5));
    boltCylPnts2.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle2) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle2), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts2, operNames), boltCyl2);

    SpacePointsVector boltCylPnts3;

    boltCylPnts3.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle3), (assortmentCamera / 2 + 30.5) * sin(boltAngle3), 0));
    boltCylPnts3.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle3), (assortmentCamera / 2 + 30.5) * sin(boltAngle3), 22.5));
    boltCylPnts3.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle3) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle3), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts3, operNames), boltCyl3);

    SpacePointsVector boltCylPnts4;

    boltCylPnts4.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle4), (assortmentCamera / 2 + 30.5) * sin(boltAngle4), 0));
    boltCylPnts4.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle4), (assortmentCamera / 2 + 30.5) * sin(boltAngle4), 22.5));
    boltCylPnts4.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle4) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle4), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts4, operNames), boltCyl4);

    SpacePointsVector boltCylPnts5;

    boltCylPnts5.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle5), (assortmentCamera / 2 + 30.5) * sin(boltAngle5), 0));
    boltCylPnts5.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle5), (assortmentCamera / 2 + 30.5) * sin(boltAngle5), 22.5));
    boltCylPnts5.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle5) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle5), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts5, operNames), boltCyl5);

    SpacePointsVector boltCylPnts6;

    boltCylPnts6.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle6), (assortmentCamera / 2 + 30.5) * sin(boltAngle6), 0));
    boltCylPnts6.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle6), (assortmentCamera / 2 + 30.5) * sin(boltAngle6), 22.5));
    boltCylPnts6.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle6) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle6), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts6, operNames), boltCyl6);

    SpacePointsVector boltCylPnts7;

    boltCylPnts7.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle7), (assortmentCamera / 2 + 30.5) * sin(boltAngle7), 0));
    boltCylPnts7.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle7), (assortmentCamera / 2 + 30.5) * sin(boltAngle7), 22.5));
    boltCylPnts7.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle7) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle7), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts7, operNames), boltCyl7);

    SpacePointsVector boltCylPnts8;

    boltCylPnts8.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle8), (assortmentCamera / 2 + 30.5) * sin(boltAngle8), 0));
    boltCylPnts8.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle8), (assortmentCamera / 2 + 30.5) * sin(boltAngle8), 22.5));
    boltCylPnts8.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle8) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle8), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts8, operNames), boltCyl8);

    ::BooleanResult(extractHoleMergeSolid2, cm_Copy, boltCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl);

    ::BooleanResult(mergeBoltCyl, cm_Copy, boltCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl2);

    ::BooleanResult(mergeBoltCyl2, cm_Copy, boltCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl3);

    ::BooleanResult(mergeBoltCyl3, cm_Copy, boltCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl4);

    ::BooleanResult(mergeBoltCyl4, cm_Copy, boltCyl5, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl5);

    ::BooleanResult(mergeBoltCyl5, cm_Copy, boltCyl6, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl6);

    ::BooleanResult(mergeBoltCyl6, cm_Copy, boltCyl7, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergeBoltCyl7);

    ::BooleanResult(mergeBoltCyl7, cm_Copy, boltCyl8, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), resultSolid);

    return resultSolid;
}