#include "stdafx.h"
#include "VCL_Voxel_Texture_Atlas_3_Direction.h"

/**********************************************************************************/
VCL_Voxel_Texture_Atlas_3_Direction::VCL_Voxel_Texture_Atlas_3_Direction()
/**********************************************************************************/
{
	zz_classic_mode = true;
}
/**********************************************************************************/
VCL_Voxel_Texture_Atlas_3_Direction::~VCL_Voxel_Texture_Atlas_3_Direction()
/**********************************************************************************/
{

}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::Set_Parameters(
	bool in_shape_coding_mode_on_off,
	int in_plane_mode_sequence,
	int in_alignment_mode,
	int in_block_size,
	int in_threshold_for_cluster_size,
	float in_threshold_for_segmentation,
	int in_threshold_cluster,
	int in_map_width,
	int in_map_height)
/**********************************************************************************/
{
	sp_Set_Parameter_for_Segmentation(in_threshold_for_segmentation);
	sp_Set_Parameter_Map_Size(in_map_width, in_map_height);

	zz_alignment_mode = in_alignment_mode;
	zz_block_size = in_block_size;
	zz_plane_mode_sequence = in_plane_mode_sequence;
	zz_threshold_cluster = in_threshold_cluster;
}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::sp_Set_Parameter_Map_Size(
	int in_map_width,
	int in_map_height)
/**********************************************************************************/
{
	zz_width = in_map_width;
	zz_height = in_map_height;
}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::generate_Voxel_Texture_Atlas_3_Direction(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	CKvSet_of_MatrixUcharRgb *out_voxel_maps,
	CKvSet_of_MatrixBool *out_voxel_map_masks,
	CKvSet_of_MatrixInt *out_voxel_idx_maps)
/**********************************************************************************/
{
	CKvSet_of_MatrixInt set_of_voxel_map_idx;
	CKvSet_of_MatrixBool set_of_voxel_map_idx_mask;

	CKvMatrixInt position_of_segmented_voxel_map;
	CKvMatrixUcharRgb *voxel_map;
	CKvMatrixInt *voxel_map_idx;
	CKvMatrixBool *mask, Max_mask;
	int num_plane_mode = in_plane_mode_sequence.size();
	int k;

	voxel_map     = out_voxel_maps->c_Create(num_plane_mode);
	mask          = out_voxel_map_masks->c_Create(num_plane_mode);
	voxel_map_idx = out_voxel_idx_maps->c_Create(num_plane_mode);

	CKvStopWatch sw;
	sw.c_Create(1);
	sw.r_Reset(0);

	for (k = 0; k < num_plane_mode; k++)
	{
		Partitioning_Voxels_And_Generate_Voxel_Maps(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_plane_mode_sequence[k],//int in_plane_mode_sequence,
			zz_alignment_mode,//int in_alignment_mode,
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
			&set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)

		gpvm_Generate_Packing_Voxel_Map_for_Video(
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
			&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
			&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_points,
			&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
			zz_block_size,//int in_block_size,
			&position_of_segmented_voxel_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
			&voxel_map[k],//CKvMatrixUcharRgb *out_voxel_map,
			&voxel_map_idx[k],//CKvMatrixInt *out_voxel_map_idx_or_NULL,
			&mask[k],//CKvMatrixBool *out_mask_or_NULL,
			&Max_mask);//CKvMatrixBool *out_Max_mask_or_NULL);

		//LCKvIO_FileJpg iof_jpg;
		//CKvString str;
		//str.fm_Format("%d_img.bmp", k);
		//iof_jpg.si_Save_Image(str, false, &voxel_map[k]);
	}


	//Modify_Texture_Atlases(
	//	&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors,
	//	out_voxel_maps,//CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
	//	out_voxel_idx_maps);//CKvSet_of_MatrixInt *in_set_of_voxel_map_idx)

	Modify_Texture_Atlases(
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors,
		out_voxel_maps,//CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
		out_voxel_map_masks,//CKvSet_of_MatrixBool *io_set_of_voxel_map_masks,
		out_voxel_idx_maps);//CKvSet_of_MatrixInt *in_set_of_voxel_map_idx)

	//printf("[Error Map Gen]\n");
	//Generate_Error_Maps(
	//	&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_points,
	//	&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors,
	//	out_voxel_maps,//CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
	//	out_voxel_map_masks,//CKvSet_of_MatrixBool *io_set_of_voxel_map_masks,
	//	out_voxel_idx_maps,//CKvSet_of_MatrixInt *in_set_of_voxel_map_idx,
	//	15);//int in_filter_size)

	printf("[Done] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));

	//zz_vcl_ii.interpolate_background(
	//	out_voxel_maps->gpe(0),
	//	out_voxel_map_masks->gpe(0),
	//	out_voxel_maps->gpe(0));

	//zz_vcl_ii.interpolate_background(
	//	out_voxel_maps->gpe(1),
	//	out_voxel_map_masks->gpe(1),
	//	out_voxel_maps->gpe(1));

	//zz_vcl_ii.interpolate_background(
	//	out_voxel_maps->gpe(2),
	//	out_voxel_map_masks->gpe(2),
	//	out_voxel_maps->gpe(2));

	//printf("[Interpolate] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));

}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::generate_Voxel_Texture_Atlas_3_Direction2(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	CKvSet_of_MatrixUcharRgb *out_voxel_maps,
	CKvSet_of_MatrixBool *out_voxel_map_masks,
	CKvSet_of_MatrixInt *out_voxel_idx_maps)
/**********************************************************************************/
{
	std::vector<bool> voxel_flag;
	CKvSet_of_MatrixInt    set_of_voxel_map_idx;
	CKvSet_of_MatrixBool   set_of_voxel_map_idx_mask;
	CKvDepot_of_MatrixInt  set_of_large_index_maps, set_of_small_index_maps, depot_of_large_index_maps, depot_of_small_index_maps;
	CKvDepot_of_MatrixBool set_of_large_masks, set_of_small_masks, depot_of_large_masks, depot_of_small_masks;
	
	CKvMatrixInt position_of_segmented_voxel_map;
	CKvMatrixUcharRgb *voxel_map;
	CKvMatrixInt *voxel_map_idx;
	CKvMatrixBool *mask, Max_mask;
	int num_plane_mode = in_plane_mode_sequence.size();
	int k, num_voxels;

	voxel_map     = out_voxel_maps->c_Create(2);
	mask          = out_voxel_map_masks->c_Create(2);
	voxel_map_idx = out_voxel_idx_maps->c_Create(2);

	num_voxels    = in_docube->gsp_Get_Surface_Points_Pointer()->ne_Number_of_Elements();

	CKvStopWatch sw;
	sw.c_Create(1);
	sw.r_Reset(0);

	voxel_flag = std::vector<bool>(num_voxels, false);
	depot_of_large_index_maps.in_Initialize(); depot_of_small_index_maps.in_Initialize();
	depot_of_large_masks.in_Initialize();      depot_of_small_masks.in_Initialize();
	for (k = 0; k < num_plane_mode; k++)
	{
		Partitioning_Voxels_And_Generate_Voxel_Maps(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_plane_mode_sequence[k],//int in_plane_mode_sequence,
			zz_alignment_mode,//int in_alignment_mode,
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
			&set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)

		printf("k=%d/%d) [Extract_Voxel_Patch_Maps]\n",k, num_plane_mode);
		Extract_Voxel_Patch_Maps(
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_of_voxel_maps,
			&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_of_voxel_map_masks,
			voxel_flag,//std::vector<bool> &in_voxel_flag,
			1,//int in_closing_size,
			&set_of_large_index_maps,//CKvDepot_of_MatrixInt *out_set_of_large_index_maps,
			&set_of_large_masks,//CKvDepot_of_MatrixBool *out_set_of_large_masks,
			&set_of_small_index_maps,//CKvDepot_of_MatrixInt *out_set_of_small_index_maps,
			&set_of_small_masks);//CKvDepot_of_MatrixBool *out_set_of_small_masks);

		depot_of_large_index_maps.ap_Append(false, &set_of_large_index_maps, NULL,NULL);
		depot_of_small_index_maps.ap_Append(false, &set_of_small_index_maps, NULL, NULL);
		depot_of_large_masks.ap_Append(false, &set_of_large_masks, NULL,NULL);
		depot_of_small_masks.ap_Append(false, &set_of_small_masks, NULL,NULL);
	}

	set_of_large_index_maps.in_Initialize();
	set_of_small_index_maps.in_Initialize();
	set_of_large_masks.in_Initialize();
	set_of_small_masks.in_Initialize();

	depot_of_large_index_maps.ex_Export(&set_of_voxel_map_idx);
	depot_of_large_masks.ex_Export(&set_of_voxel_map_idx_mask);
	gpvm_Generate_Packing_Voxel_Map_for_Video(
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_points,
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
		zz_block_size,//int in_block_size,
		&position_of_segmented_voxel_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		&voxel_map[0],//CKvMatrixUcharRgb *out_voxel_map,
		&voxel_map_idx[0],//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		&mask[0],//CKvMatrixBool *out_mask_or_NULL,
		&Max_mask);//CKvMatrixBool *out_Max_mask_or_NULL);

	depot_of_small_index_maps.ex_Export(&set_of_voxel_map_idx);
	depot_of_small_masks.ex_Export(&set_of_voxel_map_idx_mask);
	gpvm_Generate_Packing_Voxel_Map_for_Video(
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_points,
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
		zz_block_size,//int in_block_size,
		&position_of_segmented_voxel_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		&voxel_map[1],//CKvMatrixUcharRgb *out_voxel_map,
		&voxel_map_idx[1],//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		&mask[1],//CKvMatrixBool *out_mask_or_NULL,
		&Max_mask);//CKvMatrixBool *out_Max_mask_or_NULL);
	printf("[Done] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));

	zz_vcl_ii.interpolate_background(
		out_voxel_maps->gpe(0),
		out_voxel_map_masks->gpe(0),
		out_voxel_maps->gpe(0));

	zz_vcl_ii.interpolate_background(
		out_voxel_maps->gpe(1),
		out_voxel_map_masks->gpe(1),
		out_voxel_maps->gpe(1));

	printf("[Interpolate] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));
}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::generate_Voxel_Texture_Atlas_3_Direction3(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	CKvSet_of_MatrixUcharRgb *out_voxel_maps,
	CKvSet_of_MatrixBool *out_voxel_map_masks,
	CKvSet_of_MatrixInt *out_voxel_idx_maps,
	CKvMatrixInt *out_pt_list_for_secong_img_N_by_3)
/**********************************************************************************/
{
	std::vector<bool> voxel_flag;
	CKvSet_of_MatrixInt    set_of_voxel_map_idx;
	CKvSet_of_MatrixBool   set_of_voxel_map_idx_mask;
	CKvDepot_of_MatrixInt  set_of_large_index_maps, set_of_small_index_maps, depot_of_large_index_maps, depot_of_small_index_maps;
	CKvDepot_of_MatrixBool set_of_large_masks, set_of_small_masks, depot_of_large_masks, depot_of_small_masks;
	CKvMatrixInt position_of_segmented_voxel_map, prediction_list_and_position_N_by_3;
	CKvMatrixUcharRgb *voxel_map;
	CKvMatrixInt *voxel_map_idx;
	CKvMatrixBool *mask, Max_mask;
	int num_plane_mode = in_plane_mode_sequence.size();
	int k, num_voxels;

	voxel_map = out_voxel_maps->c_Create(2);
	mask = out_voxel_map_masks->c_Create(2);
	voxel_map_idx = out_voxel_idx_maps->c_Create(2);

	num_voxels = in_docube->gsp_Get_Surface_Points_Pointer()->ne_Number_of_Elements();

	CKvStopWatch sw;
	sw.c_Create(1);
	sw.r_Reset(0);

	voxel_flag = std::vector<bool>(num_voxels, false);
	depot_of_large_index_maps.in_Initialize(); depot_of_small_index_maps.in_Initialize();
	depot_of_large_masks.in_Initialize();      depot_of_small_masks.in_Initialize();
	for (k = 0; k < num_plane_mode; k++)
	{
		Partitioning_Voxels_And_Generate_Voxel_Maps(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_plane_mode_sequence[k],//int in_plane_mode_sequence,
			zz_alignment_mode,//int in_alignment_mode,
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
			&set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)

		printf("k=%d/%d) [Extract_Voxel_Patch_Maps]\n", k, num_plane_mode);
		Extract_Voxel_Patch_Maps(
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_of_voxel_maps,
			&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_of_voxel_map_masks,
			voxel_flag,//std::vector<bool> &in_voxel_flag,
			3,//int in_closing_size,
			&set_of_large_index_maps,//CKvDepot_of_MatrixInt *out_set_of_large_index_maps,
			&set_of_large_masks,//CKvDepot_of_MatrixBool *out_set_of_large_masks,
			&set_of_small_index_maps,//CKvDepot_of_MatrixInt *out_set_of_small_index_maps,
			&set_of_small_masks);//CKvDepot_of_MatrixBool *out_set_of_small_masks);

		depot_of_large_index_maps.ap_Append(false, &set_of_large_index_maps, NULL, NULL);
		depot_of_small_index_maps.ap_Append(false, &set_of_small_index_maps, NULL, NULL);
		depot_of_large_masks.ap_Append(false, &set_of_large_masks, NULL, NULL);
		depot_of_small_masks.ap_Append(false, &set_of_small_masks, NULL, NULL);
	}

	set_of_large_index_maps.in_Initialize();
	set_of_small_index_maps.in_Initialize();
	set_of_large_masks.in_Initialize();
	set_of_small_masks.in_Initialize();

	depot_of_large_index_maps.ex_Export(&set_of_voxel_map_idx);
	depot_of_large_masks.ex_Export(&set_of_voxel_map_idx_mask);
	gpvm_Generate_Packing_Voxel_Map_for_Video(
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_points,
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
		zz_block_size,//int in_block_size,
		&position_of_segmented_voxel_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		&voxel_map[0],//CKvMatrixUcharRgb *out_voxel_map,
		&voxel_map_idx[0],//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		&mask[0],//CKvMatrixBool *out_mask_or_NULL,
		&Max_mask);//CKvMatrixBool *out_Max_mask_or_NULL);

	depot_of_small_index_maps.ex_Export(&set_of_voxel_map_idx);
	depot_of_small_masks.ex_Export(&set_of_voxel_map_idx_mask);
	Generate_Prediction_Image(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		&voxel_map[0],//CKvMatrixUcharRgb *in_voxel_map,
		&mask[0],//CKvMatrixBool *in_voxel_mask,
		&voxel_map_idx[0],//CKvMatrixInt  *in_voxel_idx_map,
		&position_of_segmented_voxel_map,//CKvMatrixInt *in_position_of_blob,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_of_prediction_map,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_of_prediction_mask,
		&voxel_map[1],//CKvMatrixUcharRgb *out_prediction_voxel_map,
		out_pt_list_for_secong_img_N_by_3);//CKvMatrixInt *out_prediction_list_and_position_N_by_3)


	printf("[Done] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));

	zz_vcl_ii.interpolate_background(
		out_voxel_maps->gpe(0),
		out_voxel_map_masks->gpe(0),
		out_voxel_maps->gpe(0));

	zz_vcl_ii.interpolate_background(
		out_voxel_maps->gpe(1),
		out_voxel_map_masks->gpe(0),
		out_voxel_maps->gpe(1));

	printf("[Interpolate] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));
}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::generate_Voxel_Texture_Atlas_3_Direction3_Fast(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	CKvSet_of_MatrixUcharRgb *out_voxel_maps,
	CKvSet_of_MatrixBool *out_voxel_map_masks,
	CKvSet_of_MatrixInt *out_voxel_idx_maps,
	CKvMatrixInt *out_pt_list_for_secong_img_N_by_3)
/**********************************************************************************/
{
	std::vector<bool> voxel_flag;
	CKvSet_of_MatrixInt    set_of_voxel_map_idx;
	CKvSet_of_MatrixBool   set_of_voxel_map_idx_mask;
	CKvDepot_of_MatrixInt  set_of_large_index_maps, set_of_small_index_maps, depot_of_large_index_maps, depot_of_small_index_maps;
	CKvDepot_of_MatrixBool set_of_large_masks, set_of_small_masks, depot_of_large_masks, depot_of_small_masks;
	CKvMatrixInt position_of_segmented_voxel_map, prediction_list_and_position_N_by_3;
	CKvMatrixUcharRgb *voxel_map;
	CKvMatrixInt *voxel_map_idx;
	CKvMatrixBool *mask, Max_mask;
	int num_plane_mode = in_plane_mode_sequence.size();
	int k, num_voxels;

	voxel_map = out_voxel_maps->c_Create(2);
	mask = out_voxel_map_masks->c_Create(2);
	voxel_map_idx = out_voxel_idx_maps->c_Create(2);

	num_voxels = in_docube->gsp_Get_Surface_Points_Pointer()->ne_Number_of_Elements();

	CKvStopWatch sw;
	sw.c_Create(1);
	sw.r_Reset(0);

	voxel_flag = std::vector<bool>(num_voxels, false);
	depot_of_large_index_maps.in_Initialize(); depot_of_small_index_maps.in_Initialize();
	depot_of_large_masks.in_Initialize();      depot_of_small_masks.in_Initialize();
	for (k = 0; k < num_plane_mode; k++)
	{
		Partitioning_Voxels_And_Generate_Voxel_Maps(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_plane_mode_sequence[k],//int in_plane_mode_sequence,
			zz_alignment_mode,//int in_alignment_mode,
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
			&set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)

		printf("k=%d/%d) [Extract_Voxel_Patch_Maps]\n", k, num_plane_mode);
		Extract_Voxel_Patch_Maps(
			&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_of_voxel_maps,
			&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_of_voxel_map_masks,
			voxel_flag,//std::vector<bool> &in_voxel_flag,
			3,//int in_closing_size,
			&set_of_large_index_maps,//CKvDepot_of_MatrixInt *out_set_of_large_index_maps,
			&set_of_large_masks,//CKvDepot_of_MatrixBool *out_set_of_large_masks,
			&set_of_small_index_maps,//CKvDepot_of_MatrixInt *out_set_of_small_index_maps,
			&set_of_small_masks);//CKvDepot_of_MatrixBool *out_set_of_small_masks);

		depot_of_large_index_maps.ap_Append(false, &set_of_large_index_maps, NULL, NULL);
		depot_of_small_index_maps.ap_Append(false, &set_of_small_index_maps, NULL, NULL);
		depot_of_large_masks.ap_Append(false, &set_of_large_masks, NULL, NULL);
		depot_of_small_masks.ap_Append(false, &set_of_small_masks, NULL, NULL);
	}

	set_of_large_index_maps.in_Initialize();
	set_of_small_index_maps.in_Initialize();
	set_of_large_masks.in_Initialize();
	set_of_small_masks.in_Initialize();

	depot_of_large_index_maps.ex_Export(&set_of_voxel_map_idx);
	depot_of_large_masks.ex_Export(&set_of_voxel_map_idx_mask);
	gpvm_Generate_Packing_Voxel_Map_for_Video(
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_points,
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
		zz_block_size,//int in_block_size,
		&position_of_segmented_voxel_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		&voxel_map[0],//CKvMatrixUcharRgb *out_voxel_map,
		&voxel_map_idx[0],//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		&mask[0],//CKvMatrixBool *out_mask_or_NULL,
		&Max_mask);//CKvMatrixBool *out_Max_mask_or_NULL);

	depot_of_small_index_maps.ex_Export(&set_of_voxel_map_idx);
	depot_of_small_masks.ex_Export(&set_of_voxel_map_idx_mask);
	Generate_Prediction_Image(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		&voxel_map[0],//CKvMatrixUcharRgb *in_voxel_map,
		&mask[0],//CKvMatrixBool *in_voxel_mask,
		&voxel_map_idx[0],//CKvMatrixInt  *in_voxel_idx_map,
		&position_of_segmented_voxel_map,//CKvMatrixInt *in_position_of_blob,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_of_prediction_map,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_of_prediction_mask,
		&voxel_map[1],//CKvMatrixUcharRgb *out_prediction_voxel_map,
		out_pt_list_for_secong_img_N_by_3);//CKvMatrixInt *out_prediction_list_and_position_N_by_3)


	printf("[Done] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));

	zz_vcl_ii.interpolate_background(
		out_voxel_maps->gpe(0),
		out_voxel_map_masks->gpe(0),
		out_voxel_maps->gpe(0));

	zz_vcl_ii.interpolate_background(
		out_voxel_maps->gpe(1),
		out_voxel_map_masks->gpe(0),
		out_voxel_maps->gpe(1));

	printf("[Interpolate] : Total Time : %lf\n", sw.get_Get_Elapsed_Time(0));
}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::Partitioning_Voxels_And_Generate_Voxel_Maps(
	VCL_DoCube_X_Color *in_docube,
	int in_plane_mode_sequence,
	int in_alignment_mode,
	CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
	CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)
/**********************************************************************************/
{
	std::vector<Voxel_Slice_Scanned_Data> voxel_sequence;

	gvsi_Get_Voxel_Sequence(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//int &in_plane_mode_sequence,
		voxel_sequence);//std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence)

	//gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction_and_Closing(
	//	in_docube,//VCL_DoCube_X_Color *in_docube,
	//	in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
	//	voxel_sequence);//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence)

	gvmv_Generate_Voxel_Maps(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		voxel_sequence,//std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
		in_alignment_mode,//int in_alignment_mode,
		out_set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map,
		out_set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_mask)
}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::Extract_Voxel_Patch_Maps(
	CKvSet_of_MatrixInt *in_set_of_voxel_maps,
	CKvSet_of_MatrixBool *in_set_of_voxel_map_masks,
	std::vector<bool> &in_voxel_flag,
	int in_closing_size,
	CKvDepot_of_MatrixInt *out_set_of_large_index_maps,
	CKvDepot_of_MatrixBool *out_set_of_large_masks,
	CKvDepot_of_MatrixInt *out_set_of_small_index_maps,
	CKvDepot_of_MatrixBool *out_set_of_small_masks)
/**********************************************************************************/
{
	CKvSdkimRunCode src;
	CKvXrunsetShort xrunshort, obj_blob;
	LCKvUtility_for_Xrunset lu_xrun;
	CKvDepot_of_MatrixInt depot_of_large_index_maps, depot_of_small_index_maps;
	CKvDepot_of_MatrixBool depot_of_large_masks, depot_of_small_masks;
	int num_maps, num_obj, hh, ww, j, i, k;

	CKvMatrixInt  *p_voxel_maps  = in_set_of_voxel_maps->vp();
	CKvMatrixBool *p_voxel_masks = in_set_of_voxel_map_masks->vp();

	num_maps = in_set_of_voxel_maps->vs();

	for (k = 0; k < num_maps; k++)
	{
		int **pp_voxel_maps = p_voxel_maps[k].mps(ww, hh);
		bool **pp_voxel_masks = p_voxel_masks[k].mp();

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (pp_voxel_maps[j][i] != -1)
				{
					if (in_voxel_flag[pp_voxel_maps[j][i]] == false)
					{
						in_voxel_flag[pp_voxel_maps[j][i]] = true;
					}
					else
					{
						pp_voxel_maps[j][i]  = -1;
						pp_voxel_masks[j][i] = false;
					}
				}
			}
		}
	}

	//printf("[1]\n");
	out_set_of_large_index_maps->in_Initialize();
	out_set_of_large_masks->in_Initialize();
	out_set_of_small_index_maps->in_Initialize();
	out_set_of_small_masks->in_Initialize();
	for (k = 0; k < num_maps; k++)
	{
		if (p_voxel_masks[k].mw() == 1)
		{
			out_set_of_small_index_maps->ap_Append(false, &p_voxel_maps[k], NULL);
			out_set_of_small_masks->ap_Append(false, &p_voxel_masks[k], NULL);
			continue;
		}

		xrunshort.im_Import(p_voxel_masks[k]);
		//lu_xrun.clo_Closing(xrunshort, in_closing_size, in_closing_size, xrunshort);
		//src.im_Import(xrunshort, true);
		
		//Get_Region_Index_and_Mask(
		//	&src,//CKvSdkimRunCode *in_mask,
		//	&p_voxel_maps[k],//CKvMatrixInt *in_index_map,
		//	&depot_of_large_index_maps,//CKvDepot_of_MatrixInt *out_depot_of_large_index_maps,
		//	&depot_of_large_masks,//CKvDepot_of_MatrixBool *out_depot_of_large_masks,
		//	&depot_of_small_index_maps,//CKvDepot_of_MatrixInt *out_depot_of_small_index_maps,
		//	&depot_of_small_masks);//CKvDepot_of_MatrixBool *out_depot_of_small_masks);

		Get_Region_Index_and_Mask(
			&xrunshort,//CKvXrunsetShort *in_mask,
			&p_voxel_maps[k],//CKvMatrixInt *in_index_map,
			in_closing_size,//int in_closing_size,
			&depot_of_large_index_maps,//CKvDepot_of_MatrixInt *out_depot_of_large_index_maps,
			&depot_of_large_masks,//CKvDepot_of_MatrixBool *out_depot_of_large_masks,
			&depot_of_small_index_maps,//CKvDepot_of_MatrixInt *out_depot_of_small_index_maps,
			&depot_of_small_masks);//CKvDepot_of_MatrixBool *out_depot_of_small_masks);

		if (depot_of_large_index_maps.ne_Number_of_Elements() != 0)
		{
			out_set_of_large_index_maps->ap_Append(false, &depot_of_large_index_maps, NULL, NULL);
			out_set_of_large_masks->ap_Append(false, &depot_of_large_masks, NULL, NULL);
		}
		
		if (depot_of_small_index_maps.ne_Number_of_Elements() != 0)
		{
			out_set_of_small_index_maps->ap_Append(false, &depot_of_small_index_maps, NULL, NULL);
			out_set_of_small_masks->ap_Append(false, &depot_of_small_masks, NULL, NULL);
		}
	}
}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::Get_Region_Index_and_Mask(
	CKvSdkimRunCode *in_mask,
	CKvMatrixInt *in_index_map,
	CKvDepot_of_MatrixInt *out_depot_of_large_index_maps,
	CKvDepot_of_MatrixBool *out_depot_of_large_masks,
	CKvDepot_of_MatrixInt *out_depot_of_small_index_maps,
	CKvDepot_of_MatrixBool *out_depot_of_small_masks)
/**********************************************************************************/
{
	CKvMatrixInt sub_index_map; CKvMatrixBool sub_mask;
	CKvXrunsetShort obj_blob;
	CKvSet_of_Pixel set_of_pixel; CKvPixel *p_pixel;
	CKvRect rect;
	int minx, maxx, miny, maxy, ww, hh;
	int j, i, num_pixel, num_obj;
	int **p_index;
	bool dummy;

	num_obj = in_mask->no_Number_of_Objects();
	p_index = in_index_map->mp();
	out_depot_of_large_index_maps->in_Initialize();
	out_depot_of_large_masks->in_Initialize();
	out_depot_of_small_index_maps->in_Initialize();
	out_depot_of_small_masks->in_Initialize();
	for (j = 0; j < num_obj; j++)
	{
		in_mask->exo_EXtract_an_Object(j, obj_blob, NULL);
		obj_blob.gsp_Get_Set_of_Pixels(set_of_pixel);

		obj_blob.gcr_Get_Circumscribed_Rectangle(dummy, rect);
		rect.g_Get(minx, miny, maxx, maxy);

		ww = maxx - minx + 1;
		hh = maxy - miny + 1;

		int **p_sub_index = sub_index_map.c_Create(hh, ww, -1);
		bool **p_sub_mask = sub_mask.c_Create(hh, ww, false);

		p_pixel = set_of_pixel.vps(num_pixel);
		for (i = 0; i < num_pixel; i++)
		{
			p_sub_index[p_pixel[i].y - miny][p_pixel[i].x - minx] = p_index[p_pixel[i].y][p_pixel[i].x];
			p_sub_mask[p_pixel[i].y - miny][p_pixel[i].x - minx] = true;
		}

		if (set_of_pixel.vs() < zz_threshold_cluster)
		{
			out_depot_of_small_index_maps->ap_Append(false, &sub_index_map, NULL);
			out_depot_of_small_masks->ap_Append(false, &sub_mask, NULL);
		}
		else
		{
			out_depot_of_large_index_maps->ap_Append(false, &sub_index_map, NULL);
			out_depot_of_large_masks->ap_Append(false, &sub_mask, NULL);
		}
	}


}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::Get_Region_Index_and_Mask(
	CKvXrunsetShort *in_mask,
	CKvMatrixInt *in_index_map,
	int in_closing_size,
	CKvDepot_of_MatrixInt *out_depot_of_large_index_maps,
	CKvDepot_of_MatrixBool *out_depot_of_large_masks,
	CKvDepot_of_MatrixInt *out_depot_of_small_index_maps,
	CKvDepot_of_MatrixBool *out_depot_of_small_masks)
/**********************************************************************************/
{
	CKvSdkimRunCode src, original_src;
	CKvXrunsetShort xrunshort;
	LCKvUtility_for_Xrunset lu_xrun;
	CKvMatrixInt sub_index_map; CKvMatrixBool sub_mask, original_pattern;
	CKvXrunsetShort obj_blob;
	CKvSet_of_Pixel set_of_pixel; CKvPixel *p_pixel;
	CKvRect rect;
	int minx, maxx, miny, maxy, ww, hh;
	int j, i, num_pixel, num_obj;
	int **p_index;
	bool dummy;

	//src.im_Import(*in_mask, true);
	lu_xrun.clo_Closing(*in_mask, in_closing_size, in_closing_size, xrunshort);
	src.im_Import(xrunshort, true);
	//original_src.im_Import(*in_mask, true);

	in_mask->ex_Export(true, false, original_pattern);
	bool **p_op = original_pattern.mp();

	num_obj = src.no_Number_of_Objects();
	p_index = in_index_map->mp();
	out_depot_of_large_index_maps->in_Initialize();
	out_depot_of_large_masks->in_Initialize();
	out_depot_of_small_index_maps->in_Initialize();
	out_depot_of_small_masks->in_Initialize();
	for (j = 0; j < num_obj; j++)
	{
		src.exo_EXtract_an_Object(j, obj_blob, NULL);
		obj_blob.gsp_Get_Set_of_Pixels(set_of_pixel);

		obj_blob.gcr_Get_Circumscribed_Rectangle(dummy, rect);
		rect.g_Get(minx, miny, maxx, maxy);

		ww = maxx - minx + 1;
		hh = maxy - miny + 1;

		int **p_sub_index = sub_index_map.c_Create(hh, ww, -1);
		bool **p_sub_mask = sub_mask.c_Create(hh, ww, false);

		p_pixel = set_of_pixel.vps(num_pixel);
		for (i = 0; i < num_pixel; i++)
		{
			p_sub_index[p_pixel[i].y - miny][p_pixel[i].x - minx] = p_index[p_pixel[i].y][p_pixel[i].x];

			if (p_op[p_pixel[i].y][p_pixel[i].x] == true)
			{
				p_sub_mask[p_pixel[i].y - miny][p_pixel[i].x - minx] = true;
			}
		}

		if (set_of_pixel.vs() < zz_threshold_cluster)
		{
			out_depot_of_small_index_maps->ap_Append(false, &sub_index_map, NULL);
			out_depot_of_small_masks->ap_Append(false, &sub_mask, NULL);
		}
		else
		{
			out_depot_of_large_index_maps->ap_Append(false, &sub_index_map, NULL);
			out_depot_of_large_masks->ap_Append(false, &sub_mask, NULL);
		}
	}


}
/**********************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::gpvm_Generate_Packing_Voxel_Map_for_Video(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvDepot_of_Point3Df *in_points,
	CKvDepot_of_RgbaF *in_colors_or_NULL,
	int in_block_size,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixInt *out_voxel_map_idx_or_NULL,
	CKvMatrixBool *out_mask_or_NULL,
	CKvMatrixBool *out_Max_mask_or_NULL)
//********************************************************************************
{
	CKvMatrixUcharRgb tmp_blob;
	CKvMatrixInt tmp_idx_map, idx_map;
	CKvMatrixBool mask, Max_mask;
	CKvSet_of_RgbaF set_colors; CKvRgbaF *p_colors;
	CKvSet_of_Point3Df set_of_points;
	int num_blob, dum, ww, hh, j, i, k;
	int **p_idx, **p_psvm = NULL;
	unsigned char **pr, **pg, **pb;
	std::vector<std::vector<int>> size_vec;

	size_vec = std::vector<std::vector<int>>(in_set_voxel_maps->vs(), std::vector<int>(2));
	for (k = 0; k < in_set_voxel_maps->vs(); k++)
	{
		size_vec[k][0] = in_set_voxel_maps->gpe_Get_Pointer_of_Element(k)->mw();
		size_vec[k][1] = in_set_voxel_maps->gpe_Get_Pointer_of_Element(k)->mh();
	}

	printf("[Resize_Voxel_Blobs]\n");
	Resize_Voxel_Blobs(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
		in_set_masks,//CKvSet_of_MatrixBool *io_set_voxel_masks,
		in_block_size);//int in_block_size)

	printf("[pvm_Pack_Voxel_Map_With_Scanning_Axis]\n");
	pvm_Pack_Voxel_Map_With_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_block_size,//int in_block_size,
		false,//bool in_image_fixed,
		true,//bool in_pack_flexible,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxe_map,
		&idx_map,//CKvMatrixInt *out_voxel_map,
		&mask);//CKvMatrixBool *out_mask);

	//pvm_Pack_Voxel_Map_With_Scanning_Axis(
	//	in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
	//	in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
	//	in_points,//CKvDepot_of_Point3Df *in_points,
	//	out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
	//	&idx_map,//CKvMatrixInt *out_voxel_map,
	//	&mask,//CKvMatrixBool *out_mask,
	//	&Max_mask);//CKvMatrixBool *out_Max_mask);

	if (in_colors_or_NULL != NULL)
	{
		p_idx = idx_map.mps(ww, hh);
		out_voxel_map->c_Create(hh, ww,
			Kv_Rgb((unsigned char)0, (unsigned char)0, (unsigned char)0));
		pr = out_voxel_map->mp(pg, pb);

		in_colors_or_NULL->e_Export(&set_colors);
		p_colors = set_colors.vp();

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_idx[j][i] != -1)
				{
					pr[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].r));
					pg[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].g));
					pb[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].b));
				}

			}
		}

		p_psvm = out_position_of_segmented_voxe_map->mps(dum, num_blob);
		for (k = 0; k < num_blob; k++)
		{
			p_psvm[k][2] = size_vec[k][0];
			p_psvm[k][3] = size_vec[k][1];
		}

	}

	if (out_voxel_map_idx_or_NULL != NULL)
	{
		out_voxel_map_idx_or_NULL->cp_Copy(&idx_map);
	}

	if (out_mask_or_NULL != NULL)
	{
		out_mask_or_NULL->cp_Copy(&mask);
	}

	if (out_Max_mask_or_NULL != NULL)
	{
		Max_mask.c_Create(out_voxel_map->mh(), out_voxel_map->mw(), false);
		for (k = 0; k < num_blob; k++)
		{
			mask.c_Create(size_vec[k][1], size_vec[k][0], true);
			Max_mask.sb_Set_Block(p_psvm[k][0], p_psvm[k][1], &mask);
		}
		out_Max_mask_or_NULL->cp_Copy(&Max_mask);
	}

}
//********************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::Modify_Texture_Atlases(
	CKvDepot_of_RgbaF *in_colors,
	CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
	CKvSet_of_MatrixInt *in_set_of_voxel_map_idx)
//********************************************************************************
{
	// Set (0,0,0) : If voxel colors are already occupied on other texture atlases. 
	std::vector<bool> flags;
	int num_img = io_set_of_voxel_maps->vs();
	int k, j,i, ww, hh, num_voxel;
	unsigned char **pr, **pg, **pb;
	int **p_idx;

	num_voxel = in_colors->ne_Number_of_Elements();
	flags = std::vector<bool>(num_voxel, false);
	for (k = 0; k < num_img; k++)
	{
		io_set_of_voxel_maps->vp()[k].ms(ww, hh);
		pr    = io_set_of_voxel_maps->vp()[k].mp(pg, pb);
		p_idx = in_set_of_voxel_map_idx->vp()[k].mp();

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_idx[j][i] != -1)
				{
					if (flags[p_idx[j][i]] == true)
					{
						pr[j][i] = pg[j][i] = pb[j][i] = 0;
					}
					else
					{
						flags[p_idx[j][i]] = true;
					}
				}
			}
		}
	}



}
//********************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::Modify_Texture_Atlases(
	CKvDepot_of_RgbaF *in_colors,
	CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
	CKvSet_of_MatrixBool *io_set_of_voxel_map_masks,
	CKvSet_of_MatrixInt *in_set_of_voxel_map_idx)
//********************************************************************************
{
	// Set (0,0,0) : If voxel colors are already occupied on other texture atlases. 
	std::vector<bool> flags;
	int num_img = io_set_of_voxel_maps->vs();
	int k, j, i, ww, hh, num_voxel;
	unsigned char **pr, **pg, **pb;
	int **p_idx; bool **p_mask;

	num_voxel = in_colors->ne_Number_of_Elements();
	flags = std::vector<bool>(num_voxel, false);
	for (k = 0; k < num_img; k++)
	{
		io_set_of_voxel_maps->vp()[k].ms(ww, hh);
		pr     = io_set_of_voxel_maps->vp()[k].mp(pg, pb);
		p_idx  = in_set_of_voxel_map_idx->vp()[k].mp();
		p_mask = io_set_of_voxel_map_masks->vp()[k].mp();

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_idx[j][i] != -1)
				{
					if (flags[p_idx[j][i]] == true)
					{
						pr[j][i] = pg[j][i] = pb[j][i] = 0;
						p_mask[j][i] = false;
					}
					else
					{
						flags[p_idx[j][i]] = true;
					}
				}
			}
		}
	}

}
//********************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::Generate_Error_Maps(
	CKvDepot_of_Point3Df *in_points,
	CKvDepot_of_RgbaF *in_colors,
	CKvSet_of_MatrixUcharRgb *io_set_of_voxel_maps,
	CKvSet_of_MatrixBool *io_set_of_voxel_map_masks,
	CKvSet_of_MatrixInt *in_set_of_voxel_map_idx,
	int in_filter_size)
//********************************************************************************
{
	// Set (0,0,0) : If voxel colors are already occupied on other texture atlases. 
	CKvMatrixIntRgb tmp_error_map;
	CKvSet_of_RgbaF set_rgbaf; CKvSet_of_Point3Df set_point3df;
	std::vector<bool> flags;
	int num_img = io_set_of_voxel_maps->vs();
	int k, j, i, m, n, ww, hh, num_voxel;
	unsigned char **pr, **pg, **pb;
	double mean_r, mean_g, mean_b;
	double error_r, error_g, error_b;
	int count;
	int **p_idx, **prI, **pgI, **pbI;;
	bool **p_mask;

	num_voxel = in_colors->ne_Number_of_Elements();
	flags = std::vector<bool>(num_voxel, false);

	in_colors->e_Export(&set_rgbaf);
	CKvRgbaF *p_rgbaf = set_rgbaf.vp();

	in_points->e_Export(&set_point3df);
	CKvPoint3Df *p_point3df = set_point3df.vp();


	for (k = 0; k < num_img; k++)
	{
		io_set_of_voxel_maps->vp()[k].ms(ww, hh);
		pr = io_set_of_voxel_maps->vp()[k].mp(pg, pb);
		p_idx = in_set_of_voxel_map_idx->vp()[k].mp();
		p_mask = io_set_of_voxel_map_masks->vp()[k].mp();

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_idx[j][i] != -1)
				{
					if (flags[p_idx[j][i]] == true)
					{
						pr[j][i] = pg[j][i] = pb[j][i] = 0;
						p_mask[j][i] = false;
					}
					else
					{
						flags[p_idx[j][i]] = true;
					}
				}
			}
		}
	}

	/********************* Find Errors ********************/
	int half = in_filter_size / 2; double min_error, max_error;
	double weight, weight_sum=0.0;
	for (k = 1; k < num_img; k++)
	{
		io_set_of_voxel_maps->vp()[k].ms(ww, hh);
		pr = io_set_of_voxel_maps->vp()[k].mp(pg, pb);
		p_idx = in_set_of_voxel_map_idx->vp()[k].mp();
		p_mask = io_set_of_voxel_map_masks->vp()[k].mp();

		tmp_error_map.c_Create(hh, ww, Kv_RgbI(0, 0, 0));
		prI = tmp_error_map.mp(pgI, pbI);

		min_error = DBL_MAX;
		max_error = DBL_MIN;


		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_mask[j][i] == true)
				{
					int miny = (j - half) < 0      ? 0    : j - half;
					int maxy = (j + half) > hh-1   ? hh-1 : j + half;
					int minx = (i - half) < 0      ? 0    : i - half;
					int maxx = (i + half) > ww - 1 ? ww-1 : i + half;

					mean_r = mean_g = mean_b = weight_sum = 0.0;
					count = 0;
					for (m = miny; m <= maxy; m++)
					{
						for (n = minx; n <= maxx; n++)
						{
							if ((p_idx[m][n] != -1) && (m!=j) && (n!=i) && (p_mask[m][n] == false))
							{
								weight = 1.0/(double)p_point3df[p_idx[j][i]].d_Distance(p_point3df[p_idx[m][n]]);
								weight_sum += weight;

								mean_r += (weight*(double)(p_rgbaf[p_idx[m][n]].r));
								mean_g += (weight*(double)(p_rgbaf[p_idx[m][n]].g));
								mean_b += (weight*(double)(p_rgbaf[p_idx[m][n]].b));
								count++;

							}
						}
					}

					if (count != 0)
					{
						mean_r = (double)mean_r / weight_sum;
						mean_g = (double)mean_g / weight_sum;
						mean_b = (double)mean_b / weight_sum;

						error_r = (double)(p_rgbaf[p_idx[j][i]].r) - mean_r;
						error_g = (double)(p_rgbaf[p_idx[j][i]].g) - mean_g;
						error_b = (double)(p_rgbaf[p_idx[j][i]].b) - mean_b;

						if ((abs(error_r*255.0) > 120) || (abs(error_g*255.0) > 120) || (abs(error_b*255.0) > 120))
						{
							pr[j][i] = 255;
							pg[j][i] = 0;
							pb[j][i] = 0;
						}

						pr[j][i] = (unsigned char)(128.0 + error_r*255.0);
						pg[j][i] = (unsigned char)(128.0 + error_g*255.0);
						pb[j][i] = (unsigned char)(128.0 + error_b*255.0);

						prI[j][i] = error_r*255.0;
						pgI[j][i] = error_g*255.0;
						pbI[j][i] = error_b*255.0;

						if (min_error > error_r) { min_error = error_r; }
						if (min_error > error_g) { min_error = error_g; }
						if (min_error > error_b) { min_error = error_b; }

						if (max_error < error_r) { max_error = error_r; }
						if (max_error < error_g) { max_error = error_g; }
						if (max_error < error_b) { max_error = error_b; }
					}
					else
					{
						pr[j][i] = 0;
						pg[j][i] = 255;
						pb[j][i] = 0;

						prI[j][i] = 0;
						pgI[j][i] = 255;
						pbI[j][i] = 0;
					}
				}
				else
				{
					pr[j][i] = 128;
					pg[j][i] = 128;
					pb[j][i] = 128;

					prI[j][i] = 0;
					pgI[j][i] = 0;
					pbI[j][i] = 0;
				}
			}
		}

		printf("k=%d/3)\n", k);
		printf("MIN Error : %lf\n", min_error*255);
		printf("MAX Error : %lf\n", max_error*255);
	}


}
//********************************************************************************














/**********************************************************************************/
bool VCL_Voxel_Texture_Atlas_3_Direction::gvsi_Get_Voxel_Sequence(
	VCL_DoCube_X_Color *in_docube,
	int &in_plane_mode_sequence,
	std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence)
/**********************************************************************************/
{
	JANG_Xrunset3dShort xrunset3d;
	std::vector<Voxel_Slice_Scanned_Data> tmp;
	int ww, hh, dd, k, num_plane_mode = 1, num_p;

	in_docube->gr_Get_Resolution(ww, hh, dd);
	xrunset3d.jimport(&in_docube->e_Export_Xvectors_Boundary_Point(), dd);

	num_p = in_docube->gsp_Get_Surface_Points_Pointer()->ne_Number_of_Elements();

	CKvStopWatch sw;
	sw.c_Create(1); sw.r_Reset(0);
	int num_scanned = 0;
	out_voxel_sequence = std::vector<Voxel_Slice_Scanned_Data>(ww);

	if (Get_Surface_Voxel_Indices(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		&xrunset3d,//CKvXrunset3dShort *io_docube,
		in_plane_mode_sequence,//int in_plane_mode,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		&out_voxel_sequence))//std::vector<Voxel_Slice_Scanned_Data> *out_voxel_sequence)
	{
		num_scanned++;
	}

	if (num_scanned != num_plane_mode)
	{
		out_voxel_sequence.erase(out_voxel_sequence.begin() + num_scanned, out_voxel_sequence.end());
	}

	printf("		[Get Scanned Voxel Colors] -> %lf\n", sw.get_Get_Elapsed_Time(0));


	return true;
}
/**********************************************************************************/
bool VCL_Voxel_Texture_Atlas_3_Direction::gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction_and_Closing(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence)
//************************************************************************
{
	LCKvUtility_for_Xrunset3d lux3d;
	JANG_Xrunset3dShort xrunset3d;
	std::vector<Voxel_Slice_Scanned_Data> tmp;
	int ww, hh, dd, k, num_plane_mode, num_p;

	in_docube->gr_Get_Resolution(ww, hh, dd);
	xrunset3d.jimport(&in_docube->e_Export_Xvectors_Boundary_Point(), dd);

	num_p = in_docube->gsp_Get_Surface_Points_Pointer()->ne_Number_of_Elements();
	num_plane_mode = in_plane_mode_sequence.size();
	out_voxel_sequence = std::vector<std::vector< Voxel_Slice_Scanned_Data>>(num_plane_mode);

	CKvStopWatch sw;
	sw.c_Create(1); sw.r_Reset(0);
	int num_scanned = 0;
	for (k = 0; k < num_plane_mode; k++)
	{
		out_voxel_sequence[k] = std::vector<Voxel_Slice_Scanned_Data>(ww);

		if (gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction_Fast3(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			&xrunset3d,//CKvXrunset3dShort *io_docube,
			in_plane_mode_sequence[k],//int in_plane_mode,
			ww,//int &in_ww,
			hh,//int &in_hh,
			dd,//int &in_dd,
			&out_voxel_sequence[k]))//std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence)
		{
			num_scanned++;
			lux3d.clo_Closing(xrunset3d, 1, 1, 1, xrunset3d);
		}
	}

	if (num_scanned != num_plane_mode)
	{
		out_voxel_sequence.erase(out_voxel_sequence.begin() + num_scanned, out_voxel_sequence.end());
	}

	printf("		[Get Scanned Voxel Colors with Closing] -> %lf\n", sw.get_Get_Elapsed_Time(0));

}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::gvmv_Generate_Voxel_Maps(
	VCL_DoCube_X_Color *in_docube,
	std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
	int in_alignment_mode,
	CKvSet_of_MatrixInt *out_set_of_voxel_map,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask)
/**********************************************************************************/
{
	std::vector<std::vector<std::vector<int>>> texture_on_code;
	CKvDepot_of_MatrixInt  set_voxel_maps;
	CKvDepot_of_MatrixBool set_voxel_map_masks;
	CKvSet_of_VectorInt set_circular_shift;
	CKvSet_of_VectorInt set_offsets;
	CKvStopWatch sw;
	sw.c_Create(1);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	sw.r_Reset(0);
	stoc_Segment_Voxel_Slice_Scanned_Data(
		in_voxel_sequence,//std::vector<Voxel_Slice_Peeling_Chain_Code> &in_texture_on_code,
		texture_on_code);//std::vector<std::vector<std::vector<int>>> &out_aligned_texture_on_code)

	printf("Time : %lf\n", sw.get_Get_Elapsed_Time(0));

	printf("[atoc_Align_Texture_on_Code]\n");
	sw.r_Reset(0);

	atoc_Align_Texture_on_Code(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_alignment_mode,//int in_alignment_mode,
		texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		&set_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
		&set_offsets);//CKvSet_of_VectorInt *out_offsets);

	printf("Time : %lf\n", sw.get_Get_Elapsed_Time(0));

	if (s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
		&set_offsets,//CKvSet_of_VectorInt *in_offsets,
		&set_voxel_maps,//CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
		&set_voxel_map_masks))//CKvDepot_of_MatrixBool *out_set_masks)
	{
		if (set_voxel_maps.ne_Number_of_Elements() != 0)
		{
			set_voxel_maps.ex_Export(out_set_of_voxel_map);
			set_voxel_map_masks.ex_Export(out_set_of_voxel_mask);
		}
	}

}
/**********************************************************************************/
bool VCL_Voxel_Texture_Atlas_3_Direction::s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	CKvSet_of_VectorInt *in_offsets,
	CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
	CKvDepot_of_MatrixBool *out_set_masks)
//************************************************************************
{
	CKvMatrixBool *p_masks;
	CKvMatrixInt  *p_voxel_maps;;
	CKvDepot_of_MatrixInt  depot_voxel_maps;
	CKvDepot_of_MatrixBool depot_voxel_masks;
	CKvDepot_of_RgbaF      voxel_colors; CKvSet_of_RgbaF vc; CKvRgbaF *rgbaf;
	CKvVectorInt           *p_offset;
	int *p_po, max_length, tmp, count_depot, count_pv;
	int num_blob, num_toc, num_element, k, l, j;
	int **p_idx, **p_im;
	bool **p_pm;

	p_offset = in_offsets->vps(num_blob); if (num_blob == 0) { return false; }
	depot_voxel_maps.in_Initialize(num_blob);
	depot_voxel_masks.in_Initialize(num_blob);

	p_voxel_maps = depot_voxel_maps.gps_Get_Pointer_of_Space()->vp();
	p_masks = depot_voxel_masks.gps_Get_Pointer_of_Space()->vp();
	count_depot = voxel_colors.ne_Number_of_Elements();

	in_docube->gvc_Get_Voxel_Color().e_Export(&vc);
	rgbaf = vc.vp();

	count_depot = count_pv = 0;
	int num_prediction_voxel = 0;
	for (k = 0; k < num_blob; k++)
	{
		max_length = -1;
		p_po = p_offset[k].vps(num_toc);
		for (l = 0; l < num_toc; l++)
		{
			tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
			if (max_length < tmp)
			{
				max_length = tmp;
			}
		}
		p_voxel_maps[count_depot].c_Create(num_toc, max_length, (int)-1);
		p_masks[count_depot].c_Create(num_toc, max_length, false);

		p_idx = p_voxel_maps[count_depot].mp();
		p_pm = p_masks[count_depot].mp();
		for (l = 0; l < num_toc; l++)
		{
			num_element = in_segmented_texture_on_code[k][l].size();

			for (j = 0; j < num_element; j++)
			{
				p_idx[l][j + p_po[l]] = (int)in_segmented_texture_on_code[k][l][j];
				if ((int)in_segmented_texture_on_code[k][l][j] != -1)
				{
					p_pm[l][j + p_po[l]] = true;
				}

			}
		}
		count_depot++;
	}

	if (count_depot != 0)
	{
		depot_voxel_maps.cne_Change_Number_of_Elements(count_depot);
		depot_voxel_masks.cne_Change_Number_of_Elements(count_depot);

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}
	else
	{
		depot_voxel_maps.in_Initialize();
		depot_voxel_masks.in_Initialize();

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}


	return true;
}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
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
	CKvMatrixBool *out_mask_or_NULL)
//************************************************************************
{
	CKvSet_of_MatrixBool tmp_set_masks;
	CKvMatrixInt idx_map; CKvMatrixBool mask;
	CKvSet_of_RgbaF set_colors; CKvRgbaF *p_colors;
	int ww, hh, j, i; unsigned char **pr, **pg, **pb;
	int **p_idx; bool **p_mask;
	bool tmp_pack_flexible;

	if (in_line_ei_mode_on_true_off_false)
	{
		tmp_pack_flexible = false;
	}
	else
	{
		tmp_pack_flexible = in_pack_flexible;
	}

	Resize_Voxel_Blobs(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
		in_set_masks,//CKvSet_of_MatrixBool *io_set_voxel_masks,
		in_block_size);//int in_block_size)

	pvm_Pack_Voxel_Map_With_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_block_size,//int in_block_size,
		true,//bool in_image_fixed,
		tmp_pack_flexible,//bool in_pack_flexible,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxe_map,
		&idx_map,//CKvMatrixInt *out_voxel_map,
		&mask);//CKvMatrixBool *out_mask);

	if (in_colors_or_NULL != NULL)
	{
		p_idx = idx_map.mps(ww, hh);
		out_voxel_map->c_Create(hh, ww,
			Kv_Rgb((unsigned char)0, (unsigned char)0, (unsigned char)0));
		pr = out_voxel_map->mp(pg, pb);

		in_colors_or_NULL->e_Export(&set_colors);
		p_colors = set_colors.vp();

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_idx[j][i] != -1)
				{
					pr[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].r));
					pg[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].g));
					pb[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].b));
				}

			}
		}

		//if (in_line_ei_mode_on_true_off_false)
		//{
		//	CKvMatrixUcharRgb tmp_blob;
		//	CKvMatrixInt tmp_idx_map;
		//	int num_blob, dum, k;
		//	int **p_psvm = out_position_of_segmented_voxe_map->mps(dum, num_blob);
		//	for (k = 0; k < num_blob; k++)
		//	{
		//		out_voxel_map->gb_Get_Block(
		//			p_psvm[k][0],
		//			p_psvm[k][1],
		//			p_psvm[k][2],
		//			p_psvm[k][3],
		//			&tmp_blob);
		//		EI_DCT_Line_Forward_on_Spatial_Domain(
		//			&tmp_blob,//CKvMatrixInt *io_img,
		//			in_set_masks->gpe(k));//CKvMatrixBool *in_mask)
		//		out_voxel_map->sb_Set_Block(
		//			p_psvm[k][0],
		//			p_psvm[k][1],
		//			&tmp_blob);
		//		idx_map.gb_Get_Block(
		//			p_psvm[k][0],
		//			p_psvm[k][1],
		//			p_psvm[k][2],
		//			p_psvm[k][3],
		//			&tmp_idx_map);
		//		EI_DCT_Line_Forward_on_Spatial_Domain(
		//			&tmp_idx_map,//CKvMatrixInt *io_img,
		//			in_set_masks->gpe(k));//CKvMatrixBool *in_mask)
		//		idx_map.sb_Set_Block(
		//			p_psvm[k][0],
		//			p_psvm[k][1],
		//			&tmp_idx_map);
		//	}
		//}
	}

	if (out_voxel_map_idx_or_NULL != NULL)
	{
		p_idx = idx_map.mps(ww, hh);
		p_mask = mask.mp();
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_mask[j][i] == false)
				{
					p_idx[j][i] = -1;
				}
			}
		}
		out_voxel_map_idx_or_NULL->cp_Copy(&idx_map);
	}

	if (out_mask_or_NULL != NULL)
	{
		out_mask_or_NULL->cp_Copy(&mask);
	}

}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::pvm_Pack_Voxel_Map_With_Scanning_Axis(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvDepot_of_Point3Df *in_points,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask,
	CKvMatrixBool *out_Max_mask)
//************************************************************************
{
	CKvPoint3Df pt;
	CKvMatrixInt *p_voxel_maps, tmp2;
	CKvMatrixBool *p_mask, tmp, max_mask;
	CKvMatrixBool dummy_max_mask;
	int max_width, height, width, global_height, global_width;
	int img_height, img_width;
	int num_blob, missing_count, yy, k, i;
	int **p1, **p2;
	bool locationFound, **p_patch, **pp_mask;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask = in_set_masks->vp();

	global_height = zz_height;
	global_width  = zz_width;

	out_mask->c_Create(global_height, global_width, false);
	out_Max_mask->c_Create(global_height, global_width, false);
	out_voxel_map->c_Create(global_height, global_width, (int)-1);

	int **p_position_of_blob = out_position_of_segmented_voxel_map->c_Create(num_blob, 4);

	for (k = 0; k < num_blob; k++)
	{
		p_mask[k].ms(width, height);
		if (zz_classic_mode)
		{
			max_mask.cp_Copy(&p_mask[k]);
		}
		else
		{
			max_mask.c_Create(height, width, true);
		}

		// Find Voxel Idx
		for (i = 0; i < p_voxel_maps[k].mw(); i++)
		{
			if (p_voxel_maps[k].mp()[0][i] != -1)
			{
				break;
			}
		}

		in_points->ge_Get_Element(
			p_voxel_maps[k].mp()[0][i],
			pt);
		/////////////////////////////////////

		if (zz_plane_mode_sequence == 0)
		{
			zz_y = pt.x;
		}
		else if (zz_plane_mode_sequence == 1)
		{
			zz_y = pt.y;
		}
		else
		{
			zz_y = pt.z;
		}

		locationFound = false;
		zz_x = missing_count = 0;

		yy = zz_y;
		img_height = yy + 1;

		img_width = global_width;
		while (!locationFound)
		{
			for (int y = yy; y < img_height && !locationFound; ++y)
			{
				for (int x = 0; x < img_width && !locationFound; ++x)
				{
					zz_x = x;
					zz_y = y;
					if (checkFitPatchCanvas(
						out_Max_mask,//CKvMatrixBool *in_mask,
						&max_mask,//CKvMatrixBool *in_patch_mask,
						global_width,//int canvasStrideBlk,
						global_height,//int canvasHeightBlk,
						zz_safeguard))//int safeguard = 0)
					{
						locationFound = true;
					}
				}
			}

			if (!locationFound)
			{
				if (missing_count == 0)
				{
					yy = 0;
					img_height = global_height;
					missing_count++;
				}
				else
				{
					global_width *= 2;

					tmp.cp_Copy(out_mask);
					out_mask->c_Create(global_height, global_width, false);
					out_mask->sb_Set_Block(0, 0, &tmp);

					tmp.cp_Copy(out_Max_mask);
					out_Max_mask->c_Create(global_height, global_width, false);
					out_Max_mask->sb_Set_Block(0, 0, &tmp);

					tmp2.cp_Copy(out_voxel_map);
					out_voxel_map->c_Create(global_height, global_width, (int)-1);
					out_voxel_map->sb_Set_Block(0, 0, &tmp2);
				}
			}
		}

		p_position_of_blob[k][0] = zz_x;
		p_position_of_blob[k][1] = zz_y;
		p_position_of_blob[k][2] = width;
		p_position_of_blob[k][3] = height;

		p_patch = p_mask[k].mp();
		pp_mask = out_mask->mp();
		bool **p_dumy = out_Max_mask->mp();
		bool **p_max = max_mask.mp();
		p1 = out_voxel_map->mp();
		p2 = p_voxel_maps[k].mp();
		for (int v0 = 0; v0 < p_mask[k].mh(); ++v0)
		{
			int v = zz_y + v0;
			for (int u0 = 0; u0 < p_mask[k].mw(); ++u0)
			{
				int u = zz_x + u0;
				if (p_patch[v0][u0] == true && pp_mask[v][u] == false)
				{
					pp_mask[v][u] = true;
					p1[v][u] = p2[v0][u0];
				}

				if (zz_classic_mode)
				{
					p_dumy[v][u] = p_max[v0][u0] || p_dumy[v][u];
				}
				else
				{
					p_dumy[v][u] = true;
				}


			}
		}

	}

	global_height = zz_height;

}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::pvm_Pack_Voxel_Map_With_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_image_fixed,
	bool in_pack_flexible,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvVectorShort table;
	int ww, hh, j, i;

	zz_block_size = in_block_size;

	pvm_Pack_Voxel_Map_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_image_fixed,//bool in_fixed_image,
		in_pack_flexible,//bool in_pack_flexible,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixInt *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	if (in_image_fixed == false)
	{
		bool **p_m = out_mask->mps(ww, hh);
		int min_x, max_x, min_y, max_y;

		min_x = ww; max_x = -1;
		min_y = hh; max_y = -1;
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_m[j][i])
				{
					if (min_x > i) { min_x = i; }
					if (max_x < i) { max_x = i; }
					if (min_y > j) { min_y = j; }
					if (max_y < j) { max_y = j; }
				}
			}
		}

		CKvMatrixInt A; CKvMatrixBool B;
		A.cp_Copy(out_voxel_map); B.cp_Copy(out_mask);
		A.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_voxel_map);
		B.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_mask);
	}

}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::pvm_Pack_Voxel_Map_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	bool in_fixed_image,
	bool in_pack_flexible,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixInt *p_voxel_maps, tmp2;
	CKvMatrixBool *p_mask, tmp, max_mask;
	CKvMatrixBool dummy_max_mask;
	int max_width, height, width, global_height, global_width;
	int num_blob, k;
	int **p1, **p2;
	bool locationFound, **p_patch, **pp_mask;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask = in_set_masks->vp();

	/**********************************************************/
	if (in_fixed_image == false)
	{
		max_width = -1;
		for (k = 0; k < num_blob; k++)
		{
			if (max_width < p_voxel_maps[k].mw())
			{
				max_width = p_voxel_maps[k].mw();
			}
		}
		global_width = max_width + zz_safeguard + 1;
		/*********************************************/
		if (global_width < 100) { global_width = 256; }
		global_height = zz_height;
	}
	else
	{
		global_height = zz_height;
		global_width = zz_width;
	}

	/**********************************************************/
	out_mask->c_Create(global_height, global_width, false);
	dummy_max_mask.c_Create(global_height, global_width, false);
	out_voxel_map->c_Create(global_height, global_width, (int)-1);

	int **p_position_of_blob = out_position_of_segmented_voxel_map->c_Create(num_blob, 4);

	//CKvScreen sc[3];

	for (k = 0; k < num_blob; k++)
	{
		p_mask[k].ms(width, height);

		if (in_pack_flexible == false)
		{
			max_mask.c_Create(height, width, true);
		}
		else
		{
			Set_New_Masks(
				&p_mask[k],//CKvMatrixBool *in_mask,
				&max_mask);//CKvMatrixBool *out_mask)
		}

		locationFound = false;
		zz_x = zz_y = 0;
		while (!locationFound)
		{
			for (int y = 0; y < global_height && !locationFound; ++y)
			{
				for (int x = 0; x < global_width && !locationFound; ++x)
				{
					zz_x = x;
					zz_y = y;
					if (checkFitPatchCanvas(
						&dummy_max_mask,//CKvMatrixBool *in_mask,
						&max_mask,//CKvMatrixBool *in_patch_mask,
						global_width,//int canvasStrideBlk,
						global_height,//int canvasHeightBlk,
						zz_safeguard))//int safeguard = 0)
					{
						locationFound = true;
					}
				}
			}

			if (!locationFound)
			{
				global_height *= 2;

				tmp.cp_Copy(out_mask);
				out_mask->c_Create(global_height, global_width, false);
				out_mask->sb_Set_Block(0, 0, &tmp);

				tmp.cp_Copy(&dummy_max_mask);
				dummy_max_mask.c_Create(global_height, global_width, false);
				dummy_max_mask.sb_Set_Block(0, 0, &tmp);

				tmp2.cp_Copy(out_voxel_map);
				out_voxel_map->c_Create(global_height, global_width, (int)-1);
				out_voxel_map->sb_Set_Block(0, 0, &tmp2);

			}
		}

		p_position_of_blob[k][0] = zz_x;
		p_position_of_blob[k][1] = zz_y;
		p_position_of_blob[k][2] = width;
		p_position_of_blob[k][3] = height;

		p_patch = p_mask[k].mp();
		pp_mask = out_mask->mp();
		bool **p_dumy = dummy_max_mask.mp();
		bool **p_max = max_mask.mp();
		p1 = out_voxel_map->mp();
		p2 = p_voxel_maps[k].mp();
		for (int v0 = 0; v0 < p_mask[k].mh(); ++v0)
		{
			int v = zz_y + v0;
			for (int u0 = 0; u0 < p_mask[k].mw(); ++u0)
			{
				int u = zz_x + u0;
				if (p_patch[v0][u0] == true && pp_mask[v][u] == false)
				{
					pp_mask[v][u] = true;
					p1[v][u] = p2[v0][u0];
				}

				if (in_pack_flexible == false)
				{
					p_dumy[v][u] = true;
				}
				else
				{
					p_dumy[v][u] = p_max[v0][u0] || p_dumy[v][u];
				}

			}
		}
		//sc[0].s_d_Display(out_mask);
		//sc[1].s_d_Display(&dummy_max_mask);
		//sc[2].s_d_Display(&p_mask[k]);
		//Kv_Printf("k=%d/%d) zz_x = %d zz_y = %d",k,num_blob, zz_x,zz_y);
	}
}
//************************************************************************
bool VCL_Voxel_Texture_Atlas_3_Direction::checkFitPatchCanvas(
	CKvMatrixBool *in_mask,
	CKvMatrixBool *in_patch_mask,
	int canvasStrideBlk,
	int canvasHeightBlk,
	int safeguard)
//************************************************************************
{
	int pos;
	int deltaY, deltaX;
	int v0_p, u0_p;
	int v0, u0;
	bool *p_mask, **p_pmask;

	in_patch_mask->ms(u0_p, v0_p);
	p_mask = in_mask->vp();
	p_pmask = in_patch_mask->mp();

	for (v0 = 0; v0 < v0_p; ++v0)
	{
		for (u0 = 0; u0 < u0_p; ++u0)
		{
			for (deltaY = -safeguard; deltaY < safeguard + 1; deltaY++)
			{
				for (deltaX = -safeguard; deltaX < safeguard + 1; deltaX++)
				{
					//printf("deltaY = %d delta = %d\r", deltaY, deltaX);
					//To do here;
					pos = patchBlock2CanvasBlock(
						u0 + deltaX,
						v0 + deltaY,
						canvasStrideBlk,
						canvasHeightBlk);

					if (pos < 0)
					{
						return false;
					}
					else if (p_pmask[v0][u0] && p_mask[pos])
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}
//************************************************************************
int VCL_Voxel_Texture_Atlas_3_Direction::patchBlock2CanvasBlock(
	int in_uBlk,
	int in_vBlk,
	int in_mask_width,
	int in_mask_height)
//************************************************************************
{
	int x, y;

	x = in_uBlk + zz_x;
	y = in_vBlk + zz_y;

	if (x < 0) return -1;
	if (y < 0) return -1;
	if (x >= in_mask_width) return -1;
	if (y >= in_mask_height) return -1;

	return (y*in_mask_width + x);
}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::Resize_Voxel_Blobs(
	CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
	CKvSet_of_MatrixBool *io_set_voxel_masks,
	int in_block_size)
//************************************************************************
{
	CKvMatrixInt voxel_map_idx, *p_set_idx;
	CKvMatrixBool voxel_mask, *p_set_mask;
	int num_blob, k;
	int ww, hh, nww, nhh;

	p_set_idx = io_set_voxel_maps_idx->vps(num_blob);
	p_set_mask = io_set_voxel_masks->vp();

	for (k = 0; k < num_blob; k++)
	{
		p_set_idx[k].ms(ww, hh);

		nww = ceil((double)ww / (double)in_block_size)*in_block_size;
		nhh = ceil((double)hh / (double)in_block_size)*in_block_size;

		voxel_map_idx.cp_Copy(&p_set_idx[k]);
		voxel_mask.cp_Copy(&p_set_mask[k]);

		p_set_idx[k].c_Create(nhh, nww, -1);
		p_set_mask[k].c_Create(nhh, nww, false);

		p_set_idx[k].sb_Set_Block(0, 0, &voxel_map_idx);
		p_set_mask[k].sb_Set_Block(0, 0, &voxel_mask);
	}
}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::Set_New_Masks(
	CKvMatrixBool *in_mask,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool block_mask;
	int ww, hh, j, i, k, sz;
	bool **p_mask, *p_block;
	int block_size = zz_block_size;

	p_mask = in_mask->mps(ww, hh);
	out_mask->c_Create(hh, ww, false);
	for (j = 0; j < hh; j += block_size)
	{
		for (i = 0; i < ww; i += block_size)
		{
			in_mask->gb_Get_Block(i, j, zz_block_size, zz_block_size, &block_mask);
			p_block = block_mask.vps(sz);
			for (k = 0; k < sz; k++)
			{
				if (p_block[k] == true)
				{
					block_mask.sm_Set_Matrix(true);
					break;
				}
			}
			out_mask->sb_Set_Block(i, j, &block_mask);
		}
	}

}
//************************************************************************
void VCL_Voxel_Texture_Atlas_3_Direction::Generate_Prediction_Image(
	VCL_DoCube_X_Color *in_docube,
	CKvMatrixUcharRgb *in_voxel_map,
	CKvMatrixBool *in_voxel_mask,
	CKvMatrixInt  *in_voxel_idx_map,
	CKvMatrixInt *in_position_of_blob,
	CKvSet_of_MatrixInt *in_set_of_prediction_map,
	CKvSet_of_MatrixBool *in_set_of_prediction_mask,
	CKvMatrixUcharRgb *out_prediction_voxel_map,
	CKvMatrixInt *out_prediction_list_and_position_N_by_3)
/******************************************************************************/
{
	std::vector<std::vector<int>> voxel_idx_table;
	std::vector<bool> voxel_idx_flag;
	CKvXRmatrixInt xrmat; CKvVectorInt element;
	CKvSet_of_RgbaF set_voxel_colors; CKvRgbaF *p_colors;
	CKvSet_of_Point3Df set_point3d;   CKvPoint3Df *p_point3d;
	CKvMatrixInt *p_p; CKvMatrixBool flag_map;
	int **p_pp, *p_table, num_prediction_map, num_voxel;
	int k, j, i, ww, hh, xx, yy, count = 0;
	bool **p_flag;

	out_prediction_voxel_map->cp_Copy(in_voxel_map);
	out_prediction_voxel_map->ms(ww, hh);
	p_flag = flag_map.c_Create(hh, ww, false);
	p_p = in_set_of_prediction_map->vps(num_prediction_map);

	in_docube->gvc_Get_Voxel_Color().e_Export(&set_voxel_colors);
	p_colors = set_voxel_colors.vps(num_voxel);

	in_docube->gsp_Get_Surface_Points().e_Export(&set_point3d);
	p_point3d = set_point3d.vps(num_voxel);

	xrmat.in_Initialize(num_voxel, 3);
	p_table = element.c_Create(3);

	voxel_idx_table = std::vector<std::vector<int>>(num_voxel, std::vector<int>(2, -1));
	p_pp = in_voxel_idx_map->mps(ww, hh);
#pragma omp parallel
	{
#pragma omp for private(i)
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_pp[j][i] != -1)
				{
					voxel_idx_table[p_pp[j][i]][0] = j; // y
					voxel_idx_table[p_pp[j][i]][1] = i; // x
				}
			}
		}
	}

	CKvSet2d_of_XvectorShort xvec_2d;
	xvec_2d = in_docube->e_Export_Xvectors_Boundary_Point();

	//CKvSet2d_of_XvectorInt index_2d;
	//index_2d = in_docube->grlv_Get_RL_to_Voxel_Index2();

	voxel_idx_flag = std::vector<bool>(num_voxel, false);
	for (k = 0; k < num_prediction_map; k++)
	{
		//printf("k=%d/%d)\n", k, num_prediction_map);
		p_pp = p_p[k].mps(ww, hh);
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_pp[j][i] != -1)
				{
					Find_Closet_Point3(
						&p_colors[p_pp[j][i]],//CKvRgbaF *in_color,
						&p_point3d[p_pp[j][i]],//CKvPoint3Df *in_point,
						p_point3d,//CKvPoint3Df *in_all_points,
						&xvec_2d,//CKvSet2d_of_XvectorShort *in_xvector_2d,
						in_docube,//VCL_DoCube_X_Color *in_docube,
						voxel_idx_table,//std::vector<std::vector<int>> &in_voxel_idx_table,
						voxel_idx_flag,//std::vector<bool> &io_voxel_idx_flag,
						out_prediction_voxel_map,//CKvMatrixUcharRgb *io_prediction_map,
						xx,//int &out_x,
						yy);//int &out_y)


					p_table[0] = p_pp[j][i];
					p_table[1] = xx;
					p_table[2] = yy;

					xrmat.ae_Append_Element(&element);
					count++;
				}
			}
		}
	}

	if (count != 0)
	{
		xrmat.z_cne(count);
		xrmat.ex_Export(out_prediction_list_and_position_N_by_3, NULL);
	}

}
/******************************************************************************/
void VCL_Voxel_Texture_Atlas_3_Direction::Find_Closet_Point3(
	CKvRgbaF *in_color,
	CKvPoint3Df *in_point,
	CKvPoint3Df *in_all_points,
	CKvSet2d_of_XvectorShort *in_xvector_2d,
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<int>> &in_voxel_idx_table,
	std::vector<bool> &io_voxel_idx_flag,
	CKvMatrixUcharRgb *io_prediction_map,
	int &out_x,
	int &out_y)
//************************************************************************
{
	CKvXvectorShort **p_xvec2d;
	float min_dist = FLT_MAX, dist;
	int xx, yy, zz, idx, voxel_idx;
	int block_sz = 1;
	int j, i, k, ww, hh;
	int xo, x1, yo, y1, zo, z1;
	unsigned char **pr, **pg, **pb;

	xx = (int)in_point->x;
	yy = (int)in_point->y;
	zz = (int)in_point->z;

	p_xvec2d = in_xvector_2d->mps(ww, hh);

	while (1)
	{
		xo = (xx - block_sz) < 0 ? 0 : (xx - block_sz);
		yo = (yy - block_sz) < 0 ? 0 : (yy - block_sz);
		zo = (zz - block_sz) < 0 ? 0 : (zz - block_sz);

		x1 = (xx + block_sz) > ww - 1 ? ww - 1 : (xx + block_sz);
		y1 = (yy + block_sz) > hh - 1 ? hh - 1 : (yy + block_sz);
		z1 = (zz + block_sz) > hh - 1 ? hh - 1 : (zz + block_sz);

		min_dist = FLT_MAX;
		for (j = yo; j <= y1; j++)
		{
			for (i = xo; i <= x1; i++)
			{
				for (k = zo; k <= z1; k++)
				{
					idx = in_docube->gvi_Get_Voxel_Index(
						i,//int in_x,
						j,//int in_y,
						k);//int in_z)

					if (idx != -1)
					{
						if ((io_voxel_idx_flag[idx] == false) &&
							(in_voxel_idx_table[idx][0] != -1))
						{
							dist = in_point->d_Distance(in_all_points[idx]);
							if (min_dist > dist)
							{
								min_dist = dist;
								out_x = in_voxel_idx_table[idx][1];
								out_y = in_voxel_idx_table[idx][0];
								voxel_idx = idx;
							}
						}
					}
				}
			}
		}

		if (min_dist != FLT_MAX)
		{
			break;
		}
		else
		{
			block_sz = block_sz * 2;
		}
	}

	pr = io_prediction_map->mp(pg, pb);

	io_voxel_idx_flag[voxel_idx] = true;
	pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
	pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
	pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);
}
//************************************************************************
bool VCL_Voxel_Texture_Atlas_3_Direction::sim_Save_Index_Map(
	CKvString str,
	CKvMatrixInt *in_index_map)
//************************************************************************
{
	FILE *fp = NULL;
	int ww, hh;

	in_index_map->ms(ww, hh);

	fopen_s(&fp, (char *)str, "wb");
	if (fp == NULL)
	{
		return false;
	}

	fwrite(&hh, sizeof(int), 1, fp);
	fwrite(&ww, sizeof(int), 1, fp);

	int *p = in_index_map->vp();

	fwrite(p, sizeof(int), ww*hh, fp);

	fclose(fp);
	return true;
}
//************************************************************************




