#include "stdafx.h"
#include "VCL_Voxel_Color_Sequence_Aligner_DTW.h"

//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_DTW::VCL_Voxel_Color_Sequence_Aligner_DTW()
//************************************************************************
{

}
//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_DTW::~VCL_Voxel_Color_Sequence_Aligner_DTW()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Align_Texture_on_Codes_Using_DTW(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	std::vector<std::vector<std::vector<std::vector<int>>>> warping_index;
	std::vector<int> modified_reference_warping_index, modified_target_warping_index;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v; CKvVoxel *p_setv;
	CKvVectorInt *p_vec;
	int num_blob, num_toc, max_length, max_idx, current_size, m, n;
	int *p_v;
	int sz, max_idx_bw;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec    = out_offset->c_Create(num_blob);

	warping_index = std::vector<std::vector<std::vector<std::vector<int>>>>(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		//printf("m=%d/%d) num_toc=%d\n", m,num_blob, num_toc);
		if (num_toc == 0 || num_toc == 1) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		max_length = -1;
		for (n = 0; n < num_toc; n++)
		{
			current_size = io_segmented_texture_on_code[m][n].size();
			if (current_size > max_length)
			{
				max_length = current_size;
				max_idx = n;
			}
		}
		
		warping_index[m] = std::vector<std::vector<std::vector<int>>>(num_toc);
		for (n = max_idx - 1; n > -1; n--)
		{
			Align_Two_Texture_on_Codes_Using_DTW(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				warping_index[m][n],//std::vector<std::vector<int>> &out_warping_index,
				max_length,//int in_max_length,
				p_v[n]);//int &out_offset);
		}
		for (n = max_idx + 1; n < num_toc; n++)
		{
			Align_Two_Texture_on_Codes_Using_DTW(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				warping_index[m][n],//std::vector<std::vector<int>> &out_warping_index,
				max_length,//int in_max_length,
				p_v[n]);//int &out_offset);
		}

		std::vector<int> tmp = io_segmented_texture_on_code[m][max_idx];

		if (max_idx != 0)
		{
			max_idx_bw = max_idx - 1;
		}
		else
		{
			max_idx_bw = max_idx + 1;
		}
		sz = warping_index[m][max_idx_bw][0].size();

		io_segmented_texture_on_code[m][max_idx] = std::vector<int>(sz,-1);
		for (n = 0; n < sz; n++)
		{
			io_segmented_texture_on_code[m][max_idx][n] = tmp[warping_index[m][max_idx_bw][0][n]];
		}


		modified_reference_warping_index = warping_index[m][max_idx_bw][0];
		for (n = max_idx - 1; n > -1; n--)
		{
			Modify_Texture_on_Codes(
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				warping_index[m][n][0],//std::vector<int> &in_reference_warping_index,
				warping_index[m][n][1],//std::vector<int> &in_target_warping_index,
				modified_reference_warping_index,//std::vector<int> &in_modified_reference_warping_index,
				modified_target_warping_index);//std::vector<int> &out_modified_target_warping_index);
			modified_reference_warping_index = modified_target_warping_index;
		}

		modified_reference_warping_index = warping_index[m][max_idx_bw][0];
		for (n = max_idx + 1; n < num_toc; n++)
		{
			Modify_Texture_on_Codes(
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				warping_index[m][n][0],//std::vector<int> &in_reference_warping_index,
				warping_index[m][n][1],//std::vector<int> &in_target_warping_index,
				modified_reference_warping_index,//std::vector<int> &in_modified_reference_warping_index,
				modified_target_warping_index);//std::vector<int> &out_modified_target_warping_index);

			modified_reference_warping_index = modified_target_warping_index;
		}
	}



}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Align_Texture_on_Codes_Using_DTW2(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	std::vector<std::vector<std::vector<std::vector<int>>>> warping_index;
	std::vector<int> modified_reference_warping_index, modified_target_warping_index;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v; CKvVoxel *p_setv;
	CKvVectorInt *p_vec;
	int num_blob, num_toc, max_length, max_idx, current_size, m, n;
	int *p_v;
	int sz, max_idx_bw;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);

	warping_index = std::vector<std::vector<std::vector<std::vector<int>>>>(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0 || num_toc == 1) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);

		warping_index[m] = std::vector<std::vector<std::vector<int>>>(num_toc-1);
		for (n = 0; n < num_toc-1; n++)
		{
			Align_Two_Texture_on_Codes_Using_DTW(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n+1],//std::vector<int> &io_target_texture_on_code,
				warping_index[m][n],//std::vector<std::vector<int>> &out_warping_index,
				max_length,//int in_max_length,
				p_v[n]);//int &out_offset);

			//printf("n=%d/%d)\n", n, num_toc - 1);
			//printf("refer sz : %d // target sz : %d\n",
			//	io_segmented_texture_on_code[m][n].size(),
			//	io_segmented_texture_on_code[m][n + 1].size());
			//printf("sz 0 : %d sz 1 : %d\n", 
			//	warping_index[m][n][0].size(),
			//	warping_index[m][n][1].size());
			//for (int k = 0; k < warping_index[m][n][0].size() ; k++)
			//{
			//	printf("%d %d\n", warping_index[m][n][0][k], warping_index[m][n][1][k]);
			//}
			//Kv_Printf("~~~~~");
		}

		Warping_Texture_on_Codes(
			io_segmented_texture_on_code[m],//std::vector<std::vector<int>> &io_texture_on_codes,
			warping_index[m]);//std::vector<std::vector<std::vector<int>>> &in_warping_index)
	}
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Align_Two_Texture_on_Codes_Using_DTW(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<int>> &out_warping_index,
	int &io_max_length,
	int &out_offset)
//************************************************************************
{
	std::vector<std::vector<double>> distance_map;
	std::vector<std::vector<bool>> cornerstone_map;
	std::vector<std::vector<std::vector<double>>> sub_distance_maps;
	std::vector<std::vector<int>> set_of_left_upper_xy;
	std::vector<std::vector<int>> warping_index;
	double score;
	int k, l, num_sub_blocks;

	// Set distance map.
	// voxels on X-Slice or Y-Slice? -> distance = 0 .
	Set_Distance_Map_Fast(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		in_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		distance_map,//std::vector<std::vector<double>> &out_distance_map)
		cornerstone_map);//std::vector<std::vector<bool>> &out_cornerstone_map);
	//Kv_Printf("~~~");

	//// Extract sub blocks. 
	Extract_Sub_Distance_Maps(
		distance_map,//std::vector<std::vector<double>> &in_distance_map,
		cornerstone_map,//std::vector<std::vector<bool>> &in_distance_map,
		sub_distance_maps,//std::vector<std::vector<std::vector<double>>> &out_sub_blocks);
		set_of_left_upper_xy);//std::vector<std::vector<int>> &out_set_of_left_upper_xy); 

	//// Do Dynamic Time Warping on each sub block.
	out_warping_index = std::vector<std::vector<int>>(2, std::vector<int>());
	num_sub_blocks = sub_distance_maps.size();

	for (k = 0; k < num_sub_blocks; k++)
	{
		dtw_Dynamic_Time_Warping2(
			sub_distance_maps[k],//std::vector<std::vector<double>> &in_distance_map,
			warping_index,//std::vector<int> &out_warping_index,
			NULL,//std::vector<std::vector<double>> *out_summation_distance_map_or_NULL,
			score);//double &out_score)

		// Add index for offset
		for (l = 0; l < warping_index[0].size(); l++)
		{
			warping_index[0][l] = warping_index[0][l] + set_of_left_upper_xy[k][0]; // X
			warping_index[1][l] = warping_index[1][l] + set_of_left_upper_xy[k][1]; // Y
		}

		// Update Warping index
		out_warping_index[0].insert(
			std::end(out_warping_index[0]), 
			std::begin(warping_index[0]), 
			std::end(warping_index[0]));

		out_warping_index[1].insert(
			std::end(out_warping_index[1]),
			std::begin(warping_index[1]),
			std::end(warping_index[1]));
	}
	out_offset = 0;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Set_Distance_Map(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<double>> &out_distance_map)
//************************************************************************
{
	std::vector<std::vector<double>> shift_distance_map;
	std::vector<int> cpy_ToC;
	std::vector<double> score_vector;
	int num_ref, num_target, idx, k;

	CKvStopWatch sw;


	num_ref = in_reference_texture_on_code.size();
	num_target = in_target_texture_on_code.size();

	// Find Distance Map. 
	/************************************************************************************/
	sw.c_Create(1);
	sw.r_Reset(0);
	out_distance_map = std::vector<std::vector<double>>(num_target);
	for (k = 0; k < num_target; k++)
	{
		out_distance_map[k] = std::vector<double>(num_ref, DBL_MAX);
	}
	printf("Init -> %lf\n", sw.get_Get_Elapsed_Time(0));


	sw.r_Reset(0);
	if (!Update_Distance_Map_Using_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		in_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		out_distance_map))//std::vector<std::vector<double>> &out_distance_map)
	{
		exit(0);
		return;
	}
	printf("Update_Distance_Map_Using_Correspondances_Using_Slices -> %lf\n", sw.get_Get_Elapsed_Time(0));
	/************************************************************************************/


	/************************************************************************************/
	sw.r_Reset(0);
	score_vector = std::vector<double>(num_target);
#define omp parallel
	{
#define omp for private(shift_distance_map)
		for (k = 0; k < num_target; k++)
		{
			Circular_Shift_Distance_Map(
				out_distance_map,//std::vector<std::vector<double>> &in_distance_map,
				shift_distance_map,//std::vector<std::vector<double>> &out_distance_map,
				k);//int in_circular_shift_factor) -> 4초

			score_vector[k] = find_corner_stone_points(
				shift_distance_map,//std::vector<std::vector<double>> &in_distance_map,
				5);//int in_mask_length) -> 6초 ? 
		}
	}

	Find_Max(score_vector, &idx);
	printf("Circular_Shift_Distance_Map -> %lf\n", sw.get_Get_Elapsed_Time(0));
	/************************************************************************************/


	sw.r_Reset(0);
	sv_Circular_Shift_Vector(idx, in_target_texture_on_code);
	out_distance_map = std::vector<std::vector<double>>(num_target);
	for (k = 0; k < num_target; k++)
	{
		out_distance_map[k] = std::vector<double>(num_ref, DBL_MAX);
	}
	printf("Init -> %lf\n", sw.get_Get_Elapsed_Time(0));

	if (!Update_Distance_Map_Using_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		in_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		out_distance_map))//std::vector<std::vector<double>> &out_distance_map)
	{
		exit(0);
		return;
	}
	printf("Update_Distance_Map_Using_Correspondances_Using_Slices -> %lf\n", sw.get_Get_Elapsed_Time(0));

	/***********************************************/
	CKvMatrix dist_map;
	int ww, hh;
	hh = out_distance_map.size();
	ww = out_distance_map[0].size();
	double **p_dm = dist_map.c_Create(hh, ww);
	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			if (out_distance_map[j][i] == DBL_MAX)
			{
				p_dm[j][i] = 0;
			}
			else
			{
				p_dm[j][i] = 255;
			}

		}
	}
	CKvScreen sc;
	sc.s_d_Display(1.0, 0.0, &dist_map);
	Kv_Printf("~~~~");
	/***********************************************/

	sw.r_Reset(0);
	if (!Update_Distance_Map_Using_Euclidean_Distance(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		in_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		out_distance_map))//std::vector<std::vector<double>> &out_distance_map)
	{
		exit(0);
		return;
	}
	printf("Update_Distance_Map_Using_Euclidean_Distance -> %lf\n", sw.get_Get_Elapsed_Time(0));
	/************************************************************************************/
	/************************************************************************************/


	Kv_Printf("!!!!!!");


}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Set_Distance_Map_Fast(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<double>> &out_distance_map,
	std::vector<std::vector<bool>> &out_cornerstone_map)
//************************************************************************
{
	std::vector<std::vector<int>> idx_on_dm_yx;
	std::vector<std::vector<int>> shift_distance_map;
	std::vector<int> cpy_ToC;
	std::vector<double> score_vector;
	int num_ref, num_target, idx, k, m;
	int mask_length, ww, hh;

	num_ref    = in_reference_texture_on_code.size();
	num_target = in_target_texture_on_code.size();

	// Find Distance Map. 
	/************************************************************************************/
	if (!Update_Distance_Map_Using_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		in_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		idx_on_dm_yx))//std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
	{
		exit(0);
		return;
	}

	int sz = idx_on_dm_yx.size();
	printf("==========================================\n");
	printf("==========================================\n");
	printf("==========================================\n");
	printf("===== refer =====\n");
	for (k = 0; k < in_reference_texture_on_code.size(); k++)
	{
		printf("%d ", in_reference_texture_on_code[k]);
	}
	printf("\n");
	printf("===== target =====\n");
	for (k = 0; k < in_target_texture_on_code.size(); k++)
	{
		printf("%d ", in_target_texture_on_code[k]);
	}
	printf("\n");
	printf("==========================================\n");
	printf("==========================================\n");
	for (k = 0; k < sz; k++)
	{
		for (m = 0; m < idx_on_dm_yx[k].size(); m++)
		{
			printf("y=%d x=%d\n", k, idx_on_dm_yx[k][m]);
		}
	}
	printf("==========================================\n");
	printf("==========================================\n");
	printf("==========================================\n");

	/************************************************************************************/

	/************************************************************************************/
	// Align Signal Ver1. 
//	score_vector = std::vector<double>(num_target);
//	hh = in_target_texture_on_code.size();
//	ww = in_reference_texture_on_code.size();
//	mask_length = ceil((double)min(hh, ww)*0.2);
//#define omp parallel
//	{
//#define omp for private(shift_distance_map)
//		for (k = 0; k < num_target; k++)
//		{
//			Circular_Shift_Index_On_Distance_Map(
//				idx_on_dm_yx,//std::vector<std::vector<int>> &in_distance_map_index,
//				shift_distance_map,//std::vector<std::vector<int>> &out_distance_map_index,
//				k);//int in_circular_shift_factor)
//
//			score_vector[k] = find_corner_stone_points(
//				shift_distance_map,//std::vector<std::vector<int>> &in_distance_map_idx,
//				num_target,//int in_distance_map_hh,
//				num_ref,//int in_distance_map_ww,
//				mask_length);//int in_mask_length)
//		}
//	}
//	Find_Max(score_vector, &idx);
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
	sv_Circular_Shift_Vector(idx, in_target_texture_on_code);
	out_distance_map = std::vector<std::vector<double>>(num_target);
	for (k = 0; k < num_target; k++)
	{
		out_distance_map[k] = std::vector<double>(num_ref, DBL_MAX);
	}

	if (!Update_Distance_Map_Using_Correspondances_Using_Slices(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		in_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		out_distance_map))//std::vector<std::vector<double>> &out_distance_map)
	{
		exit(0);
		return;
	}

	out_cornerstone_map = std::vector<std::vector<bool>>(out_distance_map.size());
	for (k = 0; k < out_distance_map.size(); k++)
	{
		out_cornerstone_map[k] = std::vector<bool>(out_distance_map[k].size(), false);
	}


#define omp parallel
	{
#define omp for private(m)
		for (k = 0; k < out_cornerstone_map.size(); k++)
		{
			for (m = 0; m < out_cornerstone_map[k].size(); m++)
			{
				if (out_distance_map[k][m] == 0)
				{
					out_cornerstone_map[k][m] = true;
				}
			}
		}
	}


	if (!Update_Distance_Map_Using_Euclidean_Distance(
		in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
		in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
		in_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		out_distance_map))//std::vector<std::vector<double>> &out_distance_map)
	{
		exit(0);
		return;
	}
	/************************************************************************************/

}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_DTW::Update_Distance_Map_Using_Correspondances_Using_Slices(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<double>> &out_distance_map)
//************************************************************************
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
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_DTW::Update_Distance_Map_Using_Correspondances_Using_Slices(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<int>> &out_idx_on_distance_map_yx)
//************************************************************************
{
	std::vector<int> indices;
	int k, q, num_target_voxel, num_reference_voxel;

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
				for (q = 0; q < indices.size(); q++)
				{
					out_idx_on_distance_map_yx[k].push_back(indices[q]);
					//out_distance_map[k][indices[q]] = 0.0;
				}
			}
		}
	}

	return true;

}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_DTW::Update_Distance_Map_Using_Euclidean_Distance(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &in_target_texture_on_code,
	std::vector<std::vector<double>> &out_distance_map)
//************************************************************************
{
	CKvVectorInt indices;
	CKvVoxel *p_voxel;
	int k, l, num_target_voxel, num_reference_voxel;
	bool flag = false;

	num_reference_voxel = in_reference_texture_on_code.size();
	num_target_voxel = in_target_texture_on_code.size();

	if (num_target_voxel != out_distance_map.size())
	{
		return false;
	}

	p_voxel = in_set_of_voxels->vp();

#pragma omp parallel
	{
#pragma omp for private(l)
		for (k = 0; k < num_target_voxel; k++)
		{
			for (l = 0; l < num_reference_voxel; l++)
			{
				if (out_distance_map[k][l] == DBL_MAX)
				{
					out_distance_map[k][l] = p_voxel[in_target_texture_on_code[k]].ds_Distance_Squared(
						p_voxel[in_reference_texture_on_code[l]]);
				}

			}
		}
	}



	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_DTW::Extract_Sub_Distance_Maps(
	std::vector<std::vector<double>> &in_distance_map,
	std::vector<std::vector<bool>> &in_cornerstone_map,
	std::vector<std::vector<std::vector<double>>> &out_sub_blocks,
	std::vector<std::vector<int>> &out_set_of_left_upper_xy)
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
	int xx, yy;

	CKvMatrixUcharRgb tmp_matrgb, origin_rgb;


	if (in_distance_map.size() != in_cornerstone_map.size())
	{
		Kv_Printf("[Extract_Sub_Distance_Maps] : Wrong size of two input maps.\ndistance = %d corner map = %d",
			in_distance_map.size(), in_cornerstone_map.size());
		exit(0);
		return false;
	}

	if (in_distance_map[0].size() != in_cornerstone_map[0].size())
	{
		Kv_Printf("[Extract_Sub_Distance_Maps] : Wrong size of two input maps.\ndistance = %d corner map = %d",
			in_distance_map[0].size(), in_cornerstone_map[0].size());
		exit(0);
		return false;
	}

	hh = in_distance_map.size();
	ww = in_distance_map[0].size();

	/******************************************************/
	tmp_matrgb.c_Create(hh, ww);
	unsigned char **r, **g, **b;
	r = tmp_matrgb.mp(g, b);
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (in_cornerstone_map[j][i] == true)
			{
				r[j][i] = g[j][i] = b[j][i] = 255;
			}
			else
			{
				r[j][i] = g[j][i] = b[j][i] = 0;
			}
		}
	}
	origin_rgb.cp_Copy(&tmp_matrgb);
	/******************************************************/

	lu_xy = std::vector<int>(2);
	rl_xy = std::vector<int>(2);
	lu_xy[0] = lu_xy[1] = 0;
	
	out_sub_blocks = std::vector<std::vector<std::vector<double>>>();
	out_set_of_left_upper_xy = std::vector<std::vector<int>>();
	while (1)
	{
		//printf("	[Find_End_Point_of_Sub_Block_Matrix]\n");
		Find_End_Point_of_Sub_Block_Matrix(
			in_cornerstone_map,//std::vector<std::vector<bool>> &in_cornerstone_map,
			lu_xy,//std::vector<int> &in_left_upper_xy,
			rl_xy);//std::vector<int> &out_right_lower_xy);

		//Get Sub block matrix. 
		if (!get_block_matrix(
			in_distance_map,//std::vector<std::vector<T>> &in_matrix,
			lu_xy,//std::vector<int> &in_left_upper_xy,
			rl_xy,//std::vector<int> &in_right_lower_xy,
			sub_block_matrix))//std::vector<std::vector<T>> &out_block_matrix)
		{
			//plot_data_for_debug(
			//	in_cornerstone_map);//std::vector<std::vector<bool>> &in_sub_distance_map);

			Kv_Printf("Error : [Extract_Sub_Distance_Maps]");
			exit(0);
			return false;
		}

		// Push back to the depot of sub blocks. 
		out_sub_blocks.push_back(sub_block_matrix);
		out_set_of_left_upper_xy.push_back(lu_xy);

		plot_data_for_debug(
			tmp_matrgb,//CKvMatrixUcharRgb &in_distance_map,
			in_cornerstone_map,//std::vector<std::vector<bool>> &in_sub_distance_map,
			lu_xy,//std::vector<int> &in_left_upper_xy,
			rl_xy);//std::vector<int> &in_right_lower_xy)

		printf("(%d %d) <-> (%d %d)\n", lu_xy[0], lu_xy[1], rl_xy[0], rl_xy[1]);


		// Set Next sub block start point. 
		// -> 다음 column을 확인하여, 면밀하게 update할 필요가 있음. 
		if (((rl_xy[0]) < (ww - 1)) &&
			((rl_xy[1]) < (hh - 1)))
		{
			if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]    ] == true) &&
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
			else if ((in_cornerstone_map[rl_xy[1] + 1][rl_xy[0]    ] == false) &&
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
		else
		{
			lu_xy[0] = rl_xy[0] + 1;
			lu_xy[1] = rl_xy[1] + 1;
		}
		
		//lu_xy[0] = rl_xy[0] + 1;
		//lu_xy[1] = rl_xy[1] + 1;


		// Check break condition. 
		if ((lu_xy[0] > ww - 1) || (lu_xy[1] > hh - 1))
		{
			break;
		}
	}

	//zz_sc[0].s_d_Display(&tmp_matrgb);
	//plot_data_for_debug(in_distance_map);
	CKvScreen sc[2];
	sc[0].s_d_Display(&origin_rgb);
	sc[1].s_d_Display(&tmp_matrgb);
	Kv_Printf("!!!");

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_DTW::Find_End_Point_of_Sub_Block_Matrix(
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
	//printf("		start_column=%d\n", start_column);
	while (xx < ww-1)
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
				//printf("[1-2]\n");
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

					out_right_lower_xy[0] = xx-1;
					out_right_lower_xy[1] = yy;

					return true;
				}
				else
				{
					// 역류하지 않은 case. 이 경우는 계속 searching 해 나감. 
					//printf("[2-2]\n");
					yy = nyy;
				}
			}
		}
		else // Blank 
		{
			if (start_column == true) // 처음 column이 true 이면서, 비어있는 column을 발견했을때임. 
			{
				// Block을 완성했기 때문에, 탈출. 
				//printf("[3-1]\n");
				out_right_lower_xy[0] = xx-1;
				out_right_lower_xy[1] = yy;
				return true;
			}

			// 처음 column이 false이면서, 비어있는 column을 발견하면, 아무것도 할일 없이 다음 위치로 이동. 
			//printf("[4-1]\n");

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
// 전제가 column index의 column은 true를 포함. 
// row index의 위치는... ? 일단 column index-1 이하로는 true가 없음. 
bool VCL_Voxel_Color_Sequence_Aligner_DTW::Find_Optimal_Position(
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
	//printf("[Is_True_Column_Full]-[2] %d / %d\n",xx, in_cornerstone_map[0].size());
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
					//xx = searching_xx;
					//escape_flag = true;
					//break;
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
				out_xx_if_false = ww-1;
				out_yy_if_false = hh-1;
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
bool VCL_Voxel_Color_Sequence_Aligner_DTW::Is_Reverse_Path( // 전제가 (column index - 1)와 row index 의 위치는 true. 
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
	CKvXrunset1dShort xrun1d;
	CKvVectorBool vec_bool;
	int dummy,np,k,z, hh, ww,xx;
	short *p,zo,z1;
	bool flag=false;

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
	
	a.push_back(in_column_index+1); a.push_back(0);
	b.push_back(ww-1);              b.push_back(hh-1);
	
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
bool VCL_Voxel_Color_Sequence_Aligner_DTW::check_bool_vector(
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
	short target_z=SHORT_MAX;

	bool *p_vec = vec_bool.c_Create(in_vector.size());
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
void VCL_Voxel_Color_Sequence_Aligner_DTW::Circular_Shift_Distance_Map(
	std::vector<std::vector<double>> &in_distance_map,
	std::vector<std::vector<double>> &out_distance_map,
	int in_circular_shift_factor)
//************************************************************************
{
	std::vector<std::vector<double>> tmp = in_distance_map;
	int ww, hh, j, i;

	if (in_circular_shift_factor == 0)
	{
		out_distance_map = tmp;
		return;
	}

	hh = tmp.size();
	ww = tmp[0].size();

	out_distance_map = std::vector<std::vector<double>>(hh, std::vector<double>(ww));

#define omp parallel
	{
#define omp for private(i)
		for (j = in_circular_shift_factor; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				out_distance_map[j - in_circular_shift_factor][i] = tmp[j][i];
			}
		}
	}

#define omp parallel
	{
#define omp for private(i)
		for (j = 0; j < in_circular_shift_factor; j++)
		{
			for (i = 0; i < ww; i++)
			{
				out_distance_map[hh - in_circular_shift_factor + j][i] = tmp[j][i];
			}
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Circular_Shift_Index_On_Distance_Map(
	std::vector<std::vector<int>> &in_distance_map_index,
	std::vector<std::vector<int>> &out_distance_map_index,
	int in_circular_shift_factor)
//************************************************************************
{
	std::vector<std::vector<int>> tmp = in_distance_map_index;
	int hh, j, i, idx;

	if (in_circular_shift_factor == 0)
	{
		out_distance_map_index = tmp;
		return;
	}

	hh = tmp.size();
	out_distance_map_index = std::vector<std::vector<int>>(hh);

#define omp parallel
	{
#define omp for private(i)
		for (j = in_circular_shift_factor; j < hh; j++)
		{
			for (i = 0; i < tmp[j].size(); i++)
			{
				out_distance_map_index[j - in_circular_shift_factor].push_back(tmp[j][i]);
			}
		}
	}

#define omp parallel
	{
#define omp for private(i)
		for (j = 0; j < in_circular_shift_factor; j++)
		{
			for (i = 0; i < tmp[j].size(); i++)
			{
				out_distance_map_index[hh - in_circular_shift_factor + j].push_back(tmp[j][i]);
			}
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Modify_Texture_on_Codes(
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	std::vector<int> &in_reference_warping_index, // X
	std::vector<int> &in_target_warping_index,    // Y
	std::vector<int> &in_modified_reference_warping_index,
	std::vector<int> &out_modified_target_warping_index)
//************************************************************************
{
	std::vector<std::vector<int>> bunch_of_modified_target_ToC;
	std::vector<std::vector<int>> bunch_of_modified_target_WI;
	std::vector<std::vector<int>> lu_table;
	std::vector<std::vector<int>> update_lu_table;
	std::vector<int> modified_target_texture_on_code;
	std::vector<int> indices;
	int num_target_index, num_refer_index, num_correspoints;
	int num_modified_refer_index;
	int length_of_target, k, l, p;

	num_refer_index  = in_reference_warping_index.size();
	num_target_index = in_target_warping_index.size();

	if (num_refer_index != num_target_index)
	{
		Kv_Printf("[Modify_Texture_on_Codes] : Wrong Input Index vectors : %d %d\n",
			num_refer_index, num_target_index);
		exit(0);
	}

	length_of_target = io_target_texture_on_code.size();
	lu_table         = std::vector<std::vector<int>>(length_of_target, std::vector<int>());

	for (k = 0; k < num_target_index; k++)
	{
		lu_table[in_target_warping_index[k]].push_back(in_reference_warping_index[k]);
	}
	
	num_modified_refer_index = in_modified_reference_warping_index.size();
	update_lu_table = std::vector<std::vector<int>>(num_modified_refer_index, std::vector<int>());
	for (k = 0; k < length_of_target; k++)
	{
		num_correspoints = lu_table[k].size();
		for (l = 0; l < num_correspoints; l++)
		{
			if (find_element_in_vector(
				in_modified_reference_warping_index,//std::vector<T> &in_vector, -> 원본 reference ToC의 index들이 들어가 있음. 
				lu_table[k][l],//T in_value,
				indices))//std::vector<int> &out_indices)
			{
				for (p = 0; p < indices.size(); p++)
				{
					update_lu_table[indices[p]].push_back(k);
				}
			}
		}
	}

	// Ver 2
	std::vector<bool> flag = std::vector<bool>(num_target_index, false);
	bunch_of_modified_target_ToC = std::vector<std::vector<int>>(num_modified_refer_index, std::vector<int>());
	bunch_of_modified_target_WI  = std::vector<std::vector<int>>(num_modified_refer_index, std::vector<int>());
	for (k = 0; k < num_modified_refer_index; k++)
	{
		num_target_index = update_lu_table[k].size();
		if (num_target_index == 0)
		{
			bunch_of_modified_target_ToC[k].push_back(-1);
			bunch_of_modified_target_WI[k].push_back(-1);
		}
		else if (num_target_index == 1)
		{
			if (flag[update_lu_table[k][0]] == false)
			{
				flag[update_lu_table[k][0]] = true;
				bunch_of_modified_target_ToC[k].push_back(io_target_texture_on_code[update_lu_table[k][0]]);
				bunch_of_modified_target_WI[k].push_back(update_lu_table[k][0]);
			}
			else
			{
				bunch_of_modified_target_ToC[k].push_back(-1);
				bunch_of_modified_target_WI[k].push_back(-1);
			}
		}
		else
		{
			for (l = 0; l < num_target_index; l++)
			{
				if (flag[update_lu_table[k][l]] == false)
				{
					flag[update_lu_table[k][l]] = true;
					bunch_of_modified_target_ToC[k].push_back(io_target_texture_on_code[update_lu_table[k][l]]);
					bunch_of_modified_target_WI[k].push_back(update_lu_table[k][l]);
				}
			}
		}
	}

	/***********************************************************************************************************/

	/***********************************************************************************************************/
	// Ver1-1
	//out_modified_target_warping_index = std::vector<int>();
	//modified_target_texture_on_code   = std::vector<int>();
	//std::vector<bool> flag            = std::vector<bool>(num_target_index, false);
	//for (k = 0; k < num_modified_refer_index; k++)
	//{
	//	num_target_index = update_lu_table[k].size();
	//	if (num_target_index == 0)
	//	{
	//		out_modified_target_warping_index.push_back(-1);
	//		modified_target_texture_on_code.push_back(-1);
	//	}
	//	else
	//	{
	//		if (flag[update_lu_table[k][0]] == false)
	//		{
	//			flag[update_lu_table[k][0]] = true;
	//			out_modified_target_warping_index.push_back(update_lu_table[k][0]);
	//			modified_target_texture_on_code.push_back(io_target_texture_on_code[update_lu_table[k][0]]);
	//		}
	//		else
	//		{
	//			out_modified_target_warping_index.push_back(-1);
	//			modified_target_texture_on_code.push_back(-1);
	//		}
	//	}
	//}
	//for (k = 0; k < num_modified_refer_index; k++)
	//{
	//	num_target_index = update_lu_table[k].size();
	//	if (num_target_index > 1)
	//	{
	//		//out_modified_target_warping_index[k] = -1;
	//		//modified_target_texture_on_code[k]   = -1;
	//		flag[update_lu_table[k][l]] = false;
	//		for (l = 1; l < num_target_index; l++)
	//		{
	//			if (flag[update_lu_table[k][l]] == false)
	//			{
	//			}
	//		}
	//	}
	//}
	// Ver1-2
	//int count;
	//for (k = 0; k < num_modified_refer_index; k++)
	//{
	//	if (num_target_index == 0)
	//	{
	//		out_modified_target_warping_index.push_back(-1);
	//		modified_target_texture_on_code.push_back(-1);
	//	}
	//	else if (num_target_index == 1)
	//	{
	//		if (flag[update_lu_table[k][0]] == false)
	//		{
	//			flag[update_lu_table[k][0]] = true;
	//			out_modified_target_warping_index.push_back(update_lu_table[k][0]);
	//			modified_target_texture_on_code.push_back(io_target_texture_on_code[update_lu_table[k][0]]);
	//		}
	//		else
	//		{
	//			out_modified_target_warping_index.push_back(-1);
	//			modified_target_texture_on_code.push_back(-1);
	//		}
	//	}
	//	else // num_target_index > 1
	//	{
	//		for (l = 0; l < num_target_index; l++)
	//		{
	//			if (flag[update_lu_table[k][l]] == false)
	//			{
	//				flag[update_lu_table[k][l]] = true;
	//				out_modified_target_warping_index.push_back(update_lu_table[k][l]);
	//				modified_target_texture_on_code.push_back(io_target_texture_on_code[update_lu_table[k][l]]);
	//			}
	//			else
	//			{
	//				out_modified_target_warping_index.push_back(-1);
	//				modified_target_texture_on_code.push_back(-1);
	//			}
	//		}
	//	}
	//}






	/******************************************************************************************/
	io_target_texture_on_code = std::vector<int>(modified_target_texture_on_code.size());
	for (k = 0; k < modified_target_texture_on_code.size(); k++)
	{
		//printf("k=%d/%d) %d\n", k, modified_target_texture_on_code.size(), modified_target_texture_on_code[k]);
		io_target_texture_on_code[k] = modified_target_texture_on_code[k];
	}
	//Kv_Printf("!!!");
	//io_target_texture_on_code = modified_target_texture_on_code;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Modify_Texture_on_Codes2(
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	std::vector<int> &in_reference_warping_index,
	std::vector<int> &in_target_warping_index,
	std::vector<int> &in_modified_reference_warping_index,
	std::vector<int> &out_modified_target_warping_index)
//************************************************************************
{
	std::vector<std::vector<int>> bunch_of_modified_target_ToC;
	std::vector<std::vector<int>> bunch_of_modified_target_WI;
	std::vector<std::vector<int>> lu_table;
	std::vector<std::vector<int>> update_lu_table;
	std::vector<int> modified_target_texture_on_code;
	std::vector<int> indices;
	int num_target_index, num_refer_index, num_correspoints;
	int num_modified_refer_index;
	int length_of_target, k, l, p;

	num_refer_index = in_reference_warping_index.size();
	num_target_index = in_target_warping_index.size();

	if (num_refer_index != num_target_index)
	{
		Kv_Printf("[Modify_Texture_on_Codes] : Wrong Input Index vectors : %d %d\n",
			num_refer_index, num_target_index);
		exit(0);
	}

	length_of_target = io_target_texture_on_code.size();
	lu_table = std::vector<std::vector<int>>(length_of_target, std::vector<int>());

	for (k = 0; k < num_target_index; k++)
	{
		lu_table[in_target_warping_index[k]].push_back(in_reference_warping_index[k]);
	}

	num_modified_refer_index = in_modified_reference_warping_index.size();
	update_lu_table = std::vector<std::vector<int>>(num_modified_refer_index, std::vector<int>());
	for (k = 0; k < length_of_target; k++)
	{
		num_correspoints = lu_table[k].size();
		for (l = 0; l < num_correspoints; l++)
		{
			if (find_element_in_vector(
				in_modified_reference_warping_index,//std::vector<T> &in_vector, -> 원본 reference ToC의 index들이 들어가 있음. 
				lu_table[k][l],//T in_value,
				indices))//std::vector<int> &out_indices)
			{
				for (p = 0; p < indices.size(); p++)
				{
					update_lu_table[indices[p]].push_back(k);
				}
			}
		}
	}

	// Ver 2
	std::vector<bool> flag = std::vector<bool>(num_target_index, false);
	bunch_of_modified_target_ToC = std::vector<std::vector<int>>(num_modified_refer_index, std::vector<int>());
	bunch_of_modified_target_WI = std::vector<std::vector<int>>(num_modified_refer_index, std::vector<int>());
	for (k = 0; k < num_modified_refer_index; k++)
	{
		num_target_index = update_lu_table[k].size();
		if (num_target_index == 0)
		{
			bunch_of_modified_target_ToC[k].push_back(-1);
			bunch_of_modified_target_WI[k].push_back(-1);
		}
		else if (num_target_index == 1)
		{
			if (flag[update_lu_table[k][0]] == false)
			{
				flag[update_lu_table[k][0]] = true;
				bunch_of_modified_target_ToC[k].push_back(io_target_texture_on_code[update_lu_table[k][0]]);
				bunch_of_modified_target_WI[k].push_back(update_lu_table[k][0]);
			}
			else
			{
				bunch_of_modified_target_ToC[k].push_back(-1);
				bunch_of_modified_target_WI[k].push_back(-1);
			}
		}
		else
		{
			for (l = 0; l < num_target_index; l++)
			{
				if (flag[update_lu_table[k][l]] == false)
				{
					flag[update_lu_table[k][l]] = true;
					bunch_of_modified_target_ToC[k].push_back(io_target_texture_on_code[update_lu_table[k][l]]);
					bunch_of_modified_target_WI[k].push_back(update_lu_table[k][l]);
				}
			}
		}
	}

	/***********************************************************************************************************/

	/******************************************************************************************/
	io_target_texture_on_code = std::vector<int>(modified_target_texture_on_code.size());
	for (k = 0; k < modified_target_texture_on_code.size(); k++)
	{
		io_target_texture_on_code[k] = modified_target_texture_on_code[k];
	}
	/******************************************************************************************/
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Warping_Texture_on_Codes(
	std::vector<std::vector<int>> &io_texture_on_codes,
	std::vector<std::vector<std::vector<int>>> &in_warping_index)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> modified_texture_on_codes;
	int num_toc, num_element, k, l;

	/********************************************************************************/
	// Initialize Memory  modified_texture_on_codes
	num_toc = io_texture_on_codes.size();
	modified_texture_on_codes = std::vector<std::vector<std::vector<int>>>(num_toc);
	for (k = 0; k < num_toc; k++)
	{
		num_element = (int)io_texture_on_codes[k].size();
		modified_texture_on_codes[k] = std::vector<std::vector<int>>(num_element);
		for (l = 0; l < num_element; l++)
		{
			modified_texture_on_codes[k][l].push_back(io_texture_on_codes[k][l]);
		}
	}
	/********************************************************************************/

	warping_texture_on_code_forward(
		io_texture_on_codes,//std::vector<std::vector<int>> &in_texture_on_codes,
		modified_texture_on_codes,//std::vector<std::vector<std::vector<int>>> &io_modified_texture_on_codes,
		in_warping_index);//std::vector<std::vector<std::vector<int>>> &in_warping_index)

	//warping_texture_on_code_backward(
	//	io_texture_on_codes,//std::vector<std::vector<int>> &in_texture_on_codes,
	//	modified_texture_on_codes,//std::vector<std::vector<std::vector<int>>> &io_modified_texture_on_codes,
	//	in_warping_index);//std::vector<std::vector<std::vector<int>>> &in_warping_index)


	/************************************************************************ decode ******/
	std::vector<std::vector<std::vector<int>>> warping_table;
	std::vector<bool> flag;
	int m, j, num_dim, num_element1;

	warping_table = std::vector<std::vector<std::vector<int>>>((int)(num_toc - 1));
	for (k = 0; k < num_toc - 1; k++)
	{
		warping_table[k] = std::vector<std::vector<int>>(io_texture_on_codes[k].size()); // 
		flag = std::vector<bool>(io_texture_on_codes[k + 1].size(), false);
		num_element = in_warping_index[k][0].size();
		for (m = 0; m < num_element; m++)
		{
			if (flag[in_warping_index[k][1][m]] == false)
			{
				flag[in_warping_index[k][1][m]] = true;
				warping_table[k][in_warping_index[k][0][m]].push_back(in_warping_index[k][1][m]);
			}
		}

		for (m = 0; m < warping_table[k].size(); m++)
		{
			if (warping_table[k][m].size() == 0)
			{
				warping_table[k][m].push_back(-1);
			}
		}
	}
	/********************************************************************************/
	/********************************************************************************/
	io_texture_on_codes = std::vector<std::vector<int>>(num_toc);
	num_dim = modified_texture_on_codes[0].size();
	for (l = 0; l < num_dim; l++)
	{
		num_element = modified_texture_on_codes[0][l].size();
		for (m = 0; m < num_element; m++)
		{
			io_texture_on_codes[0].push_back(modified_texture_on_codes[0][l][m]);
		}
	}
	for (k = 0; k < num_toc - 1; k++)
	{
		num_dim = warping_table[k].size();
		for (l = 0; l < num_dim; l++)
		{
			num_element = warping_table[k][l].size();
			for (m = 0; m < num_element; m++)
			{
				if (warping_table[k][l][m] != -1)
				{
					num_element1 = modified_texture_on_codes[k + 1][warping_table[k][l][m]].size();
					for (j = 0; j < num_element1; j++)
					{
						io_texture_on_codes[k + 1].push_back(modified_texture_on_codes[k + 1][warping_table[k][l][m]][j]);
					}
				}
				else
				{
					io_texture_on_codes[k + 1].push_back(-1);
				}
			}
		}
	}
	/****************************************************************************************/



}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Warping_Texture_on_Codes2(
	std::vector<std::vector<int>> &io_texture_on_codes,
	std::vector<std::vector<std::vector<int>>> &in_warping_index)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> modified_texture_on_codes;
	std::vector<std::vector<std::vector<int>>> warping_table;
	std::vector<bool> flag;
	int num_toc, num_element, num_element1, num_element2, num_dim, count;
	int k, l, m, j;

	// Initialize Memory  modified_texture_on_codes
	num_toc = io_texture_on_codes.size();
	modified_texture_on_codes = std::vector<std::vector<std::vector<int>>>(num_toc);
	for (k = 0; k < num_toc; k++)
	{
		num_element = (int)io_texture_on_codes[k].size();
		modified_texture_on_codes[k] = std::vector<std::vector<int>>(num_element);
		for (l = 0; l < num_element; l++)
		{
			modified_texture_on_codes[k][l].push_back(io_texture_on_codes[k][l]);
		}
	}

	if (in_warping_index.size() != (num_toc - 1))
	{
		Kv_Printf("Error in_warping_index -> %d %d\n", in_warping_index.size(), (num_toc - 1));
		exit(0);
		return;
	}

	// Initialize Memory  warping_table 0 -> num_toc
	warping_table = std::vector<std::vector<std::vector<int>>>((int)(num_toc - 1));
	for (k = 0; k < num_toc - 1; k++)
	{
		warping_table[k] = std::vector<std::vector<int>>(io_texture_on_codes[k].size()); // 
		flag             = std::vector<bool>(io_texture_on_codes[k + 1].size(), false);
		num_element = in_warping_index[k][0].size();
		for (m = 0; m < num_element; m++)
		{
			if (flag[in_warping_index[k][1][m]] == false)
			{
				flag[in_warping_index[k][1][m]] = true;
				warping_table[k][in_warping_index[k][0][m]].push_back(in_warping_index[k][1][m]);
			}
		}

		for (m = 0; m < warping_table[k].size(); m++)
		{
			if (warping_table[k][m].size() == 0)
			{
				warping_table[k][m].push_back(-1);
			}
		}
	}

	// Update modified_texture_on_codes.
	for (k = 0; k < num_toc - 1; k++)
	{
		num_element = warping_table[k].size();
		for (l = 0; l < num_element; l++)
		{
			num_element1 = 0;
			for (m = 0; m < warping_table[k][l].size(); m++)
			{
				if (warping_table[k][l][m] != -1)
				{
					num_element1 += (int)(modified_texture_on_codes[k + 1][warping_table[k][l][m]].size());
				}
				else
				{
					num_element1++;
				}
				
			}
			num_element2 = (int)(modified_texture_on_codes[k][l].size());

			if (num_element1 > num_element2)
			{
				for (m = num_element2; m < num_element1; m++)
				{
					modified_texture_on_codes[k][l].push_back(-1);
				}
			}
		}
	}



	/************************************************************************ decode ******/
	io_texture_on_codes = std::vector<std::vector<int>>(num_toc);
	num_dim = modified_texture_on_codes[0].size();
	for (l = 0; l < num_dim; l++)
	{
		num_element = modified_texture_on_codes[0][l].size();
		for (m = 0; m < num_element; m++)
		{
			io_texture_on_codes[0].push_back(modified_texture_on_codes[0][l][m]);
		}
	}
	for (k = 0; k < num_toc-1; k++)
	{
		num_dim = warping_table[k].size();
		for (l = 0; l < num_dim; l++)
		{
			num_element = warping_table[k][l].size();
			for (m = 0; m < num_element; m++)
			{
				if (warping_table[k][l][m] != -1)
				{
					num_element1 = modified_texture_on_codes[k + 1][warping_table[k][l][m]].size();
					for (j = 0; j < num_element1; j++)
					{
						io_texture_on_codes[k + 1].push_back(modified_texture_on_codes[k + 1][warping_table[k][l][m]][j]);
					}
				}
				else
				{
					io_texture_on_codes[k + 1].push_back(-1);
				}
			}
		}
	}




}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::Warping_Texture_on_Codes3(
	std::vector<std::vector<int>> &io_texture_on_codes,
	std::vector<std::vector<std::vector<int>>> &in_warping_index)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> modified_texture_on_codes;
	std::vector<std::vector<std::vector<int>>> warping_table;
	std::vector<bool> flag;
	int num_toc, num_element, num_element1, num_element2, num_element3, num_dim, count;
	int k, l, m, n, j, num_del, idx, dummy;


	/****************************************************************************************/
	// Initialize Memory  modified_texture_on_codes
	num_toc = io_texture_on_codes.size();
	modified_texture_on_codes = std::vector<std::vector<std::vector<int>>>(num_toc);
	for (k = 0; k < num_toc; k++)
	{
		num_element = (int)io_texture_on_codes[k].size();
		modified_texture_on_codes[k] = std::vector<std::vector<int>>(num_element);
		for (l = 0; l < num_element; l++)
		{
			modified_texture_on_codes[k][l].push_back(io_texture_on_codes[k][l]);
		}
	}

	if (in_warping_index.size() != (num_toc - 1))
	{
		Kv_Printf("Error in_warping_index -> %d %d\n", in_warping_index.size(), (num_toc - 1));
		exit(0);
		return;
	}
	/****************************************************************************************/

	/****************************************************************************************/
	// Initialize Memory  warping_table 0 -> num_toc
	warping_table = std::vector<std::vector<std::vector<int>>>((int)(num_toc - 1));
	for (k = 0; k < num_toc - 1; k++)
	{
		warping_table[k] = std::vector<std::vector<int>>(io_texture_on_codes[k].size()); // 
		flag = std::vector<bool>(io_texture_on_codes[k + 1].size(), false);
		num_element = in_warping_index[k][0].size();
		for (m = 0; m < num_element; m++)
		{
			if (flag[in_warping_index[k][1][m]] == false)
			{
				flag[in_warping_index[k][1][m]] = true;
				warping_table[k][in_warping_index[k][0][m]].push_back(in_warping_index[k][1][m]);
			}
		}

		for (m = 0; m < warping_table[k].size(); m++)
		{
			if (warping_table[k][m].size() == 0)
			{
				warping_table[k][m].push_back(-1);
			}
		}
	}
	/****************************************************************************************/

	/****************************************************************************************/
	// Update modified_texture_on_codes.
	for (k = 0; k < num_toc - 1; k++)
	{
		num_element = warping_table[k].size();
		for (l = 0; l < num_element; l++)
		{
			num_element1 = 0;
			for (m = 0; m < warping_table[k][l].size(); m++)
			{
				if (warping_table[k][l][m] != -1)
				{
					num_element1 += (int)(modified_texture_on_codes[k + 1][warping_table[k][l][m]].size());
				}
				else
				{
					num_element1++;
				}
			}

			num_element2 = (int)(modified_texture_on_codes[k][l].size());
			if (num_element1 > num_element2)
			{
				num_del = num_element1 - num_element2;
				for (m = num_element2; m < num_element1; m++)
				{
					modified_texture_on_codes[k][l].push_back(-1);
				}
				
				for (m = k-1 ; m > -1 ; m--)
				{
					if (find_element_in_2dvector(
						warping_table[m],//std::vector<std::vector<T>> &in_vectors,
						l,//T in_value,
						idx,//int &out_index_row,
						dummy))//int &out_index_column)
					{

						num_element3 = modified_texture_on_codes[m][idx].size();
						if (num_element1 > num_element3)
						{
							for (j = num_element3; j < num_element1; j++)
							{
								modified_texture_on_codes[m][idx].push_back(-1);
							}
						}

						//Kv_Printf("Critical Error in [find_element_in_vector]");
						//exit(0);
					}

				}
			}
		}
	}
	/****************************************************************************************/






	/************************************************************************ decode ******/
	io_texture_on_codes = std::vector<std::vector<int>>(num_toc);
	num_dim = modified_texture_on_codes[0].size();
	for (l = 0; l < num_dim; l++)
	{
		num_element = modified_texture_on_codes[0][l].size();
		for (m = 0; m < num_element; m++)
		{
			io_texture_on_codes[0].push_back(modified_texture_on_codes[0][l][m]);
		}
	}
	for (k = 0; k < num_toc - 1; k++)
	{
		num_dim = warping_table[k].size();
		for (l = 0; l < num_dim; l++)
		{
			num_element = warping_table[k][l].size();
			for (m = 0; m < num_element; m++)
			{
				if (warping_table[k][l][m] != -1)
				{
					num_element1 = modified_texture_on_codes[k + 1][warping_table[k][l][m]].size();
					for (j = 0; j < num_element1; j++)
					{
						io_texture_on_codes[k + 1].push_back(modified_texture_on_codes[k + 1][warping_table[k][l][m]][j]);
					}
				}
				else
				{
					io_texture_on_codes[k + 1].push_back(-1);
				}
			}
		}
	}
	/****************************************************************************************/


}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::warping_texture_on_code_forward(
	std::vector<std::vector<int>> &in_texture_on_codes,
	std::vector<std::vector<std::vector<int>>> &io_modified_texture_on_codes,
	std::vector<std::vector<std::vector<int>>> &in_warping_index)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> warping_table;
	std::vector<bool> flag;
	int num_toc, num_element, num_element1, num_element2, num_element3, num_dim, count;
	int k, l, m, n, j, num_del, idx, dummy;

	num_toc = io_modified_texture_on_codes.size();
	if (in_warping_index.size() != (num_toc - 1))
	{
		Kv_Printf("Error in_warping_index -> %d %d\n", in_warping_index.size(), (num_toc - 1));
		exit(0);
		return;
	}

	/****************************************************************************************/
	// Initialize Memory  warping_table 0 -> num_toc
	warping_table = std::vector<std::vector<std::vector<int>>>((int)(num_toc - 1));
	for (k = 0; k < num_toc - 1; k++)
	{
		warping_table[k] = std::vector<std::vector<int>>(in_texture_on_codes[k].size()); // 
		flag = std::vector<bool>(in_texture_on_codes[k + 1].size(), false);
		num_element = in_warping_index[k][0].size();
		for (m = 0; m < num_element; m++)
		{
			if (flag[in_warping_index[k][1][m]] == false)
			{
				flag[in_warping_index[k][1][m]] = true;
				warping_table[k][in_warping_index[k][0][m]].push_back(in_warping_index[k][1][m]);
			}
		}

		for (m = 0; m < warping_table[k].size(); m++)
		{
			if (warping_table[k][m].size() == 0)
			{
				warping_table[k][m].push_back(-1);
			}
		}
	}
	/****************************************************************************************/

	/****************************************************************************************/
	// Update modified_texture_on_codes.
	for (k = 0; k < num_toc - 1; k++)
	{
		num_element = warping_table[k].size();
		for (l = 0; l < num_element; l++)
		{
			num_element1 = 0;
			for (m = 0; m < warping_table[k][l].size(); m++)
			{
				if (warping_table[k][l][m] != -1)
				{
					num_element1 += (int)(io_modified_texture_on_codes[k + 1][warping_table[k][l][m]].size());
				}
				else
				{
					num_element1++;
				}
			}

			num_element2 = (int)(io_modified_texture_on_codes[k][l].size());
			if (num_element1 > num_element2)
			{
				num_del = num_element1 - num_element2;
				for (m = num_element2; m < num_element1; m++)
				{
					io_modified_texture_on_codes[k][l].push_back(-1);
				}

				for (m = k - 1; m > -1; m--)
				{
					if (find_element_in_2dvector(
						warping_table[m],//std::vector<std::vector<T>> &in_vectors,
						l,//T in_value,
						idx,//int &out_index_row,
						dummy))//int &out_index_column)
					{

						num_element3 = io_modified_texture_on_codes[m][idx].size();
						if (num_element1 > num_element3)
						{
							for (j = num_element3; j < num_element1; j++)
							{
								io_modified_texture_on_codes[m][idx].push_back(-1);
							}
						}
					}
				}
			}
		}
	}
	/****************************************************************************************/


}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::warping_texture_on_code_backward(
	std::vector<std::vector<int>> &in_texture_on_codes,
	std::vector<std::vector<std::vector<int>>> &io_modified_texture_on_codes,
	std::vector<std::vector<std::vector<int>>> &in_warping_index)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> warping_table;
	std::vector<bool> flag;
	int num_toc, num_element, num_element1, num_element2, num_element3, num_dim, count;
	int k, l, m, n, j, num_del, idx, dummy;

	num_toc = io_modified_texture_on_codes.size();
	if (in_warping_index.size() != (num_toc - 1))
	{
		Kv_Printf("Error in_warping_index -> %d %d\n", in_warping_index.size(), (num_toc - 1));
		exit(0);
		return;
	}

	/****************************************************************************************/
	// Initialize Memory  warping_table 0 -> num_toc
	warping_table = std::vector<std::vector<std::vector<int>>>((int)(num_toc));
	for (k = num_toc - 1; k > 0; k--)
	{
		warping_table[k] = std::vector<std::vector<int>>(in_texture_on_codes[k].size());
		flag             = std::vector<bool>(in_texture_on_codes[k - 1].size(), false);
		for (m = 0; m < in_warping_index[k - 1][1].size(); m++)
		{
			if (flag[in_warping_index[k - 1][0][m]] == false)
			{
				flag[in_warping_index[k - 1][0][m]] = true;
				warping_table[k][in_warping_index[k - 1][1][m]].push_back(in_warping_index[k - 1][0][m]);
			}
		}

		for (m = 0; m < warping_table[k].size(); m++)
		{
			if (warping_table[k][m].size() == 0)
			{
				warping_table[k][m].push_back(-1);
			}
		}
	}
	/****************************************************************************************/

	/****************************************************************************************/
	// Update modified_texture_on_codes.
	for (k = num_toc - 1; k > 0; k--)
	{
		num_element = warping_table[k].size();
		for (l = 0; l < num_element ; l++)
		{
			num_element1 = 0;
			for (m = 0; m < warping_table[k][l].size(); m++)
			{
				if (warping_table[k][l][m] != -1)
				{
					num_element1 += (int)(io_modified_texture_on_codes[k - 1][warping_table[k][l][m]].size());
				}
				else
				{
					num_element1++;
				}
			}
			num_element2 = io_modified_texture_on_codes[k][l].size();
			if (num_element1 > num_element2)
			{
				for (m = num_element1; m < num_element2; m++)
				{
					io_modified_texture_on_codes[k][l].push_back(-1);
				}

				for (m = k + 1; m < num_toc; m++)
				{
					if (find_element_in_2dvector(
						warping_table[m],//std::vector<std::vector<T>> &in_vectors,
						l,//T in_value,
						idx,//int &out_index_row,
						dummy))//int &out_index_column)
					{
						num_element3 = io_modified_texture_on_codes[m][idx].size();
						if (num_element1 > num_element3)
						{
							for (j = num_element3; j < num_element1; j++)
							{
								io_modified_texture_on_codes[m][idx].push_back(-1);
							}
						}
					}

				}
			}
		}
	}








}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::plot_data_for_debug(
	CKvMatrixUcharRgb &in_distance_map,
	std::vector<std::vector<bool>> &in_sub_distance_map,
	std::vector<int> &in_left_upper_xy,
	std::vector<int> &in_right_lower_xy)
//************************************************************************
{
	//CKvScreen sc[3];
	CKvMatrixUcharRgb before_map;
	CKvMatrixBool sub_map;
	std::vector<std::vector<bool>> sub_vv_map;
	int ww, hh, j, i;
	unsigned char **pr, **pg, **pb;
	bool **p_sm;

	get_block_matrix(
		in_sub_distance_map,//std::vector<std::vector<T>> &in_matrix,
		in_left_upper_xy,//std::vector<int> &in_left_upper_xy,
		in_right_lower_xy,//std::vector<int> &in_right_lower_xy,
		sub_vv_map);//std::vector<std::vector<T>> &out_block_matrix);


	hh = sub_vv_map.size();
	ww = sub_vv_map[0].size();
	p_sm = sub_map.c_Create(hh, ww, false);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p_sm[j][i] = sub_vv_map[j][i];
		}
	}

	before_map.cp_Copy(&in_distance_map);
	in_distance_map.ms(ww, hh);
	pr = in_distance_map.mp(pg, pb);

	for (i = in_left_upper_xy[0]; i < in_right_lower_xy[0] + 1; i++)
	{
		pr[in_left_upper_xy[1]][i] = 0;
		pg[in_left_upper_xy[1]][i] = 255;
		pb[in_left_upper_xy[1]][i] = 0;
	}

	for (i = in_left_upper_xy[0]; i < in_right_lower_xy[0] + 1; i++)
	{
		pr[in_right_lower_xy[1]][i] = 0;
		pg[in_right_lower_xy[1]][i] = 255;
		pb[in_right_lower_xy[1]][i] = 0;
	}

	for (j = in_left_upper_xy[1]; j < in_right_lower_xy[1] + 1; j++)
	{
		pr[j][in_left_upper_xy[0]] = 0;
		pg[j][in_left_upper_xy[0]] = 255;
		pb[j][in_left_upper_xy[0]] = 0;
	}

	for (j = in_left_upper_xy[1]; j < in_right_lower_xy[1] + 1; j++)
	{
		pr[j][in_right_lower_xy[0]] = 0;
		pg[j][in_right_lower_xy[0]] = 255;
		pb[j][in_right_lower_xy[0]] = 0;
	}

	//sc[0].s_d_Display(&in_distance_map);
	//sc[1].s_d_Display(&before_map);
	//sc[2].s_d_Display(&sub_map);
	//Kv_Printf("!!! Check Sub Maps");
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::plot_data_for_debug(
	std::vector<std::vector<bool>> &in_sub_distance_map)
//************************************************************************
{
	CKvScreen sc;
	CKvMatrixBool sub_map;
	int ww, hh, j, i;
	unsigned char **pr, **pg, **pb;
	bool **p_sm;


	hh = in_sub_distance_map.size();
	ww = in_sub_distance_map[0].size();
	p_sm = sub_map.c_Create(hh, ww, false);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p_sm[j][i] = in_sub_distance_map[j][i];
		}
	}


	sc.s_d_Display(&sub_map);

	Kv_Printf("!!! Check Upper Block Maps");
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_DTW::plot_data_for_debug(
	std::vector<std::vector<double>> &in_map)
//************************************************************************
{
	CKvMatrixUchar tmp;
	double min_val, max_val;
	int ww, hh, j, i;

	min_val = DBL_MAX;
	max_val = -1.0;

	hh = in_map.size();
	ww = in_map[0].size();

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (min_val > in_map[j][i])
			{
				min_val = in_map[j][i];
			}

			if (max_val < in_map[j][i])
			{
				max_val = in_map[j][i];
			}
		}
	}

	unsigned char **p = tmp.c_Create(hh, ww, (unsigned char)0);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			p[j][i] = (unsigned char)(255.0 / (max_val - min_val)*(in_map[j][i] - min_val));
		}
	}

	//zz_sc[2].s_d_Display(&tmp);

}
//************************************************************************
