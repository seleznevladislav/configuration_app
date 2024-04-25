#include "BuildMathModel.h"
#include <mb_property.h>

using namespace BuildMathModel;

SPtr<MbAssembly> ParametricModelCreator::CreatePneumocylinderAssembly(ConfigParams params)
{
    // Набор параметров для параметризации
    /*const int lengthK = params.lengthK;
    const int length0 = params.length0;
    const int LENGTH = params.LENGTH;
    const int length1 = params.length1;
    const double length2 = 250.0;
    const double length3 = 230.0;

    const double assortmentInnerTubes = 25.0;
    const double assortmentOuterTubes = 57.0;
    const double assortmentCamera = 219.0;
    const double thicknessInnerTubes = 3.0;
    const double thicknessOuterTubes = 4.0;
    const double thicknessCamera = 8.0;
    const double diametrY = 32.0;
    const double t = 80.0;

    const double curvedLength = 72.5;*/

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

    MbPlacement3D lcs;

    SPtr<MbSolid> capSolid(createCup_005(t, assortmentOuterTubes, assortmentCamera));
    SPtr<MbSolid> headExhangerGridSolid(createOuterPipesGrid_004(length2, diametrY, thicknessInnerTubes, t, assortmentInnerTubes, assortmentCamera, thicknessCamera));
    SPtr<MbSolid> headExhangerGridSecondSolid(createInnerPipesGrid_006(length3, assortmentCamera, thicknessCamera));
    SPtr<MbSolid> outerPipeSolid(createOuterPipe_002(lengthK, assortmentOuterTubes, thicknessOuterTubes));
    SPtr<MbSolid> innerPipeSolid(createInnerPipe_001(assortmentInnerTubes, thicknessInnerTubes, LENGTH - curvedLength - 60 - 55));
    SPtr<MbSolid> supportSolid(createSupport_003(assortmentCamera, assortmentOuterTubes, t));
    SPtr<MbSolid> curvedPipeSolid(createCurvedPipe_007(assortmentInnerTubes, thicknessInnerTubes, t));
    SPtr<MbSolid> pipeHolderShort(createPipeHolder_010(20, assortmentInnerTubes));

    // GOST
    SPtr<MbSolid> flangeE(buildFlangeE(diametrY));
    SPtr<MbSolid> flangeF(buildFlangeF(diametrY));
    SPtr<MbSolid> screw35(buildFlangeScrew35());
    SPtr<MbSolid> pipeScrewWasher(buildWasher10());
    SPtr<MbSolid> screw55(buildScrew55());


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

    /* screw55Item->Move(MbVector3D(200, 200, 0));*/

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

        MtGeomArgument Plane1(headExhangerGridSolid->GetFace(37), headExhangerGridItem);
        MtGeomArgument Plane2(supportSolid->GetFace(33), supportItem);
        assm->AddConstraint(GCM_DISTANCE, Plane1, Plane2, MtParVariant(ArgLength1));

        // Grid concentric
        MtGeomArgument GeomArgGridConc(headExhangerGridSolid->GetFace(34), headExhangerGridItem);
        MtGeomArgument GeomArgGrid2Conc(headExhangerGridSolid->GetFace(29), headExhangerGridItem);
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
        double lengthBetweenInnerPipesAndGrid = LENGTH - length3 - lengthK - length2 - curvedLength;
        MtParVariant ArgLengthBetweenPipesAndGrid(lengthBetweenInnerPipesAndGrid);

        MtGeomArgument PlaneGrid(headExhangerGridSolid->GetFace(38), headExhangerGridItem);
        MtGeomArgument PlaneGrid2(headExhangerGridSolid->GetFace(38), headExhangerGridItem);
        MtGeomArgument PlaneGrid3(headExhangerGridSolid->GetFace(38), headExhangerGridItem);
        MtGeomArgument PlaneGrid4(headExhangerGridSolid->GetFace(38), headExhangerGridItem);

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
        MtGeomArgument GeomArgRoundGridAndCapConc3(headExhangerGridSolid->GetFace(38), headExhangerGridItem);

        assm->AddConstraint(GCM_CONCENTRIC, GeomArgRoundGridConc, GeomArgRoundGridAndCapConc);
        assm->AddConstraint(GCM_CONCENTRIC, GeomArgRoundGridConc2, GeomArgRoundGridAndCapConc2);

        double lengthBetweenGrids = -(lengthK + 250);
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
        MtGeomArgument GeometryArgPipeHolderCoinc(headExhangerGridSolid->GetFace(38), headExhangerGridItem);
        MtGeomArgument GeometryArgPipeHolderCoinc2(pipeHolderShort->GetFace(0), pipeHolderShortItem);
        MtGeomArgument GeometryArgPipeHolderConc(innerPipeSolid->GetFace(3), innerPipeItem);
        MtGeomArgument GeometryArgPipeHolderConc2(pipeHolderShort->GetFace(10), pipeHolderShortItem);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgPipeHolderCoinc, GeometryArgPipeHolderCoinc2);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgPipeHolderConc, GeometryArgPipeHolderConc2);

        MtGeomArgument GeometryArgPipeHolderCoinc3(headExhangerGridSolid->GetFace(38), headExhangerGridItem);
        MtGeomArgument GeometryArgPipeHolderCoinc4(pipeHolderShort->GetFace(0), pipeHolderShortItem2);
        MtGeomArgument GeometryArgPipeHolderConc3(innerPipeSolid->GetFace(3), innerPipeItem2);
        MtGeomArgument GeometryArgPipeHolderConc4(pipeHolderShort->GetFace(10), pipeHolderShortItem2);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgPipeHolderCoinc3, GeometryArgPipeHolderCoinc4);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgPipeHolderConc3, GeometryArgPipeHolderConc4);

        MtGeomArgument GeometryArgPipeHolderCoinc5(headExhangerGridSolid->GetFace(38), headExhangerGridItem);
        MtGeomArgument GeometryArgPipeHolderCoinc6(pipeHolderShort->GetFace(0), pipeHolderShortItem3);
        MtGeomArgument GeometryArgPipeHolderConc5(innerPipeSolid->GetFace(3), innerPipeItem3);
        MtGeomArgument GeometryArgPipeHolderConc6(pipeHolderShort->GetFace(10), pipeHolderShortItem3);

        assm->AddConstraint(GCM_COINCIDENT, GeometryArgPipeHolderCoinc5, GeometryArgPipeHolderCoinc6);
        assm->AddConstraint(GCM_CONCENTRIC, GeometryArgPipeHolderConc5, GeometryArgPipeHolderConc6);

        MtGeomArgument GeometryArgPipeHolderCoinc7(headExhangerGridSolid->GetFace(38), headExhangerGridItem);
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
        MtGeomArgument GeometryArgFlanecECoinc2(headExhangerGridSolid->GetFace(31), headExhangerGridItem);

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

    string casingPipeName = "Casing pipe";
    string headExchangePipeName = "Heat exchange pipe";
    string distributionChamberName = "Heat exhanger chamber";
    string rotatableChamberName = "Casing chamber";
    string supportName = "Support";
    string capName = "Cap";
    string roundPipeName = "Rounded pipe";

    MbProductInfo casingPipeInfo(false, casingPipeName, casingPipeName, casingPipeName);
    MbProductInfo headExchangePipeInfo(false, headExchangePipeName, headExchangePipeName, headExchangePipeName);
    MbProductInfo distributionChamberInfo(false, headExchangePipeName, headExchangePipeName, headExchangePipeName);
    MbProductInfo rotatableChamberInfo(false, rotatableChamberName, rotatableChamberName, rotatableChamberName);
    MbProductInfo supportInfo(false, supportName, supportName, supportName);
    MbProductInfo capInfo(false, capName, capName, capName);
    MbProductInfo roundedPipeInfo(false, roundPipeName, roundPipeName, roundPipeName);

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
    
    assm->AddAttribute(TTRMInfo);

    return assm;
}

SPtr<MbAssembly> ParametricModelCreator::CreateTTOR(BuildParamsZarubin params)
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

    const double visotaOpori = 850;
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
    SPtr<MbSolid> Detail_005(Zarubincreate_005_kamera(ktDiam, ktThickness));
    Detail_005->SetColor(101, 150, 94);
    SPtr<MbSolid> Detail_006(Zarubincreate_006_RezhetkaTeplTube());
    Detail_006->SetColor(218, 145, 85);
    SPtr<MbSolid> Detail_007(Zarubincreate_007_FlanecKozhux(ktDiam, ktThickness));
    Detail_007->SetColor(47, 218, 56);

    SPtr<MbSolid> Detail_008(Zarubincreate_008_FlanecSpecial(ttDiam, ttThickness));
    SPtr<MbSolid> Detail_009(Zarubincreate_009_curevedTube(ttDiam, ttThickness, t));
    Detail_009->SetColor(71, 91, 71);
    SPtr<MbSolid> Detail_010(Zarubincreate_010_Connector(ktDiam, ktThickness, t));
    SPtr<MbSolid> Detail_011(Zarubincreate_011_ConnectorWithFlanec(ktDiam, ktThickness, t, visotaOpori));
    
    std::vector<SPtr<MbItem>> pair;

    SPtr<MbInstance> Item_001_001(new MbInstance(*Detail_001, lcs));

    //MbProperties* mp = new MbProperties();

    //  size_t currpos = 'Test';

    //  TCHAR bc = IDS_ITEM_0733;

    //  mp->SetName(bc);
    //  c3d::string_t description;
    //  //IDS_ITEM_0733

    //  Item_001_001->GetProperties(*mp);

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

    SPtr<MbInstance> Item_007_005(new MbInstance(*Detail_007, lcs));
    SPtr<MbInstance> Item_007_006(new MbInstance(*Detail_007, lcs));

    // Фланцы специальнае для камеры
    SPtr<MbInstance> Item_008_001(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_002(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_003(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_004(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_005(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_006(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_007(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_008(new MbInstance(*Detail_008, lcs));

    // Фланцы специальнае для камеры
    SPtr<MbInstance> Item_008_009(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_010(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_011(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_012(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_013(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_014(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_015(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_016(new MbInstance(*Detail_008, lcs));

    SPtr<MbInstance> Item_009_001(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_002(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_003(new MbInstance(*Detail_009, lcs));

    SPtr<MbInstance> Item_010_001(new MbInstance(*Detail_010, lcs));
    SPtr<MbInstance> Item_011_001(new MbInstance(*Detail_011, lcs));

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

    //pair.push_back(Item_005_001);

    pair.push_back(Item_006_001);
    pair.push_back(Item_006_002);
    pair.push_back(Item_006_003);
    pair.push_back(Item_006_004);

    pair.push_back(Item_007_001);
    pair.push_back(Item_007_002);
    pair.push_back(Item_007_003);
    pair.push_back(Item_007_004);

    pair.push_back(Item_007_005);
    pair.push_back(Item_007_006);

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

    pair.push_back(Item_009_001);
    pair.push_back(Item_009_002);
    pair.push_back(Item_009_003);

    pair.push_back(Item_010_001);
    pair.push_back(Item_011_001);

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
            MtGeomArgument Face2321(Detail_011->GetFace(2), Item_011_001);

            assm->AddConstraint(GCM_DISTANCE, Face1312, Face2321, MtParVariant(l2 - shirinaOpori));

            MtGeomArgument ConnectorsPlane1(Detail_002->GetFace(3), Item_002_001);
            MtGeomArgument ConnectorsPlane2(Detail_002->GetFace(3), Item_002_002);

            MtGeomArgument ConnectorsPlane3(Detail_011->GetFace(8), Item_011_001);
            MtGeomArgument ConnectorsPlane4(Detail_011->GetFace(9), Item_011_001);

            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane2, ConnectorsPlane3);
            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane1, ConnectorsPlane4);
        }

        // Расстояние/Концентричность Соединение
        {

            MtGeomArgument Face1312(Detail_003->GetFace(3), Item_003_001);
            MtGeomArgument Face2321(Detail_010->GetFace(2), Item_010_001);

            assm->AddConstraint(GCM_DISTANCE, Face1312, Face2321, MtParVariant(l2 - shirinaOpori));

            MtGeomArgument ConnectorsPlane1(Detail_002->GetFace(3), Item_002_003);
            MtGeomArgument ConnectorsPlane2(Detail_002->GetFace(3), Item_002_004);

            MtGeomArgument ConnectorsPlane3(Detail_010->GetFace(4), Item_010_001);
            MtGeomArgument ConnectorsPlane4(Detail_010->GetFace(5), Item_010_001);

            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane2, ConnectorsPlane3);
            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane1, ConnectorsPlane4);
        }

        // Совмещение/Концентричность фланцы на соеденение
        {
            MtGeomArgument facebody1(Detail_007->GetFace(12), Item_007_005);
            MtGeomArgument facebody2(Detail_007->GetFace(12), Item_007_006);
            MtGeomArgument facebody3(Detail_011->GetFace(5), Item_011_001);
            MtGeomArgument facebody4(Detail_011->GetFace(0), Item_011_001);

            assm->AddConstraint(GCM_COINCIDENT, facebody1, facebody3);
            assm->AddConstraint(GCM_COINCIDENT, facebody2, facebody4);

            MtGeomArgument ConnectorsPlane1(Detail_007->GetFace(3), Item_007_005);
            MtGeomArgument ConnectorsPlane2(Detail_007->GetFace(3), Item_007_006);

            MtGeomArgument ConnectorsPlane3(Detail_011->GetFace(12), Item_011_001);
            MtGeomArgument ConnectorsPlane4(Detail_011->GetFace(17), Item_011_001);

            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane1, ConnectorsPlane3);
            assm->AddConstraint(GCM_CONCENTRIC, ConnectorsPlane2, ConnectorsPlane4);
        }
    }

    assm->EvaluateConstraints();


    assm->Rotate(axVert, M_PI/2);
    assm->Rotate(ayVert, M_PI);

    return assm;
}