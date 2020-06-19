#include "stdafx.h"
#include "VCL_Voxel_Scanner_Slice.h"
#include <algorithm>

//************************************************************************
VCL_Voxel_Scanner_Slice::VCL_Voxel_Scanner_Slice()
//************************************************************************
{

}
//************************************************************************
VCL_Voxel_Scanner_Slice::~VCL_Voxel_Scanner_Slice()
//************************************************************************
{

}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction(
	VCL_DoCube_X_Color *in_docube,
	CKvXrunset3dShort *io_xrunset3d,
	int in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence)
//************************************************************************
{
	LCKvUtility_for_Xrunset3d luxrunset3d;
	CKvXrunset3dShort tmp_xrunset3d;
	CKvSdkimRunCode3D sdkim_runcode3d;
	CKvXvectorInt list_of_object_labels_for_FIRST_slice, list_of_object_labels_for_SECOND_slice, EQUIVALENCE_list;
	CKvXrunsetShort null_rs;
	CKvXrunset3dShort cpy_runsets;
	CKvSdkimChainCode sdkim_chain_code;
	CKvSdkimRunCode sdkim_run_code, previous_run_code, null_rc, an_obj_run_code;
	CKvXrunsetShort xrun_set, an_object;
	CKvXvectorShort b_pt; CKvMatrixBool masks;
	CKvXvectorInt chain_code_voxel_idx;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	CKvVolumeBool cpy_volume;
	CKvStopWatch sw, sw1, sw2;
	double time1, time2;
	long long voxel_idx;
	int k, m, l, num_obj, nb_obj_tot;
	int count, element_count, new_label_for_SECOND_slice;
	short *p_b_pt, num_pt;
	bool ***p_mask, mode = true;

	if (io_xrunset3d->net_Number_of_Elements_Total() == 0)
	{
		return false;
	}

	/****************************************************************************************/
	out_voxel_sequence = std::vector<Voxel_Slice_Scanned_Data>(in_ww);
	/****************************************************************************************/
	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);

	/***************Update Scanned Voxels******************/
	//io_xrunset3d->ex_Export(true, false, cpy_volume);
	cpy_volume.c_Create(in_dd, in_hh, in_ww, false);
	p_mask = cpy_volume.tp();
	/******************************************************/

	count = 0;
	sw.c_Create(1); sw1.c_Create(1); sw2.c_Create(2);
	sw.r_Reset(0);
	time1 = time2 = 0.0;
	for (k = 0; k < in_ww; k++)
	{
		sw1.r_Reset(0);
		gfv_Get_Slice_Voxels(
			io_xrunset3d,//CKvXrunset3dShort *in_runsets,
			in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
			k,//int in_coordinate_at_the_corresponding_axis,
			&xrun_set);//CKvXrunsetShort *out_fragment_voxel);

		//num_obj = (int)xrun_set.npx_Number_of_Pixels();
		sdkim_run_code.im_Import(xrun_set, true);
		num_obj = sdkim_run_code.no_Number_of_Objects();

		if (num_obj == 0) { continue; }

		if (count != 0)
		{
			gfv_Get_Slice_Voxels(
				io_xrunset3d,//CKvXrunset3dShort *in_runsets,
				in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
				k - 1,//int in_coordinate_at_the_corresponding_axis,
				&xrun_set);//CKvXrunsetShort *out_fragment_voxel);
			previous_run_code.im_Import(xrun_set, true);

			nb_obj_tot = previous_run_code.no_Number_of_Objects() + sdkim_run_code.no_Number_of_Objects();
			list_of_object_labels_for_FIRST_slice.in_Initialize(1);
			EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
			//EQUIVALENCE_list.in_Initialize(std::max(1, nb_obj_tot));
			new_label_for_SECOND_slice = 0;

			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				previous_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				previous_run_code,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				sdkim_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			out_voxel_sequence[count].blob_index_for_before_slice    = std::vector<int>(EQUIVALENCE_list.ne());
			out_voxel_sequence[count].list_of_index_for_connectivity = std::vector<int>(list_of_object_labels_for_SECOND_slice.ne());

			for (m = 0; m < EQUIVALENCE_list.ne(); m++)
			{
				out_voxel_sequence[count].blob_index_for_before_slice[m] = EQUIVALENCE_list.ge_Get_Element(m);
			}

			for (m = 0; m < list_of_object_labels_for_SECOND_slice.ne(); m++)
			{
				out_voxel_sequence[count].list_of_index_for_connectivity[m] = list_of_object_labels_for_SECOND_slice.ge_Get_Element(m);
			}
		}
		else
		{
			out_voxel_sequence[count].blob_index_for_before_slice.push_back(-1);
			out_voxel_sequence[count].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				out_voxel_sequence[count].list_of_index_for_connectivity[m] = m;
			}
		}
		
		out_voxel_sequence[count].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
		time1 += sw1.get_Get_Elapsed_Time(0);

		sw2.r_Reset(0);
		sdkim_chain_code.im_Import(sdkim_run_code);

		for (m = 0; m < num_obj; m++)
		{
			/*********************************************************************************/
			// New Version
			/*********************************************************************************/
			sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
				true,//bool in_mode_for_external_boundary,
				m,//int in_boundary_index,
				NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
				NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
				NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
				&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
				&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
				NULL);//int *out_object_index_related_to_the_boundary___or_NULL);
			/*********************************************************************************/

			gps_Get_Pixel_String(
				&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
				&chain_codes,//CKvXvectorUchar *in_chain_code,
				&b_pt);//CKvXvectorShort *out_boundary_pixel)

			b_pt.z_gii_Get_Informations_Internal(NULL, &p_b_pt, NULL, NULL);
			num_pt = b_pt.ne();

			//out_voxel_sequence[count].voxel_idx_sequences[m] = std::vector<int>(num_pt / 2);
			element_count = 0;
			for (l = 0; l < num_pt; l += 2)
			{
				/******************************************************************/
				//Get Voxel Index Vector
				if (in_plane_mode == 0)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						k,
						p_b_pt[l + 1],
						p_b_pt[l]);

					p_mask[p_b_pt[l]][p_b_pt[l + 1]][k] = true;
				}
				else if (in_plane_mode == 1)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						p_b_pt[l + 1],
						k,
						p_b_pt[l]);

					p_mask[p_b_pt[l]][k][p_b_pt[l + 1]] = true;
				}
				else if (in_plane_mode == 2)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						p_b_pt[l],
						p_b_pt[l + 1],
						k);

					p_mask[k][p_b_pt[l + 1]][p_b_pt[l]] = true;
				}

				if (voxel_idx == -1)
				{
					printf("[[Error : Invalid Voxel Index]]\n");
					return false;
				}
				/******************************************************************/
				out_voxel_sequence[count].voxel_idx_sequences[m].push_back((int)voxel_idx);
				//out_voxel_sequence[count].voxel_idx_sequences[m][element_count] = (int)voxel_idx;
				//element_count++;
			}
		}
		
		count++;
		time2 += sw2.get_Get_Elapsed_Time(0);
	}

	printf("Elapsed Time for Generate Voxel Color & Connectivity : %lf\n", sw.get_Get_Elapsed_Time(0));
	printf("Elapsed Time for Finding Connectivity : %lf\n", time1);
	printf("Elapsed Time for Generating Color Arrays : %lf\n", time2);
	out_voxel_sequence.erase(out_voxel_sequence.begin() + count, out_voxel_sequence.end());
	tmp_xrunset3d.im_Import(cpy_volume);
	luxrunset3d.eo_Exclusive_OR(*io_xrunset3d, tmp_xrunset3d, *io_xrunset3d);


	return true;
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction_Fast(
	VCL_DoCube_X_Color *in_docube,
	CKvXrunset3dShort *io_xrunset3d,
	int in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> &out_voxel_sequence)
//************************************************************************
{
	LCKvUtility_for_Xrunset3d luxrunset3d;
	CKvSet_of_SdkimRunCode set_runcode;
	CKvXrunset3dShort tmp_xrunset3d;
	CKvXrunsetShort null_rs;
	CKvSdkimRunCode null_rc, *p_runcode;
	CKvXrunsetShort *xrun_set; CKvSet_of_XrunsetShort set_xrunset;
	CKvVolumeBool cpy_volume;
	int k, m, num_obj;
	bool ***p_mask;
	CKvStopWatch sw, sw1, sw2;

	if (io_xrunset3d->net_Number_of_Elements_Total() == 0)
	{
		return false;
	}

	/****************************************************************************************/
	out_voxel_sequence = std::vector<Voxel_Slice_Scanned_Data>(in_ww);
	/****************************************************************************************/
	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);

	/***************Update Scanned Voxels******************/
	//io_xrunset3d->ex_Export(true, false, cpy_volume);
	cpy_volume.c_Create(in_dd, in_hh, in_ww, false);
	p_mask = cpy_volume.tp();
	/******************************************************/


	//Set parallel loop
	p_runcode = set_runcode.c_Create(in_ww);
	xrun_set  = set_xrunset.c_Create(in_ww);
#pragma omp parallel
	{
#pragma omp for private(k)
		for (k = 0; k < in_ww; k++)
		{
			gfv_Get_Slice_Voxels(
				io_xrunset3d,//CKvXrunset3dShort *in_runsets,
				in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
				k,//int in_coordinate_at_the_corresponding_axis,
				&xrun_set[k]);//CKvXrunsetShort *out_fragment_voxel);
			p_runcode[k].im_Import(xrun_set[k], true);
		}
	}

	set_xrunset.c_Create(1);

	/***************************************************************************************/
	//Find Start Slice No parallel
	int start_slice;
	for (k = 0; k < in_ww; k++)
	{
		num_obj = p_runcode[k].no_Number_of_Objects();
		if (num_obj != 0)
		{
			start_slice = k;
			out_voxel_sequence[k].blob_index_for_before_slice.push_back(-1);
			out_voxel_sequence[k].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				out_voxel_sequence[k].list_of_index_for_connectivity[m] = m;
			}
			out_voxel_sequence[k].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);

			Get_Surface_Voxel_Index(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				p_mask,//bool ***io_volume_pointer,
				out_voxel_sequence[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			break;
		}
		else
		{
			out_voxel_sequence[k].blob_index_for_before_slice.push_back(-2);
		}
	}
	/***************************************************************************************/

	/***************************************************************************************/
	//Set parallel loop
	sw.c_Create(1);
	sw.r_Reset(0);
#pragma omp parallel
	{
#pragma omp for private(k)
		for (k = (start_slice + 1); k < in_ww; k++)
		{
			if (p_runcode[k].no_Number_of_Objects() == 0)
			{
				out_voxel_sequence[k].blob_index_for_before_slice.push_back(-2);
				continue;
			}

			Get_Connectivity_btw_Slices(
				p_runcode[k],//CKvSdkimRunCode &in_current_runcode,
				p_runcode[k - 1],//CKvSdkimRunCode &in_previous_runcode,
				null_rc,//CKvSdkimRunCode &in_null_runcode,
				out_voxel_sequence[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			Get_Surface_Voxel_Index(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				p_mask,//bool ***io_volume_pointer,
				out_voxel_sequence[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)
		}
	}
	printf(" Elapsed Time for Connectivity and Surface Index: %lf\n ", sw.get_Get_Elapsed_Time(0));
	/***************************************************************************************/

	sw.r_Reset(0);
	for (std::vector<Voxel_Slice_Scanned_Data>::size_type k = 0; k < out_voxel_sequence.size();)
	{
		if (out_voxel_sequence[k].blob_index_for_before_slice[0] == -2)
		{
			out_voxel_sequence.erase(out_voxel_sequence.begin() + k);
		}
		else
		{
			k++;
		}
	}
	printf(" Elapsed Time for Rearrange Data: %lf\n ", sw.get_Get_Elapsed_Time(0));

	sw.r_Reset(0);
	tmp_xrunset3d.im_Import(cpy_volume);
	printf(" Elapsed Time for Import Data : %lf\n ", sw.get_Get_Elapsed_Time(0));
	
	
	sw.r_Reset(0);
	luxrunset3d.eo_Exclusive_OR(*io_xrunset3d, tmp_xrunset3d, *io_xrunset3d);
	printf(" Elapsed Time for XOR: %lf\n ", sw.get_Get_Elapsed_Time(0));

	return true;
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction_Fast2(
	VCL_DoCube_X_Color *in_docube,
	JANG_Xrunset3dShort *io_xrunset3d,
	int in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> *out_voxel_sequence)
//************************************************************************
{
	LCKvUtility_for_Xrunset3d luxrunset3d;
	CKvSet_of_SdkimRunCode set_runcode;
	JANG_Xrunset3dShort tmp_xrunset3d;
	CKvXrunsetShort null_rs;
	CKvSdkimRunCode null_rc, *p_runcode;
	CKvSet_of_XrunsetShort set_xrunset;
	CKvVolumeBool cpy_volume;
	int k, m, num_obj;
	bool ***p_mask;

	if (io_xrunset3d->net_Number_of_Elements_Total() == 0)
	{
		return false;
	}

	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);

	/***************Update Scanned Voxels******************/
	io_xrunset3d->ex_Export(true, false, cpy_volume);
	//cpy_volume.c_Create(in_dd, in_hh, in_ww, false);
	p_mask = cpy_volume.tp();
	/******************************************************/

	//Set parallel loop
	p_runcode = set_runcode.c_Create(in_ww);
#pragma omp parallel num_threads(8)
	{
#pragma omp for private(k)
		for (k = 0; k < in_ww; k++)
		{
			gfv_Get_Slice_Voxels(
				io_xrunset3d,//CKvXrunset3dShort *in_runsets,
				in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
				k,//int in_coordinate_at_the_corresponding_axis,
				&p_runcode[k]);//CKvSdkimRunCode *out_fragment_voxel)
		}
	}

	/***************************************************************************************/
	//Find Start Slice No parallel
	int start_slice;
	for (k = 0; k < in_ww; k++)
	{
		num_obj = p_runcode[k].no_Number_of_Objects();
		if (num_obj != 0)
		{
			start_slice = k;
			(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-1);
			(*out_voxel_sequence)[k].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				(*out_voxel_sequence)[k].list_of_index_for_connectivity[m] = m;
			}
			(*out_voxel_sequence)[k].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);

			Get_Surface_Voxel_Index2(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				p_mask,//bool ***io_volume_pointer,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			break;
		}
		else
		{
			(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
		}
	}
	/***************************************************************************************/
	
	/***************************************************************************************/
	//Set parallel loop

#pragma omp parallel num_threads(8)
	{
#pragma omp for private(k)
		for (k = (start_slice + 1); k < in_ww; k++)
		{
			if (p_runcode[k].no_Number_of_Objects() == 0)
			{
				(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
				continue;
			}

			Get_Connectivity_btw_Slices(
				p_runcode[k],//CKvSdkimRunCode &in_current_runcode,
				p_runcode[k - 1],//CKvSdkimRunCode &in_previous_runcode,
				null_rc,//CKvSdkimRunCode &in_null_runcode,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			Get_Surface_Voxel_Index2(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				p_mask,//bool ***io_volume_pointer,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)
		}
	}
	/***************************************************************************************/

	for (std::vector<Voxel_Slice_Scanned_Data>::size_type k = 0; k < (*out_voxel_sequence).size();)
	{
		if ((*out_voxel_sequence)[k].blob_index_for_before_slice[0] == -2)
		{
			(*out_voxel_sequence).erase((*out_voxel_sequence).begin() + k);
		}
		else
		{
			k++;
		}
	}

	//tmp_xrunset3d.jimport(&cpy_volume);
	io_xrunset3d->jimport(
		&cpy_volume,//CKvVolumeBool *in_volume,
		in_docube->zz_Xlim[0],//int in_xmin,
		in_docube->zz_Xlim[1],//int in_xmanx,
		in_docube->zz_Ylim[0],//int in_ymin,
		in_docube->zz_Ylim[1]);//int in_ymax)

	//luxrunset3d.eo_Exclusive_OR(*io_xrunset3d, tmp_xrunset3d, *io_xrunset3d);

	return true;
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::gvs_Get_Voxel_Sequence_with_Iterative_Surface_Following_with_Orthogonal_Direction_Fast3(
	VCL_DoCube_X_Color *in_docube,
	JANG_Xrunset3dShort *io_xrunset3d,
	int in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> *out_voxel_sequence)
//************************************************************************
{
	CKvSet_of_SdkimRunCode set_runcode;
	CKvXrunsetShort null_rs;
	CKvSdkimRunCode null_rc, *p_runcode;
	CKvSet_of_MatrixBool set_pattern;
	CKvMatrixBool *pattern;
	CKvStopWatch sw;
	int k, m, num_obj, start_slice;

	if (io_xrunset3d->net_Number_of_Elements_Total() == 0)
	{
		return false;
	}

	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);
	pattern = set_pattern.c_Create(in_dd);

	//Set parallel loop
	p_runcode = set_runcode.c_Create(in_ww);
#pragma omp parallel num_threads(8)
	{
#pragma omp for private(k)
		for (k = 0; k < in_ww; k++)
		{
			gfv_Get_Slice_Voxels(
				io_xrunset3d,//CKvXrunset3dShort *in_runsets,
				in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
				k,//int in_coordinate_at_the_corresponding_axis,
				&p_runcode[k]);//CKvSdkimRunCode *out_fragment_voxel)
			pattern[k].c_Create(in_hh, in_ww, false);
		}
	}

	/***************************************************************************************/
	//Find Start Slice No parallel
	for (k = 0; k < in_ww; k++)
	{
		num_obj = p_runcode[k].no_Number_of_Objects();
		if (num_obj != 0)
		{
			start_slice = k;
			(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-1);
			(*out_voxel_sequence)[k].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				(*out_voxel_sequence)[k].list_of_index_for_connectivity[m] = m;
			}
			(*out_voxel_sequence)[k].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);

			Get_Surface_Voxel_Index3(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				&pattern[k],//CKvMatrixBool *io_slice,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			io_xrunset3d->Update(
				&pattern[k],//CKvMatrixBool *in_slice,
				in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
				k);//int in_coordinate_at_the_corresponding_axis)

			break;
		}
		else
		{
			(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
		}
	}
	/***************************************************************************************/

	/***************************************************************************************/
	//Set parallel loop
#pragma omp parallel num_threads(8)
	{
#pragma omp for private(k)
		for (k = (start_slice + 1); k < in_ww; k++)
		{
			if (p_runcode[k].no_Number_of_Objects() == 0)
			{
				(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
				continue;
			}

			Get_Connectivity_btw_Slices(
				p_runcode[k],//CKvSdkimRunCode &in_current_runcode,
				p_runcode[k - 1],//CKvSdkimRunCode &in_previous_runcode,
				null_rc,//CKvSdkimRunCode &in_null_runcode,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			Get_Surface_Voxel_Index3(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				&pattern[k],//CKvMatrixBool *io_slice,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)
		}
	}


	/***************************************************************************************/
	// For Time Measuring 
//	sw.c_Create(1);
//	sw.r_Reset(0);
//#pragma omp parallel num_threads(8)
//	{
//#pragma omp for private(k)
//		for (k = (start_slice + 1); k < in_ww; k++)
//		{
//			if (p_runcode[k].no_Number_of_Objects() == 0)
//			{
//				(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
//				continue;
//			}
//
//			Get_Connectivity_btw_Slices(
//				p_runcode[k],//CKvSdkimRunCode &in_current_runcode,
//				p_runcode[k - 1],//CKvSdkimRunCode &in_previous_runcode,
//				null_rc,//CKvSdkimRunCode &in_null_runcode,
//				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)
//		}
//	}
//	printf("	[Find Connectivity] -> %lf\n", sw.get_Get_Elapsed_Time(0));
//
//
//	sw.r_Reset(0);
//#pragma omp parallel num_threads(8)
//	{
//#pragma omp for private(k)
//		for (k = (start_slice + 1); k < in_ww; k++)
//		{
//			if (p_runcode[k].no_Number_of_Objects() == 0)
//			{
//				(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
//				continue;
//			}
//
//			Get_Surface_Voxel_Index3(
//				in_docube,//VCL_DoCube_X_Color *in_docube,
//				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
//				k,//int in_slice_index,
//				in_plane_mode,//int in_plane_mode,
//				&pattern[k],//CKvMatrixBool *io_slice,
//				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)
//		}
//	}
//	printf("	[Get Surface Voxel Indices] -> %lf\n", sw.get_Get_Elapsed_Time(0));
	/***************************************************************************************/


	/***************************************************************************************/
	//Update Voxels
	sw.r_Reset(0);
	for (k = (start_slice + 1); k < in_ww; k++)
	{
		io_xrunset3d->Update(
			&pattern[k],//CKvMatrixBool *in_slice,
			in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
			k);//int in_coordinate_at_the_corresponding_axis)
	}
	printf("	[Update io_xrunset3d] -> %lf\n", sw.get_Get_Elapsed_Time(0));
	/***************************************************************************************/

	for (std::vector<Voxel_Slice_Scanned_Data>::size_type k = 0; k < (*out_voxel_sequence).size();)
	{
		if ((*out_voxel_sequence)[k].blob_index_for_before_slice[0] == -2)
		{
			(*out_voxel_sequence).erase((*out_voxel_sequence).begin() + k);
		}
		else
		{
			k++;
		}
	}


	return true;
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::Get_Surface_Voxel_Indices(
	VCL_DoCube_X_Color *in_docube,
	CKvXrunset3dShort *in_xrunset3d,
	int in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> *out_voxel_sequence)
//************************************************************************
{
	LCKvUtility_for_Xrunset3d luxrunset3d;
	CKvSet_of_SdkimRunCode set_runcode;
	JANG_Xrunset3dShort tmp_xrunset3d;
	CKvXrunsetShort null_rs;
	CKvSdkimRunCode null_rc, *p_runcode;
	CKvSet_of_XrunsetShort set_xrunset;
	CKvVolumeBool cpy_volume;
	int k, m, num_obj;
	bool ***p_mask;
	CKvStopWatch sw; sw.c_Create(1); sw.r_Reset(0);


	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);

	/***************Update Scanned Voxels******************/
	cpy_volume.c_Create(in_dd, in_hh, in_ww, false);
	p_mask = cpy_volume.tp();
	/******************************************************/

	//Set parallel loop
	p_runcode = set_runcode.c_Create(in_ww);
#pragma omp parallel num_threads(8)
	{
#pragma omp for private(k)
		for (k = 0; k < in_ww; k++)
		{
			gfv_Get_Slice_Voxels(
				in_xrunset3d,//CKvXrunset3dShort *in_runsets,
				in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
				k,//int in_coordinate_at_the_corresponding_axis,
				&p_runcode[k]);//CKvSdkimRunCode *out_fragment_voxel)
		}
	}

	/***************************************************************************************/
	//Find Start Slice No parallel
	int start_slice;
	for (k = 0; k < in_ww; k++)
	{
		num_obj = p_runcode[k].no_Number_of_Objects();
		if (num_obj != 0)
		{
			start_slice = k;
			(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-1);
			(*out_voxel_sequence)[k].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				(*out_voxel_sequence)[k].list_of_index_for_connectivity[m] = m;
			}
			(*out_voxel_sequence)[k].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);

			Get_Surface_Voxel_Index(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				p_mask,//bool ***io_volume_pointer,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			break;
		}
		else
		{
			(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
		}
	}
	/***************************************************************************************/

	/***************************************************************************************/
	//Set parallel loop

#pragma omp parallel num_threads(8)
	{
#pragma omp for private(k)
		for (k = (start_slice + 1); k < in_ww; k++)
		{
			if (p_runcode[k].no_Number_of_Objects() == 0)
			{
				(*out_voxel_sequence)[k].blob_index_for_before_slice.push_back(-2);
				continue;
			}

			Get_Connectivity_btw_Slices(
				p_runcode[k],//CKvSdkimRunCode &in_current_runcode,
				p_runcode[k - 1],//CKvSdkimRunCode &in_previous_runcode,
				null_rc,//CKvSdkimRunCode &in_null_runcode,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)

			Get_Surface_Voxel_Index(
				in_docube,//VCL_DoCube_X_Color *in_docube,
				&p_runcode[k],//CKvSdkimRunCode *in_runcode,
				k,//int in_slice_index,
				in_plane_mode,//int in_plane_mode,
				p_mask,//bool ***io_volume_pointer,
				(*out_voxel_sequence)[k]);//Voxel_Slice_Scanned_Data &out_voxel_sequence)
		}
	}
	/***************************************************************************************/

	for (std::vector<Voxel_Slice_Scanned_Data>::size_type k = 0; k < (*out_voxel_sequence).size();)
	{
		if ((*out_voxel_sequence)[k].blob_index_for_before_slice[0] == -2)
		{
			(*out_voxel_sequence).erase((*out_voxel_sequence).begin() + k);
		}
		else
		{
			k++;
		}
	}

	return true;
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gsvpd_Get_Surface_Voxels_in_Plane_Direction(
	VCL_DoCube_X_Color *in_docube,
	int in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	CKvDepot_of_Point3Df *out_voxels,
	CKvDepot_of_RgbaF *out_colors)
//************************************************************************
{
	CKvXrunset3dShort xrunset3d;
	CKvSdkimChainCode sdkim_chain_code;
	CKvSdkimRunCode sdkim_run_code, an_obj_run_code;
	CKvXrunsetShort xrun_set, an_object;
	CKvXvectorShort b_pt;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	CKvDepot_of_RgbaF colors;
	CKvPoint3Df pt3d; CKvRgbaF rgbaf;
	long long voxel_idx;
	int k, m, l, num_obj, num_chain_code;
	int count, num_voxels, dum;
	bool mode = true;

	/****************************************************************************************/
	/****************************************************************************************/
	xrunset3d.im_Import(in_docube->e_Export_Xvectors_Boundary_Point(), in_dd);
	colors = in_docube->gvc_Get_Voxel_Color();
	num_voxels = colors.ne_Number_of_Elements();
	out_voxels->in_Initialize(num_voxels);
	out_colors->in_Initialize(num_voxels);
	count = 0;
	for (k = 0; k < in_ww; k++)
	{
		gfv_Get_Slice_Voxels(
			&xrunset3d,//CKvXrunset3dShort *in_runsets,
			in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
			k,//int in_coordinate_at_the_corresponding_axis,
			&xrun_set);//CKvXrunsetShort *out_fragment_voxel);

		sdkim_run_code.im_Import(xrun_set, true);
		num_obj = sdkim_run_code.no_Number_of_Objects();
		if (num_obj == 0) { continue; }

		for (m = 0; m < num_obj; m++)
		{
			sdkim_run_code.exo_EXtract_an_Object(m, an_object, NULL);
			an_obj_run_code.im_Import(an_object, true);
			sdkim_chain_code.im_Import(an_obj_run_code);

			num_chain_code = sdkim_chain_code.no_Number_of_Objects();

			sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
				true,//bool in_mode_for_external_boundary,
				0,//int in_boundary_index,
				NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
				NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
				NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
				&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
				&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
				NULL);//int *out_object_index_related_to_the_boundary___or_NULL);

			gps_Get_Pixel_String(
				&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
				&chain_codes,//CKvXvectorUchar *in_chain_code,
				&b_pt);//CKvXvectorShort *out_boundary_pixel)

			for (l = 0; l < b_pt.ne(); l += 2)
			{
				/******************************************************************/
				//Get Voxel Index Vector
				if (in_plane_mode == 0)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						k,
						b_pt.ge_Get_Element(l + 1),
						b_pt.ge_Get_Element(l));

					pt3d.x = k;
					pt3d.y = b_pt.ge_Get_Element(l + 1);
					pt3d.z = b_pt.ge_Get_Element(l);

					colors.ge_Get_Element(voxel_idx, rgbaf);

					out_voxels->ap_Append(false, pt3d, dum);
					out_colors->ap_Append(false, rgbaf, dum);
				}
				else if (in_plane_mode == 1)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						b_pt.ge_Get_Element(l + 1),
						k,
						b_pt.ge_Get_Element(l));

					pt3d.x = b_pt.ge_Get_Element(l + 1);
					pt3d.y = k;
					pt3d.z = b_pt.ge_Get_Element(l);

					colors.ge_Get_Element(voxel_idx, rgbaf);

					out_voxels->ap_Append(false, pt3d, dum);
					out_colors->ap_Append(false, rgbaf, dum);
				}
				else if (in_plane_mode == 2)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						b_pt.ge_Get_Element(l),
						b_pt.ge_Get_Element(l + 1),
						k);

					pt3d.x = b_pt.ge_Get_Element(l);
					pt3d.y = b_pt.ge_Get_Element(l + 1);
					pt3d.z = k;

					colors.ge_Get_Element(voxel_idx, rgbaf);

					out_voxels->ap_Append(false, pt3d, dum);
					out_colors->ap_Append(false, rgbaf, dum);
				}

				if (voxel_idx == -1)
				{
					printf("[[Error : Invalid Voxel Index]]\n");
					return;
				}
				count++;
			}
		}
	}

	out_voxels->cne_Change_Number_of_Elements(count);
	out_colors->cne_Change_Number_of_Elements(count);
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gsvpd_Get_Surface_Voxels_in_Plane_Direction(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> &in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	CKvDepot_of_Point3Df *out_voxels,
	CKvDepot_of_RgbaF *out_colors)
//************************************************************************
{
	CKvDepot_of_Point3Df voxels[3];
	CKvDepot_of_RgbaF colors[3];
	CKvSet_of_Point3Df set_p[2];
	CKvSet_of_RgbaF set_color;
	float xyz[3], rgbaf[4];
	int num1, num2, dum;
	int num_plane_mode, num_pt, k, m, n;

	num_plane_mode = in_plane_mode.size();

	for (k = 0; k < num_plane_mode; k++)
	{
		gsvpd_Get_Surface_Voxels_in_Plane_Direction(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_plane_mode[k],//int in_plane_mode,
			in_ww,//int &in_ww,
			in_hh,//int &in_hh,
			in_dd,//int &in_dd,
			&voxels[k],//CKvDepot_of_Point3Df *out_voxels,
			&colors[k]);//CKvDepot_of_RgbaF *out_colors)

		if (k != 0)
		{
			num_pt = voxels[k].ne_Number_of_Elements();
			colors[k].in_Initialize(num_pt);
			rgbaf[0] = 0.f;
			rgbaf[1] = 1.f;
			rgbaf[2] = 0.f;
			rgbaf[3] = 0.f;

			for (m = 0; m < num_pt; m++)
			{
				colors[k].se_Set_Element(m, rgbaf);
			}
		}
	}

	for (int k = 1; k < num_plane_mode; k++)
	{
		voxels[k].e_Export(&set_p[k - 1]); voxels[k].in_Initialize();
	}

	for (int k = 1; k < num_plane_mode; k++)
	{
		CKvPoint3Df *p_pt3 = set_p[k - 1].vps(num2);
		for (m = 0; m < num2; m++)
		{
			num1 = voxels[0].ne_Number_of_Elements();
			for (n = 0; n < num1; n++)
			{
				voxels[0].ge_Get_Element(n, xyz);
				if ((p_pt3[m].x == xyz[0]) &&
					(p_pt3[m].y == xyz[1]) &&
					(p_pt3[m].z == xyz[2]))
				{
					break;
				}
			}

			if (n == num1)
			{
				voxels[0].ap_Append(false, p_pt3[m], dum);
				colors[k].ge_Get_Element(m, rgbaf);
				colors[0].ap_Append(false, rgbaf, dum);
			}
		}
	}

	out_voxels->cp_Copy(&voxels[0]);
	out_colors->cp_Copy(&colors[0]);
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gsvpd_Get_Rest_of_Surface_Voxels(
	VCL_DoCube_X_Color *in_docube,
	std::vector<int> &in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	CKvDepot_of_Point3Df *out_voxels,
	CKvDepot_of_RgbaF *out_colors)
//************************************************************************
{
	CKvDepot_of_Point3Df voxels[3];
	CKvDepot_of_RgbaF colors[3];
	CKvSet_of_Point3Df set_p[2];
	CKvSet_of_RgbaF set_color;
	float xyz[3], rgbaf[4];
	int num1, num2, dum, count;
	int num_plane_mode, k, m, n;

	num_plane_mode = in_plane_mode.size();

	for (k = 0; k < num_plane_mode; k++)
	{
		gsvpd_Get_Surface_Voxels_in_Plane_Direction(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			in_plane_mode[k],//int in_plane_mode,
			in_ww,//int &in_ww,
			in_hh,//int &in_hh,
			in_dd,//int &in_dd,
			&voxels[k],//CKvDepot_of_Point3Df *out_voxels,
			&colors[k]);//CKvDepot_of_RgbaF *out_colors)

		printf(" Num Voxel %d/%d ) %d\n", k + 1, num_plane_mode, voxels[k].ne_Number_of_Elements());
	}

	for (int k = 1; k < num_plane_mode; k++)
	{
		voxels[k].e_Export(&set_p[k - 1]);
		voxels[k].in_Initialize();
	}

	count = 0;
	int num_voxel = in_docube->gsp_Get_Surface_Points().ne_Number_of_Elements();
	out_voxels->in_Initialize(num_voxel);
	out_colors->in_Initialize(num_voxel);

	for (int k = 1; k < num_plane_mode; k++)
	{
		CKvPoint3Df *p_pt3 = set_p[k - 1].vps(num2);
		for (m = 0; m < num2; m++)
		{
			num1 = voxels[0].ne_Number_of_Elements();
			for (n = 0; n < num1; n++)
			{
				voxels[0].ge_Get_Element(n, xyz);
				if ((p_pt3[m].x == xyz[0]) &&
					(p_pt3[m].y == xyz[1]) &&
					(p_pt3[m].z == xyz[2]))
				{
					break;
				}
			}

			if (n == num1)
			{
				out_voxels->ap_Append(false, p_pt3[m], dum);
				colors[k].ge_Get_Element(m, rgbaf);
				out_colors->ap_Append(false, rgbaf, dum);
				count++;
			}
		}
	}

	printf("count = %d\n", count);
	out_voxels->cne_Change_Number_of_Elements(count);
	out_colors->cne_Change_Number_of_Elements(count);
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gcs_Get_Peeling_Voxel_Sequence(
	VCL_DoCube_X_Color *in_docube,
	int &in_plane_mode_0X_1Y_2Z_3zy,
	std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code)
//************************************************************************
{
	int ww, hh, dd;

	in_docube->gr_Get_Resolution(ww, hh, dd);
	in_docube->gsp_Generate_Indices_of_Surface_Points();
	//in_docube->gsp_Generate_Surface_Points_And_RL_Indices();

	if (in_plane_mode_0X_1Y_2Z_3zy == 0)
	{
		gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_X(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			ww, hh, dd,//int in_ww, int in_hh, int in_dd,
			out_texture_on_code);//std::vector<Voxel_Slice_Peeling_Chain_Code> *out_texture_on_code);
	}
	else if (in_plane_mode_0X_1Y_2Z_3zy == 1)
	{
		gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Y(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			ww, hh, dd,//int in_ww, int in_hh, int in_dd,
			out_texture_on_code);//std::vector<Voxel_Slice_Peeling_Chain_Code> *out_texture_on_code);
	}
	else if (in_plane_mode_0X_1Y_2Z_3zy == 2)
	{
		gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Z(
			in_docube,//VCL_DoCube_X_Color *in_docube,
			ww, hh, dd,//int in_ww, int in_hh, int in_dd,
			out_texture_on_code);//std::vector<Voxel_Slice_Peeling_Chain_Code> *out_texture_on_code);
	}
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::Get_Connectivity_btw_Slices(
	CKvSdkimRunCode &in_current_runcode,
	CKvSdkimRunCode &in_previous_runcode,
	CKvSdkimRunCode &in_null_runcode,
	Voxel_Slice_Scanned_Data &out_voxel_sequence)
//************************************************************************
{
	CKvSdkimRunCode3D sdkim_runcode3d;
	CKvXvectorInt list_of_object_labels_for_FIRST_slice, list_of_object_labels_for_SECOND_slice, EQUIVALENCE_list;
	int nb_obj_tot, new_label_for_SECOND_slice, m;


	nb_obj_tot = in_previous_runcode.no_Number_of_Objects() + in_current_runcode.no_Number_of_Objects();
	list_of_object_labels_for_FIRST_slice.in_Initialize(1);
	EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
	//EQUIVALENCE_list.in_Initialize(std::max(1, nb_obj_tot));
	new_label_for_SECOND_slice = 0;

	sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
		in_null_runcode,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
		in_previous_runcode,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
		list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
		new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
		EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
		list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

	list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
	sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
		in_previous_runcode,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
		in_current_runcode,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
		list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
		new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
		EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
		list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

	out_voxel_sequence.blob_index_for_before_slice = std::vector<int>(EQUIVALENCE_list.ne());
	out_voxel_sequence.list_of_index_for_connectivity = std::vector<int>(list_of_object_labels_for_SECOND_slice.ne());

	for (m = 0; m < EQUIVALENCE_list.ne(); m++)
	{
		out_voxel_sequence.blob_index_for_before_slice[m] = EQUIVALENCE_list.ge_Get_Element(m);
	}

	for (m = 0; m < list_of_object_labels_for_SECOND_slice.ne(); m++)
	{
		out_voxel_sequence.list_of_index_for_connectivity[m] = list_of_object_labels_for_SECOND_slice.ge_Get_Element(m);
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::Get_Surface_Voxel_Index(
	VCL_DoCube_X_Color *in_docube,
	CKvSdkimRunCode *in_runcode,
	int in_slice_index,
	int in_plane_mode,
	bool ***io_volume_pointer,
	Voxel_Slice_Scanned_Data &out_voxel_sequence)
//************************************************************************
{
	CKvSdkimChainCode sdkim_chain_code;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	CKvXvectorShort b_pt;
	int m, l, num_obj, num_pt, element_count, voxel_idx;
	short *p_b_pt;

	sdkim_chain_code.im_Import(*in_runcode);
	num_obj = sdkim_chain_code.no_Number_of_Objects();

	if (num_obj == 0)
	{
		printf("# of object : 0\n");
		return false;
	}

	out_voxel_sequence.voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
	for (m = 0; m < num_obj; m++)
	{
		/*********************************************************************************/
		// New Version
		/*********************************************************************************/
		sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
			true,//bool in_mode_for_external_boundary,
			m,//int in_boundary_index,
			NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
			NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
			NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
			&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
			&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
			NULL);//int *out_object_index_related_to_the_boundary___or_NULL);
		/*********************************************************************************/

		gps_Get_Pixel_String(
			&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
			&chain_codes,//CKvXvectorUchar *in_chain_code,
			&b_pt);//CKvXvectorShort *out_boundary_pixel)

		b_pt.z_gii_Get_Informations_Internal(NULL, &p_b_pt, NULL, NULL);
		num_pt = b_pt.ne();

		element_count = 0;
		for (l = 0; l < num_pt; l += 2)
		{
			/******************************************************************/
			//Get Voxel Index Vector
			if (in_plane_mode == 0)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					in_slice_index,
					p_b_pt[l + 1],
					p_b_pt[l]);

				io_volume_pointer[p_b_pt[l]][p_b_pt[l + 1]][in_slice_index] = true;
			}
			else if (in_plane_mode == 1)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					p_b_pt[l + 1],
					in_slice_index,
					p_b_pt[l]);

				io_volume_pointer[p_b_pt[l]][in_slice_index][p_b_pt[l + 1]] = true;
			}
			else if (in_plane_mode == 2)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					p_b_pt[l],
					p_b_pt[l + 1],
					in_slice_index);

				io_volume_pointer[in_slice_index][p_b_pt[l + 1]][p_b_pt[l]] = true;
			}

			if (voxel_idx == -1)
			{
				printf("[[Error : Invalid Voxel Index]]\n");
				return false;
			}
			/******************************************************************/
			out_voxel_sequence.voxel_idx_sequences[m].push_back((int)voxel_idx);
		}
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::Get_Surface_Voxel_Index2(
	VCL_DoCube_X_Color *in_docube,
	CKvSdkimRunCode *in_runcode,
	int in_slice_index,
	int in_plane_mode,
	bool ***io_volume_pointer,
	Voxel_Slice_Scanned_Data &out_voxel_sequence)
//************************************************************************
{
	CKvSdkimChainCode sdkim_chain_code;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	CKvXvectorShort b_pt;
	int m, l, num_obj, num_pt, element_count, voxel_idx;
	short *p_b_pt;

	sdkim_chain_code.im_Import(*in_runcode);
	num_obj = sdkim_chain_code.no_Number_of_Objects();

	if (num_obj == 0)
	{
		printf("# of object : 0\n");
		return false;
	}

	out_voxel_sequence.voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
	for (m = 0; m < num_obj; m++)
	{
		/*********************************************************************************/
		// New Version
		/*********************************************************************************/
		sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
			true,//bool in_mode_for_external_boundary,
			m,//int in_boundary_index,
			NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
			NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
			NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
			&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
			&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
			NULL);//int *out_object_index_related_to_the_boundary___or_NULL);
		/*********************************************************************************/

		gps_Get_Pixel_String(
			&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
			&chain_codes,//CKvXvectorUchar *in_chain_code,
			&b_pt);//CKvXvectorShort *out_boundary_pixel)

		b_pt.z_gii_Get_Informations_Internal(NULL, &p_b_pt, NULL, NULL);
		num_pt = b_pt.ne();

		element_count = 0;
		for (l = 0; l < num_pt; l += 2)
		{
			/******************************************************************/
			//Get Voxel Index Vector
			if (in_plane_mode == 0)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					in_slice_index,
					p_b_pt[l + 1],
					p_b_pt[l]);

				io_volume_pointer[p_b_pt[l]][p_b_pt[l + 1]][in_slice_index] = false;
			}
			else if (in_plane_mode == 1)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					p_b_pt[l + 1],
					in_slice_index,
					p_b_pt[l]);

				io_volume_pointer[p_b_pt[l]][in_slice_index][p_b_pt[l + 1]] = false;
			}
			else if (in_plane_mode == 2)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					p_b_pt[l],
					p_b_pt[l + 1],
					in_slice_index);

				io_volume_pointer[in_slice_index][p_b_pt[l + 1]][p_b_pt[l]] = false;
			}

			if (voxel_idx == -1)
			{
				printf("[[Error : Invalid Voxel Index]]\n");
				return false;
			}
			/******************************************************************/
			out_voxel_sequence.voxel_idx_sequences[m].push_back((int)voxel_idx);
		}
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Scanner_Slice::Get_Surface_Voxel_Index3(
	VCL_DoCube_X_Color *in_docube,
	CKvSdkimRunCode *in_runcode,
	int in_slice_index,
	int in_plane_mode,
	CKvMatrixBool *io_slice,
	Voxel_Slice_Scanned_Data &out_voxel_sequence)
//************************************************************************
{
	CKvSdkimChainCode sdkim_chain_code;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	CKvXvectorShort b_pt;
	int m, l, num_obj, num_pt, element_count, voxel_idx;
	short *p_b_pt;
	bool **p_slice;

	sdkim_chain_code.im_Import(*in_runcode);
	num_obj = sdkim_chain_code.no_Number_of_Objects();

	if (num_obj == 0)
	{
		printf("# of object : 0\n");
		return false;
	}

	out_voxel_sequence.voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
	p_slice = io_slice->mp();
	for (m = 0; m < num_obj; m++)
	{
		/*********************************************************************************/
		// New Version
		/*********************************************************************************/
		sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
			true,//bool in_mode_for_external_boundary,
			m,//int in_boundary_index,
			NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
			NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
			NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
			&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
			&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
			NULL);//int *out_object_index_related_to_the_boundary___or_NULL);
		/*********************************************************************************/

		gps_Get_Pixel_String(
			&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
			&chain_codes,//CKvXvectorUchar *in_chain_code,
			&b_pt);//CKvXvectorShort *out_boundary_pixel)

		b_pt.z_gii_Get_Informations_Internal(NULL, &p_b_pt, NULL, NULL);
		num_pt = b_pt.ne();

		element_count = 0;
		for (l = 0; l < num_pt; l += 2)
		{
			/******************************************************************/
			//Get Voxel Index Vector
			if (in_plane_mode == 0)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					in_slice_index,
					p_b_pt[l + 1],
					p_b_pt[l]);
			}
			else if (in_plane_mode == 1)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					p_b_pt[l + 1],
					in_slice_index,
					p_b_pt[l]);
			}
			else if (in_plane_mode == 2)
			{
				voxel_idx = in_docube->gvi_Get_Voxel_Index(
					p_b_pt[l],
					p_b_pt[l + 1],
					in_slice_index);
			}

			if (voxel_idx == -1)
			{
				printf("[[Error : Invalid Voxel Index]]\n");
				return false;
			}
			/******************************************************************/
			p_slice[p_b_pt[l+1]][p_b_pt[l]] = true;
			out_voxel_sequence.voxel_idx_sequences[m].push_back((int)voxel_idx);
		}
	}

	return true;
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_X(
	VCL_DoCube_X_Color *in_docube,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code)
//************************************************************************
{
	CKvSdkimRunCode3D sdkim_runcode3d;
	CKvXvectorInt list_of_object_labels_for_FIRST_slice, list_of_object_labels_for_SECOND_slice, EQUIVALENCE_list;
	CKvXrunsetShort null_rs;
	CKvXrunset3dShort runsets;
	CKvSdkimChainCode sdkim_chain_code;
	CKvSdkimRunCode sdkim_run_code, previous_run_code, null_rc, an_obj_run_code;
	CKvXrunsetShort xrun_set, an_object;
	CKvXvectorShort b_pt; CKvMatrixBool masks;
	CKvXvectorInt chain_code_voxel_idx;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	int k, m, l, num_obj, num_chain_code, nb_obj_tot;
	int count, new_label_for_SECOND_slice;
	short *p_b_pt;
	long long voxel_idx;

	/****************************************************************************************/
	out_texture_on_code = std::vector<Voxel_Slice_Scanned_Data>(in_ww);
	/****************************************************************************************/
	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);
	runsets.im_Import(in_docube->e_Export_Xvectors_Boundary_Point(), in_dd);
	count = 0;
	for (k = 0; k < in_ww; k++)
	{
		printf("	slice idx = %d/%d\r", k, in_ww);
		gfv_Get_Slice_Voxels(
			&runsets,//CKvXrunset3dShort *in_runsets,
			0,//int in_plane_mode_0X_1Y_2Z_3zy,
			k,//int in_coordinate_at_the_corresponding_axis,
			&xrun_set);//CKvXrunsetShort *out_fragment_voxel);

		sdkim_run_code.im_Import(xrun_set, true);
		num_obj = sdkim_run_code.no_Number_of_Objects();
		if (num_obj == 0) { continue; }

		/*******************************************************************************************************/
		if (count != 0)
		{
			gfv_Get_Slice_Voxels(
				&runsets,//CKvXrunset3dShort *in_runsets,
				0,//int in_plane_mode_0X_1Y_2Z_3zy,
				k - 1,//int in_coordinate_at_the_corresponding_axis,
				&xrun_set);//CKvXrunsetShort *out_fragment_voxel);
			previous_run_code.im_Import(xrun_set, true);

			nb_obj_tot = previous_run_code.no_Number_of_Objects() + sdkim_run_code.no_Number_of_Objects();
			list_of_object_labels_for_FIRST_slice.in_Initialize(1);
			EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
			//EQUIVALENCE_list.in_Initialize(std::max(1, nb_obj_tot));
			new_label_for_SECOND_slice = 0;

			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				previous_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				previous_run_code,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				sdkim_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			out_texture_on_code[count].blob_index_for_before_slice = std::vector<int>(EQUIVALENCE_list.ne());
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(list_of_object_labels_for_SECOND_slice.ne());

			for (m = 0; m < EQUIVALENCE_list.ne(); m++)
			{
				out_texture_on_code[count].blob_index_for_before_slice[m] = EQUIVALENCE_list.ge_Get_Element(m);
			}
			for (m = 0; m < list_of_object_labels_for_SECOND_slice.ne(); m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = list_of_object_labels_for_SECOND_slice.ge_Get_Element(m);
			}
		}
		else
		{
			out_texture_on_code[count].blob_index_for_before_slice.push_back(-1);
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = m;
			}
			//out_texture_on_code[count].list_of_index_for_connectivity.push_back(-1);
		}
		/*******************************************************************************************************/
		out_texture_on_code[count].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
		for (m = 0; m < num_obj; m++)
		{
			sdkim_run_code.exo_EXtract_an_Object(m, an_object, NULL);
			an_object.ex_Export(true, false, masks);
			while (1)
			{
				an_obj_run_code.im_Import(an_object, true);
				sdkim_chain_code.im_Import(an_obj_run_code);
				num_chain_code = sdkim_chain_code.no_Number_of_Objects();

				if (num_chain_code == 0) { break; }

				bool **p_mask = masks.mp();

				sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
					true,//bool in_mode_for_external_boundary,
					0,//n,//int in_boundary_index,
					NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
					NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
					NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
					&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
					&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
					NULL);//int *out_object_index_related_to_the_boundary___or_NULL);

				gps_Get_Pixel_String(
					&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
					&chain_codes,//CKvXvectorUchar *in_chain_code,
					&b_pt);//CKvXvectorShort *out_boundary_pixel)

				b_pt.z_gii_Get_Informations_Internal(NULL, &p_b_pt, NULL, NULL);

				for (l = 0; l < b_pt.ne(); l += 2)
				{
					/******************************************************************/
					//Get Voxel Index Vector
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						k,
						p_b_pt[l + 1],
						p_b_pt[l]);

					//voxel_idx = in_docube->gvi_Get_Voxel_Index(
					//	k,
					//	b_pt.ge_Get_Element(l + 1),
					//	b_pt.ge_Get_Element(l));

					if (voxel_idx == -1)
					{
						printf("Invalid Voxel Index\n");
						return;
					}

					/******************************************************************/
					out_texture_on_code[count].voxel_idx_sequences[m].push_back((int)voxel_idx);
	
					p_mask[p_b_pt[l + 1]][p_b_pt[l]] = false;

					//masks.se_Set_Element(
					//	b_pt.ge_Get_Element(l),
					//	b_pt.ge_Get_Element(l + 1),
					//	false);
				}
				an_object.im_Import(masks);
			}
		}
		count++;
	}

	out_texture_on_code.erase(out_texture_on_code.begin() + count, out_texture_on_code.end());
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Y(
	VCL_DoCube_X_Color *in_docube,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code)
//************************************************************************
{
	CKvSdkimRunCode3D sdkim_runcode3d;
	CKvXvectorInt list_of_object_labels_for_FIRST_slice, list_of_object_labels_for_SECOND_slice, EQUIVALENCE_list;
	CKvXrunsetShort null_rs;
	CKvXrunset3dShort runsets;
	CKvSdkimChainCode sdkim_chain_code;
	CKvSdkimRunCode sdkim_run_code, previous_run_code, null_rc, an_obj_run_code;
	CKvXrunsetShort xrun_set, an_object;
	CKvXvectorShort b_pt; CKvMatrixBool masks;
	CKvXvectorInt chain_code_voxel_idx;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	int k, m, n, l, num_obj, num_chain_code, nb_obj_tot;
	int count, new_label_for_SECOND_slice;
	short *p_b_pt;
	long long voxel_idx;

	/****************************************************************************************/
	out_texture_on_code = std::vector<Voxel_Slice_Scanned_Data>(in_ww);
	/****************************************************************************************/
	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);
	runsets.im_Import(in_docube->e_Export_Xvectors_Boundary_Point(), in_dd); count = 0;
	for (k = 0; k < in_ww; k++)
	{
		printf("	slice idx = %d/%d\r", k, in_ww);
		gfv_Get_Slice_Voxels(
			&runsets,//CKvXrunset3dShort *in_runsets,
			1,//int in_plane_mode_0X_1Y_2Z_3zy,
			k,//int in_coordinate_at_the_corresponding_axis,
			&xrun_set);//CKvXrunsetShort *out_fragment_voxel);

		sdkim_run_code.im_Import(xrun_set, true);
		num_obj = sdkim_run_code.no_Number_of_Objects();
		if (num_obj == 0) { continue; }
		/*******************************************************************************************************/
		if (count != 0)
		{
			gfv_Get_Slice_Voxels(
				&runsets,//CKvXrunset3dShort *in_runsets,
				1,//int in_plane_mode_0X_1Y_2Z_3zy,
				k - 1,//int in_coordinate_at_the_corresponding_axis,
				&xrun_set);//CKvXrunsetShort *out_fragment_voxel);
			previous_run_code.im_Import(xrun_set, true);

			nb_obj_tot = previous_run_code.no_Number_of_Objects() + sdkim_run_code.no_Number_of_Objects();
			list_of_object_labels_for_FIRST_slice.in_Initialize(1);
			EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
			//EQUIVALENCE_list.in_Initialize(std::max(1, nb_obj_tot));
			new_label_for_SECOND_slice = 0;

			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				previous_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				previous_run_code,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				sdkim_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			out_texture_on_code[count].blob_index_for_before_slice = std::vector<int>(EQUIVALENCE_list.ne());
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(list_of_object_labels_for_SECOND_slice.ne());
			for (m = 0; m < EQUIVALENCE_list.ne(); m++)
			{
				out_texture_on_code[count].blob_index_for_before_slice[m] = EQUIVALENCE_list.ge_Get_Element(m);
			}
			for (m = 0; m < list_of_object_labels_for_SECOND_slice.ne(); m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = list_of_object_labels_for_SECOND_slice.ge_Get_Element(m);
			}
		}
		else
		{

			out_texture_on_code[count].blob_index_for_before_slice.push_back(-1);
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = m;
			}
			//out_texture_on_code[count].blob_index_for_before_slice.push_back(-1);
			//out_texture_on_code[count].list_of_index_for_connectivity.push_back(-1);
		}
		/*******************************************************************************************************/
		out_texture_on_code[count].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
		for (m = 0; m < num_obj; m++)
		{
			sdkim_run_code.exo_EXtract_an_Object(m, an_object, NULL);
			an_object.ex_Export(true, false, masks);
			while (1)
			{
				an_obj_run_code.im_Import(an_object, true);
				sdkim_chain_code.im_Import(an_obj_run_code);
				num_chain_code = sdkim_chain_code.no_Number_of_Objects();

				if (num_chain_code == 0) { break; }
				
				bool **p_mask = masks.mp();

				for (n = 0; n < num_chain_code; n++)
				{
					sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
						true,//bool in_mode_for_external_boundary,
						n,//int in_boundary_index,
						NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
						NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
						NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
						&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
						&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
						NULL);//int *out_object_index_related_to_the_boundary___or_NULL);

					gps_Get_Pixel_String(
						&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
						&chain_codes,//CKvXvectorUchar *in_chain_code,
						&b_pt);//CKvXvectorShort *out_boundary_pixel)

					b_pt.z_gii_Get_Informations_Internal(NULL, &p_b_pt, NULL, NULL);

					for (l = 0; l < b_pt.ne(); l += 2)
					{
						/*******************************/
						//Get Voxel Index Vector
						voxel_idx = in_docube->gvi_Get_Voxel_Index(
							p_b_pt[l+1],
							k,
							p_b_pt[l]);

						//voxel_idx = in_docube->gvi_Get_Voxel_Index(
						//	b_pt.ge_Get_Element(l + 1),
						//	k,
						//	b_pt.ge_Get_Element(l));
						
						if (voxel_idx == -1)
						{
							printf("Invalid Voxel Index\n");
							return;
						}
						/*******************************/
						out_texture_on_code[count].voxel_idx_sequences[m].push_back((int)voxel_idx);

						p_mask[p_b_pt[l+1]][p_b_pt[l]] = false;

						//masks.se_Set_Element(
						//	b_pt.ge_Get_Element(l),
						//	b_pt.ge_Get_Element(l + 1),
						//	false);
					}
				}
				an_object.im_Import(masks);
			}
		}
		count++;
	}

	out_texture_on_code.erase(out_texture_on_code.begin() + count, out_texture_on_code.end());

}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gcs_Get_Peeling_Voxel_Sequence_Plane_Mode_Z(
	VCL_DoCube_X_Color *in_docube,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code)
//************************************************************************
{
	CKvSdkimRunCode3D sdkim_runcode3d;
	CKvXvectorInt list_of_object_labels_for_FIRST_slice, list_of_object_labels_for_SECOND_slice, EQUIVALENCE_list;
	CKvXrunsetShort null_rs;
	CKvXrunset3dShort runsets;
	CKvSdkimChainCode sdkim_chain_code;
	CKvSdkimRunCode sdkim_run_code, previous_run_code, null_rc, an_obj_run_code;
	CKvXrunsetShort xrun_set, an_object;
	CKvXvectorShort b_pt; CKvMatrixBool masks;
	CKvXvectorInt chain_code_voxel_idx;
	CKvXvectorUchar chain_codes;
	CKvPixelShort FIRST_point_in_the_chain_codes;
	int k, m, n, l, num_obj, num_chain_code, nb_obj_tot;
	int count, new_label_for_SECOND_slice;
	short *p_b_pt;
	long long voxel_idx;

	/****************************************************************************************/
	out_texture_on_code = std::vector<Voxel_Slice_Scanned_Data>(in_ww);
	/****************************************************************************************/
	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);
	runsets.im_Import(in_docube->e_Export_Xvectors_Boundary_Point(), in_dd); count = 0;
	for (k = 0; k < in_ww; k++)
	{
		printf("	slice idx = %d/%d\r", k, in_ww);
		gfv_Get_Slice_Voxels(
			&runsets,//CKvXrunset3dShort *in_runsets,
			2,//int in_plane_mode_0X_1Y_2Z_3zy,
			k,//int in_coordinate_at_the_corresponding_axis,
			&xrun_set);//CKvXrunsetShort *out_fragment_voxel);

		sdkim_run_code.im_Import(xrun_set, true);
		num_obj = sdkim_run_code.no_Number_of_Objects();
		if (num_obj == 0) { continue; }
		/*******************************************************************************************************/
		if (count != 0)
		{
			gfv_Get_Slice_Voxels(
				&runsets,//CKvXrunset3dShort *in_runsets,
				2,//int in_plane_mode_0X_1Y_2Z_3zy,
				k - 1,//int in_coordinate_at_the_corresponding_axis,
				&xrun_set);//CKvXrunsetShort *out_fragment_voxel);
			previous_run_code.im_Import(xrun_set, true);

			nb_obj_tot = previous_run_code.no_Number_of_Objects() + sdkim_run_code.no_Number_of_Objects();
			list_of_object_labels_for_FIRST_slice.in_Initialize(1);
			EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
			//EQUIVALENCE_list.in_Initialize(std::max(1, nb_obj_tot));
			new_label_for_SECOND_slice = 0;

			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				previous_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				previous_run_code,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				sdkim_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			out_texture_on_code[count].blob_index_for_before_slice = std::vector<int>(EQUIVALENCE_list.ne());
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(list_of_object_labels_for_SECOND_slice.ne());
			for (m = 0; m < EQUIVALENCE_list.ne(); m++)
			{
				out_texture_on_code[count].blob_index_for_before_slice[m] = EQUIVALENCE_list.ge_Get_Element(m);
			}
			for (m = 0; m < list_of_object_labels_for_SECOND_slice.ne(); m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = list_of_object_labels_for_SECOND_slice.ge_Get_Element(m);
			}
		}
		else
		{
			out_texture_on_code[count].blob_index_for_before_slice.push_back(-1);
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = m;
			}

			//out_texture_on_code[count].blob_index_for_before_slice.push_back(-1);
			//out_texture_on_code[count].list_of_index_for_connectivity.push_back(-1);
		}
		/*******************************************************************************************************/
		out_texture_on_code[count].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
		for (m = 0; m < num_obj; m++)
		{
			sdkim_run_code.exo_EXtract_an_Object(m, an_object, NULL);
			an_object.ex_Export(true, false, masks);
			while (1)
			{
				an_obj_run_code.im_Import(an_object, true);
				sdkim_chain_code.im_Import(an_obj_run_code);
				num_chain_code = sdkim_chain_code.no_Number_of_Objects();

				if (num_chain_code == 0) { break; }
				
				bool **p_mask = masks.mp();

				for (n = 0; n < num_chain_code; n++)
				{
					sdkim_chain_code.gspb_Get_Set_of_Points_on_a_Boundary(
						true,//bool in_mode_for_external_boundary,
						n,//int in_boundary_index,
						NULL,//CKvXvectorShort *out_CORNER_points___or_NULL,
						NULL,//CKvXvectorShort *out_RUN_points___or_NULL,
						NULL,//CKvXvectorShort *out_ALL_BOUNDARY_points___or_NULL,
						&chain_codes,//CKvXvectorUchar *out_chain_codes___or_NULL,
						&FIRST_point_in_the_chain_codes,//CKvPixelShort *out_FIRST_point_in_the_chain_codes___or_NULL,
						NULL);//int *out_object_index_related_to_the_boundary___or_NULL);

					gps_Get_Pixel_String(
						&FIRST_point_in_the_chain_codes,//CKvPixelShort *in_first_pixel,
						&chain_codes,//CKvXvectorUchar *in_chain_code,
						&b_pt);//CKvXvectorShort *out_boundary_pixel)

					b_pt.z_gii_Get_Informations_Internal(NULL, &p_b_pt, NULL, NULL);

					for (l = 0; l < b_pt.ne(); l += 2)
					{
						/*******************************/
						//Get Voxel Index Vector
						voxel_idx = in_docube->gvi_Get_Voxel_Index(
							p_b_pt[l],
							p_b_pt[l + 1],
							k);

						if (voxel_idx == -1)
						{
							printf("Invalid Voxel Index\n");
							return;
						}
						/*******************************/
						out_texture_on_code[count].voxel_idx_sequences[m].push_back((int)voxel_idx);

						p_mask[p_b_pt[l + 1]][p_b_pt[l]] = false;
					}
				}
				an_object.im_Import(masks);
			}
		}
		count++;
	}

	out_texture_on_code.erase(out_texture_on_code.begin() + count, out_texture_on_code.end());
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gcs_Get_Voxel_Idx_in_Plane(
	VCL_DoCube_X_Color *in_docube,
	int &in_plane_mode,
	int &in_ww,
	int &in_hh,
	int &in_dd,
	std::vector<Voxel_Slice_Scanned_Data> &out_texture_on_code)
//************************************************************************
{
	CKvSdkimRunCode3D sdkim_runcode3d;
	CKvXvectorInt list_of_object_labels_for_FIRST_slice, list_of_object_labels_for_SECOND_slice, EQUIVALENCE_list;
	CKvXrunsetShort null_rs;
	CKvXrunset3dShort runsets;
	CKvSdkimRunCode sdkim_run_code, previous_run_code, null_rc, an_obj_run_code;
	CKvXrunsetShort xrun_set, an_object;
	CKvSet_of_PixelShort set_pixels;

	int k, m, l, num_obj, nb_obj_tot;
	int count, new_label_for_SECOND_slice;
	long long voxel_idx;

	/****************************************************************************************/
	out_texture_on_code = std::vector<Voxel_Slice_Scanned_Data>(in_ww);
	/****************************************************************************************/
	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);
	runsets.im_Import(in_docube->e_Export_Xvectors_Boundary_Point(), in_dd);
	count = 0;
	for (k = 0; k < in_ww; k++)
	{
		if (k == 583)
		{
			printf("count= %d\n", count);
		}

		//printf("	slice idx = %d/%d\r", k, in_ww);
		gfv_Get_Slice_Voxels(
			&runsets,//CKvXrunset3dShort *in_runsets,
			in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
			k,//int in_coordinate_at_the_corresponding_axis,
			&xrun_set);//CKvXrunsetShort *out_fragment_voxel);

		sdkim_run_code.im_Import(xrun_set, true);
		num_obj = sdkim_run_code.no_Number_of_Objects();
		if (num_obj == 0) { continue; }

		/*******************************************************************************************************/
		if (count != 0)
		{
			gfv_Get_Slice_Voxels(
				&runsets,//CKvXrunset3dShort *in_runsets,
				in_plane_mode,//int in_plane_mode_0X_1Y_2Z_3zy,
				k - 1,//int in_coordinate_at_the_corresponding_axis,
				&xrun_set);//CKvXrunsetShort *out_fragment_voxel);
			previous_run_code.im_Import(xrun_set, true);

			nb_obj_tot = previous_run_code.no_Number_of_Objects() + sdkim_run_code.no_Number_of_Objects();
			list_of_object_labels_for_FIRST_slice.in_Initialize(1);
			EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
			new_label_for_SECOND_slice = 0;

			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				previous_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
			sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
				previous_run_code,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
				sdkim_run_code,//CKvSdkimRunCode &in_runset_of_SECOND_slice,
				list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
				new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
				EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
				list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

			out_texture_on_code[count].blob_index_for_before_slice = std::vector<int>(EQUIVALENCE_list.ne());
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(list_of_object_labels_for_SECOND_slice.ne());

			for (m = 0; m < EQUIVALENCE_list.ne(); m++)
			{
				out_texture_on_code[count].blob_index_for_before_slice[m] = EQUIVALENCE_list.ge_Get_Element(m);
			}
			for (m = 0; m < list_of_object_labels_for_SECOND_slice.ne(); m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = list_of_object_labels_for_SECOND_slice.ge_Get_Element(m);
			}
		}
		else
		{
			out_texture_on_code[count].blob_index_for_before_slice.push_back(-1);
			out_texture_on_code[count].list_of_index_for_connectivity = std::vector<int>(num_obj);
			for (m = 0; m < num_obj; m++)
			{
				out_texture_on_code[count].list_of_index_for_connectivity[m] = m;
			}
			//out_texture_on_code[count].list_of_index_for_connectivity.push_back(-1);
		}
		/*******************************************************************************************************/
		out_texture_on_code[count].voxel_idx_sequences = std::vector<std::vector<int>>(num_obj);
		for (m = 0; m < num_obj; m++)
		{
			sdkim_run_code.exo_EXtract_an_Object(m, an_object, NULL);
			an_object.gsp_Get_Set_of_Pixels(set_pixels);
			CKvPixelShort *p_pixel = set_pixels.vp();

			for (l = 0; l < set_pixels.vs(); l++)
			{
				/******************************************************************/
				//Get Voxel Index Vector
				if (in_plane_mode == 0)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						k,
						p_pixel[l].y,
						p_pixel[l].x);
				}
				else if(in_plane_mode == 1)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						p_pixel[l].y,
						k,
						p_pixel[l].x);
				}
				else if (in_plane_mode == 2)
				{
					voxel_idx = in_docube->gvi_Get_Voxel_Index(
						p_pixel[l].x,
						p_pixel[l].y,
						k);
				}

				if (voxel_idx < 0)
				{
					printf("Invalid Voxel Index\n");
					return;
				}

				/******************************************************************/
				out_texture_on_code[count].voxel_idx_sequences[m].push_back((int)voxel_idx);
			}
		}
		count++;
	}

	out_texture_on_code.erase(out_texture_on_code.begin() + count, out_texture_on_code.end());
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gps_Get_Pixel_String(
	CKvPixelShort *in_first_pixel,
	CKvXvectorUchar *in_chain_code,
	CKvXvectorShort *out_boundary_pixel)
//************************************************************************
{
	CKvPixelShort pt;
	int num_chain_code, k;
	unsigned char code;

	num_chain_code = in_chain_code->ne();

	out_boundary_pixel->in_Initialize(2 * num_chain_code);
	out_boundary_pixel->ae_Append_Element(in_first_pixel->x);
	out_boundary_pixel->ae_Append_Element(in_first_pixel->y);

	pt.x = in_first_pixel->x; pt.y = in_first_pixel->y;
	for (k = 0; k < num_chain_code; k++)
	{
		code = in_chain_code->ge_Get_Element(k);

		switch (code)
		{
		case 0:
			pt.x++;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;

		case 1:
			pt.x++;
			pt.y++;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;

		case 2:
			pt.y++;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;

		case 3:
			pt.x--;
			pt.y++;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;

		case 4:
			pt.x--;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;

		case 5:
			pt.x--;
			pt.y--;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;

		case 6:
			pt.y--;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;

		case 7:
			pt.x++;
			pt.y--;
			out_boundary_pixel->ae_Append_Element(pt.x);
			out_boundary_pixel->ae_Append_Element(pt.y);
			break;
		}
	}

	fup_Find_Unique_Pixel(out_boundary_pixel);

}
//************************************************************************
void VCL_Voxel_Scanner_Slice::fup_Find_Unique_Pixel(
	CKvXvectorShort *io_pixel)
//************************************************************************
{
	CKvXvectorShort unique_pixel;
	int m, n; short a[2]; bool flag;

	unique_pixel.in_Initialize(io_pixel->ne());
	unique_pixel.ae_Append_Element(io_pixel->ge_Get_Element(0));
	unique_pixel.ae_Append_Element(io_pixel->ge_Get_Element(1));

	for (m = 2; m < io_pixel->ne(); m += 2)
	{
		a[0] = io_pixel->ge_Get_Element(m);
		a[1] = io_pixel->ge_Get_Element(m + 1);
		flag = false;
		for (n = 0; n < unique_pixel.ne(); n += 2)
		{
			if ((a[0] == unique_pixel.ge_Get_Element(n)) &&
				(a[1] == unique_pixel.ge_Get_Element(n + 1)))
			{
				flag = true;
				break;
			}
		}
		if (flag == false)
		{
			unique_pixel.ae_Append_Element(a[0]);
			unique_pixel.ae_Append_Element(a[1]);
		}
	}

	io_pixel->cp_Copy(&unique_pixel);
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gfv_Get_Slice_Voxels(
	CKvXrunset3dShort *in_runsets,
	int in_plane_mode_0X_1Y_2Z_3zy,
	int in_coordinate_at_the_corresponding_axis,
	CKvXrunsetShort *out_fragment_voxel)
//************************************************************************
{
	CKvSet2d_of_Xrunset1dShort fragment_voxel, runset3d_xrunset;
	CKvSet_of_Xrunset1dShort slice_voxel;
	CKvSet2d_of_XvectorShort tmp;
	CKvXrunset1dShort *p_slice_voxel;
	int ww, hh, dd, n;

	in_runsets->ts(ww, hh, dd);
	/***********************Get Information for Fragment Voxel****************************/
	if (in_plane_mode_0X_1Y_2Z_3zy == 0)
	{
		p_slice_voxel = slice_voxel.c_Create(hh);
		for (n = 0; n < hh; n++)
		{
			in_runsets->gr_z_Get_a_Ray_along_Z_axis(
				in_coordinate_at_the_corresponding_axis,//int in_x,
				n,//int in_y,
				p_slice_voxel[n]);//CKvXrunset1dShort &out_ray);
		}
		out_fragment_voxel->im_Import(slice_voxel);

	}
	else if (in_plane_mode_0X_1Y_2Z_3zy == 1)
	{
		p_slice_voxel = slice_voxel.c_Create(ww);
		for (n = 0; n < ww; n++)
		{
			in_runsets->gr_z_Get_a_Ray_along_Z_axis(
				n,//int in_x,
				in_coordinate_at_the_corresponding_axis,//int in_y,
				p_slice_voxel[n]);//CKvXrunset1dShort &out_ray);
		}
		out_fragment_voxel->im_Import(slice_voxel);
	}
	else if (in_plane_mode_0X_1Y_2Z_3zy == 2)
	{
		in_runsets->gs_xy_Get_Slice_XY(
			in_coordinate_at_the_corresponding_axis,
			*out_fragment_voxel);
	}
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::gfv_Get_Slice_Voxels(
	CKvXrunset3dShort *in_runsets,
	int in_plane_mode_0X_1Y_2Z_3zy,
	int in_coordinate_at_the_corresponding_axis,
	CKvSdkimRunCode *out_fragment_voxel)
//************************************************************************
{
	CKvXrunsetShort a;

	gfv_Get_Slice_Voxels(
		in_runsets,//CKvXrunset3dShort *in_runsets,
		in_plane_mode_0X_1Y_2Z_3zy,//int in_plane_mode_0X_1Y_2Z_3zy,
		in_coordinate_at_the_corresponding_axis,//int in_coordinate_at_the_corresponding_axis,
		&a);//CKvXrunsetShort *out_fragment_voxel)

	out_fragment_voxel->im_Import(a, true);
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::Exclusive_OR_Parallel(
	CKvXrunset3dShort *in_runset1,
	CKvXrunset3dShort *in_runset2,
	CKvXrunset3dShort *out_runset)
//************************************************************************
{
//	//z_psr_Pointer_of_Set_of_Rays()
//
//	LCKvUtility_for_Xrunset1d lux1d;
//	CKvSet2d_of_Xrunset1dShort set_of_1d_runsets1;
//	CKvSet2d_of_Xrunset1dShort set_of_1d_runsets2;
//	CKvSet2d_of_Xrunset1dShort set_of_1d_runsets3;
//	int ww, hh, dd, j, i;
//
//	in_runset1->ex_Export(set_of_1d_runsets1);
//	in_runset2->ex_Export(set_of_1d_runsets2);
//	in_runset1->ts_Volume_Width_Height_Depth_of_corresponding_volume(ww, hh, dd);
//
//	CKvXrunset1dShort **p1 = set_of_1d_runsets1.mp();
//	CKvXrunset1dShort **p2 = set_of_1d_runsets2.mp();
//	CKvXrunset1dShort **p3 = set_of_1d_runsets3.c_Create(hh, ww);
//#pragma omp parallel
//	{
//#pragma omp for private(i)
//		for (j = 0; j < hh; j++)
//		{
//			for (i = 0; i < ww; i++)
//			{
//				lux1d.eo_Exclusive_OR(p1[j][i], p2[j][i], p3[j][i]);
//			}
//		}
//	}
//
//	out_runset->im_Import(set_of_1d_runsets3);
}
//************************************************************************
void VCL_Voxel_Scanner_Slice::Import_Fast(
	CKvVolumeBool *in_volume,
	CKvXrunset3dShort *out_xrunset)
//************************************************************************
{
	CKvSet2d_of_Xrunset1dShort set2d_xrunset1d;
	int ww, hh, dd, j, i;

	bool ***p_volume = in_volume->tps(ww,hh,dd);

	CKvXrunset1dShort **p_xrunset1d = set2d_xrunset1d.c_Create(hh, ww);

#pragma omp parallel
	{
#pragma omp for private(i)
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				Set_Xrunset1d(
					in_volume,//CKvVolumeBool *in_volume,
					i,//int x,
					j,//int y,
					&p_xrunset1d[j][i]);//CKvXrunset1dShort *out_xrunset)
			}
		}
	}


	out_xrunset->im_Import(set2d_xrunset1d);

}
//************************************************************************
void VCL_Voxel_Scanner_Slice::Set_Xrunset1d(
	CKvVolumeBool *in_volume,
	int x,
	int y,
	CKvXrunset1dShort *out_xrunset)
//************************************************************************
{
	CKvVectorBool pattern;
	int ww, hh, dd, k;

	bool ***p_volume = in_volume->tps(ww, hh, dd);
	bool *p_pattern  = pattern.c_Create(dd);

	for (k = 0; k < dd; k++)
	{
		p_pattern[k] = p_volume[k][y][x];
	}
	out_xrunset->im_Import(pattern);

}
//************************************************************************
void VCL_Voxel_Scanner_Slice::tm_Test_Module(
	VCL_DoCube_X_Color *in_docube,
	int in_ww,
	int in_hh,
	int in_dd)
//************************************************************************
{
	CKvSdkimRunCode3D sdkim_runcode3d;
	CKvXrunset3dShort runsets;
	CKvXrunsetShort xrun_set, an_object;
	CKvSet_of_XrunsetShort set_xrunset;
	CKvSet_of_SdkimRunCode set_runcode;
	CKvSdkimRunCode *p_runcode;
	CKvXvectorInt list_of_object_labels_for_FIRST_slice;
	CKvXvectorInt EQUIVALENCE_list;
	CKvXvectorInt list_of_object_labels_for_SECOND_slice;
	CKvSdkimRunCode null_rc;
	CKvXrunsetShort null_rs, tmp;
	CKvMatrixBool mat_bool1, mat_bool2;
	int new_label_for_SECOND_slice, nb_obj_tot;
	int *a, num, k, j; bool f;

	null_rs.cn_Create_Null_pattern(in_hh, in_ww);
	null_rc.im_Import(null_rs, true);

	new_label_for_SECOND_slice = nb_obj_tot = 0;

	runsets.im_Import(in_docube->e_Export_Xvectors_Boundary_Point(), in_dd);
	//runsets.im_Import(in_docube->e_Export_Xvectors(), in_dd);
	p_runcode = set_runcode.c_Create(in_ww);

	for (k = 0; k < in_ww; k++)
	{
		gfv_Get_Slice_Voxels(
			&runsets,//CKvXrunset3dShort *in_runsets,
			1,//int in_plane_mode_0X_1Y_2Z_3zy,
			k,//int in_coordinate_at_the_corresponding_axis,
			&xrun_set);//CKvXrunsetShort *out_fragment_voxel);

		p_runcode[k].im_Import(xrun_set, true);
		nb_obj_tot += p_runcode[k].no_Number_of_Objects();
	}

	// Slice 마다 확인하는 module.
	/************************************************************************************************************************/
	for (k = 0; k < in_ww - 1; k++)
	{
		nb_obj_tot = p_runcode[k].no_Number_of_Objects() + p_runcode[k + 1].no_Number_of_Objects();
		list_of_object_labels_for_FIRST_slice.in_Initialize(1);
		EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
		//EQUIVALENCE_list.in_Initialize(std::max(1, nb_obj_tot));
		new_label_for_SECOND_slice = 0;

		sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
			null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
			p_runcode[k],//CKvSdkimRunCode &in_runset_of_SECOND_slice,
			list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
			new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
			EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
			list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

		list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
		sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
			p_runcode[k],//CKvSdkimRunCode &in_runset_of_FIRST_slice,
			p_runcode[k + 1],//CKvSdkimRunCode &in_runset_of_SECOND_slice,
			list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
			new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
			EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
			list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);

		if (p_runcode[k + 1].no_Number_of_Objects() != 0)
		{
			printf("k=%d/%d) %d\n", k + 1, in_ww, p_runcode[k + 1].no_Number_of_Objects());
			printf("[EQUIVALENCE_list]\n");
			EQUIVALENCE_list.z_gii(NULL, &a, &num, NULL);
			for (j = 0; j < num; j++)
			{
				printf("j=%d/%d) %d\n", j, num, a[j]);
			}
			printf("[list_of_object_labels_for_SECOND_slice]\n");
			list_of_object_labels_for_SECOND_slice.z_gii(NULL, &a, &num, NULL);
			for (j = 0; j < num; j++)
			{
				printf("j=%d/%d) %d\n", j, num, a[j]);
			}

			printf("//////////////////////////////////////////////////////\n");
		}

		CKvScreen sc[2];
		//if ((p_runcode[k + 1].no_Number_of_Objects() == 3) || (p_runcode[k].no_Number_of_Objects() == 3))
		//{
		p_runcode[k].ex_Export(tmp, f);
		tmp.ex_Export(true, false, mat_bool1);
		sc[0].s_d_Display(&mat_bool1);
		for (j = 0; j < p_runcode[k].no_Number_of_Objects(); j++)
		{
			p_runcode[k].exo_EXtract_an_Object(j, tmp, NULL);
			tmp.ex_Export(true, false, mat_bool2);
			sc[1].s_d_Display(&mat_bool2);
			Kv_Printf("%d ) Label %d", k, j);
		}
		p_runcode[k + 1].ex_Export(tmp, f);
		tmp.ex_Export(true, false, mat_bool1);
		sc[0].s_d_Display(&mat_bool1);
		for (j = 0; j < p_runcode[k + 1].no_Number_of_Objects(); j++)
		{
			p_runcode[k + 1].exo_EXtract_an_Object(j, tmp, NULL);
			tmp.ex_Export(true, false, mat_bool2);
			sc[1].s_d_Display(&mat_bool2);
			Kv_Printf("%d ) Label %d", k + 1, j);
		}
		//}


	}
	/************************************************************************************************************************/

	// Slice 마다 확인하는 module.
	/************************************************************************************************************************/
	//for (k = 0; k < in_ww-1; k++)
	//{
	//	nb_obj_tot = p_runcode[k].no_Number_of_Objects() + p_runcode[k + 1].no_Number_of_Objects();
	//	list_of_object_labels_for_FIRST_slice.in_Initialize(1);
	//	EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
	//	new_label_for_SECOND_slice = 0;
	//	sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
	//		null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
	//		p_runcode[k],//CKvSdkimRunCode &in_runset_of_SECOND_slice,
	//		list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
	//		new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
	//		EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
	//		list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);
	//	
	//	list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);
	//	sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
	//		p_runcode[k],//CKvSdkimRunCode &in_runset_of_FIRST_slice,
	//		p_runcode[k+1],//CKvSdkimRunCode &in_runset_of_SECOND_slice,
	//		list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
	//		new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
	//		EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
	//		list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);
	//	
	//	
	//	if (p_runcode[k + 1].no_Number_of_Objects() != 0)
	//	{
	//		printf("k=%d/%d) %d\n", k+1, in_ww, p_runcode[k+1].no_Number_of_Objects());
	//		printf("[EQUIVALENCE_list]\n");
	//		EQUIVALENCE_list.z_gii(NULL, &a, &num, NULL);
	//		for (j = 0; j < num; j++)
	//		{
	//			printf("j=%d/%d) %d\n", j, num, a[j]);
	//		}
	//		printf("[list_of_object_labels_for_SECOND_slice]\n");
	//		list_of_object_labels_for_SECOND_slice.z_gii(NULL, &a, &num, NULL);
	//		for (j = 0; j < num; j++)
	//		{
	//			printf("j=%d/%d) %d\n", j, num, a[j]);
	//		}
	//		
	//		printf("//////////////////////////////////////////////////////\n");
	//	}
	//}
	/************************************************************************************************************************/


	// Object 전체를 확인. 
	/************************************************************************************************************************/
	//list_of_object_labels_for_FIRST_slice.in_Initialize(1);
	//EQUIVALENCE_list.in_Initialize(max(1, nb_obj_tot));
	//new_label_for_SECOND_slice = 0;
	//
	//sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
	//	null_rc,//CKvSdkimRunCode &in_runset_of_FIRST_slice,
	//	p_runcode[k],//CKvSdkimRunCode &in_runset_of_SECOND_slice,
	//	list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
	//	new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
	//	EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
	//	list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);
	//list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);	
	//
	//for (k = 0; k < in_ww - 1; k++)
	//{
	//	sdkim_runcode3d.uz_aloc_Assign_Labels_to_Objects_considering_Connectedness(
	//		p_runcode[k],//CKvSdkimRunCode &in_runset_of_FIRST_slice,
	//		p_runcode[k + 1],//CKvSdkimRunCode &in_runset_of_SECOND_slice,
	//		list_of_object_labels_for_FIRST_slice,//CKvXvectorInt &in_list_of_object_labels_for_FIRST_slice,
	//		new_label_for_SECOND_slice,//int &io_new_label_for_SECOND_slice,
	//		EQUIVALENCE_list,//CKvXvectorInt &io_EQUIVALENCE_list,
	//		list_of_object_labels_for_SECOND_slice);//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice);
	//	list_of_object_labels_for_FIRST_slice.cp_Copy(&list_of_object_labels_for_SECOND_slice);	
	//	if (p_runcode[k + 1].no_Number_of_Objects() != 0)
	//	{
	//		printf("k=%d/%d) %d\n", k + 1, in_ww, p_runcode[k + 1].no_Number_of_Objects());
	//		printf("[EQUIVALENCE_list]\n");
	//		EQUIVALENCE_list.z_gii(NULL, &a, &num, NULL);
	//		for (j = 0; j < num; j++)
	//		{
	//			printf("j=%d/%d) %d\n", j, num, a[j]);
	//		}
	//		printf("[list_of_object_labels_for_SECOND_slice]\n");
	//		list_of_object_labels_for_SECOND_slice.z_gii(NULL, &a, &num, NULL);
	//		for (j = 0; j < num; j++)
	//		{
	//			printf("j=%d/%d) %d\n", j, num, a[j]);
	//		}
	//		printf("//////////////////////////////////////////////////////\n");
	//	}
	//}
	/************************************************************************************************************************/

}
//************************************************************************