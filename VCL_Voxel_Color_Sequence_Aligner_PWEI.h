#pragma once
#include "VCL_Voxel_Color_Sequence_Aligner_Base.h"
#include "VCL_DTW.h"
#include "VCL_EI_DCT.h"

class VCL_Voxel_Color_Sequence_Aligner_PWEI :
	public VCL_Voxel_Color_Sequence_Aligner_Base, VCL_DTW
{
public:
	VCL_Voxel_Color_Sequence_Aligner_PWEI();
	~VCL_Voxel_Color_Sequence_Aligner_PWEI();

	void Align_Texture_on_Codes_Using_PWEI(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_offset);

	void Align_Texture_on_Codes_Using_PWEI(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		int in_number_of_control_points,
		CKvSet_of_VectorInt *out_offset);

	void Align_Texture_on_Codes_Using_PWEI2(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		int in_number_of_control_points,
		CKvSet_of_VectorInt *out_offset);

	void Align_Texture_on_Codes_Using_PWEI_Approximate(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_offset);

	void Align_Texture_on_Codes_Using_PWEI_Approximate2(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		int in_scanning_mode,
		CKvSet_of_VectorInt *out_offset);

protected:

	void Find_EI_Ranges(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_texture_on_code_N1,
		std::vector<int> &in_texture_on_code_N2,
		std::vector<std::vector<int>> &out_set_of_left_upper_xy,
		std::vector<std::vector<int>> &out_set_of_right_lower_xy,
		int &out_offset);

	void Find_EI_Ranges(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_texture_on_code_N1,
		std::vector<int> &in_texture_on_code_N2,
		int in_number_of_control_points,
		std::vector<std::vector<int>> &out_set_of_left_upper_xy,
		std::vector<std::vector<int>> &out_set_of_right_lower_xy,
		int &out_offset);

	void EI_Warping(
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int in_max_length,
		std::vector<std::vector<int>> in_set_of_left_upper_xy, 
		std::vector<std::vector<int>> in_set_of_right_lower_xy);

	void Update_EI_Warping_Index(
		std::vector<std::vector<int>> &io_warping_index,
		int in_current_ToC_index,
		std::vector<std::vector<int>> &in_warping_index);

	bool Update_EI_Warping(
		std::vector<int> &in_texture_on_code_N1,
		std::vector<int> &in_texture_on_code_N2,
		std::vector<std::vector<int>> &in_set_of_left_upper_xy,
		std::vector<std::vector<int>> &in_set_of_right_lower_xy,
		std::vector<std::vector<std::vector<int>>> &io_updated_EI_warping);

	bool Update_EI_Warping2(
		std::vector<int> &in_texture_on_code_N1,
		std::vector<int> &in_texture_on_code_N2,
		std::vector<std::vector<int>> &in_set_of_left_upper_xy,
		std::vector<std::vector<int>> &in_set_of_right_lower_xy,
		std::vector<std::vector<std::vector<int>>> &io_updated_EI_warping);

	bool Extract_Sub_Distance_Maps(
		std::vector<std::vector<bool>> &in_cornerstone_map,
		std::vector<std::vector<int>> &out_set_of_left_upper_xy,
		std::vector<std::vector<int>> &out_set_of_right_lower_xy);

	bool Find_End_Point_of_Sub_Block_Matrix(
		std::vector<std::vector<bool>> &in_cornerstone_map,
		std::vector<int> &in_left_upper_xy,
		std::vector<int> &out_right_lower_xy);

	bool linear_project_point(
		int in_index_x,
		std::vector<std::vector<int>> &in_set_of_left_upper_xy,
		std::vector<std::vector<int>> &in_set_of_right_lower_xy,
		std::vector<float> &out_y);

	bool linear_project_point(
		int in_x1, int in_x2,
		int in_y1, int in_y2,
		std::vector<float> &out_y);

	bool linear_project_point(
		int in_x1, int in_x2,
		int in_y1, int in_y2,
		std::vector<std::vector<float>> &out_table_xy);

	bool linear_project_point(
		int in_x1, int in_x2,
		int in_y1, int in_y2,
		std::vector<std::vector<float>> &out_table_xy,
		std::vector<std::vector<float>> &out_cumulative_table_xy);
	
	//////////////////////////////////////////////////////////////////
	void find_common_coordinate_range(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<std::vector<int>> &in_segmented_texture_on_code,
		int in_scanning_mode,
		int in_max_resolution,
		double *out_x_range_or_NULL,
		double *out_y_range_or_NULL,
		double *out_z_range_or_NULL,
		int &out_max_length_idx);

	void align_ToC(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &io_segmented_texture_on_code,
		CKvVoxel *in_center_voxel,
		int in_max_Size_of_ToC,
		int in_criteria_center_on_ToC,
		int in_center_val);

	//////////////////////////////////////////////////////////////////

protected:

	void Set_Connectivity_Map(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		std::vector<std::vector<bool>> &out_cornerstone_map);

	void Set_Connectivity_Map(
		CKvSet_of_Voxel *in_set_of_voxels,
		int in_number_of_control_points,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		std::vector<std::vector<bool>> &out_cornerstone_map);

	void Set_Connectivity_Map(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code);

	bool Update_Correspondances_Using_Slices(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<int>> &out_idx_on_distance_map_yx);

	bool Update_K_Correspondances_Using_Slices(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		int in_number_of_control_points,
		std::vector<std::vector<int>> &out_idx_on_distance_map_yx);

	bool Update_Correspondances_Using_Slices(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		int in_mode,
		std::vector<std::vector<int>> &out_idx_on_distance_map_yx);

	bool Update_Correspondances_Using_Slices(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<double>> &out_distance_map);

	bool Find_Optimal_Position(
		std::vector<std::vector<bool>> &in_cornerstone_map,
		int in_column_index,
		int in_row_index,
		std::vector<bool> *in_vector_on_column_index_or_NULL,
		int &out_xx_if_false,
		int &out_yy_if_false);

	bool Is_Reverse_Path( // 전제가 (column index - 1)와 row index 의 위치는 true. 
		std::vector<std::vector<bool>> &in_cornerstone_map,
		int in_column_index,
		int in_row_index,
		std::vector<bool> *in_vector_on_column_index_or_NULL,
		int &out_xx_if_false,
		int &out_yy_if_false);

	bool check_bool_vector(
		std::vector<bool> &in_vector,
		int in_criteria_index,
		short &out_z);

	bool save_data(
		CKvString in_str,
		std::vector<std::vector<int>> &in_data);

	bool save_data(
		std::vector<std::vector<int>> &in_data);

private:
	VCL_EI_DCT zz_vcl_ei_for_align;

};
