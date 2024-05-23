#include <cassert>

#include <action_general.h>
#include <action_point.h>
#include <action_curve.h>
#include <action_curve3d.h>
#include <action_surface_curve.h>
#include <action_surface.h>
#include <action_direct.h>
#include <action_mesh.h>
#include <action_phantom.h>
#include <action_shell.h>
#include <action_sheet.h>
#include <action_solid.h>
#include <templ_s_array.h>
#include <templ_rp_array.h>
#include <templ_array2.h>
#include <templ_sptr.h>
#include <templ_dptr.h>
#include <mb_cart_point.h>
#include <mb_cart_point3d.h>
#include <mb_matrix.h>
#include <mb_matrix3d.h>
#include <mb_matrixnn.h>
#include <mb_placement.h>
#include <mb_placement3d.h>
#include <mb_axis3d.h>
#include <mb_thread.h>
#include <mb_nurbs_function.h>
#include <function.h>
#include <function_factory.h>
#include <curve.h>
#include <cur_line_segment.h>
#include <cur_arc.h>
#include <cur_nurbs.h>
#include <cur_hermit.h>
#include <cur_reparam_curve.h>
#include <cur_trimmed_curve.h>
#include <cur_character_curve.h>
#include <cur_polyline.h>
#include <cur_projection_curve.h>
#include <cur_contour.h>
#include <region.h>
#include <contour_graph.h>
#include <curve3d.h>
#include <cur_line_segment3d.h>
#include <cur_arc3d.h>
#include <cur_nurbs3d.h>
#include <cur_hermit3d.h>
#include <cur_trimmed_curve3d.h>
#include <cur_reparam_curve3d.h>
#include <cur_character_curve3d.h>
#include <cur_contour3d.h>
#include <cur_plane_curve.h>
#include <cur_surface_curve.h>
#include <cur_contour_on_surface.h>
#include <cur_silhouette_curve.h>
#include <cur_surface_intersection.h>
#include <cur_spiral.h>
#include <surface.h>
#include <surf_plane.h>
#include <surf_cylinder_surface.h>
#include <surf_cone_surface.h>
#include <surf_sphere_surface.h>
#include <surf_torus_surface.h>
#include <surf_extrusion_surface.h>
#include <surf_revolution_surface.h>
#include <surf_lofted_surface.h>
#include <surf_evolution_surface.h>
#include <surf_spiral_surface.h>
#include <surf_spline_surface.h>
#include <surf_offset_surface.h>
#include <surf_curve_bounded_surface.h>
#include <attribute_item.h>
#include <attr_color.h>
#include <attr_common_attribute.h>
#include <attr_dencity.h>
#include <attr_identifier.h>
#include <attr_selected.h>
#include <attr_user_attribute.h>
#include <attribute_container.h>
#include <name_item.h>
#include <name_check.h>
#include <topology.h>
#include <topology_faceset.h>
#include <check_geometry.h>
#include <point3d.h>
#include <point_frame.h>
#include <wire_frame.h>
#include <creator.h>
#include <cr_simple_creator.h>
#include <solid.h>
#include <instance_item.h>
#include <plane_instance.h>
#include <space_instance.h>
#include <assembly.h>
#include <mesh.h>
#include <mesh_grid.h>
#include <mesh_primitive.h>
#include <mesh_polygon.h>
#include <map_create.h>
#include <map_lump.h>
#include <map_section.h>
#include <map_section_complex.h>
#include <map_vestige.h>
#include <map_thread.h>
#include <tri_face.h>
#include <mip_curve_properties.h>
#include <mip_solid_area_volume.h>
#include <mip_solid_mass_inertia.h>
#include "gcm_api.h"
#include "gcm_constraint.h"
#include "gcm_manager.h"
#include "gcm_types.h"
#include <cdet_bool.h>
#include <cdet_data.h>
#include <cdet_utility.h>
#include <part_solid.h>
#include <mb_class_traits.h>
#include <tool_time_test.h>
#include <constraint.h>
#include <alg_base.h>
#include <alg_draw.h>
#include <generic_utility.h>
#include <conv_model_exchange.h>
#include <algorithm>
#include <ctime>
#include <functional>
#include <map>
#include <vector>
#include <utility>
#include <cur_cone_spiral.h>
#include <math_namespace.h>
#include <last.h>

#include <vector>
#include "alg_curve_fillet.h"
#include "mb_placement3d.h"
#include "cur_polyline.h"
#include "surf_plane.h"
#include "action_solid.h"
#include "cur_arc.h"
//Our Files
#include <math_namespace.h>
#include "BuildParams.h"
//GCM
#include "gcm_api.h"
#include "gcm_constraint.h"
#include "gcm_manager.h"
#include "gcm_types.h"

// Attributes 
#include <attr_product.h>

// Base
#include <iostream>
#include <unordered_map>


using namespace c3d;
using namespace std;

namespace BuildMathModel {
	class ParametricModelCreator {
		static int colorScheme;
		static int variantsConf;
		static int variantsConfB;

		static int assemblyHeight;
		static int assemblyHeightTTOR;
	public:
		static MbModel* CreatePneymocylinderModelTTRM(ConfigParams params, int configurationQuantity);
		static MbModel* CreatePneymocylinderModelZarubin(BuildParamsForHeatExchangerTTOR params, int configurationQuantity);

		static MbModel* CreatePneymocylinderModelFukina(ConfigParams_IP params);
		static MbModel* CreatePneymocylinderModelVasinkina(ConfigParams_IU params);

		static SPtr<MbSolid> ParametricModelCreator::CreateMassivOtv(SPtr<MbSolid> Osnova, double b,
			double d, double D1, double Y, int num, MbAxis3D axis, double grad, double X);

	private:
		static  SPtr<MbAssembly> CreatePneumocylinderAssembly(ConfigParams params, int configurationQuantity);
		static  SPtr<MbAssembly> CreateTTOR(BuildParamsForHeatExchangerTTOR params, int configurationQuantity);

		static  SPtr<MbAssembly> CreateIP(ConfigParams_IP params);
		static  SPtr<MbAssembly> CreateIU(ConfigParams_IU params);

	private:
		// details
		static SPtr<MbSolid> ParametricModelCreator::createInnerPipe_001(double ttDiam, double ttThickness, double length);
        static SPtr<MbSolid> ParametricModelCreator::createOuterPipe_002(int lengthK, double assortmentOuterTubes, double thicknessOuterTubes);
        static SPtr<MbSolid> ParametricModelCreator::createSupport_003(double assortmentCamera, double assortmentOuterTubes, double t);
        static SPtr<MbSolid> ParametricModelCreator::createOuterPipesGrid_004(double length2, double diametrY, double thickness, double t, double assortmentInnerTubes, double assortmentCamera, double thicknessCamera);
        static SPtr<MbSolid> ParametricModelCreator::createCup_005(double t, double assortmentOuterTubes, double assortmentCamera);
		static SPtr<MbSolid> ParametricModelCreator::createInnerPipesGrid_006(double length3, double assortmentCamera, double thicknessCamera);
		static SPtr<MbSolid> ParametricModelCreator::createCurvedPipe_007(double ttDiam, double ttThickness, double t);
		static SPtr<MbSolid> ParametricModelCreator::createPipeHolder_010(double holderLength, double assortmentInnerTubes);

		// TTRM GOST
		static SPtr<MbSolid> ParametricModelCreator::buildFlangeE(double diametrY);
		static SPtr<MbSolid> ParametricModelCreator::buildFlangeF(double diametrY);
		static SPtr<MbSolid> ParametricModelCreator::buildFlangeScrew35();
		static SPtr<MbSolid> ParametricModelCreator::buildWasher10();
		static SPtr<MbSolid> ParametricModelCreator::buildScrew55();

		// TTOR details
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_001_tubeTeploobmen(double Lk, double ttDiam, double ttThickness);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_002_tubeKozhux(double lK, double ktDiam, double ktThickness);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_003_opora(double dV, double ktDiam, double ktThickness, double t, double visotaOpori, double shirinaOpori);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_004_reshetkaKozhux(double ktDiam, double ktThickness, double t);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_005_kamera(double ktDiam, double ktThickness, double l3);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_006_RezhetkaTeplTube(double ttDiam, double ttThickness);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_007_FlanecKozhux(double ktDiam, double ktThickness);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_008_FlanecSpecial(double ttDiam, double ttThickness);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_009_curevedTube(double ttDiam, double ttThickness, double t);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_010_Connector(double ktDiam, double ktThickness, double t, double dU);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_011_ConnectorWithFlanec(double ktDiam, double ktThickness, double t, double visotaOpori, double dU);
		static SPtr<MbSolid> ParametricModelCreator::Zarubincreate_012_curevedTubeBig(double ttDiam, double ttThickness, double visotaOpori, double t);

		// GHOSTS TTOR
		static SPtr<MbSolid> ParametricModelCreator::BoltGostTTOR(double diam, bool simpleModeOn);
		static SPtr<MbSolid> ParametricModelCreator::NutBoltGostTTOR(double diam);
		static SPtr<MbSolid> ParametricModelCreator::WasherGostTTOR(double diam);

		//IP details
		static SPtr<MbSolid> ParametricModelCreator::CreateStoyka_IP(int Dvne, double L_Base);
		static SPtr<MbSolid> ParametricModelCreator::CreateKrKameraEll_IP(ConfigParams_IP params);
		static SPtr<MbSolid> ParametricModelCreator::CreateUnionKamera_IP(double L, int Dvne, double p, int DKr, double Dn, double Dy, double s);
		static SPtr<MbSolid> ParametricModelCreator::CreateUnionKzh_IP(int Dvne, double L_Base, double s, double p, int DKr, double Dn, double Dy);
		static SPtr<MbSolid> ParametricModelCreator::CreateOsnovaNePodResh_IP(int Dvne, double p, int DKr /*double D1, double S, double D4*/);
		static SPtr<MbSolid> ParametricModelCreator::CreateOsnovaPeregorodka_IP(int Dvne, double p, int DKr /*double S1, double D3*/);
		static SPtr<MbSolid> ParametricModelCreator::CreateOsnovaReshPod_IP(int Dvne, double p, int DKr);
		static SPtr<MbSolid> ParametricModelCreator::CreateRingOn_IP(int Dvne, double p, int DKr);
		static SPtr<MbSolid> ParametricModelCreator::CreateKrPlGol_IP(ConfigParams_IP params);
		static SPtr<MbSolid> ParametricModelCreator::CreatePipe_IP(int Dvne, double p, double DKr);
		static SPtr<MbSolid> ParametricModelCreator::CreateUnionCover_IP(int Dy, double p, int id, int Dvne, int DKr);

		//IU details
		static SPtr<MbSolid> ParametricModelCreator::CreateUnionKozhuh_IU(ConfigParams_IU params);
		static SPtr<MbSolid> ParametricModelCreator::CreateKamera(ConfigParams_IU params);
		static SPtr<MbSolid> ParametricModelCreator::CreateStoyka(int Dvne, double L_Base);
		static SPtr<MbSolid> ParametricModelCreator::CreateUnionCover(int Dy, double p, int id, int Dvne, int DKr);
		static SPtr<MbSolid> ParametricModelCreator::CreateOsnovaNePodResh(int Dvne, double p, int DKr);
		static SPtr<MbSolid> ParametricModelCreator::CreateOsnovaReshPod(int Dvne, double p, int DKr);
		static SPtr<MbSolid> ParametricModelCreator::CreateOsnovaPeregorodka(int Dvne, double p, int DKr);
		static SPtr<MbSolid> ParametricModelCreator::CreatePipe(double RAD_EXT, double RAD_INT, double p, int Dvne, int DKr);
		
	};

	};
