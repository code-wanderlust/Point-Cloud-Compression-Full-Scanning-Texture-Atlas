#include "stdafx.h"
#include "VCL_Image_Interpolation.h"


/********************************************************************************/
VCL_Image_Interpolation::VCL_Image_Interpolation()
/********************************************************************************/
{

}
/********************************************************************************/
VCL_Image_Interpolation::~VCL_Image_Interpolation()
/********************************************************************************/
{

}
/********************************************************************************/
void VCL_Image_Interpolation::interpolate_background(
	CKvMatrixUchar *in_image,
	CKvXrunsetShort *in_mask,
	CKvMatrixUchar *out_image)
/********************************************************************************/
{
	CKvDepot_of_MatrixUchar mipVec;
	CKvDepot_of_MatrixBool mipOccupancyMapVec;
	CKvMatrixBool occupancyMapTemp;
	CKvMatrixUchar tmp_mip;
	CKvMatrixBool tmp_mipOccupancy;
	int miplev = 0, i=0;

	in_mask->ex_Export(true, false, occupancyMapTemp);
	out_image->cp_Copy(in_image);

	// create coarse image by dyadic sampling
	mipVec.in_Initialize();
	mipOccupancyMapVec.in_Initialize();
	while (1)
	{
		if (miplev > 0)
		{
			CreateCoarseLayer(
				mipVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mip,
				mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mipOccupancy);
		}
		else
		{
			CreateCoarseLayer(
				out_image,
				&tmp_mip,
				&occupancyMapTemp,
				&tmp_mipOccupancy);
		}

		mipVec.ap_Append(false, &tmp_mip, NULL);
		mipOccupancyMapVec.ap_Append(false, &tmp_mipOccupancy, NULL);

		if (tmp_mip.mw() <= 4 || tmp_mip.mh() <= 4) break;
		++miplev;
	}
	miplev++;

	// push phase: inpaint laplacian
	regionFill(
		mipVec.gep_Get_Element_Pointer(miplev-1),
		mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
		mipVec.gep_Get_Element_Pointer(miplev - 1));

	for (i = miplev - 1; i >= 0; --i)
	{
		if (i > 0)
		{
			regionFill(
				mipVec.gep_Get_Element_Pointer(i - 1),
				mipOccupancyMapVec.gep_Get_Element_Pointer(i - 1),
				mipVec.gep_Get_Element_Pointer(i));
		}
		else
		{
			regionFill(
				out_image,
				&occupancyMapTemp,
				mipVec.gep_Get_Element_Pointer(i));
		}
	}
}
/********************************************************************************/
void VCL_Image_Interpolation::interpolate_background(
	CKvMatrixUcharRgb *in_image,
	CKvXrunsetShort *in_mask,
	CKvMatrixUcharRgb *out_image)
/********************************************************************************/
{
	CKvDepot_of_MatrixUcharRgb mipVec;
	CKvDepot_of_MatrixBool mipOccupancyMapVec;
	CKvMatrixBool occupancyMapTemp;
	CKvMatrixUcharRgb tmp_mip;
	CKvMatrixBool tmp_mipOccupancy;
	int miplev = 0, i = 0;

	in_mask->ex_Export(true, false, occupancyMapTemp);
	out_image->cp_Copy(in_image);

	// create coarse image by dyadic sampling
	mipVec.in_Initialize();
	mipOccupancyMapVec.in_Initialize();
	while (1)
	{
		if (miplev > 0)
		{
			CreateCoarseLayer(
				mipVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mip,
				mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mipOccupancy);
		}
		else
		{
			CreateCoarseLayer(
				out_image,
				&tmp_mip,
				&occupancyMapTemp,
				&tmp_mipOccupancy);
		}

		mipVec.ap_Append(false, &tmp_mip, NULL);
		mipOccupancyMapVec.ap_Append(false, &tmp_mipOccupancy, NULL);

		if (tmp_mip.mw() <= 4 || tmp_mip.mh() <= 4) break;
		++miplev;
	}
	miplev++;

	// push phase: inpaint laplacian
	regionFill(
		mipVec.gep_Get_Element_Pointer(miplev - 1),
		mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
		mipVec.gep_Get_Element_Pointer(miplev - 1));

	for (i = miplev - 1; i >= 0; --i)
	{
		if (i > 0)
		{
			regionFill(
				mipVec.gep_Get_Element_Pointer(i - 1),
				mipOccupancyMapVec.gep_Get_Element_Pointer(i - 1),
				mipVec.gep_Get_Element_Pointer(i));
		}
		else
		{
			regionFill(
				out_image,
				&occupancyMapTemp,
				mipVec.gep_Get_Element_Pointer(i));
		}
	}
}
/********************************************************************************/
void VCL_Image_Interpolation::interpolate_background(
	CKvMatrixUcharRgb *in_image,
	CKvMatrixBool *in_mask,
	CKvMatrixUcharRgb *out_image)
/********************************************************************************/
{
	CKvDepot_of_MatrixUcharRgb mipVec;
	CKvDepot_of_MatrixBool mipOccupancyMapVec;
	CKvMatrixBool occupancyMapTemp;
	CKvMatrixUcharRgb tmp_mip;
	CKvMatrixBool tmp_mipOccupancy;
	int miplev = 0, i = 0;

	occupancyMapTemp.cp_Copy(in_mask);
	out_image->cp_Copy(in_image);

	// create coarse image by dyadic sampling
	mipVec.in_Initialize();
	mipOccupancyMapVec.in_Initialize();
	while (1)
	{
		if (miplev > 0)
		{
			CreateCoarseLayer(
				mipVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mip,
				mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mipOccupancy);
		}
		else
		{
			CreateCoarseLayer(
				out_image,
				&tmp_mip,
				&occupancyMapTemp,
				&tmp_mipOccupancy);
		}

		mipVec.ap_Append(false, &tmp_mip, NULL);
		mipOccupancyMapVec.ap_Append(false, &tmp_mipOccupancy, NULL);

		if (tmp_mip.mw() <= 4 || tmp_mip.mh() <= 4) break;
		++miplev;
	}
	miplev++;

	// push phase: inpaint laplacian
	regionFill(
		mipVec.gep_Get_Element_Pointer(miplev - 1),
		mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
		mipVec.gep_Get_Element_Pointer(miplev - 1));

	for (i = miplev - 1; i >= 0; --i)
	{
		if (i > 0)
		{
			regionFill(
				mipVec.gep_Get_Element_Pointer(i - 1),
				mipOccupancyMapVec.gep_Get_Element_Pointer(i - 1),
				mipVec.gep_Get_Element_Pointer(i));
		}
		else
		{
			regionFill(
				out_image,
				&occupancyMapTemp,
				mipVec.gep_Get_Element_Pointer(i));
		}
	}
}
/********************************************************************************/
void VCL_Image_Interpolation::interpolate_background(
	CKvMatrixShort *in_image,
	CKvXrunsetShort *in_mask,
	CKvMatrixShort *out_image)
/********************************************************************************/
{
	CKvDepot_of_MatrixShort mipVec;
	CKvDepot_of_MatrixBool mipOccupancyMapVec;
	CKvMatrixBool occupancyMapTemp;
	CKvMatrixShort tmp_mip;
	CKvMatrixBool tmp_mipOccupancy;
	int miplev = 0, i = 0;

	in_mask->ex_Export(true, false, occupancyMapTemp);
	out_image->cp_Copy(in_image);

	// create coarse image by dyadic sampling
	mipVec.in_Initialize();
	mipOccupancyMapVec.in_Initialize();
	while (1)
	{
		if (miplev > 0)
		{
			CreateCoarseLayer(
				mipVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mip,
				mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
				&tmp_mipOccupancy);
		}
		else
		{
			CreateCoarseLayer(
				out_image,
				&tmp_mip,
				&occupancyMapTemp,
				&tmp_mipOccupancy);
		}

		mipVec.ap_Append(false, &tmp_mip, NULL);
		mipOccupancyMapVec.ap_Append(false, &tmp_mipOccupancy, NULL);

		if (tmp_mip.mw() <= 4 || tmp_mip.mh() <= 4) break;
		++miplev;
	}
	miplev++;

	// push phase: inpaint laplacian
	regionFill(
		mipVec.gep_Get_Element_Pointer(miplev - 1),
		mipOccupancyMapVec.gep_Get_Element_Pointer(miplev - 1),
		mipVec.gep_Get_Element_Pointer(miplev - 1));

	for (i = miplev - 1; i >= 0; --i)
	{
		if (i > 0)
		{
			regionFill(
				mipVec.gep_Get_Element_Pointer(i - 1),
				mipOccupancyMapVec.gep_Get_Element_Pointer(i - 1),
				mipVec.gep_Get_Element_Pointer(i));
		}
		else
		{
			regionFill(
				out_image,
				&occupancyMapTemp,
				mipVec.gep_Get_Element_Pointer(i));
		}
	}

}
/********************************************************************************/




/* harmonic background filling algorithm */
// interpolate using 5-point laplacian inpainting
/********************************************************************************/
void VCL_Image_Interpolation::regionFill(
	CKvMatrixUchar *io_image,
	CKvMatrixBool *in_mask,
	CKvMatrixUchar *in_downsized_image)
/********************************************************************************/
{
	int stride        = io_image->mw(); 
	int numElem       = 0;
	int numSparseElem = 0;
	std::vector<uint32_t> indexing;

	int ww_mask, hh_mask;
	int ww_io_image, hh_io_image;
	int ww_dwn_image, hh_dwn_image;

	bool **p_mask;
	unsigned char **p_io_img, **p_dwn_img;


	io_image->ms(ww_io_image, hh_io_image);
	in_mask->ms(ww_mask, hh_mask);
	in_downsized_image->ms(ww_dwn_image, hh_dwn_image);

	p_mask    = in_mask->mp();
	p_io_img  = io_image->mp();
	p_dwn_img = in_downsized_image->mp();

	indexing.resize( ww_mask * hh_mask );
	for (int j = 0; j < hh_mask; j++)
	{
		for (int i = 0; i < ww_mask ; i++)
		{
			if (p_mask[j][i] == 0)
			{
				indexing[j*ww_mask+i] = numElem;
				numElem++;
			}
		}
	}

	// create a sparse matrix with the coefficients
	std::vector<uint32_t> iSparse;
	std::vector<uint32_t> jSparse;
	std::vector<double>   valSparse;

	iSparse.resize(numElem * 5);
	jSparse.resize(numElem * 5);
	valSparse.resize(numElem * 5);

	// create an initial solution using the low-resolution
	std::vector<double> b;
	b.resize(numElem);

	// fill in the system
	int idx = 0;
	int idxSparse = 0;
	for (int row = 0; row < hh_io_image; row++)
	{
		for (int column = 0; column < ww_io_image; column++)
		{
			if (p_mask[row][column] == false)
			{
				int count = 0;
				b[idx] = 0;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if ((i == j) || (i == -j))
						{
							continue;
						}
						if ((column + j < 0) || (column + j > ww_io_image - 1))
						{
							continue;
						}
						if ((row + i < 0) || (row + i > hh_io_image - 1))
						{
							continue;
						}
						count++;

						if (p_mask[row+i][column+j] == true)
						{
							b[idx] += p_io_img[row + i][column + j];
						}
						else
						{
							iSparse[idxSparse] = idx;
							jSparse[idxSparse] = indexing[column + j + stride * (row + i)];
							valSparse[idxSparse] = -1;
							idxSparse++;
						}

					}
				}
				// now insert the weight of the center pixel
				iSparse[idxSparse] = idx;
				jSparse[idxSparse] = idx;
				valSparse[idxSparse] = count; // Matrix A
				idx++;
				idxSparse++;
			}
		}
	}
	numSparseElem = idxSparse;

	// Gauss-Seidel relaxation
	// now solve the linear system Ax=b using Gauss-Siedel relaxation, with initial guess coming from the lower
	// resolution
	std::vector<double> x;
	x.resize(numElem);
	if (in_downsized_image->mw() == io_image->mw())
	{
		// low resolution image not provided, let's use for the initialization the mean value of the active pixels
		double mean = 0.0;
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == true)
				{
					mean += double(p_io_img[row][column]);
					idx++;
				}
			}
		}

		mean /= idx;
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == false)
				{
					x[idx] = mean;
					idx++;
				}
			}
		}
	}
	else
	{
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == false)
				{
					x[idx] = p_dwn_img[(int)(row / 2)][(int)(column / 2)];
					idx++;
				}
			}
		}
	}

	int    maxIteration = 1024;
	double maxError = 0.00001;
	int it = 0;
	for (; it < maxIteration; it++)
	{
		int    idxSparse = 0;
		double error = 0;
		double val = 0;
		for (int centerIdx = 0; centerIdx < numElem; centerIdx++)
		{
			// add the b result
			val = b[centerIdx];
			while ((idxSparse < numSparseElem) && (iSparse[idxSparse] == centerIdx))
			{
				if (valSparse[idxSparse] < 0)
				{
					val += x[jSparse[idxSparse]];
					idxSparse++;
				}
				else
				{
					// final value
					val /= valSparse[idxSparse];

					// accumulate the error
					error += (val - x[centerIdx]) * (val - x[centerIdx]);

					// update the value
					x[centerIdx] = val;
					idxSparse++;
				}
			}

		}
		error = error / numElem;
		if (error < maxError) { break; }
	}
	
	
	// put the value back in the image
	idx = 0;
	for (int row = 0; row < hh_io_image; row++)
	{
		for (int column = 0; column < ww_io_image; column++)
		{
			if (p_mask[row][column] == false)
			{
				p_io_img[row][column] = (unsigned char)x[idx];
				idx++;
			}
		}
	}
}
/********************************************************************************/
void VCL_Image_Interpolation::regionFill(
	CKvMatrixUcharRgb *io_image,
	CKvMatrixBool *in_mask,
	CKvMatrixUcharRgb *in_downsized_image)
/********************************************************************************/
{
	int stride = io_image->mw();
	int numElem = 0;
	int numSparseElem = 0;
	std::vector<uint32_t> indexing;

	int ww_mask, hh_mask;
	int ww_io_image, hh_io_image;
	int ww_dwn_image, hh_dwn_image;

	bool **p_mask;
	unsigned char **p_io_r_img, **p_io_g_img, **p_io_b_img;
	unsigned char **p_dwn_r_img, **p_dwn_g_img, **p_dwn_b_img;


	io_image->ms(ww_io_image, hh_io_image);
	in_mask->ms(ww_mask, hh_mask);
	in_downsized_image->ms(ww_dwn_image, hh_dwn_image);

	p_mask    = in_mask->mp();
	p_io_r_img  = io_image->mp(p_io_g_img, p_io_b_img);
	p_dwn_r_img = in_downsized_image->mp(p_dwn_g_img, p_dwn_b_img);

	indexing.resize(ww_mask * hh_mask);
	for (int j = 0; j < hh_mask; j++)
	{
		for (int i = 0; i < ww_mask; i++)
		{
			if (p_mask[j][i] == 0)
			{
				indexing[j*ww_mask + i] = numElem;
				numElem++;
			}
		}
	}

	// create a sparse matrix with the coefficients
	std::vector<uint32_t> iSparse;
	std::vector<uint32_t> jSparse;
	std::vector<double>   valSparse;

	iSparse.resize(numElem * 5);
	jSparse.resize(numElem * 5);
	valSparse.resize(numElem * 5);

	// create an initial solution using the low-resolution
	std::vector<double> b[3];

	b[0].resize(numElem);
	b[1].resize(numElem);
	b[2].resize(numElem);

	// fill in the system
	int idx = 0;
	int idxSparse = 0;
	for (int row = 0; row < hh_io_image; row++)
	{
		for (int column = 0; column < ww_io_image; column++)
		{
			if (p_mask[row][column] == false)
			{
				int count = 0;
				b[0][idx] = 0;
				b[1][idx] = 0;
				b[2][idx] = 0;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if ((i == j) || (i == -j))
						{
							continue;
						}
						if ((column + j < 0) || (column + j > ww_io_image - 1))
						{
							continue;
						}
						if ((row + i < 0) || (row + i > hh_io_image - 1))
						{
							continue;
						}
						count++;

						if (p_mask[row + i][column + j] == true)
						{
							b[0][idx] += p_io_r_img[row + i][column + j];
							b[1][idx] += p_io_g_img[row + i][column + j];
							b[2][idx] += p_io_b_img[row + i][column + j];
						}
						else
						{
							iSparse[idxSparse] = idx;
							jSparse[idxSparse] = indexing[column + j + stride * (row + i)];
							valSparse[idxSparse] = -1;
							idxSparse++;
						}

					}
				}
				// now insert the weight of the center pixel
				iSparse[idxSparse] = idx;
				jSparse[idxSparse] = idx;
				valSparse[idxSparse] = count; // Matrix A
				idx++;
				idxSparse++;
			}
		}
	}
	numSparseElem = idxSparse;

	// Gauss-Seidel relaxation
	// now solve the linear system Ax=b using Gauss-Siedel relaxation, with initial guess coming from the lower
	// resolution
	std::vector<double> x[3];
	x[0].resize(numElem);
	x[1].resize(numElem);
	x[2].resize(numElem);

	if (in_downsized_image->mw() == io_image->mw())
	{
		// low resolution image not provided, let's use for the initialization the mean value of the active pixels
		double mean[3] = { 0.0, 0.0, 0.0 };
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == true)
				{
					mean[0] += double(p_io_r_img[row][column]);
					mean[1] += double(p_io_g_img[row][column]);
					mean[2] += double(p_io_b_img[row][column]);
					idx++;
				}
			}
		}

		mean[0] /= idx;
		mean[1] /= idx;
		mean[2] /= idx;
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == false)
				{
					x[0][idx] = mean[0];
					x[1][idx] = mean[1];
					x[2][idx] = mean[2];
					idx++;
				}
			}
		}
	}
	else
	{
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == false)
				{
					x[0][idx] = p_dwn_r_img[(int)(row / 2)][(int)(column / 2)];
					x[1][idx] = p_dwn_g_img[(int)(row / 2)][(int)(column / 2)];
					x[2][idx] = p_dwn_b_img[(int)(row / 2)][(int)(column / 2)];
					idx++;
				}
			}
		}
	}

	int    maxIteration = 1024;
	double maxError = 0.00001;
	for (int cc = 0; cc < 3; cc++)
	{
		int it = 0;
		for (; it < maxIteration; it++)
		{
			int    idxSparse = 0;
			double error = 0;
			double val = 0;
			for (int centerIdx = 0; centerIdx < numElem; centerIdx++)
			{
				// add the b result
				val = b[cc][centerIdx];
				while ((idxSparse < numSparseElem) && (iSparse[idxSparse] == centerIdx))
				{
					if (valSparse[idxSparse] < 0)
					{
						val += x[cc][jSparse[idxSparse]];
						idxSparse++;
					}
					else
					{
						// final value
						val /= valSparse[idxSparse];

						// accumulate the error
						error += (val - x[cc][centerIdx]) * (val - x[cc][centerIdx]);

						// update the value
						x[cc][centerIdx] = val;
						idxSparse++;
					}
				}

			}
			error = error / numElem;
			if (error < maxError) { break; }
		}
	}



	// put the value back in the image
	idx = 0;
	for (int row = 0; row < hh_io_image; row++)
	{
		for (int column = 0; column < ww_io_image; column++)
		{
			if (p_mask[row][column] == false)
			{
				p_io_r_img[row][column] = (unsigned char)x[0][idx];
				p_io_g_img[row][column] = (unsigned char)x[1][idx];
				p_io_b_img[row][column] = (unsigned char)x[2][idx];
				idx++;
			}
		}
	}
}
/********************************************************************************/
void VCL_Image_Interpolation::regionFill(
	CKvMatrixShort *io_image,
	CKvMatrixBool *in_mask,
	CKvMatrixShort *in_downsized_image)
/********************************************************************************/
{
	int stride = io_image->mw();
	int numElem = 0;
	int numSparseElem = 0;
	std::vector<uint32_t> indexing;

	int ww_mask, hh_mask;
	int ww_io_image, hh_io_image;
	int ww_dwn_image, hh_dwn_image;

	bool **p_mask;
	short **p_io_img, **p_dwn_img;


	io_image->ms(ww_io_image, hh_io_image);
	in_mask->ms(ww_mask, hh_mask);
	in_downsized_image->ms(ww_dwn_image, hh_dwn_image);

	p_mask    = in_mask->mp();
	p_io_img  = io_image->mp();
	p_dwn_img = in_downsized_image->mp();

	indexing.resize(ww_mask * hh_mask);
	for (int j = 0; j < hh_mask; j++)
	{
		for (int i = 0; i < ww_mask; i++)
		{
			if (p_mask[j][i] == 0)
			{
				indexing[j*ww_mask + i] = numElem;
				numElem++;
			}
		}
	}

	// create a sparse matrix with the coefficients
	std::vector<uint32_t> iSparse;
	std::vector<uint32_t> jSparse;
	std::vector<double>   valSparse;

	iSparse.resize(numElem * 5);
	jSparse.resize(numElem * 5);
	valSparse.resize(numElem * 5);

	// create an initial solution using the low-resolution
	std::vector<double> b;
	b.resize(numElem);

	// fill in the system
	int idx = 0;
	int idxSparse = 0;
	for (int row = 0; row < hh_io_image; row++)
	{
		for (int column = 0; column < ww_io_image; column++)
		{
			if (p_mask[row][column] == false)
			{
				int count = 0;
				b[idx] = 0;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if ((i == j) || (i == -j))
						{
							continue;
						}
						if ((column + j < 0) || (column + j > ww_io_image - 1))
						{
							continue;
						}
						if ((row + i < 0) || (row + i > hh_io_image - 1))
						{
							continue;
						}
						count++;

						if (p_mask[row + i][column + j] == true)
						{
							b[idx] += p_io_img[row + i][column + j];
						}
						else
						{
							iSparse[idxSparse] = idx;
							jSparse[idxSparse] = indexing[column + j + stride * (row + i)];
							valSparse[idxSparse] = -1;
							idxSparse++;
						}

					}
				}
				// now insert the weight of the center pixel
				iSparse[idxSparse] = idx;
				jSparse[idxSparse] = idx;
				valSparse[idxSparse] = count; // Matrix A
				idx++;
				idxSparse++;
			}
		}
	}
	numSparseElem = idxSparse;

	// Gauss-Seidel relaxation
	// now solve the linear system Ax=b using Gauss-Siedel relaxation, with initial guess coming from the lower
	// resolution
	std::vector<double> x;
	x.resize(numElem);
	if (in_downsized_image->mw() == io_image->mw())
	{
		// low resolution image not provided, let's use for the initialization the mean value of the active pixels
		double mean = 0.0;
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == true)
				{
					mean += double(p_io_img[row][column]);
					idx++;
				}
			}
		}

		mean /= idx;
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == false)
				{
					x[idx] = mean;
					idx++;
				}
			}
		}
	}
	else
	{
		idx = 0;
		for (int row = 0; row < hh_io_image; row++)
		{
			for (int column = 0; column < ww_io_image; column++)
			{
				if (p_mask[row][column] == false)
				{
					x[idx] = p_dwn_img[(int)(row / 2)][(int)(column / 2)];
					idx++;
				}
			}
		}
	}

	int    maxIteration = 1024;
	double maxError = 0.00001;
	int it = 0;
	for (; it < maxIteration; it++)
	{
		int    idxSparse = 0;
		double error = 0;
		double val = 0;
		for (int centerIdx = 0; centerIdx < numElem; centerIdx++)
		{
			// add the b result
			val = b[centerIdx];
			while ((idxSparse < numSparseElem) && (iSparse[idxSparse] == centerIdx))
			{
				if (valSparse[idxSparse] < 0)
				{
					val += x[jSparse[idxSparse]];
					idxSparse++;
				}
				else
				{
					// final value
					val /= valSparse[idxSparse];

					// accumulate the error
					error += (val - x[centerIdx]) * (val - x[centerIdx]);

					// update the value
					x[centerIdx] = val;
					idxSparse++;
				}
			}

		}
		error = error / numElem;
		if (error < maxError) { break; }
	}


	// put the value back in the image
	idx = 0;
	for (int row = 0; row < hh_io_image; row++)
	{
		for (int column = 0; column < ww_io_image; column++)
		{
			if (p_mask[row][column] == false)
			{
				p_io_img[row][column] = (short)x[idx];
				idx++;
			}
		}
	}
}
/********************************************************************************/
void VCL_Image_Interpolation::CreateCoarseLayer(
	CKvMatrixUchar *in_image,
	CKvMatrixUchar *out_mip,
	CKvMatrixBool *in_occupancyMap,
	CKvMatrixBool *out_mipOccupancyMap)
/********************************************************************************/
{
	int dyadicWidth = 1;
	while (dyadicWidth < in_image->mw()) dyadicWidth *= 2;
	int dyadicHeight = 1;
	while (dyadicHeight < in_image->mh()) dyadicHeight *= 2;

	// allocate the mipmap with half the resolution
	out_mip->c_Create((dyadicHeight / 2), (dyadicWidth / 2));
	out_mipOccupancyMap->c_Create((dyadicHeight / 2), (dyadicWidth / 2),false);

	bool **p_occupancyMap = in_occupancyMap->mp();
	bool **p_mipoccupancyMap = out_mipOccupancyMap->mp();
	unsigned char **p_in_image = in_image->mp();
	unsigned char **p_out_mip = out_mip->mp();
	int    stride = in_image->mw();
	int    newStride = (dyadicWidth / 2);
	int    x, y, i, j;
	double num, den;
	for (y = 0; y < out_mip->mh(); y++)
	{
		for (x = 0; x < out_mip->mw(); x++)
		{
			num = 0;
			den = 0;
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 2; j++)
				{
					int row =
						(2 * y + i) < 0 ? 0 : (2 * y + i) >= in_image->mh() ? in_image->mh() - 1 : (2 * y + i);
					int column =
						(2 * x + j) < 0 ? 0 : (2 * x + j) >= in_image->mw() ? in_image->mw() - 1 : (2 * x + j);
					if (p_occupancyMap[row][column] == true)
					{
						den++;
						num += p_in_image[row][column];
					}
				}
			}
			if (den > 0)
			{
				p_mipoccupancyMap[y][x] = true;
				p_out_mip[y][x] = (unsigned char)std::round(num / den);
			}
		}
	}
}
/********************************************************************************/
void VCL_Image_Interpolation::CreateCoarseLayer(
	CKvMatrixUcharRgb *in_image,
	CKvMatrixUcharRgb *out_mip,
	CKvMatrixBool *in_occupancyMap,
	CKvMatrixBool *out_mipOccupancyMap)
/********************************************************************************/
{
	int dyadicWidth = 1;
	while (dyadicWidth < in_image->mw()) dyadicWidth *= 2;
	int dyadicHeight = 1;
	while (dyadicHeight < in_image->mh()) dyadicHeight *= 2;

	// allocate the mipmap with half the resolution
	out_mip->c_Create((dyadicHeight / 2), (dyadicWidth / 2));
	out_mipOccupancyMap->c_Create((dyadicHeight / 2), (dyadicWidth / 2), false);

	bool **p_occupancyMap = in_occupancyMap->mp();
	bool **p_mipoccupancyMap = out_mipOccupancyMap->mp();

	unsigned char **p_in_image_r, **p_in_image_g, **p_in_image_b;
	unsigned char **p_out_mip_r, **p_out_mip_g, **p_out_mip_b;

	p_in_image_r = in_image->mp(p_in_image_g, p_in_image_b);
	p_out_mip_r = out_mip->mp(p_out_mip_g, p_out_mip_b);

	int    stride = in_image->mw();
	int    newStride = (dyadicWidth / 2);
	int    x, y, i, j;
	double num[3], den;


	for (y = 0; y < out_mip->mh(); y++)
	{
		for (x = 0; x < out_mip->mw(); x++)
		{
			num[0] = 0;
			num[1] = 0;
			num[2] = 0;
			den = 0;
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 2; j++)
				{
					int row =
						(2 * y + i) < 0 ? 0 : (2 * y + i) >= in_image->mh() ? in_image->mh() - 1 : (2 * y + i);
					int column =
						(2 * x + j) < 0 ? 0 : (2 * x + j) >= in_image->mw() ? in_image->mw() - 1 : (2 * x + j);
					if (p_occupancyMap[row][column] == true)
					{
						den++;
						//num += p_in_image[row][column];
						num[0] += p_in_image_r[row][column];
						num[1] += p_in_image_g[row][column];
						num[2] += p_in_image_b[row][column];
					}
				}
			}
			if (den > 0)
			{
				p_mipoccupancyMap[y][x] = true;
				//p_out_mip[y][x] = (unsigned char)std::round(num / den);
				p_out_mip_r[y][x] = (unsigned char)std::round(num[0] / den);
				p_out_mip_g[y][x] = (unsigned char)std::round(num[1] / den);
				p_out_mip_b[y][x] = (unsigned char)std::round(num[2] / den);
			}
		}
	}


}
/********************************************************************************/
void VCL_Image_Interpolation::CreateCoarseLayer(
	CKvMatrixShort *in_image,
	CKvMatrixShort *out_mip,
	CKvMatrixBool *in_occupancyMap,
	CKvMatrixBool *out_mipOccupancyMap)
/********************************************************************************/
{
	int dyadicWidth = 1;
	while (dyadicWidth < in_image->mw()) dyadicWidth *= 2;
	int dyadicHeight = 1;
	while (dyadicHeight < in_image->mh()) dyadicHeight *= 2;

	// allocate the mipmap with half the resolution
	out_mip->c_Create((dyadicHeight / 2), (dyadicWidth / 2));
	out_mipOccupancyMap->c_Create((dyadicHeight / 2), (dyadicWidth / 2), false);

	bool **p_occupancyMap = in_occupancyMap->mp();
	bool **p_mipoccupancyMap = out_mipOccupancyMap->mp();
	short **p_in_image = in_image->mp();
	short **p_out_mip = out_mip->mp();
	int    stride = in_image->mw();
	int    newStride = (dyadicWidth / 2);
	int    x, y, i, j;
	double num, den;
	for (y = 0; y < out_mip->mh(); y++)
	{
		for (x = 0; x < out_mip->mw(); x++)
		{
			num = 0;
			den = 0;
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 2; j++)
				{
					int row =
						(2 * y + i) < 0 ? 0 : (2 * y + i) >= in_image->mh() ? in_image->mh() - 1 : (2 * y + i);
					int column =
						(2 * x + j) < 0 ? 0 : (2 * x + j) >= in_image->mw() ? in_image->mw() - 1 : (2 * x + j);
					if (p_occupancyMap[row][column] == true)
					{
						den++;
						num += p_in_image[row][column];
					}
				}
			}
			if (den > 0)
			{
				p_mipoccupancyMap[y][x] = true;
				p_out_mip[y][x] = (short)std::round(num / den);
			}
		}
	}
}
/********************************************************************************/




















// Prototype
/********************************************************************************/
//template <typename T>
//void VCL_Image_Interpolation::dilateHarmonicBackgroundFill(
//	PCCFrameContext& frame,
//	PCCImage<T, 3>& image)
///********************************************************************************/
//{
//	auto                               occupancyMapTemp = frame.getOccupancyMap();
//	int                                i = 0;
//	std::vector<PCCImage<T, 3>>        mipVec;
//	std::vector<std::vector<uint32_t>> mipOccupancyMapVec;
//	int                                miplev = 0;
//
//	// create coarse image by dyadic sampling
//	while (1)
//	{
//		mipVec.resize(mipVec.size() + 1);
//		mipOccupancyMapVec.resize(mipOccupancyMapVec.size() + 1);
//		if (miplev > 0)
//		{
//			CreateCoarseLayer(
//				mipVec[miplev - 1],
//				mipVec[miplev],
//				mipOccupancyMapVec[miplev - 1],
//				mipOccupancyMapVec[miplev]);
//		}
//		else
//		{
//			CreateCoarseLayer(
//				image,
//				mipVec[miplev],
//				occupancyMapTemp,
//				mipOccupancyMapVec[miplev]);
//		}
//
//		if (mipVec[miplev].getWidth() <= 4 || mipVec[miplev].getHeight() <= 4) break;
//		++miplev;
//	}
//	miplev++;
//	// push phase: inpaint laplacian
//	regionFill(
//		mipVec[miplev - 1],
//		mipOccupancyMapVec[miplev - 1],
//		mipVec[miplev - 1]);
//	for (i = miplev - 1; i >= 0; --i)
//	{
//		if (i > 0)
//		{
//			regionFill(
//				mipVec[i - 1],
//				mipOccupancyMapVec[i - 1],
//				mipVec[i]);
//		}
//		else
//		{
//			regionFill(
//				image,
//				occupancyMapTemp,
//				mipVec[i]);
//		}
//	}
//}
/********************************************************************************/
//template <typename T>
//void VCL_Image_Interpolation::CreateCoarseLayer(
//	PCCImage<T, 3> & image,
//	PCCImage<T, 3> & mip,
//	std::vector<uint32_t> & occupancyMap,
//	std::vector<uint32_t> & mipOccupancyMap)
///********************************************************************************/
//{
//	int dyadicWidth = 1;
//	while (dyadicWidth < image.getWidth()) dyadicWidth *= 2;
//	int dyadicHeight = 1;
//	while (dyadicHeight < image.getHeight()) dyadicHeight *= 2;
//
//	// allocate the mipmap with half the resolution
//	mip.resize((dyadicWidth / 2), (dyadicHeight / 2));
//	mipOccupancyMap.resize((dyadicWidth / 2) * (dyadicHeight / 2), 0);
//	int    stride = image.getWidth();
//	int    newStride = (dyadicWidth / 2);
//	int    x, y, i, j;
//	double num[3], den;
//	for (y = 0; y < mip.getHeight(); y++)
//	{
//		for (x = 0; x < mip.getWidth(); x++)
//		{
//			num[0] = 0;
//			num[1] = 0;
//			num[2] = 0;
//			den = 0;
//			for (i = 0; i < 2; i++)
//			{
//				for (j = 0; j < 2; j++)
//				{
//					int row =
//						(2 * y + i) < 0 ? 0 : (2 * y + i) >= image.getHeight() ? image.getHeight() - 1 : (2 * y + i);
//					int column =
//						(2 * x + j) < 0 ? 0 : (2 * x + j) >= image.getWidth() ? image.getWidth() - 1 : (2 * x + j);
//					if (occupancyMap[column + stride * row] == 1)
//					{
//						den++;
//						for (int cc = 0; cc < 3; cc++)
//						{
//							num[cc] += image.getValue(cc, column, row);
//						}
//					}
//				}
//			}
//			if (den > 0)
//			{
//				mipOccupancyMap[x + newStride * y] = 1;
//				for (int cc = 0; cc < 3; cc++)
//				{
//					mip.setValue(cc, x, y, std::round(num[cc] / den));
//				}
//			}
//		}
//	}
//}
/********************************************************************************/
//template <typename T>
//void VCL_Image_Interpolation::regionFill(
//	PCCImage<T, 3> & image,
//	std::vector<uint32_t> & occupancyMap,
//	PCCImage<T, 3> & imageLowRes)
///********************************************************************************/
//{
//	int                   stride = image.getWidth();
//	int                   numElem = 0;
//	int                   numSparseElem = 0;
//	std::vector<uint32_t> indexing;
//
//
//	indexing.resize(occupancyMap.size());
//	for (int i = 0; i < occupancyMap.size(); i++)
//	{
//		if (occupancyMap[i] == 0)
//		{
//			indexing[i] = numElem;
//			numElem++;
//		}
//	}
//
//	// create a sparse matrix with the coefficients
//	std::vector<uint32_t> iSparse;
//	std::vector<uint32_t> jSparse;
//	std::vector<double>   valSparse;
//
//	iSparse.resize(numElem * 5);
//	jSparse.resize(numElem * 5);
//	valSparse.resize(numElem * 5);
//
//	// create an initial solution using the low-resolution
//	std::vector<double> b[3];
//	b[0].resize(numElem);
//	b[1].resize(numElem);
//	b[2].resize(numElem);
//
//	// fill in the system
//	int idx = 0;
//	int idxSparse = 0;
//	for (int row = 0; row < image.getHeight(); row++)
//	{
//		for (int column = 0; column < image.getWidth(); column++)
//		{
//			if (occupancyMap[column + stride * row] == 0)
//			{
//				int count = 0;
//				b[0][idx] = 0;
//				b[1][idx] = 0;
//				b[2][idx] = 0;
//				for (int i = -1; i < 2; i++)
//				{
//					for (int j = -1; j < 2; j++)
//					{
//						if ((i == j) || (i == -j))
//						{
//							continue;
//						}
//						if ((column + j < 0) || (column + j > image.getWidth() - 1))
//						{
//							continue;
//						}
//						if ((row + i < 0) || (row + i > image.getHeight() - 1))
//						{
//							continue;
//						}
//						count++;
//
//						if (occupancyMap[column + j + stride * (row + i)] == 1)
//						{
//							b[0][idx] += image.getValue(0, column + j, row + i);
//							b[1][idx] += image.getValue(1, column + j, row + i);
//							b[2][idx] += image.getValue(2, column + j, row + i);
//						}
//						else
//						{
//							iSparse[idxSparse] = idx;
//							jSparse[idxSparse] = indexing[column + j + stride * (row + i)];
//							valSparse[idxSparse] = -1;
//							idxSparse++;
//						}
//
//					}
//				}
//				// now insert the weight of the center pixel
//				iSparse[idxSparse] = idx;
//				jSparse[idxSparse] = idx;
//				valSparse[idxSparse] = count; // Matrix A
//				idx++;
//				idxSparse++;
//			}
//		}
//	}
//	numSparseElem = idxSparse;
//
//	// Gauss-Seidel relaxation
//	// now solve the linear system Ax=b using Gauss-Siedel relaxation, with initial guess coming from the lower
//	// resolution
//	std::vector<double> x[3];
//	x[0].resize(numElem);
//	x[1].resize(numElem);
//	x[2].resize(numElem);
//	if (imageLowRes.getWidth() == image.getWidth())
//	{
//		// low resolution image not provided, let's use for the initialization the mean value of the active pixels
//		double mean[3] = { 0.0, 0.0, 0.0 };
//		idx = 0;
//		for (int row = 0; row < image.getHeight(); row++)
//		{
//			for (int column = 0; column < image.getWidth(); column++)
//			{
//				if (occupancyMap[column + stride * row] == 1)
//				{
//					mean[0] += double(image.getValue(0, column, row));
//					mean[1] += double(image.getValue(1, column, row));
//					mean[2] += double(image.getValue(2, column, row));
//					idx++;
//				}
//			}
//		}
//
//		mean[0] /= idx;
//		mean[1] /= idx;
//		mean[2] /= idx;
//		idx = 0;
//		for (int row = 0; row < image.getHeight(); row++) {
//			for (int column = 0; column < image.getWidth(); column++) {
//				if (occupancyMap[column + stride * row] == 0) {
//					x[0][idx] = mean[0];
//					x[1][idx] = mean[1];
//					x[2][idx] = mean[2];
//					idx++;
//				}
//			}
//		}
//	}
//	else
//	{
//		idx = 0;
//		for (int row = 0; row < image.getHeight(); row++)
//		{
//			for (int column = 0; column < image.getWidth(); column++)
//			{
//				if (occupancyMap[column + stride * row] == 0)
//				{
//					x[0][idx] = imageLowRes.getValue(0, column / 2, row / 2);
//					x[1][idx] = imageLowRes.getValue(1, column / 2, row / 2);
//					x[2][idx] = imageLowRes.getValue(2, column / 2, row / 2);
//					idx++;
//				}
//			}
//		}
//	}
//
//	int    maxIteration = 1024;
//	double maxError = 0.00001;
//	for (int cc = 0; cc < 3; cc++)
//	{
//		int it = 0;
//		for (; it < maxIteration; it++)
//		{
//			int    idxSparse = 0;
//			double error = 0;
//			double val = 0;
//			for (int centerIdx = 0; centerIdx < numElem; centerIdx++)
//			{
//				// add the b result
//				val = b[cc][centerIdx];
//				while ((idxSparse < numSparseElem) && (iSparse[idxSparse] == centerIdx))
//				{
//					if (valSparse[idxSparse] < 0)
//					{
//						val += x[cc][jSparse[idxSparse]];
//						idxSparse++;
//					}
//					else
//					{
//						// final value
//						val /= valSparse[idxSparse];
//
//						// accumulate the error
//						error += (val - x[cc][centerIdx]) * (val - x[cc][centerIdx]);
//
//						// update the value
//						x[cc][centerIdx] = val;
//						idxSparse++;
//					}
//				}
//
//			}
//			error = error / numElem;
//			if (error < maxError) { break; }
//		}
//	}
//
//	// put the value back in the image
//	idx = 0;
//	for (int row = 0; row < image.getHeight(); row++) {
//		for (int column = 0; column < image.getWidth(); column++) {
//			if (occupancyMap[column + stride * row] == 0) {
//				image.setValue(0, column, row, x[0][idx]);
//				image.setValue(1, column, row, x[1][idx]);
//				image.setValue(2, column, row, x[2][idx]);
//				idx++;
//			}
//		}
//	}
//}
/********************************************************************************/

