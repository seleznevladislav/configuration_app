#include "../BuildMathModel.h"

using namespace BuildMathModel;

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_011_ConnectorWithFlanec(double ktDiam, double ktThickness, double t, double visotaOpori, double dU)
{
    const double inner = ktDiam + 2 * ktThickness;
    const double outer = (ktDiam + 10) + 2 * ktThickness;
    const double Length = 255;
    const double highFlanec = 40;
    const double high = visotaOpori - 2 * highFlanec;
    const double middle = t / 2;

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);

    MbSNameMaker filletNames(ct_FilletSolid, MbSNameMaker::i_SideNone, 0);
    SmoothValues params;
    params.distance1 = 10;
    params.distance2 = 20;
    params.form = st_Fillet;
    params.conic = 0;
    params.prolong = false;
    params.smoothCorner = SmoothValues::ec_uniform;
    params.keepCant = ts_negative;
    params.strict = true;

    // Outer нижний
    SpacePointsVector bigRadiusCylPnts;

    bigRadiusCylPnts.push_back(MbCartPoint3D(0, 0, Length / 2));
    bigRadiusCylPnts.push_back(MbCartPoint3D(0, 0, -Length / 2));
    bigRadiusCylPnts.push_back(MbCartPoint3D(outer / 2, 0, 0));

    SolidSPtr bigRadiusCyl;

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPnts, blockNames), bigRadiusCyl);

    // Outer верхний
    SpacePointsVector bigRadiusCylPntsSecond;

    bigRadiusCylPntsSecond.push_back(MbCartPoint3D(0, t, Length / 2));
    bigRadiusCylPntsSecond.push_back(MbCartPoint3D(0, t, -Length / 2));
    bigRadiusCylPntsSecond.push_back(MbCartPoint3D(outer / 2, t, 0));

    SolidSPtr bigRadiusCylSecond;

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPntsSecond, blockNames), bigRadiusCylSecond);

    // Outer Центральный
    SpacePointsVector bigRadiusCylPntsThird;

    bigRadiusCylPntsThird.push_back(MbCartPoint3D(0, middle + high / 2, 0));
    bigRadiusCylPntsThird.push_back(MbCartPoint3D(0, middle - high / 2, 0));
    bigRadiusCylPntsThird.push_back(MbCartPoint3D(dU / 2 + 10, middle + high / 2, 0));

    SolidSPtr bigRadiusCylThird;

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPntsThird, blockNames), bigRadiusCylThird);

    SolidSPtr resultOuterFirst;

    ::BooleanResult(bigRadiusCyl, cm_Copy, bigRadiusCylThird, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), resultOuterFirst);

    SolidSPtr resultOuterSecond;

    ::BooleanResult(resultOuterFirst, cm_Copy, bigRadiusCylSecond, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), resultOuterSecond);

    // Inner нижний
    SpacePointsVector bigRadiusCylPntsInner;

    bigRadiusCylPntsInner.push_back(MbCartPoint3D(0, 0, Length / 2));
    bigRadiusCylPntsInner.push_back(MbCartPoint3D(0, 0, -Length / 2));
    bigRadiusCylPntsInner.push_back(MbCartPoint3D(inner / 2, 0, 0));

    SolidSPtr bigRadiusCylInner;

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPntsInner, blockNames), bigRadiusCylInner);

    // Inner верхний
    SpacePointsVector bigRadiusCylPntsSecondInner;

    bigRadiusCylPntsSecondInner.push_back(MbCartPoint3D(0, t, Length / 2));
    bigRadiusCylPntsSecondInner.push_back(MbCartPoint3D(0, t, -Length / 2));
    bigRadiusCylPntsSecondInner.push_back(MbCartPoint3D(inner / 2, t, 0));

    SolidSPtr bigRadiusCylSecondInner;

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPntsSecondInner, blockNames), bigRadiusCylSecondInner);

    // Inner центральный
    SpacePointsVector bigRadiusCylPntsThirdInner;

    bigRadiusCylPntsThirdInner.push_back(MbCartPoint3D(0, middle + high / 2, 0));
    bigRadiusCylPntsThirdInner.push_back(MbCartPoint3D(0, middle - high / 2, 0));
    bigRadiusCylPntsThirdInner.push_back(MbCartPoint3D(dU / 2, middle + high / 2, 0));

    SolidSPtr bigRadiusCylThirdInner;

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, bigRadiusCylPntsThirdInner, blockNames), bigRadiusCylThirdInner);

    SolidSPtr resultOuterFirstInner;

    ::BooleanResult(bigRadiusCylInner, cm_Copy, bigRadiusCylThirdInner, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), resultOuterFirstInner);

    SolidSPtr resultOuterSecondInner;

    ::BooleanResult(resultOuterFirstInner, cm_Copy, bigRadiusCylSecondInner, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), resultOuterSecondInner);

    SolidSPtr resultBody;

    ::BooleanResult(resultOuterSecond, cm_Copy, resultOuterSecondInner, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), resultBody);

    // Скругление

    RPArray<MbCurveEdge> allEdges;
    resultBody->GetEdges(allEdges);

    c3d::EdgesSPtrVector initCurves;
    initCurves.clear();
    initCurves.emplace_back(allEdges[20]);
    initCurves.emplace_back(allEdges[23]);
    initCurves.emplace_back(allEdges[25]);
    initCurves.emplace_back(allEdges[26]);

    FacesSPtrVector initBounds;

    SolidSPtr filletBody;
    MbResultType res = ::FilletSolid(*(MbSolid*)resultBody, cm_Copy, MbShellFilletValues(initCurves, initBounds, params, filletNames), filletBody);

    return filletBody;
}