#include "stdafx.h"
#include "VCL_EI_DCT.h"

//************************************************************************
VCL_EI_DCT::VCL_EI_DCT()
//************************************************************************
{

}
//************************************************************************
VCL_EI_DCT::~VCL_EI_DCT()
//************************************************************************
{

}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Forward(
	CKvVectorUcharRgb *io_img,
	CKvVectorBool *in_mask)
//************************************************************************
{
	LCKvAlgebra_for_Matrix am;
	CKvVectorUcharRgb cpy_triangle_img; CKvVectorBool cpy_mask; CKvRgb rgb; CKvMatrix C; CKvVector f1, f2; CKvXvector x_f1;
	unsigned char **ptr_img, **ptr_img2, *ptr_r, *ptr_g, *ptr_b, *ptr_r2, *ptr_g2, *ptr_b2;
	double tmp1, tmp2, s1, s2, a, b, cos_val; double **ptr_C, *ptr_f2;
	int sz, j, i, l, m, k, c, M, N; bool *p_mask, *p_cpy_mask;
	int hh;

	cpy_triangle_img.cp_Copy(io_img); cpy_mask.cp_Copy(in_mask);

	ptr_r = cpy_triangle_img.vp(ptr_g, ptr_b);
	p_cpy_mask = cpy_mask.vp();
	sz = cpy_triangle_img.vs();

	ptr_r2 = io_img->vp(ptr_g2, ptr_b2);
	ptr_img = new unsigned char *[3]; ptr_img2 = new unsigned char *[3];
	p_mask = in_mask->vp();

	c = 0;
	for (j = 0; j < sz; j++)
	{
		if (p_mask[j])
		{
			c++;
		}
	}

	if (sz == c)
	{ 	
		delete[] ptr_img2;
		delete[] ptr_img;
		return true;
	}

	if (c == 0)
	{
		delete[] ptr_img2;
		delete[] ptr_img;
		return false; 
	}

	// row
	N  = sz; 
	hh = 1;
	ptr_img[0]  = ptr_r;  ptr_img[1]  = ptr_g;  ptr_img[2]  = ptr_b;
	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;

	for (c = 0; c < 3; c++)
	{
		x_f1.in_Initialize(1);
		M = 0;
		for (i = 0; i < sz; i++)
		{
			if (p_mask[i] == true)
			{
				x_f1.ap_Append((double)ptr_img[c][i]);
				M++;
			}
		}

		if (M != 0)
		{
			x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(N, M);

			for (l = 0; l < N; l++)
			{
				for (m = 0; m < M; m++)
				{

					tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)N);
					tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)M);

					a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp1 - tmp2));

					if ((a*b) == 0.0)
					{
						cos_val = 0.0;
						for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
						ptr_C[l][m] = sqrt((double)((double)N / (double)M))*sqrt(1.0 / (double)((double)N*(double)M)) +
							(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
					}
					else
					{
						s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
						s2 = sin((PI / 4)*(tmp1 - tmp2)*((2 * M) - 1)) / b;
						ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
						ptr_C[l][m] += (double)((double)(1.0 / (double)M) - (double)(1.0 / sqrt((double)N*M)));
					}
				}
			}

			am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
			ptr_f2 = f2.vp();

			for (i = 0; i < sz; i++)
			{
				ptr_img2[c][i] = (unsigned char)limit(ptr_f2[i]);
			}
		}
	}


	delete[] ptr_img2;
	delete[] ptr_img;
	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Forward_on_Spatial_Domain(
	CKvVectorUcharRgb *io_img,
	CKvVectorBool *in_mask)
//************************************************************************
{
	CKvVectorUcharRgb ei_img;
	CKvVectorInt pt_list;
	CKvVectorInt ei_pt_list;
	bool *p_mask;
	float scale,d1,d2;
	int num_max_size, num_valid_point_size, k, m, min_position;
	int *p_ptlist, *p_eipt;
	unsigned char *pr1, *pg1, *pb1, *pr2, *pg2, *pb2;

	p_mask = in_mask->vps(num_max_size);
	num_valid_point_size = 0;
	for (k = 0; k < num_max_size; k++)
	{
		if (p_mask[k] == true)
		{
			num_valid_point_size++;
		}
	}

	if (num_valid_point_size == 0)
	{
		return false;
	}

	if (num_max_size == num_valid_point_size) 
	{ 
		return true; 
	}

	if (num_valid_point_size == 1)
	{
		pr2 = io_img->vp(pg2, pb2);
		unsigned char Red, Green, Blue;
		for (k = 0; k < num_max_size; k++)
		{
			if (p_mask[k] == true)
			{
				Red   = pr2[k];
				Green = pg2[k];
				Blue  = pb2[k];
				break;
			}
		}

		io_img->sv_Set_Vector(Kv_Rgb(Red, Green, Blue));

		return true;
	}

	p_ptlist = pt_list.c_Create(num_valid_point_size);
	num_valid_point_size = 0;
	min_position = INT_MAX;
	for (k = 0; k < num_max_size; k++)
	{
		if (p_mask[k] == true)
		{
			p_ptlist[num_valid_point_size] = k;
			num_valid_point_size++;
			if (min_position > k)
			{
				min_position = k;
			}
		}
	}

	scale = (float)(num_max_size-1) / (float)(p_ptlist[num_valid_point_size - 1] - min_position);
	p_eipt = ei_pt_list.c_Create(num_valid_point_size);
	for (k = 0; k < num_valid_point_size; k++)
	{
		p_eipt[k] = int(round((float)(p_ptlist[k]-min_position) * scale));
	}

	ei_img.c_Create(num_max_size, Kv_Rgb(0, 0, 0));
	pr1 = ei_img.vp(pg1, pb1);
	pr2 = io_img->vp(pg2, pb2);
	for (k = 0; k < num_valid_point_size-1; k++)
	{
		for (m = p_eipt[k]; m <= p_eipt[k + 1]; m++)
		{
			d1 = abs(p_eipt[k] - m);
			d2 = abs(p_eipt[k+1] - m);
			pr1[m] = (d2/(d1+d2))*pr2[p_ptlist[k]] + (d1 / (d1 + d2))*pr2[p_ptlist[k+1]];
			pg1[m] = (d2/(d1+d2))*pg2[p_ptlist[k]] + (d1 / (d1 + d2))*pg2[p_ptlist[k+1]];
			pb1[m] = (d2/(d1+d2))*pb2[p_ptlist[k]] + (d1 / (d1 + d2))*pb2[p_ptlist[k+1]];
		}
	}

	io_img->cp_Copy(&ei_img);

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Forward_on_Spatial_Domain(
	CKvVectorInt *io_img,
	CKvVectorBool *in_mask)
//************************************************************************
{
	CKvVectorInt ei_img;
	CKvVectorInt pt_list;
	CKvVectorInt ei_pt_list;
	bool *p_mask;
	float scale, d1, d2;
	int num_max_size, num_valid_point_size, k, m, min_position;
	int *p_ptlist, *p_eipt;
	int *p_io_img, *p_ei_img, idx;

	p_mask = in_mask->vps(num_max_size);
	num_valid_point_size = 0;
	min_position = INT_MAX;
	for (k = 0; k < num_max_size; k++)
	{
		if (p_mask[k] == true)
		{
			num_valid_point_size++;
			if (min_position > k)
			{
				min_position = k;
			}
		}
	}

	if (num_valid_point_size == 0)
	{
		return false;
	}

	if (num_max_size == num_valid_point_size)
	{
		return true;
	}

	if (num_valid_point_size == 1)
	{
		//p_io_img = io_img->vp();
		//for (k = 0; k < num_max_size; k++)
		//{
		//	if (p_mask[k] == true)
		//	{
		//		idx = p_io_img[k];
		//		break;
		//	}
		//}
		//for (k = 0; k < num_max_size; k++)
		//{
		//	p_io_img[k] = idx;
		//}

		return true;
	}

	scale = (float)(num_max_size-1) / (float)(num_valid_point_size-1);

	p_io_img = io_img->vp();
	p_ei_img = ei_img.c_Create(num_max_size, -1);
	
	for (k = 0; k < num_max_size; k++)
	{
		idx = (int)round((float)(k) / scale) + min_position;
		if (idx > (num_max_size - 1))
		{
			idx = (num_max_size - 1);
		}
		p_ei_img[k] = p_io_img[idx];
	}

	io_img->cp_Copy(&ei_img);

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Forward_on_Spatial_Domain2(
	CKvVectorInt *io_img,
	CKvVectorBool *in_mask)
//************************************************************************
{
	CKvVectorInt ei_img;
	CKvVectorInt pt_list;
	CKvVectorInt ei_pt_list;
	bool *p_mask;
	float scale, d1, d2;
	int num_max_size, num_valid_point_size, k, m;
	int *p_ptlist, *p_eipt, min_position;
	int *p_io_img, *p_ei_img, idx;

	p_mask = in_mask->vps(num_max_size);
	num_valid_point_size = 0;
	min_position = INT_MAX;
	for (k = 0; k < num_max_size; k++)
	{
		if (p_mask[k] == true)
		{
			num_valid_point_size++;
			if (min_position > k)
			{
				min_position = k;
			}
		}
	}

	if (num_valid_point_size == 0)
	{
		return false;
	}

	if (num_max_size == num_valid_point_size)
	{
		return true;
	}

	if (num_valid_point_size == 1)
	{
		p_io_img = io_img->vp();
		for (k = 0; k < num_max_size; k++)
		{
			if (p_mask[k] == true)
			{
				idx = p_io_img[k];
				break;
			}
		}
		for (k = 0; k < num_max_size; k++)
		{
			p_io_img[k] = idx;
		}

		return true;
	}

	scale = (float)(num_max_size - 1) / (float)(num_valid_point_size - 1);

	p_io_img = io_img->vp();
	p_ei_img = ei_img.c_Create(num_max_size, -1);

	for (k = 0; k < num_valid_point_size; k++)
	{
		idx = (int)round((k)*scale);
		if (idx > num_max_size)
		{
			idx = num_max_size - 1;
		}
		p_ei_img[idx] = p_io_img[k+ min_position];
	}


	io_img->cp_Copy(&ei_img);

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Forward_on_Spatial_Domain_Right(
	CKvVectorUcharRgb *io_img_vec,
	CKvVectorInt *io_idx_vec,
	CKvVectorInt *in_reference_idx_vec,
	CKvVectorBool *in_mask,
	CKvSet_of_Point3Df *in_set_of_voxels,
	std::vector<int> &io_control_points,
	bool &io_mode,
	int in_number_of_control_point_for_half_plane)
//************************************************************************
{
	CKvVectorUcharRgb part_img, block_img, out_img_vec;
	CKvVectorInt part_idx, block_idx, out_idx_vec;
	CKvVectorBool block_mask;
	CKvPoint3Df *p_voxel;
	std::vector<int> current_control_points;
	std::vector<int> current_start_points;
	std::vector<int> control_start_points;
	std::vector<int> control_end_points;
	float stepf, cost_min, cost, p;
	int *p_refer_idx_vec, *p_io_idx_vec;
	int num_blob, sz, idx, idx_sz, step, k, l;
	int max_position;
	bool *p_mask;

	num_blob        = in_number_of_control_point_for_half_plane + 1;
	p_io_idx_vec    = io_idx_vec->vps(sz);
	p_mask          = in_mask->vp();
	idx_sz          = 0;
	max_position    = INT_MIN;
	for (k = 0; k < sz; k++)
	{
		if (p_mask[k] == true)
		{
			idx_sz++;
			if (max_position < k)
			{
				max_position = k;
			}
		}
	}

	p_refer_idx_vec = in_reference_idx_vec->vps(sz);
	if (idx_sz < num_blob)
	{
		EI_DCT_Forward_on_Spatial_Domain(
			io_img_vec,//CKvVectorUcharRgb *io_img,
			in_mask);//CKvVectorBool *in_mask);

		EI_DCT_Forward_on_Spatial_Domain2(
			io_idx_vec,//CKvVectorInt *io_img,
			in_mask);//CKvVectorBool *in_mask);

		io_mode = true;

		return true;
	}

	
	if ((io_control_points.size()) == 0 && (io_mode==false))
	{
		step = sz / num_blob;
		for (k = step; k < sz; k+=step)
		{
			control_end_points.push_back(k);
		}
	}
	else if ((io_control_points.size() == 0) && (io_mode == true))
	{
		EI_DCT_Forward_on_Spatial_Domain(
			io_img_vec,//CKvVectorUcharRgb *io_img,
			in_mask);//CKvVectorBool *in_mask);

		EI_DCT_Forward_on_Spatial_Domain2(
			io_idx_vec,//CKvVectorInt *io_img,
			in_mask);//CKvVectorBool *in_mask);

		transform_points(
			idx_sz,//int in_current_length,
			in_number_of_control_point_for_half_plane,//int in_number_of_control_points,
			sz,//int in_max_length,
			io_control_points);//std::vector<int> &out_control_points);

		io_mode = false;

		return true;
	}
	else
	{
		control_end_points = io_control_points;
	}

	p_voxel = in_set_of_voxels->vp();
	stepf   = (float)idx_sz / (float)num_blob;
	printf("sz = %d idx_sz = %d stepf = %f\n", sz, idx_sz, stepf);
	k = 0; 
	p = 0.f;
	while (k < in_number_of_control_point_for_half_plane)
	{
		cost_min = FLT_MAX;
		for (l = ceil(p); l < p + stepf; l++)
		{
			cost = p_voxel[p_refer_idx_vec[control_end_points[k]]].d_Distance(p_voxel[p_io_idx_vec[l]]);
			if (cost_min > cost)
			{
				cost_min = cost;
				idx = l;
			}
		}
		current_control_points.push_back(idx);
		k++;
		p += stepf;
	}
	current_control_points.push_back(max_position);


	current_start_points.push_back(0);
	for (k = 0; k < current_control_points.size()-1; k++)
	{
		current_start_points.push_back(current_control_points[k] + 1);
	}

	control_end_points.push_back(sz-1);
	control_start_points.push_back(0);
	for (k = 0; k < control_end_points.size() - 1; k++)
	{
		control_start_points.push_back(control_end_points[k] + 1);
	}

	/*******************************************************************/
	out_img_vec.c_Create(sz, Kv_Rgb(0, 0, 0));
	out_idx_vec.c_Create(sz, -1);

	printf("control size : %d\n", control_start_points.size());
	for (k = 0; k < control_start_points.size(); k++)
	{
		printf("%d %d\n", control_start_points[k], control_end_points[k]);
	}
	
	printf("current size : %d\n", current_control_points.size());
	for (k = 0; k < current_control_points.size(); k++)
	{
		printf("%d %d\n", current_start_points[k], current_control_points[k]);
	}

	for (k = 0; k < current_control_points.size(); k++)
	{
		block_img.c_Create( (control_end_points[k] - control_start_points[k] + 1), Kv_Rgb(0, 0, 0));
		block_idx.c_Create( (control_end_points[k] - control_start_points[k] + 1), -1);
		block_mask.c_Create((control_end_points[k] - control_start_points[k] + 1), false);

		io_img_vec->g_Get(
			current_start_points[k],
			current_control_points[k] - current_start_points[k] + 1,
			&part_img);
		io_idx_vec->g_Get(
			current_start_points[k],
			current_control_points[k] - current_start_points[k] + 1,
			&part_idx);

		printf("sz_block=%d sz_part=%d\n", block_img.vs(), part_img.vs());
		block_img.s_Set(0, &part_img);
		printf("sz_block=%d sz_part=%d\n", block_idx.vs(), part_idx.vs());
		block_idx.s_Set(0, &part_idx);
		printf("sz_block=%d sz_part=%d\n", block_mask.vs(), current_control_points[k] - current_start_points[k] + 1);
		block_mask.s_Set(0, current_control_points[k] - current_start_points[k] + 1, true);

		EI_DCT_Forward_on_Spatial_Domain(
			&block_img,//CKvVectorUcharRgb *io_img,
			&block_mask);//CKvVectorBool *in_mask);

		EI_DCT_Forward_on_Spatial_Domain2(
			&block_idx,//CKvVectorInt *io_img,
			&block_mask);//CKvVectorBool *in_mask);

		printf("sz_out=%d sz_block=%d pt=%d\n", out_img_vec.vs(), block_img.vs(), control_start_points[k]);
		out_img_vec.s_Set(control_start_points[k], &block_img);
		printf("sz_out=%d sz_block=%d pt=%d\n", out_idx_vec.vs(), block_idx.vs(), control_start_points[k]);
		out_idx_vec.s_Set(control_start_points[k], &block_idx);

	}

	io_img_vec->cp_Copy(&out_img_vec);
	io_idx_vec->cp_Copy(&out_idx_vec);
	io_control_points = std::vector<int>(current_control_points.size() - 1);
	for (k = 0; k < current_control_points.size() - 1; k++)
	{
		io_control_points[k] = current_control_points[k];
	}
	io_mode = false;
	/*******************************************************************/

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Forward_on_Spatial_Domain_Left(
	CKvVectorUcharRgb *io_img_vec,
	CKvVectorInt *io_idx_vec,
	CKvVectorInt *in_reference_idx_vec,
	CKvVectorBool *in_mask,
	CKvSet_of_Point3Df *in_set_of_voxels,
	std::vector<int> &io_control_points,
	bool &io_mode,
	int in_number_of_control_point_for_half_plane)
//************************************************************************
{
	CKvVectorUcharRgb rev_img_vec;
	CKvVectorInt rev_idx_vec;
	CKvVectorInt rev_reference_idx_vec;
	CKvVectorBool rev_mask;

	rev_img_vec.cp_Copy(io_img_vec);
	rev_idx_vec.cp_Copy(io_idx_vec);
	rev_reference_idx_vec.cp_Copy(in_reference_idx_vec);
	rev_mask.cp_Copy(in_mask);

	reverse_vector(
		&rev_img_vec,//CKvVectorUcharRgb *io_img_vec,
		&rev_idx_vec,//CKvVectorInt *io_idx_vec,
		&rev_reference_idx_vec,//CKvVectorInt *io_reference_idx_vec,
		&rev_mask);//CKvVectorBool *io_mask)

	if (!EI_DCT_Forward_on_Spatial_Domain_Right(
		&rev_img_vec,//CKvVectorUcharRgb *io_img_vec,
		&rev_idx_vec,//CKvVectorInt *io_idx_vec,
		&rev_reference_idx_vec,//CKvVectorInt *in_reference_idx_vec,
		&rev_mask,//CKvVectorBool *in_mask,
		in_set_of_voxels,//CKvSet_of_Point3Df *in_set_of_voxels,
		io_control_points,//std::vector<int> &io_control_points,
		io_mode,//bool &io_mode,
		in_number_of_control_point_for_half_plane))//int in_number_of_control_point_for_half_plane);
	{
		return false;
	}

	reverse_vector(
		&rev_img_vec,//CKvVectorUcharRgb *io_img_vec,
		&rev_idx_vec,//CKvVectorInt *io_idx_vec,
		&rev_reference_idx_vec,//CKvVectorInt *io_reference_idx_vec,
		&rev_mask);//CKvVectorBool *io_mask)

	io_img_vec->cp_Copy(&rev_img_vec);
	io_idx_vec->cp_Copy(&rev_idx_vec);
	in_reference_idx_vec->cp_Copy(&rev_reference_idx_vec);
	in_mask->cp_Copy(&rev_mask);

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Forward(
	CKvMatrixUcharRgb *io_img,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	LCKvAlgebra_for_Matrix am;
	CKvMatrixUcharRgb cpy_triangle_img; CKvMatrixBool cpy_mask; CKvRgb rgb; CKvMatrix C; CKvVector f1, f2; CKvXvector x_f1;
	unsigned char ***ptr_img, ***ptr_img2, **ptr_r, **ptr_g, **ptr_b, **ptr_r2, **ptr_g2, **ptr_b2;
	double tmp1, tmp2, s1, s2, a, b, cos_val; double **ptr_C, *ptr_f2;
	int ww, hh, j, i, l, m, k, c, M, N; bool **p_mask, **p_cpy_mask;


	cpy_triangle_img.cp_Copy(io_img); cpy_mask.cp_Copy(in_mask);
	ptr_r = cpy_triangle_img.mp(ptr_g, ptr_b);
	p_cpy_mask = cpy_mask.mp();
	cpy_triangle_img.ms(ww, hh);

	ptr_r2 = io_img->mp(ptr_g2, ptr_b2);
	ptr_img = new unsigned char **[3]; ptr_img2 = new unsigned char **[3];
	p_mask = in_mask->mp();

	c = 0;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_mask[j][i])
			{
				c++;
			}
		}
	}

	if (hh*ww == c)
	{ 	
		delete[] ptr_img2;
		delete[] ptr_img;
		return true;
	}

	if (c == 0)
	{
		delete[] ptr_img2;
		delete[] ptr_img;
		return false;
	}


	// row
	N = ww;
	ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;

	for (c = 0; c < 3; c++)
	{
		for (j = 0; j < hh; j++)
		{
			x_f1.in_Initialize(1);
			M = 0;
			for (i = 0; i < ww; i++)
			{
				if (p_mask[j][i] == true)
				{
					x_f1.ap_Append((double)ptr_img[c][j][i]);
					M++;
				}
			}

			if (M != 0)
			{
				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(N, M);

				for (l = 0; l < N; l++)
				{
					for (m = 0; m < M; m++)
					{

						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)N);
						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)M);

						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp1 - tmp2));

						if ((a*b) == 0.0)
						{
							cos_val = 0.0;
							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
							ptr_C[l][m] = sqrt((double)((double)N / (double)M))*sqrt(1.0 / (double)((double)N*(double)M)) +
								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
						}
						else
						{
							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
							s2 = sin((PI / 4)*(tmp1 - tmp2)*((2 * M) - 1)) / b;
							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
							ptr_C[l][m] += (double)((double)(1.0 / (double)M) - (double)(1.0 / sqrt((double)N*M)));
						}
					}
				}

				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
				ptr_f2 = f2.vp();

				for (i = 0; i < ww; i++)
				{
					ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[i]);
					p_cpy_mask[j][i] = true;
				}
			}
		}
	}

	// col
	N = hh;
	ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;

	for (c = 0; c < 3; c++)
	{
		for (i = 0; i < ww; i++)
		{
			x_f1.in_Initialize(1);
			M = 0;
			for (j = 0; j < hh; j++)
			{
				if (p_cpy_mask[j][i] == true)
				{
					x_f1.ap_Append((double)ptr_img2[c][j][i]);
					M++;
				}
			}

			if (M != 0)
			{
				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(N, M);

				for (l = 0; l < N; l++)
				{
					for (m = 0; m < M; m++)
					{

						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)N);
						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)M);

						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp1 - tmp2));

						if ((a*b) == 0.0)
						{
							cos_val = 0.0;
							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
							ptr_C[l][m] = sqrt((double)((double)N / (double)M))*sqrt(1.0 / (double)((double)N*(double)M)) +
								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
						}
						else
						{
							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
							s2 = sin((PI / 4)*(tmp1 - tmp2)*((2 * M) - 1)) / b;
							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
							ptr_C[l][m] += (double)((double)(1.0 / (double)M) - (double)(1.0 / sqrt((double)N*M)));
						}
					}
				}

				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
				ptr_f2 = f2.vp();

				for (j = 0; j < hh; j++)
				{
					ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[j]);
				}
			}
		}
	}

	delete[] ptr_img2;
	delete[] ptr_img;
	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Line_Forward(
	CKvMatrixUcharRgb *io_img,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	CKvMatrixUcharRgb ei_map;
	CKvSet_of_VectorUcharRgb set_line_color;
	CKvSet_of_VectorBool     set_line_mask;
	CKvVectorUcharRgb        *p_line;
	CKvVectorBool            *p_line_mask;
	int ww, hh, y;

	io_img->ms(ww, hh);
	p_line      = set_line_color.c_Create(hh);
	p_line_mask = set_line_mask.c_Create(hh);
	ei_map.c_Create(hh, ww, Kv_Rgb(0, 0, 0));

#pragma omp parallel
	{
#pragma omp for
		for (y = 0; y < hh; y++)
		{
			io_img->gr_Get_Row(0, y, ww, &p_line[y]);
			in_mask->gr_Get_Row(0, y, ww, &p_line_mask[y]);
			EI_DCT_Forward(
				&p_line[y],//CKvVectorUcharRgb *io_img,
				&p_line_mask[y]);//CKvVectorBool *in_mask)
			ei_map.sr_Set_Row(0, y, &p_line[y]);
		}
	}

	io_img->cp_Copy(&ei_map);

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Line_Forward_on_Spatial_Domain(
	CKvMatrixUcharRgb *io_img,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	CKvMatrixUcharRgb ei_map;
	CKvSet_of_VectorUcharRgb set_line_color;
	CKvSet_of_VectorBool     set_line_mask;
	CKvVectorUcharRgb        *p_line;
	CKvVectorBool            *p_line_mask;
	int ww, hh, y;

	io_img->ms(ww, hh);
	p_line = set_line_color.c_Create(hh);
	p_line_mask = set_line_mask.c_Create(hh);
	ei_map.c_Create(hh, ww, Kv_Rgb(0, 0, 0));

#pragma omp parallel
	{
#pragma omp for
		for (y = 0; y < hh; y++)
		{
			io_img->gr_Get_Row(0, y, ww, &p_line[y]);
			in_mask->gr_Get_Row(0, y, ww, &p_line_mask[y]);
			EI_DCT_Forward_on_Spatial_Domain(
				&p_line[y],//CKvVectorUcharRgb *io_img,
				&p_line_mask[y]);//CKvVectorBool *in_mask)
			ei_map.sr_Set_Row(0, y, &p_line[y]);
		}
	}

	io_img->cp_Copy(&ei_map);

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Line_Forward_on_Spatial_Domain(
	CKvMatrixInt *io_img,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	CKvMatrixInt ei_map;
	CKvSet_of_VectorInt  set_line_color;
	CKvSet_of_VectorBool set_line_mask;
	CKvVectorInt         *p_line;
	CKvVectorBool        *p_line_mask;
	int ww, hh, y;

	io_img->ms(ww, hh);
	p_line      = set_line_color.c_Create(hh);
	p_line_mask = set_line_mask.c_Create(hh);
	ei_map.c_Create(hh, ww, -1);

	//printf("hh = %d ww = %d\n", hh, ww);

#pragma omp parallel
	{
#pragma omp for
		for (y = 0; y < hh; y++)
		{
			io_img->gr_Get_Row(0, y, ww, &p_line[y]);
			in_mask->gr_Get_Row(0, y, ww, &p_line_mask[y]);
			EI_DCT_Forward_on_Spatial_Domain2(
				&p_line[y],//CKvVectorUcharRgb *io_img,
				&p_line_mask[y]);//CKvVectorBool *in_mask)
			ei_map.sr_Set_Row(0, y, &p_line[y]);
		}
	}

	io_img->cp_Copy(&ei_map);

	return true;
}
//************************************************************************
void VCL_EI_DCT::EI_Block_DCT_Forward(
	CKvMatrixUcharRgb *io_img,
	int &in_block_x,
	int &in_block_y,
	CKvMatrixBool *io_mask)
//************************************************************************
{
	CKvMatrixUcharRgb color_images; CKvMatrixBool mask;
	CKvMatrixUcharRgb block_img;   CKvMatrixBool block_mask;
	double lww, lhh;
	int ww, hh, j, i;

	io_img->ms(ww, hh);

	lww = (double)ww / (double)in_block_x;
	lhh = (double)hh / (double)in_block_y;

	lww = ceil(lww);
	lhh = ceil(lhh);

	ww = lww * in_block_x;
	hh = lhh * in_block_y;

	color_images.c_Create(hh, ww, Kv_Rgb(0, 0, 0));
	mask.c_Create(hh, ww, false);

	color_images.sb_Set_Block(0, 0, io_img);
	mask.sb_Set_Block(0, 0, io_mask);

	for (j = 0; j < hh; j += in_block_y)
	{
		for (i = 0; i < ww; i += in_block_x)
		{
			color_images.gb_Get_Block(i, j, in_block_x, in_block_y, &block_img);
			mask.gb_Get_Block(i, j, in_block_x, in_block_y, &block_mask);
			EI_DCT_Forward(&block_img, &block_mask);
			color_images.sb_Set_Block(i, j, &block_img);
		}
	}

	io_img->cp_Copy(&color_images);
	io_mask->cp_Copy(&mask);
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Backward(
	CKvVectorUcharRgb *io_img,
	CKvVectorBool *in_mask)
//************************************************************************
{
	LCKvAlgebra_for_Matrix am; CKvMatrix C;
	CKvVectorUcharRgb cpy_img; CKvVector f1, f2;
	unsigned char **ptr_img, **ptr_img2, *ptr_r, *ptr_g, *ptr_b, *ptr_r2, *ptr_g2, *ptr_b2;
	int j, l, m, k, c, M, N; bool *p_mask;
	double **ptr_C, *ptr_f1, *ptr_f2, tmp1, tmp2, a , b, cos_val, s1, s2;
	int count;

	ptr_img = new unsigned char *[3]; ptr_img2 = new unsigned char *[3];
	p_mask = in_mask->vp();

	cpy_img.cp_Copy(io_img);
	ptr_r  = cpy_img.vp(ptr_g, ptr_b);
	ptr_r2 = io_img->vp(ptr_g2, ptr_b2);

	N = io_img->vs();
	M = 0;
	for (k = 0; k < N; k++)
	{
		if (p_mask[k] == true)
		{
			M++;
		}
	}
	if (M == N) { return false; }

	ptr_img = new unsigned char *[3]; ptr_img2 = new unsigned char *[3];
	ptr_img[0]  = ptr_r;  ptr_img[1]  = ptr_g;  ptr_img[2]  = ptr_b;
	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;

	ptr_C = C.c_Create(M, N);
	for (c = 0; c < 3; c++)
	{
		ptr_f1 = f1.c_Create(N);
		for (k = 0; k < N; k++)
		{
			ptr_f1[k] = (double)ptr_img[c][k];
		}

		for (l = 0; l < M; l++)
		{
			for (m = 0; m < N; m++)
			{
				tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)M);
				tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)N);

				a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp2 - tmp1));

				if ((a*b) == 0.0)
				{
					cos_val = 0.0;
					for (k = 0; k < M; k++) { cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
					ptr_C[l][m] = sqrt((double)((double)M / (double)N))*sqrt(1.0 / (double)((double)N*(double)M)) +
						(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
				}
				else
				{
					s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
					s2 = sin((PI / 4)*(tmp2 - tmp1)*((2 * M) - 1)) / b;
					ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
					ptr_C[l][m] += (double)((double)(1.0 / (double)N) - (double)(1.0 / sqrt((double)N*M)));
				}
			}
		}




		am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
		ptr_f2 = f2.vp();

		count = 0;
		for (j = 0; j < N; j++)
		{
			if (p_mask[j] == true)
			{
				ptr_img2[c][j] = (unsigned char)limit(ptr_f2[count]);
				count++;
			}
			else
			{
				ptr_img2[c][j] = 0;
			}
		}
	}


	delete[] ptr_img2;
	delete[] ptr_img;
	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Backward_on_Spatial_Domain(
	CKvVectorUcharRgb *io_img,
	CKvVectorBool *in_mask)
//************************************************************************
{
	CKvVectorUcharRgb iei_img;
	CKvVectorInt pt_list;
	CKvVectorInt ei_pt_list;
	bool *p_mask;
	float scale;
	int num_max_size, num_valid_point_size, k;
	int *p_ptlist, *p_eipt;
	unsigned char *pr1, *pg1, *pb1, *pr2, *pg2, *pb2;

	p_mask = in_mask->vps(num_max_size);
	num_valid_point_size = 0;
	for (k = 0; k < num_max_size; k++)
	{
		if (p_mask[k] == true)
		{
			num_valid_point_size++;
		}
	}

	if (num_valid_point_size == 0)
	{
		return false;
	}

	if (num_max_size == num_valid_point_size)
	{
		return true;
	}

	if (num_valid_point_size == 1)
	{
		pr2 = io_img->vp(pg2, pb2);
		unsigned char Red, Green, Blue;
		for (k = 0; k < num_max_size; k++)
		{
			if (p_mask[k] == true)
			{
				Red   = pr2[k];
				Green = pg2[k];
				Blue  = pb2[k];

				io_img->sv_Set_Vector(Kv_Rgb(0, 0, 0));
				io_img->se_Set_Element(k,Kv_Rgb(Red,Green,Blue));

				break;
			}
		}

		return true;
	}


	p_ptlist = pt_list.c_Create(num_valid_point_size);
	num_valid_point_size = 0;
	for (k = 0; k < num_max_size; k++)
	{
		if (p_mask[k] == true)
		{
			p_ptlist[num_valid_point_size] = k;
			num_valid_point_size++;
		}
	}

	scale = (float)(num_max_size-1) / (float)p_ptlist[num_valid_point_size - 1];
	p_eipt = ei_pt_list.c_Create(num_valid_point_size);
	for (k = 0; k < num_valid_point_size; k++)
	{
		p_eipt[k] = int(round((float)p_ptlist[k] * scale));
	}

	iei_img.c_Create(num_max_size, Kv_Rgb(0, 0, 0));
	pr1 = iei_img.vp(pg1, pb1);
	pr2 = io_img->vp(pg2, pb2);

	for (k = 0; k < num_valid_point_size; k++)
	{
		pr1[p_ptlist[k]] = pr2[p_eipt[k]];
		pg1[p_ptlist[k]] = pg2[p_eipt[k]];
		pb1[p_ptlist[k]] = pb2[p_eipt[k]];
	}

	io_img->cp_Copy(&iei_img);

	return true;
}
//************************************************************************
void VCL_EI_DCT::EI_DCT_Backward(
	CKvMatrixUcharRgb *io_img,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	LCKvAlgebra_for_Matrix am;
	CKvMatrixUcharRgb cpy_triangle_img; 
	CKvMatrixBool cpy_mask; CKvRgb rgb; 
	CKvMatrix C; CKvVector f1, f2; CKvXvector x_f1;
	CKvVectorBool row_bool;
	unsigned char ***ptr_img, ***ptr_img2, **ptr_r, **ptr_g, **ptr_b, **ptr_r2, **ptr_g2, **ptr_b2;
	double tmp1, tmp2, s1, s2, a, b, cos_val; double **ptr_C, *ptr_f2;
	int ww, hh, j, i, l, m, k, c, M, N, count; bool **p_mask, **p_cpy_mask;

	rgb.r = 0; rgb.g = 0; rgb.b = 0;

	cpy_triangle_img.cp_Copy(io_img); 
	cpy_mask.cp_Copy(in_mask);
	
	ptr_r      = cpy_triangle_img.mp(ptr_g, ptr_b);
	p_cpy_mask = cpy_mask.mp();
	p_mask     = in_mask->mp();

	cpy_triangle_img.ms(ww, hh);

	c = 0;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_mask[j][i])
			{
				c++;
			}
		}
	}
	if (hh*ww == c){ return; }

	io_img->c_Create(hh, ww, rgb);
	ptr_r2 = io_img->mp(ptr_g2, ptr_b2);

	ptr_img = new unsigned char **[3]; ptr_img2 = new unsigned char **[3];
	p_mask = in_mask->mp();

	for (j = 0; j < hh; j++)
	{
		cpy_mask.gr_Get_Row(0, j, ww, &row_bool);

		for (i = 0; i < ww; i++)
		{
			if (row_bool.ge_Get_Element(i) == true)
			{
				cpy_mask.sr_Set_Row(0, j, ww, true);
				break;
			}
		}
	}

	// col
	N = hh;

	ptr_img[0] = ptr_r;   ptr_img[1] = ptr_g;   ptr_img[2] = ptr_b;
	ptr_img2[0] = ptr_r2; ptr_img2[1] = ptr_g2; ptr_img2[2] = ptr_b2;


	for (c = 0; c < 3; c++)
	{
		for (i = 0; i< ww; i++)
		{
			x_f1.in_Initialize(1);
			M = 0;
			for (j = 0; j < hh; j++)
			{
				x_f1.ap_Append((double)ptr_img[c][j][i]);
				if (p_cpy_mask[j][i] == true){ M++; }
			}

			if (M != 0)
			{
				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(M, N);

				for (l = 0; l < M; l++)
				{
					for (m = 0; m < N; m++)
					{
						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)M);
						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)N);

						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp2 - tmp1));

						if ((a*b) == 0.0)
						{
							cos_val = 0.0;
							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
							ptr_C[l][m] = sqrt((double)((double)M / (double)N))*sqrt(1.0 / (double)((double)N*(double)M)) +
								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
						}
						else
						{
							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
							s2 = sin((PI / 4)*(tmp2 - tmp1)*((2 * M) - 1)) / b;
							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
							ptr_C[l][m] += (double)((double)(1.0 / (double)N) - (double)(1.0 / sqrt((double)N*M)));
						}
					}
				}

				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
				ptr_f2 = f2.vp();

				count = 0;
				for (j = 0; j < hh; j++)
				{
					if (p_cpy_mask[j][i] == true)
					{
						ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[count]);
						count++;
					}
				}
			}
		}
	}

	//row
	N = ww;

	for (c = 0; c < 3; c++)
	{
		for (j = 0; j < hh; j++)
		{
			x_f1.in_Initialize(1);
			M = 0;
			for (i = 0; i < ww; i++)
			{
				x_f1.ap_Append((double)ptr_img2[c][j][i]);
				if (p_mask[j][i] == true){ M++; }
			}

			if (M != 0)
			{
				x_f1.ex_Export(&f1, NULL); ptr_C = C.c_Create(M, N);

				for (l = 0; l < M; l++)
				{
					for (m = 0; m < N; m++)
					{
						tmp1 = (double)(((2.0*(double)l) + 1.0) / (double)M);
						tmp2 = (double)(((2.0*(double)m) + 1.0) / (double)N);

						a = sin((PI / 4)*(tmp1 + tmp2)); b = sin((PI / 4)*(tmp2 - tmp1));

						if ((a*b) == 0.0)
						{
							cos_val = 0.0;
							for (k = 0; k < M; k++){ cos_val += (cos(tmp1*(PI / 2)*k)*cos(tmp2*(PI / 2)*k)); }
							ptr_C[l][m] = sqrt((double)((double)M / (double)N))*sqrt(1.0 / (double)((double)N*(double)M)) +
								(2.0 / sqrt((double)((double)N*(double)M)))*(cos_val - 1.0);
						}
						else
						{
							s1 = sin((PI / 4)*(tmp1 + tmp2)*((2 * M) - 1)) / a;
							s2 = sin((PI / 4)*(tmp2 - tmp1)*((2 * M) - 1)) / b;
							ptr_C[l][m] = (double)((double)(s1 + s2) / (2.0*sqrt((double)((double)N*(double)M))));
							ptr_C[l][m] += (double)((double)(1.0 / (double)N) - (double)(1.0 / sqrt((double)N*M)));
						}
					}
				}

				am.mmv_Multiply_Matrix_Vector(&C, &f1, &f2);
				ptr_f2 = f2.vp();

				count = 0;
				for (i = 0; i < ww; i++)
				{
					if (p_mask[j][i] == true)
					{
						ptr_img2[c][j][i] = (unsigned char)limit(ptr_f2[count]);
						count++;
					}
					else
					{
						ptr_img2[c][j][i] = 0;
					}
				}
			}
		}
	}

	delete[] ptr_img2;
	delete[] ptr_img;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Line_Backward(
	CKvMatrixUcharRgb *io_img,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	CKvMatrixUcharRgb iei_map;
	CKvSet_of_VectorUcharRgb set_line_color;
	CKvSet_of_VectorBool     set_line_mask;
	CKvVectorUcharRgb        *p_line;
	CKvVectorBool            *p_line_mask;
	int ww, hh, y;

	io_img->ms(ww, hh);
	p_line = set_line_color.c_Create(hh);
	p_line_mask = set_line_mask.c_Create(hh);
	iei_map.c_Create(hh, ww, Kv_Rgb(0, 0, 0));

#pragma omp parallel
	{
#pragma omp for
		for (y = 0; y < hh; y++)
		{
			io_img->gr_Get_Row(0, y, ww, &p_line[y]);
			in_mask->gr_Get_Row(0, y, ww, &p_line_mask[y]);
			EI_DCT_Backward(
				&p_line[y],//CKvVectorUcharRgb *io_img,
				&p_line_mask[y]);//CKvVectorBool *in_mask)
			iei_map.sr_Set_Row(0, y, &p_line[y]);
		}
	}

	io_img->cp_Copy(&iei_map);

	return true;
}
//************************************************************************
bool VCL_EI_DCT::EI_DCT_Line_Backward_on_Spatial_Domain(
	CKvMatrixUcharRgb *io_img,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	CKvMatrixUcharRgb iei_map;
	CKvSet_of_VectorUcharRgb set_line_color;
	CKvSet_of_VectorBool     set_line_mask;
	CKvVectorUcharRgb        *p_line;
	CKvVectorBool            *p_line_mask;
	int ww, hh, y;

	io_img->ms(ww, hh);
	p_line = set_line_color.c_Create(hh);
	p_line_mask = set_line_mask.c_Create(hh);
	iei_map.c_Create(hh, ww, Kv_Rgb(0, 0, 0));

#pragma omp parallel
	{
#pragma omp for
		for (y = 0; y < hh; y++)
		{
			io_img->gr_Get_Row(0, y, ww, &p_line[y]);
			in_mask->gr_Get_Row(0, y, ww, &p_line_mask[y]);
			EI_DCT_Backward_on_Spatial_Domain(
				&p_line[y],//CKvVectorUcharRgb *io_img,
				&p_line_mask[y]);//CKvVectorBool *in_mask)
			iei_map.sr_Set_Row(0, y, &p_line[y]);
		}
	}

	io_img->cp_Copy(&iei_map);

	return true;
}
//************************************************************************
void VCL_EI_DCT::EI_Block_DCT_Backward(
	CKvMatrixUcharRgb *io_img,
	int &in_block_x,
	int &in_block_y,
	CKvMatrixBool *in_mask)
//************************************************************************
{
	CKvMatrixUcharRgb color_images; CKvMatrixBool mask;
	CKvMatrixUcharRgb block_img;    CKvMatrixBool block_mask;
	int ww, hh, j, i;

	io_img->ms(ww, hh);

	color_images.cp_Copy(io_img);
	mask.cp_Copy(in_mask);
	
	for (j = 0; j < hh; j += in_block_y)
	{
		for (i = 0; i < ww; i += in_block_x)
		{
			color_images.gb_Get_Block(i, j, in_block_x, in_block_y, &block_img);
			mask.gb_Get_Block(i, j, in_block_x, in_block_y, &block_mask);
			EI_DCT_Backward(&block_img, &block_mask);
			color_images.sb_Set_Block(i, j, &block_img);
		}
	}

	io_img->cp_Copy(&color_images);
	in_mask->cp_Copy(&mask);
}
//************************************************************************
void VCL_EI_DCT::reverse_vector(
	CKvVectorUcharRgb *io_img_vec,
	CKvVectorInt *io_idx_vec,
	CKvVectorInt *io_reference_idx_vec,
	CKvVectorBool *io_mask)
//************************************************************************
{
	CKvVectorUcharRgb tmp_img_vec;
	CKvVectorInt tmp_idx_vec;
	CKvVectorInt tmp_reference_idx_vec;
	CKvVectorBool tmp_mask;
	int sz, k;
	unsigned char *pr1, *pg1, *pb1;
	unsigned char *pr2, *pg2, *pb2;
	int *p_idx1, *p_idx2;
	bool *p_mask1, *p_mask2;

	sz = io_img_vec->vs();
	tmp_img_vec.c_Create(sz);
	pr1 = io_img_vec->vp(pg1, pb1);
	pr2 = tmp_img_vec.vp(pg2, pb2);

	for (k = 0; k < sz; k++)
	{
		pr2[k] = pr1[sz - 1 - k];
		pg2[k] = pg1[sz - 1 - k];
		pb2[k] = pb1[sz - 1 - k];
	}

	sz = io_idx_vec->vs();
	tmp_idx_vec.c_Create(sz);
	p_idx1 = io_idx_vec->vp();
	p_idx2 = tmp_idx_vec.vp();

	for (k = 0; k < sz; k++)
	{
		p_idx2[k] = p_idx1[sz - 1 - k];
	}

	sz = io_reference_idx_vec->vs();
	tmp_reference_idx_vec.c_Create(sz);
	p_idx1 = io_reference_idx_vec->vp();
	p_idx2 = tmp_reference_idx_vec.vp();

	for (k = 0; k < sz; k++)
	{
		p_idx2[k] = p_idx1[sz - 1 - k];
	}

	sz = io_mask->vs();
	tmp_mask.c_Create(sz);
	p_mask1 = io_mask->vp();
	p_mask2 = tmp_mask.vp();

	for (k = 0; k < sz; k++)
	{
		p_mask2[k] = p_mask1[sz - 1 - k];
	}

	io_img_vec->cp_Copy(&tmp_img_vec);
	io_idx_vec->cp_Copy(&tmp_idx_vec);
	io_reference_idx_vec->cp_Copy(&tmp_reference_idx_vec);
	io_mask->cp_Copy(&tmp_mask);
}
//************************************************************************
void VCL_EI_DCT::transform_points(
	int in_current_length,
	int in_number_of_control_points,
	int in_max_length,
	std::vector<int> &out_control_points)
//************************************************************************
{
	float stepf, cost_min, scale, l, p;
	int num_blob, k;

	num_blob = in_number_of_control_points + 1;
	stepf = (float)in_current_length / (float)num_blob;
	scale = (float)(in_max_length - 1) / (float)(in_current_length - 1);
	k = 0;
	p = stepf;
	while (k < in_number_of_control_points)
	{
		out_control_points.push_back(p*scale);
		k++;
		p += stepf;
	}


}
//************************************************************************
void VCL_EI_DCT::transform_point(
	int &in_position,
	int &out_position,
	int &in_max_length)
//************************************************************************
{

}
//************************************************************************
