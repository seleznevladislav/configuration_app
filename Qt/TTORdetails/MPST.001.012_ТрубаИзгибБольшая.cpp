#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void CreateSketchIzgibBigf(RPArray<MbContour>& _arrContours, double ttDiam, double ttThickness, double t)
{
    // Построение образующей в виде двух концентрических окружностей
    const double RAD_EXT =  (ttDiam + 2 * ttThickness) / 2; // Радиус внешней окружности
    const double RAD_INT = ttDiam / 2; // Радиус внутренней окружности

    //По оси Y сдвинута вверх (параметр t/2)
    MbArc* pCircleExt = new MbArc(MbCartPoint(0, t), RAD_EXT);
    MbArc* pCircleInt = new MbArc(MbCartPoint(0, t), RAD_INT);

    _arrContours.push_back(new MbContour(*pCircleExt, true));
    _arrContours.push_back(new MbContour(*pCircleInt, true));
}

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_012_curevedTubeBig(double ttDiam, double ttThickness, double assemblyHeight, double t)
{
    double length = assemblyHeight / 2 - t / 2; // Половина высоты
    double radius = t/4;

    MbPlacement3D* plLine = new MbPlacement3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 1), MbCartPoint3D(0, 1, 0));

    MbLineSegment* pL12D = new MbLineSegment(MbCartPoint(0, length), MbCartPoint(20, length));
    MbLineSegment3D* pL1 = new MbLineSegment3D(*pL12D, *plLine);

    MbArc* pA12D = new MbArc(MbCartPoint(20, length - radius), MbCartPoint(20, length), MbCartPoint(20 + radius, length - radius), -1);
    MbArc3D* pA1 = new MbArc3D(*pA12D, *plLine);

    MbLineSegment* pLMiddle = new MbLineSegment(MbCartPoint(20 + radius, length - radius), MbCartPoint(20 + radius, -length + radius));
    MbLineSegment3D* pL2 = new MbLineSegment3D(*pLMiddle, *plLine);

    MbArc* pANext = new MbArc(MbCartPoint(20, -length + radius), MbCartPoint(20 + radius, -length + radius), MbCartPoint(20, -length), -1);
    MbArc3D* pA1Next = new MbArc3D(*pANext, *plLine);

    MbLineSegment* pL32D = new MbLineSegment(MbCartPoint(20, -length), MbCartPoint(0, -length));
    MbLineSegment3D* pL3 = new MbLineSegment3D(*pL32D, *plLine);

    MbContour3D* pLine = new MbContour3D();
    pLine->AddSegment(*pL1, false);
    pLine->AddSegment(*pA1, false);
    pLine->AddSegment(*pL2, false);
    pLine->AddSegment(*pA1Next, false);
    pLine->AddSegment(*pL3, false);

    RPArray<MbContour> arrContours;
    CreateSketchIzgibBigf(arrContours, ttDiam, ttThickness, length);


    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbSweptData sweptData(*pPlaneXY, arrContours);

    //Именователи - параметры
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