
// Texture_Atlas_KAISION_2.0_VS2017_V6Dlg.h: 헤더 파일
//

#pragma once


// CTextureAtlasKAISION20VS2017V6Dlg 대화 상자
class CTextureAtlasKAISION20VS2017V6Dlg : public CDialogEx
{
// 생성입니다.
public:
	CTextureAtlasKAISION20VS2017V6Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTURE_ATLAS_KAISION_20_VS2017_V6_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	VCL_DoCube_X_Color zz_docube;
	int zz_cluster_size;
	int m_scanning_direction;
	float m_cluster_threshold;

	afx_msg void OnBnClickedLoadPly();
	afx_msg void OnBnClickedGenerateThreeWayTextureMap();
	afx_msg void OnBnClickedLoadImage();
	int m_voxel_resolution;
	int m_direction_of_slice;
	int m_block_size;
	int m_alignment;
	int m_threshold_for_classifying_tm;
	int m_direction_sub_mode;
};
