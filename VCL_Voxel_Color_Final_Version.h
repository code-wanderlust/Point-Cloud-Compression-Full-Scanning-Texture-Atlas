#pragma once


#include "VCL_Voxel_Color_Map.h"
#include "VCL_Voxel_Segmentation.h"
#include "VCL_Voxel_Color_Sequence_Aligner.h"
#include "Voxel_Slice_Scanned_Data.h"
#include <vector>


class VCL_Voxel_Color_Final_Version : public VCL_Voxel_Color_Map,
									  public VCL_Voxel_Segmentation,
									  public VCL_Voxel_Color_Sequence_Aligner
{
public:
	VCL_Voxel_Color_Final_Version();
	~VCL_Voxel_Color_Final_Version();

	void Set_Parameters(
		bool in_shape_coding_mode_on_off,
		int in_plane_mode_sequence,
		int in_alignment_mode,
		int in_block_size,
		int in_threshold_for_cluster_size,
		float in_threshold_for_segmentation,
		int in_map_width,
		int in_map_height);

	bool Save_Parameters(
		CKvString in_filename,
		bool in_shape_coding_mode_on_off,
		std::vector<int> in_direction_of_scan,
		int in_alignment_mode,
		int in_block_size,
		int in_threshold_for_cluster_size,
		float in_threshold_for_segmentation,
		int in_searching_range,
		int in_error_prediction_mode,
		int in_error_threshold,
		int in_encoder_type);

	bool Load_Parameters(
		CKvString in_filename,
		bool &in_shape_coding_mode_on_off,
		std::vector<int> &in_direction_of_scan,
		int &in_alignment_mode,
		int &in_block_size,
		int &in_threshold_for_cluster_size,
		float &in_threshold_for_segmentation,
		int &in_searching_range,
		int &in_error_prediction_mode,
		int &in_error_threshold,
		int &in_encoder_type);

	/***********************************************/
	void Generate_Voxel_Map(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		bool in_shape_coding_mode_on_off,
		int in_alignment_mode,
		int in_block_size,
		int in_encoder,
		CKvMatrixUcharRgb *out_voxel_maps,
		CKvMatrixBool *out_voxel_map_mask,
		CKvMatrixInt *out_voxel_idx_map);

	void Generate_Voxel_Map_with_Multiple_Images(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		bool in_shape_coding_mode_on_off,
		int in_alignment_mode,
		int in_block_size,
		int in_encoder,
		CKvSet_of_MatrixUcharRgb *out_voxel_maps,
		CKvMatrixBool *out_voxel_map_mask,
		CKvMatrixInt *out_voxel_idx_map,
		CKvMatrixInt *out_list_for_position_N_by_3_second_image);

	void Generate_Voxel_Map_with_Error_Prediction(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		bool in_shape_coding_mode_on_off,
		int in_alignment_mode,
		int in_block_size,
		int in_encoder,
		int in_prediction_searching_range,
		int in_prediction_mode,
		int in_error_threshold,
		CKvMatrixUcharRgb *out_voxel_maps,
		CKvMatrixBool *out_voxel_map_mask,
		CKvMatrixInt *out_voxel_idx_map,
		CKvMatrixUcharRgb *out_error_maps,
		CKvMatrixBool *out_error_map_mask,
		CKvMatrixInt *out_error_idx_map);

	void Generate_Voxel_Map_with_Hybrid_Coding(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		bool in_shape_coding_mode_on_off,
		int in_alignment_mode,
		int in_block_size,
		int in_encoder,
		CKvMatrixUcharRgb *out_voxel_maps,
		CKvMatrixBool *out_voxel_map_mask,
		CKvMatrixInt *out_voxel_idx_map,
		CKvDepot_of_Point3Df *out_voxel_dust_points,
		CKvDepot_of_RgbaF *out_voxel_dust_colors);

	/***********************************************/
	
	void Import_Voxel_Map(
		CKvMatrixUcharRgb *in_voxel_map,
		CKvMatrixBool *in_voxel_map_mask,
		CKvMatrixInt *in_voxel_map_index,
		VCL_DoCube_X_Color *io_docube);

	void Import_Voxel_Map_with_Multple_Maps(
		CKvMatrixUcharRgb *in_voxel_map_1,
		CKvMatrixUcharRgb *in_voxel_map_2,
		CKvMatrixBool *in_voxel_map_mask,
		CKvMatrixInt *in_voxel_map_index,
		CKvMatrixInt *in_list_for_position,
		VCL_DoCube_X_Color *io_docube);


	void Import_Voxel_Map_With_Error_Prediction(
		CKvMatrixUcharRgb *in_voxel_map,
		CKvMatrixBool *in_voxel_map_mask,
		CKvMatrixInt *in_voxel_map_index,
		CKvMatrixUcharRgb *in_error__map,
		CKvMatrixBool *in_error_map_mask,
		CKvMatrixInt *in_error_map_index,
		int in_prediction_searching_range,
		int in_prediction_mode,
		VCL_DoCube_X_Color *io_docube);


	/***********************************************/
	bool sim_Save_Index_Map(
		CKvString str,
		CKvMatrixInt *in_index_map);
	bool sim_Save_Index_Map(
		CKvString str,
		CKvVectorInt *in_index_map);
	bool sap_Save_Align_Parameters(
		CKvString str,
		std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		int in_blob_size_threshold,
		CKvSet_of_VectorInt *in_circular_shift,
		CKvSet_of_VectorInt *in_off);
	bool sv_Save_Vector(
		CKvString str,
		std::vector<int> &in_vector);



	bool lim_Load_Index_Map(
		CKvString str,
		CKvMatrixInt *in_index_map);
	bool lim_Load_Index_Map(
		CKvString str,
		CKvVectorInt *in_index_map);
	bool Save_Vector_Txt(
		CKvString str,
		CKvVector *in_vector);
	/***********************************************/


	bool Get_PSNR(
		CKvDepot_of_RgbaF *in_depot_color1,
		CKvDepot_of_RgbaF *in_depot_color2,
		bool in_mode_true_yuv_or_false_rgb,
		double &out_channel_Y_or_R,
		double &out_channel_U_or_G,
		double &out_channel_V_or_B);
	bool Get_PSNR(
		CKvDepot_of_RgbaF *in_depot_color1,
		CKvDepot_of_RgbaF *in_depot_color2,
		bool in_mode_true_yuv_or_false_rgb,
		CKvMatrixInt *in_texture_index,
		CKvMatrixInt *in_prediction_index,
		std::vector<double> &out_texture_map_psnr,
		std::vector<double> &out_prediction_map_psnr);

	int Get_File_Size_in_Byte(CKvString in_filename);
	int Get_Number_of_Voxels(CKvMatrixBool *in_mask);

protected:

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

	void Generate_Prediction_Image2(
		VCL_DoCube_X_Color *in_docube,
		CKvMatrixUcharRgb *in_voxel_map,
		CKvMatrixBool *in_voxel_mask,
		CKvMatrixInt  *in_voxel_idx_map,
		CKvMatrixInt *in_position_of_blob,
		CKvSet_of_MatrixInt *in_set_of_prediction_map,
		CKvSet_of_MatrixBool *in_set_of_prediction_mask,
		CKvMatrixUcharRgb *out_prediction_voxel_map,
		CKvMatrixInt *out_prediction_list_and_position_N_by_3);

	void Find_Closet_Point(
		CKvRgbaF *in_color,
		CKvPoint3Df *in_point,
		CKvPoint3Df *in_all_points,
		CKvMatrixUcharRgb *io_prediction_map,
		CKvMatrixInt *in_voxel_index_map,
		CKvMatrixBool *in_prediction_map_mask,
		CKvMatrixBool *io_flag_map,
		int &out_x,
		int &out_y);

	void Find_Closet_Point_Fast(
		CKvRgbaF *in_color,
		CKvPoint3Df *in_point,
		CKvPoint3Df *in_all_points,
		CKvMatrixUcharRgb *io_prediction_map,
		CKvMatrixInt *in_voxel_index_map,
		CKvMatrixBool *in_prediction_map_mask,
		CKvMatrixBool *io_flag_map,
		int &out_x,
		int &out_y);

	void Find_Closet_Point(
		CKvRgbaF *in_color,
		CKvPoint3Df *in_point,
		CKvPoint3Df *in_all_points,
		CKvMatrixInt *in_position_blob,
		CKvMatrixUcharRgb *io_prediction_map,
		CKvMatrixInt *in_voxel_index_map,
		CKvMatrixBool *in_prediction_map_mask,
		CKvMatrixBool *io_flag_map,
		int &out_x,
		int &out_y);

	void Find_Closet_Point_Fast(
		CKvRgbaF *in_color,
		CKvPoint3Df *in_point,
		CKvPoint3Df *in_all_points,
		CKvMatrixInt *in_position_blob,
		CKvMatrixUcharRgb *io_prediction_map,
		CKvMatrixInt *in_voxel_index_map,
		CKvMatrixBool *in_prediction_map_mask,
		CKvMatrixBool *io_flag_map,
		int &out_x,
		int &out_y);

	void Find_Closet_Point(
		CKvRgbaF *in_color,
		CKvMatrixUcharRgb *io_prediction_map,
		CKvMatrixBool *in_prediction_map_mask,
		CKvMatrixBool *io_flag_map,
		int &out_x,
		int &out_y);

	void Find_Closet_Point2(
		CKvRgbaF *in_color,
		CKvPoint3Df *in_point,
		CKvPoint3Df *in_all_points,
		std::vector<std::vector<int>> &in_voxel_idx_table,
		std::vector<bool> &io_voxel_idx_flag,
		CKvMatrixUcharRgb *io_prediction_map,
		int &out_x,
		int &out_y);

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

	void Partitioning_Voxels_And_Generate_Voxel_Maps(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		int in_alignment_mode,
		int in_threshold_for_cluster_size,
		CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask,
		CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
		CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask);
	
	void Partitioning_Voxels_And_Generate_Voxel_Maps(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		int in_alignment_mode,
		int in_threshold_for_cluster_size,
		int in_random_number,
		CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask,
		CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
		CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask);


	bool gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence);

	bool gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction_and_Closing(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence);

	void gvmv_Generate_Voxel_Maps(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
		int in_alignment_mode,
		int in_threshold_for_cluster_size,
		CKvSet_of_MatrixInt *out_set_of_voxel_map,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask,
		CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction);

	void gvmv_Generate_Voxel_Maps_Video_Temporal_Alignment(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
		int in_alignment_mode,
		int in_threshold_for_cluster_size,
		CKvSet_of_MatrixInt *out_set_of_voxel_map,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask,
		CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction);



	void gvmv_Generate_Voxel_Maps(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
		int in_alignment_mode,
		int in_threshold_for_cluster_size,
		int in_random_number,
		CKvSet_of_MatrixInt *out_set_of_voxel_map,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask,
		CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction);


	void Reordering_Voxel_Map_By_Map_Size(
		CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
		CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask);

	/****************************************************************************/
	void Classify_Voxe_Map_and_Generate_Error_Map(
		VCL_DoCube_X_Color *in_docube,
		CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
		CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask,
		CKvSet_of_MatrixInt *io_error_map_idx,
		CKvSet_of_MatrixBool *io_error_map_mask,
		int in_searching_range,
		int in_prediction_mode,
		int in_error_threshold,
		CKvSet_of_MatrixIntRgb *out_error_map);
	bool gpevm_Generate_Prediction_Error_Voxel_Map(
		VCL_DoCube_X_Color *in_docube,
		CKvSet_of_MatrixInt *io_set_of_voxel_idx_for_prediction,
		CKvSet_of_MatrixBool *io_set_of_voxel_mask_for_prediction,
		int in_searching_range,
		int in_prediction_mode,
		int in_error_threshold,
		CKvSet_of_MatrixIntRgb *out_error_prediction_voxel_map,
		CKvSet_of_MatrixInt *out_texture_map_for_error_prediction_fail,
		CKvSet_of_MatrixBool *out_texture_mask_for_error_prediction_fail,
		CKvVectorInt *out_idx_for_prediction_target);
	bool gpevm_Generate_Prediction_Error_Voxel_Map_With_Average(
		VCL_DoCube_X_Color *in_docube,
		CKvPoint3Df *in_set_pointer_point3df,
		CKvRgbaF    *in_set_pointer_colors,
		CKvVectorBool *in_prediction_flag,
		CKvMatrixInt *in_voxel_idx_map_for_prediction,
		CKvMatrixBool *in_voxel_mask,
		int in_searching_range,
		int in_error_threshold,
		CKvMatrixIntRgb *out_error_map,
		CKvMatrixInt *out_texture_map_for_prediction_fail,
		float *out_error_or_NULL);
	bool gpevm_Generate_Prediction_Error_Voxel_Map_With_Distance_Weight(
		VCL_DoCube_X_Color *in_docube,
		CKvPoint3Df   *in_set_pointer_point3df,
		CKvRgbaF      *in_set_pointer_colors,
		CKvVectorBool *in_prediction_flag,
		CKvMatrixInt *in_voxel_idx_map_for_prediction,
		CKvMatrixBool *in_voxel_mask,
		int in_searching_range,
		int in_error_threshold,
		CKvMatrixIntRgb *out_error_map,
		CKvMatrixInt *out_texture_map_for_prediction_fail,
		float *out_error_or_NULL);
	bool Get_index_and_Color_in_Block(
		int in_xx, int in_yy, int in_zz,
		int in_searching_range,
		VCL_DoCube_X_Color *in_docube,
		CKvRgbaF *in_set_pointer_colors,
		CKvVectorBool *in_prediction_flag,
		std::vector<int> &out_voxel_index_Memory_Already_Set,
		std::vector<std::vector<float>> &out_depot_colors_Memory_Already_Set,
		int &out_num_count);
	void Find_Error_With_Average(
		std::vector<std::vector<float>> &in_depot_colors,
		int in_num_size,
		CKvRgbaF *in_target_colors,
		int *out_error_in_yuv);
	void Find_Error_With_Distance_Weight(
		std::vector<std::vector<float>> &in_depot_colors,
		std::vector<int> &in_voxel_index_Memory_Already_Set,
		std::vector<float> &in_wiehgt_Memory_Already_Set,
		CKvPoint3Df *in_pointer_point3d,
		CKvRgbaF *in_target_colors,
		CKvPoint3Df *in_target_point3d,
		int in_num_size,
		int *out_error_in_yuv);
	void Find_Error_With_Distance_Weight_RGB(
		std::vector<std::vector<float>> &in_depot_colors,
		std::vector<int> &in_voxel_index_Memory_Already_Set,
		std::vector<float> &in_wiehgt_Memory_Already_Set,
		CKvPoint3Df *in_pointer_point3d,
		CKvRgbaF *in_target_colors,
		CKvPoint3Df *in_target_point3d,
		int in_num_size,
		int *out_error_in_rgb);
	void Prediction_With_Average(
		std::vector<std::vector<float>> &in_depot_colors,
		int in_num_count,
		float *out_prediction_in_yuv_3_dim);
	void Prediction_With_Distance_Weight(
		std::vector<std::vector<float>> &in_depot_colors,
		std::vector<int> &in_voxel_index_Memory_Already_Set,
		std::vector<float> &in_wiehgt_Memory_Already_Set,
		CKvPoint3Df *in_pointer_point3d,
		CKvPoint3Df *in_target_point3d,
		int in_num_size,
		float *out_prediction_in_yuv_3_dim);
	void Prediction_With_Distance_Weight_RGB(
		std::vector<std::vector<float>> &in_depot_colors,
		std::vector<int> &in_voxel_index_Memory_Already_Set,
		std::vector<float> &in_wiehgt_Memory_Already_Set,
		CKvPoint3Df *in_pointer_point3d,
		CKvPoint3Df *in_target_point3d,
		int in_num_size,
		float *out_prediction_in_rgb_3_dim);
	void Convert_To_Uchar(
		CKvMatrixIntRgb   *in_error_map,
		CKvMatrixUcharRgb *out_error_map);
	void Convert_To_Int(
		CKvMatrixUcharRgb *in_error_map,
		CKvMatrixIntRgb   *out_error_map);
	/****************************************************************************/



	void pvm_Packing_Voxel_Maps(
		VCL_DoCube_X_Color *in_docube,
		CKvSet_of_MatrixInt *in_set_voxel_map_idx,
		CKvSet_of_MatrixBool *in_set_voxel_map_idx_mask,
		CKvSet_of_MatrixInt *in_set_error_map_idx,
		CKvSet_of_MatrixBool *in_set_error_map_mask,
		CKvSet_of_MatrixIntRgb *in_set_error_map,
		int in_block_size,
		bool in_shape_coding_mode_on_off,
		char in_types_of_encoder,
		bool in_image_fixed,
		CKvMatrixInt *out_blob_position,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx,
		CKvMatrixBool *out_voxel_map_mask,
		CKvMatrixIntRgb *out_error_map,
		CKvMatrixInt *out_error_map_idx,
		CKvMatrixBool *out_error_map_mask);




	void gpvm_Generate_Packing_Voxel_Map_For_Image_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_RgbaF *in_colors_or_NULL,
		int in_block_size,
		bool in_shape_coding_mode_on_off,
		char in_types_of_encoder,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_positions_voxel_blob,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx_or_NULL,
		CKvMatrixBool *out_mask);
	void gpvm_Generate_Packing_Voxel_Map_For_Error_Image(
		CKvSet_of_MatrixInt *in_set_error_map_index,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvSet_of_MatrixIntRgb *in_set_error_maps,
		int in_block_size,
		bool in_shape_coding_mode_on_off,
		char in_types_of_encoder,
		CKvMatrixIntRgb *out_error_map,
		CKvMatrixInt *out_error_map_idx_or_NULL,
		CKvMatrixBool *out_mask);
	void gpvm_Generate_Packing_Voxel_Map_For_Alignment_Mode3(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_RgbaF *in_colors_or_NULL,
		int in_block_size,
		bool in_shape_coding_mode_on_off,
		char in_types_of_encoder,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_positions_voxel_blob,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx_or_NULL,
		CKvMatrixBool *out_mask);
	void gpvm_Generate_Packing_Voxel_Map(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_RgbaF *in_colors_or_NULL,
		int in_block_size,
		bool in_shape_coding_mode_on_off,
		char in_types_of_encoder,
		bool in_image_fixed,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx_or_NULL,
		CKvMatrixBool *out_mask_or_NULL);
	void gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_RgbaF *in_colors_or_NULL,
		int in_block_size,
		char in_types_of_encoder,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx_or_NULL,
		CKvMatrixBool *out_mask_or_NULL);
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

	void Get_Voxel_Colors_From_Texture_Map(
		CKvMatrixUcharRgb *in_voxel_map,
		CKvMatrixBool *in_voxel_map_mask,
		CKvMatrixInt *in_voxel_map_index,
		int in_number_of_voxels,
		CKvDepot_of_RgbaF *out_colors,
		CKvVectorBool *out_flag);

	void Get_Voxel_Colors_From_Texture_Map(
		CKvMatrixUcharRgb *in_voxel_map,
		CKvMatrixBool *in_voxel_map_mask,
		CKvMatrixInt *in_voxel_map_index,
		int in_number_of_voxels,
		CKvSet_of_RgbaF *out_colors);


	void RGBtoYUV_BT709(
		float *in_target_rgb,
		float *out_yuv);
	void YUVtoRGB_BT709(
		float *in_target_yuv,
		float *out_rgb);

public:
	// This module is for experiment. 
	void Generate_Voxel_Maps_WO_partitioning(
		VCL_DoCube_X_Color *in_docube,
		std::vector<int> in_plane_mode_sequence,
		int in_alignment_mode,
		int in_threshold_for_cluster_size,
		CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask,
		CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
		CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask);
	void gvmv_Generate_Voxel_Maps_WO_partitioning(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
		int in_alignment_mode,
		CKvSet_of_MatrixInt *out_set_of_voxel_map,
		CKvSet_of_MatrixBool *out_set_of_voxel_mask);

protected:
	std::vector<int> zz_voxel_idx_for_processing;
	std::vector<std::vector<float>> zz_vec_color_for_processing;
	std::vector<float> zz_weight_for_processing;

	bool zz_shape_coding_mode_on_off;
	int zz_plane_mode_sequence;
	int zz_threshold_for_cluster_size;
	int zz_prediction_searching_range;
	int zz_prediction_mode;
	int zz_error_threshold;
	int zz_alignment_mode;


	CKvSet_of_Voxel zz_before_set_of_voxels;
	CKvSet_of_RgbaF zz_before_set_of_colors;
	std::vector<std::vector<std::vector<int>>> zz_before_segmented_texture_on_code;
};
