#ifndef __VSN_PRBINCONSTRAINT_H
#define __VSN_PRBINCONSTRAINT_H

#include <tool_cstring.h>
#include <topology_item.h>

#include <scenegenerator.h>
#include "prbase.h"
#include "assemblysolver.h"

VSN_USE_NAMESPACE

/////////////////////////////////////////////////////////////////////////////////////////
// 
//  Proccess for adding a new binary constraint.
// 
/////////////////////////////////////////////////////////////////////////////////////////
class PrBinConstraint : public PrBase
{
    using selected_geom_t = std::pair<NodeKey,AsMatingGeom>;  // A pair of key and mating geometry.
    VSN_OBJECT(PrBinConstraint)
public:
    explicit PrBinConstraint(ExplodeWidget::Commands cmd,
        ExplodeWidget* pSceneWidget, bool& bRuning);
    virtual ~PrBinConstraint();
protected:
    void slotItemSelectModified();
private:  // Internal auxiliary methods.
    void UpdateSelectedGeoms();
    AssemblySolver & GetSolver(); ///< The solver maintains the process creating the constraint system.
    GCM_c_type  ConstraintType() const; ///< What type of constraint the process is creating.

public:
    virtual void CreateObject() override;
    virtual void CancelObject() override;
private:
    Color m_selectColorSave;
    std::vector<selected_geom_t> m_selectedGeoms;  // Set of geometric constraint arguments corresponding to the current selection.
private:
    VSN_DISABLE_COPY(PrBinConstraint)
};

#endif // __VSN_PRBINCONSTRAINT_H
