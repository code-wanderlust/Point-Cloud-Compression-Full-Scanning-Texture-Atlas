#include "stdafx.h"
#include "VCL_Voxel_Color_Map.h"


//************************************************************************
VCL_Voxel_Color_Map::VCL_Voxel_Color_Map()
//************************************************************************
{
	zz_var_for_debug = 0;
	zz_width  = 1920;
	zz_height = 1080;
	zz_safeguard = 0;
}
//************************************************************************
VCL_Voxel_Color_Map::~VCL_Voxel_Color_Map()
//************************************************************************
{

}
//************************************************************************
void VCL_Voxel_Color_Map::sp_Set_Parameter_Width(
	int in_map_width)
//************************************************************************
{
	zz_width = in_map_width;
}
//************************************************************************
void VCL_Voxel_Color_Map::sp_Set_Parameter_Height(
	int in_map_height)
//************************************************************************
{
	zz_height = in_map_height;
}
//************************************************************************
void VCL_Voxel_Color_Map::sp_Set_Parameter_Map_Size(
	int in_map_width,
	int in_map_height)
//************************************************************************
{
	zz_width = in_map_width;
	zz_height = in_map_height;
}
//************************************************************************
void VCL_Voxel_Color_Map::li_Load_Image_from_OpenCV(
	CKvString &in_filename,
	CKvMatrixUcharRgb *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());
	cv::Mat image = cv::imread(name, cv::IMREAD_COLOR);
	
	unsigned char **pr, **pg, **pb;

	int hh = image.rows;
	int ww = image.cols;

	out_image->c_Create(hh, ww);
	pr = out_image->mp(pg, pb);
	unsigned char *p = image.data;

	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			pr[j][i] = p[j*ww * 3 + i * 3 + 2];
			pg[j][i] = p[j*ww * 3 + i * 3 + 1];
			pb[j][i] = p[j*ww * 3 + i * 3];
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Map::li_Load_Image_from_OpenCV_16bit(
	CKvString &in_filename,
	CKvMatrixIntRgb *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());
	cv::Mat image = cv::imread(name, cv::IMREAD_COLOR);

	int **pr, **pg, **pb;

	int hh = image.rows;
	int ww = image.cols;


	out_image->c_Create(hh, ww);
	pr = out_image->mp(pg, pb);
	unsigned int *p = (unsigned int *)(image.data);

	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			pr[j][i] = p[j*ww * 3 + i * 3 + 2] - 255;
			pg[j][i] = p[j*ww * 3 + i * 3 + 1] - 255;
			pb[j][i] = p[j*ww * 3 + i * 3] - 255;
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::li_Load_Image_from_OpenCV(
	CKvString &in_filename,
	CKvMatrixUchar *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());
	cv::Mat image = cv::imread(name, cv::IMREAD_GRAYSCALE);

	int hh = image.rows;
	int ww = image.cols;

	out_image->c_Create(hh, ww);
	unsigned char **pr = out_image->mp();
	unsigned char *p   = image.data;

	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			pr[j][i] = p[j*ww + i];
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::li_Load_Image_from_OpenCV(
	CKvString &in_filename,
	CKvMatrixBool *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());
	cv::Mat image = cv::imread(name, cv::IMREAD_GRAYSCALE);

	int hh = image.rows;
	int ww = image.cols;

	out_image->c_Create(hh, ww);
	bool **pr = out_image->mp();
	unsigned char *p = image.data;

	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			if (p[j*ww + i] != 0)
			{
				pr[j][i] = true;
			}
			else
			{
				pr[j][i] = false;
			}
			
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::si_Save_Image_from_OpenCV(
	CKvString &in_filename,
	CKvMatrixUcharRgb *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());
	
	unsigned char **pr, **pg, **pb;
	int ww, hh;

	out_image->ms(ww, hh);
	pr = out_image->mp(pg, pb);

	cv::Mat image = cv::Mat(hh, ww, CV_8UC3);

	unsigned char *p = image.data;

	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			p[j*ww * 3 + i * 3 + 2] = pr[j][i];
			p[j*ww * 3 + i * 3 + 1] = pg[j][i];
			p[j*ww * 3 + i * 3] = pb[j][i];
		}
	}

	cv::imwrite(name, image);
}
//************************************************************************
void VCL_Voxel_Color_Map::si_Save_Image_from_OpenCV_16bit(
	CKvString &in_filename,
	CKvMatrixIntRgb *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());

	int **pr, **pg, **pb;
	int ww, hh;

	out_image->ms(ww, hh);
	pr = out_image->mp(pg, pb);

	cv::Mat image = cv::Mat(hh, ww, CV_16UC3);

	unsigned int *p = (unsigned int *)(image.data);

	printf("[set]\n");
	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			//image.at<unsigned int>(j, i, 2) = (unsigned int)(pr[j][i] + 255);
			//image.at<unsigned int>(j, i, 1) = (unsigned int)(pg[j][i] + 255);
			//image.at<unsigned int>(j, i, 0) = (unsigned int)(pb[j][i] + 255);
			
			image.at<cv::Vec3s>(j,i)[2] = (unsigned int)(pr[j][i] + 255);
			image.at<cv::Vec3s>(j,i)[1] = (unsigned int)(pg[j][i] + 255);
			image.at<cv::Vec3s>(j,i)[0] = (unsigned int)(pb[j][i] + 255);

			//p[j*ww * 3 + i * 3 + 2] = pr[j][i] + 255;
			//p[j*ww * 3 + i * 3 + 1] = pg[j][i] + 255;
			//p[j*ww * 3 + i * 3]     = pb[j][i] + 255;

			//printf("j=%d/%d i=%d/%d\n", j, hh, i, ww);
			//printf("	%d %d %d -> %d %d %d\n",
			//	pr[j][i],pg[j][i],pb[j][i],
			//	p[j*ww * 3 + i * 3 + 2],
			//	p[j*ww * 3 + i * 3 + 1],
			//	p[j*ww * 3 + i * 3]);
		}
	}
	
	

	printf("[cv::imwrite]\n");
	cv::imwrite(name, image);
}
//************************************************************************
void VCL_Voxel_Color_Map::si_Save_Image_from_OpenCV(
	CKvString &in_filename,
	CKvMatrixBool *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());

	bool **pr;
	int ww, hh;

	out_image->ms(ww, hh);
	pr = out_image->mp();

	cv::Mat image = cv::Mat(hh, ww, CV_8UC1);

	unsigned char *p = image.data;

	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			if (pr[j][i])
			{
				p[j*ww + i] = 255;
			}
			else
			{
				p[j*ww + i] = 0;
			}
		}
	}

	cv::imwrite(name, image);
}
//************************************************************************
void VCL_Voxel_Color_Map::si_Save_Image_from_OpenCV(
	CKvString &in_filename,
	CKvMatrixUchar *out_image)
//************************************************************************
{
	cv::String name = cv::String((char *)in_filename, in_filename.cb_Capacity_of_Buffer());

	unsigned char **pr;
	int ww, hh;

	out_image->ms(ww, hh);
	pr = out_image->mp();

	cv::Mat image = cv::Mat(hh, ww, CV_8UC1);

	unsigned char *p = image.data;

	for (int j = 0; j < hh; j++)
	{
		for (int i = 0; i < ww; i++)
		{
			p[j*ww+ i] = pr[j][i];
		}
	}

	cv::imwrite(name, image);
}
//************************************************************************









//************************************************************************
bool VCL_Voxel_Color_Map::s_gvm_Generate_Voxel_Map__All_Intra(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	int in_threshold_for_cluster_size,
	CKvSet_of_VectorInt *in_offsets,
	CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
	CKvDepot_of_MatrixBool *out_set_masks)
//************************************************************************
{
	CKvMatrixBool   *p_masks;
	CKvMatrixInt    *p_voxel_maps;
	CKvDepot_of_MatrixInt    depot_voxel_maps;
	CKvDepot_of_MatrixBool     depot_voxel_masks;
	CKvSet_of_RgbaF vc;    CKvRgbaF *rgbaf;
	CKvVectorInt               *p_offset;
	int *p_po, max_length, tmp, count_depot;
	int num_blob, num_toc, num_element, k, l, j;
	int **p_idx;
	bool **p_pm;

	p_offset = in_offsets->vps(num_blob); if (num_blob == 0) { return false; }
	depot_voxel_maps.in_Initialize(num_blob);
	depot_voxel_masks.in_Initialize(num_blob);

	p_voxel_maps = depot_voxel_maps.gps_Get_Pointer_of_Space()->vp();
	p_masks      = depot_voxel_masks.gps_Get_Pointer_of_Space()->vp();

	in_docube->gvc_Get_Voxel_Color().e_Export(&vc);
	rgbaf = vc.vp();

	count_depot = 0;
	int num_prediction_voxel = 0;
	for (k = 0; k < num_blob; k++)
	{
		max_length = -1;
		p_po = p_offset[k].vps(num_toc);
		for (l = 0; l < num_toc; l++)
		{
			tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
			if (max_length < tmp)
			{
				max_length = tmp;
			}
		}
		p_voxel_maps[count_depot].c_Create(num_toc, max_length, (int)-1);
		p_masks[count_depot].c_Create(num_toc, max_length, false);

		p_idx = p_voxel_maps[count_depot].mp();
		p_pm = p_masks[count_depot].mp();
		for (l = 0; l < num_toc; l++)
		{
			num_element = in_segmented_texture_on_code[k][l].size();

			for (j = 0; j < num_element; j++)
			{
				p_idx[l][j + p_po[l]] = (int)in_segmented_texture_on_code[k][l][j];
				if (in_segmented_texture_on_code[k][l][j] != -1)
				{
					p_pm[l][j + p_po[l]] = true;
				}
				
			}
		}
		count_depot++;
	}

	if (count_depot != 0)
	{
		depot_voxel_maps.cne_Change_Number_of_Elements(count_depot);
		depot_voxel_masks.cne_Change_Number_of_Elements(count_depot);

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}
	else
	{
		depot_voxel_maps.in_Initialize();
		depot_voxel_masks.in_Initialize();

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Map::s_gvm_Generate_Voxel_Map__Intra(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	int in_threshold_for_cluster_size,
	bool in_mode_palette_true_mode_image_false,
	CKvSet_of_VectorInt *in_offsets,
	CKvSet_of_MatrixUcharRgb *out_set_voxel_color_maps,
	CKvSet_of_MatrixBool *out_set_masks,
	std::vector<unsigned char> &out_residual_red,
	std::vector<unsigned char> &out_residual_green,
	std::vector<unsigned char> &out_residual_blue,
	std::vector<int> &out_residual_idx)
//************************************************************************
{
	CKvVectorUcharRgb residual_color_vec;
	CKvMatrixUcharRgb residual_color_map;
	CKvMatrixBool     residual_mask;

	std::vector<unsigned char> residual_red, residual_green, residual_blue;
	std::vector<int> residual_idx;

	CKvMatrixUcharRgb *p_voxel_maps;
	CKvMatrixBool *p_masks;
	CKvDepot_of_MatrixUcharRgb depot_voxel_maps;
	CKvDepot_of_MatrixBool depot_voxel_masks;
	CKvDepot_of_RgbaF voxel_colors; CKvSet_of_RgbaF vc;
	CKvRgbaF *rgbaf;
	CKvVectorInt *p_offset, num_element_blob;
	int *p_po, *p_neb, max_length, tmp, count_depot;
	unsigned char **p_r, **p_g, **p_b;
	bool **p_pm;
	int num_blob, num_toc, num_element, k, l, j;


	p_offset = in_offsets->vps(num_blob);
	depot_voxel_maps.in_Initialize(num_blob);
	depot_voxel_masks.in_Initialize(num_blob);

	p_voxel_maps = depot_voxel_maps.gps_Get_Pointer_of_Space()->vp();
	p_masks = depot_voxel_masks.gps_Get_Pointer_of_Space()->vp();
	p_neb = num_element_blob.c_Create(num_blob);
	voxel_colors = in_docube->gvc_Get_Voxel_Color();

	voxel_colors.e_Export(&vc); voxel_colors.in_Initialize();
	rgbaf = vc.vp();

	for (k = 0; k < num_blob; k++)
	{
		p_neb[k] = neb_The_Number_of_Elements_In_Blob(in_segmented_texture_on_code[k]);
	}

	count_depot = 0;
	for (k = 0; k < num_blob; k++)
	{
		max_length = -1;

		//if (in_segmented_texture_on_code[k].size() != 0 &&
		//	(in_threshold_for_cluster_size < p_neb[k]) || (in_threshold_for_cluster_size == p_neb[k]))
		if ((in_threshold_for_cluster_size < p_neb[k]) ||
			(in_threshold_for_cluster_size == p_neb[k]))
		{
			p_po = p_offset[k].vps(num_toc);

			printf("k=%d/%d)) -> %d element , num_toc = %d\n", k, num_blob, p_neb[k], num_toc);
			for (l = 0; l < num_toc; l++)
			{
				tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
				if (max_length < tmp)
				{
					max_length = tmp;
				}
			}
			p_voxel_maps[count_depot].c_Create(num_toc, max_length,
				Kv_Rgb((unsigned char)0, (unsigned char)0, (unsigned char)255));
			p_masks[count_depot].c_Create(num_toc, max_length, false);

			p_r  = p_voxel_maps[count_depot].mp(p_g, p_b);
			p_pm = p_masks[count_depot].mp();
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();

				for (j = 0; j < num_element; j++)
				{
					tmp = in_segmented_texture_on_code[k][l][j];
					p_r[l][j + p_po[l]] = (unsigned char)(rgbaf[tmp].r * 255.f);
					p_g[l][j + p_po[l]] = (unsigned char)(rgbaf[tmp].g * 255.f);
					p_b[l][j + p_po[l]] = (unsigned char)(rgbaf[tmp].b * 255.f);
					p_pm[l][j + p_po[l]] = true;
				}
			}
			count_depot++;
		}
		else
		{
			p_po = p_offset[k].vps(num_toc);
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();

				for (j = 0; j < num_element; j++)
				{
					tmp = in_segmented_texture_on_code[k][l][j];
					out_residual_red.push_back((unsigned char)(rgbaf[tmp].r * 255.f));
					out_residual_green.push_back((unsigned char)(rgbaf[tmp].g * 255.f));
					out_residual_blue.push_back((unsigned char)(rgbaf[tmp].b * 255.f));
					out_residual_idx.push_back(tmp);
				}
			}
		}
	}
	if (count_depot != 0)
	{
		depot_voxel_maps.cne_Change_Number_of_Elements(count_depot);
		depot_voxel_masks.cne_Change_Number_of_Elements(count_depot);

		depot_voxel_maps.ex_Export(out_set_voxel_color_maps);
		depot_voxel_masks.ex_Export(out_set_masks);
		
		return true;
	}

	return false;
}
//************************************************************************
bool VCL_Voxel_Color_Map::s_gvm_Generate_Voxel_Map__Intra_Hybrid_Coding(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	int in_threshold_for_cluster_size,
	bool in_mode_palette_true_mode_image_false,
	CKvSet_of_VectorInt *in_offsets,
	CKvDepot_of_MatrixUcharRgb *out_set_voxel_color_maps,
	CKvDepot_of_MatrixBool *out_set_masks,
	CKvDepot_of_Point3Df *out_voxel_dust_pt,
	CKvDepot_of_RgbaF *out_voxel_dust_color)
//************************************************************************
{
	CKvMatrixUcharRgb *p_voxel_maps;
	CKvMatrixBool     *p_masks;
	CKvDepot_of_MatrixUcharRgb depot_voxel_maps;
	CKvDepot_of_MatrixBool     depot_voxel_masks;
	CKvDepot_of_Point3Df       depot_voxel;       CKvSet_of_Point3Df dv; CKvPoint3Df *a_voxel;
	CKvDepot_of_RgbaF          voxel_colors;      CKvSet_of_RgbaF vc;    CKvRgbaF *rgbaf;
	CKvVectorInt               *p_offset, num_element_blob;
	int *p_po, *p_neb, max_length, tmp, count_depot, count_vd, dum;
	int num_blob, num_toc, num_element, k, l, j;
	unsigned char **p_r, **p_g, **p_b;
	bool **p_pm;

	p_offset = in_offsets->vps(num_blob);
	depot_voxel_maps.in_Initialize(num_blob);
	depot_voxel_masks.in_Initialize(num_blob);

	p_voxel_maps = depot_voxel_maps.gps_Get_Pointer_of_Space()->vp();
	p_masks      = depot_voxel_masks.gps_Get_Pointer_of_Space()->vp();
	p_neb        = num_element_blob.c_Create(num_blob);
	count_depot  = voxel_colors.ne_Number_of_Elements();

	in_docube->gvc_Get_Voxel_Color().e_Export(&vc);
	in_docube->gsp_Get_Surface_Points().e_Export(&dv);
	
	out_voxel_dust_pt->in_Initialize(dv.vs());
	out_voxel_dust_color->in_Initialize(dv.vs());

	a_voxel = dv.vp();
	rgbaf   = vc.vp();

	if (num_blob == 0){ return false; }

	for (k = 0; k < num_blob; k++)
	{
		p_neb[k] = neb_The_Number_of_Elements_In_Blob(in_segmented_texture_on_code[k]);
	}

	count_depot = count_vd = 0;
	for (k = 0; k < num_blob; k++)
	{
		max_length = -1;

		//if (in_segmented_texture_on_code[k].size() != 0 &&
		//	(in_threshold_for_cluster_size < p_neb[k]) || (in_threshold_for_cluster_size == p_neb[k]))
		if ((in_threshold_for_cluster_size < p_neb[k]) ||
			(in_threshold_for_cluster_size == p_neb[k]))
		{
			p_po = p_offset[k].vps(num_toc);

			//printf("	k=%d/%d)) -> %d element , num_toc = %d\n", k, num_blob, p_neb[k], num_toc);
			for (l = 0; l < num_toc; l++)
			{
				tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
				if (max_length < tmp)
				{
					max_length = tmp;
				}
			}
			p_voxel_maps[count_depot].c_Create(num_toc, max_length,
				Kv_Rgb((unsigned char)0, (unsigned char)0, (unsigned char)0));
			p_masks[count_depot].c_Create(num_toc, max_length, false);

			p_r  = p_voxel_maps[count_depot].mp(p_g, p_b);
			p_pm = p_masks[count_depot].mp();
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();

				for (j = 0; j < num_element; j++)
				{
					tmp = in_segmented_texture_on_code[k][l][j];
					p_r[l][j + p_po[l]]  = (unsigned char)(rgbaf[tmp].r * 255.f);
					p_g[l][j + p_po[l]]  = (unsigned char)(rgbaf[tmp].g * 255.f);
					p_b[l][j + p_po[l]]  = (unsigned char)(rgbaf[tmp].b * 255.f);
					p_pm[l][j + p_po[l]] = true;
				}
			}
			count_depot++;
		}
		else
		{
			num_toc = p_offset[k].vs();
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();

				for (j = 0; j < num_element; j++)
				{
					tmp = in_segmented_texture_on_code[k][l][j];
					out_voxel_dust_pt->ap_Append(false, a_voxel[tmp], dum);
					out_voxel_dust_color->ap_Append(false, rgbaf[tmp], dum);
					count_vd++;
				}
			}
		}
	}

	if (count_depot != 0)
	{
		depot_voxel_maps.cne_Change_Number_of_Elements(count_depot);
		depot_voxel_masks.cne_Change_Number_of_Elements(count_depot);

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}
	else
	{
		depot_voxel_maps.in_Initialize();
		depot_voxel_masks.in_Initialize();

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}

	if (count_vd != 0)
	{
		out_voxel_dust_pt->cne_Change_Number_of_Elements(count_vd);
		out_voxel_dust_color->cne_Change_Number_of_Elements(count_vd);
	}
	else
	{
		out_voxel_dust_pt->in_Initialize();
		out_voxel_dust_color->in_Initialize();
	}
	
	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Map::s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	int in_threshold_for_cluster_size,
	CKvSet_of_VectorInt *in_offsets,
	CKvDepot_of_MatrixUcharRgb *out_set_voxel_color_maps,
	CKvDepot_of_MatrixBool *out_set_masks,
	CKvDepot_of_MatrixInt *out_set_voxel_idx_for_prediction,
	CKvDepot_of_MatrixBool *out_set_masks_for_prediction)
//************************************************************************
{
	CKvMatrixUcharRgb *p_voxel_maps;
	CKvMatrixBool     *p_masks, *p_masks_pred;
	CKvMatrixInt    *p_idx_map;
	CKvDepot_of_MatrixUcharRgb depot_voxel_maps;
	CKvDepot_of_MatrixBool     depot_voxel_masks;
	CKvDepot_of_MatrixInt    depot_voxel_idx;
	CKvDepot_of_MatrixBool     depot_voxel_idx_masks;
	CKvDepot_of_RgbaF          voxel_colors;      CKvSet_of_RgbaF vc;    CKvRgbaF *rgbaf;
	CKvVectorInt               *p_offset, num_element_blob;
	int *p_po, *p_neb, max_length, tmp, count_depot, count_pv;
	int num_blob, num_toc, num_element, k, l, j;
	unsigned char **p_r, **p_g, **p_b;
	bool **p_pm; int **p_im;

	p_offset = in_offsets->vps(num_blob); if (num_blob == 0) { return false; }
	depot_voxel_maps.in_Initialize(num_blob);
	depot_voxel_masks.in_Initialize(num_blob);
	depot_voxel_idx.in_Initialize(num_blob);
	depot_voxel_idx_masks.in_Initialize(num_blob);

	p_voxel_maps = depot_voxel_maps.gps_Get_Pointer_of_Space()->vp();
	p_masks      = depot_voxel_masks.gps_Get_Pointer_of_Space()->vp();
	p_idx_map    = depot_voxel_idx.gps_Get_Pointer_of_Space()->vp();
	p_masks_pred = depot_voxel_idx_masks.gps_Get_Pointer_of_Space()->vp();
	p_neb        = num_element_blob.c_Create(num_blob);
	count_depot  = voxel_colors.ne_Number_of_Elements();

	in_docube->gvc_Get_Voxel_Color().e_Export(&vc);
	rgbaf = vc.vp();


	for (k = 0; k < num_blob; k++)
	{
		p_neb[k] = neb_The_Number_of_Elements_In_Blob(in_segmented_texture_on_code[k]);
	}

	count_depot = count_pv = 0;
	int num_prediction_voxel = 0;
	for (k = 0; k < num_blob; k++)
	{
		max_length = -1;
		if ((in_threshold_for_cluster_size < p_neb[k]) ||
			(in_threshold_for_cluster_size == p_neb[k]))
		{
			p_po = p_offset[k].vps(num_toc);
			for (l = 0; l < num_toc; l++)
			{
				tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
				if (max_length < tmp)
				{
					max_length = tmp;
				}
			}
			p_voxel_maps[count_depot].c_Create(num_toc, max_length,
				Kv_Rgb((unsigned char)0, (unsigned char)0, (unsigned char)0));
			p_masks[count_depot].c_Create(num_toc, max_length, false);

			p_r  = p_voxel_maps[count_depot].mp(p_g, p_b);
			p_pm = p_masks[count_depot].mp();
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();

				for (j = 0; j < num_element; j++)
				{
					tmp = in_segmented_texture_on_code[k][l][j];
					p_r[l][j + p_po[l]] = (unsigned char)(round(rgbaf[tmp].r * 255.f));
					p_g[l][j + p_po[l]] = (unsigned char)(round(rgbaf[tmp].g * 255.f));
					p_b[l][j + p_po[l]] = (unsigned char)(round(rgbaf[tmp].b * 255.f));
					p_pm[l][j + p_po[l]] = true;
				}
			}
			count_depot++;
		}
		else
		{
			p_po = p_offset[k].vps(num_toc);
			for (l = 0; l < num_toc; l++)
			{
				tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
				if (max_length < tmp)
				{
					max_length = tmp;
				}
			}
			p_idx_map[count_pv].c_Create(num_toc, max_length, -1);
			p_masks_pred[count_pv].c_Create(num_toc, max_length, false);
			p_im = p_idx_map[count_pv].mp();
			p_pm = p_masks_pred[count_pv].mp();
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();
				num_prediction_voxel += num_element;
				for (j = 0; j < num_element; j++)
				{
					p_im[l][j + p_po[l]] = in_segmented_texture_on_code[k][l][j];
					p_pm[l][j + p_po[l]] = true;
				}
			}
			count_pv++;
		}
	}

	printf("	Number of Voxels to be predicted : %d\n", num_prediction_voxel);

	if (count_depot != 0)
	{
		depot_voxel_maps.cne_Change_Number_of_Elements(count_depot);
		depot_voxel_masks.cne_Change_Number_of_Elements(count_depot);

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}
	else
	{
		depot_voxel_maps.in_Initialize();
		depot_voxel_masks.in_Initialize();

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}

	if (count_pv != 0)
	{
		depot_voxel_idx.cne_Change_Number_of_Elements(count_pv);
		depot_voxel_idx_masks.cne_Change_Number_of_Elements(count_pv);

		out_set_voxel_idx_for_prediction->cp_Copy(&depot_voxel_idx);
		out_set_masks_for_prediction->cp_Copy(&depot_voxel_idx_masks);
	}
	else
	{
		depot_voxel_idx.in_Initialize();
		depot_voxel_idx_masks.in_Initialize();

		out_set_voxel_idx_for_prediction->cp_Copy(&depot_voxel_idx);
		out_set_masks_for_prediction->cp_Copy(&depot_voxel_idx_masks);
	}

	return true;
}
//************************************************************************
bool VCL_Voxel_Color_Map::s_gvm_Generate_Voxel_Map__Intra_Prediction_Coding(
	VCL_DoCube_X_Color *in_docube,
	std::vector<std::vector<std::vector<int>>> &in_segmented_texture_on_code,
	int in_threshold_for_cluster_size,
	CKvSet_of_VectorInt *in_offsets,
	CKvDepot_of_MatrixInt *out_set_voxel_color_maps,
	CKvDepot_of_MatrixBool *out_set_masks,
	CKvDepot_of_MatrixInt *out_set_voxel_idx_for_prediction,
	CKvDepot_of_MatrixBool *out_set_masks_for_prediction)
//************************************************************************
{
	CKvMatrixBool     *p_masks, *p_masks_pred;
	CKvMatrixInt    *p_idx_map, *p_voxel_maps;;
	CKvDepot_of_MatrixInt    depot_voxel_maps;
	CKvDepot_of_MatrixBool     depot_voxel_masks;
	CKvDepot_of_MatrixInt    depot_voxel_idx;
	CKvDepot_of_MatrixBool     depot_voxel_idx_masks;
	CKvDepot_of_RgbaF          voxel_colors;      CKvSet_of_RgbaF vc;    CKvRgbaF *rgbaf;
	CKvVectorInt               *p_offset, num_element_blob;
	int *p_po, *p_neb, max_length, tmp, count_depot, count_pv;
	int num_blob, num_toc, num_element, k, l, j;

	int **p_idx;

	bool **p_pm; int **p_im;

	p_offset = in_offsets->vps(num_blob); if (num_blob == 0) { return false; }
	depot_voxel_maps.in_Initialize(num_blob);
	depot_voxel_masks.in_Initialize(num_blob);
	depot_voxel_idx.in_Initialize(num_blob);
	depot_voxel_idx_masks.in_Initialize(num_blob);

	p_voxel_maps = depot_voxel_maps.gps_Get_Pointer_of_Space()->vp();
	p_masks      = depot_voxel_masks.gps_Get_Pointer_of_Space()->vp();
	p_idx_map    = depot_voxel_idx.gps_Get_Pointer_of_Space()->vp();
	p_masks_pred = depot_voxel_idx_masks.gps_Get_Pointer_of_Space()->vp();
	p_neb        = num_element_blob.c_Create(num_blob);
	count_depot  = voxel_colors.ne_Number_of_Elements();

	in_docube->gvc_Get_Voxel_Color().e_Export(&vc);
	rgbaf = vc.vp();


	for (k = 0; k < num_blob; k++)
	{
		p_neb[k] = neb_The_Number_of_Elements_In_Blob(in_segmented_texture_on_code[k]);
	}

	count_depot = count_pv = 0;
	int num_prediction_voxel = 0;
	for (k = 0; k < num_blob; k++)
	{
		max_length = -1;
		if ((in_threshold_for_cluster_size < p_neb[k]) ||
			(in_threshold_for_cluster_size == p_neb[k]))
		{
			p_po = p_offset[k].vps(num_toc);
			for (l = 0; l < num_toc; l++)
			{
				tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
				if (max_length < tmp)
				{
					max_length = tmp;
				}
			}
			p_voxel_maps[count_depot].c_Create(num_toc, max_length, (int)-1);
			p_masks[count_depot].c_Create(num_toc, max_length, false);

			//printf("k=%d/%d) hh=%d ww=%d\n", k, num_blob, num_toc, max_length);
			p_idx = p_voxel_maps[count_depot].mp();
			p_pm = p_masks[count_depot].mp();
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();

				for (j = 0; j < num_element; j++)
				{
					p_idx[l][j + p_po[l]] = (int)in_segmented_texture_on_code[k][l][j];
					if ((int)in_segmented_texture_on_code[k][l][j] != -1)
					{
						p_pm[l][j + p_po[l]] = true;
					}
				
				}
			}
			count_depot++;
		}
		else
		{
			p_po = p_offset[k].vps(num_toc);
			for (l = 0; l < num_toc; l++)
			{
				tmp = in_segmented_texture_on_code[k][l].size() + p_po[l];
				if (max_length < tmp)
				{
					max_length = tmp;
				}
			}
			p_idx_map[count_pv].c_Create(num_toc, max_length, -1);
			p_masks_pred[count_pv].c_Create(num_toc, max_length, false);
			p_im = p_idx_map[count_pv].mp();
			p_pm = p_masks_pred[count_pv].mp();
			for (l = 0; l < num_toc; l++)
			{
				num_element = in_segmented_texture_on_code[k][l].size();
				num_prediction_voxel += num_element;
				for (j = 0; j < num_element; j++)
				{
					p_im[l][j + p_po[l]] = in_segmented_texture_on_code[k][l][j];
					if ((int)in_segmented_texture_on_code[k][l][j] != -1)
					{
						p_pm[l][j + p_po[l]] = true;
					}
					
				}
			}
			count_pv++;
		}
	}

	//printf("	Number of Voxels to be predicted : %d\n", num_prediction_voxel);

	if (count_depot != 0)
	{
		depot_voxel_maps.cne_Change_Number_of_Elements(count_depot);
		depot_voxel_masks.cne_Change_Number_of_Elements(count_depot);

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}
	else
	{
		depot_voxel_maps.in_Initialize();
		depot_voxel_masks.in_Initialize();

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}

	if (count_pv != 0)
	{
		depot_voxel_idx.cne_Change_Number_of_Elements(count_pv);
		depot_voxel_idx_masks.cne_Change_Number_of_Elements(count_pv);

		out_set_voxel_idx_for_prediction->cp_Copy(&depot_voxel_idx);
		out_set_masks_for_prediction->cp_Copy(&depot_voxel_idx_masks);
	}
	else
	{
		depot_voxel_idx.in_Initialize();
		depot_voxel_idx_masks.in_Initialize();

		out_set_voxel_idx_for_prediction->cp_Copy(&depot_voxel_idx);
		out_set_masks_for_prediction->cp_Copy(&depot_voxel_idx_masks);
	}

	return true;
}
//************************************************************************
void VCL_Voxel_Color_Map::mvm_Merge_Voxel_Map(
	CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixUcharRgb *p_voxel_maps;
	CKvMatrixBool *p_mask;
	int **p_position;
	int wwl, hhl, ww, hh;
	int num_blob, k;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask       = in_set_masks->vp();
	p_position   = out_position_of_segmented_voxel_map->c_Create(num_blob, 2, (int)0);

	wwl = 0; hhl = -1;

	for (k = 0; k < num_blob; k++)
	{
		p_voxel_maps[k].ms(ww, hh);
		if (hhl < hh){ hhl = hh; }
		wwl += ww;

		if (k != (num_blob - 1))
		{
			p_position[k + 1][0] = 0;                     // y-coordinates. 
			p_position[k + 1][1] = p_position[k][1] + ww; // x-coordinates.
		}
	}

	out_voxel_map->c_Create(hhl, wwl, Kv_Rgb(0,255,0));
	out_mask->c_Create(hhl, wwl,false);

	for (k = 0; k < num_blob; k++)
	{
		out_voxel_map->sb_Set_Block(
			p_position[k][1],
			p_position[k][0],
			&p_voxel_maps[k]);

		out_mask->sb_Set_Block(
			p_position[k][1],
			p_position[k][0],
			&p_mask[k]);
	}

}
//************************************************************************
void VCL_Voxel_Color_Map::gsci_Get_Shape_Coding_Image_EI(
	CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_voxel_map_masks,
	int &in_block_x,
	int &in_block_y,
	CKvMatrixUcharRgb *out_EI_image,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvDepot_of_MatrixUcharRgb all_depot_block_imgs, depot_block_imgs;
	CKvDepot_of_MatrixBool all_depot_block_masks, depot_block_masks;
	CKvMatrixUcharRgb *p_voxel_map;
	CKvMatrixBool *p_voxel_map_mask;
	int num_segment, k;

	p_voxel_map      = in_set_voxel_maps->vps(num_segment);
	p_voxel_map_mask = in_set_voxel_map_masks->vp();

	all_depot_block_imgs.in_Initialize(1);
	all_depot_block_masks.in_Initialize(1);
	for (k = 0; k < num_segment; k++)
	{
		gbiei_Get_Block_Images_EI(
			&p_voxel_map[k],//CKvMatrixUcharRgb *in_img,
			&p_voxel_map_mask[k],//CKvMatrixBool *in_mask,
			in_block_x,//int &in_block_x,
			in_block_y,//int &in_block_y,
			&depot_block_imgs,//CKvDepot_of_MatrixUcharRgb *out_block_imgs,
			&depot_block_masks);//CKvDepot_of_MatrixBool *out_block_masks)

		all_depot_block_imgs.ap_Append(false, &depot_block_imgs, NULL, NULL);
		all_depot_block_masks.ap_Append(false, &depot_block_masks, NULL, NULL);
	}

	cvm_Convert_Voxel_Map(
		&all_depot_block_imgs,//CKvDepot_of_MatrixUcharRgb *in_block_imgs,
		&all_depot_block_masks,//CKvDepot_of_MatrixBool *in_block_masks,
		out_EI_image,//CKvMatrixUcharRgb *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)
}
//************************************************************************
void VCL_Voxel_Color_Map::gsci_Get_Shape_Coding_Image_EI(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_voxel_map_masks,
	int &in_block_x,
	int &in_block_y,
	CKvMatrixInt *out_EI_image,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvDepot_of_MatrixInt all_depot_block_imgs, depot_block_imgs;
	CKvDepot_of_MatrixBool all_depot_block_masks, depot_block_masks;
	CKvMatrixInt *p_voxel_map;
	CKvMatrixBool *p_voxel_map_mask;
	int num_segment, num_count, k;

	p_voxel_map = in_set_voxel_maps->vps(num_segment);
	p_voxel_map_mask = in_set_voxel_map_masks->vp();

	all_depot_block_imgs.in_Initialize(num_segment*1000);
	all_depot_block_masks.in_Initialize(num_segment*1000);
	num_count = 0;
	for (k = 0; k < num_segment; k++)
	{
		gbiei_Get_Block_Images_Index(
			&p_voxel_map[k],//CKvMatrixInt *in_img,
			&p_voxel_map_mask[k],//CKvMatrixBool *in_mask,
			in_block_x,//int &in_block_x,
			in_block_y,//int &in_block_y,
			&depot_block_imgs,//CKvDepot_of_MatrixInt *out_block_imgs,
			&depot_block_masks);//CKvDepot_of_MatrixBool *out_block_masks)

		num_count += depot_block_imgs.ne_Number_of_Elements();

		all_depot_block_imgs.ap_Append(false, &depot_block_imgs, NULL, NULL);
		all_depot_block_masks.ap_Append(false, &depot_block_masks, NULL, NULL);
	}

	if (num_count != 0)
	{
		all_depot_block_imgs.cne_Change_Number_of_Elements(num_count);
		all_depot_block_masks.cne_Change_Number_of_Elements(num_count);

		cvm_Convert_Voxel_Map_Index(
			&all_depot_block_imgs,//CKvDepot_of_MatrixUcharRgb *in_block_imgs,
			&all_depot_block_masks,//CKvDepot_of_MatrixBool *in_block_masks,
			out_EI_image,//CKvMatrixUcharRgb *out_voxel_map,
			out_mask);//CKvMatrixBool *out_mask)
	}
	else
	{
		printf("Error -> gsci_Get_Shape_Coding_Image_EI\n");
		return;
	}

}
//************************************************************************
void VCL_Voxel_Color_Map::gbiei_Get_Block_Images_EI(
	CKvMatrixUcharRgb *in_img,
	CKvMatrixBool *in_mask,
	int &in_block_x,
	int &in_block_y,
	CKvDepot_of_MatrixUcharRgb *out_block_imgs,
	CKvDepot_of_MatrixBool *out_block_masks)
//************************************************************************
{
	CKvMatrixUcharRgb color_images, block_img;
	CKvMatrixBool block_mask, mask;
	double lww, lhh;
	int ww, hh, j, i, num_blocks;

	in_img->ms(ww, hh);

	lww = (double)ww / (double)in_block_x;
	lhh = (double)hh / (double)in_block_y;

	lww = ceil(lww);
	lhh = ceil(lhh);

	ww = lww * in_block_x;
	hh = lhh * in_block_y;

	color_images.c_Create(hh, ww, Kv_Rgb(0, 0, 0)); mask.c_Create(hh, ww, false);
	color_images.sb_Set_Block(0, 0, in_img);        mask.sb_Set_Block(0, 0, in_mask);
	in_img->cp_Copy(&color_images);                 in_mask->cp_Copy(&mask);
	color_images.c_Create(1, 1);                    mask.c_Create(1, 1);

	out_block_imgs->in_Initialize(lww*lhh);
	out_block_masks->in_Initialize(lww*lhh);
	num_blocks = 0;

	for (j = 0; j < hh; j += in_block_y)
	{
		for (i = 0; i < ww; i += in_block_x)
		{
			in_img->gb_Get_Block(i, j, in_block_x, in_block_y, &block_img);
			in_mask->gb_Get_Block(i, j, in_block_x, in_block_y, &block_mask);
			if (EI_DCT_Forward(&block_img, &block_mask))
			{
				out_block_imgs->ap_Append(false, &block_img, NULL);
				out_block_masks->ap_Append(false, &block_mask, NULL);
				num_blocks++;
			}
		}
	}

	out_block_imgs->cne_Change_Number_of_Elements(num_blocks);
	out_block_masks->cne_Change_Number_of_Elements(num_blocks);
}
//************************************************************************
void VCL_Voxel_Color_Map::gbiei_Get_Block_Images_Index(
	CKvMatrixInt *in_img,
	CKvMatrixBool *in_mask,
	int &in_block_x,
	int &in_block_y,
	CKvDepot_of_MatrixInt *out_block_imgs,
	CKvDepot_of_MatrixBool *out_block_masks)
//************************************************************************
{
	CKvMatrixInt color_images, block_img;
	CKvMatrixBool block_mask, mask;
	double lww, lhh;
	int ww, hh, j, i, num_blocks;

	in_img->ms(ww, hh);

	lww = (double)ww / (double)in_block_x;
	lhh = (double)hh / (double)in_block_y;

	lww = ceil(lww);
	lhh = ceil(lhh);

	ww = lww * in_block_x;
	hh = lhh * in_block_y;

	color_images.c_Create(hh, ww, (int)-1); mask.c_Create(hh, ww, false);
	color_images.sb_Set_Block(0, 0, in_img);        mask.sb_Set_Block(0, 0, in_mask);
	in_img->cp_Copy(&color_images);                 in_mask->cp_Copy(&mask);
	color_images.c_Create(1, 1);                    mask.c_Create(1, 1);

	out_block_imgs->in_Initialize(lww*lhh);
	out_block_masks->in_Initialize(lww*lhh);
	num_blocks = 0;

	for (j = 0; j < hh; j += in_block_y)
	{
		for (i = 0; i < ww; i += in_block_x)
		{
			in_img->gb_Get_Block(i, j, in_block_x, in_block_y, &block_img);
			in_mask->gb_Get_Block(i, j, in_block_x, in_block_y, &block_mask);
			if (ie_Is_Empty(&block_mask))
			{
				out_block_imgs->ap_Append(false, &block_img, NULL);
				out_block_masks->ap_Append(false, &block_mask, NULL);
				num_blocks++;
			}
		}
	}

	out_block_imgs->cne_Change_Number_of_Elements(num_blocks);
	out_block_masks->cne_Change_Number_of_Elements(num_blocks);
}
//************************************************************************
//void VCL_Voxel_Color_Map::gbiei_Get_Block_Images_Index(
//	CKvMatrixInt *in_img,
//	CKvMatrixBool *in_mask,
//	int &in_block_x,
//	int &in_block_y,
//	CKvDepot_of_MatrixInt *out_block_imgs,
//	CKvDepot_of_MatrixBool *out_block_masks)
////************************************************************************
//{
//	CKvMatrixInt color_images, block_img;
//	CKvMatrixBool block_mask, mask;
//	double lww, lhh;
//	int ww, hh, j, i, num_blocks;
//
//	in_img->ms(ww, hh);
//
//	lww = (double)ww / (double)in_block_x;
//	lhh = (double)hh / (double)in_block_y;
//
//	lww = ceil(lww);
//	lhh = ceil(lhh);
//
//	ww = lww * in_block_x;
//	hh = lhh * in_block_y;
//
//	color_images.c_Create(hh, ww, (int)-1); mask.c_Create(hh, ww, false);
//	color_images.sb_Set_Block(0, 0, in_img);mask.sb_Set_Block(0, 0, in_mask);
//	in_img->cp_Copy(&color_images);         in_mask->cp_Copy(&mask);
//	color_images.c_Create(1, 1);            mask.c_Create(1, 1);
//
//	out_block_imgs->in_Initialize(lww*lhh);
//	out_block_masks->in_Initialize(lww*lhh);
//	num_blocks = 0;
//
//	for (j = 0; j < hh; j += in_block_y)
//	{
//		for (i = 0; i < ww; i += in_block_x)
//		{
//			in_img->gb_Get_Block(i, j, in_block_x, in_block_y, &block_img);
//			in_mask->gb_Get_Block(i, j, in_block_x, in_block_y, &block_mask);
//			if (ie_Is_Empty(&block_mask))
//			{
//				out_block_imgs->ap_Append(false, &block_img, NULL);
//				out_block_masks->ap_Append(false, &block_mask, NULL);
//				num_blocks++;
//			}
//		}
//	}
//
//	out_block_imgs->cne_Change_Number_of_Elements(num_blocks);
//	out_block_masks->cne_Change_Number_of_Elements(num_blocks);
//}
//************************************************************************
void VCL_Voxel_Color_Map::cvm_Convert_Voxel_Map(
	CKvDepot_of_MatrixUcharRgb *in_block_imgs,
	CKvDepot_of_MatrixBool *in_block_masks,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvSet_of_MatrixUcharRgb *p_set_block_imgs;
	CKvSet_of_MatrixBool *p_set_block_masks;
	CKvMatrixUcharRgb *p_block_img;
	CKvMatrixBool *p_block_mask;
	int num_blocks, ww, hh, block_ww, block_hh, j, i, k;

	num_blocks = in_block_imgs->ne_Number_of_Elements();

	p_set_block_imgs  = in_block_imgs->gps_Get_Pointer_of_Space();
	p_set_block_masks = in_block_masks->gps_Get_Pointer_of_Space();
	
	p_block_img  = p_set_block_imgs->vp();
	p_block_mask = p_set_block_masks->vp();

	p_block_img[0].ms(block_ww, block_hh);

	ww = hh = (int)(sqrt((double)num_blocks) + 1.0);

	out_voxel_map->c_Create(hh*block_hh, ww*block_ww, Kv_Rgb(0, 0, 0));
	out_mask->c_Create(hh*block_hh, ww*block_ww, false);

	k = 0;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (k >= num_blocks){ continue; }

			out_voxel_map->sb_Set_Block(i*block_ww, j*block_hh, &p_block_img[k]);
			out_mask->sb_Set_Block(i*block_ww, j*block_hh, &p_block_mask[k]);
			k++;
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::cvm_Convert_Voxel_Map_Index(
	CKvDepot_of_MatrixInt *in_block_imgs,
	CKvDepot_of_MatrixBool *in_block_masks,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvSet_of_MatrixInt *p_set_block_imgs;
	CKvSet_of_MatrixBool *p_set_block_masks;
	CKvMatrixInt *p_block_img;
	CKvMatrixBool *p_block_mask;
	int num_blocks, ww, hh, block_ww, block_hh, j, i, k;

	num_blocks = in_block_imgs->ne_Number_of_Elements();

	p_set_block_imgs = in_block_imgs->gps_Get_Pointer_of_Space();
	p_set_block_masks = in_block_masks->gps_Get_Pointer_of_Space();

	p_block_img = p_set_block_imgs->vp();
	p_block_mask = p_set_block_masks->vp();

	p_block_img[0].ms(block_ww, block_hh);

	ww = hh = (int)(sqrt((double)num_blocks) + 1.0);

	out_voxel_map->c_Create(hh*block_hh, ww*block_ww, -1);
	out_mask->c_Create(hh*block_hh, ww*block_ww, false);

	k = 0;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (k >= num_blocks){ continue; }

			out_voxel_map->sb_Set_Block(i*block_ww, j*block_hh, &p_block_img[k]);
			out_mask->sb_Set_Block(i*block_ww, j*block_hh, &p_block_mask[k]);
			k++;
		}
	}
}
//************************************************************************
//void VCL_Voxel_Color_Map::cvm_Convert_Voxel_Map_Index(
//	CKvDepot_of_MatrixInt *in_block_imgs,
//	CKvDepot_of_MatrixBool *in_block_masks,
//	CKvMatrixInt *out_voxel_map,
//	CKvMatrixBool *out_mask)
////************************************************************************
//{
//	CKvSet_of_MatrixInt *p_set_block_imgs;
//	CKvSet_of_MatrixBool *p_set_block_masks;
//	CKvMatrixInt *p_block_img;
//	CKvMatrixBool *p_block_mask;
//	int num_blocks, ww, hh, block_ww, block_hh, j, i, k;
//
//	num_blocks = in_block_imgs->ne_Number_of_Elements();
//
//	p_set_block_imgs = in_block_imgs->gps_Get_Pointer_of_Space();
//	p_set_block_masks = in_block_masks->gps_Get_Pointer_of_Space();
//
//	p_block_img = p_set_block_imgs->vp();
//	p_block_mask = p_set_block_masks->vp();
//
//	p_block_img[0].ms(block_ww, block_hh);
//
//	ww = hh = (int)(sqrt((double)num_blocks) + 1.0);
//
//	out_voxel_map->c_Create(hh*block_hh, ww*block_ww, -1);
//	out_mask->c_Create(hh*block_hh, ww*block_ww, false);
//
//	k = 0;
//	for (j = 0; j < hh; j++)
//	{
//		for (i = 0; i < ww; i++)
//		{
//			if (k >= num_blocks) { continue; }
//
//			out_voxel_map->sb_Set_Block(i*block_ww, j*block_hh, &p_block_img[k]);
//			out_mask->sb_Set_Block(i*block_ww, j*block_hh, &p_block_mask[k]);
//			k++;
//		}
//	}
//}
//************************************************************************
int VCL_Voxel_Color_Map::neb_The_Number_of_Elements_In_Blob(
	std::vector<std::vector<int>> &in_texture_on_code)
//************************************************************************
{
	int num, num2, k;

	num  = in_texture_on_code.size();
	if (num == 0)
	{ 
		return 0; 
	}

	num2 = 0;
	for (k = 0; k < num; k++)
	{
		num2 += in_texture_on_code[k].size();
	}

	return num2;
}
//************************************************************************
bool VCL_Voxel_Color_Map::ie_Is_Empty(
	CKvMatrixBool *in_mask)
//************************************************************************
{
	bool *p_mask; int k, sz, count=0;

	p_mask = in_mask->vps(sz);
	for (k = 0; k < sz; k++)
	{
		if (p_mask[k]){count++;}
	}

	if (count == 0){ return false; }

	return true;
}
//************************************************************************
void VCL_Voxel_Color_Map::mvm_Merge_Voxel_Map_Vertical(
	CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixUcharRgb *p_voxel_maps;
	CKvMatrixBool *p_mask;
	int **p_position;
	int wwl, hhl, ww, hh;
	int num_blob, k;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask = in_set_masks->vp();
	p_position = out_position_of_segmented_voxel_map->c_Create(num_blob, 2, (int)0);


	hhl = 0; wwl = -1;
	p_voxel_maps[0].ms(ww, hh);
	if (wwl < ww) { wwl = ww; }
	hhl += hh;

	for (k = 1; k < num_blob; k++)
	{
		p_voxel_maps[k].ms(ww, hh);

		if (wwl < ww) { wwl = ww; }
		hhl += hh;

		p_position[k][0] = p_position[k - 1][0] + p_voxel_maps[k - 1].mh(); // y-coordinates. 
		p_position[k][1] = 0; // x-coordinates.
	}

	out_voxel_map->c_Create(hhl, wwl, Kv_Rgb(0, 255, 0));
	out_mask->c_Create(hhl, wwl, false);

	for (k = 0; k < num_blob; k++)
	{
		out_voxel_map->sb_Set_Block(
			p_position[k][1],
			p_position[k][0],
			&p_voxel_maps[k]);

		out_mask->sb_Set_Block(
			p_position[k][1],
			p_position[k][0],
			&p_mask[k]);
	}

}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map(
	CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixUcharRgb *p_voxel_maps, tmp2;
	CKvMatrixBool *p_mask, tmp;
	int max_width;
	int num_blob, k;
	unsigned char **pr1, **pg1, **pb1, **pr2, **pg2, **pb2;
	bool locationFound, **p_patch, **pp_mask;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask = in_set_masks->vp();

	/*********************************************/
	//Fit image Width
	max_width = -1;
	for (k = 0; k < num_blob; k++)
	{
		if (max_width < p_voxel_maps[k].mw())
		{
			max_width = p_voxel_maps[k].mw();
		}
	}
	zz_width = max_width + zz_safeguard + 1;
	/*********************************************/
	if (zz_width < 100) { zz_width = 256; }
	zz_height = 1920;
	out_mask->c_Create(zz_height, zz_width, false);
	out_voxel_map->c_Create(zz_height, zz_width, Kv_Rgb(0, 0, 0));

	for (k = 0; k < num_blob; k++)
	{
		locationFound = false;
		zz_x = zz_y = 0;
		while (!locationFound)
		{
			for (int y = 0; y < zz_height && !locationFound; ++y)
			{
				for (int x = 0; x < zz_width && !locationFound; ++x)
				{
					zz_x = x;
					zz_y = y;
					if (checkFitPatchCanvas(
						out_mask,//CKvMatrixBool *in_mask,
						&p_mask[k],//CKvMatrixBool *in_patch_mask,
						zz_width,//int canvasStrideBlk,
						zz_height,//int canvasHeightBlk,
						zz_safeguard))//int safeguard = 0)
					{
						locationFound = true;
					}
				}
			}

			if (!locationFound)
			{
				zz_height *= 2;

				tmp.cp_Copy(out_mask);
				out_mask->c_Create(zz_height, zz_width, false);
				out_mask->sb_Set_Block(0, 0, &tmp);

				tmp2.cp_Copy(out_voxel_map);
				out_voxel_map->c_Create(zz_height, zz_width, false);
				out_voxel_map->sb_Set_Block(0, 0, &tmp2);

			}
		}

		p_patch = p_mask[k].mp();
		pp_mask = out_mask->mp();

		pr1 = out_voxel_map->mp(pg1, pb1);
		pr2 = p_voxel_maps[k].mp(pg2, pb2);

		for (int v0 = 0; v0 < p_mask[k].mh(); ++v0)
		{
			int v = zz_y + v0;
			for (int u0 = 0; u0 < p_mask[k].mw(); ++u0)
			{
				int u = zz_x + u0;
				if (p_patch[v0][u0] == true && pp_mask[v][u] == false)
				{
					pp_mask[v][u] = true;
					//p1[v][u] = p2[v0][u0];
					pr1[v][u] = pr2[v0][u0];
					pg1[v][u] = pg2[v0][u0];
					pb1[v][u] = pb2[v0][u0];
				}

				// 원본
				//if (pp_mask[v][u] || p_patch[v0][u0])
				//{
				//	pp_mask[v][u] = true;
				//	pr1[v][u] = pr2[v0][u0];
				//	pg1[v][u] = pg2[v0][u0];
				//	pb1[v][u] = pb2[v0][u0];
				//}
				//else
				//{
				//	pp_mask[v][u] = false;
				//}
			}
		}

	}
}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixInt *p_voxel_maps, tmp2;
	CKvMatrixBool *p_mask, tmp;
	int max_width, global_height, global_width;
	int num_blob, k;
	int **p1, **p2;
	bool locationFound, **p_patch, **pp_mask;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask = in_set_masks->vp();

	/*********************************************/
	//Fit image Width
	max_width = -1;
	for (k = 0; k < num_blob; k++)
	{
		if (max_width < p_voxel_maps[k].mw())
		{
			max_width = p_voxel_maps[k].mw();
		}
	}
	global_width = max_width + zz_safeguard + 1;
	/*********************************************/
	if (global_width < 100) { global_width = 256; }
	global_height = zz_height;
	
	out_mask->c_Create(global_height, global_width, false);
	out_voxel_map->c_Create(global_height, global_width, (int)-1);

	for (k = 0; k < num_blob; k++)
	{
		//printf("k=%d/%d\n", k, num_blob);
		locationFound = false;
		zz_x = zz_y = 0;
		while (!locationFound)
		{
			for (int y = 0; y < global_height && !locationFound; ++y)
			{
				for (int x = 0; x < global_width && !locationFound; ++x)
				{
					zz_x = x;
					zz_y = y;
					if (checkFitPatchCanvas(
						out_mask,//CKvMatrixBool *in_mask,
						&p_mask[k],//CKvMatrixBool *in_patch_mask,
						global_width,//int canvasStrideBlk,
						global_height,//int canvasHeightBlk,
						zz_safeguard))//int safeguard = 0)
					{
						locationFound = true;
					}
					//printf("\n");
				}
			}

			if (!locationFound)
			{
				global_height *= 2;

				tmp.cp_Copy(out_mask);
				out_mask->c_Create(global_height, global_width, false);
				out_mask->sb_Set_Block(0, 0, &tmp);

				tmp2.cp_Copy(out_voxel_map);
				out_voxel_map->c_Create(global_height, global_width, (int)-1);
				out_voxel_map->sb_Set_Block(0, 0, &tmp2);

			}
		}

		p_patch = p_mask[k].mp();
		pp_mask = out_mask->mp();

		p1 = out_voxel_map->mp();
		p2 = p_voxel_maps[k].mp();
		for (int v0 = 0; v0 < p_mask[k].mh(); ++v0)
		{
			int v = zz_y + v0;
			for (int u0 = 0; u0 < p_mask[k].mw(); ++u0)
			{
				int u = zz_x + u0;
				if (p_patch[v0][u0] == true && pp_mask[v][u] == false)
				{
					pp_mask[v][u] = true;
					p1[v][u] = p2[v0][u0];
				}

				// 원본
				//if (pp_mask[v][u] || p_patch[v0][u0])
				//{
				//	pp_mask[v][u] = true;
				//	p1[v][u] = p2[v0][u0];
				//}
				//else
				//{
				//	pp_mask[v][u] = false;
				//}
			}
		}
	}

}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map(
	CKvSet_of_MatrixIntRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixIntRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixIntRgb *p_voxel_maps, tmp2;
	CKvMatrixBool *p_mask, tmp;
	int max_width, global_width, global_height;
	int num_blob, k;
	int **pr1, **pg1, **pb1, **pr2, **pg2, **pb2;
	bool locationFound, **p_patch, **pp_mask;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask       = in_set_masks->vp();
	/*********************************************/
	//Fit image Width
	max_width = -1;
	for (k = 0; k < num_blob; k++)
	{
		if (max_width < p_voxel_maps[k].mw())
		{
			max_width = p_voxel_maps[k].mw();
		}
	}
	global_width = max_width + zz_safeguard + 1;
	if (global_width < 100) { global_width = 512; }
	global_height = zz_width;
	/*********************************************/

	out_mask->c_Create(global_height, global_width, false);
	out_voxel_map->c_Create(global_height, global_width, Kv_RgbI(0, 0, 0));

	for (k = 0; k < num_blob; k++)
	{
		locationFound = false;
		zz_x = zz_y = 0;
		while (!locationFound)
		{
			for (int y = 0; y < global_height && !locationFound; ++y)
			{
				for (int x = 0; x < global_width && !locationFound; ++x)
				{
					zz_x = x;
					zz_y = y;
					if (checkFitPatchCanvas(
						out_mask,//CKvMatrixBool *in_mask,
						&p_mask[k],//CKvMatrixBool *in_patch_mask,
						global_width,//int canvasStrideBlk,
						global_height,//int canvasHeightBlk,
						zz_safeguard))//int safeguard = 0)
					{
						locationFound = true;
					}
				}
			}

			if (!locationFound)
			{
				global_height *= 2;
				tmp.cp_Copy(out_mask);
				out_mask->c_Create(global_height, global_width, false);
				out_mask->sb_Set_Block(0, 0, &tmp);

				tmp2.cp_Copy(out_voxel_map);
				out_voxel_map->c_Create(global_height, global_width, false);
				out_voxel_map->sb_Set_Block(0, 0, &tmp2);

			}
		}

		p_patch = p_mask[k].mp();
		pp_mask = out_mask->mp();

		pr1 = out_voxel_map->mp(pg1, pb1);
		pr2 = p_voxel_maps[k].mp(pg2, pb2);
		
		for (int v0 = 0; v0 < p_mask[k].mh(); ++v0)
		{
			int v = zz_y + v0;
			for (int u0 = 0; u0 < p_mask[k].mw(); ++u0)
			{
				int u = zz_x + u0;
				if (p_patch[v0][u0] == true && pp_mask[v][u] == false)
				{
					pp_mask[v][u] = true;
					pr1[v][u] = pr2[v0][u0];
					pg1[v][u] = pg2[v0][u0];
					pb1[v][u] = pb2[v0][u0];
				}
			}
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	bool in_fixed_image,
	bool in_pack_flexible,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixInt *p_voxel_maps, tmp2;
	CKvMatrixBool *p_mask, tmp, max_mask;
	CKvMatrixBool dummy_max_mask;
	int max_width, height, width, global_height, global_width;
	int num_blob, k;
	int **p1, **p2;
	bool locationFound, **p_patch, **pp_mask;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask = in_set_masks->vp();

	/**********************************************************/
	if (in_fixed_image == false)
	{
		max_width = -1;
		for (k = 0; k < num_blob; k++)
		{
			if (max_width < p_voxel_maps[k].mw())
			{
				max_width = p_voxel_maps[k].mw();
			}
		}
		global_width = max_width + zz_safeguard + 1;
		/*********************************************/
		if (global_width < 100) { global_width = 256; }
		global_height = zz_height;
	}
	else
	{
		global_height = zz_height;
		global_width  = zz_width;
	}

	/**********************************************************/
	out_mask->c_Create(global_height, global_width, false);
	dummy_max_mask.c_Create(global_height, global_width, false);
	out_voxel_map->c_Create(global_height, global_width, (int)-1);

	int **p_position_of_blob = out_position_of_segmented_voxel_map->c_Create(num_blob, 4);
	
	//CKvScreen sc[3];

	for (k = 0; k < num_blob; k++)
	{
		p_mask[k].ms(width, height);
		
		if (in_pack_flexible == false)
		{
			max_mask.c_Create(height, width, true);
		}
		else
		{
			Set_New_Masks(
				&p_mask[k],//CKvMatrixBool *in_mask,
				&max_mask);//CKvMatrixBool *out_mask)
		}

		locationFound = false;
		zz_x = zz_y = 0;
		while (!locationFound)
		{
			for (int y = 0; y < global_height && !locationFound; ++y)
			{
				for (int x = 0; x < global_width && !locationFound; ++x)
				{
					zz_x = x;
					zz_y = y;
					if (checkFitPatchCanvas(
						&dummy_max_mask,//CKvMatrixBool *in_mask,
						&max_mask,//CKvMatrixBool *in_patch_mask,
						global_width,//int canvasStrideBlk,
						global_height,//int canvasHeightBlk,
						zz_safeguard))//int safeguard = 0)
					{
						locationFound = true;
					}
				}
			}

			if (!locationFound)
			{
				global_height *= 2;

				tmp.cp_Copy(out_mask);
				out_mask->c_Create(global_height, global_width, false);
				out_mask->sb_Set_Block(0, 0, &tmp);

				tmp.cp_Copy(&dummy_max_mask);
				dummy_max_mask.c_Create(global_height, global_width, false);
				dummy_max_mask.sb_Set_Block(0, 0, &tmp);

				tmp2.cp_Copy(out_voxel_map);
				out_voxel_map->c_Create(global_height, global_width, (int)-1);
				out_voxel_map->sb_Set_Block(0, 0, &tmp2);

			}
		}

		p_position_of_blob[k][0] = zz_x;
		p_position_of_blob[k][1] = zz_y;
		p_position_of_blob[k][2] = width;
		p_position_of_blob[k][3] = height;

		p_patch = p_mask[k].mp();
		pp_mask = out_mask->mp();
		bool **p_dumy = dummy_max_mask.mp();
		bool **p_max = max_mask.mp();
		p1 = out_voxel_map->mp();
		p2 = p_voxel_maps[k].mp();
		for (int v0 = 0; v0 < p_mask[k].mh(); ++v0)
		{
			int v = zz_y + v0;
			for (int u0 = 0; u0 < p_mask[k].mw(); ++u0)
			{
				int u = zz_x + u0;
				if (p_patch[v0][u0] == true && pp_mask[v][u] == false)
				{
					pp_mask[v][u] = true;
					p1[v][u] = p2[v0][u0];
				}

				if (in_pack_flexible == false)
				{
					p_dumy[v][u] = true;
				}
				else
				{
					p_dumy[v][u] = p_max[v0][u0] || p_dumy[v][u];
				}
			
			}
		}
		//sc[0].s_d_Display(out_mask);
		//sc[1].s_d_Display(&dummy_max_mask);
		//sc[2].s_d_Display(&p_mask[k]);
		//Kv_Printf("k=%d/%d) zz_x = %d zz_y = %d",k,num_blob, zz_x,zz_y);
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	bool in_line_ei_on_true_off_false,
	bool in_fixed_image,
	bool in_pack_flexible,
	CKvMatrixInt *out_position_of_segmented_voxel_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixInt *p_voxel_maps, tmp2;
	CKvMatrixBool *p_mask, tmp, max_mask;
	CKvMatrixBool dummy_max_mask;
	int max_width, height, width, global_height, global_width;
	int num_blob, k;
	int **p1, **p2;
	bool locationFound, **p_patch, **pp_mask;

	p_voxel_maps = in_set_voxel_maps->vps(num_blob);
	p_mask = in_set_masks->vp();

	/**********************************************************/
	if (in_fixed_image == false)
	{
		max_width = -1;
		for (k = 0; k < num_blob; k++)
		{
			if (max_width < p_voxel_maps[k].mw())
			{
				max_width = p_voxel_maps[k].mw();
			}
		}
		global_width = max_width + zz_safeguard + 1;
		/*********************************************/
		if (global_width < 100) { global_width = 256; }
		global_height = zz_height;
	}
	else
	{
		global_height = zz_height;
		global_width = zz_width;
	}

	/**********************************************************/
	out_mask->c_Create(global_height, global_width, false);
	dummy_max_mask.c_Create(global_height, global_width, false);
	out_voxel_map->c_Create(global_height, global_width, (int)-1);

	int **p_position_of_blob = out_position_of_segmented_voxel_map->c_Create(num_blob, 4);
	//CKvScreen sc[3];

	for (k = 0; k < num_blob; k++)
	{
		p_mask[k].ms(width, height);

		if (in_pack_flexible == false)
		{
			max_mask.c_Create(height, width, true);
		}
		else
		{
			Set_New_Masks(
				&p_mask[k],//CKvMatrixBool *in_mask,
				&max_mask);//CKvMatrixBool *out_mask)
		}

		locationFound = false;
		zz_x = zz_y = 0;
		while (!locationFound)
		{
			for (int y = 0; y < global_height && !locationFound; ++y)
			{
				for (int x = 0; x < global_width && !locationFound; ++x)
				{
					zz_x = x;
					zz_y = y;
					if (checkFitPatchCanvas(
						&dummy_max_mask,//CKvMatrixBool *in_mask,
						&max_mask,//CKvMatrixBool *in_patch_mask,
						global_width,//int canvasStrideBlk,
						global_height,//int canvasHeightBlk,
						zz_safeguard))//int safeguard = 0)
					{
						locationFound = true;
					}
				}
			}

			if (!locationFound)
			{
				global_height *= 2;

				tmp.cp_Copy(out_mask);
				out_mask->c_Create(global_height, global_width, false);
				out_mask->sb_Set_Block(0, 0, &tmp);

				tmp.cp_Copy(&dummy_max_mask);
				dummy_max_mask.c_Create(global_height, global_width, false);
				dummy_max_mask.sb_Set_Block(0, 0, &tmp);

				tmp2.cp_Copy(out_voxel_map);
				out_voxel_map->c_Create(global_height, global_width, (int)-1);
				out_voxel_map->sb_Set_Block(0, 0, &tmp2);

			}
		}

		p_position_of_blob[k][0] = zz_x;
		p_position_of_blob[k][1] = zz_y;
		p_position_of_blob[k][2] = width;
		p_position_of_blob[k][3] = height;

		p_patch = p_mask[k].mp();
		pp_mask = out_mask->mp();
		bool **p_dumy = dummy_max_mask.mp();
		bool **p_max = max_mask.mp();
		p1 = out_voxel_map->mp();
		p2 = p_voxel_maps[k].mp();
		for (int v0 = 0; v0 < p_mask[k].mh(); ++v0)
		{
			int v = zz_y + v0;
			for (int u0 = 0; u0 < p_mask[k].mw(); ++u0)
			{
				int u = zz_x + u0;
				//if (in_line_ei_on_true_off_false == false)
				//{
					if (p_patch[v0][u0] == true && pp_mask[v][u] == false)
					{
						pp_mask[v][u] = true;
						p1[v][u] = p2[v0][u0];
					}
				//}
				//else
				//{
				//	if (p_max[v0][u0] == true && pp_mask[v][u] == false)
				//	{
				//		pp_mask[v][u] = true;
				//		p1[v][u] = p2[v0][u0];
				//	}
				//}


				if (in_pack_flexible == false)
				{
					p_dumy[v][u] = true;
				}
				else
				{
					p_dumy[v][u] = p_max[v0][u0] || p_dumy[v][u];
				}

			}
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::Set_New_Masks(
	CKvMatrixBool *in_mask,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool block_mask;
	int ww, hh, j, i, k, sz;
	bool **p_mask, *p_block;
	int block_size = zz_block_size;

	p_mask = in_mask->mps(ww, hh);
	out_mask->c_Create(hh, ww,false);
	for (j = 0; j < hh; j+= block_size)
	{
		for (i = 0; i < ww; i+= block_size)
		{
			in_mask->gb_Get_Block(i, j, zz_block_size, zz_block_size, &block_mask);
			p_block = block_mask.vps(sz);
			for (k = 0; k < sz; k++)
			{
				if (p_block[k] == true) 
				{ 
					block_mask.sm_Set_Matrix(true);
					break; 
				}
			}
			out_mask->sb_Set_Block(i, j, &block_mask);
		}
	}

}
//************************************************************************
bool VCL_Voxel_Color_Map::checkFitPatchCanvas(
	CKvMatrixBool *in_mask,
	CKvMatrixBool *in_patch_mask,
	int canvasStrideBlk,
	int canvasHeightBlk,
	int safeguard)
//************************************************************************
{
	int pos;
	int deltaY, deltaX;
	int v0_p, u0_p;
	int v0, u0;
	bool *p_mask, **p_pmask;

	in_patch_mask->ms(u0_p, v0_p);
	p_mask = in_mask->vp();
	p_pmask = in_patch_mask->mp();

	for (v0 = 0; v0 < v0_p; ++v0)
	{
		for (u0 = 0; u0 < u0_p; ++u0)
		{
			for (deltaY = -safeguard; deltaY < safeguard + 1; deltaY++)
			{
				for (deltaX = -safeguard; deltaX < safeguard + 1; deltaX++)
				{
					//printf("deltaY = %d delta = %d\r", deltaY, deltaX);
					//To do here;
					pos = patchBlock2CanvasBlock(
						u0 + deltaX,
						v0 + deltaY,
						canvasStrideBlk,
						canvasHeightBlk);

					if (pos < 0)
					{
						return false;
					}
					else if (p_pmask[v0][u0] && p_mask[pos])
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}
//************************************************************************
int VCL_Voxel_Color_Map::patchBlock2CanvasBlock(
	int in_uBlk,
	int in_vBlk,
	int in_mask_width,
	int in_mask_height)
//************************************************************************
{
	int x, y;

	x = in_uBlk + zz_x;
	y = in_vBlk + zz_y;

	if (x < 0) return -1;
	if (y < 0) return -1;
	if (x >= in_mask_width) return -1;
	if (y >= in_mask_height) return -1;

	return (y*in_mask_width + x);
}
//************************************************************************
void VCL_Voxel_Color_Map::Filling_Voxel_Map(
	CKvMatrixUcharRgb *in_voxel_map,
	CKvMatrixBool *in_mask,
	int in_block_size,
	bool in_shape_coding_mode,
	CKvMatrixUcharRgb *out_voxel_map)
//************************************************************************
{
	CKvMatrixUcharRgb tmp_img;
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvXvectorShort *p_xvec;
	CKvVectorShort table;
	CKvRgb rgb;
	int ww, hh, j, i, k, num_element;
	short *a, *b;
	unsigned char **pr, **pg, **pb;


	cpy_mask.cp_Copy(in_mask);
	if (in_shape_coding_mode)
	{
		geir_Get_EI_Region(
			&cpy_mask,//CKvMatrixBool *in_mask,
			in_block_size,//int in_block_size,
			&cpy_mask);//CKvMatrixBool *out_mask)
	}

	tmp_img.cp_Copy(in_voxel_map);

	tmp_img.ms(ww, hh);
	pr = tmp_img.mp(pg, pb);
	for (j = 0; j < hh; j++)
	{
		cpy_mask.gr_Get_Row(0, j, ww, &_cpy);
		runset1d.im_Import(_cpy);
		p_xvec = runset1d.z_px();

		p_xvec->z_gii(NULL, &a, &num_element, NULL);
		table.c_Create(num_element + 2);
		table.s_Set(1, num_element, a);
		b = table.vp(); b[0] = 0; b[num_element + 1] = ww;

		rgb = tmp_img.ge_Get_Element(b[0], j);
		for (k = b[0]; k < b[1]; k++)
		{
			pr[j][k] = rgb.r;
			pg[j][k] = rgb.g;
			pb[j][k] = rgb.b;
		}

		for (k = 2; k < num_element + 2; k += 2)
		{
			rgb = tmp_img.ge_Get_Element(b[k], j);
			for (i = b[k]; i < b[k + 1]; i++)
			{
				pr[j][i] = rgb.r;
				pg[j][i] = rgb.g;
				pb[j][i] = rgb.b;
			}
		}
	}

	out_voxel_map->cp_Copy(&tmp_img);
}
//************************************************************************
void VCL_Voxel_Color_Map::mvm_Merge_Voxel_Map_With_Filling(
	CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_shape_coding_mode,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvXvectorShort *p_xvec;
	CKvVectorShort table;
	CKvRgb rgb;
	int ww, hh, j, i, k, num_element;
	short *a, *b;
	unsigned char **pr, **pg, **pb;

	mvm_Merge_Voxel_Map_Vertical(
		in_set_voxel_maps,//CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixUcharRgb *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	cpy_mask.cp_Copy(out_mask);

	if (in_shape_coding_mode)
	{
		geir_Get_EI_Region(
			&cpy_mask,//CKvMatrixBool *in_mask,
			in_block_size,//int in_block_size,
			&cpy_mask);//CKvMatrixBool *out_mask)
	}

	// Filling unused space. 
	out_voxel_map->ms(ww, hh);
	pr = out_voxel_map->mp(pg, pb);
	for (j = 0; j < hh; j++)
	{
		cpy_mask.gr_Get_Row(0, j, ww, &_cpy);
		runset1d.im_Import(_cpy);
		p_xvec = runset1d.z_px();

		p_xvec->z_gii(NULL, &a, &num_element, NULL);
		table.c_Create(num_element + 2);
		table.s_Set(1, num_element, a);
		b = table.vp(); b[0] = 0; b[num_element + 1] = ww;

		rgb = out_voxel_map->ge_Get_Element(b[0], j);
		for (k = b[0]; k < b[1]; k++)
		{
			pr[j][k] = rgb.r;
			pg[j][k] = rgb.g;
			pb[j][k] = rgb.b;
		}

		for (k = 2; k < num_element + 2; k += 2)
		{
			rgb = out_voxel_map->ge_Get_Element(b[k], j);
			for (i = b[k]; i < b[k + 1]; i++)
			{
				pr[j][i] = rgb.r;
				pg[j][i] = rgb.g;
				pb[j][i] = rgb.b;
			}
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::Packing_Voxel_Map_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	CKvDepot_of_RgbaF *in_colors_or_NULL,
	int in_block_size,
	bool in_image_fixed,
	bool in_pack_flexible,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixInt *out_voxel_map_idx_or_NULL,
	CKvMatrixBool *out_mask_or_NULL)
//************************************************************************
{
	CKvMatrixInt idx_map; CKvMatrixBool mask;
	CKvSet_of_RgbaF set_colors; CKvRgbaF *p_colors;
	int ww, hh, j, i; unsigned char **pr, **pg, **pb;
	int **p_idx; bool **p_mask;

	Resize_Voxel_Blobs(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
		in_set_masks,//CKvSet_of_MatrixBool *io_set_voxel_masks,
		in_block_size);//int in_block_size)

	pvm_Pack_Voxel_Map_With_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_block_size,//int in_block_size,
		in_image_fixed,//bool in_image_fixed,
		in_pack_flexible,//bool in_pack_flexible,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxe_map,
		&idx_map,//CKvMatrixInt *out_voxel_map,
		&mask);//CKvMatrixBool *out_mask);

	if (in_colors_or_NULL != NULL)
	{
		p_idx = idx_map.mps(ww, hh);
		out_voxel_map->c_Create(hh, ww,
			Kv_Rgb((unsigned char)0, (unsigned char)0, (unsigned char)0));
		pr = out_voxel_map->mp(pg, pb);

		in_colors_or_NULL->e_Export(&set_colors);
		p_colors = set_colors.vp();

		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_idx[j][i] != -1)
				{
					pr[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].r));
					pg[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].g));
					pb[j][i] = (unsigned char)limit(round(255.f*p_colors[(int)p_idx[j][i]].b));
				}

			}
		}
	}

	if (out_voxel_map_idx_or_NULL != NULL)
	{
		p_idx = idx_map.mps(ww, hh);
		p_mask = mask.mp();
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_mask[j][i] == false)
				{
					p_idx[j][i] = -1;
				}
			}
		}
		out_voxel_map_idx_or_NULL->cp_Copy(&idx_map);
	}

	if (out_mask_or_NULL != NULL)
	{
		out_mask_or_NULL->cp_Copy(&mask);
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_With_Filling(
	CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_shape_coding_mode,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixUcharRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvXvectorShort *p_xvec;
	CKvVectorShort table;
	CKvRgb rgb;
	int ww, hh, j, i, k, num_element;
	short *a, *b;
	unsigned char **pr, **pg, **pb;

	pvm_Pack_Voxel_Map(
		in_set_voxel_maps,//CKvSet_of_MatrixUcharRgb *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixUcharRgb *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	///////////////////////////// Cut mask ////////////////////////////////
	bool **p_m = out_mask->mps(ww, hh);
	int min_x, max_x, min_y, max_y;

	min_x = ww; max_x = -1;
	min_y = hh; max_y = -1;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_m[j][i])
			{
				if (min_x > i) { min_x = i; }
				if (max_x < i) { max_x = i; }
				if (min_y > j) { min_y = j; }
				if (max_y < j) { max_y = j; }
			}
		}
	}

	CKvMatrixUcharRgb A; CKvMatrixBool B;
	A.cp_Copy(out_voxel_map); B.cp_Copy(out_mask);
	A.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_voxel_map);
	B.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_mask);
	/////////////////////////////////////////////////////////////////////////////////////////


	cpy_mask.cp_Copy(out_mask);

	if (in_shape_coding_mode)
	{
		geir_Get_EI_Region(
			&cpy_mask,//CKvMatrixBool *in_mask,
			in_block_size,//int in_block_size,
			&cpy_mask);//CKvMatrixBool *out_mask)
	}

	// Filling unused space. 
	out_voxel_map->ms(ww, hh);
	pr = out_voxel_map->mp(pg, pb);
	for (j = 0; j < hh; j++)
	{
		cpy_mask.gr_Get_Row(0, j, ww, &_cpy);
		runset1d.im_Import(_cpy);
		p_xvec = runset1d.z_px();

		p_xvec->z_gii(NULL, &a, &num_element, NULL);
		table.c_Create(num_element + 2);
		table.s_Set(1, num_element, a);
		b = table.vp(); b[0] = 0; b[num_element + 1] = ww;

		rgb = out_voxel_map->ge_Get_Element(b[0], j);
		for (k = b[0]; k < b[1]; k++)
		{
			pr[j][k] = rgb.r;
			pg[j][k] = rgb.g;
			pb[j][k] = rgb.b;
		}

		for (k = 2; k < num_element + 2; k += 2)
		{
			rgb = out_voxel_map->ge_Get_Element(b[k], j);
			for (i = b[k]; i < b[k + 1]; i++)
			{
				pr[j][i] = rgb.r;
				pg[j][i] = rgb.g;
				pb[j][i] = rgb.b;
			}
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_With_Filling(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_shape_coding_mode,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvXvectorShort *p_xvec;
	CKvVectorShort table;
	//CKvRgb rgb;
	int ww, hh, j, i, k, num_element;
	short *a, *b;
	int **p, rgb;

	pvm_Pack_Voxel_Map(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixInt *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	///////////////////////////// Cut mask ////////////////////////////////
	bool **p_m = out_mask->mps(ww, hh);
	int min_x, max_x, min_y, max_y;

	min_x = ww; max_x = -1;
	min_y = hh; max_y = -1;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_m[j][i])
			{
				if (min_x > i) { min_x = i; }
				if (max_x < i) { max_x = i; }
				if (min_y > j) { min_y = j; }
				if (max_y < j) { max_y = j; }
			}
		}
	}

	CKvMatrixInt A; CKvMatrixBool B;
	A.cp_Copy(out_voxel_map); B.cp_Copy(out_mask);
	A.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_voxel_map);
	B.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_mask);
	/////////////////////////////////////////////////////////////////////////////////////////


	cpy_mask.cp_Copy(out_mask);

	if (in_shape_coding_mode)
	{
		geir_Get_EI_Region(
			&cpy_mask,//CKvMatrixBool *in_mask,
			in_block_size,//int in_block_size,
			&cpy_mask);//CKvMatrixBool *out_mask)
	}

	// Filling unused space. 
	out_voxel_map->ms(ww, hh);
	p = out_voxel_map->mp();
	for (j = 0; j < hh; j++)
	{
		cpy_mask.gr_Get_Row(0, j, ww, &_cpy);
		runset1d.im_Import(_cpy);
		p_xvec = runset1d.z_px();
		p_xvec->z_gii(NULL, &a, &num_element, NULL);
		table.c_Create(num_element + 2);
		table.s_Set(1, num_element, a);
		b = table.vp(); b[0] = 0; b[num_element + 1] = ww;
		rgb = out_voxel_map->ge_Get_Element(b[0], j);
		for (k = b[0]; k < b[1]; k++)
		{
			p[j][k] = rgb;
		}
		for (k = 2; k < num_element + 2; k += 2)
		{
			rgb = out_voxel_map->ge_Get_Element(b[k], j);
			for (i = b[k]; i < b[k + 1]; i++)
			{
				p[j][i] = rgb;
			}
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::Resize_Voxel_Blobs(
	CKvSet_of_MatrixInt *io_set_voxel_maps_idx,
	CKvSet_of_MatrixBool *io_set_voxel_masks,
	int in_block_size)
//************************************************************************
{
	CKvMatrixInt voxel_map_idx, *p_set_idx;
	CKvMatrixBool voxel_mask, *p_set_mask;
	int num_blob, k;
	int ww, hh, nww, nhh;

	p_set_idx = io_set_voxel_maps_idx->vps(num_blob);
	p_set_mask = io_set_voxel_masks->vp();

	for (k = 0; k < num_blob; k++)
	{
		p_set_idx[k].ms(ww, hh);

		nww = ceil((double)ww / (double)in_block_size)*in_block_size;
		nhh = ceil((double)hh / (double)in_block_size)*in_block_size;

		voxel_map_idx.cp_Copy(&p_set_idx[k]);
		voxel_mask.cp_Copy(&p_set_mask[k]);

		p_set_idx[k].c_Create(nhh, nww, -1);
		p_set_mask[k].c_Create(nhh, nww, false);

		p_set_idx[k].sb_Set_Block(0, 0, &voxel_map_idx);
		p_set_mask[k].sb_Set_Block(0, 0, &voxel_mask);
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_With_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_image_fixed,
	bool in_pack_flexible,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvVectorShort table;
	int ww, hh, j, i;

	zz_block_size = in_block_size;

	pvm_Pack_Voxel_Map_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_image_fixed,//bool in_fixed_image,
		in_pack_flexible,//bool in_pack_flexible,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixInt *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	if (in_image_fixed == false)
	{
		bool **p_m = out_mask->mps(ww, hh);
		int min_x, max_x, min_y, max_y;

		min_x = ww; max_x = -1;
		min_y = hh; max_y = -1;
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_m[j][i])
				{
					if (min_x > i) { min_x = i; }
					if (max_x < i) { max_x = i; }
					if (min_y > j) { min_y = j; }
					if (max_y < j) { max_y = j; }
				}
			}
		}

		CKvMatrixInt A; CKvMatrixBool B;
		A.cp_Copy(out_voxel_map); B.cp_Copy(out_mask);
		A.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_voxel_map);
		B.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_mask);
	}

}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_With_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_line_ei_on_true_off_false,
	bool in_image_fixed,
	bool in_pack_flexible,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvSet_of_MatrixBool tmp_set_masks;
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvVectorShort table;
	int ww, hh, j, i;
	bool tmp_pack_flexible;
	
	zz_block_size = in_block_size;


	if (in_line_ei_on_true_off_false == true)
	{
		CKvMatrixBool *p_tsm = tmp_set_masks.c_Create(in_set_voxel_maps->vs());
		for (j = 0; j < in_set_voxel_maps->vs() ; j++)
		{
			EI_DCT_Line_Forward_on_Spatial_Domain(
				in_set_voxel_maps->gpe(j),//CKvMatrixInt *io_img,
				in_set_masks->gpe(j));//CKvMatrixBool *in_mask)
			p_tsm[j].c_Create(in_set_masks->gpe(j)->mh(), in_set_masks->gpe(j)->mw(), true);
		}
		tmp_pack_flexible = false;
	}
	else
	{
		tmp_set_masks.cp_Copy(in_set_masks);
		tmp_pack_flexible = in_pack_flexible;
	}

	tmp_pack_flexible = true;
	pvm_Pack_Voxel_Map_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		&tmp_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_line_ei_on_true_off_false,//bool in_line_ei_on_true_off_false,
		in_image_fixed,//bool in_fixed_image,
		tmp_pack_flexible,//bool in_pack_flexible,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixInt *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	if (in_image_fixed == false)
	{
		bool **p_m = out_mask->mps(ww, hh);
		int min_x, max_x, min_y, max_y;

		min_x = ww; max_x = -1;
		min_y = hh; max_y = -1;
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_m[j][i])
				{
					if (min_x > i) { min_x = i; }
					if (max_x < i) { max_x = i; }
					if (min_y > j) { min_y = j; }
					if (max_y < j) { max_y = j; }
				}
			}
		}

		CKvMatrixInt A; CKvMatrixBool B;
		A.cp_Copy(out_voxel_map); B.cp_Copy(out_mask);
		A.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_voxel_map);
		B.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_mask);
	}

}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_With_Filling_Maximum_Margin(
	CKvSet_of_MatrixInt *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_shape_coding_mode,
	bool in_image_fixed,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixInt *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvXvectorShort *p_xvec;
	CKvVectorShort table;
	int ww, hh, j, i, k, num_element;
	short *a, *b;
	int **p, rgb;

	pvm_Pack_Voxel_Map_Maximum_Margin(
		in_set_voxel_maps,//CKvSet_of_MatrixInt *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		in_image_fixed,//bool in_fixed_image,
		false,//bool in_pack_flexible,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixInt *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	if (in_image_fixed == false)
	{
		bool **p_m = out_mask->mps(ww, hh);
		int min_x, max_x, min_y, max_y;

		min_x = ww; max_x = -1;
		min_y = hh; max_y = -1;
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_m[j][i])
				{
					if (min_x > i) { min_x = i; }
					if (max_x < i) { max_x = i; }
					if (min_y > j) { min_y = j; }
					if (max_y < j) { max_y = j; }
				}
			}
		}

		CKvMatrixInt A; CKvMatrixBool B;
		A.cp_Copy(out_voxel_map); B.cp_Copy(out_mask);
		A.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_voxel_map);
		B.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_mask);
	}

	cpy_mask.cp_Copy(out_mask);

	if (in_shape_coding_mode)
	{
		geir_Get_EI_Region(
			&cpy_mask,//CKvMatrixBool *in_mask,
			in_block_size,//int in_block_size,
			&cpy_mask);//CKvMatrixBool *out_mask)
	}

	out_voxel_map->ms(ww, hh);
	p = out_voxel_map->mp();
	for (j = 0; j < hh; j++)
	{
		cpy_mask.gr_Get_Row(0, j, ww, &_cpy);
		runset1d.im_Import(_cpy);
		p_xvec = runset1d.z_px();
		p_xvec->z_gii(NULL, &a, &num_element, NULL);
		table.c_Create(num_element + 2);
		table.s_Set(1, num_element, a);
		b = table.vp(); 
		b[0] = 0; 
		b[num_element + 1] = ww;
		rgb = out_voxel_map->ge_Get_Element(b[0], j);
		for (k = b[0]; k < b[1]; k++)
		{
			p[j][k] = rgb;
		}
		for (k = 2; k < num_element + 2; k += 2)
		{
			rgb = out_voxel_map->ge_Get_Element(b[k], j);
			for (i = b[k]; i < b[k + 1]; i++)
			{
				p[j][i] = rgb;
			}
		}
	}


}
//************************************************************************
void VCL_Voxel_Color_Map::pvm_Pack_Voxel_Map_With_Filling(
	CKvSet_of_MatrixIntRgb *in_set_voxel_maps,
	CKvSet_of_MatrixBool *in_set_masks,
	int in_block_size,
	bool in_shape_coding_mode,
	CKvMatrixInt *out_position_of_segmented_voxe_map,
	CKvMatrixIntRgb *out_voxel_map,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool cpy_mask;
	CKvVectorBool _cpy;
	CKvXrunset1dShort runset1d;
	CKvXvectorShort *p_xvec;
	CKvVectorShort table;
	CKvRgbI rgb;
	int ww, hh, j, i, k, num_element;
	short *a, *b;
	int **pr, **pg, **pb;

	pvm_Pack_Voxel_Map(
		in_set_voxel_maps,//CKvSet_of_MatrixIntRgb *in_set_voxel_maps,
		in_set_masks,//CKvSet_of_MatrixBool *in_set_masks,
		out_position_of_segmented_voxe_map,//CKvMatrixInt *out_position_of_segmented_voxel_map,
		out_voxel_map,//CKvMatrixIntRgb *out_voxel_map,
		out_mask);//CKvMatrixBool *out_mask)

	///////////////////////////// Cut mask ////////////////////////////////
	bool **p_m = out_mask->mps(ww, hh);
	int min_x, max_x, min_y, max_y;

	min_x = ww; max_x = -1;
	min_y = hh; max_y = -1;
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			if (p_m[j][i])
			{
				if (min_x > i) { min_x = i; }
				if (max_x < i) { max_x = i; }
				if (min_y > j) { min_y = j; }
				if (max_y < j) { max_y = j; }
			}
		}
	}

	CKvMatrixIntRgb A; CKvMatrixBool B;
	A.cp_Copy(out_voxel_map); B.cp_Copy(out_mask);
	A.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_voxel_map);
	B.gb_Get_Block(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, out_mask);
	/////////////////////////////////////////////////////////////////////////////////////////


	cpy_mask.cp_Copy(out_mask);

	if (in_shape_coding_mode)
	{
		geir_Get_EI_Region(
			&cpy_mask,//CKvMatrixBool *in_mask,
			in_block_size,//int in_block_size,
			&cpy_mask);//CKvMatrixBool *out_mask)
	}

	// Filling unused space. 
	out_voxel_map->ms(ww, hh);
	pr = out_voxel_map->mp(pg, pb);
	for (j = 0; j < hh; j++)
	{
		cpy_mask.gr_Get_Row(0, j, ww, &_cpy);
		runset1d.im_Import(_cpy);
		p_xvec = runset1d.z_px();

		p_xvec->z_gii(NULL, &a, &num_element, NULL);
		table.c_Create(num_element + 2);
		table.s_Set(1, num_element, a);
		b = table.vp(); b[0] = 0; b[num_element + 1] = ww;

		rgb = out_voxel_map->ge_Get_Element(b[0], j);
		for (k = b[0]; k < b[1]; k++)
		{
			pr[j][k] = rgb.r;
			pg[j][k] = rgb.g;
			pb[j][k] = rgb.b;
		}

		for (k = 2; k < num_element + 2; k += 2)
		{
			rgb = out_voxel_map->ge_Get_Element(b[k], j);
			for (i = b[k]; i < b[k + 1]; i++)
			{
				pr[j][i] = rgb.r;
				pg[j][i] = rgb.g;
				pb[j][i] = rgb.b;
			}
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::geir_Get_EI_Region(
	CKvMatrixBool *in_mask,
	int in_block_size,
	CKvMatrixBool *out_mask)
//************************************************************************
{
	CKvMatrixBool block_mask;
	int ww, hh, sz, j, i, l;
	bool *p_bm;

	out_mask->cp_Copy(in_mask);

	out_mask->ms(ww, hh);
	for (j = 0; j < hh; j += in_block_size)
	{
		for (i = 0; i < ww; i += in_block_size)
		{
			out_mask->gb_Get_Block(i, j, in_block_size, in_block_size, &block_mask);
			p_bm = block_mask.vps(sz);

			for (l = 0; l < sz; l++)
			{
				if (p_bm[l] == true)
				{
					block_mask.sm_Set_Matrix(true);
					out_mask->sb_Set_Block(i, j, &block_mask);
					break;
				}
			}
		}
	}
}
//************************************************************************
void VCL_Voxel_Color_Map::geir_Get_EI_Region(
	CKvSet_of_MatrixBool *in_set_of_mask,
	int in_block_size,
	CKvSet_of_MatrixBool *out_set_of_mask)
//************************************************************************
{
	CKvMatrixBool block_mask;
	int ww, hh, num, sz, j, i, k, l;
	bool *p_bm;

	out_set_of_mask->cp_Copy(in_set_of_mask);
	CKvMatrixBool *p_mask = out_set_of_mask->vp();
	num = out_set_of_mask->vs();

	for (k = 0; k < num; k++)
	{
		out_set_of_mask->gpe(k)->ms(ww, hh);
		for (j = 0; j < hh; j += in_block_size)
		{
			for (i = 0; i < ww; i += in_block_size)
			{
				out_set_of_mask->gpe(k)->gb_Get_Block(i, j, in_block_size, in_block_size, &block_mask);
				p_bm = block_mask.vps(sz);

				for (l = 0; l < sz; l++)
				{
					if (p_bm[l] == true)
					{
						block_mask.sm_Set_Matrix(true);
						out_set_of_mask->gpe(k)->sb_Set_Block(i, j, &block_mask);
						break;
					}
				}
			}
		}
	}
}
//************************************************************************






//************************************************************************
bool VCL_Voxel_Color_Map::s_gvm_Generate_Voxel_Map__wo_partitioning(
	VCL_DoCube_X_Color *in_docube,
	std::vector<Voxel_Slice_Scanned_Data> &in_voxel_sequence,
	CKvSet_of_VectorInt *in_offsets,
	CKvDepot_of_MatrixInt  *out_set_voxel_color_maps,
	CKvDepot_of_MatrixBool *out_set_masks)
//************************************************************************
{
	CKvMatrixBool          *p_masks;
	CKvMatrixInt           *p_voxel_maps;;
	CKvDepot_of_MatrixInt  depot_voxel_maps;
	CKvDepot_of_MatrixBool depot_voxel_masks;
	CKvDepot_of_RgbaF      voxel_colors;      
	CKvSet_of_RgbaF        vc;    
	CKvRgbaF               *rgbaf;
	CKvVectorInt           *p_offset;
	int *p_po, max_length, tmp, count_depot;
	int num_blob, num_toc, num_element, k, l, j, i;
	int **p_idx;
	bool **p_pm;

	p_offset = in_offsets->vps(num_blob); 
	if (num_blob == 0) { return false; }
	
	depot_voxel_maps.in_Initialize(1);
	depot_voxel_masks.in_Initialize(1);

	p_voxel_maps = depot_voxel_maps.gps_Get_Pointer_of_Space()->vp();
	p_masks      = depot_voxel_masks.gps_Get_Pointer_of_Space()->vp();
	count_depot  = voxel_colors.ne_Number_of_Elements();

	in_docube->gvc_Get_Voxel_Color().e_Export(&vc);
	rgbaf = vc.vp();

	printf("-----num_blob = %d\n", num_blob);

	max_length = -1;
	for (k = 0; k < num_blob; k++)
	{
		if (in_voxel_sequence[k].voxel_idx_sequences.size() == 0)
		{
			continue;
		}

		p_po = p_offset[k].vps(num_toc);
		tmp = 0;
		for (l = 0; l < num_toc; l++)
		{
			num_element = in_voxel_sequence[k].voxel_idx_sequences[l].size();
			tmp	= num_element - 1 + p_po[l];
			if (max_length < tmp)
			{
				max_length = tmp;
			}
			//tmp += in_voxel_sequence[k].voxel_idx_sequences[l].size() + p_po[l];
		}
	}
	
	printf("------max_length = %d\n", max_length);


	count_depot = 0;
	p_voxel_maps[count_depot].c_Create(num_blob, max_length, (int)-1);
	p_masks[count_depot].c_Create(num_blob, max_length, false);

	p_idx = p_voxel_maps[count_depot].mp();
	p_pm  = p_masks[count_depot].mp();
	for (k = 0; k < num_blob; k++)
	{
		if (in_voxel_sequence[k].voxel_idx_sequences.size() == 0)
		{
			continue;
		}

		p_po = p_offset[k].vps(num_toc);

		for (l = 0; l < num_toc; l++)
		{
			num_element = in_voxel_sequence[k].voxel_idx_sequences[l].size();

			for (j = 0; j < num_element; j++)
			{
				p_idx[k][j + p_po[l]] = (int)in_voxel_sequence[k].voxel_idx_sequences[l][j];
				
				if ((int)in_voxel_sequence[k].voxel_idx_sequences[l][j] != -1)
				{
					p_pm[k][j + p_po[l]] = true;
				}

			}
		}
		//count_depot++;

	}

	count_depot = 1;
	if (count_depot != 0)
	{
		depot_voxel_maps.cne_Change_Number_of_Elements(count_depot);
		depot_voxel_masks.cne_Change_Number_of_Elements(count_depot);

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);
	}
	else
	{
		depot_voxel_maps.in_Initialize();
		depot_voxel_masks.in_Initialize();

		out_set_voxel_color_maps->cp_Copy(&depot_voxel_maps);
		out_set_masks->cp_Copy(&depot_voxel_masks);

		return false;
	}



	/******************************************************************************************/
	int ww, hh; bool **p_bool_pointer; int valid_count = 0;
	for (k = 0; k < count_depot; k++)
	{
		p_bool_pointer = out_set_masks->gep_Get_Element_Pointer(k)->mps(ww, hh);
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_bool_pointer[j][i] == true)
				{
					valid_count++;
				}
			}
		}
	}

	printf(">>>>>>>>> valid_count = %d <<<<<<<<<<<\n", valid_count);

	if (Kv_Printf("Do you want to display ?"))
	{
		int **p_int_pointer; CKvMatrixUcharRgb mat;
		for (k = 0; k < count_depot; k++)
		{
			p_int_pointer = out_set_voxel_color_maps->gep_Get_Element_Pointer(k)->mps(ww, hh);
			mat.c_Create(hh, ww, Kv_Rgb(0, 0, 0));
			for (j = 0; j < hh; j++)
			{
				for (i = 0; i < ww; i++)
				{
					if (p_int_pointer[j][i] != -1)
					{
						unsigned char r = (unsigned char)(rgbaf[p_int_pointer[j][i]].r*255.f);
						unsigned char g = (unsigned char)(rgbaf[p_int_pointer[j][i]].g*255.f);
						unsigned char b = (unsigned char)(rgbaf[p_int_pointer[j][i]].b*255.f);

						mat.se_Set_Element(i, j, &r, &g, &b);
					}
				}
			}

			CKvScreen sc;
			sc.s_d_Display(&mat); Kv_Printf("k=%d / %d", k, count_depot);

		}
	}


	/******************************************************************************************/






	return true;
}
//************************************************************************





