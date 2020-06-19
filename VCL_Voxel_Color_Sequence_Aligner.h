#pragma once
#include "Voxel_Slice_Scanned_Data.h"

#include "VCL_Voxel_Color_Sequence_Aligner_Distance.h"
#include "VCL_Voxel_Color_Sequence_Aligner_Color.h"
#include "VCL_Voxel_Color_Sequence_Aligner_Connectivity.h"
#include "VCL_Voxel_Color_Sequence_Aligner_DTW.h"
#include "VCL_Voxel_Color_Sequence_Aligner_PWEI.h"

//#include <vector>
//#include "VCL_DTW.h"



class  VCL_Voxel_Color_Sequence_Aligner : 
public VCL_Voxel_Color_Sequence_Aligner_Distance,
	   VCL_Voxel_Color_Sequence_Aligner_Color,
	   VCL_Voxel_Color_Sequence_Aligner_Connectivity,
	   VCL_Voxel_Color_Sequence_Aligner_DTW,
	   VCL_Voxel_Color_Sequence_Aligner_PWEI
{
public:
	VCL_Voxel_Color_Sequence_Aligner();
	~VCL_Voxel_Color_Sequence_Aligner();
	


protected:

/**********************************************************************************/
// Alignment Modules
/**********************************************************************************/

	// Main Module. 
	void atoc_Align_Texture_on_Code(
		VCL_DoCube_X_Color *in_docube,
		int in_alignment_mode,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offsets);

	void atoc_Align_Texture_on_Code_for_Video_Color(
		VCL_DoCube_X_Color *in_docube,
		int in_alignment_mode,
		CKvSet_of_Voxel *in_before_set_of_voxels,
		CKvSet_of_RgbaF *in_before_set_of_colors,
		std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offsets);

	void atoc_Align_Texture_on_Code(
		VCL_DoCube_X_Color *in_docube,
		int in_alignment_mode,
		int &in_random_number,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offsets);

	// Align Module W/O partitioning -> This module is only for experiment. 
	/**********************************************************************************/
	// Main Module. 
	void atoc_Align_Texture_on_Code_WO_partitioning(
		VCL_DoCube_X_Color *in_docube,
		int in_alignment_mode,
		std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offsets);

	void acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance_WO_Partitining(
		VCL_DoCube_X_Color *in_docube,
		std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
		std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	void acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference_WO_Partitining(
		VCL_DoCube_X_Color *in_docube,
		std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
		std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code,
		CKvSet_of_VectorInt *out_circular_shift,
		CKvSet_of_VectorInt *out_offset);

	bool fcm_Find_Connectivity_Matrix(
		Voxel_Slice_Scanned_Data &in_current_pcc,
		Voxel_Slice_Scanned_Data &in_before_pcc,
		CKvMatrixBool &out_connectivity_matrix); // # of before ToC - by - # of current ToC. 
	/**********************************************************************************/



};