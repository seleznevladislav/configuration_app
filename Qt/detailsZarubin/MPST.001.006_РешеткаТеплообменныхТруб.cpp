#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

SolidSPtr HolyHole2(SolidSPtr* previus, int holes, double DiametrCircle, double radius) {

    // ����������� ������ ��� ���������� ���� � ������� ������� �������� 
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
    boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 100));
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
        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 100));
        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1) + radius, DiametrCircle / 2 * sin(ANG1), 0));

        SolidSPtr buffer;

        ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);

        ::BooleanResult(news, cm_Copy, boltCyl, cm_Copy,
            MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), buffer);

        news = buffer;

    }



    return news;
}

void CreateSketcherResetkaTruba(RPArray<MbContour>& _arrContours)
{

    double ANG1 = 90 * DEG_TO_RAD;
    const double rIn = 99 / 2;
    const double rOut = 235 / 2;

    const double PIn = 145 / 2;
    const double POut = 167 / 2;


    SArray<MbCartPoint> arrPnts(100);
    arrPnts.Add(MbCartPoint(rIn, 0));
    arrPnts.Add(MbCartPoint(rOut, 0));
    arrPnts.Add(MbCartPoint(rOut, 14));
    arrPnts.Add(MbCartPoint(POut, 14));
    arrPnts.Add(MbCartPoint(POut, 11));
    arrPnts.Add(MbCartPoint(PIn, 11));
    arrPnts.Add(MbCartPoint(PIn, 14));
    arrPnts.Add(MbCartPoint(rIn, 14));


    MbLineSegment* pLine0 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[6], arrPnts[7]);
    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[7], arrPnts[0]);




    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine0);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);


    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_006_RezhetkaTeplTube()
{
    // ��������� �� (�� ��������� ��������� � ������� ��)
    MbPlacement3D pl;

    // �������� ���������� ��� ���� ������������
    RPArray<MbContour> arrContours;
    CreateSketcherResetkaTruba(arrContours);

    //���������
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneXY, arrContours);

    //���� ��������
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // ����������� ��� �������� ���������� ���� �������� � ��� �������� ����������
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // ��� �������� ��� ���������� ����:
    MbAxis3D axis(pl.GetAxisZ());

    // ����� �������-������� ��� ���������� �������� ���� ��������
    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    SolidSPtr Solid(pSolid);

    SolidSPtr result = HolyHole2(&Solid, 4, 200, 9);

    return result;
}