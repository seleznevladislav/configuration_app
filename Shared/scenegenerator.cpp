#include <scenegenerator.h>

#include <instance.h>
#include <assembly.h>
#include <mesh.h>
#include <mb_class_traits.h>
#include <mb_variables.h>
#include <plane_instance.h>
#include <space_instance.h>
#include <attr_product.h>
#include <action_mesh.h>
#include <op_mesh_parameter.h>

#include <conv_model_exchange.h>
#include <conv_exchange_settings.h>
#include <vsn_renderstatistics.h>
#include <vsn_mathgeometry.h>
#include <vsn_mathrepresentation.h>

#include <last.h>

using namespace c3d;

/* GeometryHash */
class GeometryHash
{
public:
    // конструктор по умолчанию
    explicit GeometryHash()
        : m_bIsSameBody(false)
        , m_bIsSameMeshBody(false)
    {
    }
    // деструктор
    ~GeometryHash()
    {
        ClearGeometries();
    }
public:
    void AppendMathGeometry(const MbItem* pItem, MbItem* pNewItem);
    void AppendGeometry(const MbItem* pItem, Geometry* pMesh);
    void AppendSegmentData(const MbItem* pItem, SceneSegmentData* pData);
    void RemoveGeometry(const MbItem* pItem);
    void ClearGeometries();
public:
    MbItem* FindMathGeometryByItem(const MbItem* pSrcItem);
    SceneSegmentData* FindSegmentDataByItem(const MbItem* pItem);
public:
    bool m_bIsSameBody;
    bool m_bIsSameMeshBody;
    ObjectMap<const MbItem*, MbItem*> m_mapMathGeometry;
    ObjectMap<const MbItem*, Geometry*> m_mapGeometry;
    ObjectMap<const MbItem*, SceneSegmentData*> m_mapSegmentData;
private:
    VSN_DISABLE_COPY(GeometryHash);
};

//-----------------------------------------------------------------------------
//
// ---
void GeometryHash::AppendGeometry(const MbItem* pItem, Geometry* pMesh)
{
    auto it = m_mapGeometry.find(pItem);
    if (it == m_mapGeometry.end())
        m_mapGeometry.insert(pItem, pMesh);
}

//-----------------------------------------------------------------------------
//
// ---
void GeometryHash::AppendMathGeometry(const MbItem* pItem, MbItem* pNewItem)
{
    if (!m_mapMathGeometry.contains(pItem))
        m_mapMathGeometry.insert(pItem, pNewItem);
}

//-----------------------------------------------------------------------------
//
// ---
void GeometryHash::AppendSegmentData(const MbItem* pItem, SceneSegmentData* pData)
{
    auto it = m_mapSegmentData.find(pItem);
    if (it == m_mapSegmentData.end())
        m_mapSegmentData.insert(pItem, pData);
}

//-----------------------------------------------------------------------------
//
// ---
void GeometryHash::RemoveGeometry(const MbItem* pItem)
{
    auto it = m_mapGeometry.find(pItem);
    if (it != m_mapGeometry.end())
        m_mapGeometry.erase(it);

    auto it2 = m_mapSegmentData.find(pItem);
    if (it2 != m_mapSegmentData.end())
        m_mapSegmentData.erase(it2);

}

//-----------------------------------------------------------------------------
//
// ---
void GeometryHash::ClearGeometries()
{
    m_mapGeometry.clear();
    m_mapSegmentData.clear();
}

//-----------------------------------------------------------------------------
//
// ---
SceneSegmentData* GeometryHash::FindSegmentDataByItem(const MbItem* pItem)
{
    auto it = m_mapSegmentData.find(pItem);
    if (it != m_mapSegmentData.end())
        return it.value();

    if (!m_bIsSameBody)
        return nullptr;

    for (auto it = m_mapSegmentData.begin(); it != m_mapSegmentData.end(); ++it) 
    {
        if (it.key()->IsSame(*pItem, 0.1))
            return it.value();
    }
    return nullptr;
}

//-----------------------------------------------------------------------------
//
// ---
MbItem* GeometryHash::FindMathGeometryByItem(const MbItem* pSrcItem)
{
    auto it = m_mapMathGeometry.find(pSrcItem);
    if (it != m_mapMathGeometry.end())
        return it.value();

    if (!m_bIsSameBody)
        return nullptr;

    for (auto it = m_mapMathGeometry.begin(); it != m_mapMathGeometry.end(); ++it)
    {
        if (it.key()->IsSame(*pSrcItem, 0.1))
            return it.value();
    }
    return nullptr;
}

//-----------------------------------------------------------------------------
//
// ---
void ProgressBuild::RegisterGeometryRep(const GeometryRep* pGeometryRep)
{
    auto& list = pGeometryRep->GetGeomList();
    for (auto it : list)
        RegisterGeometry(it);
}

//-----------------------------------------------------------------------------
//
// ---
void ProgressBuild::RegisterGeometry(const Geometry* pGeometry)
{
    if (!m_geometries.contains(pGeometry))
    {
        m_countGeometry++;
        m_geometries << pGeometry;
        Object::Connect(pGeometry, &Geometry::OnBuildCompleted, this, &ProgressBuild::slotFinishBuildGeometry);
    }
}

//-----------------------------------------------------------------------------
//
// ---
void ProgressBuild::UnregisterGeometry(const Geometry* pGeometry)
{
    auto index = m_geometries.indexOf(pGeometry);
    if (index != -1)
    {
        m_geometries.removeAt(index);
        m_currentGeometry++;
        Object::Disconnect(pGeometry, &Geometry::OnBuildCompleted, this, &ProgressBuild::slotFinishBuildGeometry);
        emit ValueModified(m_currentGeometry);
    }
    if (m_geometries.size() == 0)
    {
        if (m_pGenerator != nullptr)
            m_pGenerator->ClearProgressBuild();
    }
}

//-----------------------------------------------------------------------------
// Конструктор по умолчанию
// ---
ProgressBuild::ProgressBuild(SceneGenerator* pGenerator)
    : Object(nullptr)
    , m_currentGeometry(0)
    , m_countGeometry(0)
    , m_pGenerator(pGenerator)
{
}

//-----------------------------------------------------------------------------
// Деструктор
// ---
ProgressBuild::~ProgressBuild()
{
    emit BuildAllCompleted();
}

//-----------------------------------------------------------------------------
// 
void ProgressBuild::AppendGeometry(const Geometry* pGeometry)
{
    if (pGeometry == nullptr)
        return;
    RegisterGeometry(pGeometry);
}

//-----------------------------------------------------------------------------
//
// ---
void ProgressBuild::slotFinishBuildRep()
{
    if (GeometryRep* pRepresentation = vobject_cast<GeometryRep*>(GetSender()))
    {
        const ObjectList<Geometry*>& list = pRepresentation->GetGeomList();
        for (auto it : list)
            UnregisterGeometry(it);
    }
}

//-----------------------------------------------------------------------------
//
// ---
void ProgressBuild::slotFinishBuildGeometry(Geometry* pGeometry)
{
    UnregisterGeometry(pGeometry);
}

//-----------------------------------------------------------------------------
// Конструктор по умолчанию.
// ---
SceneGenerator::SceneGenerator(Object* parent)
    : Object(parent)
    , m_pProgressBuild(nullptr)
    , m_pGeometryHash(nullptr)
    , m_visualSag(Math::visualSag)
{
}

//-----------------------------------------------------------------------------
// Деструктор
// ---
SceneGenerator::~SceneGenerator()
{
    VSN_ASSERT(m_pProgressBuild == nullptr);
}

//-----------------------------------------------------------------------------
// Прочитать модель из памяти.
// ---
SPtr<MbModel> SceneGenerator::LoadModel(const char* pMemory, size_t length)
{
    MbModel* pModel = nullptr;
    if (pMemory == nullptr)
        return SPtr<MbModel>(pModel);

    C3DExchangeBuffer buffer;
    buffer.Init(pMemory, length);
    ConvConvertorProperty3D convProperties;
    SPtr<MbModel> pLoadMathModel(new MbModel);
    MbeConvResType readRes = ImportFromBuffer(*pLoadMathModel, buffer, mxf_C3D, &convProperties, nullptr);
    if (readRes == cnv_Error)
        pLoadMathModel.reset();
    return pLoadMathModel;
}

//-----------------------------------------------------------------------------
// Создать прогресс построения геометрии.
// ---
ProgressBuild* SceneGenerator::CreateProgressBuild()
{
    if (m_pProgressBuild != nullptr)
        VSN_DELETE_AND_NULL(m_pProgressBuild);
    m_pProgressBuild = new ProgressBuild(this);
    return m_pProgressBuild;
}

//-----------------------------------------------------------------------------
// Создать граф сцены с MbModel
// ---
SceneSegment* SceneGenerator::CreateSceneSegment(SPtr<MbModel> pModel, SceneSegment* pParent,
    bool bCheckIsSameBody, bool bCheckIsSameMeshBody, const std::vector<MbeSpaceType>& spaceTypes)
{
    m_items.clear();
    m_pGeometryHash = nullptr;
    RenderStatistics::ResetLoadedBodies();
    if (pModel == nullptr)
    {
        vsnWarning("GeometryFactory::CreateMathRep: MbModel cannot be null.");
        return nullptr;
    }
    pParent = new SceneSegment(pParent);

    std::vector<MbAttribute*> attrib;
    pModel->GetAttributes(attrib, at_ProductAttribute, at_ProductInfo);
    if (!attrib.empty())
    {
        MbProductInfo* prod = static_cast<MbProductInfo*>(attrib[0]);
        c3d::string_t sId, sName, sDescr;
        prod->GetData(sId, sName, sDescr);
        if (!sName.empty())
            pParent->SetObjectName(String(sName.c_str()));
//        if (!sId.empty())
//            pParent->setObjectDesignation(sId.c_str());
    }
    else
    {
//        SimpleName name = pModel->GetModelName();
        pParent->SetObjectName(String("Model"));
    }

    emit CreateSegment(pParent, nullptr);
    emit CreateSegmentItem(pParent, nullptr, pModel, nullptr, nullptr);
    m_pGeometryHash = std::make_shared<GeometryHash>();
    m_pGeometryHash->m_bIsSameBody = bCheckIsSameBody;
    m_pGeometryHash->m_bIsSameMeshBody = bCheckIsSameMeshBody;
    LoadSceneFromModel(pModel, pParent, spaceTypes);
    return pParent;
}

//-----------------------------------------------------------------------------
// Построение полигонального представления для отрисовки сцены
// ---
void SceneGenerator::StartBuildGeometry(CommandType buildType)
{
    for (auto it = m_pGeometryHash->m_mapGeometry.begin(); it != m_pGeometryHash->m_mapGeometry.end(); ++it)
    {
        const MbItem* pItem = it.key();
        Geometry* pMathGeometry = it.value();
        if (MathRepresentation* pMathRepresentation = vobject_cast<MathRepresentation*>(pMathGeometry->GetParent()))
        {
            pMathRepresentation->SetBuildType(buildType);
            if (pMathRepresentation->GetMathItem() == nullptr)
                pMathRepresentation->SetMathItem(pItem, m_visualSag);
            else
                pMathRepresentation->RebuildGeometry();
        }
    }
}

void SceneGenerator::SetVisualSag(double visualSag)
{
  m_visualSag = visualSag;
}

static String GetItemName(const String& _name, const MbItem* pItem)
{
    VSN_ASSERT(pItem != nullptr);
    String name = _name;
    std::vector<MbAttribute*> attrib;
    pItem->GetAttributes(attrib, at_ProductAttribute, at_ProductInfo);
    if (!attrib.empty())
    {
        MbProductInfo* prod = static_cast<MbProductInfo*>(attrib[0]);
        c3d::string_t sId, sName, sDescr;
        prod->GetData(sId, sName, sDescr);
        name = sName.c_str() + String("-(") + _name + String(')');
    }
    return name;
}

//-----------------------------------------------------------------------------
// построить сцену сегментов из математической модели
// ---
void SceneGenerator::BuildSceneFromModel(const MbItem* pItem, const MbItem* pParentItem, SceneSegment* pParent,
    const std::vector<MbeSpaceType>& spaceTypes, const MbMatrix3D& mx, uint& countGeom)
{
    if (pItem == nullptr)
        return;

    MbeSpaceType type = pItem->IsA();
    bool bEnabled = std::find(spaceTypes.cbegin(), spaceTypes.cend(), type) != spaceTypes.cend();
    if (bEnabled && (type == st_Solid || type == st_Mesh || type == st_PointFrame ||
        type == st_WireFrame || type == st_SpaceInstance))
    {
#if 0
        const MbFormNote note(GlobalFlags::IsBuildQuadGrid(), true, false);
        MbMesh* mesh = new MbMesh(note.DoExact());
        mesh->AttributesAssign(*pItem);
        pItem->AttributesConvert(*mesh);
        mesh->CopyItemName(*pItem);
        mesh->SetRefItem(pItem);
        MbRegDuplicate* iReg = nullptr;
        pItem->CalculateMesh(MbStepData(ist_SpaceStep, /*Math::visualSag*/0.02), note, *mesh);
        SceneSegmentData* pSegmentData = CreateSegmentData(mesh);
#else
        SceneSegmentData* pSegmentData = CreateSegmentData(pItem);
#endif
        SceneSegment* pNewSegment = new SceneSegment(pSegmentData, pParent);
        pNewSegment->SetObjectName(GetItemName(String("Body"), pItem));

        const SimpleName name = pItem->GetItemName();
        pNewSegment->SetProperty(C3D_PATHITEM_ID, name);

        emit CreateSegment(pNewSegment, pParent);
        emit CreateSegmentItem(pNewSegment, pParent, nullptr, pItem, pParentItem);

        if (m_pProgressBuild)
        {
            if (MathRepresentation* pRepresentation = vobject_cast<MathRepresentation*>(pNewSegment->GetRepresentation()))
                m_pProgressBuild->RegisterGeometryRep(pRepresentation);
        }
        countGeom++;
    }
    else if (bEnabled && type == st_Instance)
    {
        const MbInstance* pInstance = (const MbInstance*)pItem;
        SceneSegment* pNewSegment = new SceneSegment(pParent);
        pNewSegment->SetObjectName(GetItemName(String("Instance"), pItem));

        const SimpleName name = pInstance->GetItemName();
        pNewSegment->SetProperty(C3D_PATHITEM_ID, name);

        emit CreateSegment(pNewSegment, pParent);
        emit CreateSegmentItem(pNewSegment, pParent, nullptr, pInstance, pParentItem);
        if (m_pProgressBuild != nullptr)
        {
            if (MathRepresentation* pRepresentation = vobject_cast<MathRepresentation*>(pNewSegment->GetRepresentation()))
                m_pProgressBuild->RegisterGeometryRep(pRepresentation);
        }

        pNewSegment->GetTransform().SetMatrix(pInstance->GetPlacement().GetMatrixFrom());
        BuildSceneFromModel(pInstance->GetItem(), pInstance, pNewSegment, spaceTypes, pInstance->GetPlacement().GetMatrixFrom() * mx, countGeom);
    }
    else if (bEnabled && type == st_Assembly)
    {
        const MbAssembly* pAssembly = (const MbAssembly*)pItem;

        SceneSegment* pNewSegment = new SceneSegment(pParent);
        pNewSegment->SetObjectName(GetItemName(String("Assembly"), pItem));

        const SimpleName name = pAssembly->GetItemName();
        pNewSegment->SetProperty(C3D_PATHITEM_ID, name);

        emit CreateSegment(pNewSegment, pParent);
        emit CreateSegmentItem(pNewSegment, pParent, nullptr, pAssembly, pParentItem);

        if (m_pProgressBuild != nullptr)
        {
            if (MathRepresentation* pRepresentation = vobject_cast<MathRepresentation*>(pNewSegment->GetRepresentation()))
                m_pProgressBuild->RegisterGeometryRep(pRepresentation);
        }

        for (size_t i = 0, iCount = pAssembly->ItemsCount(); i < iCount; i++)
            BuildSceneFromModel(pAssembly->GetItem(i), pAssembly, pNewSegment, spaceTypes, mx, countGeom);
    }
    else if (bEnabled && type == st_PlaneInstance)
    {
    }
}

static void GatherSameItemsFromModel(const MbItem* pItem, const std::vector<MbeSpaceType>& spaceTypes, std::vector<SPtr<MbItem>>& items)
{
    if (pItem == nullptr)
        return;

    MbeSpaceType type = pItem->IsA();
    bool bEnabled = std::find(spaceTypes.cbegin(), spaceTypes.cend(), type) != spaceTypes.cend();
    if (bEnabled && (type == st_Solid || type == st_Mesh || type == st_PointFrame ||
        type == st_WireFrame || type == st_SpaceInstance))
    {
        SPtr<MbItem> ptrItem((MbItem*)pItem);
        items.push_back(ptrItem);
    }
    else if (bEnabled && type == st_Instance)
    {
        const MbInstance* pInstance = (const MbInstance*)pItem;
        GatherSameItemsFromModel(pInstance->GetItem(), spaceTypes, items);
    }
    else if (bEnabled && type == st_Assembly)
    {
        const MbAssembly* pAssembly = (const MbAssembly*)pItem;
        for (size_t i = 0, iCount = pAssembly->ItemsCount(); i < iCount; i++)
            GatherSameItemsFromModel(pAssembly->GetItem(i), spaceTypes, items);
    }
}

//-----------------------------------------------------------------------------
//
// --
void SceneGenerator::LoadSceneFromModel(SPtr<MbModel> pModel, SceneSegment* pParent, const std::vector<MbeSpaceType>& spaceTypes)
{
    VSN_ASSERT(pModel != nullptr);
    uint countGeom = 0;
    MbMatrix3D mx;
    m_ptrMaterialCache = std::make_shared<MaterialCache>();
    if (m_pGeometryHash->m_bIsSameMeshBody)
    {
        MbModel::ItemConstIterator drawIter_1(pModel->CBegin());
        MbModel::ItemConstIterator drawEnd_1(pModel->CEnd());
        for (; drawIter_1 != drawEnd_1; ++drawIter_1)
            GatherSameItemsFromModel(*drawIter_1, spaceTypes, m_items);

        bool absAcc = true;
        double accuracy = 0.05;
        MbMeshToInstanceParams p(accuracy, absAcc);
        MbMeshToInstanceResults convertRes;
        ItemsSPtrVector meshContainerOld = m_items;
        if (::ConvertMeshToInstance(m_items, p, convertRes) || convertRes._nReplaced == 0)
        {
            pModel->DeleteItems();
            for (auto it : m_items)
                pModel->AddItem(*it);
        }

        MbModel::ItemConstIterator drawIter(pModel->CBegin());
        MbModel::ItemConstIterator drawEnd(pModel->CEnd());
        for (; drawIter != drawEnd; ++drawIter)
            BuildSceneFromModel(*drawIter, nullptr, pParent, spaceTypes, mx, countGeom);
    }
    else
    {
        MbModel::ItemConstIterator drawIter(pModel->CBegin());
        MbModel::ItemConstIterator drawEnd(pModel->CEnd());
        for (; drawIter != drawEnd; ++drawIter)
            BuildSceneFromModel(*drawIter, nullptr, pParent, spaceTypes, mx, countGeom);
    }

    if (countGeom == 0)
        ClearProgressBuild();
}

//-----------------------------------------------------------------------------
//
// --
void SceneGenerator::ClearProgressBuild()
{
    if (m_pProgressBuild != nullptr)
        m_pProgressBuild->DeferredDelete();
    m_ptrMaterialCache.reset();
    VSN_DELETE_AND_NULL(m_pProgressBuild);
}

//-----------------------------------------------------------------------------
//
// --
SceneSegmentData* SceneGenerator::CreateSegmentData(const MbItem* pItem)
{
    SceneSegmentData* pSegmentData = nullptr;
    if (m_pGeometryHash != nullptr)
        pSegmentData = m_pGeometryHash->FindSegmentDataByItem(pItem);

    if (pSegmentData == nullptr)
    {
        MathRepresentation* pMathRep = new MathRepresentation();
#ifdef VSN_OS_UNIX
        pMathRep->SetBuildType(CommandType::Synchronous);
#endif
        pSegmentData = new SceneSegmentData(pMathRep);
        MathGeometry* pMathGeometry = vobject_cast<MathGeometry*>(pMathRep->GetGeomList().front());
        pMathGeometry->SetCustomKey(NodeKey(static_cast<uint64>(pItem->GetItemName())));
        pMathGeometry->SetMaterialCache(m_ptrMaterialCache);
        // регистрировать математику 
        if (m_pGeometryHash != nullptr)
        {
            m_pGeometryHash->AppendGeometry(pItem, pMathGeometry);
            m_pGeometryHash->AppendSegmentData(pItem, pSegmentData);
        }
        return pSegmentData;
    }
    else
        return pSegmentData;
}

//-----------------------------------------------------------------------------
// Функция возвращает основную сборку модели, если она существует.
// The function returns the main assembly of the model, if it exists.
// ---
const MbAssembly* GetMainAssembly(const MbModel* pModel)
{
    if (pModel == nullptr)
        return nullptr;

    const MbAssembly* rootAssm = nullptr;
    MbModel::ItemConstIterator iter(pModel->CBegin()), last(pModel->CEnd());
    // It is considered the first found assembly is the main.
    for (; iter != last; ++iter)
    {
        if (const MbAssembly* assmItem = isa_cast<const MbAssembly*>(*iter))
        {
            if (rootAssm == nullptr)
                rootAssm = assmItem;
            else
                return nullptr; // it requires uniqueness of the main assembly.
        }
    }

    return rootAssm;
}

//-----------------------------------------------------------------------------
// Функция возвращает существующую основную сборку или создает ее, объединяя все элементы модели.
// The function returns an existing main assembly or creates one by uniting all elements of the model.
// ---
MbAssembly* UniteToMainAssembly(MbModel* pModel)
{
    if (pModel == nullptr)
        return nullptr;

    if (const MbAssembly* assmItem = GetMainAssembly(pModel))
        return const_cast<MbAssembly*>(assmItem);

    // If the model is yet empty or hasn't assembly, create a new root assembly.
    std::vector<MbItem*> unitedItems; // (!) It is not owning.
    pModel->DetachItems(unitedItems);

    SPtr<MbAssembly> pNewAssm(new MbAssembly(unitedItems));
    if (pModel->AddItem(*pNewAssm) != nullptr)
        return pNewAssm.get();

    return nullptr;
}
