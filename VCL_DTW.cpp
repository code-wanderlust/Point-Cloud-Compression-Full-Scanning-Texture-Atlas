#include "stdafx.h"
#include "VCL_DTW.h"

//************************************************************************
VCL_DTW::VCL_DTW()
//************************************************************************
{

}
//************************************************************************
VCL_DTW::~VCL_DTW()
//************************************************************************
{

}
//************************************************************************
template<typename T>
void VCL_DTW::dtw_Dynamic_Time_Warping(
	std::vector<T> &io_reference_texture_on_code,
	std::vector<T> &io_target_texture_on_code,
	std::vector<int> &out_warping_index,
	double &out_score)
//************************************************************************
{


}
//************************************************************************
void VCL_DTW::dtw_Dynamic_Time_Warping(
	std::vector<std::vector<double>> &in_distance_map,
	std::vector<std::vector<int>>  &out_warping_index,
	std::vector<std::vector<double>> *out_summation_distance_map_or_NULL,
	double &out_score)
//************************************************************************
{
	std::vector<std::vector<double>> D;
	std::vector<std::vector<int>> trace_back_matrix;
	std::vector<int> xp, yq;
	double min_val;
	int ww, hh, j, i, idx, sz;

	hh = in_distance_map.size();
	ww = in_distance_map[0].size();

	/*********************** Initialize ***********************/
	D                 = std::vector<std::vector<double>>(hh + 1, std::vector<double>(ww + 1, 0.0));
	trace_back_matrix = std::vector<std::vector<int>>   (hh,     std::vector<int>   (ww,     0));



	for (j = 0; j < hh+1; j++)
	{
		D[j][0] = 99999999.0;
	}

	for (i = 0; i < ww + 1; i++)
	{
		D[0][i] = 99999999.0;
	}

	for (j = 1; j < hh + 1; j++)
	{
		for (i = 1; i < ww + 1; i++)
		{
			D[j][i] = in_distance_map[j - 1][i - 1];
		}
	}
	D[0][0] = 0.0;

	/***********************************************************/

	// Calculate minimum distance. 
	//printf("hh = %d ww = %d\n", hh, ww);
	//printf("D hh = %d D ww = %d\n", D.size(), D[0].size());
	//for (j = 0; j < D.size() ; j++)
	//{
	//	for (i = 0; i < D[0].size(); i++)
	//	{
	//		printf("%lf ", D[j][i]);
	//	}
	//	printf("\n");
	//}


	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			min_val = Find_Mininum(D[j][i], D[j][i + 1], D[j + 1][i], idx);
		
			//printf("j=%d/%d i=%d/%d\n", j, hh, i, ww);
			//printf("D[j][i] : %lf, D[j][i+1] : %lf, D[j+1][i] : %lf, Min : %lf, idx : %d\n",
			//	D[j][i], D[j][i + 1], D[j + 1][i], min_val, idx);
			//Kv_Printf("Check Values");
			
			D[j+1][i+1] += min_val;
			trace_back_matrix[j][i] = idx;
		}
	}

	printf("trace_back_matrix = %d\n", trace_back_matrix[hh - 1][ww - 1]);
	//for (j = 0; j < D.size(); j++)
	//{
	//	for (i = 0; i < D[0].size(); i++)
	//	{
	//		printf("j=%d i=%d) %lf\n", j, i, D[j][i]);
	//	}
	//}
	//Kv_Printf("check again");
	
	out_score = D[hh][ww];

	// Trace-back
	i = ww - 1;
	j = hh - 1;
	xp = std::vector<int>();
	yq = std::vector<int>();

	xp.push_back(i);
	yq.push_back(j);

	while ((i > 0) && (j > 0))
	{
		idx = trace_back_matrix[j][i];
		if (idx == 0)
		{
			i--;
			j--;
		}
		else if (idx == 1)
		{
			i--;
		}
		else if (idx == 2)
		{
			j--;
		}
		else
		{
			Kv_Printf(" Invalid Track Back Index : j=%d i=%d : val=%d", j, i, idx);
		}
		
		xp.push_back(i);
		yq.push_back(j);
	}
	
	//printf("j=%d i=%d\n",j,i);
	//Kv_Printf("~");

	sz = xp.size();
	out_warping_index = std::vector<std::vector<int>>(2,std::vector<int>(sz));
	for (j = 0; j < sz; j++)
	{
		out_warping_index[0][sz-1-j] = xp[j];
		out_warping_index[1][sz-1-j] = yq[j];
	}

	if (out_summation_distance_map_or_NULL != NULL)
	{
		(*out_summation_distance_map_or_NULL) = std::vector<std::vector<double>>(hh, std::vector<double>(ww));
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				(*out_summation_distance_map_or_NULL)[j][i] = D[j + 1][i + 1];
			}
		}
	}

}
//************************************************************************
void VCL_DTW::dtw_Dynamic_Time_Warping2(
	std::vector<std::vector<double>> &in_distance_map,
	std::vector<std::vector<int>> &out_warping_index,
	std::vector<std::vector<double>> *out_summation_distance_map_or_NULL,
	double &out_score)
//************************************************************************
{
	std::vector<std::vector<double>> D;
	std::vector<int> xp, yq;
	double min_val;
	int ww, hh, j, i, idx, sz;

	hh = in_distance_map.size();
	ww = in_distance_map[0].size();

	/*********************** Initialize ***********************/
	D = std::vector<std::vector<double>>(hh, std::vector<double>(ww, 0.0));


	D[0][0] = in_distance_map[0][0];

	for (j = 1; j < hh; j++)
	{
		D[j][0] = in_distance_map[j][0] + D[j-1][0];
	}

	for (i = 1; i < ww; i++)
	{
		D[0][i] = in_distance_map[0][i] + D[0][i-1];
	}

	for (j = 1; j < hh; j++)
	{
		for (i = 1; i < ww; i++)
		{
			min_val = Find_Mininum(D[j-1][i], D[j][i-1], D[j-1][i-1], idx);
			D[j][i] = in_distance_map[j][i] + min_val;
		}
	}

	j = i = 0;
	out_warping_index = std::vector<std::vector<int>>(2,std::vector<int>());
	out_warping_index[0].push_back(i);
	out_warping_index[1].push_back(j);
	while ((j != (hh - 1)) || (i != (ww - 1)))
	{
		if (j == (hh - 1)) // if you reached to the last row (n-1) then go only one step forward in last row
		{
			i++;
		}
		else if (i == (ww - 1)) //if you have reached to the last column (m-1) then go only one step upward in last column
		{
			j++;
		}
		else
		{
			Find_Mininum(D[j + 1][i], D[j][i + 1], D[j + 1][i + 1], idx);
			if (idx == 0)
			{
				j++;
			}
			else if (idx == 1)
			{
				i++;
			}
			else
			{
				i++;
				j++;
			}
		}
		out_warping_index[0].push_back(i);
		out_warping_index[1].push_back(j);
	}

	if (out_summation_distance_map_or_NULL != NULL)
	{
		(*out_summation_distance_map_or_NULL) = std::vector<std::vector<double>>(hh, std::vector<double>(ww));
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				(*out_summation_distance_map_or_NULL)[j][i] = D[j][i];
			}
		}
	}

}
//************************************************************************
void VCL_DTW::sdtw_Simple_Dynamic_Time_Warping(
	std::vector<std::vector<double>> &in_distance_map,
	double &out_score,
	int in_mask_length)
//************************************************************************
{
	std::vector<std::vector<double>> cpy_distance_map;
	int ww, hh, j, i;
	int mask_width, dummy;

	hh = in_distance_map.size();
	ww = in_distance_map[0].size();

	mask_width = max(in_mask_length, abs(hh - ww));

	cpy_distance_map = std::vector<std::vector<double>>(hh);

	for (j = 0; j < hh; j++)
	{
		cpy_distance_map[j] = std::vector<double>(ww, DBL_MAX);
		for (i = max(1, j - mask_width); i < min(ww, j + mask_width); i++)
		{
			cpy_distance_map[j][i] = in_distance_map[j][i];
		}
	}

	for (j = 1; j < hh; j++)
	{
		for (i = max(1, j - mask_width); i < min(ww, j + mask_width); i++)
		{
			cpy_distance_map[j][i] += Find_Mininum(
										cpy_distance_map[j - 1][i],
										cpy_distance_map[j][i - 1],
										cpy_distance_map[j - 1][i - 1],
										dummy);
		}
	}

	out_score = cpy_distance_map[hh-1][ww-1];


}
//************************************************************************
int VCL_DTW::find_corner_stone_points(
	std::vector<std::vector<double>> &in_distance_map,
	int in_mask_length)
//************************************************************************
{
	int ww, hh, j, i;
	int mask_width, count=0;

	hh = in_distance_map.size();
	ww = in_distance_map[0].size();

	mask_width = max(in_mask_length, abs(hh - ww));

#define omp parallel
	{
#define omp for private(i) reduction(+:count)
		for (j = 0; j < hh; j++)
		{
			for (i = max(0, j - mask_width); i <= min(ww - 1, j + mask_width); i++)
			{
				if (in_distance_map[j][i] == 0.0)
				{
					count++;
				}
			}
		}
	}

	return count;
}
//************************************************************************
int VCL_DTW::find_corner_stone_points(
	std::vector<std::vector<int>> &in_distance_map_idx,
	int in_distance_map_hh,
	int in_distance_map_ww,
	int in_mask_length)
//************************************************************************
{
	int num_p, ww, hh, j, i, k;
	int mask_width, count = 0;

	num_p = in_distance_map_idx.size();

	hh = in_distance_map_hh;
	ww = in_distance_map_ww;

	if (num_p != hh)
	{
		Kv_Printf("Error ![find_corner_stone_points]\n");
		exit(0);
	}

	mask_width = max(in_mask_length, abs(hh - ww));

//#define omp parallel
//	{
//#define omp for private(i) reduction(+:count)
//		for (j = 0; j < num_p; j++)
//		{
//			for (i = 0; i < in_distance_map_idx[j].size(); i++)
//			{
//				if ((max(0, j - mask_width) <= in_distance_map_idx[j][i]) &&
//					(min(ww - 1, j + mask_width) >= in_distance_map_idx[j][i]))
//				{
//					count++;
//				}
//
//			}
//		}
//	}

	for (j = 0; j < num_p; j++)
	{
		for (i = 0; i < in_distance_map_idx[j].size(); i++)
		{
			if ((max(0, j - mask_width) <= in_distance_map_idx[j][i]) &&
				(min(ww - 1, j + mask_width) >= in_distance_map_idx[j][i]))
			{
				count++;
			}
			
		}
	}

	return count;
}
//************************************************************************
double VCL_DTW::Find_Mininum(
	double &in_val_down,
	double &in_val_right,
	double &in_val_diagonal,
	int &out_min_index_0_1_2)
//************************************************************************
{
	double min_val;

	if (in_val_down < in_val_right)
	{
		min_val = in_val_down;
		out_min_index_0_1_2 = 0;
	}
	else
	{
		min_val = in_val_right;
		out_min_index_0_1_2 = 1;
	}

	if (in_val_diagonal < min_val)
	{
		min_val = in_val_diagonal;
		out_min_index_0_1_2 = 2;
	}

	return min_val;
}
//************************************************************************
double VCL_DTW::Find_Max(
	std::vector<double> &in_vector,
	int *out_index_or_NULL)
//************************************************************************
{
	double val;
	int k,sz, idx;

	sz = in_vector.size();

	val = in_vector[0];
	idx = 0;

	for (k = 0; k < sz; k++)
	{
		if (val < in_vector[k])
		{
			val = in_vector[k];
			idx = k;
		}
	}

	if (out_index_or_NULL != NULL)
	{
		*out_index_or_NULL = idx;
	}

	return val;
}
//************************************************************************
//template<typename T>
//bool VCL_DTW::get_block_matrix(
//	std::vector<std::vector<T>> &in_matrix,
//	std::vector<int> &in_left_upper_xy,
//	std::vector<int> &in_right_lower_xy,
//	std::vector<std::vector<T>> &out_block_matrix)
////************************************************************************
//{
//	int ww, hh, sub_ww, sub_hh, j, i;
//
//	hh = in_matrix.size();
//	ww = in_matrix[0].size();
//
//
//	//Check X
//	if ((in_left_upper_xy[0] < 0) || 
//		(in_left_upper_xy[0] > ww - 1)||
//		(in_right_lower_xy[0] < 0) ||
//		(in_right_lower_xy[0] > ww - 1))
//	{
//		return false;
//	}
//	//Check Y
//	if ((in_left_upper_xy[1] < 0) || 
//		(in_left_upper_xy[1] > hh - 1)||
//		(in_right_lower_xy[1] < 0) ||
//		(in_right_lower_xy[1] > hh - 1))
//	{
//		return false;
//	}
//
//	sub_ww = (in_right_lower_xy[0] - in_left_upper_xy[0] + 1);
//	sub_hh = (in_right_lower_xy[1] - in_left_upper_xy[1] + 1);
//	if ((sub_ww <= 0) || (sub_hh <= 0))
//	{
//		return false;
//	}
//
//	/******************************************************************/
//	// Get Block
//	/******************************************************************/
//	out_block_matrix = std::vector<std::vector<T>>(sub_hh);
//	for (j = 0; j < sub_hh; j++)
//	{
//		out_block_matrix[j] = std::vector<T>(sub_ww);
//	}
//
//	for (j = in_left_upper_xy[1] ; j <= in_right_lower_xy[1] ; j++)
//	{
//		for (i = in_left_upper_xy[0] ; i <= in_right_lower_xy[0] ; i++)
//		{
//			out_block_matrix[j - in_left_upper_xy[1]][i - in_left_upper_xy[0]] = in_matrix[j][i];
//		}
//	}
//	/******************************************************************/
//
//
//	return true;
//}
//************************************************************************
//template<typename T>
//bool VCL_DTW::get_row(
//	std::vector<std::vector<T>> &in_matrix,
//	int in_row_index,
//	std::vector<T> &out_vector)
////************************************************************************
//{
//	int ww, hh, i;
//
//	hh = in_matrix.size();
//	ww = in_matrix[0].size();
//
//	if ((in_row_index < 0) || (in_row_index > hh-1))
//	{
//		return false;
//	}
//
//	out_vector = std::vector<T>(ww);
//#define omp parallel
//	{
//#define omp for
//		for (i = 0; i < ww; i++)
//		{
//			out_vector[i] = in_matrix[in_row_index][i];
//		}
//	}
//
//
//	return true;
//}
//************************************************************************
//template<typename T>
//bool VCL_DTW::get_column(
//	std::vector<std::vector<T>> &in_matrix,
//	int in_column_index,
//	std::vector<T> &out_vector)
////************************************************************************
//{
//	int ww, hh, j;
//
//	hh = in_matrix.size();
//	ww = in_matrix[0].size();
//
//	if ((in_column_index < 0) || (in_column_index > ww - 1))
//	{
//		return false;
//	}
//
//	out_vector = std::vector<T>(hh);
//#define omp parallel
//	{
//#define omp for
//		for (j = 0; j < hh; j++)
//		{
//			out_vector[j] = in_matrix[j][in_column_index];
//		}
//	}
//
//
//	return true;
//}
//************************************************************************
bool VCL_DTW::Is_matrix_include_true(
	std::vector<std::vector<bool>> &in_matrix)
//************************************************************************
{
	int ww, hh, j, i;

	hh = in_matrix.size();
	ww = in_matrix[0].size();

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (in_matrix[j][i]==true)
			{
				return true;
			}
		}
	}

	return false;
}
//************************************************************************
//template<typename T>
//bool VCL_DTW::Is_True_Column(
//	std::vector<std::vector<T>> &in_matrix,
//	int in_column_index,
//	int in_row_index,
//	std::vector<T> &out_vector)
////************************************************************************
//{
//	int ww, hh, j, i,count=0;
//
//	hh = in_matrix.size();
//	ww = in_matrix[0].size();
//
//	if ((in_column_index < 0) || 
//		(in_column_index > ww - 1)||
//		(in_row_index < 0) ||
//		(in_row_index > hh - 1))
//	{
//		Kv_Printf(
//			"Error : VCL_DTW : [Is_True_Column] : Out of Range : column_index = %d/%d : row_index = %d/%d\n",
//			in_column_index, ww-1, in_row_index, hh-1);
//		exit(0);
//		return false;
//	}
//
//	out_vector = std::vector<T>(hh- in_row_index);
//#define omp parallel
//	{
//#define omp for reduction(:+count)
//		for (j = in_row_index; j < hh; j++)
//		{
//			out_vector[j- in_row_index] = (in_matrix[j][in_column_index]);
//			if ((in_matrix[j][in_column_index]) != 0)
//			{
//				count++;
//			}
//		}
//	}
//
//	if (count == 0)
//	{
//		return false;
//	}
//
//	return true;
//}
//************************************************************************
//template<typename T>
//bool VCL_DTW::Is_True_Column_Full(
//	std::vector<std::vector<T>> &in_matrix,
//	int in_column_index,
//	std::vector<T> &out_vector)
////************************************************************************
//{
//	int ww, hh, j, i, count = 0;
//
//	hh = in_matrix.size();
//	ww = in_matrix[0].size();
//
//	if ((in_column_index < 0) ||
//		(in_column_index > ww - 1))
//	{
//		Kv_Printf(
//			"Error : VCL_DTW : [Is_True_Column_Full] : Out of Range : column_index = %d/%d\n",
//			in_column_index, ww - 1);
//		exit(0);
//		return false;
//	}
//
//	out_vector = std::vector<T>(hh);
//#define omp parallel
//	{
//#define omp for reduction(:+count)
//		for (j = 0; j < hh; j++)
//		{
//			out_vector[j] = (in_matrix[j][in_column_index]);
//			if ((in_matrix[j][in_column_index]) != 0)
//			{
//				count++;
//			}
//		}
//	}
//
//	if (count == 0)
//	{
//		return false;
//	}
//
//	return true;
//}
//************************************************************************
//template<typename T>
//bool VCL_DTW::Is_True_Row(
//	std::vector<std::vector<T>> &in_matrix,
//	int in_column_index,
//	int in_row_index,
//	std::vector<T> &out_vector)
////************************************************************************
//{
//	int ww, hh, j, i, count = 0;
//
//	hh = in_matrix.size();
//	ww = in_matrix[0].size();
//
//	if ((in_column_index < 0) ||
//		(in_column_index > ww - 1) ||
//		(in_row_index < 0) ||
//		(in_row_index > hh - 1))
//	{
//		Kv_Printf(
//			"Error : VCL_DTW : [Is_True_Row] : Out of Range : column_index = %d/%d : row_index = %d/%d\n",
//			in_column_index, ww - 1, in_row_index, hh - 1);
//		exit(0);
//		return false;
//	}
//
//	out_vector = std::vector<T>(ww - in_column_index);
//#define omp parallel
//	{
//#define omp for reduction(:+count)
//		for (i = in_column_index; i < ww; i++)
//		{
//			out_vector[i - in_row_index] = (in_matrix[in_column_index][i-in_row_index]);
//			if ((in_matrix[in_column_index][i- in_row_index]) != 0)
//			{
//				count++;
//			}
//		}
//	}
//
//	if (count == 0)
//	{
//		return false;
//	}
//
//	return true;
//}
//************************************************************************
//template<typename T>
//bool VCL_DTW::Is_True_Row_Full(
//	std::vector<std::vector<T>> &in_matrix,
//	int in_row_index,
//	std::vector<T> &out_vector)
////************************************************************************
//{
//	int ww, hh, j, i, count = 0;
//
//	hh = in_matrix.size();
//	ww = in_matrix[0].size();
//
//	if ((in_row_index < 0) ||
//		(in_row_index > hh - 1))
//	{
//		Kv_Printf(
//			"Error : VCL_DTW : [Is_True_Row_Full] : Out of Range : row_index = %d/%d\n",
//			in_row_index, ww - 1);
//		exit(0);
//		return false;
//	}
//
//	out_vector = std::vector<T>(ww);
//#define omp parallel
//	{
//#define omp for reduction(:+count)
//		for (i = 0; i < ww; i++)
//		{
//			out_vector[j] = (in_matrix[in_row_index][i]);
//			if ((in_matrix[in_row_index][i]) != 0)
//			{
//				count++;
//			}
//		}
//	}
//
//	if (count == 0)
//	{
//		return false;
//	}
//
//	return true;
//}
//************************************************************************









