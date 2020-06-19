#include "stdafx.h"
#include "VCL_Voxel_Color_Sequence_Aligner_Color.h"


//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Color::VCL_Voxel_Color_Sequence_Aligner_Color()
//************************************************************************
{

}
//************************************************************************
VCL_Voxel_Color_Sequence_Aligner_Color::~VCL_Voxel_Color_Sequence_Aligner_Color()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, m, n;
	int *p_v, *p_pcs;

	v = in_docube->gvc_Get_Voxel_Color();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);
		for (n = 1; n < num_toc; n++)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference2(
				&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			//p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference_WO_translation(
			//	&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
			//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}
	}


}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel vv; CKvSet_of_Voxel set_vv;
	CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, num, sz, m, n;
	int *p_v, *p_pcs;
	int max_length, criteria_idx;

	v = in_docube->gvc_Get_Voxel_Color();
	v.e_Export(&set_v); v.in_Initialize();

	vv = in_docube->gsp_Get_Surface_Voxels();
	vv.e_Export(&set_vv); v.in_Initialize();


	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
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
		// Align Largest texture on code. -> Using Geometry
		Rotate_Texture_on_Code_by_Centering(
			&set_vv,//CKvSet_of_Voxel *in_set_of_voxels,
			io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &io_texture_on_code,
			1);//int in_plane_mode)
		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Start
		for (n = criteria_idx + 1; n < num_toc; n++)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
		/*****************************************************************************************/

		//for (n = 1; n < num_toc; n++)
		//{
		//	p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
		//		&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
		//		io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
		//		io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
		//		p_v[n]);//int &out_offset);
		//	//p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference_WO_translation(
		//	//	&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
		//	//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
		//	//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
		//	//	p_v[n]);//int &out_offset);
		//	p_v[n] = p_v[n] + p_v[n - 1];
		//}
	}


}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2_for_video(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	std::vector<std::vector<std::vector<int>>> &in_before_segmented_texture_on_code,
	CKvSet_of_Voxel *in_before_set_of_voxels,
	CKvSet_of_RgbaF *in_before_set_of_colors,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel vv; CKvSet_of_Voxel set_vv;
	CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, num, sz, m, n;
	int *p_v, *p_pcs;
	int max_length, criteria_idx;
	int global_circular_shift;

	v = in_docube->gvc_Get_Voxel_Color();
	v.e_Export(&set_v); v.in_Initialize();

	vv = in_docube->gsp_Get_Surface_Voxels();
	vv.e_Export(&set_vv); v.in_Initialize();


	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
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
		CKvSet_of_RgbaF before_set_colors;

		if (Find_Corresponding_ToC(
			in_before_segmented_texture_on_code,//std::vector<std::vector<std::vector<int>>> &in_before_ToC,
			io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &in_current_ToC,
			in_before_set_of_voxels,//CKvSet_of_Voxel *in_before_voxels,
			in_before_set_of_colors,//CKvSet_of_RgbaF *in_before_colors,
			in_docube,//VCL_DoCube_X_Color *in_current_docube,
			&before_set_colors))//CKvSet_of_RgbaF *out_ToC_in_before)
		{
			Rotate_Texture_on_Code_by_Color(
				&set_v,//CKvSet_of_RgbaF *in_set_of_colors,
				io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &io_texture_on_code,
				&before_set_colors,//CKvSet_of_RgbaF *in_before_colors,
				global_circular_shift);//int &out_circular_shift);
			zz_global_circular_shift.push_back(global_circular_shift);
		}
		else
		{
			Rotate_Texture_on_Code_by_Centering(
				&set_vv,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][criteria_idx],//std::vector<int> &io_texture_on_code,
				1);//int in_plane_mode)
		}

		/*****************************************************************************************/

		/*****************************************************************************************/
		// Align Start
		for (n = criteria_idx + 1; n < num_toc; n++)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
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
void VCL_Voxel_Color_Sequence_Aligner_Color::acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference2(
	VCL_DoCube_X_Color *in_docube,
	int in_random_number,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_Voxel vv; CKvSet_of_Voxel set_vv;
	CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, num, sz, m, n;
	int *p_v, *p_pcs;
	int max_length, criteria_idx;

	v = in_docube->gvc_Get_Voxel_Color();
	v.e_Export(&set_v); v.in_Initialize();

	vv = in_docube->gsp_Get_Surface_Voxels();
	vv.e_Export(&set_vv); v.in_Initialize();


	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
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
		//	&set_vv,//CKvSet_of_Voxel *in_set_of_voxels,
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
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
		/*****************************************************************************************/

		//for (n = 1; n < num_toc; n++)
		//{
		//	p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
		//		&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
		//		io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
		//		io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
		//		p_v[n]);//int &out_offset);
		//	//p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference_WO_translation(
		//	//	&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
		//	//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
		//	//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
		//	//	p_v[n]);//int &out_offset);
		//	p_v[n] = p_v[n] + p_v[n - 1];
		//}
	}


}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::acc_Align_Texture_on_Codes_Using_Voxel_Color_Difference_YCbCr(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	CKvVectorInt *p_vec;
	int num_blob, num_toc, m, n, dum;
	int *p_v;

	v = in_docube->gvc_Get_Voxel_Color();
	v.e_Export(&set_v); v.in_Initialize();
	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		p_v = p_vec[m].c_Create(num_toc, 0);

		for (n = 1; n < num_toc; n++)
		{
			dum = atd_Align_Two_Texture_on_Codes_Using_Color_Difference_YCbCr(
				&set_v,//CKvSet_of_RgbaF *in_set_of_voxel_colors,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			//dum = atc_Align_Two_Texture_on_Codes_Using_Color_Correlation_Fast(
			//	&set_v,//CKvSet_of_RgbaF *in_set_of_voxels,
			//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}
	}



}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::acc_Align_Texture_on_Codes_Using_Voxel_Color_Correlation(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	CKvVectorInt *p_vec;
	int num_blob, num_toc, m, n, dum;
	int *p_v;

	v = in_docube->gvc_Get_Voxel_Color();
	v.e_Export(&set_v); v.in_Initialize();
	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);

	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		p_v = p_vec[m].c_Create(num_toc, 0);

		for (n = 1; n < num_toc; n++)
		{
			//dum = atc_Align_Two_Texture_on_Codes_Using_Color_Correlation(
			//	&set_v,//CKvSet_of_RgbaF *in_set_of_voxels,
			//	io_segmented_texture_on_code[m][n-1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	p_v[n]);//int &out_offset);

			dum = atc_Align_Two_Texture_on_Codes_Using_Color_Correlation_Fast(
				&set_v,//CKvSet_of_RgbaF *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			//atc_Align_Two_Texture_on_Codes_Using_Color_Correlation(
			//	&set_v,//CKvSet_of_RgbaF *in_set_of_voxels,
			//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	dum,//int &out_circular_shift,
			//	p_v[n]);//int &out_shift)

			//atc_Align_Two_Texture_on_Codes_Using_Color_Correlation_Fast(
			//	&set_v,//CKvSet_of_RgbaF *in_set_of_voxels,
			//	io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
			//	io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
			//	dum,//int &out_circular_shift,
			//	p_v[n]);//int &out_shift)

			p_v[n] = p_v[n] + p_v[n - 1];
		}
	}


}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::acc_Center_Align_Texture_on_Codes_Using_Voxel_Color_Difference(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &io_segmented_texture_on_code,
	CKvSet_of_VectorInt *out_circular_shift,
	CKvSet_of_VectorInt *out_offset)
//************************************************************************
{
	CKvDepot_of_RgbaF v; CKvSet_of_RgbaF set_v;
	CKvVectorInt *p_vec, *p_cs;
	int num_blob, num_toc, m, n;
	int *p_v, *p_pcs;
	int max_length, criteria_idx;

	v = in_docube->gvc_Get_Voxel_Color();
	v.e_Export(&set_v); v.in_Initialize();

	num_blob = io_segmented_texture_on_code.size();
	p_vec = out_offset->c_Create(num_blob);
	p_cs = out_circular_shift->c_Create(num_blob);
	for (m = 0; m < num_blob; m++)
	{
		num_toc = io_segmented_texture_on_code[m].size();
		p_v = p_vec[m].c_Create(num_toc, 0);
		p_pcs = p_cs[m].c_Create(num_toc, 0);

		// Find Largest texture on code
		max_length = -1;
		for (n = 0; n < num_toc; n++)
		{
			//printf("n=%d/%d) size = %d\n", n, num_toc, io_segmented_texture_on_code[m][n].size());
			if (max_length < (int)(io_segmented_texture_on_code[m][n].size()))
			{
				max_length = (int)(io_segmented_texture_on_code[m][n].size());
				criteria_idx = n;
			}
		}

		for (n = criteria_idx + 1; n < num_toc; n++)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference_Centering(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n - 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n - 1];
		}

		for (n = criteria_idx - 1; n > -1; n--)
		{
			p_pcs[n] = atd_Align_Two_Texture_on_Codes_Using_Color_Difference_Centering(
				&set_v,//CKvSet_of_Voxel *in_set_of_voxels,
				io_segmented_texture_on_code[m][n + 1],//std::vector<int> &in_reference_texture_on_code,
				io_segmented_texture_on_code[m][n],//std::vector<int> &io_target_texture_on_code,
				p_v[n]);//int &out_offset);

			p_v[n] = p_v[n] + p_v[n + 1];
		}
	}
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::atd_Align_Two_Texture_on_Codes_Using_Color_Difference_WO_translation(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	CKvVector cost_val;
	CKvRgbaF *p_in_color;
	std::vector<int> tmp;
	double min_cost; float error;
	int sz1, sz2, search_length, circular_shift_idx, l, k;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }

	p_in_color = in_set_of_voxel_colors->vp();

	min_cost = 999999999999999999.9999999;
	for (l = 0; l < sz2; l++)
	{
		tmp = io_target_texture_on_code;

		sv_Circular_Shift_Vector(
			l,//int in_location,
			tmp);//CKvVectorUcharYCbCr *io_vector)

		error = 0.f;
#pragma omp parallel
		{
#pragma omp for reduction(+:error)
			for (k = 0; k < search_length; k++)
			{

				error += abs(p_in_color[in_reference_texture_on_code[k]].r - p_in_color[tmp[k]].r);
				error += abs(p_in_color[in_reference_texture_on_code[k]].g - p_in_color[tmp[k]].g);
				error += abs(p_in_color[in_reference_texture_on_code[k]].b - p_in_color[tmp[k]].b);
			}
		}

		if (min_cost > error)
		{
			min_cost = error;
			circular_shift_idx = l;
		}
	}

	sv_Circular_Shift_Vector(
		circular_shift_idx,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)
	out_offset = 0;

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::atd_Align_Two_Texture_on_Codes_Using_Color_Difference(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	CKvVector cost_val;
	CKvRgbaF *p_in_color;
	std::vector<int> tmp;
	double v_cr, *p_cost_val;
	double min_cost; float error;
	unsigned char *p_r2, *p_g2;
	int sz1, sz2, search_length, circular_shift_idx, l, k, j;
	int length, num_points;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }
	length = abs(sz2 - sz1) + 1;

	p_in_color = in_set_of_voxel_colors->vp();

	min_cost = 999999999999999999.9999999;

	if (sz1 < sz2)
	{
		for (j = 0; j < length; j++)
		{
			for (l = 0; l < sz2; l++)
			{
				tmp = io_target_texture_on_code;
				sv_Circular_Shift_Vector(l, tmp);
				error = 0.0;
				num_points = 0;
#pragma omp parallel
				{
#pragma omp for reduction(+:error) 
					for (k = 0; k < search_length; k++)
					{
						//error += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[tmp[k + j]]);
						error += abs(p_in_color[in_reference_texture_on_code[k]].r - p_in_color[tmp[k + j]].r);
						error += abs(p_in_color[in_reference_texture_on_code[k]].g - p_in_color[tmp[k + j]].g);
						error += abs(p_in_color[in_reference_texture_on_code[k]].b - p_in_color[tmp[k + j]].b);
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
				tmp = io_target_texture_on_code;
				sv_Circular_Shift_Vector(l, tmp);
				error = 0.0;
#pragma omp parallel
				{
#pragma omp for reduction(+:error)
					for (k = 0; k < search_length; k++)
					{
						//error += (double)p_v[in_reference_texture_on_code[k + j]].ds_Distance_Squared(p_v[tmp[k]]);
						error += abs(p_in_color[in_reference_texture_on_code[k + j]].r - p_in_color[tmp[k]].r);
						error += abs(p_in_color[in_reference_texture_on_code[k + j]].g - p_in_color[tmp[k]].g);
						error += abs(p_in_color[in_reference_texture_on_code[k + j]].b - p_in_color[tmp[k]].b);
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

	sv_Circular_Shift_Vector(
		circular_shift_idx,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)
	//out_offset = 0;

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::atd_Align_Two_Texture_on_Codes_Using_Color_Difference2(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	CKvVector cost_val;
	CKvRgbaF *p_in_color;
	std::vector<int> tmp;
	double v_cr, *p_cost_val;
	double min_cost; float error;
	unsigned char *p_r2, *p_g2;
	int sz1, sz2, search_length, circular_shift_idx, l, k, j;
	int length, num_points;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }
	length = abs(sz2 - sz1) + 1;

	p_in_color = in_set_of_voxel_colors->vp();

	min_cost = 999999999999999999.9999999;

	if (sz1 < sz2)
	{
		for (j = 0; j < length; j++)
		{
			for (l = 0; l < sz2; l++)
			{
				tmp = io_target_texture_on_code;
				sv_Circular_Shift_Vector(l, tmp);
				error = 0.0;
				num_points = 0;
#pragma omp parallel
				{
#pragma omp for reduction(+:error) reduction(+:num_points)
					for (k = 0; k < search_length; k++)
					{
						//error += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[tmp[k + j]]);
						error += abs(p_in_color[in_reference_texture_on_code[k]].r - p_in_color[tmp[k + j]].r);
						error += abs(p_in_color[in_reference_texture_on_code[k]].g - p_in_color[tmp[k + j]].g);
						error += abs(p_in_color[in_reference_texture_on_code[k]].b - p_in_color[tmp[k + j]].b);
						num_points += 3;
					}
				}

				if (num_points != 0)
				{
					error /= (double)num_points;
					if (min_cost > error)
					{
						min_cost = error;
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
				error = 0.0;
				num_points = 0;
#pragma omp parallel
				{
#pragma omp for reduction(+:error) reduction(+:num_points)
					for (k = 0; k < search_length; k++)
					{
						//error += (double)p_v[in_reference_texture_on_code[k + j]].ds_Distance_Squared(p_v[tmp[k]]);
						error += abs(p_in_color[in_reference_texture_on_code[k + j]].r - p_in_color[tmp[k]].r);
						error += abs(p_in_color[in_reference_texture_on_code[k + j]].g - p_in_color[tmp[k]].g);
						error += abs(p_in_color[in_reference_texture_on_code[k + j]].b - p_in_color[tmp[k]].b);
						num_points += 3;
					}
				}

				if (num_points != 0)
				{
					error /= (double)num_points;
					if (min_cost > error)
					{
						min_cost = error;
						circular_shift_idx = l;
						out_offset = j;
					}
				}
			}
		}
	}

	sv_Circular_Shift_Vector(
		circular_shift_idx,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::atd_Align_Two_Texture_on_Codes_Using_Color_Difference_Centering(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	CKvVector cost_val;
	CKvRgbaF *p_in_color;
	std::vector<int> tmp;
	double v_cr, *p_cost_val;
	double min_cost; float error;
	unsigned char *p_r2, *p_g2;
	int sz1, sz2, search_length, circular_shift_idx, l, k, j;
	int length;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }

	length = abs(sz2 - sz1) + 1;
	j = (int)((length) / 2.0);

	p_in_color = in_set_of_voxel_colors->vp();
	min_cost = 999999999999999999.9999999;

	if (sz1 < sz2)
	{
		for (l = 0; l < sz2; l++)
		{
			tmp = io_target_texture_on_code;
			sv_Circular_Shift_Vector(l, tmp);
			error = 0.0;
			for (k = 0; k < search_length; k++)
			{
				//error += (double)p_v[in_reference_texture_on_code[k]].ds_Distance_Squared(p_v[tmp[k + j]]);

				error += abs(p_in_color[in_reference_texture_on_code[k]].r - p_in_color[tmp[k + j]].r);
				error += abs(p_in_color[in_reference_texture_on_code[k]].g - p_in_color[tmp[k + j]].g);
				error += abs(p_in_color[in_reference_texture_on_code[k]].b - p_in_color[tmp[k + j]].b);
			}

			if (min_cost > error)
			{
				min_cost = error;
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
			error = 0.0;
			for (k = 0; k < search_length; k++)
			{
				//error += (double)p_v[in_reference_texture_on_code[k + j]].ds_Distance_Squared(p_v[tmp[k]]);
				error += abs(p_in_color[in_reference_texture_on_code[k + j]].r - p_in_color[tmp[k]].r);
				error += abs(p_in_color[in_reference_texture_on_code[k + j]].g - p_in_color[tmp[k]].g);
				error += abs(p_in_color[in_reference_texture_on_code[k + j]].b - p_in_color[tmp[k]].b);
			}

			if (min_cost > error)
			{
				min_cost = error;
				circular_shift_idx = l;
				out_offset = j;
			}
		}
	}

	sv_Circular_Shift_Vector(
		circular_shift_idx,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::atd_Align_Two_Texture_on_Codes_Using_Color_Difference_YCbCr(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	CKvVector cost_val;
	CKvVectorUcharYCbCr y1, y2; CKvVectorUcharYCbCr tmp;
	//std::vector<int> tmp;
	double v_y, v_cb, v_cr, *p_cost_val;
	double min_cost;
	unsigned char *p_r1, *p_g1, *p_b1, *p_r2, *p_g2, *p_b2;
	int sz1, sz2, search_length, circular_shift_idx, l;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }

	ccs_Convert_Color_Space(
		in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
		in_reference_texture_on_code,//CKvVectorInt *in_chain_code,
		&y1);//CKvVectorUcharYCbCr *out_color)

	ccs_Convert_Color_Space(
		in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
		io_target_texture_on_code,//std::vector<int> &in_chain_code,
		&y2);//CKvVectorUcharYCbCr *out_color)

	p_r1 = y1.vp(p_g1, p_b1);
	p_cost_val = cost_val.c_Create(sz2, 0.0);
	min_cost = 999999999999999999.9999999;
	for (l = 0; l < sz2; l++)
	{
		tmp.cp_Copy(&y2);

		sv_Circular_Shift_Vector(
			l,//int in_location,
			&tmp);//CKvVectorUcharYCbCr *io_vector)

		p_r2 = tmp.vp(p_g2, p_b2);

		v_y = sacd_Summation_of_Absolute_Color_Differece(
			search_length,//int in_dimension, 
			p_r1,//unsigned char *in_vector_a, 
			p_r2);//unsigned char *in_vector_b);

		v_cb = sacd_Summation_of_Absolute_Color_Differece(
			search_length,//int in_dimension, 
			p_g1,//unsigned char *in_vector_a, 
			p_g2);//unsigned char *in_vector_b);

		v_cr = sacd_Summation_of_Absolute_Color_Differece(
			search_length,//int in_dimension, 
			p_b1,//unsigned char *in_vector_a, 
			p_b2);//unsigned char *in_vector_b);

		//p_cost_val[l] = ((v_y + v_cb + v_cr) / 3.0);
		p_cost_val[l] = v_y + v_cb + v_cr;
		if (min_cost > p_cost_val[l])
		{
			min_cost = p_cost_val[l];
			circular_shift_idx = l;
		}
	}

	sv_Circular_Shift_Vector(
		circular_shift_idx,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)
	out_offset = 0;

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::atc_Align_Two_Texture_on_Codes_Using_Color_Correlation(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	LCKvAlgebra_Basic lab;
	CKvVector cost_val;
	CKvVectorUcharYCbCr y1, y2;
	std::vector<int> tmp;
	double v_y, v_cb, v_cr, *p_cost_val;
	double max_val;
	unsigned char *p_r1, *p_g1, *p_b1, *p_r2, *p_g2, *p_b2;
	int sz1, sz2, search_length, circular_shift_idx, l;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }

	ccs_Convert_Color_Space(
		in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
		in_reference_texture_on_code,//CKvVectorInt *in_chain_code,
		&y1);//CKvVectorUcharYCbCr *out_color)

	p_r1 = y1.vp(p_g1, p_b1);
	p_cost_val = cost_val.c_Create(sz2, 0.0);
	max_val = -100.0;
	for (l = 0; l < sz2; l++)
	{
		tmp = io_target_texture_on_code;
		sv_Circular_Shift_Vector(l, tmp);

		ccs_Convert_Color_Space(
			in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
			tmp,//std::vector<int> &in_chain_code,
			&y2);//CKvVectorUcharYCbCr *out_color)
		p_r2 = y2.vp(p_g2, p_b2);

		v_y = lab.ncc_Normalized_Cross_Correlation(
			search_length,//int in_dimension, 
			p_r1,//unsigned char *in_vector_a, 
			p_r2,//unsigned char *in_vector_b, 
			1);//int in_mode_of_computing);

		v_cb = lab.ncc_Normalized_Cross_Correlation(
			search_length,//int in_dimension, 
			p_g1,//unsigned char *in_vector_a, 
			p_g2,//unsigned char *in_vector_b, 
			1);//int in_mode_of_computing);

		v_cr = lab.ncc_Normalized_Cross_Correlation(
			search_length,//int in_dimension, 
			p_b1,//unsigned char *in_vector_a, 
			p_b2,//unsigned char *in_vector_b, 
			1);//int in_mode_of_computing);

		p_cost_val[l] = ((v_y + v_cb + v_cr) / 3.0);
		if (max_val < p_cost_val[l])
		{
			max_val = p_cost_val[l];
			circular_shift_idx = l;
		}
	}

	sv_Circular_Shift_Vector(
		circular_shift_idx,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)
	out_offset = 0;

	return circular_shift_idx;
}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::atc_Align_Two_Texture_on_Codes_Using_Color_Correlation_Fast(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_offset)
//************************************************************************
{
	LCKvAlgebra_Basic lab;
	CKvVector cost_val;
	CKvVectorUcharYCbCr y1, y2;
	std::vector<int> tmp;
	double v_y, v_cb, v_cr, *p_cost_val;
	double max_val;
	unsigned char *p_r1, *p_g1, *p_b1, *p_r2, *p_g2, *p_b2;
	int sz1, sz2, search_length, circular_shift_idx, l;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	if (sz1 > sz2) { search_length = sz2; }
	else { search_length = sz1; }

	ccs_Convert_Color_Space(
		in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
		in_reference_texture_on_code,//CKvVectorInt *in_chain_code,
		&y1);//CKvVectorUcharYCbCr *out_color)

	p_r1 = y1.vp(p_g1, p_b1);
	p_cost_val = cost_val.c_Create(sz2, 0.0);
	max_val = -100.0;
#pragma omp parallel
	{
//#pragma omp for firstprivate(sz2), private(p_r2), private(p_g2), private(p_b2), private(tmp), private(y2)
#pragma omp for private(p_r2), private(p_g2), private(p_b2), private(tmp), private(y2)
		for (l = 0; l < sz2; l++)
		{
			tmp = io_target_texture_on_code;
			sv_Circular_Shift_Vector(l, tmp);

			ccs_Convert_Color_Space(
				in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
				tmp,//std::vector<int> &in_chain_code,
				&y2);//CKvVectorUcharYCbCr *out_color)
			p_r2 = y2.vp(p_g2, p_b2);

			v_y = lab.ncc_Normalized_Cross_Correlation(
				search_length,//int in_dimension, 
				p_r1,//unsigned char *in_vector_a, 
				p_r2,//unsigned char *in_vector_b, 
				1);//int in_mode_of_computing);

			v_cb = lab.ncc_Normalized_Cross_Correlation(
				search_length,//int in_dimension, 
				p_g1,//unsigned char *in_vector_a, 
				p_g2,//unsigned char *in_vector_b, 
				1);//int in_mode_of_computing);

			v_cr = lab.ncc_Normalized_Cross_Correlation(
				search_length,//int in_dimension, 
				p_b1,//unsigned char *in_vector_a, 
				p_b2,//unsigned char *in_vector_b, 
				1);//int in_mode_of_computing);

			p_cost_val[l] = ((v_y + v_cb + v_cr) / 3.0);
		}
	}

	for (l = 0; l < sz2; l++)
	{
		if (max_val < p_cost_val[l])
		{
			max_val = p_cost_val[l];
			circular_shift_idx = l;
		}
	}

	sv_Circular_Shift_Vector(
		circular_shift_idx,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)
	out_offset = 0;

	return circular_shift_idx;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::atc_Align_Two_Texture_on_Codes_Using_Color_Correlation(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_circular_shift,
	int &out_shift)
//************************************************************************
{
	CKvMatrix all_cross_cor;
	CKvVector cost_val, cross_cor;
	CKvVectorUcharYCbCr y1, y2;
	std::vector<int> tmp;
	int sz1, sz2, sz, l;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	ccs_Convert_Color_Space(
		in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
		in_reference_texture_on_code,//CKvVectorInt *in_chain_code,
		&y1);//CKvVectorUcharYCbCr *out_color)

	sz = sz1 + sz2 - 1;
	all_cross_cor.c_Create(sz2, sz, -1.0);

	for (l = 0; l < sz2; l++)
	{
		tmp = io_target_texture_on_code;
		sv_Circular_Shift_Vector(l, tmp);

		ccs_Convert_Color_Space(
			in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
			tmp,//CKvVectorInt *in_chain_code,
			&y2);//CKvVectorUcharYCbCr *out_color)

		ncc_Normalized_Cross_Correlation(
			&y1,//CKvVectorUcharYCbCr *in_reference_signal,
			&y2,//CKvVectorUcharYCbCr *in_target_signal,
			&cross_cor);//CKvVector *out_correlation_result)

		all_cross_cor.sr_Set_Row(0, l, &cross_cor);
	}

	all_cross_cor.max_Maximum(&out_shift, &out_circular_shift);
	out_shift = out_shift - (sz2 - 1);
	sv_Circular_Shift_Vector(
		out_circular_shift,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::atc_Align_Two_Texture_on_Codes_Using_Color_Correlation_Fast(
	CKvSet_of_RgbaF *in_set_of_voxel_colors,
	std::vector<int> &in_reference_texture_on_code,
	std::vector<int> &io_target_texture_on_code,
	int &out_circular_shift,
	int &out_shift)
//************************************************************************
{
	CKvMatrix all_cross_cor;
	CKvVector cost_val, cross_cor;
	CKvVectorUcharYCbCr y1, y2;
	std::vector<int> tmp;
	int sz1, sz2, sz, l;

	sz1 = in_reference_texture_on_code.size();
	sz2 = io_target_texture_on_code.size();

	ccs_Convert_Color_Space(
		in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
		in_reference_texture_on_code,//CKvVectorInt *in_chain_code,
		&y1);//CKvVectorUcharYCbCr *out_color)

	sz = sz1 + sz2 - 1;
	all_cross_cor.c_Create(sz2, sz, -1.0);

	for (l = 0; l < sz2; l++)
	{
		tmp = io_target_texture_on_code;
		sv_Circular_Shift_Vector(l, tmp);

		ccs_Convert_Color_Space(
			in_set_of_voxel_colors,//CKvSet_of_RgbaF *in_color,
			tmp,//CKvVectorInt *in_chain_code,
			&y2);//CKvVectorUcharYCbCr *out_color)

		ncc_Normalized_Cross_Correlation_Fast(
			&y1,//CKvVectorUcharYCbCr *in_reference_signal,
			&y2,//CKvVectorUcharYCbCr *in_target_signal,
			&cross_cor);//CKvVector *out_correlation_result)

		all_cross_cor.sr_Set_Row(0, l, &cross_cor);
	}

	all_cross_cor.max_Maximum(&out_shift, &out_circular_shift);
	out_shift = out_shift - (sz2 - 1);
	sv_Circular_Shift_Vector(
		out_circular_shift,//int in_location,
		io_target_texture_on_code);//CKvVectorInt *io_point_indicies)
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::ccs_Convert_Color_Space(
	CKvSet_of_RgbaF *in_color,
	std::vector<int> &in_chain_code,
	CKvVectorUcharYCbCr *out_color)
//************************************************************************
{
	LCKvUtility_for_YCbCr lu_ycbcr;
	CKvVectorUcharRgb v_rgb;
	CKvRgbaF *p;
	unsigned char *pr, *pg, *pb;
	int num, k;

	p = in_color->vp();
	num = in_chain_code.size();
	pr = v_rgb.c_Create(num, pg, pb);
	for (k = 0; k < num; k++)
	{
		pr[k] = (unsigned char)(255.f*p[in_chain_code[k]].r);
		pg[k] = (unsigned char)(255.f*p[in_chain_code[k]].g);
		pb[k] = (unsigned char)(255.f*p[in_chain_code[k]].b);
	}

	lu_ycbcr.vry_Vector_Rgb_to_YCbCr(
		&v_rgb,//CKvVectorUcharRgb *in_source,
		false,//bool in_itu_601_mode,
		out_color);//CKvVectorUcharYCbCr *out_destination);

}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::ccs_Convert_Color_Space(
	CKvSet_of_RgbaF *in_color,
	std::vector<int> &in_chain_code,
	int in_max_length,
	CKvVectorUcharYCbCr *out_color)
//************************************************************************
{
	LCKvUtility_for_YCbCr lu_ycbcr;
	CKvVectorUcharRgb v_rgb;
	CKvRgbaF *p; CKvRgb rgb;
	unsigned char *pr, *pg, *pb;
	int num, k;

	rgb.r = rgb.g = rgb.b = 0;
	p = in_color->vp();
	num = in_chain_code.size();
	v_rgb.c_Create(in_max_length, rgb);
	pr = v_rgb.vp(pg, pb);

	for (k = 0; k < num; k++)
	{
		pr[k] = (unsigned char)(255.f*p[in_chain_code[k]].r);
		pg[k] = (unsigned char)(255.f*p[in_chain_code[k]].g);
		pb[k] = (unsigned char)(255.f*p[in_chain_code[k]].b);
	}

	lu_ycbcr.vry_Vector_Rgb_to_YCbCr(
		&v_rgb,//CKvVectorUcharRgb *in_source,
		false,//bool in_itu_601_mode,
		out_color);//CKvVectorUcharYCbCr *out_destination);

}
//************************************************************************
int VCL_Voxel_Color_Sequence_Aligner_Color::sacd_Summation_of_Absolute_Color_Differece(
	int in_dimension,
	unsigned char *in_vector_a,
	unsigned char *in_vector_b)
//************************************************************************
{
	int error = 0;
#pragma omp parallel
	{
#pragma omp for reduction(+:error)
		for (int k = 0; k < in_dimension; k++)
		{
			error += abs((int)in_vector_a[k] - (int)in_vector_b[k]);
		}
	}

	return error;
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::ncc_Normalized_Cross_Correlation(
	CKvVectorUcharYCbCr *in_reference_signal,
	CKvVectorUcharYCbCr *in_target_signal,
	CKvVector *out_correlation_result)
//************************************************************************
{
	LCKvAlgebra_Basic lab;
	CKvVectorUchar Y1, Y2, Cb1, Cb2, Cr1, Cr2;
	double ncc_Y, ncc_Cb, ncc_Cr;
	double *p_correlation;
	unsigned char *p_r1, *p_g1, *p_b1, *p_r2, *p_g2, *p_b2;
	unsigned char *p_sr, *p_sg, *p_sb;
	int sz1, sz2, sz, length;
	int k;

	p_r1 = in_reference_signal->vps(p_g1, p_b1, sz1);
	p_r2 = in_target_signal->vps(p_g2, p_b2, sz2);

	length = sz1 + ((sz2 - 1) * 2);

	Y1.c_Create(length, (unsigned char)0);  Y1.s_Set(sz2 - 1, sz1, p_r1);
	Cb1.c_Create(length, (unsigned char)0); Cb1.s_Set(sz2 - 1, sz1, p_g1);
	Cr1.c_Create(length, (unsigned char)0); Cr1.s_Set(sz2 - 1, sz1, p_b1);

	Y2.c_Create(length);
	Cb2.c_Create(length);
	Cr2.c_Create(length);

	sz = sz1 + sz2 - 1;
	p_correlation = out_correlation_result->c_Create(sz, 0.0);

	p_r1 = Y1.vp();
	p_g1 = Cb1.vp();
	p_b1 = Cr1.vp();

	for (k = 0; k < sz; k++)
	{
		Y2.sv_Set_Vector((unsigned char)0);
		Cb2.sv_Set_Vector((unsigned char)0);
		Cr2.sv_Set_Vector((unsigned char)0);

		Y2.s_Set(k, sz2, p_r2);
		Cb2.s_Set(k, sz2, p_g2);
		Cr2.s_Set(k, sz2, p_b2);

		p_sr = Y2.vp();
		p_sg = Cb2.vp();
		p_sb = Cr2.vp();

		ncc_Y = lab.ncc_Normalized_Cross_Correlation(
			length,//int in_dimension, 
			p_r1,//unsigned char *in_vector_a, 
			p_sr,//unsigned char *in_vector_b, 
			1);//int in_mode_of_computing);

		ncc_Cb = lab.ncc_Normalized_Cross_Correlation(
			length,//int in_dimension, 
			p_g1,//unsigned char *in_vector_a, 
			p_sg,//unsigned char *in_vector_b, 
			1);//int in_mode_of_computing);

		ncc_Cr = lab.ncc_Normalized_Cross_Correlation(
			length,//int in_dimension, 
			p_b1,//unsigned char *in_vector_a, 
			p_sb,//unsigned char *in_vector_b, 
			1);//int in_mode_of_computing);

		p_correlation[k] = (ncc_Y + ncc_Cb + ncc_Cr) / 3.0;
		//p_correlation[k] = ncc_Y;
	}
}
//************************************************************************
void VCL_Voxel_Color_Sequence_Aligner_Color::ncc_Normalized_Cross_Correlation_Fast(
	CKvVectorUcharYCbCr *in_reference_signal,
	CKvVectorUcharYCbCr *in_target_signal,
	CKvVector *out_correlation_result)
//************************************************************************
{
	LCKvAlgebra_Basic lab;
	CKvVectorUchar Y1, Cb1, Cr1;
	double *p_correlation;
	unsigned char *p_r1, *p_g1, *p_b1, *p_r2, *p_g2, *p_b2;
	int sz1, sz2, sz, length;
	int k;

	p_r1 = in_reference_signal->vps(p_g1, p_b1, sz1);
	p_r2 = in_target_signal->vps(p_g2, p_b2, sz2);

	length = sz1 + ((sz2 - 1) * 2);

	Y1.c_Create(length, (unsigned char)0);  Y1.s_Set(sz2 - 1, sz1, p_r1);
	Cb1.c_Create(length, (unsigned char)0); Cb1.s_Set(sz2 - 1, sz1, p_g1);
	Cr1.c_Create(length, (unsigned char)0); Cr1.s_Set(sz2 - 1, sz1, p_b1);

	sz = sz1 + sz2 - 1;
	p_correlation = out_correlation_result->c_Create(sz, 0.0);

	p_r1 = Y1.vp();
	p_g1 = Cb1.vp();
	p_b1 = Cr1.vp();

#pragma omp parallel
	{
#pragma omp for firstprivate(length), firstprivate(sz2)
		for (k = 0; k < sz; k++)
		{
			p_correlation[k] = ncc_Normalized_Cross_Correlation(
				length,//int &in_dimension,
				sz2,//int &in_target_size,
				k,//int &in_k,
				p_r1,//unsigned char *in_y_reference,
				p_g1,//unsigned char *in_cb_reference,
				p_b1,//unsigned char *in_cr_reference,
				p_r2,//unsigned char *in_y_target,
				p_g2,//unsigned char *in_cb_target,
				p_b2);//unsigned char *in_cr_target)
		}
	}

}
//************************************************************************
double VCL_Voxel_Color_Sequence_Aligner_Color::ncc_Normalized_Cross_Correlation(
	int in_dimension,
	int in_target_size,
	int in_k,
	unsigned char *in_y_reference,
	unsigned char *in_cb_reference,
	unsigned char *in_cr_reference,
	unsigned char *in_y_target,
	unsigned char *in_cb_target,
	unsigned char *in_cr_target)
//************************************************************************
{
	LCKvAlgebra_Basic lab;
	CKvVectorUchar Y2, Cb2, Cr2;
	double ncc_Y, ncc_Cb, ncc_Cr;
	unsigned char *p_y_t, *p_cb_t, *p_cr_t;
	unsigned char *a, *b, *c;

	a = in_y_target;
	b = in_cb_target;
	c = in_cr_target;

	Y2.c_Create(in_dimension, (unsigned char)0);  Y2.s_Set(in_k, in_target_size, a);
	Cb2.c_Create(in_dimension, (unsigned char)0); Cb2.s_Set(in_k, in_target_size, b);
	Cr2.c_Create(in_dimension, (unsigned char)0); Cr2.s_Set(in_k, in_target_size, c);

	p_y_t = Y2.vp();
	p_cb_t = Cb2.vp();
	p_cr_t = Cr2.vp();

	ncc_Y = lab.ncc_Normalized_Cross_Correlation(
		in_dimension,//int in_dimension, 
		in_y_reference,//unsigned char *in_vector_a, 
		p_y_t,//unsigned char *in_vector_b, 
		1);//int in_mode_of_computing);

	ncc_Cb = lab.ncc_Normalized_Cross_Correlation(
		in_dimension,//int in_dimension, 
		in_cb_reference,//unsigned char *in_vector_a, 
		p_cb_t,//unsigned char *in_vector_b, 
		1);//int in_mode_of_computing);

	ncc_Cr = lab.ncc_Normalized_Cross_Correlation(
		in_dimension,//int in_dimension, 
		in_cr_reference,//unsigned char *in_vector_a, 
		p_cr_t,//unsigned char *in_vector_b, 
		1);//int in_mode_of_computing);

	return (ncc_Y + ncc_Cb + ncc_Cr) / 3.0;
}
//************************************************************************




