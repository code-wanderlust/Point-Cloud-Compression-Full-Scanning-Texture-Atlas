#pragma once
#include "VCL_Voxel_Color_Sequence_Aligner_Base.h"

class VCL_Voxel_Color_Sequence_Aligner_Distance : public VCL_Voxel_Color_Sequence_Aligner_Base
{
public:
	VCL_Voxel_Color_Sequence_Aligner_Distance();
	~VCL_Voxel_Color_Sequence_Aligner_Distance();

	void acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2_for_video(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
		CKvSet_of_Voxel *in_before_set_of_voxels,
		CKvSet_of_RgbaF *in_before_set_of_colors,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2(
		VCL_DoCube_X_Color *in_docube,
		int in_random_number,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance3(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance_Piece_Wise(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		int in_number_of_element_for_mask,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Center_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);



	// Core Algorithms utilizing Euclidean Distance
	/**********************************************************************************/
protected:
	void atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_WO_translation(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	int atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	int atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance2(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	int atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Double_Circular_Shift(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	int atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Centering(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	int atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Piece_Wise(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int in_number_of_element_for_mask,
		int &out_offset);

	/**********************************************************************************/

	bool compute_cost_over_ToC(
		CKvSet_of_Voxel *in_set_of_voxels,
		int in_buffer_size,
		std::vector<int> &io_texture_on_code,
		std::vector<int> &in_sub_patch,
		CKvVectorBool *io_texture_on_code_mask,
		std::vector<int> &out_matched_patch,
		std::vector<int> &out_location_of_elements_in_matched_path);

	bool compute_cost(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_texture_on_code,
		std::vector<int> &in_sub_patch,
		std::vector<int> &out_matched_patch,
		std::vector<int> &out_location_of_elements_in_matched_patch,
		float &out_cost);

	float find_min_cost(
		std::vector<float> &in_cost_vector,
		int &out_idx);


};
