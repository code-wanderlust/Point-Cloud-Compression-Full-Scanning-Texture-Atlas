#pragma once
#include <vector>

class VCL_Voxel_Color_Sequence_Aligner_Base
{
public:
	VCL_Voxel_Color_Sequence_Aligner_Base();
	~VCL_Voxel_Color_Sequence_Aligner_Base();

	void input_random_number(
		int &in_random_number);

	void sv_Circular_Shift_Vector(
		int in_location,
		std::vector<int> &io_vector);

	void sv_Circular_Shift_Vector(
		int in_location,
		CKvVectorUcharYCbCr *io_vector);
	
	void sv_Circular_Shift_Vector(
		int in_location,
		CKvSet_of_RgbaF *io_vector);

	void Find_Mean(
		std::vector<int> &in_ToC,
		CKvSet_of_Voxel *in_set_of_voxel,
		CKvPoint3D *out_mean_point);

	void Find_Minimums(
		CKvVector *in_vector,
		CKvVectorInt *out_mininum_indices);

	void Find_Minimums(
		std::vector<double> &in_vector,
		std::vector<int> &out_mininum_indices);

	bool Find_Minimums(
		CKvVector *in_vector,
		double in_threshold_for_distance,
		CKvVectorInt *out_mininum_indices);

	bool Find_Candidate_of_Corresponds(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		int in_index_of_target_voxel,
		CKvVectorInt *out_mininum_indices);

	bool Find_Candidate_of_Corresponds_In_X_Slice(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		int in_index_of_target_voxel,
		CKvVectorInt *out_mininum_indices);
	bool Find_Candidate_of_Corresponds_In_X_Slice(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		int in_index_of_target_voxel,
		std::vector<int> &out_mininum_indices);
	bool Find_Candidate_of_Corresponds_In_X_Slice(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		int in_index_of_target_voxel,
		std::vector<bool> &io_reference_flag,
		std::vector<int> &out_mininum_indices);

	bool Find_Candidate_of_Corresponds_In_Z_Slice(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		int in_index_of_target_voxel,
		CKvVectorInt *out_mininum_indices);
	bool Find_Candidate_of_Corresponds_In_Z_Slice(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		int in_index_of_target_voxel,
		std::vector<int> &out_mininum_indices);

	bool Find_Candidate_of_Corresponds_In_Y_Slice(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &in_reference_texture_on_code,
		int in_index_of_target_voxel,
		std::vector<int> &out_mininum_indices);

	bool Rotate_Texture_on_Code_by_Centering(
		CKvSet_of_Voxel *in_set_of_voxels,
		std::vector<int> &io_texture_on_code,
		int in_plane_mode);

	bool Rotate_Texture_on_Code_by_Color(
		CKvSet_of_RgbaF *in_set_of_colors,
		std::vector<int> &io_texture_on_code,
		CKvSet_of_RgbaF *in_before_colors,
		int &out_circular_shift);

	bool Find_Corresponding_ToC(
		std::vector<std::vector<std::vector<int>>> &in_before_ToC,
		std::vector<int> &in_current_ToC,
		CKvSet_of_Voxel *in_before_voxels,
		CKvSet_of_RgbaF *in_before_colors,
		VCL_DoCube_X_Color *in_current_docube,
		CKvSet_of_RgbaF *out_ToC_in_before);

	void Rotate_Texture_on_Code_Randomly(
		std::vector<int> &io_texture_on_code);

	bool Save_File(
		std::vector<std::vector<double>> &in_data);


protected:
	std::vector<int> zz_global_circular_shift;
	int zz_rn_for_align;

};
