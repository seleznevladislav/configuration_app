#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void createSketchSecond(RPArray<MbContour>& _arrContours)
{
    SArray<MbCartPoint> arrPnts(100);

    arrPnts.Add(MbCartPoint(0, 239)); //0
    arrPnts.Add(MbCartPoint(0, 233)); //1

    arrPnts.Add(MbCartPoint(0, 39)); //центр дуги большой 0

    arrPnts.Add(MbCartPoint(85.23, 213.28)); //3

    arrPnts.Add(MbCartPoint(54.75, 184));//центр дуги маленькой 0

    arrPnts.Add(MbCartPoint(103.5, 184)); //5

    arrPnts.Add(MbCartPoint(103.5, 0)); //6
    arrPnts.Add(MbCartPoint(157.5, 0)); //7
    arrPnts.Add(MbCartPoint(157.5, 22)); //8
    arrPnts.Add(MbCartPoint(109.5, 22)); //9
    arrPnts.Add(MbCartPoint(109.5, 184));//10

    arrPnts.Add(MbCartPoint(70.910221, 184));//центр дуги маленькой 11

    arrPnts.Add(MbCartPoint(87.86, 218.67)); //12

    arrPnts.Add(MbCartPoint(0, 39)); //центр дуги большой 13

    arrPnts.Add(MbCartPoint(0, 239)); //14

    arrPnts.Add(MbCartPoint(70.910221, 184)); // центр  15


    const double RADIUSB1 = 194;
    const double RADIUSB2 = 200;

    const double RADIUSM1 = 32.589779;
    const double RADIUSM2 = 38.589779;

    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[0]);
    MbArc* pArc1 = new MbArc(arrPnts[2], RADIUSB2, arrPnts[0], arrPnts[12], -1);
    MbArc* pArc2 = new MbArc(arrPnts[15], RADIUSM2, arrPnts[12], arrPnts[10], -1);

    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[10], arrPnts[9]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[9], arrPnts[8]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[8], arrPnts[7]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[7], arrPnts[6]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[6], arrPnts[5]);


    MbArc* pArc3 = new MbArc(arrPnts[11], RADIUSM1, arrPnts[5], arrPnts[3], 1);
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

void createWallXZSecond(RPArray<MbContour>& _arrContours)
{
    SArray<MbCartPoint> wallPnts(7);

    wallPnts.Add(MbCartPoint(0, 233));
    wallPnts.Add(MbCartPoint(-85.23, 213.28));
    wallPnts.Add(MbCartPoint(-103.5, 184));
    wallPnts.Add(MbCartPoint(-103.5, 0));
    wallPnts.Add(MbCartPoint(103.5, 0));
    wallPnts.Add(MbCartPoint(103.5, 184));
    wallPnts.Add(MbCartPoint(85.23, 213.28));


    MbLineSegment* pLine = new MbLineSegment(wallPnts[0], wallPnts[1]);
    MbLineSegment* pLine2 = new MbLineSegment(wallPnts[1], wallPnts[2]);
    MbLineSegment* pLine3 = new MbLineSegment(wallPnts[2], wallPnts[3]);
    MbLineSegment* pLine4 = new MbLineSegment(wallPnts[3], wallPnts[4]);
    MbLineSegment* pLine5 = new MbLineSegment(wallPnts[4], wallPnts[5]);
    MbLineSegment* pLine6 = new MbLineSegment(wallPnts[5], wallPnts[6]);
    MbLineSegment* pLine7 = new MbLineSegment(wallPnts[6], wallPnts[0]);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::create_inner_pipes_grid_006()
{
    // ��������� �� (�� ��������� ��������� � ������� ��)
    MbPlacement3D pl;

    RPArray<MbContour> arrMainContours;
    createSketchSecond(arrMainContours);

    //���������
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));

    MbSweptData sweptData(*pPlaneYZ, arrMainContours);

    //���� ��������
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // ����������� ��� �������� ���������� ���� �������� � ��� �������� ����������
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // ��� �������� ��� ���������� ����:
    MbAxis3D axis(pl.GetAxisZ());

    // ����� �������-������� ��� ���������� �������� ���� ��������
    MbSolid* pSolid;

    ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    MbSNameMaker namesElSolid(ct_ElementarySolid, MbSNameMaker::i_SideNone, 0);
    SolidSPtr pCylSolid;
    SpacePointsVector cylPnts;

    cylPnts.push_back(MbCartPoint3D(0, -170, 88));
    cylPnts.push_back(MbCartPoint3D(0, 170, 88));
    cylPnts.push_back(MbCartPoint3D(0, -170, 98));

    // ���������� �����
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, cylPnts, namesElSolid), pCylSolid);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    SolidSPtr pMergeSolid, pMainSolid(pSolid);

    // ������ �����������
    ::BooleanResult(pMainSolid, cm_Copy, pCylSolid, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), pMergeSolid);

    ////������������
    RPArray<MbContour> arrExtrudeContours;

    createWallXZSecond(arrExtrudeContours);

    MbSweptData sweptDataInnerWall(*pPlaneXZ, arrExtrudeContours);

    MbVector3D dirX(1, 0, 0);
    MbVector3D dirY(0, 1, 0);
    MbVector3D dirZ(0, 0, 1);

    MbSolid* pWallYZ;
    MbSolid* pExtrudeSolid;

    ::RevolutionSolid(sweptDataInnerWall, axis, revParms, operNames, cNames, pExtrudeSolid);

    SolidSPtr pInnerSolid(pExtrudeSolid), pDifferentSolid;

    // ������ ���������
    ::BooleanResult(pMergeSolid, cm_Copy, pInnerSolid, cm_Copy, MbBooleanOperationParams(bo_Difference, false, operBoolNames), pDifferentSolid);

    // ������� ������ XZ
    ::ExtrusionSolid(sweptDataInnerWall, dirY, NULL, NULL, false, ExtrusionValues(0, 10), operNames, cNames, pWallYZ);

    SolidSPtr unionSolid, wallYZ(pWallYZ);
    // ������ �����������
    ::BooleanResult(pDifferentSolid, cm_Copy, wallYZ, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), unionSolid);

    SolidSPtr holeSolid, holeSolid2, mergeHole, mergeHole2;

    SpacePointsVector holePnts, holePnts2;

    holePnts.push_back(MbCartPoint3D(0, 50, 88));
    holePnts.push_back(MbCartPoint3D(0, 170, 88));
    holePnts.push_back(MbCartPoint3D(0, 50, 94));

    holePnts2.push_back(MbCartPoint3D(0, -50, 88));
    holePnts2.push_back(MbCartPoint3D(0, -170, 88));
    holePnts2.push_back(MbCartPoint3D(0, -50, 94));

    // ���������� �����
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

    boltCylPnts.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle), 280 / 2 * sin(boltAngle), 0));
    boltCylPnts.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle), 280 / 2 * sin(boltAngle), 25));
    boltCylPnts.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle) + 5, 280 / 2 * sin(boltAngle), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, operNames), boltCyl);

    SpacePointsVector boltCylPnts2;

    boltCylPnts2.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle2), 280 / 2 * sin(boltAngle2), 0));
    boltCylPnts2.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle2), 280 / 2 * sin(boltAngle2), 25));
    boltCylPnts2.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle2) + 5, 280 / 2 * sin(boltAngle2), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts2, operNames), boltCyl2);

    SpacePointsVector boltCylPnts3;

    boltCylPnts3.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle3), 280 / 2 * sin(boltAngle3), 0));
    boltCylPnts3.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle3), 280 / 2 * sin(boltAngle3), 25));
    boltCylPnts3.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle3) + 5, 280 / 2 * sin(boltAngle3), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts3, operNames), boltCyl3);

    SpacePointsVector boltCylPnts4;

    boltCylPnts4.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle4), 280 / 2 * sin(boltAngle4), 0));
    boltCylPnts4.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle4), 280 / 2 * sin(boltAngle4), 25));
    boltCylPnts4.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle4) + 5, 280 / 2 * sin(boltAngle4), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts4, operNames), boltCyl4);

    SpacePointsVector boltCylPnts5;

    boltCylPnts5.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle5), 280 / 2 * sin(boltAngle5), 0));
    boltCylPnts5.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle5), 280 / 2 * sin(boltAngle5), 25));
    boltCylPnts5.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle5) + 5, 280 / 2 * sin(boltAngle5), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts5, operNames), boltCyl5);

    SpacePointsVector boltCylPnts6;

    boltCylPnts6.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle6), 280 / 2 * sin(boltAngle6), 0));
    boltCylPnts6.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle6), 280 / 2 * sin(boltAngle6), 25));
    boltCylPnts6.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle6) + 5, 280 / 2 * sin(boltAngle6), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts6, operNames), boltCyl6);

    SpacePointsVector boltCylPnts7;

    boltCylPnts7.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle7), 280 / 2 * sin(boltAngle7), 0));
    boltCylPnts7.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle7), 280 / 2 * sin(boltAngle7), 25));
    boltCylPnts7.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle7) + 5, 280 / 2 * sin(boltAngle7), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts7, operNames), boltCyl7);

    SpacePointsVector boltCylPnts8;

    boltCylPnts8.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle8), 280 / 2 * sin(boltAngle8), 0));
    boltCylPnts8.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle8), 280 / 2 * sin(boltAngle8), 25));
    boltCylPnts8.push_back(MbCartPoint3D(280 / 2 * cos(boltAngle8) + 5, 280 / 2 * sin(boltAngle8), 0));

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