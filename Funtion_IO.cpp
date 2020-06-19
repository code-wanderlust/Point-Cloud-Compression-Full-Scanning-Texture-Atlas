
#include "stdafx.h"
#include "Funtion_IO.h"

/*********************************************************/
bool lm_Load_mesh(
	bool in_mode_texture,
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvMesh_of_Triangle *out_mesh,
	CKvMatrixInt *out_N_by_3_mesh,
	CKvDepot_of_Pointf *out_point2d_or_NULL,
	CKvMesh_of_TriImage *out_triImage_or_NULL)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvString *p_mesh;
	CKvString temp;
	int lengh;
	float x, y, z;
	int idx[3];
	int i;
	char t_char, t_char2;
	bool exist_texture;

	CKvSet_of_Point3D out_set_vertex;
	CKvSet_of_VectorInt out_set_face;
	CKvSet_of_Point out_set_texturept;
	CKvSet_of_VectorInt out_set_textureidx;

	CKvPoint3D *p_vertex;
	CKvPoint *p_texturept=NULL;
	CKvVectorInt *p_face;
	CKvVectorInt *p_textureidx=NULL;

	int num_vertex, num_face, num_texturept;
	int offset[3];
	CKvSet_of_VectorInt element;

	if (!(io_file.ltf_Load_from_Text_File(in_filename, true, &t_mesh)))
		return false;

	// Initialization
	p_mesh = t_mesh.vps(lengh);
	num_face = 0; num_vertex = 0; num_texturept = 0;

	//count numbers
	for (i = 0; i<lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);

		if (temp.ie_Is_Equal("#"))
			continue;

		if (p_mesh[i].l_Left(2).ie_Is_Equal("vt"))
		{
			num_texturept++;
		}
		else if (temp.ie_Is_Equal("v"))
		{
			num_vertex++;
		}
		else if (temp.ie_Is_Equal("f"))
		{
			num_face++;
		}
	}

	exist_texture = (num_texturept > 0);

	//Initialize and reset counters
	//p_vertex = out_set_vertex.c_Create(std::max(num_vertex, 1));
	//p_face = out_set_face.c_Create(std::max(num_face, 1));
	p_vertex = out_set_vertex.c_Create(max(num_vertex, 1));
	p_face = out_set_face.c_Create(max(num_face, 1));
	if (exist_texture)
	{
		//p_texturept = out_set_texturept.c_Create(std::max(num_texturept, 1));
		//p_textureidx = out_set_textureidx.c_Create(std::max(num_face, 1));
		p_texturept = out_set_texturept.c_Create(max(num_texturept, 1));
		p_textureidx = out_set_textureidx.c_Create(max(num_face, 1));
	}


	int min_num = 0xffff;
	num_face = 0; num_vertex = 0; num_texturept = 0;

	for (i = 0; i<lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);


		if (temp.ie_Is_Equal("#"))
			continue;
	
		if (exist_texture && p_mesh[i].l_Left(2).ie_Is_Equal("vt"))
		{
			sscanf_s(p_mesh[i], "%c%c %f %f", &t_char, sizeof(t_char), &t_char2, sizeof(t_char2), &x, &y);
			//if(x>1) x-=1; 
			//if(y>1) y-=1;
			p_texturept[num_texturept++].s_Set(x, y);
		}
		else if (temp.ie_Is_Equal("v"))
		{
			sscanf_s(p_mesh[i], "%c %f %f %f", &t_char, sizeof(t_char),&x, &y, &z);
			p_vertex[num_vertex++].s_Set(x, y, z);
			//printf("x = %f y = %f z = %f\n", x, y, z); Kv_Printf("Check Point");
		}
		else if (temp.ie_Is_Equal("f"))
		{
			if (exist_texture)
			{
				int idx2[3];
				sscanf_s(p_mesh[i], "%c %d/%d %d/%d %d/%d",
					&t_char, sizeof(t_char),&(idx[0]), &(idx2[0]), &(idx[1]), &(idx2[1]), &(idx[2]), &(idx2[2]));
				p_textureidx[num_face].c_Create(3);
				p_textureidx[num_face].sv_Set_Vector(idx2);
			}
			else
			{
				sscanf_s(p_mesh[i], "%c %d %d %d", &t_char, sizeof(t_char), &(idx[0]), &(idx[1]), &(idx[2]));
			}
			//printf("f1 = %d f2 = %d f3 = %d\n", idx[0], idx[1], idx[2]); Kv_Printf("Check Face");
			p_face[num_face].c_Create(3);

			if (min_num > idx[0]) min_num = idx[0];
			if (min_num > idx[1]) min_num = idx[1];
			if (min_num > idx[2]) min_num = idx[2];

			p_face[num_face++].sv_Set_Vector(idx);
		}
	}

	if (min_num != 0) // offset compensation
	{
		for (i = 0; i<num_face; i++)
		{
			int* t_idx;
			t_idx = p_face[i].vp();
			t_idx[0] -= min_num;
			t_idx[1] -= min_num;
			t_idx[2] -= min_num;

			if (exist_texture)
			{
				t_idx = p_textureidx[i].vp();
				t_idx[0] -= min_num;
				t_idx[1] -= min_num;
				t_idx[2] -= min_num;
			}
		}
	}

////////////////////////////////////////////////////////////////////
	printf("[Output Setting]...... %d \n", out_set_vertex.vs());
	int num;
	//Set 3D Points
	out_point3d->im_Import(&out_set_vertex);

	//Set 3D Mesh
	if (out_mesh != NULL)
	{
		CKvVectorInt tmp_triangles; 
		offset[0] = offset[1] = offset[2] = 0;
		
		/******************************************************************************/
		out_mesh->in_Initialize(num_face);
		for (i = 0; i < num_face; i++)
		{
			out_mesh->u_me_Make_Element(&(p_face[i]), 0, 1, &element);
			out_mesh->ap_Append(false, &element, offset, num);
		}
		//p_tt = tmp_triangles.c_Create((num_face - 1) * 3, 0);
		//for (i = 0; i < num_face-1; i++)
		//{
		//	p_tt[3 * i]   = p_face[i].ge_Get_Element(0);
		//	p_tt[3 * i+1] = p_face[i].ge_Get_Element(1);
		//	p_tt[3 * i+2] = p_face[i].ge_Get_Element(2);
		//}
		//out_mesh->in_Initialize();
		//out_mesh->u_me_Make_Element(&tmp_triangles, 0, 1, &element);
		//out_mesh->ap_Append(false, &element, offset, num);
		//p_tt = tmp_triangles.c_Create(3, 0);
		//p_tt[0] = p_face[num_face - 1].ge_Get_Element(0);
		//p_tt[1] = p_face[num_face - 1].ge_Get_Element(1);
		//p_tt[2] = p_face[num_face - 1].ge_Get_Element(2);
		//out_mesh->u_me_Make_Element(&tmp_triangles, 0, 1, &element);
		//out_mesh->ap_Append(false, &element, offset, num);
		/******************************************************************************/
	}

	out_N_by_3_mesh->c_Create(num_face, 3);
	for (i = 0; i<num_face; i++)
	{
		int* t_idx;
		t_idx = p_face[i].vp();
		out_N_by_3_mesh->se_Set_Element(0, i, t_idx[0]);
		out_N_by_3_mesh->se_Set_Element(1, i, t_idx[1]);
		out_N_by_3_mesh->se_Set_Element(2, i, t_idx[2]);
	}

	if (exist_texture)
	{
		if (out_point2d_or_NULL != NULL)
		{
			out_point2d_or_NULL->im_Import(&out_set_texturept);
		}

		if (out_triImage_or_NULL != NULL)
		{
			out_triImage_or_NULL->in_Initialize(num_face);
			for (i = 0; i < num_face; i++)
			{
				out_triImage_or_NULL->u_me_Make_Element(&(p_face[i]), &(p_textureidx[i]), 0, &element);
				out_triImage_or_NULL->ap_Append(false, &element, offset, num);
			}
		}
	}

////////////////////////////////////////////////////////////////////

	return true;
}
/*********************************************************/
bool lm_Load_mesh(
	bool in_mode_texture,
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvMesh_of_Triangle *out_mesh,
	CKvMatrixInt *out_N_by_3_mesh,
	CKvDepot_of_Pointf *out_point2d_or_NULL,
	CKvMesh_of_TriImage *out_triImage_or_NULL,
	CKvMatrix *out_vertex_normal_vectors)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvString *p_mesh;
	CKvString temp;
	int lengh;
	float x, y, z;
	int idx[3];
	int i;
	char t_char, t_char2;
	bool exist_texture, exist_normal;

	CKvSet_of_Point3D out_set_vertex;
	CKvSet_of_VectorInt out_set_face;
	CKvSet_of_Point out_set_texturept;
	CKvSet_of_VectorInt out_set_textureidx;
	CKvMatrix out_set_normals;

	CKvPoint3D *p_vertex;
	CKvPoint *p_texturept = NULL;
	CKvVectorInt *p_face;
	CKvVectorInt *p_textureidx = NULL;

	int num_vertex, num_face, num_texturept, num_normal;
	int offset[3];
	CKvSet_of_VectorInt element;

	if (!(io_file.ltf_Load_from_Text_File(in_filename, true, &t_mesh)))
		return false;

	// Initialization
	p_mesh = t_mesh.vps(lengh);
	num_face = 0; num_vertex = 0; num_texturept = 0; num_normal = 0;

	//count numbers
	for (i = 0; i<lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);

		if (temp.ie_Is_Equal("#"))
			continue;

		if (p_mesh[i].l_Left(2).ie_Is_Equal("vt"))
		{
			num_texturept++;
		}
		else if (temp.ie_Is_Equal("v"))
		{
			num_vertex++;
		}
		else if (temp.ie_Is_Equal("f"))
		{
			num_face++;
		}
		else if (temp.ie_Is_Equal("vn"))
		{
			num_normal++;
		}
	}

	exist_texture = (num_texturept > 0);
	exist_normal = (num_normal > 0);

	//Initialize and reset counters
	//p_vertex = out_set_vertex.c_Create(std::max(num_vertex, 1));
	//p_face = out_set_face.c_Create(std::max(num_face, 1));
	p_vertex = out_set_vertex.c_Create(max(num_vertex, 1));
	p_face = out_set_face.c_Create(max(num_face, 1));

	if (exist_texture)
	{
		p_texturept = out_set_texturept.c_Create(max(num_texturept, 1));
		p_textureidx = out_set_textureidx.c_Create(max(num_face, 1));
		//p_texturept = out_set_texturept.c_Create(std::max(num_texturept, 1));
		//p_textureidx = out_set_textureidx.c_Create(std::max(num_face, 1));
	}
	if (exist_normal)
	{
		out_set_normals.c_Create(max(num_normal, 1),3);
		//out_set_normals.c_Create(std::max(num_normal, 1),3);
	}


	int min_num = 0xffff;
	num_face = 0; num_vertex = 0; num_texturept = 0;

	for (i = 0; i<lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);


		if (temp.ie_Is_Equal("#"))
			continue;

		if (exist_texture && p_mesh[i].l_Left(2).ie_Is_Equal("vt"))
		{
			sscanf(p_mesh[i], "%c%c %f %f", &t_char, &t_char2, &x, &y);
			//sscanf_s(p_mesh[i], "%c%c %f %f", &t_char, sizeof(t_char), &t_char2, sizeof(t_char2), &x, &y);
			//if(x>1) x-=1; 
			//if(y>1) y-=1;
			p_texturept[num_texturept++].s_Set(x, y);
		}
		else if (temp.ie_Is_Equal("v"))
		{
			sscanf(p_mesh[i], "%c %f %f %f", &t_char, &x, &y, &z);
			//sscanf_s(p_mesh[i], "%c %f %f %f", &t_char, sizeof(t_char), &x, &y, &z);
			p_vertex[num_vertex++].s_Set(x, y, z);
			//printf("x = %f y = %f z = %f\n", x, y, z); Kv_Printf("Check Point");
		}
		else if (temp.ie_Is_Equal("f"))
		{
			if (exist_texture)
			{
				int idx2[3];
				sscanf(p_mesh[i], "%c %d/%d %d/%d %d/%d",
					&t_char,
					&(idx[0]), &(idx2[0]),
					&(idx[1]), &(idx2[1]),
					&(idx[2]), &(idx2[2]));
				//sscanf_s(p_mesh[i], "%c %d/%d %d/%d %d/%d",
				//	&t_char, sizeof(t_char), 
				//	&(idx[0]), &(idx2[0]), 
				//	&(idx[1]), &(idx2[1]), 
				//	&(idx[2]), &(idx2[2]));
				p_textureidx[num_face].c_Create(3);
				p_textureidx[num_face].sv_Set_Vector(idx2);
			}
			else
			{
				//sscanf_s(p_mesh[i], "%c %d %d %d", &t_char, sizeof(t_char), &(idx[0]), &(idx[1]), &(idx[2]));
				sscanf(p_mesh[i], "%c %d %d %d", &t_char, &(idx[0]), &(idx[1]), &(idx[2]));
			}
			//printf("f1 = %d f2 = %d f3 = %d\n", idx[0], idx[1], idx[2]); Kv_Printf("Check Face");
			p_face[num_face].c_Create(3);

			if (min_num > idx[0]) min_num = idx[0];
			if (min_num > idx[1]) min_num = idx[1];
			if (min_num > idx[2]) min_num = idx[2];

			p_face[num_face++].sv_Set_Vector(idx);
		}
	}

	if (min_num != 0) // offset compensation
	{
		for (i = 0; i<num_face; i++)
		{
			int* t_idx;
			t_idx = p_face[i].vp();
			t_idx[0] -= min_num;
			t_idx[1] -= min_num;
			t_idx[2] -= min_num;

			if (exist_texture)
			{
				t_idx = p_textureidx[i].vp();
				t_idx[0] -= min_num;
				t_idx[1] -= min_num;
				t_idx[2] -= min_num;
			}
		}
	}

	////////////////////////////////////////////////////////////////////
	printf("[Output Setting]......\n");
	int num;
	//Set 3D Points
	out_point3d->im_Import(&out_set_vertex);

	//Set 3D Mesh
	if (out_mesh != NULL)
	{
		CKvVectorInt tmp_triangles; int *p_tt;
		offset[0] = offset[1] = offset[2] = 0;

		/******************************************************************************/
		p_tt = tmp_triangles.c_Create((num_face - 1) * 3, 0);
		for (i = 0; i < num_face - 1; i++)
		{
			p_tt[3 * i] = p_face[i].ge_Get_Element(0);
			p_tt[3 * i + 1] = p_face[i].ge_Get_Element(1);
			p_tt[3 * i + 2] = p_face[i].ge_Get_Element(2);
		}

		out_mesh->in_Initialize();
		out_mesh->u_me_Make_Element(&tmp_triangles, 0, 1, &element);
		out_mesh->ap_Append(false, &element, offset, num);

		p_tt = tmp_triangles.c_Create(3, 0);
		p_tt[0] = p_face[num_face - 1].ge_Get_Element(0);
		p_tt[1] = p_face[num_face - 1].ge_Get_Element(1);
		p_tt[2] = p_face[num_face - 1].ge_Get_Element(2);

		out_mesh->u_me_Make_Element(&tmp_triangles, 0, 1, &element);
		out_mesh->ap_Append(false, &element, offset, num);
		/******************************************************************************/
	}

	out_N_by_3_mesh->c_Create(num_face, 3);
	for (i = 0; i<num_face; i++)
	{
		int* t_idx;
		t_idx = p_face[i].vp();
		out_N_by_3_mesh->se_Set_Element(0, i, t_idx[0]);
		out_N_by_3_mesh->se_Set_Element(1, i, t_idx[1]);
		out_N_by_3_mesh->se_Set_Element(2, i, t_idx[2]);
	}

	if (exist_texture)
	{
		if (out_point2d_or_NULL != NULL)
		{
			out_point2d_or_NULL->im_Import(&out_set_texturept);

		}

		if (out_triImage_or_NULL != NULL)
		{
			out_triImage_or_NULL->in_Initialize(num_face);
			for (i = 0; i < num_face; i++)
			{
				out_triImage_or_NULL->u_me_Make_Element(&(p_face[i]), &(p_textureidx[i]), 0, &element);
				out_triImage_or_NULL->ap_Append(false, &element, offset, num);
			}
		}
	}

	////////////////////////////////////////////////////////////////////

	return true;
}
/*********************************************************/
bool lm_Load_mesh_for_Vertex_Color(
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvMesh_of_Triangle *out_mesh,
	CKvMatrixInt *out_N_by_3_mesh,
	CKvDepot_of_RgbaF *out_depot_color)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvSet_of_Point3D out_set_vertex;
	CKvSet_of_VectorInt out_set_face, element;
	CKvPoint3D *p_vertex; CKvRgbaF rgbaf;
	CKvVectorInt *p_face;
	CKvString *p_mesh, temp;
	int lengh, i, idx[3], offset[3];
	int num_vertex, num_face, dim, num;
	float x, y, z, r, g, b;
	char t_char;

	if (!(io_file.ltf_Load_from_Text_File(in_filename, true, &t_mesh)))
		return false;

	// Initialization
	p_mesh = t_mesh.vps(lengh);
	num_face = 0; num_vertex = 0;

	//count numbers
	for (i = 0; i<lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);

		if (temp.ie_Is_Equal("#"))
			continue;

		if (temp.ie_Is_Equal("v"))
		{
			num_vertex++;
		}
		else if (temp.ie_Is_Equal("f"))
		{
			num_face++;
		}
	}

	//Initialize and reset counters
	p_vertex = out_set_vertex.c_Create(max(num_vertex, 1));
	p_face = out_set_face.c_Create(max(num_face, 1));
	//p_vertex = out_set_vertex.c_Create(std::max(num_vertex, 1));
	//p_face = out_set_face.c_Create(std::max(num_face, 1));
	out_depot_color->in_Initialize(num_vertex);

	int min_num = 0xffff;
	num_face = 0; num_vertex = 0;

	for (i = 0; i < lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);

		if (temp.ie_Is_Equal("#"))
			continue;

		if (temp.ie_Is_Equal("v"))
		{
			sscanf_s(p_mesh[i], "%c %f %f %f %f %f %f", &t_char, sizeof(t_char), &x, &y, &z, &r, &g, &b);
			//sscanf_s(p_mesh[i], "%c %f %f %f %d %d %d", &t_char, &x, &y, &z, &r, &g, &b);

			//printf("%f %f %f %f %f %f\n", x, y, z, r, g, b); Kv_Printf("!!!");

			p_vertex[num_vertex++].s_Set(x, y, z);
			rgbaf.r = (float)((float)r / 255.f); 
			rgbaf.g = (float)((float)g / 255.f);
			rgbaf.b = (float)((float)b / 255.f);
			out_depot_color->ap_Append(false, rgbaf, dim);
		}
		else if (temp.ie_Is_Equal("f"))
		{
			sscanf_s(p_mesh[i], "%c %d %d %d", &t_char, sizeof(t_char), &(idx[0]), &(idx[1]), &(idx[2]));
			//sscanf_s(p_mesh[i], "%c %d %d %d", &t_char, &(idx[0]), &(idx[1]), &(idx[2]));

			p_face[num_face].c_Create(3);

			if (min_num > idx[0]) min_num = idx[0];
			if (min_num > idx[1]) min_num = idx[1];
			if (min_num > idx[2]) min_num = idx[2];

			p_face[num_face++].sv_Set_Vector(idx);
		}
	}

	if (min_num != 0) // offset compensation
	{
		for (i = 0; i<num_face; i++)
		{
			int* t_idx;
			t_idx = p_face[i].vp();
			t_idx[0] -= min_num;
			t_idx[1] -= min_num;
			t_idx[2] -= min_num;
		}
	}

	//Set 3D Points
	out_point3d->im_Import(&out_set_vertex);

	//Set 3D Mesh
	if (out_mesh != NULL)
	{
		offset[0] = offset[1] = offset[2] = 0;
		/******************************************************************************/
		out_mesh->in_Initialize(num_face);
		for (i = 0; i < num_face; i++)
		{
			out_mesh->u_me_Make_Element(&(p_face[i]), 0, 1, &element);
			out_mesh->ap_Append(false, &element, offset, num);
		}
		/******************************************************************************/
	}
	
	out_N_by_3_mesh->c_Create(num_face, 3);
	for (i = 0; i<num_face; i++)
	{
		int* t_idx;
		t_idx = p_face[i].vp();
		out_N_by_3_mesh->se_Set_Element(0, i, t_idx[0]);
		out_N_by_3_mesh->se_Set_Element(1, i, t_idx[1]);
		out_N_by_3_mesh->se_Set_Element(2, i, t_idx[2]);
	}




	return true;
}
/*********************************************************/
bool lm_Load_Voxel(
	CKvString &in_filename,
	CKvDepot_of_Point3Df *out_point3d,
	CKvDepot_of_RgbaF *out_depot_color)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvSet_of_Point3D out_set_vertex;
	CKvPoint3D *p_vertex; CKvRgbaF rgbaf;
	CKvString *p_mesh, temp;
	int lengh, i;
	int num_vertex, dim;
	float x, y, z, r, g, b;
	char t_char;

	if (!(io_file.ltf_Load_from_Text_File(in_filename, true, &t_mesh)))
		return false;

	// Initialization
	p_mesh = t_mesh.vps(lengh);
	num_vertex = 0;

	//count numbers
	for (i = 0; i<lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);

		if (temp.ie_Is_Equal("#"))
			continue;

		if (temp.ie_Is_Equal("v"))
		{
			num_vertex++;
		}
	}

	//Initialize and reset counters
	//p_vertex = out_set_vertex.c_Create(std::max(num_vertex, 1));
	p_vertex = out_set_vertex.c_Create(max(num_vertex, 1));
	out_depot_color->in_Initialize(num_vertex);

	num_vertex = 0;

	for (i = 0; i < lengh; i++)
	{
		temp = p_mesh[i].l_Left(1);

		if (temp.ie_Is_Equal("#"))
			continue;

		if (temp.ie_Is_Equal("v"))
		{
			sscanf_s(p_mesh[i], "%c %f %f %f %f %f %f", &t_char, sizeof(t_char), &x, &y, &z, &r, &g, &b);

			p_vertex[num_vertex++].s_Set(x, y, z);
			rgbaf.r = (float)((float)r / 255.f); 
			rgbaf.g = (float)((float)g / 255.f);
			rgbaf.b = (float)((float)b / 255.f);
			out_depot_color->ap_Append(false, rgbaf, dim);
		}
	}

	//Set 3D Points
	out_point3d->im_Import(&out_set_vertex);

	return true;
}
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvXrunset3dShort *out_runset_3d,
	CKvSet2d_of_XvectorUcharRgb *out_set_colors)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvString *p_mesh, temp, dum_str1;
	CKvSet2d_of_XvectorShort set2d_short; CKvXvectorShort **xvec;
	CKvVolumeBool voxels; bool ***p_voxels;
	CKvVolumeUcharRgb voxel_color; 
	CKvXvectorUcharRgb **p_set_color; CKvRgb rgb;
	int k, i, j, zz, lengh, num_vertex, data_index, bp_sz, np;
	int count;
	short zo, z1, *a;
	float x, y, z;
	unsigned char r, g, b;
	unsigned char ***p_r, ***p_g, ***p_b;
	char *p_char;


	if (!(io_file.ltf_Load_from_Text_File(in_filename, true, &t_mesh)))
		return false;
	p_mesh = t_mesh.vps(lengh);
	num_vertex = 0;
	//count numbers
	for (i = 0; i < lengh; i++)
	{
		if (p_mesh[i].ie_Is_Equal("end_header")){ data_index = i + 1; break; }
		temp = p_mesh[i].l_Left(1);
		if (temp.ie_Is_Equal("e"))
		{
			p_char = p_mesh[i].bps_Buffer_Pointer_and_Size(bp_sz);
			dum_str1.in_Initialize();
			for (int m = 15; m < bp_sz; m++){ dum_str1.ap_Append(p_char[m]); }
			sscanf_s(dum_str1, "%d", &num_vertex);
			//printf("num Voxel = %d\n", num_vertex);
		}
	}

	p_voxels = voxels.c_Create(in_voxel_resolution, in_voxel_resolution, in_voxel_resolution, false);
	voxel_color.c_Create(in_voxel_resolution, in_voxel_resolution, in_voxel_resolution);
	p_r = voxel_color.tp(p_g, p_b);
	for (i = data_index; i < lengh; i++)
	{
		sscanf_s(p_mesh[i], "%f %f %f %d %d %d", &x, &y, &z, &r, &g, &b);
		p_voxels[(int)z][(int)y][(int)x] = true;
		p_r[(int)z][(int)y][(int)x] = r;
		p_g[(int)z][(int)y][(int)x] = g;
		p_b[(int)z][(int)y][(int)x] = b;
	}

	out_runset_3d->im_Import(voxels);
	voxels.c_Create(1, 1, 1);
	p_set_color = out_set_colors->c_Create(in_voxel_resolution, in_voxel_resolution);
	out_runset_3d->ex_Export(set2d_short, NULL);
	xvec = set2d_short.mp();
	for (i = 0 ; i < in_voxel_resolution ; i++)
	{
		for (j = 0 ; j < in_voxel_resolution ; j++)
		{
			xvec[j][i].z_gii_Get_Informations_Internal(NULL, &a, &np, NULL);
			p_set_color[j][i].in_Initialize(10*np);
			//p_set_color[j][i].z_gii(NULL, &pr, &pg, &pb, NULL, NULL);
			count = 0;
			for (k = 0; k < np;)
			{
				zo = a[k]; k++; 
				z1 = a[k]; k++;
				for (zz = zo; zz <= z1; zz++)
				{
					rgb.r = p_r[zz][j][i];
					rgb.g = p_g[zz][j][i];
					rgb.b = p_b[zz][j][i];
					p_set_color[j][i].ae_Append_Element(rgb);
					count++;
					//point3d.x = (double)i;
					//point3d.y = (double)j;
					//point3d.z = (double)z;
					//out_set_of_voxel->ap_Append(false, point3d, dum);
					//xvec_idx[j][i].ae_Append_Element(count);
					//count++;
				}
			}
			p_set_color[j][i].z_cne(count);
		}
	}
	
	return true;
}
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_set_of_voxels,
	CKvDepot_of_RgbaF *out_set_colors)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvString *p_mesh, temp, dum_str1;
	CKvPoint3Df point3df; CKvRgbaF rgbaf;
	int i, lengh, num_vertex, data_index, bp_sz, dum;
	float x, y, z;
	float r, g, b;
	char *p_char;


	if (!(io_file.ltf_Load_from_Text_File(in_filename, true, &t_mesh)))
	{
		return false;
	}
	p_mesh = t_mesh.vps(lengh);
	num_vertex = 0;

	//count numbers
	for (i = 0; i < lengh; i++)
	{
		if (p_mesh[i].ie_Is_Equal("end_header")){ data_index = i + 1; break; }
		
		temp = p_mesh[i].l_Left(1);
		
		if (temp.ie_Is_Equal("e"))
		{
			p_char = p_mesh[i].bps_Buffer_Pointer_and_Size(bp_sz);
			dum_str1.in_Initialize();
			for (int m = 15; m < bp_sz; m++)
			{ 
				dum_str1.ap_Append(p_char[m]); 
			}
			sscanf_s(dum_str1, "%d", &num_vertex);
		}
	}

	out_set_of_voxels->in_Initialize(num_vertex);
	out_set_colors->in_Initialize(num_vertex);
	for (i = data_index; i < lengh; i++)
	{
		sscanf_s(p_mesh[i], "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
		
		point3df.x = x;             
		point3df.y = y;            
		point3df.z = z;
		rgbaf.r = (float)(r)/255.f; 
		rgbaf.g = (float)(g)/255.f; 
		rgbaf.b = (float)(b)/255.f;
		rgbaf.a = 0.f;

		out_set_of_voxels->ap_Append(false, point3df, dum);
		out_set_colors->ap_Append(false, rgbaf, dum);
	}

	return true;
}
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_set_of_voxels,
	CKvDepot_of_Rgb *out_set_colors)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvString *p_mesh, temp, dum_str1;
	CKvPoint3Df point3df; CKvRgb rgbaf;
	int i, lengh, num_vertex, data_index, bp_sz, dum;
	float x, y, z;
	float r, g, b;
	char *p_char;


	if (!(io_file.ltf_Load_from_Text_File(in_filename, true, &t_mesh)))
	{
		return false;
	}
	p_mesh = t_mesh.vps(lengh);
	num_vertex = 0;

	//count numbers
	for (i = 0; i < lengh; i++)
	{
		if (p_mesh[i].ie_Is_Equal("end_header")) { data_index = i + 1; break; }

		temp = p_mesh[i].l_Left(1);

		if (temp.ie_Is_Equal("e"))
		{
			p_char = p_mesh[i].bps_Buffer_Pointer_and_Size(bp_sz);
			dum_str1.in_Initialize();
			for (int m = 15; m < bp_sz; m++)
			{
				dum_str1.ap_Append(p_char[m]);
			}
			sscanf_s(dum_str1, "%d", &num_vertex);
		}
	}

	out_set_of_voxels->in_Initialize(num_vertex);
	out_set_colors->in_Initialize(num_vertex);
	for (i = data_index; i < lengh; i++)
	{
		sscanf_s(p_mesh[i], "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);

		point3df.x = x;
		point3df.y = y;
		point3df.z = z;
		rgbaf.r = r;
		rgbaf.g = g;
		rgbaf.b = b;

		out_set_of_voxels->ap_Append(false, point3df, dum);
		out_set_colors->ap_Append(false, rgbaf, dum);
	}

	return true;
}
/*********************************************************/
bool lm_Load_Ply_File_for_Vertex_Color2(
	CKvString &in_filename,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_set_of_voxels,
	CKvDepot_of_RgbaF *out_set_colors)
/*********************************************************/
{
	LCKvIO_FileJpg io_file;
	CKvVectorString t_mesh;
	CKvString dum_str1;
	CKvPoint3Df point3df; CKvRgbaF rgbaf;
	int num_vertex, bp_sz, dum;
	float x, y, z;
	float r, g, b;
	char *p_char;
	char line[256];
	CKvString kvline;

	FILE *file = fopen(in_filename, "r");
	if (file != NULL)
	{
		while (fgets(line, sizeof(line), file) != NULL)
		{
			kvline.im_Import(line);
			if (kvline.ne() > 9)
			{
				if (kvline.l_Left(10).ie_Is_Equal("end_header"))
				{
					break;
				}
			}

			if (kvline.ne() > 10)
			{
				if (kvline.l_Left(14).ie_Is_Equal("element vertex"))
				{
					p_char = kvline.bps_Buffer_Pointer_and_Size(bp_sz);
					dum_str1.in_Initialize();
					for (int m = 15; m < bp_sz; m++)
					{
						dum_str1.ap_Append(p_char[m]);
					}
					sscanf_s(dum_str1, "%d", &num_vertex);
				}
			}
		}

		out_set_of_voxels->in_Initialize(num_vertex);
		out_set_colors->in_Initialize(num_vertex);
		while (fgets(line, sizeof(line), file) != NULL)
		{
			kvline.im_Import(line);
			sscanf_s(kvline, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			point3df.x = x;
			point3df.y = y;
			point3df.z = z;
			rgbaf.r = (float)(r) / 255.f;
			rgbaf.g = (float)(g) / 255.f;
			rgbaf.b = (float)(b) / 255.f;
			rgbaf.a = 0.f;

			out_set_of_voxels->ap_Append(false, point3df, dum);
			out_set_colors->ap_Append(false, rgbaf, dum);
		}
	}
	else
	{
		return false;
	}

	fclose(file);
	return true;
}
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
	Util_Mesh_Generation *io_mesh_generation)
/*********************************************************/
{
	LCKvAlgebra_for_Matrix lam;
	CKvSet_of_Pmatrix3D cpy_pmats;
	CKvSet_of_Point3D set_of_point3d;
	CKvDepot_of_RgbaF depot_color; 
	CKvMesh_of_Triangle mesh_triangle;
	CKvMatrix H; 
	int num_v, k;

	out_Docube->sc_Set_Cube(in_ww, in_hh, in_dd);
	out_Docube->sr_Set_Rays(0.0f, 0.f, 0.f);
	//out_Docube->sr_Set_Rays(0.f, 0.f, 0.f);

	in_N_by_3_mesh->t_Transpose(in_N_by_3_mesh);
	in_point3d->e_Export(&set_of_point3d);
	out_Docube->lfm_Load_From_Mesh(
		&set_of_point3d,
		in_N_by_3_mesh, 
		&H);
	io_mesh_generation->mm_Make_Meshes(
		out_Docube,
		out_DoCube_point3d, 
		&depot_color, 
		&mesh_triangle);
	out_mesh_triangle->cp_Copy(&mesh_triangle);

	if (!Mesh_of_Triangle_To_CKvMatrix(
		true,//bool in_mode_tri_tetra_true_or_only_tri_false,
		&mesh_triangle,//CKvMesh_of_Triangle *in_mesh,
		out_N_by_3_DoCube_mesh))//CKvMatrixInt *out_N_by_3_mesh);
	{
		return false;
	}

	num_v = io_pmats->vs();
	cpy_pmats.cp_Copy(io_pmats);
	for (k = 0; k < num_v; k++)
	{
		lam.mmm_Multiply_Matrix_Matrix(
			cpy_pmats.gpe_Get_Pointer_of_Element(k)->mpp_Matrix_Pointer_of_P_matrix(),
			&H,
			io_pmats->gpe_Get_Pointer_of_Element(k)->mpp_Matrix_Pointer_of_P_matrix());
	}
	/*********************************************************************************/
	CKvVectorShort **p_dd;
	int valid_ray = 0, indx = 0, j, i;
	p_dd = out_Docube->gp_Get_Pointer()->mp();

	for (j = 0; j < in_hh; j++)
	{
		for (i = 0; i < in_ww; i++)
		{
			if (p_dd[j][i].ge_Get_Element(0) == NOPOINTVALDoCube){ continue; }
			valid_ray++;
			indx+=p_dd[j][i].vs();
		}
	}
	printf("Num Valid Ray = %d\n", valid_ray);
	printf("Num S E point = %d\n",indx);
	/*********************************************************************************/

	return true;
}
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
	VCL_DoCube *out_Docube)
/*********************************************************/
{
	LCKvAlgebra_for_Matrix lam;
	CKvSet_of_Pmatrix3D cpy_pmats;
	CKvSet_of_Point3D set_of_point3d;
	CKvDepot_of_RgbaF depot_color;
	CKvMesh_of_Triangle mesh_triangle;
	CKvMatrix H;
	int num_v, k;

	out_Docube->sc_Set_Cube(in_ww, in_hh, in_dd);
	out_Docube->sr_Set_Rays(in_ang_x, in_ang_y, in_ang_z);

	in_N_by_3_mesh->t_Transpose(in_N_by_3_mesh);
	in_point3d->e_Export(&set_of_point3d);

	CKvStopWatch sw;
	double time = 0.0, f_time;
	sw.c_Create(1);
	
	printf("[lfm_Load_From_Mesh]\n");

	sw.r_Reset(0);
	out_Docube->lfm_Load_From_Mesh2(
		&set_of_point3d,
		in_N_by_3_mesh,
		&H);

	f_time = sw.get_Get_Elapsed_Time(0);
	printf("\ntime = %lf\n", f_time);

	num_v = io_pmats->vs();
	cpy_pmats.cp_Copy(io_pmats);
	for (k = 0; k < num_v; k++)
	{
		lam.mmm_Multiply_Matrix_Matrix(
			cpy_pmats.gpe_Get_Pointer_of_Element(k)->mpp_Matrix_Pointer_of_P_matrix(),
			&H,
			io_pmats->gpe_Get_Pointer_of_Element(k)->mpp_Matrix_Pointer_of_P_matrix());
	}


	return true;
}
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
	VCL_DoCube *out_Docube)
/*********************************************************/
{
	LCKvAlgebra_for_Matrix lam;
	CKvSet_of_Pmatrix3D cpy_pmats;
	CKvSet_of_Point3D set_of_point3d;
	CKvDepot_of_RgbaF depot_color;
	CKvMesh_of_Triangle mesh_triangle;
	int num_v, k;

	out_Docube->sc_Set_Cube(in_ww, in_hh, in_dd);
	out_Docube->sr_Set_Rays(in_ang_x, in_ang_y, in_ang_z);

	in_N_by_3_mesh->t_Transpose(in_N_by_3_mesh);
	in_point3d->e_Export(&set_of_point3d);

	CKvStopWatch sw;
	double time = 0.0, f_time;
	sw.c_Create(1);

	sw.r_Reset(0);
	out_Docube->lfm_Load_From_Mesh(
		&set_of_point3d,
		in_N_by_3_mesh,
		out_homography);

	f_time = sw.get_Get_Elapsed_Time(0);
	printf("\ntime = %lf\n", f_time);

	num_v = io_pmats->vs();
	cpy_pmats.cp_Copy(io_pmats);
	for (k = 0; k < num_v; k++)
	{
		lam.mmm_Multiply_Matrix_Matrix(
			cpy_pmats.gpe_Get_Pointer_of_Element(k)->mpp_Matrix_Pointer_of_P_matrix(),
			out_homography,
			io_pmats->gpe_Get_Pointer_of_Element(k)->mpp_Matrix_Pointer_of_P_matrix());
	}


	return true;
}
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
	VCL_DoCube *out_Docube)
/*********************************************************/
{
	CKvSet_of_Point3D set_of_point3d;
	CKvDepot_of_RgbaF depot_color;
	CKvMesh_of_Triangle mesh_triangle;
	CKvMatrix H;

	out_Docube->sc_Set_Cube(in_ww, in_hh, in_dd);
	out_Docube->sr_Set_Rays(in_ang_x, in_ang_y, in_ang_z);

	in_N_by_3_mesh->t_Transpose(in_N_by_3_mesh);
	in_point3d->e_Export(&set_of_point3d);

	CKvStopWatch sw;
	double time = 0.0, f_time;
	sw.c_Create(1);

	printf("[lfm_Load_From_Mesh]\n");

	sw.r_Reset(0);
	out_Docube->lfm_Load_From_Mesh2(
		&set_of_point3d,
		in_N_by_3_mesh,
		&H);

	f_time = sw.get_Get_Elapsed_Time(0);
	printf("\ntime = %lf\n", f_time);

	return true;
}
/*********************************************************/
bool vpc_Voxelize_Point_Cloud(
	CKvDepot_of_Point3Df *in_point_cloud,
	CKvDepot_of_RgbaF *in_point_cloud_color,
	int in_voxel_resolution,
	CKvDepot_of_Point3Df *out_voxel,
	CKvDepot_of_RgbaF *out_voxel_color)
/*********************************************************/
{
	CKvSet_of_Point3Df set_voxels;
	CKvSet_of_RgbaF set_colors;
	float **p_pc;
	float min_x, max_x, min_y, max_y, min_z, max_z;
	float Length[3], max_Length, scale;
	int num_pt, k;


	num_pt = in_point_cloud->ne_Number_of_Elements();
	printf("num_pt = %d\n", num_pt);


	if (in_point_cloud->ne_Number_of_Elements() != 
		in_point_cloud_color->ne_Number_of_Elements())
	{
		return false;
	}
	
	if (num_pt == 0)
	{ 
		return false; 
	}


	p_pc = in_point_cloud->gps_Get_Pointer_of_Space()->mp();

	printf("hh = %d ww = %d\n",
		in_point_cloud->gps_Get_Pointer_of_Space()->mh(),
		in_point_cloud->gps_Get_Pointer_of_Space()->mw());

	min_x = max_x = p_pc[0][0];
	min_y = max_y = p_pc[0][1];
	min_z = max_z = p_pc[0][2];

	for (k = 0; k < num_pt; k++)
	{
		if (min_x > p_pc[k][0]){ min_x = p_pc[k][0]; }
		if (max_x < p_pc[k][0]){ max_x = p_pc[k][0]; }

		if (min_y > p_pc[k][1]){ min_y = p_pc[k][1]; }
		if (max_y < p_pc[k][1]){ max_y = p_pc[k][1]; }
		
		if (min_z > p_pc[k][2]){ min_z = p_pc[k][2]; }
		if (max_z < p_pc[k][2]){ max_z = p_pc[k][2]; }
	}

	Length[0] = max_x - min_x;
	Length[1] = max_y - min_y;
	Length[2] = max_z - min_z;
	max_Length = -1.f;
	for (k = 0; k < 3; k++)
	{
		if (max_Length < Length[k])
		{
			max_Length = Length[k];
		}
	}

	scale = (float)(in_voxel_resolution-1) / max_Length;
	CKvPoint3Df *p_sv = set_voxels.c_Create(num_pt);

	for (k = 0; k < num_pt; k++)
	{
		p_sv[k].x = round(scale*(p_pc[k][0] - min_x));
		p_sv[k].y = round(scale*(p_pc[k][1] - min_y));
		p_sv[k].z = round(scale*(p_pc[k][2] - min_z));
	}

	out_voxel->im_Import(&set_voxels);
	out_voxel_color->cp_Copy(in_point_cloud_color);

	return true;
}
/*********************************************************/
bool r3p_Rotate_3D_Points(
	CKvDepot_of_Point3Df *in_object3d,
	CKvDepot_of_Point3Df *out_object3d)
/*********************************************************/
{
	LCKvAlgebra_for_Matrix la_mat;
	LCKvUtility_for_Matrix_Factorization lu_mf;
	LCKvUtility_for_Classifier lu_c;
	CKvSet_of_Point3D object3d;
	CKvDepot_of_Pattern depot_pattern;
	CKvPoint3D *p_object3d;
	CKvMatrix data, orthogonal_mat, A, new_data; CKvVector mean_vec, tmp;
	CKvMatrix_Symmetric cov_sym;
	CKvMatrix_Diagonal mat_diag;
	double **p_data;  
	int num, k;

	in_object3d->e_Export(&object3d);
	p_object3d = object3d.vps(num);
	p_data        = data.c_Create(num, 3);
	for (k = 0; k < num; k++)
	{
		p_data[k][0] = p_object3d[k].x;
		p_data[k][1] = p_object3d[k].y;
		p_data[k][2] = p_object3d[k].z;
	}

	depot_pattern.im_Import(true, &data);
	lu_c.gcv_Get_Covariance_matrix(
		&depot_pattern,
		&mean_vec,
		NULL,
		&cov_sym);

	lu_mf.devj_Decompose_to_find_Eigen_Values_using_Jacobi_rotations(
		&cov_sym, 
		&A,
		&mat_diag);

	orthogonal_mat.c_Create(3, 3);

	A.gc_Get_Column(2, 0, 3, &tmp);
	orthogonal_mat.sc_Set_Column(0, 0, &tmp);

	A.gc_Get_Column(1, 0, 3, &tmp);
	orthogonal_mat.sc_Set_Column(1, 0, &tmp);

	A.gc_Get_Column(0, 0, 3, &tmp);
	orthogonal_mat.sc_Set_Column(2, 0, &tmp);

	la_mat.mmm_Multiply_Matrix_Matrix(&data, &orthogonal_mat, &new_data);
	p_data = new_data.mp();
	for (k = 0; k < num; k++)
	{
		p_object3d[k].x = p_data[k][0];
		p_object3d[k].y = p_data[k][1];
		p_object3d[k].z = p_data[k][2];
	}

	out_object3d->im_Import(&object3d);



	//int ww, hh, j, i; double *p_diag;
	//printf("Diagonal matrix\n");
	//p_diag = mat_diag.vps(ww);
	//for (j = 0; j < ww; j++)
	//{
	//	printf("%lf ", p_diag[j]);
	//}
	//printf("\n");
	//double **p_om;
	//printf("Orthogonal matrix\n");
	//p_om = orthogonal_mat.mps(ww, hh);
	//for (j = 0; j < hh; j++)
	//{
	//	for (i = 0; i < ww; i++)
	//	{
	//		printf("%lf ", p_om[j][i]);
	//	}
	//	printf("\n");
	//}



	return true;
}
/*********************************************************/
bool Mesh_of_Triangle_To_CKvMatrix(
	bool in_mode_tri_tetra_true_or_only_tri_false,
	CKvMesh_of_Triangle *in_mesh,
	CKvMatrixInt *out_N_by_3_mesh)
/*********************************************************/
{
	CKvVectorInt *idx_tri, *f_color_tri, *b_color_tri;
	CKvVectorInt *idx_tetra, *f_color_tetra, *b_color_tetra;
	int **p_n3_mesh;
	int *p_idx_tri;
	int *p_idx_tetra;
	int *t_idx, sz_tri, sz_tetra, sz_total, num_mesh, p, j, i;

	if (in_mode_tri_tetra_true_or_only_tri_false)
	{
		in_mesh->gep_Get_Element_Pointers(0, idx_tri, f_color_tri, b_color_tri);
		in_mesh->gep_Get_Element_Pointers(1, idx_tetra, f_color_tetra, b_color_tetra);
		p_idx_tri = idx_tri->vps(sz_tri);
		p_idx_tetra = idx_tetra->vps(sz_tetra);
		sz_total = sz_tri + sz_tetra;
		t_idx = new int[sz_total];
		
		for (p = 0; p < sz_tri; p++){ t_idx[p] = p_idx_tri[p]; }
		for (p = 0; p < sz_tetra; p++){ t_idx[sz_tri + p] = p_idx_tetra[p]; }

		num_mesh = sz_total/3;
		p_n3_mesh = out_N_by_3_mesh->c_Create(num_mesh, 3);
		
		for (j = 0; j < num_mesh; j++)
		{
			for (i = 0; i < 3; i++)
			{
				p_n3_mesh[j][i] = (int)t_idx[j*3+i];
			}
		}

		delete[]t_idx;
	}
	else
	{
		in_mesh->gep_Get_Element_Pointers(0, idx_tri, f_color_tri, b_color_tri);
		p_idx_tri = idx_tri->vps(sz_tri);
		num_mesh = sz_tri/3;

		p_n3_mesh = out_N_by_3_mesh->c_Create(num_mesh, 3);
		for (j = 0; j < num_mesh; j++)
		{
			for (i = 0; i < 3; i++)
			{
				p_n3_mesh[j][i] = (int)p_idx_tri[j * 3 + i];
			}
		}
	}

	return true;
}
/*********************************************************/
bool CKvMatrix_To_Mesh_of_Triangle(
	CKvMatrixInt *in_N_by_3_mesh,
	CKvMesh_of_Triangle *out_mesh)
/*********************************************************/
{
	CKvSet_of_VectorInt element;
	CKvVectorInt list;
	int **p_n3_mesh;
	int *p_list, offset[3] = {0, };
	int num_mesh, dim, j;

	p_n3_mesh = in_N_by_3_mesh->mps(dim, num_mesh);
	out_mesh->in_Initialize(num_mesh);
	p_list = list.c_Create(3);

	for (j = 0; j < num_mesh; j++)
	{
		p_list[0] = p_n3_mesh[j][0]; p_list[1] = p_n3_mesh[j][1]; p_list[2] = p_n3_mesh[j][2];
		out_mesh->u_me_Make_Element(&list, 0, 0, &element);
		out_mesh->ap_Append(false, &element, offset, dim);
	}

	return true;
}
/*********************************************************/
bool sat_Save_as_Txt(
	CKvString in_filename,
	CKvVectorInt *in_vector)
/*********************************************************/
{
	FILE *fp=NULL;
	int sz, k;
	int *p_v;

	fopen_s(&fp,(char *)in_filename, "w");
	if (fp == NULL) return false;

	p_v = in_vector->vps(sz);

	for (k = 0; k < sz; k++){ fprintf(fp, "%d\n", p_v[k]); }

	fclose(fp);

	return true;
}
/*********************************************************/
bool sat_Save_as_Txt(
	CKvString in_filename,
	CKvMatrixInt *in_matrix)
/*********************************************************/
{
	FILE *fp = NULL;
	int ww,hh, m,n;
	int **p_v;

	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	p_v = in_matrix->mps(ww,hh);

	for (m = 0; m < hh; m++)
	{
		for (n = 0; n < ww; n++)
		{
			fprintf(fp, "%d ", p_v[m][n]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return true;
}
/*********************************************************/
bool sat_Save_as_Txt(
	CKvString in_filename,
	CKvMatrixFloat *in_matrix)
/*********************************************************/
{
	FILE *fp = NULL;
	int ww, hh, m, n;
	float **p_v;

	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	p_v = in_matrix->mps(ww, hh);

	for (m = 0; m < hh; m++)
	{
		for (n = 0; n < ww; n++)
		{
			fprintf(fp, "%f ", p_v[m][n]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return true;
}
/*********************************************************/
bool sat_Save_as_Txt_Normal_Map(
	CKvString in_filename,
	CKvMatrixFloatRgb *in_normal)
/*********************************************************/
{
	FILE *fp = NULL;
	CKvString names;
	int ww, hh, j, i;
	float **p_x, **p_y, **p_z;

	p_x = in_normal->mps(p_y, p_z, ww, hh);

	names = "normal_x.txt";
	fopen_s(&fp, (char *)names, "w");
	if (fp == NULL) return false;
	
	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			fprintf(fp, "%f ", p_x[j][i]);
		}
		fprintf(fp, "\n");
	}
	
	fclose(fp);

	names = "normal_y.txt";
	fopen_s(&fp, (char *)names, "w");
	if (fp == NULL) return false;

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			fprintf(fp, "%f ", p_y[j][i]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	names = "normal_z.txt";
	fopen_s(&fp, (char *)names, "w");
	if (fp == NULL) return false;

	for (j = 0; j < hh; j++)
	{
		for (i = 0; i < ww; i++)
		{
			fprintf(fp, "%f ", p_z[j][i]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return true;
}
/*********************************************************/
bool sao_Save_as_Obj(
	CKvString in_filename,
	CKvSet_of_Point3D *in_points_or_NULL,
	CKvMatrixInt *in_mesh_or_NULL)
/*********************************************************/
{
	FILE *fp = NULL;
	CKvPoint3D *p_point;
	int sz, dim, k;
	int **p_v;

	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	if (in_points_or_NULL != NULL)
	{
		p_point = in_points_or_NULL->vps(sz);
		for (k = 0; k < sz; k++){ fprintf(fp, "v %lf %lf %lf\n", p_point[k].x, p_point[k].y, p_point[k].z); }
	}

	if (in_mesh_or_NULL != NULL)
	{
		p_v = in_mesh_or_NULL->mps(dim, sz);
		if (dim == 3)
		{  
			for (k = 0; k < sz; k++){ fprintf(fp, "f %d %d %d\n", p_v[k][0] + 1, p_v[k][1] + 1, p_v[k][2] + 1); }
		}
	}

	fclose(fp);

	return true;
}
/*********************************************************/
bool sao_Save_as_Obj(
	CKvString in_filename,
	CKvSet_of_Point3D *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL,
	CKvMatrixInt *in_mesh_or_NULL)
//********************************************************************************
{
	FILE *fp = NULL;
	CKvPoint3D *p_point;
	CKvMatrixFloat *p_rgbaf;
	int sz, dim, k; float **pp_rgbaf;
	int **p_v;

	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	if (in_points_or_NULL != NULL)
	{
		if (in_vertex_colors_or_NULL == NULL)
		{
			p_point = in_points_or_NULL->vps(sz);
			for (k = 0; k < sz; k++){ fprintf(fp, "v %lf %lf %lf\n", p_point[k].x, p_point[k].y, p_point[k].z); }
		}
		else
		{
			p_point = in_points_or_NULL->vps(sz);
			p_rgbaf = in_vertex_colors_or_NULL->gps_Get_Pointer_of_Space();
			pp_rgbaf = p_rgbaf->mp();
			for (k = 0; k < sz; k++)
			{ 
				fprintf(fp, "v %lf %lf %lf %d %d %d\n", 
					p_point[k].x,   p_point[k].y,   p_point[k].z, 
					(int)(255.f*pp_rgbaf[k][0]), (int)(255.f*pp_rgbaf[k][1]), (int)(255.f*pp_rgbaf[k][2])); 
			}
		}
	}

	if (in_mesh_or_NULL != NULL)
	{
		p_v = in_mesh_or_NULL->mps(dim, sz);
		if (dim == 3)
		{  
			for (k = 0; k < sz; k++){ fprintf(fp, "f %d %d %d\n", 
				p_v[k][0] + 1, p_v[k][1] + 1, p_v[k][2] + 1 ); }
		}
	}

	fclose(fp);

	return true;

}
//********************************************************************************
bool sao_Save_as_Obj(
	CKvString in_filename,
	CKvSet_of_Voxel *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL,
	CKvMatrixInt *in_mesh_or_NULL)
//********************************************************************************
{
	FILE *fp = NULL;
	CKvVoxel *p_point;
	CKvMatrixFloat *p_rgbaf;
	int sz, dim, k; float **pp_rgbaf;
	int **p_v;

	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	if (in_points_or_NULL != NULL)
	{
		if (in_vertex_colors_or_NULL == NULL)
		{
			p_point = in_points_or_NULL->vps(sz);
			for (k = 0; k < sz; k++){ fprintf(fp, "v %d %d %d\n", p_point[k].x, p_point[k].y, p_point[k].z); }
		}
		else
		{
			p_point = in_points_or_NULL->vps(sz);
			p_rgbaf = in_vertex_colors_or_NULL->gps_Get_Pointer_of_Space();
			pp_rgbaf = p_rgbaf->mp();
			for (k = 0; k < sz; k++)
			{ 
				fprintf(fp, "v %d %d %d %d %d %d\n", 
					p_point[k].x,   p_point[k].y,   p_point[k].z, 
					(int)(255.f*pp_rgbaf[k][0]), (int)(255.f*pp_rgbaf[k][1]), (int)(255.f*pp_rgbaf[k][2])); 
			}
		}
	}

	if (in_mesh_or_NULL != NULL)
	{
		p_v = in_mesh_or_NULL->mps(dim, sz);
		if (dim == 3)
		{  
			for (k = 0; k < sz; k++){ fprintf(fp, "f %d %d %d\n", p_v[k][0] + 1, p_v[k][1] + 1, p_v[k][2] + 1); }
		}
	}

	fclose(fp);

	return true;
}
//********************************************************************************
bool sap_Save_as_Ply(
	CKvString in_filename,
	CKvSet_of_Voxel *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL)
//********************************************************************************
{
	FILE *fp = NULL;
	CKvVoxel *p_point;
	CKvMatrixFloat *p_rgbaf;
	int sz, k; float **pp_rgbaf;

	sz = in_points_or_NULL->vs();
	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	fprintf(fp, "ply\n");
	fprintf(fp, "format ascii 1.0\n");
	fprintf(fp, "element vertex %d\n",sz);
	fprintf(fp, "property float x\n");
	fprintf(fp, "property float y\n");
	fprintf(fp, "property float z\n");
	fprintf(fp, "property uchar red\n");
	fprintf(fp, "property uchar green\n");
	fprintf(fp, "property uchar blue\n");
	fprintf(fp, "end_header\n");

	if (in_points_or_NULL != NULL)
	{
		if (in_vertex_colors_or_NULL == NULL)
		{
			p_point = in_points_or_NULL->vps(sz);
			for (k = 0; k < sz; k++)
			{ 
				fprintf(fp, "v %d %d %d\n", p_point[k].x, p_point[k].y, p_point[k].z); 
			}
		}
		else
		{
			p_point = in_points_or_NULL->vps(sz);
			p_rgbaf = in_vertex_colors_or_NULL->gps_Get_Pointer_of_Space();
			pp_rgbaf = p_rgbaf->mp();
			for (k = 0; k < sz; k++)
			{
				fprintf(fp, "%d %d %d %d %d %d\n",
					p_point[k].x, p_point[k].y, p_point[k].z,
					(int)(255.f*pp_rgbaf[k][0]), (int)(255.f*pp_rgbaf[k][1]), (int)(255.f*pp_rgbaf[k][2]));
			}
		}
	}

	fclose(fp);

	return true;
}
//********************************************************************************
bool sap_Save_as_Ply(
	CKvString in_filename,
	CKvSet_of_Point3Df *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL)
//********************************************************************************
{
	FILE *fp = NULL;
	CKvPoint3Df *p_point;
	CKvMatrixFloat *p_rgbaf;
	int sz, k; float **pp_rgbaf;

	sz = in_points_or_NULL->vs();
	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	fprintf(fp, "ply\n");
	fprintf(fp, "format ascii 1.0\n");
	fprintf(fp, "element vertex %d\n", sz);
	fprintf(fp, "property float x\n");
	fprintf(fp, "property float y\n");
	fprintf(fp, "property float z\n");
	fprintf(fp, "property uchar red\n");
	fprintf(fp, "property uchar green\n");
	fprintf(fp, "property uchar blue\n");
	fprintf(fp, "end_header\n");

	if (in_points_or_NULL != NULL)
	{
		if (in_vertex_colors_or_NULL == NULL)
		{
			p_point = in_points_or_NULL->vps(sz);
			for (k = 0; k < sz; k++)
			{
				fprintf(fp, "%f %f %f 255 0 0\n", p_point[k].x, p_point[k].y, p_point[k].z);
			}
		}
		else
		{
			p_point = in_points_or_NULL->vps(sz);
			p_rgbaf = in_vertex_colors_or_NULL->gps_Get_Pointer_of_Space();
			pp_rgbaf = p_rgbaf->mp();
			for (k = 0; k < sz; k++)
			{
				fprintf(fp, "%f %f %f %d %d %d\n",
					p_point[k].x, p_point[k].y, p_point[k].z,
					(int)(255.f*pp_rgbaf[k][0]), (int)(255.f*pp_rgbaf[k][1]), (int)(255.f*pp_rgbaf[k][2]));
			}
		}
	}

	fclose(fp);

	return true;
}
//********************************************************************************
bool sap_Save_as_Ply(
	CKvString in_filename,
	CKvSet_of_Point3D *in_points_or_NULL,
	CKvDepot_of_RgbaF *in_vertex_colors_or_NULL)
//********************************************************************************
{
	FILE *fp = NULL;
	CKvPoint3D *p_point;
	CKvMatrixFloat *p_rgbaf;
	int sz, k; float **pp_rgbaf;

	sz = in_points_or_NULL->vs();
	fopen_s(&fp, (char *)in_filename, "w");
	if (fp == NULL) return false;

	fprintf(fp, "ply\n");
	fprintf(fp, "format ascii 1.0\n");
	fprintf(fp, "element vertex %d\n", sz);
	fprintf(fp, "property float x\n");
	fprintf(fp, "property float y\n");
	fprintf(fp, "property float z\n");
	fprintf(fp, "property uchar red\n");
	fprintf(fp, "property uchar green\n");
	fprintf(fp, "property uchar blue\n");
	fprintf(fp, "end_header\n");

	if (in_points_or_NULL != NULL)
	{
		if (in_vertex_colors_or_NULL == NULL)
		{
			p_point = in_points_or_NULL->vps(sz);
			for (k = 0; k < sz; k++)
			{
				fprintf(fp, "%f %f %f 255 0 0\n", p_point[k].x, p_point[k].y, p_point[k].z);
			}
		}
		else
		{
			p_point = in_points_or_NULL->vps(sz);
			p_rgbaf = in_vertex_colors_or_NULL->gps_Get_Pointer_of_Space();
			pp_rgbaf = p_rgbaf->mp();
			for (k = 0; k < sz; k++)
			{
				fprintf(fp, "%f %f %f %d %d %d\n",
					p_point[k].x, p_point[k].y, p_point[k].z,
					(int)(255.f*pp_rgbaf[k][0]), (int)(255.f*pp_rgbaf[k][1]), (int)(255.f*pp_rgbaf[k][2]));
			}
		}
	}

	fclose(fp);

	return true;
}
//********************************************************************************
void g3pouac_Get_3D_Point_Object_Using_All_Camera(
	CKvSet_of_Pmatrix3D *in_set_of_P_matrices,
	CKvSet_of_SdkCode *in_set_of_silhouette_images,
	CKvSet_of_Point3D *out_3D_Points)
//********************************************************************************
{
	int i, j, k, l;
	int size, point_num;
	int num_3D_points = 0;
	bool dummy = false;
	int *index;
	CKvMatrix pi_;
	CKvMatrix arg_pi_;
	CKvMatrix_Diagonal dmat;
	CKvMatrix rmat;
	LCKvUtility_for_Linear_Equation le;
	LCKvUtility_for_Matrix_Factorization mf;

	CKvPmatrix3D *pmat;
	CKvSet_of_Rect rec_set;
	CKvRect *rec;
	CKvPixel lt, rb;
	CKvPoint p1, p2;

	CKvSet_of_Hline set_of_Hline;
	CKvHline *line;

	CKvSet_of_Hplane set_of_Hplane;
	CKvHplane *plane;

	CKvSet_of_Point3D set_points;
	CKvPoint3D *point3D;
	CKvPoint3D *out_point3D;

	CKvPoint point_on_plane;

	double **p_pi_;
	double **p_arg_pi_;
	double plane_normal_1[4];
	double plane_normal_2[4];
	double plane_normal_3[4];



	LCKvUtility_for_DoSurfaceFloat zz_udos;

	zz_udos.grs_Get_Rectangles_of_Silhouettes(in_set_of_silhouette_images, &rec_set);
	rec = rec_set.vps_Vector_Pointer_and_Size(size);
	pmat = in_set_of_P_matrices->vp_Vector_Pointer();

	p_pi_ = pi_.c_Create(3, 4, 0.0);
	p_arg_pi_ = arg_pi_.c_Create(3, 3, 0.0);
	line = set_of_Hline.c_Create(4);
	plane = set_of_Hplane.c_Create(size * 4);
	num_3D_points = (4 * size*(2 * size - 1)*(4 * size - 1)) / 3;
	index = new int[num_3D_points];
	memset(index, 1, sizeof(int)*num_3D_points);

	//printf("num_3D_points = %d\n",num_3D_points);

	for (j = 0; j<size; j++){

		rec[j].g_Get(lt, rb);

		p1.s_Set((double)(lt.x), (double)(lt.y));
		p2.s_Set((double)(lt.x), (double)(rb.y));
		line[0].s_Set(p1, p2);

		p1.s_Set((double)(rb.x), (double)(lt.y));
		p2.s_Set((double)(rb.x), (double)(rb.y));
		line[1].s_Set(p1, p2);

		p1.s_Set((double)(lt.x), (double)(lt.y));
		p2.s_Set((double)(rb.x), (double)(lt.y));
		line[2].s_Set(p1, p2);

		p1.s_Set((double)(lt.x), (double)(rb.y));
		p2.s_Set((double)(rb.x), (double)(rb.y));
		line[3].s_Set(p1, p2);

		for (i = 0; i<4; i++){
			pmat[j].fbpl_Find_Back_Projection_of_Line(line[i], plane[4 * j + i]);
			//printf("plane_num = %d\n",4*j+i);
		}
	}

	point3D = set_points.c_Create(num_3D_points);
	double **p_rmat = rmat.c_Create(4, 4, 0.0);
	num_3D_points = 0;
	//printf("[Calculate points...]\n");
	for (i = 0; i < size * 4; i++)
	{
		for (j = i + 1; j < size * 4; j++)
		{
			for (k = j + 1; k < size * 4; k++)
			{
				//printf("Doing...i=%d,j=%d,k=%d\n",i,j,k);
				plane[i].g_Get(plane_normal_1); plane[j].g_Get(plane_normal_2); plane[k].g_Get(plane_normal_3);

				for (l = 0; l < 4; l++){

					p_pi_[0][l] = plane_normal_1[l];
					p_pi_[1][l] = plane_normal_2[l];
					p_pi_[2][l] = plane_normal_3[l];

					if (l != 3){

						p_arg_pi_[0][l] = plane_normal_1[l];
						p_arg_pi_[1][l] = plane_normal_2[l];
						p_arg_pi_[2][l] = plane_normal_3[l];

					}
				}
				//////////////////////////////////////////////////////////////////////////////////////
				if ((mf.rqr_Rank_based_on_QR_decomposition_with_column_pivoting(&pi_) == 3) &&
					(mf.rqr_Rank_based_on_QR_decomposition_with_column_pivoting(&arg_pi_) == 3))
				{
					//////////////////////////////////////////////////////////////////////////////////////
					le.dsvb_Decompose_to_find_Singular_Values_via_Bidiagonalization(&pi_, NULL, &dmat, &rmat);

					p_rmat[0][3] /= p_rmat[3][3];
					p_rmat[1][3] /= p_rmat[3][3];
					p_rmat[2][3] /= p_rmat[3][3];
					p_rmat[3][3] /= p_rmat[3][3];

					point3D[num_3D_points].s_Set(p_rmat[0][3], p_rmat[1][3], p_rmat[2][3]);
					num_3D_points++;
				}
				///////////////////////////////////////////////////////////////
			}
		}
	}

	int usable_num = 0;
	point_num = set_points.vs_Vector_Size();

	for (i = 0; i<point_num; i++){
		for (j = 0; j<size; j++){

			rec[j].g_Get(lt, rb);
			pmat[j].tp_Transform_Point(point3D[i], point_on_plane);

			if (!((rb.x + 0.5 >= point_on_plane.x) &&
				(lt.x - 0.5 <= point_on_plane.x) &&
				(rb.y + 0.5 >= point_on_plane.y) &&
				(lt.y - 0.5 <= point_on_plane.y))){
				index[i] = 0;


			}
		}

		if (index[i] != 0) usable_num++;

	}

	out_point3D = out_3D_Points->c_Create(usable_num);

	usable_num = 0;
	for (i = 0; i<point_num; i++){

		if (index[i] != 0){

			out_point3D[usable_num].s_Set(point3D[i].x, point3D[i].y, point3D[i].z);
			usable_num++;
		}

	}

	delete[] index;

}
//********************************************************************************