#ifndef VCL_VOXEL_MAP_PCC
#define VCL_VOXEL_MAP_PCC

#include "VCL_EI_DCT.h"
#include <opencv2/opencv.hpp>

#include "Voxel_Slice_Scanned_Data.h"

#include <vector>
//#define INVALID_CODE -1
//#define START_CODE 0
//#define APPEND_CODE 1

class VCL_Voxel_Color_Map : public VCL_EI_DCT
{
public:
	VCL_Voxel_Color_Map();
	~VCL_Voxel_Color_Map();

	void sp_Set_Parameter_Width(
		int in_map_width);

	void sp_Set_Parameter_Height(
		int in_map_height);

	void sp_Set_Parameter_Map_Size(
		int in_map_width,
		int in_map_height);

	void li_Load_Image_from_OpenCV(
		CKvString &in_filename,
		CKvMatrixUcharRgb *out_image);

	void li_Load_Image_from_OpenCV_16bit(
		CKvString &in_filename,
		CKvMatrixIntRgb *out_image);

	void li_Load_Image_from_OpenCV(
		CKvString &in_filename,
		CKvMatrixUchar *out_image);

	void li_Load_Image_from_OpenCV(
		CKvString &in_filename,
		CKvMatrixBool *out_image);

	void si_Save_Image_from_OpenCV(
		CKvString &in_filename,
		CKvMatrixUcharRgb *out_image);

	void si_Save_Image_from_OpenCV_16bit(
		CKvString &in_filename,
		CKvMatrixIntRgb *out_image);

	void si_Save_Image_from_OpenCV(
		CKvString &in_filename,
		CKvMatrixBool *out_image);

	void si_Save_Image_from_OpenCV(
		CKvString &in_filename,
		CKvMatrixUchar *out_image);

protected:

	bool s_gvm_Generate_Voxel_Map__All_Intra(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		int in_threshold_for_cluster_size,
		CKvSet_of_VectorInt *in_offsets,
		CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
		CKvDepot_of_MatrixBool *out_set_masks);


	bool s_gvm_Generate_Voxel_Map__Intra(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		int in_threshold_for_cluster_size,
		bool in_mode_palette_true_mode_image_false,
		CKvSet_of_VectorInt *in_offsets,
		CKvSet_of_MatrixUcharRgb *out_set_voxel_color_maps,
		CKvSet_of_MatrixBool *out_set_masks,
		std::vector<unsigned char> &out_residual_red,
		std::vector<unsigned char> &out_residual_green,
		std::vector<unsigned char> &out_residual_blue,
		std::vector<int> &out_residual_idx);

	bool s_gvm_Generate_Voxel_Map__Intra_Hybrid_Coding(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		int in_threshold_for_cluster_size,
		bool in_mode_palette_true_mode_image_false,
		CKvSet_of_VectorInt *in_offsets,
		CKvDepot_of_MatrixUcharRgb *out_set_voxel_color_maps,
		CKvDepot_of_MatrixBool *out_set_masks,
		CKvDepot_of_Point3Df *io_voxel_dust_pt,
		CKvDepot_of_RgbaF *io_voxel_dust_color);

	bool s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		int in_threshold_for_cluster_size,
		CKvSet_of_VectorInt *in_offsets,
		CKvDepot_of_MatrixUcharRgb *out_set_voxel_color_maps,
		CKvDepot_of_MatrixBool *out_set_masks,
		CKvDepot_of_MatrixInt *out_set_voxel_idx_for_prediction,
		CKvDepot_of_MatrixBool *out_set_masks_for_prediction);

	bool s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
		VCL_DoCube_X_Color *in_docube,
		std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		int in_threshold_for_cluster_size,
		CKvSet_of_VectorInt *in_offsets,
		CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
		CKvDepot_of_MatrixBool *out_set_masks,
		CKvDepot_of_MatrixInt *out_set_voxel_idx_for_prediction,
		CKvDepot_of_MatrixBool *out_set_masks_for_prediction);

	void mvm_Merge_Voxel_Map(
		CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	void Packing_Voxel_Map_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvDepot_of_RgbaF *in_colors_or_NULL,
		int in_block_size,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixInt *out_voxel_map_idx_or_NULL,
		CKvMatrixBool *out_mask_or_NULL);

	void pvm_Pack_Voxel_Map_With_Filling(
		CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_shape_coding_mode,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map_With_Filling(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_shape_coding_mode,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void Resize_Voxel_Blobs(
		CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
		CKvSet_of_MatrixBool *io_set_voxel_masks,
		int in_block_size);

	void pvm_Pack_Voxel_Map_With_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map_With_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_line_ei_on_true_off_false,
		bool in_image_fixed,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map_With_Filling_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_shape_coding_mode,
		bool in_image_fixed,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map_With_Filling(
		CKvSet_of_MatrixIntRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_shape_coding_mode,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixIntRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	/*************************************************************/
	void mvm_Merge_Voxel_Map_Vertical(
		CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map(
		CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map(
		CKvSet_of_MatrixIntRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixIntRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		bool in_fixed_image,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void pvm_Pack_Voxel_Map_Maximum_Margin(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		bool in_line_ei_on_true_off_false,
		bool in_fixed_image,
		bool in_pack_flexible,
		CKvMatrixInt *out_position_of_segmented_voxel_map,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	void Set_New_Masks(
		CKvMatrixBool *in_mask,
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

	void Filling_Voxel_Map(
		CKvMatrixUcharRgb *in_set_voxel_maps,
		CKvMatrixBool *in_set_masks,
		int in_block_size,
		bool in_shape_coding_mode,
		CKvMatrixUcharRgb *out_voxel_map);

	void mvm_Merge_Voxel_Map_With_Filling(
		CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_masks,
		int in_block_size,
		bool in_shape_coding_mode,
		CKvMatrixInt *out_position_of_segmented_voxe_map,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	void geir_Get_EI_Region(
		CKvMatrixBool *in_set_of_mask,
		int in_block_size,
		CKvMatrixBool *out_set_of_mask);

	void geir_Get_EI_Region(
		CKvSet_of_MatrixBool *in_set_of_mask,
		int in_block_size,
		CKvSet_of_MatrixBool *out_set_of_mask);


	/**********************************************************************************/
	void gsci_Get_Shape_Coding_Image_EI(
		CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_voxel_map_masks,
		int &in_block_x,
		int &in_block_y,
		CKvMatrixUcharRgb *out_EI_image,
		CKvMatrixBool *out_mask);

	void gsci_Get_Shape_Coding_Image_EI(
		CKvSet_of_MatrixInt *in_set_voxel_maps,
		CKvSet_of_MatrixBool *in_set_voxel_map_masks,
		int &in_block_x,
		int &in_block_y,
		CKvMatrixInt *out_EI_image,
		CKvMatrixBool *out_mask);

	void gbiei_Get_Block_Images_EI(
		CKvMatrixUcharRgb *in_img,
		CKvMatrixBool *in_mask,
		int &in_block_x,
		int &in_block_y,
		CKvDepot_of_MatrixUcharRgb *out_block_imgs,
		CKvDepot_of_MatrixBool *out_block_masks);

	void gbiei_Get_Block_Images_Index(
		CKvMatrixInt *in_img,
		CKvMatrixBool *in_mask,
		int &in_block_x,
		int &in_block_y,
		CKvDepot_of_MatrixInt *out_block_imgs,
		CKvDepot_of_MatrixBool *out_block_masks);

	//void gbiei_Get_Block_Images_Index(
	//	CKvMatrixInt *in_img,
	//	CKvMatrixBool *in_mask,
	//	int &in_block_x,
	//	int &in_block_y,
	//	CKvDepot_of_MatrixInt *out_block_imgs,
	//	CKvDepot_of_MatrixBool *out_block_masks);

	void cvm_Convert_Voxel_Map(
		CKvDepot_of_MatrixUcharRgb *in_block_imgs,
		CKvDepot_of_MatrixBool *in_block_masks,
		CKvMatrixUcharRgb *out_voxel_map,
		CKvMatrixBool *out_mask);

	void cvm_Convert_Voxel_Map_Index(
		CKvDepot_of_MatrixInt *in_block_imgs,
		CKvDepot_of_MatrixBool *in_block_masks,
		CKvMatrixInt *out_voxel_map,
		CKvMatrixBool *out_mask);

	//void cvm_Convert_Voxel_Map_Index(
	//	CKvDepot_of_MatrixInt *in_block_imgs,
	//	CKvDepot_of_MatrixBool *in_block_masks,
	//	CKvMatrixInt *out_voxel_map,
	//	CKvMatrixBool *out_mask);

	int neb_The_Number_of_Elements_In_Blob(
		std::vector<std::vector<int>> &in_texture_on_code);

	bool ie_Is_Empty(CKvMatrixBool *in_mask);


protected:
	int zz_var_for_debug;
	int zz_width, zz_height;
	int zz_x, zz_y; // For Packing Algorithm
	int zz_safeguard;
	int zz_block_size;

	//This is for experimental function that is used in the paper. 
	bool s_gvm_Generate_Voxel_Map__wo_partitioning(
		VCL_DoCube_X_Color *in_docube,
		std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
		//std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		CKvSet_of_VectorInt *in_offsets,
		CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
		CKvDepot_of_MatrixBool *out_set_masks);


	//void s_gvm_Generate_Voxel_Map(
	//	VCL_DoCube_X_Color *in_docube,
	//	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	//	int in_threshold_for_cluster_size,
	//	CKvSet_of_VectorInt *in_offsets,
	//	CKvSet_of_MatrixUcharRgb *out_set_voxel_color_maps,
	//	CKvSet_of_MatrixBool *out_set_masks);
	//void s_gvm_Generate_Voxel_Map_Index(
	//	VCL_DoCube_X_Color *in_docube,
	//	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	//	CKvSet_of_VectorInt *in_offsets,
	//	CKvSet_of_MatrixInt *out_set_voxel_color_maps,
	//	CKvSet_of_MatrixBool *out_set_masks);
	//void s_gvm_Generate_Voxel_Map_Index(
	//	VCL_DoCube_X_Color *in_docube,
	//	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	//	int in_threshold_for_cluster_size,
	//	CKvSet_of_VectorInt *in_offsets,
	//	CKvSet_of_MatrixInt *out_set_voxel_color_maps,
	//	CKvSet_of_MatrixBool *out_set_masks,
	//	std::vector<int> &out_predict_voxel_index);
	//void cvti_Color_Vector_To_Image(
	//	CKvVectorUcharRgb *in_color_vec,
	//	int in_block_size,
	//	CKvMatrixUcharRgb *out_image,
	//	CKvMatrixBool *out_mask);
	//void cvti_Color_Vector_To_Image2(
	//	CKvVectorUcharRgb *in_color_vec,
	//	int in_block_size,
	//	CKvMatrixUcharRgb *out_image,
	//	CKvMatrixBool *out_mask);
	//void dvm_Decode_Voxel_Map_1d_EI(
	//	CKvMatrixUcharRgb *in_voxel_map_1d_ei,
	//	CKvMatrixBool *in_mask,
	//	int &in_length,
	//	CKvMatrixUcharRgb *out_decode_voxel_map);
	//void gsci_Get_Shape_Coding_Image_Index(
	//	CKvSet_of_MatrixInt *in_set_voxel_maps,
	//	CKvSet_of_MatrixBool *in_set_voxel_map_masks,
	//	int &in_block_x,
	//	int &in_block_y,
	//	CKvMatrixInt *out_EI_image,
	//	CKvMatrixBool *out_mask);
	//void gvm_Generate_Voxel_Map(
	//	VCL_DoCube_X_Color *in_docube,
	//	std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
	//	int &in_plane_mode_0X_1Y_2Z_3zy,
	//	int in_alignment_mode,
	//	CKvMatrixUcharRgb *out_voxel_map,
	//	CKvMatrixBool *out_voxel_map_mask);
	//void gvm_Generate_Voxel_Map(
	//	VCL_DoCube_X_Color *in_docube,
	//	std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
	//	int &in_plane_mode_0X_1Y_2Z_3zy,
	//	int in_alignment_mode,
	//	CKvSet_of_MatrixUcharRgb *out_set_voxel_map,
	//	CKvSet_of_MatrixBool *out_set_voxel_map_mask);
	//void gvm_Generate_Voxel_Map_Index(
	//	VCL_DoCube_X_Color *in_docube,
	//	std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
	//	int &in_plane_mode_0X_1Y_2Z_3zy,
	//	int in_alignment_mode,
	//	CKvSet_of_MatrixInt *out_set_voxel_map,
	//	CKvSet_of_MatrixBool *out_set_voxel_map_mask);
	//void gvm_Generate_Voxel_Map_2D(
	//	VCL_DoCube_X_Color *in_docube,
	//	int &in_plane_mode_0X_1Y_2Z_3zy,
	//	bool in_shape_coding_mode_on_off,
	//	int in_alignment_mode,
	//	int in_block_x,
	//	int in_block_y,
	//	CKvMatrixUcharRgb *out_voxel_map,
	//	CKvMatrixBool *out_voxel_map_mask);
	//void gvm_Generate_Voxel_Map_2D_Only_Surface(
	//	VCL_DoCube_X_Color *in_docube,
	//	int &in_plane_mode_0X_1Y_2Z_3zy,
	//	bool in_shape_coding_mode_on_off,
	//	int in_alignment_mode,
	//	int in_block_x,
	//	int in_block_y,
	//	CKvMatrixUcharRgb *out_voxel_map,
	//	CKvMatrixBool *out_voxel_map_mask);
	//void i_Import_Voxel_Texture_Map(
	//	VCL_DoCube_X_Color *io_docube,
	//	int &in_plane_mode_0X_1Y_2Z_3zy,
	//	int in_alignment_mode,
	//	int in_block_size,
	//	CKvMatrixUcharRgb *in_voxel_color_map);
	//void s_gvm_Generate_Voxel_Map(
	//	VCL_DoCube_X_Color *in_docube,
	//	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	//	CKvSet_of_VectorInt *in_offsets,
	//	CKvSet_of_MatrixUcharRgb *out_set_voxel_color_maps,
	//	CKvSet_of_MatrixBool *out_set_masks);
	//**********************************************************************************/
	// Segmentation Module
	//**********************************************************************************/
	//void stoc_Segment_Texture_on_Code(
	//	std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
	//	std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code);
	//void atoc_Analyze_Texture_on_Code(
	//	Voxel_Slice_Scanned_Data &in_current_texture_on_code,
	//	Voxel_Slice_Scanned_Data &in_before_texture_on_code,
	//	int &in_current_maximum_label, 
	//	std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code);
	//void acm_Analyze_Connectivity_Matrix(
	//	Voxel_Slice_Scanned_Data &in_current_texture_on_code,
	//	Voxel_Slice_Scanned_Data &in_before_texture_on_code,
	//	int &in_current_maximum_label,
	//	CKvMatrixBool *in_connectivity_matrix,
	//	CKvVectorChar *out_state_code_of_current_texture_on_code);
	//bool fcm_Find_Connectivity_Matrix(
	//	Voxel_Slice_Scanned_Data &in_current_pcc,
	//	Voxel_Slice_Scanned_Data &in_before_pcc,
	//	CKvMatrixBool &out_connectivity_matrix);

};

#endif