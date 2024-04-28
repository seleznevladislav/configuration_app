#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;


SPtr<MbSolid> ParametricModelCreator::createCup_005(double t, double assortmentOuterTubes, double assortmentCamera)
{
    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);

    SolidSPtr pipeCap, bigCircle, smallRadiusCyl;

    SpacePointsVector bigCirclePnts;

    bigCirclePnts.push_back(MbCartPoint3D(0, 0, 0));
    bigCirclePnts.push_back(MbCartPoint3D(0, 0, 25));
    bigCirclePnts.push_back(MbCartPoint3D(assortmentCamera / 2 + 48, 0, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigCirclePnts, blockNames), bigCircle);

    SolidSPtr pipeCyl, pipeCyl2, pipeCyl3, pipeCyl4;
    SolidSPtr mergePipeCyl, mergePipeCyl2, mergePipeCyl3, mergePipeCyl4;

    SpacePointsVector pipeCylPnts;

    const double halfOfT = t / 2;
    const double radiusOfOuterTube = assortmentOuterTubes / 2;

    pipeCylPnts.push_back(MbCartPoint3D(halfOfT, halfOfT, 0));
    pipeCylPnts.push_back(MbCartPoint3D(halfOfT, halfOfT, 25));
    pipeCylPnts.push_back(MbCartPoint3D(halfOfT + radiusOfOuterTube, halfOfT, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, pipeCylPnts, blockNames), pipeCyl);

    SpacePointsVector pipeCylPnts2;

    pipeCylPnts2.push_back(MbCartPoint3D(halfOfT, -halfOfT, 0));
    pipeCylPnts2.push_back(MbCartPoint3D(halfOfT, -halfOfT, 25));
    pipeCylPnts2.push_back(MbCartPoint3D(halfOfT + radiusOfOuterTube, -halfOfT, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, pipeCylPnts2, blockNames), pipeCyl2);

    SpacePointsVector pipeCylPnts3;

    pipeCylPnts3.push_back(MbCartPoint3D(-halfOfT, -halfOfT, 0));
    pipeCylPnts3.push_back(MbCartPoint3D(-halfOfT, -halfOfT, 25));
    pipeCylPnts3.push_back(MbCartPoint3D(-(halfOfT + radiusOfOuterTube), -halfOfT, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, pipeCylPnts3, blockNames), pipeCyl3);

    SpacePointsVector pipeCylPnts4;

    pipeCylPnts4.push_back(MbCartPoint3D(-halfOfT, halfOfT, 0));
    pipeCylPnts4.push_back(MbCartPoint3D(-halfOfT, halfOfT, 25));
    pipeCylPnts4.push_back(MbCartPoint3D(-(halfOfT + radiusOfOuterTube), halfOfT, 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, pipeCylPnts4, blockNames), pipeCyl4);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    ::BooleanResult(bigCircle, cm_Copy, pipeCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergePipeCyl);

    ::BooleanResult(mergePipeCyl, cm_Copy, pipeCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergePipeCyl2);

    ::BooleanResult(mergePipeCyl2, cm_Copy, pipeCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergePipeCyl3);

    ::BooleanResult(mergePipeCyl3, cm_Copy, pipeCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), mergePipeCyl4);

    SolidSPtr boltCyl, boltCyl2, boltCyl3, boltCyl4, boltCyl5, boltCyl6, boltCyl7, boltCyl8;
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
    boltCylPnts.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle), (assortmentCamera / 2 + 30.5) * sin(boltAngle), 25));
    boltCylPnts.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), boltCyl);

    SpacePointsVector boltCylPnts2;

    boltCylPnts2.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle2), (assortmentCamera / 2 + 30.5) * sin(boltAngle2), 0));
    boltCylPnts2.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle2), (assortmentCamera / 2 + 30.5) * sin(boltAngle2), 25));
    boltCylPnts2.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle2) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle2), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts2, blockNames), boltCyl2);

    SpacePointsVector boltCylPnts3;

    boltCylPnts3.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle3), (assortmentCamera / 2 + 30.5) * sin(boltAngle3), 0));
    boltCylPnts3.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle3), (assortmentCamera / 2 + 30.5) * sin(boltAngle3), 25));
    boltCylPnts3.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle3) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle3), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts3, blockNames), boltCyl3);

    SpacePointsVector boltCylPnts4;

    boltCylPnts4.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle4), (assortmentCamera / 2 + 30.5) * sin(boltAngle4), 0));
    boltCylPnts4.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle4), (assortmentCamera / 2 + 30.5) * sin(boltAngle4), 25));
    boltCylPnts4.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle4) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle4), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts4, blockNames), boltCyl4);

    SpacePointsVector boltCylPnts5;

    boltCylPnts5.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle5), (assortmentCamera / 2 + 30.5) * sin(boltAngle5), 0));
    boltCylPnts5.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle5), (assortmentCamera / 2 + 30.5) * sin(boltAngle5), 25));
    boltCylPnts5.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle5) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle5), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts5, blockNames), boltCyl5);

    SpacePointsVector boltCylPnts6;

    boltCylPnts6.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle6), (assortmentCamera / 2 + 30.5) * sin(boltAngle6), 0));
    boltCylPnts6.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle6), (assortmentCamera / 2 + 30.5) * sin(boltAngle6), 25));
    boltCylPnts6.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle6) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle6), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts6, blockNames), boltCyl6);

    SpacePointsVector boltCylPnts7;

    boltCylPnts7.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle7), (assortmentCamera / 2 + 30.5) * sin(boltAngle7), 0));
    boltCylPnts7.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle7), (assortmentCamera / 2 + 30.5) * sin(boltAngle7), 25));
    boltCylPnts7.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle7) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle7), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts7, blockNames), boltCyl7);

    SpacePointsVector boltCylPnts8;

    boltCylPnts8.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle8), (assortmentCamera / 2 + 30.5) * sin(boltAngle8), 0));
    boltCylPnts8.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle8), (assortmentCamera / 2 + 30.5) * sin(boltAngle8), 25));
    boltCylPnts8.push_back(MbCartPoint3D((assortmentCamera / 2 + 30.5) * cos(boltAngle8) + 5, (assortmentCamera / 2 + 30.5) * sin(boltAngle8), 0));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts8, blockNames), boltCyl8);

    ::BooleanResult(mergePipeCyl4, cm_Copy, boltCyl, cm_Copy,
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
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pipeCap);


    return pipeCap;
}