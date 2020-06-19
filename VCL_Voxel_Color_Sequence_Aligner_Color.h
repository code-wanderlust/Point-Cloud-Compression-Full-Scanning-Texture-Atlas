#pragma once
#include "VCL_Voxel_Color_Sequence_Aligner_Base.h"


class VCL_Voxel_Color_Sequence_Aligner_Color :public VCL_Voxel_Color_Sequence_Aligner_Base
{
public:
	VCL_Voxel_Color_Sequence_Aligner_Color();
	~VCL_Voxel_Color_Sequence_Aligner_Color();



	void acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2_for_video(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
		CKvSet_of_Voxel *in_before_set_of_voxels,
		CKvSet_of_RgbaF *in_before_set_of_colors,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);


	void acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2(
		VCL_DoCube_X_Color *in_docube,
		int in_random_number,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference_YCbCr(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Color_Correlation(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_offset);

	void acc_Center_Align_Texture_on_Codes_Using_Voxel_Color_Difference(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);


	// Core Algorithms utilizing Color Difference
	/**********************************************************************************/
protected:
	int atd_Align_Two_Texture_on_Codes_Using_Color_Difference_WO_translation(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset); 

	int atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset); 

	int atd_Align_Two_Texture_on_Codes_Using_Color_Difference2(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);


	int atd_Align_Two_Texture_on_Codes_Using_Color_Difference_Centering(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	int atd_Align_Two_Texture_on_Codes_Using_Color_Difference_YCbCr(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset); 
	
	int atc_Align_Two_Texture_on_Codes_Using_Color_Correlation(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	int atc_Align_Two_Texture_on_Codes_Using_Color_Correlation_Fast(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_offset);

	void atc_Align_Two_Texture_on_Codes_Using_Color_Correlation(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_circular_shift,
		int &out_shift);

	void atc_Align_Two_Texture_on_Codes_Using_Color_Correlation_Fast(
		CKvSet_of_RgbaF *in_set_of_voxel_colors,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int &out_circular_shift,
		int &out_shift);

	  /**********************************************************************************/



	void ccs_Convert_Color_Space(
		CKvSet_of_RgbaF *in_color,
		std::vector<int> &in_chain_code,
		CKvVectorUcharYCbCr *out_color); ////

	void ccs_Convert_Color_Space(
		CKvSet_of_RgbaF *in_color,
		std::vector<int> &in_chain_code,
		int in_max_length,
		CKvVectorUcharYCbCr *out_color); ////

	int sacd_Summation_of_Absolute_Color_Differece(
		int in_dimension,
		unsigned char *in_vector_a,
		unsigned char *in_vector_b); ////

	void ncc_Normalized_Cross_Correlation(
		CKvVectorUcharYCbCr *in_reference_signal,
		CKvVectorUcharYCbCr *in_target_signal,
		CKvVector *out_correlation_result); ////

	void ncc_Normalized_Cross_Correlation_Fast(
		CKvVectorUcharYCbCr *in_reference_signal,
		CKvVectorUcharYCbCr *in_target_signal,
		CKvVector *out_correlation_result); ////

	double ncc_Normalized_Cross_Correlation(
		int in_dimension,
		int in_target_size,
		int in_k,
		unsigned char *in_y_reference,
		unsigned char *in_cb_reference,
		unsigned char *in_cr_reference,
		unsigned char *in_y_target,
		unsigned char *in_cb_target,
		unsigned char *in_cr_target); ////


	


};
