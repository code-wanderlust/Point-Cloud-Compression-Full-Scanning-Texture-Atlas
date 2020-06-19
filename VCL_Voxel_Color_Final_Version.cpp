#include "stdafx.h"
#include "VCL_Voxel_Color_Final_Version.h"


/******************************************************************************/
VCL_Voxel_Color_Final_Version::VCL_Voxel_Color_Final_Version()
/******************************************************************************/
{

}
/******************************************************************************/
VCL_Voxel_Color_Final_Version::~VCL_Voxel_Color_Final_Version()
/******************************************************************************/
{

}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Set_Parameters(
	bool in_shape_coding_mode_on_off,
	int in_plane_mode_sequence,
	int in_alignment_mode,
	int in_block_size,
	int in_threshold_for_cluster_size,
	float in_threshold_for_segmentation,
	int in_map_width,
	int in_map_height)
//************************************************************************
{
	sp_Set_Parameter_for_Segmentation(in_threshold_for_segmentation);
	sp_Set_Parameter_Map_Size(in_map_width, in_map_height);

	zz_shape_coding_mode_on_off   = in_shape_coding_mode_on_off;
	zz_alignment_mode             = in_alignment_mode;
	zz_block_size                 = in_block_size;
	zz_threshold_for_cluster_size = in_threshold_for_cluster_size;
	zz_plane_mode_sequence        = in_plane_mode_sequence;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::Save_Parameters(
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
	int in_encoder_type)
//************************************************************************
{
	FILE *fp = NULL;
	int k, sz;

	fopen_s(&fp, (char *)in_filename, "wb");
	if (fp == NULL)
	{
		return false;
	}

	fwrite(&in_block_size, sizeof(int), 1, fp);
	fwrite(&in_threshold_for_cluster_size, sizeof(int), 1, fp);
	sz = in_direction_of_scan.size();
	fwrite(&sz, sizeof(int), 1, fp);
	for (k = 0; k < sz; k++)
	{
		fwrite(&in_direction_of_scan[k], sizeof(int), 1, fp);
	}

	fwrite(&in_threshold_for_segmentation, sizeof(float), 1, fp);
	fwrite(&in_searching_range, sizeof(int), 1, fp);
	fwrite(&in_shape_coding_mode_on_off, sizeof(int), 1, fp);
	fwrite(&in_alignment_mode, sizeof(int), 1, fp);
	fwrite(&in_error_prediction_mode, sizeof(int), 1, fp);
	fwrite(&in_error_threshold, sizeof(int), 1, fp);
	fwrite(&in_encoder_type, sizeof(int), 1, fp);

	fclose(fp);
	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::Load_Parameters(
	CKvString in_filename,
	bool &out_shape_coding_mode_on_off,
	std::vector<int> &out_direction_of_scan,
	int &out_alignment_mode,
	int &out_block_size,
	int &out_threshold_for_cluster_size,
	float &out_threshold_for_segmentation,
	int &out_searching_range,
	int &out_error_prediction_mode,
	int &out_error_threshold,
	int &out_encoder_type)
//************************************************************************
{
	FILE *fp = NULL;
	int k, sz;

	fopen_s(&fp, (char *)in_filename, "rb");
	if (fp == NULL)
	{
		return false;
	}

	fread(&out_block_size, sizeof(int), 1, fp);
	fread(&out_threshold_for_cluster_size, sizeof(int), 1, fp);
	fread(&sz, sizeof(int), 1, fp);
	out_direction_of_scan = std::vector<int>(sz);
	
	for (k = 0; k < sz; k++)
	{
		fread(&out_direction_of_scan[k], sizeof(int), 1, fp);
	}

	fread(&out_threshold_for_segmentation, sizeof(float), 1, fp);
	fread(&out_searching_range, sizeof(int), 1, fp);
	fread(&out_shape_coding_mode_on_off, sizeof(int), 1, fp);
	fread(&out_alignment_mode, sizeof(int), 1, fp);
	fread(&out_error_prediction_mode, sizeof(int), 1, fp);
	fread(&out_error_threshold, sizeof(int), 1, fp);
	fread(&out_encoder_type, sizeof(int), 1, fp);

	fclose(fp);
	return true;
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Generate_Voxel_Map(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	bool in_shape_coding_mode_on_off,
	int in_alignment_mode,
	int in_block_size,
	int in_encoder,
	CKvMatrixUcharRgb *out_voxel_maps,
	CKvMatrixBool *out_voxel_map_mask,
	CKvMatrixInt *out_voxel_idx_map)
/******************************************************************************/
{
	CKvDepot_of_MatrixInt  merge_idx; 
	CKvDepot_of_MatrixBool merge_mask;
	CKvSet_of_MatrixInt    set_of_voxel_map_idx,      set_of_voxel_map_small_blob_idx;
	CKvSet_of_MatrixBool   set_of_voxel_map_idx_mask, set_of_voxel_map_small_blob_idx_mask;
	CKvMatrixInt blob_position;

	Partitioning_Voxels_And_Generate_Voxel_Maps(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		in_alignment_mode,//int in_alignment_mode,
		zz_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)
		&set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
		&set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask)

	merge_idx.ap_Append(true, &set_of_voxel_map_idx, NULL,NULL);
	merge_idx.ap_Append(false, &set_of_voxel_map_small_blob_idx, NULL,NULL);

	merge_mask.ap_Append(true, &set_of_voxel_map_idx_mask, NULL, NULL);
	merge_mask.ap_Append(false, &set_of_voxel_map_small_blob_idx_mask, NULL, NULL);

	merge_idx.ex_Export(&set_of_voxel_map_idx);       merge_idx.in_Initialize();
	merge_mask.ex_Export(&set_of_voxel_map_idx_mask); merge_mask.in_Initialize();

	gpvm_Generate_Packing_Voxel_Map_For_Image_Maximum_Margin(
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
		in_block_size,//int in_block_size,
		in_shape_coding_mode_on_off,//bool in_shape_coding_mode_on_off,
		in_encoder,//char in_types_of_encoder,
		true,//bool in_image_fixed,
		false,//bool in_pack_flexible,
		&blob_position,//CKvMatrixInt *out_positions_voxel_blob,
		out_voxel_maps,//CKvMatrixUcharRgb *out_voxel_map,
		out_voxel_idx_map,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		out_voxel_map_mask);//CKvMatrixBool *out_mask)


}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Generate_Voxel_Map_with_Multiple_Images(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	bool in_shape_coding_mode_on_off,
	int in_alignment_mode,
	int in_block_size,
	int in_encoder,
	CKvSet_of_MatrixUcharRgb *out_voxel_maps,
	CKvMatrixBool *out_voxel_map_mask,
	CKvMatrixInt *out_voxel_idx_map,
	CKvMatrixInt *out_list_for_position_N_by_3_second_image)
/******************************************************************************/
{
	CKvSet_of_MatrixInt    set_of_voxel_map_idx, set_of_voxel_map_small_blob_idx;
	CKvSet_of_MatrixBool   set_of_voxel_map_idx_mask, set_of_voxel_map_small_blob_idx_mask;
	CKvSet_of_MatrixIntRgb set_of_error_map;
	CKvMatrixInt blob_position;

	Partitioning_Voxels_And_Generate_Voxel_Maps(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		in_alignment_mode,//int in_alignment_mode,
		zz_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)
		&set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
		&set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask)

	printf("num blob = %d\n", set_of_voxel_map_idx.vs());
	out_voxel_maps->c_Create(2);

	printf("[gpvm_Generate_Packing_Voxel_Map_For_Image_Maximum_Margin]\n");
	gpvm_Generate_Packing_Voxel_Map_For_Image_Maximum_Margin(
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors,
		in_block_size,//int in_block_size,
		in_shape_coding_mode_on_off,//bool in_shape_coding_mode_on_off,
		in_encoder,//char in_types_of_encoder,
		false,//bool in_image_fixed,
		false,//bool in_pack_flexible,
		&blob_position,//CKvMatrixInt *out_positions_voxel_blob,
		out_voxel_maps->gpe(0),//CKvMatrixUcharRgb *out_voxel_map,
		out_voxel_idx_map,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		out_voxel_map_mask);//CKvMatrixBool *out_mask)

	//CKvScreen sc;
	//sc.s_d_Display(out_voxel_maps->gpe(0));
	//Kv_Printf("!!!");

	printf("[Generate_Prediction_Image] -> %d\n", set_of_voxel_map_small_blob_idx.vs());
	if (set_of_voxel_map_small_blob_idx.vs() != 0)
	{
		Generate_Prediction_Image2(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			out_voxel_maps->gpe(0),//CKvMatrixUcharRgb *in_voxel_map,
			out_voxel_map_mask,//CKvMatrixBool *in_voxel_mask,
			out_voxel_idx_map,//CKvMatrixInt  *in_voxel_idx_map,
			&blob_position,//CKvMatrixInt *in_position_of_blob,
			&set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *in_set_of_prediction_map,
			&set_of_voxel_map_small_blob_idx_mask,//CKvSet_of_MatrixBool *in_set_of_prediction_mask,
			out_voxel_maps->gpe(1),//CKvMatrixUcharRgb *out_prediction_voxel_map,
			out_list_for_position_N_by_3_second_image);//CKvMatrixInt *out_prediction_list_and_position_N_by_3)
	}
	printf("[Finish]\n");

}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Generate_Voxel_Map_with_Error_Prediction(
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
	CKvMatrixInt *out_error_idx_map)
/******************************************************************************/
{
	CKvSet_of_MatrixInt    set_of_voxel_map_idx, set_of_voxel_map_small_blob_idx;
	CKvSet_of_MatrixBool   set_of_voxel_map_idx_mask, set_of_voxel_map_small_blob_idx_mask;
	CKvSet_of_MatrixIntRgb set_of_error_map;

	CKvMatrixInt blob_position; CKvMatrixIntRgb error_map;

	zz_prediction_searching_range = in_prediction_searching_range;
	zz_prediction_mode = in_prediction_mode;
	zz_error_threshold = in_error_threshold;

	Partitioning_Voxels_And_Generate_Voxel_Maps(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		in_alignment_mode,//int in_alignment_mode,
		zz_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)
		&set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
		&set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask)

	printf("# of Large Texture Maps : %d\n", set_of_voxel_map_idx.vs());
	printf("# of Small Texture Maps : %d\n", set_of_voxel_map_small_blob_idx.vs());

	Classify_Voxe_Map_and_Generate_Error_Map(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask,
		&set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *io_error_map_idx,
		&set_of_voxel_map_small_blob_idx_mask,//CKvSet_of_MatrixBool *io_error_map_mask,
		zz_prediction_searching_range,//int in_searching_range,
		zz_prediction_mode,//int in_prediction_mode,
		zz_error_threshold,//int in_error_threshold,
		&set_of_error_map);//CKvSet_of_MatrixIntRgb *out_error_map)

	printf("# of Large Texture Maps : %d\n", set_of_voxel_map_idx.vs());
	printf("# of Small Texture Maps : %d\n", set_of_voxel_map_small_blob_idx.vs());

	//return;

	pvm_Packing_Voxel_Maps(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_map_idx,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_voxel_map_idx_mask,
		&set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *in_set_error_map_idx,
		&set_of_voxel_map_small_blob_idx_mask,//CKvSet_of_MatrixBool *in_set_error_map_mask,
		&set_of_error_map,//CKvSet_of_MatrixIntRgb *in_set_error_map,
		zz_block_size,//int in_block_size,
		in_shape_coding_mode_on_off,//bool in_shape_coding_mode_on_off,
		in_encoder,//char in_types_of_encoder,
		false,//bool in_image_fixed,
		&blob_position,//CKvMatrixInt *out_blob_position,
		out_voxel_maps,//CKvMatrixUcharRgb *out_voxel_map,
		out_voxel_idx_map,//CKvMatrixInt *out_voxel_map_idx,
		out_voxel_map_mask,//CKvMatrixBool *out_voxel_map_mask,
		&error_map,//CKvMatrixIntRgb *out_error_map,
		out_error_idx_map,//CKvMatrixInt *out_error_map_idx,
		out_error_map_mask);//CKvMatrixBool *out_error_map_mask)

	Convert_To_Uchar(&error_map, out_error_maps);
}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Generate_Voxel_Map_with_Hybrid_Coding(
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
	CKvDepot_of_RgbaF *out_voxel_dust_colors)
/******************************************************************************/
{
	CKvSet_of_MatrixInt    set_of_voxel_map_idx, set_of_voxel_map_small_blob_idx;
	CKvSet_of_MatrixBool   set_of_voxel_map_idx_mask, set_of_voxel_map_small_blob_idx_mask;
	CKvMatrixInt blob_position;

	Partitioning_Voxels_And_Generate_Voxel_Maps(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		in_alignment_mode,//int in_alignment_mode,
		zz_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask)
		&set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
		&set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask)

	gpvm_Generate_Packing_Voxel_Map_For_Image_Maximum_Margin(
		&set_of_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
		in_block_size,//int in_block_size,
		in_shape_coding_mode_on_off,//bool in_shape_coding_mode_on_off,
		in_encoder,//char in_types_of_encoder,
		true,//bool in_image_fixed,
		false,//bool in_pack_flexible,
		&blob_position,//CKvMatrixInt *out_positions_voxel_blob,
		out_voxel_maps,//CKvMatrixUcharRgb *out_voxel_map,
		out_voxel_idx_map,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		out_voxel_map_mask);//CKvMatrixBool *out_mask)

	CKvSet_of_Point3Df set_points; CKvSet_of_RgbaF set_colors;
	CKvPoint3Df *p_points; CKvRgbaF *p_rgbaf;
	CKvMatrixInt *p_small_blob;
	int num_blob, num_dust_voxels; int **p_idx; int dum, ww, hh, k, j, i;

	p_small_blob = set_of_voxel_map_small_blob_idx.vps(num_blob);
	in_docube->gsp_Get_Surface_Points().e_Export(&set_points);
	in_docube->gvc_Get_Voxel_Color().e_Export(&set_colors);

	p_points = set_points.vp();
	p_rgbaf = set_colors.vp();

	out_voxel_dust_points->in_Initialize(set_points.vs());
	out_voxel_dust_colors->in_Initialize(set_points.vs());

	num_dust_voxels = 0;
	for (k = 0; k < num_blob; k++)
	{
		p_idx = p_small_blob[k].mps(ww, hh);
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_idx[j][i] != -1)
				{
					out_voxel_dust_points->ap_Append(false, p_points[p_idx[j][i]], dum);
					out_voxel_dust_colors->ap_Append(false, p_rgbaf[p_idx[j][i]], dum);
					num_dust_voxels++;
				}
			}
		}
	}

	out_voxel_dust_points->cne_Change_Number_of_Elements(num_dust_voxels);
	out_voxel_dust_colors->cne_Change_Number_of_Elements(num_dust_voxels);

}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Import_Voxel_Map(
	CKvMatrixUcharRgb *in_voxel_map,
	CKvMatrixBool *in_voxel_map_mask,
	CKvMatrixInt *in_voxel_map_index,
	VCL_DoCube_X_Color *io_docube)
/******************************************************************************/
{
	CKvDepot_of_RgbaF depot_rgbaf;
	CKvVectorBool flags;
	int num_voxel;

	num_voxel = io_docube->gsp_Get_Surface_Points().ne_Number_of_Elements();
	Get_Voxel_Colors_From_Texture_Map(
		in_voxel_map,//CKvMatrixUcharRgb *in_voxel_map,
		in_voxel_map_mask,//CKvMatrixBool *in_voxel_map_mask,
		in_voxel_map_index,//CKvMatrixInt *in_voxel_map_index,
		num_voxel,//int in_number_of_voxels,
		&depot_rgbaf,//CKvDepot_of_RgbaF *out_colors,
		&flags);//CKvVectorBool *out_flag);

	io_docube->i_Import_Color_Vector(&depot_rgbaf);
}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Import_Voxel_Map_with_Multple_Maps(
	CKvMatrixUcharRgb *in_voxel_map_1,
	CKvMatrixUcharRgb *in_voxel_map_2,
	CKvMatrixBool *in_voxel_map_mask,
	CKvMatrixInt *in_voxel_map_index,
	CKvMatrixInt *in_list_for_position,
	VCL_DoCube_X_Color *io_docube)
/******************************************************************************/
{
	CKvDepot_of_RgbaF depot_rgbaf; CKvRgbaF *p_rgbaf;
	CKvSet_of_RgbaF set_rgbaf;
	CKvVectorBool flags;
	unsigned char **pr, **pg, **pb;
	int num_point, num_voxel, j, dim, ww, hh;
	int **p_list;

	num_voxel = io_docube->gsp_Get_Surface_Points().ne_Number_of_Elements();
	Get_Voxel_Colors_From_Texture_Map(
		in_voxel_map_1,//CKvMatrixUcharRgb *in_voxel_map,
		in_voxel_map_mask,//CKvMatrixBool *in_voxel_map_mask,
		in_voxel_map_index,//CKvMatrixInt *in_voxel_map_index,
		num_voxel,//int in_number_of_voxels,
		&depot_rgbaf,//CKvDepot_of_RgbaF *out_colors,
		&flags);//CKvVectorBool *out_flag);

	depot_rgbaf.e_Export(&set_rgbaf);
	p_rgbaf = set_rgbaf.vp();
	pr     = in_voxel_map_2->mp(pg, pb);
	in_voxel_map_2->ms(ww, hh);
	p_list = in_list_for_position->mps(dim, num_point);
	printf("dim=%d num_point=%d\n", dim, num_point);
	for (j = 0; j < num_point; j++)
	{
		//printf("j=%d/%d) %d %d %d\n", j, num_point, p_list[j][0], p_list[j][1], p_list[j][2]);
		//Kv_Printf("!!!!");
		if ((p_list[j][1] < 0) || (p_list[j][1] > ww-1) || (p_list[j][2] < 0) || (p_list[j][2] > hh-1))
		{
			continue;
		}
		p_rgbaf[p_list[j][0]].r = (float)(pr[p_list[j][2]][p_list[j][1]]) / 255.f;
		p_rgbaf[p_list[j][0]].g = (float)(pg[p_list[j][2]][p_list[j][1]]) / 255.f;
		p_rgbaf[p_list[j][0]].b = (float)(pb[p_list[j][2]][p_list[j][1]]) / 255.f;
	}

	depot_rgbaf.im_Import(&set_rgbaf);
	io_docube->i_Import_Color_Vector(&depot_rgbaf);
}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Import_Voxel_Map_With_Error_Prediction(
	CKvMatrixUcharRgb *in_voxel_map,
	CKvMatrixBool *in_voxel_map_mask,
	CKvMatrixInt *in_voxel_map_index,
	CKvMatrixUcharRgb *in_error_map,
	CKvMatrixBool *in_error_map_mask,
	CKvMatrixInt *in_error_map_index,
	int in_prediction_searching_range,
	int in_prediction_mode,
	VCL_DoCube_X_Color *io_docube)
/******************************************************************************/
{
	CKvDepot_of_RgbaF  depot_rgbaf; 
	CKvSet_of_RgbaF    set_rgbaf;
	CKvSet_of_Point3Df set_points3d;
	CKvMatrixIntRgb    error_map_int;
	CKvVectorBool      flags;
	CKvPoint3Df *p_pt3df; CKvRgbaF *p_rgbaf;
	float prediction_yuv[3], prediction_rgb[3];
	int **pr, **pg, **pb;
	bool **pmask;
	int **p_vmi;
	int j, i, ww, hh, num_voxel, searching_count;

	printf("Searching Range : %d\n", in_prediction_searching_range);

	num_voxel = io_docube->gsp_Get_Surface_Points().ne_Number_of_Elements();
	Get_Voxel_Colors_From_Texture_Map(
		in_voxel_map,//CKvMatrixUcharRgb *in_voxel_map,
		in_voxel_map_mask,//CKvMatrixBool *in_voxel_map_mask,
		in_voxel_map_index,//CKvMatrixInt *in_voxel_map_index,
		num_voxel,//int in_number_of_voxels,
		&depot_rgbaf,//CKvDepot_of_RgbaF *out_colors);
		&flags);//CKvVectorBool * out_flag);
	zz_voxel_idx_for_processing = std::vector<int>((int)pow((2 * in_prediction_searching_range + 1), 3));
	zz_vec_color_for_processing = std::vector<std::vector<float>>((int)pow((2 * in_prediction_searching_range + 1), 3), std::vector<float>(3));
	zz_weight_for_processing    = std::vector<float>((int)pow((2 * in_prediction_searching_range + 1), 3));

	Convert_To_Int(in_error_map, &error_map_int);

	depot_rgbaf.e_Export(&set_rgbaf);
	p_rgbaf = set_rgbaf.vps(num_voxel);
	io_docube->gsp_Get_Surface_Points().e_Export(&set_points3d);
	p_pt3df = set_points3d.vp();

	if ((in_error_map != NULL) &&
		(in_error_map_mask != NULL) &&
		(in_error_map_index != NULL))
	{
		p_vmi = in_error_map_index->mps(ww, hh);
		in_error_map->ms(ww, hh);
		in_error_map_mask->ms(ww, hh);

		pr = error_map_int.mp(pg, pb);
		pmask = in_error_map_mask->mp();
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if ((pmask[j][i] == true) && (p_vmi[j][i] != -1) && (p_vmi[j][i] < num_voxel))
				{
					p_rgbaf[(int)p_vmi[j][i]].a = 1.f;
					if (Get_index_and_Color_in_Block(
						(int)p_pt3df[(int)p_vmi[j][i]].x,//int in_xx, 
						(int)p_pt3df[(int)p_vmi[j][i]].y,//int in_yy, 
						(int)p_pt3df[(int)p_vmi[j][i]].z,//int in_zz,
						in_prediction_searching_range,//int in_searching_range,
						io_docube,//VCL_DoCube_X_Color *in_docube,
						p_rgbaf,//CKvRgbaF *in_set_rgbaf,
						&flags,//CKvVectorBool *in_prediction_flag,
						zz_voxel_idx_for_processing,//std::vector<int> &out_voxel_index_Memory_Already_Set,
						zz_vec_color_for_processing,//std::vector<std::vector<float>> &out_depot_colors_Memory_Already_Set,
						searching_count))//int &out_num_count);
					{
						if (in_prediction_mode == 0)
						{
							printf("Here?\n");
							Prediction_With_Average(
								zz_vec_color_for_processing,//std::vector<std::vector<float>> &in_depot_colors,
								searching_count,//int in_num_count,
								prediction_yuv);//float *out_prediction_in_yuv_3_dim)
						}
						else if (in_prediction_mode == 1)
						{
							//Prediction_With_Distance_Weight(
							//	zz_vec_color_for_processing,//std::vector<std::vector<float>> &in_depot_colors,
							//	zz_voxel_idx_for_processing,//std::vector<int> &in_voxel_index_Memory_Already_Set,
							//	zz_weight_for_processing,//std::vector<float> &in_wiehgt_Memory_Already_Set,
							//	p_pt3df,//CKvPoint3Df *in_pointer_point3d,
							//	&p_pt3df[(int)p_vmi[j][i]],//CKvPoint3Df *in_target_point3d,
							//	searching_count,//int in_num_size,
							//	prediction_yuv);//float *out_prediction_in_yuv_3_dim);
							Prediction_With_Distance_Weight_RGB(
								zz_vec_color_for_processing,//std::vector<std::vector<float>> &in_depot_colors,
								zz_voxel_idx_for_processing,//std::vector<int> &in_voxel_index_Memory_Already_Set,
								zz_weight_for_processing,//std::vector<float> &in_wiehgt_Memory_Already_Set,
								p_pt3df,//CKvPoint3Df *in_pointer_point3d,
								&p_pt3df[(int)p_vmi[j][i]],//CKvPoint3Df *in_target_point3d,
								searching_count,//int in_num_size,
								prediction_yuv);//float *out_prediction_in_yuv_3_dim);
						}

						prediction_rgb[0] = ((float)((float)pr[j][i]) / 255.f) + prediction_yuv[0];
						prediction_rgb[1] = ((float)((float)pg[j][i]) / 255.f) + prediction_yuv[1];
						prediction_rgb[2] = ((float)((float)pb[j][i]) / 255.f) + prediction_yuv[2];

						p_rgbaf[(int)p_vmi[j][i]].r = (prediction_rgb[0] > 1.f) ? 1.f : (prediction_rgb[0] < 0.f) ? 0.f : prediction_rgb[0];
						p_rgbaf[(int)p_vmi[j][i]].g = (prediction_rgb[1] > 1.f) ? 1.f : (prediction_rgb[1] < 0.f) ? 0.f : prediction_rgb[1];
						p_rgbaf[(int)p_vmi[j][i]].b = (prediction_rgb[2] > 1.f) ? 1.f : (prediction_rgb[2] < 0.f) ? 0.f : prediction_rgb[2];

						// In YUV
						//prediction_yuv[0] = ((float)((float)pr[j][i]) / 255.f) + prediction_yuv[0];
						//prediction_yuv[1] = ((float)((float)pg[j][i]) / 255.f) + prediction_yuv[1];
						//prediction_yuv[2] = ((float)((float)pb[j][i]) / 255.f) + prediction_yuv[2];
						//YUVtoRGB_BT709(
						//	prediction_yuv,//float *in_target_yuv,
						//	prediction_rgb);//float *out_rgb);
						//
						//p_rgbaf[(int)p_vmi[j][i]].r = (prediction_rgb[0] > 1.f) ? 1.f : (prediction_rgb[0] < 0.f) ? 0.f : prediction_rgb[0];
						//p_rgbaf[(int)p_vmi[j][i]].g = (prediction_rgb[1] > 1.f) ? 1.f : (prediction_rgb[1] < 0.f) ? 0.f : prediction_rgb[1];
						//p_rgbaf[(int)p_vmi[j][i]].b = (prediction_rgb[2] > 1.f) ? 1.f : (prediction_rgb[2] < 0.f) ? 0.f : prediction_rgb[2];
					}
					else
					{
						p_rgbaf[(int)p_vmi[j][i]].r = (float)1.f;
						p_rgbaf[(int)p_vmi[j][i]].g = (float)1.f;
						p_rgbaf[(int)p_vmi[j][i]].b = (float)1.f;
					}
				}
			}
		}
	}

	depot_rgbaf.im_Import(&set_rgbaf);
	io_docube->i_Import_Color_Vector(&depot_rgbaf);
}
/******************************************************************************/


/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Generate_Prediction_Image(
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

	for (k = 0; k < num_prediction_map; k++)
	{
		p_pp = p_p[k].mps(ww, hh);
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_pp[j][i] != -1)
				{
					// Geometric
					Find_Closet_Point(
						&p_colors[p_pp[j][i]],//CKvRgbaF *in_color,
						&p_point3d[p_pp[j][i]],//CKvPoint3Df *in_point,
						p_point3d,//CKvPoint3Df *in_all_points,
						out_prediction_voxel_map,//CKvMatrixUcharRgb *io_prediction_map,
						in_voxel_idx_map,//CKvMatrixInt *in_voxel_index_map,
						in_voxel_mask,//CKvMatrixBool *in_prediction_map_mask,
						&flag_map,//CKvMatrixBool *io_flag_map,
						xx,//int &out_x,
						yy);//int &out_y)

					//Find_Closet_Point(
					//	&p_colors[p_pp[j][i]],//CKvRgbaF *in_color,
					//	out_prediction_voxel_map,//CKvMatrixUcharRgb *io_prediction_map,
					//	in_voxel_mask,//CKvMatrixBool *in_prediction_map_mask,
					//	&flag_map,//CKvMatrixBool *io_flag_map,
					//	xx,//int &out_x,
					//	yy);//int &out_y);

					//// Fast
					//Find_Closet_Point_Fast(
					//	&p_colors[p_pp[j][i]],//CKvRgbaF *in_color,
					//	&p_point3d[p_pp[j][i]],//CKvPoint3Df *in_point,
					//	p_point3d,//CKvPoint3Df *in_all_points,
					//	in_position_of_blob,//CKvMatrixInt *in_position_blob,
					//	out_prediction_voxel_map,//CKvMatrixUcharRgb *io_prediction_map,
					//	in_voxel_idx_map,//CKvMatrixInt *in_voxel_index_map,
					//	in_voxel_mask,//CKvMatrixBool *in_prediction_map_mask,
					//	&flag_map,//CKvMatrixBool *io_flag_map,
					//	xx,//int &out_x,
					//	yy);//int &out_y)


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
void VCL_Voxel_Color_Final_Version::Generate_Prediction_Image2(
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

	voxel_idx_table = std::vector<std::vector<int>>(num_voxel, std::vector<int>(2,-1));
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
void VCL_Voxel_Color_Final_Version::Find_Closet_Point(
	CKvRgbaF *in_color,
	CKvPoint3Df *in_point,
	CKvPoint3Df *in_all_points,
	CKvMatrixUcharRgb *io_prediction_map,
	CKvMatrixInt *in_voxel_index_map,
	CKvMatrixBool *in_prediction_map_mask,
	CKvMatrixBool *io_flag_map,
	int &out_x,
	int &out_y)
//************************************************************************
{
	float dist, min_dist = FLT_MAX;
	unsigned char **pr, **pg, **pb;
	bool **p_mask, **p_flag;
	int ww, hh, j, i;
	int **p_idx;

	p_mask = in_prediction_map_mask->mps(ww, hh);
	p_flag = io_flag_map->mp();
	pr = io_prediction_map->mp(pg, pb);
	p_idx = in_voxel_index_map->mp();

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if ((p_mask[j][i] == true) && 
				(p_flag[j][i] == false) && 
				(p_idx[j][i] != -1))
			{
				dist = in_point->ds_Distance_Squared(in_all_points[p_idx[j][i]]);

				if (min_dist > dist)
				{
					min_dist = dist;
					out_x = i;
					out_y = j;
				}
			}
		}
	}

	p_flag[out_y][out_x] = true;
	pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
	pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
	pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);


	//VCL_EI_DCT.limit();

	//pr[out_y][out_x] = (unsigned char)0;
	//pg[out_y][out_x] = (unsigned char)255;
	//pb[out_y][out_x] = (unsigned char)0;
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Closet_Point_Fast(
	CKvRgbaF *in_color,
	CKvPoint3Df *in_point,
	CKvPoint3Df *in_all_points,
	CKvMatrixUcharRgb *io_prediction_map,
	CKvMatrixInt *in_voxel_index_map,
	CKvMatrixBool *in_prediction_map_mask,
	CKvMatrixBool *io_flag_map,
	int &out_x,
	int &out_y)
//************************************************************************
{
	std::vector<float> dist;
	float min_dist = FLT_MAX;
	unsigned char **pr, **pg, **pb;
	bool **p_mask, **p_flag;
	int ww, hh, j, i;
	int **p_idx;

	p_mask = in_prediction_map_mask->mps(ww, hh);
	p_flag = io_flag_map->mp();
	pr = io_prediction_map->mp(pg, pb);
	p_idx = in_voxel_index_map->mp();

	dist = std::vector<float>(ww*hh, FLT_MAX);

#pragma omp parallel
	{
#pragma omp for
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if ((p_mask[j][i] == true) &&
					(p_flag[j][i] == false) &&
					(p_idx[j][i] != -1))
				{
					dist[j*ww + i] = in_point->ds_Distance_Squared(in_all_points[p_idx[j][i]]);
					//if (min_dist > dist)
					//{
					//	min_dist = dist;
					//	out_x = i;
					//	out_y = j;
					//}
				}
			}
		}
	}


	for (j = 0; j < ww*hh; j++)
	{
		if (min_dist > dist[j])
		{
			min_dist = dist[j];
			out_x = (int)(j%ww);
			out_y = (int)(j/ww);
		}
	}


	p_flag[out_y][out_x] = true;
	pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
	pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
	pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Closet_Point(
	CKvRgbaF *in_color,
	CKvPoint3Df *in_point,
	CKvPoint3Df *in_all_points,
	CKvMatrixInt *in_position_blob,
	CKvMatrixUcharRgb *io_prediction_map,
	CKvMatrixInt *in_voxel_index_map,
	CKvMatrixBool *in_prediction_map_mask,
	CKvMatrixBool *io_flag_map,
	int &out_x,
	int &out_y)
//************************************************************************
{
	float dist, min_dist = 100000000.f;
	unsigned char **pr, **pg, **pb;
	bool **p_mask, **p_flag;
	int dum, num_blob, ww, hh, j, i, k;
	int **p_idx, **p_position;

	p_mask = in_prediction_map_mask->mps(ww, hh);
	p_flag = io_flag_map->mp();
	pr = io_prediction_map->mp(pg, pb);
	p_idx = in_voxel_index_map->mp();
	p_position = in_position_blob->mps(dum, num_blob);

	for (k = 0; k < num_blob; k++)
	{
		for (j = p_position[k][1]; j < p_position[k][1] + p_position[k][3]; j++)
		{
			for (i = p_position[k][0]; i < p_position[k][0] + p_position[k][2]; i++)
			{
				if ((p_mask[j][i] == true) && (p_flag[j][i] == false) && (p_idx[j][i] != -1))
				{
					dist = (in_point->x - in_all_points[p_idx[j][i]].x)*(in_point->x - in_all_points[p_idx[j][i]].x) +
							(in_point->y - in_all_points[p_idx[j][i]].y)*(in_point->y - in_all_points[p_idx[j][i]].y) +
							(in_point->z - in_all_points[p_idx[j][i]].z)*(in_point->z - in_all_points[p_idx[j][i]].z);

					//if ((dist == 1)|| (dist == 2)|| (dist == 3))
					if (dist <= 3)
					{
						out_x = i;
						out_y = j;
						p_flag[out_y][out_x] = true;
						pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
						pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
						pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);
						return;
					}


					if (min_dist > dist)
					{
						min_dist = dist;
						out_x = i;
						out_y = j;
					}
				}
			}
		}
	}

	p_flag[out_y][out_x] = true;
	pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
	pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
	pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Closet_Point_Fast(
	CKvRgbaF *in_color,
	CKvPoint3Df *in_point,
	CKvPoint3Df *in_all_points,
	CKvMatrixInt *in_position_blob,
	CKvMatrixUcharRgb *io_prediction_map,
	CKvMatrixInt *in_voxel_index_map,
	CKvMatrixBool *in_prediction_map_mask,
	CKvMatrixBool *io_flag_map,
	int &out_x,
	int &out_y)
//************************************************************************
{
	std::vector<std::vector<float>> dist;
	float min_dist = FLT_MAX;
	unsigned char **pr, **pg, **pb;
	bool **p_mask, **p_flag;
	int dum, num_blob, ww, hh, j, i, k;
	int **p_idx, **p_position;

	p_mask = in_prediction_map_mask->mps(ww, hh);
	p_flag = io_flag_map->mp();
	pr = io_prediction_map->mp(pg, pb);
	p_idx = in_voxel_index_map->mp();
	p_position = in_position_blob->mps(dum, num_blob);
	
	dist = std::vector<std::vector<float>>(num_blob);
#pragma omp parallel
	{
#pragma omp for private(j), private(i)
		for (k = 0; k < num_blob; k++)
		{
			dist[k] = std::vector<float>(p_position[k][3] * p_position[k][2], FLT_MAX);
			for (j = p_position[k][1]; j < p_position[k][1] + p_position[k][3]; j++)
			{
				for (i = p_position[k][0]; i < p_position[k][0] + p_position[k][2]; i++)
				{
					if ((p_mask[j][i] == true) && (p_flag[j][i] == false) && (p_idx[j][i] != -1))
					{
						dist[k][(j - p_position[k][1])*p_position[k][2] + (i - p_position[k][0])] =
							(in_point->x - in_all_points[p_idx[j][i]].x)*(in_point->x - in_all_points[p_idx[j][i]].x) +
							(in_point->y - in_all_points[p_idx[j][i]].y)*(in_point->y - in_all_points[p_idx[j][i]].y) +
							(in_point->z - in_all_points[p_idx[j][i]].z)*(in_point->z - in_all_points[p_idx[j][i]].z);
					}
				}
			}
		}
	}


	for (k = 0; k < num_blob; k++)
	{
		for (j = 0; j < dist[k].size(); j++)
		{
			if (min_dist > dist[k][j])
			{
				min_dist = dist[k][j];
				out_y = (j % p_position[k][2]) + p_position[k][1];
				out_x = (j / p_position[k][2]) + p_position[k][0];
			}
		}
		
	}

	p_flag[out_y][out_x] = true;
	pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
	pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
	pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Closet_Point(
	CKvRgbaF *in_color,
	CKvMatrixUcharRgb *io_prediction_map,
	CKvMatrixBool *in_prediction_map_mask,
	CKvMatrixBool *io_flag_map,
	int &out_x,
	int &out_y)
//************************************************************************
{
	float dist, min_dist = 10.f;
	unsigned char **pr, **pg, **pb;
	bool **p_mask, **p_flag;
	int ww, hh, j, i;

	p_mask = in_prediction_map_mask->mps(ww, hh);
	p_flag = io_flag_map->mp();
	pr = io_prediction_map->mp(pg, pb);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if ((p_mask[j][i] == true) && (p_flag[j][i] == false))
			{
				dist = pow(in_color->r - ((float)(pr[j][i]) / 255.f), 2) +
					pow(in_color->g - ((float)(pg[j][i]) / 255.f), 2) +
					pow(in_color->b - ((float)(pb[j][i]) / 255.f), 2);

				if (min_dist > dist)
				{
					min_dist = dist;
					out_x = i;
					out_y = j;
				}
			}
		}
	}

	p_flag[out_y][out_x] = true;
	pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
	pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
	pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Closet_Point2(
	CKvRgbaF *in_color,
	CKvPoint3Df *in_point,
	CKvPoint3Df *in_all_points,
	std::vector<std::vector<int>> &in_voxel_idx_table,
	std::vector<bool> &io_voxel_idx_flag,
	CKvMatrixUcharRgb *io_prediction_map,
	int &out_x,
	int &out_y)
//************************************************************************
{
	std::vector<float> dist;
	float min_dist = FLT_MAX;
	int num_voxel, voxel_idx, k;
	unsigned char **pr, **pg, **pb;

	num_voxel = in_voxel_idx_table.size();
	dist      = std::vector<float>(num_voxel, FLT_MAX);

#pragma omp parallel
	{
#pragma omp for
		for (k = 0; k < num_voxel; k++)
		{
			if ((in_voxel_idx_table[k][0] != -1) && (io_voxel_idx_flag[k] == false))
			{
				dist[k] = in_point->d_Distance(in_all_points[k]);
			}
		}
	}

	
	for (k = 0; k < num_voxel; k++)
	{
		if (min_dist > dist[k])
		{
			min_dist = dist[k];
			out_x = in_voxel_idx_table[k][1];
			out_y = in_voxel_idx_table[k][0];
			voxel_idx = k;
		}
	}

	pr = io_prediction_map->mp(pg, pb);

	io_voxel_idx_flag[voxel_idx] = true;
	pr[out_y][out_x] = (unsigned char)limit(in_color->r *255.f);
	pg[out_y][out_x] = (unsigned char)limit(in_color->g *255.f);
	pb[out_y][out_x] = (unsigned char)limit(in_color->b *255.f);

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Closet_Point3(
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

	p_xvec2d    = in_xvector_2d->mps(ww, hh);

	while (1)
	{
		xo = (xx - block_sz) < 0 ? 0 : (xx - block_sz);
		yo = (yy - block_sz) < 0 ? 0 : (yy - block_sz);
		zo = (zz - block_sz) < 0 ? 0 : (zz - block_sz);

		x1 = (xx + block_sz) > ww-1 ? ww-1 : (xx + block_sz);
		y1 = (yy + block_sz) > hh-1 ? hh-1 : (yy + block_sz);
		z1 = (zz + block_sz) > hh-1 ? hh-1 : (zz + block_sz);

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
void VCL_Voxel_Color_Final_Version::Partitioning_Voxels_And_Generate_Voxel_Maps(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	int in_alignment_mode,
	int in_threshold_for_cluster_size,
	CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
	CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask,
	CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
	CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask)
/******************************************************************************/
{
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> voxel_texture_sequence;

	printf("[gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction]\n");
	gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		voxel_texture_sequence);//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence)

	printf("[gvmv_Generate_Voxel_Maps]\n");
	//gvmv_Generate_Voxel_Maps(
	//	in_docube,//VCL_DoCube_X_Color *in_docube,
	//	voxel_texture_sequence,//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
	//	in_alignment_mode,//int in_alignment_mode,
	//	in_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
	//	out_set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map,
	//	out_set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *out_set_of_voxel_mask)
	//	out_set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
	//	out_set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction)

	gvmv_Generate_Voxel_Maps_Video_Temporal_Alignment(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		voxel_texture_sequence,//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
		in_alignment_mode,//int in_alignment_mode,
		in_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
		out_set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map,
		out_set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *out_set_of_voxel_mask)
		out_set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
		out_set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction)


	printf("[Reordering_Voxel_Map_By_Map_Size 1]\n");
	Reordering_Voxel_Map_By_Map_Size(
		out_set_of_voxel_map_idx,//CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
		out_set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask)

	//printf("[Reordering_Voxel_Map_By_Map_Size 2]\n");
	//Reordering_Voxel_Map_By_Map_Size(
	//	out_set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
	//	out_set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask)
}
/******************************************************************************/
void VCL_Voxel_Color_Final_Version::Partitioning_Voxels_And_Generate_Voxel_Maps(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	int in_alignment_mode,
	int in_threshold_for_cluster_size,
	int in_random_number,
	CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
	CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask,
	CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
	CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask)
/******************************************************************************/
{
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> voxel_texture_sequence;

	printf("[gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction]\n");
	gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		voxel_texture_sequence);//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence)

	printf("[gvmv_Generate_Voxel_Maps]\n");
	gvmv_Generate_Voxel_Maps(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		voxel_texture_sequence,//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
		in_alignment_mode,//int in_alignment_mode,
		in_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
		in_random_number,//int in_random_number,
		out_set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map,
		out_set_of_voxel_map_idx_mask,//CKvSet_of_MatrixBool *out_set_of_voxel_mask)
		out_set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
		out_set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction)

	printf("[Reordering_Voxel_Map_By_Map_Size 1]\n");
	Reordering_Voxel_Map_By_Map_Size(
		out_set_of_voxel_map_idx,//CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
		out_set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask)

	//printf("[Reordering_Voxel_Map_By_Map_Size 2]\n");
	//Reordering_Voxel_Map_By_Map_Size(
	//	out_set_of_voxel_map_small_blob_idx,//CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
	//	out_set_of_voxel_map_small_blob_idx_mask);//CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask)
}
/******************************************************************************/
bool VCL_Voxel_Color_Final_Version::gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence)
//************************************************************************
{
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
		}
	}

	if (num_scanned != num_plane_mode)
	{
		out_voxel_sequence.erase(out_voxel_sequence.begin() + num_scanned, out_voxel_sequence.end());
	}

	printf("		[Get Scanned Voxel Colors] -> %lf\n", sw.get_Get_Elapsed_Time(0));


	/*****************************************************************/
	// For Standard Deviation. 
	//CKvXvector xvector;
	//xvector.in_Initialize(1);
	//int num_voxel;
	//for (k = 0; k < num_scanned; k++)
	//{
	//	int num_plane = out_voxel_sequence[k].size();
	//	num_voxel = 0;
	//	for (int l = 0; l < num_plane; l++)
	//	{
	//		for (int j = 0; j < out_voxel_sequence[k][l].voxel_idx_sequences.size(); j++)
	//		{
	//			num_voxel += out_voxel_sequence[k][l].voxel_idx_sequences[j].size();
	//			if (k == 0)
	//			{
	//				xvector.ae_Append_Element(out_voxel_sequence[k][l].voxel_idx_sequences[j].size());
	//			}
	//		}
	//	}
	//	printf("Num Voxel -> k=%d/%d ) %d\n", k, num_scanned, num_voxel);
	//}
	//CKvVector vec_len; double std;
	//xvector.ex_Export(&vec_len, NULL);
	//vec_len.m_Mean_and_Stdev(std);
	//printf("std = %lf\n", std);
	/*****************************************************************/



	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction_and_Closing(
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
void VCL_Voxel_Color_Final_Version::gvmv_Generate_Voxel_Maps(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
	int in_alignment_mode,
	int in_threshold_for_cluster_size,
	CKvSet_of_MatrixInt *out_set_of_voxel_map,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask,
	CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> texture_on_code;
	CKvDepot_of_MatrixInt    depot_voxel_maps;
	CKvDepot_of_MatrixBool   depot_voxel_masks;

	CKvDepot_of_MatrixInt    set_voxel_maps;
	CKvDepot_of_MatrixBool   set_voxel_map_masks;

	CKvDepot_of_MatrixInt    depot_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   depot_voxel_map_masks_for_prediction;

	CKvDepot_of_MatrixInt    set_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   set_voxel_map_masks_for_prediction;

	CKvSet_of_VectorInt        set_offsets, set_circular_shift;
	CKvMatrixInt               position_of_segmented_voxel_map;
	int num_plane_mode, k;
	int resolution_x, resolution_y, resolution_z;

	num_plane_mode = in_voxel_sequence.size();
	in_docube->gr_Get_Resolution(resolution_x, resolution_y, resolution_z);

	depot_voxel_maps.in_Initialize();
	depot_voxel_masks.in_Initialize();
	depot_voxel_idx_for_prediction.in_Initialize();
	depot_voxel_map_masks_for_prediction.in_Initialize();
	
	CKvStopWatch sw;
	sw.c_Create(1);
	

	for (k = 0; k < num_plane_mode; k++)
	{
		printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
		sw.r_Reset(0);
		stoc_Segment_Voxel_Slice_Scanned_Data(
			in_voxel_sequence[k],//std::vector<Voxel_Slice_Peeling_Chain_Code> &in_texture_on_code,
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

		/**********************************************************/
		// For Debug
		//CKvString file_name;
		//file_name = "align_params.bind";
		//sap_Save_Align_Parameters(
		//	file_name,//CKvString str,
		//	texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	in_threshold_for_cluster_size,//int in_blob_size_threshold,
		//	&set_circular_shift,//CKvSet_of_VectorInt *in_circular_shift,
		//	&set_offsets);//CKvSet_of_VectorInt *in_offset)
		//printf("[SAVE Align Params]\n");
		//return;
		//
		/**********************************************************/
		printf("[s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding]\n");
		if (s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
			in_docube, //VCL_DoCube_X_Color *in_docube,
			texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
			in_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
			&set_offsets,//CKvSet_of_VectorInt *in_offsets,
			&set_voxel_maps,//CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
			&set_voxel_map_masks,//CKvDepot_of_MatrixBool *out_set_masks,
			&set_voxel_idx_for_prediction,//CKvDepot_of_MatrixInt *out_set_voxel_idx_for_prediction,
			&set_voxel_map_masks_for_prediction))//CKvDepot_of_MatrixBool *out_set_masks_for_prediction);
		{
			depot_voxel_maps.ap_Append(false, &set_voxel_maps, NULL, NULL);
			depot_voxel_masks.ap_Append(false, &set_voxel_map_masks, NULL, NULL);
			depot_voxel_idx_for_prediction.ap_Append(false, &set_voxel_idx_for_prediction, NULL, NULL);
			depot_voxel_map_masks_for_prediction.ap_Append(false, &set_voxel_map_masks_for_prediction, NULL, NULL);
		}
	}

	if (depot_voxel_maps.ne_Number_of_Elements() != 0)
	{
		depot_voxel_maps.ex_Export(out_set_of_voxel_map);
		depot_voxel_masks.ex_Export(out_set_of_voxel_mask);
	}

	if (depot_voxel_idx_for_prediction.ne_Number_of_Elements() != 0)
	{
		depot_voxel_idx_for_prediction.ex_Export(out_set_of_voxel_idx_for_prediction);
		depot_voxel_map_masks_for_prediction.ex_Export(out_set_of_voxel_mask_for_prediction);
	}

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::gvmv_Generate_Voxel_Maps_Video_Temporal_Alignment(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
	int in_alignment_mode,
	int in_threshold_for_cluster_size,
	CKvSet_of_MatrixInt *out_set_of_voxel_map,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask,
	CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> texture_on_code;
	CKvDepot_of_MatrixInt    depot_voxel_maps;
	CKvDepot_of_MatrixBool   depot_voxel_masks;

	CKvDepot_of_MatrixInt    set_voxel_maps;
	CKvDepot_of_MatrixBool   set_voxel_map_masks;

	CKvDepot_of_MatrixInt    depot_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   depot_voxel_map_masks_for_prediction;

	CKvDepot_of_MatrixInt    set_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   set_voxel_map_masks_for_prediction;

	CKvSet_of_VectorInt        set_offsets, set_circular_shift;
	CKvMatrixInt               position_of_segmented_voxel_map;
	int num_plane_mode, k;
	int resolution_x, resolution_y, resolution_z;

	num_plane_mode = in_voxel_sequence.size();
	in_docube->gr_Get_Resolution(resolution_x, resolution_y, resolution_z);

	depot_voxel_maps.in_Initialize();
	depot_voxel_masks.in_Initialize();
	depot_voxel_idx_for_prediction.in_Initialize();
	depot_voxel_map_masks_for_prediction.in_Initialize();

	CKvStopWatch sw;
	sw.c_Create(1);


	for (k = 0; k < num_plane_mode; k++)
	{
		printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
		sw.r_Reset(0);
		stoc_Segment_Voxel_Slice_Scanned_Data(
			in_voxel_sequence[k],//std::vector<Voxel_Slice_Peeling_Chain_Code> &in_texture_on_code,
			texture_on_code);//std::vector<std::vector<std::vector<int>>> &out_aligned_texture_on_code)

		printf("Time : %lf\n", sw.get_Get_Elapsed_Time(0));

		printf("[atoc_Align_Texture_on_Code]\n");
		sw.r_Reset(0);

		//if (zz_before_segmented_texture_on_code.size() == 0)
		if(zz_before_set_of_voxels.vs() > 0)
		{
			atoc_Align_Texture_on_Code(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				in_alignment_mode,//int in_alignment_mode,
				texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
				&set_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
				&set_offsets);//CKvSet_of_VectorInt *out_offsets);
		}
		else
		{
			// Only for Video Color Temporal Alignment. 
			atoc_Align_Texture_on_Code_for_Video_Color(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				in_alignment_mode,//int in_alignment_mode,
				&zz_before_set_of_voxels,//CKvSet_of_Voxel *in_before_set_of_voxels,
				&zz_before_set_of_colors,//CKvSet_of_RgbaF *in_before_set_of_colors,
				zz_before_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
				texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
				&set_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
				&set_offsets);//CKvSet_of_VectorInt *out_offsets)
		}


		if (k == 0)
		{
			zz_before_segmented_texture_on_code = texture_on_code;
		}

		printf("Time : %lf\n", sw.get_Get_Elapsed_Time(0));

		/**********************************************************/
		// For Debug
		//CKvString file_name;
		//file_name = "align_params.bind";
		//sap_Save_Align_Parameters(
		//	file_name,//CKvString str,
		//	texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	in_threshold_for_cluster_size,//int in_blob_size_threshold,
		//	&set_circular_shift,//CKvSet_of_VectorInt *in_circular_shift,
		//	&set_offsets);//CKvSet_of_VectorInt *in_offset)
		//printf("[SAVE Align Params]\n");
		//return;
		//
		/**********************************************************/
		printf("[s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding]\n");
		if (s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
			in_docube, //VCL_DoCube_X_Color *in_docube,
			texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
			in_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
			&set_offsets,//CKvSet_of_VectorInt *in_offsets,
			&set_voxel_maps,//CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
			&set_voxel_map_masks,//CKvDepot_of_MatrixBool *out_set_masks,
			&set_voxel_idx_for_prediction,//CKvDepot_of_MatrixInt *out_set_voxel_idx_for_prediction,
			&set_voxel_map_masks_for_prediction))//CKvDepot_of_MatrixBool *out_set_masks_for_prediction);
		{
			depot_voxel_maps.ap_Append(false, &set_voxel_maps, NULL, NULL);
			depot_voxel_masks.ap_Append(false, &set_voxel_map_masks, NULL, NULL);
			depot_voxel_idx_for_prediction.ap_Append(false, &set_voxel_idx_for_prediction, NULL, NULL);
			depot_voxel_map_masks_for_prediction.ap_Append(false, &set_voxel_map_masks_for_prediction, NULL, NULL);
		}
	}

	CKvDepot_of_Voxel dv; CKvDepot_of_RgbaF drgb;

	drgb = in_docube->gvc_Get_Voxel_Color();
	drgb.e_Export(&zz_before_set_of_colors); drgb.in_Initialize();

	dv = in_docube->gsp_Get_Surface_Voxels();
	dv.e_Export(&zz_before_set_of_voxels); dv.in_Initialize();


	if (depot_voxel_maps.ne_Number_of_Elements() != 0)
	{
		depot_voxel_maps.ex_Export(out_set_of_voxel_map);
		depot_voxel_masks.ex_Export(out_set_of_voxel_mask);
	}

	if (depot_voxel_idx_for_prediction.ne_Number_of_Elements() != 0)
	{
		depot_voxel_idx_for_prediction.ex_Export(out_set_of_voxel_idx_for_prediction);
		depot_voxel_map_masks_for_prediction.ex_Export(out_set_of_voxel_mask_for_prediction);
	}
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::gvmv_Generate_Voxel_Maps(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
	int in_alignment_mode,
	int in_threshold_for_cluster_size,
	int in_random_number,
	CKvSet_of_MatrixInt *out_set_of_voxel_map,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask,
	CKvSet_of_MatrixInt *out_set_of_voxel_idx_for_prediction,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask_for_prediction)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> texture_on_code;
	CKvDepot_of_MatrixInt    depot_voxel_maps;
	CKvDepot_of_MatrixBool   depot_voxel_masks;

	CKvDepot_of_MatrixInt    set_voxel_maps;
	CKvDepot_of_MatrixBool   set_voxel_map_masks;

	CKvDepot_of_MatrixInt    depot_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   depot_voxel_map_masks_for_prediction;

	CKvDepot_of_MatrixInt    set_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   set_voxel_map_masks_for_prediction;

	CKvSet_of_VectorInt        set_offsets, set_circular_shift;
	CKvMatrixInt               position_of_segmented_voxel_map;
	int num_plane_mode, k;
	int resolution_x, resolution_y, resolution_z;

	num_plane_mode = in_voxel_sequence.size();
	in_docube->gr_Get_Resolution(resolution_x, resolution_y, resolution_z);

	depot_voxel_maps.in_Initialize();
	depot_voxel_masks.in_Initialize();
	depot_voxel_idx_for_prediction.in_Initialize();
	depot_voxel_map_masks_for_prediction.in_Initialize();

	CKvStopWatch sw;
	sw.c_Create(1);


	for (k = 0; k < num_plane_mode; k++)
	{
		printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
		sw.r_Reset(0);
		stoc_Segment_Voxel_Slice_Scanned_Data(
			in_voxel_sequence[k],//std::vector<Voxel_Slice_Peeling_Chain_Code> &in_texture_on_code,
			texture_on_code);//std::vector<std::vector<std::vector<int>>> &out_aligned_texture_on_code)

		printf("Time : %lf\n", sw.get_Get_Elapsed_Time(0));

		printf("[atoc_Align_Texture_on_Code]\n");
		sw.r_Reset(0);

		atoc_Align_Texture_on_Code(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_alignment_mode,//int in_alignment_mode,
			in_random_number,//int &in_random_number,
			texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			&set_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			&set_offsets);//CKvSet_of_VectorInt *out_offsets);


		printf("Time : %lf\n", sw.get_Get_Elapsed_Time(0));

		/**********************************************************/
		// For Debug
		//CKvString file_name;
		//file_name = "align_params.bind";
		//sap_Save_Align_Parameters(
		//	file_name,//CKvString str,
		//	texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	in_threshold_for_cluster_size,//int in_blob_size_threshold,
		//	&set_circular_shift,//CKvSet_of_VectorInt *in_circular_shift,
		//	&set_offsets);//CKvSet_of_VectorInt *in_offset)
		//printf("[SAVE Align Params]\n");
		//return;
		//
		/**********************************************************/
		printf("[s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding]\n");
		if (s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
			in_docube, //VCL_DoCube_X_Color *in_docube,
			texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
			in_threshold_for_cluster_size,//int in_threshold_for_cluster_size,
			&set_offsets,//CKvSet_of_VectorInt *in_offsets,
			&set_voxel_maps,//CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
			&set_voxel_map_masks,//CKvDepot_of_MatrixBool *out_set_masks,
			&set_voxel_idx_for_prediction,//CKvDepot_of_MatrixInt *out_set_voxel_idx_for_prediction,
			&set_voxel_map_masks_for_prediction))//CKvDepot_of_MatrixBool *out_set_masks_for_prediction);
		{
			depot_voxel_maps.ap_Append(false, &set_voxel_maps, NULL, NULL);
			depot_voxel_masks.ap_Append(false, &set_voxel_map_masks, NULL, NULL);
			depot_voxel_idx_for_prediction.ap_Append(false, &set_voxel_idx_for_prediction, NULL, NULL);
			depot_voxel_map_masks_for_prediction.ap_Append(false, &set_voxel_map_masks_for_prediction, NULL, NULL);
		}
	}

	if (depot_voxel_maps.ne_Number_of_Elements() != 0)
	{
		depot_voxel_maps.ex_Export(out_set_of_voxel_map);
		depot_voxel_masks.ex_Export(out_set_of_voxel_mask);
	}

	if (depot_voxel_idx_for_prediction.ne_Number_of_Elements() != 0)
	{
		depot_voxel_idx_for_prediction.ex_Export(out_set_of_voxel_idx_for_prediction);
		depot_voxel_map_masks_for_prediction.ex_Export(out_set_of_voxel_mask_for_prediction);
	}
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Reordering_Voxel_Map_By_Map_Size(
	CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
	CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask)
//************************************************************************
{
	CKvSet_of_MatrixInt  depot_map;
	CKvSet_of_MatrixBool depot_map_mask;
	CKvVectorShort vec_size_hh;
	CKvVectorShort vec_size_ww;
	CKvVectorInt old_index;
	CKvRanker ranker;
	int num_map, k, hh, ww;

	CKvMatrixInt  *p_map = io_set_of_voxel_map_idx->vps(num_map);
	CKvMatrixBool *p_mask = io_set_of_voxel_map_idx_mask->vp();

	short *p_hh = vec_size_hh.c_Create(num_map);
	short *p_ww = vec_size_ww.c_Create(num_map);

	for (k = 0; k < num_map; k++)
	{
		p_map[k].ms(ww, hh);
		p_hh[k] = hh;
		p_ww[k] = ww;
	}

	ranker.s_Sort(&vec_size_hh, true, 0, num_map, &old_index);
	int *p_oi = old_index.vp();

	depot_map.c_Create(num_map);
	depot_map_mask.c_Create(num_map);
	for (k = 0; k < num_map; k++)
	{
		depot_map.gpe(k)->cp_Copy(&p_map[p_oi[k]]);
		depot_map_mask.gpe(k)->cp_Copy(&p_mask[p_oi[k]]);
	}

	io_set_of_voxel_map_idx->cp_Copy(&depot_map);
	io_set_of_voxel_map_idx_mask->cp_Copy(&depot_map_mask);
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Classify_Voxe_Map_and_Generate_Error_Map(
	VCL_DoCube_X_Color *in_docube,
	CKvSet_of_MatrixInt *io_set_of_voxel_map_idx,
	CKvSet_of_MatrixBool *io_set_of_voxel_map_idx_mask,
	CKvSet_of_MatrixInt *io_error_map_idx,
	CKvSet_of_MatrixBool *io_error_map_mask,
	int in_searching_range,
	int in_prediction_mode,
	int in_error_threshold,
	CKvSet_of_MatrixIntRgb *out_error_map)
//************************************************************************
{
	CKvSet_of_MatrixInt set_of_voxel_map_for_prediction_fail;
	CKvSet_of_MatrixBool set_of_voxel_map_mask_for_prediction_fail;
	CKvDepot_of_MatrixInt tmp1, tmp2;
	CKvDepot_of_MatrixBool tmp3, tmp4;
	int prev_num, current_num;
	bool cond;

	do
	{
		prev_num = io_error_map_idx->vs();
		cond = gpevm_Generate_Prediction_Error_Voxel_Map(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_error_map_idx,//CKvSet_of_MatrixInt *io_set_of_voxel_idx_for_prediction,
			io_error_map_mask,//CKvSet_of_MatrixBool *io_set_of_voxel_mask_for_prediction,
			in_searching_range,//int in_searching_range,
			in_prediction_mode,//int in_prediction_mode,
			in_error_threshold,//int in_error_threshold,
			out_error_map,//CKvSet_of_MatrixIntRgb *out_error_prediction_voxel_map,
			&set_of_voxel_map_for_prediction_fail,//CKvSet_of_MatrixInt *out_texture_map_for_error_prediction_fail,
			&set_of_voxel_map_mask_for_prediction_fail,//CKvSet_of_MatrixBool *out_texture_mask_for_error_prediction_fail,
			NULL);//CKvVectorInt *out_idx_for_prediction_target_or_NULL)

		current_num = io_error_map_idx->vs();

		if (cond == false)
		{

			tmp1.im_Import(io_set_of_voxel_map_idx);  tmp2.im_Import(&set_of_voxel_map_for_prediction_fail);
			tmp1.ap_Append(false, &tmp2, NULL, NULL); tmp2.in_Initialize();
			tmp1.ex_Export(io_set_of_voxel_map_idx);  tmp1.in_Initialize();
			set_of_voxel_map_for_prediction_fail.c_Create(1);


			tmp3.im_Import(io_set_of_voxel_map_idx_mask);   tmp4.im_Import(&set_of_voxel_map_mask_for_prediction_fail);
			tmp3.ap_Append(false, &tmp4, NULL, NULL);       tmp4.in_Initialize();
			tmp3.ex_Export(io_set_of_voxel_map_idx_mask);   tmp3.in_Initialize();
			set_of_voxel_map_mask_for_prediction_fail.c_Create(1);
		}
	} while (prev_num != current_num);

}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::gpevm_Generate_Prediction_Error_Voxel_Map(
	VCL_DoCube_X_Color *in_docube,
	CKvSet_of_MatrixInt *io_set_of_voxel_idx_for_prediction,
	CKvSet_of_MatrixBool *io_set_of_voxel_mask_for_prediction,
	int in_searching_range,
	int in_prediction_mode,
	int in_error_threshold,
	CKvSet_of_MatrixIntRgb *out_error_prediction_voxel_map,
	CKvSet_of_MatrixInt *out_texture_map_for_error_prediction_fail,
	CKvSet_of_MatrixBool *out_texture_mask_for_error_prediction_fail,
	CKvVectorInt *out_idx_for_prediction_target)
//************************************************************************
{
	CKvDepot_of_MatrixIntRgb depot_error_map;
	CKvDepot_of_MatrixBool depot_masks; CKvDepot_of_MatrixBool set_error_masks;
	CKvDepot_of_MatrixInt depot_error_fail_texture_map, depot_error_maps;
	CKvVectorBool prediction_flag;
	CKvSet_of_Point3Df set_pt3df; CKvSet_of_RgbaF set_rgbaf;
	CKvPoint3Df *p_set_pointer_pt3df; CKvRgbaF *p_set_pointer_colors;
	int num_blob, k, l, idx_sz, num_voxel;
	int *p_idx; bool *p_mask, *p_flag, mode_result;
	int num_count1 = 0, num_count2 = 0;

	num_blob = io_set_of_voxel_idx_for_prediction->vs();
	num_voxel = in_docube->gsp_Get_Surface_Points().ne_Number_of_Elements();
	p_flag = prediction_flag.c_Create(num_voxel, false);

	for (k = 0; k < num_blob; k++)
	{
		p_idx = io_set_of_voxel_idx_for_prediction->vp()[k].vps(idx_sz);
		p_mask = io_set_of_voxel_mask_for_prediction->vp()[k].vp();
		for (l = 0; l < idx_sz; l++)
		{
			if (p_mask[l] == true)
			{
				p_flag[p_idx[l]] = true;
			}
		}
	}

	out_error_prediction_voxel_map->c_Create(num_blob);
	set_error_masks.in_Initialize(num_blob);
	depot_error_maps.in_Initialize(num_blob);
	out_texture_map_for_error_prediction_fail->c_Create(num_blob);
	out_texture_mask_for_error_prediction_fail->c_Create(num_blob);

	/******************************************************************************/
	CKvMatrixInt  *p_svip = io_set_of_voxel_idx_for_prediction->vp();
	CKvMatrixBool   *p_svmp = io_set_of_voxel_mask_for_prediction->vp();
	CKvMatrixIntRgb *p_epvm = out_error_prediction_voxel_map->vp();
	CKvMatrixInt  *p_tmepf = out_texture_map_for_error_prediction_fail->vp();
	CKvMatrixBool   *p_tmmepf = out_texture_mask_for_error_prediction_fail->vp();
	/******************************************************************************/

	/******************************************************************************/
	zz_voxel_idx_for_processing = std::vector<int>((int)pow((2 * in_searching_range + 1), 3));
	zz_vec_color_for_processing = std::vector<std::vector<float>>((int)pow((2 * in_searching_range + 1), 3), std::vector<float>(3));
	zz_weight_for_processing = std::vector<float>((int)pow((2 * in_searching_range + 1), 3));
	/******************************************************************************/

	in_docube->gsp_Get_Surface_Points().e_Export(&set_pt3df);
	p_set_pointer_pt3df = set_pt3df.vp();

	in_docube->gvc_Get_Voxel_Color().e_Export(&set_rgbaf);
	p_set_pointer_colors = set_rgbaf.vp();

	for (k = 0; k < num_blob; k++)
	{
		if (in_prediction_mode == 0)
		{
			mode_result = gpevm_Generate_Prediction_Error_Voxel_Map_With_Average(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				p_set_pointer_pt3df,
				p_set_pointer_colors,
				&prediction_flag,//CKvVectorBool *in_prediction_flag,
				&p_svip[k],//io_set_of_voxel_idx_for_prediction->gpe(k),//CKvMatrixInt *in_voxel_idx_map_for_prediction,
				&p_svmp[k],//io_set_of_voxel_mask_for_prediction->gpe(k),//CKvMatrixBool *in_voxel_mask,
				in_searching_range,//int in_searching_range,
				in_error_threshold,//int in_error_threshold,
				&p_epvm[num_count2],//out_error_prediction_voxel_map->gpe(num_count2),//CKvMatrixIntRgb *out_error_map,
				&p_tmepf[num_count1],//out_texture_map_for_error_prediction_fail->gpe(num_count1)))//CKvMatrixInt *out_texture_map_for_prediction_fail);
				NULL);
		}
		else if (in_prediction_mode == 1)
		{
			mode_result = gpevm_Generate_Prediction_Error_Voxel_Map_With_Distance_Weight(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				p_set_pointer_pt3df,
				p_set_pointer_colors,
				&prediction_flag,//CKvVectorBool *in_prediction_flag,
				&p_svip[k],//io_set_of_voxel_idx_for_prediction->gpe(k),//CKvMatrixInt *in_voxel_idx_map_for_prediction,
				&p_svmp[k],//io_set_of_voxel_mask_for_prediction->gpe(k),//CKvMatrixBool *in_voxel_mask,
				in_searching_range,//int in_searching_range,
				in_error_threshold,//int in_error_threshold,
				&p_epvm[num_count2],//out_error_prediction_voxel_map->gpe(num_count2),//CKvMatrixIntRgb *out_error_map,
				&p_tmepf[num_count1],//out_texture_map_for_error_prediction_fail->gpe(num_count1)))//CKvMatrixInt *out_texture_map_for_prediction_fail);
				NULL);
		}

		if (!mode_result)
		{
			p_tmmepf[num_count1].cp_Copy(&p_svmp[k]);
			num_count1++; //Append to the texture map Case;
		}
		else
		{
			depot_error_maps.ap_Append(false, &p_svip[k], NULL);
			set_error_masks.ap_Append(false, &p_svmp[k], NULL);
			num_count2++; //Prediction Case;
		}
	}

	if (num_count1 != 0)
	{
		depot_error_fail_texture_map.im_Import(out_texture_map_for_error_prediction_fail);
		depot_error_fail_texture_map.cne_Change_Number_of_Elements(num_count1);
		depot_error_fail_texture_map.ex_Export(out_texture_map_for_error_prediction_fail);
		depot_error_fail_texture_map.in_Initialize();

		depot_masks.im_Import(out_texture_mask_for_error_prediction_fail);
		depot_masks.cne_Change_Number_of_Elements(num_count1);
		depot_masks.ex_Export(out_texture_mask_for_error_prediction_fail);
		depot_masks.in_Initialize();
	}

	if (num_count2 != 0)
	{
		depot_error_map.im_Import(out_error_prediction_voxel_map);
		depot_error_map.cne_Change_Number_of_Elements(num_count2);
		depot_error_map.ex_Export(out_error_prediction_voxel_map);
		depot_error_map.in_Initialize();

		set_error_masks.cne_Change_Number_of_Elements(num_count2);
		set_error_masks.ex_Export(io_set_of_voxel_mask_for_prediction);
		depot_error_maps.cne_Change_Number_of_Elements(num_count2);
		depot_error_maps.ex_Export(io_set_of_voxel_idx_for_prediction);
	}

	if (num_count1 != 0)
	{
		return false;
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::gpevm_Generate_Prediction_Error_Voxel_Map_With_Average(
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
	float *out_error_or_NULL)
//************************************************************************
{
	int error_in_yuv[3];
	int **p_idxm;
	int ww, hh, j, i;
	int invalid_pt = 0, searching_count;
	int **p_r, **p_g, **p_b;
	bool **p_mask;

	p_idxm = in_voxel_idx_map_for_prediction->mps(ww, hh);
	p_mask = in_voxel_mask->mp();

	out_error_map->c_Create(hh, ww, Kv_RgbI(0, 0, 0));
	p_r = out_error_map->mp(p_g, p_b);

	if (out_error_or_NULL != NULL) { *out_error_or_NULL = 0.f; }
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if ((p_mask[j][i] == true) && (p_idxm[j][i] != -1))
			{
				if (Get_index_and_Color_in_Block(
					(int)in_set_pointer_point3df[(int)p_idxm[j][i]].x,
					(int)in_set_pointer_point3df[(int)p_idxm[j][i]].y,
					(int)in_set_pointer_point3df[(int)p_idxm[j][i]].z,
					in_searching_range,//int in_searching_range,
					in_docube,//VCL_DoCube_X_Color *in_docube,
					in_set_pointer_colors,//CKvRgbaF *in_set_rgbaf,
					in_prediction_flag,//CKvVectorBool *in_prediction_flag,
					zz_voxel_idx_for_processing,//std::vector<int> &out_voxel_index_Memory_Already_Set,
					zz_vec_color_for_processing,//std::vector<std::vector<float>> &out_depot_colors_Memory_Already_Set,
					searching_count))//int &out_num_count);
				{
					Find_Error_With_Average(
						zz_vec_color_for_processing,//std::vector<std::vector<float>> &in_depot_colors,
						searching_count,//int in_num_size,
						&in_set_pointer_colors[(int)p_idxm[j][i]],//CKvRgbaF *in_target_colors,
						error_in_yuv);//std::vector<int> &out_error_in_yuv)

					if ((abs(error_in_yuv[0]) > in_error_threshold) ||
						(abs(error_in_yuv[1]) > in_error_threshold) ||
						(abs(error_in_yuv[2]) > in_error_threshold))
					{
						out_texture_map_for_prediction_fail->cp_Copy(in_voxel_idx_map_for_prediction);
						return false;

					}

					p_r[j][i] = error_in_yuv[0];
					p_g[j][i] = error_in_yuv[1];
					p_b[j][i] = error_in_yuv[2];

					if (out_error_or_NULL != NULL)
					{
						if (*out_error_or_NULL < error_in_yuv[0]) { *out_error_or_NULL = error_in_yuv[0]; }
						if (*out_error_or_NULL < error_in_yuv[1]) { *out_error_or_NULL = error_in_yuv[1]; }
						if (*out_error_or_NULL < error_in_yuv[2]) { *out_error_or_NULL = error_in_yuv[2]; }
					}
				}
				else
				{
					out_texture_map_for_prediction_fail->cp_Copy(in_voxel_idx_map_for_prediction);
					return false;
				}
			}
		}
	}


	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::gpevm_Generate_Prediction_Error_Voxel_Map_With_Distance_Weight(
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
	float *out_error_or_NULL)
//************************************************************************
{
	int error_in_yuv[3];
	int **p_idxm;
	int ww, hh, j, i;
	int invalid_pt = 0, searching_count;
	int **p_r, **p_g, **p_b;
	bool **p_mask;

	p_idxm = in_voxel_idx_map_for_prediction->mps(ww, hh);
	p_mask = in_voxel_mask->mp();

	out_error_map->c_Create(hh, ww, Kv_RgbI(0, 0, 0));
	p_r = out_error_map->mp(p_g, p_b);
	if (out_error_or_NULL != NULL) { *out_error_or_NULL = 0.f; }
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if ((p_mask[j][i] == true) && (p_idxm[j][i] != -1))
			{
				if (Get_index_and_Color_in_Block(
					(int)in_set_pointer_point3df[(int)p_idxm[j][i]].x,
					(int)in_set_pointer_point3df[(int)p_idxm[j][i]].y,
					(int)in_set_pointer_point3df[(int)p_idxm[j][i]].z,
					in_searching_range,//int in_searching_range,
					in_docube,//VCL_DoCube_X_Color *in_docube,
					in_set_pointer_colors,//CKvRgbaF *in_set_rgbaf,
					in_prediction_flag,//CKvVectorBool *in_prediction_flag,
					zz_voxel_idx_for_processing,//std::vector<int> &out_voxel_index_Memory_Already_Set,
					zz_vec_color_for_processing,//std::vector<std::vector<float>> &out_depot_colors_Memory_Already_Set,
					searching_count))//int &out_num_count);
				{
					/**************************************************************/
					Find_Error_With_Distance_Weight_RGB(
						zz_vec_color_for_processing,//std::vector<std::vector<float>> &in_depot_colors,
						zz_voxel_idx_for_processing,//std::vector<int> &in_voxel_index_Memory_Already_Set,
						zz_weight_for_processing,//std::vector<float> &in_wiehgt_Memory_Already_Set,
						in_set_pointer_point3df,//CKvPoint3Df *in_pointer_point3d,
						&in_set_pointer_colors[(int)p_idxm[j][i]],//CKvRgbaF *in_target_colors,
						&in_set_pointer_point3df[(int)p_idxm[j][i]],//CKvPoint3Df *in_target_point3d,
						searching_count,//int in_num_size,
						error_in_yuv);//int *out_error_in_yuv)

					/**************************************************************/
					//Find_Error_With_Distance_Weight(
					//	zz_vec_color_for_processing,//std::vector<std::vector<float>> &in_depot_colors,
					//	zz_voxel_idx_for_processing,//std::vector<int> &in_voxel_index_Memory_Already_Set,
					//	zz_weight_for_processing,//std::vector<float> &in_wiehgt_Memory_Already_Set,
					//	in_set_pointer_point3df,//CKvPoint3Df *in_pointer_point3d,
					//	&in_set_pointer_colors[(int)p_idxm[j][i]],//CKvRgbaF *in_target_colors,
					//	&in_set_pointer_point3df[(int)p_idxm[j][i]],//CKvPoint3Df *in_target_point3d,
					//	searching_count,//int in_num_size,
					//	error_in_yuv);//int *out_error_in_yuv)
					/**************************************************************/

					if ((abs(error_in_yuv[0]) > in_error_threshold) ||
						(abs(error_in_yuv[1]) > in_error_threshold) ||
						(abs(error_in_yuv[2]) > in_error_threshold))
					{
						out_texture_map_for_prediction_fail->cp_Copy(in_voxel_idx_map_for_prediction);
						return false;

					}

					p_r[j][i] = error_in_yuv[0];
					p_g[j][i] = error_in_yuv[1];
					p_b[j][i] = error_in_yuv[2];

					if (out_error_or_NULL != NULL)
					{
						if (*out_error_or_NULL < error_in_yuv[0]) { *out_error_or_NULL = error_in_yuv[0]; }
						if (*out_error_or_NULL < error_in_yuv[1]) { *out_error_or_NULL = error_in_yuv[1]; }
						if (*out_error_or_NULL < error_in_yuv[2]) { *out_error_or_NULL = error_in_yuv[2]; }
					}
					/**************************************************************/
				}
				else
				{
					out_texture_map_for_prediction_fail->cp_Copy(in_voxel_idx_map_for_prediction);
					return false;
				}
			}
		}
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::Get_index_and_Color_in_Block(
	int in_xx, int in_yy, int in_zz,
	int in_searching_range,
	VCL_DoCube_X_Color *in_docube,
	CKvRgbaF *in_set_pointer_colors,
	CKvVectorBool *in_prediction_flag,
	std::vector<int> &out_voxel_index_Memory_Already_Set,
	std::vector<std::vector<float>> &out_depot_colors_Memory_Already_Set,
	int &out_num_count)
//************************************************************************
{
	int ww, hh, dd, j, i, k;
	int minx, maxx, miny, maxy, minz, maxz;
	int idx; bool *p_pred_flag;
	int count = 0;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	maxx = ((in_xx + in_searching_range) >= ww) ? (ww - 1) : (in_xx + in_searching_range);
	minx = ((in_xx - in_searching_range) < 0) ? 0 : (in_xx - in_searching_range);

	maxy = ((in_yy + in_searching_range) >= hh) ? (hh - 1) : (in_yy + in_searching_range);
	miny = ((in_yy - in_searching_range) < 0) ? 0 : (in_yy - in_searching_range);

	maxz = ((in_zz + in_searching_range) >= dd) ? (dd - 1) : (in_zz + in_searching_range);
	minz = ((in_zz - in_searching_range) < 0) ? 0 : (in_zz - in_searching_range);

	/**************************************/
	p_pred_flag = in_prediction_flag->vp();

	for (j = miny; j <= maxy; j++)
	{
		for (i = minx; i <= maxx; i++)
		{
			for (k = minz; k <= maxz; k++)
			{
				idx = in_docube->gvi_Get_Voxel_Index(i, j, k);
				if (p_pred_flag[(int)idx] == false && (idx != -1))
				{
					out_voxel_index_Memory_Already_Set[count] = idx;
					out_depot_colors_Memory_Already_Set[count][0] = in_set_pointer_colors[(int)idx].r;
					out_depot_colors_Memory_Already_Set[count][1] = in_set_pointer_colors[(int)idx].g;
					out_depot_colors_Memory_Already_Set[count][2] = in_set_pointer_colors[(int)idx].b;
					count++;
				}
			}
		}
	}

	if (count == 0)
	{
		return false;
	}

	out_num_count = count;

	return true;
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Error_With_Average(
	std::vector<std::vector<float>> &in_depot_colors,
	int in_num_size,
	CKvRgbaF *in_target_colors,
	int *out_error_in_yuv)
//************************************************************************
{
	float target_yuv[3], prediction_yuv[3];
	float target_rgb[3], tmp_color[3];
	int k, num_p;

	//num_p = in_depot_colors.size();
	num_p = in_num_size;
	tmp_color[0] = tmp_color[1] = tmp_color[2] = 0.f;

	for (k = 0; k < num_p; k++)
	{
		tmp_color[0] += in_depot_colors[k][0];
		tmp_color[1] += in_depot_colors[k][1];
		tmp_color[2] += in_depot_colors[k][2];
	}

	tmp_color[0] /= (float)num_p;
	tmp_color[1] /= (float)num_p;
	tmp_color[2] /= (float)num_p;

	RGBtoYUV_BT709(tmp_color, prediction_yuv);

	target_rgb[0] = in_target_colors->r;
	target_rgb[1] = in_target_colors->g;
	target_rgb[2] = in_target_colors->b;

	RGBtoYUV_BT709(target_rgb, target_yuv);

	out_error_in_yuv[0] = (int)(round(255.f*(target_yuv[0] - prediction_yuv[0])));
	out_error_in_yuv[1] = (int)(round(255.f*(target_yuv[1] - prediction_yuv[1])));
	out_error_in_yuv[2] = (int)(round(255.f*(target_yuv[2] - prediction_yuv[2])));
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Error_With_Distance_Weight(
	std::vector<std::vector<float>> &in_depot_colors,
	std::vector<int> &in_voxel_index_Memory_Already_Set,
	std::vector<float> &in_wiehgt_Memory_Already_Set,
	CKvPoint3Df *in_pointer_point3d,
	CKvRgbaF *in_target_colors,
	CKvPoint3Df *in_target_point3d,
	int in_num_size,
	int *out_error_in_yuv)
//************************************************************************
{
	float target_yuv[3], prediction_yuv[3];
	float target_rgb[3], tmp_color[3];
	float summation = 0.f;
	int k, num_p;

	num_p = in_num_size;

	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] =
			1.f / in_target_point3d->d_Distance(in_pointer_point3d[(int)in_voxel_index_Memory_Already_Set[k]]);
		summation += in_wiehgt_Memory_Already_Set[k];
	}
	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] /= summation;
	}
	tmp_color[0] = tmp_color[1] = tmp_color[2] = 0.f;
	for (k = 0; k < num_p; k++)
	{
		tmp_color[0] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][0];
		tmp_color[1] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][1];
		tmp_color[2] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][2];
	}

	RGBtoYUV_BT709(tmp_color, prediction_yuv);
	target_rgb[0] = in_target_colors->r;
	target_rgb[1] = in_target_colors->g;
	target_rgb[2] = in_target_colors->b;
	RGBtoYUV_BT709(target_rgb, target_yuv);

	out_error_in_yuv[0] = (int)(round(255.f*(target_yuv[0] - prediction_yuv[0])));
	out_error_in_yuv[1] = (int)(round(255.f*(target_yuv[1] - prediction_yuv[1])));
	out_error_in_yuv[2] = (int)(round(255.f*(target_yuv[2] - prediction_yuv[2])));
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Find_Error_With_Distance_Weight_RGB(
	std::vector<std::vector<float>> &in_depot_colors,
	std::vector<int> &in_voxel_index_Memory_Already_Set,
	std::vector<float> &in_wiehgt_Memory_Already_Set,
	CKvPoint3Df *in_pointer_point3d,
	CKvRgbaF *in_target_colors,
	CKvPoint3Df *in_target_point3d,
	int in_num_size,
	int *out_error_in_rgb)
//************************************************************************
{
	float target_rgb[3], prediction_color[3];
	float summation = 0.f;
	int k, num_p;

	num_p = in_num_size;

	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] =
			1.f / in_target_point3d->d_Distance(in_pointer_point3d[(int)in_voxel_index_Memory_Already_Set[k]]);
		summation += in_wiehgt_Memory_Already_Set[k];
	}
	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] /= summation;
	}
	prediction_color[0] = prediction_color[1] = prediction_color[2] = 0.f;
	for (k = 0; k < num_p; k++)
	{
		prediction_color[0] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][0];
		prediction_color[1] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][1];
		prediction_color[2] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][2];
	}

	target_rgb[0] = in_target_colors->r;
	target_rgb[1] = in_target_colors->g;
	target_rgb[2] = in_target_colors->b;

	out_error_in_rgb[0] = (int)(round(255.f*(target_rgb[0] - prediction_color[0])));
	out_error_in_rgb[1] = (int)(round(255.f*(target_rgb[1] - prediction_color[1])));
	out_error_in_rgb[2] = (int)(round(255.f*(target_rgb[2] - prediction_color[2])));
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Prediction_With_Average(
	std::vector<std::vector<float>> &in_depot_colors,
	int in_num_count,
	float *out_prediction_in_yuv_3_dim)
//************************************************************************
{
	float tmp_color[3];
	int k, num_p;

	num_p = in_num_count;
	tmp_color[0] = tmp_color[1] = tmp_color[2] = 0.f;

	for (k = 0; k < num_p; k++)
	{
		tmp_color[0] += in_depot_colors[k][0];
		tmp_color[1] += in_depot_colors[k][1];
		tmp_color[2] += in_depot_colors[k][2];
	}

	tmp_color[0] /= (float)num_p;
	tmp_color[1] /= (float)num_p;
	tmp_color[2] /= (float)num_p;

	RGBtoYUV_BT709(tmp_color, out_prediction_in_yuv_3_dim);

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Prediction_With_Distance_Weight(
	std::vector<std::vector<float>> &in_depot_colors,
	std::vector<int> &in_voxel_index_Memory_Already_Set,
	std::vector<float> &in_wiehgt_Memory_Already_Set,
	CKvPoint3Df *in_pointer_point3d,
	CKvPoint3Df *in_target_point3d,
	int in_num_size,
	float *out_prediction_in_yuv_3_dim)
//************************************************************************
{
	float tmp_color[3];
	float summation = 0.f;
	int k, num_p;

	num_p = in_num_size;

	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] =
			1.f / in_target_point3d->d_Distance(in_pointer_point3d[(int)in_voxel_index_Memory_Already_Set[k]]);
		summation += in_wiehgt_Memory_Already_Set[k];
	}
	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] /= summation;
	}
	tmp_color[0] = tmp_color[1] = tmp_color[2] = 0.f;
	for (k = 0; k < num_p; k++)
	{
		tmp_color[0] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][0];
		tmp_color[1] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][1];
		tmp_color[2] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][2];
	}

	RGBtoYUV_BT709(tmp_color, out_prediction_in_yuv_3_dim);

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Prediction_With_Distance_Weight_RGB(
	std::vector<std::vector<float>> &in_depot_colors,
	std::vector<int> &in_voxel_index_Memory_Already_Set,
	std::vector<float> &in_wiehgt_Memory_Already_Set,
	CKvPoint3Df *in_pointer_point3d,
	CKvPoint3Df *in_target_point3d,
	int in_num_size,
	float *out_prediction_in_rgb_3_dim)
//************************************************************************
{
	float summation = 0.f;
	int k, num_p;

	num_p = in_num_size;

	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] =
			1.f / in_target_point3d->d_Distance(in_pointer_point3d[(int)in_voxel_index_Memory_Already_Set[k]]);
		summation += in_wiehgt_Memory_Already_Set[k];
	}
	for (k = 0; k < num_p; k++)
	{
		in_wiehgt_Memory_Already_Set[k] /= summation;
	}
	out_prediction_in_rgb_3_dim[0] = out_prediction_in_rgb_3_dim[1] = out_prediction_in_rgb_3_dim[2] = 0.f;
	for (k = 0; k < num_p; k++)
	{
		out_prediction_in_rgb_3_dim[0] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][0];
		out_prediction_in_rgb_3_dim[1] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][1];
		out_prediction_in_rgb_3_dim[2] += in_wiehgt_Memory_Already_Set[k] * in_depot_colors[k][2];
	}
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Convert_To_Uchar(
	CKvMatrixIntRgb *in_error_map,
	CKvMatrixUcharRgb *out_error_map)
//************************************************************************
{
	int **pr, **pg, **pb;
	unsigned char **pr8, **pg8, **pb8;
	int ww, hh, j, i;

	in_error_map->ms(ww, hh);
	pr = in_error_map->mp(pg, pb);

	out_error_map->c_Create(hh, ww, Kv_Rgb(0, 0, 0));
	pr8 = out_error_map->mp(pg8, pb8);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			pr8[j][i] = (unsigned char)limit((float)(pr[j][i] + 255) / 2.f);
			pg8[j][i] = (unsigned char)limit((float)(pg[j][i] + 255) / 2.f);
			pb8[j][i] = (unsigned char)limit((float)(pb[j][i] + 255) / 2.f);
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Convert_To_Int(
	CKvMatrixUcharRgb *in_error_map,
	CKvMatrixIntRgb *out_error_map)
//************************************************************************
{
	int **pr, **pg, **pb;
	unsigned char **pr8, **pg8, **pb8;
	int ww, hh, j, i;
	int R, G, B;

	in_error_map->ms(ww, hh);
	pr8 = in_error_map->mp(pg8, pb8);

	out_error_map->c_Create(hh, ww);
	pr = out_error_map->mp(pg, pb);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			R = pr8[j][i] * 2;
			G = pg8[j][i] * 2;
			B = pb8[j][i] * 2;

			pr[j][i] = (int)(R - 255);
			pg[j][i] = (int)(G - 255);
			pb[j][i] = (int)(B - 255);
		}
	}

}
//************************************************************************












//************************************************************************
void VCL_Voxel_Color_Final_Version::pvm_Packing_Voxel_Maps(
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
	CKvMatrixBool *out_error_map_mask)
//************************************************************************
{
	if (zz_alignment_mode == 2)
	{
		gpvm_Generate_Packing_Voxel_Map_For_Alignment_Mode3(
			in_set_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
			in_set_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
			&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors_or_NULL,
			in_block_size,//int in_block_size,
			in_shape_coding_mode_on_off,//bool in_shape_coding_mode_on_off,
			in_types_of_encoder,//char in_types_of_encoder,
			in_image_fixed,//bool in_image_fixed,
			false,//bool in_pack_flexible,
			out_blob_position,//CKvMatrixInt *out_positions_voxel_blob,
			out_voxel_map,//CKvMatrixUcharRgb *out_voxel_map,
			out_voxel_map_idx,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
			out_voxel_map_mask);//CKvMatrixBool *out_mask);
	}
	else
	{
		gpvm_Generate_Packing_Voxel_Map_For_Image_Maximum_Margin(
			in_set_voxel_map_idx,//CKvSet_of_MatrixInt *in_set_voxel_maps,
			in_set_voxel_map_idx_mask,//CKvSet_of_MatrixBool *in_set_masks,
			&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_colors,
			in_block_size,//int in_block_size,
			in_shape_coding_mode_on_off,//bool in_shape_coding_mode_on_off,
			in_types_of_encoder,//char in_types_of_encoder,
			in_image_fixed,//bool in_image_fixed,
			true,//bool in_pack_flexible,
			out_blob_position,//CKvMatrixInt *out_positions_voxel_blob,
			out_voxel_map,//CKvMatrixUcharRgb *out_voxel_map,
			out_voxel_map_idx,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
			out_voxel_map_mask);//CKvMatrixBool *out_mask)
	}

	gpvm_Generate_Packing_Voxel_Map_For_Error_Image(
		in_set_error_map_idx,//CKvSet_of_MatrixInt *in_set_error_map_index,
		in_set_error_map_mask,//CKvSet_of_MatrixBool *in_set_masks,
		in_set_error_map,//CKvSet_of_MatrixIntRgb *in_set_error_maps,
		in_block_size,//int in_block_size,
		in_shape_coding_mode_on_off,//bool in_shape_coding_mode_on_off,
		in_types_of_encoder,//char in_types_of_encoder,
		out_error_map,//CKvMatrixIntRgb *out_error_map,
		out_error_map_idx,//CKvMatrixInt *out_error_map_idx_or_NULL,
		out_error_map_mask);//CKvMatrixBool *out_mask)
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::gpvm_Generate_Packing_Voxel_Map_For_Image_Maximum_Margin(
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
	CKvMatrixBool *out_mask)
//************************************************************************
{
	if (in_types_of_encoder == 0)
	{
		printf("	[gsci_Get_Shape_Coding_Image_EI]\n");
		gsci_Get_Shape_Coding_Image_EI(
			in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
			in_set_masks,//CKvSet_of_MatrixBool *in_set_voxel_map_masks,
			in_block_size,//int &in_block_x,
			in_block_size,//int &in_block_y,
			out_voxel_map_idx_or_NULL,//CKvMatrixInt *out_EI_image,
			out_mask);//CKvMatrixBool *out_mask)
		CKvSet_of_RgbaF set_rgbaf; in_colors_or_NULL->e_Export(&set_rgbaf);
		CKvRgbaF *p_rgbaf = set_rgbaf.vp();
		int ww, hh, j, i; unsigned char **pr, **pg, **pb;

		int **p_idx = out_voxel_map_idx_or_NULL->mps(ww, hh);
		out_voxel_map->c_Create(hh, ww, Kv_Rgb(0, 0, 0));
		pr = out_voxel_map->mp(pg, pb);

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if ((int)p_idx[j][i] != -1)
				{
					pr[j][i] = limit(round(p_rgbaf[(int)p_idx[j][i]].r*255.f));
					pg[j][i] = limit(round(p_rgbaf[(int)p_idx[j][i]].g*255.f));
					pb[j][i] = limit(round(p_rgbaf[(int)p_idx[j][i]].b*255.f));
				}
			}
		}
	}
	else
	{
		gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
			in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
			in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
			in_colors_or_NULL,//CKvDepot_of_RgbaF *in_colors_or_NULL,
			in_block_size,//int in_block_size,
			in_shape_coding_mode_on_off,//bool in_line_ei_mode_on_true_off_false,
			in_image_fixed,//bool in_image_fixed,
			in_pack_flexible,//bool in_pack_flexible,
			out_positions_voxel_blob,//CKvMatrixInt *out_position_of_segmented_voxe_map,
			out_voxel_map,//CKvMatrixUcharRgb *out_voxel_map,
			out_voxel_map_idx_or_NULL,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
			out_mask);//CKvMatrixBool *out_mask)

		/************* Ver 1. ************/
		//gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
		//	in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		//	in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		//	in_colors_or_NULL,//CKvDepot_of_RgbaF *in_colors_or_NULL,
		//	in_block_size,//int in_block_size,
		//	in_types_of_encoder,//char in_types_of_encoder,
		//	in_image_fixed,//bool in_image_fixed,
		//	in_pack_flexible,//bool in_pack_flexible,
		//	out_positions_voxel_blob,//CKvMatrixInt *out_position_of_segmented_voxe_map,
		//	out_voxel_map,//CKvMatrixUcharRgb *out_voxel_map,
		//	out_voxel_map_idx_or_NULL,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		//	out_mask);//CKvMatrixBool *out_mask)
		//if (in_shape_coding_mode_on_off)
		//{
		//	CKvMatrixUcharRgb voxel_blob, cpy_blob;
		//	CKvMatrixBool blob_mask;
		//	int num_blob = in_set_voxel_maps->vs();
		//	int k;
		//	int **p_position = out_positions_voxel_blob->mp();
		//	for (k = 0; k < num_blob; k++)
		//	{
		//		out_voxel_map->gb_Get_Block(
		//			p_position[k][0],
		//			p_position[k][1],
		//			in_set_voxel_maps->gpe(k)->mw(),
		//			in_set_voxel_maps->gpe(k)->mh(),
		//			&voxel_blob);
		//		out_mask->gb_Get_Block(
		//			p_position[k][0],
		//			p_position[k][1],
		//			in_set_voxel_maps->gpe(k)->mw(),
		//			in_set_voxel_maps->gpe(k)->mh(),
		//			&blob_mask);
		//		EI_DCT_Line_Forward_on_Spatial_Domain(
		//			&voxel_blob,//CKvMatrixUcharRgb *io_img,
		//			&blob_mask);//CKvMatrixBool *in_mask);
		//		out_voxel_map->sb_Set_Block(
		//			p_position[k][0],
		//			p_position[k][1],
		//			&voxel_blob);
		//	}
		//}
		/*****************************************************************/
	}
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::gpvm_Generate_Packing_Voxel_Map_For_Error_Image(
	CKvSet_of_MatrixInt *in_set_error_map_index,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvSet_of_MatrixIntRgb *in_set_error_maps,
	int in_block_size,
	bool in_shape_coding_mode_on_off,
	char in_types_of_encoder,
	CKvMatrixIntRgb *out_error_map,
	CKvMatrixInt *out_error_map_idx_or_NULL,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixInt pts;
	
	// Generate Voxel Error Map
	pvm_Pack_Voxel_Map_With_Filling(
		in_set_error_maps,//CKvSet_of_MatrixIntRgb *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_block_size,//int in_block_size,
		in_shape_coding_mode_on_off,//bool in_shape_coding_mode,
		&pts,//CKvMatrixInt *out_position_of_segmented_voxe_map,
		out_error_map,//CKvMatrixIntRgb *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask);

	// Generate Voxel Error Index Map
	gpvm_Generate_Packing_Voxel_Map(
		in_set_error_map_index,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		NULL,//CKvDepot_of_RgbaF *in_colors,
		in_block_size,//int in_block_size,
		false,//bool in_shape_coding_mode_on_off,
		1,//char in_types_of_encoder,
		false,//bool in_image_fixed,
		NULL,//CKvMatrixUcharRgb *out_voxel_map,
		out_error_map_idx_or_NULL,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		NULL);//CKvMatrixBool *out_mask)
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::gpvm_Generate_Packing_Voxel_Map_For_Alignment_Mode3(
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
	CKvMatrixBool *out_mask)
//************************************************************************
{
	gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_colors_or_NULL,//CKvDepot_of_RgbaF *in_colors_or_NULL,
		in_block_size,//int in_block_size,
		in_types_of_encoder,//char in_types_of_encoder,
		in_image_fixed,//bool in_image_fixed,
		in_pack_flexible,//bool in_pack_flexible,
		out_positions_voxel_blob,//CKvMatrixInt *out_position_of_segmented_voxe_map,
		out_voxel_map,//CKvMatrixUcharRgb *out_voxel_map,
		out_voxel_map_idx_or_NULL,//CKvMatrixInt *out_voxel_map_idx_or_NULL,
		out_mask);//CKvMatrixBool *out_mask)

	LCKvUtility_for_Xrunset lux;
	CKvXrunsetShort xrunset1, xrunset2;
	CKvSet_of_XvectorShort xrunset1d; CKvXvectorShort *p_x;
	CKvMatrixUcharRgb voxel_blob, cpy_blob;
	CKvMatrixBool blob_mask;
	unsigned char **pr, **pg, **pb;
	int num_blob = in_set_voxel_maps->vs();
	int k, m, n, z, np; short *a, zo, z1;
	int idx1, idx2;
	float d1, d2;

	int **p_position = out_positions_voxel_blob->mp();
	for (k = 0; k < num_blob; k++)
	{
		out_voxel_map->gb_Get_Block(
			p_position[k][0],
			p_position[k][1],
			in_set_voxel_maps->gpe(k)->mw(),
			in_set_voxel_maps->gpe(k)->mh(),
			&voxel_blob);

		out_mask->gb_Get_Block(
			p_position[k][0],
			p_position[k][1],
			in_set_voxel_maps->gpe(k)->mw(),
			in_set_voxel_maps->gpe(k)->mh(),
			&blob_mask);

		xrunset1.im_Import(blob_mask);
		lux.com_Complement(xrunset1, xrunset2);
		xrunset2.ex_Export(xrunset1d, NULL);

		xrunset2.ex_Export(true, false, blob_mask);

		p_x = xrunset1d.vp();
		pr = voxel_blob.mp(pg, pb);
		for (m = 0; m < in_set_voxel_maps->gpe(k)->mh(); m++)
		{
			p_x[m].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			for (n = 0; n < np;)
			{
				zo = a[n]; n++;
				z1 = a[n]; n++;

				if (zo == 0) { idx1 = 0; }
				else if (zo == in_set_voxel_maps->gpe(k)->mw() - 1) { idx1 = in_set_voxel_maps->gpe(k)->mw() - 2; }
				else { idx1 = zo - 1; }
				if (z1 == in_set_voxel_maps->gpe(k)->mw() - 1) { idx2 = in_set_voxel_maps->gpe(k)->mw() - 1; }
				else if (z1 == 0) { idx2 = 1; }
				else { idx2 = z1 + 1; }

				for (z = zo; z <= z1; z++)
				{
					d1 = abs(idx1 - z); d2 = abs(idx2 - z);
					pr[m][z] = (d2 / (d1 + d2))*(float)pr[m][idx1] + (d1 / (d1 + d2))*(float)pr[m][idx2];
					pg[m][z] = (d2 / (d1 + d2))*(float)pg[m][idx1] + (d1 / (d1 + d2))*(float)pg[m][idx2];
					pb[m][z] = (d2 / (d1 + d2))*(float)pb[m][idx1] + (d1 / (d1 + d2))*(float)pb[m][idx2];
				}
			}
		}

		out_voxel_map->sb_Set_Block(
			p_position[k][0],
			p_position[k][1],
			&voxel_blob);
	}

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::gpvm_Generate_Packing_Voxel_Map(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvDepot_of_RgbaF *in_colors_or_NULL,
	int in_block_size,
	bool in_shape_coding_mode_on_off,
	char in_types_of_encoder,
	bool in_image_fixed,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixInt *out_voxel_map_idx_or_NULL,
	CKvMatrixBool *out_mask_or_NULL)
//************************************************************************
{
	CKvMatrixInt tmp; CKvMatrixInt idx_map; CKvMatrixBool mask;
	CKvSet_of_RgbaF set_colors; CKvRgbaF *p_colors;
	int ww, hh, j, i; unsigned char **pr, **pg, **pb;
	int **p_idx; bool **p_mask;

	pvm_Pack_Voxel_Map_With_Filling(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_block_size,//int in_block_size,
		in_shape_coding_mode_on_off,//bool in_shape_coding_mode,
		&tmp,//CKvMatrixInt *out_position_of_segmented_voxe_map,
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
void VCL_Voxel_Color_Final_Version::gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
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
	CKvMatrixBool *out_mask_or_NULL)
//************************************************************************
{
	CKvMatrixInt idx_map; CKvMatrixBool mask;
	CKvSet_of_RgbaF set_colors; CKvRgbaF *p_colors;
	int ww, hh, j, i; unsigned char **pr, **pg, **pb;
	int **p_idx; bool **p_mask;

	Resize_Voxel_Blobs(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
		in_set_masks,//CKvSet_of_MatrixBool *io_set_voxel_masks,
		in_block_size);//int in_block_size)

	pvm_Pack_Voxel_Map_With_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_block_size,//int in_block_size,
		in_image_fixed,//bool in_image_fixed,
		in_pack_flexible,//bool in_pack_flexible,
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
		//CKvScreen sc;
		//sc.s_d_Display(out_voxel_map);
		//Kv_Printf("hh = %d ww = %d\n", out_voxel_map->mh(), out_voxel_map->mw());
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
void VCL_Voxel_Color_Final_Version::gpvm_Generate_Packing_Voxel_Map_Maximum_Margin(
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
		
		//CKvScreen sc;
		//sc.s_d_Display(out_voxel_map);
		//Kv_Printf("~~~");


		if (in_line_ei_mode_on_true_off_false)
		{
			CKvMatrixUcharRgb tmp_blob;
			CKvMatrixInt tmp_idx_map;
			int num_blob, dum, k;
			int **p_psvm = out_position_of_segmented_voxe_map->mps(dum, num_blob);
			for (k = 0; k < num_blob; k++)
			{
				out_voxel_map->gb_Get_Block(
					p_psvm[k][0],
					p_psvm[k][1],
					p_psvm[k][2],
					p_psvm[k][3],
					&tmp_blob);

				EI_DCT_Line_Forward_on_Spatial_Domain(
					&tmp_blob,//CKvMatrixInt *io_img,
					in_set_masks->gpe(k));//CKvMatrixBool *in_mask)

				out_voxel_map->sb_Set_Block(
					p_psvm[k][0],
					p_psvm[k][1], 
					&tmp_blob);

				idx_map.gb_Get_Block(
					p_psvm[k][0],
					p_psvm[k][1],
					p_psvm[k][2],
					p_psvm[k][3],
					&tmp_idx_map);

				EI_DCT_Line_Forward_on_Spatial_Domain(
					&tmp_idx_map,//CKvMatrixInt *io_img,
					in_set_masks->gpe(k));//CKvMatrixBool *in_mask)

				idx_map.sb_Set_Block(
					p_psvm[k][0],
					p_psvm[k][1],
					&tmp_idx_map);
			}
		}
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
void VCL_Voxel_Color_Final_Version::Get_Voxel_Colors_From_Texture_Map(
	CKvMatrixUcharRgb *in_voxel_map,
	CKvMatrixBool *in_voxel_map_mask,
	CKvMatrixInt *in_voxel_map_index,
	int in_number_of_voxels,
	CKvDepot_of_RgbaF *out_colors,
	CKvVectorBool *out_flag)
//************************************************************************
{
	CKvSet_of_RgbaF set_rgbaf; CKvRgbaF *p_rgbaf;
	unsigned char **pr, **pg, **pb;
	bool **p_mask, *p_flag;
	int **p_idx;
	int j, i, ww, hh;

	p_idx = in_voxel_map_index->mps(ww,hh);
	pr = in_voxel_map->mp(pg, pb);
	p_mask = in_voxel_map_mask->mp();

	p_flag  = out_flag->c_Create(in_number_of_voxels, true);
	p_rgbaf = set_rgbaf.c_Create(in_number_of_voxels);
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_idx[j][i] != -1)
			{
				p_rgbaf[p_idx[j][i]].r = (float)(pr[j][i]) / 255.f;
				p_rgbaf[p_idx[j][i]].g = (float)(pg[j][i]) / 255.f;
				p_rgbaf[p_idx[j][i]].b = (float)(pb[j][i]) / 255.f;
				p_rgbaf[p_idx[j][i]].a = 1.f;
				p_flag[p_idx[j][i]] = false;
			}
		}
	}

	out_colors->im_Import(&set_rgbaf);
}
//************************************************************************
void VCL_Voxel_Color_Final_Version::Get_Voxel_Colors_From_Texture_Map(
	CKvMatrixUcharRgb *in_voxel_map,
	CKvMatrixBool *in_voxel_map_mask,
	CKvMatrixInt *in_voxel_map_index,
	int in_number_of_voxels,
	CKvSet_of_RgbaF *out_colors)
//************************************************************************
{
	CKvRgbaF *p_rgbaf;
	unsigned char **pr, **pg, **pb;
	bool **p_mask;
	int **p_idx;
	int j, i, ww, hh;

	in_voxel_map->ms(ww, hh);
	pr = in_voxel_map->mp(pg, pb);
	p_mask = in_voxel_map_mask->mp();
	p_idx = in_voxel_map_index->mp();

	p_rgbaf = out_colors->c_Create(in_number_of_voxels);
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_idx[j][i] != -1)
			{
				p_rgbaf[p_idx[j][i]].r = (float)(pr[j][i]) / 255.f;
				p_rgbaf[p_idx[j][i]].g = (float)(pg[j][i]) / 255.f;
				p_rgbaf[p_idx[j][i]].b = (float)(pb[j][i]) / 255.f;
			}
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::RGBtoYUV_BT709(
	float *in_target_rgb,
	float *out_yuv)
//************************************************************************
{
	float CMat[3][3];
	int j, i;

	CMat[0][0] = 0.2126;
	CMat[0][1] = 0.7152;
	CMat[0][2] = 0.0722;

	CMat[1][0] = -0.1146;
	CMat[1][1] = -0.3854;
	CMat[1][2] = 0.5000;

	CMat[2][0] = 0.5000;
	CMat[2][1] = -0.4542;
	CMat[2][2] = -0.0458;

	out_yuv[0] = out_yuv[1] = out_yuv[2] = 0.f;
	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 3; i++)
		{
			out_yuv[j] += CMat[j][i] * in_target_rgb[i];
		}
	}

	out_yuv[1] += (0.5);
	out_yuv[2] += (0.5);

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::YUVtoRGB_BT709(
	float *in_target_yuv,
	float *out_rgb)
//************************************************************************
{
	float CMat[3][3];
	int j, i;

	CMat[0][0] = 1;
	CMat[0][1] = -0.000151500715093214;
	CMat[0][2] = 1.57476527603610;

	CMat[1][0] = 1;
	CMat[1][1] = -0.187280215689950;
	CMat[1][2] = -0.468124625436137;

	CMat[2][0] = 1;
	CMat[2][1] = 1.85560968578229;
	CMat[2][2] = 0.000105739981300146;

	out_rgb[0] = out_rgb[1] = out_rgb[2] = 0.f;

	in_target_yuv[1] -= (0.5);
	in_target_yuv[2] -= (0.5);

	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 3; i++)
		{
			out_rgb[j] += CMat[j][i] * in_target_yuv[i];
		}
	}
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::sim_Save_Index_Map(
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
bool VCL_Voxel_Color_Final_Version::sim_Save_Index_Map(
	CKvString str,
	CKvVectorInt *in_index_map)
//************************************************************************
{
	FILE *fp = NULL;
	int sz;

	sz = in_index_map->vs();

	fopen_s(&fp, (char *)str, "wb");
	if (fp == NULL)
	{
		return false;
	}

	fwrite(&sz, sizeof(int), 1, fp);

	int *p = in_index_map->vp();

	fwrite(p, sizeof(int), sz, fp);

	fclose(fp);
	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::sap_Save_Align_Parameters(
	CKvString str,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	int in_blob_size_threshold,
	CKvSet_of_VectorInt *in_circular_shift,
	CKvSet_of_VectorInt *in_offset)
//************************************************************************
{
	CKvSet_of_VectorInt tmp1, tmp2;
	FILE *fp = NULL;
	int num_blob, num_slice;
	int k, l;
	int num_voxel;

	fopen_s(&fp, (char *)str, "wb");
	if (fp == NULL)
	{
		return false;
	}

	tmp1.cp_Copy(in_circular_shift);
	tmp2.cp_Copy(in_offset);

	num_blob = io_segmented_texture_on_code.size();
	CKvVectorInt *p1 = tmp1.vp();
	CKvVectorInt *p2 = tmp2.vp();
	for (k = 0; k < num_blob; k++)
	{
		num_slice = io_segmented_texture_on_code[k].size();
		num_voxel = 0;
		for (l = 0; l < num_slice; l++)
		{
			num_voxel += io_segmented_texture_on_code[k][l].size();
		}

		if (in_blob_size_threshold < num_voxel)
		{
			int *pp1 = p1[k].vp();
			//int *pp2 = p2[k].vp();

			//for (l = 0; l < p1[k].vs()-1; l++)
			//{
			//	pp1[l + 1] = pp1[l + 1] - pp1[l];
			//}

			//for (l = 0; l < p2[k].vs() - 1; l++)
			//{
			//	pp2[l + 1] = pp2[l + 1] - pp2[l];
			//}

			fwrite(pp1, sizeof(short), p1[k].vs(), fp);
			//fwrite(pp2, sizeof(short), p2[k].vs(), fp);
		}
	}

	for (k = 0; k < num_blob; k++)
	{
		num_slice = io_segmented_texture_on_code[k].size();
		num_voxel = 0;
		for (l = 0; l < num_slice; l++)
		{
			num_voxel += io_segmented_texture_on_code[k][l].size();
		}

		if (in_blob_size_threshold < num_voxel)
		{
			//int *pp1 = p1[k].vp();
			int *pp2 = p2[k].vp();

			//for (l = 0; l < p1[k].vs()-1; l++)
			//{
			//	pp1[l + 1] = pp1[l + 1] - pp1[l];
			//}

			//for (l = 0; l < p2[k].vs() - 1; l++)
			//{
			//	pp2[l + 1] = pp2[l + 1] - pp2[l];
			//}

			//fwrite(pp1, sizeof(short), p1[k].vs(), fp);
			fwrite(pp2, sizeof(short), p2[k].vs(), fp);
		}
	}




	fclose(fp);
	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::sv_Save_Vector(
	CKvString str,
	std::vector<int> &in_vector)
//************************************************************************
{
	FILE *fp = NULL;
	int sz, *p;

	fopen_s(&fp, (char *)str, "wb");
	if (fp == NULL)
	{
		return false;
	}

	sz = in_vector.size();
	p = in_vector.data();

	fwrite(&sz, sizeof(int), 1, fp);
	fwrite(p, sizeof(int), sz, fp);

	fclose(fp);
	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::lim_Load_Index_Map(
	CKvString str,
	CKvMatrixInt *in_index_map)
//************************************************************************
{
	FILE *fp = NULL;
	int ww, hh;

	fopen_s(&fp, (char *)str, "rb");
	if (fp == NULL)
	{
		Kv_Printf("Fail To open %s\n", (char *)str);
		return false;
	}

	fread(&hh, sizeof(int), 1, fp);
	fread(&ww, sizeof(int), 1, fp);

	in_index_map->c_Create(hh, ww);
	int *p = in_index_map->vp();

	fread(p, sizeof(int), ww*hh, fp);

	fclose(fp);

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::lim_Load_Index_Map(
	CKvString str,
	CKvVectorInt *in_index_map)
//************************************************************************
{
	FILE *fp = NULL;
	int sz;

	fopen_s(&fp, (char *)str, "rb");
	if (fp == NULL)
	{
		return false;
	}

	fread(&sz, sizeof(int), 1, fp);

	in_index_map->c_Create(sz);
	int *p = in_index_map->vp();

	fread(p, sizeof(int), sz, fp);

	fclose(fp);

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::Save_Vector_Txt(
	CKvString str,
	CKvVector *in_vector)
//************************************************************************
{
	FILE *fp = NULL;
	double *p_vec;
	int sz, k;
	
	p_vec = in_vector->vps(sz);
	fopen_s(&fp, (char *)str, "w");
	if (fp == NULL){return false;}
	for (k = 0; k < sz; k++){fprintf_s(fp, "%lf\n", p_vec[k]);}
	
	fclose(fp);

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::Get_PSNR(
	CKvDepot_of_RgbaF *in_depot_color1,
	CKvDepot_of_RgbaF *in_depot_color2,
	bool in_mode_true_yuv_or_false_rgb,
	double &out_channel_Y_or_R,
	double &out_channel_U_or_G,
	double &out_channel_V_or_B)
//************************************************************************
{
	CKvSet_of_RgbaF set1, set2;
	CKvRgbaF *p1, *p2;
	double C1 = 0.0, C2 = 0.0, C3 = 0.0;
	double error;
	float rgb1[3], yuv1[3], rgb2[3], yuv2[3];
	int num_v1, num_v2, k;

	num_v1 = in_depot_color1->ne_Number_of_Elements();
	num_v2 = in_depot_color2->ne_Number_of_Elements();

	if (num_v1 != num_v2) { return false; }

	in_depot_color1->e_Export(&set1);
	in_depot_color2->e_Export(&set2);

	p1 = set1.vp();
	p2 = set2.vp();

	if (in_mode_true_yuv_or_false_rgb)
	{
		for (k = 0; k < num_v1; k++)
		{
			rgb1[0] = p1[k].r;
			rgb1[1] = p1[k].g;
			rgb1[2] = p1[k].b;

			RGBtoYUV_BT709(
				rgb1,//float *in_target_rgb,
				yuv1);//float *out_yuv);

			rgb2[0] = p2[k].r;
			rgb2[1] = p2[k].g;
			rgb2[2] = p2[k].b;

			RGBtoYUV_BT709(
				rgb2,//float *in_target_rgb,
				yuv2);//float *out_yuv);

			error = (yuv1[0] - yuv2[0]);
			C1 = C1 + (error*error);
			error = (yuv1[1] - yuv2[1]);
			C2 = C2 + (error*error);
			error = (yuv1[2] - yuv2[2]);
			C3 = C3 + (error*error);
		}

		C1 /= double(num_v1);
		C2 /= double(num_v1);
		C3 /= double(num_v1);

		if (C1 != 0)
		{
			out_channel_Y_or_R = 10 * log10(1.0 / C1);
		}
		else
		{
			out_channel_Y_or_R = -1;
		}
		if (C2 != 0)
		{
			out_channel_U_or_G = 10 * log10(1.0 / C2);
		}
		else
		{
			out_channel_U_or_G = -1;
		}
		if (C3 != 0)
		{
			out_channel_V_or_B = 10 * log10(1.0 / C3);
		}
		else
		{
			out_channel_V_or_B = -1;
		}
	}
	else
	{
		for (k = 0; k < num_v1; k++)
		{
			rgb1[0] = p1[k].r;
			rgb1[1] = p1[k].g;
			rgb1[2] = p1[k].b;

			rgb2[0] = p2[k].r;
			rgb2[1] = p2[k].g;
			rgb2[2] = p2[k].b;

			error = (rgb1[0] - rgb2[0]);
			C1 = C1 + (error*error);
			error = (rgb1[1] - rgb2[1]);
			C2 = C2 + (error*error);
			error = (rgb1[2] - rgb2[2]);
			C3 = C3 + (error*error);
		}

		C1 /= double(num_v1);
		C2 /= double(num_v1);
		C3 /= double(num_v1);

		if (C1 != 0)
		{
			out_channel_Y_or_R = 10 * log10(1.0 / C1);
		}
		else
		{
			out_channel_Y_or_R = -1;
		}
		if (C2 != 0)
		{
			out_channel_U_or_G = 10 * log10(1.0 / C2);
		}
		else
		{
			out_channel_U_or_G = -1;
		}
		if (C3 != 0)
		{
			out_channel_V_or_B = 10 * log10(1.0 / C3);
		}
		else
		{
			out_channel_V_or_B = -1;
		}
	}

	return true;

}
//************************************************************************
bool VCL_Voxel_Color_Final_Version::Get_PSNR(
	CKvDepot_of_RgbaF *in_depot_color1,
	CKvDepot_of_RgbaF *in_depot_color2,
	bool in_mode_true_yuv_or_false_rgb,
	CKvMatrixInt *in_texture_index,
	CKvMatrixInt *in_prediction_index,
	std::vector<double> &out_texture_map_psnr,
	std::vector<double> &out_prediction_map_psnr)
//************************************************************************
{
	CKvSet_of_RgbaF set1, set2;
	CKvRgbaF *p1, *p2;
	double C1 = 0.0, C2 = 0.0, C3 = 0.0;
	double error;
	float rgb1[3], yuv1[3], rgb2[3], yuv2[3];
	int num_v1, num_v2, j, i;

	num_v1 = in_depot_color1->ne_Number_of_Elements();
	num_v2 = in_depot_color2->ne_Number_of_Elements();

	if (num_v1 != num_v2) { return false; }

	in_depot_color1->e_Export(&set1);
	in_depot_color2->e_Export(&set2);

	p1 = set1.vp();
	p2 = set2.vp();

	int ww1, hh1;
	int **pi1 = in_texture_index->mps(ww1, hh1);

	int ww2, hh2;
	int **pi2 = in_prediction_index->mps(ww2, hh2);

	if (in_mode_true_yuv_or_false_rgb)
	{
		num_v1 = 0;
		out_texture_map_psnr[0] = out_texture_map_psnr[1] = out_texture_map_psnr[2] = 0.0;
		for (j = 0; j < hh1; j++)
		{
			for (i = 0; i < ww1; i++)
			{
				if (pi1[j][i] != -1)
				{
					num_v1++;
					int indx = pi1[j][i];
					rgb1[0] = p1[indx].r;
					rgb1[1] = p1[indx].g;
					rgb1[2] = p1[indx].b;

					RGBtoYUV_BT709(
						rgb1,//float *in_target_rgb,
						yuv1);//float *out_yuv);

					rgb2[0] = p2[indx].r;
					rgb2[1] = p2[indx].g;
					rgb2[2] = p2[indx].b;

					RGBtoYUV_BT709(
						rgb2,//float *in_target_rgb,
						yuv2);//float *out_yuv);

					error = (yuv1[0] - yuv2[0]);
					out_texture_map_psnr[0] = out_texture_map_psnr[0] + (error*error);
					error = (yuv1[1] - yuv2[1]);
					out_texture_map_psnr[1] = out_texture_map_psnr[1] + (error*error);
					error = (yuv1[2] - yuv2[2]);
					out_texture_map_psnr[2] = out_texture_map_psnr[2] + (error*error);
				}
			}
		}
		out_texture_map_psnr[0] /= double(num_v1);
		out_texture_map_psnr[1] /= double(num_v1);
		out_texture_map_psnr[2] /= double(num_v1);

		if (out_texture_map_psnr[0] != 0)
		{
			out_texture_map_psnr[0] = 10 * log10(1.0 / out_texture_map_psnr[0]);
		}
		else
		{
			out_texture_map_psnr[0] = -1;
		}
		if (out_texture_map_psnr[1] != 0)
		{
			out_texture_map_psnr[1] = 10 * log10(1.0 / out_texture_map_psnr[1]);
		}
		else
		{
			out_texture_map_psnr[1] = -1;
		}
		if (out_texture_map_psnr[2] != 0)
		{
			out_texture_map_psnr[2] = 10 * log10(1.0 / out_texture_map_psnr[2]);
		}
		else
		{
			out_texture_map_psnr[2] = -1;
		}



		num_v2 = 0;
		out_prediction_map_psnr[0] = out_prediction_map_psnr[1] = out_prediction_map_psnr[2] = 0.0;
		for (j = 0; j < hh2; j++)
		{
			for (i = 0; i < ww2; i++)
			{
				if (pi2[j][i] != -1)
				{
					num_v2++;
					int indx = pi2[j][i];
					rgb1[0] = p1[indx].r;
					rgb1[1] = p1[indx].g;
					rgb1[2] = p1[indx].b;

					RGBtoYUV_BT709(
						rgb1,//float *in_target_rgb,
						yuv1);//float *out_yuv);

					rgb2[0] = p2[indx].r;
					rgb2[1] = p2[indx].g;
					rgb2[2] = p2[indx].b;

					RGBtoYUV_BT709(
						rgb2,//float *in_target_rgb,
						yuv2);//float *out_yuv);

					error = (yuv1[0] - yuv2[0]);
					out_prediction_map_psnr[0] = out_prediction_map_psnr[0] + (error*error);
					error = (yuv1[1] - yuv2[1]);
					out_prediction_map_psnr[1] = out_prediction_map_psnr[1] + (error*error);
					error = (yuv1[2] - yuv2[2]);
					out_prediction_map_psnr[2] = out_prediction_map_psnr[2] + (error*error);
				}
			}
		}
		out_prediction_map_psnr[0] /= double(num_v2);
		out_prediction_map_psnr[1] /= double(num_v2);
		out_prediction_map_psnr[2] /= double(num_v2);

		if (out_prediction_map_psnr[0] != 0)
		{
			out_prediction_map_psnr[0] = 10 * log10(1.0 / out_prediction_map_psnr[0]);
		}
		else
		{
			out_prediction_map_psnr[0] = -1;
		}
		if (out_prediction_map_psnr[1] != 0)
		{
			out_prediction_map_psnr[1] = 10 * log10(1.0 / out_prediction_map_psnr[1]);
		}
		else
		{
			out_prediction_map_psnr[1] = -1;
		}
		if (out_prediction_map_psnr[2] != 0)
		{
			out_prediction_map_psnr[2] = 10 * log10(1.0 / out_prediction_map_psnr[2]);
		}
		else
		{
			out_prediction_map_psnr[2] = -1;
		}



	}

	return true;

}
//************************************************************************
int VCL_Voxel_Color_Final_Version::Get_File_Size_in_Byte(
	CKvString in_filename)
//************************************************************************
{
	std::ifstream ifs((char *)in_filename, std::ios::in | std::ios::binary);

	int size = 0;
	if (ifs.is_open() == 0)
	{
		printf("Error: Can Note Access To %s\n",(char *)in_filename);
	}

	ifs.seekg(0, std::ios::end);
	size = ifs.tellg();
	ifs.close();

	return size;
}
//************************************************************************
int VCL_Voxel_Color_Final_Version::Get_Number_of_Voxels(
	CKvMatrixBool *in_mask)
//************************************************************************
{
	bool **p_mask;
	int ww, hh,j,i;
	int num_voxels=0;

	p_mask = in_mask->mps(ww, hh);
	
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_mask[j][i] == true)
			{
				num_voxels++;
			}
		}
	}

	return num_voxels;
}
//************************************************************************







//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
// This module is for experiment. 
void VCL_Voxel_Color_Final_Version::Generate_Voxel_Maps_WO_partitioning(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> in_plane_mode_sequence,
	int in_alignment_mode,
	int in_threshold_for_cluster_size,
	CKvSet_of_MatrixInt *out_set_of_voxel_map_idx,
	CKvSet_of_MatrixBool *out_set_of_voxel_map_idx_mask,
	CKvSet_of_MatrixInt *out_set_of_voxel_map_small_blob_idx,
	CKvSet_of_MatrixBool *out_set_of_voxel_map_small_blob_idx_mask)
//************************************************************************
{
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> voxel_texture_sequence;

	printf("[gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction]\n");
	gvsi_Get_Voxel_Sequence_With_Iterative_Orthogonal_Direction(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		voxel_texture_sequence);//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &out_voxel_sequence)

	printf("[gvmv_Generate_Voxel_Maps]\n");
	gvmv_Generate_Voxel_Maps_WO_partitioning(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		voxel_texture_sequence,//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
		in_alignment_mode,//int in_alignment_mode,
		out_set_of_voxel_map_idx,//CKvSet_of_MatrixInt *out_set_of_voxel_map,
		out_set_of_voxel_map_idx_mask);//CKvSet_of_MatrixBool *out_set_of_voxel_mask)

}
//************************************************************************
void VCL_Voxel_Color_Final_Version::gvmv_Generate_Voxel_Maps_WO_partitioning(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
	int in_alignment_mode,
	CKvSet_of_MatrixInt *out_set_of_voxel_map,
	CKvSet_of_MatrixBool *out_set_of_voxel_mask)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> texture_on_code;
	CKvDepot_of_MatrixInt    depot_voxel_maps;
	CKvDepot_of_MatrixBool   depot_voxel_masks;

	CKvDepot_of_MatrixInt    set_voxel_maps;
	CKvDepot_of_MatrixBool   set_voxel_map_masks;

	CKvDepot_of_MatrixInt    depot_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   depot_voxel_map_masks_for_prediction;

	CKvDepot_of_MatrixInt    set_voxel_idx_for_prediction;
	CKvDepot_of_MatrixBool   set_voxel_map_masks_for_prediction;

	CKvSet_of_VectorInt        set_offsets, set_circular_shift;
	CKvMatrixInt               position_of_segmented_voxel_map;
	int num_plane_mode, k;
	int resolution_x, resolution_y, resolution_z;

	num_plane_mode = in_voxel_sequence.size();
	in_docube->gr_Get_Resolution(resolution_x, resolution_y, resolution_z);

	depot_voxel_maps.in_Initialize();
	depot_voxel_masks.in_Initialize();
	depot_voxel_idx_for_prediction.in_Initialize();
	depot_voxel_map_masks_for_prediction.in_Initialize();

	CKvStopWatch sw;
	sw.c_Create(1);

	
	for (k = 0; k < num_plane_mode; k++)
	{
		printf("[atoc_Align_Texture_on_Code]\n");
		sw.r_Reset(0);

		atoc_Align_Texture_on_Code_WO_partitioning(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_alignment_mode,//int in_alignment_mode,
			in_voxel_sequence[k],//std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
			&set_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			&set_offsets);//CKvSet_of_VectorInt *out_offsets);

		printf("Time : %lf\n", sw.get_Get_Elapsed_Time(0));

		/**********************************************************/
		printf("[s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding]\n");
		s_gvm_Generate_Voxel_Map__wo_partitioning(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_voxel_sequence[k],//std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
			&set_offsets,//CKvSet_of_VectorInt *in_offsets,
			&set_voxel_maps,//CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
			&set_voxel_map_masks);//CKvDepot_of_MatrixBool *out_set_masks)


	}

}
//************************************************************************
