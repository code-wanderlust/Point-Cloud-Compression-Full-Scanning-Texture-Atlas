#pragma once

#include "VCL_Voxel_Segmentation.h"
#include "VCL_Voxel_Color_Sequence_Aligner.h"
#include "Voxel_Slice_Scanned_Data.h"
#include "VCL_Image_Interpolation.h"

class VCL_Voxel_Texture_Atlas_3_Direction : public VCL_Voxel_Segmentation,
											public VCL_Voxel_Color_Sequence_Aligner
{
public:
	VCL_Voxel_Texture_Atlas_3_Direction();
	~VCL_Voxel_Texture_Atlas_3_Direction();

	void Set_Parameters(
		bool in_shape_coding_mode_on_off,
		int in_plane_mode_sequence,
		int in_alignment_mode,
		int in_block_size,
		int in_threshold_for_cluster_size,
		float in_threshold_for_segmentation,
		int in_threshold_cluster,
		int in_map_width,
		int in_map_height);

	void generate_Voxel_Texture_Atlas_3_Direction(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		CKvSet_of_MatrixUcharRgb *out_voxel_maps,
		CKvSet_of_MatrixBool *out_voxel_map_masks,
		CKvSet_of_MatrixInt *out_voxel_idx_maps);

	void generate_Voxel_Texture_Atlas_3_Direction2(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		CKvSet_of_MatrixUcharRgb *out_voxel_maps,
		CKvSet_of_MatrixBool *out_voxel_map_masks,
		CKvSet_of_MatrixInt *out_voxel_idx_maps);

	void generate_Voxel_Texture_Atlas_3_Direction3(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		CKvSet_of_MatrixUcharRgb *out_voxel_maps,
		CKvSet_of_MatrixBool *out_voxel_map_masks,
		CKvSet_of_MatrixInt *out_voxel_idx_maps,
		CKvMatrixInt *out_pt_list_for_secong_img_N_by_3);

	void generate_Voxel_Texture_Atlas_3_Direction3_Fast(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		CKvSet_of_MatrixUcharRgb *out_voxel_maps,
		CKvSet_of_MatrixBool *out_voxel_map_masks,
		CKvSet_of_MatrixInt *out_voxel_idx_maps,
		CKvMatrixInt *out_pt_list_for_secong_img_N_by_3);


	bool sim_Save_Index_Map(
		CKvString str,
		CKvMatrixInt *in_index_map);

protected:

	void Partitioning_Voxels_And_Generate_Voxel_Maps(
		VCL_DoCube_X_Color *in_docube,
		int in_plane_mode_sequence,
		int in_alignment_mode,
		CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask);

	void Extract_Voxel_Patch_Maps(
		CKvSet_of_MatrixInt *in_set_of_voxel_maps,
		CKvSet_of_MatrixBool *in_set_of_voxel_map_masks,
		std::vector<bool> &in_voxel_flag,
		int in_closing_size,
		CKvDepot_of_MatrixInt *out_set_of_large_index_maps,
		CKvDepot_of_MatrixBool *out_set_of_large_masks,
		CKvDepot_of_MatrixInt *out_set_of_small_index_maps,
		CKvDepot_of_MatrixBool *out_set_of_small_masks);

	void Get_Region_Index_and_Mask(
		CKvSdkimRunCode *in_mask,
		CKvMatrixInt *in_index_map,
		CKvDepot_of_MatrixInt *out_depot_of_large_index_maps,
		CKvDepot_of_MatrixBool *out_depot_of_large_masks,
		CKvDepot_of_MatrixInt *out_depot_of_small_index_maps,
		CKvDepot_of_MatrixBool *out_depot_of_small_masks);

	void Get_Region_Index_and_Mask(
		CKvXrunsetShort *in_mask,
		CKvMatrixInt *in_index_map,
		int in_closing_size,
		CKvDepot_of_MatrixInt *out_depot_of_large_index_maps,
		CKvDepot_of_MatrixBool *out_depot_of_large_masks,
		CKvDepot_of_MatrixInt *out_depot_of_small_index_maps,
		CKvDepot_of_MatrixBool *out_depot_of_small_masks);

	void gpvm_Generate_Packing_Voxel_Map_for_Video(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_Point3Df *in_points,
		CKvDepot_of_RgbaF *in_colors_or_NULL,
		int in_block_size,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx_or_NULL,
		CKvMatrixBool *out_mask_or_NULL,
		CKvMatrixBool *out_Max_mask_or_NULL);

	void Modify_Texture_Atlases(
		CKvDepot_of_RgbaF *in_colors,
		CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
		CKvSet_of_MatrixInt *in_set_of_voxel_map_idx);

	void Modify_Texture_Atlases(
		CKvDepot_of_RgbaF *in_colors,
		CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
		CKvSet_of_MatrixBool *io_set_of_voxel_map_masks,
		CKvSet_of_MatrixInt *in_set_of_voxel_map_idx);

	void Generate_Error_Maps(
		CKvDepot_of_Point3Df *in_points,
		CKvDepot_of_RgbaF *in_colors,
		CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
		CKvSet_of_MatrixBool *io_set_of_voxel_map_masks,
		CKvSet_of_MatrixInt *in_set_of_voxel_map_idx,
		int in_filter_size);

protected:

	void sp_Set_Parameter_Map_Size(
		int in_map_width,
		int in_map_height);

	bool gvsi_Get_Voxel_Sequence(
		VCL_DoCube_X_Color *in_docube,
		int &in_plane_mode_sequence,
		std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence);

	bool gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction_and_Closing(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence);

	void gvmv_Generate_Voxel_Maps(
		VCL_DoCube_X_Color *in_docube,
		std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
		int in_alignment_mode,
		CKvSet_of_MatrixInt *out_set_of_voxel_map,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask);

	bool s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		CKvSet_of_VectorInt *in_offsets,
		CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
		CKvDepot_of_MatrixBool *out_set_masks);

	void gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_RgbaF *in_colors_or_NULL,
		int in_block_size,
		bool in_line_ei_mode_on_true_off_false,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx_or_NULL,
		CKvMatrixBool *out_mask_or_NULL);

	void pvm_Pack_Voxel_Map_With_Scanning_Axis(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_Point3Df *in_points,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask,
		CKvMatrixBool *out_Max_mask);

	void pvm_Pack_Voxel_Map_With_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		bool in_fixed_image,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	bool checkFitPatchCanvas(
		CKvMatrixBool *in_mask,
		CKvMatrixBool *in_patch_mask,
		int canvasStrideBlk,
		int canvasHeightBlk,
		int safeguard);

	int patchBlock2CanvasBlock(
		int in_uBlk,
		int in_vBlk,
		int in_mask_width,
		int in_mask_height);

	void Resize_Voxel_Blobs(
		CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
		CKvSet_of_MatrixBool *io_set_voxel_masks,
		int in_block_size);

	void Set_New_Masks(
		CKvMatrixBool *in_mask,
		CKvMatrixBool *out_mask);

	void Generate_Prediction_Image(
		VCL_DoCube_X_Color *in_docube,
		CKvMatrixUcharRgb *in_voxel_map,
		CKvMatrixBool *in_voxel_mask,
		CKvMatrixInt  *in_voxel_idx_map,
		CKvMatrixInt *in_position_of_blob,
		CKvSet_of_MatrixInt *in_set_of_prediction_map,
		CKvSet_of_MatrixBool *in_set_of_prediction_mask,
		CKvMatrixUcharRgb *out_prediction_voxel_map,
		CKvMatrixInt *out_prediction_list_and_position_N_by_3);

	void Find_Closet_Point3(
		CKvRgbaF *in_color,
		CKvPoint3Df *in_point,
		CKvPoint3Df *in_all_points,
		CKvSet2d_of_XvectorShort *in_xvector_2d,
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<int>> &in_voxel_idx_table,
		std::vector<bool> &io_voxel_idx_flag,
		CKvMatrixUcharRgb *io_prediction_map,
		int &out_x,
		int &out_y);

	int zz_plane_mode_sequence;
	int zz_alignment_mode;
	int zz_block_size;
	int zz_width, zz_height;
	int zz_x, zz_y; // For Packing Algorithm
	int zz_safeguard;
	bool zz_classic_mode;

private:


	VCL_Image_Interpolation zz_vcl_ii;
	int zz_threshold_cluster;

	template<typename T>
	inline T limit(const T& value)
	{
		return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
	}

};
