#include "stdafx.h"

/*********************************************************/
bool lm_Load_mesh(
	bool in_mode_texture,
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvMesh_of_Triangle *out_mesh,
	CKvMatrixInt *out_N_by_3_mesh,
	CKvDepot_of_Pointf *out_point2d_or_NULL,
	CKvMesh_of_TriImage *out_triImage_or_NULL);
/*********************************************************/
bool lm_Load_mesh(
	bool in_mode_texture,
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvMesh_of_Triangle *out_mesh,
	CKvMatrixInt *out_N_by_3_mesh,
	CKvDepot_of_Pointf *out_point2d_or_NULL,
	CKvMesh_of_TriImage *out_triImage_or_NULL,
	CKvMatrix *out_vertex_normal_vectors_or_NULL);
/*********************************************************/
bool lm_Load_mesh_for_Vertex_Color(
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvMesh_of_Triangle *out_mesh,
	CKvMatrixInt *out_N_by_3_mesh,
	CKvDepot_of_RgbaF *out_depot_color);
/*********************************************************/
bool lm_Load_Voxel(
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvDepot_of_RgbaF *out_depot_color);
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvXrunset3dShort *out_set_of_voxels,
	CKvSet2d_of_XvectorUcharRgb *out_set_colors);
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_set_of_voxels,
	CKvDepot_of_RgbaF *out_set_colors);
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_set_of_voxels,
	CKvDepot_of_Rgb *out_set_colors);
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color2(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_set_of_voxels,
	CKvDepot_of_RgbaF *out_set_colors);
/*********************************************************/
bool mtd_Mesh_To_DoCube(
	CKvDepot_of_Point3Df *in_point3d,
	CKvMatrixInt *in_N_by_3_mesh,
	int in_ww,
	int in_hh,
	int in_dd,
	CKvSet_of_Pmatrix3D *io_pmats,
	VCL_DoCube *out_Docube,
	CKvDepot_of_Point3Df *out_DoCube_point3d,
	CKvMatrixInt *out_N_by_3_DoCube_mesh,
	CKvMesh_of_Triangle *out_mesh_triangle,
	Util_Mesh_Generation *io_mesh_generation);
/*********************************************************/
bool mtd_Mesh_To_DoCube(
	CKvDepot_of_Point3Df *in_point3d,
	CKvMatrixInt *in_N_by_3_mesh,
	int in_ww,
	int in_hh,
	int in_dd,
	float in_ang_x,
	float in_ang_y,
	float in_ang_z,
	CKvSet_of_Pmatrix3D *io_pmats,
	VCL_DoCube *out_Docube);
/*********************************************************/
bool mtd_Mesh_To_DoCube(
	CKvDepot_of_Point3Df *in_point3d,
	CKvMatrixInt *in_N_by_3_mesh,
	int in_ww,
	int in_hh,
	int in_dd,
	float in_ang_x,
	float in_ang_y,
	float in_ang_z,
	CKvSet_of_Pmatrix3D *io_pmats,
	CKvMatrix *out_homography,
	VCL_DoCube *out_Docube);
/*********************************************************/
bool mtd_Mesh_To_DoCube(
	CKvDepot_of_Point3Df *in_point3d,
	CKvMatrixInt *in_N_by_3_mesh,
	int in_ww,
	int in_hh,
	int in_dd,
	float in_ang_x,
	float in_ang_y,
	float in_ang_z,
	VCL_DoCube *out_Docube);
/*********************************************************/

bool vpc_Voxelize_Point_Cloud(
	CKvDepot_of_Point3Df *in_point_cloud,
	CKvDepot_of_RgbaF *in_point_cloud_color,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_voxel,
	CKvDepot_of_RgbaF *out_voxel_color);



/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
bool r3p_Rotate_3D_Points(
	CKvDepot_of_Point3Df *in_object3d,
	CKvDepot_of_Point3Df *out_object3d);
/*********************************************************/
bool Mesh_of_Triangle_To_CKvMatrix(
	bool in_mode_tri_tetra_true_or_only_tri_false,
	CKvMesh_of_Triangle *in_mesh,
	CKvMatrixInt *out_N_by_3_mesh);
/*********************************************************/
bool CKvMatrix_To_Mesh_of_Triangle(
	CKvMatrixInt *in_N_by_3_mesh,
	CKvMesh_of_Triangle *out_mesh);
/*********************************************************/



/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
bool sat_Save_as_Txt(
	CKvString in_filename,
	CKvVectorInt *in_vector);
/*********************************************************/
bool sat_Save_as_Txt(
	CKvString in_filename,
	CKvMatrixInt *in_matrix);
/*********************************************************/
bool sat_Save_as_Txt(
	CKvString in_filename,
	CKvMatrixFloat *in_matrix);
/*********************************************************/
bool sat_Save_as_Txt_Normal_Map(
	CKvString in_filename,
	CKvMatrixFloatRgb *in_normal);
/*********************************************************/
bool sao_Save_as_Obj(
	CKvString in_filename,
	CKvSet_of_Point3D *in_points,
	CKvMatrixInt *in_mesh);
/*********************************************************/
bool sao_Save_as_Obj(
	CKvString in_filename,
	CKvSet_of_Point3D *in_points,
	CKvDepot_of_RgbaF *in_vertex_colors,
	CKvMatrixInt *in_mesh);
/*********************************************************/
bool sao_Save_as_Obj(
	CKvString in_filename,
	CKvSet_of_Voxel *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL,
	CKvMatrixInt *in_mesh_or_NULL);
/*********************************************************/
bool sap_Save_as_Ply(
	CKvString in_filename,
	CKvSet_of_Voxel *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL);
/*********************************************************/
bool sap_Save_as_Ply(
	CKvString in_filename,
	CKvSet_of_Point3Df *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL);
/*********************************************************/
bool sap_Save_as_Ply(
	CKvString in_filename,
	CKvSet_of_Point3D *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL);
/*********************************************************/
void g3pouac_Get_3D_Point_Object_Using_All_Camera(
	CKvSet_of_Pmatrix3D *in_set_of_P_matrices,
	CKvSet_of_SdkCode *in_set_of_silhouette_images,
	CKvSet_of_Point3D *out_3D_Points);
/*********************************************************/


