#include "stdafx.h"
#include "JANG_Xrunset3dShort.h"

/*******************************************************/
JANG_Xrunset3dShort::JANG_Xrunset3dShort()
/*******************************************************/
{

}
/*******************************************************/
JANG_Xrunset3dShort::~JANG_Xrunset3dShort()
/*******************************************************/
{

}
/*******************************************************/
void JANG_Xrunset3dShort::jimport(
	CKvVolumeBool *in_volume)
/*******************************************************/
{
	CKvSet2d_of_VectorBool set2d_bool; CKvVectorBool **p_bool;
	CKvXrunset1dShort **px;
	int ww, hh, dd, j, i, k;
	bool ***p, *pvec;

	CKvStopWatch sw;
	sw.c_Create(1);

	sw.r_Reset(0);
	p      = in_volume->tps(ww, hh, dd);
	p_bool = set2d_bool.c_Create(hh, ww);
	printf("			[jimport] : set memory : %lf\n", sw.get_Get_Elapsed_Time(0));
	

	sw.r_Reset(0);
#pragma omp parallel num_threads(8)
	{
#pragma omp for private(i)
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				p_bool[j][i].c_Create(dd);
			}
		}
	}
	printf("			[jimport] : Create VectorBools : %lf\n", sw.get_Get_Elapsed_Time(0));

	sw.r_Reset(0);
#pragma omp parallel num_threads(8)
	{
#pragma omp for private(i) private(k) private(pvec)
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				pvec = p_bool[j][i].vp();
				for (k = 0; k < dd; k++)
				{
					pvec[k] = p[k][j][i];
					//p_bool[j][i].se_Set_Element(k, p[k][j][i]);
				}
			}
		}
	}
	printf("			[jimport] : Set VectorBools : %lf\n", sw.get_Get_Elapsed_Time(0));

	sw.r_Reset(0);
	px = zz_space.c_Create(hh, ww);
	printf("			[jimport] : set memory zz_space : %lf\n", sw.get_Get_Elapsed_Time(0));



	sw.r_Reset(0);
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			px[j][i].im_Import(p_bool[j][i]);
		}
	}
	printf("			[jimport] : import VectorBool to zz_space : %lf\n", sw.get_Get_Elapsed_Time(0));
}
/*******************************************************/
void JANG_Xrunset3dShort::jimport(
	CKvVolumeBool *in_volume,
	int in_xmin,
	int in_xmanx,
	int in_ymin,
	int in_ymax)
/*******************************************************/
{
	CKvSet2d_of_VectorBool set2d_bool; CKvVectorBool **p_bool;
	CKvXrunset1dShort **px;
	int ww, hh, dd, j, i, k;
	bool ***p, *pvec;


	p = in_volume->tps(ww, hh, dd);
	p_bool = set2d_bool.c_Create(hh, ww);


#pragma omp parallel num_threads(8)
	{
#pragma omp for private(i)
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				p_bool[j][i].c_Create(dd,false);
			}
		}
	}

#pragma omp parallel num_threads(8)
	{
#pragma omp for private(i) private(k) private(pvec)
		for (j = in_ymin; j <= in_ymax; j++)
		{
			for (i = in_xmin; i <= in_xmanx; i++)
			{
				pvec = p_bool[j][i].vp();
				for (k = 0; k < dd; k++)
				{
					pvec[k] = p[k][j][i];
				}
			}
		}
	}

	px = zz_space.c_Create(hh, ww);

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			px[j][i].im_Import(p_bool[j][i]);
		}
	}
}
/*******************************************************/
void JANG_Xrunset3dShort::jimport(
	CKvSet2d_of_XvectorShort *in_xvector, 
	int in_dd)
/*******************************************************/
{
	CKvXrunset1dShort **px;
	CKvXvectorShort **p;
	int ww, hh, j, i;


	p = in_xvector->mps(ww, hh);
	px = zz_space.c_Create(hh, ww);

#pragma omp parallel num_threads(8)
	{
#pragma omp for private(i)
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				px[j][i].im_Import(p[j][i], in_dd);
			}
		}
	}

}
/*******************************************************/
void JANG_Xrunset3dShort::convert(
	bool ***in_volume_pointer,
	int in_hh,
	int in_ww,
	int in_max_dd,
	CKvXrunset1dShort *out_xrunset)
/*******************************************************/
{
	CKvVectorBool tmp;
	int k; bool *pb;
	
	pb = tmp.c_Create(in_max_dd);
	for (k = 0; k < in_max_dd; k++)
	{
		pb[k] = in_volume_pointer[k][in_hh][in_ww];
	}

	out_xrunset->im_Import(tmp);
}
/*******************************************************/
void JANG_Xrunset3dShort::Update(
	CKvMatrixBool *in_slice,
	int in_plane_mode_0X_1Y_2Z_3zy,
	int in_coordinate_at_the_corresponding_axis)
/*******************************************************/
{
	LCKvUtility_for_Xrunset1d lux1d;
	CKvXrunsetShort xrunset; 
	CKvXrunset1dShort xrunset1d, **p_xrunset1d;
	int ww, hh, j, i;

	in_slice->ms(ww, hh);
	p_xrunset1d = zz_space.mp();

	if (in_plane_mode_0X_1Y_2Z_3zy == 0)
	{
		xrunset.im_Import(*in_slice);
		for (j = 0; j < hh; j++)
		{
			xrunset.gr_Get_Row(j, xrunset1d);
			lux1d.eo_Exclusive_OR(
				xrunset1d,
				p_xrunset1d[j][in_coordinate_at_the_corresponding_axis],
				p_xrunset1d[j][in_coordinate_at_the_corresponding_axis]);
		}
	}
	else if (in_plane_mode_0X_1Y_2Z_3zy == 1)
	{
		xrunset.im_Import(*in_slice);
		for (i = 0; i < ww; i++)
		{
			xrunset.gr_Get_Row(i, xrunset1d);
			lux1d.eo_Exclusive_OR(
				xrunset1d,
				p_xrunset1d[in_coordinate_at_the_corresponding_axis][i],
				p_xrunset1d[in_coordinate_at_the_corresponding_axis][i]);
		}
	}
	else if (in_plane_mode_0X_1Y_2Z_3zy == 2)
	{
		CKvVectorBool pattern;
		bool *p_pattern = pattern.c_Create(hh, false);

		p_pattern[in_coordinate_at_the_corresponding_axis] = true;
		xrunset1d.im_Import(pattern);

		bool **p_mask = in_slice->mp();
		for (j = 0; j < hh; j++)
		{
			for (i = 0; i < ww; i++)
			{
				if (p_mask[j][i] == true)
				{
					lux1d.eo_Exclusive_OR(
						xrunset1d,
						p_xrunset1d[j][i],
						p_xrunset1d[j][i]);
				}
			}
		}
	}


}
/*******************************************************/






