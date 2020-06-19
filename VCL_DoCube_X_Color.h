#ifndef VCL_DOCUBE_X_COLOR
#define VCL_DOCUBE_X_COLOR

//#include <VCL_DoCube.h>

class VCL_DoCube_X_Color : public VCL_DoCube, public CKvSdkimChainCode
{
public:

	/****************************************************************************/
	VCL_DoCube_X_Color();
	virtual ~VCL_DoCube_X_Color();

	void cp_Copy_Data(VCL_DoCube_X_Color &in_docube);
	/********************************************************************/
	
	void i_Import(CKvSet2d_of_XvectorShort *in_runset);
	void i_Import(CKvSet2d_of_VectorShort *in_runset);
	void i_Import(CKvDepot_of_Point3Df *in_voxels,int in_ww,int in_hh,int in_dd);
	void i_Import(
		CKvDepot_of_Point3Df *in_3d_points,
		int in_hh, 
		int in_ww, 
		int in_dd,
		CKvDepot_of_RgbaF *in_depot_colors,
		bool in_inverse_true_or_false);
	void i_Import_ordered_data(
		CKvDepot_of_Point3Df *in_3d_points,
		int in_hh,
		int in_ww,
		int in_dd,
		CKvDepot_of_RgbaF *in_depot_colors);
	void i_Import_ordered_data2(
		CKvDepot_of_Point3Df *in_3d_points,
		int in_hh,
		int in_ww,
		int in_dd,
		CKvDepot_of_RgbaF *in_depot_colors);

	void i_Import_Surface_Voxels(
		CKvSet2d_of_XvectorShort *in_runset);

	void i_Import_DoCube_Using_Surface_Voxels(
		CKvSet2d_of_XvectorShort *in_runset,
		int in_dd);

	/********************************************************************/


	CKvSet2d_of_XvectorShort e_Export_Xvectors_Boundary_Point()
	{ 
		return zz_depth_data_w_bp; 
	}

	bool e_Export_Xvectors_Boundary_Point(
		CKvSet2d_of_XvectorShort *out_depth_data_w_bp);

	CKvSet2d_of_XvectorShort e_Export_Xvectors()
	{
		CKvSet2d_of_XvectorShort tmp;

		cd_Conversion_Data(
			&zz_depth_data,//CKvSet2d_of_VectorShort *in_set_of_vector,
			&tmp);//CKvSet2d_of_XvectorShort *out_set_of_vector);
		return tmp;
	}
	/****************************************************************************/

	/****************************************************************************/
	void gmd_Generate_Modified_DoCube();
	void gsp_Generate_Indices_of_Surface_Points();
	void gsp_Generate_Surface_Voxels(CKvSet_of_Voxel *out_set_of_voxel);
	void gsp_Generate_Surface_Points(CKvDepot_of_Point3Df *out_set_of_voxel);
	void gsp_Generate_Surface_Points(bool in_mode_int_true_or_float_false, CKvDepot_of_Point3Df *out_set_of_voxel);
	void gsp_Generate_Surface_Points_Condition_Free(bool in_mode_int_true_or_float_false, CKvDepot_of_Point3Df *out_set_of_voxel);
	void gsp_Generate_Surface_Voxels(CKvDepot_of_Voxel *out_set_of_voxel);
	void gvtdt_Generate_Voxel_To_DoCube_Table(CKvMatrixInt *out_table_N_by_4);
	/****************************************************************************/


	/**Import Color**/
	/****************************************************************************/
	void i_Import_Color(CKvSet2d_of_XvectorUcharRgb *in_colors){ zz_color.cp_Copy(in_colors); }
	void i_Import_Color(CKvDepot_of_RgbaF *in_depot_of_color);
	void i_Import_Color_Vector(CKvDepot_of_RgbaF *in_depot_of_color){ zz_depot_of_color.cp_Copy(in_depot_of_color); }
	void i_Import_Color_Vector(CKvSet_of_RgbaF *in_set_of_colors) { /*zz_depot_of_color.in_Initialize(); */zz_depot_of_color.im_Import(in_set_of_colors); }
	void i_Import_Color_and_Convert_Vector(CKvSet2d_of_XvectorUcharRgb *in_colors, int in_num_voxel);
	void i_Import_Color_Vector_and_Convert_2D(CKvDepot_of_RgbaF *in_depot_of_color);

	void i_Import_Color_Palette(
		CKvSet_of_MatrixUcharRgb *in_color_palette,
		CKvSet_of_MatrixInt *in_voxel_color_indices);
	void i_Import_Color_Palette(
		CKvVectorUchar *in_intensity,
		CKvSet_of_YCbCr *in_cbcr,
		CKvMatrixInt *in_voxel_color_indices);
	void i_Import_Color_Palette(
		CKvMatrixUchar *in_intensity,
		CKvSet_of_YCbCr *in_cbcr,
		CKvMatrixInt *in_voxel_color_indices);
	/****************************************************************************/


	/**Getter**/
	/****************************************************************************/
	int gvi_Get_Voxel_Index(int in_x, int in_y, int in_z);
	long long gvi_Get_Voxel_Index2(int in_x, int in_y, int in_z, int in_search_start);
	CKvSet2d_of_VectorInt    grlv_Get_RL_to_Voxel_Index();
	CKvSet2d_of_VectorShort  grl_Get_RL_vector();

	void gmd_Get_Modified_DoCube(CKvSet2d_of_XvectorShort *out_modified_docube)
	{
		out_modified_docube->cp_Copy(&zz_depth_data_w_bp);
	}
	double gss_Get_Step_Size(){ return zz_step_size_; }

	CKvDepot_of_Point3Df     *gsp_Get_Surface_Points_Pointer(){ return &zz_surface_point3d; }
	CKvDepot_of_Point3Df     gsp_Get_Surface_Points(){ return zz_surface_point3d; }
	CKvDepot_of_Voxel        gsp_Get_Surface_Voxels(){ return zz_surface_voxels; }
	CKvDepot_of_RgbaF        gvc_Get_Voxel_Color(){ return zz_depot_of_color; }
	CKvSet2d_of_XvectorUcharRgb gvc_Get_Voxel_Color_2d(){ return zz_color; }
	CKvSet2d_of_XvectorInt grlv_Get_RL_to_Voxel_Index2(){ return zz_set_point_to_rl_idx; }
	CKvSet2d_of_XvectorShort grl_Get_RL_Xvector()
	{
		CKvSet2d_of_XvectorShort depth_data_x;
		cd_Conversion_Data(&zz_depth_data, &depth_data_x);
		return depth_data_x;
	}

	void gdp_Get_Display_Parameters(
		CKvDepot_of_Point3Df *out_surface_points,
		CKvDepot_of_RgbaF *out_surface_colors,
		CKvMesh_of_Point *out_mesh_of_points)
	{
		out_surface_points->cp_Copy(&zz_surface_point3d);
		out_surface_colors->cp_Copy(&zz_depot_of_color);
		out_mesh_of_points->cp_Copy(&zz_mesh_of_point);
	}
	/****************************************************************************/


	/**DATA I\O**/
	/****************************************************************************/
	bool sc_Save_Color(CKvString in_filename);
	bool sc_Save_Color2(CKvString in_filename);
	bool sc_Save_Color3(CKvString in_filename);

	bool spi_Save_Palette_Indices(CKvString in_filename);
	bool sp_Save_Points(
		CKvString in_filename,
		CKvDepot_of_Point3Df *in_point3d);
	/****************************************************************************/



	/****************************************************************************/
private:

	bool fbp_Find_Boundary_Points(
		CKvSet2d_of_XvectorShort &in_set_of_runsets,
		int in_length_of_ray_segment,
		bool in_MODE_for_TESTing_input_runsets,
		CKvSet2d_of_XvectorShort *out_BOUNDARY_set_of_runsets___NULL,
		CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
		CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL);

	bool fbp_Find_Boundary_Points_Using_Neighborhood(
		CKvXvectorShort *in_runset_on_a_CURRENT_ray_segment,
		CKvXvectorShort *in_runset_on_a_PREVIOUS_ray_segment,
		CKvXvectorShort *in_runset_on_a_NEXT_ray_segment,
		CKvXvectorShort *in_runset_on_a_UPPER_ray_segment,
		CKvXvectorShort *in_runset_on_a_LOWER_ray_segment,
		int in_length_of_ray_segment,
		bool in_MODE_for_TESTing_input_runsets,
		CKvXvectorShort *out_BOUNDARY_RUNSET_of_the_CURRENT_ray_segment___or_NULL,
		CKvXvectorInt *io_buffer_for_tempo_states___or_NULL,
		CKvXvectorShort *io_buffer_for_tempo_runsets___or_NULL);

	bool u_trr_Test_Runset_on_a_Ray_segment(
		CKvXvectorShort &in_runset,
		int in_length_of_ray_segment);

private:

	bool cd_Conversion_Data(
		CKvSet2d_of_VectorShort *in_set_of_vector,
		CKvSet2d_of_XvectorShort *out_set_of_vector);

	void cd_Conversion_Data_Surface_Voxel_To_DoCube(
		CKvXrunset3dShort *in_surface_rle,
		CKvXrunset3dShort *out_docube_rle);

	bool ddc_Decode_DoCube(
		CKvSet2d_of_XvectorShort *in_set_of_vector,
		bool in_mode_int_true_or_float_false,
		CKvDepot_of_Point3Df *out_set_of_voxel);

	bool ddc_Decode_DoCube(
		CKvSet2d_of_XvectorShort *in_set_of_vector,
		CKvSet_of_Voxel *out_set_of_voxel);

	bool ddc_Decode_DoCube_Indices(
		CKvSet2d_of_XvectorShort *in_set_of_vector);

	void convert_data(
		CKvVectorBool *in_pattern,
		CKvXvectorShort *out_xvecShort);
protected:

	CKvSet2d_of_XvectorShort    zz_depth_data_w_bp;
	CKvSet2d_of_XvectorUcharRgb zz_color;
	CKvSet2d_of_XvectorInt    zz_set_point_to_rl_idx;

	CKvSet2d_of_MatrixInt zz_palette_index;

	CKvDepot_of_Point3Df zz_surface_point3d;
	CKvDepot_of_Voxel zz_surface_voxels;

	CKvDepot_of_RgbaF zz_depot_of_color;
	CKvMesh_of_Point zz_mesh_of_point;
	double zz_step_size_;

	CKvDepot_of_MatrixInt zz_voxel_index_patch;

	CKvStopWatch zz_stop_watch;
	double zz_time;

	template<typename T>
	inline T limit(const T& value)
	{
		return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
	}

public:
	int zz_Xlim[2];
	int zz_Ylim[2];

};

#endif