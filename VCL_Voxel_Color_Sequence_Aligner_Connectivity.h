#pragma once
#include "VCL_Voxel_Color_Sequence_Aligner_Base.h"



class VCL_Voxel_Color_Sequence_Aligner_Connectivity : public VCL_Voxel_Color_Sequence_Aligner_Base
{
public:
	VCL_Voxel_Color_Sequence_Aligner_Connectivity();
	~VCL_Voxel_Color_Sequence_Aligner_Connectivity();

	void acc_Align_Texture_on_Codes_Using_Connectivity(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_offset);

	// Core Algorithms utilizing Connectivity
	/**********************************************************************************/
protected:
	void atc_Align_Two_Texture_on_Codes_Using_Connectivity(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int in_scanning_direction,
		int &out_offset);

	void atc_Align_Two_Texture_on_Codes_Using_Connectivity2(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int in_scanning_direction,
		int &out_offset);

	void atc_Align_Two_Texture_on_Codes_Using_Connectivity3(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int in_scanning_direction,
		int &out_offset);

	void atc_Align_Two_Texture_on_Codes_Using_Connectivity4(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		std::vector<int> &io_target_texture_on_code,
		int in_scanning_direction,
		int &out_offset);

	void Find_Texture_on_Codes_From_Arrange_Table(
		std::vector<std::vector<int>> &in_arrange_table,
		std::vector<int> &out_texture_on_code);

	bool Find_Texture_on_Codes_From_Arrange_Table2(
		std::vector<std::vector<int>> &in_arrange_table,
		std::vector<int> &in_target_texture_on_code,
		std::vector<bool> &in_flag_for_target_texture_on_code,
		std::vector<int> &out_texture_on_code);

	void eh_Estimate_Homography(
		CKvMatrixInt *in_corresponding_idx,
		bool true_forward_false_inverse,
		CKvMatrix *out_homography);


};
