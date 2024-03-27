#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

SolidSPtr HolyHole3(SolidSPtr* previus, int holes, double DiametrCircle, double radius) {

    // Именователь граней для построения тела с помощью булевой операции 
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

void CreateSketcherFlanecKozhuh(RPArray<MbContour>& _arrContours)
{

    double ANG1 = 90 * DEG_TO_RAD;

    SArray<MbCartPoint> arrPnts(100);
    arrPnts.Add(MbCartPoint(133 / 2, 40));
    arrPnts.Add(MbCartPoint(133 / 2, 0));
    arrPnts.Add(MbCartPoint(145 / 2, 0));
    arrPnts.Add(MbCartPoint(145 / 2, 3));
    arrPnts.Add(MbCartPoint(167 / 2, 3));
    arrPnts.Add(MbCartPoint(167 / 2, 0));
    arrPnts.Add(MbCartPoint(178 / 2, 0));
    arrPnts.Add(MbCartPoint(180 / 2, 2));
    arrPnts.Add(MbCartPoint(235 / 2, 2));
    arrPnts.Add(MbCartPoint(235 / 2, 14));
    arrPnts.Add(MbCartPoint(158 / 2, 14));//10
    arrPnts.Add(MbCartPoint(158 / 2, 18));//центр
    arrPnts.Add(MbCartPoint(158 / 2 + 4 * cos(ANG1), 18 + 4 * sin(ANG1)));
    arrPnts.Add(MbCartPoint(133 / 2 + 5, 40));

    //(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1)

    arrPnts.Add(MbCartPoint(-315, 65));
    arrPnts.Add(MbCartPoint(0, 400));


    const double RADIUSB = 4;

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

    //MbLineSegment* pLinetest1 = new MbLineSegment(arrPnts[10], arrPnts[11]);
    //MbLineSegment* pLinetest2 = new MbLineSegment(arrPnts[11], arrPnts[12]);
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
    //pContour->AddSegment(pLinetest1);
    //pContour->AddSegment(pLinetest2);
    pContour->AddSegment(pLine10);
    pContour->AddSegment(pLine11);


    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_007_FlanecKozhux()
{
    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    CreateSketcherFlanecKozhuh(arrContours);

    //Плоскость
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbSweptData sweptData(*pPlaneXY, arrContours);

    //Угол Вращения
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Ось вращения для построения тела:
    MbAxis3D axis(pl.GetAxisZ());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    SolidSPtr Solid(pSolid);

    SolidSPtr result = HolyHole3(&Solid, 4, 200, 9);

    //TestVariables::viewManager->AddObject(TestVariables::SURFACECURVE_Style, arrContours[0]);
    // TestVariables::viewManager->AddObject(TestVariables::SURFACECURVE_Style, result);

    return result;
}