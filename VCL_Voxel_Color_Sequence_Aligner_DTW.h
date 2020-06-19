#pragma once
#include "VCL_Voxel_Color_Sequence_Aligner_Base.h"
#include "VCL_DTW.h"

class  VCL_Voxel_Color_Sequence_Aligner_DTW : 
public VCL_Voxel_Color_Sequence_Aligner_Base, VCL_DTW
{
public:
	VCL_Voxel_Color_Sequence_Aligner_DTW();
	~VCL_Voxel_Color_Sequence_Aligner_DTW();

	void Align_Texture_on_Codes_Using_DTW(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_offset); 

	void Align_Texture_on_Codes_Using_DTW2(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_offset);

	// Core Algorithms utilizing Dynamic Time Warping
	/**********************************************************************************/
protected:
	void Align_Two_Texture_on_Codes_Using_DTW(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<int>> &out_warping_index,
		int &io_max_length,
		int &out_offset); 

	/********************************************************/
	void Set_Distance_Map(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<double>> &out_distance_map); 

	void Set_Distance_Map_Fast(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<double>> &out_distance_map,
		std::vector<std::vector<bool>> &out_cornerstone_map); 
	/********************************************************/

	/********************************************************/
	bool Update_Distance_Map_Using_Correspondances_Using_Slices(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<double>> &out_distance_map); 

	bool Update_Distance_Map_Using_Correspondances_Using_Slices(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<int>> &out_idx_on_distance_map_yx); 

	bool Update_Distance_Map_Using_Euclidean_Distance(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &in_target_texture_on_code,
		std::vector<std::vector<double>> &out_distance_map); 
	/********************************************************/

	/********************************************************/
	bool Extract_Sub_Distance_Maps(
		std::vector<std::vector<double>> &in_distance_map,
		std::vector<std::vector<bool>> &in_cornerstone_map,
		std::vector<std::vector<std::vector<double>>> &out_sub_blocks,
		std::vector<std::vector<int>> &out_set_of_left_upper_xy); 
	bool Find_End_Point_of_Sub_Block_Matrix(
		std::vector<std::vector<bool>> &in_cornerstone_map,
		std::vector<int> &in_left_upper_xy,
		std::vector<int> &out_right_lower_xy);

	bool Find_Optimal_Position(
		std::vector<std::vector<bool>> &in_cornerstone_map,
		int in_column_index,
		int in_row_index,
		std::vector<bool> *in_vector_on_column_index_or_NULL,
		int &out_xx_if_false,
		int &out_yy_if_false);

	bool Is_Reverse_Path(
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
	/********************************************************/

	/********************************************************/
	void Circular_Shift_Distance_Map(
		std::vector<std::vector<double>> &in_distance_map,
		std::vector<std::vector<double>> &out_distance_map,
		int in_circular_shift_factor); 

	void Circular_Shift_Index_On_Distance_Map(
		std::vector<std::vector<int>> &in_distance_map_index,
		std::vector<std::vector<int>> &out_distance_map_index,
		int in_circular_shift_factor); 
	/********************************************************/

	/********************************************************/
	void Modify_Texture_on_Codes(
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		std::vector<int> &in_reference_warping_index,
		std::vector<int> &in_target_warping_index,
		std::vector<int> &in_modified_reference_warping_index,
		std::vector<int> &out_modified_target_warping_index);

	void Modify_Texture_on_Codes2(
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		std::vector<int> &in_reference_warping_index,
		std::vector<int> &in_target_warping_index,
		std::vector<int> &in_modified_reference_warping_index,
		std::vector<int> &out_modified_target_warping_index);

	void Warping_Texture_on_Codes(
		std::vector<std::vector<int>> &io_texture_on_codes,
		std::vector<std::vector<std::vector<int>>> &in_warping_index);

	void Warping_Texture_on_Codes2(
		std::vector<std::vector<int>> &io_texture_on_codes,
		std::vector<std::vector<std::vector<int>>> &in_warping_index);

	void Warping_Texture_on_Codes3(
		std::vector<std::vector<int>> &io_texture_on_codes,
		std::vector<std::vector<std::vector<int>>> &in_warping_index);
	
	void warping_texture_on_code_forward(
		std::vector<std::vector<int>> &in_texture_on_codes,
		std::vector<std::vector<std::vector<int>>> &io_modified_texture_on_codes,
		std::vector<std::vector<std::vector<int>>> &in_warping_index);
	void warping_texture_on_code_backward(
		std::vector<std::vector<int>> &in_texture_on_codes,
		std::vector<std::vector<std::vector<int>>> &io_modified_texture_on_codes,
		std::vector<std::vector<std::vector<int>>> &in_warping_index);


	template<typename T>
	bool find_element_in_vector(
		std::vector<T> &in_vector,
		T in_value,
		std::vector<int> &out_indices)
	{
		out_indices = std::vector<int>();
		int sz = in_vector.size();
		for (int k = 0; k < sz; k++)
		{
			if (in_value == in_vector[k])
			{
				out_indices.push_back(k);
			}
		}

		if (out_indices.size() != 0)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	bool find_element_in_2dvector(
		std::vector<std::vector<T>> &in_vectors,
		T in_value,
		int &out_index_row,
		int &out_index_column)
	{
		int ww;
		int hh = in_vectors.size();
		int j, i;
		
		for (j = 0; j < hh; j++)
		{
			ww = in_vectors[j].size();
			for (i = 0; i < ww; i++)
			{
				if (in_vectors[j][i] == in_value)
				{
					out_index_row = j;
					out_index_column = i;
					return true;
				}
			}
		}

		return false;
	}
	/********************************************************/

	/********************************************************/
	void plot_data_for_debug(
		CKvMatrixUcharRgb &in_distance_map,
		std::vector<std::vector<bool>> &in_sub_distance_map,
		std::vector<int> &in_left_upper_xy,
		std::vector<int> &in_right_lower_xy);
	void plot_data_for_debug(
		std::vector<std::vector<bool>> &in_sub_distance_map);
	void plot_data_for_debug(
		std::vector<std::vector<double>> &in_map);
	/********************************************************/

	/**********************************************************************************/

private:



	//CKvScreen zz_sc[3];


};
