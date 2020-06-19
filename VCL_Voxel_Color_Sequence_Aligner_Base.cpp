#include "stdafx.h"
#include "VCL_Voxel_Color_Sequence_Aligner_Base.h"


//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Base::VCL_Voxel_Color_Sequence_Aligner_Base()
//************************************************************************
{

}
//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Base::~VCL_Voxel_Color_Sequence_Aligner_Base()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::input_random_number(
	int &in_random_number)
//************************************************************************
{
	zz_rn_for_align = in_random_number;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::sv_Circular_Shift_Vector(
	int in_location,
	std::vector<int> &io_vector)
//************************************************************************
{
	std::vector<int> tmp;
	int sz, count, k;

	if (in_location == 0) { return; }

	tmp = io_vector;
	sz = tmp.size();
	count = 0;
	for (k = in_location; k < sz; k++)
	{
		tmp[count] = io_vector[k];
		count++;
	}

	for (k = 0; k < in_location; k++)
	{
		tmp[count] = io_vector[k];
		count++;
	}

	io_vector = tmp;

	return;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::sv_Circular_Shift_Vector(
	int in_location,
	CKvVectorUcharYCbCr *io_vector)
//************************************************************************
{
	CKvVectorUcharYCbCr tmp;
	int sz, count, k;
	unsigned char *py_io, *pcb_io, *pcr_io;
	unsigned char *py, *pcb, *pcr;

	if (in_location == 0) { return; }


	tmp.cp_Copy(io_vector);
	sz = tmp.vs();
	py = tmp.vp(pcb, pcr);
	py_io = io_vector->vp(pcb_io, pcr_io);

	count = 0;
	for (k = in_location; k < sz; k++)
	{
		py[count] = py_io[k];
		pcb[count] = pcb_io[k];
		pcr[count] = pcr_io[k];
		count++;
	}

	for (k = 0; k < in_location; k++)
	{
		py[count] = py_io[k];
		pcb[count] = pcb_io[k];
		pcr[count] = pcr_io[k];
		count++;
	}

	io_vector->cp_Copy(&tmp);
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::sv_Circular_Shift_Vector(
	int in_location,
	CKvSet_of_RgbaF *io_vector)
//************************************************************************
{
	CKvSet_of_RgbaF tmp;
	int sz, count, k;
	CKvRgbaF *p1, *p2;

	if (in_location == 0) { return; }

	tmp.cp_Copy(io_vector);
	sz = tmp.vs();

	p1 = tmp.vp();
	p2 = io_vector->vp();

	count = 0;
	for (k = in_location; k < sz; k++)
	{
		p1[count] = p2[k];
		count++;
	}

	for (k = 0; k < in_location; k++)
	{
		p1[count] = p2[k];
		count++;
	}

	io_vector->cp_Copy(&tmp);
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::Find_Mean(
	std::vector<int> &in_ToC,
	CKvSet_of_Voxel *in_set_of_voxel,
	CKvPoint3D *out_mean_point)
//************************************************************************
{
	CKvVoxel *p_set_voxel;
	int sz, k;
	double xx, yy, zz;

	sz = in_ToC.size();

	p_set_voxel = in_set_of_voxel->vp();
	xx = yy = zz = 0.0;
	for (k = 0; k < sz; k++)
	{
		xx += (double)p_set_voxel[in_ToC[k]].x;
		yy += (double)p_set_voxel[in_ToC[k]].y;
		zz += (double)p_set_voxel[in_ToC[k]].z;
	}

	xx /= (double)sz;
	yy /= (double)sz;
	zz /= (double)sz;

	out_mean_point->x = xx;
	out_mean_point->y = yy;
	out_mean_point->z = zz;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::Find_Minimums(
	CKvVector *in_vector,
	CKvVectorInt *out_mininum_indices)
//************************************************************************
{
	CKvXvectorInt tmp;
	double *p_vec, min;
	int num, k, count = 0;

	p_vec = in_vector->vps(num);
	tmp.in_Initialize(num);
	min = DBL_MAX;
	for (k = 0; k < num; k++)
	{
		if (p_vec[k] < min)
		{
			min = p_vec[k];
		}
	}

	for (k = 0; k < num; k++)
	{
		if (p_vec[k] == min)
		{
			tmp.ae_Append_Element(k);
			count++;
		}
	}

	if (count != 0)
	{
		tmp.z_cne(count);
	}
	else
	{
		Kv_Printf("Error");
		return;
	}

	tmp.ex_Export(out_mininum_indices, NULL);

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::Find_Minimums(
	std::vector<double> &in_vector,
	std::vector<int> &out_mininum_indices)
//************************************************************************
{
	double *p_vec, min;
	int num, k, idx;

	out_mininum_indices = std::vector<int>();
	num = in_vector.size();
	min = DBL_MAX;
	for (k = 0; k < num; k++)
	{
		if (in_vector[k] < min)
		{
			min = in_vector[k];
			idx = k;
		}
	}
	out_mininum_indices.push_back(idx);


	//for (k = 0; k < num; k++)
	//{
	//	if (in_vector[k] == min)
	//	{
	//		out_mininum_indices.push_back(k);
	//	}
	//}

}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Minimums(
	CKvVector *in_vector,
	double in_threshold_for_distance,
	CKvVectorInt *out_mininum_indices)
//************************************************************************
{
	CKvXvectorInt tmp;
	double *p_vec, min;
	int num, k, count = 0;

	p_vec = in_vector->vps(num);
	tmp.in_Initialize(num);
	min = DBL_MAX;
	//min = 9999999999999999999999999.0;
	for (k = 0; k < num; k++)
	{
		if (p_vec[k] < min)
		{
			min = p_vec[k];
		}
	}

	if (in_threshold_for_distance < min)
	{
		return false;
	}

	for (k = 0; k < num; k++)
	{
		if (p_vec[k] == min)
		{
			tmp.ae_Append_Element(k);
			count++;
		}
	}

	tmp.ex_Export(out_mininum_indices, NULL);

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Candidate_of_Corresponds(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	int in_index_of_target_voxel,
	CKvVectorInt *out_mininum_indices)
//************************************************************************
{
	std::vector<int> tmp_indices, location_indices;
	CKvVector distv;
	CKvVoxel *p_voxels;
	double *p_double;
	int sz, k;

	p_voxels = in_set_of_voxels->vp();
	sz = in_reference_texture_on_code.size();
	p_double = distv.c_Create(sz, DBL_MAX);

	for (k = 0; k < sz; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			if ((p_voxels[in_index_of_target_voxel].x == p_voxels[in_reference_texture_on_code[k]].x) ||
				(p_voxels[in_index_of_target_voxel].y == p_voxels[in_reference_texture_on_code[k]].y) ||
				(p_voxels[in_index_of_target_voxel].z == p_voxels[in_reference_texture_on_code[k]].z))
			{
				tmp_indices.push_back(in_reference_texture_on_code[k]);
				location_indices.push_back(k);
			}
		}
	}

	if (tmp_indices.size() > 0)
	{
		for (k = 0; k < tmp_indices.size(); k++)
		{
			p_double[location_indices[k]] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[tmp_indices[k]]);
		}
	}
	else
	{
		for (k = 0; k < sz; k++)
		{
			if (in_reference_texture_on_code[k] != -1)
			{
				p_double[k] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[in_reference_texture_on_code[k]]);
			}
		}
	}

	//Find_Minimums(
	//	&distv,//CKvVector *in_vector,
	//	out_mininum_indices);//CKvVectorInt *out_mininum_indices);

	if (!Find_Minimums(
		&distv,//CKvVector *in_vector,
		6.0,//double in_threshold_for_distance,
		out_mininum_indices))//CKvVectorInt *out_mininum_indices);
	{
		return false;
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Candidate_of_Corresponds_In_X_Slice(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	int in_index_of_target_voxel,
	CKvVectorInt *out_mininum_indices)
//************************************************************************
{
	std::vector<int> tmp_indices, location_indices;
	CKvVector distv;
	CKvVoxel *p_voxels;
	double *p_double;
	int num_ref, k;

	p_voxels = in_set_of_voxels->vp();
	num_ref = in_reference_texture_on_code.size();
	p_double = distv.c_Create(num_ref, DBL_MAX);

	for (k = 0; k < num_ref; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			if ((p_voxels[in_index_of_target_voxel].x ==
				p_voxels[in_reference_texture_on_code[k]].x))
			{
				tmp_indices.push_back(in_reference_texture_on_code[k]);
				location_indices.push_back(k);
			}
		}
	}

	if (tmp_indices.size() > 0)
	{
		for (k = 0; k < tmp_indices.size(); k++)
		{
			p_double[location_indices[k]] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[tmp_indices[k]]);
		}
	}
	else
	{
		return false;
	}

	Find_Minimums(
		&distv,//CKvVector *in_vector,
		out_mininum_indices);//CKvVectorInt *out_mininum_indices);

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Candidate_of_Corresponds_In_X_Slice( //--------
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	int in_index_of_target_voxel,
	std::vector<int> &out_mininum_indices)
//************************************************************************
{
	std::vector<int> tmp_indices, location_indices;
	std::vector<double> distv;
	CKvVoxel *p_voxels;
	int num_ref, k;

	p_voxels = in_set_of_voxels->vp();
	num_ref = in_reference_texture_on_code.size();
	distv = std::vector<double>(num_ref, DBL_MAX);


	for (k = 0; k < num_ref; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			if ((p_voxels[in_index_of_target_voxel].x ==
				p_voxels[in_reference_texture_on_code[k]].x))
			{
				tmp_indices.push_back(in_reference_texture_on_code[k]);
				location_indices.push_back(k);
			}
		}
	}

	if (tmp_indices.size() > 0)
	{
		for (k = 0; k < tmp_indices.size(); k++)
		{
			distv[location_indices[k]] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[tmp_indices[k]]);
		}
	}
	else
	{
		return false;
	}

	Find_Minimums(
		distv,//std::vector<double> &in_vector,
		out_mininum_indices);//std::vector<int> &out_mininum_indices)

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Candidate_of_Corresponds_In_X_Slice(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	int in_index_of_target_voxel,
	std::vector<bool> &io_reference_flag,
	std::vector<int> &out_mininum_indices)
//************************************************************************
{
	std::vector<int> tmp_indices, location_indices;
	std::vector<double> distv;
	CKvVoxel *p_voxels;
	int num_ref, k;

	p_voxels = in_set_of_voxels->vp();
	num_ref = in_reference_texture_on_code.size();
	distv = std::vector<double>(num_ref, DBL_MAX);


	for (k = 0; k < num_ref; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			if ((p_voxels[in_index_of_target_voxel].x ==
				p_voxels[in_reference_texture_on_code[k]].x))
			{
				tmp_indices.push_back(in_reference_texture_on_code[k]);
				location_indices.push_back(k);
			}
		}
	}

	if (tmp_indices.size() > 0)
	{
		for (k = 0; k < tmp_indices.size(); k++)
		{
			distv[location_indices[k]] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[tmp_indices[k]]);
		}
	}
	else
	{
		return false;
	}

	Find_Minimums(
		distv,//std::vector<double> &in_vector,
		out_mininum_indices);//std::vector<int> &out_mininum_indices)




	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Candidate_of_Corresponds_In_Z_Slice(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	int in_index_of_target_voxel,
	CKvVectorInt *out_mininum_indices)
//************************************************************************
{
	std::vector<int> tmp_indices, location_indices;
	CKvVector distv;
	CKvVoxel *p_voxels;
	double *p_double;
	int sz, k;

	p_voxels = in_set_of_voxels->vp();
	sz = in_reference_texture_on_code.size();
	p_double = distv.c_Create(sz, DBL_MAX);

	for (k = 0; k < sz; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			if ((p_voxels[in_index_of_target_voxel].z == p_voxels[in_reference_texture_on_code[k]].z))
			{
				tmp_indices.push_back(in_reference_texture_on_code[k]);
				location_indices.push_back(k);
			}
		}
	}

	if (tmp_indices.size() > 0)
	{
		for (k = 0; k < tmp_indices.size(); k++)
		{
			p_double[location_indices[k]] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[tmp_indices[k]]);
		}
	}
	else
	{
		return false;
	}

	Find_Minimums(
		&distv,//CKvVector *in_vector,
		out_mininum_indices);//CKvVectorInt *out_mininum_indices);

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Candidate_of_Corresponds_In_Z_Slice(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	int in_index_of_target_voxel,
	std::vector<int> &out_mininum_indices)
//************************************************************************
{
	std::vector<int> tmp_indices, location_indices;
	std::vector<double> distv;
	CKvVoxel *p_voxels;
	int num_ref, k;

	p_voxels = in_set_of_voxels->vp();
	num_ref = in_reference_texture_on_code.size();
	distv = std::vector<double>(num_ref, DBL_MAX);


	for (k = 0; k < num_ref; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			if ((p_voxels[in_index_of_target_voxel].z ==
				p_voxels[in_reference_texture_on_code[k]].z))
			{
				tmp_indices.push_back(in_reference_texture_on_code[k]);
				location_indices.push_back(k);
			}
		}
	}

	if (tmp_indices.size() > 0)
	{
		for (k = 0; k < tmp_indices.size(); k++)
		{
			distv[location_indices[k]] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[tmp_indices[k]]);
		}
	}
	else
	{
		return false;
	}

	Find_Minimums(
		distv,//std::vector<double> &in_vector,
		out_mininum_indices);//std::vector<int> &out_mininum_indices)

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Candidate_of_Corresponds_In_Y_Slice(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	int in_index_of_target_voxel,
	std::vector<int> &out_mininum_indices)
//************************************************************************
{
	std::vector<int> tmp_indices, location_indices;
	std::vector<double> distv;
	CKvVoxel *p_voxels;
	int num_ref, k;

	p_voxels = in_set_of_voxels->vp();
	num_ref = in_reference_texture_on_code.size();
	distv = std::vector<double>(num_ref, DBL_MAX);


	for (k = 0; k < num_ref; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			if ((p_voxels[in_index_of_target_voxel].y ==
				p_voxels[in_reference_texture_on_code[k]].y))
			{
				tmp_indices.push_back(in_reference_texture_on_code[k]);
				location_indices.push_back(k);
			}
		}
	}

	if (tmp_indices.size() > 0)
	{
		for (k = 0; k < tmp_indices.size(); k++)
		{
			distv[location_indices[k]] = p_voxels[in_index_of_target_voxel].ds_Distance_Squared(p_voxels[tmp_indices[k]]);
		}
	}
	else
	{
		return false;
	}

	Find_Minimums(
		distv,//std::vector<double> &in_vector,
		out_mininum_indices);//std::vector<int> &out_mininum_indices)

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Rotate_Texture_on_Code_by_Centering(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &io_texture_on_code,
	int in_plane_mode)
//************************************************************************
{
	CKvVoxel *p_voxel;
	int main_cord_min, main_cord_max;
	int c_cord_main, c_cord_sub, center;
	int sub_cord_min, sub_cord_max;
	int sz, k, a, b, min_ab, min_sub, max_sub, min_idx;

	p_voxel = in_set_of_voxels->vp();

	main_cord_min = INT_MAX;
	main_cord_max = INT_MIN;
	sub_cord_min  = INT_MAX;
	sub_cord_max  = INT_MIN;
	min_ab        = INT_MAX;
	min_sub		  = INT_MAX;
	max_sub		  = INT_MIN;
	if (in_plane_mode == 0) // X-slice
	{
		sz = io_texture_on_code.size();
		for (k = 0; k < sz; k++)
		{
			if (main_cord_min > p_voxel[io_texture_on_code[k]].y)
			{
				main_cord_min = p_voxel[io_texture_on_code[k]].y;
			}

			if (main_cord_max < p_voxel[io_texture_on_code[k]].y)
			{
				main_cord_max = p_voxel[io_texture_on_code[k]].y;
			}

			if (sub_cord_min > p_voxel[io_texture_on_code[k]].z)
			{
				sub_cord_min = p_voxel[io_texture_on_code[k]].z;
			}

			if (sub_cord_max < p_voxel[io_texture_on_code[k]].z)
			{
				sub_cord_max = p_voxel[io_texture_on_code[k]].z;
			}
		}

		for (k = 0; k < sz; k++)
		{
			a = abs(main_cord_min - p_voxel[io_texture_on_code[k]].y);
			b = abs(main_cord_max - p_voxel[io_texture_on_code[k]].y);
			if (min_ab > abs(a - b))
			{
				if (min_sub > p_voxel[io_texture_on_code[k]].z)
				{
					min_ab = abs(a - b);
					min_sub = p_voxel[io_texture_on_code[k]].z;
					min_idx = k;
				}
			}
		}
	}
	else if (in_plane_mode == 1) // Y-slice
	{
		sz = io_texture_on_code.size();
		for (k = 0; k < sz; k++)
		{
			if (main_cord_min > p_voxel[io_texture_on_code[k]].x)
			{
				main_cord_min = p_voxel[io_texture_on_code[k]].x;
			}

			if (main_cord_max < p_voxel[io_texture_on_code[k]].x)
			{
				main_cord_max = p_voxel[io_texture_on_code[k]].x;
			}

			if (sub_cord_min > p_voxel[io_texture_on_code[k]].z)
			{
				sub_cord_min = p_voxel[io_texture_on_code[k]].z;
			}

			if (sub_cord_max < p_voxel[io_texture_on_code[k]].z)
			{
				sub_cord_max = p_voxel[io_texture_on_code[k]].z;
			}
		}

		//ver 1
		for (k = 0; k < sz; k++)
		{
			if (p_voxel[io_texture_on_code[k]].z == (int)((sub_cord_max + sub_cord_min) / 2))
			{
				if (max_sub < p_voxel[io_texture_on_code[k]].x)
				{
					max_sub = p_voxel[io_texture_on_code[k]].x;
					min_idx = k;
				}
			}
		}
		//ver 2
		//for (k = 0; k < sz; k++)
		//{
		//	if (p_voxel[io_texture_on_code[k]].x == (int)((main_cord_max + main_cord_min) / 2))
		//	{
		//		if (max_sub < p_voxel[io_texture_on_code[k]].z)
		//		{
		//			max_sub = p_voxel[io_texture_on_code[k]].z;
		//			min_idx = k;
		//		}
		//	}
		//}
	}
	else if (in_plane_mode == 2) // Z-slice
	{
		sz = io_texture_on_code.size();
		for (k = 0; k < sz; k++)
		{
			if (main_cord_min > p_voxel[io_texture_on_code[k]].x)
			{
				main_cord_min = p_voxel[io_texture_on_code[k]].x;
			}

			if (main_cord_max < p_voxel[io_texture_on_code[k]].x)
			{
				main_cord_max = p_voxel[io_texture_on_code[k]].x;
			}

			if (sub_cord_min > p_voxel[io_texture_on_code[k]].y)
			{
				sub_cord_min = p_voxel[io_texture_on_code[k]].y;
			}

			if (sub_cord_max < p_voxel[io_texture_on_code[k]].y)
			{
				sub_cord_max = p_voxel[io_texture_on_code[k]].y;
			}
		}

		/**********************************************************************************************/
		// Ver1
		for (k = 0; k < sz; k++)
		{
			if (p_voxel[io_texture_on_code[k]].x == (int)((main_cord_max + main_cord_min) / 2))
			{
				if (max_sub < p_voxel[io_texture_on_code[k]].y)
				{
					max_sub = p_voxel[io_texture_on_code[k]].y;
					min_idx = k;
				}
			}
		}
		/**********************************************************************************************/


		/**********************************************************************************************/
		// Ver2
		//for (k = 0; k < sz; k++)
		//{
		//	if (p_voxel[io_texture_on_code[k]].y == (int)((sub_cord_max + sub_cord_min) / 2))
		//	{
		//		if (max_sub < p_voxel[io_texture_on_code[k]].x)
		//		{
		//			max_sub = p_voxel[io_texture_on_code[k]].x;
		//			min_idx = k;
		//		}
		//	}
		//}
		/**********************************************************************************************/
	}

	center = (int)(sz / 2);
	a = min_idx - center;
	if (a > 0)
	{
		sv_Circular_Shift_Vector(
			a,//int in_location,
			io_texture_on_code);//std::vector<int> &io_vector);
	}
	else if (a < 0)
	{
		sv_Circular_Shift_Vector(
			min_idx + sz - center,//int in_location,
			io_texture_on_code);//std::vector<int> &io_vector);
	}


	return true;

}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Rotate_Texture_on_Code_by_Color(
	CKvSet_of_RgbaF *in_set_of_colors,
	std::vector<int> &io_texture_on_code,
	CKvSet_of_RgbaF *in_before_colors,
	int &out_circular_shift)
//************************************************************************
{
	CKvSet_of_RgbaF tmp_io_ToC, tmp;
	CKvRgbaF *p1, *p2, *p_in_colors;
	double min_cost, error;
	int sz1, sz2, k, j, i, l;
	int search_length, length;
	int circular_shift_idx, out_offset;

	sz2 = io_texture_on_code.size();
	sz1 = in_before_colors->vs_Vector_Size();
	p1 =  tmp_io_ToC.c_Create(sz2);
	p2 =  in_before_colors->vp();
	p_in_colors = in_set_of_colors->vp();

	for (k = 0; k < sz2; k++)
	{
		p1[k].r = p_in_colors[io_texture_on_code[k]].r;
		p1[k].g = p_in_colors[io_texture_on_code[k]].g;
		p1[k].b = p_in_colors[io_texture_on_code[k]].b;
		p1[k].a = p_in_colors[io_texture_on_code[k]].a;
	}

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }
	length = abs(sz2 - sz1) + 1;

	min_cost = 999999999999999999.9999999;
	if (sz1 < sz2)
	{
		for (j = 0; j < length; j++)
		{
			for (l = 0; l < sz2; l++)
			{
				tmp = tmp_io_ToC;
				sv_Circular_Shift_Vector(l, &tmp);
				error = 0.0;
#pragma omp parallel
				{
#pragma omp for reduction(+:error)
					for (k = 0; k < search_length; k++)
					{
						error += abs(p2[k].r - p1[k + j].r);
						error += abs(p2[k].g - p1[k + j].g);
						error += abs(p2[k].b - p1[k + j].b);
					}
				}

				if (min_cost > error)
				{
					min_cost = error;
					circular_shift_idx = l;
					out_offset = -j;
				}
			}
		}
	}
	else
	{
		for (j = 0; j < length; j++)
		{
			for (l = 0; l < sz2; l++)
			{
				tmp = tmp_io_ToC;
				sv_Circular_Shift_Vector(l, &tmp);
				error = 0.0;
#pragma omp parallel
				{
#pragma omp for reduction(+:error)
					for (k = 0; k < search_length; k++)
					{
						error += abs(p2[k + j].r - p1[k].r);
						error += abs(p2[k + j].g - p1[k].g);
						error += abs(p2[k + j].b - p1[k].b);
					}
				}

				if (min_cost > error)
				{
					min_cost = error;
					circular_shift_idx = l;
					out_offset = j;
				}
			}
		}
	}

	sv_Circular_Shift_Vector(circular_shift_idx, io_texture_on_code);
	//tmp = tmp_io_ToC;
	//sv_Circular_Shift_Vector(circular_shift_idx, &tmp);
	//zz_before_colors.cp_Copy(&tmp);

	out_circular_shift = circular_shift_idx;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Find_Corresponding_ToC(
	std::vector<std::vector<std::vector<int>>> &in_before_ToC,
	std::vector<int> &in_current_ToC,
	CKvSet_of_Voxel *in_before_voxels,
	CKvSet_of_RgbaF *in_before_colors,
	VCL_DoCube_X_Color *in_current_docube,
	CKvSet_of_RgbaF *out_ToC_in_before)
//************************************************************************
{
	std::vector<int> depot_blob_idx;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVoxel *p_voxel, *p_before_voxel;
	CKvPoint3D mean_current_point, mean_before_point;
	double dist, min_dist;
	int num_blob_before, num_slice_before, num_element_before, refer_y;
	int k, m, n, refer_blob_index, refer_slice_index;

	v = in_current_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();
	p_voxel = set_v.vp();
	p_before_voxel = in_before_voxels->vp();
	refer_y = p_voxel[in_current_ToC[0]].y;

	// Find current mean
	Find_Mean(in_current_ToC, &set_v, &mean_current_point);


	min_dist = DBL_MAX;
	num_blob_before = in_before_ToC.size();
	for (k = 0; k < num_blob_before; k++)
	{
		num_slice_before = in_before_ToC[k].size();
		for (m = 0; m < num_slice_before; m++)
		{
			if (refer_y == p_before_voxel[in_before_ToC[k][m][0]].y)
			{
				Find_Mean(in_before_ToC[k][m], in_before_voxels, &mean_before_point);
				dist = mean_before_point.d_Distance(mean_current_point);

				if (min_dist > dist)
				{
					min_dist = dist;
					refer_blob_index = k;
					refer_slice_index = m;
				}
				break;
			}
		}
	}

	if (min_dist == DBL_MAX)
	{
		return false;
	}


	num_element_before = in_before_ToC[refer_blob_index][refer_slice_index].size();
	CKvRgbaF *p_color  = out_ToC_in_before->c_Create(num_element_before);
	CKvRgbaF *p_before_color = in_before_colors->vp();
	for (k = 0; k < num_element_before;k++)
	{
		p_color[k].r = p_before_color[in_before_ToC[refer_blob_index][refer_slice_index][k]].r;
		p_color[k].g = p_before_color[in_before_ToC[refer_blob_index][refer_slice_index][k]].g;
		p_color[k].b = p_before_color[in_before_ToC[refer_blob_index][refer_slice_index][k]].b;
		p_color[k].a = p_before_color[in_before_ToC[refer_blob_index][refer_slice_index][k]].a;
	}

	return true;

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Base::Rotate_Texture_on_Code_Randomly(
	std::vector<int> &io_texture_on_code)
//************************************************************************
{
	/*srand((unsigned int)time(NULL));*/
	
	int num = rand();
	int sz = io_texture_on_code.size();

	num = num % (sz);

	sv_Circular_Shift_Vector(
		num,//int in_location,
		io_texture_on_code);//std::vector<int> &io_vector);
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Base::Save_File(
	std::vector<std::vector<double>> &in_data)
//************************************************************************
{
	FILE *fp = NULL;
	int ww, hh, j, i;

	fopen_s(&fp, "distance_map.txt", "w");
	if (fp == NULL) return false;


	hh = in_data.size();

	for (j = 0; j < hh; j++)
	{
		ww = in_data[j].size();
		for (i = 0; i < ww; i++)
		{
			if (in_data[j][i] == DBL_MAX)
			{
				fprintf(fp, "-1 ");
			}
			else
			{
				fprintf(fp, "%lf ", in_data[j][i]);
			}
		}
		fprintf(fp, "\n");
	}

	return true;
}
//************************************************************************




