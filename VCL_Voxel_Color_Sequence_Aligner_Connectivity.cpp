#include "stdafx.h"
#include "VCL_Voxel_Color_Sequence_Aligner_Connectivity.h"


//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Connectivity::VCL_Voxel_Color_Sequence_Aligner_Connectivity()
//************************************************************************
{

}
//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Connectivity::~VCL_Voxel_Color_Sequence_Aligner_Connectivity()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Connectivity::acc_Align_Texture_on_Codes_Using_Connectivity(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec;
	int num_blob, num_toc, max_length, max_idx, current_size, m, n;
	int *p_v;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{

		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

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

		for (n = max_idx - 1; n > -1; n--)
		{
			atc_Align_Two_Texture_on_Codes_Using_Connectivity3(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				max_length,//int in_max_length,
				p_v[n]);//int &out_offset);
			//atc_Align_Two_Texture_on_Codes_Using_Connectivity3(
			//	&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
			//	io_segmented_texture_on_code[m][n+1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	max_length,//int in_max_length,
			//	p_v[n]);//int &out_offset);
		}


		for (n = max_idx + 1; n < num_toc; n++)
		{
			atc_Align_Two_Texture_on_Codes_Using_Connectivity3(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				max_length,//int in_max_length,
				p_v[n]);//int &out_offset);

			//atc_Align_Two_Texture_on_Codes_Using_Connectivity3(
			//	&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
			//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	max_length,//int in_max_length,
			//	p_v[n]);//int &out_offset);
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Connectivity::atc_Align_Two_Texture_on_Codes_Using_Connectivity(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int in_scanning_direction,
	int &out_offset)
//************************************************************************
{
	LCKvAlgebra_for_Matrix lam;
	CKvMatrixInt corres_idx; CKvMatrix homography;
	CKvVector b, np;
	std::vector<int> tmp;
	std::vector<bool> flag_vec;
	CKvVoxel *p_voxel;
	int sz, num_voxel_in_target, num_voxel_in_reference, idx, k, m;
	int **p_corres, count, rest_points;
	float min_dist, dist;
	bool case_flag = false;

	sz = in_reference_texture_on_code.size();
	num_voxel_in_reference = 0;

	for (k = 0; k < sz; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			num_voxel_in_reference++;
		}
	}
	num_voxel_in_target = io_target_texture_on_code.size();
	tmp = std::vector<int>(sz, -1);
	p_voxel = in_set_of_voxels->vp();

	count = 0;
	if (num_voxel_in_reference <= num_voxel_in_target)
		// Reference에 있는 모든 점들은 대응점을 가진다. 즉, Target에 있는 일부 점들은 대응점을 찾을 수 없어서 후 처리를 해야함. 
	{
		p_corres = corres_idx.c_Create(num_voxel_in_reference, 2);
		flag_vec = std::vector<bool>(num_voxel_in_target, false);

		for (k = 0; k < sz; k++) // in_reference_texture_on_code loop
		{
			if (in_reference_texture_on_code[k] != -1)
			{
				min_dist = 99999999999999.f;
				for (m = 0; m < num_voxel_in_target; m++) // io_target_texture_on_code loop
				{
					dist = p_voxel[in_reference_texture_on_code[k]].ds_Distance_Squared(p_voxel[io_target_texture_on_code[m]]);
					if ((min_dist > dist) && (flag_vec[m] == false))
					{
						min_dist = dist;
						idx = m;
					}
				}

				flag_vec[idx] = true;
				p_corres[count][0] = k;   // Reference
				p_corres[count][1] = idx; // Target
				count++;
			}
		}

		rest_points = num_voxel_in_target - count;
	}
	else
		// Target에 있는 모든 점들은 대응점을 가진다. -> 후 처리 단계는 필요 없음. 
	{
		p_corres = corres_idx.c_Create(num_voxel_in_target, 2);
		flag_vec = std::vector<bool>(sz, false);
		for (m = 0; m < num_voxel_in_target; m++) // io_target_texture_on_code loop
		{
			min_dist = 99999999999999.f;
			for (k = 0; k < sz; k++) // in_reference_texture_on_code loop
			{
				if (in_reference_texture_on_code[k] != -1)
				{
					dist = p_voxel[in_reference_texture_on_code[k]].ds_Distance_Squared(p_voxel[io_target_texture_on_code[m]]);
					if ((min_dist > dist) && (flag_vec[k] == false))
					{
						min_dist = dist;
						idx = k;
					}
				}
			}
			flag_vec[idx] = true;
			p_corres[count][0] = idx; // Reference
			p_corres[count][1] = m;   // Target
			count++;
		}

		rest_points = num_voxel_in_target - count;
	}

	int dim, num_point;

	p_corres = corres_idx.mps(dim, num_point);
	flag_vec = std::vector<bool>(io_target_texture_on_code.size(), false);
	for (k = 0; k < num_point; k++)
	{
		tmp[p_corres[k][0]] = io_target_texture_on_code[p_corres[k][1]];
		flag_vec[p_corres[k][1]] = true;
	}

	// 다뤄지지 않은 voxel들 처리과정이 필요함. 
	//if (rest_points != 0)
	//{
	//	std::vector<int> rest_voxels;

	//	for (k = 0; k < flag_vec.size(); k++)
	//	{
	//		if (flag_vec[k] == false)
	//		{
	//			rest_voxels.push_back(io_target_texture_on_code[k]);
	//		}
	//	}

	//	for (k = 0; k < rest_voxels.size(); k++)
	//	{
	//		min_dist = 99999999999999.f;
	//		for (m = 0; m < in_reference_texture_on_code.size(); m++)
	//		{
	//			if (in_reference_texture_on_code[m] != -1)
	//			{
	//				dist = p_voxel[in_reference_texture_on_code[m]].ds_Distance_Squared(p_voxel[rest_voxels[k]]);
	//				if (min_dist > dist)
	//				{
	//					min_dist = dist;
	//					idx = m;
	//				}
	//			}
	//		}

	//		min_dist = 99999999999999.f;
	//		for (m = 0; m < tmp.size(); m++)
	//		{
	//			if (tmp[m] == -1)
	//			{
	//				dist = abs(m - idx);
	//				if (min_dist > dist)
	//				{
	//					min_dist = dist;
	//					idx2 = m;
	//				}
	//			}
	//		}
	//		tmp[idx2] = rest_voxels[k];
	//	}
	//}

	io_target_texture_on_code = tmp;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Connectivity::atc_Align_Two_Texture_on_Codes_Using_Connectivity2(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int in_scanning_direction,
	int &out_offset)
//************************************************************************
{
	std::vector<std::vector<int>> table;
	std::vector<int> tmp;
	std::vector<std::vector<int>> arrange_table;
	std::vector<bool> flag_for_target;
	CKvSet_of_Vector dist_vectors;
	CKvVector *p_distv;
	CKvVectorInt minimum_indices;
	CKvVoxel *p_voxel;
	int *p_indices, num_voxel_in_target, sz, k, m;
	double *p_double;

	sz = in_reference_texture_on_code.size();
	num_voxel_in_target = io_target_texture_on_code.size();
	p_voxel = in_set_of_voxels->vp();

	p_distv = dist_vectors.c_Create(num_voxel_in_target);
	for (k = 0; k < num_voxel_in_target; k++)
	{
		p_double = p_distv[k].c_Create(sz, 999999999.0);
		for (m = 0; m < sz; m++)
		{
			if (in_reference_texture_on_code[m] != -1)
			{
				p_double[m] = p_voxel[in_reference_texture_on_code[m]].ds_Distance_Squared(p_voxel[io_target_texture_on_code[k]]);
			}
		}
	}

	table = std::vector<std::vector<int>>(num_voxel_in_target);
	flag_for_target = std::vector<bool>(num_voxel_in_target, false);
	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (Find_Minimums(
			&p_distv[k],//CKvVector *in_vector,
			999999.0,//double in_threshold_for_distance,
			&minimum_indices))//CKvVectorInt *out_mininum_indices)
		{
			flag_for_target[k] = true;
			p_indices = minimum_indices.vp(); //indices in reference
			for (m = 0; m < minimum_indices.vs(); m++)
			{
				table[k].push_back(p_indices[m]);
			}
		}
	}

	std::vector<int>  hist = std::vector<int>(sz, 0);
	arrange_table = std::vector<std::vector<int>>(sz);
	int min_idx, min_bin = sz;
	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (table[k].size() == 1)
		{
			arrange_table[table[k][0]].push_back(io_target_texture_on_code[k]);
			hist[table[k][0]]++;
		}
	}

	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (table[k].size() > 1)
		{
			min_bin = sz;
			for (m = 0; m < table[k].size(); m++)
			{
				if (min_bin > hist[table[k][m]])
				{
					min_bin = hist[table[k][m]];
					min_idx = m;
				}
			}
			arrange_table[table[k][min_idx]].push_back(io_target_texture_on_code[k]);
			hist[table[k][min_idx]]++;
		}
	}

	Find_Texture_on_Codes_From_Arrange_Table(
		arrange_table,//std::vector<std::vector<int>> &in_arrange_table,
		tmp);//std::vector<int> &out_texture_on_code);
	io_target_texture_on_code = tmp;

	//if (!Find_Texture_on_Codes_From_Arrange_Table2(
	//	arrange_table,//std::vector<std::vector<int>> &in_arrange_table,
	//	io_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
	//	flag_for_target,//std::vector<bool> &in_flag_for_target_texture_on_code,
	//	tmp))//std::vector<int> &out_texture_on_code)
	//{
	//	tmp = std::vector<int>(sz, -1);
	//	for (m = 0; m < io_target_texture_on_code.size(); m++)
	//	{
	//		tmp[m] = io_target_texture_on_code[m];
	//	}
	//}
	//else
	//{
	//	io_target_texture_on_code = tmp;
	//}

	printf("Modified Size : %d\n", io_target_texture_on_code.size());
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Connectivity::atc_Align_Two_Texture_on_Codes_Using_Connectivity3(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int in_scanning_direction,
	int &out_offset)
//************************************************************************
{
	std::vector<std::vector<int>> table;
	std::vector<int> tmp, hist;
	std::vector<std::vector<int>> arrange_table;
	std::vector<bool> flag_for_target;
	CKvSet_of_Vector dist_vectors;
	CKvVectorInt minimum_indices;
	int *p_indices, num_voxel_in_target, sz, k, m;

	sz = in_reference_texture_on_code.size();
	num_voxel_in_target = io_target_texture_on_code.size();

	table = std::vector<std::vector<int>>(num_voxel_in_target);
	flag_for_target = std::vector<bool>(num_voxel_in_target, false);
	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (Find_Candidate_of_Corresponds(
			in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
			in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
			io_target_texture_on_code[k],//int in_index_of_target_voxel,
			&minimum_indices))//CKvVectorInt *out_mininum_indices);
		{
			flag_for_target[k] = true;
			p_indices = minimum_indices.vp();
			for (m = 0; m < minimum_indices.vs(); m++)
			{
				table[k].push_back(p_indices[m]);
			}
		}
	}



	hist = std::vector<int>(sz, 0);
	arrange_table = std::vector<std::vector<int>>(sz);
	int min_idx, min_bin = sz;
	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (table[k].size() == 1)
		{
			arrange_table[table[k][0]].push_back(io_target_texture_on_code[k]);
			hist[table[k][0]]++;
		}
	}

	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (table[k].size() > 1)
		{
			min_bin = sz;
			for (m = 0; m < table[k].size(); m++)
			{
				if (min_bin > hist[table[k][m]])
				{
					min_bin = hist[table[k][m]];
					min_idx = m;
				}
			}
			arrange_table[table[k][min_idx]].push_back(io_target_texture_on_code[k]);
			hist[table[k][min_idx]]++;
		}
	}

	//Find_Texture_on_Codes_From_Arrange_Table(
	//	arrange_table,//std::vector<std::vector<int>> &in_arrange_table,
	//	tmp);//std::vector<int> &out_texture_on_code);
	//io_target_texture_on_code = tmp;

	if (!Find_Texture_on_Codes_From_Arrange_Table2(
		arrange_table,//std::vector<std::vector<int>> &in_arrange_table,
		io_target_texture_on_code,//std::vector<int> &in_target_texture_on_code,
		flag_for_target,//std::vector<bool> &in_flag_for_target_texture_on_code,
		tmp))//std::vector<int> &out_texture_on_code)
	{
		tmp = std::vector<int>(sz, -1);
		for (m = 0; m < io_target_texture_on_code.size(); m++)
		{
			tmp[m] = io_target_texture_on_code[m];
		}
		io_target_texture_on_code = tmp;
	}
	else
	{
		io_target_texture_on_code = tmp;
	}
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Connectivity::atc_Align_Two_Texture_on_Codes_Using_Connectivity4(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int in_scanning_direction,
	int &out_offset)
//************************************************************************
{
	std::vector<std::vector<int>> table;
	std::vector<int> tmp;
	std::vector<std::vector<int>> arrange_table;
	std::vector<bool> flag_for_target, flag_for_reference;
	CKvSet_of_Vector dist_vectors;
	CKvVectorInt minimum_indices;
	int *p_indices, num_voxel_in_target, sz, k, m;

	sz = in_reference_texture_on_code.size();
	num_voxel_in_target = io_target_texture_on_code.size();

	table = std::vector<std::vector<int>>(num_voxel_in_target);
	flag_for_target = std::vector<bool>(num_voxel_in_target, false);
	flag_for_reference = std::vector<bool>(sz, false);

	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (Find_Candidate_of_Corresponds_In_Z_Slice(
			in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
			in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
			io_target_texture_on_code[k],//int in_index_of_target_voxel,
			&minimum_indices))//CKvVectorInt *out_mininum_indices);
		{
			flag_for_target[k] = true;
			p_indices = minimum_indices.vp();
			flag_for_reference[p_indices[0]] = true;
			for (m = 0; m < minimum_indices.vs(); m++)
			{
				table[k].push_back(p_indices[m]); // table[k] 에는 in_reference_texture_on_code 의 인덱스가 있고 크기는 io_target임. 
			}
		}
	}

	//for (k = 0; k < num_voxel_in_target; k++)
	//{
	//	if (flag_for_target[k] == false)
	//	{
	//		if (Find_Candidate_of_Corresponds_In_Z_Slice(
	//			in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
	//			in_reference_texture_on_code,//std::vector<int> &in_reference_texture_on_code,
	//			io_target_texture_on_code[k],//int in_index_of_target_voxel,
	//			&minimum_indices))//CKvVectorInt *out_mininum_indices);
	//		{
	//			p_indices = minimum_indices.vp();
	//			if (flag_for_reference[p_indices[0]] == false)
	//			{
	//				flag_for_target[k] = true;
	//				//p_indices = minimum_indices.vp();
	//				flag_for_reference[p_indices[0]] = true;
	//				for (m = 0; m < minimum_indices.vs(); m++)
	//				{
	//					table[k].push_back(p_indices[m]); // table[k] 에는 in_reference_texture_on_code 의 인덱스가 있고 크기는 io_target임. 
	//				}
	//			}

	//		}
	//	}
	//}


	tmp = std::vector<int>(sz, -1);
	for (k = 0; k < num_voxel_in_target; k++)
	{
		if (table[k].size() != 0 && flag_for_target[k])
		{
			tmp[table[k][0]] = io_target_texture_on_code[k];
		}
	}



	io_target_texture_on_code = tmp;

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Connectivity::Find_Texture_on_Codes_From_Arrange_Table(
	std::vector<std::vector<int>> &in_arrange_table,
	std::vector<int> &out_texture_on_code)
//************************************************************************
{
	int current_idx, before_idx, after_idx, sz, k, m, j;

	sz = in_arrange_table.size();

	out_texture_on_code = std::vector<int>(sz, -1);
	for (k = 0; k < sz; k++)
	{
		if (in_arrange_table[k].size() != 0)
		{
			out_texture_on_code[k] = in_arrange_table[k][0];
		}
	}

	for (k = 0; k < sz; k++)
	{
		if (in_arrange_table[k].size() > 1)
		{
			current_idx = k;
			for (m = 1; m < in_arrange_table[k].size(); m++)
			{
				before_idx = 1000 * sz;
				for (j = 0; j < current_idx; j++)
				{
					if (out_texture_on_code[j] == -1)
					{
						before_idx = j;
					}
				}

				after_idx = 1000 * sz;
				for (j = sz - 1; j > current_idx; j--)
				{
					if (out_texture_on_code[j] == -1)
					{
						after_idx = j;
					}
				}

				if (abs(current_idx - before_idx) < abs(current_idx - after_idx))
				{
					// out_texture_on_code 에서 current_idx 에서 바로 왼쪽 방향으로 in_arrange_table[k][m]을 삽입 후, 왼쪽 방향에 있는 -1 데이터 삭제.
					out_texture_on_code.erase(out_texture_on_code.begin() + before_idx);
					out_texture_on_code.insert(out_texture_on_code.begin() + current_idx, in_arrange_table[k][m]);
					//out_texture_on_code.insert(out_texture_on_code.begin() + current_idx-1, in_arrange_table[k][m]);

				}
				else
				{
					// out_texture_on_code 에서 current_idx 에서 바로 오른쪽 방향으로 in_arrange_table[k][m]을 삽입 후, 오른쪽 방향에 있는 -1 데이터 삭제. 
					out_texture_on_code.erase(out_texture_on_code.begin() + after_idx);
					out_texture_on_code.insert(out_texture_on_code.begin() + current_idx + 1, in_arrange_table[k][m]);
				}

			}
		}
	}



}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Connectivity::Find_Texture_on_Codes_From_Arrange_Table2(
	std::vector<std::vector<int>> &in_arrange_table,
	std::vector<int> &in_target_texture_on_code,
	std::vector<bool> &in_flag_for_target_texture_on_code,
	std::vector<int> &out_texture_on_code)
//************************************************************************
{
	int current_idx, before_idx, after_idx, _idx, Location, sz, k, m, j;
	int num_element_in_target, num_true_element;
	int flag;

	num_true_element = 0;
	num_element_in_target = in_flag_for_target_texture_on_code.size();
	for (k = 0; k < num_element_in_target; k++)
	{
		if (in_flag_for_target_texture_on_code[k] == true)
		{
			num_true_element++;
		}
	}
	if (num_true_element == 0)
	{
		return false;
	}

	sz = in_arrange_table.size();

	out_texture_on_code = std::vector<int>(sz, -1);
	for (k = 0; k < sz; k++)
	{
		if (in_arrange_table[k].size() != 0)
		{
			out_texture_on_code[k] = in_arrange_table[k][0];
		}
	}

	for (k = 0; k < sz; k++)
	{
		if (in_arrange_table[k].size() > 1)
		{
			current_idx = k;
			for (m = 1; m < in_arrange_table[k].size(); m++)
			{
				before_idx = 1000 * sz;
				for (j = 0; j < current_idx; j++)
				{
					if (out_texture_on_code[j] == -1)
					{
						before_idx = j;
					}
				}

				after_idx = 1000 * sz;
				for (j = sz - 1; j > current_idx; j--)
				{
					if (out_texture_on_code[j] == -1)
					{
						after_idx = j;
					}
				}

				if (abs(current_idx - before_idx) < abs(current_idx - after_idx))
				{
					// out_texture_on_code 에서 current_idx 에서 바로 왼쪽 방향으로 in_arrange_table[k][m]을 삽입 후, 왼쪽 방향에 있는 -1 데이터 삭제.
					out_texture_on_code.erase(out_texture_on_code.begin() + before_idx);
					out_texture_on_code.insert(out_texture_on_code.begin() + current_idx, in_arrange_table[k][m]);
					//out_texture_on_code.insert(out_texture_on_code.begin() + current_idx-1, in_arrange_table[k][m]);

				}
				else
				{
					// out_texture_on_code 에서 current_idx 에서 바로 오른쪽 방향으로 in_arrange_table[k][m]을 삽입 후, 오른쪽 방향에 있는 -1 데이터 삭제. 
					out_texture_on_code.erase(out_texture_on_code.begin() + after_idx);
					out_texture_on_code.insert(out_texture_on_code.begin() + current_idx + 1, in_arrange_table[k][m]);
				}
			}
		}
	}


	while (1)
	{
		for (k = 0; k < num_element_in_target; k++)
		{

			flag = 0;
			if (in_flag_for_target_texture_on_code[k] == false)
			{
				/*****************************************************************************************************/
				if (k == 0)
				{
					before_idx = 1000 * sz;
					after_idx = 1;
				}
				else if (k == num_element_in_target - 1)
				{
					before_idx = num_element_in_target - 2;
					after_idx = 1000 * sz;
				}
				else
				{
					before_idx = k - 1;
					after_idx = k + 1;
				}
				/*****************************************************************************************************/

				/*****************************************************************************************************/
				flag = 0;
				if ((before_idx != 1000 * sz) && (in_flag_for_target_texture_on_code[before_idx]) == true)
				{
					_idx = before_idx;
					flag = 1;
				}
				else if ((after_idx != 1000 * sz) && (in_flag_for_target_texture_on_code[after_idx]) == true)
				{
					_idx = after_idx;
					flag = -1;
				}
				/*****************************************************************************************************/

				/*****************************************************************************************************/
				if (flag == 0)
				{
					continue;
				}
				/*****************************************************************************************************/

				/*****************************************************************************************************/
				for (j = 0; j < sz; j++)
				{
					if (in_target_texture_on_code[_idx] == out_texture_on_code[j])
					{
						Location = j;
						break;
					}
				}
				/*****************************************************************************************************/

				/*****************************************************************************************************/
				if (flag == 1)
				{
					out_texture_on_code.insert(out_texture_on_code.begin() + Location, in_target_texture_on_code[_idx]);

					before_idx = 1000 * sz;
					for (j = 0; j < Location; j++)
					{
						if (out_texture_on_code[j] == -1)
						{
							before_idx = j;
						}
					}
					after_idx = 1000 * sz;
					for (j = out_texture_on_code.size() - 1; j > Location; j--)
					{
						if (out_texture_on_code[j] == -1)
						{
							after_idx = j;
						}
					}

					if (abs(Location - before_idx) < abs(Location - after_idx))
					{
						out_texture_on_code.erase(out_texture_on_code.begin() + before_idx);
					}
					else
					{
						out_texture_on_code.erase(out_texture_on_code.begin() + after_idx);
					}

				}
				else if (flag == -1)
				{
					// Insert Data
					if (Location == (sz - 1))
					{
						out_texture_on_code.push_back(in_target_texture_on_code[_idx]);
					}
					else
					{
						out_texture_on_code.insert(out_texture_on_code.begin() + Location + 1, in_target_texture_on_code[_idx]);
					}

					//Find Empty Index
					before_idx = 1000 * sz;
					for (j = 0; j < Location + 1; j++)
					{
						if (out_texture_on_code[j] == -1)
						{
							before_idx = j;
						}
					}
					after_idx = 1000 * sz;
					for (j = out_texture_on_code.size() - 1; j > Location + 1; j--)
					{
						if (out_texture_on_code[j] == -1)
						{
							after_idx = j;
						}
					}

					//Delete Empty Index
					if (abs(Location - before_idx) < abs(Location - after_idx))
					{
						out_texture_on_code.erase(out_texture_on_code.begin() + before_idx);
					}
					else
					{
						out_texture_on_code.erase(out_texture_on_code.begin() + after_idx);
					}
				}
				/*****************************************************************************************************/
				in_flag_for_target_texture_on_code[k] = true;
			}
		}

		num_true_element = 0;
		for (k = 0; k < num_element_in_target; k++)
		{
			if (in_flag_for_target_texture_on_code[k] == true)
			{
				num_true_element++;
			}
		}
		if (num_true_element == num_element_in_target)
		{
			break;
		}
	}

	return true;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Connectivity::eh_Estimate_Homography(
	CKvMatrixInt *in_corresponding_idx,
	bool true_forward_false_inverse,
	CKvMatrix *out_homography)
//************************************************************************
{
	LCKvUtility_for_Linear_Equation lule;
	CKvMatrix A, H; CKvVector b, x;
	double **p_A, *p_b, *p_x, **p_homo;
	int **p_corres;
	int dim, num_p, k;

	p_corres = in_corresponding_idx->mps(dim, num_p);
	for (k = 0; k < num_p; k++)
	{
		printf("%d %d\n", p_corres[k][0], p_corres[k][1]);
	}

	p_A = A.c_Create(num_p * 2, 4, 0.0);
	p_b = b.c_Create(num_p * 2, 1.0);

	for (k = 0; k < num_p; k++)
	{
		p_A[(2 * k)][0] = p_corres[k][0];
		p_A[(2 * k)][1] = 1.0;

		p_A[(2 * k) + 1][2] = p_corres[k][0];
		p_A[(2 * k) + 1][3] = 1.0;

		p_b[2 * k] = p_corres[k][1];

	}

	lule.ssv_Solve_based_on_SVD(&A, 0.01, &b, &x);

	p_x = x.vp();
	p_homo = H.c_Create(2, 2);

	p_homo[0][0] = p_x[0];
	p_homo[0][1] = p_x[1];
	p_homo[1][0] = p_x[2];
	p_homo[1][1] = p_x[3];


	if (true_forward_false_inverse == false)
	{
		lule.pisv_Pseudo_Inverse_matrix_based_on_SVD(&H, 0.001, out_homography);
	}
	else
	{
		out_homography->cp_Copy(&H);
	}

}
//************************************************************************

