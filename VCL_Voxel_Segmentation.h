#pragma once
#include <vector>
#include "Voxel_Slice_Scanned_Data.h"
#include "VCL_Voxel_Scanner_Slice.h"

#define INVALID_CODE -1
#define START_CODE 0
#define APPEND_CODE 1

class VCL_Voxel_Segmentation : public VCL_Voxel_Scanner_Slice
{
public:
	VCL_Voxel_Segmentation();
	~VCL_Voxel_Segmentation();

	void sp_Set_Parameter_for_Segmentation(
		float in_threshold); 

	void svd_Segmentation_of_Voxel_Data(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);

	void svd_Segmentation_of_Voxel_Data_along_a_Slice(
		VCL_DoCube_X_Color *in_docube,
		int in_slice_mode_0_X_1_Y_2_Z,
		CKvSet_of_Xrunset3dShort *out_voxel_segments);

	void svd_Segmentation_of_Voxel_Data_along_a_Slice(
		VCL_DoCube_X_Color *in_docube,
		int in_slice_mode_0_X_1_Y_2_Z,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);

	void svd_Segmentation_of_Voxel_Data_along_a_Slice(
		VCL_DoCube_X_Color *in_docube,
		int in_slice_mode_0_X_1_Y_2_Z,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments,
		std::vector<std::vector<std::vector<float>>> &out_voxel_colors);


	void psr_Plot_Segmentation_Result(
		CKvSet_of_Xrunset3dShort *in_voxel_segments);

	void psr_Plot_Segmentation_Result(
		std::vector<std::vector<std::vector<float>>> &in_voxel_segment);

	void psr_Plot_Segmentation_Result(
		std::vector<std::vector<std::vector<float>>> &in_voxel_segment,
		std::vector<std::vector<std::vector<float>>> &in_voxel_color_segment);

protected:


	/*******************************************************************************/
	void stoc_Segment_Voxel_Slice_Scanned_Data(
		std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code);

	void atoc_Analyze_Voxel_Sequences(
		Voxel_Slice_Scanned_Data &in_current_texture_on_code,
		Voxel_Slice_Scanned_Data &in_before_texture_on_code,
		int &in_current_maximum_label,
		std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code);

	void acm_Analyze_Connectivity_Matrix(
		Voxel_Slice_Scanned_Data &in_current_texture_on_code,
		Voxel_Slice_Scanned_Data &in_before_texture_on_code,
		int &in_current_maximum_label,
		CKvMatrixBool *in_connectivity_matrix,
		CKvVectorChar *out_state_code_of_current_texture_on_code);

	bool fcm_Find_Connectivity_Matrix(
		Voxel_Slice_Scanned_Data &in_current_pcc,
		Voxel_Slice_Scanned_Data &in_before_pcc,
		CKvMatrixBool &out_connectivity_matrix);

	bool fcm_Find_Connectivity_Matrix2(
		Voxel_Slice_Scanned_Data &in_current_pcc,
		Voxel_Slice_Scanned_Data &in_before_pcc,
		CKvMatrixBool &out_connectivity_matrix);
	/*******************************************************************************/
	
	void avs_Analyze_Voxel_Segments(
		std::vector<std::vector<std::vector<int>>> &in_voxel_segments_X,
		std::vector<std::vector<std::vector<int>>> &in_voxel_segments_Y,
		std::vector<std::vector<std::vector<int>>> &in_voxel_segments_Z,
		std::vector<std::vector<int>> &out_voxel_segments);

	void avs_Analyze_Voxel_Segments(
		std::vector<std::vector<std::vector<float>>> &in_voxel_segments_X,
		std::vector<std::vector<std::vector<float>>> &in_voxel_segments_Y,
		std::vector<std::vector<std::vector<float>>> &in_voxel_segments_Z,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);

	/*******************************************************************************/
	void svd_Segment_Voxel_Data_in_X_Slice(
		VCL_DoCube_X_Color *in_docube,
		CKvSet_of_Xrunset3dShort *out_voxel_segments);

	void svd_Segment_Voxel_Data_in_Y_Slice(
		VCL_DoCube_X_Color *in_docube,
		CKvSet_of_Xrunset3dShort *out_voxel_segments);

	void svd_Segment_Voxel_Data_in_Z_Slice(
		VCL_DoCube_X_Color *in_docube,
		CKvSet_of_Xrunset3dShort *out_voxel_segments);
	/*******************************************************************************/

	/*******************************************************************************/
	void svd_Segment_Voxel_Data_in_X_Slice(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);

	void svd_Segment_Voxel_Data_in_Y_Slice(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);

	void svd_Segment_Voxel_Data_in_Z_Slice(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);
	/*******************************************************************************/

	/*******************************************************************************/
	void gv_Get_Voxels_From_Segment(
		CKvDepot_of_Point3Df *in_voxels,
		std::vector<std::vector<std::vector<int>>> &in_segments,
		int in_ww,
		int in_hh,
		int in_dd,
		CKvSet_of_Xrunset3dShort *out_voxel_segments);

	void gv_Get_Voxels_From_Segment(
		CKvDepot_of_Point3Df *in_voxels,
		std::vector<std::vector<std::vector<int>>> &in_segments,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);

	void gv_Get_Voxel_Colors_From_Segment(
		CKvDepot_of_RgbaF *in_voxels,
		std::vector<std::vector<std::vector<int>>> &in_segments,
		std::vector<std::vector<std::vector<float>>> &out_voxel_colors);

	void gv_Get_Voxels_From_Table(
		CKvDepot_of_Point3Df *in_voxels,
		std::vector<std::vector<int>> &in_table,
		std::vector<std::vector<std::vector<float>>> &out_voxel_segments);

	void gv_Get_Voxels_From_Segment(
		CKvDepot_of_Point3Df *in_voxels,
		std::vector<std::vector<int>> &in_voxel_sequence,
		CKvSet_of_Point3Df *out_voxels);

	void gv_Get_Voxels_From_Segment(
		CKvDepot_of_Point3Df *in_voxels,
		std::vector<std::vector<int>> &in_voxel_sequence,
		std::vector<std::vector<float>> &out_voxels);

	void gv_Get_Voxel_Colors_From_Segment(
		CKvDepot_of_RgbaF *in_voxel_colors,
		std::vector<std::vector<int>> &in_voxel_sequence,
		std::vector<std::vector<float>> &out_voxel_colors);

	void gv_Get_Voxels_From_Segment(
		CKvDepot_of_Point3Df *in_voxels,
		std::vector<std::vector<int>> &in_voxel_sequence,
		int in_ww,
		int in_hh,
		int in_dd,
		CKvVolumeBool *out_volume);

	void gv_Get_Voxels_From_Segment(
		CKvDepot_of_Point3Df *in_voxels,
		std::vector<std::vector<int>> &in_voxel_sequence,
		int in_ww,
		int in_hh,
		int in_dd,
		CKvXrunset3dShort *out_xrunset3d);
	/*******************************************************************************/

protected:
	float zz_threshold; // The threshold for segmentation. 
};


