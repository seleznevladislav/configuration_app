#include "../BuildMathModel.h"

using namespace BuildMathModel;

// TOZO: надо понять как правильно высчитывать depth
// TOZO: Не учавствует в расчета диаметр камеры (radiusMal потом включить)
const double DEG_TO_RAD = M_PI / 180.0;

struct mpst001004
{
    double Db;    // 1 - Диаметр внешний
    double Dm;    // 2 - Диаметр внутренний
    double Hb;    // 3 - Высота внешнего диаметра
    double Hm;    // 4 - Высота внутреннего диаметра
};

// 307 - тк на камере 301 проклада на 6 мм
std::vector<mpst001004> parametersFor004 = {
    //1     2    3    4 
    {400,  307, 90, 80},
    {510,  417, 130, 110},
};

SolidSPtr HolyHole(SolidSPtr* previus, int holes, double DiametrCircle, double radius) {

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
        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1), DiametrCircle / 2 * sin(ANG1), 200));
        boltCylPnts.push_back(MbCartPoint3D(DiametrCircle / 2 * cos(ANG1) + radius, DiametrCircle / 2 * sin(ANG1), 0));

        SolidSPtr buffer;

        ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);

        ::BooleanResult(news, cm_Copy, boltCyl, cm_Copy,
            MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), buffer);

        news = buffer;

    }



    return news;
}

void CreateSketcherResetka(RPArray<MbContour>& _arrContours, double ktDiam)
{

    int index = 0;

    if (ktDiam < 219) { //ktDiam = 133
        index = 0;
    }
    else if (ktDiam >= 219) { //ktDiam = 219
        index = 1;
    }

    double ANG1 = 180 * DEG_TO_RAD;

    const double dTruba = ktDiam;

    const double Db = parametersFor004[index].Db;
    const double Dm = parametersFor004[index].Dm;
    const double Hb = parametersFor004[index].Hb;
    const double Hm = parametersFor004[index].Hm;

    SArray<MbCartPoint> arrPnts(6);
    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint(Db, 0));
    arrPnts.Add(MbCartPoint(Db, Hm));
    arrPnts.Add(MbCartPoint(Dm, Hm));
    arrPnts.Add(MbCartPoint(Dm, Hb));
    arrPnts.Add(MbCartPoint(0, Hb));


    MbLineSegment* pLine0 = new MbLineSegment(arrPnts[0], arrPnts[1]);
    MbLineSegment* pLine1 = new MbLineSegment(arrPnts[1], arrPnts[2]);
    MbLineSegment* pLine2 = new MbLineSegment(arrPnts[2], arrPnts[3]);
    MbLineSegment* pLine3 = new MbLineSegment(arrPnts[3], arrPnts[4]);
    MbLineSegment* pLine4 = new MbLineSegment(arrPnts[4], arrPnts[5]);

    MbContour* pContour = new MbContour();

    pContour->AddSegment(pLine0);
    pContour->AddSegment(pLine1);
    pContour->AddSegment(pLine2);
    pContour->AddSegment(pLine3);
    pContour->AddSegment(pLine4);

    _arrContours.push_back(pContour);
}

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_004_reshetkaKozhux(double ktDiam, double ktThickness, double t)
{

    int index = 0;

    if (ktDiam < 219) { //ktDiam = 133
        index = 0;
    }
    else if (ktDiam >= 219) { //ktDiam = 219
        index = 1;
    }

    double ANG1 = 180 * DEG_TO_RAD;

    const double dTruba = ktDiam;

    const double Db = parametersFor004[index].Db;
    const double Dm = parametersFor004[index].Dm;
    const double Hb = parametersFor004[index].Hb;
    const double Hm = parametersFor004[index].Hm;

    const double depth = Hb;
    const double radiusMal = (ktDiam + 2 * ktThickness) / 2;
    // const double radiusMal = 143 / 2;

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    MbPlacement3D pl;

    RPArray<MbContour> arrContours;
    CreateSketcherResetka(arrContours, ktDiam);

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

    SolidSPtr result = HolyHole(&Solid, 32, (Db-30) * 2, 24 / 2);

    SolidSPtr pSmallCyl, pSmallCyl2, pSmallCyl3, pSmallCyl4, pMergeSmallCyllBlock, pMergeSmallCyllBlock2, pMergeSmallCyllBlock3, pMergeSmallCyllBlock4;

    SpacePointsVector smallCylPnts;

    smallCylPnts.push_back(MbCartPoint3D(-t / 2, -t / 2, 0));
    smallCylPnts.push_back(MbCartPoint3D(-t / 2, -t / 2, depth));
    smallCylPnts.push_back(MbCartPoint3D(-t / 2 + radiusMal, -t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts, blockNames), pSmallCyl);

    ::BooleanResult(result, cm_Copy, pSmallCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock);

    SpacePointsVector smallCylPnts2;

    smallCylPnts2.push_back(MbCartPoint3D(t / 2, t / 2, 0));
    smallCylPnts2.push_back(MbCartPoint3D(t / 2, t / 2, depth));
    smallCylPnts2.push_back(MbCartPoint3D(t / 2 + radiusMal, t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts2, blockNames), pSmallCyl2);

    ::BooleanResult(pMergeSmallCyllBlock, cm_Copy, pSmallCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock2);

    SpacePointsVector smallCylPnts3;

    smallCylPnts3.push_back(MbCartPoint3D(t / 2, -t / 2, 0));
    smallCylPnts3.push_back(MbCartPoint3D(t / 2, -t / 2, depth));
    smallCylPnts3.push_back(MbCartPoint3D(t / 2 + radiusMal, -t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts3, blockNames), pSmallCyl3);

    ::BooleanResult(pMergeSmallCyllBlock2, cm_Copy, pSmallCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock3);

    SpacePointsVector smallCylPnts4;

    smallCylPnts4.push_back(MbCartPoint3D(-t / 2, t / 2, 0));
    smallCylPnts4.push_back(MbCartPoint3D(-t / 2, t / 2, depth));
    smallCylPnts4.push_back(MbCartPoint3D(-t / 2 + radiusMal, t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts4, blockNames), pSmallCyl4);

    ::BooleanResult(pMergeSmallCyllBlock3, cm_Copy, pSmallCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock4);

    return pMergeSmallCyllBlock4;
}