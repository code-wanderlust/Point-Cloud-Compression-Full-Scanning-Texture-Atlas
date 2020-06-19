#pragma once
#include <vector>

class VCL_DTW
{
public:
	VCL_DTW();
	~VCL_DTW();



	template<typename T>
	void dtw_Dynamic_Time_Warping(
		std::vector<T> &io_reference_texture_on_code,
		std::vector<T> &io_target_texture_on_code,
		std::vector<int> &out_warping_index,
		double &out_score);

	void dtw_Dynamic_Time_Warping(
		std::vector<std::vector<double>> &in_distance_map,
		std::vector<std::vector<int>> &out_warping_index,
		std::vector<std::vector<double>> *out_summation_distance_map_or_NULL,
		double &out_score);

	void dtw_Dynamic_Time_Warping2(
		std::vector<std::vector<double>> &in_distance_map,
		std::vector<std::vector<int>> &out_warping_index,
		std::vector<std::vector<double>> *out_summation_distance_map_or_NULL,
		double &out_score);

	void sdtw_Simple_Dynamic_Time_Warping(
		std::vector<std::vector<double>> &in_distance_map,
		double &out_score,
		int in_mask_length);

	int find_corner_stone_points(
		std::vector<std::vector<double>> &in_distance_map,
		int in_mask_length);

	int find_corner_stone_points(
		std::vector<std::vector<int>> &in_distance_map_idx,
		int in_distance_map_hh,
		int in_distance_map_ww,
		int in_mask_length);

protected:

	/********************************************/
	double Find_Mininum(
		double &in_val_down,
		double &in_val_right,
		double &in_val_diagonal,
		int &out_min_index_0_1_2);

	double Find_Max(
		std::vector<double> &in_vector,
		int *out_index_or_NULL);
	/********************************************/

	/********************************************/
	template<typename T>
	bool get_block_matrix(
		std::vector<std::vector<T>> &in_matrix,
		std::vector<int> &in_left_upper_xy,
		std::vector<int> &in_right_lower_xy,
		std::vector<std::vector<T>> &out_block_matrix)
	{
		int ww, hh, sub_ww, sub_hh, j, i;

		hh = in_matrix.size();
		ww = in_matrix[0].size();


		//Check X
		if ((in_left_upper_xy[0] < 0) ||
			(in_left_upper_xy[0] > ww - 1) ||
			(in_right_lower_xy[0] < 0) ||
			(in_right_lower_xy[0] > ww - 1))
		{
			return false;
		}
		//Check Y
		if ((in_left_upper_xy[1] < 0) ||
			(in_left_upper_xy[1] > hh - 1) ||
			(in_right_lower_xy[1] < 0) ||
			(in_right_lower_xy[1] > hh - 1))
		{
			return false;
		}

		sub_ww = (in_right_lower_xy[0] - in_left_upper_xy[0] + 1);
		sub_hh = (in_right_lower_xy[1] - in_left_upper_xy[1] + 1);
		if ((sub_ww <= 0) || (sub_hh <= 0))
		{
			return false;
		}

		/******************************************************************/
		// Get Block
		/******************************************************************/
		out_block_matrix = std::vector<std::vector<T>>(sub_hh);
		for (j = 0; j < sub_hh; j++)
		{
			out_block_matrix[j] = std::vector<T>(sub_ww);
		}

		for (j = in_left_upper_xy[1]; j <= in_right_lower_xy[1]; j++)
		{
			for (i = in_left_upper_xy[0]; i <= in_right_lower_xy[0]; i++)
			{
				out_block_matrix[j - in_left_upper_xy[1]][i - in_left_upper_xy[0]] = in_matrix[j][i];
			}
		}
		/******************************************************************/


		return true;
	}

	template<typename T>
	bool get_row(
		std::vector<std::vector<T>> &in_matrix,
		int in_row_index,
		std::vector<T> &out_vector)
	{
		int ww, hh, i;

		hh = in_matrix.size();
		ww = in_matrix[0].size();

		if ((in_row_index < 0) || (in_row_index > hh - 1))
		{
			return false;
		}

		out_vector = std::vector<T>(ww);
//#define omp parallel
//		{
//#define omp for
			for (i = 0; i < ww; i++)
			{
				out_vector[i] = in_matrix[in_row_index][i];
//			}
		}


		return true;
	}

	template<typename T>
	bool get_column(
		std::vector<std::vector<T>> &in_matrix,
		int in_column_index,
		std::vector<T> &out_vector)
	{
		int ww, hh, j;

		hh = in_matrix.size();
		ww = in_matrix[0].size();

		if ((in_column_index < 0) || (in_column_index > ww - 1))
		{
			return false;
		}

		out_vector = std::vector<T>(hh);
//#define omp parallel
//		{
//#define omp for
			for (j = 0; j < hh; j++)
			{
				out_vector[j] = in_matrix[j][in_column_index];
			}
//		}


		return true;
	}
	/********************************************/
	
	/********************************************/
	bool Is_matrix_include_true(
		std::vector<std::vector<bool>> &in_matrix);
	/********************************************/

	/********************************************/
	template<typename T>
	bool Is_True_Column(
		std::vector<std::vector<T>> &in_matrix,
		int in_column_index,
		int in_row_index,
		std::vector<T> &out_vector)
	{
		int ww, hh, j, i, count = 0;

		hh = in_matrix.size();
		ww = in_matrix[0].size();

		if ((in_column_index < 0) ||
			(in_column_index > ww - 1) ||
			(in_row_index < 0) ||
			(in_row_index > hh - 1))
		{
			Kv_Printf(
				"Error : VCL_DTW : [Is_True_Column] : Out of Range : column_index = %d/%d : row_index = %d/%d\n",
				in_column_index, ww - 1, in_row_index, hh - 1);
			exit(0);
			return false;
		}

		out_vector = std::vector<T>(hh - in_row_index);
//#define omp parallel
//		{
//#define omp for reduction(:+count)
			for (j = in_row_index; j < hh; j++)
			{
				out_vector[j - in_row_index] = (in_matrix[j][in_column_index]);
				if ((in_matrix[j][in_column_index]) != 0)
				{
					count++;
				}
			}
//		}

		if (count == 0)
		{
			return false;
		}

		return true;
	}


	template<typename T>
	bool Is_True_Column_Full(
		std::vector<std::vector<T>> &in_matrix,
		int in_column_index,
		std::vector<T> &out_vector)
	{
		int ww, hh, j, i, count = 0;

		hh = in_matrix.size();
		ww = in_matrix[0].size();

		if ((in_column_index < 0) ||
			(in_column_index > ww - 1))
		{
			Kv_Printf(
				"Error : VCL_DTW : [Is_True_Column_Full] : Out of Range : column_index = %d/%d\n",
				in_column_index, ww - 1);
			exit(0);
			return false;
		}

		out_vector = std::vector<T>(hh);
//#define omp parallel
//		{
//#define omp for reduction(:+count)
			for (j = 0; j < hh; j++)
			{
				out_vector[j] = (in_matrix[j][in_column_index]);
				if ((in_matrix[j][in_column_index]) != 0)
				{
					count++;
				}
			}
//		}

		if (count == 0)
		{
			return false;
		}

		return true;
	}
	/********************************************/

	/********************************************/
	template<typename T>
	bool Is_True_Row(
		std::vector<std::vector<T>> &in_matrix,
		int in_column_index,
		int in_row_index,
		std::vector<T> &out_vector)
	{
		int ww, hh, j, i, count = 0;

		hh = in_matrix.size();
		ww = in_matrix[0].size();

		if ((in_column_index < 0) ||
			(in_column_index > ww - 1) ||
			(in_row_index < 0) ||
			(in_row_index > hh - 1))
		{
			Kv_Printf(
				"Error : VCL_DTW : [Is_True_Row] : Out of Range : column_index = %d/%d : row_index = %d/%d\n",
				in_column_index, ww - 1, in_row_index, hh - 1);
			exit(0);
			return false;
		}

		out_vector = std::vector<T>(ww - in_column_index);
//#define omp parallel
//		{
//#define omp for reduction(:+count)
			for (i = in_column_index; i < ww; i++)
			{
				out_vector[i - in_row_index] = (in_matrix[in_column_index][i - in_row_index]);
				if ((in_matrix[in_column_index][i - in_row_index]) != 0)
				{
					count++;
				}
			}
//		}

		if (count == 0)
		{
			return false;
		}

		return true;
	}

	template<typename T>
	bool Is_True_Row_Full(
		std::vector<std::vector<T>> &in_matrix,
		int in_row_index,
		std::vector<T> &out_vector)
	{
		int ww, hh, j, i, count = 0;

		hh = in_matrix.size();
		ww = in_matrix[0].size();

		if ((in_row_index < 0) ||
			(in_row_index > hh - 1))
		{
			Kv_Printf(
				"Error : VCL_DTW : [Is_True_Row_Full] : Out of Range : row_index = %d/%d\n",
				in_row_index, ww - 1);
			exit(0);
			return false;
		}

		out_vector = std::vector<T>(ww);
//#define omp parallel
//		{
//#define omp for reduction(:+count)
			for (i = 0; i < ww; i++)
			{
				out_vector[i] = (in_matrix[in_row_index][i]);
				if ((in_matrix[in_row_index][i]) != 0)
				{
					count++;
				}
			}
//		}

		if (count == 0)
		{
			return false;
		}

		return true;
	}
	/********************************************/

};