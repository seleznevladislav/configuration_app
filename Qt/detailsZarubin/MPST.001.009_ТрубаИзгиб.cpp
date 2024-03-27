#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void CreateSketchIzgib(RPArray<MbContour>& _arrContours)
{
    // ���������� ���������� � ���� ���� ��������������� �����������
    const double RAD_EXT = 99.0 / 2; // ������ ������� ����������
    const double RAD_INT = 89.0 / 2; // ������ ���������� ����������

    //�� ��� Y �������� ����� (�������� t/2)
    MbArc* pCircleExt = new MbArc(MbCartPoint(0, 127.5), RAD_EXT);
    MbArc* pCircleInt = new MbArc(MbCartPoint(0, 127.5), RAD_INT);

    _arrContours.push_back(new MbContour(*pCircleExt, true));
    _arrContours.push_back(new MbContour(*pCircleInt, true));
}

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_009_curevedTube()
{
    MbPlacement3D* plLine = new MbPlacement3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 1), MbCartPoint3D(0, 1, 0));

    const double PL = 255 / 2;

    MbLineSegment* pL12D = new MbLineSegment(MbCartPoint(0, PL), MbCartPoint(20, PL));
    MbLineSegment3D* pL1 = new MbLineSegment3D(*pL12D, *plLine);

    MbArc* pA12D = new MbArc(MbCartPoint(20, 0), MbCartPoint(20, PL), MbCartPoint(20, -PL), -1);
    MbArc3D* pA1 = new MbArc3D(*pA12D, *plLine);

    MbLineSegment* pL32D = new MbLineSegment(MbCartPoint(20, -PL), MbCartPoint(0, -PL));
    MbLineSegment3D* pL3 = new MbLineSegment3D(*pL32D, *plLine);

    MbContour3D* pLine = new MbContour3D();
    pLine->AddSegment(*pL1, false);
    pLine->AddSegment(*pA1, false);
    pLine->AddSegment(*pL3, false);

    RPArray<MbContour> arrContours;
    CreateSketchIzgib(arrContours);

    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbSweptData sweptData(*pPlaneXY, arrContours);

    //����������� - ���������
    EvolutionValues evParms;
    evParms.SetKeepingAngle();
    evParms.SetBySurfaceNormal(false);
    MbSNameMaker operNames(ct_CurveEvolutionSolid, MbSNameMaker::i_SideNone, 0);
    MbSNameMaker cNames(ct_CurveSweptSolid, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> contourNames(1, 0, false);
    contourNames.Add(&cNames);
    MbSNameMaker spineNames(ct_ElementarySolid, MbSNameMaker::i_SideNone, 0);

    MbSolid* pSolid = nullptr;

    MbResultType res = ::EvolutionSolid(sweptData, *pLine, evParms, operNames, contourNames, spineNames, pSolid);

    SolidSPtr variable = SolidSPtr(pSolid);

    return variable;
}