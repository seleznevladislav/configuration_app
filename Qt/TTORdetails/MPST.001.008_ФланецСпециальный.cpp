// ���� 12815-80
// ���������� 5 - ������ � �����

#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

struct GOST_12815_80
{
    double Dy;    // 1   
    double D;     // 2   
    double D1;    // 3   
    double D2;    // 4   
    double D3;    // 5   
    double D4;    // 6   
    double D5;    // 7  �������� 
    double D6;    // 8   
    double d;     // 9  
    double n;     // 10  
    double h;     // 11  
    double h1;    // 12  
    double h2;    // 13  
    double diam;  // 14  
};

std::vector<GOST_12815_80> configurationZarubin = {
    //1     2    3    4   5     6    7   8     9    10 11 12 13  14
    {80,   185, 150, 128, 101, 115, 100, 116,  18,  4, 3, 4, 3,  16},
    {100,  205, 170, 148, 117, 137, 116, 138,  18,  4, 3, 4, 3,  16},
    {125,  235, 200, 178, 146, 166, 145, 167,  18,  8, 3, 4, 3,  16},
    {150,  260, 225, 202, 171, 191, 170, 192,  18,  8, 3, 4, 3,  16},
};

SolidSPtr HolyHole412(SolidSPtr* previus, int holes, double DiametrCircle, double radius) {

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

void CreateSketcherFlanecTrubadif(RPArray<MbContour>& _arrContours, double ttDiam, double ttThickness, int index)
{

    // const double Dy = configurationZarubin[index].Dy;

    double ANG1 = 180 * DEG_TO_RAD;

    const double dTruba = ttDiam;

    const double D = configurationZarubin[index].D;
    const double D2 = configurationZarubin[index].D2;
    const double D5 = configurationZarubin[index].D5;
    const double D6 = configurationZarubin[index].D6;
    const double h = configurationZarubin[index].h;
    const double h2 = configurationZarubin[index].h2;
    const double RADIUSB = 4;

    SArray<MbCartPoint> arrPnts(20);
    arrPnts.Add(MbCartPoint(dTruba / 2, 40));
    arrPnts.Add(MbCartPoint(dTruba / 2, 0));
    arrPnts.Add(MbCartPoint(D5 / 2, 0));
    arrPnts.Add(MbCartPoint(D5 / 2, h));
    arrPnts.Add(MbCartPoint(D6 / 2, h));
    arrPnts.Add(MbCartPoint(D6 / 2, 0));//
    arrPnts.Add(MbCartPoint(D2 / 2, 0));
    arrPnts.Add(MbCartPoint((D2 + 2) / 2, h2));
    arrPnts.Add(MbCartPoint(D / 2, h2));
    arrPnts.Add(MbCartPoint(D / 2, 14));//
    arrPnts.Add(MbCartPoint((dTruba + ttThickness * 2 + 16) / 2, 14));//10
    arrPnts.Add(MbCartPoint((dTruba + ttThickness * 2 + 16) / 2, 18));//�����
    arrPnts.Add(MbCartPoint((dTruba + ttThickness * 2 + 16) / 2 + RADIUSB * cos(ANG1), 18 + RADIUSB * sin(ANG1)));
    arrPnts.Add(MbCartPoint(dTruba / 2 + ttThickness, 40));

    MbLineSegment* pLine0 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[4], arrPnts[5]);
    MbLineSegment* pLine5 = new MbLineSegment(arrPnts[5], arrPnts[6]);
    MbLineSegment* pLine6 = new MbLineSegment(arrPnts[6], arrPnts[7]);
    MbLineSegment* pLine7 = new MbLineSegment(arrPnts[7], arrPnts[8]);
    MbLineSegment* pLine8 = new MbLineSegment(arrPnts[8], arrPnts[9]);
    MbLineSegment* pLine9 = new MbLineSegment(arrPnts[9], arrPnts[10]);


    MbArc* pArc1 = new MbArc(arrPnts[11], RADIUSB, arrPnts[10], arrPnts[12], -1);

    MbLineSegment* pLine10 = new MbLineSegment(arrPnts[12], arrPnts[13]);
    MbLineSegment* pLine11 = new MbLineSegment(arrPnts[13], arrPnts[0]);


    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine0);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);
    pContour->AddSegment(pLine5);
    pContour->AddSegment(pLine6);
    pContour->AddSegment(pLine7);
    pContour->AddSegment(pLine8);
    pContour->AddSegment(pLine9);
    pContour->AddSegment(pArc1);
    pContour->AddSegment(pLine10);
    pContour->AddSegment(pLine11);


    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_008_FlanecSpecial(double ttDiam, double ttThickness)
{
    int index = 0;

    // �� ������� d3
    if (ttDiam < 100) { //ttDiam = 89
        index = 0;
    }
    else if (ttDiam < 116) { //ttDiam = 108
        index = 1;
    }
    else if (ttDiam < 145) { //ttDiam = 133
        index = 2;
    }
    else if (ttDiam < 170) { //ttDiam = 159
        index = 3;
    }

    //const double n = configurationZarubin[index].n; // TOZO: �������� ������ ����� � ����������� �� ���-�� ���������
    const double n = 4;
    const double D1 = configurationZarubin[index].D1;

    // ��������� �� (�� ��������� ��������� � ������� ��)
    MbPlacement3D pl;

    // �������� ���������� ��� ���� ������������
    RPArray<MbContour> arrContours;
    CreateSketcherFlanecTrubadif(arrContours, ttDiam, ttThickness, index);

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

    SolidSPtr result = HolyHole412(&Solid, n, D1, 8);

    return result;
}