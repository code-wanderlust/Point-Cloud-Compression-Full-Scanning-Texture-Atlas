
// Texture_Atlas_KAISION_2.0_VS2017_V6Dlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Texture_Atlas_KAISION_2.0_VS2017_V6.h"
#include "Texture_Atlas_KAISION_2.0_VS2017_V6Dlg.h"
#include "afxdialogex.h"


/***********************************************************/
#include "Funtion_IO.h"
#include "VCL_Voxel_Color_Final_Version.h"
#include "VCL_Voxel_Texture_Atlas_3_Direction.h"

/***********************************************************/


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTextureAtlasKAISION20VS2017V6Dlg 대화 상자



CTextureAtlasKAISION20VS2017V6Dlg::CTextureAtlasKAISION20VS2017V6Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEXTURE_ATLAS_KAISION_20_VS2017_V6_DIALOG, pParent)
	, m_voxel_resolution(1024)
	, m_direction_of_slice(1)
	, m_block_size(16)
	, m_alignment(1)
	, m_threshold_for_classifying_tm(128)
	, m_direction_sub_mode(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextureAtlasKAISION20VS2017V6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_voxel_resolution);
	DDX_Text(pDX, IDC_EDIT2, m_direction_of_slice);
	DDX_Text(pDX, IDC_EDIT3, m_block_size);
	DDX_Text(pDX, IDC_EDIT4, m_alignment);
	DDX_Text(pDX, IDC_EDIT5, m_threshold_for_classifying_tm);
	DDX_Text(pDX, IDC_EDIT6, m_direction_sub_mode);
}

BEGIN_MESSAGE_MAP(CTextureAtlasKAISION20VS2017V6Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_PLY, &CTextureAtlasKAISION20VS2017V6Dlg::OnBnClickedLoadPly)
	ON_BN_CLICKED(IDC_GENERATE_THREE_WAY_TEXTURE_MAP, &CTextureAtlasKAISION20VS2017V6Dlg::OnBnClickedGenerateThreeWayTextureMap)
	ON_BN_CLICKED(IDC_LOAD_IMAGE, &CTextureAtlasKAISION20VS2017V6Dlg::OnBnClickedLoadImage)
END_MESSAGE_MAP()


// CTextureAtlasKAISION20VS2017V6Dlg 메시지 처리기

BOOL CTextureAtlasKAISION20VS2017V6Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTextureAtlasKAISION20VS2017V6Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTextureAtlasKAISION20VS2017V6Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTextureAtlasKAISION20VS2017V6Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/****************************************************************/
void CTextureAtlasKAISION20VS2017V6Dlg::OnBnClickedLoadPly()
/****************************************************************/
{
	UpdateData(true);
	LCKvIO io;
	CKvString file_name;
	CKvDepot_of_Point3Df voxels;
	CKvDepot_of_RgbaF colors;
	int ww, hh, dd;

	if (!io.gof_Get_Open_Filename(".ply file", file_name))
	{
		return;
	}

	printf("[Load Ply]......\n");
	ww = hh = dd = m_voxel_resolution = 1024;

	lm_Load_Ply_File_for_Vertex_Color2(
		file_name,//CKvString &in_filename,
		m_voxel_resolution,//int in_voxel_resolution,
		&voxels,//CKvDepot_of_Point3Df *out_set_of_voxels,
		&colors);//CKvDepot_of_RgbaF *out_set_colors);

	printf("[i_Import]......\n");
	zz_docube.i_Import(
		&voxels,//CKvDepot_of_Point3Df *in_3d_points,
		hh,//int in_hh,
		ww,//int in_ww,
		dd,//int in_dd,
		&colors,//CKvDepot_of_RgbaF *in_depot_colors);
		false);

	printf("num depot voxel = %d\n", zz_docube.gsp_Get_Surface_Voxels().ne_Number_of_Elements());
	printf("Num Voxels = %d\n", voxels.ne_Number_of_Elements());
	printf("Num Colors = %d\n", colors.ne_Number_of_Elements());
	printf("[Done]\n");


}
/***********************************************************************************/
void CTextureAtlasKAISION20VS2017V6Dlg::OnBnClickedLoadImage()
/***********************************************************************************/
{
	CKvMatrixBool P;
	CKvXrunsetShort a;

	P.c_Create(1000, 1, true);
	a.im_Import(P);
	printf("[done]\n");

	return;

	CKvScreen sc[2];
	LCKvIO_FileJpg iofj;
	CKvMatrixUchar img; CKvMatrixBool pattern;
	CKvString name;

	name = "D:\\Dropbox\\Photos\\01.bmp";
	iofj.li_Load_Image(name, false, &img);
	
	CKvXrunsetShort xrunshort, obj_blob;
	CKvSdkimRunCode src;
	
	xrunshort.im_Import(img, 100, true);
	src.im_Import(xrunshort, true);

	int num_obj = src.no_Number_of_Objects();
	int j;

	sc[0].s_d_Display(&img);

	for (j = 0; j < num_obj; j++)
	{
		src.exo_EXtract_an_Object(j, obj_blob, NULL);
		obj_blob.ex_Export(true, false, pattern);
		sc[1].s_d_Display(&pattern);
		Kv_Printf("!!%d/%d!!", j, num_obj);
	}

}
/***********************************************************************************/
void CTextureAtlasKAISION20VS2017V6Dlg::OnBnClickedGenerateThreeWayTextureMap()
/***********************************************************************************/
{
	OnBnClickedLoadPly();

	UpdateData(true);

	VCL_Voxel_Texture_Atlas_3_Direction vvtad;
	VCL_Voxel_Color_Final_Version vvcfv;
	std::vector<int> plane_mode_sequence;

	CKvSet_of_MatrixUcharRgb voxel_maps;
	CKvSet_of_MatrixBool voxel_map_masks;
	CKvSet_of_MatrixInt voxel_idx_maps;
	CKvMatrixInt pt_list;
	CKvString file_name;

	vvtad.Set_Parameters(
		false,//bool in_shape_coding_mode_on_off,
		m_direction_of_slice,//int in_plane_mode_sequence,
		m_alignment,//int in_alignment_mode,
		m_block_size,//int in_block_size,
		1024,//int in_threshold_for_cluster_size,
		1.f,//float in_threshold_for_segmentation,
		m_threshold_for_classifying_tm,//int in_threshold_cluster,
		1500,//int in_map_width,
		1500);//int in_map_height);

	plane_mode_sequence = std::vector<int>(3);
	if (m_direction_of_slice == 0)
	{
		if (m_direction_sub_mode == 0)
		{
			plane_mode_sequence[0] = 0;
			plane_mode_sequence[1] = 1;
			plane_mode_sequence[2] = 2;
		}
		else if (m_direction_sub_mode == 1)
		{
			plane_mode_sequence[0] = 0;
			plane_mode_sequence[1] = 2;
			plane_mode_sequence[2] = 1;
		}
	}
	else if (m_direction_of_slice == 1)
	{
		if (m_direction_sub_mode == 0)
		{
			plane_mode_sequence[0] = 1;
			plane_mode_sequence[1] = 0;
			plane_mode_sequence[2] = 2;
		}
		else if (m_direction_sub_mode == 1)
		{
			plane_mode_sequence[0] = 1;
			plane_mode_sequence[1] = 2;
			plane_mode_sequence[2] = 0;
		}
	}
	else
	{
		if (m_direction_sub_mode == 0)
		{
			plane_mode_sequence[0] = 2;
			plane_mode_sequence[1] = 0;
			plane_mode_sequence[2] = 1;
		}
		else if (m_direction_sub_mode == 1)
		{
			plane_mode_sequence[0] = 2;
			plane_mode_sequence[1] = 1;
			plane_mode_sequence[2] = 0;
		}
	}

	printf("Scan Order : %d %d %d\n", 
		plane_mode_sequence[0],
		plane_mode_sequence[1],
		plane_mode_sequence[2]);


	/********************************************************************/
	vvtad.generate_Voxel_Texture_Atlas_3_Direction(
		&zz_docube,//VCL_DoCube_X_Color *in_docube,
		plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
		&voxel_maps,//CKvSet_of_MatrixUcharRgb *out_voxel_maps,
		&voxel_map_masks,//CKvSet_of_MatrixBool *out_voxel_map_masks,
		&voxel_idx_maps);//CKvSet_of_MatrixInt *out_voxel_idx_maps);

	file_name = "map_00.png";
	vvcfv.si_Save_Image_from_OpenCV(
		file_name,//CKvString &in_filename,
		voxel_maps.gpe(0));//CKvMatrixUcharRgb *out_image);

	file_name = "map_01.png";
	vvcfv.si_Save_Image_from_OpenCV(
		file_name,//CKvString &in_filename,
		voxel_maps.gpe(1));//CKvMatrixUcharRgb *out_image);

	file_name = "map_02.png";
	vvcfv.si_Save_Image_from_OpenCV(
		file_name,//CKvString &in_filename,
		voxel_maps.gpe(2));//CKvMatrixUcharRgb *out_image);

	file_name = "mask_00.png";
	vvcfv.si_Save_Image_from_OpenCV(
		file_name,//CKvString &in_filename,
		voxel_map_masks.gpe(0));//CKvMatrixUcharRgb *out_image);

	file_name = "mask_01.png";
	vvcfv.si_Save_Image_from_OpenCV(
		file_name,//CKvString &in_filename,
		voxel_map_masks.gpe(1));//CKvMatrixUcharRgb *out_image);

	file_name = "mask_02.png";
	vvcfv.si_Save_Image_from_OpenCV(
		file_name,//CKvString &in_filename,
		voxel_map_masks.gpe(2));//CKvMatrixUcharRgb *out_image);

	file_name = "voxel_map_idx_00.bidx";
	vvcfv.sim_Save_Index_Map(
		file_name,
		voxel_idx_maps.gpe(0));

	file_name = "voxel_map_idx_01.bidx";
	vvcfv.sim_Save_Index_Map(
		file_name,
		voxel_idx_maps.gpe(1));

	file_name = "voxel_map_idx_02.bidx";
	vvcfv.sim_Save_Index_Map(
		file_name,
		voxel_idx_maps.gpe(2));

	printf("[DONE]!!!!!!!!!!!!!!!!\n");
	/********************************************************************/


	/********************************************************************/
	//vvtad.generate_Voxel_Texture_Atlas_3_Direction3(
	//	&zz_docube,//VCL_DoCube_X_Color *in_docube,
	//	plane_mode_sequence,//std::vector<int> in_plane_mode_sequence,
	//	&voxel_maps,//CKvSet_of_MatrixUcharRgb *out_voxel_maps,
	//	&voxel_map_masks,//CKvSet_of_MatrixBool *out_voxel_map_masks,
	//	&voxel_idx_maps,//CKvSet_of_MatrixInt *out_voxel_idx_maps,
	//	&pt_list);//CKvMatrixInt *out_pt_list_for_secong_img_N_by_3);

	//file_name = "map_00.png";
	//vvcfv.si_Save_Image_from_OpenCV(
	//	file_name,//CKvString &in_filename,
	//	voxel_maps.gpe(0));//CKvMatrixUcharRgb *out_image);

	//file_name = "map_01.png";
	//vvcfv.si_Save_Image_from_OpenCV(
	//	file_name,//CKvString &in_filename,
	//	voxel_maps.gpe(1));//CKvMatrixUcharRgb *out_image);

	//file_name = "mask.png";
	//vvcfv.si_Save_Image_from_OpenCV(
	//	file_name,//CKvString &in_filename,
	//	voxel_map_masks.gpe(0));//CKvMatrixUcharRgb *out_image);

	//file_name = "voxel_map_idx.bidx";
	//vvcfv.sim_Save_Index_Map(
	//	file_name,
	//	voxel_idx_maps.gpe(0));

	//file_name = "pt_list_prediction.bidx";
	//vvcfv.sim_Save_Index_Map(
	//	file_name,
	//	&pt_list);

	//printf("[DONE]!!!!!!!!!!!!!!!!\n");
	/********************************************************************/
}
/***********************************************************************************/



