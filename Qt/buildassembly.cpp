#include "BuildMathModel.h"

using namespace BuildMathModel;

SPtr<MbAssembly> ParametricModelCreator::CreatePneumocylinderAssembly(ConfigParams params)
{
    // Набор параметров для параметризации
    const int lengthK = params.lengthK;
    const int length0 = params.length0;
    const int LENGTH = params.LENGTH;
    const int length1 = params.length1;
    
    MbPlacement3D lcs;

    SPtr<MbSolid> capSolid(create_cup_005());
    SPtr<MbSolid> headExhangerGridSolid(create_outer_pipes_grid_004());
    SPtr<MbSolid> headExhangerGridSecondSolid(create_inner_pipes_grid_006());
    SPtr<MbSolid> outerPipeSolid(create_outer_pipe_002(lengthK));
    SPtr<MbSolid> innerPipeSolid(create_inner_pipe_001());
    SPtr<MbSolid> supportSolid(create_support_003());
    SPtr<MbSolid> curvedPipeSolid(create_curved_pipe_007());
    //capSolid->SetSimpleAttribute(MbAttribute)

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
    //pair.push_back(curvedPipeItem);
    //curvedPipeItem->Move(MbVector3D(100, 100, 1850));

    pair.push_back(supportItem);
    supportItem->SetColor(0, 0, 200);
    pair.push_back(supportItem2);
    supportItem2->SetColor(0, 0, 200);

    MbAxis3D axxVert(MbVector3D(0, 1, 0));

    supportItem2->Rotate(axxVert, M_PI);
    headExhangerGridSecondItem->Rotate(axxVert, M_PI);

    SPtr<MbAssembly> assm(new MbAssembly(pair));
    {
        // Constractions

        double lengthP0 = -745;
        MtParVariant ArgLengthP0(lengthP0);

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
        double distanse_length1 = -(length1 + (10 / 2));
        MtParVariant ArgLength1(distanse_length1);

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
        double lengthBetweenInnerPipesAndGrid = 2150 - 230 - lengthK - 250 - 50;
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
       /* MtGeomArgument GeometryArgCurvedPipeCoinc(curvedPipeSolid->GetFace(0), curvedPipeItem);
        MtGeomArgument GeometryArgCurvedPipeCoinc2(innerPipeSolid->GetFace(0), innerPipeItem);*/

        /*MtGeomArgument GeometryArgCurvedPipeConc(curvedPipeSolid->GetFace(2), curvedPipeItem);
        MtGeomArgument GeometryArgCurvedPipeConc2(innerPipeSolid->GetFace(2), innerPipeItem);*/

        //assm->AddConstraint(GCM_COINCIDENT, GeometryArgCurvedPipeCoinc, GeometryArgCurvedPipeCoinc2);
        //assm->AddConstraint(GCM_CONCENTRIC, GeometryArgCurvedPipeConc, GeometryArgCurvedPipeConc2);
    }

    assm->EvaluateConstraints();
    
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

    double distanceRezhetka = 500.0;
    double distanceTubesKozhux = (-1) * (distanceRezhetka + 90.0);
    double distanceTubesTeploobmen = -660.0;
#pragma endregion

    MbPlacement3D lcs;

    // Item_008_001->Move(MbVector3D (3000,20,20));
    // MbAxis3D axVert(MbVector3D(1, 0, 0));
    // MbAxis3D ayVert(MbVector3D(0, 1, 0));
    // MbAxis3D azVert(MbVector3D(0, 0, 1));

    SPtr<MbSolid> Detail_001(Zarubincreate_001_tubeTeploobmen(lK, ttDiam, ttThickness));
    SPtr<MbSolid> Detail_002(Zarubincreate_002_tubeKozhux(lK, ktDiam, ktThickness));
    SPtr<MbSolid> Detail_003(Zarubincreate_003_opora(dV, ktDiam, ktThickness, t));
    SPtr<MbSolid> Detail_004(Zarubincreate_004_reshetkaKozhux(ktDiam, ktThickness, t));
    SPtr<MbSolid> Detail_005(Zarubincreate_005_kamera());
    SPtr<MbSolid> Detail_006(Zarubincreate_006_RezhetkaTeplTube());
    SPtr<MbSolid> Detail_007(Zarubincreate_007_FlanecKozhux());
    SPtr<MbSolid> Detail_008(Zarubincreate_008_FlanecSpecial());
    SPtr<MbSolid> Detail_009(Zarubincreate_009_curevedTube(ttDiam, ttThickness, t));

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

    // Камера
    SPtr<MbInstance> Item_008_001(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_002(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_003(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_004(new MbInstance(*Detail_008, lcs));

    SPtr<MbInstance> Item_008_005(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_006(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_007(new MbInstance(*Detail_008, lcs));
    SPtr<MbInstance> Item_008_008(new MbInstance(*Detail_008, lcs));

    SPtr<MbInstance> Item_009_001(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_002(new MbInstance(*Detail_009, lcs));
    SPtr<MbInstance> Item_009_003(new MbInstance(*Detail_009, lcs));

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

    pair.push_back(Item_009_001);
    pair.push_back(Item_009_002);
    pair.push_back(Item_009_003);

    SPtr<MbAssembly> assm(new MbAssembly(pair));
    {

        // Расстояние Опора - Опора
        {
            MtGeomArgument Face1(Detail_003->GetFace(2), Item_003_001);
            MtGeomArgument Face2(Detail_003->GetFace(4), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, Face1, Face2, MtParVariant(l0));
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
            MtGeomArgument Plane1(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument Plane2(Detail_004->GetFace(0), Item_004_001);

            assm->AddConstraint(GCM_DISTANCE, Plane1, Plane2, MtParVariant(distanceRezhetka));
        }

        // Концентричность Опора - Решетка кожуховых труб
        {
            MtGeomArgument VariableForHole1(Detail_003->GetFace(36), Item_003_002);
            MtGeomArgument VariableForHole2(Detail_003->GetFace(39), Item_003_002);
            MtGeomArgument VariableForHole3(Detail_003->GetFace(38), Item_003_002);
            MtGeomArgument VariableForHole4(Detail_003->GetFace(37), Item_003_002);

            MtGeomArgument OtvtubeTeploobmennaya1(Detail_004->GetFace(2), Item_004_001);
            MtGeomArgument OtvtubeTeploobmennaya2(Detail_004->GetFace(4), Item_004_001);
            MtGeomArgument OtvtubeTeploobmennaya3(Detail_003->GetFace(38), Item_003_001);
            MtGeomArgument OtvtubeTeploobmennaya4(Detail_003->GetFace(37), Item_003_001);

            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeTeploobmennaya1, VariableForHole1);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeTeploobmennaya2, VariableForHole2);
            assm->AddConstraint(GCM_CONCENTRIC, VariableForHole3, OtvtubeTeploobmennaya3);
            assm->AddConstraint(GCM_CONCENTRIC, VariableForHole4, OtvtubeTeploobmennaya4);
        }

        // Расстояние Опора - Кожуховые трубы
        {
            MtGeomArgument PlaneOfTubeKozhux1(Detail_002->GetFace(0), Item_002_001);
            MtGeomArgument PlaneOfTubeKozhux2(Detail_002->GetFace(0), Item_002_002);
            MtGeomArgument PlaneOfTubeKozhux3(Detail_002->GetFace(0), Item_002_003);
            MtGeomArgument PlaneOfTubeKozhux4(Detail_002->GetFace(0), Item_002_004);

            MtGeomArgument PlaneOfOporaForward1(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForward2(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForward3(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForward4(Detail_003->GetFace(2), Item_003_002);

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

            MtGeomArgument PlaneOfOporaForwards1(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards2(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards3(Detail_003->GetFace(2), Item_003_002);
            MtGeomArgument PlaneOfOporaForwards4(Detail_003->GetFace(2), Item_003_002);

            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny1, PlaneOfOporaForwards1, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny2, PlaneOfOporaForwards2, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny3, PlaneOfOporaForwards3, MtParVariant(distanceTubesTeploobmen));
            assm->AddConstraint(GCM_DISTANCE, PlaneOfTubeTeploobmeny4, PlaneOfOporaForwards4, MtParVariant(distanceTubesTeploobmen));
        }

        // Совмещение Решетка кожуховых труб - Камера
        {
            MtGeomArgument Planeofkamera1(Detail_005->GetFace(1), Item_005_001);
            MtGeomArgument Planeofkamera2(Detail_005->GetFace(3), Item_005_001);

            MtGeomArgument PlaneViaRezhetka1(Detail_004->GetFace(41), Item_004_001);
            MtGeomArgument PlaneViaRezhetka2(Detail_004->GetFace(3), Item_004_001);

            assm->AddConstraint(GCM_COINCIDENT, Planeofkamera1, PlaneViaRezhetka1);
            assm->AddConstraint(GCM_CONCENTRIC, Planeofkamera2, PlaneViaRezhetka2);
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

        // Совмещение Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument getPlaneOfTubeTeploobmennaya1(Detail_001->GetFace(0), Item_001_001);
            MtGeomArgument getPlaneOfTubeTeploobmennaya2(Detail_001->GetFace(0), Item_001_002);
            MtGeomArgument getPlaneOfTubeTeploobmennaya3(Detail_001->GetFace(0), Item_001_003);
            MtGeomArgument getPlaneOfTubeTeploobmennaya4(Detail_001->GetFace(0), Item_001_004);

            MtGeomArgument getPlaneFlanecSpec1(Detail_008->GetFace(12), Item_008_001);
            MtGeomArgument getPlaneFlanecSpec2(Detail_008->GetFace(12), Item_008_002);
            MtGeomArgument getPlaneFlanecSpec3(Detail_008->GetFace(12), Item_008_003);
            MtGeomArgument getPlaneFlanecSpec4(Detail_008->GetFace(12), Item_008_004);


            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya1, getPlaneFlanecSpec1);
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya2, getPlaneFlanecSpec2);
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya3, getPlaneFlanecSpec3);
            assm->AddConstraint(GCM_COINCIDENT, getPlaneOfTubeTeploobmennaya4, getPlaneFlanecSpec4);
        }

        // Концентричность Фланец Специальный - Труба теплообменная
        {
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya1(Detail_001->GetFace(2), Item_001_001);
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya2(Detail_001->GetFace(2), Item_001_002);
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya3(Detail_001->GetFace(2), Item_001_003);
            MtGeomArgument subgetPlaneOfTubeTeploobmennaya4(Detail_001->GetFace(2), Item_001_004);

            MtGeomArgument subgetPlaneFlanecSpec1(Detail_008->GetFace(3), Item_008_001);
            MtGeomArgument subgetPlaneFlanecSpec2(Detail_008->GetFace(3), Item_008_002);
            MtGeomArgument subgetPlaneFlanecSpec3(Detail_008->GetFace(3), Item_008_003);
            MtGeomArgument subgetPlaneFlanecSpec4(Detail_008->GetFace(3), Item_008_004);


            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya1, subgetPlaneFlanecSpec1);
            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya2, subgetPlaneFlanecSpec2);
            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya3, subgetPlaneFlanecSpec3);
            assm->AddConstraint(GCM_CONCENTRIC, subgetPlaneOfTubeTeploobmennaya4, subgetPlaneFlanecSpec4);
        }

        //// Совмещение Фланец Специальный - Фланец Специальный (В Камере)
        //{
        //    MtGeomArgument PlaneOfFlanecFirst1(Detail_008->GetFace(2), Item_008_001);
        //    MtGeomArgument PlaneOfFlanecFirst2(Detail_008->GetFace(2), Item_008_002);
        //    MtGeomArgument PlaneOfFlanecFirst3(Detail_008->GetFace(2), Item_008_003);
        //    MtGeomArgument PlaneOfFlanecFirst4(Detail_008->GetFace(2), Item_008_004);

        //    MtGeomArgument PlaneOfFlanecSecond1(Detail_008->GetFace(9), Item_008_001);
        //    MtGeomArgument PlaneOfFlanecSecond2(Detail_008->GetFace(9), Item_008_002);
        //    MtGeomArgument PlaneOfFlanecSecond3(Detail_008->GetFace(9), Item_008_003);
        //    MtGeomArgument PlaneOfFlanecSecond4(Detail_008->GetFace(9), Item_008_004);


        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst1, PlaneOfFlanecSecond1);
        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst2, PlaneOfFlanecSecond2);
        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst3, PlaneOfFlanecSecond3);
        //    assm->AddConstraint(GCM_COINCIDENT, PlaneOfFlanecFirst4, PlaneOfFlanecSecond4);
        //}

        //// Концентричность Фланец Специальный - Фланец Специальный (В Камере)
        //{
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya1(Detail_006->GetFace(3), Item_006_001);
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya2(Detail_006->GetFace(3), Item_006_002);
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya3(Detail_006->GetFace(3), Item_006_003);
        //    MtGeomArgument SubPlaneOfTubeTeploobmennaya4(Detail_006->GetFace(3), Item_006_004);

        //    MtGeomArgument SubPlaneFlanecTeploobmennaya1(Detail_007->GetFace(3), Item_007_001);
        //    MtGeomArgument SubPlaneFlanecTeploobmennaya2(Detail_007->GetFace(3), Item_007_002);
        //    MtGeomArgument SubPlaneFlanecTeploobmennaya3(Detail_007->GetFace(3), Item_007_003);
        //    MtGeomArgument SubPlaneFlanecTeploobmennaya4(Detail_007->GetFace(3), Item_007_004);


        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya1, SubPlaneFlanecTeploobmennaya1);
        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya2, SubPlaneFlanecTeploobmennaya2);
        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya3, SubPlaneFlanecTeploobmennaya3);
        //    assm->AddConstraint(GCM_CONCENTRIC, SubPlaneOfTubeTeploobmennaya4, SubPlaneFlanecTeploobmennaya4);
        //}

        // Совмещение Изогнутая труба - Теплообменные трубы 
        {
            MtGeomArgument PlaneofCurevedTube1(Detail_009->GetFace(0), Item_009_001);
            MtGeomArgument PlaneofCurevedTube2(Detail_009->GetFace(0), Item_009_002);
            MtGeomArgument PlaneofCurevedTube3(Detail_009->GetFace(0), Item_009_003);

            MtGeomArgument PlaneForCoincedent1(Detail_001->GetFace(0), Item_001_001);
            MtGeomArgument PlaneForCoincedent2(Detail_001->GetFace(0), Item_001_002);
            MtGeomArgument PlaneForCoincedent3(Detail_001->GetFace(1), Item_001_003);

            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube1, PlaneForCoincedent1);
            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube2, PlaneForCoincedent2);
            assm->AddConstraint(GCM_COINCIDENT, PlaneofCurevedTube3, PlaneForCoincedent3);
        }

        // !!! Концентричность Изогнутая труба - Теплообменные трубы 
        {
            /*MtGeomArgument VariableForPlany1(Detail_001->GetFace(2), Item_001_001);
            MtGeomArgument VariableForPlany2(Detail_001->GetFace(2), Item_001_002);
            MtGeomArgument VariableForPlany4(Detail_001->GetFace(2), Item_001_003);
            MtGeomArgument VariableForPlany5(Detail_001->GetFace(2), Item_001_004);

            MtGeomArgument OtvtubeCureved1(Detail_009->GetFace(3), Item_009_001);
            MtGeomArgument OtvtubeCureved2(Detail_009->GetFace(3), Item_009_002);
            MtGeomArgument OtvtubeCureved4(Detail_009->GetFace(2), Item_009_001);
            MtGeomArgument OtvtubeCureved5(Detail_009->GetFace(2), Item_009_002);

            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved1, VariableForPlany1);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved2, VariableForPlany2);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved4, VariableForPlany4);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved5, VariableForPlany5);*/

            MtGeomArgument VariableForPlany6(Detail_001->GetFace(1), Item_001_002);
            MtGeomArgument OtvtubeCureved6(Detail_009->GetFace(0), Item_009_003);
            assm->AddConstraint(GCM_COINCIDENT, OtvtubeCureved6, VariableForPlany6, MtParVariant(GCM_OPPOSITE));

            MtGeomArgument VariableForPlany3(Detail_003->GetFace(36), Item_003_001);
            MtGeomArgument OtvtubeCureved3(Detail_009->GetFace(5), Item_009_003);
            assm->AddConstraint(GCM_CONCENTRIC, OtvtubeCureved3, VariableForPlany3);
        }
    }

    assm->EvaluateConstraints();

    return assm;
}