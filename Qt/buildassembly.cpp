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

    pair.push_back(Item_009_001);
    pair.push_back(Item_009_002);
    pair.push_back(Item_009_003);

    SPtr<MbAssembly> assm(new MbAssembly(pair));

    //MbCartPoint3D p[3];
    //p.x = 1.0;

    //MbPlacement3D tmpPlace(p[0], p[1], p[2], false);
    //assm->SetPlacement(tmpPlace);
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


        // Совмещение/Концентричность Решетка кожуховых труб - Камера
        {
            MtGeomArgument Planeofkamera1(Detail_005->GetFace(1), Item_005_001);
            MtGeomArgument Planeofkamera2(Detail_005->GetFace(3), Item_005_001);

            MtGeomArgument PlaneViaRezhetka1(Detail_004->GetFace(41), Item_004_001);
            MtGeomArgument PlaneViaRezhetka2(Detail_004->GetFace(3), Item_004_001);

            assm->AddConstraint(GCM_COINCIDENT, Planeofkamera1, PlaneViaRezhetka1);
            assm->AddConstraint(GCM_CONCENTRIC, Planeofkamera2, PlaneViaRezhetka2);
        }

    }

    assm->EvaluateConstraints();

    return assm;
}