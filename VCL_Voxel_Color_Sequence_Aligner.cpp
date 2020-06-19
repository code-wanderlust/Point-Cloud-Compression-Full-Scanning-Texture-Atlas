#include "stdafx.h"
#include "VCL_Voxel_Color_Sequence_Aligner.h"


//************************************************************************
VCL_Voxel_Color_Sequence_Aligner::VCL_Voxel_Color_Sequence_Aligner()
//************************************************************************
{

}
//************************************************************************
VCL_Voxel_Color_Sequence_Aligner::~VCL_Voxel_Color_Sequence_Aligner()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner::atoc_Align_Texture_on_Code(
	VCL_DoCube_X_Color *in_docube,
	int in_alignment_mode,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offsets)
//************************************************************************
{
	CKvVectorInt *p_offset;
	int *p_po, min_value;
	int num_blob, num_toc, m, n;


	if (in_alignment_mode == 0)
	{
		acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);
	}
	else if (in_alignment_mode == 1)
	{
		acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);

	}
	else if (in_alignment_mode == 2)
	{
		//acc_Align_Texture_on_Codes_Using_Connectivity(
		//	in_docube,//VCL_DoCube_X_Color *in_docube,
		//	io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	out_offsets);//CKvSet_of_VectorInt *out_offset)

		//Align_Texture_on_Codes_Using_DTW2(
		//	in_docube,//VCL_DoCube_X_Color *in_docube,
		//	io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	out_offsets);//CKvSet_of_VectorInt *out_offset)

		//Align_Texture_on_Codes_Using_PWEI(
		//	in_docube,//VCL_DoCube_X_Color *in_docube,
		//	io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	out_offsets);//CKvSet_of_VectorInt *out_offset)

		//Align_Texture_on_Codes_Using_PWEI(
		//	in_docube,//VCL_DoCube_X_Color *in_docube,
		//	io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	2,//int in_number_of_control_points,
		//	out_offsets);//CKvSet_of_VectorInt *out_offset)

		acc_Center_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);

		Align_Texture_on_Codes_Using_PWEI2(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
				2,//int in_number_of_control_points,
				out_offsets);//CKvSet_of_VectorInt *out_offset)


		//Align_Texture_on_Codes_Using_PWEI_Approximate2(
		//	in_docube,//VCL_DoCube_X_Color *in_docube,
		//	io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
		//	1,
		//	out_offsets);//CKvSet_of_VectorInt *out_offset)
	}


	p_offset = out_offsets->vps(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		p_po = p_offset[m].vps(num_toc);
		min_value = p_offset[m].min_Minimum(NULL);
		for (n = 0; n < num_toc; n++)
		{
			p_po[n] = p_po[n] - min_value;
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner::atoc_Align_Texture_on_Code_for_Video_Color(
	VCL_DoCube_X_Color *in_docube,
	int in_alignment_mode,
	CKvSet_of_Voxel *in_before_set_of_voxels,
	CKvSet_of_RgbaF *in_before_set_of_colors,
	std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offsets)
//************************************************************************
{
	CKvVectorInt *p_offset;
	int *p_po, min_value;
	int num_blob, num_toc, m, n;


	if (in_alignment_mode == 0)
	{
		acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2_for_video(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			in_before_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
			in_before_set_of_voxels,//CKvSet_of_Voxel *in_before_set_of_voxels,
			in_before_set_of_colors,//CKvSet_of_RgbaF *in_before_set_of_colors,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);
	}
	else if (in_alignment_mode == 1)
	{
		acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2_for_video(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			in_before_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
			in_before_set_of_voxels,//CKvSet_of_Voxel *in_before_set_of_voxels,
			in_before_set_of_colors,//CKvSet_of_RgbaF *in_before_set_of_colors,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);
	}
	else if (in_alignment_mode == 2)
	{

	}


	p_offset = out_offsets->vps(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		p_po = p_offset[m].vps(num_toc);
		min_value = p_offset[m].min_Minimum(NULL);
		for (n = 0; n < num_toc; n++)
		{
			p_po[n] = p_po[n] - min_value;
		}
	}



}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner::atoc_Align_Texture_on_Code(
	VCL_DoCube_X_Color *in_docube,
	int in_alignment_mode,
	int &in_random_number,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offsets)
//************************************************************************
{
	CKvVectorInt *p_offset;
	int *p_po, min_value;
	int num_blob, num_toc, m, n;


	if (in_alignment_mode == 0)
	{
		acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_random_number,//int in_random_number,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);
	}
	else if (in_alignment_mode == 1)
	{
		acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_random_number,//int in_random_number,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);
	}
	else if (in_alignment_mode == 2)
	{

		acc_Center_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);

		Align_Texture_on_Codes_Using_PWEI2(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			io_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
			2,//int in_number_of_control_points,
			out_offsets);//CKvSet_of_VectorInt *out_offset)
	}


	p_offset = out_offsets->vps(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		p_po = p_offset[m].vps(num_toc);
		min_value = p_offset[m].min_Minimum(NULL);
		for (n = 0; n < num_toc; n++)
		{
			p_po[n] = p_po[n] - min_value;
		}
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
//************************************************************************
//************************************************************************
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner::atoc_Align_Texture_on_Code_WO_partitioning(
	VCL_DoCube_X_Color *in_docube,
	int in_alignment_mode,
	std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offsets)
//************************************************************************
{
	std::vector<std::vector<std::vector<int>>> toc;
	CKvVectorInt *p_offset;
	int *p_po, min_value;
	int num_blob, num_toc, m, n;

	if (in_alignment_mode == 0)
	{
		acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance_WO_Partitining(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_voxel_sequence,//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
			toc,//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);
	}
	else if (in_alignment_mode == 1)
	{

		acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference_WO_Partitining(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_voxel_sequence,//std::vector<std::vector<Voxel_Slice_Scanned_Data>> &in_voxel_sequence,
			toc,//std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code,
			out_circular_shift,//CKvSet_of_VectorInt *out_circular_shift,
			out_offsets);//CKvSet_of_VectorInt *out_offset);
	}


	p_offset = out_offsets->vps(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		p_po = p_offset[m].vps(num_toc);
		min_value = p_offset[m].min_Minimum(NULL);
		for (n = 0; n < num_toc; n++)
		{
			p_po[n] = p_po[n] - min_value;
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner::acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance_WO_Partitining(
	VCL_DoCube_X_Color *in_docube,
	std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
	std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	std::vector<std::vector<int>> tmp_memory;
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	CKvMatrixBool connectivity_matrix; bool **p_cm;
	int num_slice, num_toc, m, j, i, ww, hh;
	int *p_v, *p_pcs;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_slice = in_voxel_sequence.size();
	p_vec     = out_offset->c_Create(num_slice);
	p_cs      = out_circular_shift->c_Create(num_slice);

	p_vec[0].c_Create(in_voxel_sequence[0].voxel_idx_sequences.size(),0);
	p_cs[0].c_Create(in_voxel_sequence[0].voxel_idx_sequences.size(),0);
	for (m = 1; m < num_slice; m++)
	{
		num_toc = in_voxel_sequence[m].voxel_idx_sequences.size();
		if (num_toc == 0)
		{
			continue;
		}

		fcm_Find_Connectivity_Matrix(
			in_voxel_sequence[m],//Voxel_Slice_Scanned_Data &in_current_pcc,
			in_voxel_sequence[m-1],//Voxel_Slice_Scanned_Data &in_before_pcc,
			connectivity_matrix);//CKvMatrixBool &out_connectivity_matrix);

		p_v   = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

		p_cm = connectivity_matrix.mps(ww,hh);

		for (j = 0; j < hh; j++) // before
		{
			for (i = 0; i < ww; i++) // current
			{
				if (p_cm[j][i] == true)
				{
					p_pcs[i] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
						&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
						in_voxel_sequence[m - 1].voxel_idx_sequences[j],//std::vector<int> &in_reference_texture_on_code,
						in_voxel_sequence[m].voxel_idx_sequences[i],//std::vector<int> &io_target_texture_on_code,
						p_v[i]);//int &out_offset);

					p_v[i] = p_v[i] + p_vec[m - 1].ge_Get_Element(j);

				}

			}
		}
	}



}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner::acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference_WO_Partitining(
	VCL_DoCube_X_Color *in_docube,
	std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
	std::vector<std::vector<std::vector<int>>> &out_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	//CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	//CKvVectorInt *p_vec, *p_cs;
	//int num_blob, num_toc, m, n, dum;
	//int *p_v, *p_pcs;

	//v = in_docube->gvc_Get_Voxel_Color();
	//v.e_Export(&set_v); v.in_Initialize();

	//num_blob = io_segmented_texture_on_code.size();
	//p_vec = out_offset->c_Create(num_blob);
	//p_cs = out_circular_shift->c_Create(num_blob);
	//for (m = 0; m < num_blob; m++)
	//{
	//	num_toc = io_segmented_texture_on_code[m].size();
	//	p_v = p_vec[m].c_Create(num_toc, 0);
	//	p_pcs = p_cs[m].c_Create(num_toc, 0);
	//	for (n = 1; n < num_toc; n++)
	//	{
	//		p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
	//			&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
	//			io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
	//			io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
	//			p_v[n]);//int &out_offset);

	//		p_v[n] = p_v[n] + p_v[n - 1];
	//	}
	//}

}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner::fcm_Find_Connectivity_Matrix(
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


