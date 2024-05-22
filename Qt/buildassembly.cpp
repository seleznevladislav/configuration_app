#include "BuildMathModel.h"
#include <mb_property.h>

using namespace BuildMathModel;

SPtr<MbAssembly> ParametricModelCreator::CreatePneumocylinderAssembly(ConfigParams params, int configurationQuantity)
{
    // Набор параметров для параметризации
    const int LENGTH = params.LENGTH;
    const int lengthK = params.lengthK;
    const double length0 = params.length0;
    const double length1 = params.length1;
    const double length2 = params.length2;
    const double length3 = params.length3;

    const double assortmentInnerTubes = params.assortmentInnerTubes;
    const double thicknessInnerTubes = params.thicknessInnerTubes;

    const double assortmentOuterTubes = params.assortmentOuterTubes;
    const double thicknessOuterTubes = params.thicknessOuterTubes;

    const double assortmentCamera = params.assortmentCamera;
    const double thicknessCamera = params.thicknessCamera;
    
    const double diametrY = params.diametrY;
    const double t = params.t;

    const double curvedLength = 72.5;

    double lengthBetweenInnerPipesAndGrid = LENGTH - length3 - lengthK - length2 - curvedLength;

    MbPlacement3D lcs;

    SPtr<MbSolid> capSolid(createCup_005(t, assortmentOuterTubes, assortmentCamera));
    capSolid->SetColor(132, 169, 140);
    SPtr<MbSolid> headExhangerGridSolid(createOuterPipesGrid_004(length2, diametrY, thicknessInnerTubes, t, assortmentInnerTubes, assortmentCamera, thicknessCamera));
    headExhangerGridSolid->SetColor(202, 210, 197);
    SPtr<MbSolid> headExhangerGridSecondSolid(createInnerPipesGrid_006(length3, assortmentCamera, thicknessCamera));
    headExhangerGridSecondSolid->SetColor(202, 210, 197);
    SPtr<MbSolid> outerPipeSolid(createOuterPipe_002(lengthK, assortmentOuterTubes, thicknessOuterTubes));
    outerPipeSolid->SetColor(202, 210, 197);
    SPtr<MbSolid> innerPipeSolid(createInnerPipe_001(assortmentInnerTubes, thicknessInnerTubes, lengthBetweenInnerPipesAndGrid + length2 + lengthK + 88 - curvedLength / 2));
    innerPipeSolid->SetColor(132, 169, 140);
    SPtr<MbSolid> supportSolid(createSupport_003(assortmentCamera, assortmentOuterTubes, t));
    supportSolid->SetColor(82, 121, 111);
    SPtr<MbSolid> curvedPipeSolid(createCurvedPipe_007(assortmentInnerTubes, thicknessInnerTubes, t));
    curvedPipeSolid->SetColor(132, 169, 140);
    SPtr<MbSolid> pipeHolderShort(createPipeHolder_010(20, assortmentInnerTubes));
    pipeHolderShort->SetColor(53, 79, 82);

    // GOST
    SPtr<MbSolid> flangeE(buildFlangeE(diametrY));
    flangeE->SetColor(202, 210, 197);
    SPtr<MbSolid> flangeF(buildFlangeF(diametrY));
    flangeF->SetColor(202, 210, 197);
    SPtr<MbSolid> screw35(buildFlangeScrew35());
    screw35->SetColor(53, 79, 82);
    SPtr<MbSolid> pipeScrewWasher(buildWasher10());
    pipeScrewWasher->SetColor(53, 79, 82);
    SPtr<MbSolid> screw55(buildScrew55());
    pipeScrewWasher->SetColor(53, 79, 82);


    // Крышки
    SPtr<MbInstance> capItem(new MbInstance(*capSolid, lcs));
    SPtr<MbInstance> capItem2(new MbInstance(*capSolid, lcs));
    capItem->SetItemName(123);

    // Решетки
    SPtr<MbInstance> headExhangerGridItem(new MbInstance(*headExhangerGridSolid, lcs));
    SPtr<MbInstance> headExhangerGridSecondItem(new MbInstance(*headExhangerGridSecondSolid, lcs));

    // Опоры
    SPtr<MbInstance> supportItem(new MbInstance(*supportSolid, lcs));
    SPtr<MbInstance> supportItem2(new MbInstance(*supportSolid, lcs));
    // Кожуховые трубы
    SPtr<MbInstance> outerPipeItem(new MbInstance(*outerPipeSolid, lcs));
    SPtr<MbInstance> outerPipeItem2(new MbInstance(*outerPipeSolid, lcs));
    SPtr<MbInstance> outerPipeItem3(new MbInstance(*outerPipeSolid, lcs));
    SPtr<MbInstance> outerPipeItem4(new MbInstance(*outerPipeSolid, lcs));
    // Теплообменные трубы
    SPtr<MbInstance> innerPipeItem(new MbInstance(*innerPipeSolid, lcs));
    SPtr<MbInstance> innerPipeItem2(new MbInstance(*innerPipeSolid, lcs));
    SPtr<MbInstance> innerPipeItem3(new MbInstance(*innerPipeSolid, lcs));
    SPtr<MbInstance> innerPipeItem4(new MbInstance(*innerPipeSolid, lcs));
    // Соединительная труба
    SPtr<MbInstance> curvedPipeItem(new MbInstance(*curvedPipeSolid, lcs));
    SPtr<MbInstance> curvedPipeItem2(new MbInstance(*curvedPipeSolid, lcs));
    SPtr<MbInstance> curvedPipeItem3(new MbInstance(*curvedPipeSolid, lcs));
    // Держатель
    SPtr<MbInstance> pipeHolderShortItem(new MbInstance(*pipeHolderShort, lcs));
    SPtr<MbInstance> pipeHolderShortItem2(new MbInstance(*pipeHolderShort, lcs));
    SPtr<MbInstance> pipeHolderShortItem3(new MbInstance(*pipeHolderShort, lcs));
    SPtr<MbInstance> pipeHolderShortItem4(new MbInstance(*pipeHolderShort, lcs));

    //GOST
    SPtr<MbInstance> flangeEItem(new MbInstance(*flangeE, lcs));
    SPtr<MbInstance> flangeEItem2(new MbInstance(*flangeE, lcs));
    SPtr<MbInstance> flangeFItem(new MbInstance(*flangeF, lcs));
    SPtr<MbInstance> flangeFItem2(new MbInstance(*flangeF, lcs));

    SPtr<MbInstance> screw35Item(new MbInstance(*screw35, lcs));
    SPtr<MbInstance> screw35Item2(new MbInstance(*screw35, lcs));
    SPtr<MbInstance> screw35Item3(new MbInstance(*screw35, lcs));
    SPtr<MbInstance> screw35Item4(new MbInstance(*screw35, lcs));

    SPtr<MbInstance> screw35Item5(new MbInstance(*screw35, lcs));
    SPtr<MbInstance> screw35Item6(new MbInstance(*screw35, lcs));
    SPtr<MbInstance> screw35Item7(new MbInstance(*screw35, lcs));
    SPtr<MbInstance> screw35Item8(new MbInstance(*screw35, lcs));

    SPtr<MbInstance> pipeScrewWasherItem(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem2(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem3(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem4(new MbInstance(*pipeScrewWasher, lcs));

    SPtr<MbInstance> pipeScrewWasherItem5(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem6(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem7(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem8(new MbInstance(*pipeScrewWasher, lcs));

    // Washer
    SPtr<MbInstance> pipeScrewWasherItem9(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem10(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem11(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem12(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem13(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem14(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem15(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem16(new MbInstance(*pipeScrewWasher, lcs));

    SPtr<MbInstance> pipeScrewWasherItem17(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem18(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem19(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem20(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem21(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem22(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem23(new MbInstance(*pipeScrewWasher, lcs));
    SPtr<MbInstance> pipeScrewWasherItem24(new MbInstance(*pipeScrewWasher, lcs));

    SPtr<MbInstance> screw55Item(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item2(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item3(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item4(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item5(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item6(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item7(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item8(new MbInstance(*screw55, lcs));

    SPtr<MbInstance> screw55Item9(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item10(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item11(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item12(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item13(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item14(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item15(new MbInstance(*screw55, lcs));
    SPtr<MbInstance> screw55Item16(new MbInstance(*screw55, lcs));


    std::vector<SPtr<MbItem>> pair;
    //Переменные для подсборки
    pair.push_back(capItem);
    pair.push_back(capItem2);
    pair.push_back(headExhangerGridItem);
    pair.push_back(headExhangerGridSecondItem);

    // outer pipes
    pair.push_back(outerPipeItem);
    pair.push_back(outerPipeItem2);
    pair.push_back(outerPipeItem3);
    pair.push_back(outerPipeItem4);

    // inner pipes
    pair.push_back(innerPipeItem);
    pair.push_back(innerPipeItem2);
    pair.push_back(innerPipeItem3);
    pair.push_back(innerPipeItem4);

    // curved pipes
    pair.push_back(curvedPipeItem);
    pair.push_back(curvedPipeItem2);
    pair.push_back(curvedPipeItem3);

    // holders
    pair.push_back(pipeHolderShortItem);
    pair.push_back(pipeHolderShortItem2);
    pair.push_back(pipeHolderShortItem3);
    pair.push_back(pipeHolderShortItem4);

    pair.push_back(supportItem);
    supportItem->SetColor(0, 0, 200);
    pair.push_back(supportItem2);
    supportItem2->SetColor(0, 0, 200);

    // GOST
    pair.push_back(flangeEItem);
    pair.push_back(flangeEItem2);
    pair.push_back(flangeFItem);
    pair.push_back(flangeFItem2);

    pair.push_back(screw35Item);
    pair.push_back(screw35Item2);
    pair.push_back(screw35Item3);
    pair.push_back(screw35Item4);

    pair.push_back(screw35Item5);
    pair.push_back(screw35Item6);
    pair.push_back(screw35Item7);
    pair.push_back(screw35Item8);

    pair.push_back(pipeScrewWasherItem);
    pair.push_back(pipeScrewWasherItem2);
    pair.push_back(pipeScrewWasherItem3);
    pair.push_back(pipeScrewWasherItem4);

    pair.push_back(pipeScrewWasherItem5);
    pair.push_back(pipeScrewWasherItem6);
    pair.push_back(pipeScrewWasherItem7);
    pair.push_back(pipeScrewWasherItem8);

    pair.push_back(pipeScrewWasherItem9);
    pair.push_back(pipeScrewWasherItem10);
    pair.push_back(pipeScrewWasherItem11);
    pair.push_back(pipeScrewWasherItem12);
    pair.push_back(pipeScrewWasherItem13);
    pair.push_back(pipeScrewWasherItem14);
    pair.push_back(pipeScrewWasherItem15);
    pair.push_back(pipeScrewWasherItem16);

    pair.push_back(pipeScrewWasherItem17);
    pair.push_back(pipeScrewWasherItem18);
    pair.push_back(pipeScrewWasherItem19);
    pair.push_back(pipeScrewWasherItem20);
    pair.push_back(pipeScrewWasherItem21);
    pair.push_back(pipeScrewWasherItem22);
    pair.push_back(pipeScrewWasherItem23);
    pair.push_back(pipeScrewWasherItem24);

    pair.push_back(screw55Item);
    pair.push_back(screw55Item2);
    pair.push_back(screw55Item3);
    pair.push_back(screw55Item4);
    pair.push_back(screw55Item5);
    pair.push_back(screw55Item6);
    pair.push_back(screw55Item7);
    pair.push_back(screw55Item8);

    pair.push_back(screw55Item9);
    pair.push_back(screw55Item10);
    pair.push_back(screw55Item11);
    pair.push_back(screw55Item12);
    pair.push_back(screw55Item13);
    pair.push_back(screw55Item14);
    pair.push_back(screw55Item15);
    pair.push_back(screw55Item16);

    MbAxis3D axxVert(MbVector3D(0, 1, 0));
    MbAxis3D ayyVert(MbVector3D(1, 0, 0));

    curvedPipeItem2->Rotate(axxVert, M_PI);
    curvedPipeItem3->Rotate(axxVert, M_PI);
    supportItem2->Rotate(axxVert, M_PI);

    flangeEItem->Rotate(ayyVert, -M_PI / 2);
    flangeFItem->Rotate(ayyVert, M_PI / 2);
    flangeEItem2->Rotate(ayyVert, M_PI / 2);
    flangeFItem2->Rotate(ayyVert, -M_PI / 2);

    screw35Item->Rotate(ayyVert, -M_PI / 2);
    screw35Item2->Rotate(ayyVert, -M_PI / 2);
    screw35Item3->Rotate(ayyVert, -M_PI / 2);
    screw35Item4->Rotate(ayyVert, -M_PI / 2);

    screw35Item5->Rotate(ayyVert, M_PI / 2);
    screw35Item6->Rotate(ayyVert, M_PI / 2);
    screw35Item7->Rotate(ayyVert, M_PI / 2);
    screw35Item8->Rotate(ayyVert, M_PI / 2);

    headExhangerGridSecondItem->Rotate(axxVert, M_PI);

    SPtr<MbAssembly> assm(new MbAssembly(pair));
    {
        // Constractions
        MtParVariant ArgLengthP0(-length0);

        MtGeomArgument GeomArgSupport11(supportSolid->GetFace(37), supportItem2);
        MtGeomArgument GeomArgSupport12(supportSolid->GetFace(33), supportItem);
        assm->AddConstraint(GCM_DISTANCE, GeomArgSupport11, GeomArgSupport12, ArgLengthP0);

        // Outer pipes concentric
        MtGeomArgument GeomArgPipeConc(outerPipeSolid->GetFace(3), outerPipeItem);
        MtGeomArgument GeomArgPipe2Conc(outerPipeSolid->GetFace(3), outerPipeItem2);
        MtGeomArgument GeomArgPipe3Conc(outerPipeSolid->GetFace(3), outerPipeItem3);
        MtGeomArgument GeomArgPipe4Conc(outerPipeSolid->GetFace(3), outerPipeItem4);

        MtGeomArgument GeomArgSupportConc(supportSolid->GetFace(40), supportItem);
        MtGeomArgument GeomArgSupport2Conc(supportSolid->GetFace(39), supportItem);
        MtGeomArgument GeomArgSupport3Conc(supportSolid->GetFace(38), supportItem);
        MtGeomArgument GeomArgSupport4Conc(supportSolid->GetFace(41), supportItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipeConc, GeomArgSupportConc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipe2Conc, GeomArgSupport2Conc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipe3Conc, GeomArgSupport3Conc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipe4Conc, GeomArgSupport4Conc);

        // Inner pipes concentric
        MtGeomArgument GeomArgPipe5Conc(innerPipeSolid->GetFace(2), innerPipeItem);
        MtGeomArgument GeomArgPipe6Conc(innerPipeSolid->GetFace(2), innerPipeItem2);
        MtGeomArgument GeomArgPipe7Conc(innerPipeSolid->GetFace(2), innerPipeItem3);
        MtGeomArgument GeomArgPipe8Conc(innerPipeSolid->GetFace(2), innerPipeItem4);

        MtGeomArgument GeomArgSupport5Conc(supportSolid->GetFace(40), supportItem);
        MtGeomArgument GeomArgSupport6Conc(supportSolid->GetFace(39), supportItem);
        MtGeomArgument GeomArgSupport7Conc(supportSolid->GetFace(38), supportItem);
        MtGeomArgument GeomArgSupport8Conc(supportSolid->GetFace(41), supportItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipe5Conc, GeomArgSupport5Conc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipe6Conc, GeomArgSupport6Conc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipe7Conc, GeomArgSupport7Conc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgPipe8Conc, GeomArgSupport8Conc);

        // Head Exhanger Grid 
        const int supportWallThickness = 10;
        double length1Dist = -(length1 - diametrY / 2 + supportWallThickness / 2);
        MtParVariant ArgLength1(length1Dist);

        MtGeomArgument Plane1(headExhangerGridSolid->GetFace(36), headExhangerGridItem);
        MtGeomArgument Plane2(supportSolid->GetFace(33), supportItem);
        assm->AddConstraint(GCM_DISTANCE, Plane1, Plane2, MtParVariant(ArgLength1));

        // Grid concentric
        MtGeomArgument GeomArgGridConc(headExhangerGridSolid->GetFace(31), headExhangerGridItem);
        MtGeomArgument GeomArgGrid2Conc(headExhangerGridSolid->GetFace(28), headExhangerGridItem);
        MtGeomArgument GeomArgGrid3Conc(supportSolid->GetFace(38), supportItem);
        MtGeomArgument GeomArgGrid4Conc(supportSolid->GetFace(40), supportItem);

        MtGeomArgument GeomArgSupportGridConc(supportSolid->GetFace(40), supportItem2);
        MtGeomArgument GeomArgSupportGrid2Conc(supportSolid->GetFace(39), supportItem2);
        MtGeomArgument GeomArgSupportGrid3Conc(supportSolid->GetFace(41), supportItem2);
        MtGeomArgument GeomArgSupportGrid4Conc(supportSolid->GetFace(39), supportItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeomArgGridConc, GeomArgSupportGridConc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgGrid2Conc, GeomArgSupportGrid2Conc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgGrid3Conc, GeomArgSupportGrid3Conc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgGrid4Conc, GeomArgSupportGrid4Conc);

        // Outer Pipe Coincident
        MtGeomArgument GeomArgPipeCoinc(outerPipeSolid->GetFace(0), outerPipeItem);
        MtGeomArgument GeomArgPipe2Coinc(outerPipeSolid->GetFace(0), outerPipeItem2);
        MtGeomArgument GeomArgPipe3Coinc(outerPipeSolid->GetFace(0), outerPipeItem3);
        MtGeomArgument GeomArgPipe4Coinc(outerPipeSolid->GetFace(0), outerPipeItem4);

        MtGeomArgument GeomArgGridCoinc(headExhangerGridSolid->GetFace(0), headExhangerGridItem);
        MtGeomArgument GeomArgGrid2Coinc(headExhangerGridSolid->GetFace(0), headExhangerGridItem);
        MtGeomArgument GeomArgGrid3Coinc(headExhangerGridSolid->GetFace(0), headExhangerGridItem);
        MtGeomArgument GeomArgGrid4Coinc(headExhangerGridSolid->GetFace(0), headExhangerGridItem);

        assm->AddConstraint(GCM_COINCIDENT, GeomArgPipeCoinc, GeomArgGridCoinc, MtParVariant(GCM_OPPOSITE));
        assm->AddConstraint(GCM_COINCIDENT, GeomArgPipe2Coinc, GeomArgGrid2Coinc, MtParVariant(GCM_OPPOSITE));
        assm->AddConstraint(GCM_COINCIDENT, GeomArgPipe3Coinc, GeomArgGrid3Coinc, MtParVariant(GCM_OPPOSITE));
        assm->AddConstraint(GCM_COINCIDENT, GeomArgPipe4Coinc, GeomArgGrid4Coinc, MtParVariant(GCM_OPPOSITE));

        // Outer Pipe Distanse
        MtParVariant ArgLengthBetweenPipesAndGrid(lengthBetweenInnerPipesAndGrid);

        MtGeomArgument PlaneGrid(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument PlaneGrid2(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument PlaneGrid3(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument PlaneGrid4(headExhangerGridSolid->GetFace(37), headExhangerGridItem);

        MtGeomArgument PlaneInnerPipe(innerPipeSolid->GetFace(0), innerPipeItem);
        MtGeomArgument PlaneInnerPipe2(innerPipeSolid->GetFace(0), innerPipeItem2);
        MtGeomArgument PlaneInnerPipe3(innerPipeSolid->GetFace(0), innerPipeItem3);
        MtGeomArgument PlaneInnerPipe4(innerPipeSolid->GetFace(0), innerPipeItem4);

        assm->AddConstraint(GCM_DISTANCE, PlaneGrid, PlaneInnerPipe, MtParVariant(ArgLengthBetweenPipesAndGrid));
        assm->AddConstraint(GCM_DISTANCE, PlaneGrid2, PlaneInnerPipe2, MtParVariant(ArgLengthBetweenPipesAndGrid));
        assm->AddConstraint(GCM_DISTANCE, PlaneGrid3, PlaneInnerPipe3, MtParVariant(ArgLengthBetweenPipesAndGrid));
        assm->AddConstraint(GCM_DISTANCE, PlaneGrid4, PlaneInnerPipe4, MtParVariant(ArgLengthBetweenPipesAndGrid));

        // Cap Dependencies
        MtGeomArgument GeomArgGridCapCoinc(headExhangerGridSolid->GetFace(0), headExhangerGridItem);
        MtGeomArgument GeomArgGridCapCoinc2(capSolid->GetFace(1), capItem);

        MtGeomArgument GeomArgGridCapConc(headExhangerGridSolid->GetFace(16), headExhangerGridItem);
        MtGeomArgument GeomArgGridCapConc2(capSolid->GetFace(12), capItem);

        assm->AddConstraint(GCM_COINCIDENT, GeomArgGridCapCoinc, GeomArgGridCapCoinc2);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgGridCapConc, GeomArgGridCapConc2);

        // Round Grid Dependencies
        MtGeomArgument GeomArgRoundGridConc(headExhangerGridSecondSolid->GetFace(13), headExhangerGridSecondItem);
        MtGeomArgument GeomArgRoundGridConc2(headExhangerGridSecondSolid->GetFace(4), headExhangerGridSecondItem);
        MtGeomArgument GeomArgRoundGridConc3(headExhangerGridSecondSolid->GetFace(0), headExhangerGridSecondItem);

        MtGeomArgument GeomArgRoundGridAndCapConc(headExhangerGridSolid->GetFace(2), headExhangerGridItem);
        MtGeomArgument GeomArgRoundGridAndCapConc2(headExhangerGridSolid->GetFace(18), headExhangerGridItem);
        MtGeomArgument GeomArgRoundGridAndCapConc3(headExhangerGridSolid->GetFace(37), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeomArgRoundGridConc, GeomArgRoundGridAndCapConc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgRoundGridConc2, GeomArgRoundGridAndCapConc2);

        double lengthBetweenGrids = -(lengthK + length2);
        MtParVariant ArgLengthBetweenGrids(lengthBetweenGrids);
        assm->AddConstraint(GCM_DISTANCE, GeomArgRoundGridAndCapConc3, GeomArgRoundGridConc3, MtParVariant(ArgLengthBetweenGrids));

        // Caps Concentric
        MtGeomArgument GeometryArgGridAndCapConc(headExhangerGridSolid->GetFace(17), headExhangerGridItem);
        MtGeomArgument GeometryArgGridAndCapConc2(headExhangerGridSolid->GetFace(5), headExhangerGridItem);
        MtGeomArgument GeometryArgGridAndCapConc3(capSolid->GetFace(13), capItem);
        MtGeomArgument GeometryArgGridAndCapConc4(capSolid->GetFace(5), capItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgGridAndCapConc, GeometryArgGridAndCapConc3);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgGridAndCapConc2, GeometryArgGridAndCapConc4);

        // Cap2 Coinc 
        MtGeomArgument GeometryArgCap2Conc(headExhangerGridSecondSolid->GetFace(0), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgCap2Conc2(capSolid->GetFace(0), capItem2);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgCap2Conc, GeometryArgCap2Conc2);

        MtGeomArgument GeometryArgGrid2AndCap2Conc(capSolid->GetFace(7), capItem);
        MtGeomArgument GeometryArgGrid2AndCap2Conc2(capSolid->GetFace(10), capItem);
        MtGeomArgument GeometryArgGrid2AndCap2Conc3(capSolid->GetFace(7), capItem2);
        MtGeomArgument GeometryArgGrid2AndCap2Conc4(capSolid->GetFace(10), capItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgGrid2AndCap2Conc, GeometryArgGrid2AndCap2Conc3);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgGrid2AndCap2Conc2, GeometryArgGrid2AndCap2Conc4);

        // Curved Pipe
        MtGeomArgument GeometryArgCurvedPipeCoinc(curvedPipeSolid->GetFace(0), curvedPipeItem);
        MtGeomArgument GeometryArgCurvedPipeCoinc2(innerPipeSolid->GetFace(0), innerPipeItem);
        MtGeomArgument GeometryArgCurvedPipeConc(curvedPipeSolid->GetFace(3), curvedPipeItem);
        MtGeomArgument GeometryArgCurvedPipeConc2(innerPipeSolid->GetFace(2), innerPipeItem);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgCurvedPipeCoinc, GeometryArgCurvedPipeCoinc2);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgCurvedPipeConc, GeometryArgCurvedPipeConc2);

        MtGeomArgument GeometryArgCurvedPipeCoinc3(curvedPipeSolid->GetFace(1), curvedPipeItem);
        MtGeomArgument GeometryArgCurvedPipeCoinc4(innerPipeSolid->GetFace(0), innerPipeItem3);
        MtGeomArgument GeometryArgCurvedPipeConc3(curvedPipeSolid->GetFace(2), curvedPipeItem);
        MtGeomArgument GeometryArgCurvedPipeConc4(innerPipeSolid->GetFace(2), innerPipeItem3);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgCurvedPipeCoinc3, GeometryArgCurvedPipeCoinc4);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgCurvedPipeConc3, GeometryArgCurvedPipeConc4);

        // Curved Pipe grid
        MtGeomArgument GeometryArgCurvedPipeCoinc5(curvedPipeSolid->GetFace(0), curvedPipeItem2);
        MtGeomArgument GeometryArgCurvedPipeCoinc6(innerPipeSolid->GetFace(1), innerPipeItem);
        MtGeomArgument GeometryArgCurvedPipeConc5(curvedPipeSolid->GetFace(3), curvedPipeItem2);
        MtGeomArgument GeometryArgCurvedPipeConc6(innerPipeSolid->GetFace(2), innerPipeItem);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgCurvedPipeCoinc5, GeometryArgCurvedPipeCoinc6);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgCurvedPipeConc5, GeometryArgCurvedPipeConc6);

        MtGeomArgument GeometryArgCurvedPipeCoinc7(curvedPipeSolid->GetFace(1), curvedPipeItem2);
        MtGeomArgument GeometryArgCurvedPipeCoinc8(innerPipeSolid->GetFace(1), innerPipeItem2);
        MtGeomArgument GeometryArgCurvedPipeConc7(curvedPipeSolid->GetFace(2), curvedPipeItem2);
        MtGeomArgument GeometryArgCurvedPipeConc8(innerPipeSolid->GetFace(2), innerPipeItem2);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgCurvedPipeCoinc7, GeometryArgCurvedPipeCoinc8);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgCurvedPipeConc7, GeometryArgCurvedPipeConc8);

        // Second Curved Pipe grid
        MtGeomArgument GeometryArgCurvedPipeCoinc9(curvedPipeSolid->GetFace(0), curvedPipeItem3);
        MtGeomArgument GeometryArgCurvedPipeCoinc10(innerPipeSolid->GetFace(1), innerPipeItem3);
        MtGeomArgument GeometryArgCurvedPipeConc9(curvedPipeSolid->GetFace(3), curvedPipeItem3);
        MtGeomArgument GeometryArgCurvedPipeConc10(innerPipeSolid->GetFace(2), innerPipeItem3);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgCurvedPipeCoinc9, GeometryArgCurvedPipeCoinc10);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgCurvedPipeConc9, GeometryArgCurvedPipeConc10);

        MtGeomArgument GeometryArgCurvedPipeCoinc11(curvedPipeSolid->GetFace(1), curvedPipeItem3);
        MtGeomArgument GeometryArgCurvedPipeCoinc12(innerPipeSolid->GetFace(1), innerPipeItem4);
        MtGeomArgument GeometryArgCurvedPipeConc11(curvedPipeSolid->GetFace(2), curvedPipeItem3);
        MtGeomArgument GeometryArgCurvedPipeConc12(innerPipeSolid->GetFace(2), innerPipeItem4);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgCurvedPipeCoinc11, GeometryArgCurvedPipeCoinc12);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgCurvedPipeConc11, GeometryArgCurvedPipeConc12);

        // Pipe holders
        MtGeomArgument GeometryArgPipeHolderCoinc(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument GeometryArgPipeHolderCoinc2(pipeHolderShort->GetFace(0), pipeHolderShortItem);
        MtGeomArgument GeometryArgPipeHolderConc(innerPipeSolid->GetFace(3), innerPipeItem);
        MtGeomArgument GeometryArgPipeHolderConc2(pipeHolderShort->GetFace(10), pipeHolderShortItem);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgPipeHolderCoinc, GeometryArgPipeHolderCoinc2);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgPipeHolderConc, GeometryArgPipeHolderConc2);

        MtGeomArgument GeometryArgPipeHolderCoinc3(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument GeometryArgPipeHolderCoinc4(pipeHolderShort->GetFace(0), pipeHolderShortItem2);
        MtGeomArgument GeometryArgPipeHolderConc3(innerPipeSolid->GetFace(3), innerPipeItem2);
        MtGeomArgument GeometryArgPipeHolderConc4(pipeHolderShort->GetFace(10), pipeHolderShortItem2);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgPipeHolderCoinc3, GeometryArgPipeHolderCoinc4);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgPipeHolderConc3, GeometryArgPipeHolderConc4);

        MtGeomArgument GeometryArgPipeHolderCoinc5(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument GeometryArgPipeHolderCoinc6(pipeHolderShort->GetFace(0), pipeHolderShortItem3);
        MtGeomArgument GeometryArgPipeHolderConc5(innerPipeSolid->GetFace(3), innerPipeItem3);
        MtGeomArgument GeometryArgPipeHolderConc6(pipeHolderShort->GetFace(10), pipeHolderShortItem3);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgPipeHolderCoinc5, GeometryArgPipeHolderCoinc6);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgPipeHolderConc5, GeometryArgPipeHolderConc6);

        MtGeomArgument GeometryArgPipeHolderCoinc7(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument GeometryArgPipeHolderCoinc8(pipeHolderShort->GetFace(0), pipeHolderShortItem4);
        MtGeomArgument GeometryArgPipeHolderConc7(innerPipeSolid->GetFace(3), innerPipeItem4);
        MtGeomArgument GeometryArgPipeHolderConc8(pipeHolderShort->GetFace(10), pipeHolderShortItem4);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgPipeHolderCoinc7, GeometryArgPipeHolderCoinc8);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgPipeHolderConc7, GeometryArgPipeHolderConc8);

        // GOST
        // Flange E
        MtGeomArgument GeometryArgFlanecEConc(flangeE->GetFace(10), flangeEItem);
        MtGeomArgument GeometryArgFlanecECoinc(flangeE->GetFace(9), flangeEItem);
        MtGeomArgument GeometryArgFlanecEConc2(headExhangerGridSolid->GetFace(23), headExhangerGridItem);
        MtGeomArgument GeometryArgFlanecECoinc2(headExhangerGridSolid->GetFace(30), headExhangerGridItem);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgFlanecECoinc, GeometryArgFlanecECoinc2);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgFlanecEConc, GeometryArgFlanecEConc2);

        MtGeomArgument GeometryArgFlanecEConc3(flangeE->GetFace(10), flangeEItem2);
        MtGeomArgument GeometryArgFlanecECoinc3(flangeE->GetFace(9), flangeEItem2);
        MtGeomArgument GeometryArgFlanecEConc4(headExhangerGridSolid->GetFace(26), headExhangerGridItem);
        MtGeomArgument GeometryArgFlanecECoinc4(headExhangerGridSolid->GetFace(33), headExhangerGridItem);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgFlanecECoinc3, GeometryArgFlanecECoinc4);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgFlanecEConc3, GeometryArgFlanecEConc4);

        // Flange F
        MtGeomArgument GeometryArgFlanecFConc(flangeF->GetFace(10), flangeFItem);
        MtGeomArgument GeometryArgFlanecFDist(flangeF->GetFace(9), flangeFItem);
        MtGeomArgument GeometryArgFlanecFConc2(headExhangerGridSolid->GetFace(26), headExhangerGridItem);
        MtGeomArgument GeometryArgFlanecFDist2(flangeE->GetFace(0), flangeEItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgFlanecFConc, GeometryArgFlanecFConc2);

        double lengthBetweenFlanges = 1;
        MtParVariant ArgLengthBetweenFlanges(lengthBetweenFlanges);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgFlanecFDist, GeometryArgFlanecFDist2, MtParVariant(ArgLengthBetweenFlanges));

        MtGeomArgument GeometryArgFlangesConc(flangeE->GetFace(7), flangeEItem);
        MtGeomArgument GeometryArgFlangesConc2(flangeF->GetFace(2), flangeFItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgFlangesConc, GeometryArgFlangesConc2);

        MtGeomArgument GeometryArgFlanecFConc3(flangeF->GetFace(10), flangeFItem2);
        MtGeomArgument GeometryArgFlanecFDist3(flangeF->GetFace(9), flangeFItem2);
        MtGeomArgument GeometryArgFlanecFConc4(headExhangerGridSolid->GetFace(23), headExhangerGridItem);
        MtGeomArgument GeometryArgFlanecFDist4(flangeE->GetFace(0), flangeEItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgFlanecFConc3, GeometryArgFlanecFConc4);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgFlanecFDist3, GeometryArgFlanecFDist4, MtParVariant(ArgLengthBetweenFlanges));

        MtGeomArgument GeometryArgFlangesConc3(flangeE->GetFace(7), flangeEItem2);
        MtGeomArgument GeometryArgFlangesConc4(flangeF->GetFace(2), flangeFItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgFlangesConc3, GeometryArgFlangesConc4);

        // Screw 35
        double lengthForScrew = 2;
        MtParVariant ArgLengthScrew(lengthForScrew);

        MtGeomArgument GeometryArgScrew35Conc(screw35->GetFace(4), screw35Item);
        MtGeomArgument GeometryArgScrew35Dist(flangeF->GetFace(1), flangeFItem);
        MtGeomArgument GeometryArgScrew35Conc2(flangeF->GetFace(2), flangeFItem);
        MtGeomArgument GeometryArgScrew35Dist2(screw35->GetFace(9), screw35Item);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc, GeometryArgScrew35Conc2);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist, GeometryArgScrew35Dist2, MtParVariant(ArgLengthScrew));

        MtGeomArgument GeometryArgScrew35Conc3(screw35->GetFace(4), screw35Item2);
        MtGeomArgument GeometryArgScrew35Dist3(flangeF->GetFace(1), flangeFItem);
        MtGeomArgument GeometryArgScrew35Conc4(flangeF->GetFace(5), flangeFItem);
        MtGeomArgument GeometryArgScrew35Dist4(screw35->GetFace(9), screw35Item2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc3, GeometryArgScrew35Conc4);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist3, GeometryArgScrew35Dist4, MtParVariant(ArgLengthScrew));

        MtGeomArgument GeometryArgScrew35Conc5(screw35->GetFace(4), screw35Item3);
        MtGeomArgument GeometryArgScrew35Dist5(flangeF->GetFace(1), flangeFItem);
        MtGeomArgument GeometryArgScrew35Conc6(flangeF->GetFace(6), flangeFItem);
        MtGeomArgument GeometryArgScrew35Dist6(screw35->GetFace(9), screw35Item3);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc5, GeometryArgScrew35Conc6);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist5, GeometryArgScrew35Dist6, MtParVariant(ArgLengthScrew));

        MtGeomArgument GeometryArgScrew35Conc7(screw35->GetFace(4), screw35Item4);
        MtGeomArgument GeometryArgScrew35Dist7(flangeF->GetFace(1), flangeFItem);
        MtGeomArgument GeometryArgScrew35Conc8(flangeF->GetFace(7), flangeFItem);
        MtGeomArgument GeometryArgScrew35Dist8(screw35->GetFace(9), screw35Item4);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc7, GeometryArgScrew35Conc8);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist7, GeometryArgScrew35Dist8, MtParVariant(ArgLengthScrew));

        MtGeomArgument GeometryArgScrew35Conc9(screw35->GetFace(4), screw35Item5);
        MtGeomArgument GeometryArgScrew35Dist9(flangeF->GetFace(1), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Conc10(flangeF->GetFace(2), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Dist10(screw35->GetFace(9), screw35Item5);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc9, GeometryArgScrew35Conc10);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist9, GeometryArgScrew35Dist10, MtParVariant(ArgLengthScrew));

        MtGeomArgument GeometryArgScrew35Conc11(screw35->GetFace(4), screw35Item6);
        MtGeomArgument GeometryArgScrew35Dist11(flangeF->GetFace(1), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Conc12(flangeF->GetFace(5), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Dist12(screw35->GetFace(9), screw35Item6);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc11, GeometryArgScrew35Conc12);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist11, GeometryArgScrew35Dist12, MtParVariant(ArgLengthScrew));

        MtGeomArgument GeometryArgScrew35Conc13(screw35->GetFace(4), screw35Item7);
        MtGeomArgument GeometryArgScrew35Dist13(flangeF->GetFace(1), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Conc14(flangeF->GetFace(6), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Dist14(screw35->GetFace(9), screw35Item7);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc13, GeometryArgScrew35Conc14);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist13, GeometryArgScrew35Dist14, MtParVariant(ArgLengthScrew));

        MtGeomArgument GeometryArgScrew35Conc15(screw35->GetFace(4), screw35Item8);
        MtGeomArgument GeometryArgScrew35Dist15(flangeF->GetFace(1), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Conc16(flangeF->GetFace(7), flangeFItem2);
        MtGeomArgument GeometryArgScrew35Dist16(screw35->GetFace(9), screw35Item8);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgScrew35Conc15, GeometryArgScrew35Conc16);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgScrew35Dist15, GeometryArgScrew35Dist16, MtParVariant(ArgLengthScrew));

        // Washer 10
        MtGeomArgument GeometryArgWasher10Conc(pipeScrewWasher->GetFace(12), pipeScrewWasherItem);
        MtGeomArgument GeometryArgWasger10Coinc(pipeScrewWasher->GetFace(0), pipeScrewWasherItem);
        MtGeomArgument GeometryArgWasher10Conc2(screw35->GetFace(4), screw35Item);
        MtGeomArgument GeometryArgWasher10Coinc2(flangeE->GetFace(1), flangeEItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc, GeometryArgWasher10Conc2);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc, GeometryArgWasher10Coinc2);

        MtGeomArgument GeometryArgWasher10Conc3(pipeScrewWasher->GetFace(12), pipeScrewWasherItem2);
        MtGeomArgument GeometryArgWasger10Coinc3(pipeScrewWasher->GetFace(0), pipeScrewWasherItem2);
        MtGeomArgument GeometryArgWasher10Conc4(screw35->GetFace(4), screw35Item2);
        MtGeomArgument GeometryArgWasher10Coinc4(flangeE->GetFace(1), flangeEItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc3, GeometryArgWasher10Conc4);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc3, GeometryArgWasher10Coinc4);

        MtGeomArgument GeometryArgWasher10Conc5(pipeScrewWasher->GetFace(12), pipeScrewWasherItem3);
        MtGeomArgument GeometryArgWasger10Coinc5(pipeScrewWasher->GetFace(0), pipeScrewWasherItem3);
        MtGeomArgument GeometryArgWasher10Conc6(screw35->GetFace(4), screw35Item3);
        MtGeomArgument GeometryArgWasher10Coinc6(flangeE->GetFace(1), flangeEItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc5, GeometryArgWasher10Conc6);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc5, GeometryArgWasher10Coinc6);

        MtGeomArgument GeometryArgWasher10Conc7(pipeScrewWasher->GetFace(12), pipeScrewWasherItem4);
        MtGeomArgument GeometryArgWasger10Coinc7(pipeScrewWasher->GetFace(0), pipeScrewWasherItem4);
        MtGeomArgument GeometryArgWasher10Conc8(screw35->GetFace(4), screw35Item4);
        MtGeomArgument GeometryArgWasher10Coinc8(flangeE->GetFace(1), flangeEItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc7, GeometryArgWasher10Conc8);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc7, GeometryArgWasher10Coinc8);

        MtGeomArgument GeometryArgWasher10Conc9(pipeScrewWasher->GetFace(12), pipeScrewWasherItem5);
        MtGeomArgument GeometryArgWasger10Coinc9(pipeScrewWasher->GetFace(0), pipeScrewWasherItem5);
        MtGeomArgument GeometryArgWasher10Conc10(screw35->GetFace(4), screw35Item5);
        MtGeomArgument GeometryArgWasher10Coinc10(flangeE->GetFace(1), flangeEItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc9, GeometryArgWasher10Conc10);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc9, GeometryArgWasher10Coinc10);

        MtGeomArgument GeometryArgWasher10Conc11(pipeScrewWasher->GetFace(12), pipeScrewWasherItem6);
        MtGeomArgument GeometryArgWasger10Coinc11(pipeScrewWasher->GetFace(0), pipeScrewWasherItem6);
        MtGeomArgument GeometryArgWasher10Conc12(screw35->GetFace(4), screw35Item6);
        MtGeomArgument GeometryArgWasher10Coinc12(flangeE->GetFace(1), flangeEItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc11, GeometryArgWasher10Conc12);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc11, GeometryArgWasher10Coinc12);

        MtGeomArgument GeometryArgWasher10Conc13(pipeScrewWasher->GetFace(12), pipeScrewWasherItem7);
        MtGeomArgument GeometryArgWasger10Coinc13(pipeScrewWasher->GetFace(0), pipeScrewWasherItem7);
        MtGeomArgument GeometryArgWasher10Conc14(screw35->GetFace(4), screw35Item7);
        MtGeomArgument GeometryArgWasher10Coinc14(flangeE->GetFace(1), flangeEItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc13, GeometryArgWasher10Conc14);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc13, GeometryArgWasher10Coinc14);

        MtGeomArgument GeometryArgWasher10Conc15(pipeScrewWasher->GetFace(12), pipeScrewWasherItem8);
        MtGeomArgument GeometryArgWasger10Coinc15(pipeScrewWasher->GetFace(0), pipeScrewWasherItem8);
        MtGeomArgument GeometryArgWasher10Conc16(screw35->GetFace(4), screw35Item8);
        MtGeomArgument GeometryArgWasher10Coinc16(flangeE->GetFace(1), flangeEItem2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgWasher10Conc15, GeometryArgWasher10Conc16);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgWasger10Coinc15, GeometryArgWasher10Coinc16);

        // Screw 55
        MtGeomArgument GeometryArgsScrew55Conc(screw55->GetFace(18), screw55Item);
        MtGeomArgument GeometryArgsScrew55Coinc(screw55->GetFace(19), screw55Item);
        MtGeomArgument GeometryArgsScrew55Conc2(headExhangerGridSolid->GetFace(6), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc2(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc, GeometryArgsScrew55Conc2);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc, GeometryArgsScrew55Coinc2);

        MtGeomArgument GeometryArgsScrew55Conc3(screw55->GetFace(18), screw55Item2);
        MtGeomArgument GeometryArgsScrew55Coinc3(screw55->GetFace(19), screw55Item2);
        MtGeomArgument GeometryArgsScrew55Conc4(headExhangerGridSolid->GetFace(15), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc4(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc3, GeometryArgsScrew55Conc4);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc3, GeometryArgsScrew55Coinc4);

        MtGeomArgument GeometryArgsScrew55Conc5(screw55->GetFace(18), screw55Item3);
        MtGeomArgument GeometryArgsScrew55Coinc5(screw55->GetFace(19), screw55Item3);
        MtGeomArgument GeometryArgsScrew55Conc6(headExhangerGridSolid->GetFace(2), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc6(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc5, GeometryArgsScrew55Conc6);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc5, GeometryArgsScrew55Coinc6);

        MtGeomArgument GeometryArgsScrew55Conc7(screw55->GetFace(18), screw55Item4);
        MtGeomArgument GeometryArgsScrew55Coinc7(screw55->GetFace(19), screw55Item4);
        MtGeomArgument GeometryArgsScrew55Conc8(headExhangerGridSolid->GetFace(18), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc8(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc7, GeometryArgsScrew55Conc8);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc7, GeometryArgsScrew55Coinc8);

        MtGeomArgument GeometryArgsScrew55Conc9(screw55->GetFace(18), screw55Item5);
        MtGeomArgument GeometryArgsScrew55Coinc9(screw55->GetFace(19), screw55Item5);
        MtGeomArgument GeometryArgsScrew55Conc10(headExhangerGridSolid->GetFace(4), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc10(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc9, GeometryArgsScrew55Conc10);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc9, GeometryArgsScrew55Coinc10);

        MtGeomArgument GeometryArgsScrew55Conc11(screw55->GetFace(18), screw55Item6);
        MtGeomArgument GeometryArgsScrew55Coinc11(screw55->GetFace(19), screw55Item6);
        MtGeomArgument GeometryArgsScrew55Conc12(headExhangerGridSolid->GetFace(17), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc12(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc11, GeometryArgsScrew55Conc12);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc11, GeometryArgsScrew55Coinc12);

        MtGeomArgument GeometryArgsScrew55Conc13(screw55->GetFace(18), screw55Item7);
        MtGeomArgument GeometryArgsScrew55Coinc13(screw55->GetFace(19), screw55Item7);
        MtGeomArgument GeometryArgsScrew55Conc14(headExhangerGridSolid->GetFace(5), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc14(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc13, GeometryArgsScrew55Conc14);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc13, GeometryArgsScrew55Coinc14);

        MtGeomArgument GeometryArgsScrew55Conc15(screw55->GetFace(18), screw55Item8);
        MtGeomArgument GeometryArgsScrew55Coinc15(screw55->GetFace(19), screw55Item8);
        MtGeomArgument GeometryArgsScrew55Conc16(headExhangerGridSolid->GetFace(16), headExhangerGridItem);
        MtGeomArgument GeometryArgsScrew55Coinc16(headExhangerGridSolid->GetFace(1), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc15, GeometryArgsScrew55Conc16);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc15, GeometryArgsScrew55Coinc16);

        MtGeomArgument GeometryArgsScrew55Conc17(screw55->GetFace(18), screw55Item9);
        MtGeomArgument GeometryArgsScrew55Coinc17(screw55->GetFace(19), screw55Item9);
        MtGeomArgument GeometryArgsScrew55Conc18(headExhangerGridSecondSolid->GetFace(12), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc18(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc17, GeometryArgsScrew55Conc18, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc17, GeometryArgsScrew55Coinc18, GCM_OPPOSITE);

        MtGeomArgument GeometryArgsScrew55Conc19(screw55->GetFace(18), screw55Item10);
        MtGeomArgument GeometryArgsScrew55Coinc19(screw55->GetFace(19), screw55Item10);
        MtGeomArgument GeometryArgsScrew55Conc20(headExhangerGridSecondSolid->GetFace(5), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc20(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc19, GeometryArgsScrew55Conc20, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc19, GeometryArgsScrew55Coinc20, GCM_OPPOSITE);

        MtGeomArgument GeometryArgsScrew55Conc21(screw55->GetFace(18), screw55Item11);
        MtGeomArgument GeometryArgsScrew55Coinc21(screw55->GetFace(19), screw55Item11);
        MtGeomArgument GeometryArgsScrew55Conc22(headExhangerGridSecondSolid->GetFace(13), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc22(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc21, GeometryArgsScrew55Conc22, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc21, GeometryArgsScrew55Coinc22, GCM_OPPOSITE);

        MtGeomArgument GeometryArgsScrew55Conc23(screw55->GetFace(18), screw55Item12);
        MtGeomArgument GeometryArgsScrew55Coinc23(screw55->GetFace(19), screw55Item12);
        MtGeomArgument GeometryArgsScrew55Conc24(headExhangerGridSecondSolid->GetFace(4), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc24(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc23, GeometryArgsScrew55Conc24, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc23, GeometryArgsScrew55Coinc24, GCM_OPPOSITE);

        MtGeomArgument GeometryArgsScrew55Conc25(screw55->GetFace(18), screw55Item13);
        MtGeomArgument GeometryArgsScrew55Coinc25(screw55->GetFace(19), screw55Item13);
        MtGeomArgument GeometryArgsScrew55Conc26(headExhangerGridSecondSolid->GetFace(14), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc26(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc25, GeometryArgsScrew55Conc26, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc25, GeometryArgsScrew55Coinc26, GCM_OPPOSITE);

        MtGeomArgument GeometryArgsScrew55Conc27(screw55->GetFace(18), screw55Item14);
        MtGeomArgument GeometryArgsScrew55Coinc27(screw55->GetFace(19), screw55Item14);
        MtGeomArgument GeometryArgsScrew55Conc28(headExhangerGridSecondSolid->GetFace(2), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc28(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc27, GeometryArgsScrew55Conc28, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc27, GeometryArgsScrew55Coinc28, GCM_OPPOSITE);

        MtGeomArgument GeometryArgsScrew55Conc29(screw55->GetFace(18), screw55Item15);
        MtGeomArgument GeometryArgsScrew55Coinc29(screw55->GetFace(19), screw55Item15);
        MtGeomArgument GeometryArgsScrew55Conc30(headExhangerGridSecondSolid->GetFace(11), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc30(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc29, GeometryArgsScrew55Conc30, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc29, GeometryArgsScrew55Coinc30, GCM_OPPOSITE);

        MtGeomArgument GeometryArgsScrew55Conc31(screw55->GetFace(18), screw55Item16);
        MtGeomArgument GeometryArgsScrew55Coinc31(screw55->GetFace(19), screw55Item16);
        MtGeomArgument GeometryArgsScrew55Conc32(headExhangerGridSecondSolid->GetFace(6), headExhangerGridSecondItem);
        MtGeomArgument GeometryArgsScrew55Coinc32(headExhangerGridSecondSolid->GetFace(1), headExhangerGridSecondItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsScrew55Conc31, GeometryArgsScrew55Conc32, GCM_OPPOSITE);
        assm->AddConstraint(GCM_COINCIDENT, GeometryArgsScrew55Coinc31, GeometryArgsScrew55Coinc32, GCM_OPPOSITE);

        double lengthForWasher = 3.0;
        MtParVariant ArgLengthWasher(lengthForWasher);

        MtGeomArgument GeometryArgsWasherConc(pipeScrewWasher->GetFace(12), pipeScrewWasherItem9);
        MtGeomArgument GeometryArgsWasherDist(pipeScrewWasher->GetFace(0), pipeScrewWasherItem9);
        MtGeomArgument GeometryArgsWasherConc2(screw55->GetFace(18), screw55Item9);
        MtGeomArgument GeometryArgsWasherDist2(screw55->GetFace(0), screw55Item9);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc, GeometryArgsWasherConc2);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist, GeometryArgsWasherDist2, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc3(pipeScrewWasher->GetFace(12), pipeScrewWasherItem10);
        MtGeomArgument GeometryArgsWasherDist3(pipeScrewWasher->GetFace(0), pipeScrewWasherItem10);
        MtGeomArgument GeometryArgsWasherConc4(screw55->GetFace(18), screw55Item10);
        MtGeomArgument GeometryArgsWasherDist4(screw55->GetFace(0), screw55Item10);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc3, GeometryArgsWasherConc4);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist3, GeometryArgsWasherDist4, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc5(pipeScrewWasher->GetFace(12), pipeScrewWasherItem11);
        MtGeomArgument GeometryArgsWasherDist5(pipeScrewWasher->GetFace(0), pipeScrewWasherItem11);
        MtGeomArgument GeometryArgsWasherConc6(screw55->GetFace(18), screw55Item11);
        MtGeomArgument GeometryArgsWasherDist6(screw55->GetFace(0), screw55Item11);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc5, GeometryArgsWasherConc6);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist5, GeometryArgsWasherDist6, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc7(pipeScrewWasher->GetFace(12), pipeScrewWasherItem12);
        MtGeomArgument GeometryArgsWasherDist7(pipeScrewWasher->GetFace(0), pipeScrewWasherItem12);
        MtGeomArgument GeometryArgsWasherConc8(screw55->GetFace(18), screw55Item12);
        MtGeomArgument GeometryArgsWasherDist8(screw55->GetFace(0), screw55Item12);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc7, GeometryArgsWasherConc8);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist7, GeometryArgsWasherDist8, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc9(pipeScrewWasher->GetFace(12), pipeScrewWasherItem13);
        MtGeomArgument GeometryArgsWasherDist9(pipeScrewWasher->GetFace(0), pipeScrewWasherItem13);
        MtGeomArgument GeometryArgsWasherConc10(screw55->GetFace(18), screw55Item13);
        MtGeomArgument GeometryArgsWasherDist10(screw55->GetFace(0), screw55Item13);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc9, GeometryArgsWasherConc10);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist9, GeometryArgsWasherDist10, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc11(pipeScrewWasher->GetFace(12), pipeScrewWasherItem14);
        MtGeomArgument GeometryArgsWasherDist11(pipeScrewWasher->GetFace(0), pipeScrewWasherItem14);
        MtGeomArgument GeometryArgsWasherConc12(screw55->GetFace(18), screw55Item14);
        MtGeomArgument GeometryArgsWasherDist12(screw55->GetFace(0), screw55Item14);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc11, GeometryArgsWasherConc12);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist11, GeometryArgsWasherDist12, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc13(pipeScrewWasher->GetFace(12), pipeScrewWasherItem15);
        MtGeomArgument GeometryArgsWasherDist13(pipeScrewWasher->GetFace(0), pipeScrewWasherItem15);
        MtGeomArgument GeometryArgsWasherConc14(screw55->GetFace(18), screw55Item15);
        MtGeomArgument GeometryArgsWasherDist14(screw55->GetFace(0), screw55Item15);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc13, GeometryArgsWasherConc14);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist13, GeometryArgsWasherDist14, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc15(pipeScrewWasher->GetFace(12), pipeScrewWasherItem16);
        MtGeomArgument GeometryArgsWasherDist15(pipeScrewWasher->GetFace(0), pipeScrewWasherItem16);
        MtGeomArgument GeometryArgsWasherConc16(screw55->GetFace(18), screw55Item16);
        MtGeomArgument GeometryArgsWasherDist16(screw55->GetFace(0), screw55Item16);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc15, GeometryArgsWasherConc16);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist15, GeometryArgsWasherDist16, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc17(pipeScrewWasher->GetFace(12), pipeScrewWasherItem17);
        MtGeomArgument GeometryArgsWasherDist17(pipeScrewWasher->GetFace(0), pipeScrewWasherItem17);
        MtGeomArgument GeometryArgsWasherConc18(screw55->GetFace(18), screw55Item);
        MtGeomArgument GeometryArgsWasherDist18(screw55->GetFace(0), screw55Item);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc17, GeometryArgsWasherConc18);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist17, GeometryArgsWasherDist18, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc19(pipeScrewWasher->GetFace(12), pipeScrewWasherItem18);
        MtGeomArgument GeometryArgsWasherDist19(pipeScrewWasher->GetFace(0), pipeScrewWasherItem18);
        MtGeomArgument GeometryArgsWasherConc20(screw55->GetFace(18), screw55Item2);
        MtGeomArgument GeometryArgsWasherDist20(screw55->GetFace(0), screw55Item2);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc19, GeometryArgsWasherConc20);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist19, GeometryArgsWasherDist20, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc21(pipeScrewWasher->GetFace(12), pipeScrewWasherItem19);
        MtGeomArgument GeometryArgsWasherDist21(pipeScrewWasher->GetFace(0), pipeScrewWasherItem19);
        MtGeomArgument GeometryArgsWasherConc22(screw55->GetFace(18), screw55Item3);
        MtGeomArgument GeometryArgsWasherDist22(screw55->GetFace(0), screw55Item3);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc21, GeometryArgsWasherConc22);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist21, GeometryArgsWasherDist22, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc23(pipeScrewWasher->GetFace(12), pipeScrewWasherItem20);
        MtGeomArgument GeometryArgsWasherDist23(pipeScrewWasher->GetFace(0), pipeScrewWasherItem20);
        MtGeomArgument GeometryArgsWasherConc24(screw55->GetFace(18), screw55Item4);
        MtGeomArgument GeometryArgsWasherDist24(screw55->GetFace(0), screw55Item4);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc23, GeometryArgsWasherConc24);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist23, GeometryArgsWasherDist24, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc25(pipeScrewWasher->GetFace(12), pipeScrewWasherItem21);
        MtGeomArgument GeometryArgsWasherDist25(pipeScrewWasher->GetFace(0), pipeScrewWasherItem21);
        MtGeomArgument GeometryArgsWasherConc26(screw55->GetFace(18), screw55Item5);
        MtGeomArgument GeometryArgsWasherDist26(screw55->GetFace(0), screw55Item5);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc25, GeometryArgsWasherConc26);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist25, GeometryArgsWasherDist26, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc27(pipeScrewWasher->GetFace(12), pipeScrewWasherItem22);
        MtGeomArgument GeometryArgsWasherDist27(pipeScrewWasher->GetFace(0), pipeScrewWasherItem22);
        MtGeomArgument GeometryArgsWasherConc28(screw55->GetFace(18), screw55Item6);
        MtGeomArgument GeometryArgsWasherDist28(screw55->GetFace(0), screw55Item6);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc27, GeometryArgsWasherConc28);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist27, GeometryArgsWasherDist28, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc29(pipeScrewWasher->GetFace(12), pipeScrewWasherItem23);
        MtGeomArgument GeometryArgsWasherDist29(pipeScrewWasher->GetFace(0), pipeScrewWasherItem23);
        MtGeomArgument GeometryArgsWasherConc30(screw55->GetFace(18), screw55Item7);
        MtGeomArgument GeometryArgsWasherDist30(screw55->GetFace(0), screw55Item7);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc29, GeometryArgsWasherConc30);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist29, GeometryArgsWasherDist30, MtParVariant(ArgLengthWasher));

        MtGeomArgument GeometryArgsWasherConc31(pipeScrewWasher->GetFace(12), pipeScrewWasherItem24);
        MtGeomArgument GeometryArgsWasherDist31(pipeScrewWasher->GetFace(0), pipeScrewWasherItem24);
        MtGeomArgument GeometryArgsWasherConc32(screw55->GetFace(18), screw55Item8);
        MtGeomArgument GeometryArgsWasherDist32(screw55->GetFace(0), screw55Item8);

        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgsWasherConc31, GeometryArgsWasherConc32);
        assm->AddConstraint(GCM_DISTANCE, GeometryArgsWasherDist31, GeometryArgsWasherDist32, MtParVariant(ArgLengthWasher));
    }

    assm->EvaluateConstraints();

    string casingPipeName = "1";
    string headExchangePipeId = "0";
    string distributionChamberName = "2";
    string rotatableChamberName = "3";
    string supportName = "4";
    string capName = "5";
    string roundPipeName = "1";
    string flangeEName = "6";
    string flangeFName = "7";

    MbProductInfo casingPipeInfo(false, casingPipeName, casingPipeName, casingPipeName);
    MbProductInfo headExchangePipeInfo(false, headExchangePipeId, headExchangePipeId, headExchangePipeId);
    MbProductInfo distributionChamberInfo(false, distributionChamberName, distributionChamberName, distributionChamberName);
    MbProductInfo rotatableChamberInfo(false, rotatableChamberName, rotatableChamberName, rotatableChamberName);
    MbProductInfo supportInfo(false, supportName, supportName, supportName);
    MbProductInfo capInfo(false, capName, capName, capName);
    MbProductInfo roundedPipeInfo(false, roundPipeName, roundPipeName, roundPipeName);
    MbProductInfo flangeEInfo(false, flangeEName, flangeEName, flangeEName);
    MbProductInfo flangeFInfo(false, flangeFName, flangeFName, flangeFName);

    MbProductInfo TTRMInfo(false, "TTRM", "TTRM", "TTRM");

    innerPipeItem->AddAttribute(casingPipeInfo);
    innerPipeItem2->AddAttribute(casingPipeInfo);
    innerPipeItem3->AddAttribute(casingPipeInfo);
    innerPipeItem4->AddAttribute(casingPipeInfo);
    innerPipeSolid->AddAttribute(casingPipeInfo);

    outerPipeItem->AddAttribute(headExchangePipeInfo);
    outerPipeItem2->AddAttribute(headExchangePipeInfo);
    outerPipeItem3->AddAttribute(headExchangePipeInfo);
    outerPipeItem4->AddAttribute(headExchangePipeInfo);
    outerPipeSolid->AddAttribute(casingPipeInfo);

    headExhangerGridItem->AddAttribute(distributionChamberInfo);
    headExhangerGridSolid->AddAttribute(distributionChamberInfo);

    headExhangerGridSecondItem->AddAttribute(rotatableChamberInfo);
    headExhangerGridSecondSolid->AddAttribute(rotatableChamberInfo);

    supportItem->AddAttribute(supportInfo);
    supportItem2->AddAttribute(supportInfo);
    supportSolid->AddAttribute(supportInfo);

    capItem->AddAttribute(capInfo);
    capItem2->AddAttribute(capInfo);
    capSolid->AddAttribute(capInfo);

    curvedPipeItem->AddAttribute(roundedPipeInfo);
    curvedPipeItem2->AddAttribute(roundedPipeInfo);
    curvedPipeItem3->AddAttribute(roundedPipeInfo);
    curvedPipeSolid->AddAttribute(roundedPipeInfo);

    flangeE->AddAttribute(flangeEInfo);
    flangeEItem->AddAttribute(flangeEInfo);
    flangeEItem->AddAttribute(flangeEInfo);
   
    flangeF->AddAttribute(flangeFInfo);
    flangeFItem->AddAttribute(flangeFInfo);
    flangeFItem->AddAttribute(flangeFInfo);

    assm->AddAttribute(TTRMInfo);

    if (configurationQuantity > 0)
    {
        std::vector<SPtr<MbItem>> assmPairs;

        for (int i = 0; i < configurationQuantity + 1; ++i)
        {
            SPtr<MbInstance> assmInstance(new MbInstance(*assm, lcs));

            assmPairs.push_back(assmInstance);

            if (i >= 1)
            {
                assmInstance->Move(MbVector3D(0, assemblyHeight * i, 0));
            }
        }

        SPtr<MbAssembly> assmBlock(new MbAssembly(assmPairs));

        return assmBlock;
    }

    return assm;
}

SPtr<MbAssembly> ParametricModelCreator::CreateTTOR(BuildParamsForHeatExchangerTTOR params, int configurationQuantity)
{
#pragma region PARAMS
    double ttDiam = params.ttDiam;
    double ttThickness = params.ttThickness;
    double ktDiam = params.ktDiam;
    double ktThickness = params.ktThickness;
    double lK = params.lK;
    double L = params.L;
    double dV = params.dV;
    double dU = params.dU;
    double H = params.H;
    double H1 = params.H1;
    double l0 = params.l0;
    double l1 = params.l1;
    double l2 = params.l2;
    double l3 = params.l3;
    double t = params.t;
    double turnOnStandart = params.turnOnStandart;
    double simpleMode = params.simpleMode;

    const double visotaOpori = H1*2;
    const double shirinaOpori = 200;
    double distanceRezhetka = 500; //l2 - 400
    double distanceTubesKozhux = (-1) * (distanceRezhetka + 90.0);
    double distanceTubesTeploobmen = -660.0;
#pragma endregion

    MbPlacement3D lcs;

    // Item_008_001->Move(MbVector3D (3000,20,20));
    MbAxis3D axVert(MbVector3D(1, 0, 0));
    MbAxis3D ayVert(MbVector3D(0, 1, 0));
    MbAxis3D azVert(MbVector3D(0, 0, 1));

    SPtr<MbSolid> Detail_001(Zarubincreate_001_tubeTeploobmen(lK, ttDiam, ttThickness));
    Detail_001->SetColor(235, 172, 165);
    SPtr<MbSolid> Detail_002(Zarubincreate_002_tubeKozhux(lK, ktDiam, ktThickness));
    Detail_002->SetColor(165, 175, 235);
    SPtr<MbSolid> Detail_003(Zarubincreate_003_opora(dV, ktDiam, ktThickness, t, visotaOpori, shirinaOpori));
    Detail_003->SetColor(80, 84, 84);
    SPtr<MbSolid> Detail_004(Zarubincreate_004_reshetkaKozhux(ktDiam, ktThickness, t));
    Detail_004->SetColor(147, 218, 136);
    SPtr<MbSolid> Detail_005(Zarubincreate_005_kamera(ktDiam, ktThickness, l3));
    Detail_005->SetColor(101, 150, 94);
    SPtr<MbSolid> Detail_006(Zarubincreate_006_RezhetkaTeplTube(ttDiam, ttThickness));
    Detail_006->SetColor(218, 145, 85);
    SPtr<MbSolid> Detail_007(Zarubincreate_007_FlanecKozhux(ktDiam, ktThickness));
    Detail_007->SetColor(47, 218, 56);

    SPtr<MbSolid> Detail_008(Zarubincreate_008_FlanecSpecial(ttDiam, ttThickness));
    Detail_008->SetColor(83, 123, 151);
    SPtr<MbSolid> Detail_009(Zarubincreate_009_curevedTube(ttDiam, ttThickness, t));
    Detail_009->SetColor(71, 91, 71);
    SPtr<MbSolid> Detail_010(Zarubincreate_010_Connector(ktDiam, ktThickness, t, dU));

    SPtr<MbSolid> Detail_011(Zarubincreate_011_ConnectorWithFlanec(ktDiam, ktThickness, t, visotaOpori, dU));

    // Bolts
    SPtr<MbSolid> BoltM8(BoltGostTTOR(12));
    BoltM8->SetColor(151, 148, 139);

    SPtr<MbSolid> BoltM10(BoltGostTTOR(10));
    BoltM10->SetColor(151, 148, 139);
    
    SPtr<MbSolid> BoltM12(BoltGostTTOR(12));
    BoltM12->SetColor(151, 148, 139);

    SPtr<MbSolid> BoltM16(BoltGostTTOR(16));
    BoltM16->SetColor(151, 148, 139);

    // Washers
    SPtr<MbSolid> WasherM8(WasherGostTTOR(12));
    WasherM8->SetColor(151, 148, 139);
    
    SPtr<MbSolid> WasherM10(WasherGostTTOR(10));
    WasherM10->SetColor(151, 148, 139);
    
    SPtr<MbSolid> WasherM12(WasherGostTTOR(12));
    WasherM12->SetColor(151, 148, 139);

    SPtr<MbSolid> WasherM16(WasherGostTTOR(16));
    WasherM16->SetColor(151, 148, 139);

    // Гайки
    SPtr<MbSolid> NutBoltM8(NutBoltGostTTOR(12));
    NutBoltM8->SetColor(151, 148, 139);
    
    SPtr<MbSolid> NutBoltM10(NutBoltGostTTOR(10));
    NutBoltM10->SetColor(151, 148, 139);
    
    SPtr<MbSolid> NutBoltM12(NutBoltGostTTOR(12));
    NutBoltM12->SetColor(151, 148, 139);
    
    SPtr<MbSolid> NutBoltM16(NutBoltGostTTOR(16));
    NutBoltM16->SetColor(151, 148, 139);
    
    std::vector<SPtr<MbItem>> pair;

    SPtr<MbInstance> Item_001_001(new MbInstance(*Detail_001, lcs));
    SPtr<MbInstance> Item_001_002(new MbInstance(*Detail_001, lcs));
    SPtr<MbInstance> Item_001_003(new MbInstance(*Detail_001, lcs));
    SPtr<MbInstance> Item_001_004(new MbInstance(*Detail_001, lcs));

    SPtr<MbInstance> Item_002_001(new MbInstance(*Detail_002, lcs));
    SPtr<MbInstance> Item_002_002(new MbInstance(*Detail_002, lcs));
    SPtr<MbInstance> Item_002_003(new MbInstance(*Detail_002, lcs));
    SPtr<MbInstance> Item_002_004(new MbInstance(*Detail_002, lcs));

    SPtr<MbInstance> Item_003_001(new MbInstance(*Detail_003, lcs));
    SPtr<MbInstance> Item_003_002(new MbInstance(*Detail_003, lcs));

    SPtr<MbInstance> Item_004_001(new MbInstance(*Detail_004, lcs));

    SPtr<MbInstance> Item_005_001(new MbInstance(*Detail_005, lcs));

    SPtr<MbInstance> Item_006_001(new MbInstance(*Detail_006, lcs));
    SPtr<MbInstance> Item_006_002(new MbInstance(*Detail_006, lcs));
    SPtr<MbInstance> Item_006_003(new MbInstance(*Detail_006, lcs));
    SPtr<MbInstance> Item_006_004(new MbInstance(*Detail_006, lcs));

    SPtr<MbInstance> Item_007_001(new MbInstance(*Detail_007, lcs));
    SPtr<MbInstance> Item_007_002(new MbInstance(*Detail_007, lcs));
    SPtr<MbInstance> Item_007_003(new MbInstance(*Detail_007, lcs));
    SPtr<MbInstance> Item_007_004(new MbInstance(*Detail_007, lcs));

    // Фланцы специальнае для камеры
    SPtr<MbInstance> Item_008_001(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_002(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_003(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_004(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_005(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_006(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_007(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_008(new MbInstance(*Detail_008, lcs));

    // Фланцы специальнае для внешних
    SPtr<MbInstance> Item_008_009(new MbInstance(*Detail_008, lcs));// Check
    SPtr<MbInstance> Item_008_010(new MbInstance(*Detail_008, lcs));// Check
    SPtr<MbInstance> Item_008_011(new MbInstance(*Detail_008, lcs));// Check
    SPtr<MbInstance> Item_008_012(new MbInstance(*Detail_008, lcs));// Check
    SPtr<MbInstance> Item_008_013(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_014(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_015(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_016(new MbInstance(*Detail_008, lcs));

    SPtr<MbInstance> Item_008_017(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_018(new MbInstance(*Detail_008, lcs));

    SPtr<MbInstance> Item_009_001(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_002(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_003(new MbInstance(*Detail_009, lcs));

    SPtr<MbInstance> Item_010_001(new MbInstance(*Detail_010, lcs));
    SPtr<MbInstance> Item_011_001(new MbInstance(*Detail_011, lcs));


#pragma region PUSH_BACK
    //Переменные для подсборки
    pair.push_back(Item_001_001);
    pair.push_back(Item_001_002);
    pair.push_back(Item_001_003);
    pair.push_back(Item_001_004);

    pair.push_back(Item_002_001);
    pair.push_back(Item_002_002);
    pair.push_back(Item_002_003);
    pair.push_back(Item_002_004);

    pair.push_back(Item_003_001);
    pair.push_back(Item_003_002);

    pair.push_back(Item_004_001);

    pair.push_back(Item_005_001);

    pair.push_back(Item_006_001);
    pair.push_back(Item_006_002);
    pair.push_back(Item_006_003);
    pair.push_back(Item_006_004);

    pair.push_back(Item_007_001);
    pair.push_back(Item_007_002);
    pair.push_back(Item_007_003);
    pair.push_back(Item_007_004);

    pair.push_back(Item_008_001);
    pair.push_back(Item_008_002);
    pair.push_back(Item_008_003);
    pair.push_back(Item_008_004);
    pair.push_back(Item_008_005);
    pair.push_back(Item_008_006);
    pair.push_back(Item_008_007);
    pair.push_back(Item_008_008);

    pair.push_back(Item_008_009);
    pair.push_back(Item_008_010);
    pair.push_back(Item_008_011);
    pair.push_back(Item_008_012);
    pair.push_back(Item_008_013);
    pair.push_back(Item_008_014);
    pair.push_back(Item_008_015);
    pair.push_back(Item_008_016);

    pair.push_back(Item_008_017);
    pair.push_back(Item_008_018);

    pair.push_back(Item_009_001);
    pair.push_back(Item_009_002);
    pair.push_back(Item_009_003);

    pair.push_back(Item_010_001);
    pair.push_back(Item_011_001);

    SPtr<MbInstance> BoltM8_001;
    SPtr<MbInstance> BoltM8_002;
    SPtr<MbInstance> BoltM8_003;
    SPtr<MbInstance> BoltM8_004;

    SPtr<MbInstance> BoltM10_001;
    SPtr<MbInstance> BoltM10_002;
    SPtr<MbInstance> BoltM10_003;
    SPtr<MbInstance> BoltM10_004;
    SPtr<MbInstance> BoltM10_005;
    SPtr<MbInstance> BoltM10_006;
    SPtr<MbInstance> BoltM10_007;
    SPtr<MbInstance> BoltM10_008;
    SPtr<MbInstance> BoltM10_009;
    SPtr<MbInstance> BoltM10_010;
    SPtr<MbInstance> BoltM10_011;
    SPtr<MbInstance> BoltM10_012;
    SPtr<MbInstance> BoltM10_013;
    SPtr<MbInstance> BoltM10_014;
    SPtr<MbInstance> BoltM10_015;
    SPtr<MbInstance> BoltM10_016;
                                
    SPtr<MbInstance> BoltM12_001;
    SPtr<MbInstance> BoltM12_002;
    SPtr<MbInstance> BoltM12_003;
    SPtr<MbInstance> BoltM12_004;
    SPtr<MbInstance> BoltM12_005;
    SPtr<MbInstance> BoltM12_006;
    SPtr<MbInstance> BoltM12_007;
    SPtr<MbInstance> BoltM12_008;
    SPtr<MbInstance> BoltM12_009;
    SPtr<MbInstance> BoltM12_010;
    SPtr<MbInstance> BoltM12_011;
    SPtr<MbInstance> BoltM12_012;
    SPtr<MbInstance> BoltM12_013;
    SPtr<MbInstance> BoltM12_014;
    SPtr<MbInstance> BoltM12_015;
    SPtr<MbInstance> BoltM12_016;

    SPtr<MbInstance> BoltM16_001;
    SPtr<MbInstance> BoltM16_002;
    SPtr<MbInstance> BoltM16_003;
    SPtr<MbInstance> BoltM16_004;
    SPtr<MbInstance> BoltM16_005;
    SPtr<MbInstance> BoltM16_006;
    SPtr<MbInstance> BoltM16_007;
    SPtr<MbInstance> BoltM16_008;
    SPtr<MbInstance> BoltM16_009;
    SPtr<MbInstance> BoltM16_010;
    SPtr<MbInstance> BoltM16_011;
    SPtr<MbInstance> BoltM16_012;
    SPtr<MbInstance> BoltM16_013;
    SPtr<MbInstance> BoltM16_014;
    SPtr<MbInstance> BoltM16_015;
    SPtr<MbInstance> BoltM16_016;

    SPtr<MbInstance> WasherM8_001;
    SPtr<MbInstance> WasherM8_002;
    SPtr<MbInstance> WasherM8_003;
    SPtr<MbInstance> WasherM8_004;

    SPtr<MbInstance> WasherM10_001;
    SPtr<MbInstance> WasherM10_002;
    SPtr<MbInstance> WasherM10_003;
    SPtr<MbInstance> WasherM10_004;
    SPtr<MbInstance> WasherM10_005;
    SPtr<MbInstance> WasherM10_006;
    SPtr<MbInstance> WasherM10_007;
    SPtr<MbInstance> WasherM10_008;
    SPtr<MbInstance> WasherM10_009;
    SPtr<MbInstance> WasherM10_010;
    SPtr<MbInstance> WasherM10_011;
    SPtr<MbInstance> WasherM10_012;
    SPtr<MbInstance> WasherM10_013;
    SPtr<MbInstance> WasherM10_014;
    SPtr<MbInstance> WasherM10_015;
    SPtr<MbInstance> WasherM10_016;
                                  ;
    SPtr<MbInstance> WasherM12_001;
    SPtr<MbInstance> WasherM12_002;
    SPtr<MbInstance> WasherM12_003;
    SPtr<MbInstance> WasherM12_004;
    SPtr<MbInstance> WasherM12_005;
    SPtr<MbInstance> WasherM12_006;
    SPtr<MbInstance> WasherM12_007;
    SPtr<MbInstance> WasherM12_008;
    SPtr<MbInstance> WasherM12_009;
    SPtr<MbInstance> WasherM12_010;
    SPtr<MbInstance> WasherM12_011;
    SPtr<MbInstance> WasherM12_012;
    SPtr<MbInstance> WasherM12_013;
    SPtr<MbInstance> WasherM12_014;
    SPtr<MbInstance> WasherM12_015;
    SPtr<MbInstance> WasherM12_016;
                                  ;
    SPtr<MbInstance> WasherM16_001;
    SPtr<MbInstance> WasherM16_002;
    SPtr<MbInstance> WasherM16_003;
    SPtr<MbInstance> WasherM16_004;
    SPtr<MbInstance> WasherM16_005;
    SPtr<MbInstance> WasherM16_006;
    SPtr<MbInstance> WasherM16_007;
    SPtr<MbInstance> WasherM16_008;
    SPtr<MbInstance> WasherM16_009;
    SPtr<MbInstance> WasherM16_010;
    SPtr<MbInstance> WasherM16_011;
    SPtr<MbInstance> WasherM16_012;
    SPtr<MbInstance> WasherM16_013;
    SPtr<MbInstance> WasherM16_014;
    SPtr<MbInstance> WasherM16_015;
    SPtr<MbInstance> WasherM16_016;
                                  ;
    SPtr<MbInstance> NutBoltM8_001;
    SPtr<MbInstance> NutBoltM8_002;
    SPtr<MbInstance> NutBoltM8_003;
    SPtr<MbInstance> NutBoltM8_004;

    SPtr<MbInstance> NutBoltM10_001;
    SPtr<MbInstance> NutBoltM10_002;
    SPtr<MbInstance> NutBoltM10_003;
    SPtr<MbInstance> NutBoltM10_004;
    SPtr<MbInstance> NutBoltM10_005;
    SPtr<MbInstance> NutBoltM10_006;
    SPtr<MbInstance> NutBoltM10_007;
    SPtr<MbInstance> NutBoltM10_008;
    SPtr<MbInstance> NutBoltM10_009;
    SPtr<MbInstance> NutBoltM10_010;
    SPtr<MbInstance> NutBoltM10_011;
    SPtr<MbInstance> NutBoltM10_012;
    SPtr<MbInstance> NutBoltM10_013;
    SPtr<MbInstance> NutBoltM10_014;
    SPtr<MbInstance> NutBoltM10_015;
    SPtr<MbInstance> NutBoltM10_016;
                                   ;
    SPtr<MbInstance> NutBoltM12_001;
    SPtr<MbInstance> NutBoltM12_002;
    SPtr<MbInstance> NutBoltM12_003;
    SPtr<MbInstance> NutBoltM12_004;
    SPtr<MbInstance> NutBoltM12_005;
    SPtr<MbInstance> NutBoltM12_006;
    SPtr<MbInstance> NutBoltM12_007;
    SPtr<MbInstance> NutBoltM12_008;
    SPtr<MbInstance> NutBoltM12_009;
    SPtr<MbInstance> NutBoltM12_010;
    SPtr<MbInstance> NutBoltM12_011;
    SPtr<MbInstance> NutBoltM12_012;
    SPtr<MbInstance> NutBoltM12_013;
    SPtr<MbInstance> NutBoltM12_014;
    SPtr<MbInstance> NutBoltM12_015;
    SPtr<MbInstance> NutBoltM12_016;
                                   ;
    SPtr<MbInstance> NutBoltM16_001;
    SPtr<MbInstance> NutBoltM16_002;
    SPtr<MbInstance> NutBoltM16_003;
    SPtr<MbInstance> NutBoltM16_004;
    SPtr<MbInstance> NutBoltM16_005;
    SPtr<MbInstance> NutBoltM16_006;
    SPtr<MbInstance> NutBoltM16_007;
    SPtr<MbInstance> NutBoltM16_008;
    SPtr<MbInstance> NutBoltM16_009;
    SPtr<MbInstance> NutBoltM16_010;
    SPtr<MbInstance> NutBoltM16_011;
    SPtr<MbInstance> NutBoltM16_012;
    SPtr<MbInstance> NutBoltM16_013;
    SPtr<MbInstance> NutBoltM16_014;
    SPtr<MbInstance> NutBoltM16_015;
    SPtr<MbInstance> NutBoltM16_016;

#pragma region GHOSTS_INSTANCE
    if (turnOnStandart) {
        BoltM8_001 = new MbInstance(*BoltM8, lcs);
        BoltM8_002 = new MbInstance(*BoltM8, lcs);
        BoltM8_003 = new MbInstance(*BoltM8, lcs);
        BoltM8_004 = new MbInstance(*BoltM8, lcs);

        BoltM10_001 = new MbInstance(*BoltM10, lcs);
        BoltM10_002 = new MbInstance(*BoltM10, lcs);
        BoltM10_003 = new MbInstance(*BoltM10, lcs);
        BoltM10_004 = new MbInstance(*BoltM10, lcs);
        BoltM10_005 = new MbInstance(*BoltM10, lcs);
        BoltM10_006 = new MbInstance(*BoltM10, lcs);
        BoltM10_007 = new MbInstance(*BoltM10, lcs);
        BoltM10_008 = new MbInstance(*BoltM10, lcs);
        BoltM10_009 = new MbInstance(*BoltM10, lcs);
        BoltM10_010 = new MbInstance(*BoltM10, lcs);
        BoltM10_011 = new MbInstance(*BoltM10, lcs);
        BoltM10_012 = new MbInstance(*BoltM10, lcs);
        BoltM10_013 = new MbInstance(*BoltM10, lcs);
        BoltM10_014 = new MbInstance(*BoltM10, lcs);
        BoltM10_015 = new MbInstance(*BoltM10, lcs);
        BoltM10_016 = new MbInstance(*BoltM10, lcs);
                    
        BoltM12_001 = new MbInstance(*BoltM12, lcs);
        BoltM12_002 = new MbInstance(*BoltM12, lcs);
        BoltM12_003 = new MbInstance(*BoltM12, lcs);
        BoltM12_004 = new MbInstance(*BoltM12, lcs);
        BoltM12_005 = new MbInstance(*BoltM12, lcs);
        BoltM12_006 = new MbInstance(*BoltM12, lcs);
        BoltM12_007 = new MbInstance(*BoltM12, lcs);
        BoltM12_008 = new MbInstance(*BoltM12, lcs);
        BoltM12_009 = new MbInstance(*BoltM12, lcs);
        BoltM12_010 = new MbInstance(*BoltM12, lcs);
        BoltM12_011 = new MbInstance(*BoltM12, lcs);
        BoltM12_012 = new MbInstance(*BoltM12, lcs);
        BoltM12_013 = new MbInstance(*BoltM12, lcs);
        BoltM12_014 = new MbInstance(*BoltM12, lcs);
        BoltM12_015 = new MbInstance(*BoltM12, lcs);
        BoltM12_016 = new MbInstance(*BoltM12, lcs);
                    
        BoltM16_001 = new MbInstance(*BoltM16, lcs);
        BoltM16_002 = new MbInstance(*BoltM16, lcs);
        BoltM16_003 = new MbInstance(*BoltM16, lcs);
        BoltM16_004 = new MbInstance(*BoltM16, lcs);
        BoltM16_005 = new MbInstance(*BoltM16, lcs);
        BoltM16_006 = new MbInstance(*BoltM16, lcs);
        BoltM16_007 = new MbInstance(*BoltM16, lcs);
        BoltM16_008 = new MbInstance(*BoltM16, lcs);
        BoltM16_009 = new MbInstance(*BoltM16, lcs);
        BoltM16_010 = new MbInstance(*BoltM16, lcs);
        BoltM16_011 = new MbInstance(*BoltM16, lcs);
        BoltM16_012 = new MbInstance(*BoltM16, lcs);
        BoltM16_013 = new MbInstance(*BoltM16, lcs);
        BoltM16_014 = new MbInstance(*BoltM16, lcs);
        BoltM16_015 = new MbInstance(*BoltM16, lcs);
        BoltM16_016 = new MbInstance(*BoltM16, lcs);

        WasherM8_001 = new MbInstance(*WasherM8, lcs);
        WasherM8_002 = new MbInstance(*WasherM8, lcs);
        WasherM8_003 = new MbInstance(*WasherM8, lcs);
        WasherM8_004 = new MbInstance(*WasherM8, lcs);

        WasherM10_001 = new MbInstance(*WasherM10, lcs);
        WasherM10_002 = new MbInstance(*WasherM10, lcs);
        WasherM10_003 = new MbInstance(*WasherM10, lcs);
        WasherM10_004 = new MbInstance(*WasherM10, lcs);
        WasherM10_005 = new MbInstance(*WasherM10, lcs);
        WasherM10_006 = new MbInstance(*WasherM10, lcs);
        WasherM10_007 = new MbInstance(*WasherM10, lcs);
        WasherM10_008 = new MbInstance(*WasherM10, lcs);
        WasherM10_009 = new MbInstance(*WasherM10, lcs);
        WasherM10_010 = new MbInstance(*WasherM10, lcs);
        WasherM10_011 = new MbInstance(*WasherM10, lcs);
        WasherM10_012 = new MbInstance(*WasherM10, lcs);
        WasherM10_013 = new MbInstance(*WasherM10, lcs);
        WasherM10_014 = new MbInstance(*WasherM10, lcs);
        WasherM10_015 = new MbInstance(*WasherM10, lcs);
        WasherM10_016 = new MbInstance(*WasherM10, lcs);
                      
        WasherM12_001 = new MbInstance(*WasherM12, lcs);
        WasherM12_002 = new MbInstance(*WasherM12, lcs);
        WasherM12_003 = new MbInstance(*WasherM12, lcs);
        WasherM12_004 = new MbInstance(*WasherM12, lcs);
        WasherM12_005 = new MbInstance(*WasherM12, lcs);
        WasherM12_006 = new MbInstance(*WasherM12, lcs);
        WasherM12_007 = new MbInstance(*WasherM12, lcs);
        WasherM12_008 = new MbInstance(*WasherM12, lcs);
        WasherM12_009 = new MbInstance(*WasherM12, lcs);
        WasherM12_010 = new MbInstance(*WasherM12, lcs);
        WasherM12_011 = new MbInstance(*WasherM12, lcs);
        WasherM12_012 = new MbInstance(*WasherM12, lcs);
        WasherM12_013 = new MbInstance(*WasherM12, lcs);
        WasherM12_014 = new MbInstance(*WasherM12, lcs);
        WasherM12_015 = new MbInstance(*WasherM12, lcs);
        WasherM12_016 = new MbInstance(*WasherM12, lcs);
                      
        WasherM16_001 = new MbInstance(*WasherM16, lcs);
        WasherM16_002 = new MbInstance(*WasherM16, lcs);
        WasherM16_003 = new MbInstance(*WasherM16, lcs);
        WasherM16_004 = new MbInstance(*WasherM16, lcs);
        WasherM16_005 = new MbInstance(*WasherM16, lcs);
        WasherM16_006 = new MbInstance(*WasherM16, lcs);
        WasherM16_007 = new MbInstance(*WasherM16, lcs);
        WasherM16_008 = new MbInstance(*WasherM16, lcs);
        WasherM16_009 = new MbInstance(*WasherM16, lcs);
        WasherM16_010 = new MbInstance(*WasherM16, lcs);
        WasherM16_011 = new MbInstance(*WasherM16, lcs);
        WasherM16_012 = new MbInstance(*WasherM16, lcs);
        WasherM16_013 = new MbInstance(*WasherM16, lcs);
        WasherM16_014 = new MbInstance(*WasherM16, lcs);
        WasherM16_015 = new MbInstance(*WasherM16, lcs);
        WasherM16_016 = new MbInstance(*WasherM16, lcs);
                      
        NutBoltM8_001 = new MbInstance(*NutBoltM8, lcs);
        NutBoltM8_002 = new MbInstance(*NutBoltM8, lcs);
        NutBoltM8_003 = new MbInstance(*NutBoltM8, lcs);
        NutBoltM8_004 = new MbInstance(*NutBoltM8, lcs);

        NutBoltM10_001 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_002 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_003 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_004 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_005 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_006 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_007 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_008 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_009 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_010 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_011 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_012 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_013 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_014 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_015 = new MbInstance(*NutBoltM10, lcs);
        NutBoltM10_016 = new MbInstance(*NutBoltM10, lcs);
                       
        NutBoltM12_001 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_002 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_003 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_004 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_005 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_006 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_007 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_008 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_009 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_010 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_011 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_012 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_013 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_014 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_015 = new MbInstance(*NutBoltM12, lcs);
        NutBoltM12_016 = new MbInstance(*NutBoltM12, lcs);
                       
        NutBoltM16_001 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_002 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_003 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_004 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_005 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_006 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_007 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_008 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_009 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_010 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_011 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_012 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_013 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_014 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_015 = new MbInstance(*NutBoltM16, lcs);
        NutBoltM16_016 = new MbInstance(*NutBoltM16, lcs);

        // Ghosts М8
        pair.push_back(BoltM8_001);
        pair.push_back(BoltM8_002);
        pair.push_back(BoltM8_003);
        pair.push_back(BoltM8_004);

        pair.push_back(WasherM8_001);
        pair.push_back(WasherM8_002);
        pair.push_back(WasherM8_003);
        pair.push_back(WasherM8_004);

        pair.push_back(NutBoltM8_001);
        pair.push_back(NutBoltM8_002);
        pair.push_back(NutBoltM8_003);
        pair.push_back(NutBoltM8_004);

        // Ghosts M10
        pair.push_back(BoltM10_001);
        pair.push_back(BoltM10_002);
        pair.push_back(BoltM10_003);
        pair.push_back(BoltM10_004);
        pair.push_back(BoltM10_005);
        pair.push_back(BoltM10_006);
        pair.push_back(BoltM10_007);
        pair.push_back(BoltM10_008);
        pair.push_back(BoltM10_009);
        pair.push_back(BoltM10_010);
        pair.push_back(BoltM10_011);
        pair.push_back(BoltM10_012);
        pair.push_back(BoltM10_013);
        pair.push_back(BoltM10_014);
        pair.push_back(BoltM10_015);
        pair.push_back(BoltM10_016);

        pair.push_back(WasherM10_001);
        pair.push_back(WasherM10_002);
        pair.push_back(WasherM10_003);
        pair.push_back(WasherM10_004);
        pair.push_back(WasherM10_005);
        pair.push_back(WasherM10_006);
        pair.push_back(WasherM10_007);
        pair.push_back(WasherM10_008);
        pair.push_back(WasherM10_009);
        pair.push_back(WasherM10_010);
        pair.push_back(WasherM10_011);
        pair.push_back(WasherM10_012);
        pair.push_back(WasherM10_013);
        pair.push_back(WasherM10_014);
        pair.push_back(WasherM10_015);
        pair.push_back(WasherM10_016);

        pair.push_back(NutBoltM10_001);
        pair.push_back(NutBoltM10_002);
        pair.push_back(NutBoltM10_003);
        pair.push_back(NutBoltM10_004);
        pair.push_back(NutBoltM10_005);
        pair.push_back(NutBoltM10_006);
        pair.push_back(NutBoltM10_007);
        pair.push_back(NutBoltM10_008);
        pair.push_back(NutBoltM10_009);
        pair.push_back(NutBoltM10_010);
        pair.push_back(NutBoltM10_011);
        pair.push_back(NutBoltM10_012);
        pair.push_back(NutBoltM10_013);
        pair.push_back(NutBoltM10_014);
        pair.push_back(NutBoltM10_015);
        pair.push_back(NutBoltM10_016);

        // Ghosts M12
        pair.push_back(BoltM12_001);
        pair.push_back(BoltM12_002);
        pair.push_back(BoltM12_003);
        pair.push_back(BoltM12_004);
        pair.push_back(BoltM12_005);
        pair.push_back(BoltM12_006);
        pair.push_back(BoltM12_007);
        pair.push_back(BoltM12_008);
        pair.push_back(BoltM12_009);
        pair.push_back(BoltM12_010);
        pair.push_back(BoltM12_011);
        pair.push_back(BoltM12_012);
        pair.push_back(BoltM12_013);
        pair.push_back(BoltM12_014);
        pair.push_back(BoltM12_015);
        pair.push_back(BoltM12_016);

        pair.push_back(WasherM12_001);
        pair.push_back(WasherM12_002);
        pair.push_back(WasherM12_003);
        pair.push_back(WasherM12_004);
        pair.push_back(WasherM12_005);
        pair.push_back(WasherM12_006);
        pair.push_back(WasherM12_007);
        pair.push_back(WasherM12_008);
        pair.push_back(WasherM12_009);
        pair.push_back(WasherM12_010);
        pair.push_back(WasherM12_011);
        pair.push_back(WasherM12_012);
        pair.push_back(WasherM12_013);
        pair.push_back(WasherM12_014);
        pair.push_back(WasherM12_015);
        pair.push_back(WasherM12_016);

        pair.push_back(NutBoltM12_001);
        pair.push_back(NutBoltM12_002);
        pair.push_back(NutBoltM12_003);
        pair.push_back(NutBoltM12_004);
        pair.push_back(NutBoltM12_005);
        pair.push_back(NutBoltM12_006);
        pair.push_back(NutBoltM12_007);
        pair.push_back(NutBoltM12_008);
        pair.push_back(NutBoltM12_009);
        pair.push_back(NutBoltM12_010);
        pair.push_back(NutBoltM12_011);
        pair.push_back(NutBoltM12_012);
        pair.push_back(NutBoltM12_013);
        pair.push_back(NutBoltM12_014);
        pair.push_back(NutBoltM12_015);
        pair.push_back(NutBoltM12_016);

        //Ghosts 16
        // Ghosts M10
        pair.push_back(BoltM16_001);
        pair.push_back(BoltM16_002);
        pair.push_back(BoltM16_003);
        pair.push_back(BoltM16_004);
        pair.push_back(BoltM16_005);
        pair.push_back(BoltM16_006);
        pair.push_back(BoltM16_007);
        pair.push_back(BoltM16_008);
        pair.push_back(BoltM16_009);
        pair.push_back(BoltM16_010);
        pair.push_back(BoltM16_011);
        pair.push_back(BoltM16_012);
        pair.push_back(BoltM16_013);
        pair.push_back(BoltM16_014);
        pair.push_back(BoltM16_015);
        pair.push_back(BoltM16_016);

        pair.push_back(WasherM16_001);
        pair.push_back(WasherM16_002);
        pair.push_back(WasherM16_003);
        pair.push_back(WasherM16_004);
        pair.push_back(WasherM16_005);
        pair.push_back(WasherM16_006);
        pair.push_back(WasherM16_007);
        pair.push_back(WasherM16_008);
        pair.push_back(WasherM16_009);
        pair.push_back(WasherM16_010);
        pair.push_back(WasherM16_011);
        pair.push_back(WasherM16_012);
        pair.push_back(WasherM16_013);
        pair.push_back(WasherM16_014);
        pair.push_back(WasherM16_015);
        pair.push_back(WasherM16_016);

        pair.push_back(NutBoltM16_001);
        pair.push_back(NutBoltM16_002);
        pair.push_back(NutBoltM16_003);
        pair.push_back(NutBoltM16_004);
        pair.push_back(NutBoltM16_005);
        pair.push_back(NutBoltM16_006);
        pair.push_back(NutBoltM16_007);
        pair.push_back(NutBoltM16_008);
        pair.push_back(NutBoltM16_009);
        pair.push_back(NutBoltM16_010);
        pair.push_back(NutBoltM16_011);
        pair.push_back(NutBoltM16_012);
        pair.push_back(NutBoltM16_013);
        pair.push_back(NutBoltM16_014);
        pair.push_back(NutBoltM16_015);
        pair.push_back(NutBoltM16_016);
    }
#pragma endregion

    SPtr<MbAssembly> assm(new MbAssembly(pair));
    {

        // Расстояние Опора - Опора
        {
            MtGeomArgument Face1(Detail_003->GetFace(8), Item_003_001);
            MtGeomArgument Face2(Detail_003->GetFace(3), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, Face1, Face2, MtParVariant(l0));
        }

        // Концентричность Опора - Опора
        {
            MtGeomArgument Face1321(Detail_003->GetFace(30), Item_003_001);
            MtGeomArgument Face2312(Detail_003->GetFace(30), Item_003_002);

            assm->AddConstraint(GCM_CONCENTRIC, Face1321, Face2312);
        }

        // Совмещение Опора - Опора
        {
            MtGeomArgument Face1321ty(Detail_003->GetFace(7), Item_003_001);
            MtGeomArgument Face2312ty(Detail_003->GetFace(7), Item_003_002);

            assm->AddConstraint(GCM_COINCIDENT, Face1321ty, Face2312ty);
        }

        // Концентричность Опора - Трубы кожуховые
        {
            MtGeomArgument Otv1(Detail_003->GetFace(36), Item_003_001);
            MtGeomArgument Otv2(Detail_003->GetFace(38), Item_003_001);
            MtGeomArgument Otv3(Detail_003->GetFace(39), Item_003_001);
            MtGeomArgument Otv4(Detail_003->GetFace(37), Item_003_001);

            MtGeomArgument Otvtube1(Detail_002->GetFace(3), Item_002_001);
            MtGeomArgument Otvtube2(Detail_002->GetFace(3), Item_002_002);
            MtGeomArgument Otvtube3(Detail_002->GetFace(3), Item_002_003);
            MtGeomArgument Otvtube4(Detail_002->GetFace(3), Item_002_004);

            assm->AddConstraint(GCM_CONCENTRIC, Otv1, Otvtube1);
            assm->AddConstraint(GCM_CONCENTRIC, Otv2, Otvtube2);
            assm->AddConstraint(GCM_CONCENTRIC, Otv3, Otvtube3);
            assm->AddConstraint(GCM_CONCENTRIC, Otv4, Otvtube4);
        }

        // Концентричность Опора - Трубы теплообменные
        {
            MtGeomArgument Hole1(Detail_003->GetFace(36), Item_003_001);
            MtGeomArgument Hole2(Detail_003->GetFace(38), Item_003_001);
            MtGeomArgument Hole3(Detail_003->GetFace(39), Item_003_001);
            MtGeomArgument Hole4(Detail_003->GetFace(37), Item_003_001);

            MtGeomArgument OtvtubeTeploobmennaya1(Detail_002->GetFace(3), Item_001_001);
            MtGeomArgument OtvtubeTeploobmennaya2(Detail_002->GetFace(3), Item_001_002);
            MtGeomArgument OtvtubeTeploobmennaya3(Detail_002->GetFace(3), Item_001_003);
            MtGeomArgument OtvtubeTeploobmennaya4(Detail_002->GetFace(3), Item_001_004);

            assm->AddConstraint(GCM_CONCENTRIC, Hole1, OtvtubeTeploobmennaya1);
            assm->AddConstraint(GCM_CONCENTRIC, Hole2, OtvtubeTeploobmennaya2);
            assm->AddConstraint(GCM_CONCENTRIC, Hole3, OtvtubeTeploobmennaya3);
            assm->AddConstraint(GCM_CONCENTRIC, Hole4, OtvtubeTeploobmennaya4);
        }

        // Расстояние Опора - Решетка кожуховых труб
        {
            MtGeomArgument Plane1(Detail_003->GetFace(8), Item_003_002);
            MtGeomArgument Plane2(Detail_004->GetFace(0), Item_004_001);

            assm->AddConstraint(GCM_DISTANCE, Plane1, Plane2, MtParVariant(distanceRezhetka));
        }

        // Концентричность Опора - Решетка кожуховых труб
        {
            MtGeomArgument VariableForHole1(Detail_003->GetFace(30), Item_003_002);

            MtGeomArgument OtvtubeTeploobmennaya1(Detail_004->GetFace(3), Item_004_001);

            assm->AddConstraint(GCM_CONCENTRIC, VariableForHole1, OtvtubeTeploobmennaya1);
        }

        // Расстояние Опора - Кожуховые трубы
        {
            MtGeomArgument PlaneOfTubeKozhux1(Detail_002->GetFace(0), Item_002_001);
            MtGeomArgument PlaneOfTubeKozhux2(Detail_002->GetFace(0), Item_002_002);
            MtGeomArgument PlaneOfTubeKozhux3(Detail_002->GetFace(0), Item_002_003);
            MtGeomArgument PlaneOfTubeKozhux4(Detail_002->GetFace(0), Item_002_004);

            MtGeomArgument PlaneOfOporaForward1(Detail_003->GetFace(8), Item_003_002);
            MtGeomArgument PlaneOfOporaForward2(Detail_003->GetFace(8), Item_003_002);
            MtGeomArgument PlaneOfOporaForward3(Detail_003->GetFace(8), Item_003_002);
            MtGeomArgument PlaneOfOporaForward4(Detail_003->GetFace(8), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux1, PlaneOfOporaForward1, MtParVariant(distanceTubesKozhux));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux2, PlaneOfOporaForward2, MtParVariant(distanceTubesKozhux));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux3, PlaneOfOporaForward3, MtParVariant(distanceTubesKozhux));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeKozhux4, PlaneOfOporaForward4, MtParVariant(distanceTubesKozhux));
        }

        // Расстояние Опора - Теплообменные трубы
        {
            MtGeomArgument PlaneOfTubeTeploobmeny1(Detail_001->GetFace(0), Item_001_001);
            MtGeomArgument PlaneOfTubeTeploobmeny2(Detail_001->GetFace(0), Item_001_002);
            MtGeomArgument PlaneOfTubeTeploobmeny3(Detail_001->GetFace(0), Item_001_003);
            MtGeomArgument PlaneOfTubeTeploobmeny4(Detail_001->GetFace(0), Item_001_004);

            MtGeomArgument PlaneOfOporaForwards1(Detail_003->GetFace(8), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards2(Detail_003->GetFace(8), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards3(Detail_003->GetFace(8), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards4(Detail_003->GetFace(8), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny1, PlaneOfOporaForwards1, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny2, PlaneOfOporaForwards2, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny3, PlaneOfOporaForwards3, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny4, PlaneOfOporaForwards4, MtParVariant(distanceTubesTeploobmen));
        }

        // Совмещение Фланец кожуховый - Кожуховые трубы
        {
            MtGeomArgument PlaneOfTubeKozhux1(Detail_002->GetFace(1), Item_002_001);
            MtGeomArgument PlaneOfTubeKozhux2(Detail_002->GetFace(1), Item_002_002);
            MtGeomArgument PlaneOfTubeKozhux3(Detail_002->GetFace(1), Item_002_003);
            MtGeomArgument PlaneOfTubeKozhux4(Detail_002->GetFace(1), Item_002_004);

            MtGeomArgument PlaneFlanecKozhux1(Detail_007->GetFace(12), Item_007_001);
            MtGeomArgument PlaneFlanecKozhux2(Detail_007->GetFace(12), Item_007_002);
            MtGeomArgument PlaneFlanecKozhux3(Detail_007->GetFace(12), Item_007_003);
            MtGeomArgument PlaneFlanecKozhux4(Detail_007->GetFace(12), Item_007_004);


            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux1, PlaneFlanecKozhux1);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux2, PlaneFlanecKozhux2);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux3, PlaneFlanecKozhux3);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeKozhux4, PlaneFlanecKozhux4);
        }

        // Концентричность Фланец кожуховый - Кожуховые трубы
        {
            MtGeomArgument SubPlaneOfTubeKozhux1(Detail_002->GetFace(2), Item_002_001);
            MtGeomArgument SubPlaneOfTubeKozhux2(Detail_002->GetFace(2), Item_002_002);
            MtGeomArgument SubPlaneOfTubeKozhux3(Detail_002->GetFace(2), Item_002_003);
            MtGeomArgument SubPlaneOfTubeKozhux4(Detail_002->GetFace(2), Item_002_004);

            MtGeomArgument SubPlaneFlanecKozhux1(Detail_007->GetFace(3), Item_007_001);
            MtGeomArgument SubPlaneFlanecKozhux2(Detail_007->GetFace(3), Item_007_002);
            MtGeomArgument SubPlaneFlanecKozhux3(Detail_007->GetFace(3), Item_007_003);
            MtGeomArgument SubPlaneFlanecKozhux4(Detail_007->GetFace(3), Item_007_004);


            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux1, SubPlaneFlanecKozhux1);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux2, SubPlaneFlanecKozhux2);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux3, SubPlaneFlanecKozhux3);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeKozhux4, SubPlaneFlanecKozhux4);
        }

        // Совмещение Фланец кожуховый - Решетка теплообменная
        {
            MtGeomArgument PlaneOfTubeTeploobmennaya1(Detail_006->GetFace(2), Item_006_001);
            MtGeomArgument PlaneOfTubeTeploobmennaya2(Detail_006->GetFace(2), Item_006_002);
            MtGeomArgument PlaneOfTubeTeploobmennaya3(Detail_006->GetFace(2), Item_006_003);
            MtGeomArgument PlaneOfTubeTeploobmennaya4(Detail_006->GetFace(2), Item_006_004);

            MtGeomArgument PlaneFlanecTeploobmennaya1(Detail_007->GetFace(9), Item_007_001);
            MtGeomArgument PlaneFlanecTeploobmennaya2(Detail_007->GetFace(9), Item_007_002);
            MtGeomArgument PlaneFlanecTeploobmennaya3(Detail_007->GetFace(9), Item_007_003);
            MtGeomArgument PlaneFlanecTeploobmennaya4(Detail_007->GetFace(9), Item_007_004);


            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya1, PlaneFlanecTeploobmennaya1);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya2, PlaneFlanecTeploobmennaya2);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya3, PlaneFlanecTeploobmennaya3);
            assm->AddConstraint(GCM_COINCIDENT, PlaneOfTubeTeploobmennaya4, PlaneFlanecTeploobmennaya4);
        }

        // Концентричность Фланец кожуховый - Решетка теплообменная
        {
            MtGeomArgument SubPlaneOfTubeTeploobmennaya1(Detail_006->GetFace(3), Item_006_001);
            MtGeomArgument SubPlaneOfTubeTeploobmennaya2(Detail_006->GetFace(3), Item_006_002);
            MtGeomArgument SubPlaneOfTubeTeploobmennaya3(Detail_006->GetFace(3), Item_006_003);
            MtGeomArgument SubPlaneOfTubeTeploobmennaya4(Detail_006->GetFace(3), Item_006_004);

            MtGeomArgument SubPlaneFlanecTeploobmennaya1(Detail_007->GetFace(3), Item_007_001);
            MtGeomArgument SubPlaneFlanecTeploobmennaya2(Detail_007->GetFace(3), Item_007_002);
            MtGeomArgument SubPlaneFlanecTeploobmennaya3(Detail_007->GetFace(3), Item_007_003);
            MtGeomArgument SubPlaneFlanecTeploobmennaya4(Detail_007->GetFace(3), Item_007_004);


            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya1, SubPlaneFlanecTeploobmennaya1);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya2, SubPlaneFlanecTeploobmennaya2);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya3, SubPlaneFlanecTeploobmennaya3);
            assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya4, SubPlaneFlanecTeploobmennaya4);
        }

        // Совмещение Фланец Специальный - Труба теплообменная (В камере)
        {
            MtGeomArgument getPlaneOfTubeTeploobmennaya1(Detail_001->GetFace(0), Item_001_001);
            MtGeomArgument getPlaneOfTubeTeploobmennaya2(Detail_001->GetFace(0), Item_001_002);
            MtGeomArgument getPlaneOfTubeTeploobmennaya3(Detail_001->GetFace(0), Item_001_003);
            MtGeomArgument getPlaneOfTubeTeploobmennaya4(Detail_001->GetFace(0), Item_001_004);

            MtGeomArgument getPlaneFlanecSpec1(Detail_008->GetFace(12), Item_008_001);
            MtGeomArgument getPlaneFlanecSpec2(Detail_008->GetFace(12), Item_008_002);
            MtGeomArgument getPlaneFlanecSpec3(Detail_008->GetFace(12), Item_008_003);
            MtGeomArgument getPlaneFlanecSpec4(Detail_008->GetFace(12), Item_008_004);


            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya1, getPlaneFlanecSpec1, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya2, getPlaneFlanecSpec2, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya3, getPlaneFlanecSpec3, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya4, getPlaneFlanecSpec4, MtParVariant(GCM_OPPOSITE));
        }

        // Концентричность Фланец Специальный - Труба теплообменная (В камере) 
        {
            MtGeomArgument getPlaneOfTubeTeploobmennaya144(Detail_001->GetFace(2), Item_001_001);
            MtGeomArgument getPlaneOfTubeTeploobmennaya244(Detail_001->GetFace(2), Item_001_002);
            MtGeomArgument getPlaneOfTubeTeploobmennaya344(Detail_001->GetFace(2), Item_001_003);
            MtGeomArgument getPlaneOfTubeTeploobmennaya444(Detail_001->GetFace(2), Item_001_004);

            MtGeomArgument getPlaneFlanecSpec133(Detail_008->GetFace(14), Item_008_001);
            MtGeomArgument getPlaneFlanecSpec233(Detail_008->GetFace(14), Item_008_002);
            MtGeomArgument getPlaneFlanecSpec333(Detail_008->GetFace(14), Item_008_003);
            MtGeomArgument getPlaneFlanecSpec433(Detail_008->GetFace(14), Item_008_004);


            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya144, getPlaneFlanecSpec133, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya244, getPlaneFlanecSpec233, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya344, getPlaneFlanecSpec333, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya444, getPlaneFlanecSpec433, MtParVariant(GCM_OPPOSITE));
        }

        // Совмещение Фланец Специальный - Фланец Специальный (В камере)
        {
            MtGeomArgument getPlaneOfTubeTeploobmennaya1123(Detail_008->GetFace(9), Item_008_001);
            MtGeomArgument getPlaneOfTubeTeploobmennaya2123(Detail_008->GetFace(9), Item_008_002);
            MtGeomArgument getPlaneOfTubeTeploobmennaya3123(Detail_008->GetFace(9), Item_008_003);
            MtGeomArgument getPlaneOfTubeTeploobmennaya4123(Detail_008->GetFace(9), Item_008_004);

            MtGeomArgument getPlaneOfTubeTeploobmennaya1321(Detail_008->GetFace(9), Item_008_005);
            MtGeomArgument getPlaneOfTubeTeploobmennaya2321(Detail_008->GetFace(9), Item_008_006);
            MtGeomArgument getPlaneOfTubeTeploobmennaya3321(Detail_008->GetFace(9), Item_008_007);
            MtGeomArgument getPlaneOfTubeTeploobmennaya4321(Detail_008->GetFace(9), Item_008_008);


            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya1123, getPlaneOfTubeTeploobmennaya1321, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya2123, getPlaneOfTubeTeploobmennaya2321, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya3123, getPlaneOfTubeTeploobmennaya3321, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya4123, getPlaneOfTubeTeploobmennaya4321, MtParVariant(GCM_OPPOSITE));
        }

        // Концентричность Фланец Специальный - Фланец Специальный (В камере) 
        {
            MtGeomArgument getPlaneOfTubeTeploobmennaya144ss(Detail_008->GetFace(14), Item_008_001);
            MtGeomArgument getPlaneOfTubeTeploobmennaya244ss(Detail_008->GetFace(14), Item_008_002);
            MtGeomArgument getPlaneOfTubeTeploobmennaya344ss(Detail_008->GetFace(14), Item_008_003);
            MtGeomArgument getPlaneOfTubeTeploobmennaya444ss(Detail_008->GetFace(14), Item_008_004);

            MtGeomArgument getPlaneFlanecSpec133t(Detail_008->GetFace(14), Item_008_005);
            MtGeomArgument getPlaneFlanecSpec233t(Detail_008->GetFace(14), Item_008_006);
            MtGeomArgument getPlaneFlanecSpec333t(Detail_008->GetFace(14), Item_008_007);
            MtGeomArgument getPlaneFlanecSpec433t(Detail_008->GetFace(14), Item_008_008);


            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya144ss, getPlaneFlanecSpec133t, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya244ss, getPlaneFlanecSpec233t, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya344ss, getPlaneFlanecSpec333t, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, getPlaneOfTubeTeploobmennaya444ss, getPlaneFlanecSpec433t, MtParVariant(GCM_OPPOSITE));
        }

        // Совмещение Изогнутая труба - Фланец Специальный (В камере) 
        {
            MtGeomArgument PlaneofCurevedTube1(Detail_009->GetFace(0), Item_009_001);
            MtGeomArgument PlaneofCurevedTube2(Detail_009->GetFace(0), Item_009_002);

            MtGeomArgument PlaneForCoincedent1(Detail_008->GetFace(12), Item_008_005);
            MtGeomArgument PlaneForCoincedent2(Detail_008->GetFace(12), Item_008_006);

            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube1, PlaneForCoincedent1);
            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube2, PlaneForCoincedent2);
        }

        // Концентричность Изогнутая труба - Фланец Специальный (В камере) 
        {
            MtGeomArgument VariableForPlany1(Detail_008->GetFace(14), Item_008_005);
            MtGeomArgument VariableForPlany2(Detail_008->GetFace(14), Item_008_006);
            MtGeomArgument VariableForPlany4(Detail_008->GetFace(14), Item_008_007);
            MtGeomArgument VariableForPlany5(Detail_008->GetFace(14), Item_008_008);

            MtGeomArgument OtvtubeCureved1(Detail_009->GetFace(3), Item_009_001);
            MtGeomArgument OtvtubeCureved2(Detail_009->GetFace(3), Item_009_002);
            MtGeomArgument OtvtubeCureved4(Detail_009->GetFace(2), Item_009_001);
            MtGeomArgument OtvtubeCureved5(Detail_009->GetFace(2), Item_009_002);

            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved1, VariableForPlany1);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved2, VariableForPlany2);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved4, VariableForPlany4);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved5, VariableForPlany5);

        }

        // Совмещение Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument Argument1(Detail_001->GetFace(1), Item_001_001);
            MtGeomArgument Argument2(Detail_001->GetFace(1), Item_001_002);
            MtGeomArgument Argument3(Detail_001->GetFace(1), Item_001_003);
            MtGeomArgument Argument4(Detail_001->GetFace(1), Item_001_004);

            MtGeomArgument Argument5(Detail_008->GetFace(12), Item_008_009);
            MtGeomArgument Argument6(Detail_008->GetFace(12), Item_008_010);
            MtGeomArgument Argument7(Detail_008->GetFace(12), Item_008_011);
            MtGeomArgument Argument8(Detail_008->GetFace(12), Item_008_012);

            assm->AddConstraint(GCM_COINCIDENT, Argument1, Argument5);
            assm->AddConstraint(GCM_COINCIDENT, Argument2, Argument6);
            assm->AddConstraint(GCM_COINCIDENT, Argument3, Argument7);
            assm->AddConstraint(GCM_COINCIDENT, Argument4, Argument8);
        }

        // Концентричность Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument Argument1Plane(Detail_001->GetFace(3), Item_001_001);
            MtGeomArgument Argument2Plane(Detail_001->GetFace(3), Item_001_002);
            MtGeomArgument Argument3Plane(Detail_001->GetFace(3), Item_001_003);
            MtGeomArgument Argument4Plane(Detail_001->GetFace(3), Item_001_004);

            MtGeomArgument Argument5Plane(Detail_008->GetFace(14), Item_008_009);
            MtGeomArgument Argument6Plane(Detail_008->GetFace(14), Item_008_010);
            MtGeomArgument Argument7Plane(Detail_008->GetFace(14), Item_008_011);
            MtGeomArgument Argument8Plane(Detail_008->GetFace(14), Item_008_012);


            assm->AddConstraint(GCM_CONCENTRIC, Argument1Plane, Argument5Plane);
            assm->AddConstraint(GCM_CONCENTRIC, Argument2Plane, Argument6Plane);
            assm->AddConstraint(GCM_CONCENTRIC, Argument3Plane, Argument7Plane);
            assm->AddConstraint(GCM_CONCENTRIC, Argument4Plane, Argument8Plane);
        }

        // Совмещение Фланец Специальный - Фланец Специальный
        {
            MtGeomArgument FlanecCpecPlane1(Detail_008->GetFace(9), Item_008_009);
            MtGeomArgument FlanecCpecPlane2(Detail_008->GetFace(9), Item_008_010);
            MtGeomArgument FlanecCpecPlane3(Detail_008->GetFace(9), Item_008_011);
            MtGeomArgument FlanecCpecPlane4(Detail_008->GetFace(9), Item_008_012);

            MtGeomArgument FlanecCpecPlane5(Detail_008->GetFace(9), Item_008_013);
            MtGeomArgument FlanecCpecPlane6(Detail_008->GetFace(9), Item_008_014);
            MtGeomArgument FlanecCpecPlane7(Detail_008->GetFace(9), Item_008_015);
            MtGeomArgument FlanecCpecPlane8(Detail_008->GetFace(9), Item_008_016);


            assm->AddConstraint(GCM_COINCIDENT, FlanecCpecPlane1, FlanecCpecPlane5, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, FlanecCpecPlane2, FlanecCpecPlane6, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, FlanecCpecPlane3, FlanecCpecPlane7, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_COINCIDENT, FlanecCpecPlane4, FlanecCpecPlane8, MtParVariant(GCM_OPPOSITE));
        }

        // Концентричность Фланец Специальный - Фланец Специальный
        {
            MtGeomArgument FlanecCpecOtv1(Detail_008->GetFace(14), Item_008_009);
            MtGeomArgument FlanecCpecOtv2(Detail_008->GetFace(14), Item_008_010);
            MtGeomArgument FlanecCpecOtv3(Detail_008->GetFace(14), Item_008_011);
            MtGeomArgument FlanecCpecOtv4(Detail_008->GetFace(14), Item_008_012);

            MtGeomArgument FlanecCpecOtv5(Detail_008->GetFace(14), Item_008_013);
            MtGeomArgument FlanecCpecOtv6(Detail_008->GetFace(14), Item_008_014);
            MtGeomArgument FlanecCpecOtv7(Detail_008->GetFace(14), Item_008_015);
            MtGeomArgument FlanecCpecOtv8(Detail_008->GetFace(14), Item_008_016);


            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv1, FlanecCpecOtv5, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv2, FlanecCpecOtv6, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv3, FlanecCpecOtv7, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv4, FlanecCpecOtv8, MtParVariant(GCM_OPPOSITE));
        }

        // Совмещение Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument FlanecCpecOtvTest1(Detail_008->GetFace(12), Item_008_015);
            MtGeomArgument FlanecCpecOtvTest2(Detail_008->GetFace(12), Item_008_016);
            MtGeomArgument FlanecCpecOtvTest3(Detail_009->GetFace(0), Item_009_003);

            assm->AddConstraint(GCM_COINCIDENT, FlanecCpecOtvTest1, FlanecCpecOtvTest3);
            assm->AddConstraint(GCM_COINCIDENT, FlanecCpecOtvTest2, FlanecCpecOtvTest3);

            MtGeomArgument FlanecCpecOtvTest4(Detail_008->GetFace(14), Item_008_015);
            MtGeomArgument FlanecCpecOtvTest5(Detail_008->GetFace(14), Item_008_016);
            MtGeomArgument FlanecCpecOtvTest6(Detail_009->GetFace(2), Item_009_003);
            MtGeomArgument FlanecCpecOtvTest7(Detail_009->GetFace(3), Item_009_003);

            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtvTest4, FlanecCpecOtvTest6);
            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtvTest5, FlanecCpecOtvTest7);
        }

        // Концентричность Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument FlanecCpecOtv1(Detail_008->GetFace(14), Item_008_009);
            MtGeomArgument FlanecCpecOtv2(Detail_008->GetFace(14), Item_008_010);
            MtGeomArgument FlanecCpecOtv3(Detail_008->GetFace(14), Item_008_011);
            MtGeomArgument FlanecCpecOtv4(Detail_008->GetFace(14), Item_008_012);

            MtGeomArgument FlanecCpecOtv5(Detail_008->GetFace(14), Item_008_013);
            MtGeomArgument FlanecCpecOtv6(Detail_008->GetFace(14), Item_008_014);
            MtGeomArgument FlanecCpecOtv7(Detail_008->GetFace(14), Item_008_015);
            MtGeomArgument FlanecCpecOtv8(Detail_008->GetFace(14), Item_008_016);


            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv1, FlanecCpecOtv5, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv2, FlanecCpecOtv6, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv3, FlanecCpecOtv7, MtParVariant(GCM_OPPOSITE));
            assm->AddConstraint(GCM_CONCENTRIC, FlanecCpecOtv4, FlanecCpecOtv8, MtParVariant(GCM_OPPOSITE));
        }

        // Расстояние/Концентричность Решетка кожуховых труб - Камера
        {
            MtGeomArgument Planeofkamera1(Detail_005->GetFace(1), Item_005_001);
            MtGeomArgument Planeofkamera2(Detail_005->GetFace(3), Item_005_001);

            MtGeomArgument PlaneViaRezhetka1(Detail_004->GetFace(41), Item_004_001);
            MtGeomArgument PlaneViaRezhetka2(Detail_004->GetFace(3), Item_004_001);

            assm->AddConstraint(GCM_COINCIDENT, Planeofkamera1, PlaneViaRezhetka1);
            assm->AddConstraint(GCM_CONCENTRIC, Planeofkamera2, PlaneViaRezhetka2);
        }

        // Расстояние/Концентричность Соединение с фланцами
        {

            MtGeomArgument Face1312(Detail_003->GetFace(3), Item_003_001);
            MtGeomArgument Face2321(Detail_011->GetFace(3), Item_011_001);

            assm->AddConstraint(GCM_DISTANCE, Face1312, Face2321, MtParVariant(l2 - shirinaOpori - 40));

            MtGeomArgument ConnectorsPlane1(Detail_002->GetFace(3), Item_002_001);
            MtGeomArgument ConnectorsPlane2(Detail_002->GetFace(3), Item_002_002);

            MtGeomArgument ConnectorsPlane3(Detail_011->GetFace(7), Item_011_001);
            MtGeomArgument ConnectorsPlane4(Detail_011->GetFace(8), Item_011_001);

            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane2, ConnectorsPlane3);
            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane1, ConnectorsPlane4);
        }

        // Расстояние/Концентричность Соединение
        {
            MtGeomArgument Face1312(Detail_003->GetFace(3), Item_003_001);
            MtGeomArgument Face2321(Detail_010->GetFace(2), Item_010_001);

            assm->AddConstraint(GCM_DISTANCE, Face1312, Face2321, MtParVariant(l2 - shirinaOpori - 40));

            MtGeomArgument ConnectorsPlane1(Detail_002->GetFace(3), Item_002_003);
            MtGeomArgument ConnectorsPlane2(Detail_002->GetFace(3), Item_002_004);

            MtGeomArgument ConnectorsPlane3(Detail_010->GetFace(4), Item_010_001);
            MtGeomArgument ConnectorsPlane4(Detail_010->GetFace(5), Item_010_001);

            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane2, ConnectorsPlane3);
            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane1, ConnectorsPlane4);
        }

        // Совмещение/Концентричность фланцы на соеденение
        {

            Item_008_017->Rotate(axVert, M_PI);
            MtGeomArgument facebody1(Detail_007->GetFace(12), Item_008_017);
            MtGeomArgument facebody2(Detail_007->GetFace(12), Item_008_018);
            MtGeomArgument facebody3(Detail_011->GetFace(5), Item_011_001);
            MtGeomArgument facebody4(Detail_011->GetFace(0), Item_011_001);

            assm->AddConstraint(GCM_COINCIDENT, facebody1, facebody3);
            assm->AddConstraint(GCM_COINCIDENT, facebody2, facebody4);

            MtGeomArgument ConnectorsPlane1(Detail_007->GetFace(14), Item_008_017);
            MtGeomArgument ConnectorsPlane2(Detail_007->GetFace(14), Item_008_018);

            MtGeomArgument ConnectorsPlane3(Detail_011->GetFace(6), Item_011_001);
            MtGeomArgument ConnectorsPlane4(Detail_011->GetFace(6), Item_011_001);

            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane1, ConnectorsPlane3);
            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane2, ConnectorsPlane4);
        }

#pragma region GHOST_Constraints

        if (turnOnStandart) {
        // Концентричность ШайбыM10 - Фланец специальный внешние
        {

            MtGeomArgument WasherFaceTest1(Detail_008->GetFace(4), Item_008_013);
            MtGeomArgument WasherFaceTest2(Detail_008->GetFace(5), Item_008_013);
            MtGeomArgument WasherFaceTest3(Detail_008->GetFace(7), Item_008_013);
            MtGeomArgument WasherFaceTest4(Detail_008->GetFace(0), Item_008_013);

            MtGeomArgument WasherCircleTest1(WasherM10->GetFace(3), WasherM10_001);
            MtGeomArgument WasherCircleTest2(WasherM10->GetFace(3), WasherM10_002);
            MtGeomArgument WasherCircleTest3(WasherM10->GetFace(3), WasherM10_003);
            MtGeomArgument WasherCircleTest4(WasherM10->GetFace(3), WasherM10_004);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1, WasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2, WasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3, WasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4, WasherCircleTest4);

            //
            MtGeomArgument WasherFaceTest1s(Detail_008->GetFace(4), Item_008_014);
            MtGeomArgument WasherFaceTest2s(Detail_008->GetFace(5), Item_008_014);
            MtGeomArgument WasherFaceTest3s(Detail_008->GetFace(7), Item_008_014);
            MtGeomArgument WasherFaceTest4s(Detail_008->GetFace(0), Item_008_014);

            MtGeomArgument WasherCircleTest1s(WasherM10->GetFace(3), WasherM10_005);
            MtGeomArgument WasherCircleTest2s(WasherM10->GetFace(3), WasherM10_006);
            MtGeomArgument WasherCircleTest3s(WasherM10->GetFace(3), WasherM10_007);
            MtGeomArgument WasherCircleTest4s(WasherM10->GetFace(3), WasherM10_008);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1s, WasherCircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2s, WasherCircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3s, WasherCircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4s, WasherCircleTest4s);

            //
            MtGeomArgument WasherFaceTest1f(Detail_008->GetFace(4), Item_008_015);
            MtGeomArgument WasherFaceTest2f(Detail_008->GetFace(5), Item_008_015);
            MtGeomArgument WasherFaceTest3f(Detail_008->GetFace(7), Item_008_015);
            MtGeomArgument WasherFaceTest4f(Detail_008->GetFace(0), Item_008_015);

            MtGeomArgument WasherCircleTest1f(WasherM10->GetFace(3), WasherM10_009);
            MtGeomArgument WasherCircleTest2f(WasherM10->GetFace(3), WasherM10_010);
            MtGeomArgument WasherCircleTest3f(WasherM10->GetFace(3), WasherM10_011);
            MtGeomArgument WasherCircleTest4f(WasherM10->GetFace(3), WasherM10_012);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1f, WasherCircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2f, WasherCircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3f, WasherCircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4f, WasherCircleTest4f);

            //
            MtGeomArgument WasherFaceTest1d(Detail_008->GetFace(4), Item_008_016);
            MtGeomArgument WasherFaceTest2d(Detail_008->GetFace(5), Item_008_016);
            MtGeomArgument WasherFaceTest3d(Detail_008->GetFace(7), Item_008_016);
            MtGeomArgument WasherFaceTest4d(Detail_008->GetFace(0), Item_008_016);

            MtGeomArgument WasherCircleTest1d(WasherM10->GetFace(3), WasherM10_013);
            MtGeomArgument WasherCircleTest2d(WasherM10->GetFace(3), WasherM10_014);
            MtGeomArgument WasherCircleTest3d(WasherM10->GetFace(3), WasherM10_015);
            MtGeomArgument WasherCircleTest4d(WasherM10->GetFace(3), WasherM10_016);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1d, WasherCircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2d, WasherCircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3d, WasherCircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4d, WasherCircleTest4d);
        }
        // Совместимость ШайбыM10 - Фланец специальный внешние
        {
            MtGeomArgument WasherFaceTest1TT(Detail_008->GetFace(2), Item_008_013);

            MtGeomArgument WasherCircleTest1TT(WasherM10->GetFace(2), WasherM10_001);
            MtGeomArgument WasherCircleTest2TT(WasherM10->GetFace(2), WasherM10_002);
            MtGeomArgument WasherCircleTest3TT(WasherM10->GetFace(2), WasherM10_003);
            MtGeomArgument WasherCircleTest4TT(WasherM10->GetFace(2), WasherM10_004);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest4TT);

            //
            MtGeomArgument WasherFaceTest1TTs(Detail_008->GetFace(2), Item_008_014);

            MtGeomArgument WasherCircleTest1TTs(WasherM10->GetFace(2), WasherM10_005);
            MtGeomArgument WasherCircleTest2TTs(WasherM10->GetFace(2), WasherM10_006);
            MtGeomArgument WasherCircleTest3TTs(WasherM10->GetFace(2), WasherM10_007);
            MtGeomArgument WasherCircleTest4TTs(WasherM10->GetFace(2), WasherM10_008);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest4TTs);

            //
            MtGeomArgument WasherFaceTest1TTf(Detail_008->GetFace(2), Item_008_015);

            MtGeomArgument WasherCircleTest1TTf(WasherM10->GetFace(2), WasherM10_009);
            MtGeomArgument WasherCircleTest2TTf(WasherM10->GetFace(2), WasherM10_010);
            MtGeomArgument WasherCircleTest3TTf(WasherM10->GetFace(2), WasherM10_011);
            MtGeomArgument WasherCircleTest4TTf(WasherM10->GetFace(2), WasherM10_012);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest4TTf);

            //
            MtGeomArgument WasherFaceTest1TTd(Detail_008->GetFace(2), Item_008_016);

            MtGeomArgument WasherCircleTest1TTd(WasherM10->GetFace(2), WasherM10_013);
            MtGeomArgument WasherCircleTest2TTd(WasherM10->GetFace(2), WasherM10_014);
            MtGeomArgument WasherCircleTest3TTd(WasherM10->GetFace(2), WasherM10_015);
            MtGeomArgument WasherCircleTest4TTd(WasherM10->GetFace(2), WasherM10_016);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest4TTd);
        }
        // Концентричность Болты M10 - Фланец специальный внешние
        {

            MtGeomArgument FaceTest1(WasherM10->GetFace(1), WasherM10_001);
            MtGeomArgument FaceTest2(WasherM10->GetFace(1), WasherM10_002);
            MtGeomArgument FaceTest3(WasherM10->GetFace(1), WasherM10_003);
            MtGeomArgument FaceTest4(WasherM10->GetFace(1), WasherM10_004);

            MtGeomArgument CircleTest1(BoltM10->GetFace(5), BoltM10_001);
            MtGeomArgument CircleTest2(BoltM10->GetFace(5), BoltM10_002);
            MtGeomArgument CircleTest3(BoltM10->GetFace(5), BoltM10_003);
            MtGeomArgument CircleTest4(BoltM10->GetFace(5), BoltM10_004);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1, CircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2, CircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3, CircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4, CircleTest4);

            //
            MtGeomArgument FaceTest1s(WasherM10->GetFace(1), WasherM10_005);
            MtGeomArgument FaceTest2s(WasherM10->GetFace(1), WasherM10_006);
            MtGeomArgument FaceTest3s(WasherM10->GetFace(1), WasherM10_007);
            MtGeomArgument FaceTest4s(WasherM10->GetFace(1), WasherM10_008);

            MtGeomArgument CircleTest1s(BoltM10->GetFace(5), BoltM10_005);
            MtGeomArgument CircleTest2s(BoltM10->GetFace(5), BoltM10_006);
            MtGeomArgument CircleTest3s(BoltM10->GetFace(5), BoltM10_007);
            MtGeomArgument CircleTest4s(BoltM10->GetFace(5), BoltM10_008);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1s, CircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2s, CircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3s, CircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4s, CircleTest4s);

            //
            MtGeomArgument FaceTest1f(WasherM10->GetFace(1), WasherM10_009);
            MtGeomArgument FaceTest2f(WasherM10->GetFace(1), WasherM10_010);
            MtGeomArgument FaceTest3f(WasherM10->GetFace(1), WasherM10_011);
            MtGeomArgument FaceTest4f(WasherM10->GetFace(1), WasherM10_012);

            MtGeomArgument CircleTest1f(BoltM10->GetFace(5), BoltM10_009);
            MtGeomArgument CircleTest2f(BoltM10->GetFace(5), BoltM10_010);
            MtGeomArgument CircleTest3f(BoltM10->GetFace(5), BoltM10_011);
            MtGeomArgument CircleTest4f(BoltM10->GetFace(5), BoltM10_012);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1f, CircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2f, CircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3f, CircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4f, CircleTest4f);

            //
            MtGeomArgument FaceTest1d(WasherM10->GetFace(1), WasherM10_013);
            MtGeomArgument FaceTest2d(WasherM10->GetFace(1), WasherM10_014);
            MtGeomArgument FaceTest3d(WasherM10->GetFace(1), WasherM10_015);
            MtGeomArgument FaceTest4d(WasherM10->GetFace(1), WasherM10_016);

            MtGeomArgument CircleTest1d(BoltM10->GetFace(5), BoltM10_013);
            MtGeomArgument CircleTest2d(BoltM10->GetFace(5), BoltM10_014);
            MtGeomArgument CircleTest3d(BoltM10->GetFace(5), BoltM10_015);
            MtGeomArgument CircleTest4d(BoltM10->GetFace(5), BoltM10_016);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1d, CircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2d, CircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3d, CircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4d, CircleTest4d);
        }
        // Совместимость Болты M10 - Фланец специальный внешние
        {
            MtGeomArgument FaceTest1TT(WasherM10->GetFace(0), WasherM10_001);
            MtGeomArgument FaceTest2TT(WasherM10->GetFace(0), WasherM10_002);
            MtGeomArgument FaceTest3TT(WasherM10->GetFace(0), WasherM10_003);
            MtGeomArgument FaceTest4TT(WasherM10->GetFace(0), WasherM10_004);

            MtGeomArgument CircleTest1TT(BoltM10->GetFace(6), BoltM10_001);
            MtGeomArgument CircleTest2TT(BoltM10->GetFace(6), BoltM10_002);
            MtGeomArgument CircleTest3TT(BoltM10->GetFace(6), BoltM10_003);
            MtGeomArgument CircleTest4TT(BoltM10->GetFace(6), BoltM10_004);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TT, CircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest2TT, CircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest3TT, CircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest4TT, CircleTest4TT);

            //
            MtGeomArgument FaceTest1TTs(WasherM10->GetFace(0), WasherM10_005);
            MtGeomArgument FaceTest2TTs(WasherM10->GetFace(0), WasherM10_006);
            MtGeomArgument FaceTest3TTs(WasherM10->GetFace(0), WasherM10_007);
            MtGeomArgument FaceTest4TTs(WasherM10->GetFace(0), WasherM10_008);

            MtGeomArgument CircleTest1TTs(BoltM10->GetFace(6), BoltM10_005);
            MtGeomArgument CircleTest2TTs(BoltM10->GetFace(6), BoltM10_006);
            MtGeomArgument CircleTest3TTs(BoltM10->GetFace(6), BoltM10_007);
            MtGeomArgument CircleTest4TTs(BoltM10->GetFace(6), BoltM10_008);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest4TTs);

            //
            MtGeomArgument FaceTest1TTf(WasherM10->GetFace(0), WasherM10_009);
            MtGeomArgument FaceTest2TTf(WasherM10->GetFace(0), WasherM10_010);
            MtGeomArgument FaceTest3TTf(WasherM10->GetFace(0), WasherM10_011);
            MtGeomArgument FaceTest4TTf(WasherM10->GetFace(0), WasherM10_012);

            MtGeomArgument CircleTest1TTf(BoltM10->GetFace(6), BoltM10_009);
            MtGeomArgument CircleTest2TTf(BoltM10->GetFace(6), BoltM10_010);
            MtGeomArgument CircleTest3TTf(BoltM10->GetFace(6), BoltM10_011);
            MtGeomArgument CircleTest4TTf(BoltM10->GetFace(6), BoltM10_012);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest4TTf);

            //
            MtGeomArgument FaceTest1TTd(WasherM10->GetFace(0), WasherM10_013);
            MtGeomArgument FaceTest2TTd(WasherM10->GetFace(0), WasherM10_014);
            MtGeomArgument FaceTest3TTd(WasherM10->GetFace(0), WasherM10_015);
            MtGeomArgument FaceTest4TTd(WasherM10->GetFace(0), WasherM10_016);

            MtGeomArgument CircleTest1TTd(BoltM10->GetFace(6), BoltM10_013);
            MtGeomArgument CircleTest2TTd(BoltM10->GetFace(6), BoltM10_014);
            MtGeomArgument CircleTest3TTd(BoltM10->GetFace(6), BoltM10_015);
            MtGeomArgument CircleTest4TTd(BoltM10->GetFace(6), BoltM10_016);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest4TTd);
        }
        // Концентричность Гайка M10 - Фланец специальный внутренний
        {

            MtGeomArgument setWasherFaceTest1(Detail_008->GetFace(4), Item_008_009);
            MtGeomArgument setWasherFaceTest2(Detail_008->GetFace(5), Item_008_009);
            MtGeomArgument setWasherFaceTest3(Detail_008->GetFace(7), Item_008_009);
            MtGeomArgument setWasherFaceTest4(Detail_008->GetFace(0), Item_008_009);
            MtGeomArgument setWasherCircleTest1(NutBoltM10->GetFace(2), NutBoltM10_001);
            MtGeomArgument setWasherCircleTest2(NutBoltM10->GetFace(2), NutBoltM10_002);
            MtGeomArgument setWasherCircleTest3(NutBoltM10->GetFace(2), NutBoltM10_003);
            MtGeomArgument setWasherCircleTest4(NutBoltM10->GetFace(2), NutBoltM10_004);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1, setWasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2, setWasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3, setWasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4, setWasherCircleTest4);

            //
            MtGeomArgument setWasherFaceTest1s(Detail_008->GetFace(4), Item_008_010);
            MtGeomArgument setWasherFaceTest2s(Detail_008->GetFace(5), Item_008_010);
            MtGeomArgument setWasherFaceTest3s(Detail_008->GetFace(7), Item_008_010);
            MtGeomArgument setWasherFaceTest4s(Detail_008->GetFace(0), Item_008_010);
            MtGeomArgument setWasherCircleTest1s(NutBoltM10->GetFace(2), NutBoltM10_005);
            MtGeomArgument setWasherCircleTest2s(NutBoltM10->GetFace(2), NutBoltM10_006);
            MtGeomArgument setWasherCircleTest3s(NutBoltM10->GetFace(2), NutBoltM10_007);
            MtGeomArgument setWasherCircleTest4s(NutBoltM10->GetFace(2), NutBoltM10_008);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1s, setWasherCircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2s, setWasherCircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3s, setWasherCircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4s, setWasherCircleTest4s);

            //
            MtGeomArgument setWasherFaceTest1f(Detail_008->GetFace(4), Item_008_011);
            MtGeomArgument setWasherFaceTest2f(Detail_008->GetFace(5), Item_008_011);
            MtGeomArgument setWasherFaceTest3f(Detail_008->GetFace(7), Item_008_011);
            MtGeomArgument setWasherFaceTest4f(Detail_008->GetFace(0), Item_008_011);
            MtGeomArgument setWasherCircleTest1f(NutBoltM10->GetFace(2), NutBoltM10_009);
            MtGeomArgument setWasherCircleTest2f(NutBoltM10->GetFace(2), NutBoltM10_010);
            MtGeomArgument setWasherCircleTest3f(NutBoltM10->GetFace(2), NutBoltM10_011);
            MtGeomArgument setWasherCircleTest4f(NutBoltM10->GetFace(2), NutBoltM10_012);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1f, setWasherCircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2f, setWasherCircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3f, setWasherCircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4f, setWasherCircleTest4f);

            //
            MtGeomArgument setWasherFaceTest1d(Detail_008->GetFace(4), Item_008_012);
            MtGeomArgument setWasherFaceTest2d(Detail_008->GetFace(5), Item_008_012);
            MtGeomArgument setWasherFaceTest3d(Detail_008->GetFace(7), Item_008_012);
            MtGeomArgument setWasherFaceTest4d(Detail_008->GetFace(0), Item_008_012);
            MtGeomArgument setWasherCircleTest1d(NutBoltM10->GetFace(2), NutBoltM10_013);
            MtGeomArgument setWasherCircleTest2d(NutBoltM10->GetFace(2), NutBoltM10_014);
            MtGeomArgument setWasherCircleTest3d(NutBoltM10->GetFace(2), NutBoltM10_015);
            MtGeomArgument setWasherCircleTest4d(NutBoltM10->GetFace(2), NutBoltM10_016);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1d, setWasherCircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2d, setWasherCircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3d, setWasherCircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4d, setWasherCircleTest4d);
        }
        // Совместимость Гайка M10 - Фланец специальный внутренний
        {
            MtGeomArgument setWasherFaceTest1TT(Detail_008->GetFace(2), Item_008_009);
            MtGeomArgument setWasherCircleTest1TT(NutBoltM10->GetFace(0), NutBoltM10_001);
            MtGeomArgument setWasherCircleTest2TT(NutBoltM10->GetFace(0), NutBoltM10_002);
            MtGeomArgument setWasherCircleTest3TT(NutBoltM10->GetFace(0), NutBoltM10_003);
            MtGeomArgument setWasherCircleTest4TT(NutBoltM10->GetFace(0), NutBoltM10_004);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest4TT);

            //
            MtGeomArgument setWasherFaceTest1TTs(Detail_008->GetFace(2), Item_008_010);
            MtGeomArgument setWasherCircleTest1TTs(NutBoltM10->GetFace(0), NutBoltM10_005);
            MtGeomArgument setWasherCircleTest2TTs(NutBoltM10->GetFace(0), NutBoltM10_006);
            MtGeomArgument setWasherCircleTest3TTs(NutBoltM10->GetFace(0), NutBoltM10_007);
            MtGeomArgument setWasherCircleTest4TTs(NutBoltM10->GetFace(0), NutBoltM10_008);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest4TTs);

            //
            MtGeomArgument setWasherFaceTest1TTf(Detail_008->GetFace(2), Item_008_011);
            MtGeomArgument setWasherCircleTest1TTf(NutBoltM10->GetFace(0), NutBoltM10_009);
            MtGeomArgument setWasherCircleTest2TTf(NutBoltM10->GetFace(0), NutBoltM10_010);
            MtGeomArgument setWasherCircleTest3TTf(NutBoltM10->GetFace(0), NutBoltM10_011);
            MtGeomArgument setWasherCircleTest4TTf(NutBoltM10->GetFace(0), NutBoltM10_012);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest4TTf);

            //
            MtGeomArgument setWasherFaceTest1TTd(Detail_008->GetFace(2), Item_008_012);
            MtGeomArgument setWasherCircleTest1TTd(NutBoltM10->GetFace(0), NutBoltM10_013);
            MtGeomArgument setWasherCircleTest2TTd(NutBoltM10->GetFace(0), NutBoltM10_014);
            MtGeomArgument setWasherCircleTest3TTd(NutBoltM10->GetFace(0), NutBoltM10_015);
            MtGeomArgument setWasherCircleTest4TTd(NutBoltM10->GetFace(0), NutBoltM10_016);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest4TTd);
        }
        
        // Концентричность Гайка - Решетка теплообменных труб M12
        {

            MtGeomArgument setWasherFaceTest1(Detail_006->GetFace(4), Item_006_001);
            MtGeomArgument setWasherFaceTest2(Detail_006->GetFace(5), Item_006_001);
            MtGeomArgument setWasherFaceTest3(Detail_006->GetFace(7), Item_006_001);
            MtGeomArgument setWasherFaceTest4(Detail_006->GetFace(0), Item_006_001);
            MtGeomArgument setWasherCircleTest1(NutBoltM12->GetFace(2), NutBoltM12_001);
            MtGeomArgument setWasherCircleTest2(NutBoltM12->GetFace(2), NutBoltM12_002);
            MtGeomArgument setWasherCircleTest3(NutBoltM12->GetFace(2), NutBoltM12_003);
            MtGeomArgument setWasherCircleTest4(NutBoltM12->GetFace(2), NutBoltM12_004);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1, setWasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2, setWasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3, setWasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4, setWasherCircleTest4);

            //
            MtGeomArgument setWasherFaceTest1s(Detail_006->GetFace(4), Item_006_002);
            MtGeomArgument setWasherFaceTest2s(Detail_006->GetFace(5), Item_006_002);
            MtGeomArgument setWasherFaceTest3s(Detail_006->GetFace(7), Item_006_002);
            MtGeomArgument setWasherFaceTest4s(Detail_006->GetFace(0), Item_006_002);
            MtGeomArgument setWasherCircleTest1s(NutBoltM12->GetFace(2), NutBoltM12_005);
            MtGeomArgument setWasherCircleTest2s(NutBoltM12->GetFace(2), NutBoltM12_006);
            MtGeomArgument setWasherCircleTest3s(NutBoltM12->GetFace(2), NutBoltM12_007);
            MtGeomArgument setWasherCircleTest4s(NutBoltM12->GetFace(2), NutBoltM12_008);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1s, setWasherCircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2s, setWasherCircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3s, setWasherCircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4s, setWasherCircleTest4s);

            //
            MtGeomArgument setWasherFaceTest1f(Detail_006->GetFace(4), Item_006_003);
            MtGeomArgument setWasherFaceTest2f(Detail_006->GetFace(5), Item_006_003);
            MtGeomArgument setWasherFaceTest3f(Detail_006->GetFace(7), Item_006_003);
            MtGeomArgument setWasherFaceTest4f(Detail_006->GetFace(0), Item_006_003);
            MtGeomArgument setWasherCircleTest1f(NutBoltM12->GetFace(2), NutBoltM12_009);
            MtGeomArgument setWasherCircleTest2f(NutBoltM12->GetFace(2), NutBoltM12_010);
            MtGeomArgument setWasherCircleTest3f(NutBoltM12->GetFace(2), NutBoltM12_011);
            MtGeomArgument setWasherCircleTest4f(NutBoltM12->GetFace(2), NutBoltM12_012);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1f, setWasherCircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2f, setWasherCircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3f, setWasherCircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4f, setWasherCircleTest4f);

            //
            MtGeomArgument setWasherFaceTest1d(Detail_006->GetFace(4), Item_006_004);
            MtGeomArgument setWasherFaceTest2d(Detail_006->GetFace(5), Item_006_004);
            MtGeomArgument setWasherFaceTest3d(Detail_006->GetFace(7), Item_006_004);
            MtGeomArgument setWasherFaceTest4d(Detail_006->GetFace(0), Item_006_004);
            MtGeomArgument setWasherCircleTest1d(NutBoltM12->GetFace(2), NutBoltM12_013);
            MtGeomArgument setWasherCircleTest2d(NutBoltM12->GetFace(2), NutBoltM12_014);
            MtGeomArgument setWasherCircleTest3d(NutBoltM12->GetFace(2), NutBoltM12_015);
            MtGeomArgument setWasherCircleTest4d(NutBoltM12->GetFace(2), NutBoltM12_016);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1d, setWasherCircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2d, setWasherCircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3d, setWasherCircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4d, setWasherCircleTest4d);
        }
        // Совместимость Гайка - Решетка теплообменных труб M12
        {
            MtGeomArgument setWasherFaceTest1TT(Detail_006->GetFace(1), Item_006_001);
            MtGeomArgument setWasherCircleTest1TT(NutBoltM12->GetFace(1), NutBoltM12_001);
            MtGeomArgument setWasherCircleTest2TT(NutBoltM12->GetFace(1), NutBoltM12_002);
            MtGeomArgument setWasherCircleTest3TT(NutBoltM12->GetFace(1), NutBoltM12_003);
            MtGeomArgument setWasherCircleTest4TT(NutBoltM12->GetFace(1), NutBoltM12_004);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest4TT);

            //
            MtGeomArgument setWasherFaceTest1TTs(Detail_006->GetFace(1), Item_006_002);
            MtGeomArgument setWasherCircleTest1TTs(NutBoltM12->GetFace(1), NutBoltM12_005);
            MtGeomArgument setWasherCircleTest2TTs(NutBoltM12->GetFace(1), NutBoltM12_006);
            MtGeomArgument setWasherCircleTest3TTs(NutBoltM12->GetFace(1), NutBoltM12_007);
            MtGeomArgument setWasherCircleTest4TTs(NutBoltM12->GetFace(1), NutBoltM12_008);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest4TTs);

            //
            MtGeomArgument setWasherFaceTest1TTf(Detail_006->GetFace(1), Item_006_003);
            MtGeomArgument setWasherCircleTest1TTf(NutBoltM12->GetFace(1), NutBoltM12_009);
            MtGeomArgument setWasherCircleTest2TTf(NutBoltM12->GetFace(1), NutBoltM12_010);
            MtGeomArgument setWasherCircleTest3TTf(NutBoltM12->GetFace(1), NutBoltM12_011);
            MtGeomArgument setWasherCircleTest4TTf(NutBoltM12->GetFace(1), NutBoltM12_012);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest4TTf);

            //
            MtGeomArgument setWasherFaceTest1TTd(Detail_006->GetFace(1), Item_006_004);
            MtGeomArgument setWasherCircleTest1TTd(NutBoltM12->GetFace(1), NutBoltM12_013);
            MtGeomArgument setWasherCircleTest2TTd(NutBoltM12->GetFace(1), NutBoltM12_014);
            MtGeomArgument setWasherCircleTest3TTd(NutBoltM12->GetFace(1), NutBoltM12_015);
            MtGeomArgument setWasherCircleTest4TTd(NutBoltM12->GetFace(1), NutBoltM12_016);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest4TTd);
        }
        // Концентричность ШайбыM12 - Фланец специальный внешние
        {

            MtGeomArgument WasherFaceTest1(Detail_007->GetFace(4), Item_007_001);
            MtGeomArgument WasherFaceTest2(Detail_007->GetFace(5), Item_007_001);
            MtGeomArgument WasherFaceTest3(Detail_007->GetFace(7), Item_007_001);
            MtGeomArgument WasherFaceTest4(Detail_007->GetFace(0), Item_007_001);

            MtGeomArgument WasherCircleTest1(WasherM12->GetFace(3), WasherM12_001);
            MtGeomArgument WasherCircleTest2(WasherM12->GetFace(3), WasherM12_002);
            MtGeomArgument WasherCircleTest3(WasherM12->GetFace(3), WasherM12_003);
            MtGeomArgument WasherCircleTest4(WasherM12->GetFace(3), WasherM12_004);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1, WasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2, WasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3, WasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4, WasherCircleTest4);

            //
            MtGeomArgument WasherFaceTest1s(Detail_007->GetFace(4), Item_007_002);
            MtGeomArgument WasherFaceTest2s(Detail_007->GetFace(5), Item_007_002);
            MtGeomArgument WasherFaceTest3s(Detail_007->GetFace(7), Item_007_002);
            MtGeomArgument WasherFaceTest4s(Detail_007->GetFace(0), Item_007_002);

            MtGeomArgument WasherCircleTest1s(WasherM12->GetFace(3), WasherM12_005);
            MtGeomArgument WasherCircleTest2s(WasherM12->GetFace(3), WasherM12_006);
            MtGeomArgument WasherCircleTest3s(WasherM12->GetFace(3), WasherM12_007);
            MtGeomArgument WasherCircleTest4s(WasherM12->GetFace(3), WasherM12_008);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1s, WasherCircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2s, WasherCircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3s, WasherCircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4s, WasherCircleTest4s);

            //
            MtGeomArgument WasherFaceTest1f(Detail_007->GetFace(4), Item_007_003);
            MtGeomArgument WasherFaceTest2f(Detail_007->GetFace(5), Item_007_003);
            MtGeomArgument WasherFaceTest3f(Detail_007->GetFace(7), Item_007_003);
            MtGeomArgument WasherFaceTest4f(Detail_007->GetFace(0), Item_007_003);

            MtGeomArgument WasherCircleTest1f(WasherM12->GetFace(3), WasherM12_009);
            MtGeomArgument WasherCircleTest2f(WasherM12->GetFace(3), WasherM12_010);
            MtGeomArgument WasherCircleTest3f(WasherM12->GetFace(3), WasherM12_011);
            MtGeomArgument WasherCircleTest4f(WasherM12->GetFace(3), WasherM12_012);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1f, WasherCircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2f, WasherCircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3f, WasherCircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4f, WasherCircleTest4f);

            //
            MtGeomArgument WasherFaceTest1d(Detail_007->GetFace(4), Item_007_004);
            MtGeomArgument WasherFaceTest2d(Detail_007->GetFace(5), Item_007_004);
            MtGeomArgument WasherFaceTest3d(Detail_007->GetFace(7), Item_007_004);
            MtGeomArgument WasherFaceTest4d(Detail_007->GetFace(0), Item_007_004);

            MtGeomArgument WasherCircleTest1d(WasherM12->GetFace(3), WasherM12_013);
            MtGeomArgument WasherCircleTest2d(WasherM12->GetFace(3), WasherM12_014);
            MtGeomArgument WasherCircleTest3d(WasherM12->GetFace(3), WasherM12_015);
            MtGeomArgument WasherCircleTest4d(WasherM12->GetFace(3), WasherM12_016);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1d, WasherCircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2d, WasherCircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3d, WasherCircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4d, WasherCircleTest4d);
        }
        // Совместимость ШайбыM12 - Фланец специальный внешние
        {
            MtGeomArgument WasherFaceTest1TT(Detail_007->GetFace(2), Item_007_001);

            MtGeomArgument WasherCircleTest1TT(WasherM12->GetFace(2), WasherM12_001);
            MtGeomArgument WasherCircleTest2TT(WasherM12->GetFace(2), WasherM12_002);
            MtGeomArgument WasherCircleTest3TT(WasherM12->GetFace(2), WasherM12_003);
            MtGeomArgument WasherCircleTest4TT(WasherM12->GetFace(2), WasherM12_004);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest4TT);

            //
            MtGeomArgument WasherFaceTest1TTs(Detail_007->GetFace(2), Item_007_002);

            MtGeomArgument WasherCircleTest1TTs(WasherM12->GetFace(2), WasherM12_005);
            MtGeomArgument WasherCircleTest2TTs(WasherM12->GetFace(2), WasherM12_006);
            MtGeomArgument WasherCircleTest3TTs(WasherM12->GetFace(2), WasherM12_007);
            MtGeomArgument WasherCircleTest4TTs(WasherM12->GetFace(2), WasherM12_008);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest4TTs);

            //
            MtGeomArgument WasherFaceTest1TTf(Detail_007->GetFace(2), Item_007_003);

            MtGeomArgument WasherCircleTest1TTf(WasherM12->GetFace(2), WasherM12_009);
            MtGeomArgument WasherCircleTest2TTf(WasherM12->GetFace(2), WasherM12_010);
            MtGeomArgument WasherCircleTest3TTf(WasherM12->GetFace(2), WasherM12_011);
            MtGeomArgument WasherCircleTest4TTf(WasherM12->GetFace(2), WasherM12_012);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest4TTf);

            //
            MtGeomArgument WasherFaceTest1TTd(Detail_007->GetFace(2), Item_007_004);

            MtGeomArgument WasherCircleTest1TTd(WasherM12->GetFace(2), WasherM12_013);
            MtGeomArgument WasherCircleTest2TTd(WasherM12->GetFace(2), WasherM12_014);
            MtGeomArgument WasherCircleTest3TTd(WasherM12->GetFace(2), WasherM12_015);
            MtGeomArgument WasherCircleTest4TTd(WasherM12->GetFace(2), WasherM12_016);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest4TTd);
        }
        // Концентричность Болты M12 - Фланец специальный внешние
        {

            MtGeomArgument FaceTest1(WasherM12->GetFace(1), WasherM12_001);
            MtGeomArgument FaceTest2(WasherM12->GetFace(1), WasherM12_002);
            MtGeomArgument FaceTest3(WasherM12->GetFace(1), WasherM12_003);
            MtGeomArgument FaceTest4(WasherM12->GetFace(1), WasherM12_004);

            MtGeomArgument CircleTest1(BoltM12->GetFace(5), BoltM12_001);
            MtGeomArgument CircleTest2(BoltM12->GetFace(5), BoltM12_002);
            MtGeomArgument CircleTest3(BoltM12->GetFace(5), BoltM12_003);
            MtGeomArgument CircleTest4(BoltM12->GetFace(5), BoltM12_004);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1, CircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2, CircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3, CircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4, CircleTest4);

            //
            MtGeomArgument FaceTest1s(WasherM12->GetFace(1), WasherM12_005);
            MtGeomArgument FaceTest2s(WasherM12->GetFace(1), WasherM12_006);
            MtGeomArgument FaceTest3s(WasherM12->GetFace(1), WasherM12_007);
            MtGeomArgument FaceTest4s(WasherM12->GetFace(1), WasherM12_008);

            MtGeomArgument CircleTest1s(BoltM12->GetFace(5), BoltM12_005);
            MtGeomArgument CircleTest2s(BoltM12->GetFace(5), BoltM12_006);
            MtGeomArgument CircleTest3s(BoltM12->GetFace(5), BoltM12_007);
            MtGeomArgument CircleTest4s(BoltM12->GetFace(5), BoltM12_008);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1s, CircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2s, CircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3s, CircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4s, CircleTest4s);

            //
            MtGeomArgument FaceTest1f(WasherM12->GetFace(1), WasherM12_009);
            MtGeomArgument FaceTest2f(WasherM12->GetFace(1), WasherM12_010);
            MtGeomArgument FaceTest3f(WasherM12->GetFace(1), WasherM12_011);
            MtGeomArgument FaceTest4f(WasherM12->GetFace(1), WasherM12_012);

            MtGeomArgument CircleTest1f(BoltM12->GetFace(5), BoltM12_009);
            MtGeomArgument CircleTest2f(BoltM12->GetFace(5), BoltM12_010);
            MtGeomArgument CircleTest3f(BoltM12->GetFace(5), BoltM12_011);
            MtGeomArgument CircleTest4f(BoltM12->GetFace(5), BoltM12_012);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1f, CircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2f, CircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3f, CircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4f, CircleTest4f);

            //
            MtGeomArgument FaceTest1d(WasherM12->GetFace(1), WasherM12_013);
            MtGeomArgument FaceTest2d(WasherM12->GetFace(1), WasherM12_014);
            MtGeomArgument FaceTest3d(WasherM12->GetFace(1), WasherM12_015);
            MtGeomArgument FaceTest4d(WasherM12->GetFace(1), WasherM12_016);

            MtGeomArgument CircleTest1d(BoltM12->GetFace(5), BoltM12_013);
            MtGeomArgument CircleTest2d(BoltM12->GetFace(5), BoltM12_014);
            MtGeomArgument CircleTest3d(BoltM12->GetFace(5), BoltM12_015);
            MtGeomArgument CircleTest4d(BoltM12->GetFace(5), BoltM12_016);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1d, CircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2d, CircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3d, CircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4d, CircleTest4d);
        }
        // Совместимость Болты M12 - Фланец специальный внешние
        {
            MtGeomArgument FaceTest1TT(WasherM12->GetFace(0), WasherM12_001);
            MtGeomArgument FaceTest2TT(WasherM12->GetFace(0), WasherM12_002);
            MtGeomArgument FaceTest3TT(WasherM12->GetFace(0), WasherM12_003);
            MtGeomArgument FaceTest4TT(WasherM12->GetFace(0), WasherM12_004);

            MtGeomArgument CircleTest1TT(BoltM12->GetFace(6), BoltM12_001);
            MtGeomArgument CircleTest2TT(BoltM12->GetFace(6), BoltM12_002);
            MtGeomArgument CircleTest3TT(BoltM12->GetFace(6), BoltM12_003);
            MtGeomArgument CircleTest4TT(BoltM12->GetFace(6), BoltM12_004);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TT, CircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest2TT, CircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest3TT, CircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest4TT, CircleTest4TT);

            //
            MtGeomArgument FaceTest1TTs(WasherM12->GetFace(0), WasherM12_005);
            MtGeomArgument FaceTest2TTs(WasherM12->GetFace(0), WasherM12_006);
            MtGeomArgument FaceTest3TTs(WasherM12->GetFace(0), WasherM12_007);
            MtGeomArgument FaceTest4TTs(WasherM12->GetFace(0), WasherM12_008);

            MtGeomArgument CircleTest1TTs(BoltM12->GetFace(6), BoltM12_005);
            MtGeomArgument CircleTest2TTs(BoltM12->GetFace(6), BoltM12_006);
            MtGeomArgument CircleTest3TTs(BoltM12->GetFace(6), BoltM12_007);
            MtGeomArgument CircleTest4TTs(BoltM12->GetFace(6), BoltM12_008);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest4TTs);

            //
            MtGeomArgument FaceTest1TTf(WasherM12->GetFace(0), WasherM12_009);
            MtGeomArgument FaceTest2TTf(WasherM12->GetFace(0), WasherM12_010);
            MtGeomArgument FaceTest3TTf(WasherM12->GetFace(0), WasherM12_011);
            MtGeomArgument FaceTest4TTf(WasherM12->GetFace(0), WasherM12_012);

            MtGeomArgument CircleTest1TTf(BoltM12->GetFace(6), BoltM12_009);
            MtGeomArgument CircleTest2TTf(BoltM12->GetFace(6), BoltM12_010);
            MtGeomArgument CircleTest3TTf(BoltM12->GetFace(6), BoltM12_011);
            MtGeomArgument CircleTest4TTf(BoltM12->GetFace(6), BoltM12_012);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest4TTf);

            //
            MtGeomArgument FaceTest1TTd(WasherM12->GetFace(0), WasherM12_013);
            MtGeomArgument FaceTest2TTd(WasherM12->GetFace(0), WasherM12_014);
            MtGeomArgument FaceTest3TTd(WasherM12->GetFace(0), WasherM12_015);
            MtGeomArgument FaceTest4TTd(WasherM12->GetFace(0), WasherM12_016);

            MtGeomArgument CircleTest1TTd(BoltM12->GetFace(6), BoltM12_013);
            MtGeomArgument CircleTest2TTd(BoltM12->GetFace(6), BoltM12_014);
            MtGeomArgument CircleTest3TTd(BoltM12->GetFace(6), BoltM12_015);
            MtGeomArgument CircleTest4TTd(BoltM12->GetFace(6), BoltM12_016);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest4TTd);
        }
        // Концентричность ШайбыM16 - Фланец специальный внешние
        {

            MtGeomArgument WasherFaceTest1(Detail_008->GetFace(4), Item_008_005);
            MtGeomArgument WasherFaceTest2(Detail_008->GetFace(5), Item_008_005);
            MtGeomArgument WasherFaceTest3(Detail_008->GetFace(7), Item_008_005);
            MtGeomArgument WasherFaceTest4(Detail_008->GetFace(0), Item_008_005);

            MtGeomArgument WasherCircleTest1(WasherM16->GetFace(3), WasherM16_001);
            MtGeomArgument WasherCircleTest2(WasherM16->GetFace(3), WasherM16_002);
            MtGeomArgument WasherCircleTest3(WasherM16->GetFace(3), WasherM16_003);
            MtGeomArgument WasherCircleTest4(WasherM16->GetFace(3), WasherM16_004);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1, WasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2, WasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3, WasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4, WasherCircleTest4);

            //
            MtGeomArgument WasherFaceTest1s(Detail_008->GetFace(4), Item_008_006);
            MtGeomArgument WasherFaceTest2s(Detail_008->GetFace(5), Item_008_006);
            MtGeomArgument WasherFaceTest3s(Detail_008->GetFace(7), Item_008_006);
            MtGeomArgument WasherFaceTest4s(Detail_008->GetFace(0), Item_008_006);

            MtGeomArgument WasherCircleTest1s(WasherM16->GetFace(3), WasherM16_005);
            MtGeomArgument WasherCircleTest2s(WasherM16->GetFace(3), WasherM16_006);
            MtGeomArgument WasherCircleTest3s(WasherM16->GetFace(3), WasherM16_007);
            MtGeomArgument WasherCircleTest4s(WasherM16->GetFace(3), WasherM16_008);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1s, WasherCircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2s, WasherCircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3s, WasherCircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4s, WasherCircleTest4s);

            //
            MtGeomArgument WasherFaceTest1f(Detail_008->GetFace(4), Item_008_007);
            MtGeomArgument WasherFaceTest2f(Detail_008->GetFace(5), Item_008_007);
            MtGeomArgument WasherFaceTest3f(Detail_008->GetFace(7), Item_008_007);
            MtGeomArgument WasherFaceTest4f(Detail_008->GetFace(0), Item_008_007);

            MtGeomArgument WasherCircleTest1f(WasherM16->GetFace(3), WasherM16_009);
            MtGeomArgument WasherCircleTest2f(WasherM16->GetFace(3), WasherM16_010);
            MtGeomArgument WasherCircleTest3f(WasherM16->GetFace(3), WasherM16_011);
            MtGeomArgument WasherCircleTest4f(WasherM16->GetFace(3), WasherM16_012);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1f, WasherCircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2f, WasherCircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3f, WasherCircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4f, WasherCircleTest4f);

            //
            MtGeomArgument WasherFaceTest1d(Detail_008->GetFace(4), Item_008_008);
            MtGeomArgument WasherFaceTest2d(Detail_008->GetFace(5), Item_008_008);
            MtGeomArgument WasherFaceTest3d(Detail_008->GetFace(7), Item_008_008);
            MtGeomArgument WasherFaceTest4d(Detail_008->GetFace(0), Item_008_008);

            MtGeomArgument WasherCircleTest1d(WasherM16->GetFace(3), WasherM16_013);
            MtGeomArgument WasherCircleTest2d(WasherM16->GetFace(3), WasherM16_014);
            MtGeomArgument WasherCircleTest3d(WasherM16->GetFace(3), WasherM16_015);
            MtGeomArgument WasherCircleTest4d(WasherM16->GetFace(3), WasherM16_016);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1d, WasherCircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2d, WasherCircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3d, WasherCircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4d, WasherCircleTest4d);
        }
        // Совместимость ШайбыM16 - Фланец специальный внешние
        {
            MtGeomArgument WasherFaceTest1TT(Detail_008->GetFace(2), Item_008_005);

            MtGeomArgument WasherCircleTest1TT(WasherM16->GetFace(2), WasherM16_001);
            MtGeomArgument WasherCircleTest2TT(WasherM16->GetFace(2), WasherM16_002);
            MtGeomArgument WasherCircleTest3TT(WasherM16->GetFace(2), WasherM16_003);
            MtGeomArgument WasherCircleTest4TT(WasherM16->GetFace(2), WasherM16_004);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest4TT);

            //
            MtGeomArgument WasherFaceTest1TTs(Detail_008->GetFace(2), Item_008_006);

            MtGeomArgument WasherCircleTest1TTs(WasherM16->GetFace(2), WasherM16_005);
            MtGeomArgument WasherCircleTest2TTs(WasherM16->GetFace(2), WasherM16_006);
            MtGeomArgument WasherCircleTest3TTs(WasherM16->GetFace(2), WasherM16_007);
            MtGeomArgument WasherCircleTest4TTs(WasherM16->GetFace(2), WasherM16_008);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTs, WasherCircleTest4TTs);

            //
            MtGeomArgument WasherFaceTest1TTf(Detail_008->GetFace(2), Item_008_007);

            MtGeomArgument WasherCircleTest1TTf(WasherM16->GetFace(2), WasherM16_009);
            MtGeomArgument WasherCircleTest2TTf(WasherM16->GetFace(2), WasherM16_010);
            MtGeomArgument WasherCircleTest3TTf(WasherM16->GetFace(2), WasherM16_011);
            MtGeomArgument WasherCircleTest4TTf(WasherM16->GetFace(2), WasherM16_012);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTf, WasherCircleTest4TTf);

            //
            MtGeomArgument WasherFaceTest1TTd(Detail_008->GetFace(2), Item_008_008);

            MtGeomArgument WasherCircleTest1TTd(WasherM16->GetFace(2), WasherM16_013);
            MtGeomArgument WasherCircleTest2TTd(WasherM16->GetFace(2), WasherM16_014);
            MtGeomArgument WasherCircleTest3TTd(WasherM16->GetFace(2), WasherM16_015);
            MtGeomArgument WasherCircleTest4TTd(WasherM16->GetFace(2), WasherM16_016);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TTd, WasherCircleTest4TTd);
        }
        // Концентричность Болты M16 - Фланец специальный внешние
        {

            MtGeomArgument FaceTest1(WasherM16->GetFace(1), WasherM16_001);
            MtGeomArgument FaceTest2(WasherM16->GetFace(1), WasherM16_002);
            MtGeomArgument FaceTest3(WasherM16->GetFace(1), WasherM16_003);
            MtGeomArgument FaceTest4(WasherM16->GetFace(1), WasherM16_004);

            MtGeomArgument CircleTest1(BoltM16->GetFace(5), BoltM16_001);
            MtGeomArgument CircleTest2(BoltM16->GetFace(5), BoltM16_002);
            MtGeomArgument CircleTest3(BoltM16->GetFace(5), BoltM16_003);
            MtGeomArgument CircleTest4(BoltM16->GetFace(5), BoltM16_004);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1, CircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2, CircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3, CircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4, CircleTest4);

            //
            MtGeomArgument FaceTest1s(WasherM16->GetFace(1), WasherM16_005);
            MtGeomArgument FaceTest2s(WasherM16->GetFace(1), WasherM16_006);
            MtGeomArgument FaceTest3s(WasherM16->GetFace(1), WasherM16_007);
            MtGeomArgument FaceTest4s(WasherM16->GetFace(1), WasherM16_008);
                                              
            MtGeomArgument CircleTest1s(BoltM16->GetFace(5), BoltM16_005);
            MtGeomArgument CircleTest2s(BoltM16->GetFace(5), BoltM16_006);
            MtGeomArgument CircleTest3s(BoltM16->GetFace(5), BoltM16_007);
            MtGeomArgument CircleTest4s(BoltM16->GetFace(5), BoltM16_008);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1s, CircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2s, CircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3s, CircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4s, CircleTest4s);

            //
            MtGeomArgument FaceTest1f(WasherM16->GetFace(1), WasherM16_009);
            MtGeomArgument FaceTest2f(WasherM16->GetFace(1), WasherM16_010);
            MtGeomArgument FaceTest3f(WasherM16->GetFace(1), WasherM16_011);
            MtGeomArgument FaceTest4f(WasherM16->GetFace(1), WasherM16_012);
                                              
            MtGeomArgument CircleTest1f(BoltM16->GetFace(5), BoltM16_009);
            MtGeomArgument CircleTest2f(BoltM16->GetFace(5), BoltM16_010);
            MtGeomArgument CircleTest3f(BoltM16->GetFace(5), BoltM16_011);
            MtGeomArgument CircleTest4f(BoltM16->GetFace(5), BoltM16_012);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1f, CircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2f, CircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3f, CircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4f, CircleTest4f);

            //
            MtGeomArgument FaceTest1d(WasherM16->GetFace(1), WasherM16_013);
            MtGeomArgument FaceTest2d(WasherM16->GetFace(1), WasherM16_014);
            MtGeomArgument FaceTest3d(WasherM16->GetFace(1), WasherM16_015);
            MtGeomArgument FaceTest4d(WasherM16->GetFace(1), WasherM16_016);
                                              
            MtGeomArgument CircleTest1d(BoltM16->GetFace(5), BoltM16_013);
            MtGeomArgument CircleTest2d(BoltM16->GetFace(5), BoltM16_014);
            MtGeomArgument CircleTest3d(BoltM16->GetFace(5), BoltM16_015);
            MtGeomArgument CircleTest4d(BoltM16->GetFace(5), BoltM16_016);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1d, CircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2d, CircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3d, CircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4d, CircleTest4d);
        }
        // Совместимость Болты M16 - Фланец специальный внешние
        {
            MtGeomArgument FaceTest1TT(WasherM16->GetFace(0), WasherM16_001);
            MtGeomArgument FaceTest2TT(WasherM16->GetFace(0), WasherM16_002);
            MtGeomArgument FaceTest3TT(WasherM16->GetFace(0), WasherM16_003);
            MtGeomArgument FaceTest4TT(WasherM16->GetFace(0), WasherM16_004);
                                               
            MtGeomArgument CircleTest1TT(BoltM16->GetFace(6), BoltM16_001);
            MtGeomArgument CircleTest2TT(BoltM16->GetFace(6), BoltM16_002);
            MtGeomArgument CircleTest3TT(BoltM16->GetFace(6), BoltM16_003);
            MtGeomArgument CircleTest4TT(BoltM16->GetFace(6), BoltM16_004);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TT, CircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest2TT, CircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest3TT, CircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest4TT, CircleTest4TT);

            //
            MtGeomArgument FaceTest1TTs(WasherM16->GetFace(0), WasherM16_005);
            MtGeomArgument FaceTest2TTs(WasherM16->GetFace(0), WasherM16_006);
            MtGeomArgument FaceTest3TTs(WasherM16->GetFace(0), WasherM16_007);
            MtGeomArgument FaceTest4TTs(WasherM16->GetFace(0), WasherM16_008);
                                                
            MtGeomArgument CircleTest1TTs(BoltM16->GetFace(6), BoltM16_005);
            MtGeomArgument CircleTest2TTs(BoltM16->GetFace(6), BoltM16_006);
            MtGeomArgument CircleTest3TTs(BoltM16->GetFace(6), BoltM16_007);
            MtGeomArgument CircleTest4TTs(BoltM16->GetFace(6), BoltM16_008);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTs, CircleTest4TTs);

            //
            MtGeomArgument FaceTest1TTf(WasherM16->GetFace(0), WasherM16_009);
            MtGeomArgument FaceTest2TTf(WasherM16->GetFace(0), WasherM16_010);
            MtGeomArgument FaceTest3TTf(WasherM16->GetFace(0), WasherM16_011);
            MtGeomArgument FaceTest4TTf(WasherM16->GetFace(0), WasherM16_012);
                                                
            MtGeomArgument CircleTest1TTf(BoltM16->GetFace(6), BoltM16_009);
            MtGeomArgument CircleTest2TTf(BoltM16->GetFace(6), BoltM16_010);
            MtGeomArgument CircleTest3TTf(BoltM16->GetFace(6), BoltM16_011);
            MtGeomArgument CircleTest4TTf(BoltM16->GetFace(6), BoltM16_012);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTf, CircleTest4TTf);

            //
            MtGeomArgument FaceTest1TTd(WasherM16->GetFace(0), WasherM16_013);
            MtGeomArgument FaceTest2TTd(WasherM16->GetFace(0), WasherM16_014);
            MtGeomArgument FaceTest3TTd(WasherM16->GetFace(0), WasherM16_015);
            MtGeomArgument FaceTest4TTd(WasherM16->GetFace(0), WasherM16_016);
                                                
            MtGeomArgument CircleTest1TTd(BoltM16->GetFace(6), BoltM16_013);
            MtGeomArgument CircleTest2TTd(BoltM16->GetFace(6), BoltM16_014);
            MtGeomArgument CircleTest3TTd(BoltM16->GetFace(6), BoltM16_015);
            MtGeomArgument CircleTest4TTd(BoltM16->GetFace(6), BoltM16_016);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TTd, CircleTest4TTd);
        }
        // Концентричность Гайка M16 - Фланец специальный внутренний
        {

            MtGeomArgument setWasherFaceTest1(Detail_008->GetFace(4), Item_008_001);
            MtGeomArgument setWasherFaceTest2(Detail_008->GetFace(5), Item_008_001);
            MtGeomArgument setWasherFaceTest3(Detail_008->GetFace(7), Item_008_001);
            MtGeomArgument setWasherFaceTest4(Detail_008->GetFace(0), Item_008_001);
            MtGeomArgument setWasherCircleTest1(NutBoltM16->GetFace(2), NutBoltM16_001);
            MtGeomArgument setWasherCircleTest2(NutBoltM16->GetFace(2), NutBoltM16_002);
            MtGeomArgument setWasherCircleTest3(NutBoltM16->GetFace(2), NutBoltM16_003);
            MtGeomArgument setWasherCircleTest4(NutBoltM16->GetFace(2), NutBoltM16_004);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1, setWasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2, setWasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3, setWasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4, setWasherCircleTest4);

            //
            MtGeomArgument setWasherFaceTest1s(Detail_008->GetFace(4), Item_008_002);
            MtGeomArgument setWasherFaceTest2s(Detail_008->GetFace(5), Item_008_002);
            MtGeomArgument setWasherFaceTest3s(Detail_008->GetFace(7), Item_008_002);
            MtGeomArgument setWasherFaceTest4s(Detail_008->GetFace(0), Item_008_002);
            MtGeomArgument setWasherCircleTest1s(NutBoltM16->GetFace(2), NutBoltM16_005);
            MtGeomArgument setWasherCircleTest2s(NutBoltM16->GetFace(2), NutBoltM16_006);
            MtGeomArgument setWasherCircleTest3s(NutBoltM16->GetFace(2), NutBoltM16_007);
            MtGeomArgument setWasherCircleTest4s(NutBoltM16->GetFace(2), NutBoltM16_008);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1s, setWasherCircleTest1s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2s, setWasherCircleTest2s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3s, setWasherCircleTest3s);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4s, setWasherCircleTest4s);

            //
            MtGeomArgument setWasherFaceTest1f(Detail_008->GetFace(4), Item_008_003);
            MtGeomArgument setWasherFaceTest2f(Detail_008->GetFace(5), Item_008_003);
            MtGeomArgument setWasherFaceTest3f(Detail_008->GetFace(7), Item_008_003);
            MtGeomArgument setWasherFaceTest4f(Detail_008->GetFace(0), Item_008_003);
            MtGeomArgument setWasherCircleTest1f(NutBoltM16->GetFace(2), NutBoltM16_009);
            MtGeomArgument setWasherCircleTest2f(NutBoltM16->GetFace(2), NutBoltM16_010);
            MtGeomArgument setWasherCircleTest3f(NutBoltM16->GetFace(2), NutBoltM16_011);
            MtGeomArgument setWasherCircleTest4f(NutBoltM16->GetFace(2), NutBoltM16_012);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1f, setWasherCircleTest1f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2f, setWasherCircleTest2f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3f, setWasherCircleTest3f);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4f, setWasherCircleTest4f);

            //
            MtGeomArgument setWasherFaceTest1d(Detail_008->GetFace(4), Item_008_004);
            MtGeomArgument setWasherFaceTest2d(Detail_008->GetFace(5), Item_008_004);
            MtGeomArgument setWasherFaceTest3d(Detail_008->GetFace(7), Item_008_004);
            MtGeomArgument setWasherFaceTest4d(Detail_008->GetFace(0), Item_008_004);
            MtGeomArgument setWasherCircleTest1d(NutBoltM16->GetFace(2), NutBoltM16_013);
            MtGeomArgument setWasherCircleTest2d(NutBoltM16->GetFace(2), NutBoltM16_014);
            MtGeomArgument setWasherCircleTest3d(NutBoltM16->GetFace(2), NutBoltM16_015);
            MtGeomArgument setWasherCircleTest4d(NutBoltM16->GetFace(2), NutBoltM16_016);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1d, setWasherCircleTest1d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2d, setWasherCircleTest2d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3d, setWasherCircleTest3d);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4d, setWasherCircleTest4d);
        }
        // Совместимость Гайка M16 - Фланец специальный внутренний
        {
            MtGeomArgument setWasherFaceTest1TT(Detail_008->GetFace(2), Item_008_001);
            MtGeomArgument setWasherCircleTest1TT(NutBoltM16->GetFace(0), NutBoltM16_001);
            MtGeomArgument setWasherCircleTest2TT(NutBoltM16->GetFace(0), NutBoltM16_002);
            MtGeomArgument setWasherCircleTest3TT(NutBoltM16->GetFace(0), NutBoltM16_003);
            MtGeomArgument setWasherCircleTest4TT(NutBoltM16->GetFace(0), NutBoltM16_004);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest4TT);

            //
            MtGeomArgument setWasherFaceTest1TTs(Detail_008->GetFace(2), Item_008_002);
            MtGeomArgument setWasherCircleTest1TTs(NutBoltM16->GetFace(0), NutBoltM16_005);
            MtGeomArgument setWasherCircleTest2TTs(NutBoltM16->GetFace(0), NutBoltM16_006);
            MtGeomArgument setWasherCircleTest3TTs(NutBoltM16->GetFace(0), NutBoltM16_007);
            MtGeomArgument setWasherCircleTest4TTs(NutBoltM16->GetFace(0), NutBoltM16_008);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest1TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest2TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest3TTs);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTs, setWasherCircleTest4TTs);

            //
            MtGeomArgument setWasherFaceTest1TTf(Detail_008->GetFace(2), Item_008_003);
            MtGeomArgument setWasherCircleTest1TTf(NutBoltM16->GetFace(0), NutBoltM16_009);
            MtGeomArgument setWasherCircleTest2TTf(NutBoltM16->GetFace(0), NutBoltM16_010);
            MtGeomArgument setWasherCircleTest3TTf(NutBoltM16->GetFace(0), NutBoltM16_011);
            MtGeomArgument setWasherCircleTest4TTf(NutBoltM16->GetFace(0), NutBoltM16_012);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest1TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest2TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest3TTf);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTf, setWasherCircleTest4TTf);

            //
            MtGeomArgument setWasherFaceTest1TTd(Detail_008->GetFace(2), Item_008_004);
            MtGeomArgument setWasherCircleTest1TTd(NutBoltM16->GetFace(0), NutBoltM16_013);
            MtGeomArgument setWasherCircleTest2TTd(NutBoltM16->GetFace(0), NutBoltM16_014);
            MtGeomArgument setWasherCircleTest3TTd(NutBoltM16->GetFace(0), NutBoltM16_015);
            MtGeomArgument setWasherCircleTest4TTd(NutBoltM16->GetFace(0), NutBoltM16_016);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest1TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest2TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest3TTd);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TTd, setWasherCircleTest4TTd);
        }
        // Концентричность Гайка M16 - Шайба 16
        {
            MtGeomArgument sssetWasherFaceTest1(NutBoltM16->GetFace(2), NutBoltM16_001);
            MtGeomArgument sssetWasherCircleTest1(BoltM16->GetFace(5), BoltM16_001);

            assm->AddConstraint(GCM_CONCENTRIC, sssetWasherFaceTest1, sssetWasherCircleTest1);

            //
            MtGeomArgument sssetWasherFaceTest1s(NutBoltM16->GetFace(2), NutBoltM16_005);
            MtGeomArgument sssetWasherCircleTest1s(BoltM16->GetFace(5), BoltM16_005);

            assm->AddConstraint(GCM_CONCENTRIC, sssetWasherFaceTest1s, sssetWasherCircleTest1s);
            //
            MtGeomArgument sssetWasherFaceTest1f(NutBoltM16->GetFace(2), NutBoltM16_009);
            MtGeomArgument sssetWasherCircleTest1f(BoltM16->GetFace(5), BoltM16_009);

            assm->AddConstraint(GCM_CONCENTRIC, sssetWasherFaceTest1f, sssetWasherCircleTest1f);

            //
            MtGeomArgument sssetWasherFaceTest1d(NutBoltM16->GetFace(2), NutBoltM16_013);
            MtGeomArgument sssetWasherCircleTest1d(BoltM16->GetFace(5), BoltM16_013);

            assm->AddConstraint(GCM_CONCENTRIC, sssetWasherFaceTest1d, sssetWasherCircleTest1d);
        }
        // Концентричность ШайбыM8 - Фланец специальный внешние
        {

            MtGeomArgument WasherFaceTest1(Detail_008->GetFace(4), Item_008_018);
            MtGeomArgument WasherFaceTest2(Detail_008->GetFace(5), Item_008_018);
            MtGeomArgument WasherFaceTest3(Detail_008->GetFace(7), Item_008_018);
            MtGeomArgument WasherFaceTest4(Detail_008->GetFace(0), Item_008_018);

            MtGeomArgument WasherCircleTest1(WasherM8->GetFace(3), WasherM8_001);
            MtGeomArgument WasherCircleTest2(WasherM8->GetFace(3), WasherM8_002);
            MtGeomArgument WasherCircleTest3(WasherM8->GetFace(3), WasherM8_003);
            MtGeomArgument WasherCircleTest4(WasherM8->GetFace(3), WasherM8_004);

            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest1, WasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest2, WasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest3, WasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, WasherFaceTest4, WasherCircleTest4);
        }
        // Совместимость ШайбыM8 - Фланец специальный внешние
        {
            MtGeomArgument WasherFaceTest1TT(Detail_008->GetFace(2), Item_008_018);

            MtGeomArgument WasherCircleTest1TT(WasherM8->GetFace(2), WasherM8_001);
            MtGeomArgument WasherCircleTest2TT(WasherM8->GetFace(2), WasherM8_002);
            MtGeomArgument WasherCircleTest3TT(WasherM8->GetFace(2), WasherM8_003);
            MtGeomArgument WasherCircleTest4TT(WasherM8->GetFace(2), WasherM8_004);

            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, WasherFaceTest1TT, WasherCircleTest4TT);
        }
        // Концентричность Болты M8 - Фланец специальный внешние
        {

            MtGeomArgument FaceTest1(WasherM8->GetFace(1), WasherM8_001);
            MtGeomArgument FaceTest2(WasherM8->GetFace(1), WasherM8_002);
            MtGeomArgument FaceTest3(WasherM8->GetFace(1), WasherM8_003);
            MtGeomArgument FaceTest4(WasherM8->GetFace(1), WasherM8_004);

            MtGeomArgument CircleTest1(BoltM8->GetFace(5), BoltM8_001);
            MtGeomArgument CircleTest2(BoltM8->GetFace(5), BoltM8_002);
            MtGeomArgument CircleTest3(BoltM8->GetFace(5), BoltM8_003);
            MtGeomArgument CircleTest4(BoltM8->GetFace(5), BoltM8_004);

            assm->AddConstraint(GCM_CONCENTRIC, FaceTest1, CircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest2, CircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest3, CircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, FaceTest4, CircleTest4);
        }
        // Совместимость Болты M8 - Фланец специальный внешние
        {
            MtGeomArgument FaceTest1TT(WasherM8->GetFace(0), WasherM8_001);
            MtGeomArgument FaceTest2TT(WasherM8->GetFace(0), WasherM8_002);
            MtGeomArgument FaceTest3TT(WasherM8->GetFace(0), WasherM8_003);
            MtGeomArgument FaceTest4TT(WasherM8->GetFace(0), WasherM8_004);

            MtGeomArgument CircleTest1TT(BoltM8->GetFace(6), BoltM8_001);
            MtGeomArgument CircleTest2TT(BoltM8->GetFace(6), BoltM8_002);
            MtGeomArgument CircleTest3TT(BoltM8->GetFace(6), BoltM8_003);
            MtGeomArgument CircleTest4TT(BoltM8->GetFace(6), BoltM8_004);

            assm->AddConstraint(GCM_COINCIDENT, FaceTest1TT, CircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest2TT, CircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest3TT, CircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, FaceTest4TT, CircleTest4TT);
        }
        // Концентричность Гайка M8 - Фланец специальный внутренний
        {

            MtGeomArgument setWasherFaceTest1(Detail_008->GetFace(4), Item_008_017);
            MtGeomArgument setWasherFaceTest2(Detail_008->GetFace(5), Item_008_017);
            MtGeomArgument setWasherFaceTest3(Detail_008->GetFace(7), Item_008_017);
            MtGeomArgument setWasherFaceTest4(Detail_008->GetFace(0), Item_008_017);
            MtGeomArgument setWasherCircleTest1(NutBoltM8->GetFace(2), NutBoltM8_001);
            MtGeomArgument setWasherCircleTest2(NutBoltM8->GetFace(2), NutBoltM8_002);
            MtGeomArgument setWasherCircleTest3(NutBoltM8->GetFace(2), NutBoltM8_003);
            MtGeomArgument setWasherCircleTest4(NutBoltM8->GetFace(2), NutBoltM8_004);

            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest1, setWasherCircleTest1);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest2, setWasherCircleTest2);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest3, setWasherCircleTest3);
            assm->AddConstraint(GCM_CONCENTRIC, setWasherFaceTest4, setWasherCircleTest4);
        }
        // Совместимость Гайка M8 - Фланец специальный внутренний
        {
            MtGeomArgument setWasherFaceTest1TT(Detail_008->GetFace(2), Item_008_017);
            MtGeomArgument setWasherCircleTest1TT(NutBoltM8->GetFace(0), NutBoltM8_001);
            MtGeomArgument setWasherCircleTest2TT(NutBoltM8->GetFace(0), NutBoltM8_002);
            MtGeomArgument setWasherCircleTest3TT(NutBoltM8->GetFace(0), NutBoltM8_003);
            MtGeomArgument setWasherCircleTest4TT(NutBoltM8->GetFace(0), NutBoltM8_004);

            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest1TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest2TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest3TT);
            assm->AddConstraint(GCM_COINCIDENT, setWasherFaceTest1TT, setWasherCircleTest4TT);
        }
        }
#pragma endregion


        SPtr<MbInstance> Item_008_017(new MbInstance(*Detail_008, lcs));
        SPtr<MbInstance> Item_008_018(new MbInstance(*Detail_008, lcs));
    }

    assm->EvaluateConstraints();

    string zero    = "0";
    string first   = "1";
    string second  = "2";
    string third   = "3";
    string forth   = "4";
    string fifth   = "5";
    string six     = "6";
    string seven   = "7";
    string eight   = "8";
    string nine    = "9";
    string ten     = "10";

    MbProductInfo zeroProductInfo    (false, zero, zero, zero);
    MbProductInfo firstProductInfo   (false, first, first, first);
    MbProductInfo secondProductInfo  (false, second, second, second);
    MbProductInfo thirdProductInfo   (false, third, third, third);
    MbProductInfo forthProductInfo   (false, forth, forth, forth);
    MbProductInfo fifthProductInfo   (false, fifth, fifth, fifth);
    MbProductInfo sixProductInfo     (false, six, six, six);
    MbProductInfo sevenProductInfo   (false, seven, seven, seven);
    MbProductInfo eightProductInfo   (false, eight, eight, eight);
    MbProductInfo nineProductInfo    (false, nine, nine, nine);
    MbProductInfo tenProductInfo     (false, ten, ten, ten);

    MbProductInfo AssemblyInfo(false, "TTOR", "TTOR", "TTOR");

    Item_001_001->AddAttribute(zeroProductInfo);
    Item_001_002->AddAttribute(zeroProductInfo);
    Item_001_003->AddAttribute(zeroProductInfo);
    Item_001_004->AddAttribute(zeroProductInfo);
    Item_002_001->AddAttribute(firstProductInfo);
    Item_002_002->AddAttribute(firstProductInfo);
    Item_002_003->AddAttribute(firstProductInfo);
    Item_002_004->AddAttribute(firstProductInfo);
    Item_003_001->AddAttribute(secondProductInfo);
    Item_003_002->AddAttribute(secondProductInfo);
    Item_004_001->AddAttribute(thirdProductInfo);
    Item_005_001->AddAttribute(forthProductInfo);
    Item_006_001->AddAttribute(fifthProductInfo);
    Item_006_002->AddAttribute(fifthProductInfo);
    Item_006_003->AddAttribute(fifthProductInfo);
    Item_006_004->AddAttribute(fifthProductInfo);
    Item_007_001->AddAttribute(sixProductInfo);
    Item_007_002->AddAttribute(sixProductInfo);
    Item_007_003->AddAttribute(sixProductInfo);
    Item_007_004->AddAttribute(sixProductInfo);
    Item_008_001->AddAttribute(sevenProductInfo);
    Item_008_002->AddAttribute(sevenProductInfo);
    Item_008_003->AddAttribute(sevenProductInfo);
    Item_008_004->AddAttribute(sevenProductInfo);
    Item_008_005->AddAttribute(sevenProductInfo);
    Item_008_006->AddAttribute(sevenProductInfo);
    Item_008_007->AddAttribute(sevenProductInfo);
    Item_008_008->AddAttribute(sevenProductInfo);
    Item_008_009->AddAttribute(sevenProductInfo);
    Item_008_010->AddAttribute(sevenProductInfo);
    Item_008_011->AddAttribute(sevenProductInfo);
    Item_008_012->AddAttribute(sevenProductInfo);
    Item_008_013->AddAttribute(sevenProductInfo);
    Item_008_014->AddAttribute(sevenProductInfo);
    Item_008_015->AddAttribute(sevenProductInfo);
    Item_008_016->AddAttribute(sevenProductInfo);
    Item_008_017->AddAttribute(sevenProductInfo);
    Item_008_018->AddAttribute(sevenProductInfo);
    Item_009_001->AddAttribute(eightProductInfo);
    Item_009_002->AddAttribute(eightProductInfo);
    Item_009_003->AddAttribute(eightProductInfo);
    Item_010_001->AddAttribute(nineProductInfo);
    Item_011_001->AddAttribute(tenProductInfo);

    assm->AddAttribute(AssemblyInfo);
    assm->Rotate(axVert, -M_PI / 2);
    if (turnOnStandart) {
        assm->Rotate(azVert, -M_PI / 2);
    }

    if (configurationQuantity > 0)
    {
        std::vector<SPtr<MbItem>> assmPairs;


        SPtr<MbSolid> Detail_012(Zarubincreate_012_curevedTubeBig(ttDiam, ttThickness, visotaOpori, t));

        Detail_012->Rotate(ayVert, M_PI);
        Detail_012->Move(MbVector3D(75, -825 + assemblyHeightTTOR,- 657.5));

        for (int i = 0; i < configurationQuantity + 1; ++i)
        {
            SPtr<MbInstance> assmInstance(new MbInstance(*assm, lcs));

            SPtr<MbInstance> Item_012_001(new MbInstance(*Detail_012, lcs));

            assmPairs.push_back(assmInstance);

            if (i >= 1)
            {
                Item_012_001->Move(MbVector3D(0, -assemblyHeightTTOR * i, 0));
                assmPairs.push_back(Item_012_001);
                assmInstance->Move(MbVector3D(0, -assemblyHeightTTOR* i, 0));
            }
        }

        SPtr<MbAssembly> assmBlock(new MbAssembly(assmPairs));

        return assmBlock;
    }

    return assm;
}

SPtr<MbAssembly> ParametricModelCreator::CreateIP(ConfigParams_IP params)
{
    // Кожух
    const double Dvne = params.Dvne;
    const double L_Base = params.L_Base;
    const double s = params.s;
    const double p = params.p;
    const double Dn = params.Dn;
    // Камера
    const double L = 622;
    const double DKr = params.DKr;

    //Кольцо плавающей головки
    double D_ring = 396;//внутренный диаметр колца
    double s1_ring = 40;// толщина фланца
    double S_ring = 30;//толщина кольца

    SPtr<MbSolid> pKozhuh(CreateUnionKzh_IP(params.Dvne, params.L_Base, params.s, params.p, params.DKr, params.Dn, params.Dy)); //Кожух
    SPtr<MbSolid> pKamera(CreateUnionKamera_IP(L, params.Dvne, params.p, params.DKr, params.Dn, params.Dy, params.s)); //Камера

    SPtr<MbSolid> pKrKameraEl(CreateKrKameraEll_IP(params));//Крышка на камере эллипс
    SPtr<MbSolid> pKrKamera(CreateUnionCover_IP(params.Dy, params.p, 6, params.Dvne, params.DKr)); //Крышка на камере снизу
    SPtr<MbSolid> pKrKamera1(CreateUnionCover_IP(params.Dy, params.p, 7, params.Dvne, params.DKr)); //Крышка на камере вверху
    SPtr<MbSolid> pKrBig(CreateUnionCover_IP(params.Dy, params.p, 11, params.Dvne, params.DKr)); //Крышка самая большая
    SPtr<MbSolid> pKrVtorayaSverhu(CreateUnionCover_IP(params.Dy, params.p, 10, params.Dvne, params.DKr)); //Крышка вторая сверху
    SPtr<MbSolid> pKrTretyaSverhu(CreateUnionCover_IP(params.Dy, params.p, 9, params.Dvne, params.DKr)); //Крышка третья сверху
    SPtr<MbSolid> pKrFirstNis(CreateUnionCover_IP(params.Dy, params.p, 3, params.Dvne, params.DKr)); //Крышка первая внизу
    SPtr<MbSolid> pKrsamSmall(CreateUnionCover_IP(params.Dy, params.p, 4, params.Dvne, params.DKr)); //Крышка вторая внизу
    SPtr<MbSolid> pKrNaSkose(CreateUnionCover_IP(params.Dy, params.p, 5, params.Dvne, params.DKr)); //Крышка третья внизу
    SPtr<MbSolid> pKrSboku(CreateUnionCover_IP(params.Dy, params.p, 1, params.Dvne, params.DKr)); //Крышка сбоку
    SPtr<MbSolid> pStoika(CreateStoyka_IP(params.Dvne, params.L_Base)); //Cтойка
    SPtr<MbSolid> pReshNePod(CreateOsnovaNePodResh_IP(params.Dvne, params.p, params.DKr)); //Неподвижная решетка
    SPtr<MbSolid> pPeregorodka(CreateOsnovaPeregorodka_IP(params.Dvne, params.p, params.DKr /*S1,D3*/));//Перегородка
    SPtr<MbSolid> pReshetkaPod(CreateOsnovaReshPod_IP(params.Dvne, params.p, params.DKr /*S,D2,D5*/));//Подвижная решетка
    SPtr<MbSolid> pRingPL(CreateRingOn_IP(params.Dvne, params.p, params.DKr));//Кольцо плавающей головки
    SPtr<MbSolid> pKrPlGol(CreateKrPlGol_IP(params));//Крышка плавающей головки
    SPtr<MbSolid> pPipe1(CreatePipe_IP(params.Dvne, params.p, params.DKr));//Труба

    pKozhuh->SetColor(255, 204, 255);
    pKamera->SetColor(255, 204, 255);
    pKrKamera->SetColor(255, 204, 255);
    pKrKamera1->SetColor(255, 204, 255);
    pKrBig->SetColor(255, 204, 255);
    pKrVtorayaSverhu->SetColor(255, 204, 255);
    pKrTretyaSverhu->SetColor(255, 204, 255);
    pKrFirstNis->SetColor(255, 204, 255);
    pKrsamSmall->SetColor(255, 204, 255);
    pKrNaSkose->SetColor(255, 204, 255);
    pKrSboku->SetColor(255, 204, 255);
    pStoika->SetColor(255, 204, 255);

    MbAxis3D axVert(MbVector3D(1, 0, 0));
    MbAxis3D ayVert(MbVector3D(0, 1, 0));
    MbAxis3D azVert(MbVector3D(0, 0, 1));
    pKrKamera1->Rotate(axVert, M_PI);
    //  pKrKamera1->Rotate(axVert, M_PI);
    pKrBig->Rotate(axVert, M_PI);
    pKrVtorayaSverhu->Rotate(axVert, M_PI);
    pKrTretyaSverhu->Rotate(axVert, M_PI);
    pKrSboku->Rotate(azVert, -M_PI / 2);

    pReshNePod->Rotate(ayVert, M_PI / 2);
    pReshNePod->Rotate(azVert, M_PI / 2);
    pPeregorodka->Rotate(ayVert, M_PI / 2);
    pPeregorodka->Rotate(azVert, M_PI / 2);
    pKrPlGol->Rotate(azVert, M_PI);
    pKrPlGol->Rotate(ayVert, -M_PI);
    // pPipe1->Rotate(axVert, M_PI );
   //  pPipe1->Rotate(ayVert, M_PI/2);

    pReshetkaPod->Rotate(ayVert, M_PI / 2);
    pReshetkaPod->Rotate(azVert, M_PI / 2);
    /*
         if (params.Dvne == 840) {
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 366 / 2) - 180, 0)));
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L/2 - 86 - 700 + 500/2, 0, 0)));
         }
          else if (params.Dvne == 1040) {
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 470 / 2) - 180, 0)));
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L / 2 - 86 - 700 + 500 / 2, 0, 0)));
        }
          else if (params.Dvne == 1240) {
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 568 / 2) - 180, 0)));
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L / 2 - 86 - 700 + 500 / 2, 0, 0)));
        }
         else if (params.Dvne == 1440) {
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 660 / 2) - 180, 0)));
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L / 2 - 86 - 700 + 500 / 2, 0, 0)));
        }
         else if (params.Dvne == 1640) {
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 760 / 2) - 180, 0)));
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L / 2 - 86 - 700 + 500 / 2, 0, 0)));
        }
         else if ((params.Dvne == 1840) && (params.DKr == 1000)) {
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 862 / 2) - 180, 0)));
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L / 2 - 86 - 700 + 500 / 2, 0, 0)));
        }
         else  if ((params.Dvne == 1840) && (params.DKr == 1100)) {
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 962 / 2) - 180, 0)));
            pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L / 2 - 86 - 700 + 500 / 2, 0, 0)));
        }
         else  if (params.Dvne == 2040) {
             pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -(params.Dvne - 1060 / 2) - 180, 0)));
             pPipe1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(params.L_Base - 6000 - L / 2 - 86 - 700 + 500 / 2, 0, 0)));
         }*/
    MbPlacement3D lcs;
    SPtr<MbInstance> comp1(new MbInstance(*pKozhuh, lcs));
    SPtr<MbInstance> comp2(new MbInstance(*pKamera, lcs));
    SPtr<MbInstance> comp3(new MbInstance(*pKrKameraEl, lcs));
    SPtr<MbInstance> comp4(new MbInstance(*pKrKamera, lcs));
    SPtr<MbInstance> comp5(new MbInstance(*pKrKamera1, lcs));
    SPtr<MbInstance> comp6(new MbInstance(*pKrBig, lcs));
    SPtr<MbInstance> comp7(new MbInstance(*pKrVtorayaSverhu, lcs));
    SPtr<MbInstance> comp8(new MbInstance(*pKrTretyaSverhu, lcs));
    SPtr<MbInstance> comp9(new MbInstance(*pKrFirstNis, lcs));
    SPtr<MbInstance> comp10(new MbInstance(*pKrsamSmall, lcs));
    SPtr<MbInstance> comp11(new MbInstance(*pKrNaSkose, lcs));
    SPtr<MbInstance> comp12(new MbInstance(*pKrSboku, lcs));
    SPtr<MbInstance> comp13(new MbInstance(*pStoika, lcs));
    SPtr<MbInstance> comp14(new MbInstance(*pStoika, lcs));
    SPtr<MbInstance> comp15(new MbInstance(*pReshNePod, lcs));
    SPtr<MbInstance> comp16(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp17(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp18(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp19(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp20(new MbInstance(*pReshetkaPod, lcs));
    SPtr<MbInstance> comp21(new MbInstance(*pRingPL, lcs));
    SPtr<MbInstance> comp22(new MbInstance(*pKrPlGol, lcs));

    // трубы
    SPtr<MbInstance> comp23(new MbInstance(*pPipe1, lcs));

    std::vector<SPtr<MbInstance>> pair;
    pair.push_back(comp1);
    pair.push_back(comp2);
    pair.push_back(comp3);
    pair.push_back(comp4);
    pair.push_back(comp5);
    pair.push_back(comp6);
    pair.push_back(comp7);
    pair.push_back(comp8);
    pair.push_back(comp9);
    pair.push_back(comp10);
    pair.push_back(comp11);
    pair.push_back(comp12);
    pair.push_back(comp13);
    pair.push_back(comp14);
    pair.push_back(comp15);
    pair.push_back(comp16);
    pair.push_back(comp17);
    pair.push_back(comp18);
    pair.push_back(comp19);
    pair.push_back(comp20);
    pair.push_back(comp21);
    pair.push_back(comp22);
    pair.push_back(comp23);

    SPtr<MbAssembly> assm(new MbAssembly(pair));

    {

        //Концентричность кожуха и камеры
        MtGeomArgument face11(pKozhuh->GetFace(25), comp1);
        MtGeomArgument face21(pKamera->GetFace(43), comp2);
        assm->AddConstraint(GCM_CONCENTRIC, face11, face21);

        // Концентричность камеры и эл крышки
        //face22 - камера
        MtGeomArgument face33(pKrKameraEl->GetFace(5), comp3);
        MtGeomArgument face22(pKamera->GetFace(7), comp2);
        assm->AddConstraint(GCM_CONCENTRIC, face22, face33);

        //Линейный размер между камерой и эл крышкой
        MtGeomArgument face23(pKamera->GetFace(6), comp2);
        MtGeomArgument face32(pKrKameraEl->GetFace(0), comp3);
        assm->AddConstraint(GCM_DISTANCE, face23, face32, 0.0);


        // Концентричность камеры и крышки
        MtGeomArgument face222(pKamera->GetFace(80), comp2);
        MtGeomArgument face44(pKrKamera->GetFace(1), comp4);
        assm->AddConstraint(GCM_CONCENTRIC, face222, face44);

        //Линейный размер между камерой и крышкой
        MtGeomArgument face24(pKamera->GetFace(9), comp2);
        MtGeomArgument face42(pKrKamera->GetFace(14), comp4);
        assm->AddConstraint(GCM_DISTANCE, face24, face42, 0.0);


        // Концентричность камеры и крышки вверху
        MtGeomArgument face2222(pKamera->GetFace(69), comp2);
        MtGeomArgument face55(pKrKamera1->GetFace(1), comp5);
        assm->AddConstraint(GCM_CONCENTRIC, face2222, face55);

        //Линейный размер между камерой и крышкой вверху
        MtGeomArgument face25(pKamera->GetFace(12), comp2);
        MtGeomArgument face52(pKrKamera1->GetFace(14), comp5);
        assm->AddConstraint(GCM_DISTANCE, face25, face52, 0.0);


        // Концентричность кожуха и самой большой крышки
        MtGeomArgument face111(pKozhuh->GetFace(91), comp1);
        MtGeomArgument face66(pKrBig->GetFace(1), comp6);
        assm->AddConstraint(GCM_CONCENTRIC, face111, face66);

        //Линейный размер между кожухом и самой большой крышкой
        MtGeomArgument face16(pKozhuh->GetFace(57), comp1);
        MtGeomArgument face61(pKrBig->GetFace(24), comp6);
        assm->AddConstraint(GCM_DISTANCE, face16, face61, 0.0);


        // Концентричность кожуха и второй крышки сверху
        MtGeomArgument face1111(pKozhuh->GetFace(113), comp1);
        MtGeomArgument face77(pKrVtorayaSverhu->GetFace(1), comp7);
        assm->AddConstraint(GCM_CONCENTRIC, face1111, face77);

        //Линейный размер между кожухом и второй крышкой сверху
        MtGeomArgument face17(pKozhuh->GetFace(58), comp1);
        MtGeomArgument face71(pKrVtorayaSverhu->GetFace(8), comp7);
        assm->AddConstraint(GCM_DISTANCE, face17, face71, 0.0);


        // Концентричность кожуха и третьей крышки сверху
        MtGeomArgument face11111(pKozhuh->GetFace(118), comp1);
        MtGeomArgument face88(pKrTretyaSverhu->GetFace(1), comp8);
        assm->AddConstraint(GCM_CONCENTRIC, face11111, face88);

        //Линейный размер между кожухом и третьей крышкой сверху
        MtGeomArgument face18(pKozhuh->GetFace(59), comp1);
        MtGeomArgument face81(pKrTretyaSverhu->GetFace(14), comp8);
        assm->AddConstraint(GCM_DISTANCE, face18, face81, 0.0);



        // Концентричность кожуха и ПЕРВОЙ крышки СНИЗУ
        MtGeomArgument face100(pKozhuh->GetFace(131), comp1);
        MtGeomArgument face99(pKrFirstNis->GetFace(1), comp9);
        assm->AddConstraint(GCM_CONCENTRIC, face100, face99);

        //Линейный размер между кожухом и ПЕРВОЙ крышкой СНИЗУ
        MtGeomArgument face19(pKozhuh->GetFace(61), comp1);
        MtGeomArgument face91(pKrFirstNis->GetFace(12), comp9);
        assm->AddConstraint(GCM_DISTANCE, face19, face91, 0.0);


        // Концентричность кожуха и ВТОРОЙ крышки СНИЗУ
        MtGeomArgument faceKK(pKozhuh->GetFace(140), comp1);
        MtGeomArgument face10(pKrsamSmall->GetFace(1), comp10);
        assm->AddConstraint(GCM_CONCENTRIC, faceKK, face10);

        //Линейный размер между кожухом и ВТОРОЙ крышкой СНИЗУ
        MtGeomArgument face110(pKozhuh->GetFace(62), comp1);
        MtGeomArgument face101(pKrsamSmall->GetFace(8), comp10);
        assm->AddConstraint(GCM_DISTANCE, face110, face101, 0.0);


        // Концентричность кожуха и ТРЕТЬЕЙ крышки СНИЗУ
        MtGeomArgument faceK(pKozhuh->GetFace(71), comp1);
        MtGeomArgument face3kr(pKrNaSkose->GetFace(1), comp11);
        assm->AddConstraint(GCM_CONCENTRIC, faceK, face3kr);

        //Линейный размер между кожухом и ТРЕТЬЕЙ крышкой СНИЗУ
        MtGeomArgument faceK11(pKozhuh->GetFace(23), comp1);
        MtGeomArgument face11K(pKrNaSkose->GetFace(14), comp11);
        assm->AddConstraint(GCM_DISTANCE, face11K, faceK11, 0.0);


        // Концентричность кожуха и крышкей СБОКУ
        MtGeomArgument faceH(pKozhuh->GetFace(151), comp1);
        MtGeomArgument faceS(pKrSboku->GetFace(1), comp12);
        assm->AddConstraint(GCM_CONCENTRIC, faceH, faceS);

        //Линейный размер между кожухом и крышкей СБОКУ
        MtGeomArgument faceSH(pKozhuh->GetFace(87), comp1);
        MtGeomArgument faceHS(pKrSboku->GetFace(14), comp12);
        assm->AddConstraint(GCM_DISTANCE, faceSH, faceHS, 0.0);


        // Концентричность кожуха и СТОЙКИ
        MtGeomArgument facek(pKozhuh->GetFace(4), comp1);//,было 6
        MtGeomArgument faceSt(pStoika->GetFace(25), comp13);
        assm->AddConstraint(GCM_CONCENTRIC, facek, faceSt);

        //Линейный размер между кожухом и СТОЙКОЙ
        MtGeomArgument faceKo(pKozhuh->GetFace(87), comp1);
        MtGeomArgument faceKoSt(pStoika->GetFace(5), comp13);
        assm->AddConstraint(GCM_DISTANCE, faceKo, faceKoSt, -(params.L_Base - 300) / 1.23 / 4);


        // Концентричность кожуха и СТОЙКИ 1
        MtGeomArgument faceSt1(pStoika->GetFace(25), comp14);
        assm->AddConstraint(GCM_CONCENTRIC, facek, faceSt1);

        //Линейный размер между кожухом и СТОЙКОЙ 1
        MtGeomArgument faceKoSt1(pStoika->GetFace(5), comp14);
        assm->AddConstraint(GCM_DISTANCE, faceKo, faceKoSt1, -(params.L_Base - 300) / 1.23);

        // Концентричность камеры и Решетки непод
        MtGeomArgument face233(pReshNePod->GetFace(9), comp15);
        assm->AddConstraint(GCM_CONCENTRIC, face21, face233);

        //Линейный размер между камерой и неплдвижной решеткой
        MtGeomArgument face144(pKamera->GetFace(7), comp2);
        MtGeomArgument face244(pReshNePod->GetFace(4), comp15);
        assm->AddConstraint(GCM_DISTANCE, face144, face244, 0.0);

        //Линейный размер между кожухом и неподвижной решеткой
        MtGeomArgument facekNepod(pKozhuh->GetFace(8), comp1);
        MtGeomArgument facekNe1pod(pReshNePod->GetFace(10), comp15);
        assm->AddConstraint(GCM_DISTANCE, facekNepod, facekNe1pod, 0.0);

        //Концентричность между неподвижной решеткой и перегородкой
        MtGeomArgument faceNe2(pReshNePod->GetFace(1), comp15);
        MtGeomArgument facePer1(pPeregorodka->GetFace(2), comp16);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer1);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument faceNe2Per1(pReshNePod->GetFace(0), comp15);
        MtGeomArgument facePer1Ne2(pPeregorodka->GetFace(0), comp16);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer1Ne2, 1160.0);

        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePer2(pPeregorodka->GetFace(2), comp17);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer2);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument facePer2Ne2(pPeregorodka->GetFace(0), comp17);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer2Ne2, 2350.0);


        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePer3(pPeregorodka->GetFace(2), comp18);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer3);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument facePer3Ne2(pPeregorodka->GetFace(0), comp18);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer3Ne2, 3550.0);


        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePer4(pPeregorodka->GetFace(2), comp19);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer4);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument facePer4Ne2(pPeregorodka->GetFace(0), comp19);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer4Ne2, 4750.0);


        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePRESH(pReshetkaPod->GetFace(5), comp20);
        MtGeomArgument faceNe1Ne(pReshNePod->GetFace(9), comp15);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe1Ne, facePRESH);


        //Линейный размер между Решеткой непод и Решеткой под
        MtGeomArgument faceNePo(pReshNePod->GetFace(0), comp15);
        MtGeomArgument facePoNe(pReshetkaPod->GetFace(1), comp20); //1
        assm->AddConstraint(GCM_DISTANCE, faceNePo, facePoNe, 5920.0);

        // Концентричность Решетки под и кольца плавающей головки
        MtGeomArgument facePerRing(pRingPL->GetFace(14), comp21);
        MtGeomArgument facePo1(pReshetkaPod->GetFace(5), comp20); //5
        assm->AddConstraint(GCM_CONCENTRIC, facePerRing, facePo1);

        //Линейный размер между кольцом и Решеткой под
        MtGeomArgument facePerRing555(pRingPL->GetFace(17), comp21);
        MtGeomArgument facePerKrPl2555(pReshetkaPod->GetFace(4), comp20);
        assm->AddConstraint(GCM_DISTANCE, facePerRing555, facePerKrPl2555, 0.0);


        // Концентричность между крышкой и подвижной решеткой
        MtGeomArgument facePerKrPl(pKrPlGol->GetFace(5), comp22);
        assm->AddConstraint(GCM_CONCENTRIC, facePerKrPl, facePo1);

        // Линейниый размер между крышкой и подвижной решеткой 
        MtGeomArgument facePerKrPl1(pKrPlGol->GetFace(18), comp22);
        MtGeomArgument facePerKrPl2(pReshetkaPod->GetFace(6), comp20);
        assm->AddConstraint(GCM_DISTANCE, facePerKrPl1, facePerKrPl2, 0.0);


        int face = 0;
        switch (params.Dvne) {
        case 800:
            face = 200;
            break;
        case 1000:
            face = 109;
            break;
        case 1200:
            face = 252;
            break;
        case 1400:
            face = 352;
            break;
        case 1600:
            face = 452;
            break;
        case 1800:
            face = 952;
            break;
        case 2000:
            face = 1052;
            break;
        }

        MtGeomArgument facePerTube(pReshetkaPod->GetFace(1), comp20);
        MtGeomArgument faceTubePer(pPipe1->GetFace(face), comp23);//152
        assm->AddConstraint(GCM_PARALLEL, facePerTube, faceTubePer, 40);


    }
    assm->EvaluateConstraints();

    return assm;
}

SPtr<MbAssembly> ParametricModelCreator::CreateIU(ConfigParams_IU params)
{
    // TODO: 3) Здесь деструктуризируете ваши переменные и уже вставляете куда вам нужно


    SPtr<MbSolid> pKozhuh(CreateUnionKozhuh_IU(params)); //Кожух
    SPtr<MbSolid> pKamera(CreateKamera(params)); //Камера
    // SPtr<MbSolid> pKrKameraEl(CreateUnionCover());//Крышка на камере эллипс
    SPtr<MbSolid> pKrKamera(CreateUnionCover(150, params.p, 6, params.diamVne, params.D_Kam)); //Крышка на камере снизу
    SPtr<MbSolid> pKrKamera1(CreateUnionCover(150, params.p, 7, params.diamVne, params.D_Kam)); //Крышка на камере вверху
    SPtr<MbSolid> pKrBig(CreateUnionCover(500, params.p, 11, params.diamVne, params.D_Kam)); //Крышка самая большая
    SPtr<MbSolid> pKrVtorayaSverhu(CreateUnionCover(80, params.p, 10, params.diamVne, params.D_Kam)); //Крышка вторая сверху
    SPtr<MbSolid> pKrTretyaSverhu(CreateUnionCover(250, params.p, 9, params.diamVne, params.D_Kam)); //Крышка третья сверху
    SPtr<MbSolid> pKrFirstNis(CreateUnionCover(100, params.p, 3, params.diamVne, params.D_Kam)); //Крышка первая внизу
    SPtr<MbSolid> pKrsamSmall(CreateUnionCover(60, params.p, 4, params.diamVne, params.D_Kam)); //Крышка вторая внизу
    SPtr<MbSolid> pKrNaSkose(CreateUnionCover(200, params.p, 5, params.diamVne, params.D_Kam)); //Крышка третья внизу
    SPtr<MbSolid> pKrSboku(CreateUnionCover(200, params.p, 1, params.diamVne, params.D_Kam)); //Крышка сбоку
    SPtr<MbSolid> pStoika(CreateStoyka(params.diamVne, (params.Length - 300) / 1.23)); //Cтойка

    SPtr<MbSolid> pReshetkaNEPod(CreateOsnovaNePodResh(params.diam, params.p, params.D_Kam));
    //SPtr<MbSolid> pReshetkaPod(CreateOsnovaReshPod(params.diamVne, params.p, params.D_Kam));
    SPtr<MbSolid> pPeregorodka(CreateOsnovaPeregorodka(params.diam, params.p, params.D_Kam));

    SPtr<MbSolid> pPipe(CreatePipe(25, 20, params.p, params.diamVne, params.D_Kam));



    pKozhuh->SetColor(135, 206, 250);
    pKamera->SetColor(135, 206, 250);
    pKrKamera->SetColor(135, 206, 250);
    pKrKamera1->SetColor(135, 206, 250);
    pKrBig->SetColor(135, 206, 250);
    pKrVtorayaSverhu->SetColor(135, 206, 250);
    pKrTretyaSverhu->SetColor(135, 206, 250);
    pKrFirstNis->SetColor(135, 206, 250);
    pKrsamSmall->SetColor(135, 206, 250);
    pKrNaSkose->SetColor(135, 206, 250);
    pKrSboku->SetColor(135, 206, 250);
    pStoika->SetColor(135, 206, 250);

    pPipe->SetColor(153, 153, 255);
    //pPeregorodka->SetColor(153, 153, 255);




    MbAxis3D axVert(MbVector3D(1, 0, 0));
    MbAxis3D ayVert(MbVector3D(0, 1, 0));
    MbAxis3D azVert(MbVector3D(0, 0, 1));
    pKrKamera->Rotate(axVert, M_PI);
    pKrBig->Rotate(axVert, M_PI);
    pKrVtorayaSverhu->Rotate(axVert, M_PI);
    pKrTretyaSverhu->Rotate(axVert, M_PI);
    pKamera->Rotate(ayVert, M_PI);
    pReshetkaNEPod->Rotate(ayVert, M_PI / 2);
    pReshetkaNEPod->Rotate(azVert, M_PI / 2);
    //pReshetkaPod->Rotate(ayVert, M_PI / 2);
    //pReshetkaPod->Rotate(azVert, M_PI / 2);
    pPeregorodka->Rotate(ayVert, M_PI / 2);
    pPeregorodka->Rotate(azVert, M_PI / 2);




    pKrSboku->Rotate(axVert, M_PI);
    pKrSboku->Rotate(azVert, M_PI / 2);

    //pPipe->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-700+1200-420/2, -(params.diamVne-372/2)-180, 0)));


    double bigD;
    if (params.p == 1.6) {
        switch (params.diamVne) {
        case 840:
            bigD = 366;
            break;
        case 1040:
            bigD = 462;
            break;
        case 1240:
            bigD = 560;
            break;
        case 1440:
            bigD = 652;
            break;
        case 1640:
            bigD = 755; //752
            break;
        case 1840:
            if (params.D_Kam == 1000) {
                bigD = 854;
                break;
            }
            if (params.D_Kam == 1100) {
                bigD = 954;
                break;
            }
        case 2040:
            bigD = 1052;
            break;
        }
    }


    int l5;

    switch (params.D_Kam) {
    case 500:
        l5 = 320;
        break;
    case 600:
        l5 = 380;
        break;
    case 700:
        l5 = 440;
        break;
    case 800:
        l5 = 500;
        break;
    case 900:
        l5 = 560;
        break;
    case 1000:
        l5 = 570;
        break;
    case 1100:
        l5 = 600;
        break;
    case 1200:
        l5 = 610;
        break;
    }

    double pol_Kam = l5 - params.D_Kam / 4;

    double L_Kzh = params.Length - pol_Kam;


    //что если двигать влево, а потом вправо до камеры
    double b = 40;
    // pPipe->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-700 + (params.Length - 300) / 5.23 + (params.Length - 300) / 1.23 + b - 5950.0 - bigD / 2, -(params.diamVne - 372 / 2) - 180, 0)));


    MbPlacement3D lcs;
    SPtr<MbInstance> comp1(new MbInstance(*pKozhuh, lcs));
    SPtr<MbInstance> comp2(new MbInstance(*pKamera, lcs));
    //SPtr<MbInstance> comp3(new MbInstance(*pKrKameraEl, lcs));
    SPtr<MbInstance> comp4(new MbInstance(*pKrKamera, lcs));
    SPtr<MbInstance> comp5(new MbInstance(*pKrKamera1, lcs));
    SPtr<MbInstance> comp6(new MbInstance(*pKrBig, lcs));
    SPtr<MbInstance> comp7(new MbInstance(*pKrVtorayaSverhu, lcs));
    SPtr<MbInstance> comp8(new MbInstance(*pKrTretyaSverhu, lcs));
    SPtr<MbInstance> comp9(new MbInstance(*pKrFirstNis, lcs));
    SPtr<MbInstance> comp10(new MbInstance(*pKrsamSmall, lcs));
    SPtr<MbInstance> comp11(new MbInstance(*pKrNaSkose, lcs));
    SPtr<MbInstance> comp12(new MbInstance(*pKrSboku, lcs));
    SPtr<MbInstance> comp13(new MbInstance(*pStoika, lcs));
    SPtr<MbInstance> comp14(new MbInstance(*pStoika, lcs));
    SPtr<MbInstance> comp15(new MbInstance(*pReshetkaNEPod, lcs));
    SPtr<MbInstance> comp16(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp17(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp18(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp19(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp20(new MbInstance(*pPeregorodka, lcs));
    SPtr<MbInstance> comp21(new MbInstance(*pPipe, lcs));



    std::vector<SPtr<MbInstance>> pair;
    pair.push_back(comp1);
    pair.push_back(comp2);
    // pair.push_back(comp3);
    pair.push_back(comp4);
    pair.push_back(comp5);
    pair.push_back(comp6);
    pair.push_back(comp7);
    pair.push_back(comp8);
    pair.push_back(comp9);
    pair.push_back(comp10);
    pair.push_back(comp11);
    pair.push_back(comp12);
    pair.push_back(comp13);
    pair.push_back(comp14);
    pair.push_back(comp15);
    pair.push_back(comp16);
    pair.push_back(comp17);
    pair.push_back(comp18);
    pair.push_back(comp19);
    pair.push_back(comp20);
    pair.push_back(comp21);


    SPtr<MbAssembly> assm(new MbAssembly(pair));

    {
        //Концентричность кожуха и камеры
        MtGeomArgument face11(pKozhuh->GetFace(25), comp1);
        MtGeomArgument face21(pKamera->GetFace(13), comp2);
        assm->AddConstraint(GCM_CONCENTRIC, face11, face21);

        //убрать эту зависимость
        //Линейный размер между кожухом и камерой
        MtGeomArgument face12(pKozhuh->GetFace(8), comp1);//было 36
        MtGeomArgument face22(pKamera->GetFace(0), comp2);
        assm->AddConstraint(GCM_DISTANCE, face12, face22, 40.0);

        // Концентричность камеры и крышки
        MtGeomArgument face222(pKamera->GetFace(53), comp2);
        MtGeomArgument face44(pKrKamera->GetFace(1), comp4);
        assm->AddConstraint(GCM_CONCENTRIC, face222, face44);

        //Линейный размер между камерой и крышкой
        MtGeomArgument face24(pKamera->GetFace(12), comp2);
        MtGeomArgument face42(pKrKamera->GetFace(10), comp4);
        assm->AddConstraint(GCM_DISTANCE, face24, face42, 0.0);


        // Концентричность камеры и крышки вверху
        MtGeomArgument face2222(pKamera->GetFace(44), comp2);
        MtGeomArgument face55(pKrKamera1->GetFace(1), comp5);
        assm->AddConstraint(GCM_CONCENTRIC, face2222, face55);

        //Линейный размер между камерой и крышкой вверху
        MtGeomArgument face25(pKamera->GetFace(11), comp2);
        MtGeomArgument face52(pKrKamera1->GetFace(12), comp5);
        assm->AddConstraint(GCM_DISTANCE, face25, face52, 0.0);


        // Концентричность кожуха и самой большой крышки
        MtGeomArgument face111(pKozhuh->GetFace(91), comp1);
        MtGeomArgument face66(pKrBig->GetFace(1), comp6);
        assm->AddConstraint(GCM_CONCENTRIC, face111, face66);

        //Линейный размер между кожухом и самой большой крышкой
        MtGeomArgument face16(pKozhuh->GetFace(57), comp1);
        MtGeomArgument face61(pKrBig->GetFace(24), comp6);
        assm->AddConstraint(GCM_DISTANCE, face16, face61, 0.0);


        // Концентричность кожуха и второй крышки сверху
        MtGeomArgument face1111(pKozhuh->GetFace(113), comp1);
        MtGeomArgument face77(pKrVtorayaSverhu->GetFace(1), comp7);
        assm->AddConstraint(GCM_CONCENTRIC, face1111, face77);

        //Линейный размер между кожухом и второй крышкой сверху
        MtGeomArgument face17(pKozhuh->GetFace(58), comp1);
        MtGeomArgument face71(pKrVtorayaSverhu->GetFace(8), comp7);
        assm->AddConstraint(GCM_DISTANCE, face17, face71, 0.0);


        // Концентричность кожуха и третьей крышки сверху
        MtGeomArgument face11111(pKozhuh->GetFace(118), comp1);
        MtGeomArgument face88(pKrTretyaSverhu->GetFace(1), comp8);
        assm->AddConstraint(GCM_CONCENTRIC, face11111, face88);

        //Линейный размер между кожухом и третьей крышкой сверху
        MtGeomArgument face18(pKozhuh->GetFace(59), comp1);
        MtGeomArgument face81(pKrTretyaSverhu->GetFace(16), comp8);
        assm->AddConstraint(GCM_DISTANCE, face18, face81, 0.0);



        // Концентричность кожуха и ПЕРВОЙ крышки СНИЗУ
        MtGeomArgument face100(pKozhuh->GetFace(131), comp1);
        MtGeomArgument face99(pKrFirstNis->GetFace(1), comp9);
        assm->AddConstraint(GCM_CONCENTRIC, face100, face99);

        //Линейный размер между кожухом и ПЕРВОЙ крышкой СНИЗУ
        MtGeomArgument face19(pKozhuh->GetFace(61), comp1);
        MtGeomArgument face91(pKrFirstNis->GetFace(12), comp9);
        assm->AddConstraint(GCM_DISTANCE, face19, face91, 0.0);


        // Концентричность кожуха и ВТОРОЙ крышки СНИЗУ
        MtGeomArgument faceKK(pKozhuh->GetFace(140), comp1);
        MtGeomArgument face10(pKrsamSmall->GetFace(1), comp10);
        assm->AddConstraint(GCM_CONCENTRIC, faceKK, face10);

        //Линейный размер между кожухом и ВТОРОЙ крышкой СНИЗУ
        MtGeomArgument face110(pKozhuh->GetFace(62), comp1);
        MtGeomArgument face101(pKrsamSmall->GetFace(8), comp10);
        assm->AddConstraint(GCM_DISTANCE, face110, face101, 0.0);


        // Концентричность кожуха и ТРЕТЬЕЙ крышки СНИЗУ
        MtGeomArgument faceK(pKozhuh->GetFace(71), comp1);
        MtGeomArgument face3kr(pKrNaSkose->GetFace(1), comp11);
        assm->AddConstraint(GCM_CONCENTRIC, faceK, face3kr);

        //Линейный размер между кожухом и ТРЕТЬЕЙ крышкой СНИЗУ
        MtGeomArgument faceK11(pKozhuh->GetFace(23), comp1);
        MtGeomArgument face11K(pKrNaSkose->GetFace(12), comp11);
        assm->AddConstraint(GCM_DISTANCE, face11K, faceK11, 0.0);


        // Концентричность кожуха и крышкей СБОКУ
        MtGeomArgument faceH(pKozhuh->GetFace(151), comp1);
        MtGeomArgument faceS(pKrSboku->GetFace(1), comp12);
        assm->AddConstraint(GCM_CONCENTRIC, faceH, faceS);

        //Линейный размер между кожухом и крышкей СБОКУ
        MtGeomArgument faceSH(pKozhuh->GetFace(87), comp1);
        MtGeomArgument faceHS(pKrSboku->GetFace(10), comp12);
        assm->AddConstraint(GCM_DISTANCE, faceSH, faceHS, 0.0);



        // Концентричность кожуха и СТОЙКИ
        MtGeomArgument facek(pKozhuh->GetFace(4), comp1);//,было 6
        MtGeomArgument faceSt(pStoika->GetFace(25), comp13);
        assm->AddConstraint(GCM_CONCENTRIC, facek, faceSt);

        //Линейный размер между кожухом и СТОЙКОЙ
        MtGeomArgument faceKo(pKozhuh->GetFace(87), comp1);
        MtGeomArgument faceKoSt(pStoika->GetFace(5), comp13);
        assm->AddConstraint(GCM_DISTANCE, faceKo, faceKoSt, -(params.Length - 300) / 1.23 / 4);


        // Концентричность кожуха и СТОЙКИ 1
        MtGeomArgument faceSt1(pStoika->GetFace(25), comp14);
        assm->AddConstraint(GCM_CONCENTRIC, facek, faceSt1);

        //Линейный размер между кожухом и СТОЙКОЙ 1
        MtGeomArgument faceKoSt1(pStoika->GetFace(5), comp14);
        assm->AddConstraint(GCM_DISTANCE, faceKo, faceKoSt1, -(params.Length - 300) / 1.23);
    }


    {
        // Концентричность камеры и Решетки непод
        MtGeomArgument facekam(pKamera->GetFace(13), comp2);
        MtGeomArgument faceNe(pReshetkaNEPod->GetFace(5), comp15);
        assm->AddConstraint(GCM_CONCENTRIC, facekam, faceNe);

        MtGeomArgument facekamNe(pKamera->GetFace(0), comp2);
        MtGeomArgument faceNekam(pReshetkaNEPod->GetFace(6), comp15);
        assm->AddConstraint(GCM_DISTANCE, facekamNe, faceNekam, 0.0);


        // Концентричность Решетки непод и перегородкой
        MtGeomArgument faceNe2(pReshetkaNEPod->GetFace(9), comp15);
        MtGeomArgument facePer1(pPeregorodka->GetFace(2), comp16);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer1);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument faceNe2Per1(pReshetkaNEPod->GetFace(0), comp15);//0
        MtGeomArgument facePer1Ne2(pPeregorodka->GetFace(1), comp16);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer1Ne2, 1160.0);

        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePer2(pPeregorodka->GetFace(2), comp17);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer2);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument facePer2Ne2(pPeregorodka->GetFace(1), comp17);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer2Ne2, 2350.0);

        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePer3(pPeregorodka->GetFace(2), comp18);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer3);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument facePer3Ne2(pPeregorodka->GetFace(1), comp18);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer3Ne2, 3550.0);

        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePer4(pPeregorodka->GetFace(2), comp19);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer4);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument facePer4Ne2(pPeregorodka->GetFace(1), comp19);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer4Ne2, 4750.0);

        // Концентричность Решетки непод и перегородкой
        MtGeomArgument facePer44(pPeregorodka->GetFace(2), comp20);
        assm->AddConstraint(GCM_CONCENTRIC, faceNe2, facePer44);

        //Линейный размер между Решеткой непод и перегородкой
        MtGeomArgument facePer44Ne2(pPeregorodka->GetFace(1), comp20);
        assm->AddConstraint(GCM_DISTANCE, faceNe2Per1, facePer44Ne2, 5950.0);


        int face = 0;
        switch (params.diam) {
        case 800:
            face = 152;
            break;
        case 1000:
            face = 152;
            break;
        case 1200:
            face = 252;
            break;
        case 1400:
            face = 352;
            break;
        case 1600:
            face = 452;
            break;
        case 1800:
            face = 952;
            break;
        case 2000:
            face = 1052;
            break;
        }


        MtGeomArgument facePerTube(pPeregorodka->GetFace(2), comp20);
        MtGeomArgument faceTubePer(pPipe->GetFace(face), comp21);
        assm->AddConstraint(GCM_PARALLEL, facePerTube, faceTubePer);

    }



    assm->EvaluateConstraints();

    return assm;
}