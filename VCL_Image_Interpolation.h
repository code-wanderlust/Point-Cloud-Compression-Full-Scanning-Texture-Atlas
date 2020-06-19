#pragma once
#include <vector>

class VCL_Image_Interpolation
{
public:
	VCL_Image_Interpolation();
	~VCL_Image_Interpolation();

	void interpolate_background(
		CKvMatrixUchar *in_image,
		CKvXrunsetShort *in_mask,
		CKvMatrixUchar *out_image);

	void interpolate_background(
		CKvMatrixUcharRgb *in_image,
		CKvXrunsetShort *in_mask,
		CKvMatrixUcharRgb *out_image);

	void interpolate_background(
		CKvMatrixUcharRgb *in_image,
		CKvMatrixBool *in_mask,
		CKvMatrixUcharRgb *out_image);

	void interpolate_background(
		CKvMatrixShort *in_image,
		CKvXrunsetShort *in_mask,
		CKvMatrixShort *out_image);

protected:
	void CreateCoarseLayer(
		CKvMatrixUchar *in_image,
		CKvMatrixUchar *out_mip,
		CKvMatrixBool *in_occupancyMap,
		CKvMatrixBool *out_mipOccupancyMap);

	void CreateCoarseLayer(
		CKvMatrixUcharRgb *in_image,
		CKvMatrixUcharRgb *out_mip,
		CKvMatrixBool *in_occupancyMap,
		CKvMatrixBool *out_mipOccupancyMap);

	void CreateCoarseLayer(
		CKvMatrixShort *in_image,
		CKvMatrixShort *out_mip,
		CKvMatrixBool *in_occupancyMap,
		CKvMatrixBool *out_mipOccupancyMap);

	void regionFill(
		CKvMatrixUchar *io_image,
		CKvMatrixBool *in_mask,
		CKvMatrixUchar *in_downsized_image);

	void regionFill(
		CKvMatrixUcharRgb *io_image,
		CKvMatrixBool *in_mask,
		CKvMatrixUcharRgb *in_downsized_image);

	void regionFill(
		CKvMatrixShort *io_image,
		CKvMatrixBool *in_mask,
		CKvMatrixShort *in_downsized_image);

};
