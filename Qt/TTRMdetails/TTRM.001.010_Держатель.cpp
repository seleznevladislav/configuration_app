#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

void createHolderSketch(RPArray<MbContour>& _arrContours, int assortmentInnerTubes, int holderLength)
{
    const double radius = assortmentInnerTubes / 2;

    SArray<MbCartPoint> arrPnts(6);

    arrPnts.Add(MbCartPoint(radius, 0));
    arrPnts.Add(MbCartPoint(radius + 5, 0));
    arrPnts.Add(MbCartPoint(radius + 10, 5));
    arrPnts.Add(MbCartPoint(radius + 10, holderLength - 5));
    arrPnts.Add(MbCartPoint(radius + 5, holderLength));
    arrPnts.Add(MbCartPoint(radius, holderLength));

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

void createPolygonSketch(RPArray<MbContour>& _arrContours, int assortmentInnerTubes)
{
    const double radius = assortmentInnerTubes / 2;
    const double x = radius + 10;
    const double y = (sqrt(3) * x) / 2;

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

SPtr<MbSolid> ParametricModelCreator::createPipeHolder_010(double holderLength, double assortmentInnerTubes)
{
    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    createHolderSketch(arrContours, assortmentInnerTubes, holderLength);

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
    createPolygonSketch(arrContours2, assortmentInnerTubes);

    //Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbSweptData sweptData2(*pPlaneXY, arrContours2);

    MbVector3D dir(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 1));

    const double HEIGHT_FORWARD = holderLength, HEIGHT_BACKWARD = 0;

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