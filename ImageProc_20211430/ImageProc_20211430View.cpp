
// ImageProc_20211430View.cpp: CImageProc20211430View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProc_20211430.h"
#endif

#include "ImageProc_20211430Doc.h"
#include "ImageProc_20211430View.h"
#include "COpenCVDlg.h"

#include <vfw.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProc20211430View

IMPLEMENT_DYNCREATE(CImageProc20211430View, CScrollView)

BEGIN_MESSAGE_MAP(CImageProc20211430View, CScrollView)
	ON_COMMAND(ID_PIXEL_ADD, &CImageProc20211430View::OnPixelAdd)
	ON_COMMAND(ID_PIXEL_SUB, &CImageProc20211430View::OnPixelSub)
	ON_COMMAND(ID_PIXEL_MUL, &CImageProc20211430View::OnPixelMul)
	ON_COMMAND(ID_PIXEL_DIV, &CImageProc20211430View::OnPixelDiv)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CImageProc20211430View::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_CONTRAST_STRETCH, &CImageProc20211430View::OnPixelContrastStretch)
	ON_COMMAND(ID_BINARIZATION, &CImageProc20211430View::OnPixelBinarization)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &CImageProc20211430View::OnPixelTwoImageAdd)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_SUB, &CImageProc20211430View::OnPixelTwoImageSub)
	ON_COMMAND(ID_REGION_SMOOTHING, &CImageProc20211430View::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_SHARPENING, &CImageProc20211430View::OnRegionSharpening)
	ON_COMMAND(ID_REGION_EMBOSSING, &CImageProc20211430View::OnRegionEmbossing)
	ON_COMMAND(ID_REGION_PREWITT, &CImageProc20211430View::OnRegionPrewitt)
	ON_COMMAND(ID_REGION_ROBERTS, &CImageProc20211430View::OnRegionRoberts)
	ON_COMMAND(ID_REGION_SOBEL, &CImageProc20211430View::OnRegionSobel)
	ON_COMMAND(ID_REGION_AVERAGE_FILTERING, &CImageProc20211430View::OnRegionAverageFiltering)
	ON_COMMAND(ID_REGION_MEDIAN_FILTERING, &CImageProc20211430View::OnRegionMedianFiltering)
	ON_COMMAND(ID_MOPOLOGY_COLOR_GRAY, &CImageProc20211430View::OnMopologyColorGray)
	ON_COMMAND(ID_MOPOLOGY_BINARIZATION, &CImageProc20211430View::OnMopologyBinarization)
	ON_COMMAND(ID_MOPOLOGY_EROSION, &CImageProc20211430View::OnMopologyErosion)
	ON_COMMAND(ID_MOPOLOGY_OPENING, &CImageProc20211430View::OnMopologyOpening)
	ON_COMMAND(ID_MOPOLOGY_DILATION, &CImageProc20211430View::OnMopologyDilation)
	ON_COMMAND(ID_MOPOLOGY_CLOSING, &CImageProc20211430View::OnMopologyClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CImageProc20211430View::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_BILINEAR_INTERPOLATION, &CImageProc20211430View::OnGeometryZoominBilinearInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CImageProc20211430View::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEAN_SUB, &CImageProc20211430View::OnGeometryZoomoutMeanSub)
	ON_COMMAND(ID_GEOMETRY_AVG_FILTERING, &CImageProc20211430View::OnGeometryAvgFiltering)
	ON_COMMAND(ID_GEOMETRY_ROTATION, &CImageProc20211430View::OnGeometryRotation)
	ON_COMMAND(ID_GEOMETRY_HOLIZANTIAL_FLIP, &CImageProc20211430View::OnGeometryHolizantialFlip)
	ON_COMMAND(ID_GEOMETRY_VERTICAL_FLIP, &CImageProc20211430View::OnGeometryVerticalFlip)
	ON_COMMAND(ID_GEOMETRY_WARPING, &CImageProc20211430View::OnGeometryWarping)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_AVI_VIEW, &CImageProc20211430View::OnAviView)
	ON_COMMAND(ID_OPENCV, &CImageProc20211430View::OnOpencv)
END_MESSAGE_MAP()

// CImageProc20211430View 생성/소멸

CImageProc20211430View::CImageProc20211430View() noexcept
{
	bAviMode = false;

}

CImageProc20211430View::~CImageProc20211430View()
{
}

BOOL CImageProc20211430View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageProc20211430View 그리기

void CImageProc20211430View::OnDraw(CDC* pDC)
{
	CImageProc20211430Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (bAviMode)
	{
		//재생
		LoadAviFile(pDC);
		bAviMode = false;
		return;
	}

	int x, y;
	if (pDoc->InputImg != NULL)
	{
		if (pDoc->depth == 1)
		{
		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->InputImg[y][x], pDoc->InputImg[y][x], pDoc->InputImg[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->ImageHeight; y++)
				for (x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(x, y, RGB(pDoc->InputImg[y][3 * x + 0], pDoc->InputImg[y][3 * x + 1], pDoc->InputImg[y][3 * x + 2]));
		}
	}

	if (pDoc->ResultImg != NULL)
	{
		if (pDoc->depth == 1)
		{
		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
				pDC->SetPixel(pDoc->ImageWidth + 20 + x, y, RGB(pDoc->ResultImg[y][x], pDoc->ResultImg[y][x], pDoc->ResultImg[y][x]));
			}
		else
		{
			for (y = 0; y < pDoc->ImageHeight; y++)
				for (x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(pDoc->ImageWidth + 20 + x, y, RGB(pDoc->ResultImg[y][3 * x + 0], pDoc->ResultImg[y][3 * x + 1], pDoc->ResultImg[y][3 * x + 2]));
		}
	}

	if (pDoc->InputImg2 != NULL)
	{
		if (pDoc->depth == 1)
		{
			for (y = 0; y < pDoc->ImageHeight; y++)
				for (x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(2 * (pDoc->ImageWidth + 20) + x, y, RGB(pDoc->InputImg2[y][x], pDoc->InputImg2[y][x], pDoc->InputImg2[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->ImageHeight; y++)
				for (x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(2 * (pDoc->ImageWidth + 20) + x, y, RGB(pDoc->InputImg2[y][3 * x + 0], pDoc->InputImg2[y][3 * x + 1], pDoc->InputImg2[y][3 * x + 2]));
		}
	}

	if (pDoc->gResultImg != NULL)
	{
		if (pDoc->depth == 1)
		{
			for (y = 0; y < pDoc->gImageHeight; y++)
				for (x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x, pDoc->ImageHeight+20 + y, RGB(pDoc->gResultImg[y][x], pDoc->gResultImg[y][x], pDoc->gResultImg[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->gImageHeight; y++)
				for (x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x, pDoc->ImageHeight + 20 + y, RGB(pDoc->gResultImg[y][3 * x + 0], pDoc->gResultImg[y][3 * x + 1], pDoc->gResultImg[y][3 * x + 2]));
		}
	}
}	

void CImageProc20211430View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 2048;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImageProc20211430View 진단

#ifdef _DEBUG
void CImageProc20211430View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProc20211430View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProc20211430Doc* CImageProc20211430View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProc20211430Doc)));
	return (CImageProc20211430Doc*)m_pDocument;
}
#endif //_DEBUG


// CImageProc20211430View 메시지 처리기


void CImageProc20211430View::OnPixelAdd()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	if (pDoc->InputImg == NULL) return;

	int x, y, value;

	for (y = 0;y < pDoc->ImageHeight;y++)
		for (x = 0; x < pDoc->ImageWidth * pDoc->depth; x++)
	{
		value = pDoc->InputImg[y][x] + 50;
		if (value > 255)	value = 255;
		else if (value < 0) value = 0;
		pDoc->ResultImg[y][x] = value;

	}

	Invalidate();
}


void CImageProc20211430View::OnPixelSub()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	int x, y, value;

	for (y = 0;y < pDoc->ImageHeight;y++)
		for (x = 0; x < pDoc->ImageWidth * pDoc->depth; x++)
	{
		value = pDoc->InputImg[y][x] - 50;
		if (value > 255)	value = 255;
		else if (value < 0) value = 0;
		pDoc->ResultImg[y][x] = value;

	}

	Invalidate();
}


void CImageProc20211430View::OnPixelMul()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	int x, y, value;

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth * pDoc->depth; x++)
		{
		value = pDoc->InputImg[y][x] * 1.5;
		if (value > 255)	value = 255;
		else if (value < 0) value = 0;
		pDoc->ResultImg[y][x] = value;

	}

	Invalidate();
}


void CImageProc20211430View::OnPixelDiv()
{
	CImageProc20211430Doc* pDoc = GetDocument();
	                                                                                                                                                                                                                    
	int x, y, value;

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth * pDoc->depth; x++)
	{
		value = pDoc->InputImg[y][x] / 1.5;
		if (value > 255)	value = 255;
		else if (value < 0) value = 0;
		pDoc->ResultImg[y][x] = value;

	}

	Invalidate();
}

void CImageProc20211430View::OnPixelHistoEq()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	int x, y, i, k;
	int acc_hist;
	float N = 256 * 256;
	int hist[256], sum[256];

	for (i = 0; i < 256; i++)	hist[i] = 0;
	//히스토그램 구하기
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
		{
			k = pDoc->InputImg[y][x];
			hist[k]++;
		}

	//누적분포 구하기
	acc_hist = 0;
	for (i = 0; i < 256; i++)
	{
		acc_hist += hist[i];
		sum[i] = acc_hist;
	}

	//평활화 화소값저장
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
		{
			k = pDoc->InputImg[y][x];
			pDoc->ResultImg[y][x] = (float)sum[k] / N * 255;
		}

	Invalidate();
}


void CImageProc20211430View::OnPixelContrastStretch()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	int x, y;
	int min = 256, max = 0;


	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			if (pDoc->InputImg[y][x] > max)
				max = pDoc->InputImg[y][x];
			if (pDoc->InputImg[y][x] < min)
				min = pDoc->InputImg[y][x];
		}

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			pDoc->ResultImg[y][x] = (float)(pDoc->InputImg[y][x] - min) / (max - min) * 255;
		}

	Invalidate();
}


void CImageProc20211430View::OnPixelBinarization()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	int x, y, value;
	int threshold = 100;

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
		{
			value = pDoc->InputImg[y][x];
			if (value >= threshold)	pDoc->ResultImg[y][x] = 255;
			else							pDoc->ResultImg[y][x] = 0;
		}

	Invalidate();		

}


void CImageProc20211430View::OnPixelTwoImageAdd()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	pDoc->LoadSecondImageFile(ar);
	file.Close();
	//LoadTwoImage();

	int x, y, value;
	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			value = 0.3*pDoc->InputImg[y][x] + 0.7*pDoc->InputImg2[y][x];
			if (value > 255)		value = 255;
			else if (value < 0)		value = 0;
			pDoc->ResultImg[y][x] = value;
		}

	Invalidate();

}


void CImageProc20211430View::LoadTwoImage()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CFile file;
		file.Open(dlg.GetPathName(), CFile::modeRead);
		file.Read(pDoc->InputImg2, 256 * 256);
		file.Close();
	}
}


void CImageProc20211430View::OnPixelTwoImageSub()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	pDoc->LoadSecondImageFile(ar);
	file.Close();
	//LoadTwoImage();


	int x, y, value;
	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			value = abs(pDoc->InputImg[y][x] - pDoc->InputImg2[y][x]);
			if (value > 64)		value = 255;
			else						value = 0;
			if (value > 255)		value = 255;
			else if (value < 0)	value = 0;
			pDoc->ResultImg[y][x] = value;
		}

	Invalidate();
}
void CImageProc20211430View::OnRegionSmoothing()
{
	CImageProc20211430Doc* pDoc = GetDocument();
	float kernel[3][3] = { {1 / 9.0, 1 / 9.0, 1 / 9.0},
							{1 / 9.0, 1 / 9.0, 1 / 9.0},
							{1 / 9.0, 1 / 9.0, 1 / 9.0} };
	Convolve(pDoc->InputImg, pDoc->ResultImg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 0, pDoc->depth);
	Invalidate();
}
void CImageProc20211430View::Convolve(unsigned char** InImg, unsigned char** OutImg, int cols, int rows, float mask[][3], int bias, int depth)
{
	int x, y, value, i, j;
	float sum, redsum, greensum, bluesum;
	for (y = 1; y < rows - 1; y++)
		for (x = 1; x < cols - 1; x++)
		{
			if (depth == 1)
			{
				sum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						sum += InImg[y - 1 + j][x - 1 + i] * mask[j][i];
					}
				sum = sum + bias;
				if (sum > 255)       sum = 255;
				else if (sum < 0)    sum = 0;
				OutImg[y][x] = sum;
			}
			else
			{
				redsum = 0; greensum = 0; bluesum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						redsum += InImg[y - 1 + j][3 * (x - 1 + i) + 0] * mask[j][i];
						greensum += InImg[y - 1 + j][3 * (x - 1 + i) + 1] * mask[j][i];
						bluesum += InImg[y - 1 + j][3 * (x - 1 + i) + 2] * mask[j][i];
					}
				redsum += bias;
				greensum += bias;
				bluesum += bias;
				if (redsum > 255)        redsum = 255;
				else if (redsum < 0) redsum = 0;
				if (greensum > 255)      greensum = 255;
				else if (redsum < 0) greensum = 0;
				if (bluesum > 255)       bluesum = 255;
				else if (bluesum < 0)    bluesum = 0;
				OutImg[y][3 * x + 0] = redsum;
				OutImg[y][3 * x + 1] = greensum;
				OutImg[y][3 * x + 2] = bluesum;
			}
		}
}
void CImageProc20211430View::OnRegionSharpening()
{
	CImageProc20211430Doc* pDoc = GetDocument();
	float kernel[3][3] = { {0,  -1,    0},
								{-1,  5,   -1},
								{0,  -1,    0} };
	Convolve(pDoc->InputImg, pDoc->ResultImg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 0, pDoc->depth);
	Invalidate();
}

void CImageProc20211430View::OnRegionEmbossing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImageProc20211430View::OnRegionPrewitt()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	float Hmask[3][3] = {	{-1,  -1,   -1},
									{0,  0,   0},
									{1,  1,    1} };
	float Vmask[3][3] = { {-1,  0,   1},
									{-1 ,  0,   1},
									{-1,  0,    1} };

	unsigned char** Er, ** Ec;
	int x, y, value;

	//메모리 할당
	Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->ImageHeight; i++)
	{
		Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
	}

	Convolve(pDoc->InputImg, Er, pDoc->ImageWidth, pDoc->ImageHeight, Hmask, 0, pDoc->depth);
	Convolve(pDoc->InputImg, Ec, pDoc->ImageWidth, pDoc->ImageHeight, Vmask, 0, pDoc->depth);

	for(y=0;y<pDoc->ImageHeight;y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
				if (value > 255)	value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultImg[y][x] = value;
			}
			else {
				value = sqrt(Er[y][3 * x + 0] * Er[y][3 * x + 0] + Ec[y][3 * x + 0] * Ec[y][3 * x + 0]);
				if (value > 255)	value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultImg[y][3 * x + 0] = value;

				value = sqrt(Er[y][3 * x + 1] * Er[y][3 * x + 1] + Ec[y][3 * x + 1] * Ec[y][3 * x + 1]);
				if (value > 255)	value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultImg[y][3 * x + 1] = value;

				value = sqrt(Er[y][3 * x + 2] * Er[y][3 * x + 2] + Ec[y][3 * x + 2] * Ec[y][3 * x + 2]);
				if (value > 255)	value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultImg[y][3 * x + 2] = value;

				value = sqrt(pDoc->ResultImg[y][3 * x + 0] * pDoc->ResultImg[y][3 * x + 0] +
					pDoc->ResultImg[y][3 * x + 1] * pDoc->ResultImg[y][3 * x + 1] +
					pDoc->ResultImg[y][3 * x + 2] * pDoc->ResultImg[y][3 * x + 2]);
				if (value > 255)	value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultImg[y][3 * x + 0] = value;
				pDoc->ResultImg[y][3 * x + 1] = value;
				pDoc->ResultImg[y][3 * x + 2] = value;

			}


		}

	// 메모리 해제
	for (int i = 0; i < pDoc->ImageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();

}


	void CImageProc20211430View::OnRegionRoberts()
	{
		CImageProc20211430Doc* pDoc = GetDocument();

		float Hmask[3][3] = { { -1, 0, 0 },
								{  0, 1, 0 },
								{  0, 0, 0 } };
		float Vmask[3][3] = { {  0, 0,-1 },
								{  0, 1, 0 },
								{  0, 0, 0 } };

		unsigned char** Er, ** Ec;
		int x, y, value;

		//메모리 할당
		Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
		Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
		for (int i = 0; i < pDoc->ImageHeight; i++)
		{
			Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
			Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		}

		Convolve(pDoc->InputImg, Er, pDoc->ImageWidth, pDoc->ImageHeight, Hmask, 0, pDoc->depth);
		Convolve(pDoc->InputImg, Ec, pDoc->ImageWidth, pDoc->ImageHeight, Vmask, 0, pDoc->depth);

		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][x] = value;
				}
				else {
					value = sqrt(Er[y][3 * x + 0] * Er[y][3 * x + 0] + Ec[y][3 * x + 0] * Ec[y][3 * x + 0]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][3 * x + 0] = value;

					value = sqrt(Er[y][3 * x + 1] * Er[y][3 * x + 1] + Ec[y][3 * x + 1] * Ec[y][3 * x + 1]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][3 * x + 1] = value;

					value = sqrt(Er[y][3 * x + 2] * Er[y][3 * x + 2] + Ec[y][3 * x + 2] * Ec[y][3 * x + 2]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][3 * x + 2] = value;

					value = sqrt(pDoc->ResultImg[y][3 * x + 0] * pDoc->ResultImg[y][3 * x + 0] +
						pDoc->ResultImg[y][3 * x + 1] * pDoc->ResultImg[y][3 * x + 1] +
						pDoc->ResultImg[y][3 * x + 2] * pDoc->ResultImg[y][3 * x + 2]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;

					pDoc->ResultImg[y][3 * x + 0] = value;
					pDoc->ResultImg[y][3 * x + 1] = value;
					pDoc->ResultImg[y][3 * x + 2] = value;

				}


			}

		// 메모리 해제
		for (int i = 0; i < pDoc->ImageHeight; i++)
		{
			free(Er[i]);
			free(Ec[i]);
		}
		free(Er);
		free(Ec);

		Invalidate();
	}


	void CImageProc20211430View::OnRegionSobel()
	{
		CImageProc20211430Doc* pDoc = GetDocument();

		float Hmask[3][3] = { {-1, -2, -1},
						   { 0,  0,  0},
						   { 1,  2,  1} };
		float Vmask[3][3] = { {-1,  0,  1},
								{-2,  0,  2},
								{-1,  0,  1} };

		unsigned char** Er, ** Ec;
		int x, y, value;

		//메모리 할당
		Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
		Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
		for (int i = 0; i < pDoc->ImageHeight; i++)
		{
			Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
			Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		}

		Convolve(pDoc->InputImg, Er, pDoc->ImageWidth, pDoc->ImageHeight, Hmask, 0, pDoc->depth);
		Convolve(pDoc->InputImg, Ec, pDoc->ImageWidth, pDoc->ImageHeight, Vmask, 0, pDoc->depth);

		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][x] = value;
				}
				else {
					value = sqrt(Er[y][3 * x + 0] * Er[y][3 * x + 0] + Ec[y][3 * x + 0] * Ec[y][3 * x + 0]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][3 * x + 0] = value;

					value = sqrt(Er[y][3 * x + 1] * Er[y][3 * x + 1] + Ec[y][3 * x + 1] * Ec[y][3 * x + 1]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][3 * x + 1] = value;

					value = sqrt(Er[y][3 * x + 2] * Er[y][3 * x + 2] + Ec[y][3 * x + 2] * Ec[y][3 * x + 2]);
					if (value > 255)	value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultImg[y][3 * x + 2] = value;

					
				}
			}


		// 메모리 해제
		for (int i = 0; i < pDoc->ImageHeight; i++)
		{
			free(Er[i]);
			free(Ec[i]);
		}
		free(Er);
		free(Ec);

		Invalidate();

	}


	void CImageProc20211430View::OnRegionAverageFiltering()
	{
		CImageProc20211430Doc* pDoc = GetDocument();

		int x, y, i, j;
		int xcor, ycor;
		int sum, count, rsum, gsum, bsum;

		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					sum = 0;
					count = 0;
					for (j = -2; j <= 2; j++)
						for (i = -2; i <= 2; i++)
						{
							ycor = y + j;
							xcor = x + i;

							if (0 <= ycor && ycor <= pDoc->ImageHeight - 1 && 0 <= xcor && xcor <= pDoc->ImageWidth - 1)
							{
								sum += pDoc->InputImg[ycor][xcor];
								count++;
							}
						}

					pDoc->ResultImg[y][x] = sum / count;

				}
				else
				{
					rsum = gsum = bsum = 0;
					count = 0;
					for (j = -2; j <= 2; j++)
						for (i = -2; i <= 2; i++)
						{
							ycor = y + j;
							xcor = x + i;

							if (0 <= ycor && ycor <= pDoc->ImageHeight - 1 && 0 <= xcor && xcor <= pDoc->ImageWidth - 1)
							{
								rsum += pDoc->InputImg[ycor][3 * xcor + 0];
								gsum += pDoc->InputImg[ycor][3 * xcor + 1];
								bsum += pDoc->InputImg[ycor][3 * xcor + 2];
								count++;
							}
						}
					pDoc->ResultImg[y][3 * x + 0] = rsum / count;
					pDoc->ResultImg[y][3 * x + 1] = gsum / count;
					pDoc->ResultImg[y][3 * x + 2] = bsum / count;
				}

				
			}
		Invalidate();
	}


	void CImageProc20211430View::OnRegionMedianFiltering()
	{
		CImageProc20211430Doc* pDoc = GetDocument();
		int x, y, i, j;
		int n[9], temp;

		for (y = 1; y < pDoc->ImageHeight-1; y++)
			for (x = 1; x < pDoc->ImageWidth - 1; x++)
			{
				if (pDoc->depth == 1)
				{
					n[0] = pDoc->InputImg[y - 1][x - 1];
					n[1] = pDoc->InputImg[y - 1][x - 0];
					n[2] = pDoc->InputImg[y - 1][x + 1];
					n[3] = pDoc->InputImg[y - 0][x - 1];
					n[4] = pDoc->InputImg[y - 0][x - 0];
					n[5] = pDoc->InputImg[y - 0][x + 1];
					n[6] = pDoc->InputImg[y + 1][x - 1];
					n[7] = pDoc->InputImg[y + 1][x - 0];
					n[8] = pDoc->InputImg[y + 1][x + 1];

					//sorting
					for (i = 8; i > 0; i--)
						for (j = 0; j < i; j++)
							if (n[j] > n[j + 1])
							{
								temp = n[j + 1];
								n[j + 1] = n[j];
								n[j] = temp;
							}
					pDoc->ResultImg[y][x] = n[4];
				}
				else
				{
					n[0] = pDoc->InputImg[y - 1][3 * (x - 1) + 0];
					n[1] = pDoc->InputImg[y - 1][3 * (x - 0) + 0];
					n[2] = pDoc->InputImg[y - 1][3 * (x + 1) + 0];
					n[3] = pDoc->InputImg[y - 0][3 * (x - 1) + 0];
					n[4] = pDoc->InputImg[y - 0][3 * (x - 0) + 0];
					n[5] = pDoc->InputImg[y - 0][3 * (x + 1) + 0];
					n[6] = pDoc->InputImg[y + 1][3 * (x - 1) + 0];
					n[7] = pDoc->InputImg[y + 1][3 * (x - 0) + 0];
					n[8] = pDoc->InputImg[y + 1][3 * (x + 1) + 0];

//sorting
for (i = 8; i > 0; i--)
for (j = 0; j < i; j++)
	if (n[j] > n[j + 1])
	{
		temp = n[j + 1];
		n[j + 1] = n[j];
		n[j] = temp;
	}
pDoc->ResultImg[y][3 * x + 0] = n[4];

n[0] = pDoc->InputImg[y - 1][3 * (x - 1) + 1];
n[1] = pDoc->InputImg[y - 1][3 * (x - 0) + 1];
n[2] = pDoc->InputImg[y - 1][3 * (x + 1) + 1];
n[3] = pDoc->InputImg[y - 0][3 * (x - 1) + 1];
n[4] = pDoc->InputImg[y - 0][3 * (x - 0) + 1];
n[5] = pDoc->InputImg[y - 0][3 * (x + 1) + 1];
n[6] = pDoc->InputImg[y + 1][3 * (x - 1) + 1];
n[7] = pDoc->InputImg[y + 1][3 * (x - 0) + 1];
n[8] = pDoc->InputImg[y + 1][3 * (x + 1) + 1];

//sorting
for (i = 8; i > 0; i--)
for (j = 0; j < i; j++)
	if (n[j] > n[j + 1])
	{
		temp = n[j + 1];
		n[j + 1] = n[j];
		n[j] = temp;
	}
pDoc->ResultImg[y][3 * x + 1] = n[4];

n[0] = pDoc->InputImg[y - 1][3 * (x - 1) + 2];
n[1] = pDoc->InputImg[y - 1][3 * (x - 0) + 2];
n[2] = pDoc->InputImg[y - 1][3 * (x + 1) + 2];
n[3] = pDoc->InputImg[y - 0][3 * (x - 1) + 2];
n[4] = pDoc->InputImg[y - 0][3 * (x - 0) + 2];
n[5] = pDoc->InputImg[y - 0][3 * (x + 1) + 2];
n[6] = pDoc->InputImg[y + 1][3 * (x - 1) + 2];
n[7] = pDoc->InputImg[y + 1][3 * (x - 0) + 2];
n[8] = pDoc->InputImg[y + 1][3 * (x + 1) + 2];

//sorting
for (i = 8; i > 0; i--)
for (j = 0; j < i; j++)
	if (n[j] > n[j + 1])
	{
		temp = n[j + 1];
		n[j + 1] = n[j];
		n[j] = temp;
	}
pDoc->ResultImg[y][3 * x + 2] = n[4];
				}
			}
			Invalidate();
	}


	void CImageProc20211430View::OnMopologyColorGray()
	{
		CImageProc20211430Doc* pDoc = GetDocument();
		if (pDoc->depth == 1)		return;

		int x, y;
		int gray;

		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
			{
				gray = (pDoc->InputImg[y][3 * x + 0] + pDoc->InputImg[y][3 * x + 1] + pDoc->InputImg[y][3 * x + 2]) / 3;
				pDoc->InputImg[y][3 * x + 0] = gray;
				pDoc->InputImg[y][3 * x + 1] = gray;
				pDoc->InputImg[y][3 * x + 2] = gray;
			}
		Invalidate();
	}


	void CImageProc20211430View::OnMopologyBinarization()
	{
		CImageProc20211430Doc* pDoc = GetDocument();

		int x, y;
		int thresh = 128;
		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					if (pDoc->InputImg[y][x] > thresh)
						pDoc->InputImg[y][x] = 255;
					else
						pDoc->InputImg[y][x] = 0;
				}
				else
				{
					if ((pDoc->InputImg[y][3 * x + 0] + pDoc->InputImg[y][3 * x + 1] + pDoc->InputImg[y][3 * x + 2]) / 3
		> thresh)
					{
						pDoc->InputImg[y][3 * x + 0] = 255;
						pDoc->InputImg[y][3 * x + 1] = 255;
						pDoc->InputImg[y][3 * x + 2] = 255;
					}
					else
					{
						pDoc->InputImg[y][3 * x + 0] = 0;
						pDoc->InputImg[y][3 * x + 1] = 0;
						pDoc->InputImg[y][3 * x + 2] = 0;
					}
				}
			}
		Invalidate();
	}


	void CImageProc20211430View::OnMopologyErosion()
	{
		CImageProc20211430Doc* pDoc = GetDocument();

		int x, y, i, j;
		int min, rmin, gmin, bmin = 255;

		for ( y = 1; y < pDoc->ImageHeight-1; y++)
			for (x = 1; x < pDoc->ImageWidth - 1; x++)
			{
				if (pDoc->depth == 1)
				{
					min = 255;
						for (j = -1; j <= 1; j++)
							for (i = -1; i <= 1; i++)
							{
								if (pDoc->InputImg[y + j][x + i] < min)
									min = pDoc->InputImg[y + j][x + i];
							}
					pDoc->ResultImg[y][x] = min;
				}
				else
				{
					rmin = 255; gmin = 255; bmin = 255;
						for (j = -1; j <= 1; j++)
							for (i = -1; i <= 1; i++)
							{
								if (pDoc->InputImg[y + j][3 * (x + i) + 0] < rmin)
									rmin = pDoc->InputImg[y + j][3 * (x + i) + 0];
								if (pDoc->InputImg[y + j][3 * (x + i) + 1] < gmin)
									gmin = pDoc->InputImg[y + j][3 * (x + i) + 1];
								if (pDoc->InputImg[y + j][3 * (x + i) + 2] < bmin)
									bmin = pDoc->InputImg[y + j][3 * (x + i) + 2];
							}
					pDoc->ResultImg[y][3*x+0] = rmin;
					pDoc->ResultImg[y][3 * x + 1] = gmin;
					pDoc->ResultImg[y][3 * x + 2] = bmin;
				}

			}

		Invalidate();
	}


	void CImageProc20211430View::OnMopologyDilation()
	{

		CImageProc20211430Doc* pDoc = GetDocument();

		int x, y, i, j;
		int max, rmax, gmax, bmax = 0;

		for (y = 1; y < pDoc->ImageHeight - 1; y++)
			for (x = 1; x < pDoc->ImageWidth - 1; x++)
			{
				if (pDoc->depth == 1)
				{
					max = 0;
					for (j = -1; j <= 1; j++)
						for (i = -1; i <= 1; i++)
						{
							if (pDoc->InputImg[y + j][x + i] > max)
								max = pDoc->InputImg[y + j][x + i];
						}
					pDoc->ResultImg[y][x] = max;
				}
				else
				{
					rmax = 0; gmax = 0; bmax = 0;
					for (j = -1; j <= 1; j++)
						for (i = -1; i <= 1; i++)
						{
							if (pDoc->InputImg[y + j][3 * (x + i) + 0] > rmax)
								rmax = pDoc->InputImg[y + j][3 * (x + i) + 0];
							if (pDoc->InputImg[y + j][3 * (x + i) + 1] > gmax)
								gmax = pDoc->InputImg[y + j][3 * (x + i) + 1];
							if (pDoc->InputImg[y + j][3 * (x + i) + 2] > bmax)
								bmax = pDoc->InputImg[y + j][3 * (x + i) + 2];
						}
					pDoc->ResultImg[y][3 * x + 0] = rmax;
					pDoc->ResultImg[y][3 * x + 1] = gmax;
					pDoc->ResultImg[y][3 * x + 2] = bmax;
				}

			}

		Invalidate();
	}


	void CImageProc20211430View::OnMopologyOpening()
	{
		OnMopologyErosion();
		CopyResultToInput();
		OnMopologyErosion();
		CopyResultToInput();
		OnMopologyErosion();

		CopyResultToInput();
		OnMopologyDilation();
		CopyResultToInput();
		OnMopologyDilation();
		CopyResultToInput();
		OnMopologyDilation();
	}



	void CImageProc20211430View::CopyResultToInput()
	{
		CImageProc20211430Doc* pDoc = GetDocument();

		int x, y;

		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth*pDoc->depth; x++)
			{
				pDoc->InputImg[y][x] = pDoc->ResultImg[y][x];
			}
	}


	void CImageProc20211430View::OnMopologyClosing()
	{
		OnMopologyDilation();
		CopyResultToInput();
		OnMopologyDilation();
		CopyResultToInput();
		OnMopologyDilation();

		CopyResultToInput();
		OnMopologyErosion();
		CopyResultToInput();
		OnMopologyErosion();
		CopyResultToInput();
		OnMopologyErosion();
	}


	void CImageProc20211430View::OnGeometryZoominPixelCopy()
	{
		CImageProc20211430Doc* pDoc = GetDocument();

		int x, y;
		int xscale = 3;
		int yscale = 2;
		if (pDoc->gResultImg != NULL)
		{
			for (int i = 0; i < pDoc->gImageHeight; i++)
				free(pDoc->gResultImg[i]);
			free(pDoc->gResultImg);
		}
		pDoc->gImageWidth = pDoc->ImageWidth * xscale;
		pDoc->gImageHeight = pDoc->ImageHeight * yscale;
		// 메모리할당
		pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
		for (int i = 0; i < pDoc->gImageHeight; i++)
		{
			pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
		}
		//역방향사상
		for (y = 0;y < pDoc->gImageHeight;y++)
			for (x = 0; x < pDoc->gImageWidth; x++)
			{
				if (pDoc->depth == 1)
					pDoc->gResultImg[y][x] = pDoc->InputImg[y / yscale][x / xscale];
				else
				{
					pDoc->gResultImg[y][3 * x + 0] = pDoc->InputImg[y / yscale][3 * (x / xscale) + 0];
					pDoc->gResultImg[y][3 * x + 1] = pDoc->InputImg[y / yscale][3 * (x / xscale) + 1];
					pDoc->gResultImg[y][3 * x + 2] = pDoc->InputImg[y / yscale][3 * (x / xscale) + 2];
				}
			}
		/*  // 전방향사상
			for (y = 0; y < pDoc->ImageHeight; y++)
				for (x = 0; x < pDoc->ImageWidth; x++)
				{
					pDoc->gResultImg[y*yscale][x*xscale] = pDoc->InputImg[y][x];

				}
		*/
		Invalidate();
	}


void CImageProc20211430View::OnGeometryZoominBilinearInterpolation()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	int x, y;
	float xscale = 2.3;
	float yscale = 1.5;
	float src_x, src_y;
	float alpha, beta;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	int E, F;
	if (pDoc->gResultImg != NULL)
	{
		for (int i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gResultImg[i]);
		free(pDoc->gResultImg);
	}
	pDoc->gImageWidth = pDoc->ImageWidth * xscale + 0.5;
	pDoc->gImageHeight = pDoc->ImageHeight * yscale + 0.5;
	// 메모리할당
	pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gImageHeight; i++)
	{
		pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}
	//역방향사상
	for (y = 0; y < pDoc->gImageHeight; y++)
		for (x = 0; x < pDoc->gImageWidth; x++)
		{
			src_x = x / xscale;
			src_y = y / yscale;
			alpha = src_x - (int)src_x;
			beta = src_y - (int)src_y;
			Ax = (int)src_x;
			Ay = (int)src_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;
			if (Bx > pDoc->ImageWidth - 1) Bx = pDoc->ImageWidth - 1;
			if (Cy > pDoc->ImageHeight - 1) Cy = pDoc->ImageHeight - 1;
			if (Dx > pDoc->ImageWidth - 1) Dx = pDoc->ImageWidth - 1;
			if (Dy > pDoc->ImageHeight - 1) Dy = pDoc->ImageHeight - 1;
			if (pDoc->depth == 1)
			{
				E = (1 - alpha) * pDoc->InputImg[Ay][Ax] + alpha * pDoc->InputImg[By][Bx];
				F = (1 - alpha) * pDoc->InputImg[Cy][Cx] + alpha * pDoc->InputImg[Dy][Dx];
				pDoc->gResultImg[y][x] = (1 - beta) * E + beta * F;
			}
			else
			{
				E = (1 - alpha) * pDoc->InputImg[Ay][3 * Ax + 0] + alpha * pDoc->InputImg[By][3 * Bx + 0];
				F = (1 - alpha) * pDoc->InputImg[Cy][3 * Cx + 0] + alpha * pDoc->InputImg[Dy][3 * Dx + 0];
				pDoc->gResultImg[y][3 * x + 0] = (1 - beta) * E + beta * F;
				E = (1 - alpha) * pDoc->InputImg[Ay][3 * Ax + 1] + alpha * pDoc->InputImg[By][3 * Bx + 1];
				F = (1 - alpha) * pDoc->InputImg[Cy][3 * Cx + 1] + alpha * pDoc->InputImg[Dy][3 * Dx + 1];
				pDoc->gResultImg[y][3 * x + 1] = (1 - beta) * E + beta * F;
				E = (1 - alpha) * pDoc->InputImg[Ay][3 * Ax + 2] + alpha * pDoc->InputImg[By][3 * Bx + 2];
				F = (1 - alpha) * pDoc->InputImg[Cy][3 * Cx + 2] + alpha * pDoc->InputImg[Dy][3 * Dx + 2];
				pDoc->gResultImg[y][3 * x + 2] = (1 - beta) * E + beta * F;
			}
		}
	Invalidate();
}



 void CImageProc20211430View::OnGeometryZoomoutSubsampling()
 {
	 CImageProc20211430Doc* pDoc = GetDocument();

	 int xscale = 3;
	 int yscale = 2;
	 int x, y;

	 if (pDoc->gResultImg != NULL)
	 {
		 for (int i = 0; i < pDoc->gImageHeight; i++)
			 free(pDoc->gResultImg[i]);
		 free(pDoc->gResultImg);
	 }

	 pDoc->gImageWidth = pDoc->ImageWidth / xscale;
	 pDoc->gImageHeight = pDoc->ImageHeight / yscale;
	 //메모리할당
	 pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	 for (int i = 0; i < pDoc->gImageHeight; i++)
	 {
		 pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	 }

	 //역방향사상
	 for (y = 0; y < pDoc->gImageHeight; y++)
		 for (x = 0; x < pDoc->gImageWidth; x++)
		 {
			 if (pDoc->depth == 1)
				 pDoc->gResultImg[y][x] = pDoc->InputImg[y * yscale][x * xscale];
			 else
			 {
				 pDoc->gResultImg[y][3 * x + 0] = pDoc->InputImg[y * yscale][3 * (x * xscale) + 0];
				 pDoc->gResultImg[y][3 * x + 1] = pDoc->InputImg[y * yscale][3 * (x * xscale) + 1];
				 pDoc->gResultImg[y][3 * x + 2] = pDoc->InputImg[y * yscale][3 * (x * xscale) + 2];


			 }

		 }

	 Invalidate();
 }


 void CImageProc20211430View::OnGeometryZoomoutMeanSub()
 {
	 OnRegionSmoothing();
	 CopyResultToInput();
	 OnGeometryZoomoutSubsampling();

 }


 void CImageProc20211430View::OnGeometryAvgFiltering()
 {
	 CImageProc20211430Doc* pDoc = GetDocument();

	 int xscale = 3;     // 1/3
	 int yscale = 2;     // 1/2
	 int x, y, i, j;
	 int sum, rsum, gsum, bsum;
	 int src_x, src_y;

	 if (pDoc->gResultImg != NULL)
	 {
		 for (int i = 0; i < pDoc->gImageHeight; i++)
			 free(pDoc->gResultImg[i]);
		 free(pDoc->gResultImg);
	 }

	 pDoc->gImageWidth = pDoc->ImageWidth / xscale;
	 pDoc->gImageHeight = pDoc->ImageHeight / yscale;
	 // 메모리할당
	 pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	 for (int i = 0; i < pDoc->gImageHeight; i++)
	 {
		 pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	 }


	 //전방향사상
	 for (y = 0; y < pDoc->ImageHeight; y+= yscale)
		 for (x = 0; x < pDoc->ImageWidth; x+= xscale)
		 {
			 if (pDoc->depth == 1)
			 {

				 sum = 0;
				 for (j = 0; j < yscale; j++)
					 for (i = 0; i < xscale; i++)
					 {
						 src_x = x + i;
						 src_y = y + j;

						 sum += pDoc->InputImg[src_y][src_x];
					 }
				 pDoc->gResultImg[y / yscale][x / xscale] = sum / (yscale * xscale);
			 }
			 else
			 {
				 rsum = 0; gsum = 0; bsum = 0;
				 for (j = 0; j < yscale; j++)
					 for (i = 0; i < xscale; i++)
					 {
						 src_x = x + i;
						 src_y = y + j;

						 rsum += pDoc->InputImg[src_y][3 * src_x + 0];
						 gsum += pDoc->InputImg[src_y][3 * src_x + 1];
						 bsum += pDoc->InputImg[src_y][3 * src_x + 2];
					 }
				 pDoc->gResultImg[y / yscale][3 *(x / xscale) + 0] = rsum / (yscale * xscale);
				 pDoc->gResultImg[y / yscale][3 * (x / xscale) + 1] = gsum / (yscale * xscale);
				 pDoc->gResultImg[y / yscale][3 * (x / xscale) + 2] = bsum / (yscale * xscale);
			 }

		 }

	 Invalidate();
 }

#define PI 3.1416926521

#include "CAngleDialog.h"
void CImageProc20211430View::OnGeometryRotation()
 {
	 CImageProc20211430Doc* pDoc = GetDocument();
	 CAngleDialog dlg;

	 int angle = 30; //degree
	 float radian;
	 int Hy;
	 int Cx, Cy;
	 int x, y, xdiff, ydiff;
	 int x_source, y_source;

	 dlg.m_iAngle = angle;
	 if (dlg.DoModal() == IDCANCEL)  return;
	 angle = dlg.m_iAngle;

	 radian = PI / 180 * angle;
	 //y의 마지막
	 Hy = pDoc->ImageHeight - 1;
	 //중심점
	 Cx = pDoc->ImageWidth / 2;
	 Cy = pDoc->ImageHeight / 2;

	 if (pDoc->gResultImg != NULL)
	 {
		 for (int i = 0; i < pDoc->gImageHeight; i++)
			 free(pDoc->gResultImg[i]);
		 free(pDoc->gResultImg);
	 }

	 pDoc->gImageWidth = pDoc->ImageHeight * fabs(cos(PI / 2 - radian)) + pDoc->ImageWidth * fabs(cos(radian));
	 pDoc->gImageHeight = pDoc->ImageHeight * fabs(cos(radian)) + pDoc->ImageWidth * fabs(cos(PI / 2 - radian));
	 //메모리할당
	 pDoc->gResultImg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	 for (int i = 0; i < pDoc->gImageHeight; i++)
	 {
		 pDoc->gResultImg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	 }

			xdiff = (pDoc->gImageWidth - pDoc->ImageWidth) / 2;
			ydiff = (pDoc->gImageHeight - pDoc->ImageHeight) / 2;

			for (y = -ydiff; y < pDoc->gImageHeight - ydiff; y++)
				for (x = -xdiff; x < pDoc->gImageWidth - xdiff; x++)
				{
					x_source = (Hy - y - Cx) * sin(radian) + (x - Cx) * cos(radian) + Cx;
					y_source = Hy - ((Hy - y - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);

					if (pDoc->depth == 1)
					{
						if (x_source<0 || x_source>pDoc->ImageWidth - 1 ||
							y_source<0 || y_source>pDoc->ImageHeight - 1)
							pDoc->gResultImg[y + ydiff][x + xdiff] = 255;
						else
							pDoc->gResultImg[y + ydiff][x + xdiff] = pDoc->InputImg[y_source][x_source];
					}
					else
					{
						if (x_source<0 || x_source>pDoc->ImageWidth - 1 ||
							y_source<0 || y_source>pDoc->ImageHeight - 1)
						{
							pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 0] = 255;
							pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 1] = 255;
							pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 2] = 255;
						}
						else
						{
							pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 0] = pDoc->InputImg[y_source][3 * x_source + 0];
							pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 1] = pDoc->InputImg[y_source][3 * x_source + 1];
							pDoc->gResultImg[y + ydiff][3 * (x + xdiff) + 2] = pDoc->InputImg[y_source][3 * x_source + 2];
						}
					}
				}

			Invalidate();
 }


void CImageProc20211430View::OnGeometryHolizantialFlip()
{
	CImageProc20211430Doc* pDoc = GetDocument();
	int x, y;

	for(y=0;y<pDoc->ImageHeight;y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			if(pDoc->depth==1)
				pDoc->ResultImg[y][x] = pDoc->InputImg[y][pDoc->ImageWidth - 1 - x];
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = pDoc->InputImg[y][3 * (pDoc->ImageWidth - 1 - x) + 0];
				pDoc->ResultImg[y][3 * x + 1] = pDoc->InputImg[y][3 * (pDoc->ImageWidth - 1 - x) + 1];
				pDoc->ResultImg[y][3 * x + 2] = pDoc->InputImg[y][3 * (pDoc->ImageWidth - 1 - x) + 2];
			}
		}

	Invalidate();
}


void CImageProc20211430View::OnGeometryVerticalFlip()
{
	CImageProc20211430Doc* pDoc = GetDocument();
	int x, y;

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth * pDoc->depth; x++)
		{
			pDoc->ResultImg[pDoc->ImageHeight - 1 - y][x] = pDoc->InputImg[y][x];
		}
	Invalidate();
}



typedef struct
{
	int Px;
	int Py;
	int Qx;
	int Qy;
}control_line;

control_line mctrl_source = { 100,100,150,150 };
control_line mctrl_dest = { 100,100,200,200 };

void CImageProc20211430View::OnGeometryWarping()
{
	CImageProc20211430Doc* pDoc = GetDocument();

	control_line source_lines[5] = { {100,100,150,150},
		{0,0,pDoc->ImageWidth - 1,0},{pDoc->ImageWidth - 1,0,pDoc->ImageWidth - 1,pDoc->ImageHeight - 1},
		{pDoc->ImageWidth - 1, pDoc->ImageHeight - 1,0,pDoc->ImageHeight - 1},{0,pDoc->ImageHeight - 1,0,0} };
	control_line dest_lines[5] = { {100,100,200,200},
		{0,0,pDoc->ImageWidth - 1,0},{pDoc->ImageWidth - 1,0,pDoc->ImageWidth - 1,pDoc->ImageHeight - 1},
		{pDoc->ImageWidth - 1, pDoc->ImageHeight - 1,0,pDoc->ImageHeight - 1},{0,pDoc->ImageHeight - 1,0,0} };

	source_lines[0] = mctrl_source;
	dest_lines[0] = mctrl_dest;

	int x, y;

	double u;	// 수직 교차점의 위치
	double h;	// 제어선으로부터 픽셀의 수직 변위
	double d;	// 제어선과 픽셀 사이의 거리
	double tx, ty;		// 결과영상 픽셀에 대응되는 입력 영상 픽셀 사이의 변위의 합
	double xp, yp;		// 각 제어선에 대해 계산된 입력 영상의 대응되는 픽셀 위치
	double weight;	// 각 제어선의 가중치
	double totalweight;		// 가중치의 합
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	int x1, y1, x2, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int num_lines = 5;	//제어선의 개수
	int line;
	int source_x, source_y;
	int last_row, last_col;

	last_col = pDoc->ImageWidth - 1;
	last_row = pDoc->ImageHeight - 1;

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			tx = 0.0;
			ty = 0.0;
			totalweight = 0.0;

			for (line = 0; line < num_lines; line++)
			{
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;

				dest_line_length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				h = ((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

				if (u < 0)		d = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
				else if (u > 1)	d = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
				else				d = fabs(h);

				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;

				src_line_length = sqrt((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));

				xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

				weight = pow(pow(dest_line_length, p) / (a + d), b);

				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalweight += weight;
			}

			source_x = x + (tx / totalweight);
			source_y = y + (ty / totalweight);

			if (source_x < 0)				source_x = 0;
			if (source_x > last_col)	source_x = last_col;
			if (source_y < 0)				source_y = 0;
			if (source_y > last_row)	source_y = last_row;

			if(pDoc->depth==1)
				pDoc->ResultImg[y][x] = pDoc->InputImg[source_y][source_x];
			else
			{
				pDoc->ResultImg[y][3 * x + 0] = pDoc->InputImg[source_y][3 * source_x + 0];
				pDoc->ResultImg[y][3 * x + 1] = pDoc->InputImg[source_y][3 * source_x + 1];
				pDoc->ResultImg[y][3 * x + 2] = pDoc->InputImg[source_y][3 * source_x + 2];
			}
		}

	Invalidate();
}


CPoint mpos_st, mpos_end;

void CImageProc20211430View::OnLButtonDown(UINT nFlags, CPoint point)
{
	mpos_st = point;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageProc20211430View::OnLButtonUp(UINT nFlags, CPoint point)
{
	mpos_end = point;

	CDC* pDC = GetDC();
	CPen rpen;
	rpen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	pDC->SelectObject(&rpen);

	pDC->MoveTo(mpos_st);
	pDC->LineTo(mpos_end);
	ReleaseDC(pDC);

	int Ax, Ay, Bx, By;
	Ax = mpos_st. x;
	Ay = mpos_st.y;
	Bx = mpos_end.x;
	By = mpos_end.y;

	if (Ax < Bx)	mctrl_source.Px = Ax - (Bx - Ax) / 2;
	else				mctrl_source.Px = Ax + (Bx - Ax) / 2;

	if (Ay < By)	mctrl_source.Py = Ay - (By - Ay) / 2;
	else				mctrl_source.Py = Ay + (By - Ay) / 2;

	mctrl_dest.Px = mctrl_source.Px;
	mctrl_dest.Py = mctrl_source.Py;

	mctrl_source.Qx = mpos_st.x;
	mctrl_source.Qy = mpos_st.y;
	mctrl_dest.Qx = mpos_end.x;
	mctrl_dest.Qy = mpos_end.y;


	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageProc20211430View::OnAviView()
{
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Avi화일(*.avi) |*.avi|모든화일|*.*|");

	if (dlg.DoModal() == IDOK)
	{
		AviFileName = dlg.GetPathName();
		bAviMode = true;
		Invalidate();
	}
}


void CImageProc20211430View::LoadAviFile(CDC* pDC)
{
	PAVIFILE pavi;
	AVIFILEINFO fi;
	int stm;
	PAVISTREAM pstm = NULL;
	AVISTREAMINFO si;
	PGETFRAME pfrm = NULL;
	int frame;
	LPBITMAPINFOHEADER pbmpih;
	unsigned char* image;
	int x, y;

	AVIFileInit();
	AVIFileOpen(&pavi, AviFileName, OF_READ | OF_SHARE_DENY_NONE, NULL);
	AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));

	for (stm = 0; stm < fi.dwStreams; stm++)
	{
		AVIFileGetStream(pavi, &pstm, 0, stm);
		AVIStreamInfo(pstm, &si, sizeof(si));
		if (si.fccType == streamtypeVIDEO)
		{
			pfrm=AVIStreamGetFrameOpen(pstm, NULL);
			for (frame = 0; frame < si.dwLength; frame++)		 //si.dwLength
			{
				pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
				if (!pbmpih)	continue;

				image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);
				/*
				for(y=0;y<fi.dwHeight;y++)
					for (x = 0; x < fi.dwWidth; x++)
					{
						pDC->SetPixel(x, fi.dwHeight-1-y,
							RGB(image[(y * fi.dwWidth + x)*3+2],
								image[(y * fi.dwWidth + x)*3+1],
								image[(y * fi.dwWidth + x)*3+0]));
					}
				*/
				::SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, fi.dwWidth, fi.dwHeight,
					0, 0, 0, fi.dwWidth, image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
				Sleep(33);
			}
		}
	}

	AVIStreamGetFrameClose(pfrm);
	AVIStreamRelease(pstm);
	AVIFileRelease(pavi);
	AVIFileExit();
}


void CImageProc20211430View::OnOpencv()
{
	COpenCVDlg dlg;

	dlg.DoModal();
}
