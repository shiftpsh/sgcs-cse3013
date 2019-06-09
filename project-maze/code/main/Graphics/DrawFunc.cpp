/**************************************************************************
***    
***  Copyright (c) 2000 Design Automation Lab at the Sogang University,
***
***  Affiliation:   DA Lab, Sogang University, Computer Science Department,
***                 Seoul, Korea
***
***  Permission is hereby granted, free of charge, to any person obtaining 
***  a copy of this software and associated documentation files (the
***  "Software"), to deal in the Software without restriction, including
***  without limitation 
***  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
***  and/or sell copies of the Software, and to permit persons to whom the 
***  Software is furnished to do so, subject to the following conditions:
***
***  The above copyright notice and this permission notice shall be included
***  in all copies or substantial portions of the Software.
***
*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***
***
***************************************************************************/

#include <math.h>
#include "..\StdAfx.h"
#include "..\ExternView.h"
#include "..\UI\Zoom.h"
#include "Graphics_DS.h"
#include "Clipping.h"

static bool g_bInitialized = false;

// memory buffer
static CDC g_MemDC, g_MemDC2;
static CBitmap g_Bitmap, g_Bitmap2;

static BYTE *g_pBits = NULL, *g_pBits2 = NULL;
static PBYTE *g_ppRow = NULL, *g_ppRow2 = NULL;

static int g_nWidth, g_nHeight;
static int g_nImageSize;
static int g_nRowBytes;
static WORD g_crBackground = 0xFFFF;
static unsigned char g_nAlpha = 255;

// function prototype
static void fillBackground(int x, int y, int width, int height);
static void clearBuffer2(int cx, int cy);

// attribute setting functions for indirect drawing
int setROP2(int nOpCode);
unsigned char setAlpha(unsigned char a);

// indirect drawing function
bool initBuffer();
void clearBuffer();
void showBuffer();
void freeBuffer();

void DrawFont_I(double x1, double y1, double x2, double y2, COLORREF crColor, LPTSTR szFont, LPTSTR szStr);
void DrawText_I(double x1, double y1, PTSTR szStr);
void DrawLine_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
void DrawScaledLine_I(POINTDBL* pt, int nNum, double width, COLORREF crLineColor);
void DrawBox_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor, int nType);
void DrawBox_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
void DrawSolidBox_I(double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, COLORREF nFillColor);
void DrawHatchBox_I(double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, int nType, COLORREF nFillColor);
void DrawPatternBox_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF crLineColor, COLORREF crFillColor, int nPatternSize, char* pPattern);
void DrawPolyline_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor);
void DrawPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor);
void DrawPatternPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor, int nPatternSize, char* pPattern);

// direct drawing function
void DrawLine(CDC* pDC, double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
void DrawBox(CDC* pDC, double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor);
void DrawSolidBox(CDC* pDC, double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, COLORREF nFillColor);
void DrawHatchBox(CDC* pDC, double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, int nType, COLORREF nFillColor);
void DrawPolyline(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor);
void DrawPolygon(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor);
void DrawHatchPolygon(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, int type, COLORREF crFillColor);
void DrawFont(CDC* pDC, double x1, double y1, double x2, double y2, COLORREF crColor, LPTSTR szFont, LPTSTR szStr);

int setROP2(int nOpCode)
{
	return g_MemDC.SetROP2(nOpCode);
}

unsigned char setAlpha(unsigned char a)
{
	unsigned char t = g_nAlpha;
	g_nAlpha = a;
	return t;
}

bool initBuffer()
{
	DIBSECTION dib;
	HBITMAP hBitmap, hBitmap2;
	BITMAPINFO* pbmi;
	CDC* pDC = g_pView->GetDC();

	if (g_bInitialized)
		return true;
	if (g_MemDC.CreateCompatibleDC(NULL) == false || g_MemDC2.CreateCompatibleDC(NULL) == false)
		return false;

	pbmi = (PBITMAPINFO) malloc(sizeof(BITMAPINFOHEADER));
	if (pbmi == NULL)
		return false;

	g_nWidth = pDC->GetDeviceCaps(HORZRES);
	g_nHeight = pDC->GetDeviceCaps(VERTRES);
	memset((void*)pbmi, 0, sizeof(BITMAPINFOHEADER));
	pbmi->bmiHeader.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biBitCount = 16;
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biWidth = g_nWidth;
	pbmi->bmiHeader.biHeight = g_nHeight;
	pbmi->bmiHeader.biPlanes = 1;

	g_ppRow = (PBYTE*) malloc(sizeof(PBYTE) * g_nHeight);
	g_ppRow2 = (PBYTE*) malloc(sizeof(PBYTE) * g_nHeight);
	hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), pbmi, DIB_RGB_COLORS, (void**)&g_pBits, NULL, 0);
	hBitmap2 = ::CreateDIBSection(pDC->GetSafeHdc(), pbmi, DIB_RGB_COLORS, (void**)&g_pBits2, NULL, 0);
	g_pView->ReleaseDC(pDC);
	free(pbmi);

	if (hBitmap == NULL || hBitmap2 == NULL || g_pBits == NULL || g_pBits2 == NULL || g_ppRow2 == NULL)
		return false;

	// get the information of DIB section
	GetObject(hBitmap, sizeof(DIBSECTION), &dib);
	g_nImageSize = dib.dsBmih.biSizeImage;
	g_nRowBytes = dib.dsBm.bmWidthBytes;

	g_Bitmap.Attach(hBitmap);
	g_Bitmap2.Attach(hBitmap2);
	g_MemDC.SelectObject(&g_Bitmap);
	g_MemDC2.SelectObject(&g_Bitmap2);

	// set row start address
	for(int i = 0; i < g_nHeight; i++)
	{
		g_ppRow[i] = g_pBits + (g_nHeight - i - 1) * g_nRowBytes;
		g_ppRow2[i] = g_pBits2 + (g_nHeight - i - 1) * g_nRowBytes;
	}

	g_bInitialized = true;

	memset((void*)g_pBits, -1, g_nImageSize);
	return true;
}

void clearBuffer()
{
	memset((void*)g_pBits, 0, g_nImageSize);
}

static void clearBuffer2(int cx, int cy)
{
	for(int y = 0; y < cy; y++)
		for(int x = 0; x < cx; x++)
			*(WORD*)(g_ppRow2[y] + x * 2) = 0;
}

static void fillBackground(int x1, int y1, int x2, int y2)
{
	for(int i = y1; i < y2; i++)
	{
		for(int j = x1; j < x2; j++)
		{
			if (*(WORD*)(g_ppRow[i] + j * 2) == 0)
				*(WORD*)(g_ppRow[i] + j * 2) = g_crBackground;
		}
	}
}

void showBuffer()
{
	CRect rect;
	CClientDC dc(g_pView);

	g_pView->GetClientRect(&rect);
	fillBackground(0, 0, rect.right, rect.bottom);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &g_MemDC, 0, 0, SRCCOPY);
}

void printBuffer(CDC* pDC, int cx, int cy, int width, int height)
{
	int x1, y1, x2, y2;
	int xSrc, ySrc, srcWidth, srcHeight, destWidth, destHeight;

	F_trans(Fxorg, Fyorg, x1, y1);
	F_trans(Fxmax, Fymax, x2, y2);
	// find origin of source
	xSrc = x1;
	ySrc = UpsideDown_Flag ? y1 : y2;
	// calculate width, height of source
	srcWidth = x2 - x1;
	srcHeight = UpsideDown_Flag ? y2 - y1 : y1 - y2;
	// calculate width, height of dest
	destWidth = width;
	destHeight = height;
	if (width - srcWidth < height - srcHeight)
		destHeight = width * srcHeight / srcWidth;
	else
		destWidth = height * srcHeight / srcWidth;
	// find origin of dest
	cx = (cx + width - srcWidth) / 2;
	cy = (cy + height - srcHeight) / 2;
	// show buffer contents
	fillBackground(xSrc, ySrc, xSrc + srcWidth, ySrc + srcHeight);
	int caps = pDC->GetDeviceCaps(RASTERCAPS);
//	if (caps & RC_STRETCHBLT)
//		pDC->StretchBlt(cx, cy, destWidth, destHeight, &g_MemDC, xSrc, ySrc, srcWidth, srcHeight, SRCCOPY);
//	else if (caps & RC_BITBLT)
		pDC->BitBlt(cx + abs(destWidth - srcWidth) / 2, cy + abs(destHeight - srcHeight) / 2, srcWidth, srcHeight, &g_MemDC, xSrc, ySrc, SRCCOPY);
}

void refresh()
{
	CRect rect;
	CClientDC dc(g_pView);

	// prevent drawing within invalid areas of the window
	dc.ExcludeUpdateRgn(g_pView);
	g_pView->GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &g_MemDC, 0, 0, SRCCOPY);
}

void freeBuffer()
{
	if (NULL != g_ppRow)
	{
		free(g_ppRow);
		g_ppRow = NULL;
	}
	if (NULL != g_ppRow2)
	{
		free(g_ppRow2);
		g_ppRow2 = NULL;
	}

	g_MemDC.DeleteDC();
	g_MemDC2.DeleteDC();
	g_Bitmap.DeleteObject();
	g_Bitmap2.DeleteObject();
}

void DrawLine_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor)
{
	CPen pen, *pOldPen;
	double px1, px2, py1, py2;
	int dx1, dx2, dy1, dy2;

	// Zoom에 정의되어 있는 physical area내부에 선이 존재할 경우 선을 그린다
	if (ClipLine(x1, y1, x2, y2, &px1, &py1, &px2, &py2))
	{
		// 좌표변환
		F_trans(px1, py1, dx1, dy1);
		F_trans(px2, py2, dx2, dy2);
		// 선그리기
		pen.CreatePen(PS_SOLID, nWidth, nColor);
		pOldPen = g_MemDC.SelectObject(&pen);
		g_MemDC.MoveTo(dx1, dy1);
		g_MemDC.LineTo(dx2, dy2);
		g_MemDC.SelectObject(pOldPen);
		pen.DeleteObject();
	}
}

void DrawBox_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor, int nType )
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	long dx1, dy1, dx2, dy2;

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);
		// box 그리기
		pen.CreatePen(nType, nWidth, nColor);
		brush.CreateStockObject(NULL_BRUSH);
		pOldPen = g_MemDC.SelectObject(&pen);
		pOldBrush = g_MemDC.SelectObject(&brush);
		g_MemDC.Rectangle(dx1, dy1, dx2, dy2);
		g_MemDC.SelectObject(pOldPen);
		g_MemDC.SelectObject(pOldBrush);
		pen.DeleteObject();
		brush.DeleteObject();
	}
}

void DrawBox_I(double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	long dx1, dy1, dx2, dy2;

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);
		// box 그리기
		pen.CreatePen(PS_SOLID, nWidth, nColor);
		brush.CreateStockObject(NULL_BRUSH);
		pOldPen = g_MemDC.SelectObject(&pen);
		pOldBrush = g_MemDC.SelectObject(&brush);
		g_MemDC.Rectangle(dx1, dy1, dx2, dy2);
		g_MemDC.SelectObject(pOldPen);
		g_MemDC.SelectObject(pOldBrush);
		pen.DeleteObject();
		brush.DeleteObject();
	}
}

void DrawSolidBox_I(double x1, double y1, double x2, double y2, 
				  int nLineWidth, COLORREF crLineColor, COLORREF crFillColor)
{
	long dx1, dy1, dx2, dy2, t;

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);

		if (UpsideDown_Flag == 0)
			SWAP(dy1, dy2, t);

		// boundary check
		dx1 = max(0, dx1);
		dy1 = max(0, dy1);
		dx2 = min(dx2, g_nWidth);
		dy2 = min(dy2, g_nHeight);

		// box fill routine
		for(int cy = dy1; cy < dy2; cy++)
			for(int cx = dx1; cx < dx2; cx++)
			{
				WORD src = *(WORD*)(g_ppRow[cy] + cx * 2);
				BYTE r, g, b;
				r = (BYTE)((crFillColor & 0x000000FF) >> 3);
				g = (BYTE)((crFillColor & 0x0000FF00) >> 11);
				b = (BYTE)((crFillColor & 0x00FF0000) >> 19);
				if (src != 0)
				{
					r = BYTE((r * g_nAlpha + ((src & 0x7C00) >> 10) * (255 - g_nAlpha)) / 255);
					g = BYTE((g * g_nAlpha + ((src & 0x03E0) >> 5) * (255 - g_nAlpha)) / 255);
					b = BYTE((b * g_nAlpha + (src & 0x001F) * (255 - g_nAlpha)) / 255);
				}
				*(WORD*)(g_ppRow[cy] + cx * 2) = (WORD) (r << 10 | g << 5 | b) ;
			}
		// draw outline
		DrawBox_I(x1, y1, x2, y2, nLineWidth, crLineColor);
	}
}

void DrawPatternBox_I(double x1, double y1, double x2, double y2, int nWidth,
						COLORREF crLineColor, COLORREF crFillColor, int nPatternSize, char* pPattern)
{
	static unsigned char PatternMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	long dx1, dy1, dx2, dy2, t;
	int nHalfWidth = (int)ceil(nWidth / 2.0);

	if (0 >= nPatternSize || NULL == pPattern)
	{
		DrawBox_I(x1, y1, x2, y2, nWidth, crLineColor);
		return;
	}

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);

		if (UpsideDown_Flag == 0)
			SWAP(dy1, dy2, t);

		// boundary check
		dx1 = max(0, dx1);
		dy1 = max(0, dy1);
		dx2 = min(dx2, g_nWidth);
		dy2 = min(dy2, g_nHeight);

		for(int cy = dy1 + nHalfWidth; cy < dy2 - nHalfWidth; cy++)
		{
			int row = (cy % nPatternSize) * (nPatternSize >> 3);
			for(int cx = dx1 + nHalfWidth; cx < dx2 - nHalfWidth; cx++)
			{
				int col = cx % nPatternSize;
				unsigned char mask = PatternMask[col % 8];

				if ((pPattern[row + (col >> 3)] & mask) == mask)
				{
					// src와 dest의 색을 합하여 그려준다
					WORD src = *(WORD*)(g_ppRow[cy] + cx * 2);
					BYTE r, g, b;
					r = (BYTE)((crFillColor & 0x000000FF) >> 3);
					g = (BYTE)((crFillColor & 0x0000FF00) >> 11);
					b = (BYTE)((crFillColor & 0x00FF0000) >> 19);
					if (src != 0)
					{
						r = BYTE((r * g_nAlpha + ((src & 0x7C00) >> 10) * (255 - g_nAlpha)) / 255);
						g = BYTE((g * g_nAlpha + ((src & 0x03E0) >> 5) * (255 - g_nAlpha)) / 255);
						b = BYTE((b * g_nAlpha + (src & 0x001F) * (255 - g_nAlpha)) / 255);
					}
					*(WORD*)(g_ppRow[cy] + cx * 2) = (WORD) (r << 10 | g << 5 | b) ;
				}
			}
		}
		// draw outline
		DrawBox_I(x1, y1, x2, y2, nWidth, crLineColor);
	}
}

void DrawHatchBox_I(double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, 
				  int nType, COLORREF nFillColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	long dx1, dy1, dx2, dy2;

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);
		// box 그리기
		pen.CreatePen(PS_SOLID, nLineWidth, nLineColor);
		brush.CreateHatchBrush(nType, nFillColor);
		pOldPen = g_MemDC.SelectObject(&pen);
		pOldBrush = g_MemDC.SelectObject(&brush);
		g_MemDC.Rectangle(dx1, dy1, dx2, dy2);
		g_MemDC.SelectObject(pOldPen);
		g_MemDC.SelectObject(pOldBrush);
		pen.DeleteObject(); brush.DeleteObject();
	}
}

void DrawFont_I(double x1, double y1, double x2, double y2, COLORREF crColor, LPTSTR szFont, LPTSTR szStr)
{
  	CFont font, *pOldFont;
	CString str(szStr);
	CRect rect;
	COLORREF crOld;
	long dx1, dy1, dx2, dy2;

	if (ClipBox(x1, y1, x2, y2))
	{
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		// GDI resource 생성
		font.CreateFont(dy2 - dy1, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE, szFont);
		// 환경 설정
		pOldFont = g_MemDC.SelectObject(&font);
		crOld = g_MemDC.SetTextColor(crColor);
		// font 출력
		rect.SetRect(dx1, dy2, dx2, dy1);
		g_MemDC.DrawText(str, &rect, DT_CENTER);
		// DC의 설정을 원상태로 복구
		g_MemDC.SelectObject(pOldFont);
		g_MemDC.SetTextColor(crOld);
		font.DeleteObject();
	}
}

void DrawText_I(double x1, double y1, PTSTR szStr)
{
	int dx1, dy1;

	if (x1 < Pxorg || x1 > Pxmax || y1 < Pyorg || y1 > Pymax)
		return;
	F_trans(x1, y1, dx1, dy1);
	g_MemDC.SetTextColor(RGB(90, 90, 90));
	g_MemDC.TextOut(dx1, dy1, szStr);
}

void DrawPatternPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor, int nPatternSize, char* pPattern)
{
	static unsigned char PatternMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	POINT* ptArray;
	int arrLen;

	if (0 >= nPatternSize || NULL == pPattern)
	{
		DrawPolyline_I(pt, nNum, nWidth, crLineColor);
		return;
	}

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen != 0)
	{
		int x1, y1, x2, y2, tx, ty, cx, cy, t;

		// bounding box의 크기 구함
		F_trans(minx, miny, x1, y1);
		F_trans(maxx, maxy, x2, y2);
		if (UpsideDown_Flag == 0)
			SWAP(y1, y2, t);

		// 오차에 의해 x1, x2, y1, y2의 범위가 잘못 설정되는 것을 방지한다
		x1 = max(x1, 0);
		x2 = min(x2, g_nWidth);
		y1 = max(y1, 0);
		y2 = min(y2, g_nHeight);

		cx = x2 - x1;
		cy = y2 - y1;

		// draw pattern in g_Bitmap2
		for(int i = 0; i < cy; i++)
		{
			int row = (i % nPatternSize) * (nPatternSize >> 3);
			for(int j = 0; j < cx; j++)
			{
				int col = j % nPatternSize;
				unsigned char mask = PatternMask[col % 8];

				if ((pPattern[row + (col >> 3)] & mask) == mask)
				{
					// src와 dest의 색을 합하여 그려준다
					WORD src = *(WORD*)(g_ppRow[y1 + i] + (x1 + j) * 2);
					BYTE r, g, b;
					r = (BYTE)((crFillColor & 0x000000FF) >> 3);
					g = (BYTE)((crFillColor & 0x0000FF00) >> 11);
					b = (BYTE)((crFillColor & 0x00FF0000) >> 19);
					if (src != 0)
					{
						r = BYTE((r * g_nAlpha + ((src & 0x7C00) >> 10) * (255 - g_nAlpha)) / 255);
						g = BYTE((g * g_nAlpha + ((src & 0x03E0) >> 5) * (255 - g_nAlpha)) / 255);
						b = BYTE((b * g_nAlpha + (src & 0x001F) * (255 - g_nAlpha)) / 255);
					}
					*(WORD*)(g_ppRow2[i] + j * 2) = (WORD) (r << 10 | g << 5 | b) ;				
				}
			}
		}

		ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
		for(int i = 0; i < arrLen; i++)
		{
			F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
			ptArray[i].x = (LONG)tx; ptArray[i].y = (LONG)ty;
		}
		g_MemDC2.BeginPath();
		g_MemDC2.Polygon(ptArray, arrLen);
		g_MemDC2.EndPath();
		CRgn rgn;
		rgn.CreateFromPath(&g_MemDC2);
		g_MemDC.SelectClipRgn(&rgn);
		g_MemDC.BitBlt(x1, y1, cx, cy, &g_MemDC2, 0, 0, SRCPAINT);
		g_MemDC.SelectClipRgn(NULL);

		// 경계선을 그린다
		CPen pen, *old;
		pen.CreatePen(PS_SOLID, nWidth, crLineColor);
		old = g_MemDC.SelectObject(&pen);
		g_MemDC.Polyline(ptArray, arrLen);
		g_MemDC.SelectObject(old);
		pen.DeleteObject();
		free(ptArray);
		clearBuffer2(cx, cy);
	}
}

void DrawPolyline_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor)
{
	CPen pen, *pOldPen;
	POINT* ptArray;
	double tx, ty;
	int arrLen;

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen == 0)
		return;

	ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
	for(int i = 0; i < arrLen; i++)
	{
		F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
		ptArray[i].x = round(tx); ptArray[i].y = round(ty);
	}

	pen.CreatePen(PS_SOLID, nWidth, crLineColor);
	pOldPen = g_MemDC.SelectObject(&pen);

	g_MemDC.Polyline(ptArray, arrLen);
	
	g_MemDC.SelectObject(pOldPen);

	pen.DeleteObject();
	free(ptArray);
}

void DrawPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor)
{
	POINT* ptArray;
	int arrLen;

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen != 0)
	{
		int x1, y1, x2, y2, tx, ty, cx, cy, t;

		// bounding box의 크기 구함
		F_trans(minx, miny, x1, y1);
		F_trans(maxx, maxy, x2, y2);
		if (UpsideDown_Flag == 0)
			SWAP(y1, y2, t);

		// 오차에 의해 x1, x2, y1, y2의 범위가 잘못 설정되는 것을 방지한다
		x1 = max(x1, 0);
		x2 = min(x2, g_nWidth);
		y1 = max(y1, 0);
		y2 = min(y2, g_nHeight);

		cx = x2 - x1;
		cy = y2 - y1;

		for(int i = 0; i < cy; i++)
		{
			for(int j = 0; j < cx; j++)
			{
				// src와 dest의 색을 합하여 그려준다
				WORD src = *(WORD*)(g_ppRow[y1 + i] + (x1 + j) * 2);
				BYTE r, g, b;
				r = (BYTE)((crFillColor & 0x000000FF) >> 3);
				g = (BYTE)((crFillColor & 0x0000FF00) >> 11);
				b = (BYTE)((crFillColor & 0x00FF0000) >> 19);
				if (src != 0)
				{
					r = BYTE((r * g_nAlpha + ((src & 0x7C00) >> 10) * (255 - g_nAlpha)) / 255);
					g = BYTE((g * g_nAlpha + ((src & 0x03E0) >> 5) * (255 - g_nAlpha)) / 255);
					b = BYTE((b * g_nAlpha + (src & 0x001F) * (255 - g_nAlpha)) / 255);
				}
				*(WORD*)(g_ppRow2[i] + j * 2) = (WORD) (r << 10 | g << 5 | b) ;				
			}
		}

		ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
		for(int i = 0; i < arrLen; i++)
		{
			F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
			ptArray[i].x = (LONG)tx; ptArray[i].y = (LONG)ty;
		}
		g_MemDC2.BeginPath();
		g_MemDC2.Polygon(ptArray, arrLen);
		g_MemDC2.EndPath();
		CRgn rgn;
		rgn.CreateFromPath(&g_MemDC2);
		g_MemDC.SelectClipRgn(&rgn);
		g_MemDC.BitBlt(x1, y1, cx, cy, &g_MemDC2, 0, 0, SRCCOPY);
		g_MemDC.SelectClipRgn(NULL);

		// 경계선을 그린다
		CPen pen, *old;
		pen.CreatePen(PS_SOLID, nWidth, crLineColor);
		old = g_MemDC.SelectObject(&pen);
		g_MemDC.Polyline(ptArray, arrLen);
		g_MemDC.SelectObject(old);
		pen.DeleteObject();
		free(ptArray);
		clearBuffer2(cx, cy);
	}
}

void DrawHatchPolygon_I(POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, int nType, COLORREF crFillColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	POINT* ptArray;
	int arrLen, tx, ty;

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen != 0)
	{
		ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
		for(int i = 0; i < arrLen; i++)
		{
			F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
			ptArray[i].x = (LONG)tx; ptArray[i].y = (LONG)ty;
		}

		pen.CreatePen(PS_SOLID, nWidth, crLineColor);
		brush.CreateHatchBrush(nType, crFillColor);
		pOldPen = g_MemDC.SelectObject(&pen);
		pOldBrush = g_MemDC.SelectObject(&brush);
		g_MemDC.Polygon(ptArray, arrLen);
		g_MemDC.SelectObject(pOldPen);
		g_MemDC.SelectObject(pOldBrush);
		pen.DeleteObject();
		brush.DeleteObject();
		free(ptArray);
	}
}

void DrawScaledLine_I(POINTDBL* pt, int nNum, double width, COLORREF crLineColor)
{
	CPen pen, *pOldPen;
	POINT* ptArray;
	double tx, ty, dw1, dw2;
	int arrLen;

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen == 0)
		return;

	ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
	for(int i = 0; i < arrLen; i++)
	{
		F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
		ptArray[i].x = round(tx); ptArray[i].y = round(ty);
	}
	F_trans(0, width, dw1, dw2);
	// 적당한 width를 setting
	dw2 = ceil((Dymax * Dxmax) / ((Fxmax - Fxorg) * (Fymax  - Fyorg)) * (dw2 - dw1));
	pen.CreatePen(PS_SOLID, round(dw2), crLineColor);
	pOldPen = g_MemDC.SelectObject(&pen);

	g_MemDC.Polyline(ptArray, arrLen);
	
	g_MemDC.SelectObject(pOldPen);

	pen.DeleteObject();
	free(ptArray);
}

//////////////////////////////////////////////////////////////////////
// direct drawing function

void DrawLine(CDC* pDC, double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor)
{
	CPen pen, *pOldPen;
	double px1, px2, py1, py2;
	int dx1, dx2, dy1, dy2;

	// Zoom에 정의되어 있는 physical area내부에 선이 존재할 경우 선을 그린다
	if (ClipLine(x1, y1, x2, y2, &px1, &py1, &px2, &py2))
	{
		// 좌표변환
		F_trans(px1, py1, dx1, dy1);
		F_trans(px2, py2, dx2, dy2);
		// 선그리기
		pen.CreatePen(PS_SOLID, nWidth, nColor);
		pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(dx1, dy1);
		pDC->LineTo(dx2, dy2);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
}

// 내부가 비어있는 사각형을 그린다
void DrawBox(CDC* pDC, double x1, double y1, double x2, double y2, int nWidth, COLORREF nColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	long dx1, dy1, dx2, dy2;
	int nOldMode;

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);
		// box 그리기
		pen.CreatePen(PS_SOLID, nWidth, nColor);
		brush.CreateStockObject(NULL_BRUSH);
		pOldPen = pDC->SelectObject(&pen);
		pOldBrush = pDC->SelectObject(&brush);
		nOldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->Rectangle(dx1, dy1, dx2, dy2);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pDC->SetBkMode(nOldMode);
		pen.DeleteObject();
		brush.DeleteObject();
	}
}

void DrawSolidBox(CDC* pDC, double x1, double y1, double x2, double y2, 
				  int nLineWidth, COLORREF nLineColor, COLORREF nFillColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	long dx1, dy1, dx2, dy2;

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);
		// box 그리기
		pen.CreatePen(PS_SOLID, nLineWidth, nLineColor);
		brush.CreateSolidBrush(nFillColor);
		pOldPen = pDC->SelectObject(&pen);
		pOldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(dx1, dy1, dx2, dy2);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pen.DeleteObject();
		brush.DeleteObject();
	}
}

void DrawHatchBox(CDC* pDC, double x1, double y1, double x2, double y2, int nLineWidth, COLORREF nLineColor, 
				  int nType, COLORREF nFillColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	long dx1, dy1, dx2, dy2;
	int nOldMode;

	// box가 view의 내부에 있으면 그림을 그린다
	if (ClipBox(x1, y1, x2, y2))
	{
		// 좌표변환
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		CheckBoxPos(&dx1, &dy1, &dx2, &dy2);
		// box 그리기
		pen.CreatePen(PS_SOLID, nLineWidth, nLineColor);
		brush.CreateHatchBrush(nType, nFillColor);
		pOldPen = pDC->SelectObject(&pen);
		pOldBrush = pDC->SelectObject(&brush);
		nOldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->Rectangle(dx1, dy1, dx2, dy2);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pDC->SetBkMode(nOldMode);
		pen.DeleteObject();
		brush.DeleteObject();
	}
}

void DrawPolyline(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor)
{
	CPen pen, *pOldPen;
	POINT* ptArray;
	double tx, ty;
	int arrLen;

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen == 0)
		return;

	ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
	for(int i = 0; i < arrLen; i++)
	{
		F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
		ptArray[i].x = round(tx); ptArray[i].y = round(ty);
	}

	pen.CreatePen(PS_SOLID, nWidth, crLineColor);
	pOldPen = pDC->SelectObject(&pen);

	pDC->Polyline(ptArray, arrLen);
	
	pDC->SelectObject(pOldPen);

	pen.DeleteObject();
	free(ptArray);
}

void DrawPolygon(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, COLORREF crFillColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	POINT* ptArray;
	double tx, ty;
	int arrLen;

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen == 0)
		return;

	ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
	for(int i = 0; i < arrLen; i++)
	{
		F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
		ptArray[i].x = round(tx); ptArray[i].y = round(ty);
	}

	// 환경 설정
	pen.CreatePen(PS_SOLID, nWidth, crLineColor);
	brush.CreateSolidBrush(crFillColor);
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);

	pDC->Polygon(ptArray, arrLen);

	// DC의 설정을 원상태로 복구
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	pen.DeleteObject(); brush.DeleteObject();
	free(ptArray);
}

void DrawHatchPolygon(CDC* pDC, POINTDBL* pt, int nNum, int nWidth, COLORREF crLineColor, int type, COLORREF crFillColor)
{
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;
	POINT* ptArray;
	double tx, ty;
	int arrLen;

	arrLen = ClipPolygon(pt, nNum);
	if (arrLen == 0)
		return;

	ptArray = (POINT*) malloc(sizeof(POINT) * arrLen);
	for(int i = 0; i < arrLen; i++)
	{
		F_trans(vec2.at(i).x, vec2.at(i).y, tx, ty);
		ptArray[i].x = round(tx); ptArray[i].y = round(ty);
	}

	// 환경 설정
	pen.CreatePen(PS_SOLID, nWidth, crLineColor);
	brush.CreateHatchBrush(type, crFillColor);
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);

	pDC->Polygon(ptArray, arrLen);

	// DC의 설정을 원상태로 복구
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	pen.DeleteObject(); brush.DeleteObject();
	free(ptArray);
}

void DrawFont(CDC* pDC, double x1, double y1, double x2, double y2, COLORREF crColor, LPTSTR szFont, LPTSTR szStr)
{
  	CFont font, *pOldFont;
	CString str(szStr);
	CRect rect;
	COLORREF crOld;
	int nOldMode;
	int dx1, dy1, dx2, dy2;

	if (ClipBox(x1, y1, x2, y2))
	{
		F_trans(x1, y1, dx1, dy1);
		F_trans(x2, y2, dx2, dy2);
		font.CreateFont(dy2 - dy1, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE, szFont);

		pOldFont = pDC->SelectObject(&font);
		crOld = pDC->SetTextColor(crColor);
		nOldMode = pDC->SetBkMode(TRANSPARENT);
		// font 출력
		rect.SetRect(dx1, dy2, dx2, dy1);
		pDC->DrawText(str, &rect, DT_CENTER);
		// DC의 설정을 원상태로 복구
		pDC->SelectObject(pOldFont);
		pDC->SetTextColor(crOld);
		pDC->SetBkMode(nOldMode);
		font.DeleteObject();
	}
}
