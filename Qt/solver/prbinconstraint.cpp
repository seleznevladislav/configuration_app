#include "prbinconstraint.h"
#include "assemblysolver.h"

#include <last.h>

//---------------------------------------------------------------------------------------
// Вспомогательная функция выдает тип ограничения по идентификатору команды GUI.
// ---
static GCM_c_type InputConstraintType( const PrBase * proc )
{
    switch ( static_cast<ExplodeWidget::Commands>(proc->GetCustomKey().GetKey()) )
    {
        case ExplodeWidget::Coincident: return GCM_COINCIDENT;
        case ExplodeWidget::Coaxial:    return GCM_CONCENTRIC;
        case ExplodeWidget::Parallel:       return GCM_PARALLEL;
        case ExplodeWidget::Perpendicular:  return GCM_PERPENDICULAR;
        case ExplodeWidget::Distance:   return GCM_DISTANCE;
        case ExplodeWidget::Angular:    return GCM_ANGLE;
    }
    return GCM_UNKNOWN;
}

//---------------------------------------------------------------------------------------
// Returns a child segment of an assembly node including an subNode.
// ---
static SceneSegment * SubSegment(const SceneSegment* assmNode, SceneSegment* subNode)
{
    if ( subNode == nullptr )
    {
        return nullptr;
    }

    SceneSegment * instSeg = subNode;
    do
    {   
        SceneSegment * pParentSeg = instSeg->GetParentSegment();
        if ( pParentSeg == assmNode )
        { 
            return instSeg;
        }
        instSeg = pParentSeg;
    }
    while( instSeg != assmNode );
    
    return nullptr;
}

//---------------------------------------------------------------------------------------
// 
// ---
PrBinConstraint::PrBinConstraint(ExplodeWidget::Commands cmd,
    ExplodeWidget* pSceneWidget, bool& bRuning)
    : PrBase(pSceneWidget)
    , m_selectedGeoms()
{
    SetCustomKey(NodeKey(cmd));
    m_selectColorSave = pSceneWidget->selectManager()->GetSelectionColor();
    pSceneWidget->selectManager()->SetSelectionColor(Color(255, 0, 0));

    Object::Connect(pSceneWidget->selectManager().get(), &SelectionManager::signalItemSelectModified, 
        this, &PrBinConstraint::slotItemSelectModified);
    Object::Connect(this, &PrBase::signalReadyChanged,
        GetSceneWidget(), &ExplodeWidget::slotUpdateCommands);
    UpdateSelectedGeoms();
    bRuning = true;
}

//---------------------------------------------------------------------------------------
// 
// ---
PrBinConstraint::~PrBinConstraint()
{
    GetSceneWidget()->selectManager()->SetSelectionColor(m_selectColorSave);
    GetSceneWidget()->selectManager()->ResetSelect();
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrBinConstraint::slotItemSelectModified()
{
    UpdateSelectedGeoms();
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrBinConstraint::UpdateSelectedGeoms()
{
    m_selectedGeoms.clear();

    MbAssembly * pRootAssm = RootAssembly();  // The main assembly.
    if (pRootAssm==nullptr)
        return;
    
    auto lstItem = GetSceneWidget()->selectManager()->GetSelectionList();
    for (auto it = lstItem.begin(); it != lstItem.end(); ++it)
    {
        if (InstSelectionItem* pSelitem = dynamic_cast<InstSelectionItem*>(*it))
        {
            MbPath path = pSelitem->GetMathPath();
            if (!path.empty() && (pRootAssm->GetItemName() == path.front()) )
            {   
                path.RemoveInd(0);
                // It's an assembly component repositioned by the solver.
                const MbItem* component = pRootAssm->SubItem(path.front());
                // Mating geometry is specified by the component and its geometric (topoligic) element, face, edge, vertex etc.
                if (component != nullptr)
                {
                    
                    AsMatingGeom matedGeom(*component, path, pSelitem->GetPrimitiveItem());
                    m_selectedGeoms.push_back(std::make_pair(pSelitem->GetNodeKey(),matedGeom));
                }
            }
        }
    }    
    
    // Below is the selection of suitable geometric arguments for the given constraint type.
    AsMatingGeom curr;

    auto iter = m_selectedGeoms.begin();
    auto last = m_selectedGeoms.end();
    
    while (iter!=last)
    {
        if (GetSolver().CompatibleConstraint(ConstraintType(), curr, iter->second) == GCM_RESULT_Ok)
        {
            curr = iter->second;
            ++iter;
        }
        else
        {
            --last;
            std::swap(*iter, *last);
        }
    }
    m_selectedGeoms.erase(last, m_selectedGeoms.end());
    // At least two suitable geoms are required to create a binary constraint.
    SetReady(m_selectedGeoms.size() >= 2);
}

//---------------------------------------------------------------------------------------
// 
// ---
AssemblySolver & PrBinConstraint::GetSolver()
{
    VSN_ASSERT( GetSceneWidget() != nullptr && GetSceneWidget()->solver() != nullptr );
    return *GetSceneWidget()->solver();
}

//---------------------------------------------------------------------------------------
// 
// ---
GCM_c_type PrBinConstraint::ConstraintType() const
{
    return InputConstraintType(this);
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrBinConstraint::CreateObject()
{
    PrBase::CreateObject();
    
    if (m_selectedGeoms.size() >= 2) // At least two geoms are required to create a binary constraint.
    {
        auto geomOne = m_selectedGeoms.front();
        auto geomTwo = m_selectedGeoms.back();

        GCM_constraint conId = GCM_NULL;

        switch (ConstraintType())
        { 
            // Cases of dimension constraint.
            case GCM_DISTANCE:
            case GCM_ANGLE:
            {
                conId = GetSolver().AddConstraint( ConstraintType(), geomOne.second, geomTwo.second, 0.0, GCM_CLOSEST );
                GCM_closest_params cPars = GetSolver().ClosestParameters( conId );
                if ( cPars.result == GCM_RESULT_Ok )
                {
                    GetSolver().ChangeDimension( conId, cPars.dimValue.dimValue );
                    GetSolver().ChangeAlignment( conId, cPars.alignVal );
                }
                break;
            }
                
            // Cases of non-dimension binary constraint.
            default:
                conId = GetSolver().AddConstraint( ConstraintType(), geomOne.second, geomTwo.second, GCM_CLOSEST );
                break;
        }

        GCM_result result = GetSolver().Evaluate();
        if (result == GCM_RESULT_Ok)
        {
            // Update positions of the scene segments.
            UpdateAssemblyScene();
            /*
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
            */
        }
        else
        {
            // Cancel the failed constraint.
            GetSolver().RemoveConstraint(conId);
        }
    }
}

//---------------------------------------------------------------------------------------
// 
// ---
void PrBinConstraint::CancelObject()
{
    // TODO: твой код
    PrBase::CancelObject();
}

