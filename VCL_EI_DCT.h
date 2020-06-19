#include <vector>

#ifndef VCL_EI_DCT_DEFINE
#define VCL_EI_DCT_DEFINE

class VCL_EI_DCT
{
public:
	VCL_EI_DCT();
	~VCL_EI_DCT();

	bool EI_DCT_Forward(
		CKvVectorUcharRgb *io_img,
		CKvVectorBool *in_mask);

	bool EI_DCT_Forward_on_Spatial_Domain(
		CKvVectorUcharRgb *io_img,
		CKvVectorBool *in_mask);

	bool EI_DCT_Forward_on_Spatial_Domain(
		CKvVectorInt *io_img,
		CKvVectorBool *in_mask);

	bool EI_DCT_Forward_on_Spatial_Domain2(
		CKvVectorInt *io_img,
		CKvVectorBool *in_mask);

	bool EI_DCT_Forward_on_Spatial_Domain_Right(
		CKvVectorUcharRgb *io_img_vec,
		CKvVectorInt *io_idx_vec,
		CKvVectorInt *in_reference_idx_vec,
		CKvVectorBool *in_mask,
		CKvSet_of_Point3Df *in_set_of_voxels,
		std::vector<int> &io_control_points,
		bool &io_mode,
		int in_number_of_control_point_for_half_plane);

	bool EI_DCT_Forward_on_Spatial_Domain_Left(
		CKvVectorUcharRgb *io_img_vec,
		CKvVectorInt *io_idx_vec,
		CKvVectorInt *in_reference_idx_vec,
		CKvVectorBool *in_mask,
		CKvSet_of_Point3Df *in_set_of_voxels,
		std::vector<int> &io_control_points,
		bool &io_mode,
		int in_number_of_control_point_for_half_plane);

	bool EI_DCT_Forward(
		CKvMatrixUcharRgb *io_img,
		CKvMatrixBool *in_mask);

	bool EI_DCT_Line_Forward(
		CKvMatrixUcharRgb *io_img,
		CKvMatrixBool *in_mask);

	bool EI_DCT_Line_Forward_on_Spatial_Domain(
		CKvMatrixUcharRgb *io_img,
		CKvMatrixBool *in_mask);

	bool EI_DCT_Line_Forward_on_Spatial_Domain(
		CKvMatrixInt *io_img,
		CKvMatrixBool *in_mask);

	void EI_Block_DCT_Forward(
		CKvMatrixUcharRgb *io_img,
		int &in_block_x,
		int &in_block_y,
		CKvMatrixBool *io_mask);

	/*********************************************/

	bool EI_DCT_Backward(
		CKvVectorUcharRgb *io_img,
		CKvVectorBool *in_mask);
	
	bool EI_DCT_Backward_on_Spatial_Domain(
		CKvVectorUcharRgb *io_img,
		CKvVectorBool *in_mask);

	void EI_DCT_Backward(
		CKvMatrixUcharRgb *io_img,
		CKvMatrixBool *in_mask);

	bool EI_DCT_Line_Backward(
		CKvMatrixUcharRgb *io_img,
		CKvMatrixBool *in_mask);

	bool EI_DCT_Line_Backward_on_Spatial_Domain(
		CKvMatrixUcharRgb *io_img,
		CKvMatrixBool *in_mask);

	void EI_Block_DCT_Backward(
		CKvMatrixUcharRgb *io_img,
		int &in_block_x,
		int &in_block_y,
		CKvMatrixBool *in_mask);

	/************************************************/
	template<typename T>
	inline T limit(const T& value)
	{
		return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
	}
	/************************************************/

	void reverse_vector(
		CKvVectorUcharRgb *io_img_vec,
		CKvVectorInt *io_idx_vec,
		CKvVectorInt *io_reference_idx_vec,
		CKvVectorBool *io_mask);

	void transform_points(
		int in_current_length,
		int in_number_of_control_points,
		int in_max_length,
		std::vector<int> &out_control_points);

	void transform_point(
		int &in_position,
		int &out_position,
		int &in_max_length);

};


#endif