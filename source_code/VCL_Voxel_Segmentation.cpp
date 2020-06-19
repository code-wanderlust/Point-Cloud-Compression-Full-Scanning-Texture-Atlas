#include "stdafx.h"
#include "VCL_Voxel_Segmentation.h"



//************************************************************************
VCL_Voxel_Segmentation::VCL_Voxel_Segmentation()
//************************************************************************
{
	zz_threshold = 0.4f;
}
//************************************************************************
VCL_Voxel_Segmentation::~VCL_Voxel_Segmentation()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Segmentation::sp_Set_Parameter_for_Segmentation(
	float in_threshold)
//************************************************************************
{
	zz_threshold = in_threshold;
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segmentation_of_Voxel_Data(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment[3];
	std::vector<std::vector<int>> voxel_segments;
	CKvDepot_of_Point3Df depot_pt3d;
	int ww, hh, dd, plane_mode;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	/************************************************************************************/
	plane_mode = 0;
	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_%d]\n", plane_mode);
	gcs_Get_Voxel_Idx_in_Plane(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		plane_mode,//int &in_plane_mode,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment[0]);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
	/************************************************************************************/

	/************************************************************************************/
	plane_mode = 1;
	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_%d]\n", plane_mode);
	gcs_Get_Voxel_Idx_in_Plane(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		plane_mode,//int &in_plane_mode,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment[1]);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
	/************************************************************************************/

	/************************************************************************************/
	plane_mode = 2;
	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_%d]\n", plane_mode);
	gcs_Get_Voxel_Idx_in_Plane(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		plane_mode,//int &in_plane_mode,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment[2]);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
	/************************************************************************************/

	printf("[avs_Analyze_Voxel_Segments]\n");
	avs_Analyze_Voxel_Segments(
		segment[0],//std::vector<std::vector<std::vector<int>>> &in_voxel_segments_X,
		segment[1],//std::vector<std::vector<std::vector<int>>> &in_voxel_segments_Y,
		segment[2],//std::vector<std::vector<std::vector<int>>> &in_voxel_segments_Z,
		voxel_segments);//std::vector<std::vector<int>> &out_voxel_segments);

	printf("[gv_Get_Voxels_From_Table]\n");
	gv_Get_Voxels_From_Table(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		voxel_segments,//std::vector<std::vector<int>> &in_table,
		out_voxel_segments);//std::vector<std::vector<std::vector<float>>> &out_voxel_segments)

}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segmentation_of_Voxel_Data_along_a_Slice(
	VCL_DoCube_X_Color *in_docube,
	int in_slice_mode_0_X_1_Y_2_Z,
	CKvSet_of_Xrunset3dShort *out_voxel_segments)
//************************************************************************
{
	if (in_slice_mode_0_X_1_Y_2_Z == 0)
	{
		printf("[svd_Segment_Voxel_Data_in_X_Slice]\n");
		svd_Segment_Voxel_Data_in_X_Slice(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			out_voxel_segments);//CKvSet_of_Xrunset3dShort *out_voxel_segments);
	}
	else if (in_slice_mode_0_X_1_Y_2_Z == 1)
	{
		printf("[svd_Segment_Voxel_Data_in_Y_Slice]\n");
		svd_Segment_Voxel_Data_in_Y_Slice(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			out_voxel_segments);//CKvSet_of_Xrunset3dShort *out_voxel_segments);
	}
	else
	{
		printf("[svd_Segment_Voxel_Data_in_Z_Slice]\n");
		svd_Segment_Voxel_Data_in_Z_Slice(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			out_voxel_segments);//CKvSet_of_Xrunset3dShort *out_voxel_segments);
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segmentation_of_Voxel_Data_along_a_Slice(
	VCL_DoCube_X_Color *in_docube,
	int in_slice_mode_0_X_1_Y_2_Z,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{
	CKvStopWatch sw;

	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);
	printf("ww=%d hh=%d dd=%d\n", ww, hh, dd);
	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_%d]\n",in_slice_mode_0_X_1_Y_2_Z);

	sw.c_Create(1);
	sw.r_Reset(0);
	gcs_Get_Voxel_Idx_in_Plane(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_slice_mode_0_X_1_Y_2_Z,//int &in_plane_mode,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);
	printf("[gcs_Get_Voxel_Idx_in_Plane] Time : %lf", sw.get_Get_Elapsed_Time(0));

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	sw.r_Reset(0);
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
	printf("[stoc_Segment_Voxel_Slice_Scanned_Data] Time : %lf", sw.get_Get_Elapsed_Time(0));

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		out_voxel_segments);//std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segmentation_of_Voxel_Data_along_a_Slice(
	VCL_DoCube_X_Color *in_docube,
	int in_slice_mode_0_X_1_Y_2_Z,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments,
	std::vector<std::vector<std::vector<float>>> &out_voxel_colors)
//************************************************************************
{
	CKvStopWatch sw;
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);
	printf("ww=%d hh=%d dd=%d\n", ww, hh, dd);
	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_%d]\n", in_slice_mode_0_X_1_Y_2_Z);

	sw.c_Create(1);
	sw.r_Reset(0);
	gcs_Get_Voxel_Idx_in_Plane(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		in_slice_mode_0_X_1_Y_2_Z,//int &in_plane_mode,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);
	printf("[gcs_Get_Voxel_Idx_in_Plane] Time : %lf", sw.get_Get_Elapsed_Time(0));

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	sw.r_Reset(0);
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
	printf("[stoc_Segment_Voxel_Slice_Scanned_Data] Time : %lf", sw.get_Get_Elapsed_Time(0));

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		out_voxel_segments);//std::vector<std::vector<std::vector<float>>> &out_voxel_segments)

	printf("[gv_Get_Voxel_Colors_From_Segment]\n");
	gv_Get_Voxel_Colors_From_Segment(
		&in_docube->gvc_Get_Voxel_Color(),//CKvDepot_of_RgbaF *in_voxel_colors,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		out_voxel_colors);//std::vector<std::vector<std::vector<float>>> &out_voxel_colors)
}
//************************************************************************
void VCL_Voxel_Segmentation::psr_Plot_Segmentation_Result(
	CKvSet_of_Xrunset3dShort *in_voxel_segments)
//************************************************************************
{
	CKvXvectorInt pt_x_idx;
	CKvXvectorInt color_x_idx;

	CKvGraph3D graph3d; 
	CKvXrunset3dShort *p_segments;
	
	CKvDepot_of_Point3Df segment_point3d;
	CKvSet_of_Point3Df object_point3d;
	CKvDepot_of_VoxelShort depot_voxel;
	
	CKvDepot_of_RgbaF depot_color; 
	CKvSet_of_RgbaF set_colors;

	CKvMesh_of_Point mesh_point;
	CKvSet_of_VectorInt element; 
	CKvVectorInt point_idx, color_idx, point_sz;
	CKvVectorInt point_seg_idx, color_seg_idx;
	int n, k, num_voxel, all_num_voxel = 0, num_segments, dum, offset[3] = { 0, };
	int dum1, dum2, count=0;
	short **p_dv; float r, g, b;


	p_segments = in_voxel_segments->vps(num_segments);
	segment_point3d.in_Initialize();
	depot_color.in_Initialize();

	pt_x_idx.in_Initialize(10);
	color_x_idx.in_Initialize(10);

	for (k = 0; k < num_segments; k++)
	{
		p_segments[k].gdv_Get_Depot_of_Voxels(depot_voxel);
		num_voxel = depot_voxel.ne_Number_of_Elements();
		all_num_voxel += num_voxel;
		int         *p_pt_idx = point_seg_idx.c_Create(num_voxel);
		int         *p_c_idx  = color_seg_idx.c_Create(num_voxel);
		CKvPoint3Df *p_obj_pt = object_point3d.c_Create(num_voxel);
		CKvRgbaF    *p_rgbaf  = set_colors.c_Create(num_voxel);

		p_dv = depot_voxel.gps_Get_Pointer_of_Space()->mp();

		r = ((float)rand() / (RAND_MAX));
		g = ((float)rand() / (RAND_MAX));
		b = ((float)rand() / (RAND_MAX));

		for (n = 0; n < num_voxel; n++)
		{
			p_obj_pt[n].x = (float)p_dv[n][0];
			p_obj_pt[n].y = (float)p_dv[n][1];
			p_obj_pt[n].z = (float)p_dv[n][2];

			p_rgbaf[n].r = r;
			p_rgbaf[n].g = g;
			p_rgbaf[n].b = b;
			p_rgbaf[n].a = 1.f;

			p_pt_idx[n] = count;
			p_c_idx[n]  = count;
			count++;
		}
		
		segment_point3d.ap_Append(false, &object_point3d, dum1, dum2);
		depot_color.ap_Append(false, &set_colors, dum1, dum2);
		
		pt_x_idx.ap_Append(&point_seg_idx, dum1);
		color_x_idx.ap_Append(&color_seg_idx, dum1);
	}

	pt_x_idx.z_cne_Change_Number_of_Elements(all_num_voxel);
	color_x_idx.z_cne_Change_Number_of_Elements(all_num_voxel);

	pt_x_idx.ex_Export(&point_idx, NULL);
	color_x_idx.ex_Export(&color_idx, NULL);

	point_sz.c_Create(all_num_voxel, 2);

	mesh_point.u_me_Make_Element(&point_idx, &color_idx, &point_sz, &element);
	mesh_point.ap_Append(true, &element, offset, dum);

	graph3d.g_sp_Set_display_Parameters(
		true,
		Kv_RgbF(255.0f, 255.0f, 255.0f));

	graph3d.g_id_Initialize_Depots(
		&segment_point3d,
		&depot_color,
		NULL, NULL,
		NULL, NULL,
		NULL);

	graph3d.g_p_Plot(
		NULL,
		&segment_point3d,
		NULL,
		&depot_color,
		NULL, NULL,
		NULL, NULL,
		NULL,
		&mesh_point,
		true);

	
	Kv_Printf("Segmentation Results");


	if (Kv_Printf("Save-> YES or No"))
	{
		FILE *fp = NULL;
		int sz;

		sz = depot_color.ne_Number_of_Elements();
		fopen_s(&fp, "Segmentation Result.ply", "w");
		if (fp == NULL) return;

		fprintf(fp, "ply\n");
		fprintf(fp, "format ascii 1.0\n");
		fprintf(fp, "element vertex %d\n", sz);
		fprintf(fp, "property float x\n");
		fprintf(fp, "property float y\n");
		fprintf(fp, "property float z\n");
		fprintf(fp, "property uchar red\n");
		fprintf(fp, "property uchar green\n");
		fprintf(fp, "property uchar blue\n");
		fprintf(fp, "end_header\n");


		float **pp_rgbaf = depot_color.gps_Get_Pointer_of_Space()->mp();
		float **p_pt     = segment_point3d.gps_Get_Pointer_of_Space()->mp();


		for (k = 0; k < sz; k++)
		{
			fprintf(fp, "%f %f %f %d %d %d\n",
				p_pt[k][0], p_pt[k][1], p_pt[k][2],
				(int)(255.f*pp_rgbaf[k][0]), (int)(255.f*pp_rgbaf[k][1]), (int)(255.f*pp_rgbaf[k][2]));
		}

		fclose(fp);
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::psr_Plot_Segmentation_Result(
	std::vector<std::vector<std::vector<float>>> &in_voxel_segment)
//************************************************************************
{
	CKvXvectorInt pt_x_idx;
	CKvXvectorInt color_x_idx;

	CKvGraph3D graph3d;
	CKvXrunset3dShort *p_segments;

	CKvDepot_of_Point3Df segment_point3d;
	CKvSet_of_Point3Df object_point3d;
	CKvDepot_of_VoxelShort depot_voxel;

	CKvDepot_of_RgbaF depot_color;
	CKvSet_of_RgbaF set_colors;

	CKvMesh_of_Point mesh_point;
	CKvSet_of_VectorInt element;
	CKvVectorInt point_idx, color_idx, point_sz;
	CKvVectorInt point_seg_idx, color_seg_idx;
	int n, k, num_voxel, all_num_voxel = 0, num_segments, dum, offset[3] = { 0, };
	int dum1, dum2, count = 0;
	short **p_dv; float r, g, b;

	num_segments = in_voxel_segment.size();
	segment_point3d.in_Initialize();
	depot_color.in_Initialize();

	pt_x_idx.in_Initialize(10);
	color_x_idx.in_Initialize(10);

	for (k = 0; k < num_segments; k++)
	{
		num_voxel = in_voxel_segment[k].size();
		all_num_voxel += num_voxel;
		int         *p_pt_idx = point_seg_idx.c_Create(num_voxel);
		int         *p_c_idx = color_seg_idx.c_Create(num_voxel);
		CKvPoint3Df *p_obj_pt = object_point3d.c_Create(num_voxel);
		CKvRgbaF    *p_rgbaf = set_colors.c_Create(num_voxel);


		r = ((float)rand() / (RAND_MAX));
		g = ((float)rand() / (RAND_MAX));
		b = ((float)rand() / (RAND_MAX));

		for (n = 0; n < num_voxel; n++)
		{
			p_obj_pt[n].x = (float)in_voxel_segment[k][n][0];
			p_obj_pt[n].y = (float)in_voxel_segment[k][n][1];
			p_obj_pt[n].z = (float)in_voxel_segment[k][n][2];

			p_rgbaf[n].r = r;
			p_rgbaf[n].g = g;
			p_rgbaf[n].b = b;
			p_rgbaf[n].a = 1.f;

			p_pt_idx[n] = count;
			p_c_idx[n] = count;
			count++;
		}

		segment_point3d.ap_Append(false, &object_point3d, dum1, dum2);
		depot_color.ap_Append(false, &set_colors, dum1, dum2);

		pt_x_idx.ap_Append(&point_seg_idx, dum1);
		color_x_idx.ap_Append(&color_seg_idx, dum1);
	}

	pt_x_idx.z_cne_Change_Number_of_Elements(all_num_voxel);
	color_x_idx.z_cne_Change_Number_of_Elements(all_num_voxel);

	pt_x_idx.ex_Export(&point_idx, NULL);
	color_x_idx.ex_Export(&color_idx, NULL);

	point_sz.c_Create(all_num_voxel, 2);

	mesh_point.u_me_Make_Element(&point_idx, &color_idx, &point_sz, &element);
	mesh_point.ap_Append(true, &element, offset, dum);

	graph3d.g_sp_Set_display_Parameters(
		true,
		Kv_RgbF(255.0f, 255.0f, 255.0f));

	graph3d.g_id_Initialize_Depots(
		&segment_point3d,
		&depot_color,
		NULL, NULL,
		NULL, NULL,
		NULL);

	graph3d.g_p_Plot(
		NULL,
		&segment_point3d,
		NULL,
		&depot_color,
		NULL, NULL,
		NULL, NULL,
		NULL,
		&mesh_point,
		true);


	Kv_Printf("Segmentation Results");


	if (Kv_Printf("Save-> YES or No"))
	{
		FILE *fp = NULL;
		int sz;

		sz = depot_color.ne_Number_of_Elements();
		fopen_s(&fp, "Segmentation Result.ply", "w");
		if (fp == NULL) return;

		fprintf(fp, "ply\n");
		fprintf(fp, "format ascii 1.0\n");
		fprintf(fp, "element vertex %d\n", sz);
		fprintf(fp, "property float x\n");
		fprintf(fp, "property float y\n");
		fprintf(fp, "property float z\n");
		fprintf(fp, "property uchar red\n");
		fprintf(fp, "property uchar green\n");
		fprintf(fp, "property uchar blue\n");
		fprintf(fp, "end_header\n");


		float **pp_rgbaf = depot_color.gps_Get_Pointer_of_Space()->mp();
		float **p_pt = segment_point3d.gps_Get_Pointer_of_Space()->mp();


		for (k = 0; k < sz; k++)
		{
			fprintf(fp, "%f %f %f %d %d %d\n",
				p_pt[k][0], p_pt[k][1], p_pt[k][2],
				(int)(255.f*pp_rgbaf[k][0]), (int)(255.f*pp_rgbaf[k][1]), (int)(255.f*pp_rgbaf[k][2]));
		}

		fclose(fp);
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::psr_Plot_Segmentation_Result(
	std::vector<std::vector<std::vector<float>>> &in_voxel_segment,
	std::vector<std::vector<std::vector<float>>> &in_voxel_color_segment)
//************************************************************************
{
	CKvXvectorInt pt_x_idx;
	CKvXvectorInt color_x_idx;

	CKvGraph3D graph3d;

	CKvDepot_of_Point3Df segment_point3d;
	CKvSet_of_Point3Df object_point3d;

	CKvDepot_of_RgbaF depot_color;
	CKvSet_of_RgbaF set_colors;

	CKvMesh_of_Point mesh_point;
	CKvSet_of_VectorInt element;
	CKvVectorInt point_idx, color_idx, point_sz;
	CKvVectorInt point_seg_idx, color_seg_idx;
	CKvString str;
	int n, k, m,num_voxel, all_num_voxel = 0, num_segments, dum, offset[3] = { 0, };
	int dum1, dum2, count = 0;
	short **p_dv; float r, g, b;

	num_segments = in_voxel_segment.size();
	segment_point3d.in_Initialize();
	depot_color.in_Initialize();

	pt_x_idx.in_Initialize(10);
	color_x_idx.in_Initialize(10);

	for (k = 0; k < num_segments; k++)
	{
		printf("[k=%d/%d]\n", k, num_segments);
		num_voxel = in_voxel_segment[k].size();
		all_num_voxel += num_voxel;
		int         *p_pt_idx = point_seg_idx.c_Create(num_voxel);
		int         *p_c_idx  = color_seg_idx.c_Create(num_voxel);
		CKvPoint3Df *p_obj_pt = object_point3d.c_Create(num_voxel);
		CKvRgbaF    *p_rgbaf  = set_colors.c_Create(num_voxel);
		point_sz.c_Create(num_voxel, 2);

		r = ((float)rand() / (RAND_MAX));
		g = ((float)rand() / (RAND_MAX));
		b = ((float)rand() / (RAND_MAX));

		count = 0;
		for (n = 0; n < num_voxel; n++)
		{
			p_obj_pt[n].x = (float)in_voxel_segment[k][n][0];
			p_obj_pt[n].y = (float)in_voxel_segment[k][n][1];
			p_obj_pt[n].z = (float)in_voxel_segment[k][n][2];

			p_rgbaf[n].r = (float)in_voxel_color_segment[k][n][0];
			p_rgbaf[n].g = (float)in_voxel_color_segment[k][n][1];
			p_rgbaf[n].b = (float)in_voxel_color_segment[k][n][2];
			p_rgbaf[n].a = 1.f;

			p_pt_idx[n] = count;
			p_c_idx[n]  = count;
			count++;
		}

		mesh_point.u_me_Make_Element(&point_seg_idx, &color_seg_idx, &point_sz, &element);
		mesh_point.ap_Append(true, &element, offset, dum);

		segment_point3d.ap_Append(true, &object_point3d, dum1, dum2);
		depot_color.ap_Append(true, &set_colors, dum1, dum2);

		graph3d.g_sp_Set_display_Parameters(
			true,
			Kv_RgbF(255.0f, 255.0f, 255.0f));

		graph3d.g_id_Initialize_Depots(
			&segment_point3d,
			&depot_color,
			NULL, NULL,
			NULL, NULL,
			NULL);

		graph3d.g_p_Plot(
			NULL,
			&segment_point3d,
			NULL,
			&depot_color,
			NULL, NULL,
			NULL, NULL,
			NULL,
			&mesh_point,
			true);

		Kv_Printf("Segmentation Results (%d/%d)",k, num_segments);
		//if (Kv_Printf("Save-> YES or No"))
		//{
			FILE *fp = NULL;
			int sz;

			sz = depot_color.ne_Number_of_Elements();
			str.fm_Format("Segmentation Result_%05d.ply", k);
			fopen_s(&fp, (char *)str, "w");
			if (fp == NULL) return;

			fprintf(fp, "ply\n");
			fprintf(fp, "format ascii 1.0\n");
			fprintf(fp, "element vertex %d\n", sz);
			fprintf(fp, "property float x\n");
			fprintf(fp, "property float y\n");
			fprintf(fp, "property float z\n");
			fprintf(fp, "property uchar red\n");
			fprintf(fp, "property uchar green\n");
			fprintf(fp, "property uchar blue\n");
			fprintf(fp, "end_header\n");

			float **pp_rgbaf = depot_color.gps_Get_Pointer_of_Space()->mp();
			float **p_pt = segment_point3d.gps_Get_Pointer_of_Space()->mp();

			for (m = 0; m < sz; m++)
			{
				fprintf(fp, "%f %f %f %d %d %d\n",
					p_pt[m][0], p_pt[m][1], p_pt[m][2],
					(int)(255.f*pp_rgbaf[m][0]), (int)(255.f*pp_rgbaf[m][1]), (int)(255.f*pp_rgbaf[m][2]));
			}

			fclose(fp);
		//}

		printf("[Recursive]\n");
	}

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
void VCL_Voxel_Segmentation::avs_Analyze_Voxel_Segments(
	std::vector<std::vector<std::vector<int>>> &in_voxel_segments_X,
	std::vector<std::vector<std::vector<int>>> &in_voxel_segments_Y,
	std::vector<std::vector<std::vector<int>>> &in_voxel_segments_Z,
	std::vector<std::vector<int>> &out_voxel_segments)
//************************************************************************
{
	std::vector<std::vector<int>> Table;
	
	int num_voxel=0, num_blob, num_slice, num_element;
	int m, n, k;

	num_blob = in_voxel_segments_X.size();
	for (m = 0; m < num_blob ; m++)
	{
		num_slice = in_voxel_segments_X[m].size();
		for (n = 0; n < num_slice; n++)
		{
			num_voxel += in_voxel_segments_X[m][n].size();
		}
	}

	Table.resize( num_voxel, std::vector<int>(3,-1) );
	
	num_blob = in_voxel_segments_X.size();
	for (m = 0; m < num_blob; m++)
	{
		num_slice = in_voxel_segments_X[m].size();
		for (n = 0; n < num_slice; n++)
		{
			num_element = in_voxel_segments_X[m][n].size();
			for (k = 0; k < num_element; k++)
			{
				Table[ in_voxel_segments_X[m][n][k] ][0] = m;
			}
		}
	}

	num_blob = in_voxel_segments_Y.size();
	for (m = 0; m < num_blob; m++)
	{
		num_slice = in_voxel_segments_Y[m].size();
		for (n = 0; n < num_slice; n++)
		{
			num_element = in_voxel_segments_Y[m][n].size();
			for (k = 0; k < num_element; k++)
			{
				Table[ in_voxel_segments_Y[m][n][k] ][1] = m;
			}
		}
	}

	num_blob = in_voxel_segments_Z.size();
	for (m = 0; m < num_blob; m++)
	{
		num_slice = in_voxel_segments_Z[m].size();
		for (n = 0; n < num_slice; n++)
		{
			num_element = in_voxel_segments_Z[m][n].size();
			for (k = 0; k < num_element; k++)
			{
				Table[ in_voxel_segments_Z[m][n][k] ][2] = m;
			}
		}
	}

	std::vector<bool> flag_vec = std::vector<bool>(num_voxel, false);
	std::vector<int> tmp_vec;
	std::vector<int> refer = std::vector<int>(3);
	for (m = 0; m < num_voxel; m++)
	{
		tmp_vec.clear();
		if (flag_vec[m] == false)
		{
			refer[0] = Table[m][0];
			refer[1] = Table[m][1];
			refer[2] = Table[m][2];
			for (n = m; n < num_voxel; n++)
			{
				if ((refer[0] == Table[n][0]) &&
					(refer[1] == Table[n][1]) &&
					(refer[2] == Table[n][2]))
				{
					flag_vec[n] = true;
					tmp_vec.push_back(n);
				}
			}
			out_voxel_segments.push_back(tmp_vec);
		}
	}


}
//************************************************************************
void VCL_Voxel_Segmentation::avs_Analyze_Voxel_Segments(
	std::vector<std::vector<std::vector<float>>> &in_voxel_segments_X,
	std::vector<std::vector<std::vector<float>>> &in_voxel_segments_Y,
	std::vector<std::vector<std::vector<float>>> &in_voxel_segments_Z,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{






}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segment_Voxel_Data_in_X_Slice(
	VCL_DoCube_X_Color *in_docube,
	CKvSet_of_Xrunset3dShort *out_voxel_segments)
//************************************************************************
{
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_X]\n");
	gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_X(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		ww,//int in_ww,
		hh,//int in_hh,
		dd,//int in_dd,
		out_voxel_segments);//CKvSet_of_Xrunset3dShort *out_voxel_segments)
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segment_Voxel_Data_in_Y_Slice(
	VCL_DoCube_X_Color *in_docube,
	CKvSet_of_Xrunset3dShort *out_voxel_segments)
//************************************************************************
{
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Y]\n");
	gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Y(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		ww,//int in_ww,
		hh,//int in_hh,
		dd,//int in_dd,
		out_voxel_segments);//CKvSet_of_Xrunset3dShort *out_voxel_segments)
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segment_Voxel_Data_in_Z_Slice(
	VCL_DoCube_X_Color *in_docube,
	CKvSet_of_Xrunset3dShort *out_voxel_segments)
//************************************************************************
{
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Z]\n");
	gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Z(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		ww,//int in_ww,
		hh,//int in_hh,
		dd,//int in_dd,
		out_voxel_segments);//CKvSet_of_Xrunset3dShort *out_voxel_segments)
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segment_Voxel_Data_in_X_Slice(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_X]\n");
	gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_X(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		out_voxel_segments);//std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segment_Voxel_Data_in_Y_Slice(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Y]\n");
	gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Y(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		out_voxel_segments);//std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
}
//************************************************************************
void VCL_Voxel_Segmentation::svd_Segment_Voxel_Data_in_Z_Slice(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{
	std::vector<Voxel_Slice_Scanned_Data> sd;
	std::vector<std::vector<std::vector<int>>> segment;
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);

	printf("[gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Z]\n");
	gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Z(
		in_docube,//VCL_DoCube_X_Color *in_docube,
		ww,//int &in_ww,
		hh,//int &in_hh,
		dd,//int &in_dd,
		sd);//std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code);

	printf("[stoc_Segment_Voxel_Slice_Scanned_Data]\n");
	stoc_Segment_Voxel_Slice_Scanned_Data(
		sd,//std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
		segment);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)

	printf("[gv_Get_Voxels_From_Segment]\n");
	gv_Get_Voxels_From_Segment(
		&in_docube->gsp_Get_Surface_Points(),//CKvDepot_of_Point3Df *in_voxels,
		segment,//std::vector<std::vector<std::vector<int>>> &in_segments,
		out_voxel_segments);//std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
}
//************************************************************************
void VCL_Voxel_Segmentation::stoc_Segment_Voxel_Slice_Scanned_Data(
	std::vector<Voxel_Slice_Scanned_Data> &in_texture_on_code,
	std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
//************************************************************************
{
	std::vector<std::vector<int>> tmp;
	int num_valid_slice, num_pcc;
	int current_maximum_label, k;

	num_valid_slice = in_texture_on_code.size();
	current_maximum_label = 0;
	if (num_valid_slice == 0) { return; }

	out_segmented_texture_on_code.clear();

	//첫번쨰 slice의 ToC 처리.
	num_pcc = in_texture_on_code[0].voxel_idx_sequences.size();
	in_texture_on_code[0].list_of_label_for_voxel_idx_sequences = std::vector<int>(num_pcc);
	for (k = 0; k < num_pcc; k++)
	{
		tmp = std::vector<std::vector<int>>(1);
		tmp[0] = in_texture_on_code[0].voxel_idx_sequences[k];
		out_segmented_texture_on_code.push_back(tmp);
		in_texture_on_code[0].list_of_label_for_voxel_idx_sequences[k] = current_maximum_label;
		current_maximum_label++;
	}

	//그 후 slice의 ToC 처리. 
	for (k = 1; k < num_valid_slice; k++)
	{
		atoc_Analyze_Voxel_Sequences(
			in_texture_on_code[k],//Voxel_Slice_Peeling_Chain_Code &in_current_texture_on_code,
			in_texture_on_code[k - 1],//Voxel_Slice_Peeling_Chain_Code &in_before_texture_on_code,
			current_maximum_label,//int &in_current_maximum_label,
			out_segmented_texture_on_code);//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::atoc_Analyze_Voxel_Sequences(
	Voxel_Slice_Scanned_Data &in_current_texture_on_code,
	Voxel_Slice_Scanned_Data &in_before_texture_on_code,
	int &in_current_maximum_label,
	std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code)
//************************************************************************
{
	std::vector<std::vector<int>> dummy_space;
	CKvMatrixBool cont_mat;
	CKvVectorChar state_code;
	int num_current_toc, init_label, change_label, label, k;
	char *p_state;

	if (!fcm_Find_Connectivity_Matrix2(
		in_current_texture_on_code,//Voxel_Slice_Peeling_Chain_Code &in_current_pcc,
		in_before_texture_on_code,//Voxel_Slice_Peeling_Chain_Code &in_before_pcc,
		cont_mat))//CKvMatrixBool &out_connectivity_matrix);
	{
		return;
	}

	init_label = in_current_maximum_label;

	acm_Analyze_Connectivity_Matrix(
		in_current_texture_on_code,//Voxel_Slice_Peeling_Chain_Code &in_current_texture_on_code,
		in_before_texture_on_code,//Voxel_Slice_Peeling_Chain_Code &in_before_texture_on_code,
		in_current_maximum_label,//int &in_current_maximum_label,
		&cont_mat,//CKvMatrixBool *in_connectivity_matrix,
		&state_code);//CKvVectorChar *out_state_code_of_current_texture_on_code)


	change_label = in_current_maximum_label - init_label;

	if (change_label > 0)
	{
		for (k = 0; k < change_label; k++)
		{
			out_segmented_texture_on_code.push_back(dummy_space);
		}
	}

	num_current_toc = in_current_texture_on_code.voxel_idx_sequences.size();
	p_state = state_code.vp();

	for (k = 0; k < num_current_toc; k++)
	{
		label = in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[k];
		out_segmented_texture_on_code[label].push_back(in_current_texture_on_code.voxel_idx_sequences[k]);
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::acm_Analyze_Connectivity_Matrix( 
	Voxel_Slice_Scanned_Data &in_current_texture_on_code,
	Voxel_Slice_Scanned_Data &in_before_texture_on_code,
	int &in_current_maximum_label,
	CKvMatrixBool *in_connectivity_matrix, // # of before ToC - by - # of current ToC. 
	CKvVectorChar *out_state_code_of_current_texture_on_code)
//************************************************************************
{
	CKvVectorBool set_flag;
	CKvXvectorInt idx_vector;
	CKvVectorInt idx_vector1, idx_vector2;
	int col_count, count, set_count, num_current_toc, num_before_toc, j, i;
	int *p_idx_vec, *p_idx_vec2;
	char *p_state_code;
	bool **p_cm, *p_sf, flag;
	float delta_length;

	//printf("zz_threshold = %f\n", zz_threshold);

	p_cm = in_connectivity_matrix->mps(num_current_toc, num_before_toc);
	p_state_code = out_state_code_of_current_texture_on_code->c_Create(num_current_toc, INVALID_CODE);
	p_sf = set_flag.c_Create(num_current_toc, false);
	in_current_texture_on_code.list_of_label_for_voxel_idx_sequences = std::vector<int>(num_current_toc);
	set_count = 0;

	/***************************************************************************************************************/
	//                                    Check Is Start ToC (Column wise check)                                   //
	/***************************************************************************************************************/
	for (i = 0; i < num_current_toc; i++)
	{
		count = 0;
		for (j = 0; j < num_before_toc; j++)
		{
			if (p_cm[j][i] == true) { count++; }
		}

		if (count == 0) // Start Code <- I-TA (Isolated - Texture Array)
		{
			p_state_code[i] = START_CODE;
			set_count++;
			in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[i] = in_current_maximum_label;
			in_current_maximum_label++;
			p_sf[i] = true;
		}
	}
	if (set_count == num_current_toc) { return; }

	/***************************************************************************************************************/
	//                                    Check Is new Label or append (Row wise check)                            //
	/***************************************************************************************************************/
	for (j = 0; j < num_before_toc; j++)
	{
		count = 0;
		//idx_vector.in_Initialize(num_current_toc);
		//idx_vector.z_gii(NULL, &p_idx_vec, NULL, NULL);
		p_idx_vec = idx_vector1.c_Create(num_current_toc);
		for (i = 0; i < num_current_toc; i++)
		{
			if (p_cm[j][i] == true)
			{
				p_idx_vec[count] = i;
				count++;
			}
		}
		/******************************************************/
		/******************************************************/
		if (count > 1) // Start Code <- S-TA (Split - Texture Array) 
		{
			/**********************************************************************************************************************/
			//flag      = false;
			//set_count = 0;
			//for (i = 0; i < count; i++)
			//{
			//	if (p_sf[p_idx_vec[i]] == false)
			//	{
			//		/**********************************************************************************************************************/
			//		//Old version. 
			//		//p_state_code[p_idx_vec[i]] = START_CODE;
			//		//in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[i]] = in_current_maximum_label;
			//		//in_current_maximum_label++;
			//		//p_sf[p_idx_vec[i]] = true;
			//		/**********************************************************************************************************************/
			//		/**********************************************************************************************************************/
			//		//New version 1
			//		if ((float)abs((int)in_before_texture_on_code.voxel_idx_sequences[j].size() -
			//			(int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[i]].size()) < 
			//			((float)in_before_texture_on_code.voxel_idx_sequences[j].size()*zz_threshold) &&
			//			(flag == false) )
			//		{
			//			p_state_code[p_idx_vec[i]] = APPEND_CODE;
			//			in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[i]] =
			//				in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[j];
			//			flag = true;
			//		}
			//		else
			//		{
			//			p_state_code[p_idx_vec[i]] = START_CODE;
			//			in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[i]] = in_current_maximum_label;
			//			in_current_maximum_label++;
			//		}
			//		p_sf[p_idx_vec[i]] = true;
			//		/**********************************************************************************************************************/
			//	}
			//}
			/**********************************************************************************************************************/
			// New Version 2
			//flag = false;
			//set_count = 0;
			//float target_length = -1.f;
			//for (i = 0; i < count; i++)
			//{
			//	if (p_sf[p_idx_vec[i]] == false)
			//	{
			//		float larger_length = max(
			//			in_before_texture_on_code.voxel_idx_sequences[j].size(), 
			//			in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[i]].size());
			//		
			//		if ((float)abs((int)in_before_texture_on_code.voxel_idx_sequences[j].size() -
			//			(int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[i]].size()) <
			//			((float)larger_length*zz_threshold))
			//		//if ((float)abs((int)in_before_texture_on_code.voxel_idx_sequences[j].size() -
			//		//	(int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[i]].size()) <
			//		//	((float)in_before_texture_on_code.voxel_idx_sequences[j].size()*zz_threshold))
			//		{
			//			if (set_count == 0)
			//			{
			//				set_count++;
			//				p_state_code[p_idx_vec[i]] = APPEND_CODE;
			//				in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[i]] =
			//					in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[j];
			//				p_sf[p_idx_vec[i]] = true;
			//			}
			//			else
			//			{
			//				flag = true;
			//			}
			//		}
			//	}
			//	if (flag == true)
			//	{
			//		break;
			//	}
			//}
			//for (i = 0; i < count; i++)
			//{
			//	if ( (p_sf[p_idx_vec[i]] == false) || (flag == true) )
			//	{
			//		p_state_code[p_idx_vec[i]] = START_CODE;
			//		in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[i]] = in_current_maximum_label;
			//		in_current_maximum_label++;
			//		p_sf[p_idx_vec[i]] = true;
			//	}
			//}
			/**********************************************************************************************************************/
			// Test Version 3
			flag = false;
			set_count = 0;
			float target_length = FLT_MAX, delta_length;
			int target_index;
			for (i = 0; i < count; i++)
			{
				if (p_sf[p_idx_vec[i]] == false)
				{
					float larger_length = max(
						in_before_texture_on_code.voxel_idx_sequences[j].size(),
						in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[i]].size());

					delta_length = (float)abs((int)in_before_texture_on_code.voxel_idx_sequences[j].size() -
						(int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[i]].size());

					if (delta_length < ((float)larger_length*zz_threshold))
					{

						if (target_length > delta_length)
						{
							target_index = p_idx_vec[i];
							target_length = delta_length;
						}
						set_count++;
					}
				}
			}
			/***************************************/
			if (set_count != 0)
			{
				p_state_code[target_index] = APPEND_CODE;
				in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[target_index] =
					in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[j];
				p_sf[target_index] = true;

			}
			/***************************************/
			for (i = 0; i < count; i++)
			{
				if (p_sf[p_idx_vec[i]] == false)
				{
					p_state_code[p_idx_vec[i]] = START_CODE;
					in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[i]] = in_current_maximum_label;
					in_current_maximum_label++;
					p_sf[p_idx_vec[i]] = true;
				}
			}
			/**********************************************************************************************************************/
			/**********************************************************************************************************************/
			/**********************************************************************************************************************/
			/**********************************************************************************************************************/
			/**********************************************************************************************************************/
			/**********************************************************************************************************************/

		}
		/******************************************************/
		/******************************************************/
		else if (count == 1)
		{
			col_count = 0;
			p_idx_vec2 = idx_vector2.c_Create(num_before_toc);
			for (i = 0; i < num_before_toc; i++)
			{
				if (p_cm[i][p_idx_vec[0]] == true)
				{
					p_idx_vec2[col_count] = i;
					col_count++;
				}
			}

			/*****************************************************************/
			if (col_count == 1)//Append Code <- C-TA (Connected - Texture Array)
			{
				if (p_sf[p_idx_vec[0]] == false)
				{
					// You have to update New code here !
					delta_length = (float)abs((int)in_before_texture_on_code.voxel_idx_sequences[j].size() -
											  (int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[0]].size());

					float larger_length = max(
						in_before_texture_on_code.voxel_idx_sequences[j].size(),
						in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[0]].size());

					//if ( delta_length < ((float)larger_length*(float)0.2) )
					//{
						p_state_code[p_idx_vec[0]] = APPEND_CODE;
						in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] =
							in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[j];
						p_sf[p_idx_vec[0]] = true;
					//}
					//else
					//{
					//	p_state_code[p_idx_vec[0]] = START_CODE;
					//	in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] = 
					//		in_current_maximum_label;
					//	in_current_maximum_label++;
					//	p_sf[p_idx_vec[0]] = true;
					//}


					// Old Version
					/********************************************************************************/
					//p_state_code[p_idx_vec[0]] = APPEND_CODE;
					//in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] =
					//	in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[j];
					//p_sf[p_idx_vec[0]] = true;
					/********************************************************************************/
				
				
				
				}
			}
			else // Start Code <- M-TA (Merged - Texture Array)
			{
				set_count = 0;
				if (p_sf[p_idx_vec[0]] == false)
				{
					/**********************************************************************************************************************/
					/**********************************************************************************************************************/
					//Old Version
					//p_state_code[p_idx_vec[0]] = START_CODE;
					//in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] = in_current_maximum_label;
					//in_current_maximum_label++;
					//p_sf[p_idx_vec[0]] = true;
					/**********************************************************************************************************************/
					/**********************************************************************************************************************/
					//New Version
					//for (i = 0; i < col_count; i++)
					//{
					//	if ((float)abs((int)in_before_texture_on_code.voxel_idx_sequences[p_idx_vec2[i]].size() -
					//		(int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[0]].size()) < 
					//		((float)in_before_texture_on_code.voxel_idx_sequences[p_idx_vec2[i]].size()*zz_threshold)
					//		&& p_sf[p_idx_vec[0]] == false)
					//	{
					//		in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] =
					//			in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec2[i]];
					//		p_sf[p_idx_vec[0]] = true;
					//	}
					//}
					//if (p_sf[p_idx_vec[0]] == false)
					//{
					//	in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] = in_current_maximum_label;
					//	in_current_maximum_label++;
					//	p_sf[p_idx_vec[0]] = true;
					//}
					/**********************************************************************************************************************/
					/**********************************************************************************************************************/

					/**********************************************************************************************************************/
					/**********************************************************************************************************************/
					//New Version 2
					//for (i = 0; i < col_count; i++)
					//{
					//	if ((float)abs((int)in_before_texture_on_code.voxel_idx_sequences[p_idx_vec2[i]].size() -
					//		(int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[0]].size()) <
					//		((float)in_before_texture_on_code.voxel_idx_sequences[p_idx_vec2[i]].size()*zz_threshold))
					//	{
					//		set_count++;
					//		if (p_sf[p_idx_vec[0]] == false)
					//		{
					//			p_state_code[p_idx_vec[0]] = APPEND_CODE;
					//			in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] =
					//				in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec2[i]];
					//			p_sf[p_idx_vec[0]] = true;
					//		}
					//	}
					//}
					//
					///*******************************************************************
					//At the first time, if the connected texture arrays are two, we assign START CODE. 
					//However, now, we decide to assigning APPEND CODE in the same situations. 
					//So, we have to choose one, APPEND or START. 
					//********************************************************************/
					//if ((p_sf[p_idx_vec[0]] == false) ||(set_count <=2))// If the connected components are two, we assign START CODE. 
					//{
					//	p_state_code[p_idx_vec[0]] = START_CODE;
					//	in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] = in_current_maximum_label;
					//	in_current_maximum_label++;
					//	p_sf[p_idx_vec[0]] = true;
					//}
					/**********************************************************************************************************************/
					/**********************************************************************************************************************/

					/**********************************************************************************************************************/
					/**********************************************************************************************************************/
					//Test Version 3
					float target_length = FLT_MAX;
					int target_index;
					for (i = 0; i < col_count; i++)
					{
						delta_length = (float)abs((int)in_before_texture_on_code.voxel_idx_sequences[p_idx_vec2[i]].size() -
							(int)in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[0]].size());

						float larger_length = max(
							in_before_texture_on_code.voxel_idx_sequences[p_idx_vec2[i]].size(),
							in_current_texture_on_code.voxel_idx_sequences[p_idx_vec[0]].size());

						if (delta_length < ((float)larger_length*zz_threshold))
						{
							if (target_length > delta_length)
							{
								target_index = p_idx_vec2[i];
								target_length = delta_length;
							}
							set_count++;
						}
					}

					if (set_count != 0)
					{
						p_state_code[p_idx_vec[0]] = APPEND_CODE;
						in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] =
							in_before_texture_on_code.list_of_label_for_voxel_idx_sequences[target_index];
						p_sf[p_idx_vec[0]] = true;
					}

					/*******************************************************************
					At the first time, if the connected texture arrays are two, we assign START CODE.
					However, now, we decide to assign APPEND CODE in the same situations.
					So, we have to choose one, APPEND or START.
					********************************************************************/
					if (p_sf[p_idx_vec[0]] == false) // If the connected components are two, we assign START CODE. 
					{
						p_state_code[p_idx_vec[0]] = START_CODE;
						in_current_texture_on_code.list_of_label_for_voxel_idx_sequences[p_idx_vec[0]] = in_current_maximum_label;
						in_current_maximum_label++;
						p_sf[p_idx_vec[0]] = true;
					}
					/**********************************************************************************************************************/
					/**********************************************************************************************************************/
				}
				/*****************************************************************/
			}
			/******************************************************/
			/******************************************************/
		}
	}
}
//************************************************************************
bool VCL_Voxel_Segmentation::fcm_Find_Connectivity_Matrix(
	Voxel_Slice_Scanned_Data &in_current_pcc,
	Voxel_Slice_Scanned_Data &in_before_pcc,
	CKvMatrixBool &out_connectivity_matrix) // # of before ToC - by - # of current ToC. 
//************************************************************************
{
	int sz1, sz2, sz3, j, i; bool **p_fm;

	sz1 = in_before_pcc.list_of_index_for_connectivity.size();
	sz2 = in_current_pcc.list_of_index_for_connectivity.size();
	sz3 = in_current_pcc.blob_index_for_before_slice.size();

	if ((sz1 == 0) || (sz2 == 0) || (sz3 == 0))
	{

		return false;
	}

	p_fm = out_connectivity_matrix.c_Create(sz1, sz2, false);
	if (sz3 == sz1)
	{
		for (j = 0; j < sz1; j++)
		{
			for (i = 0; i < sz2; i++)
			{
				if (in_current_pcc.blob_index_for_before_slice[j] ==
					in_current_pcc.list_of_index_for_connectivity[i])
				{
					p_fm[j][i] = true;
				}
			}
		}
	}
	else
	{
		/*******************************************************************/
		for (j = 0; j < sz1; j++)
		{
			for (i = 0; i < sz2; i++)
			{
				if (in_before_pcc.list_of_index_for_connectivity[j] ==
					in_current_pcc.list_of_index_for_connectivity[i])
				{
					p_fm[j][i] = true;
				}
			}
		}
		/*******************************************************************/
	}


	return true;
}
//************************************************************************
bool VCL_Voxel_Segmentation::fcm_Find_Connectivity_Matrix2(
	Voxel_Slice_Scanned_Data &in_current_pcc,
	Voxel_Slice_Scanned_Data &in_before_pcc,
	CKvMatrixBool &out_connectivity_matrix) // # of before ToC - by - # of current ToC. 
//************************************************************************
{
	int sz1, sz2, sz3, j, i; bool **p_fm;
	int idx;

	sz1 = in_before_pcc.list_of_index_for_connectivity.size();
	sz2 = in_current_pcc.list_of_index_for_connectivity.size();
	sz3 = in_current_pcc.blob_index_for_before_slice.size();

	if ((sz1 == 0) || (sz2 == 0) || (sz3 == 0))
	{

		return false;
	}

	p_fm = out_connectivity_matrix.c_Create(sz1, sz2, false);

	for (i = 0; i < sz2; i++)
	{
		idx = in_current_pcc.list_of_index_for_connectivity[i];
		
		if (idx >= sz1 || idx < 0)
		{
			continue;
		}

		p_fm[idx][i] = true;
	}



	//if (sz3 == sz1)
	//{
	//	for (j = 0; j < sz1; j++)
	//	{
	//		for (i = 0; i < sz2; i++)
	//		{
	//			if (in_current_pcc.blob_index_for_before_slice[j] ==
	//				in_current_pcc.list_of_index_for_connectivity[i])
	//			{
	//				p_fm[j][i] = true;
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	/*******************************************************************/
	//	for (j = 0; j < sz1; j++)
	//	{
	//		for (i = 0; i < sz2; i++)
	//		{
	//			if (in_before_pcc.list_of_index_for_connectivity[j] ==
	//				in_current_pcc.list_of_index_for_connectivity[i])
	//			{
	//				p_fm[j][i] = true;
	//			}
	//		}
	//	}
	//	/*******************************************************************/
	//}


	return true;
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxels_From_Segment(
	CKvDepot_of_Point3Df *in_voxels,
	std::vector<std::vector<std::vector<int>>> &in_segments,
	int in_ww,
	int in_hh,
	int in_dd,
	CKvSet_of_Xrunset3dShort *out_voxel_segments)
//************************************************************************
{
	CKvXrunset3dShort *p_voxel_segment;
	CKvSet_of_Point3Df voxels;
	int num_segment, k;

	num_segment     = in_segments.size();
	p_voxel_segment	= out_voxel_segments->c_Create(num_segment);
	for (k = 0; k < num_segment; k++)
	{
		printf("	# of segments : %d/%d\r", k, num_segment);
		gv_Get_Voxels_From_Segment(
			in_voxels,//CKvDepot_of_Point3Df *in_voxels,
			in_segments[k],//std::vector<std::vector<int>> &in_voxel_sequence,
			in_ww,//int in_ww,
			in_hh,//int in_hh,
			in_dd,//int in_dd,
			&p_voxel_segment[k]);//CKvXrunset3dShort *out_xrunset3d)
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxels_From_Segment(
	CKvDepot_of_Point3Df *in_voxels,
	std::vector<std::vector<std::vector<int>>> &in_segments,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{
	CKvSet_of_Point3Df voxels;
	int num_segment, k;

	num_segment = in_segments.size();
	out_voxel_segments = std::vector<std::vector<std::vector<float>>>(num_segment);

	for (k = 0; k < num_segment; k++)
	{
		printf("	# of segments : %d/%d\r", k, num_segment);
		gv_Get_Voxels_From_Segment(
			in_voxels,//CKvDepot_of_Point3Df *in_voxels,
			in_segments[k],//std::vector<std::vector<int>> &in_voxel_sequence,
			out_voxel_segments[k]);//std::vector<std::vector<float>> &out_xrunset3d)
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxel_Colors_From_Segment(
	CKvDepot_of_RgbaF *in_voxel_colors,
	std::vector<std::vector<std::vector<int>>> &in_segments,
	std::vector<std::vector<std::vector<float>>> &out_voxel_colors)
//************************************************************************
{
	CKvSet_of_RgbaF voxels;
	int num_segment, k;

	num_segment = in_segments.size();
	out_voxel_colors = std::vector<std::vector<std::vector<float>>>(num_segment);

	for (k = 0; k < num_segment; k++)
	{
		printf("	# of segments : %d/%d\r", k, num_segment);
		gv_Get_Voxel_Colors_From_Segment(
			in_voxel_colors,//CKvDepot_of_RgbaF *in_voxel_colors,
			in_segments[k],//std::vector<std::vector<int>> &in_voxel_sequence,
			out_voxel_colors[k]);//std::vector<std::vector<float>> &out_voxel_colors)
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxels_From_Table(
	CKvDepot_of_Point3Df *in_voxels,
	std::vector<std::vector<int>> &in_table,
	std::vector<std::vector<std::vector<float>>> &out_voxel_segments)
//************************************************************************
{
	CKvSet_of_Point3Df voxels;
	float **p_in_voxels;
	int num_segment, num_voxel, k, l;

	num_segment = in_table.size();
	out_voxel_segments = std::vector<std::vector<std::vector<float>>>(num_segment);

	p_in_voxels = in_voxels->gps_Get_Pointer_of_Space()->mp();
	for (k = 0; k < num_segment; k++)
	{
		printf("	# of segments : %d/%d\r", k, num_segment);
		num_voxel = in_table[k].size();

		out_voxel_segments[k].resize(num_voxel, std::vector<float>(3));

		for (l = 0; l < num_voxel; l++)
		{
			out_voxel_segments[k][l][0] = p_in_voxels[in_table[k][l]][0];
			out_voxel_segments[k][l][1] = p_in_voxels[in_table[k][l]][1];
			out_voxel_segments[k][l][2] = p_in_voxels[in_table[k][l]][2];
		}
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxels_From_Segment(
	CKvDepot_of_Point3Df *in_voxels,
	std::vector<std::vector<int>> &in_voxel_sequence,
	CKvSet_of_Point3Df *out_voxels)
//************************************************************************
{
	CKvPoint3Df *p_voxel;
	float **p_in_voxels;
	int num_voxel=0, m, n;
	int num_slice, num_element;
	int count = 0;

	num_slice = in_voxel_sequence.size();
	for (m = 0 ; m < num_slice; m++)
	{
		num_voxel += in_voxel_sequence[m].size();
	}

	p_voxel     = out_voxels->c_Create(num_voxel);
	p_in_voxels = in_voxels->gps_Get_Pointer_of_Space()->mp();

	for (m = 0; m < num_slice; m++)
	{
		num_element = in_voxel_sequence[m].size();
		for (n = 0; n < num_element; n++)
		{
			p_voxel[count].x = p_in_voxels[in_voxel_sequence[m][n]][0];
			p_voxel[count].y = p_in_voxels[in_voxel_sequence[m][n]][1];
			p_voxel[count].z = p_in_voxels[in_voxel_sequence[m][n]][2];
			count++;
		}
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxels_From_Segment(
	CKvDepot_of_Point3Df *in_voxels,
	std::vector<std::vector<int>> &in_voxel_sequence,
	std::vector<std::vector<float>> &out_voxels)
//************************************************************************
{
	float **p_in_voxels;
	int num_voxel = 0, m, n;
	int num_slice, num_element;
	int count = 0;

	num_slice = in_voxel_sequence.size();
	for (m = 0; m < num_slice; m++)
	{
		num_voxel += in_voxel_sequence[m].size();
	}

	out_voxels.resize(num_voxel, std::vector<float>(3));

	p_in_voxels = in_voxels->gps_Get_Pointer_of_Space()->mp();

	for (m = 0; m < num_slice; m++)
	{
		num_element = in_voxel_sequence[m].size();
		for (n = 0; n < num_element; n++)
		{
			out_voxels[count][0] = p_in_voxels[in_voxel_sequence[m][n]][0];
			out_voxels[count][1] = p_in_voxels[in_voxel_sequence[m][n]][1];
			out_voxels[count][2] = p_in_voxels[in_voxel_sequence[m][n]][2];
			count++;
		}
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxel_Colors_From_Segment(
	CKvDepot_of_RgbaF *in_voxel_colors,
	std::vector<std::vector<int>> &in_voxel_sequence,
	std::vector<std::vector<float>> &out_voxel_colors)
//************************************************************************
{
	float **p_in_voxels;
	int num_voxel = 0, m, n;
	int num_slice, num_element;
	int count = 0;

	num_slice = in_voxel_sequence.size();
	for (m = 0; m < num_slice; m++)
	{
		num_voxel += in_voxel_sequence[m].size();
	}

	out_voxel_colors.resize(num_voxel, std::vector<float>(3));

	p_in_voxels = in_voxel_colors->gps_Get_Pointer_of_Space()->mp();

	for (m = 0; m < num_slice; m++)
	{
		num_element = in_voxel_sequence[m].size();
		for (n = 0; n < num_element; n++)
		{
			out_voxel_colors[count][0] = p_in_voxels[in_voxel_sequence[m][n]][0];
			out_voxel_colors[count][1] = p_in_voxels[in_voxel_sequence[m][n]][1];
			out_voxel_colors[count][2] = p_in_voxels[in_voxel_sequence[m][n]][2];
			count++;
		}
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxels_From_Segment(
	CKvDepot_of_Point3Df *in_voxels,
	std::vector<std::vector<int>> &in_voxel_sequence,
	int in_ww,
	int in_hh,
	int in_dd,
	CKvVolumeBool *out_volume)
//************************************************************************
{
	float **p_in_voxels;
	int m, n;
	int num_slice, num_element;
	bool ***p_volume;

	p_in_voxels = in_voxels->gps_Get_Pointer_of_Space()->mp();
	p_volume    = out_volume->c_Create(in_dd, in_hh, in_ww, false);
	num_slice = in_voxel_sequence.size();

	for (m = 0; m < num_slice; m++)
	{
		num_element = in_voxel_sequence[m].size();
		for (n = 0; n < num_element; n++)
		{
			p_volume[(int)p_in_voxels[in_voxel_sequence[m][n]][2]]
					[(int)p_in_voxels[in_voxel_sequence[m][n]][1]]
					[(int)p_in_voxels[in_voxel_sequence[m][n]][0]] = true;
		}
	}
}
//************************************************************************
void VCL_Voxel_Segmentation::gv_Get_Voxels_From_Segment(
	CKvDepot_of_Point3Df *in_voxels,
	std::vector<std::vector<int>> &in_voxel_sequence,
	int in_ww,
	int in_hh,
	int in_dd,
	CKvXrunset3dShort *out_xrunset3d)
//************************************************************************
{
	CKvVolumeBool volume;
	float **p_in_voxels;
	int m, n;
	int num_slice, num_element;
	bool ***p_volume;

	p_in_voxels = in_voxels->gps_Get_Pointer_of_Space()->mp();
	p_volume    = volume.c_Create(in_dd, in_hh, in_ww, false);
	num_slice   = in_voxel_sequence.size();

	for (m = 0; m < num_slice; m++)
	{
		num_element = in_voxel_sequence[m].size();
		for (n = 0; n < num_element; n++)
		{
			p_volume[(int)p_in_voxels[in_voxel_sequence[m][n]][2]]
					[(int)p_in_voxels[in_voxel_sequence[m][n]][1]]
					[(int)p_in_voxels[in_voxel_sequence[m][n]][0]] = true;
		}
	}

	out_xrunset3d->im_Import(volume);

}
//************************************************************************