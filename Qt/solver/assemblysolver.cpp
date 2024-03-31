/////////////////////////////////////////////////////////////////////////////////////////
//
// Assembly Constraint System
// 
/////////////////////////////////////////////////////////////////////////////////////////

#include "assemblysolver.h"

#include <attr_common_attribute.h>
#include <mb_class_traits.h>
#include <mb_axis3d.h>
#include <instance.h>

#include <last.h>

//---------------------------------------------------------------------------------------
// Data initialization constructor.
//---
AsMatingGeom::AsMatingGeom(const MbItem& compItem, const MbPath& path, const MbTopItem* elem)
    : m_compItem(&compItem)
    , m_subItemPath(path)
    , m_subItemHash(path.Hash())
    , m_subElem(elem)
{}

//---------------------------------------------------------------------------------------
// Copy initialization constructor.
//---
AsMatingGeom::AsMatingGeom(const AsMatingGeom& other)
    : m_compItem(other.m_compItem)
    , m_subItemPath(other.m_subItemPath)
    , m_subItemHash(other.m_subItemHash)
    , m_subElem(other.m_subElem)
{}

//---------------------------------------------------------------------------------------
// Assignment.
//---
AsMatingGeom& AsMatingGeom::operator = (const AsMatingGeom& other) 
{
    m_compItem = other.m_compItem;
    m_subItemPath = other.m_subItemPath;
    m_subItemHash = other.m_subItemHash;
    m_subElem = other.m_subElem;
    return *this; 
}

//---------------------------------------------------------------------------------------
/** \brief  Geometric data record specified in component LCS.
    \details  The func returns a unified representation of geometric data to transmit into the C3D Solver.
              The result is specified in local coordinates of the component.
*/
//---
GCM_g_record AsMatingGeom::SubGeomRecord() const
{
    if (m_compItem==nullptr)
      return GCM_NullGeom();

    GCM_g_record subGeomRec(GCM_NullGeom());

    MbMatrix3D subMatr; // Transformation matrix mapping sub-comp LCS to component LCS.

    if (const MbItem * subItem = m_compItem->GetItemByPath(m_subItemPath, m_subItemPath.MaxIndex(), subMatr, 0/*begIdx*/))
    {
        MtGeomVariant gRec;
        if (const MbFace * face = isa_cast<const MbFace*>(m_subElem))
        {
            gRec = GCM_GeomArgument(&face->GetSurface().GetBasisSurface(), face->IsSameSense());
        }        
        else if (const MbCurveEdge * edge = isa_cast<const MbCurveEdge*>(m_subElem))
        {
            gRec = GCM_GeomArgument(&edge->GetCurve(), edge->IsSameSense());
        }
        else if (const MbVertex * vert = isa_cast<const MbVertex*>(m_subElem))
        {
            gRec.Assign(vert->GetCartPoint());
        }
        gRec.Transform( subMatr ); // Transform to world CS.

        // Transform the geometric element into component local coordinate system (LCS).
        MbPlacement3D compLCS;
        if ( m_compItem->GetPlacement(compLCS) )
        {
            gRec.Transform( compLCS.GetMatrixInto() );
        }
        return gRec.GeomRecord();
    }
  
    return subGeomRec;
}

//---------------------------------------------------------------------------------------
// Transfomation representing the sub-geom in the world.
//---
MbMatrix3D AsMatingGeom::WcsMatrix() const 
{  
  MbPlacement3D compLCS;
  if ( m_compItem!=nullptr && m_compItem->GetPlacement(compLCS) )
  {
      const GCM_g_record gRec(SubGeomRecord());
      MbMatrix3D matr;
      matr.SetAxisX().Init(gRec.axisX);
      matr.SetAxisY().Init(gRec.axisY);
      matr.SetAxisZ().Init(gRec.axisZ);
      matr.SetOrigin().Init(gRec.origin);
      return matr.Transform( compLCS.GetMatrixFrom() );
  }  
  return MbMatrix3D::identity; 
}

//---------------------------------------------------------------------------------------
// 
//---
AssemblySolver::AssemblySolver(MbAssembly* assmModel)
    : m_gcSystem(GCM_CreateSystem())
    , m_subGeoms()
    , m_compGeoms()
    , m_assmModel(assmModel)
    , m_slvHandler(nullptr)
{
    C3D_ASSERT(m_assmModel!=nullptr);
    if (m_assmModel==nullptr)
        m_assmModel = new MbAssembly;
}

//---------------------------------------------------------------------------------------
// 
//---
AssemblySolver::~AssemblySolver()
{
    GCM_RemoveSystem(m_gcSystem);
    if (m_slvHandler != nullptr)
    { 
        m_slvHandler->EvFinish();
    }
}

//---------------------------------------------------------------------------------------
// Subscribe to the assembly solver events.
//---
void AssemblySolver::Subscribe(AsEventHandler* evHandler)
{
    if (m_slvHandler != nullptr)
    { 
        m_slvHandler->EvFinish();
    }
    m_slvHandler = evHandler;
    m_slvHandler->EvSubscribed(*this);
}


//---------------------------------------------------------------------------------------
// Get range of constraints registered in the solver.
//---
GeConstraintsRange AssemblySolver::Constraints() const
{
  return GCM_GetConstraints( m_gcSystem );
}

//---------------------------------------------------------------------------------------
// Get string record of a constraint item.
//---
std::string AssemblySolver::StrRecord(GCM_constraint cItem) const
{
    std::string strRec;
    return GCM_SExprRecord( m_gcSystem, cItem, strRec );
}

//---------------------------------------------------------------------------------------
// Request already registered geometry component of the assembly.
//---
GCM_geom AssemblySolver::CompId(const MbItem* compItem) const
{
    if (compItem == nullptr)
        return GCM_NULL;

    // Find already registered component element.
    auto geomIter = m_compGeoms.find(compItem->GetItemName());
    if (geomIter != m_compGeoms.end())
    {
        return geomIter->second;
    }

    if (compItem->IsA() == st_Assembly)
    {
        // This case represents an assembly whose LCS is global. 
        return GCM_GROUND;
    }
    
    return GCM_NULL;
}

//---------------------------------------------------------------------------------------
// Request a ready or newly registered geometry component of the assembly.
//---
GCM_geom AssemblySolver::QueryCompId(const MbItem* compItem)
{
    if (compItem == nullptr)
        return GCM_NULL;

    // Before all find already registered component geom. 
    const GCM_geom compId = CompId(compItem);
    if (compId != GCM_NULL)
    {
        return compId;
    }

    MbPlacement3D lcs;
    if (compItem->GetPlacement(lcs))
    {
        auto newGeomId = GCM_AddGeom(m_gcSystem, GCM_SolidLCS(lcs));
        if (compItem->IsA() != st_Instance)
        {
            GCM_FreezeGeom(m_gcSystem,newGeomId);
        }
        return m_compGeoms[compItem->GetItemName()] = newGeomId;
    }
    
    return GCM_NULL;
}

//---------------------------------------------------------------------------------------
// Request a ready or newly registered geometry argument descriptor.
//---
GCM_geom AssemblySolver::QueryGeomId(const AsMatingGeom& mtGeom)
{    
    // Before all find already registered sub-element. 
    auto subGeomIter = m_subGeoms.find(mtGeom.Key());
    if ( subGeomIter != m_subGeoms.end() )
    {
        return subGeomIter->second;
    }
    
    // The geometric sub-element is registering for the first time.    
    GCM_geom compId = QueryCompId( mtGeom.CompItem() );
    if (compId != GCM_NULL)
    { 
        GCM_geom & subGeomId = m_subGeoms[mtGeom.Key()];
        subGeomId = GCM_SubGeom(m_gcSystem, compId, mtGeom.SubGeomRecord());
        return subGeomId;
    }
    return GCM_NULL;
}

//---------------------------------------------------------------------------------------
// Add an binary constraint of specifed type. The call doesn't re-evaluate the constraint system.
//---
GCM_constraint AssemblySolver::AddConstraint(GCM_c_type cType, const AsMatingGeom& mtGeom1, 
                                             const AsMatingGeom& mtGeom2, VariantPar par1, VariantPar par2 )
{
    GCM_constraint conId = GCM_NULL;
    GCM_geom gArg1 = QueryGeomId( mtGeom1 );
    GCM_geom gArg2 = QueryGeomId( mtGeom2 );
    switch(cType)
    {
        case GCM_COINCIDENT:
        case GCM_CONCENTRIC:
        case GCM_PARALLEL:
        case GCM_PERPENDICULAR:
        case GCM_TANGENT:
            conId = GCM_AddBinConstraint( m_gcSystem, cType, gArg1, gArg2, par1.value.alignVal, par2.value.tanChoice );
            break;
        case GCM_DISTANCE:
            conId = GCM_AddDistance( m_gcSystem, gArg1, gArg2, par1.value.dimValue, par2.value.alignVal );
            break;
        case GCM_ANGLE:
            conId = GCM_AddAngle( m_gcSystem, gArg1, gArg2, GCM_NULL, par1.value.dimValue );
            break;
    }
    if (m_slvHandler != nullptr && conId != GCM_NULL )
        m_slvHandler->EvConstraintsChanged();
    return conId;
}

//---------------------------------------------------------------------------------------
// Get type of a geometric constraint.
//---
GCM_c_type AssemblySolver::ConstraintType(GCM_constraint conId) const
{
    if (const ItConstraintItem * cItem = GCM_ConstraintItem(m_gcSystem,conId))
        return cItem->ConstraintType();    
    return GCM_UNKNOWN;
}

//---------------------------------------------------------------------------------------
// Fix the assembly component.
//---
void AssemblySolver::Freeze(const MbItem* compItem)
{
    if (compItem)
        GCM_FreezeGeom(m_gcSystem, QueryCompId(compItem));
}

//---------------------------------------------------------------------------------------
// Reposition of the assembly components to apply re-evaluation result.
//---
void AssemblySolver::UpdatePositions(GCM_result evalRes)
{    
    for ( auto compId: m_compGeoms )
    {
        if (const MbItem* compItem = m_assmModel->SubItem(compId.first))
        {
          const_cast<MbItem*>(compItem)->SetPlacement(GCM_Placement(m_gcSystem,compId.second));
        }
    }
    if (m_slvHandler != nullptr)
    { 
        m_slvHandler->EvSceneReEvaluated(*m_assmModel,evalRes);
    }
}

//---------------------------------------------------------------------------------------
// Recalculate the assembly model so that the position of the components satisfies the constraints and dimensions.
//---
GCM_result AssemblySolver::Evaluate()
{
    GCM_result evalRes = GCM_Evaluate(m_gcSystem);
    if (evalRes == GCM_RESULT_Ok)
    {
        // Reposition of the assembly components to apply re-evaluation result.
        UpdatePositions(evalRes);
    }
    return evalRes;
}

//---------------------------------------------------------------------------------------
// Change alignment option without recalculation.
//---
GCM_result AssemblySolver::ChangeAlignment(GCM_constraint conId, GCM_alignment aVal)
{
    return GCM_ChangeAlignment(m_gcSystem, conId, aVal);
}

//---------------------------------------------------------------------------------------
// Change driving dimension value without recalculation.
//---
GCM_result AssemblySolver::ChangeDimension( GCM_constraint conId, double dVal )
{
  return GCM_ChangeDrivingDimension( m_gcSystem, conId, dVal );
}

//---------------------------------------------------------------------------------------
// This method checks compatability of the binary constraint and its geometric arguments. 
//---
GCM_result AssemblySolver::CompatibleConstraint(GCM_c_type, const AsMatingGeom&, 
                                                const AsMatingGeom&, GCM_alignment)
{
    // TODO: Yet not implemented.
    return GCM_RESULT_Ok;
}

//---------------------------------------------------------------------------------------
// Dynamic rotation to interact with the model while dragging.
//---
GCM_result AssemblySolver::DynamicRotation(const AsMatingGeom& axialGeom, double angPos)
{
    GCM_geom shaft = QueryCompId(axialGeom.CompItem());
    if (shaft == GCM_GetMovingGeom(m_gcSystem))
    {
        GCM_result evalRes = GCM_SolveReposition(m_gcSystem,angPos);
        // Reposition of the assembly components to apply re-evaluation result.
        UpdatePositions(evalRes);
        return evalRes;
    }
    // The axis of rotation is taken on the manipulation object itself.
    MbMatrix3D subGeomLcs = axialGeom.WcsMatrix();
    return GCM_PrepareReposition(m_gcSystem, shaft, subGeomLcs.GetOrigin(), subGeomLcs.GetAxisZ());
}

//---------------------------------------------------------------------------------------
// Dynamic rotation to interact with the model while dragging.
//---
GCM_result AssemblySolver::DynamicRotation(const MbItem& rtComp, const MbAxis3D& rtAxis
                                         , double angPos)
{
    GCM_geom shaft = QueryCompId(&rtComp);
    if (shaft == GCM_GetMovingGeom(m_gcSystem))
    {
        GCM_result evalRes = GCM_SolveReposition(m_gcSystem,angPos);
        // Reposition of the assembly components to apply re-evaluation result.
        UpdatePositions(evalRes);
        return evalRes;
    }
    return GCM_PrepareReposition(m_gcSystem, shaft, rtAxis.GetOrigin(), rtAxis.GetAxisZ());
}

//---------------------------------------------------------------------------------------
// Finish reposition mode.
//---
GCM_result AssemblySolver::DynamicRotationStop(const MbItem& rtComp)
{
    GCM_geom shaft = QueryCompId(&rtComp);
    if (shaft == GCM_GetMovingGeom(m_gcSystem))
    {
        GCM_FinishReposition(m_gcSystem);
        return GCM_RESULT_Ok;
    }
    return GCM_RESULT_None;
}

//---------------------------------------------------------------------------------------
// Remove constraint from the constraint system.
//---
void AssemblySolver::RemoveConstraint(GCM_constraint conId)
{
    GCM_RemoveConstraint(m_gcSystem, conId);
    if (m_slvHandler != nullptr && conId != GCM_NULL )
        m_slvHandler->EvConstraintsChanged();    
    return;
}

//---------------------------------------------------------------------------------------
// Subscribe to the log messages.
//---
void AssemblySolver::SubscribeLogs(GCM_log_func logFunc, GCM_extra_param extra) const 
{
    GCM_SubscribeJournal(m_gcSystem, logFunc, extra);
}

//----------------------------------------------------------------------------------------
// Measure alignment option and the dimension value closest to the current state of dimensional geometry.
//---
GCM_closest_params AssemblySolver::ClosestParameters(GCM_constraint dimId) const
{
    return GCM_ClosestParameters(m_gcSystem, dimId);
}

//----------------------------------------------------------------------------------------
//
//---
enum AsFileTag8: uint8
{
      FILE_TAG_Unknown = 0
    , FILE_TAG_CompId  = 0xE1
    , FILE_TAG_SubGeom = 0xE2
    , FILE_TAG_EoS = 0xFE
};

//----------------------------------------------------------------------------------------
// Read the constraint system from the file.
//---
bool AssemblySolver::Read(reader& in)
{
    C3D_ASSERT(m_compGeoms.empty());
    if ( GCM_ReadSystem(m_gcSystem, in) )
    {
        for (uint8 fTag = FILE_TAG_Unknown; in.good() && fTag!=FILE_TAG_EoS;)
        {   
            in >> fTag;
            switch (fTag)
            {
                case FILE_TAG_CompId:
                    {
                        std::pair<comp_key,GCM_geom> compId;
                        in >> compId.first;
                        in >> compId.second.id;
                        m_compGeoms.insert(compId);
                    }
                    break;
                case FILE_TAG_EoS:
                    break;
                default:
                    in.setState(io::badData);
                    break;
            }
        }
        return !!in.good();
    }

  return false;
}

//----------------------------------------------------------------------------------------
// Write the content of constraint system and its associative data to the file.
//---
bool AssemblySolver::Write(writer& out) const
{
    if (GCM_WriteSystem(m_gcSystem, out))
    {        
        // Write component keys mapping.
        for (auto compPair: m_compGeoms )
        { 
            out << FILE_TAG_CompId;
            out << compPair.first;
            out << compPair.second.id;
        }
        out << FILE_TAG_EoS;  // The end of section.
        return !!out.good();
    }

    return false;
}

//----------------------------------------------------------------------------------------
//
//---
SPtr<MbBinaryAttribute> ConstraintSystemAttribute( MbAssembly & assm )
{
    const TCHAR gcAttrTag[] = _T("C3DVision_60_AssemblySolver");
    SPtr<MbBinaryAttribute> biAttr( nullptr );
    
    c3d::AttrVector attrs;
    assm.GetAttributes(attrs,at_BinaryAttribute);
    for (auto attr:attrs)
    {
        if (attr->AttributeType() == at_BinaryAttribute)
        { 
            biAttr = static_cast<MbBinaryAttribute*>(attr);
            if (biAttr->GetPrompt()== gcAttrTag)
            {
                return biAttr;
            }
        }
    }
    if (biAttr == nullptr)
    {
        biAttr = new MbBinaryAttribute(gcAttrTag, true, std::vector<unsigned char>());
        SPtr<MbAttribute> newAttr(assm.AddAttribute(biAttr, false));
        if (newAttr->AttributeType() == at_BinaryAttribute)
        { 
            biAttr = static_cast<MbBinaryAttribute*>(newAttr.get());
        }
        
    }
    return biAttr;
}

//----------------------------------------------------------------------------------------
// Save the constraint system to the assembly attributes binary storage.
//---
bool SaveConstraintSystem(const AssemblySolver& solver, MbAssembly& assm)
{
    SPtr<MbBinaryAttribute> biAttr( ConstraintSystemAttribute(assm) );
    if (biAttr != nullptr)
    {
        membuf buf;
        writer::writer_ptr out = writer::CreateMemWriter( buf, 0 );
        if ( out.get() != nullptr && out->good() )
        {
            if (solver.Write(*out))
            {
                buf.closeBuff();
                std::vector<unsigned char> stlBuf( buf.getMemLen(), (char)0); // temporary dynamically allocated array.
                const char * stlBufData = (char*)stlBuf.data();
                buf.toMemory(stlBufData, stlBuf.size());
                biAttr->SetValue(stlBuf);                
                return true;
            }
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------
// Restore the constraint system from the assembly attributes binary storage.
//---
bool RestoreConstraintSystem(AssemblySolver& solver, MbAssembly& assm)
{    
    SPtr<MbBinaryAttribute> biAttr( ConstraintSystemAttribute(assm) );    
    if (biAttr != nullptr)
    {        
        membuf buf;        
        std::vector<unsigned char> stlBuf(biAttr->GetValue()); // temporary dynamically allocated array.
        const char * stlBufData = (char*)stlBuf.data();
        buf.fromMemory(stlBufData);
        
        reader::reader_ptr in = reader::CreateMemReader( buf, 0 );
        if (in.get() != nullptr && in->good())
        {
            if (solver.Read(*in))
            {
                buf.closeBuff();
                return true;
            }
        }
    }
    return false;
}

// eof