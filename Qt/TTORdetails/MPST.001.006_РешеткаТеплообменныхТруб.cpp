#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

struct palmParams
{
    double rOut;    // 1
    double PIn;     // 2  
    double POut;    // 3
    double D1;
};

std::vector<palmParams> palmConfiguration = {
    //1     2    3 
    {235, 145, 167, 200},
    {260, 170, 192, 225},
    {315, 228, 250, 280},
};

struct test1
{
    double Dy;    // 1   
    double D;     // 2   
    double D1;    // 3   
    double D2;    // 4   
    double D3;    // 5   
    double D4;    // 6   
    double D5;    // 7   
    double D6;    // 8   
    double d;     // 9  
    double n;     // 10  
    double h;     // 11  
    double h1;    // 12  
    double h2;    // 13  
    double diam;  // 14  
};

std::vector<test1> test1Params = {
    //1     2    3    4   5     6    7   8     9    10 11 12 13  14
    {125,  235, 200, 178, 146, 166, 145, 167,  18,  8, 3, 4, 3,  16},
    {150,  260, 225, 202, 171, 191, 170, 192,  18,  8, 3, 4, 3,  16},
    {175,  290, 255, 232, 203, 223, 202, 224,  18,  8, 3, 4, 3,  16},
    {200,  315, 280, 258, 229, 249, 228, 250,  18,  8, 4, 4, 4,  16},
    {225,  340, 305, 282, 256, 276, 255, 277,  18,  8, 4, 4, 4,  16},
    {250,  370, 335, 312, 283, 303, 282, 304,  18,  8, 4, 4, 4,  16},
    {300,  435, 395, 365, 336, 356, 335, 357,  18,  8, 4, 4, 4,  16},
    {350,  485, 445, 415, 386, 406, 385, 407,  18,  8, 5, 4, 4,  16},
    {400,  535, 495, 465, 436, 456, 435, 457,  18,  8, 5, 4, 4,  16},
};

SolidSPtr HolyHole2(SolidSPtr* previus, int holes, double DiametrCircle, double radius) {

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

void CreateSketcherResetkaTruba(RPArray<MbContour>& _arrContours, int index, double ttDiam, double ttThickness, double ktDiam, double ktThickness)
{
    int index4 = 0;

    if (ktDiam < 145) {
        index4 = 0;
    }
    else if (ktDiam < 170) {
        index4 = 1;
    }
    else if (ktDiam < 201) {
        index4 = 2;
    }
    else if (ktDiam < 228) {
        index4 = 3;
    }
    else if (ktDiam < 255) {
        index4 = 4;
    }
    else if (ktDiam < 282) {
        index4 = 5;
    }
    else if (ktDiam < 335) {
        index4 = 6;
    }
    else if (ktDiam < 385) {
        index4 = 7;
    }
    else if (ktDiam < 435) {
        index4 = 8;
    }
    else {
        index4 = -1; // TOZO: ????????
    }

    const double D = test1Params[index4].D;

    const double firstParam =  palmConfiguration[index].rOut;
    const double secondParam = palmConfiguration[index].PIn;
    const double thirdParam = palmConfiguration[index].POut;

    double ANG1 = 90 * DEG_TO_RAD;
    const double rIn = (ttDiam + ttThickness * 2) / 2;
    const double rOut = D / 2;

    const double PIn = secondParam / 2;
    const double POut = thirdParam / 2;


    SArray<MbCartPoint> arrPnts(10);
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

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_006_RezhetkaTeplTube(double ttDiam, double ttThickness, double ktDiam, double ktThickness)
{

    int index = 0;

    if (ttDiam <= 89) { //ttDiam = 89
        index = 0;
    }
    else if (ttDiam <= 108) { //ttDiam = 108
        index = 1;
    }
    else if (ttDiam <= 159) { //ttDiam = 159
        index = 2;
    }

    const double firstParam = palmConfiguration[index].D1;

    // Локальная СК (по умолчанию совпадает с мировой СК)
    MbPlacement3D pl;

    // Создание образующей для тела выдавливания
    RPArray<MbContour> arrContours;
    CreateSketcherResetkaTruba(arrContours, index, ttDiam, ttThickness, ktDiam, ktThickness);

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

    // Создание умного указателя на твердоге тело
    SolidSPtr Solid(pSolid);

    const double rastchet = (ttDiam + ttThickness * 2) / 2;
    // Создание отверстий
    SolidSPtr result = HolyHole2(&Solid, 4, firstParam, 8);

    return result;
}