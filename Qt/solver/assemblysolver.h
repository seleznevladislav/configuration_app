/////////////////////////////////////////////////////////////////////////////////////////
//
// Assembly constraints solver
// 
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ASSEMBLYSOLVER_H
#define __ASSEMBLYSOLVER_H

#include <gcm_api.h>
#include <gcm_routines.h>
#include <assembly.h>


//--------------------------------------------------------------------------------------
// Variant parameter of a geometric constraint or dimension value.
//---
struct VariantPar
{
    GCM_c_arg	value;
    // Constructor of dimension value.
    VariantPar() : value() {}
    // Constructor of dimension value.
    VariantPar( double dVal ) : value() { value = dVal; }
    // Constructor of alignment option.
    VariantPar( GCM_alignment alignVal ) : value() { value = alignVal;	}
};

//--------------------------------------------------------------------------------------
// Геометрический объект сопряжения (аргумент ограничения).
/*
  Геометрическим объектом сопряжения может быть, как неподвижный объект самой сборки,
  так и элементы ее компонентов (деталей сборки), такие как грани, ребра, вершины или
  вспомогательные оси, плоскости и точки.
*/
//---
class AsMatingGeom
{
public:
  using comp_key = SimpleName; // Key to identify a model assembly component.
  using subcomp_key = std::pair<comp_key,SimpleName>; // Key to identify a model assembly subcomponent.
  using subgeom_key = std::pair<subcomp_key,const MbTopItem*>; // Key to identify an mating element of the component.
public:
    // Empty constructor.
    AsMatingGeom(void/*empty geom*/) 
        : m_compItem(nullptr)
        , m_subItemPath()
        , m_subItemHash()
        , m_subElem()
    {}

    // Data initialization constructor.
    AsMatingGeom(const MbItem& compItem, const MbPath& path, const MbTopItem* elem);
    // Copy constructor.
    AsMatingGeom(const AsMatingGeom& other);
    AsMatingGeom& operator = (const AsMatingGeom& other);
public:    
    /// A unique key to identify a sub-element within the model.    
    subgeom_key Key() const;
    /** \brief  Geometric data record specified in component LCS.
        \details  The func returns a unified representation of geometric data to transmit into the C3D Solver.
                  The result is specified in local coordinates of the component.
    */
    GCM_g_record  SubGeomRecord() const;
    // Transfomation representing the sub-geom in the world.
    MbMatrix3D    WcsMatrix() const;
    // An item representing the assembly component to witch the geometric element belongs.
    const MbItem * CompItem() const { return m_compItem; }
private:    
    SPtr<const MbItem>    m_compItem;     ///< \en An element of the assembly model.
    MbPath                m_subItemPath;  ///< \en Path specified a model graph route from component item to sub-item containing sub-element 'm_subElem'.
    SimpleName            m_subItemHash;  ///< \en 32-bit hash of MbPath from assembly item 'm_compItem' to sub-item containing sub-element 'm_subElem'.    
    SPtr<const MbTopItem> m_subElem;      ///< \en Sub-element of the component.
    /*
    GCM_g_record          m_subGeomRec;   ///< \en Unified record of the sub-element geometry specified in LCS of the component.    
    MbMatrix3D            m_subMatr;      ///< \en Transformation matrix mapping sub-comp LCS to component LCS.
    */
};

//--------------------------------------------------------------------------------------
/// A unique key to identify a sub-element within the model.
/**
  Returns a key triplet where the first.first is a component item key, 
  the first.second is the hash of path to sub-component (see MbPath class)
  and the second is a pointer to topology element of solid geometry.
*/
//---
inline AsMatingGeom::subgeom_key AsMatingGeom::Key() const
{
  const subcomp_key subCompKey(m_compItem == nullptr ? SimpleName(-1) : m_compItem->GetItemName(), m_subItemHash);
  return std::make_pair(subCompKey,m_subElem);
}

class AssemblySolver;

//--------------------------------------------------------------------------------------
// Assembly solver event handler.
//---
struct AsEventHandler
{
    // Working with the assembly solver is started.
    virtual void EvSubscribed(const AssemblySolver &) = 0;
    // The constraints list of the solver changed (added/removed/edited).
    virtual void EvConstraintsChanged() const = 0;
    // The geometry list of the solver changed.
    virtual void EvGeomsChanged() const {}
    // Scene that under solver control is re-evaluated. Geometry state of the assembly updated.
    virtual void EvSceneReEvaluated(const MbAssembly&, GCM_result evalRes) const {}
    // Working with the assembly solver is finished.
    virtual void EvFinish() = 0;

protected:
    ~AsEventHandler(){};
};

//--------------------------------------------------------------------------------------
// This class provides the assembly constraints solving.
/*
    This class allows assembly components to be mated using a constraint system.
*/
//---
class AssemblySolver
{
public:
    using comp_key = SimpleName; // Key to identify a model assembly component.
    //using subcomp_key = MatingGeom::subcomp_key; // Key to identify a model assembly subcomponent.
    using subgeom_key = AsMatingGeom::subgeom_key; // Key to identify an mating element of the component.
    using changed_callback = void(*)(int32_t, int32_t);
public:
    AssemblySolver(MbAssembly*);
   ~AssemblySolver();
public:
    MbAssembly  & SolverAssembly() { return *m_assmModel; }
    // Get range of constraints registered in the solver.
    GeConstraintsRange Constraints() const;
    // Get string record of a constraint item.
    std::string StrRecord(GCM_constraint conId) const;
    // Request already registered geometry component of the assembly.
    GCM_geom CompId(const MbItem*) const;
    // Request a ready or newly registered geometry component of the assembly.
    GCM_geom QueryCompId(const MbItem*);
    // Request a ready or newly registered geometry argument descriptor.
    GCM_geom QueryGeomId(const AsMatingGeom&);
    // Add an binary constraint of specifed type. The call doesn't re-evaluate the constraint system.
    GCM_constraint AddConstraint(GCM_c_type, const AsMatingGeom&, const AsMatingGeom&, VariantPar par1, VariantPar par2 = GCM_CLOSEST);
    // Get type of a geometric constraint.
    GCM_c_type ConstraintType(GCM_constraint conId) const;
    // Fix the assembly component.
    void Freeze(const MbItem* compItem);
    // Update component positons in the model after re-evaluation.
    void UpdatePositions(GCM_result evalRes);
    // Remove constraint from the constraint system.
    void RemoveConstraint(GCM_constraint);
    // Subscribe to the assembly solver events.
    void Subscribe(AsEventHandler*);
    // Subscribe to the log messages.
    void SubscribeLogs(GCM_log_func, GCM_extra_param) const;
    // Read the constraint system from the file.
    bool Read(reader& in);
    // Write the content of constraint system and its associative data to the file.
    bool Write(writer& out) const;
    // Recalculate the assembly model so that the position of the components satisfies the constraints and dimensions.
    GCM_result Evaluate();
    // Change alignment option without recalculation.
    GCM_result ChangeAlignment(GCM_constraint dimId, GCM_alignment aVal);
    // Change driving dimension value without recalculation.
    GCM_result ChangeDimension(GCM_constraint dimId, double dVal);
    // This method checks compatability of the binary constraint and its geometric arguments.
    GCM_result CompatibleConstraint(GCM_c_type, const AsMatingGeom&, const AsMatingGeom&, GCM_alignment = GCM_NO_ALIGNMENT);
    // Dynamic rotation to interact with the model while dragging.
    GCM_result DynamicRotation(const AsMatingGeom& axialGeom, double angPos);
    // Dynamic rotation to interact with the model while dragging.
    GCM_result DynamicRotation(const MbItem& rtComp, const MbAxis3D& rtAxis, double angPos);
    // Finish reposition mode.
    GCM_result DynamicRotationStop(const MbItem& rtComp);
    // Measure alignment option and the dimension value closest to the current state of dimensional geometry.
    GCM_closest_params ClosestParameters(GCM_constraint dimId) const;

private:
    GCM_system       m_gcSystem;  ///< Geometric constraint system provided by C3D Solver.
    SPtr<MbAssembly> m_assmModel; ///< Assembly model containing mating components (always non-nullptr).
    std::map<subgeom_key,GCM_geom> m_subGeoms;  ///< Registered sub-geoms that are used as geometric constraint arguments.
    std::map<comp_key,GCM_geom>    m_compGeoms; ///< Registered geoms that are used as geometric component of the assembly.
    AsEventHandler* m_slvHandler; ///< An event handler subscribed to the solver.
private:
    OBVIOUS_PRIVATE_COPY(AssemblySolver);
};

using AssemblySolverPtr = std::shared_ptr<AssemblySolver>;

//----------------------------------------------------------------------------------------
// Save the constraint system to the assembly attributes binary storage.
//---
bool SaveConstraintSystem(const AssemblySolver & solver, MbAssembly & assm);

//----------------------------------------------------------------------------------------
// Restore the constraint system from the assembly attributes binary storage.
//---
bool RestoreConstraintSystem(AssemblySolver & solver, MbAssembly & assm);


#endif // __ASSEMBLYSOLVER_H

// eof