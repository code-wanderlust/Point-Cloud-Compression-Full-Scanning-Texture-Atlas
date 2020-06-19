#ifndef VCL_VOXEL_SURFACE_COLOR_SCANNER
#define VCL_VOXEL_SURFACE_COLOR_SCANNER

#include "Voxel_Slice_Scanned_Data.h"
#include "JANG_Xrunset3dShort.h"

//************************************************************************
class VCL_Voxel_Scanner_Slice
//************************************************************************
{
public:
	VCL_Voxel_Scanner_Slice();
	~VCL_Voxel_Scanner_Slice();

	bool gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction(
		VCL_DoCube_X_Color *in_docube,
		CKvXrunset3dShort *io_xrunset3d,
		int in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence);

	bool gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction_Fast(
		VCL_DoCube_X_Color *in_docube,
		CKvXrunset3dShort *io_xrunset3d,
		int in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence);

	bool gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction_Fast2(
		VCL_DoCube_X_Color *in_docube,
		JANG_Xrunset3dShort *io_xrunset3d,
		int in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> *out_voxel_sequence);

	bool gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction_Fast3(
		VCL_DoCube_X_Color *in_docube,
		JANG_Xrunset3dShort *io_xrunset3d,
		int in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> *out_voxel_sequence);

	bool Get_Surface_Voxel_Indices(
		VCL_DoCube_X_Color *in_docube,
		CKvXrunset3dShort *in_xrunset3d,
		int in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> *out_voxel_sequence);

	void gcs_Get_Peeling_Voxel_Sequence(
		VCL_DoCube_X_Color *in_docube,
		int &in_plane_mode_0X_1Y_2Z_3zy,
		std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	void gsvpd_Get_Surface_Voxels_in_Plane_Direction(
		VCL_DoCube_X_Color *in_docube,
		int in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		CKvDepot_of_Point3Df *out_voxels,
		CKvDepot_of_RgbaF *out_colors);

	void gsvpd_Get_Surface_Voxels_in_Plane_Direction(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> &in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		CKvDepot_of_Point3Df *out_voxels,
		CKvDepot_of_RgbaF *out_colors);

	void gsvpd_Get_Rest_of_Surface_Voxels(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> &in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		CKvDepot_of_Point3Df *out_voxels,
		CKvDepot_of_RgbaF *out_colors);

public:

	bool Get_Connectivity_btw_Slices(
		CKvSdkimRunCode &in_current_runcode,
		CKvSdkimRunCode &in_previous_runcode,
		CKvSdkimRunCode &in_null_runcode,
		Voxel_Slice_Scanned_Data &out_voxel_sequence);

	bool Get_Surface_Voxel_Index(
		VCL_DoCube_X_Color *in_docube,
		CKvSdkimRunCode *in_runcode,
		int in_slice_index,
		int in_plane_mode,
		bool ***io_volume_pointer,
		Voxel_Slice_Scanned_Data &out_voxel_sequence);

	bool Get_Surface_Voxel_Index2(
		VCL_DoCube_X_Color *in_docube,
		CKvSdkimRunCode *in_runcode,
		int in_slice_index,
		int in_plane_mode,
		bool ***io_volume_pointer,
		Voxel_Slice_Scanned_Data &out_voxel_sequence);

	bool Get_Surface_Voxel_Index3(
		VCL_DoCube_X_Color *in_docube,
		CKvSdkimRunCode *in_runcode,
		int in_slice_index,
		int in_plane_mode,
		CKvMatrixBool *io_slice,
		Voxel_Slice_Scanned_Data &out_voxel_sequence);

	void gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_X(
		VCL_DoCube_X_Color *in_docube,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	void gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Y(
		VCL_DoCube_X_Color *in_docube,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	void gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Z(
		VCL_DoCube_X_Color *in_docube,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	void gcs_Get_Voxel_Idx_in_Plane(
		VCL_DoCube_X_Color *in_docube,
		int &in_plane_mode,
		int &in_ww,
		int &in_hh,
		int &in_dd,
		std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	void gfv_Get_Slice_Voxels(
		CKvXrunset3dShort *in_runsets,
		int in_plane_mode_0X_1Y_2Z_3zy,
		int in_coordinate_at_the_corresponding_axis,
		CKvXrunsetShort *out_fragment_voxel);

	void gfv_Get_Slice_Voxels(
		CKvXrunset3dShort *in_runsets,
		int in_plane_mode_0X_1Y_2Z_3zy,
		int in_coordinate_at_the_corresponding_axis,
		CKvSdkimRunCode *out_fragment_voxel);


	void Exclusive_OR_Parallel(
		CKvXrunset3dShort *in_runset1,
		CKvXrunset3dShort *in_runset2,
		CKvXrunset3dShort *out_runset);

	void Import_Fast(
		CKvVolumeBool *in_volume,
		CKvXrunset3dShort *out_xrunset);

	void Set_Xrunset1d(
		CKvVolumeBool *in_volume,
		int x,
		int y,
		CKvXrunset1dShort *out_xrunset);

	void tm_Test_Module(
		VCL_DoCube_X_Color *in_docube,
		int in_ww,
		int in_hh,
		int in_dd);

	void gps_Get_Pixel_String(
		CKvPixelShort *in_first_pixel,
		CKvXvectorUchar *in_chain_code,
		CKvXvectorShort *out_boundary_pixel);

	void fup_Find_Unique_Pixel(
		CKvXvectorShort *io_pixel);

};
//************************************************************************

#endif
