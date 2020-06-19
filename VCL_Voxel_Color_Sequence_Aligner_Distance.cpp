#include "stdafx.h"
#include "VCL_Voxel_Color_Sequence_Aligner_Distance.h"

//new_snr1 =
//
//0.1224
//
//
//new_rate1 =
//
//-2.2433
//
//
//new_snr2 =
//
//0.1209
//
//
//new_rate2 =
//
//-2.1869

//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Distance::VCL_Voxel_Color_Sequence_Aligner_Distance()
//************************************************************************
{

}
//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Distance::~VCL_Voxel_Color_Sequence_Aligner_Distance()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, m, n;
	int *p_v, *p_pcs;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);
		for (n = 1; n < num_toc; n++)
		{
			//atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_WO_translation(
			//	&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
			//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	p_v[n]);//int &out_offset);

			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance2(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, num, sz, m, n;
	int *p_v, *p_pcs;
	int criteria_idx, max_length;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

		/*****************************************************************************************/
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
		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Largest texture on code.  -> Using Geometry
		Rotate_Texture_on_Code_by_Centering(
			&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
			io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &io_texture_on_code,
			1);//int in_plane_mode)
		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Start
		for (n = criteria_idx + 1; n < num_toc; n++)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
		/*****************************************************************************************/
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2_for_video(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
	CKvSet_of_Voxel *in_before_set_of_voxels,
	CKvSet_of_RgbaF *in_before_set_of_colors,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, num, sz, m, n;
	int *p_v, *p_pcs;
	int criteria_idx, max_length;
	int global_circular_shift;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec    = out_offset->c_Create(num_blob);
	p_cs     = out_circular_shift->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

		/*****************************************************************************************/
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
		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Largest texture on code. -> Using Color
		CKvDepot_of_RgbaF vv; CKvSet_of_RgbaF set_vv;
		CKvSet_of_RgbaF before_set_colors;
		
		if (Find_Corresponding_ToC(
			in_before_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_before_ToC,
			io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &in_current_ToC,
			in_before_set_of_voxels,//CKvSet_of_Voxel *in_before_voxels,
			in_before_set_of_colors,//CKvSet_of_RgbaF *in_before_colors,
			in_docube,//VCL_DoCube_X_Color *in_current_docube,
			&before_set_colors))//CKvSet_of_RgbaF *out_ToC_in_before)
		{
			vv = in_docube->gvc_Get_Voxel_Color();
			vv.e_Export(&set_vv); vv.in_Initialize();

			Rotate_Texture_on_Code_by_Color(
				&set_vv,//CKvSet_of_RgbaF *in_set_of_colors,
				io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &io_texture_on_code,
				&before_set_colors,//CKvSet_of_RgbaF *in_before_colors,
				global_circular_shift);//int &out_circular_shift);
			zz_global_circular_shift.push_back(global_circular_shift);
		}
		else
		{
			Rotate_Texture_on_Code_by_Centering(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &io_texture_on_code,
				1);//int in_plane_mode)
		}

		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Start
		for (n = criteria_idx + 1; n < num_toc; n++)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
		/*****************************************************************************************/
	}
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance2(
	VCL_DoCube_X_Color *in_docube,
	int in_random_number,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, num, sz, m, n;
	int *p_v, *p_pcs;
	int criteria_idx, max_length;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

		/*****************************************************************************************/
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
		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Largest texture on code. 
		//Rotate_Texture_on_Code_by_Centering(
		//	&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
		//	io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &io_texture_on_code,
		//	1);//int in_plane_mode)
		//Rotate_Texture_on_Code_Randomly(
		//	io_segmented_texture_on_code[m][criteria_idx]);// std::vector<int> &io_texture_on_code);

		sz  = io_segmented_texture_on_code[m][criteria_idx].size();
		num = in_random_number % (sz);
		sv_Circular_Shift_Vector(
			num,//int in_location,
			io_segmented_texture_on_code[m][criteria_idx]);//std::vector<int> &io_vector);
		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Start
		for (n = criteria_idx + 1; n < num_toc; n++)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
		/*****************************************************************************************/
	}
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance3(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, m, n;
	int *p_v, *p_pcs;

	int criteria_idx, max_length;


	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

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
			printf("m=%d/%d) n=%d/%d\n", m, num_blob, n, num_toc);
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Double_Circular_Shift(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			printf("m=%d/%d) n=%d/%d\n", m, -1, n, num_toc);
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Double_Circular_Shift(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::acc_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance_Piece_Wise(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	int in_number_of_element_for_mask,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, m, n;
	int *p_v, *p_pcs;

	int criteria_idx, max_length;


	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

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
			atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Piece_Wise(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				in_number_of_element_for_mask,//int in_number_of_element_for_mask,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Piece_Wise(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				in_number_of_element_for_mask,//int in_number_of_element_for_mask,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
	}




}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::acc_Center_Align_Texture_on_Codes_Using_Voxel_Euclidean_Distance(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel v; CKvSet_of_Voxel set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, m, n;
	int *p_v, *p_pcs;
	int criteria_idx, max_length;

	v = in_docube->gsp_Get_Surface_Voxels();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		if (num_toc == 0) continue;

		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

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
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Centering(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Centering(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Distance::atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_WO_translation(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	std::vector<int> tmp;
	CKvVoxel *p_v;
	double cost, min_cost;
	int sz1, sz2, search_length, circular_shift_idx, k, l;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }

	p_v = in_set_of_voxels->vp();
	min_cost = 999999999999999999.9999999;
	for (l = 0; l < sz2; l++)
	{
		tmp = io_target_texture_on_code;
		sv_Circular_Shift_Vector(l, tmp);
		cost = 0.0;
#pragma omp parallel
		{
#pragma omp for reduction(+:cost)
			for (k = 0; k < search_length; k++)
			{
				if (in_reference_texture_on_code[k] != -1)
				{
					cost += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[tmp[k]]);
				}

			}
		}

		if (min_cost > cost)
		{
			min_cost = cost;
			circular_shift_idx = l;
		}
	}

	sv_Circular_Shift_Vector(circular_shift_idx, io_target_texture_on_code);
	out_offset = 0;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Distance::atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	std::vector<int> tmp;
	CKvVoxel *p_v;
	double cost, min_cost;
	int sz1, sz2, length, search_length, circular_shift_idx, k, l, j;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }
	length = abs(sz2 - sz1) + 1;

	p_v = in_set_of_voxels->vp();
	min_cost = 999999999999999999.9999999;

	if (sz1 < sz2)
	{
		for (j = 0; j < length; j++)
		{
			for (l = 0; l < sz2; l++)
			{
				tmp = io_target_texture_on_code;
				sv_Circular_Shift_Vector(l, tmp);
				cost = 0.0;
#pragma omp parallel
				{
#pragma omp for reduction(+:cost)
					for (k = 0; k < search_length; k++)
					{
						cost += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[tmp[k + j]]);
					}
				}

				if (min_cost > cost)
				{
					min_cost = cost;
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
				tmp = io_target_texture_on_code;
				sv_Circular_Shift_Vector(l, tmp);
				cost = 0.0;
#pragma omp parallel
				{
#pragma omp for reduction(+:cost)
					for (k = 0; k < search_length; k++)
					{
						cost += (double)p_v[in_reference_texture_on_code[k + j]].ds_Distance_Squared(p_v[tmp[k]]);
					}
				}

				if (min_cost > cost)
				{
					min_cost = cost;
					circular_shift_idx = l;
					out_offset = j;
				}
			}
		}
	}

	sv_Circular_Shift_Vector(circular_shift_idx, io_target_texture_on_code);

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Distance::atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance2(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	std::vector<int> tmp;
	CKvVoxel *p_v;
	double cost, min_cost;
	int sz1, sz2, length, search_length, circular_shift_idx, k, l, j;
	int num_points;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }
	length = abs(sz2 - sz1) + 1;

	p_v = in_set_of_voxels->vp();
	min_cost = 999999999999999999.9999999;

	if (sz1 < sz2)
	{
		for (j = 0; j < length; j++)
		{
			for (l = 0; l < sz2; l++)
			{
				tmp = io_target_texture_on_code;
				sv_Circular_Shift_Vector(l, tmp);
				cost = 0.0; num_points = 0;

#pragma omp parallel
				{
#pragma omp for reduction(+:cost) reduction(+:num_points)
					for (k = 0; k < search_length; k++)
					{
						if ((in_reference_texture_on_code[k] != -1) && (tmp[k + j] != -1))
						{
							cost += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[tmp[k + j]]);
							num_points++;
						}
					}
				}
				
				if (num_points != 0)
				{
					cost /= (double)num_points;
					if (min_cost > cost)
					{
						min_cost = cost;
						circular_shift_idx = l;
						out_offset = -j;
					}
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
				tmp = io_target_texture_on_code;
				sv_Circular_Shift_Vector(l, tmp);
				cost = 0.0; num_points = 0;
#pragma omp parallel
				{
#pragma omp for reduction(+:cost) reduction(+:num_points)
					for (k = 0; k < search_length; k++)
					{
						if ((in_reference_texture_on_code[k] != -1) && (tmp[k + j] != -1))
						{
							cost += (double)p_v[in_reference_texture_on_code[k + j]].ds_Distance_Squared(p_v[tmp[k]]);
							num_points++;
						}
					}
				}

				if (num_points != 0)
				{
					cost /= (double)num_points;
					if (min_cost > cost)
					{
						min_cost = cost;
						circular_shift_idx = l;
						out_offset = -j;
					}
				}
			}
		}
	}

	sv_Circular_Shift_Vector(circular_shift_idx, io_target_texture_on_code);

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Distance::atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Double_Circular_Shift(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	std::vector<int> buffer_mem;
	std::vector<int> tmp;
	CKvVoxel *p_v;
	double cost, min_cost;
	int sz1, sz2, length, search_length, circular_shift_idx, k, l, j, i;
	int count;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();
	
	sz1 = 0;
	for (k = 0; k < in_reference_texture_on_code.size(); k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			sz1++;
		}
	}

	if (sz1 > sz2) { search_length = sz2; }
	else		   { search_length = sz1; }
	//length = abs(sz2 - sz1) + 1;

	sz1 = in_reference_texture_on_code.size();

	p_v = in_set_of_voxels->vp();
	min_cost = 999999999999999999.9999999;

	tmp = io_target_texture_on_code;
	for (l = 0; l < sz2; l++)
	{
		//sv_Circular_Shift_Vector(l, tmp);
		sv_Circular_Shift_Vector(1, tmp);
		buffer_mem = std::vector<int>(sz1, -1);
		for (k = 0; k < sz2; k++)
		{
			buffer_mem[k] = tmp[k];
		}
		for (j = 0; j < sz1; j++)
		{
			sv_Circular_Shift_Vector(1, buffer_mem);
			cost = 0.0;
			count = 0;
#pragma omp parallel
			{
#pragma omp for reduction(+:cost) reduction(+:count)
				for (k = 0; k < sz1; k++)
				{
					if ((in_reference_texture_on_code[k] != -1) && (buffer_mem[k] != -1))
					{
						cost += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[buffer_mem[k]]);
						count++;
					}
				}
			}
			
			if (search_length == count)
			{
				//cost /= count;
				if ((min_cost > cost) && (cost != 0.0))
				{
					min_cost = cost;
					circular_shift_idx = l;
					out_offset = j;
				}
			}

		}
	}

	tmp        = io_target_texture_on_code;
	sv_Circular_Shift_Vector(circular_shift_idx, tmp);
	buffer_mem = std::vector<int>(sz1, -1);
	for (k = 0; k < sz2; k++)
	{
		buffer_mem[k] = tmp[k];
	}
	sv_Circular_Shift_Vector(out_offset, buffer_mem);
	io_target_texture_on_code = buffer_mem;
	out_offset = 0;


	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Distance::atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Centering(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	std::vector<int> tmp;
	CKvVoxel *p_v;
	double cost, min_cost;
	int sz1, sz2, length, search_length, circular_shift_idx, k, l, j;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else           { search_length = sz1; }

	length = abs(sz2 - sz1) + 1;
	j = (int)((length) / 2.0);

	p_v = in_set_of_voxels->vp();
	min_cost = 999999999999999999.9999999;

	if (sz1 < sz2)
	{
		for (l = 0; l < sz2; l++)
		{
			tmp = io_target_texture_on_code;
			sv_Circular_Shift_Vector(l, tmp);

			cost = 0.0;
			for (k = 0; k < search_length; k++)
			{
				cost += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[tmp[k + j]]);
			}

			if (min_cost > cost)
			{
				min_cost = cost;
				circular_shift_idx = l;
				out_offset = -j;
			}
		}
	}
	else
	{
		for (l = 0; l < sz2; l++)
		{
			tmp = io_target_texture_on_code;
			sv_Circular_Shift_Vector(l, tmp);
			
			cost = 0.0;
			for (k = 0; k < search_length; k++)
			{
				cost += (double)p_v[in_reference_texture_on_code[k + j]].ds_Distance_Squared(p_v[tmp[k]]);
			}

			if (min_cost > cost)
			{
				min_cost = cost;
				circular_shift_idx = l;
				out_offset = j;
			}
		}
	}

	sv_Circular_Shift_Vector(circular_shift_idx, io_target_texture_on_code);

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Distance::atd_Align_Two_Texture_on_Codes_Using_Euclidean_Distance_Piece_Wise(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int in_number_of_element_for_mask,
	int &out_offset)
//************************************************************************
{
	// reference는 조작하지 않고, target를 조작하는 방향으로. 
	std::vector<int> updated_target_texture_on_code;
	std::vector<int> buffer_ToC;
	std::vector<int> sub_patch, matched_patch;
	std::vector<int> list_of_position;
	CKvVectorBool mask_vec;
	CKvVoxel *p_v;
	double cost, min_cost;
	int sz, length, idx, k, l, m, count;
	int patch_size, buffer_size;
	int num_element_reference, num_element_target, matched_location;
	bool *p_mask;

	sz = in_reference_texture_on_code.size();
	num_element_target = io_target_texture_on_code.size();
	num_element_reference = 0;
	for (k = 0; k < sz; k++)
	{
		if (in_reference_texture_on_code[k] != -1)
		{
			num_element_reference++;
		}
	}

	if (num_element_reference == 0)
	{
		io_target_texture_on_code = std::vector<int>(sz, -1);
		out_offset = 0;
		return 0;
	}

	patch_size                     = in_number_of_element_for_mask;
	buffer_size                    = patch_size - 1;
	updated_target_texture_on_code = std::vector<int>(sz, -1);
	if (num_element_target <= num_element_reference) // 2 <= 4 patch : 8 --> error
	{
		if (num_element_target < in_number_of_element_for_mask)
		{
			patch_size = num_element_target;
			buffer_size = patch_size - 1;
		}

		//Set buffer ToC.
		buffer_ToC = std::vector<int>(sz + buffer_size,-1);
		mask_vec.c_Create(sz + buffer_size, true);
		for (k = 0; k < sz; k++)
		{
			buffer_ToC[k + buffer_size] = in_reference_texture_on_code[k];
		}
		for (k = 0; k < buffer_size; k++)
		{
			buffer_ToC[k] = in_reference_texture_on_code[sz - buffer_size + k];
		}

		for (k = 0; k < num_element_target; k += in_number_of_element_for_mask)
		{
			/***************************Get a patch*******************************/
			idx = min(k + in_number_of_element_for_mask, num_element_target);
			sub_patch = std::vector<int>();
			for (l = k; l < idx; l++)
			{
				sub_patch.push_back(io_target_texture_on_code[l]);
			}
			/********************************************************************/

			if (compute_cost_over_ToC(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				buffer_size,//int in_buffer_size,
				buffer_ToC,//std::vector<int> &io_texture_on_code,
				sub_patch,//std::vector<int> &in_sub_patch,
				&mask_vec,//CKvVectorBool *io_texture_on_code_mask,
				matched_patch,//std::vector<int> &out_matched_patch, // in_reference_texture_on_code들의 element들로 구성되어 있음. 
				list_of_position))//std::vector<int> &out_location_of_elements_in_matched_patch) // in_reference_texture_on_code상의 위치임. (정확히는 buffer_ToC)
			{
				for (l = 0; l < list_of_position.size(); l++)
				{
					if (list_of_position[l] != -1)
					{
						updated_target_texture_on_code[list_of_position[l]] = sub_patch[l];
					}
				}
			}
		}
	}
	else
	{
		if (sz < in_number_of_element_for_mask)
		{
			patch_size = sz;
			buffer_size = patch_size - 1;
		}

		//Set buffer ToC.
		buffer_ToC = std::vector<int>(num_element_target + buffer_size, -1);
		mask_vec.c_Create(num_element_target + buffer_size, true);
		for (k = 0; k < num_element_target; k++)
		{
			buffer_ToC[k + buffer_size] = io_target_texture_on_code[k];
		}
		for (k = 0; k < buffer_size; k++)
		{
			buffer_ToC[k] = io_target_texture_on_code[num_element_target - buffer_size + k];
		}

		for (k = 0; k < sz; k += in_number_of_element_for_mask)
		{
			/***************************Get a patch*******************************/
			idx = min(k + in_number_of_element_for_mask, sz);
			sub_patch = std::vector<int>();
			for (l = k; l < idx; l++)
			{
				sub_patch.push_back(in_reference_texture_on_code[l]);
			}
			/********************************************************************/

			if (compute_cost_over_ToC(
				in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
				buffer_size,//int in_buffer_size,
				buffer_ToC,//std::vector<int> &io_texture_on_code,
				sub_patch,//std::vector<int> &in_sub_patch,
				&mask_vec,//CKvVectorBool *io_texture_on_code_mask,
				matched_patch,//std::vector<int> &out_matched_patch, //io_texture_on_code에 있는 element들로 구성되어 있음. 
				list_of_position))//std::vector<int> &out_location_of_elements_in_matched_patch) //여기에 있는 위치들은 io_texture_on_code 상에 있는 위치임. 
			{
				count = 0;
				for (l = k; l < idx; l++)
				{
					updated_target_texture_on_code[l] = matched_patch[count];
					count++;
				}
			}
		}
	}

	io_target_texture_on_code = std::vector<int>(updated_target_texture_on_code.size());
	for (k = 0; k < updated_target_texture_on_code.size(); k++)
	{
		io_target_texture_on_code[k] = updated_target_texture_on_code[k];
	}
	//io_target_texture_on_code = updated_target_texture_on_code;
	//updated_target_texture_on_code.~vector();


	out_offset = 0;
	return 0;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Distance::compute_cost_over_ToC(
	CKvSet_of_Voxel *in_set_of_voxels,
	int in_buffer_size,
	std::vector<int> &io_texture_on_code,
	std::vector<int> &in_sub_patch,
	CKvVectorBool *io_texture_on_code_mask,
	std::vector<int> &out_matched_patch,
	std::vector<int> &out_location_of_elements_in_matched_patch)
//************************************************************************
{
	std::vector<std::vector<int>> set_of_matched_patch;
	std::vector<std::vector<int>> set_of_location;
	std::vector<std::vector<int>> sub_ToC;
	std::vector<float> set_cost;
	std::vector<short> set_of_offset;
	CKvXrunset1dShort xrun1d;
	CKvXvectorShort *p_xvec;
	float min_cost;
	int sz, num_blob, count, k, l, idx, valid;
	short *ps, z, zo, z1;
	int zz_sz;


	valid = 0;
	for (k = 0; k < in_sub_patch.size(); k++)
	{
		if (in_sub_patch[k] != -1)
		{
			valid++;
		}
	}
	if (valid == 0)
	{
		return false;
	}


	xrun1d.im_Import(*io_texture_on_code_mask);
	xrun1d.z_px_Pointer_of_Xvector()->z_gii(NULL, &ps, &sz, NULL);

	if (sz == 0)
	{
		return false;
	}
	  
	num_blob = sz / 2;
	min_cost = FLT_MAX;

	set_of_matched_patch = std::vector<std::vector<int>>(num_blob);
	set_of_location      = std::vector<std::vector<int>>(num_blob);
	sub_ToC				 = std::vector<std::vector<int>>(num_blob);
	set_cost			 = std::vector<float>(num_blob);
	set_of_offset		 = std::vector<short>(num_blob);

	count = 0;
	for (k = 0; k < sz;)
	{
		zo = ps[k]; k++;
		z1 = ps[k]; k++;
		
		sub_ToC[count] = std::vector<int>();
		set_of_offset[count] = zo;
		for (z = zo; z <= z1; z++)
		{
			sub_ToC[count].push_back(io_texture_on_code[z]);
		}
		count++;
	}

	for (k = 0; k < num_blob; k++)
	{
		compute_cost(
			in_set_of_voxels,//CKvSet_of_Voxel *in_set_of_voxels,
			sub_ToC[k],//std::vector<int> &in_texture_on_code,
			in_sub_patch,//std::vector<int> &in_sub_patch,
			set_of_matched_patch[k],//std::vector<int> &out_matched_patch,
			set_of_location[k],//std::vector<int> &out_location_of_elements_in_matched_patch,
			set_cost[k]);//float &out_cost)
	}

	for (k = 0; k < num_blob; k++)
	{
		if (min_cost > set_cost[k])
		{
			min_cost = set_cost[k];
			idx = k;
		}
	}

	out_matched_patch                         = set_of_matched_patch[idx];
	out_location_of_elements_in_matched_patch = set_of_location[idx];

	// Update mask and io_texture_on_code using above results. 
	bool *p_mask = io_texture_on_code_mask->vp();
	zz_sz = io_texture_on_code_mask->vs();
	for (k = 0; k < io_texture_on_code.size(); k++)
	{
		for (l = 0; l < out_matched_patch.size(); l++)
		{
			if (io_texture_on_code[k] == out_matched_patch[l])
			{
				io_texture_on_code[k] = -1;
				p_mask[k] = false;
				//break;
			}
		}
	}

	for (k = 0; k < out_location_of_elements_in_matched_patch.size(); k++)
	{
		if (out_location_of_elements_in_matched_patch[k] != -1)
		{
			out_location_of_elements_in_matched_patch[k] = out_location_of_elements_in_matched_patch[k] + set_of_offset[idx];
			if (out_location_of_elements_in_matched_patch[k] < in_buffer_size)
			{
				out_location_of_elements_in_matched_patch[k] = io_texture_on_code.size()-in_buffer_size + out_location_of_elements_in_matched_patch[k];
				//for (k = 0; k < buffer_size; k++)
				//{
				//	buffer_ToC[k] = io_target_texture_on_code[num_element_target - buffer_size + k];
				//}
			}
		}
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Sequence_Aligner_Distance::compute_cost(
	CKvSet_of_Voxel *in_set_of_voxels,
	std::vector<int> &in_texture_on_code,
	std::vector<int> &in_sub_patch,
	std::vector<int> &out_matched_patch,
	std::vector<int> &out_location_of_elements_in_matched_patch,
	float &out_cost)
//************************************************************************
{
	std::vector<float> cost_vector;
	CKvVoxel *p_voxel;
	float cost;
	int patch_size, toc_sz, idx;
	int k, l, count, valid;

	p_voxel = in_set_of_voxels->vp();

	toc_sz      = in_texture_on_code.size();
	patch_size  = in_sub_patch.size();

	//valid = 0;
	//for (k = 0; k < patch_size; k++)
	//{
	//	if (in_sub_patch[k] != -1)
	//	{
	//		valid++;
	//	}
	//}
	//if (valid == 0)
	//{
	//	out_cost = FLT_MAX;
	//	return false;
	//}

	if (toc_sz >= patch_size) // buffer_texture_on_code <- in_texture_on_code
	{
		cost_vector = std::vector<float>(toc_sz - (patch_size - 1), 999999.f);

		// Compute cost.
		valid = 0;
		for (k = 0 ; k < toc_sz - (patch_size-1) ; k++)
		{
			cost = 0.f;
			count = 0;
			for (l = 0; l < patch_size; l++)
			{
				//Error Check
				if (in_texture_on_code.size() - 1 < k + l)
				{
					printf("Error 1) %d %d\n", in_texture_on_code.size(),k+l);
				}
				if (in_sub_patch.size() - 1 < l)
				{
					printf("Error 2) %d %d\n", in_sub_patch.size(),l);
				}
				/*****************************************************************/

				if ((in_texture_on_code[k + l] != -1) && (in_sub_patch[l] != -1))
				{
					cost += p_voxel[in_sub_patch[l]].d_Distance(p_voxel[in_texture_on_code[k + l]]);
					count++;
				}
			}
			if (count != 0)
			{
				cost_vector[k] = cost / count;
				valid++;
			}
		}

		if (valid == 0)
		{
			out_cost = 999999.f;
			return false;
		}

		// Set matched_patch.
		find_min_cost(cost_vector, idx);
		out_matched_patch = std::vector<int>();
		for (l = 0; l < patch_size; l++)
		{
			out_matched_patch.push_back(in_texture_on_code[idx + l]);
		}

		// Set out_location_of_elements_in_matched_patch
		out_location_of_elements_in_matched_patch = std::vector<int>();
		for (l = 0; l < patch_size; l++)
		{
			out_location_of_elements_in_matched_patch.push_back(idx + l);
		}
		out_cost = cost_vector[idx];
	}
	else // buffer_texture_on_code <- in_sub_patch // toc_sz < patch_size
	{
		toc_sz      = in_sub_patch.size();
		patch_size  = in_texture_on_code.size();
		cost_vector = std::vector<float>(toc_sz - (patch_size - 1), FLT_MAX);

		// Compute cost.
		valid = 0;
		for (k = 0; k < toc_sz - (patch_size - 1) ; k++)
		{
			cost = 0.f;
			count = 0;
			for (l = 0; l < patch_size; l++)
			{
				//Error Check
				if (in_texture_on_code.size() - 1 < l)
				{
					printf("Error 3) %d %d\n", in_texture_on_code.size(),l);
				}
				if (in_sub_patch.size() - 1 < k+l)
				{
					printf("Error 4) %d %d\n", in_sub_patch.size(),k+l);
				}
				/*****************************************************************/

				if ((in_sub_patch[k+l] != -1) && (in_texture_on_code[l] != -1))
				{
					cost += p_voxel[in_texture_on_code[l]].d_Distance(p_voxel[in_sub_patch[k + l]]);
					count++;
				}
			}
			if (count != 0)
			{
				cost_vector[k] = cost / count;
				valid++;
			}
		}

		if (valid == 0)
		{
			out_cost = FLT_MAX;
			return false;
		}

		// Set matched_patch.
		find_min_cost(cost_vector, idx); // 여기서 뽑은 idx는 patch에서의 위치. 
		out_matched_patch = std::vector<int>(toc_sz,-1);
		for (l = idx; l < idx + patch_size; l++)
		{
			//Error Check
			if (out_matched_patch.size() - 1 < l)
			{
				printf("Error 5) %d %d\n", out_matched_patch.size(), l);
			}
			if (in_texture_on_code.size() - 1 < l-idx)
			{
				printf("Error 6) %d %d\n", in_texture_on_code.size(), l-idx);
			}
			/*****************************************************************/

			out_matched_patch[l] = in_texture_on_code[l - idx];
		}

		// Set out_location_of_elements_in_matched_patch
		out_location_of_elements_in_matched_patch = std::vector<int>(toc_sz, -1);
		for (l = idx; l < idx+patch_size; l++)
		{
			//Error Check
			if (out_location_of_elements_in_matched_patch.size() - 1 < l)
			{
				printf("Error 7) %d %d\n", out_location_of_elements_in_matched_patch.size(), l);
			}
			/*****************************************************************/

			out_location_of_elements_in_matched_patch[l] = l - idx;
		}
		out_cost = cost_vector[idx];
	}


	return true;
}
//************************************************************************
float VCL_Voxel_Color_Sequence_Aligner_Distance::find_min_cost(
	std::vector<float> &in_cost_vector,
	int &out_idx)
//************************************************************************
{
	float min_val = FLT_MAX;
	int k, sz;

	sz = in_cost_vector.size();

	for (k = 0; k < sz; k++)
	{
		if (min_val > in_cost_vector[k])
		{
			min_val = in_cost_vector[k];
			out_idx = k;
		}
	}

	return min_val;
}
//************************************************************************








