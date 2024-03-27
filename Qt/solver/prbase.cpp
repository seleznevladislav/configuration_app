#include "prbase.h"

#include "assemblysolver.h"
#include <model_entity.h>
#include <mb_class_traits.h>
#include <scenegenerator.h>

#include <last.h>

//-----------------------------------------------------------------------------
// 
// ---
PrBase::PrBase(ExplodeWidget* pSceneWidget)
    : VSN::Node()
    , m_bReady(false)
    , m_pSceneWidget(pSceneWidget)
{
}

//-----------------------------------------------------------------------------
// 
// ---
PrBase::~PrBase() {}

//-----------------------------------------------------------------------------
// 
// ---
void PrBase::SetReady(bool bReady) 
{ 
    if (m_bReady != bReady)
    {
        m_bReady = bReady; 
        emit signalReadyChanged();
    }
}

//-----------------------------------------------------------------------------
// 
// ---
void PrBase::CreateObject()
{
    emit signalReadyChanged();
}

//-----------------------------------------------------------------------------
// 
// ---
void PrBase::CancelObject()
{
    emit signalReadyChanged();
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrBase::SetEnableFindProcess(bool bEnabled)
{
    auto lstFind = GetSceneWidget()->selectManager()->GetObjectPickSelection()->GetFindProcesses();
    for (PrBasicFindSelect* pFind : lstFind)
        pFind->SetEnabled(bEnabled);
}

//-----------------------------------------------------------------------------
// A main assembly is always contained in the model.
// ---
MbAssembly* PrBase::RootAssembly() const
{
    return const_cast<MbAssembly*>(::GetMainAssembly(GetSceneWidget()->model()));
}

//---------------------------------------------------------------------------------------
// 
// ---
AssemblySolver & PrBase::GetSolver()
{
    VSN_ASSERT( GetSceneWidget() != nullptr && GetSceneWidget()->solver() != nullptr );
    return *GetSceneWidget()->solver();
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrBase::UpdateAssemblyScene()
{
    // Update positions of the scene segments.
    const MbAssembly * assm = RootAssembly();
    const SceneSegment * pAssmSeg = GetSceneWidget()->assemblySegment(); // Segment corresponding to the root assembly.
    for( auto subSeg: pAssmSeg->GetChildSegments())
    { 
        const Variant varId = subSeg->GetProperty(C3D_PATHITEM_ID);
        if ( !varId.IsEmpty() )
        {
            if ( const MbItem * compItem = assm->SubItem(varId.GetUInt32()) )
            {                        
                MbPlacement3D lcs;
                if ( compItem->GetPlacement(lcs) )
                {
                    subSeg->GetTransform().SetMatrix(lcs.GetMatrixFrom());
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// 
//  Proccess for fixing component.
// 
/////////////////////////////////////////////////////////////////////////////////////////
class PrFixItem : public PrBase
{
    using selected_geom_t = std::pair<NodeKey,const MbItem*>; // A pair of key and fixed geometry.
    VSN_OBJECT(PrFixItem)
public:
    explicit PrFixItem(ExplodeWidget* pSceneWidget);
    virtual ~PrFixItem();
protected:
    void slotItemSelectModified();
private:  // Internal auxiliary methods.
    selected_geom_t SelectedComponent();    
public:
    virtual void CreateObject() override;
    virtual void CancelObject() override { return PrBase::CancelObject();}
private:
    Color m_selectColorSave;
    std::vector<selected_geom_t> m_selectedGeoms;
private:
    VSN_DISABLE_COPY(PrFixItem)
};

//---------------------------------------------------------------------------------------
// 
// ---
PrFixItem::PrFixItem(ExplodeWidget* pSceneWidget)
    : PrBase(pSceneWidget)
    , m_selectedGeoms()
{
    SetCustomKey(NodeKey(ExplodeWidget::FixItem));
    m_selectColorSave = pSceneWidget->selectManager()->GetSelectionColor();
    pSceneWidget->selectManager()->SetSelectionColor(Color(255, 0, 0));

    Object::Connect(pSceneWidget->selectManager().get(), &SelectionManager::signalItemSelectModified,
        this, &PrFixItem::slotItemSelectModified);
    Object::Connect(this, &PrBase::signalReadyChanged,
        GetSceneWidget(), &ExplodeWidget::slotUpdateCommands);
    SelectedComponent();
    SetReady(!m_selectedGeoms.empty());
}

//---------------------------------------------------------------------------------------
// 
// ---
PrFixItem::~PrFixItem()
{
    GetSceneWidget()->selectManager()->SetSelectionColor(m_selectColorSave);
    GetSceneWidget()->selectManager()->ResetSelect();
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrFixItem::slotItemSelectModified()
{
    SelectedComponent();
    SetReady(!m_selectedGeoms.empty());
    /*
    if (!m_selectedGeoms.empty())
    {
        // Just create operation when first selection happened.
        CreateObject();
    }
*/
}

//---------------------------------------------------------------------------------------
// 
// ---
PrFixItem::selected_geom_t PrFixItem::SelectedComponent()
{
    m_selectedGeoms.clear();

    MbAssembly * pRootAssm = RootAssembly();  // The main assembly.
    if (pRootAssm == nullptr)
        return std::make_pair(NodeKey::ZERO, nullptr);
    
    auto lstItem = GetSceneWidget()->selectManager()->GetSelectionList();
    for (auto it = lstItem.begin(); it != lstItem.end(); ++it)
    {
        if (InstSelectionItem* pSelitem = dynamic_cast<InstSelectionItem*>(*it))
        {
            MbPath path = pSelitem->GetMathPath();
            if (!path.empty() && (pRootAssm->GetItemName() == path.front()) )
            {   
                path.RemoveInd(0);
                // Its an assembly component repositioned by the solver.
                const MbItem* component = pRootAssm->SubItem(path.front());                
                if (component != nullptr)
                {
                    m_selectedGeoms.push_back(std::make_pair(pSelitem->GetNodeKey(), component));
                }
            }
        }
    }
    return m_selectedGeoms.empty() ? std::make_pair(NodeKey::ZERO, nullptr) : m_selectedGeoms.front();
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrFixItem::CreateObject()
{
    PrBase::CreateObject();        
    for(selected_geom_t compItem:m_selectedGeoms)
    {
        GetSolver().Freeze(compItem.second);
    }
}

//-----------------------------------------------------------------------------
// Start the fix item procces.
// ---
PrBase* StartFixingProcess(ExplodeWidget* pSceneWidget)
{
    return new PrFixItem(pSceneWidget);
}
