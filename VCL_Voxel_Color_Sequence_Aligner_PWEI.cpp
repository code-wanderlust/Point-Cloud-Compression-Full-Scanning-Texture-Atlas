#include "stdafx.h"
#include "VCL_Voxel_Color_Sequence_Aligner_PWEI.h"

//*************************************************************************************
VCL_Voxel_Color_Sequence_Aligner_PWEI::VCL_Voxel_Color_Sequence_Aligner_PWEI()
//*************************************************************************************
{

}
//*************************************************************************************
VCL_Voxel_Color_Sequence_Aligner_PWEI::~VCL_Voxel_Color_Sequence_Aligner_PWEI()
//*************************************************************************************
{

}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Align_Texture_on_Codes_Using_PWEI(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_offset)
//*************************************************************************************
{
	std::vector<std::vector<std::vector<int>>> ei_warping_index;
	std::vector<std::vector<int>> tmp_ToC;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec;
	std::vector<std::vector<int>> set_of_left_upper_xy;
	std::vector<std::vector<int>> set_of_right_lower_xy;
	int num_blob, num_toc, max_length, max_idx, current_size, m, n, k, l;
	int *p_v;
	int sz, max_idx_bw;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0)
		{
			exit(0);
			return;
		}
		p_v = p_vec[m].c_Create(num_toc, 0);
		if (num_toc == 1) continue;

		ei_warping_index = std::vector<std::vector<std::vector<int>>>();
		for (n = 0; n < num_toc-1; n++)
		{
			printf("m=%d/%d) n=%d/%d)\n", m, num_blob, n, num_toc);
			Find_EI_Ranges(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_texture_on_code_N1,
				io_segmented_texture_on_code[m][n+1],//std::vector<int> &in_texture_on_code_N2,
				set_of_left_upper_xy,//std::vector<std::vector<int>> &out_set_of_left_upper_xy,
				set_of_right_lower_xy,//std::vector<std::vector<int>> &out_set_of_right_lower_xy,
				p_v[n]);//int &out_offset)

			if (!Update_EI_Warping2(
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_texture_on_code_N1,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_texture_on_code_N2,
				set_of_left_upper_xy,//std::vector<std::vector<int>> &in_set_of_left_upper_xy,
				set_of_right_lower_xy,//std::vector<std::vector<int>> &in_set_of_right_lower_xy,
				ei_warping_index))//std::vector<std::vector<std::vector<int>>> &io_updated_EI_warping)
			{
				//std::vector<std::vector<int>> tttt;
				//if (!Update_Correspondances_Using_Slices(
				//	&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				//	io_segmented_texture_on_code[m][n],//std::vector<int> &in_reference_texture_on_code,
				//	io_segmented_texture_on_code[m][n+1],//std::vector<int> &in_target_texture_on_code,
				//	tttt))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
				//{
				//	exit(0);
				//	return;
				//}
				//
				//CKvMatrixBool debug_map;
				//bool **pdm = debug_map.c_Create(io_segmented_texture_on_code[m][n + 1].size(), io_segmented_texture_on_code[m][n].size(), false);
				//sz = tttt.size();
				//for (k = 0; k < sz; k++)
				//{
				//	for (l = 0; l < tttt[k].size(); l++)
				//	{
				//		pdm[k][tttt[k][l]] = true;
				//	}
				//}
				//CKvScreen sc;
				//sc.s_d_Display(&debug_map); 
				Kv_Printf("Error !!! Please Check Corres Map: m=%d/%d n=%d/%d)", m, num_blob, n, num_toc);
				exit(0);
			}

			//if ((m == 2) && (n >= 9))
			//if (     (m == 2) && (n==75)    )
			//{
			//	std::vector<std::vector<int>> tttt;
			//	if (!Update_Correspondances_Using_Slices(
			//		&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
			//		io_segmented_texture_on_code[m][n],//std::vector<int> &in_reference_texture_on_code,
			//		io_segmented_texture_on_code[m][n+1],//std::vector<int> &in_target_texture_on_code,
			//		tttt))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
			//	{
			//		exit(0);
			//		return;
			//	}
			//	CKvMatrixBool debug_map;
			//	bool **pdm = debug_map.c_Create(io_segmented_texture_on_code[m][n + 1].size(), io_segmented_texture_on_code[m][n].size(), false);
			//	sz = tttt.size();
			//	for (k = 0; k < sz; k++)
			//	{
			//		for (l = 0; l < tttt[k].size(); l++)
			//		{
			//			pdm[k][tttt[k][l]] = true;
			//		}
			//	}
			//	CKvScreen sc;
			//	sc.s_d_Display(&debug_map); 
			//	/*******************************************************************************/
			//	tmp_ToC = std::vector<std::vector<int>>(n+2, std::vector<int>());
			//	for (k = 0; k < ei_warping_index.size(); k++)
			//	{
			//		for (int i = 0; i < ei_warping_index[k].size(); i++)
			//		{
			//			for (l = 0; l < ei_warping_index[k][i].size(); l++)
			//			{
			//				tmp_ToC[i].push_back(io_segmented_texture_on_code[m][i][ei_warping_index[k][i][l]]);
			//			}
			//		}
			//	}
			//	save_data(tmp_ToC);
			//	Kv_Printf("Please Stop! n = %d",n);
			//}
			
		}

		tmp_ToC = std::vector<std::vector<int>>(num_toc, std::vector<int>());
		for (k = 0; k < ei_warping_index.size(); k++)
		{
			for (n = 0; n < ei_warping_index[k].size(); n++)
			{
				for (l = 0; l < ei_warping_index[k][n].size(); l++)
				{
					tmp_ToC[n].push_back(io_segmented_texture_on_code[m][n][ei_warping_index[k][n][l]]);
				}
			}
		}
		io_segmented_texture_on_code[m] = tmp_ToC;

		//if ((m == 4))
		//{
		//  CKvString str;
		//  str.fm_Format("index_blob_%02d.txt", m);
		//  save_data(str, tmp_ToC);
		//	Kv_Printf("Stop !");
		//}
	}

}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Align_Texture_on_Codes_Using_PWEI(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	int in_number_of_control_points,
	CKvSet_of_VectorInt *out_offset)
//*************************************************************************************
{
	// Error !
	std::vector<std::vector<std::vector<int>>> ei_warping_index;
	std::vector<std::vector<int>> tmp_ToC;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec;
	std::vector<std::vector<int>> set_of_left_upper_xy;
	std::vector<std::vector<int>> set_of_right_lower_xy;
	int num_blob, num_toc, max_length, max_idx, current_size, m, n, k, l;
	int *p_v;
	int sz, max_idx_bw;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0)
		{
			exit(0);
			return;
		}
		p_v = p_vec[m].c_Create(num_toc, 0);
		if (num_toc == 1) continue;

		ei_warping_index = std::vector<std::vector<std::vector<int>>>();
		for (n = 0; n < num_toc - 1; n++)
		{
			printf("m=%d/%d) n=%d/%d)\n", m, num_blob, n, num_toc);
			Find_EI_Ranges(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_texture_on_code_N1,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_texture_on_code_N2,
				in_number_of_control_points,//int in_number_of_control_points,
				set_of_left_upper_xy,//std::vector<std::vector<int>> &out_set_of_left_upper_xy,
				set_of_right_lower_xy,//std::vector<std::vector<int>> &out_set_of_right_lower_xy,
				p_v[n]);//int &out_offset)

			if (!Update_EI_Warping2(
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_texture_on_code_N1,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_texture_on_code_N2,
				set_of_left_upper_xy,//std::vector<std::vector<int>> &in_set_of_left_upper_xy,
				set_of_right_lower_xy,//std::vector<std::vector<int>> &in_set_of_right_lower_xy,
				ei_warping_index))//std::vector<std::vector<std::vector<int>>> &io_updated_EI_warping)
			{
				Kv_Printf("Error !!! Please Check Corres Map: m=%d/%d n=%d/%d)", m, num_blob, n, num_toc);
				exit(0);
			}


		}

		tmp_ToC = std::vector<std::vector<int>>(num_toc, std::vector<int>());
		for (k = 0; k < ei_warping_index.size(); k++)
		{
			for (n = 0; n < ei_warping_index[k].size(); n++)
			{
				for (l = 0; l < ei_warping_index[k][n].size(); l++)
				{
					tmp_ToC[n].push_back(io_segmented_texture_on_code[m][n][ei_warping_index[k][n][l]]);
				}
			}
		}
		io_segmented_texture_on_code[m] = tmp_ToC;
	}

}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Align_Texture_on_Codes_Using_PWEI2(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	int in_number_of_control_points,
	CKvSet_of_VectorInt *out_offset)
//*************************************************************************************
{
	std::vector<std::vector<std::vector<int>>> ei_warping_index;
	std::vector<std::vector<int>> tmp_ToC;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec;
	std::vector<std::vector<int>> set_of_left_upper_xy;
	std::vector<std::vector<int>> set_of_right_lower_xy;
	int num_blob, num_toc, max_length, max_idx, current_size, m, n, k, l;
	int *p_v, criteria_idx;
	int sz, max_idx_bw;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0)
		{
			exit(0);
			return;
		}
		p_v = p_vec[m].c_Create(num_toc, 0);
		if (num_toc == 1) continue;

		// Find Largest texture on code
		max_length = -1;
		for (n = 0; n < num_toc; n++)
		{
			if (max_length < (int)(io_segmented_texture_on_code[m][n].size()))
			{
				max_length = (int)(io_segmented_texture_on_code[m][n].size());
				criteria_idx = n;
			}
		}

		for (n = criteria_idx + 1; n < num_toc; n++)
		{
			Find_EI_Ranges(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n-1],//std::vector<int> &in_texture_on_code_N1, //Reference
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_texture_on_code_N2, // Target
				in_number_of_control_points,//int in_number_of_control_points,
				set_of_left_upper_xy,//std::vector<std::vector<int>> &out_set_of_left_upper_xy,
				set_of_right_lower_xy,//std::vector<std::vector<int>> &out_set_of_right_lower_xy,
				p_v[n]);//int &out_offset)
			
			EI_Warping(
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				max_length,//int in_max_length,
				set_of_left_upper_xy,//std::vector<std::vector<int>> in_set_of_left_upper_xy,
				set_of_right_lower_xy);//std::vector<std::vector<int>> in_set_of_right_lower_xy)

		}
		

		for (n = criteria_idx - 1; n > -1; n--)
		{
			Find_EI_Ranges(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_texture_on_code_N1, //Reference
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_texture_on_code_N2, // Target
				in_number_of_control_points,//int in_number_of_control_points,
				set_of_left_upper_xy,//std::vector<std::vector<int>> &out_set_of_left_upper_xy,
				set_of_right_lower_xy,//std::vector<std::vector<int>> &out_set_of_right_lower_xy,
				p_v[n]);//int &out_offset)

			EI_Warping(
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				max_length,//int in_max_length,
				set_of_left_upper_xy,//std::vector<std::vector<int>> in_set_of_left_upper_xy,
				set_of_right_lower_xy);//std::vector<std::vector<int>> in_set_of_right_lower_xy)
		}
	}
}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Align_Texture_on_Codes_Using_PWEI_Approximate(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_offset)
//*************************************************************************************
{
	std::vector<std::vector<bool>> cornerstone_map;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec;
	int num_blob, num_toc, m, n;
	int *p_v;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0)
		{
			exit(0);
			return;
		}
		p_v = p_vec[m].c_Create(num_toc, 0);
		if (num_toc == 1) continue;

		for (n = 0; n < num_toc - 1; n++)
		{
			//printf("m=%d/%d) n=%d/%d)\n", m, num_blob, n, num_toc);

			Set_Connectivity_Map(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n + 1]);//std::vector<int> &io_target_texture_on_code,
		}
	}
}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Align_Texture_on_Codes_Using_PWEI_Approximate2(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	int in_scanning_mode,
	CKvSet_of_VectorInt *out_offset)
//*************************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVoxel *p_set_v, c_voxel;
	CKvVectorInt *p_vec;
	double x_range[2], y_range[2], z_range[2];
	int k, m, n;
	int num_blob, num_toc, max_toc_sz, toc_sz, max_idx, idx, element_idx, center_idx, criteria_center, sz;
	int ww, hh, dd, center_p, distance_min, count;

	in_docube->gr_Get_Resolution(ww, hh, dd);
	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();
	p_set_v = set_v.vp();

	num_blob = io_segmented_texture_on_code.size();

	p_vec = out_offset->c_Create(num_blob);

	printf("=======Align_Texture_on_Codes_Using_PWEI_Approximate2=====\n");
	for (m = 0; m < num_blob; m++)
	{
		/***********************************************************************************/
		find_common_coordinate_range(
			&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
			io_segmented_texture_on_code[m],//std::vector<std::vector<int>> &in_segmented_texture_on_code,
			in_scanning_mode,//int in_scanning_mode,
			ww,//int in_max_resolution,
			x_range,//double *out_x_range_or_NULL,
			y_range,//double *out_y_range_or_NULL,
			z_range,//double *out_z_range_or_NULL)
			max_idx);//int &out_max_length_idx);

		//center_p = (int)round((z_range[0] + z_range[1])/2);
		center_p = (int)x_range[0];
		
		toc_sz     = io_segmented_texture_on_code[m][max_idx].size();
		max_toc_sz = io_segmented_texture_on_code[m][max_idx].size();
		for (n = 0; n < toc_sz; n++)
		{
			idx = io_segmented_texture_on_code[m][max_idx][n];
			if (center_p == p_set_v[idx].x)
			{
				element_idx     = n;
				center_idx      = (int)((max_toc_sz - 1) / 2);
				criteria_center = (int)((max_toc_sz - 1) / 2);

				printf("max_toc_sz = %d center_idx = %d criteria_center = %d\n", max_toc_sz, center_idx, criteria_center);

				c_voxel.s_Set(p_set_v[idx].x, p_set_v[idx].y, p_set_v[idx].z);

				if ((element_idx - center_idx) > 0)
				{
					sv_Circular_Shift_Vector(
						(element_idx - center_idx),//int in_location,
						io_segmented_texture_on_code[m][max_idx]);//std::vector<int> &io_vector);
				}
				else if ((element_idx - center_idx) < 0)
				{
					sv_Circular_Shift_Vector(
						(max_toc_sz - element_idx - 1),//int in_location,
						io_segmented_texture_on_code[m][max_idx]);//std::vector<int> &io_vector);
				}
				break;
			}
		}

		printf("m=%d/%d) max_toc_sz = %d center_idx = %d criteria_center = %d\n",
			      m,num_blob, max_toc_sz, center_idx, criteria_center);


		/***********************************************************************************/

		num_toc = io_segmented_texture_on_code[m].size();
		//std::vector<int> tmp_ToC = std::vector<int>(max_toc_sz, -1);
		//int *tmp_ToC = aa.c_Create(max_toc_sz, -1);
		for (k = 0; k < num_toc; k++)
		{
			p_vec[m].c_Create(num_toc, 0);

			if (max_idx == k)
			{
				continue;
			}

			align_ToC(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][k],//std::vector<int> &io_segmented_texture_on_code,
				&c_voxel,//CKvVoxel *in_center_voxel,
				max_toc_sz,//int in_max_Size_of_ToC,
				criteria_center,//int in_criteria_center_on_ToC,
				center_p);//int in_center_val);
		}

		int ww = io_segmented_texture_on_code[m][0].size();
		CKvMatrixUcharRgb tmp_img;
		unsigned char **pr, **pg, **pb;
		CKvRgbaF rrggbb;

		printf("num_toc = %d ww = %d\n", num_toc, ww);

		tmp_img.c_Create(num_toc, ww, Kv_Rgb(0,0,0));
		pr = tmp_img.mp(pg, pb);

		CKvDepot_of_RgbaF depot_rgb = in_docube->gvc_Get_Voxel_Color();

		for (int yy = 0; yy < num_toc; yy++)
		{
			for (int xx = 0; xx < ww; xx++)
			{
				if (io_segmented_texture_on_code[m][yy][xx] == -1)
				{
					continue;
				}
				depot_rgb.ge_Get_Element(io_segmented_texture_on_code[m][yy][xx], rrggbb);
				pr[yy][xx] = rrggbb.r*255.f;
				pg[yy][xx] = rrggbb.g*255.f;
				pb[yy][xx] = rrggbb.b*255.f;
			}
		}

		CKvScreen sc;
		sc.s_d_Display(&tmp_img);
		Kv_Printf("ChECK m=%d/%d)", m, num_blob);
	}


}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::find_common_coordinate_range(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<std::vector<int>> &in_segmented_texture_on_code,
	int in_scanning_mode,
	int in_max_resolution,
	double *out_x_range_or_NULL,
	double *out_y_range_or_NULL,
	double *out_z_range_or_NULL,
	int &out_max_length_idx)
//*************************************************************************************
{
	LCKvUtility_for_Xrunset1d lux1d;
	CKvXrunset1dShort rangeX, rangeY, rangeZ, sub_range;
	CKvVectorBool patternX, patternY, patternZ;
	CKvVectorBool sub_patternX, sub_patternY, sub_patternZ;
	CKvVoxel *p_sv;
	double minx, maxx, miny, maxy, minz, maxz;
	int num_toc, max_length, sz, idx, k, m;
	short *a;

	p_sv    = in_set_of_voxels->vp();
	num_toc = in_segmented_texture_on_code.size();

	minx = DBL_MAX;
	miny = DBL_MAX;
	minz = DBL_MAX;
	
	maxx = DBL_MIN;
	maxy = DBL_MIN;
	maxz = DBL_MIN;

	max_length = INT_MIN;

	for (k = 0; k < num_toc; k++)
	{
		sz = in_segmented_texture_on_code[k].size();
		if (max_length < sz)
		{
			max_length = sz;
			out_max_length_idx = k;
		}
	}

	patternX.c_Create(in_max_resolution, true);
	sub_patternX.c_Create(in_max_resolution, false);
	rangeX.im_Import(patternX);

	patternY.c_Create(in_max_resolution, true);
	sub_patternY.c_Create(in_max_resolution, false);
	rangeY.im_Import(patternY);

	patternZ.c_Create(in_max_resolution, true);
	sub_patternZ.c_Create(in_max_resolution, false);
	rangeZ.im_Import(patternZ);

	for (k = 0; k < num_toc; k++)
	{
		sz = in_segmented_texture_on_code[k].size();
		for (m = 0; m < sz; m++)
		{
			idx = in_segmented_texture_on_code[k][m];
			if (maxx < p_sv[idx].x){ maxx = p_sv[idx].x; }
			if (minx > p_sv[idx].x){ minx = p_sv[idx].x; }
			if (maxy < p_sv[idx].y){ maxy = p_sv[idx].y; }
			if (miny > p_sv[idx].y){ miny = p_sv[idx].y; }
			if (maxz < p_sv[idx].z){ maxz = p_sv[idx].z; }
			if (minz > p_sv[idx].z){ minz = p_sv[idx].z; }
		}

		sub_patternX.s_Set((int)minx, (int)(maxx - minx + 1), true);
		sub_patternY.s_Set((int)miny, (int)(maxy - miny + 1), true);
		sub_patternZ.s_Set((int)minz, (int)(maxz - minz + 1), true);

		sub_range.im_Import(sub_patternX);
		lux1d.int_Intersection(rangeX, sub_range, rangeX);

		sub_range.im_Import(sub_patternY);
		lux1d.int_Intersection(rangeY, sub_range, rangeY);

		sub_range.im_Import(sub_patternZ);
		lux1d.int_Intersection(rangeZ, sub_range, rangeZ);

		sub_patternX.sv_Set_Vector(false);
		sub_patternY.sv_Set_Vector(false);
		sub_patternZ.sv_Set_Vector(false);


	}

	rangeX.z_px_Pointer_of_Xvector()->z_gii(NULL, &a, NULL, NULL);
	out_x_range_or_NULL[0] = a[0];
	out_x_range_or_NULL[1] = a[1];

	rangeY.z_px_Pointer_of_Xvector()->z_gii(NULL, &a, NULL, NULL);
	out_y_range_or_NULL[0] = a[0];
	out_y_range_or_NULL[1] = a[1];

	rangeZ.z_px_Pointer_of_Xvector()->z_gii(NULL, &a, NULL, NULL);
	out_z_range_or_NULL[0] = a[0];
	out_z_range_or_NULL[1] = a[1];
}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::align_ToC(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &io_segmented_texture_on_code,
	CKvVoxel *in_center_voxel,
	int in_max_Size_of_ToC,
	int in_criteria_center_on_ToC,
	int in_center_val)
//*************************************************************************************
{
	std::vector<int> tmp_ToC = std::vector<int>(in_max_Size_of_ToC, -1);
	CKvVoxel *p_set_v;
	int toc_sz, center_idx, distance_min, element_idx;
	int count, idx, n;

	p_set_v = in_set_of_voxels->vp();

	toc_sz = io_segmented_texture_on_code.size();
	distance_min = INT_MAX;
	for (n = 0; n < toc_sz; n++)
	{
		idx = io_segmented_texture_on_code[n];
		if (in_center_val == p_set_v[idx].x)
		{
			if (distance_min > abs(p_set_v[idx].z - in_center_voxel->z))
			{
				distance_min = abs(p_set_v[idx].z - in_center_voxel->z);
				element_idx = n;
			}
		}
	}

	center_idx = (int)round((toc_sz - 1) / 2);
	if ((element_idx - center_idx) > 0)
	{
		sv_Circular_Shift_Vector(
			(element_idx - center_idx),//int in_location,
			io_segmented_texture_on_code);//std::vector<int> &io_vector);
	}
	else if ((element_idx - center_idx) < 0)
	{
		sv_Circular_Shift_Vector(
			(toc_sz - element_idx - 1),//int in_location,
			io_segmented_texture_on_code);//std::vector<int> &io_vector);
	}

	// Set elements. 
	count = in_criteria_center_on_ToC;
	for (n = center_idx; n > -1; n--)
	{
		tmp_ToC[count] = io_segmented_texture_on_code[n];
		count--;
	}
	count = in_criteria_center_on_ToC + 1;
	for (n = center_idx + 1; n < toc_sz; n++)
	{
		tmp_ToC[count] = io_segmented_texture_on_code[n];
		count++;
	}

	io_segmented_texture_on_code = std::vector<int>();
	for (n = 0; n < in_max_Size_of_ToC; n++)
	{
		io_segmented_texture_on_code.push_back(tmp_ToC[n]);
	}

}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Find_EI_Ranges(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_texture_on_code_N1,
	std::vector<int> &in_texture_on_code_N2,
	std::vector<std::vector<int>> &out_set_of_left_upper_xy,
	std::vector<std::vector<int>> &out_set_of_right_lower_xy,
	int &out_offset)
//*************************************************************************************
{
	std::vector<std::vector<bool>> cornerstone_map;
	std::vector<std::vector<int>> set_of_left_upper_xy;
	std::vector<std::vector<int>> set_of_right_lower_xy;
	int num_block, k;
	int starting_index1, starting_index2;
	int length1, length2;
	int warping_length1, warping_length2;

	Set_Connectivity_Map(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_texture_on_code_N1,//std::vector<int> &in_reference_texture_on_code,
		in_texture_on_code_N2,//std::vector<int> &in_target_texture_on_code,
		cornerstone_map);//std::vector<std::vector<bool>> &out_cornerstone_map)

	Extract_Sub_Distance_Maps(
		cornerstone_map,//std::vector<std::vector<bool>> &in_cornerstone_map, 
		out_set_of_left_upper_xy,//std::vector<std::vector<int>> &out_set_of_left_upper_xy, 
		out_set_of_right_lower_xy);//std::vector<std::vector<int>> &out_set_of_right_lower_xy);

	out_offset = 0;
}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Find_EI_Ranges(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_texture_on_code_N1,
	std::vector<int> &in_texture_on_code_N2,
	int in_number_of_control_points,
	std::vector<std::vector<int>> &out_set_of_left_upper_xy,
	std::vector<std::vector<int>> &out_set_of_right_lower_xy,
	int &out_offset)
//*************************************************************************************
{
	std::vector<std::vector<bool>> cornerstone_map;
	std::vector<std::vector<int>> set_of_left_upper_xy;
	std::vector<std::vector<int>> set_of_right_lower_xy;
	int num_block, k;
	int starting_index1, starting_index2;
	int length1, length2;
	int warping_length1, warping_length2;

	Set_Connectivity_Map(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_number_of_control_points,//int in_number_of_control_points,
		in_texture_on_code_N1,//std::vector<int> &in_reference_texture_on_code,
		in_texture_on_code_N2,//std::vector<int> &in_target_texture_on_code,
		cornerstone_map);//std::vector<std::vector<bool>> &out_cornerstone_map)

	Extract_Sub_Distance_Maps(
		cornerstone_map,//std::vector<std::vector<bool>> &in_cornerstone_map, 
		out_set_of_left_upper_xy,//std::vector<std::vector<int>> &out_set_of_left_upper_xy, 
		out_set_of_right_lower_xy);//std::vector<std::vector<int>> &out_set_of_right_lower_xy);

	out_offset = 0;
}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::EI_Warping(
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int in_max_length,
	std::vector<std::vector<int>> in_set_of_left_upper_xy,
	std::vector<std::vector<int>> in_set_of_right_lower_xy)
//*************************************************************************************
{
	std::vector<int> ei_target_toc;
	CKvVectorInt tmp;
	CKvVectorBool tmp_mask;
	int num_part, warping_length, k, m, z;
	int xo, x1, yo, y1;
	int *p_tmp; bool *p_tmp_mask;

	num_part = in_set_of_left_upper_xy.size();
	ei_target_toc = std::vector<int>(in_max_length);
	for (k = 0; k < num_part; k++)
	{
		xo = in_set_of_left_upper_xy[k][0]; // 0 <- reference. 
		yo = in_set_of_left_upper_xy[k][1]; // 1 <- target. 

		x1 = in_set_of_right_lower_xy[k][0];
		y1 = in_set_of_right_lower_xy[k][1];
		
		warping_length = x1 - xo + 1;
		p_tmp          = tmp.c_Create(warping_length,-1);
		p_tmp_mask     = tmp_mask.c_Create(warping_length, false);
		z = 0;
		for (m = yo; m <= y1; m++)
		{
			p_tmp[z] = io_target_texture_on_code[m];
			p_tmp_mask[z] = true;
			z++;
		}
		//printf("k=%d/%d) origin : %d warping : %d\n", k, num_part, y1-yo+1, warping_length);
		zz_vcl_ei_for_align.EI_DCT_Forward_on_Spatial_Domain2(
			&tmp,//CKvVectorInt *io_img,
			&tmp_mask);//CKvVectorBool *in_mask)
		
		p_tmp = tmp.vp();
		z = 0;
		for (m = xo ; m <= x1 ; m++)
		{
			ei_target_toc[m] = p_tmp[z];
			z++;
		}
	}

	io_target_texture_on_code = std::vector<int>(in_max_length);
	for (m = 0; m < in_max_length; m++)
	{
		io_target_texture_on_code[m] = ei_target_toc[m];
	}

	//io_target_texture_on_code = ei_target_toc;

}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Update_EI_Warping_Index(
	std::vector<std::vector<int>> &io_warping_index,
	int in_current_ToC_index,
	std::vector<std::vector<int>> &in_warping_index)
//*************************************************************************************
{
	int num_warping_index, num_element, k, m;

	if (io_warping_index.size() == 0)
	{
		io_warping_index.push_back(in_warping_index[0]);
		io_warping_index.push_back(in_warping_index[1]);
		
		return;
	}

	num_warping_index = io_warping_index.size();

	for (k = 0; k < num_warping_index; k++)
	{
		num_element = io_warping_index[k].size();
		for (m = 0; m < num_element; m += 3)
		{
			io_warping_index[k][m];
			io_warping_index[k][m+1];
			io_warping_index[k][m+2];
		}
	}


}
//*************************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Update_EI_Warping(
	std::vector<int> &in_texture_on_code_N1,
	std::vector<int> &in_texture_on_code_N2,
	std::vector<std::vector<int>> &in_set_of_left_upper_xy,
	std::vector<std::vector<int>> &in_set_of_right_lower_xy,
	std::vector<std::vector<std::vector<int>>> &io_updated_EI_warping)
//*************************************************************************************
{
	/************************************************/
	// in_set_of_left_upper_xy  : N - by - 2
	// in_set_of_right_lower_xy : N - by - 2
	// y -> N2 , x -> N1
	// x축을 y축으로 projection 시킨다고 생각해야함. 
	/************************************************/
	std::vector<std::vector<std::vector<int>>> tmp_ei_warping;
	std::vector<float> y;
	int num_block, num_element_N1, num_element_N2, num_element;
	int k, m, n, l, updated_num_element_ToC, updated_num_ToC;

	num_block       = in_set_of_left_upper_xy.size();
	num_element_N1  = in_texture_on_code_N1.size();
	num_element_N2  = in_texture_on_code_N2.size();
	updated_num_element_ToC = io_updated_EI_warping.size();

	if (updated_num_element_ToC == 0)
	{
		tmp_ei_warping = std::vector<std::vector<std::vector<int>>>
			(num_element_N2, std::vector<std::vector<int>>(2));
		for (k = 0; k < num_element_N1; k++)
		{
			if (!linear_project_point(
				k,//int in_index_x,
				in_set_of_left_upper_xy,//std::vector<std::vector<int>> &in_set_of_left_upper_xy,
				in_set_of_right_lower_xy,//std::vector<std::vector<int>> &in_set_of_right_lower_xy,
				y))//float &out_y);
			{
				printf("Error Message !\n");
				printf("num_block = %d\n", num_block);
				printf("num_element_N1 = %d\n", num_element_N1);
				printf("num_element_N2 = %d\n", num_element_N2);
				printf("updated_num_element_ToC = %d\n", updated_num_element_ToC);

				Kv_Printf("Error ! linear_project_point");
				//exit(0);
				return false;
			}

			for (l = 0; l < y.size(); l++)
			{
				tmp_ei_warping[(int)round(y[l])][0].push_back(k);
			}
			
		}

		for (k = 0; k < num_element_N2; k++)
		{
			for (m = 0; m < tmp_ei_warping[k][0].size(); m++)
			{
				tmp_ei_warping[k][1].push_back(k);
			}
		}
	}
	else
	{
		if (updated_num_element_ToC != num_element_N1)
		{
			Kv_Printf("Error ! Update_EI_Warping[0]");
			//exit(0);
			return false;
		}

		updated_num_ToC = io_updated_EI_warping[0].size();
		tmp_ei_warping = std::vector<std::vector<std::vector<int>>>
			(num_element_N2, std::vector<std::vector<int>>(updated_num_ToC + 1));

		for (k = 0; k < updated_num_element_ToC; k++)
		{
			if (!linear_project_point(
				k,//int in_index_x,
				in_set_of_left_upper_xy,//std::vector<std::vector<int>> &in_set_of_left_upper_xy,
				in_set_of_right_lower_xy,//std::vector<std::vector<int>> &in_set_of_right_lower_xy,
				y))//std::vector<float> &out_y);
			{
				printf("Error Message !\n");
				printf("num_block = %d\n", num_block);
				printf("num_element_N1 = %d\n", num_element_N1);
				printf("num_element_N2 = %d\n", num_element_N2);
				printf("updated_num_element_ToC = %d\n", updated_num_element_ToC);

				Kv_Printf("Error ! linear_project_point");
				//exit(0);
				return false;
			}

			for (l = 0; l < y.size(); l++)
			{
				for (m = 0; m < updated_num_ToC; m++)
				{
					num_element = io_updated_EI_warping[k][m].size();
					for (n = 0; n < num_element; n++)
					{
						tmp_ei_warping[(int)round(y[l])][m].push_back(io_updated_EI_warping[k][m][n]);
					}
				}
			}
		}

		for (k = 0; k < num_element_N2; k++)
		{
			for (m = 0; m < tmp_ei_warping[k][updated_num_ToC - 1].size(); m++)
			{
				tmp_ei_warping[m][updated_num_ToC].push_back(k);
			}
		}
	}

	io_updated_EI_warping = tmp_ei_warping;
	return true;
}
//*************************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Update_EI_Warping2(
	std::vector<int> &in_texture_on_code_N1,
	std::vector<int> &in_texture_on_code_N2,
	std::vector<std::vector<int>> &in_set_of_left_upper_xy,
	std::vector<std::vector<int>> &in_set_of_right_lower_xy,
	std::vector<std::vector<std::vector<int>>> &io_updated_EI_warping)
//*************************************************************************************
{
	/************************************************/
	// in_set_of_left_upper_xy  : N - by - 2
	// in_set_of_right_lower_xy : N - by - 2
	// y -> N2 , x -> N1
	// x축을 y축으로 projection 시킨다고 생각해야함. 
	/************************************************/
	std::vector<std::vector<std::vector<int>>> tmp_ei_warping;
	std::vector<std::vector<float>> table_xy;
	std::vector<float> y;
	float ratio;
	int num_block, num_element_N1, num_element_N2, num_element;
	int k, m, n, l, updated_num_element_ToC, updated_num_ToC;
	int len_X, len_Y, xx, yy;

	num_block = in_set_of_left_upper_xy.size();
	num_element_N1 = in_texture_on_code_N1.size(); // x축임
	num_element_N2 = in_texture_on_code_N2.size(); // y축임
	updated_num_element_ToC = io_updated_EI_warping.size();

	if (updated_num_element_ToC == 0)
	{
		tmp_ei_warping = std::vector<std::vector<std::vector<int>>>
			(num_element_N2, std::vector<std::vector<int>>(2));
		
		for (k = 0; k < num_block; k++)
		{
			len_X = in_set_of_right_lower_xy[k][0] - in_set_of_left_upper_xy[k][0]+1;
			len_Y = in_set_of_right_lower_xy[k][1] - in_set_of_left_upper_xy[k][1]+1;
			if (len_X < len_Y) // y축(N2값)을 x축에 투영해서 대응점을 찾기. 
			{
				linear_project_point(
					in_set_of_left_upper_xy[k][1], in_set_of_right_lower_xy[k][1],//int in_x1, int in_x2, -> 실제로는 y축
					in_set_of_left_upper_xy[k][0], in_set_of_right_lower_xy[k][0],//int in_y1, int in_y2, -> 실제로는 x축
					table_xy);//std::vector<float> &out_y)

				// 이렇게 하면 안되고, 진짜 늘려서 투영시켜야함. 그런데, 첫번째 case니까 이대로 해도 괜찮음. 
				for (l = 0; l < table_xy.size(); l++)
				{
					tmp_ei_warping[(int)round(table_xy[l][0])][0].push_back((int)round(table_xy[l][1]));
				}
			}
			else // 평소대로 x축을 y축으로.... 
			{
				linear_project_point(
					in_set_of_left_upper_xy[k][0], in_set_of_right_lower_xy[k][0],//int in_x1, int in_x2,
					in_set_of_left_upper_xy[k][1], in_set_of_right_lower_xy[k][1],//int in_y1, int in_y2,
					table_xy);//std::vector<float> &out_y)

				for (l = 0; l < table_xy.size(); l++)
				{
					tmp_ei_warping[(int)round(table_xy[l][1])][0].push_back((int)round(table_xy[l][0]));
				}
			}
		}

		for (m = 0; m < tmp_ei_warping.size(); m++)
		{
			for (n = 0; n < tmp_ei_warping[m][0].size(); n++)
			{
				tmp_ei_warping[m][1].push_back(m);
			}
		}

	}
	else
	{
		if (updated_num_element_ToC != num_element_N1)
		{
			Kv_Printf("Error ! Update_EI_Warping[0]");
			//exit(0);
			return false;
		}

		updated_num_ToC = io_updated_EI_warping[0].size();
		tmp_ei_warping = std::vector<std::vector<std::vector<int>>>
			(num_element_N2, std::vector<std::vector<int>>(updated_num_ToC + 1));

		for (k = 0; k < num_block; k++)
		{
			len_X = in_set_of_right_lower_xy[k][0] - in_set_of_left_upper_xy[k][0] + 1;
			len_Y = in_set_of_right_lower_xy[k][1] - in_set_of_left_upper_xy[k][1] + 1;

			if (len_X < len_Y) // y축 ( N2값을 x축에 투영해서 대응점을 찾기. 
			{
				//아래는 수정본. 
				std::vector<std::vector<float>> cumulative_table_yx = std::vector<std::vector<float>>(num_element_N1);
				linear_project_point(
					in_set_of_left_upper_xy[k][1], in_set_of_right_lower_xy[k][1],//int in_x1, int in_x2,
					in_set_of_left_upper_xy[k][0], in_set_of_right_lower_xy[k][0],//int in_y1, int in_y2,
					table_xy,//std::vector<float> &out_y,
					cumulative_table_yx);//std::vector<std::vector<float>> &out_cumulative_table_yx)

				for (m = 0; m < updated_num_ToC; m++)
				{
					for (l = 0; l < cumulative_table_yx.size(); l++) // x축임. 
					{
						if (cumulative_table_yx[l].size() != 0)
						{
							ratio = cumulative_table_yx[l].size() / (float)io_updated_EI_warping[l][m].size();
							if (ratio <= 1.f) // 많은 수의 voxel로 적은곳으로 mapping. 
							{
								for (n = 0; n < io_updated_EI_warping[l][m].size(); n++)
								{
									int idx = n * ratio;
									tmp_ei_warping[cumulative_table_yx[l][0] + idx][m].push_back(io_updated_EI_warping[l][m][n]);
								}
							}
							else // 적은 수의 voxel로 많은곳으로 mapping. 
							{
								for (n = 0; n < cumulative_table_yx[l].size(); n++)
								{
									int idx = (int)((cumulative_table_yx[l][n] - cumulative_table_yx[l][0]) / ratio);
									tmp_ei_warping[cumulative_table_yx[l][n]][m].push_back(io_updated_EI_warping[l][m][idx]);
								}
							}

						}
					}
					//for (l = 0; l < table_xy.size(); l++)
					//{
					//	xx = (int)round(table_xy[l][1]);
					//	yy = (int)round(table_xy[l][0]);
					//	ratio = cumulative_table_yx[xx].size() / (float)io_updated_EI_warping[xx][m].size();
					//	if (ratio <= 1.f)
					//	{
					//		for (n = 0; n < io_updated_EI_warping[xx][m].size(); n++)
					//		{
					//			tmp_ei_warping[yy][m].push_back(io_updated_EI_warping[xx][m][n]);
					//		}
					//	}
					//	else
					//	{
					//		for (n = 0; n < cumulative_table_yx[xx].size(); n++)
					//		{
					//			int idx = (int)((cumulative_table_yx[xx][n] - cumulative_table_yx[xx][0]) / ratio);
					//			tmp_ei_warping[cumulative_table_yx[xx][n]][m].push_back(io_updated_EI_warping[xx][m][idx]);
					//		}
					//	}
					//}
				}
			}
			else // 평소대로 x축을 y축으로.... 
			{
				//std::vector<std::vector<float>> cumulative_table_yx = std::vector<std::vector<float>>(num_element_N1);
				//linear_project_point(
				//	in_set_of_left_upper_xy[k][0], in_set_of_right_lower_xy[k][0],//int in_x1, int in_x2,
				//	in_set_of_left_upper_xy[k][1], in_set_of_right_lower_xy[k][1],//int in_y1, int in_y2,
				//	table_xy,//std::vector<float> &out_y,
				//	cumulative_table_yx);//std::vector<std::vector<float>> &out_cumulative_table_yx)
				//for (m = 0; m < updated_num_ToC; m++)
				//{
				//	for (l = 0; l < cumulative_table_yx.size(); l++) // y축임. 
				//	{
				//		if (cumulative_table_yx[l].size() != 0)
				//		{
				//			for (n = 0; n < cumulative_table_yx[l].size(); n++)
				//			{
				//			}
				//			ratio = cumulative_table_yx[l].size() / (float)io_updated_EI_warping[l][m].size();
				//		}
				//	}
				//}


				// Old version
				linear_project_point(
					in_set_of_left_upper_xy[k][0], in_set_of_right_lower_xy[k][0],//int in_x1, int in_x2,
					in_set_of_left_upper_xy[k][1], in_set_of_right_lower_xy[k][1],//int in_y1, int in_y2,
					table_xy);//std::vector<float> &out_y)
				for (m = 0; m < updated_num_ToC; m++)
				{
					for (l = 0; l < table_xy.size(); l++)
					{
						xx = (int)round(table_xy[l][0]);
						yy = (int)round(table_xy[l][1]);
						for (n = 0; n < io_updated_EI_warping[xx][m].size(); n++)
						{
							tmp_ei_warping[yy][m].push_back(io_updated_EI_warping[xx][m][n]);
						}
					}
				}

			}
		}

		for (m = 0; m < tmp_ei_warping.size(); m++)
		{
			for (n = 0; n < tmp_ei_warping[m][0].size(); n++)
			{
				tmp_ei_warping[m][updated_num_ToC].push_back(m);
			}
		}
	}

	io_updated_EI_warping = tmp_ei_warping;
	
	return true;
}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Set_Connectivity_Map(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	std::vector<std::vector<bool>> &out_cornerstone_map)
//*************************************************************************************
{
	std::vector<std::vector<int>> idx_on_dm_yx;
	std::vector<std::vector<int>> shift_distance_map;
	std::vector<int> cpy_ToC;
	std::vector<double> score_vector;
	int num_ref, num_target, idx, k, m, sz;

	num_ref    = in_reference_texture_on_code.size();
	num_target = io_target_texture_on_code.size();

	// Find Distance Map. 
	/************************************************************************************/
	if (!Update_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		io_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		idx_on_dm_yx))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
	{
		exit(0);
		return;
	}
	/************************************************************************************/

	/************************************************************************************/
	// Align Signal Ver2.
	int min_corres = INT_MAX;
	for (k = 0; k < num_target; k++)
	{
		for (m = 0; m < idx_on_dm_yx[k].size(); m++)
		{
			if (idx_on_dm_yx[k][m] < min_corres)
			{
				min_corres = idx_on_dm_yx[k][m];
				idx = k;
			}
		}
	}
	/************************************************************************************/

	/************************************************************************************/
	sv_Circular_Shift_Vector(idx, io_target_texture_on_code);

	if (!Update_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		io_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		idx_on_dm_yx))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
	{
		exit(0);
		return;
	}

	out_cornerstone_map = std::vector<std::vector<bool>>(num_target, std::vector<bool>(num_ref, false));
	sz = idx_on_dm_yx.size();
	for (k = 0; k < sz; k++)
	{
		for (m = 0; m < idx_on_dm_yx[k].size(); m++)
		{
			out_cornerstone_map[k][idx_on_dm_yx[k][m]] = true;
		}
	}

	//out_cornerstone_map[0][0] = true;
	//out_cornerstone_map[num_target-1][num_ref-1] = true;


}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Set_Connectivity_Map(
	CKvSet_of_Voxel *in_set_of_voxels,
	int in_number_of_control_points,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	std::vector<std::vector<bool>> &out_cornerstone_map)
//*************************************************************************************
{
	std::vector<std::vector<int>> idx_on_dm_yx;
	std::vector<std::vector<int>> shift_distance_map;
	std::vector<int> cpy_ToC;
	std::vector<double> score_vector;
	int num_ref, num_target, idx, k, m, sz;

	num_ref = in_reference_texture_on_code.size();
	num_target = io_target_texture_on_code.size();

	// Find Distance Map. 
	/************************************************************************************/
	if (!Update_K_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		io_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		in_number_of_control_points,//int in_number_of_control_points,
		idx_on_dm_yx))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
	{
		exit(0);
		return;
	}
	/************************************************************************************/

	/************************************************************************************/
	// Align Signal Ver2.
	int min_corres = INT_MAX;
	for (k = 0; k < num_target; k++)
	{
		for (m = 0; m < idx_on_dm_yx[k].size(); m++)
		{
			if (idx_on_dm_yx[k][m] < min_corres)
			{
				min_corres = idx_on_dm_yx[k][m];
				idx = k;
			}
		}
	}
	/************************************************************************************/

	/************************************************************************************/
	sv_Circular_Shift_Vector(idx, io_target_texture_on_code);

	if (!Update_K_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		io_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		in_number_of_control_points,//int in_number_of_control_points,
		idx_on_dm_yx))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
	{
		exit(0);
		return;
	}

	out_cornerstone_map = std::vector<std::vector<bool>>(num_target, std::vector<bool>(num_ref, false));
	sz = idx_on_dm_yx.size();
	for (k = 0; k < sz; k++)
	{
		for (m = 0; m < idx_on_dm_yx[k].size(); m++)
		{
			out_cornerstone_map[k][idx_on_dm_yx[k][m]] = true;
		}
	}

	//out_cornerstone_map[0][0] = true;
	//out_cornerstone_map[num_target-1][num_ref-1] = true;



}
//*************************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_PWEI::Set_Connectivity_Map(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code)
//*************************************************************************************
{
	std::vector<std::vector<int>> idx_on_dm_yx;
	std::vector<std::vector<int>> shift_distance_map;
	std::vector<int> cpy_ToC;
	std::vector<double> score_vector;
	int num_ref, num_target, idx, k, m, sz;

	num_ref = in_reference_texture_on_code.size();
	num_target = io_target_texture_on_code.size();

	// Find Distance Map. 
	/************************************************************************************/
	if (!Update_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		io_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		2,//int in_mode,
		idx_on_dm_yx))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
	{
		exit(0);
		return;
	}
	/************************************************************************************/

	/************************************************************************************/
	// Align Signal Ver2.
	int min_corres = INT_MAX;
	for (k = 0; k < num_target; k++)
	{
		for (m = 0; m < idx_on_dm_yx[k].size(); m++)
		{
			if (idx_on_dm_yx[k][m] < min_corres)
			{
				min_corres = idx_on_dm_yx[k][m];
				idx = k;
			}
		}
	}
	/************************************************************************************/

	/************************************************************************************/
	sv_Circular_Shift_Vector(idx, io_target_texture_on_code);

}
//*************************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Update_Correspondances_Using_Slices(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
//*************************************************************************************
{
	std::vector<int> indices;
	int k, q, num_target_voxel, num_reference_voxel;
	bool flag = false;
	num_reference_voxel = in_reference_texture_on_code.size();
	num_target_voxel = in_target_texture_on_code.size();
	out_idx_on_distance_map_yx = std::vector<std::vector<int>>(num_target_voxel);

	// Set Slice distance -> 0
#define omp parallel
	{
#define omp for private(indices) private(q)
		for (k = 0; k < num_target_voxel; k++)
		{
			if (Find_Candidate_of_Corresponds_In_X_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[k],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag = true;
				for (q = 0; q < indices.size(); q++)
				{
					out_idx_on_distance_map_yx[k].push_back(indices[q]);
				}
			}
		}
	}

	if (flag == false)
	{
		for (k = 0; k < num_target_voxel; k++)
		{
			if (Find_Candidate_of_Corresponds_In_Z_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[k],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag = true;
				for (q = 0; q < indices.size(); q++)
				{
					out_idx_on_distance_map_yx[k].push_back(indices[q]);
				}
			}
		}
	}
	
	if (flag == false)
	{
		for (k = 0; k < num_target_voxel; k++)
		{
			if (Find_Candidate_of_Corresponds_In_Y_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[k],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag = true;
				for (q = 0; q < indices.size(); q++)
				{
					out_idx_on_distance_map_yx[k].push_back(indices[q]);
				}
			}
		}
	}


	if (flag == false)
	{
		CKvVoxel *pp;
		pp = in_set_of_voxels->vp();
		printf("Error ! : There are no correspondences\n");

		printf("Reference Voxels XYZ\n");
		for (k = 0; k < in_reference_texture_on_code.size(); k++)
		{
			if(in_reference_texture_on_code[k] >= 0)
				printf("%d %d %d\n", 
					pp[in_reference_texture_on_code[k]].x,
					pp[in_reference_texture_on_code[k]].y,
					pp[in_reference_texture_on_code[k]].z);
		}

		printf("Target Voxels XYZ\n");
		for (k = 0; k < in_target_texture_on_code.size(); k++)
		{
			if (in_target_texture_on_code[k] >= 0)
				printf("%d %d %d\n", 
					pp[in_target_texture_on_code[k]].x,
					pp[in_target_texture_on_code[k]].y,
					pp[in_target_texture_on_code[k]].z);
		}
	}

	return true;
}
//*************************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Update_K_Correspondances_Using_Slices(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	int in_number_of_control_points,
	std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
//*************************************************************************************
{
	std::vector<int> indices;
	std::vector<bool> flag_vec;
	float step, point_step;
	int k, q, idx, num_target_voxel, num_reference_voxel;
	int count;

	num_reference_voxel = in_reference_texture_on_code.size();
	num_target_voxel    = in_target_texture_on_code.size();
	step				= (num_target_voxel - 1) / (in_number_of_control_points - 1);

	out_idx_on_distance_map_yx = std::vector<std::vector<int>>(num_target_voxel);
	flag_vec                   = std::vector<bool>(in_number_of_control_points,false);


	count = 0;
	for (point_step = 0; point_step < num_target_voxel; point_step += step)
	{
		idx = round(point_step);

		if (flag_vec[count] == false)
		{
			if (Find_Candidate_of_Corresponds_In_X_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[idx],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag_vec[count] = true;
				for (k = 0; k < indices.size(); k++)
				{
					out_idx_on_distance_map_yx[idx].push_back(indices[k]);
				}
			}
		}
		count++;
	}

	count = 0;
	for (point_step = 0; point_step < num_target_voxel; point_step += step)
	{
		idx = round(point_step);

		if (flag_vec[count] == false)
		{
			if (Find_Candidate_of_Corresponds_In_Z_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[idx],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag_vec[count] = true;
				for (k = 0; k < indices.size(); k++)
				{
					out_idx_on_distance_map_yx[idx].push_back(indices[k]);
				}
			}
		}
		count++;
	}

	count = 0;
	for (point_step = 0; point_step < num_target_voxel; point_step += step)
	{
		idx = round(point_step);

		if (flag_vec[count] == false)
		{
			if (Find_Candidate_of_Corresponds_In_Y_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[idx],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag_vec[count] = true;
				for (k = 0; k < indices.size(); k++)
				{
					out_idx_on_distance_map_yx[idx].push_back(indices[k]);
				}
			}
		}
		count++;
	}

//	if (flag == false)
//	{
//		CKvVoxel *pp;
//		pp = in_set_of_voxels->vp();
//		printf("Error ! : There are no correspondences\n");
//
//		printf("Reference Voxels XYZ\n");
//		for (k = 0; k < in_reference_texture_on_code.size(); k++)
//		{
//			if (in_reference_texture_on_code[k] >= 0)
//				printf("%d %d %d\n",
//					pp[in_reference_texture_on_code[k]].x,
//					pp[in_reference_texture_on_code[k]].y,
//					pp[in_reference_texture_on_code[k]].z);
//		}
//
//		printf("Target Voxels XYZ\n");
//		for (k = 0; k < in_target_texture_on_code.size(); k++)
//		{
//			if (in_target_texture_on_code[k] >= 0)
//				printf("%d %d %d\n",
//					pp[in_target_texture_on_code[k]].x,
//					pp[in_target_texture_on_code[k]].y,
//					pp[in_target_texture_on_code[k]].z);
//		}
//	}

	return true;
}
//*************************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Update_Correspondances_Using_Slices(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	int in_mode,
	std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
//*************************************************************************************
{
	std::vector<int> indices;
	int k, q, num_target_voxel, num_reference_voxel;
	bool flag = false;
	num_reference_voxel = in_reference_texture_on_code.size();
	num_target_voxel = in_target_texture_on_code.size();
	out_idx_on_distance_map_yx = std::vector<std::vector<int>>(num_target_voxel);

	if (in_mode == 0)
	{
		for (k = 0; k < num_target_voxel; k++)
		{
			if (Find_Candidate_of_Corresponds_In_X_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[k],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag = true;
				for (q = 0; q < indices.size(); q++)
				{
					out_idx_on_distance_map_yx[k].push_back(indices[q]);
				}
			}
		}
	}


	if(in_mode == 2)
	{
		for (k = 0; k < num_target_voxel; k++)
		{
			if (Find_Candidate_of_Corresponds_In_Z_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[k],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag = true;
				for (q = 0; q < indices.size(); q++)
				{
					out_idx_on_distance_map_yx[k].push_back(indices[q]);
				}
			}
		}
	}

	if (in_mode == 1)
	{
		for (k = 0; k < num_target_voxel; k++)
		{
			if (Find_Candidate_of_Corresponds_In_Y_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[k],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				flag = true;
				for (q = 0; q < indices.size(); q++)
				{
					out_idx_on_distance_map_yx[k].push_back(indices[q]);
				}
			}
		}
	}

	return true;
}
//*************************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Update_Correspondances_Using_Slices(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<double>> &out_distance_map)
//*************************************************************************************
{
	std::vector<int> indices;
	int k, q, num_target_voxel, num_reference_voxel;

	num_reference_voxel = in_reference_texture_on_code.size();
	num_target_voxel = in_target_texture_on_code.size();

	if (num_target_voxel != out_distance_map.size())
	{
		return false;
	}

	// Set Slice distance -> 0
#define omp parallel
	{
#define omp for private(indices) private(q)
		for (k = 0; k < num_target_voxel; k++)
		{
			if (Find_Candidate_of_Corresponds_In_X_Slice(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
				in_target_texture_on_code[k],//int in_index_of_target_voxel,
				indices))//std::vector<int> &out_mininum_indices);
			{
				for (q = 0; q < indices.size(); q++)
				{
					out_distance_map[k][indices[q]] = 0.0;
				}
			}
		}
	}

	return true;
}
//*************************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Extract_Sub_Distance_Maps(
	std::vector<std::vector<bool>> &in_cornerstone_map,
	std::vector<std::vector<int>> &out_set_of_left_upper_xy,
	std::vector<std::vector<int>> &out_set_of_right_lower_xy)
//************************************************************************
{
	/***************************************************************************************************/
	// sub block 의 (0,0)은 그전 sub block의 대각선 방향.
	// sub block의 (0,0)을 찾은 후, sub block의 마지막 (n,m)을 찾는 rule은 다음과 같다. 
	// /************************************************************************************************/
	// 1-1. Distance map에서 sub block의 첫번째 column를 찾은 후, 
	//		그 다음 column에 0이 없다면, 0을 포함하는 column을 찾을 때까지, column을 이동시킨다. 
	// 1-2. 0을 포함하는 column을 찾은 후, (0,0)의 row방향 보다 아래 방향으로 0이 끝나는 지점을 찾는다. 
	// 1-3. 0이 끝나는 지점이 그 다음 0을 포함하는 column의 시작점 보다 아래면 cut 하여 
	//      그 부분을 sub block이 끝나는 지점으로 정한다. 
	// 1-4. 만약 0을 포함하는 column을 찾지 못하고 끝났다면, 그걸로 그냥 sub block을 설정. 
	// /************************************************************************************************/
	// 2-1. Distance map에서 sub block의 첫번째 column를 찾은 후, 그 다음 column에 0이 있다면, 
	//      0이 없는 column이 나타날때까지 계속 찾아감. 
	// 2-2. 0이 없는 column이 나타났다면, 다시 0이 있는 column을 찾은 후, 끝점과 시작점을 비교하여, 
	//		sub block이 끝나는 부분을 적절하게 잘라냄. 
	// 2-3. 만약, 0이 없는 column이 계속 나타나지 않고 끝났다면, 끝나는 부분까지 sub block을 설정하고 끝냄. 
	/***************************************************************************************************/
	std::vector<std::vector<double>> sub_block_matrix;
	std::vector<double> vector;
	std::vector<int> lu_xy, rl_xy;
	int j, i, hh, ww;
	int xx, yy, sz, current_sz;

	hh = in_cornerstone_map.size();
	ww = in_cornerstone_map[0].size();

	lu_xy = std::vector<int>(2);
	rl_xy = std::vector<int>(2);
	lu_xy[0] = lu_xy[1] = 0;
	out_set_of_left_upper_xy  = std::vector<std::vector<int>>();
	out_set_of_right_lower_xy = std::vector<std::vector<int>>();
	while (1)
	{
		Find_End_Point_of_Sub_Block_Matrix(
			in_cornerstone_map,//std::vector<std::vector<bool>> &in_cornerstone_map,
			lu_xy,//std::vector<int> &in_left_upper_xy,
			rl_xy);//std::vector<int> &out_right_lower_xy);

		out_set_of_left_upper_xy.push_back(lu_xy);
		out_set_of_right_lower_xy.push_back(rl_xy);

		// Set Next sub block start point. 
		// -> 다음 column을 확인하여, 면밀하게 update할 필요가 있음. 
		if (((rl_xy[0]) < (ww - 1)) &&
			((rl_xy[1]) < (hh - 1)))
		{
			if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == true) &&
				(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == false))
			{
				lu_xy[0] = rl_xy[0];
				lu_xy[1] = rl_xy[1] + 1;
			}
			else if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == true) &&
				(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == true))
			{
				lu_xy[0] = rl_xy[0];
				lu_xy[1] = rl_xy[1] + 1;
			}
			else if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == false) &&
				(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == true))
			{
				lu_xy[0] = rl_xy[0] + 1;
				lu_xy[1] = rl_xy[1] + 1;
			}
			else if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == false) &&
				(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == false))
			{
				lu_xy[0] = rl_xy[0] + 1;
				lu_xy[1] = rl_xy[1] + 1;
			}
			else
			{
				Kv_Printf("Error : Invalid Value In points");
				exit(0);
			}
		}
		else if((rl_xy[0] == (ww-1)) && (rl_xy[1] != (hh - 1)))
		{
			lu_xy[0] = rl_xy[0];
			lu_xy[1] = rl_xy[1]+1;
		}
		else if ((rl_xy[0] != (ww - 1)) && (rl_xy[1] == (hh - 1)))
		{
			lu_xy[0] = rl_xy[0]+1;
			lu_xy[1] = rl_xy[1];
		}
		else
		{
			lu_xy[0] = rl_xy[0] + 1;
			lu_xy[1] = rl_xy[1] + 1;
		}

		// Old Ver
		//if (((rl_xy[0]) < (ww - 1)) &&
		//	((rl_xy[1]) < (hh - 1)))
		//{
		//	if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == true) &&
		//		(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == false))
		//	{
		//		lu_xy[0] = rl_xy[0];
		//		lu_xy[1] = rl_xy[1] + 1;
		//	}
		//	else if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == true) &&
		//		(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == true))
		//	{
		//		lu_xy[0] = rl_xy[0];
		//		lu_xy[1] = rl_xy[1] + 1;
		//	}
		//	else if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == false) &&
		//		(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == true))
		//	{
		//		lu_xy[0] = rl_xy[0] + 1;
		//		lu_xy[1] = rl_xy[1] + 1;
		//	}
		//	else if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]] == false) &&
		//		(in_cornerstone_map[rl_xy[1] + 1][rl_xy[0] + 1] == false))
		//	{
		//		lu_xy[0] = rl_xy[0] + 1;
		//		lu_xy[1] = rl_xy[1] + 1;
		//	}
		//	else
		//	{
		//		Kv_Printf("Error : Invalid Value In points");
		//		exit(0);
		//	}
		//}
		//else
		//{
		//	lu_xy[0] = rl_xy[0] + 1;
		//	lu_xy[1] = rl_xy[1] + 1;
		//}

		// Check break condition. 
		if ((lu_xy[0] > ww - 1) || (lu_xy[1] > hh - 1))
		{
			break;
		}
	}


	std::vector<std::vector<int>> merge_luxy;
	std::vector<std::vector<int>> merge_rlxy;

	merge_luxy = std::vector<std::vector<int>>();
	merge_rlxy = std::vector<std::vector<int>>();

	sz = out_set_of_left_upper_xy.size();
	
	merge_luxy.push_back(out_set_of_left_upper_xy[0]);
	merge_rlxy.push_back(out_set_of_right_lower_xy[0]);

	bool flag;
	for (j = 1; j < sz; j++)
	{
		current_sz = merge_luxy.size();
		flag = false;
		for (i = 0; i < current_sz; i++)
		{
			if (out_set_of_left_upper_xy[j][0] == merge_luxy[i][0])
			{
				flag = true;
				merge_rlxy[i][0] = out_set_of_right_lower_xy[j][0];
				merge_rlxy[i][1] = out_set_of_right_lower_xy[j][1];
			}
		}
		if (flag == false)
		{
			merge_luxy.push_back(out_set_of_left_upper_xy[j]);
			merge_rlxy.push_back(out_set_of_right_lower_xy[j]);
			flag = false;
		}
	}


	out_set_of_left_upper_xy  = merge_luxy;
	out_set_of_right_lower_xy = merge_rlxy;




	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Find_End_Point_of_Sub_Block_Matrix(
	std::vector<std::vector<bool>> &in_cornerstone_map,
	std::vector<int> &in_left_upper_xy,
	std::vector<int> &out_right_lower_xy)
//************************************************************************
{
	std::vector<bool> vector;
	int xx, yy, nxx, nyy, ww, hh;
	bool start_column, exit_flag = false;

	xx = in_left_upper_xy[0];
	yy = in_left_upper_xy[1];

	hh = in_cornerstone_map.size();
	ww = in_cornerstone_map[0].size();

	// Out range -> return. 
	if ((xx < 0) || (xx >= ww - 1))
	{
		out_right_lower_xy[0] = ww - 1;
		out_right_lower_xy[1] = hh - 1;

		return true;
	}

	//*************** Main routine. ************************************//
	// Initialize
	if (Is_True_Column(
		in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
		xx,//int in_column_index,
		yy,//int in_row_index,
		vector))//std::vector<T> &out_vector)
	{
		start_column = true;
	}
	else
	{
		start_column = false;
	}

	xx++;
	while (xx < ww - 1)
	{
		//printf("[Is_True_Column_Full]-[0] %d / %d\n",xx, in_cornerstone_map[0].size());
		if (Is_True_Column_Full(
			in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
			xx,//int in_column_index,
			//in_left_upper_xy[1],//int in_row_index,
			vector))//std::vector<T> &out_vector)
		{
			if (start_column == false) // 계속 비어있다가. true인 column이 발견. 
			{
				// 현재 column에 true값의 위치를 살펴야함. 
				// 이를 위해 현재(x) column의 다음(x+1) column을 가져와서. true값의 y값이 reverse 되는지 확인한다. 
				// 만약 reverse 되면, reverse가 되지 않는 column이 나올때 까지 또는 다음 비어있는 column이 나올때까지 확인. 
				// reverse가 되지 않는다면, (x+1) column의 끝부분을 end point로 정함. 
				// yy 값을 정해줘야 함. 

				if (Find_Optimal_Position(
					in_cornerstone_map,//std::vector<std::vector<bool>> &in_cornerstone_map,
					xx,//int in_column_index,
					yy,//int in_row_index,
					&vector,//std::vector<bool> *in_vector_on_column_index_or_NULL,
					nxx,//int &out_xx_if_false,
					nyy))//int &out_yy_if_false)
				{
					// Position을 찾는데 성공 !
					//printf("[1-1]\n");
					out_right_lower_xy[0] = nxx;
					out_right_lower_xy[1] = nyy;
					return true;

				}
				// Position을 찾는데 실패 -> 그냥 비어있는 vector로 간주하고 계속 돌림. 
			}
			else // 처음 column이 true 이면서, 다음 column들이 true인 경우임. 
			{
				if (Is_Reverse_Path(
					in_cornerstone_map,//std::vector<std::vector<bool>> &in_cornerstone_map,
					xx,//int in_column_index,
					yy,//int in_row_index,
					&vector,//std::vector<bool> *in_vector_on_column_index_or_NULL,
					nxx,//int &out_xx_if_false,
					nyy))//int &out_yy_if_false)
				{
					// 역류해버린 case. 이경우는 그냥 비어 있는걸로 간주하고, xx, yy를 결정해주고, return 시킴. 
					//printf("[2-1]\n");

					//out_right_lower_xy[0] = nxx;
					//out_right_lower_xy[1] = nyy;

					out_right_lower_xy[0] = xx - 1;
					out_right_lower_xy[1] = yy;

					return true;
				}
				else
				{
					// 역류하지 않은 case. 이 경우는 계속 searching 해 나감. 
					yy = nyy;
				}
			}
		}
		else // Blank 
		{
			if (start_column == true) // 처음 column이 true 이면서, 비어있는 column을 발견했을때임. 
			{
				// Block을 완성했기 때문에, 탈출. 
				out_right_lower_xy[0] = xx - 1;
				out_right_lower_xy[1] = yy;
				return true;
			}

			// 처음 column이 false이면서, 비어있는 column을 발견하면, 아무것도 할일 없이 다음 위치로 이동. 
		}

		if (exit_flag == true)
		{
			break;
		}
		xx++;
	}

	if (xx == ww - 1)
	{
		yy = hh - 1;
	}

	out_right_lower_xy[0] = xx;
	out_right_lower_xy[1] = yy;

	return false;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::linear_project_point(
	int in_index_x,
	std::vector<std::vector<int>> &in_set_of_left_upper_xy,
	std::vector<std::vector<int>> &in_set_of_right_lower_xy,
	std::vector<float> &out_y)
//************************************************************************
{
	float slope, val;
	int num_block, block_index, k;

	if (in_set_of_left_upper_xy.size() != in_set_of_right_lower_xy.size())
	{
		return false;
	}

	num_block = in_set_of_left_upper_xy.size();
	block_index = -1;
	for (k = 0; k < num_block; k++)
	{
		if ((in_set_of_left_upper_xy[k][0] <= in_index_x) &&
			(in_index_x <= in_set_of_right_lower_xy[k][0]))
		{
			block_index = k;
			break;
		}
	}

	if (block_index == -1)
	{
		printf("INPUT X : %d\n", in_index_x);
		for (k = 0; k < num_block; k++)
		{
			printf("%dth block / %d : (%d %d) <-> (%d %d)\n",k,num_block,
				in_set_of_left_upper_xy[k][0], in_set_of_left_upper_xy[k][1],
				in_set_of_right_lower_xy[k][0], in_set_of_right_lower_xy[k][1]);
		}
		

		return false;
	}

	out_y = std::vector<float>();
	if ((in_set_of_left_upper_xy[block_index][0] != in_set_of_right_lower_xy[block_index][0]) &&
		(in_set_of_left_upper_xy[block_index][1] == in_set_of_right_lower_xy[block_index][1]))
	{
		out_y.push_back(in_set_of_left_upper_xy[block_index][1]);
	}
	else if ((in_set_of_left_upper_xy[block_index][0] == in_set_of_right_lower_xy[block_index][0]) &&
			 (in_set_of_left_upper_xy[block_index][1] != in_set_of_right_lower_xy[block_index][1]))
	{
		for (k = in_set_of_left_upper_xy[block_index][1]; k <= in_set_of_right_lower_xy[block_index][1]; k++)
		{
			out_y.push_back(k);
		}
	}
	else if ((in_set_of_left_upper_xy[block_index][0] == in_set_of_right_lower_xy[block_index][0]) &&
			 (in_set_of_left_upper_xy[block_index][1] == in_set_of_right_lower_xy[block_index][1]))
	{
		out_y.push_back(in_set_of_left_upper_xy[block_index][1]);
	}
	else // 보통의 직선. 
	{
		slope = (float)(in_set_of_right_lower_xy[block_index][1] - in_set_of_left_upper_xy[block_index][1]) /
				(float)(in_set_of_right_lower_xy[block_index][0] - in_set_of_left_upper_xy[block_index][0]);

		val = slope * (float)(in_index_x - in_set_of_left_upper_xy[block_index][0]) + (float)in_set_of_left_upper_xy[block_index][1];
		out_y.push_back(val);
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::linear_project_point(
	int in_x1, int in_x2,
	int in_y1, int in_y2,
	std::vector<float> &out_y)
//************************************************************************
{
	float slope, val;
	int k;

	out_y = std::vector<float>();

	if ((in_x1!= in_x2) &&
		(in_y1 == in_y2))
	{
		for (k = in_x1; k <= in_x2; k++)
		{
			out_y.push_back(in_y1);
		}
	}
	else if ((in_x1 == in_x2) &&
			 (in_y1 != in_y2))
	{
		for (k = in_y1; k <= in_y2; k++)
		{
			out_y.push_back(k);
		}
	}
	else if ((in_x1 == in_x2) &&
			 (in_y1 == in_y2))
	{
		out_y.push_back(in_x1);
	}
	else // 보통의 직선. 
	{
		slope = (float)(in_y2 - in_y1) /
			(float)(in_x2 - in_x1);
		
		for (k = in_x1; k <= in_x2; k++)
		{
			val = slope * (float)(k - in_x1) + (float)in_y1;
			out_y.push_back(val);
		}
	}
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::linear_project_point(
	int in_x1, int in_x2,
	int in_y1, int in_y2,
	std::vector<std::vector<float>> &out_table_xy)
//************************************************************************
{
	std::vector<float> tmp_mem = std::vector<float>(2);
	float slope, val;
	int k;

	out_table_xy = std::vector<std::vector<float>>();

	if ((in_x1 != in_x2) &&
		(in_y1 == in_y2))
	{
		for (k = in_x1; k <= in_x2; k++)
		{
			tmp_mem[0] = k;
			tmp_mem[1] = in_y1;
			out_table_xy.push_back(tmp_mem);
		}
	}
	else if ((in_x1 == in_x2) &&
			 (in_y1 != in_y2))
	{
		for (k = in_y1; k <= in_y2; k++)
		{
			tmp_mem[0] = in_x1;
			tmp_mem[1] = k;
			out_table_xy.push_back(tmp_mem);
		}
	}
	else if ((in_x1 == in_x2) &&
		(in_y1 == in_y2))
	{
		tmp_mem[0] = in_x1;
		tmp_mem[1] = in_y1;
		out_table_xy.push_back(tmp_mem);
	}
	else // 보통의 직선. 
	{
		slope = (float)(in_y2 - in_y1) /
			(float)(in_x2 - in_x1);

		for (k = in_x1; k <= in_x2; k++)
		{
			val = slope * (float)(k - in_x1) + (float)in_y1;
			tmp_mem[0] = k;
			tmp_mem[1] = val;
			out_table_xy.push_back(tmp_mem);
		}
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::linear_project_point(
	int in_x1, int in_x2,
	int in_y1, int in_y2,
	std::vector<std::vector<float>> &out_table_xy,
	std::vector<std::vector<float>> &out_cumulative_table_yx) // y값에 어떤 x들이 들어있는가. 
//************************************************************************
{
	std::vector<float> tmp_mem = std::vector<float>(2);
	std::vector<float> cum_mem = std::vector<float>();
	float slope, val;
	int k, l;

	out_table_xy            = std::vector<std::vector<float>>();
	//out_cumulative_table_yx = std::vector<std::vector<float>>();

	if ((in_x1 != in_x2) &&
		(in_y1 == in_y2))
	{
		for (k = in_x1; k <= in_x2; k++)
		{
			tmp_mem[0] = k;
			tmp_mem[1] = in_y1;
			out_table_xy.push_back(tmp_mem);
			out_cumulative_table_yx[in_y1].push_back(k);

			// Update
			//if (out_cumulative_table_yx.size() == 0)
			//{
			//	out_cumulative_table_yx.push_back(cum_mem);
			//	out_cumulative_table_yx[0].push_back(in_y1);
			//	out_cumulative_table_yx[0].push_back(k);
			//}
			//else
			//{
			//	for (l = 0; l < out_cumulative_table_yx.size(); l++)
			//	{
			//		if (out_cumulative_table_yx[l][0] == in_y1)
			//		{
			//			out_cumulative_table_yx[l].push_back(k);
			//		}
			//		else
			//		{
			//			out_cumulative_table_yx.push_back(cum_mem);
			//			out_cumulative_table_yx[0].push_back(in_y1);
			//			out_cumulative_table_yx[0].push_back(k);
			//		}
			//	}
			//}

		}
	}
	else if ((in_x1 == in_x2) && (in_y1 != in_y2))
	{
		for (k = in_y1; k <= in_y2; k++)
		{
			tmp_mem[0] = in_x1;
			tmp_mem[1] = k;
			out_table_xy.push_back(tmp_mem);
			out_cumulative_table_yx[k].push_back(in_x1);
		}

		// Update
		//if (out_cumulative_table_yx.size() == 0)
		//{
		//	out_cumulative_table_yx.push_back(cum_mem);
		//	out_cumulative_table_yx[0].push_back(k);
		//	out_cumulative_table_yx[0].push_back(in_x1);
		//}
		//else
		//{
		//	for (l = 0; l < out_cumulative_table_yx.size(); l++)
		//	{
		//		if (out_cumulative_table_yx[l][0] == k)
		//		{
		//			out_cumulative_table_yx[l].push_back(in_x1);
		//		}
		//		else
		//		{
		//			out_cumulative_table_yx.push_back(cum_mem);
		//			out_cumulative_table_yx[0].push_back(k);
		//			out_cumulative_table_yx[0].push_back(in_x1);
		//		}
		//	}
		//}



	}
	else if ((in_x1 == in_x2) && (in_y1 == in_y2))
	{
		tmp_mem[0] = in_x1;
		tmp_mem[1] = in_y1;
		out_table_xy.push_back(tmp_mem);
		out_cumulative_table_yx[in_y1].push_back(in_x1);

		// Update
		//out_cumulative_table_yx.push_back(cum_mem);
		//out_cumulative_table_yx[0].push_back(in_y1);
		//out_cumulative_table_yx[0].push_back(in_x1);
	}
	else // 보통의 직선. 
	{
		slope = (float)(in_y2 - in_y1) / (float)(in_x2 - in_x1);

		for (k = in_x1; k <= in_x2; k++)
		{
			val = slope * (float)(k - in_x1) + (float)in_y1;
			tmp_mem[0] = k;
			tmp_mem[1] = val;
			out_table_xy.push_back(tmp_mem);
			out_cumulative_table_yx[(int)round(val)].push_back(k);

			//if (out_cumulative_table_yx.size() == 0)
			//{
			//	out_cumulative_table_yx.push_back(cum_mem);
			//	out_cumulative_table_yx[0].push_back(k);
			//	out_cumulative_table_yx[0].push_back((int)round(val));
			//}
			//else
			//{
			//	for (l = 0; l < out_cumulative_table_yx.size(); l++)
			//	{
			//		if (out_cumulative_table_yx[l][0] == k)
			//		{
			//			out_cumulative_table_yx[l].push_back((int)round(val));
			//		}
			//		else
			//		{
			//			out_cumulative_table_yx.push_back(cum_mem);
			//			out_cumulative_table_yx[0].push_back(k);
			//			out_cumulative_table_yx[0].push_back((int)round(val));
			//		}
			//	}
			//}


		}
	}



	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Find_Optimal_Position(
	std::vector<std::vector<bool>> &in_cornerstone_map,
	int in_column_index,
	int in_row_index,
	std::vector<bool> *in_vector_on_column_index_or_NULL,
	int &out_xx_if_false,
	int &out_yy_if_false)
//************************************************************************
{
	// 지금 문제 : return false로 가야할 것이 return true가 되고 있음......
	std::vector<std::vector<bool>> block_cornerstone_map;
	std::vector<int> a, b;
	std::vector<bool> vec;
	int dummy, k, hh, ww, xx, yy, searching_xx;
	short z1;
	bool escape_flag = false;

	hh = in_cornerstone_map.size();
	ww = in_cornerstone_map[0].size();

	if (in_vector_on_column_index_or_NULL == NULL)
	{
		//printf("[Is_True_Column_Full]-[1]\n");
		Is_True_Column_Full(
			in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
			in_column_index,//int in_column_index,
			vec);//std::vector<T> &out_vector); <- true인 column vector. 
	}

	xx = in_column_index;
	yy = in_row_index;

	a = std::vector<int>(2);
	b = std::vector<int>(2);

	//escape_flag = false;
	if (Is_True_Column_Full(
		in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
		xx,//int in_column_index,
		vec))//std::vector<T> &out_vector); <- true인 column vector. 
	{
		if (!check_bool_vector(
			vec,//std::vector<bool> &in_vector,
			in_row_index,//int in_criteria_index)
			z1))
		{
			return false; //빈 vector로 간주하겠다는 의미. 
		}

		// 좋아.. 일단 기준점 보다는 아래에 위치함. 그러면 이 놈들이 reverse 하는 놈들인지 확인을 해야하는데....
		// 문제가 그전 column들이 false값들을 가지고 있었기 때문에, 위쪽에 있는 matrix의 element들이 false를 가지고 있지 않다는 것이 보장이 안댐. 
		// 어쩔 수 없이 적절한 guard band를 설정하고 count 해야할듯 한데.... 
		searching_xx = xx;
		while (1)
		{
			searching_xx++;
			//printf("[Is_True_Column_Full]-[3] %d / %d\n",searching_xx, in_cornerstone_map[0].size());

			if (Is_True_Column_Full(
				in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
				searching_xx,//int in_column_index,
				vec))//std::vector<T> &out_vector); <- true인 column vector. 
			{
				a[0] = min(searching_xx + 1, ww - 1); a[1] = in_row_index;
				b[0] = min(searching_xx + 8, ww - 1); b[1] = z1;

				//printf("a = (%d %d) b = (%d %d) criteria = %d\n", a[0], a[1], b[0], b[1], in_row_index);
				//Kv_Printf("!!");
				get_block_matrix(
					in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
					a,//std::vector<int> &in_left_upper_xy,
					b,//std::vector<int> &in_right_lower_xy,
					block_cornerstone_map);//std::vector<std::vector<T>> &out_block_matrix);

				if (Is_matrix_include_true(
					block_cornerstone_map))//std::vector<std::vector<bool>> &in_matrix)
				{
					// True를 포함하고 있기 때문에, 초기 설정했던 xx는 오류. 
					// searching_xx를 마지막 지점으로 설정하고, 상위 while문이 돌아가야함. ???
					// -> 그냥 비어있는 vector로 간주하면 간단히 끝나는거 아닌가? 

					//plot_data_for_debug(block_cornerstone_map);

					return false;
				}
				else
				{
					// 위에서 찾은 xx와 z1은 올바른 bounding box. 종료하자. 
					out_xx_if_false = xx;
					out_yy_if_false = z1;
					return true;
				}
			}


			//if (!Is_True_Column_Full(
			//	in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
			//	searching_xx,//int in_column_index,
			//	vec))//std::vector<T> &out_vector); <- true인 column vector. 
			//{
			//	searching_xx++;
			//}
			//else
			//{
			//	a[0] = min(searching_xx + 1, ww-1); a[1] = in_row_index;
			//	b[0] = min(searching_xx + 8, ww-1); b[1] = z1;
			//	//printf("a = (%d %d) b = (%d %d) criteria = %d\n", a[0], a[1], b[0], b[1], in_row_index);
			//	//Kv_Printf("!!");
			//	get_block_matrix(
			//		in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
			//		a,//std::vector<int> &in_left_upper_xy,
			//		b,//std::vector<int> &in_right_lower_xy,
			//		block_cornerstone_map);//std::vector<std::vector<T>> &out_block_matrix);
			//	if (Is_matrix_include_true(
			//		block_cornerstone_map))//std::vector<std::vector<bool>> &in_matrix)
			//	{
			//		// True를 포함하고 있기 때문에, 초기 설정했던 xx는 오류. 
			//		// searching_xx를 마지막 지점으로 설정하고, 상위 while문이 돌아가야함. ???
			//		// -> 그냥 비어있는 vector로 간주하면 간단히 끝나는거 아닌가? 
			//		//plot_data_for_debug(block_cornerstone_map);
			//		
			//		return false;
			//		//xx = searching_xx;
			//		//escape_flag = true;
			//		//break;
			//	}
			//	else
			//	{
			//		// 위에서 찾은 xx와 z1은 올바른 bounding box. 종료하자. 
			//		out_xx_if_false = xx;
			//		out_yy_if_false = z1;
			//		return true;
			//	}
			//}

			if (searching_xx == (ww - 1))
			{
				out_xx_if_false = ww - 1;
				out_yy_if_false = hh - 1;
				return true;
			}
		}
	}
	else
	{
		//xx++;
		Kv_Printf("Error : Input vector does not include 'true'.[Find_Optimal_Position]\n");
		exit(0);
		return false;
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::Is_Reverse_Path( // 전제가 (column index - 1)와 row index 의 위치는 true. 
	std::vector<std::vector<bool>> &in_cornerstone_map,
	int in_column_index,
	int in_row_index,
	std::vector<bool> *in_vector_on_column_index_or_NULL,
	int &out_xx_if_false,
	int &out_yy_if_false)
//************************************************************************
{
	std::vector<std::vector<bool>> block_cornerstone_map;
	std::vector<int> a, b;
	std::vector<bool> vec;
	int dummy, np, k, z, hh, ww, xx;
	short *p, zo, z1;
	bool flag = false;

	if (in_vector_on_column_index_or_NULL == NULL)
	{
		//printf("[Is_True_Column_Full]-[-1]\n");
		Is_True_Column_Full(
			in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
			in_column_index,//int in_column_index,
			*in_vector_on_column_index_or_NULL);//std::vector<T> &out_vector);
	}

	if (!check_bool_vector(
		*in_vector_on_column_index_or_NULL,//std::vector<bool> &in_vector,
		in_row_index,//int in_criteria_index)
		z1))
	{
		out_xx_if_false = in_column_index--;
		out_yy_if_false = in_row_index + 1;
		return true;
	}

	hh = in_cornerstone_map.size();
	ww = in_cornerstone_map[0].size();

	a.push_back(in_column_index + 1); a.push_back(0);
	b.push_back(ww - 1);              b.push_back(hh - 1);

	get_block_matrix(
		in_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
		a,//std::vector<int> &in_left_upper_xy,
		b,//std::vector<int> &in_right_lower_xy,
		block_cornerstone_map);//std::vector<std::vector<T>> &out_block_matrix);

	for (z = in_row_index; z <= z1; z++)
	{
		if (Is_True_Row_Full(
			block_cornerstone_map,//std::vector<std::vector<T>> &in_matrix,
			z, //int in_row_index,
			vec)) //std::vector<T> &out_vector,
		{
			out_xx_if_false = in_column_index--;
			out_yy_if_false = in_row_index + 1;
			return true;
		}
	}

	out_xx_if_false = in_column_index;
	out_yy_if_false = z1;

	return false;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::check_bool_vector(
	std::vector<bool> &in_vector,
	int in_criteria_index,
	short &out_z)
//************************************************************************
{
	CKvXrunset1dShort xrun1d;
	CKvXvectorShort xvec_short;
	CKvVectorBool vec_bool;
	int k, z, np;
	short *p, zo, z1;
	short target_z = SHORT_MAX;

	bool *p_vec = vec_bool.c_Create(in_vector.size());

	if (in_vector.size() == 1)
	{
		return in_vector[0];
	}

	for (k = 0; k < in_vector.size(); k++)
	{
		p_vec[k] = in_vector[k];
	}

	xrun1d.im_Import(vec_bool);
	xrun1d.ex_Export(xvec_short, NULL);
	xvec_short.z_gii_Get_Informations_Internal(NULL, &p, &np, NULL);

	for (z = 0; z < np;)
	{
		zo = p[z]; z++;
		z1 = p[z]; z++;
		if (in_criteria_index > z1)
		{
			continue;
		}
		else
		{
			if (target_z > z1)
			{
				target_z = z1;
			}
		}
	}

	out_z = target_z;

	if (target_z == SHORT_MAX)
	{
		return false;
	}



	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::save_data(
	CKvString in_str,
	std::vector<std::vector<int>> &in_data)
//************************************************************************
{
	FILE *fp = NULL;
	//CKvString str;
	int ww, hh, j, i;

	hh = in_data.size();
	ww = in_data[0].size();

	//str = "index.txt";

	fopen_s(&fp, (char *)in_str, "w");
	if (fp == NULL)
	{
		return false;
	}

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			fprintf_s(fp, "%d ", in_data[j][i]);
		}
		fprintf_s(fp, "\n");
	}

	fclose(fp);

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_PWEI::save_data(
	std::vector<std::vector<int>> &in_data)
//************************************************************************
{
	FILE *fp = NULL;
	CKvString str;
	int ww, hh, j, i;

	hh = in_data.size();
	ww = in_data[0].size();

	str = "index.txt";

	fopen_s(&fp, (char *)str, "w");
	if (fp == NULL)
	{
		return false;
	}

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			fprintf_s(fp, "%d ", in_data[j][i]);
		}
		fprintf_s(fp, "\n");
	}
	
	fclose(fp);

	return true;

}
//************************************************************************
