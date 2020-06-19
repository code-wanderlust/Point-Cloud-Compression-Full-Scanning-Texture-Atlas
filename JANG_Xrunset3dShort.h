#pragma once



class JANG_Xrunset3dShort : public CKvXrunset3dShort
{
public:
	JANG_Xrunset3dShort();
	~JANG_Xrunset3dShort();

	void jimport(
		CKvVolumeBool *in_volume);
	void jimport(
		CKvVolumeBool *in_volume, 
		int in_xmin,
		int in_xmanx,
		int in_ymin,
		int in_ymax);
	void jimport(
		CKvSet2d_of_XvectorShort *in_xvector, 
		int dd);

	void convert(
		bool ***in_volume_pointer,
		int in_hh,
		int in_ww,
		int in_max_dd,
		CKvXrunset1dShort *out_xrunset);

	void Update(
		CKvMatrixBool *in_slice,
		int in_plane_mode_0X_1Y_2Z_3zy,
		int in_coordinate_at_the_corresponding_axis);

};
