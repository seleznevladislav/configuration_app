#include "../BuildMathModel.h"

using namespace BuildMathModel;

double thickness = -1;

const double DEG_TO_RAD = M_PI / 180.0;

SPtr<MbSolid> ParametricModelCreator::Zarubincreate_003_opora(double dV, double ktDiam, double ktThickness, double t, double visotaOpori, double shirinaOpori)
{
    // Параметры стандартные, то есть при 600
    double depth = 200;
    double high = visotaOpori;
    double width = 750;

    double minsa = 300;

    double backWallDepth = 10;
    double thikness = 10;

    double distance = 10;

    double diametr = 600;
    double radiusMal = (ktDiam + 2 * ktThickness) / 2;

    double diametrOtv = 28;
    double distanceP = 450;
    double distancePoint = (width - distanceP) / 2;

    if (dV >= 800.0) {
        depth = 250;
        width = 976;

        minsa = 400;

        backWallDepth = 10;
        thikness = 10;

        distance = 10;

        diametr = 800;
        
        diametrOtv = 28;
        distanceP = 500;
        distancePoint = (width - distanceP) / 2;
    }

    assemblyHeightTTOR = high; // Задаётся высота теплообменика, для дальнейшей организации элемента сборки в блок

    MbSNameMaker blockNames(1, MbSNameMaker::i_SideNone, 0);

    SolidSPtr pBlock, innerLeftBlock, innerCenterBlock, innerRightBlock;

    SpacePointsVector blockPnts;

    blockPnts.push_back(MbCartPoint3D(0, 0, 0));
    blockPnts.push_back(MbCartPoint3D(width, 0, 0));
    blockPnts.push_back(MbCartPoint3D(width, 0, depth));
    blockPnts.push_back(MbCartPoint3D(width, -high, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Block, blockPnts, blockNames), pBlock);

    SpacePointsVector innerLeftBlockPnts;

    innerLeftBlockPnts.push_back(MbCartPoint3D(thikness, -thikness, 0));
    innerLeftBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2, -thikness, 0));
    innerLeftBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2, -thikness, depth - backWallDepth));
    innerLeftBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2, -high + thikness, depth - backWallDepth));

    ::ElementarySolid(MbElementarySolidParams(et_Block, innerLeftBlockPnts, blockNames), innerLeftBlock);

    SpacePointsVector innerCenterBlockPnts;

    innerCenterBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2 + 10, -thikness, 0));
    innerCenterBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2 + minsa - thikness, -thikness, 0));
    innerCenterBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2 + minsa - thikness, -thikness, depth - backWallDepth));
    innerCenterBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2 + minsa - thikness, -high + thikness, depth - backWallDepth));

    ::ElementarySolid(MbElementarySolidParams(et_Block, innerCenterBlockPnts, blockNames), innerCenterBlock);

    SpacePointsVector innerRightBlockPnts;

    innerRightBlockPnts.push_back(MbCartPoint3D((width - minsa) / 2 + minsa, -thikness, 0));
    innerRightBlockPnts.push_back(MbCartPoint3D(width - thikness, -thikness, 0));
    innerRightBlockPnts.push_back(MbCartPoint3D(width - thikness, -thikness, depth - backWallDepth));
    innerRightBlockPnts.push_back(MbCartPoint3D(width - thikness, -high + thikness, depth - backWallDepth));

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

    //viewManager->AddObject(Style(1, LIGHTRED), pMergeRightBlock);

    SolidSPtr pMergeCyl, pMainCyl, pInnerCyl;

    SpacePointsVector mainCylPnts;

    mainCylPnts.push_back(MbCartPoint3D(width / 2, -high / 2, 0));
    mainCylPnts.push_back(MbCartPoint3D(width / 2, -high / 2, depth));
    mainCylPnts.push_back(MbCartPoint3D(width / 2 + diametr / 2, -high / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, mainCylPnts, blockNames), pMainCyl);

    ::BooleanResult(pMergeRightBlock, cm_Copy, pMainCyl, cm_Copy,
        MbBooleanOperationParams(bo_Union, flagsBool, operBoolNames), pMergeCyl);

    SpacePointsVector innerCylPnts;

    innerCylPnts.push_back(MbCartPoint3D(width / 2, -high / 2, 0));
    innerCylPnts.push_back(MbCartPoint3D(width / 2, -high / 2, depth / 2 - 10));
    innerCylPnts.push_back(MbCartPoint3D(width / 2 + (diametr - thikness) / 2, -high / 2, depth / 2 - 10));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, innerCylPnts, blockNames), pInnerCyl);

    MbPlacement3D XYPlane(MbCartPoint3D(0, 0, depth / 2),
        MbCartPoint3D(1, 0, depth / 2), MbCartPoint3D(0, 1, depth / 2));

    SolidSPtr pInnerMergeSolid, pSymmetryInnerMergeSolid, pSymmetryInnerCyl;

    ::SymmetrySolid(*pInnerCyl, cm_Copy, MbSymmetrySolidParams(XYPlane, blockNames, ts_neutral), pSymmetryInnerCyl);

    ::BooleanResult(pMergeCyl, cm_Copy, pInnerCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pInnerMergeSolid);

    ::BooleanResult(pInnerMergeSolid, cm_Copy, pSymmetryInnerCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pSymmetryInnerMergeSolid);

    SolidSPtr pSmallCyl, pSmallCyl2, pSmallCyl3, pSmallCyl4, pMergeSmallCyllBlock, pMergeSmallCyllBlock2, pMergeSmallCyllBlock3, pMergeSmallCyllBlock4;

    SpacePointsVector smallCylPnts;

    smallCylPnts.push_back(MbCartPoint3D(width / 2 - t / 2, -high / 2 - t / 2, 0));
    smallCylPnts.push_back(MbCartPoint3D(width / 2 - t / 2, -high / 2 - t / 2, depth));
    smallCylPnts.push_back(MbCartPoint3D(width / 2 - t / 2 + radiusMal, -high / 2 - t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts, blockNames), pSmallCyl);

    ::BooleanResult(pSymmetryInnerMergeSolid, cm_Copy, pSmallCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock);

    SpacePointsVector smallCylPnts2;

    smallCylPnts2.push_back(MbCartPoint3D(width / 2 + t / 2, -high / 2 - t / 2, 0));
    smallCylPnts2.push_back(MbCartPoint3D(width / 2 + t / 2, -high / 2 - t / 2, depth));
    smallCylPnts2.push_back(MbCartPoint3D(width / 2 + t / 2 + radiusMal, -high / 2 - t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts2, blockNames), pSmallCyl2);

    ::BooleanResult(pMergeSmallCyllBlock, cm_Copy, pSmallCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock2);

    SpacePointsVector smallCylPnts3;

    smallCylPnts3.push_back(MbCartPoint3D(width / 2 + t / 2, -high / 2 + t / 2, 0));
    smallCylPnts3.push_back(MbCartPoint3D(width / 2 + t / 2, -high / 2 + t / 2, depth));
    smallCylPnts3.push_back(MbCartPoint3D(width / 2 + t / 2 + radiusMal, -high / 2 + t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts3, blockNames), pSmallCyl3);

    ::BooleanResult(pMergeSmallCyllBlock2, cm_Copy, pSmallCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock3);

    SpacePointsVector smallCylPnts4;

    smallCylPnts4.push_back(MbCartPoint3D(width / 2 - t / 2, -high / 2 + t / 2, 0));
    smallCylPnts4.push_back(MbCartPoint3D(width / 2 - t / 2, -high / 2 + t / 2, depth));
    smallCylPnts4.push_back(MbCartPoint3D(width / 2 - t / 2 + radiusMal, -high / 2 + t / 2, depth));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, smallCylPnts4, blockNames), pSmallCyl4);

    ::BooleanResult(pMergeSmallCyllBlock3, cm_Copy, pSmallCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeSmallCyllBlock4);


    SolidSPtr pBoltCyl, pBoltCyl2, pBoltCyl3, pBoltCyl4, pMergeBoltCyllBlock, pMergeBoltCyllBlock2, pMergeBoltCyllBlock3, pMergeBoltCyllBlock4;

    SpacePointsVector boltCylPnts;

    boltCylPnts.push_back(MbCartPoint3D(distancePoint, -high, depth / 2));
    boltCylPnts.push_back(MbCartPoint3D(distancePoint, -high + thikness, depth / 2));
    boltCylPnts.push_back(MbCartPoint3D(distancePoint, -high + thikness, depth / 2 + diametrOtv / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts, blockNames), pBoltCyl);

    ::BooleanResult(pMergeSmallCyllBlock4, cm_Copy, pBoltCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock);

    SpacePointsVector boltCylPnts2;

    boltCylPnts2.push_back(MbCartPoint3D(distancePoint + distanceP, -high, depth / 2));
    boltCylPnts2.push_back(MbCartPoint3D(distancePoint + distanceP, -high + thikness, depth / 2));
    boltCylPnts2.push_back(MbCartPoint3D(distancePoint + distanceP, -high, depth / 2 + diametrOtv / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts2, blockNames), pBoltCyl2);

    ::BooleanResult(pMergeBoltCyllBlock, cm_Copy, pBoltCyl2, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock2);

    SpacePointsVector boltCylPnts3;

    boltCylPnts3.push_back(MbCartPoint3D(distancePoint + distanceP, 0, depth / 2));
    boltCylPnts3.push_back(MbCartPoint3D(distancePoint + distanceP, -thickness, depth / 2));
    boltCylPnts3.push_back(MbCartPoint3D(distancePoint + distanceP, 0, depth / 2 + diametrOtv / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts3, blockNames), pBoltCyl3);

    ::BooleanResult(pMergeBoltCyllBlock2, cm_Copy, pBoltCyl3, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock3);

    SpacePointsVector boltCylPnts4;

    boltCylPnts4.push_back(MbCartPoint3D(distancePoint, 0, depth / 2));
    boltCylPnts4.push_back(MbCartPoint3D(distancePoint, -thickness, depth / 2));
    boltCylPnts4.push_back(MbCartPoint3D(distancePoint, -0, depth / 2 + diametrOtv / 2));

    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, boltCylPnts4, blockNames), pBoltCyl4);

    ::BooleanResult(pMergeBoltCyllBlock3, cm_Copy, pBoltCyl4, cm_Copy,
        MbBooleanOperationParams(bo_Difference, flagsBool, operBoolNames), pMergeBoltCyllBlock4);

    return pMergeBoltCyllBlock2;
}