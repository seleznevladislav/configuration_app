#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void createSketchSecond(RPArray<MbContour>& _arrContours, double length3, double assortmentCamera, double thicknessCamera)
{
    std::unordered_map<int, std::vector<double>> valuesDictionary;
    // 3 x // 3 y // Hh // Rb // 
    std::vector<double> array219 = { 84.35, 27.52, 55, 200, 70.910221, 87.86, 20.33, 192, 200, 30.589779, 38.589779 };
    std::vector<double> array273 = { 105.18, 31.37, 68, 256.428867, 88.062638, 108.57, 24.12, 248.428867, 256.428867, 40.437362, 48.437362 };
    std::vector<double> array325 = { 138.15, 50.25, 81, 255, 117.404054, 142.63, 43.62, 247, 255, 37.095946, 45.095946 };

    valuesDictionary[219] = array219;
    valuesDictionary[273] = array273;
    valuesDictionary[325] = array325;

    const std::vector<double> currentValues = valuesDictionary[assortmentCamera];

    SArray<MbCartPoint> arrPnts(12);

    arrPnts.Add(MbCartPoint(0, length3)); //0
    arrPnts.Add(MbCartPoint(0, length3 - thicknessCamera)); //1
    arrPnts.Add(MbCartPoint(0, length3 - currentValues[3])); //центр дуги большой 0
    arrPnts.Add(MbCartPoint(currentValues[0], length3 - currentValues[1])); //3
    arrPnts.Add(MbCartPoint(assortmentCamera / 2 - thicknessCamera, length3 - currentValues[2])); //4
    arrPnts.Add(MbCartPoint(assortmentCamera / 2 - thicknessCamera, 0)); //5
    arrPnts.Add(MbCartPoint(assortmentCamera / 2 + 48, 0)); //6
    arrPnts.Add(MbCartPoint(assortmentCamera / 2 + 48, 22)); //7
    arrPnts.Add(MbCartPoint(assortmentCamera / 2, 22)); //8
    arrPnts.Add(MbCartPoint(assortmentCamera / 2, length3 - currentValues[2]));//9
    arrPnts.Add(MbCartPoint(currentValues[4], length3 - currentValues[2]));//центр дуги маленькой 10
    arrPnts.Add(MbCartPoint(currentValues[5], length3 - currentValues[6])); //11

    const double RADIUSB1 = currentValues[7];
    const double RADIUSB2 = currentValues[8];

    const double RADIUSM1 = currentValues[9];
    const double RADIUSM2 = currentValues[10];

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[0]);
    MbArc* pArc1 = new MbArc(arrPnts[2], RADIUSB2, arrPnts[0], arrPnts[11], -1);
    MbArc* pArc2 = new MbArc(arrPnts[10], RADIUSM2, arrPnts[11], arrPnts[10], -1);

    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[9], arrPnts[8]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[8], arrPnts[7]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[7], arrPnts[6]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[6], arrPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[5], arrPnts[4]);

    MbArc* pArc3 = new MbArc(arrPnts[10], RADIUSM1, arrPnts[4], arrPnts[3], 1);
    MbArc* pArc4 = new MbArc(arrPnts[2], RADIUSB1, arrPnts[3], arrPnts[1], 1);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pArc1);
    pContour->AddSegment(pArc2);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pArc3);
    pContour->AddSegment(pArc4);

    _arrContours.push_back(pContour);
}

void createWallXZSecond(RPArray<MbContour>& _arrContours, double length3, double assortmentCamera, double thicknessCamera)
{
    std::unordered_map<int, std::vector<double>> valuesDictionary;
    // 3 x // 3 y // Hh // Rb // 
    std::vector<double> array219 = { 84.35, 27.52, 55, 70.910221, 30.589779, 200, 192 };
    std::vector<double> array273 = { 105.18, 31.37, 68, 88.062638, 40.437362, 256.428867, 248.428867 };
    std::vector<double> array325 = { 138.15, 50.25, 81, 117.404054, 37.095946, 255, 247 };

    valuesDictionary[219] = array219;
    valuesDictionary[273] = array273;
    valuesDictionary[325] = array325;

    const std::vector<double> currentValues = valuesDictionary[assortmentCamera];

    SArray<MbCartPoint> arrPnts(12);

    arrPnts.Add(MbCartPoint(0, length3 - thicknessCamera)); // 0
    arrPnts.Add(MbCartPoint(currentValues[0], length3 - currentValues[1])); // 1
    arrPnts.Add(MbCartPoint(assortmentCamera / 2 - thicknessCamera, length3 - currentValues[2])); // 2
    arrPnts.Add(MbCartPoint(assortmentCamera / 2 - thicknessCamera, 0)); // 3
    arrPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera), 0)); // 4
    arrPnts.Add(MbCartPoint(-(assortmentCamera / 2 - thicknessCamera), length3 - currentValues[2])); // 5
    arrPnts.Add(MbCartPoint(-currentValues[0], length3 - currentValues[1])); // 6
    arrPnts.Add(MbCartPoint(currentValues[3], length3 - currentValues[2]));//центр дуги маленькой Справа 7
    arrPnts.Add(MbCartPoint(-currentValues[3], length3 - currentValues[2]));//центр дуги маленькой Слева 8
    arrPnts.Add(MbCartPoint(0, length3 - currentValues[5]));//центр дуги Большой 9

    const double RADIUSM = currentValues[4];

    const double RADIUSB = currentValues[6];


    MbArc* pArc1 = new MbArc(arrPnts[9], RADIUSB, arrPnts[0], arrPnts[1], -1);
    MbArc* pArc2 = new MbArc(arrPnts[7], RADIUSM, arrPnts[1], arrPnts[2], -1);

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[4], arrPnts[5]);

    MbArc* pArc3 = new MbArc(arrPnts[8], RADIUSM, arrPnts[5], arrPnts[6], -1);
    MbArc* pArc4 = new MbArc(arrPnts[9], RADIUSB, arrPnts[6], arrPnts[0], -1);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pArc1);
    pContour->AddSegment(pArc2);

    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);

    pContour->AddSegment(pArc3);
    pContour->AddSegment(pArc4);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::createInnerPipesGrid_006(double length3, double assortmentCamera, double thicknessCamera)
{
    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    RPArray<MbContour> arrMainContours;
    createSketchSecond(arrMainContours, length3, assortmentCamera, thicknessCamera);

    //Плоскость
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));

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

    cylPnts.push_back(MbCartPoint3D(0, -(assortmentCamera / 2 + 60.5), 88));
    cylPnts.push_back(MbCartPoint3D(0, assortmentCamera / 2 + 60.5, 88));
    cylPnts.push_back(MbCartPoint3D(0, -(assortmentCamera / 2 + 60.5), 98));

    // Построение трубы
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, cylPnts, namesElSolid), pCylSolid);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr pMergeSolid, pMainSolid(pSolid);

    //return pMainSolid;

    // Первое объеденение
    ::BooleanResult(pMainSolid, cm_Copy, pCylSolid, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), pMergeSolid);

    ////ВЫДАВЛИВАНИЕ
    RPArray<MbContour> arrExtrudeContours;

    createWallXZSecond(arrExtrudeContours, length3, assortmentCamera, thicknessCamera);

    MbSweptData sweptDataInnerWall(*pPlaneXZ, arrExtrudeContours);

    MbVector3D dirX(1, 0, 0);
    MbVector3D dirY(0, 1, 0);
    MbVector3D dirZ(0, 0, 1);

    MbSolid* pWallYZ;
    MbSolid* pExtrudeSolid;

    ::RevolutionSolid(sweptDataInnerWall, axis, revParms, operNames, cNames, pExtrudeSolid);

    SolidSPtr pInnerSolid(pExtrudeSolid), pDifferentSolid;

    // Первое вырезание
    ::BooleanResult(pMergeSolid, cm_Copy, pInnerSolid, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), pDifferentSolid);

    // Главная стенка XZ
    ::ExtrusionSolid(sweptDataInnerWall, dirY, NULL, NULL, false, ExtrusionValues(0, 10), operNames, cNames, pWallYZ);

    SolidSPtr unionSolid, wallYZ(pWallYZ);
    // Второе объеденение
    ::BooleanResult(pDifferentSolid, cm_Copy, wallYZ, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), unionSolid);

    SolidSPtr holeSolid, holeSolid2, mergeHole, mergeHole2;

    SpacePointsVector holePnts, holePnts2;

    holePnts.push_back(MbCartPoint3D(0, 50, 88));
    holePnts.push_back(MbCartPoint3D(0, assortmentCamera / 2 + 60.5, 88));
    holePnts.push_back(MbCartPoint3D(0, 50, 94));

    holePnts2.push_back(MbCartPoint3D(0, -50, 88));
    holePnts2.push_back(MbCartPoint3D(0, -(assortmentCamera / 2 + 60.5), 88));
    holePnts2.push_back(MbCartPoint3D(0, -50, 94));

    // Построение трубы
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, holePnts, namesElSolid), holeSolid);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, holePnts2, namesElSolid), holeSolid2);

    ::BooleanResult(unionSolid, cm_Copy, holeSolid, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), mergeHole);
    ::BooleanResult(mergeHole, cm_Copy, holeSolid2, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), mergeHole2);

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

    ::BooleanResult(mergeHole2, cm_Copy, boltCyl, cm_Copy,
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