#pragma once

#include <tchar.h>

//struct PixelTable
//{
//	int width;
//	int height;
//};
//struct PixelTable table[] = {
//	{ 176,120 },	//QCIF_NTSC
//	{ 176,144 },	//QCIF_PAL
//	{ 352,240 },	//CIF_NTSC
//	{ 352,288 },	//CIF_PAL
//	{ 704,480 },	//4CIF_NTSC
//	{ 704,576 },	//4CIF_PAL
//	{ 720,480 },	//D1_NTSC
//	{ 720,576 },	//D1_NTSC
//
//	{ 640,480 },	//VGA
//	{ 800,600 },	//SVGA
//	{ 1024,768 },	//XGA
//	{ 1280,1024 },	//UXGA
//	{ 1600,1200 },	//SXGA
//	{ 1280,720 },	//720HD 
//	{ 1920,1080 },	//1080HD
//};

static int WIDTH_TABLE[] = { 176, 176, 352, 352, 704, 704, 720, 720, 640, 800, 1024,1280,1600, 1280, 1920 };
static int HEIGHT_TABLE[] = { 120, 144, 240, 288, 480, 576, 480, 576, 480, 600, 768, 1024,1200,  720, 1080 };

#ifdef UNICODE
#define NAME(A) _T(#A)
#else
#define NAME(A) (#A)
#endif

static TCHAR *RESOLUTION_TYPE_NAME[] = {
	NAME(QCIF_NTSC),
	NAME(QCIF_PAL),
	NAME(CIF_NTSC),
	NAME(CIF_PAL),
	NAME(4CIF_NTSC),
	NAME(4CIF_PAL),
	NAME(D1_NTSC),
	NAME(D1_NTSC),
	NAME(VGA),
	NAME(SVGA),
	NAME(XGA),
	NAME(UXGA),
	NAME(SXGA),
	NAME(720HD),
	NAME(1080HD)
};

static TCHAR* YUV_TYPE[] =
{
	NAME(YV12),
	NAME(I420),
	NAME(YUY2 | YUYV),
	NAME(YVYU),
	NAME(Only Y),
	NAME(UYVY | UYNV | Y41P),
	NAME(NV12),
	NAME(NV21)
};

static TCHAR* YUV_TYPE_ORDER[] =
{
	NAME([Y][V][U]),
	NAME([Y][U][V]),
	NAME([YUYV]),
	NAME([YVYU]),
	NAME([YYYY]),
	NAME([UYVY]),
	NAME([Y][UV]),
	NAME([Y][VU])
};

static TCHAR* YUV_DISPLAY[] =
{
	NAME(YUV all Componets),
	NAME(Y Componets Only),
	NAME(U Componets Only),
	NAME(V Componets Only),
	NAME(UV Componets),
};
#undef NAME

#define TABLESIZE(A) (sizeof((A))/sizeof((A)[0]))