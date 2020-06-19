#include "stdafx.h"
#include "VCL_DoCube_X_Color.h"
#include <math.h>
#include <vector>

//*******************************************************************************************
VCL_DoCube_X_Color::VCL_DoCube_X_Color()
//*******************************************************************************************
{
	zz_depth_data_w_bp.c_Create(1, 1);
}
//*******************************************************************************************
VCL_DoCube_X_Color::~VCL_DoCube_X_Color()
//*******************************************************************************************
{

}
//*******************************************************************************************
void VCL_DoCube_X_Color::cp_Copy_Data(VCL_DoCube_X_Color &in_docube)
//*******************************************************************************************
{
	CKvSet2d_of_XvectorShort depth_data_w_bp;
	CKvSet2d_of_XvectorInt point_to_rl;

	depth_data_w_bp = in_docube.e_Export_Xvectors_Boundary_Point();
	this->zz_depth_data_w_bp.cp_Copy(&depth_data_w_bp);
	
	point_to_rl = in_docube.grlv_Get_RL_to_Voxel_Index2();
	this->zz_set_point_to_rl_idx.cp_Copy(&point_to_rl);

	this->zz_depot_of_color.cp_Copy(&in_docube.gvc_Get_Voxel_Color());
	this->zz_color.cp_Copy(&in_docube.gvc_Get_Voxel_Color_2d());

	this->zz_surface_point3d.cp_Copy(&in_docube.gsp_Get_Surface_Points());
	this->zz_surface_voxels.cp_Copy(&in_docube.gsp_Get_Surface_Voxels());
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import(
	CKvSet2d_of_XvectorShort *in_runset)
//*******************************************************************************************
{
	CKvXvectorShort **p_xvec;
	CKvVectorShort **p_vec;
	int ww, hh, j, i;

	p_xvec = in_runset->mps(ww, hh);
	p_vec  = zz_depth_data.c_Create(hh, ww);
	zz_step_size_ = 2.0 / double(ww);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_xvec[j][i].ne() != 0)
			{
				p_xvec[j][i].ex_Export(&p_vec[j][i], NULL);
			}
			else
			{
				p_vec[j][i].c_Create(2, NOPOINTVALDoCube);
			}
		}
	}
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import(CKvSet2d_of_VectorShort *in_runset)
//*******************************************************************************************
{
	zz_depth_data.cp_Copy(in_runset);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import(
	CKvDepot_of_Point3Df *in_voxels,
	int in_ww,
	int in_hh,
	int in_dd)
//*******************************************************************************************
{
	CKvRanker rank;
	CKvXrunset3dShort surface_rle, docube_rle;
	CKvVectorBool pattern; CKvXrunset1dShort xrunset1d;
	CKvXvectorShort **pvec; CKvVectorShort rl, **p_depth;
	CKvPoint3D point3d; 
	CKvHmatrix3D hmat3d; CKvMatrix H;
	short *p_rl;
	int j, i, k, num_voxel, np;
	bool *p_pattern;

	p_depth = zz_depth_data.c_Create(in_hh, in_ww);
	pvec = zz_depth_data_w_bp.c_Create(in_hh, in_ww);
	num_voxel = in_voxels->ne_Number_of_Elements();
	zz_step_size_ = 2.0 / double(in_ww);
	/******************************************************************************/
	for (i = 0; i < in_ww; i++)
	{
		for (j = 0; j < in_hh; j++)
		{
			pvec[j][i].in_Initialize(1);
		}
	}

	for (k = 0; k < num_voxel; k++)
	{
		in_voxels->ge_Get_Element(k, point3d);
		pvec[in_hh - 1 - (int)point3d.y][in_ww - 1 - (int)point3d.x].ae_Append_Element((short)point3d.z);
	}
	/******************************************************************************/

	/******************************************************************************/
	pattern.c_Create(in_dd, false);
	for (i = 0; i < in_ww; i++)
	{
		for (j = 0; j < in_hh; j++)
		{
			pattern.sv_Set_Vector(false);
			if (pvec[j][i].ne() != 0)
			{
				p_pattern = pattern.vp();
				pvec[j][i].ex_Export(&rl, &np);

				rank.s_Sort(&rl, false, 0, np, NULL);

				p_rl = rl.vp();
				for (k = 0; k < np; k++)
				{
					p_pattern[(int)p_rl[k]] = true;
				}
				xrunset1d.im_Import(pattern);
				xrunset1d.ex_Export(pvec[j][i], NULL);
			}
			else
			{
				xrunset1d.im_Import(pattern);
				xrunset1d.ex_Export(pvec[j][i], NULL);
			}

		}
	}
	/******************************************************************************/
	surface_rle.im_Import(zz_depth_data_w_bp, in_dd);
	cd_Conversion_Data_Surface_Voxel_To_DoCube(
		&surface_rle,//CKvXrunset3dShort *in_surface_rle,
		&docube_rle);//CKvXrunset3dShort *out_docube_rle);
	surface_rle.cn_Create_Null_pattern(1, 1, 1);
	H.ci_Create_Identity_matrix(4);
	hmat3d.i_Import(&H);
	this->CKvDoCubeShort::im_Import(hmat3d, docube_rle);
	docube_rle.cn_Create_Null_pattern(1, 1, 1);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import(
	CKvDepot_of_Point3Df *in_3d_points,
	int in_hh, 
	int in_ww, 
	int in_dd,
	CKvDepot_of_RgbaF *in_depot_colors_or_NULL,
	bool in_inverse_true_or_false)
//*******************************************************************************************
{
	CKvRanker rank;
	CKvXrunset3dShort surface_rle, docube_rle;
	CKvSet2d_of_XvectorUcharRgb set2d_rgb;
	CKvSet2d_of_XvectorShort tmp_depth; 
	CKvVectorBool pattern; CKvXrunset1dShort xrunset1d;
	CKvXvectorShort **pvec; CKvVectorShort rl;
	CKvXvectorUcharRgb **pcolor=NULL; CKvVectorUcharRgb color, cp_color;
	CKvVectorInt idx;
	CKvPoint3D point3d; CKvRgbaF rgbaf; CKvRgb rgb;
	CKvHmatrix3D hmat3d; CKvMatrix H;
	unsigned char *pr1=NULL, *pg1 = NULL, *pb1 = NULL, *pr2 = NULL, *pg2 = NULL, *pb2 = NULL;
	short *p_rl;
	int j, i, k, num_voxel, np;
	int *p_idx; bool *p_pattern;

	sc_Set_Cube(in_ww, in_hh, in_dd);

	pvec = zz_depth_data_w_bp.c_Create(in_hh, in_ww);
	if (in_depot_colors_or_NULL != NULL)
	{
		pcolor = set2d_rgb.c_Create(in_hh, in_ww);
	}

	num_voxel = in_3d_points->ne_Number_of_Elements();
	zz_step_size_ = 2.0 / double(in_ww);
	/******************************************************************************/
	for (i = 0; i < in_ww; i++)
	{
		for (j = 0; j < in_hh; j++)
		{
			pvec[j][i].in_Initialize(in_dd);
			if (in_depot_colors_or_NULL != NULL)
			{
				pcolor[j][i].in_Initialize(in_dd);
			}
		}
	}

	for (k = 0; k < num_voxel; k++)
	{
		in_3d_points->ge_Get_Element(k, point3d);

		if (((point3d.x < 0) || (point3d.x > (in_ww - 1))) ||
			((point3d.y < 0) || (point3d.y > (in_hh - 1))) ||
			((point3d.z < 0) || (point3d.z > (in_dd - 1))))
		{
			continue;
		}

		if (in_depot_colors_or_NULL != NULL)
		{
			in_depot_colors_or_NULL->ge_Get_Element(k, rgbaf);
			rgb.r = (unsigned char)(rgbaf.r*255.f);
			rgb.g = (unsigned char)(rgbaf.g*255.f);
			rgb.b = (unsigned char)(rgbaf.b*255.f);
		}


		if (!in_inverse_true_or_false)
		{
			pvec[(int)round(point3d.y)][(int)round(point3d.x)].ae_Append_Element((short)point3d.z);
			if (in_depot_colors_or_NULL != NULL)
			{
				pcolor[(int)round(point3d.y)][(int)round(point3d.x)].ae_Append_Element(rgb);
			}

		}
		else
		{
			pvec[in_hh - 1 - (int)round(point3d.y)][in_ww - 1 - (int)round(point3d.x)].ae_Append_Element((short)point3d.z);
			if (in_depot_colors_or_NULL != NULL)
			{
				pcolor[in_hh - 1 - (int)round(point3d.y)][in_ww - 1 - (int)round(point3d.x)].ae_Append_Element(rgb);
			}
		}
	}
	/******************************************************************************/

	/******************************************************************************/
	pattern.c_Create(in_dd, false);
	for (i = 0; i < in_ww; i++)
	{
		for (j = 0; j < in_hh; j++)
		{
			pattern.sv_Set_Vector(false);
			if (pvec[j][i].ne() != 0)
			{
				p_pattern = pattern.vp();
				pvec[j][i].ex_Export(&rl, &np);
				if (in_depot_colors_or_NULL != NULL)
				{
					pcolor[j][i].ex_Export(&color, NULL);
					cp_color.cp_Copy(&color);
				}


				rank.s_Sort(&rl, false, 0, np, &idx);

				p_rl = rl.vp();
				p_idx = idx.vp();
				if (in_depot_colors_or_NULL != NULL)
				{
					pr1 = color.vp(pg1, pb1);
					pr2 = cp_color.vp(pg2, pb2);
				}

				for (k = 0; k < np; k++)
				{
					p_pattern[(int)p_rl[k]] = true;
					if (in_depot_colors_or_NULL != NULL)
					{
						pr1[k] = pr2[p_idx[k]];
						pg1[k] = pg2[p_idx[k]];
						pb1[k] = pb2[p_idx[k]];
					}

				}
				xrunset1d.im_Import(pattern);
				xrunset1d.ex_Export(pvec[j][i], NULL);

				if (in_depot_colors_or_NULL != NULL)
				{
					pcolor[j][i].im_Import(&color);
				}

			}
			else
			{
				xrunset1d.im_Import(pattern);
				xrunset1d.ex_Export(pvec[j][i], NULL);
			}
		}
	}

	/******************************************************************************/
	//printf("Copy Data and Conversion\n");
	//surface_rle.im_Import(zz_depth_data_w_bp, in_dd);
	//cd_Conversion_Data_Surface_Voxel_To_DoCube(
	//	&surface_rle,//CKvXrunset3dShort *in_surface_rle,
	//	&docube_rle);//CKvXrunset3dShort *out_docube_rle);
	//surface_rle.cn_Create_Null_pattern(1, 1, 1);
	//
	//H.ci_Create_Identity_matrix(4);
	//hmat3d.i_Import(&H);
	//this->CKvDoCubeShort::im_Import(hmat3d, docube_rle);
	//docube_rle.cn_Create_Null_pattern(1, 1, 1);
	/******************************************************************************/

	if (in_depot_colors_or_NULL != NULL)
	{
		i_Import_Color_and_Convert_Vector(&set2d_rgb, num_voxel);
	}

	ddc_Decode_DoCube(
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		true,//bool in_mode_int_true_or_float_false,
		&zz_surface_point3d);//CKvDepot_of_Point3Df *out_set_of_voxel);

	CKvSet_of_Voxel set_voxels; CKvVoxel *p_sv;
	p_sv = set_voxels.c_Create(num_voxel);
	float **p_pt3d = zz_surface_point3d.gps_Get_Pointer_of_Space()->mp();
	for (k = 0; k < num_voxel; k++)
	{
		p_sv[k].x = (int)p_pt3d[k][0];
		p_sv[k].y = (int)p_pt3d[k][1];
		p_sv[k].z = (int)p_pt3d[k][2];
	}
	zz_surface_voxels.im_Import(&set_voxels);

}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_ordered_data(
	CKvDepot_of_Point3Df *in_3d_points,
	int in_hh,
	int in_ww,
	int in_dd,
	CKvDepot_of_RgbaF *in_depot_colors)
//*******************************************************************************************
{
	CKvSet_of_Voxel set_voxels; CKvVoxel *p_sv;
	CKvXrunset3dShort surface_rle, docube_rle;
	CKvRanker rank;
	CKvVectorBool pattern; CKvXrunset1dShort xrunset1d;
	CKvXvectorShort **pvec; CKvVectorShort rl;
	CKvVectorInt idx;
	CKvPoint3D point3d;
	CKvHmatrix3D hmat3d; CKvMatrix H;
	short *p_rl;
	int j, i, k, num_voxel, np;
	bool *p_pattern;

	
	sr_Set_Resolution(in_ww, in_hh, in_dd);
	
	zz_Xlim[0] = in_ww + 1; zz_Xlim[1] = -1;
	zz_Ylim[0] = in_hh + 1; zz_Ylim[1] = -1;

	pvec = zz_depth_data_w_bp.c_Create(in_hh, in_ww);
	num_voxel = in_3d_points->ne_Number_of_Elements();
	zz_step_size_ = 2.0 / double(in_ww);
	/******************************************************************************/
	for (i = 0; i < in_ww; i++)
	{
		for (j = 0; j < in_hh; j++)
		{
			pvec[j][i].in_Initialize(in_dd);
		}
	}

	for (k = 0; k < num_voxel; k++)
	{
		in_3d_points->ge_Get_Element(k, point3d);

		if (((point3d.x < 0) || (point3d.x > (in_ww - 1))) ||
			((point3d.y < 0) || (point3d.y > (in_hh - 1))) ||
			((point3d.z < 0) || (point3d.z > (in_dd - 1))))
		{
			continue;
		}

		pvec[(int)point3d.y][(int)point3d.x].ae_Append_Element((short)point3d.z);
	}
	/******************************************************************************/

	/******************************************************************************/
	pattern.c_Create(in_dd, false);
	for (i = 0; i < in_ww; i++)
	{
		for (j = 0; j < in_hh; j++)
		{
			pattern.sv_Set_Vector(false);
			if (pvec[j][i].ne() != 0)
			{
				p_pattern = pattern.vp();
				pvec[j][i].ex_Export(&rl, &np);
				rank.s_Sort(&rl, false, 0, np, &idx);
				p_rl  = rl.vp();
				for (k = 0; k < np; k++)
				{
					p_pattern[(int)p_rl[k]] = true;
				}
				xrunset1d.im_Import(pattern);
				xrunset1d.ex_Export(pvec[j][i], NULL);

				if (zz_Xlim[0] > i) { zz_Xlim[0] = i; }
				if (zz_Xlim[1] < i) { zz_Xlim[1] = i; }
				if (zz_Ylim[0] > j) { zz_Ylim[0] = j; }
				if (zz_Ylim[1] < j) { zz_Ylim[1] = j; }
			}
			else
			{
				xrunset1d.im_Import(pattern);
				xrunset1d.ex_Export(pvec[j][i], NULL);
			}
		}
	}
	//printf("Copy Data and Conversion\n");
	/******************************************************************************/

	/******************************************************************************/
	//surface_rle.im_Import(zz_depth_data_w_bp, in_dd);
	//cd_Conversion_Data(
	//	&surface_rle,//CKvXrunset3dShort *in_surface_rle,
	//	&docube_rle);//CKvXrunset3dShort *out_docube_rle);
	//surface_rle.cn_Create_Null_pattern(1, 1, 1);
	//H.ci_Create_Identity_matrix(4);
	//hmat3d.i_Import(&H);
	//this->CKvDoCubeShort::im_Import(hmat3d, docube_rle);
	//docube_rle.cn_Create_Null_pattern(1, 1, 1);
	/******************************************************************************/
	i_Import_Color_Vector(in_depot_colors);
	ddc_Decode_DoCube(
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		true,//bool in_mode_int_true_or_float_false,
		&zz_surface_point3d);//CKvDepot_of_Point3Df *out_set_of_voxel);

	p_sv = set_voxels.c_Create(num_voxel);
	float **p_pt3d = zz_surface_point3d.gps_Get_Pointer_of_Space()->mp();
	for (k = 0; k < num_voxel; k++)
	{
		p_sv[k].x = (int)p_pt3d[k][0];
		p_sv[k].y = (int)p_pt3d[k][1];
		p_sv[k].z = (int)p_pt3d[k][2];
	}
	zz_surface_voxels.im_Import(&set_voxels);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_ordered_data2(
	CKvDepot_of_Point3Df *in_3d_points,
	int in_hh,
	int in_ww,
	int in_dd,
	CKvDepot_of_RgbaF *in_depot_colors)
//*******************************************************************************************
{
	CKvSet_of_Voxel set_voxels; CKvVoxel *p_sv;
	CKvXrunset3dShort surface_rle, docube_rle;
	CKvRanker rank;
	CKvVectorBool pattern; CKvXrunset1dShort xrunset1d;
	CKvXvectorShort **pvec; CKvVectorShort rl;
	CKvVectorInt idx;
	CKvPoint3D point3d;
	CKvHmatrix3D hmat3d; CKvMatrix H;
	int j, i, k, num_voxel;

	CKvSet2d_of_VectorBool set2d_bool;
	CKvVectorBool **p_bool;
	sr_Set_Resolution(in_ww, in_hh, in_dd);

	pvec = zz_depth_data_w_bp.c_Create(in_hh, in_ww);
	p_bool = set2d_bool.c_Create(in_hh, in_ww);
	num_voxel = in_3d_points->ne_Number_of_Elements();
	zz_step_size_ = 2.0 / double(in_ww);
	/******************************************************************************/
#pragma omp parallel
	{
#pragma omp for private(j)
		for (i = 0; i < in_ww; i++)
		{
			for (j = 0; j < in_hh; j++)
			{
				p_bool[j][i].c_Create(in_dd, false);
			}
		}
	}



	double pt_array[3];
#pragma omp parallel
	{
#pragma omp for private(pt_array)
		for (k = 0; k < num_voxel; k++)
		{
			//in_3d_points->ge_Get_Element(k, point3d);
			in_3d_points->ge_Get_Element(k, pt_array);

			if (((pt_array[0] < 0) || (pt_array[0] > (in_ww - 1))) ||
				((pt_array[1] < 0) || (pt_array[1] > (in_hh - 1))) ||
				((pt_array[2] < 0) || (pt_array[2] > (in_dd - 1))))
			{
				continue;
			}

			p_bool[(int)pt_array[1]][(int)pt_array[0]].se_Set_Element((int)pt_array[2], true);

			//p_pattern = p_bool[(int)pt_array[1]][(int)pt_array[0]].vp();
			//p_pattern[(int)pt_array[2]] = true;
		}
	}

	/******************************************************************************/

	/******************************************************************************/
//#pragma omp parallel
//	{
//#pragma omp for private(j) private(xrunset1d)
		for (i = 0; i < in_ww; i++)
		{
			for (j = 0; j < in_hh; j++)
			{
				xrunset1d.im_Import(p_bool[j][i]);
				xrunset1d.ex_Export(pvec[j][i], NULL);
				//convert_data(&p_bool[j][i], &pvec[j][i]);
			}
		}
//	}


	/******************************************************************************/

	/******************************************************************************/
	i_Import_Color_Vector(in_depot_colors);
	ddc_Decode_DoCube(
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		true,//bool in_mode_int_true_or_float_false,
		&zz_surface_point3d);//CKvDepot_of_Point3Df *out_set_of_voxel);

	p_sv = set_voxels.c_Create(num_voxel);
	float **p_pt3d = zz_surface_point3d.gps_Get_Pointer_of_Space()->mp();
	for (k = 0; k < num_voxel; k++)
	{
		p_sv[k].x = (int)p_pt3d[k][0];
		p_sv[k].y = (int)p_pt3d[k][1];
		p_sv[k].z = (int)p_pt3d[k][2];
	}
	zz_surface_voxels.im_Import(&set_voxels);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_Surface_Voxels(
	CKvSet2d_of_XvectorShort *in_runset)
//*******************************************************************************************
{
	int ww, hh;

	zz_depth_data_w_bp.cp_Copy(in_runset);
	zz_depth_data_w_bp.ms(ww, hh);

	zz_step_size_ = 2.0 / double(ww);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_DoCube_Using_Surface_Voxels(
	CKvSet2d_of_XvectorShort *in_runset,
	int in_dd)
//*******************************************************************************************
{
	CKvXrunset3dShort surface_rle, docube_rle;
	CKvMatrix H; CKvHmatrix3D hmat3d;

	surface_rle.im_Import(*in_runset, in_dd);
	cd_Conversion_Data_Surface_Voxel_To_DoCube(
		&surface_rle,//CKvXrunset3dShort *in_surface_rle,
		&docube_rle);//CKvXrunset3dShort *out_docube_rle);
	surface_rle.cn_Create_Null_pattern(1, 1, 1);

	H.ci_Create_Identity_matrix(4);
	hmat3d.i_Import(&H);
	this->CKvDoCubeShort::im_Import(hmat3d, docube_rle);
	docube_rle.cn_Create_Null_pattern(1, 1, 1);

}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_Color(
	CKvDepot_of_RgbaF *in_depot_of_color)
//*******************************************************************************************
{	
	CKvXvectorInt **xvec_idx; 
	CKvXvectorUcharRgb **p_xcolor;
	CKvXvectorShort **xvec;
	CKvRgbaF rgbaf; CKvRgb rgb;
	int ww, hh, np, j, i, k, count;
	short *a;

	i_Import_Color_Vector(in_depot_of_color);

	xvec     = zz_depth_data_w_bp.mps(ww,hh);
	xvec_idx = zz_set_point_to_rl_idx.c_Create(hh, ww);
	p_xcolor = zz_color.c_Create(hh, ww);


	count = 0;
	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			xvec[j][i].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			xvec_idx[j][i].in_Initialize(1);
			p_xcolor[j][i].in_Initialize(1);

			for (k = 0; k < np; k++)
			{
				xvec_idx[j][i].ae_Append_Element(count);
				in_depot_of_color->ge_Get_Element(count, rgbaf);
				rgb.r = (unsigned char)(rgbaf.r * 255);
				rgb.g = (unsigned char)(rgbaf.g * 255);
				rgb.b = (unsigned char)(rgbaf.b * 255);
				p_xcolor[j][i].ae_Append_Element(rgb);
				count++;
			}
		}
	}
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_Color_and_Convert_Vector(
	CKvSet2d_of_XvectorUcharRgb *in_colors,
	int in_num_voxel)
//*******************************************************************************************
{
	CKvSet_of_RgbaF set_rgbaf;
	CKvXvectorUcharRgb **p_xcolor;
	CKvRgbaF rgbaf, *p_rgb;
	unsigned char *r, *g, *b;
	int ww, hh, np, j, i, k, count;

	in_colors->ms(ww, hh);
	p_xcolor = in_colors->mp();

	count = 0;
	p_rgb = set_rgbaf.c_Create(in_num_voxel);
	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			p_xcolor[j][i].z_gii(NULL, &r, &g, &b, &np, NULL);
			if (np != 0)
			{
				for (k = 0; k < np; k++)
				{
					p_rgb[count].r = (float)r[k] / 255.f;
					p_rgb[count].g = (float)g[k] / 255.f;
					p_rgb[count].b = (float)b[k] / 255.f;
					p_rgb[count].a = 1.f;
					count++;
				}
			}
		}
	}

	zz_depot_of_color.im_Import(&set_rgbaf);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_Color_Vector_and_Convert_2D(
	CKvDepot_of_RgbaF *in_depot_of_color_in_order_of_DoCube)
//*******************************************************************************************
{
	CKvXvectorShort **xvec;
	CKvXvectorUcharRgb **p_xcolor;
	CKvXvectorInt **xvec_idx;
	CKvRgbaF rgbaf; CKvRgb rgb;
	int ww, hh, np, j, i, k, z, count, count_sub;
	short *a, zo, z1;

	xvec     = zz_depth_data_w_bp.mps(ww, hh);
	p_xcolor = zz_color.c_Create(hh, ww);
	xvec_idx = zz_set_point_to_rl_idx.c_Create(hh, ww);

	count = 0;
	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			xvec[j][i].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			xvec_idx[j][i].in_Initialize(10);
			p_xcolor[j][i].in_Initialize(10);
			count_sub = 0;
			for (k = 0; k < np;)
			{
				zo = a[k]; k++;
				z1 = a[k]; k++;
				for (z = zo; z <= z1; z++)
				{
					in_depot_of_color_in_order_of_DoCube->ge_Get_Element(count, rgbaf);

					rgb.r = rgbaf.r*255.f;
					rgb.g = rgbaf.g*255.f;
					rgb.b = rgbaf.b*255.f;

					p_xcolor[j][i].ae_Append_Element(rgb);
					xvec_idx[j][i].ae_Append_Element(count);
					count++;
					count_sub++;
				}
			}
			if (count_sub != 0)
			{
				p_xcolor[j][i].z_cne_Change_Number_of_Elements(count_sub);
				xvec_idx[j][i].z_cne_Change_Number_of_Elements(count_sub);
			}
		}
	}

	zz_depot_of_color.cne_Change_Number_of_Elements(count);

	i_Import_Color_Vector(in_depot_of_color_in_order_of_DoCube);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_Color_Palette(
	CKvSet_of_MatrixUcharRgb *in_color_palette,
	CKvSet_of_MatrixInt *in_voxel_color_indices)
//*******************************************************************************************
{

}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_Color_Palette(
	CKvVectorUchar *in_intensity,
	CKvSet_of_YCbCr *in_cbcr,
	CKvMatrixInt *in_voxel_color_indices)
//*******************************************************************************************
{
	LCKvUtility_for_YCbCr lucc;
	CKvVectorInt point_idx, color_idx, point_sz; CKvSet_of_VectorInt element;
	CKvYCbCr *p_cbcr, YCbCr; CKvRgbaF rgbaf;
	int offset[3], dum, ww, num_element, k;
	int **p_pvc, *p_point_idx, *p_color_idx;
	unsigned char *p_intensity, r, g, b;

	p_pvc = in_voxel_color_indices->mps(ww, num_element);

	point_sz.c_Create(num_element, 5);
	p_point_idx = point_idx.c_Create(num_element);
	p_color_idx = color_idx.c_Create(num_element);
	p_cbcr = in_cbcr->vp();
	p_intensity = in_intensity->vp();
	zz_depot_of_color.in_Initialize(num_element);

	for (k = 0 ; k < num_element ; k++)
	{
		YCbCr.Y = p_intensity[p_pvc[k][1]];
		YCbCr.Cb = p_cbcr[p_pvc[k][0]].Cb;
		YCbCr.Cr = p_cbcr[p_pvc[k][0]].Cr;

		lucc.ccyr_Convert_YCbCr_to_RGB(false, YCbCr.Y, YCbCr.Cb, YCbCr.Cr, r, g, b);
		rgbaf.r = (float)r / 255.f; rgbaf.g = (float)g / 255.f; rgbaf.b = (float)b / 255.f;
		rgbaf.a = 0.f;
		zz_depot_of_color.ap_Append(false, rgbaf, dum);
		p_color_idx[k] = k; p_point_idx[k] = k;
	}


	offset[0] = offset[1] = offset[2] = 0;
	zz_mesh_of_point.u_me_Make_Element(&point_idx, &color_idx, &point_sz, &element);
	zz_mesh_of_point.ap_Append(true, &element, offset, dum);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::i_Import_Color_Palette(
	CKvMatrixUchar *in_intensity,
	CKvSet_of_YCbCr *in_cbcr,
	CKvMatrixInt *in_voxel_color_indices)
//*******************************************************************************************
{
	LCKvUtility_for_YCbCr lucc;
	CKvXvectorShort **p_bp;
	CKvMatrixInt **p_pi; CKvXRmatrixInt xr_mat;
	CKvVectorInt point_idx, color_idx, point_sz, voxel_indices; CKvSet_of_VectorInt element;
	CKvYCbCr *p_cbcr, YCbCr; CKvRgbaF rgbaf;
	short *a, zo,z1;
	unsigned char **p_intensity,r,g,b;
	int **p_ppi, **p_pvc, *p_point_idx, *p_color_idx, offset[3];
	int ww, hh, j, i, k, z, num_element, np, dum;

	p_bp = zz_depth_data_w_bp.mps(ww,hh);
	p_pi = zz_palette_index.c_Create(hh, ww);

	p_pvc = in_voxel_color_indices->mp();
	num_element = 0;
	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			p_bp[j][i].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			xr_mat.in_Initialize(1, 3);
			for (k = 0; k < np;)
			{
				zo = a[k]; k++;
				z1 = a[k]; k++;
				
				voxel_indices.c_Create(3);
				for (z = zo; z <= z1; z++)
				{
					voxel_indices.se_Set_Element(0, p_pvc[num_element][0]);
					voxel_indices.se_Set_Element(1, p_pvc[num_element][1]);
					voxel_indices.se_Set_Element(2, p_pvc[num_element][2]);

					xr_mat.ae_Append_Element(&voxel_indices);
					num_element++;
				}
			}
			if (np != 0){ xr_mat.ex_Export(&p_pi[j][i], NULL); }
			else{ p_pi[j][i].c_Create(1, 1, -2); }
		}
	}

	point_sz.c_Create(num_element, 5);
	p_point_idx = point_idx.c_Create(num_element);
	p_color_idx = color_idx.c_Create(num_element);
	//p_pvc       = in_voxel_color_indices->mps(ww,hh);
	p_cbcr      = in_cbcr->vp();
	p_intensity = in_intensity->mp();
	zz_depot_of_color.in_Initialize(num_element);


	//Ver DEBUG
	/**********************************************************************************/
	num_element = 0;
	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			p_ppi = p_pi[j][i].mps(dum, np);
			if (dum != 1)
			{
				for (k = 0; k < np; k++)
				{
					YCbCr.Y = p_intensity[p_ppi[k][2]][p_ppi[k][1]];
					YCbCr.Cb = p_cbcr[p_ppi[k][0]].Cb;
					YCbCr.Cr = p_cbcr[p_ppi[k][0]].Cr;

					lucc.ccyr_Convert_YCbCr_to_RGB(false, YCbCr.Y, YCbCr.Cb, YCbCr.Cr, r, g, b);
					rgbaf.r = (float)r / 255.f;
					rgbaf.g = (float)g / 255.f;
					rgbaf.b = (float)b / 255.f;
					rgbaf.a = 0.f;

					zz_depot_of_color.ap_Append(false, rgbaf, dum);
					p_color_idx[num_element] = num_element; p_point_idx[num_element] = num_element;
					num_element++;
				}
			}
		}
	}


	//Ver 1
	/**********************************************************************************/
	//for (k = 0; k < num_element; k++)
	//{
	//	YCbCr.Y  = p_intensity[p_pvc[k][2]][p_pvc[k][1]];
	//	YCbCr.Cb = p_cbcr[p_pvc[k][0]].Cb;
	//	YCbCr.Cr = p_cbcr[p_pvc[k][0]].Cr;
	//	
	//	lucc.ccyr_Convert_YCbCr_to_RGB(false, YCbCr.Y, YCbCr.Cb, YCbCr.Cr, r, g, b);
	//	rgbaf.r = (float)r / 255.f; 
	//	rgbaf.g = (float)g / 255.f; 
	//	rgbaf.b = (float)b / 255.f;
	//	rgbaf.a = 0.f;

	//	//rgbaf.r = (float)YCbCr.Y / 255.f;
	//	//rgbaf.g = (float)YCbCr.Cb / 255.f; 
	//	//rgbaf.b = (float)YCbCr.Cr / 255.f;
	//	//rgbaf.a = 0.f;

	//	zz_depot_of_color.ap_Append(false, rgbaf, dum);
	//	p_color_idx[k] = k; p_point_idx[k] = k;
	//}
	/**********************************************************************************/

	offset[0] = offset[1] = offset[2] = 0;
	zz_mesh_of_point.u_me_Make_Element(&point_idx, &color_idx, &point_sz, &element);
	zz_mesh_of_point.ap_Append(true, &element, offset, dum);
}
//*******************************************************************************************
bool VCL_DoCube_X_Color::e_Export_Xvectors_Boundary_Point(
	CKvSet2d_of_XvectorShort *out_depth_data_w_bp)
//*******************************************************************************************
{
	CKvXvectorShort **p1, **p2;
	int ww, hh, j, i;

	p1 = zz_depth_data_w_bp.mps(ww, hh);
	p2 = out_depth_data_w_bp->c_Create(hh, ww);

#pragma omp parallel
	{
#pragma omp for private(i)
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				p2[j][i].cp_Copy(&p1[j][i]);
			}
		}
	}


	return true;
}
//*******************************************************************************************
void VCL_DoCube_X_Color::gmd_Generate_Modified_DoCube()
//*******************************************************************************************
{
	CKvStopWatch sw;
	CKvSet2d_of_XvectorShort depth_data_x;

	cd_Conversion_Data(
		&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
		&depth_data_x);//CKvSet2d_of_XvectorShort *out_set_of_vector)

	sw.c_Create(1);
	sw.r_Reset(0);

	fbp_Find_Boundary_Points(
		depth_data_x,//CKvSet2d_of_XvectorShort &in_set_of_runsets,
		zz_number_of_levels,//int in_length_of_ray_segment,
		false,//bool in_MODE_for_TESTing_input_runsets,
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
		NULL,//CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
		NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)

	printf("New Generate Surface Points Time: %f\n", sw.get_Get_Elapsed_Time(0));

}
//*******************************************************************************************
void VCL_DoCube_X_Color::gsp_Generate_Indices_of_Surface_Points()
//*******************************************************************************************
{
	CKvSet2d_of_XvectorShort depth_data_x;
	CKvSet_of_Voxel set_voxels;
	int ww, hh;

	zz_depth_data_w_bp.ms(ww, hh);
	if ((ww == 1) && (hh == 1))
	{
		cd_Conversion_Data(
			&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
			&depth_data_x);//CKvSet2d_of_XvectorShort *out_set_of_vector)

		fbp_Find_Boundary_Points(
			depth_data_x,//CKvSet2d_of_XvectorShort &in_set_of_runsets,
			zz_number_of_levels,//int in_length_of_ray_segment,
			false,//bool in_MODE_for_TESTing_input_runsets,
			&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
			NULL,//CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
			NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
	}

	ddc_Decode_DoCube_Indices(
		&zz_depth_data_w_bp);//CKvSet2d_of_XvectorShort *in_set_of_vector,
}
//*******************************************************************************************
void VCL_DoCube_X_Color::gsp_Generate_Surface_Voxels(
	CKvSet_of_Voxel *out_set_of_voxel)
//*******************************************************************************************
{
	CKvSet2d_of_XvectorShort depth_data_x;
	int ww, hh;

	zz_depth_data_w_bp.ms(ww, hh);
	printf("ww = %d hh = %d\n", ww, hh);
	if ((ww == 1) && (hh == 1))
	{
		cd_Conversion_Data(
			&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
			&depth_data_x);//CKvSet2d_of_XvectorShort *out_set_of_vector)

		fbp_Find_Boundary_Points(
			depth_data_x,//CKvSet2d_of_XvectorShort &in_set_of_runsets,
			zz_number_of_levels,//int in_length_of_ray_segment,
			false,//bool in_MODE_for_TESTing_input_runsets,
			&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
			NULL,//CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
			NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
	}

	ddc_Decode_DoCube(
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		out_set_of_voxel);//CKvSet_of_Voxel *out_set_of_voxel)

	zz_surface_voxels.im_Import(out_set_of_voxel);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::gsp_Generate_Surface_Voxels(
	CKvDepot_of_Voxel *out_set_of_voxel)
//*******************************************************************************************
{
	CKvSet2d_of_XvectorShort depth_data_x;
	CKvSet_of_Voxel set_of_voxel;
	int ww, hh;

	if (zz_surface_point3d.ne_Number_of_Elements() != 0)
	{
		int num = zz_surface_point3d.ne_Number_of_Elements();
		CKvVoxel *p_sv = set_of_voxel.c_Create(num);
		double pt3[3];

		for (int k = 0; k < num; k++)
		{
			zz_surface_point3d.ge_Get_Element(k, pt3);
			p_sv[k].x = pt3[0];
			p_sv[k].y = pt3[1];
			p_sv[k].z = pt3[2];
		}
	}
	else
	{
		zz_depth_data_w_bp.ms(ww, hh);
		if ((ww == 1) && (hh == 1))
		{
			cd_Conversion_Data(
				&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
				&depth_data_x);//CKvSet2d_of_XvectorShort *out_set_of_vector)

			fbp_Find_Boundary_Points(
				depth_data_x,//CKvSet2d_of_XvectorShort &in_set_of_runsets,
				zz_number_of_levels,//int in_length_of_ray_segment,
				false,//bool in_MODE_for_TESTing_input_runsets,
				&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
				NULL,//CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
				NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
		}

		ddc_Decode_DoCube(
			&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
			&set_of_voxel);//CKvSet_of_Voxel *out_set_of_voxel)
	}


	zz_surface_voxels.im_Import(&set_of_voxel);
	if (out_set_of_voxel != NULL)
	{
		out_set_of_voxel->cp_Copy(&zz_surface_voxels);
	}
}
//*******************************************************************************************
void VCL_DoCube_X_Color::gsp_Generate_Surface_Points(
	CKvDepot_of_Point3Df *out_set_of_voxel)
//*******************************************************************************************
{
	CKvSet2d_of_XvectorShort depth_data_x;
	int ww, hh;

	zz_depth_data_w_bp.ms(ww, hh);
	if ((ww == 1) && (hh == 1))
	{
		cd_Conversion_Data(
			&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
			&depth_data_x);//CKvSet2d_of_XvectorShort *out_set_of_vector)

		fbp_Find_Boundary_Points(
			depth_data_x,//CKvSet2d_of_XvectorShort &in_set_of_runsets,
			zz_number_of_levels,//int in_length_of_ray_segment,
			false,//bool in_MODE_for_TESTing_input_runsets,
			&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
			NULL,//CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
			NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
	}

	ddc_Decode_DoCube(
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		false,
		&zz_surface_point3d);//CKvSet_of_Voxel *out_set_of_voxel)
	out_set_of_voxel->cp_Copy(&zz_surface_point3d);
}
//*******************************************************************************************
void VCL_DoCube_X_Color::gsp_Generate_Surface_Points(
	bool in_mode_int_true_or_float_false,
	CKvDepot_of_Point3Df *out_set_of_voxel)
//*******************************************************************************************
{
	CKvSet2d_of_XvectorShort depth_data_x;
	int ww, hh;

	zz_depth_data_w_bp.ms(ww, hh);
	if ((ww == 1) && (hh == 1))
	{
		cd_Conversion_Data(
			&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
			&depth_data_x);//CKvSet2d_of_XvectorShort *out_set_of_vector)

		fbp_Find_Boundary_Points(
			depth_data_x,//CKvSet2d_of_XvectorShort &in_set_of_runsets,
			zz_number_of_levels,//int in_length_of_ray_segment,
			false,//bool in_MODE_for_TESTing_input_runsets,
			&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
			NULL,//CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
			NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
	}

	ddc_Decode_DoCube(
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		in_mode_int_true_or_float_false,
		&zz_surface_point3d);//CKvSet_of_Voxel *out_set_of_voxel)
	out_set_of_voxel->cp_Copy(&zz_surface_point3d);

}
//*******************************************************************************************
void VCL_DoCube_X_Color::gsp_Generate_Surface_Points_Condition_Free(
	bool in_mode_int_true_or_float_false, 
	CKvDepot_of_Point3Df *out_set_of_voxel)
//*******************************************************************************************
{
	CKvSet2d_of_XvectorShort depth_data_x;

	cd_Conversion_Data(
		&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
		&depth_data_x);//CKvSet2d_of_XvectorShort *out_set_of_vector)

	fbp_Find_Boundary_Points(
		depth_data_x,//CKvSet2d_of_XvectorShort &in_set_of_runsets,
		zz_number_of_levels,//int in_length_of_ray_segment,
		false,//bool in_MODE_for_TESTing_input_runsets,
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
		NULL,//CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
		NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)

	ddc_Decode_DoCube(
		&zz_depth_data_w_bp,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		in_mode_int_true_or_float_false,
		&zz_surface_point3d);//CKvSet_of_Voxel *out_set_of_voxel)

	out_set_of_voxel->cp_Copy(&zz_surface_point3d);
}
//*******************************************************************************************
bool VCL_DoCube_X_Color::fbp_Find_Boundary_Points(
	CKvSet2d_of_XvectorShort &in_set_of_runsets,
	int in_length_of_ray_segment,
	bool in_MODE_for_TESTing_input_runsets,
	CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
	CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
	CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
//*******************************************************************************************
{
	CKvXvectorShort **xin, **xboundary;
	int x, y, ww, hh, dd, hh1, ww1;

	dd        = in_length_of_ray_segment;
	xin       = in_set_of_runsets.mps(ww, hh); 
	xboundary = out_BOUNDARY_set_of_runsets___NULL->c_Create(hh, ww);
	zz_step_size_ = 2.0 / double(ww);

	hh1 = hh - 1; ww1 = ww - 1;	

	zz_time = 0.0;
	zz_stop_watch.c_Create(1);
	zz_stop_watch.r_Reset(0);

#pragma omp parallel num_threads(8)
	{
#pragma omp for private(x)
		for (y = 0; y < hh; y++)
		{
			for (x = 0; x < ww; x++)
			{
				if ((y == 0) || (y == hh1) || (x == 0) || (x == ww1))
				{
					xboundary[y][x].cp_Copy(&xin[y][x], 2);//(2);
				}
				else
				{
					fbp_Find_Boundary_Points_Using_Neighborhood(
						&xin[y][x],//CKvXvectorShort &in_runset_on_a_CURRENT_ray_segment,
						&xin[y - 1][x],//CKvXvectorShort &in_runset_on_a_PREVIOUS_ray_segment,
						&xin[y + 1][x],//CKvXvectorShort &in_runset_on_a_NEXT_ray_segment,
						&xin[y][x - 1],//CKvXvectorShort &in_runset_on_a_UPPER_ray_segment,
						&xin[y][x + 1],//CKvXvectorShort &in_runset_on_a_LOWER_ray_segment,
						dd,//int in_length_of_ray_segment,
						in_MODE_for_TESTing_input_runsets,//bool in_MODE_for_TESTing_input_runsets,
						&xboundary[y][x],//CKvXvectorShort *out_BOUNDARY_RUNSET_of_the_CURRENT_ray_segment___or_NULL,
						NULL, //CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
						NULL);//CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)

					//if (!fbp_Find_Boundary_Points_Using_Neighborhood(
					//	xin[y][x],//CKvXvectorShort &in_runset_on_a_CURRENT_ray_segment,
					//	xin[y - 1][x],//CKvXvectorShort &in_runset_on_a_PREVIOUS_ray_segment,
					//	xin[y + 1][x],//CKvXvectorShort &in_runset_on_a_NEXT_ray_segment,
					//	xin[y][x - 1],//CKvXvectorShort &in_runset_on_a_UPPER_ray_segment,
					//	xin[y][x + 1],//CKvXvectorShort &in_runset_on_a_LOWER_ray_segment,
					//	dd,//int in_length_of_ray_segment,
					//	in_MODE_for_TESTing_input_runsets,//bool in_MODE_for_TESTing_input_runsets,
					//	&xboundary[y][x], NULL, NULL)//CKvXvectorShort *out_BOUNDARY_RUNSET_of_the_CURRENT_ray_segment___or_NULL, CKvXvectorInt *io_buffer_for_tempo_states___or_NULL, CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
					//	)
					//{
					//	return false;
					//}
				}
			}
		}
	}

	zz_time = zz_stop_watch.get_Get_Elapsed_Time(0);
	//printf("Time internal = %lf\n", zz_time);

	return true;
//error:
//	zsm("u_pri_Peel_a_set_of_Runsets_representing_a_binary_Image");
//	return false;

}
//********************************************************************************************
bool VCL_DoCube_X_Color::fbp_Find_Boundary_Points_Using_Neighborhood(
	CKvXvectorShort *in_runset_on_a_CURRENT_ray_segment,
	CKvXvectorShort *in_runset_on_a_PREVIOUS_ray_segment,
	CKvXvectorShort *in_runset_on_a_NEXT_ray_segment,
	CKvXvectorShort *in_runset_on_a_UPPER_ray_segment,
	CKvXvectorShort *in_runset_on_a_LOWER_ray_segment,
	int in_length_of_ray_segment,
	bool in_MODE_for_TESTing_input_runsets,
	CKvXvectorShort *out_BOUNDARY_RUNSET_of_the_CURRENT_ray_segment___or_NULL,
	CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
	CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL)
//*******************************************************************************************
{
	CKvXvectorShort xxvec_tempo, *PRUN;
	CKvXvectorInt xxvec_ndx, *PNDX;
	CKvRanker ranker;
	CKvXvectorShort *PC, *PP, *PN, *PU, *PL, PO, *PZ;

	short *C, *P, *N, *U, *L, *T, *Q; int *I;
	int TWO, THREE, nc, np, nn, nu, nl, no, nt, capacity, k, ndx, ww = in_length_of_ray_segment;
	short xo, x1; int state;

	in_runset_on_a_CURRENT_ray_segment->z_gii(NULL, &C, &nc, NULL);
	if (nc == 0)
	{
		out_BOUNDARY_RUNSET_of_the_CURRENT_ray_segment___or_NULL->cp_Copy(in_runset_on_a_CURRENT_ray_segment, 2);
		return true;
	}
	
	if ((PRUN = io_buffer_for_tempo_runsets___or_NULL) == NULL) PRUN = &xxvec_tempo;
	if ((PNDX = io_buffer_for_tempo_states___or_NULL) == NULL) PNDX = &xxvec_ndx;

	PC = in_runset_on_a_CURRENT_ray_segment;
	PP = in_runset_on_a_PREVIOUS_ray_segment;
	PN = in_runset_on_a_NEXT_ray_segment;
	PU = in_runset_on_a_UPPER_ray_segment;
	PL = in_runset_on_a_LOWER_ray_segment;

	PZ = out_BOUNDARY_RUNSET_of_the_CURRENT_ray_segment___or_NULL;

	//PO = &out_PEELed_RUNSET_the_CURRENT_ray_segment;

	if (in_MODE_for_TESTing_input_runsets)
	{
		if (!u_trr_Test_Runset_on_a_Ray_segment(PC[0], ww)){ return false; } //gerr("Bad CURRENT runset");
		if (!u_trr_Test_Runset_on_a_Ray_segment(PP[0], ww)){ return false; } //gerr("Bad PREVIOUS runset");
		if (!u_trr_Test_Runset_on_a_Ray_segment(PN[0], ww)){ return false; } //gerr("Bad NEXT runset");
		if (!u_trr_Test_Runset_on_a_Ray_segment(PU[0], ww)){ return false; } //gerr("Bad UPPER runset");
		if (!u_trr_Test_Runset_on_a_Ray_segment(PL[0], ww)){ return false; } //gerr("Bad LOWER runset");

	}

	
	PP->z_gii(NULL, &P, &np, NULL);
	PN->z_gii(NULL, &N, &nn, NULL);
	PU->z_gii(NULL, &U, &nu, NULL);
	PL->z_gii(NULL, &L, &nl, NULL);

	if ((nc*np*nn*nu*nl) == 0)
	{
		PZ->cp_Copy(PC, 2);
		return true;
	}

	nt = nc + np + nn + nu + nl;
	PRUN->z_gii(NULL, &T, NULL, &capacity);

	if (capacity<nt)
	{
		PRUN->in_Initialize(nt + 2);
		PNDX->in_Initialize(nt + 2);
	}
	else
	{
		PRUN->z_cne_Change_Number_of_Elements(0);
		PNDX->z_cne_Change_Number_of_Elements(0);
	}

	/******************************* Set Data**************************************/
	PRUN->z_gii(NULL, &T, &ndx, &k); if ((ndx != 0) || (k<nt)){ return false; } //gerr("[1]");
	PNDX->z_gii(NULL, &I, &ndx, &k); if ((ndx != 0) || (k<nt)){ return false; } //gerr("[2]");

	for (ndx = k = 0; k<nc;)
	{
		T[ndx] = xo = C[k] + 1; I[ndx] = 1; k++; ndx++;
		T[ndx] = x1 = C[k];     I[ndx] = -1; k++; ndx++;
		if (x1<xo) { ndx -= 2; continue; }
	}

	for (k = 0; k<np;)
	{
		T[ndx] = P[k];     I[ndx] = 1; k++; ndx++;
		T[ndx] = P[k] + 1; I[ndx] = -1; k++; ndx++;
	}

	for (k = 0; k<nn;)
	{
		T[ndx] = N[k];     I[ndx] = 1; k++; ndx++;
		T[ndx] = N[k] + 1; I[ndx] = -1; k++; ndx++;
	}

	for (k = 0; k<nu;)
	{
		T[ndx] = U[k];     I[ndx] = 1; k++; ndx++;
		T[ndx] = U[k] + 1; I[ndx] = -1; k++; ndx++;
	}

	for (k = 0; k<nl;)
	{
		T[ndx] = L[k];     I[ndx] = 1; k++; ndx++;
		T[ndx] = L[k] + 1; I[ndx] = -1; k++; ndx++;
	}


	PRUN->z_cne(ndx); PNDX->z_cne(ndx); nt = ndx;
	/******************************************************************************/
	//zz_stop_watch.r_Reset(0);
	ranker.s_Sort(T, false, 0, nt, I);

	TWO = 4; THREE = 5;

	state = 0; //first_run=true;
	ndx = 0; if ((nt % 2) != 0){ return false; }// gerr("[3]");

	for (k = 0; k<nt; k++)
	{
		if (I[k]>0)
		{
			state++;
			if (state == THREE){ xo = T[ndx] = T[k]; ndx++; }
		}
		else if (I[k]<0)
		{
			state--;
			if (state == TWO)
			{
				T[ndx] = T[k] - 1; ndx++;
				if (T[ndx - 2] > T[ndx - 1]) ndx -= 2;
			}
		}
		else{ return false; }  //gerr("[4]");
	}
	PRUN->z_cne(ndx); nt = ndx;
	PO.cp_Copy(PRUN, 2);

	PRUN->z_cne_Change_Number_of_Elements(0);
	PNDX->z_cne_Change_Number_of_Elements(0);

	PRUN->z_gii(NULL, &T, &ndx, NULL); if (ndx != 0){ return false; } //gerr("[5]");
	PNDX->z_gii(NULL, &I, &ndx, NULL); if (ndx != 0){ return false; } // gerr("[6]");

	PO.z_gii(NULL, &Q, &no, NULL); if (no != nt){ return false; } // gerr("[7]");

	ndx = 0;
	T[ndx] = 0; I[ndx] = 1; ndx++;
	for (k = 0; k<no;)
	{
		T[ndx] = Q[k]; I[ndx] = -1; ndx++; k++;
		T[ndx] = Q[k] + 1; I[ndx] = 1; ndx++; k++;
	}
	T[ndx] = ww; I[ndx] = -1; ndx++;

	for (k = 0; k<nc;)
	{
		T[ndx] = C[k]; I[ndx] = 1; k++; ndx++;
		T[ndx] = C[k] + 1; I[ndx] = -1; k++; ndx++;
	}

	PRUN->z_cne(ndx);
	PNDX->z_cne(ndx); nt = ndx; if ((nt % 2) != 0){ return false; } // gerr("[8]");

	ranker.s_Sort(T, false, 0, nt, I);

	TWO = 1; THREE = 2;

	state = 0; //first_run=true;
	ndx = 0; if ((nt % 2) != 0){ return false; } // gerr("[9]");

	xo = x1 = -100;
	for (k = 0; k<nt; k++)
	{
		if (I[k]>0)
		{
			state++;
			if (state == THREE) { xo = T[ndx] = T[k]; ndx++; }
		}
		else if (I[k]<0)
		{
			state--;
			if (state == TWO)
			{
				x1 = T[ndx] = T[k] - 1; ndx++;
				if (xo>x1) ndx -= 2;
			}
		}
		else{ return false; } // gerr("[10]");
	}
	PRUN->z_cne(ndx);
	PZ->cp_Copy(PRUN, 2);

	//zz_time += zz_stop_watch.get_Get_Elapsed_Time(0);

	return true;
}
//*******************************************************************************************
bool VCL_DoCube_X_Color::u_trr_Test_Runset_on_a_Ray_segment(
	CKvXvectorShort &in_runset,
	int in_length_of_ray_segment)
//*******************************************************************************************
{
	short *a; int ww, ne, xo, x1, xp, k;

	in_runset.z_gii_Get_Informations_Internal(NULL, &a, &ne, NULL);
	ww = in_length_of_ray_segment;

	if (ww <= 0) return false;
	if ((ne % 2) != 0) return false;

	for (xp = -2, k = 0; k<ne;)
	{
		xo = a[k]; k++; if ((xo - xp)<2) return false;
		xp = x1 = a[k]; k++; if ((x1<xo) || (x1 >= ww)) return false;
	}
	return true;
}
//********************************************************************************************
bool VCL_DoCube_X_Color::cd_Conversion_Data(
	CKvSet2d_of_VectorShort *in_set_of_vector,
	CKvSet2d_of_XvectorShort *out_set_of_vector)
//********************************************************************************************
{
	CKvVectorShort **p_set_vector;
	CKvXvectorShort **p_set_xvector;
	int ww, hh, j, i;

	p_set_vector  = in_set_of_vector->mps(ww, hh);
	p_set_xvector = out_set_of_vector->c_Create(hh, ww);

	for (j = 0 ; j < hh ; j++)
	{
		for (i = 0 ; i < ww ; i++)
		{
			if (p_set_vector[j][i].ge_Get_Element(0) == NOPOINTVALDoCube)
			{
				p_set_xvector[j][i].in_Initialize(2);
			}
			else
			{
				p_set_xvector[j][i].in_Initialize(p_set_vector[j][i].vs());
				p_set_xvector[j][i].ap_Append(
					&p_set_vector[j][i], 
					p_set_vector[j][i].vs());
			}
		}
	}

	return true;
}
//********************************************************************************************
void VCL_DoCube_X_Color::cd_Conversion_Data_Surface_Voxel_To_DoCube(
	CKvXrunset3dShort *in_surface_rle,
	CKvXrunset3dShort *out_docube_rle)
//********************************************************************************************
{
	//CKvSdkimChainCode chain_code;
	CKvSdkimRunCode runcode;
	CKvXrunsetShort slice, rle;
	CKvXvectorShort *p_run_points, **p_set_2d_run;
	CKvSet_of_XvectorShort set_of_run_points;
	CKvSet_of_XvectorShort set_of_run_points2;
	CKvSet2d_of_XvectorShort set_2d_of_run;
	LCKvUtility_for_Xrunset lu_xrunset;
	int hh, ww, dd, m, k;
	int num_obj, number_of_internal_boundaries;

	in_surface_rle->ts(ww, hh, dd);
	out_docube_rle->cn_Create_Null_pattern(dd, hh, ww);
	number_of_internal_boundaries = 0;
	p_set_2d_run = set_2d_of_run.c_Create(hh, ww);
	set_of_run_points2.c_Create(1);
	for (k = 0; k < hh; k++)
	{
		//in_sp->gs_xz_Get_Slice_XZ(k, slice);
		in_surface_rle->gs_zx_Get_Slice_ZX(k, slice);
		runcode.im_Import(slice, true);

		CKvSdkimChainCode::im_Import(runcode);
		num_obj = CKvSdkimChainCode::no_Number_of_Objects();

		if (num_obj != 0)
		{
			slice.cn_Create_Null_pattern(hh, ww);
			for (m = 0; m < num_obj; m++)
			{
				p_run_points = set_of_run_points.c_Create(1);
				CKvSdkimChainCode::gspb_Get_Set_of_Points_on_a_Boundary(
					true,//bool in_mode_for_external_boundary,
					m,//int in_boundary_index,
					&p_run_points[0],//CKvXvectorShort *out_CORNER_points___or_NULL,
					NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
					NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
					NULL,//CKvXvectorUchar *out_chain_codes___or_NULL,
					NULL,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
					NULL);//int *out_object_index_related_to_the_boundary___or_NULL);

				CKvSdkimChainCode::u_gxc_Get_an_Xrunset_from_Corner_points(
					//u_gxr_Get_an_Xrunset_from_Run_points(
					set_of_run_points,//CKvSet_of_XvectorShort &in_set_of_run_points_on_EXTERNAL_boundaries,
					set_of_run_points2,//CKvSet_of_XvectorShort &in_set_of_run_points_on_INTETRNAL_or_HOLE_boundaries,
					1,//int in_number_of_external_boundaries,
					number_of_internal_boundaries,//int in_number_of_internal_boundaries,
					ww,//int in_width_of_binary_image,
					dd,//int in_height_of_binary_image,
					rle);//CKvXrunsetShort &out_runset);

				lu_xrunset.uni_Union(rle, slice, slice);
			}

			slice.ex_Export(set_of_run_points, NULL);
			p_run_points = set_of_run_points.vp();
			for (m = 0; m < ww; m++)
			{
				p_set_2d_run[k][m].cp_Copy(&p_run_points[m]);
			}
		}
		else
		{
			for (m = 0; m < ww; m++)
			{
				p_set_2d_run[k][m].in_Initialize(2);
			}
		}
	}

	out_docube_rle->im_Import(set_2d_of_run, dd);
}
//********************************************************************************************
bool VCL_DoCube_X_Color::ddc_Decode_DoCube(
	CKvSet2d_of_XvectorShort *in_set_of_vector,
	bool in_mode_int_true_or_float_false,
	CKvDepot_of_Point3Df *out_set_of_voxel)
//********************************************************************************************
{
	CKvXvectorShort **xvec; CKvXvectorInt **xvec_idx; CKvPoint3D point3d;
	short *a, zo, z1;
	int ww, hh, j, i, k, z, np, dum, count=0;
	double slop;

	xvec     = in_set_of_vector->mps(ww, hh);
	xvec_idx = zz_set_point_to_rl_idx.c_Create(hh, ww);
	out_set_of_voxel->in_Initialize(ww*hh*10);
	if (!in_mode_int_true_or_float_false)
	{
		slop = (2.0 - (double)zz_step_size_) / (double)(ww-1.0);
	}

	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			xvec[j][i].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			xvec_idx[j][i].in_Initialize(1);
			for (k = 0; k < np;)
			{
				zo = a[k]; k++; 
				z1 = a[k]; k++;
				for (z = zo; z <= z1; z++)
				{
					if (in_mode_int_true_or_float_false)
					{
						point3d.x = (double)i;
						point3d.y = (double)j;
						point3d.z = (double)z;
						out_set_of_voxel->ap_Append(false, point3d, dum);
						xvec_idx[j][i].ae_Append_Element(count);
						count++;
					}
					else
					{
						point3d.x = slop*(double)i - 1.0 + (zz_step_size_/2.0);
						point3d.y = slop*(double)j - 1.0 + (zz_step_size_/2.0);
						point3d.z = slop*(double)z - 1.0 + (zz_step_size_/2.0);
						out_set_of_voxel->ap_Append(false, point3d, dum);
						xvec_idx[j][i].ae_Append_Element(count);
						count++;
					}
				}
			}
		}
	}

	out_set_of_voxel->cne_Change_Number_of_Elements(count);

	return true;
}
//********************************************************************************************
bool VCL_DoCube_X_Color::ddc_Decode_DoCube(
	CKvSet2d_of_XvectorShort *in_set_of_vector,
	CKvSet_of_Voxel *out_set_of_voxel)
//********************************************************************************************
{
	CKvDepot_of_Point3Df set_of_points;
	CKvPoint3D point3d; CKvVoxel *p_voxel;
	int nump, k;

	ddc_Decode_DoCube(
		in_set_of_vector,//CKvSet2d_of_XvectorShort *in_set_of_vector,
		true,//bool in_mode_int_true_or_float_false,
		&set_of_points);//CKvDepot_of_Point3Df *out_set_of_voxel)
	
	nump    = set_of_points.ne_Number_of_Elements();
	p_voxel = out_set_of_voxel->c_Create(nump);
	for (k = 0; k < nump; k++)
	{
		set_of_points.ge_Get_Element(k, point3d);
		p_voxel[k].x = (int)point3d.x;
		p_voxel[k].y = (int)point3d.y;
		p_voxel[k].z = (int)point3d.z;
	}
	
	return true;
}
//********************************************************************************************
bool VCL_DoCube_X_Color::ddc_Decode_DoCube_Indices(
	CKvSet2d_of_XvectorShort *in_set_of_vector)
//********************************************************************************************
{
	CKvXvectorShort **xvec; CKvXvectorInt **xvec_idx;
	short *a, zo, z1;
	int ww, hh, j, i, k, z, np, count = 0;

	xvec = in_set_of_vector->mps(ww, hh);
	xvec_idx = zz_set_point_to_rl_idx.c_Create(hh, ww);

	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			xvec[j][i].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			xvec_idx[j][i].in_Initialize(1);
			for (k = 0; k < np;)
			{
				zo = a[k]; k++;
				z1 = a[k]; k++;
				
				for (z = zo; z <= z1; z++)
				{
					xvec_idx[j][i].ae_Append_Element(count);
					count++; 
				}
			}
		}
	}

	return true;
}
//********************************************************************************************
void VCL_DoCube_X_Color::convert_data(
	CKvVectorBool *in_pattern,
	CKvXvectorShort *out_xvecShort)
//********************************************************************************************
{
	CKvXrunset1dShort xrunset1d;

	xrunset1d.im_Import(*in_pattern);
	xrunset1d.ex_Export(*out_xvecShort, NULL);
}
//********************************************************************************************
void VCL_DoCube_X_Color::gvtdt_Generate_Voxel_To_DoCube_Table(
	CKvMatrixInt *out_table_N_by_4)
//********************************************************************************************
{
	CKvXvectorShort **xvec;
	short *a, zo, z1;
	int ww, hh, j, i, k, z, np, num, count = 0, Run_count = 0;
	int **p_table;

	xvec     = zz_depth_data_w_bp.mps(ww, hh);
	num      = zz_surface_point3d.ne_Number_of_Elements();
	p_table  = out_table_N_by_4->c_Create(num, 4);

	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			xvec[j][i].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			Run_count = -1;
			for (k = 0; k < np;)
			{
				zo = a[k]; k++;
				z1 = a[k]; k++;
				Run_count++;
				for (z = zo; z <= z1; z++)
				{
					p_table[count][0] = i;
					p_table[count][1] = j;
					p_table[count][2] = Run_count;
					p_table[count][3] = z-zo;
					count++;
				}
			}
		}
	}
}
//********************************************************************************************
bool VCL_DoCube_X_Color::sc_Save_Color(
	CKvString in_filename)
//********************************************************************************************
{
	FILE *fp = NULL; LCKvIO_FileJpg io_file_jpeg;
	CKvXvectorUcharRgb **p_color;
	CKvMatrixUcharRgb img;
	CKvString a1, a2;
	int ww, hh, j, i, k, np;
	unsigned char **pr, **pg, **pb;
	unsigned char *xr, *xg, *xb;

	a1.fm_Format("%s.txt", (char *)in_filename);
	fopen_s(&fp, (char *)a1, "w");
	if (fp == NULL) return false;

	p_color = zz_color.mps(ww, hh);
	img.c_Create(hh, ww, Kv_Rgb(0,0,0));
	pr = img.mp(pg, pb);

	fprintf(fp, "%d %d %d\n", ww,hh,ww);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p_color[j][i].z_gii_Get_Informations_Internal(
				NULL, &xr, &xg, &xb, &np, NULL);
			if (np != 0)
			{
				pr[j][i] = xr[0]; pg[j][i] = xg[0]; pb[j][i] = xb[0];
				for (k = 1; k < np; k++)
				{
					fprintf(fp,"%d %d %d ",
						(int)((int)xr[k] - (int)xr[k - 1]),
						(int)((int)xg[k] - (int)xg[k - 1]), 
						(int)((int)xb[k] - (int)xb[k - 1]));
				}
				fprintf(fp, "\n");
			}
		}
	}
	fclose(fp);

	a2.fm_Format("%s.bmp", (char *)in_filename);
	io_file_jpeg.sbf_Save_as_BMP_File(a2, &img);


	return true;
}
//********************************************************************************************
bool VCL_DoCube_X_Color::sc_Save_Color2(
	CKvString in_filename)
//********************************************************************************************
{
	FILE *fp = NULL; LCKvIO_FileJpg io_file_jpeg;
	CKvXvectorUcharRgb **p_color;
	CKvMatrixUcharRgb img, residual;
	CKvString a1, a2;
	int ww, hh, j, i, k, np, max_np;
	unsigned char **pr, **pg, **pb;
	unsigned char **prr, **pgr, **pbr;
	unsigned char *xr, *xg, *xb;

	a1.fm_Format("%s.bmp", (char *)in_filename);
	a2.fm_Format("%s_residual.bmp", (char *)in_filename);

	p_color = zz_color.mps(ww, hh);
	img.c_Create(hh, ww, Kv_Rgb(0, 0, 0));
	pr = img.mp(pg, pb);

	max_np = -1;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p_color[j][i].z_gii_Get_Informations_Internal(
				NULL, &xr, &xg, &xb, &np, NULL);
			if (max_np < np){ max_np = np; }
		}
	}
	printf("Maximum RL Length : %d\n", max_np);
	residual.c_Create(hh*ww, max_np-1,Kv_Rgb(0,0,0));
	prr = residual.mp(pgr, pbr);
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p_color[j][i].z_gii_Get_Informations_Internal(
				NULL, &xr, &xg, &xb, &np, NULL);
			if (np != 0)
			{
				pr[j][i] = xr[0]; pg[j][i] = xg[0]; pb[j][i] = xb[0];
				for (k = 1; k < np; k++)
				{
					prr[j*ww + i][k - 1] = abs((int)((int)xr[k] - (int)xr[k - 1]));
					pgr[j*ww + i][k - 1] = abs((int)((int)xg[k] - (int)xg[k - 1]));
					pbr[j*ww + i][k - 1] = abs((int)((int)xb[k] - (int)xb[k - 1]));
				}
			}
		}
	}
	
	io_file_jpeg.sbf_Save_as_BMP_File(a1, &img);
	io_file_jpeg.sbf_Save_as_BMP_File(a2, &residual);
	
	return true;
}
//********************************************************************************************
bool VCL_DoCube_X_Color::sc_Save_Color3(
	CKvString in_filename)
//********************************************************************************************
{
	FILE *fp = NULL; LCKvIO_FileJpg io_file_jpeg;
	CKvXvectorUcharRgb **p_color;
	CKvMatrixUcharRgb img, residual;
	CKvXRmatrixUcharRgb residual_X; CKvVectorUcharRgb element;
	CKvString a1, a2; CKvRgb rgb;
	int ww, hh, j, i, k, np, max_np;
	unsigned char **pr, **pg, **pb;
	unsigned char **prr, **pgr, **pbr;
	unsigned char *xr, *xg, *xb;

	a1.fm_Format("%s.bmp", (char *)in_filename);
	a2.fm_Format("%s_residual.bmp", (char *)in_filename);

	p_color = zz_color.mps(ww, hh);
	img.c_Create(hh, ww, Kv_Rgb(0, 0, 0));
	pr = img.mp(pg, pb);

	max_np = -1;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p_color[j][i].z_gii_Get_Informations_Internal(
				NULL, &xr, &xg, &xb, &np, NULL);
			if (max_np < np){ max_np = np; }
		}
	}
	printf("Maximum RL Length : %d\n", max_np);
	//residual.c_Create(hh*ww, max_np - 1, Kv_Rgb(0, 0, 0));

	residual_X.in_Initialize(hh*ww, max_np - 1);
	element.c_Create(max_np - 1);
	//residual_X.ap_Append()

	prr = residual.mp(pgr, pbr);
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p_color[j][i].z_gii_Get_Informations_Internal(
				NULL, &xr, &xg, &xb, &np, NULL);
			if (np != 0)
			{
				element.sv_Set_Vector(Kv_Rgb(0, 0, 0));

				pr[j][i] = xr[0]; pg[j][i] = xg[0]; pb[j][i] = xb[0];
				for (k = 1; k < np; k++)
				{
					rgb.r = abs((int)((int)xr[k] - (int)xr[k - 1]));
					rgb.g = abs((int)((int)xg[k] - (int)xg[k - 1]));
					rgb.b = abs((int)((int)xb[k] - (int)xb[k - 1]));

					element.se_Set_Element(k - 1, rgb);
				}

				residual_X.ap_Append(&element);

			}
		}
	}

	residual_X.ex_Export(&residual,NULL);

	io_file_jpeg.sbf_Save_as_BMP_File(a1, &img);
	io_file_jpeg.sbf_Save_as_BMP_File(a2, &residual);

	return true;
}
//********************************************************************************************
bool VCL_DoCube_X_Color::spi_Save_Palette_Indices(
	CKvString in_filename)
//********************************************************************************************
{
	FILE *fp = NULL;
	CKvMatrixInt **p;
	int **pp;
	int ww, hh, num_element, dim, j, i, k;

	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	p = zz_palette_index.mps(ww, hh);
	for (i = 0; i < ww; i++)
	{
		for (j = 0; j < hh; j++)
		{
			pp = p[j][i].mps(dim, num_element);
			if (dim!=1)
			{
				fprintf(fp, "%d %d\n", j, i);
				for (k = 0; k < num_element; k++)
				{
					fprintf(fp, "%d %d %d\n", pp[k][0], pp[k][1], pp[k][2]);
				}
				//fprintf(fp, "\n");
			}
		}
	}

	fclose(fp);
	return true;
}
//********************************************************************************************
bool VCL_DoCube_X_Color::sp_Save_Points(
	CKvString in_filename,
	CKvDepot_of_Point3Df *in_point3d)
//********************************************************************************************
{
	FILE *fp = NULL;
	CKvPoint3D pt;
	int num_element, k;

	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	num_element = in_point3d->ne_Number_of_Elements();
	for (k = 0; k < num_element; k++)
	{
		in_point3d->ge_Get_Element(k, pt);
		fprintf(fp, "%lf %lf %lf\n", pt.x, pt.y, pt.z);
	}

	fclose(fp);
	return true;
}
//********************************************************************************************
int VCL_DoCube_X_Color::gvi_Get_Voxel_Index(
	int in_x, 
	int in_y, 
	int in_z)
//********************************************************************************************
{
	CKvXvectorInt **p_xv;
	CKvXvectorShort **p_bp;
	short zo, z1, *pointer;
	int ww, hh, z, k;
	int *pointer_idx;

	p_xv = zz_set_point_to_rl_idx.mps(ww, hh);
	if (((ww <= in_x) && (0 > in_x)) || ((hh <= in_y) && (0 > in_y))){ return -1;}
	
	p_bp = zz_depth_data_w_bp.mp();

	p_bp[in_y][in_x].z_gii_Get_Informations_Internal(NULL, &pointer, &ww, NULL);
	p_xv[in_y][in_x].z_gii_Get_Informations_Internal(NULL, &pointer_idx, NULL, NULL);
	hh = 0;

	for (k = 0; k < ww; )
	{
		zo = pointer[k]; k++;
		z1 = pointer[k]; k++;

		if ((zo <= in_z) && (in_z <= z1))
		{
			for (z = zo; z <= z1; z++)
			{
				if (in_z == z)
				{
					return (int)pointer_idx[hh];
				}
				hh++;
			}
		}
		else
		{
			hh += (z1 - zo + 1);
		}
	}

	
	return -1;
}
//********************************************************************************************
long long VCL_DoCube_X_Color::gvi_Get_Voxel_Index2(
	int in_x,
	int in_y,
	int in_z,
	int in_search_start)
//********************************************************************************************
{
	int num_p, dim, k;
	float **p_m = zz_surface_point3d.gps_Get_Pointer_of_Space()->mps(dim, num_p);

	for (k = in_search_start; k < num_p; k++)
	{
		if (p_m[k][0] == in_x &&p_m[k][1] == in_y &&p_m[k][2] == in_z)
		{
			return k;
		}
	}

	for (k = in_search_start; k >= 0 ; k--)
	{
		if (p_m[k][0] == in_x &&p_m[k][1] == in_y &&p_m[k][2] == in_z)
		{
			return k;
		}
	}

	return -1;
}
//********************************************************************************************
CKvSet2d_of_VectorInt VCL_DoCube_X_Color::grlv_Get_RL_to_Voxel_Index()
//********************************************************************************************
{
	CKvSet2d_of_VectorInt set_idx;
	CKvXvectorInt **p_xv; CKvVectorInt **p_v;
	int ww, hh, j, i, k, sz;

	p_xv = zz_set_point_to_rl_idx.mps(ww, hh);
	p_v  = set_idx.c_Create(hh, ww);

	for (j = 0 ; j < hh ; j++)
	{
		for (i = 0 ; i < ww ; i++)
		{
			sz = p_xv[j][i].ne_Number_of_Elements();
			if (sz == 0)
			{
				p_v[j][i].c_Create(1, NOPOINTVALDoCube);
			}
			else
			{
				p_v[j][i].c_Create(sz);
				for (k = 0; k < sz; k++)
				{
					p_v[j][i].se_Set_Element(k, p_xv[j][i].ge_Get_Element(k));
				}
			}

		}
	}

	return set_idx;
}
//********************************************************************************************
CKvSet2d_of_VectorShort VCL_DoCube_X_Color::grl_Get_RL_vector()
//********************************************************************************************
{
	CKvSet2d_of_VectorShort data;
	CKvXvectorShort **p_x; CKvVectorShort **p_v;
	int ww, hh, sz, j, i, k;

	p_x = zz_depth_data_w_bp.mps(ww, hh);
	p_v = data.c_Create(hh, ww);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			sz = p_x[j][i].ne_Number_of_Elements();
			if (sz == 0)
			{
				p_v[j][i].c_Create(1, NOPOINTVALDoCube);
			}
			else
			{
				p_v[j][i].c_Create(sz);
				for (k = 0; k < sz; k++)
				{
					p_v[j][i].se_Set_Element(k, p_x[j][i].ge_Get_Element(k));
				}
			}
		}
	}

	return data;
}
//********************************************************************************************
//void VCL_DoCube_X_Color::EI_DCT_Forward(
//	CKvMatrixUcharRgb *io_img,
//	CKvMatrixBool *in_mask)
////********************************************************************************************
//{
//	LCKvAlgebra_for_Matrix am;
//	CKvMatrixUcharRgb cpy_triangle_img; CKvMatrixBool cpy_mask; CKvRgb rgb; CKvMatrix C; CKvVector f1, f2; CKvXvector x_f1;
//	unsigned char ***ptr_img, ***ptr_img2, **ptr_r, **ptr_g, **ptr_b, **ptr_r2, **ptr_g2, **ptr_b2;
//	double tmp1, tmp2, s1, s2, a, b, cos_val; double **ptr_C, *ptr_f2;
//	int ww, hh, j, i, l, m, k, c, M, N; bool **p_mask, **p_cpy_mask;
//	
//
//	cpy_triangle_img.cp_Copy(io_img); cpy_mask.cp_Copy(in_mask);
//	ptr_r = cpy_triangle_img.mp(ptr_g, ptr_b);
//	p_cpy_mask = cpy_mask.mp();
//	cpy_triangle_img.ms(ww, hh);
//
//	ptr_r2 = io_img->mp(ptr_g2, ptr_b2);
//	ptr_img = new unsigned char **[3]; ptr_img2 = new unsigned char **[3];
//	p_mask = in_mask->mp();
//	
//	c = 0;
//	for (j = 0; j < hh; j++)
//	{
//		for (i = 0; i < ww; i++)
//		{
//			if (p_mask[j][i])
//			{
//				c++;
//			}
//		}
//	}
//	if (hh*ww == c){ return; }
//
//
//
//	// row
//	N = ww;
//	ptr_img[0]  = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
//	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;
//
//	for (c = 0; c < 3; c++)
//	{
//		for (j = 0; j < hh; j++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (i = 0; i < ww; i++)
//			{
//				if (p_mask[j][i] == true)
//				{
//					x_f1.ap_Append((double)ptr_img[c][j][i]);
//					M++;
//				}
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(N, M);
//
//				for (l = 0; l < N; l++)
//				{
//					for (m = 0; m < M; m++)
//					{
//
//						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)N);
//						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)M);
//
//						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp1 - tmp2));
//
//						if ((a*b) == 0.0)
//						{
//							cos_val = 0.0;
//							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//							ptr_C[l][m] = sqrt((double)((double)N / (double)M))*sqrt(1.0 / (double)((double)N*(double)M)) +
//								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//						}
//						else
//						{
//							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//							s2 = sin((PI / 4)*(tmp1 - tmp2)*((2 * M) - 1)) / b;
//							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//							ptr_C[l][m] += (double)((double)(1.0 / (double)M) - (double)(1.0 / sqrt((double)N*M)));
//						}
//					}
//				}
//
//				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//				ptr_f2 = f2.vp();
//
//				for (i = 0; i < ww; i++)
//				{
//					ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[i]);
//					p_cpy_mask[j][i] = true;
//				}
//			}
//		}
//	}
//
//	// col
//	N = hh;
//	ptr_img[0]  = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
//	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;
//
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 0; i < ww; i++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (j = 0; j < hh; j++)
//			{
//				if (p_cpy_mask[j][i] == true)
//				{
//					x_f1.ap_Append((double)ptr_img2[c][j][i]);
//					M++;
//				}
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(N, M);
//
//				for (l = 0; l < N; l++)
//				{
//					for (m = 0; m < M; m++)
//					{
//
//						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)N);
//						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)M);
//
//						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp1 - tmp2));
//
//						if ((a*b) == 0.0)
//						{
//							cos_val = 0.0;
//							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//							ptr_C[l][m] = sqrt((double)((double)N / (double)M))*sqrt(1.0 / (double)((double)N*(double)M)) +
//								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//						}
//						else
//						{
//							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//							s2 = sin((PI / 4)*(tmp1 - tmp2)*((2 * M) - 1)) / b;
//							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//							ptr_C[l][m] += (double)((double)(1.0 / (double)M) - (double)(1.0 / sqrt((double)N*M)));
//						}
//					}
//				}
//
//				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//				ptr_f2 = f2.vp();
//
//				for (j = 0; j < hh; j++)
//				{
//					ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[j]);
//				}
//			}
//		}
//	}
//
//	delete[] ptr_img2;
//	delete[] ptr_img;
//}
////********************************************************************************************
//void VCL_DoCube_X_Color::EI_DCT_Backward(
//	CKvMatrixUcharRgb *io_img,
//	CKvMatrixBool *in_mask)
////********************************************************************************************
//{
//	LCKvAlgebra_for_Matrix am;
//	CKvMatrixUcharRgb cpy_triangle_img; CKvMatrixBool cpy_mask; CKvRgb rgb; CKvMatrix C; CKvVector f1, f2; CKvXvector x_f1;
//	CKvVectorBool row_bool;
//	unsigned char ***ptr_img, ***ptr_img2, **ptr_r, **ptr_g, **ptr_b, **ptr_r2, **ptr_g2, **ptr_b2;
//	double tmp1, tmp2, s1, s2, a, b, cos_val; double **ptr_C, *ptr_f2;
//	int ww, hh, j, i, l, m, k, c, M, N, count; bool **p_mask, **p_cpy_mask;
//
//	rgb.r = 0; rgb.g = 0; rgb.b = 0;
//
//	cpy_triangle_img.cp_Copy(io_img); cpy_mask.cp_Copy(in_mask);
//	ptr_r = cpy_triangle_img.mp(ptr_g, ptr_b);
//	p_cpy_mask = cpy_mask.mp();
//	cpy_triangle_img.ms(ww, hh);
//	
//	c = 0;
//	for (j = 0; j < hh; j++)
//	{
//		for (i = 0; i < ww; i++)
//		{
//			if (p_mask[j][i])
//			{
//				c++;
//			}
//		}
//	}
//	if (hh*ww == c){ return; }
//		
//	io_img->c_Create(hh, ww, rgb);
//	ptr_r2 = io_img->mp(ptr_g2, ptr_b2);
//
//	ptr_img = new unsigned char **[3]; ptr_img2 = new unsigned char **[3];
//	p_mask = in_mask->mp();
//
//	for (j = 0; j < hh; j++)
//	{
//		cpy_mask.gr_Get_Row(0, j, ww, &row_bool);
//		
//		for (i = 0; i < ww; i++)
//		{
//			if (row_bool.ge_Get_Element(i) == true)
//			{
//				cpy_mask.sr_Set_Row(0, j, ww, true);
//				break;
//			}
//		}
//	}
//
//	// col
//	N = hh;
//
//	ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
//	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;
//
//
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 0; i< ww; i++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (j = 0; j < hh; j++)
//			{
//				x_f1.ap_Append((double)ptr_img[c][j][i]);
//				if (p_cpy_mask[j][i] == true){ M++; }
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(M, N);
//
//				for (l = 0; l < M; l++)
//				{
//					for (m = 0; m < N; m++)
//					{
//						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)M);
//						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)N);
//
//						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp2 - tmp1));
//
//						if ((a*b) == 0.0)
//						{
//							cos_val = 0.0;
//							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//							ptr_C[l][m] = sqrt((double)((double)M / (double)N))*sqrt(1.0 / (double)((double)N*(double)M)) +
//								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//						}
//						else
//						{
//							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//							s2 = sin((PI / 4)*(tmp2 - tmp1)*((2 * M) - 1)) / b;
//							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//							ptr_C[l][m] += (double)((double)(1.0 / (double)N) - (double)(1.0 / sqrt((double)N*M)));
//						}
//					}
//				}
//
//				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//				ptr_f2 = f2.vp();
//
//				count = 0;
//				for (j = 0; j < hh; j++)
//				{
//					if (p_cpy_mask[j][i] == true)
//					{
//						ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[count]);
//						count++;
//					}
//				}
//			}
//		}
//	}
//
//	//row
//	N = ww;
//
//	for (c = 0; c < 3; c++)
//	{
//		for (j = 0; j < hh; j++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (i = 0; i < ww; i++)
//			{
//				x_f1.ap_Append((double)ptr_img2[c][j][i]);
//				if (p_mask[j][i] == true){ M++; }
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(M, N);
//
//				for (l = 0; l < M; l++)
//				{
//					for (m = 0; m < N; m++)
//					{
//						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)M);
//						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)N);
//
//						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp2 - tmp1));
//
//						if ((a*b) == 0.0)
//						{
//							cos_val = 0.0;
//							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//							ptr_C[l][m] = sqrt((double)((double)M / (double)N))*sqrt(1.0 / (double)((double)N*(double)M)) +
//								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//						}
//						else
//						{
//							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//							s2 = sin((PI / 4)*(tmp2 - tmp1)*((2 * M) - 1)) / b;
//							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//							ptr_C[l][m] += (double)((double)(1.0 / (double)N) - (double)(1.0 / sqrt((double)N*M)));
//						}
//					}
//				}
//
//				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//				ptr_f2 = f2.vp();
//
//				count = 0;
//				for (i = 0; i < ww; i++)
//				{
//					if (p_mask[j][i] == true)
//					{
//						ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[count]);
//						count++;
//					}
//					else
//					{
//						ptr_img2[c][j][i] = 0;
//					}
//				}
//			}
//		}
//	}
//
//	delete[] ptr_img2;
//	delete[] ptr_img;
//}
////*******************************************************************************************
//void VCL_DoCube_X_Color::Block_EI_DCT_Forward(
//	CKvMatrixUcharRgb *io_img,
//	CKvMatrixBool *in_mask)
////********************************************************************************************
//{
//	CKvMatrixUcharRgb color_images, block_img;
//	CKvMatrixBool tmp_mask, block_mask;
//	double lww, lhh;
//	int ww, hh, j, i; int dx, dy;
//
//	dx = dy = 8;
//
//	io_img->ms(ww, hh);
//	color_images.cp_Copy(io_img);
//
//	lww = (double)ww / 8.0; lhh = (double)hh / 8.0;
//	lww = ceil(lww); lhh = ceil(lhh);
//	ww = lww * 8; hh = lhh * 8;
//
//	io_img->c_Create(hh, ww, Kv_Rgb(0, 0, 0));
//	io_img->sb_Set_Block(0, 0, &color_images);
//
//	tmp_mask.c_Create(hh, ww, false);
//	tmp_mask.sb_Set_Block(0, 0, in_mask);
//
//	for (j = 0; j < hh; j += dy)
//	{
//		for (i = 0; i < ww; i += dx)
//		{
//			io_img->gb_Get_Block(i, j, dx, dy, &block_img);
//			tmp_mask.gb_Get_Block(i, j, dx, dy, &block_mask);
//			EI_DCT_Forward(&block_img, &block_mask);
//			io_img->sb_Set_Block(i, j, &block_img);
//		}
//	}
//}
////********************************************************************************************
//void VCL_DoCube_X_Color::Block_EI_DCT_Backward(
//	CKvMatrixUcharRgb *io_img,
//	CKvMatrixBool *in_mask)
////********************************************************************************************
//{
//
//}
////********************************************************************************************
//void VCL_DoCube_X_Color::EI_DCT_Forward_2(
//	CKvMatrixUcharRgb *io_img,
//	CKvMatrixBool *in_mask)
////********************************************************************************************
//{
//	LCKvTransform lt;
//	LCKvAlgebra_for_Matrix am;
//	CKvMatrixUcharRgb cpy_triangle_img; CKvMatrixBool cpy_mask; CKvRgb rgb; CKvMatrix C; CKvVector f1, f2; CKvXvector x_f1;
//	unsigned char ***ptr_img, ***ptr_img2, **ptr_r, **ptr_g, **ptr_b, **ptr_r2, **ptr_g2, **ptr_b2;
//	double tmp1, tmp2, s1, s2; double **ptr_C, *ptr_f2;
//	int ww, hh, j, i, l, m, k, c, M, N; bool **p_mask, **p_cpy_mask;
//
//	cpy_triangle_img.cp_Copy(io_img); cpy_mask.cp_Copy(in_mask);
//	ptr_r = cpy_triangle_img.mp(ptr_g, ptr_b);
//	p_cpy_mask = cpy_mask.mp();
//	cpy_triangle_img.ms(ww, hh);
//
//	ptr_r2 = io_img->mp(ptr_g2, ptr_b2);
//	ptr_img = new unsigned char **[3]; ptr_img2 = new unsigned char **[3];
//	p_mask = in_mask->mp();
//
//	// row
//	N = ww;
//	ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
//	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;
//
//	for (c = 0; c < 3; c++)
//	{
//		for (j = 0; j < hh; j++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (i = 0; i < ww; i++)
//			{
//				if (p_mask[j][i] == true)
//				{
//					x_f1.ap_Append((double)ptr_img2[c][j][i]);
//					M++;
//				}
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL);
//
//				if (M % 2 == 0)
//				{
//					f2.c_Create(ww, 0.0);
//					lt.dctff_DCT_Forward_Fast(&f1);
//					f2.s_Set(0, &f1);
//					lt.dctif_DCT_Inverse_Fast(&f2);
//				}
//				else
//				{
//					lt.dctf_DCT_Forward(&f1); 				
//					
//					f2.c_Create(ww, 0.0); 
//					f2.s_Set(0, &f1);
//					
//					lt.dcti_DCT_Inverse(&f2);
//				}
//
//				//for (l = 0; l < N; l++)
//				//{
//				//	for (m = 0; m < M; m++)
//				//	{
//				//		tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)N);
//				//		tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)M);
//				//		a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp1 - tmp2));
//				//		if ((a*b) == 0.0)
//				//		{
//				//			cos_val = 0.0;
//				//			for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//				//			ptr_C[l][m] = sqrt((double)((double)N / (double)M))*sqrt(1.0 / (double)((double)N*(double)M)) +
//				//				(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//				//		}
//				//		else
//				//		{
//				//			s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//				//			s2 = sin((PI / 4)*(tmp1 - tmp2)*((2 * M) - 1)) / b;
//				//			ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//				//			ptr_C[l][m] += (double)((double)(1.0 / (double)M) - (double)(1.0 / sqrt((double)N*M)));
//				//		}
//				//	}
//				//}
//				//am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//				ptr_f2 = f2.vp();
//
//				for (i = 0; i < ww; i++)
//				{
//					ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[i]);
//					p_cpy_mask[j][i] = true;
//				}
//			}
//		}
//	}
//
//	CKvScreen sc;
//	sc.s_d_Display(io_img); Kv_Printf("Encode1");
//	sc.s_d_Display(&cpy_mask); Kv_Printf("Encode1");
//
//	// col
//	N = hh;
//	ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
//	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;
//
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 0; i < ww; i++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (j = 0; j < hh; j++)
//			{
//				if (p_cpy_mask[j][i] == true)
//				{
//					x_f1.ap_Append((double)ptr_img2[c][j][i]);
//					M++;
//				}
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL);
//
//				if (M % 2 == 0)
//				{					
//					lt.dctff_DCT_Forward_Fast(&f1);
//					f2.c_Create(hh, 0.0); f2.s_Set(0, &f1);
//					lt.dctif_DCT_Inverse_Fast(&f2);
//				}
//				else
//				{
//					lt.dctf_DCT_Forward(&f1);
//					f2.c_Create(hh, 0.0); f2.s_Set(0, &f1);
//					lt.dcti_DCT_Inverse(&f2);
//				}
//
//				//for (l = 0; l < N; l++)
//				//{
//				//	for (m = 0; m < M; m++)
//				//	{
//				//		tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)N);
//				//		tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)M);
//				//		a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp1 - tmp2));
//				//		if ((a*b) == 0.0)
//				//		{
//				//			cos_val = 0.0;
//				//			for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//				//			ptr_C[l][m] = sqrt((double)((double)N / (double)M))*sqrt(1.0 / (double)((double)N*(double)M)) +
//				//				(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//				//		}
//				//		else
//				//		{
//				//			s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//				//			s2 = sin((PI / 4)*(tmp1 - tmp2)*((2 * M) - 1)) / b;
//				//			ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//				//			ptr_C[l][m] += (double)((double)(1.0 / (double)M) - (double)(1.0 / sqrt((double)N*M)));
//				//		}
//				//	}
//				//}
//				//am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//
//				ptr_f2 = f2.vp();
//
//				for (j = 0; j < hh; j++)
//				{
//					ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[j]);
//				}
//			}
//		}
//	}
//
//	sc.s_d_Display(io_img); Kv_Printf("Encode2");
//
//
//
//	delete[] ptr_img2;
//	delete[] ptr_img;
//}
////********************************************************************************************
//void VCL_DoCube_X_Color::EI_DCT_Backward_2(
//	CKvMatrixUcharRgb *io_img,
//	CKvMatrixBool *in_mask)
////********************************************************************************************
//{
//	LCKvAlgebra_for_Matrix am;
//	CKvMatrixUcharRgb cpy_triangle_img; CKvMatrixBool cpy_mask; CKvRgb rgb; CKvMatrix C; CKvVector f1, f2; CKvXvector x_f1;
//	CKvVectorBool row_bool;
//	unsigned char ***ptr_img, ***ptr_img2, **ptr_r, **ptr_g, **ptr_b, **ptr_r2, **ptr_g2, **ptr_b2;
//	double tmp1, tmp2, s1, s2, a, b, cos_val; double **ptr_C, *ptr_f2;
//	int ww, hh, j, i, l, m, k, c, M, N, count; bool **p_mask, **p_cpy_mask;
//
//	rgb.r = 0; rgb.g = 0; rgb.b = 0;
//
//	cpy_triangle_img.cp_Copy(io_img); cpy_mask.cp_Copy(in_mask);
//	ptr_r = cpy_triangle_img.mp(ptr_g, ptr_b);
//	p_cpy_mask = cpy_mask.mp();
//	cpy_triangle_img.ms(ww, hh);
//	io_img->c_Create(hh, ww, rgb);
//	ptr_r2 = io_img->mp(ptr_g2, ptr_b2);
//
//	ptr_img = new unsigned char **[3]; ptr_img2 = new unsigned char **[3];
//	p_mask = in_mask->mp();
//
//	for (j = 0; j < hh; j++)
//	{
//		cpy_mask.gr_Get_Row(0, j, ww, &row_bool);
//
//		for (i = 0; i < ww; i++)
//		{
//			if (row_bool.ge_Get_Element(i) == true)
//			{
//				cpy_mask.sr_Set_Row(0, j, ww, true);
//				break;
//			}
//		}
//	}
//
//	// col
//	N = hh;
//
//	ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
//	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;
//
//
//	for (c = 0; c < 3; c++)
//	{
//		for (i = 0; i< ww; i++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (j = 0; j < hh; j++)
//			{
//				x_f1.ap_Append((double)ptr_img[c][j][i]);
//				if (p_cpy_mask[j][i] == true){ M++; }
//				//if (ipit_Is_Point_In_Triangle(in_position, i, j)){ M++; }
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(M, N);
//
//				for (l = 0; l < M; l++)
//				{
//					for (m = 0; m < N; m++)
//					{
//						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)M);
//						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)N);
//
//						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp2 - tmp1));
//
//						if ((a*b) == 0.0)
//						{
//							cos_val = 0.0;
//							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//							ptr_C[l][m] = sqrt((double)((double)M / (double)N))*sqrt(1.0 / (double)((double)N*(double)M)) +
//								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//						}
//						else
//						{
//							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//							s2 = sin((PI / 4)*(tmp2 - tmp1)*((2 * M) - 1)) / b;
//							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//							ptr_C[l][m] += (double)((double)(1.0 / (double)N) - (double)(1.0 / sqrt((double)N*M)));
//						}
//					}
//				}
//
//				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//				ptr_f2 = f2.vp();
//
//				count = 0;
//				for (j = 0; j < hh; j++)
//				{
//					//if (ipit_Is_Point_In_Triangle(in_position, i, j))
//					if (p_cpy_mask[j][i] == true)
//					{
//						ptr_img2[c][j][i] = (unsigned char)limit((unsigned char)ptr_f2[count]);
//						count++;
//					}
//				}
//			}
//		}
//	}
//
//	//sc.s_d_Display(io_img); Kv_Printf("Decode1");
//
//	//row
//	N = ww;
//
//	//ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
//	//ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;
//
//
//	for (c = 0; c < 3; c++)
//	{
//		for (j = 0; j < hh; j++)
//		{
//			x_f1.in_Initialize(1);
//			M = 0;
//			for (i = 0; i < ww; i++)
//			{
//				x_f1.ap_Append((double)ptr_img2[c][j][i]);
//				if (p_mask[j][i] == true){ M++; }
//				//if (ipit_Is_Point_In_Triangle(in_position, i, j)){ M++; }
//			}
//
//			if (M != 0)
//			{
//				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(M, N);
//
//				for (l = 0; l < M; l++)
//				{
//					for (m = 0; m < N; m++)
//					{
//						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)M);
//						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)N);
//
//						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp2 - tmp1));
//
//						if ((a*b) == 0.0)
//						{
//							cos_val = 0.0;
//							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
//							ptr_C[l][m] = sqrt((double)((double)M / (double)N))*sqrt(1.0 / (double)((double)N*(double)M)) +
//								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
//						}
//						else
//						{
//							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
//							s2 = sin((PI / 4)*(tmp2 - tmp1)*((2 * M) - 1)) / b;
//							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
//							ptr_C[l][m] += (double)((double)(1.0 / (double)N) - (double)(1.0 / sqrt((double)N*M)));
//						}
//					}
//				}
//
//				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
//				ptr_f2 = f2.vp();
//
//				count = 0;
//				for (i = 0; i < ww; i++)
//				{
//					//if (ipit_Is_Point_In_Triangle(in_position, i, j))
//					if (p_mask[j][i] == true)
//					{
//						ptr_img2[c][j][i] = (unsigned char)limit((unsigned char)ptr_f2[count]);
//						count++;
//					}
//					else
//					{
//						ptr_img2[c][j][i] = 0;
//					}
//				}
//			}
//		}
//	}
//
//	delete[] ptr_img2;
//	delete[] ptr_img;
//}
////*******************************************************************************************
