#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

SPtr<MbSolid> ParametricModelCreator::createSupport_003(double assortmentCamera, double assortmentOuterTubes, double t)
{
    std::unordered_map<int, std::vector<int>> valuesDictionary;

    // Создаем массивы чисел для каждого ключа
    std::vector<int> array219 = { 370, 150, 260, 50, 150 };
    std::vector<int> array273 = { 475, 150, 310, 75, 200 };
    std::vector<int> array325 = { 576, 180, 420, 150, 300 };

    valuesDictionary[219] = array219;
    valuesDictionary[273] = array273;
    valuesDictionary[325] = array325;

    const std::vector<int> currentValues = valuesDictionary[assortmentCamera];

    assemblyHeight = currentValues[0]; // Задаётся высота теплообменика, для дальнейшей организации элемента сборки в блок

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);

    SolidSPtr pBlock, innerLeftBlock, innerCenterBlock, innerRightBlock;

    SpacePointsVector blockPnts;

    blockPnts.push_back(MbCartPoint3D(0, 0, 0));
    blockPnts.push_back(MbCartPoint3D(currentValues[2], 0, 0));
    blockPnts.push_back(MbCartPoint3D(currentValues[2], 0, currentValues[1]));
    blockPnts.push_back(MbCartPoint3D(currentValues[2], -currentValues[0], currentValues[1]));

    ::ElementarySolid(MbElementarySolidParams(et_Block, blockPnts, blockNames), pBlock);

    SpacePointsVector innerLeftBlockPnts;

    innerLeftBlockPnts.push_back(MbCartPoint3D(10, -10, 0));
    innerLeftBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[3] / 2, -10, 0));
    innerLeftBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[3] / 2, -10, currentValues[1] - 10));
    innerLeftBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[3] / 2, -currentValues[0] + 10, currentValues[1] - 10));

    ::ElementarySolid(MbElementarySolidParams(et_Block, innerLeftBlockPnts, blockNames), innerLeftBlock);

    SpacePointsVector innerCenterBlockPnts;

    innerCenterBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[3] / 2 + 8, -10, 0));
    innerCenterBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[3] / 2 - 8, -10, 0));
    innerCenterBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[3] / 2 - 8, -10, currentValues[1] - 10));
    innerCenterBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[3] / 2 - 8, -currentValues[0] + 10, currentValues[1] - 10));

    ::ElementarySolid(MbElementarySolidParams(et_Block, innerCenterBlockPnts, blockNames), innerCenterBlock);

    SpacePointsVector innerRightBlockPnts;

    innerRightBlockPnts.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[3] / 2, -10, 0));
    innerRightBlockPnts.push_back(MbCartPoint3D(currentValues[2] - 10, -10, 0));
    innerRightBlockPnts.push_back(MbCartPoint3D(currentValues[2] - 10, -10, currentValues[1] - 10));
    innerRightBlockPnts.push_back(MbCartPoint3D(currentValues[2] - 10, -currentValues[0] + 10, currentValues[1] - 10));

    ::ElementarySolid(MbElementarySolidParams(et_Block, innerRightBlockPnts, blockNames), innerRightBlock);

    SolidSPtr pMergeLeftBlock, pMergeCenterBlock, pMergeRightBlock;

    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    MbBooleanFlags flagsBool;
    flagsBool.InitBoolean(true);
    flagsBool.SetMergingFaces(true);
    flagsBool.SetMergingEdges(true);


    ::BooleanResult(pBlock, cm_Copy, innerLeftBlock, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeLeftBlock);

    ::BooleanResult(pMergeLeftBlock, cm_Copy, innerCenterBlock, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeCenterBlock);

    ::BooleanResult(pMergeCenterBlock, cm_Copy, innerRightBlock, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeRightBlock);

    SolidSPtr pMergeCyl, pMainCyl, pInnerCyl;

    SpacePointsVector mainCylPnts;

    mainCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2, -(currentValues[0] / 2), 0));
    mainCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2, -(currentValues[0] / 2), currentValues[1]));
    mainCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2 + assortmentCamera / 2, -(currentValues[0] / 2), currentValues[1]));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, mainCylPnts, blockNames), pMainCyl);

    ::BooleanResult(pMergeRightBlock, cm_Copy, pMainCyl, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), pMergeCyl);

    SpacePointsVector innerCylPnts;

    innerCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2, -(currentValues[0] / 2), 0));
    innerCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2, -(currentValues[0] / 2), currentValues[1] / 2 - 5));
    innerCylPnts.push_back(MbCartPoint3D(
        currentValues[2] / 2 + (assortmentCamera - 8) / 2,  // кооридината на оси X
        -(currentValues[0] / 2),                            // кооридината на оси Y
        currentValues[1] / 2 - 5));                         // кооридината на оси Z

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, innerCylPnts, blockNames), pInnerCyl);

    MbPlacement3D XYPlane(MbCartPoint3D(0, 0, currentValues[1] / 2),
        MbCartPoint3D(1, 0, currentValues[1] / 2), MbCartPoint3D(0, 1, currentValues[1] / 2));

    SolidSPtr pInnerMergeSolid, pSymmetryInnerMergeSolid, pSymmetryInnerCyl;

    ::SymmetrySolid(*pInnerCyl, cm_Copy, MbSymmetrySolidParams(XYPlane, blockNames, ts_neutral), pSymmetryInnerCyl);

    ::BooleanResult(pMergeCyl, cm_Copy, pInnerCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pInnerMergeSolid);

    ::BooleanResult(pInnerMergeSolid, cm_Copy, pSymmetryInnerCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pSymmetryInnerMergeSolid);

    SolidSPtr pSmallCyl, pSmallCyl2, pSmallCyl3, pSmallCyl4, pMergeSmallCyllBlock, pMergeSmallCyllBlock2, pMergeSmallCyllBlock3, pMergeSmallCyllBlock4;

    SpacePointsVector smallCylPnts;

    smallCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - t / 2, -(currentValues[0] / 2 - t / 2), 0));
    smallCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - t / 2, -(currentValues[0] / 2 - t / 2), currentValues[1] / 2 + 5));
    smallCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - t / 2 + assortmentOuterTubes / 2, -(currentValues[0] / 2 - t / 2), currentValues[1] / 2 + 5));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts, blockNames), pSmallCyl);

    ::BooleanResult(pSymmetryInnerMergeSolid, cm_Copy, pSmallCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock);

    SpacePointsVector smallCylPnts2;

    smallCylPnts2.push_back(MbCartPoint3D(currentValues[2] / 2 + t / 2, -(currentValues[0] / 2 - t / 2), 0));
    smallCylPnts2.push_back(MbCartPoint3D(currentValues[2] / 2 + t / 2, -(currentValues[0] / 2 - t / 2), currentValues[1] / 2 + 5));
    smallCylPnts2.push_back(MbCartPoint3D(currentValues[2] / 2 + t / 2 + assortmentOuterTubes / 2, -(currentValues[0] / 2 - t / 2), currentValues[1] / 2 + 5));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts2, blockNames), pSmallCyl2);

    ::BooleanResult(pMergeSmallCyllBlock, cm_Copy, pSmallCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock2);

    SpacePointsVector smallCylPnts3;

    smallCylPnts3.push_back(MbCartPoint3D(currentValues[2] / 2 + t / 2, -(currentValues[0] / 2 + t / 2), 0));
    smallCylPnts3.push_back(MbCartPoint3D(currentValues[2] / 2 + t / 2, -(currentValues[0] / 2 + t / 2), currentValues[1] / 2 + 5));
    smallCylPnts3.push_back(MbCartPoint3D(currentValues[2] / 2 + t / 2 + assortmentOuterTubes / 2, -(currentValues[0] / 2 + t / 2), currentValues[1] / 2 + 5));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts3, blockNames), pSmallCyl3);

    ::BooleanResult(pMergeSmallCyllBlock2, cm_Copy, pSmallCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock3);

    SpacePointsVector smallCylPnts4;

    smallCylPnts4.push_back(MbCartPoint3D(currentValues[2] / 2 - t / 2, -(currentValues[0] / 2 + t / 2), 0));
    smallCylPnts4.push_back(MbCartPoint3D(currentValues[2] / 2 - t / 2, -(currentValues[0] / 2 + t / 2), currentValues[1] / 2 + 5));
    smallCylPnts4.push_back(MbCartPoint3D(currentValues[2] / 2 - t / 2 + assortmentOuterTubes / 2, -(currentValues[0] / 2 + t / 2), currentValues[1] / 2 + 5));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts4, blockNames), pSmallCyl4);

    ::BooleanResult(pMergeSmallCyllBlock3, cm_Copy, pSmallCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock4);

    SolidSPtr pBoltCyl, pBoltCyl2, pBoltCyl3, pBoltCyl4, pMergeBoltCyllBlock, pMergeBoltCyllBlock2, pMergeBoltCyllBlock3, pMergeBoltCyllBlock4;

    SpacePointsVector boltCylPnts;

    boltCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[4] / 2, -currentValues[0], currentValues[1] / 2));
    boltCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[4] / 2, -currentValues[0] + 10, currentValues[1] / 2));
    boltCylPnts.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[4] / 2, -currentValues[0], currentValues[1] / 2 + 28 / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), pBoltCyl);

    ::BooleanResult(pMergeSmallCyllBlock4, cm_Copy, pBoltCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock);

    SpacePointsVector boltCylPnts2;

    boltCylPnts2.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[4] / 2, -currentValues[0], currentValues[1] / 2));
    boltCylPnts2.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[4] / 2, -currentValues[0] + 10, currentValues[1] / 2));
    boltCylPnts2.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[4] / 2, -currentValues[0], currentValues[1] / 2 + 28 / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts2, blockNames), pBoltCyl2);

    ::BooleanResult(pMergeBoltCyllBlock, cm_Copy, pBoltCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock2);

    SpacePointsVector boltCylPnts3;

    boltCylPnts3.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[4] / 2, 0, currentValues[1] / 2));
    boltCylPnts3.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[4] / 2, -10, currentValues[1] / 2));
    boltCylPnts3.push_back(MbCartPoint3D(currentValues[2] / 2 + currentValues[4] / 2, 0, currentValues[1] / 2 + 28 / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts3, blockNames), pBoltCyl3);

    ::BooleanResult(pMergeBoltCyllBlock2, cm_Copy, pBoltCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock3);

    SpacePointsVector boltCylPnts4;

    boltCylPnts4.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[4] / 2, 0, currentValues[1] / 2));
    boltCylPnts4.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[4] / 2, -10, currentValues[1] / 2));
    boltCylPnts4.push_back(MbCartPoint3D(currentValues[2] / 2 - currentValues[4] / 2, -0, currentValues[1] / 2 + 28 / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts4, blockNames), pBoltCyl4);

    ::BooleanResult(pMergeBoltCyllBlock3, cm_Copy, pBoltCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock4);

    return pMergeBoltCyllBlock4;
}