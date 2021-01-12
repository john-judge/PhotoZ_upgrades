#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800
#pragma once
#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <errno.h>
#include <crtdbg.h>
#include <direct.h>
#include <commctrl.h>
#include <iostream>
#include <complex.h>

#include "smDirectX.h"
#include "sm.h"
#include "smcolors.h"
#include "smconst.h"
#include "smfiles.h"
#include "smsecure.h"
#include "sentc.h"
#include "resource.h"

#include "edtinc.h"
#include "NIDAQmx.h"

#define MSWIN
#define MSWIN32
#define MSWINDLL
#include "comm.h"
//#include "stdafx.h"

#define SCREEN_WIDTH        640  // size of screen
#define SCREEN_HEIGHT       480
extern int screen_height, screen_width, window_client_x0, window_client_y0;

HIDDEN char colorsFnameOld[MAX_PATH] = ".\\SMSYSDAT\\OMCOLORS.DAT",
colorsFname[MAX_PATH] = ".\\SMSYSDAT\\OMCOLORS_NEW.DAT",
*colorsError =     "Error %s Colors File %s",
*openError   =     "Opening",
*closeError  =     "Closing";

char colorsFnamePrf[MAX_PATH];

#define DRAWBLACK		RGB(1,1,1)

HIDDEN int colorMap[] = {	0x00010101,
							0x000000c0,
							0x0000c000,
							0x0000c0c0,
							0x00c00000,
							0x00c000c0,
							0x00c0c000,
							0x00c0c0c0,
							0x00808080,
							0x000000FF,
							0x0000E000,
							0x0000FFFF,
							0x00FF0000,
							0x00FF00FF,
							0x00FFFF00,
							0x00FFFFFF,
							0x00010100 };

#define COLOROFF (sizeof(colorMap)/sizeof(int)-1)

int colors[OMCOLORS];
short int fillPercent[OMPERCENTS];
char genLabels[OMGENLABELS][LABELLEN];
char genAbbrvs[OMGENLABELS][ABBRVLEN];

#define O_MODE (O_BINARY | O_CREAT | O_RDWR)
#define S_MODE (S_IWRITE | S_IREAD)

HFONT oldFont = NULL, omFontBig = NULL, omFontBigFat = NULL, omFontSmall = NULL, omFontMedium = NULL, 
			omFontData = NULL, omFontDataSm = NULL, omFontRprt = NULL, omFontRprtTn = NULL,
			CourierFont;

//64HFONT oldFont,omFontBig = NULL,omFontBigFat = NULL,omFontSmall = NULL,/*64omFontXSmall1 = NULL,*/omFontMedium = NULL,
//64	omFontData = NULL,omFontDataSm = NULL,/*64omFontRprtBg1=NULL,*//*64omFontRprtMd1=NULL,*/omFontRprt=NULL,/*64omFontRprtSm1=NULL,*/omFontRprtTn=NULL;

static int buttonWidth,buttonHeight,scrollColor;

static int keyStrX;
char *dataString = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

#define UPARROW			"U"//"\030"
#define DNARROW	 		"D"//"\031"
#define RTARROW			"R"//"\032"
#define LTARROW			"L"//"\033"

RB_CPLUS_APIPACKET    ApiPacket;    /* Cplus packet                         */

#define SCAN_F1		0x3B
#define SCAN_F2		0x3C
#define SCAN_F3		0x3D
#define SCAN_F4		0x3E
#define SCAN_F5		0x3F
#define SCAN_F6		0x3F
#define SCAN_F7		0x40
#define SCAN_F8		0x41
#define SCAN_F9		0x42
//???
#define SCAN_HOME	0x47
#define SCAN_UP		0x48
#define SCAN_PGUP	0x49
#define SCAN_LEFT	0x4B
#define SCAN_RIGHT	0x4D
#define SCAN_END	0x4F
#define SCAN_DOWN	0x50
#define SCAN_PGDN	0x51
#define	SCAN_INS	0x52
#define SCAN_DEL	0x53

OMOPTIONTYPE OMOPTION[] = {
{		 0,		 2, 12,0,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 1,		 2, 16,0,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 2,		 4, 16,0,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 3,		 4, 16,0,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 4,		 6, 0, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 5,		 8, 0, -1,0,0,			0,0,NULL,	NULL,			4,"",		NULL  },
{		 6,		11, 0, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 7,		 9, 0, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 8,		13, 0, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		 9,		14, 0, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		10,		16, 4, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		11,		17, 4, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		12,		18, 4, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		13,		20, 0, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		14,		35, 12, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		15,		36, 12, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		16,		 8, 21, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{		17,		38, 0, -1,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{(OM_QUIT+1),	27, 0, 0,0,0,			0,0,NULL,	NULL,			0,"",		NULL  },
{(OM_QUIT+2),	0,  0, 0,0,SCAN_LEFT,	0,0,VK_LEFT,DIK_LEFT,		0,NULL,		NULL  },
{(OM_QUIT+3),	0,  0, 0,0,SCAN_RIGHT,	0,0,VK_RIGHT,DIK_RIGHT,		0,NULL,		NULL  },
{(OM_QUIT+4),	0,  0, 0,0,SCAN_UP,		0,0,VK_UP,	DIK_UP,			0,NULL,		NULL  },
{(OM_QUIT+5),	0,  0, 0,0,SCAN_DOWN,	0,0,VK_DOWN,DIK_DOWN,		0,NULL,		NULL  },
{(OM_QUIT+6),	0,  0, 0,0,0,			0,0,VK_SUBTRACT,DIK_SUBTRACT,0,NULL,	NULL  },
{(OM_QUIT+7),	0,  0, 0,0,0,			0,0,VK_ADD,	DIK_ADD,		0,NULL,		NULL  },
{(OM_QUIT+8),	0,  0, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+9),	0,  0, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+10),	22, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+11),	23, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+12),	24, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+13),	25, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+14),	26, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+15),	27, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+16),	28, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+17),	29, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+18),	30, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+19),	31, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+20),	32, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+21),	33, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
{(OM_QUIT+22),	34, 1, 0,0,0,			0,0,0,		0,				0,NULL,		NULL  },
};

/****NI dynamic linked functions ****
HINSTANCE hGetProcIDDLL;
typedef int (__stdcall* func_ptr_t)(...);
func_ptr_t func_ptr[DLL_FUNCTIONS_N];
/******/


// OM operating parameters
float xarray[XYBUFSIZE],yarray[XYBUFSIZE];
int traceX[XYBUFSIZE], traceY[XYBUFSIZE];
OMDATATYPE d = { 0,0,0,0,xarray,yarray };
int omSize, omSizeSave;
static int hotButton = -1, hotChckBox = -1, hotButtonHot = FALSE;
int sampleTime = 3,drawColor0=YELLOW,drawColor;

HIDDEN int colorDarkShadow = DKGRAY, colorLightShadow = MIDGRAY, colorRegular = LTGRAY, colorLight = WHITE, colorBlack = BLACK, colorWindow = LTGRAY, colorGrayText = LTGRAY2, colorText = BLACK,
			colorTab = LTGRAY, colorCheckBox = LTGRAY, colorArrow = LTGRAY, colorDrawBlack = RGB(1,1,1), 
			colorInfoBkgrnd = LTGRAY;

extern char home_dir[MAX_PATH];
extern HMENU	main_menu_handle, layout_menu, ROI_menu, OMIT_menu, FFT_menu;

extern HCURSOR arrowCursor,crossCursor;
extern int popup_flag, filePHT_flag, hide_overscan_flag, num_bit_line, seq_pt_cnt, curSeq, seq_click_x, pat_cnt;
extern bool seq_bitAr[2048][24];
extern int func_pat_seq[100];
extern char func_ar[100][50];
extern char subr_ar[100][50], func_ar[100][50], pat_ar[100][50];
extern char bitN_ar[30][10], seq_name[256];
extern int seq_reps_ar[3][100];
extern int this_progCycles;
extern HWND main_window_handle, tools_window_handle, disp_setting_handle;
extern HINSTANCE main_instance; 
extern int crossCursorOn, viewer_flag, acq_gain;
extern char multi_file_names[50][_MAX_FNAME];

bool win64_flag;
char dirSYSDAT[MAX_PATH] = "\\SMSYSDAT";
char dirIMAGES[MAX_PATH] = "\\SMDATA";
char prefFname[MAX_PATH] = ".\\SMSYSDAT\\DEFAULT.PRF";
char tmp_file_dir[MAX_PATH];

OMTYPE OM[2500];

OMTYPE OM_BASIC[] = {	
{ OM_MAIN,		 OM_NONE,		OM_DISPLAY, OM_WARM,0,			0,"",					0,					10,10,768,576, &colorTab		},
{ OM_MEASUREVIEW,OM_MAIN,		OM_TAB,		OM_WARM,0,			1,"Data Display",		0,					15, 0, 100, 19, &colorTab },
{ SM_MOVIEVIEW,	 OM_MAIN,		OM_TAB,		OM_COLD,0,			1,"Movie Display",		0,					0, 0, 100, 19, &colorTab },
{ SM_CARDIOVIEW, OM_MAIN,		OM_NULL,		OM_COLD,0,			1,"Cardio Analysis",	0,					0, 0, 100, 19, &colorTab },
{ SM_MOVIEWIN,	 SM_MOVIEVIEW,	OM_NULL,	OM_COLD,0,			1,"",					0,					10, 10,  10,  10, &colorBlack },
{ SM_CARDIOWIN,	 SM_CARDIOVIEW,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 80, 19, &colorTab		},
{ OM_ALLBIOPSY,  OM_MAIN,		OM_NULL,	OM_COLD,0,			1,"All Biopsies",		0,					0, 0, 80, 19, &colorTab		},
{ OM_OPTIONSVIEW,OM_MAIN,		OM_NULL,	OM_COLD,0,			1,"Options",			0,					0, 0, 80, 19, &colorTab		},
{ OM_HELPVIEW,	 OM_MAIN,		OM_NULL,    OM_COLD,0,			1,"How To",				0,					0, 0, 80, 19, &colorTab		},
{ OM_MEASUREWIN, OM_MEASUREVIEW,OM_DISPLAY, OM_WARM,0,			1,"",					0,					10,10,768,576, &colorBlack },
{ SM_TRACEWIN,	 OM_MEASUREVIEW,OM_DISPLAY, OM_WARM,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_QUESTION,	 SM_TRACEWIN,OM_VIDEOBUTTON,OM_WARM,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CARDIOPNL,	 SM_CARDIOVIEW,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_MKMAP,		 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_CABINUP,	 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_CABINDN,	 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_CABINLB,	 SM_CARDIOPNL,	OM_LABEL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MAPBOX1,	 SM_CARDIOPNL,	OM_CHCKBX2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MAPBOX2,	 SM_CARDIOPNL,	OM_CHCKBX2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MAPBOX3,	 SM_CARDIOPNL,	OM_CHCKBX2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MAPBOX4,	 SM_CARDIOPNL,	OM_CHCKBX2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ MAP_CALC2,	 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ MAP_CALC3,	 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ MAP_CALC4,	 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_CASTART,	 SM_CARDIOPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CASTART_UP, SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CASTART_DN, SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CAEND,		 SM_CARDIOPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CAEND_UP,	 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CAEND_DN,	 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ CUR_CA_FRM,	 SM_CARDIOPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ CUR_CA_FRM_UP, SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ CUR_CA_FRM_DN, SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ ACTV_START,	 SM_CARDIOPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ ACTV_START_UP, SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ ACTV_START_DN, SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ ACTV_END,		 SM_CARDIOPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ ACTV_END_UP,	 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ ACTV_END_DN,	 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ CV_BINUP,		 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ CV_BINDN,		 SM_CARDIOPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ CV_BINLB,		 SM_CARDIOPNL,	OM_LABEL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ CARDIO_PLAY,	 SM_CARDIOPNL,	OM_VIDEOBUTTON,OM_COLD,0,		0,"",					0,					0, 0,  0,  0, &colorWindow },
{ CARDIO_PAUSE,	 SM_CARDIOPNL,	OM_VIDEOBUTTON,OM_COLD,0,		0,"",					0,					0, 0,  0,  0, &colorWindow },
{ CARDIO_COLOR,	 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ CARDIO_BW,	 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_APD50,		 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_APD80,		 SM_CARDIOPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MOVIEPNL,	 SM_MOVIEVIEW,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_MKMOVIE,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_RDOVRLAY,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_DATAOVLY,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MVSUBBOX,	 SM_MOVIEPNL,	OM_CHCKBX2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MVFLTBOX,	 SM_MOVIEPNL,	OM_CHCKBX2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MVBINUP,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MVBINDN,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MVBINLB,	 SM_MOVIEPNL,	OM_LABEL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ OV_MODE_LB,	 SM_MOVIEPNL,	OM_LABEL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_OV_MODE1,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_OV_MODE2,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_OV_MODE3,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ MOV_SCALE_LB,	 SM_MOVIEPNL,	OM_LABEL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MV_SCALE1,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MV_SCALE2,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MV_SCALE3,	 SM_MOVIEPNL,	OM_BUTTON2,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ SM_MVSTART,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_MVSTART_UP, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_MVSTART_DN, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_MVEND,		 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_MVEND_UP,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_MVEND_DN,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCALEMIN,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMIN_UP,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMIN_DN,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCALEMIN2, SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMIN_UP2, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMIN_DN2, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCALEMAX,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMAX_UP,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMAX_DN,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCALEMAX2, SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMAX_UP2, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_SCMAX_DN2, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_DISP_CUT,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_DISPCUT_UP,SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_DISPCUT_DN,SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MV_TRANSPARENC,SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MV_TRANSP_UP,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MV_TRANSP_DN,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CURMOVF,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CURMOVF_UP, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_CURMOVF_DN, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_FRAME1,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_F1_UP,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_F1_DN,	 SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_NUMCOL,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_NUMC_UP, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_NUMC_DN, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_NUMROW,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_NUMR_UP, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_NUMR_DN, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_STEP,	 SM_MOVIEPNL,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_STEP_UP, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MULTI_STEP_DN, SM_MOVIEPNL,	OM_ARROW,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_PLAY,		 SM_MOVIEPNL,	OM_VIDEOBUTTON,OM_COLD,0,		0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_PAUSE,	 SM_MOVIEPNL,	OM_VIDEOBUTTON,OM_COLD,0,		0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_MODE_LB,	 SM_MOVIEPNL,	OM_LABEL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox },
{ MOV_MOVIE,	 SM_MOVIEPNL,	OM_RADIOBUTTON,OM_COLD,0,		0,"",					0,					0, 0,  0,  0, &colorWindow },
{ MOV_MULTI,	 SM_MOVIEPNL,	OM_RADIOBUTTON,OM_COLD,0,		0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_IMGSCBASE,	 OM_MEASUREWIN,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_IMGSCALE,	 SM_IMGSCBASE,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_IMGSCLUP,	 SM_IMGSCBASE,	OM_NULL,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_IMGSCLDN,	 SM_IMGSCBASE,	OM_NULL,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_IMGSCRUP,	 SM_IMGSCBASE,	OM_NULL,	OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_IMGSCRDN,	 SM_IMGSCBASE,	OM_NULL,	OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_DATAREAD,	 OM_MEASUREWIN,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_NEXTREAD,	 SM_DATAREAD,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_PREVREAD,	 SM_DATAREAD,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_FIRSTFRM,	 SM_DATAREAD,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNCPNL,	 OM_MEASUREWIN,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNC1,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_BNC2,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNC3,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNC4,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNC5,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNC6,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNC7,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_BNC8,		 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_BNC_FLTR,	 SM_BNCPNL,		OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_BNCGAIN,	 SM_BNCPNL,		OM_SLIDER,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SM_BNCGNUP,	 SM_BNCPNL,		OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_BNCGNDN,	 SM_BNCPNL,		OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SP_FLTPNL,	 OM_MEASUREWIN,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SP_LPTYPE_UP,	 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_LPTYPE_DN,	 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_LP_UP,		 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_LP_DN,		 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_LP_LABEL,	 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_HPTYPE_UP,	 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_HPTYPE_DN,	 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_HP_UP,		 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_HP_DN,		 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SP_HP_LABEL,	 SP_FLTPNL,		OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SM_TOOLPNL,	 OM_MEASUREWIN,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TPNLTEXT,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_PLAY,		 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_PAUSE,		 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_NEXTFRAME,  SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_PREVFRAME,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_DRAW,		 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_DRAGBOX,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_KERNEL,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_KSIZEUP,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_KSIZEDN,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_REMOVE,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_THRESHOLD,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SM_ADD_CUR_ROI,SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow },
{ SM_TPOLARITY,	 SM_TOOLPNL,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_TRCEXP,	 SM_TRACEWIN,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TRCEXP_UP,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TRCEXP_DN,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_LOWPASS,	 SM_TRACEWIN,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_LOWPASS_UP, SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_LOWPASS_DN, SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_HIGHPASS,	 SM_TRACEWIN,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_HIGHPASS_UP,SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_HIGHPASS_DN,SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TRCSTART,	 SM_TRACEWIN,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TRCSTT_UP,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TRCSTT_DN,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_REFSTART,	 SM_TRACEWIN,	OM_SLIDER,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_REFSTT_UP,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_REFSTT_DN,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TRCGAIN,	 SM_TRACEWIN,	OM_SLIDER,	OM_COLD,500,		1,"",					0,					0, 0, 76, 19, &colorWindow		},
{ SM_TRCGNUP,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_TRCGNDN,	 SM_TRACEWIN,	OM_ARROW,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow },
{ SM_HISTGAIN,   OM_MEASUREWIN,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_HISTEXP,	 OM_MEASUREWIN,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0,  0,  0, &colorArrow,	&OMOPTION[OM_VWUPARR]},
{ OM_MENUBOX,	 OM_NONE,		OM_MENU,    OM_WARM,0,			1,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_FNAMEBOX,	 OM_MENUBOX,	OM_NAVIGTR,	OM_WARM,0,			1,"",					0,					0, 0,  0,  0, &colorWindow		},
{ SM_FNAMEUPARR, SM_FNAMEBOX,	OM_ARROW,	OM_COLD,OM_UPARR,	1,"",					0,					0, 0,  0,  0, &colorArrow,	&OMOPTION[OM_SCRUPARR] },
{ SM_FNAMEDNARR, SM_FNAMEBOX,	OM_ARROW,	OM_COLD,OM_DNARR,	1,"",					0,					0, 0,  0,  0, &colorArrow,	&OMOPTION[OM_SCRDNARR] },
{ OM_OBJBOX,	 OM_MENUBOX,	OM_NAVIGTR, OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ OM_OBJUPARR,	 OM_OBJBOX,		OM_ZOOM,	OM_COLD,OM_ZOOMIN,	0,"",					0,					0, 0,  0,  0, &colorWindow,	&OMOPTION[OM_OBUPARR] },
{ OM_OBJDNARR,	 OM_OBJBOX,		OM_ZOOM,	OM_COLD,OM_ZOOMOUT,	0,"",					0,					0, 0,  0,  0, &colorWindow,	&OMOPTION[OM_OBDNARR] },
{ OM_NAVIBOX,	 OM_MENUBOX,	OM_NAVIGTR, OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorWindow		},
{ OM_FLDLTARR,	 OM_NAVIBOX,	OM_ARROW,   OM_COLD,OM_LTARR,	0,"",					0,					0, 0,  0,  0, &colorWindow,	&OMOPTION[OM_NGLTARR] },
{ OM_FLDRTARR,	 OM_NAVIBOX,	OM_ARROW,   OM_COLD,OM_RTARR,	0,"",					0,					0, 0,  0,  0, &colorWindow,	&OMOPTION[OM_NGRTARR] },
{ OM_FLDUPARR,	 OM_NAVIBOX,	OM_ARROW,   OM_COLD,OM_UPARR,	0,"",					0,					0, 0,  0,  0, &colorWindow,	&OMOPTION[OM_NGUPARR] },
{ OM_FLDDNARR,	 OM_NAVIBOX,	OM_ARROW,   OM_COLD,OM_DNARR,	0,"",					0,					0, 0,  0,  0, &colorWindow,	&OMOPTION[OM_NGDNARR] },
{ SM_HISTOBOX,	 OM_MENUBOX,	OM_NULL,	OM_COLD,0,			1,"",					0,					0, 0,  0,  0, &colorCheckBox,	&OMOPTION[SM_HISTCHCK] },
{ SM_DKSUBBOX,	 OM_MENUBOX,	OM_CHCKBOX,	OM_COLD,0,			1,"",					0,					0, 0,  0,  0, &colorCheckBox,	&OMOPTION[SM_DSUBCHCK] },
{ SM_HLOCKBOX,	 OM_MENUBOX,	OM_CHCKBOX,	OM_COLD,0,			1,"",					0,					0, 0,  0,  0, &colorCheckBox,	&OMOPTION[SM_HLCKCHCK] },
{ SM_AUTOSBOX,	 OM_MENUBOX,	OM_CHCKBOX,	OM_COLD,0,			1,"",					0,					0, 0,  0,  0, &colorCheckBox,	&OMOPTION[SM_AUTOCHCK] },
{ SM_FOCUSBOX,	 OM_MENUBOX,	OM_CHCKBOX,	OM_COLD,0,			1,"",					0,					0, 0,  0,  0, &colorCheckBox,	&OMOPTION[SM_FCSCHCK] },
{ SM_OVWRTBOX,	 OM_MENUBOX,	OM_CHCKBOX,	OM_COLD,0,			0,"",					0,					0, 0,  0,  0, &colorCheckBox,	&OMOPTION[OM_OBJ]	},
{ SM_RESET,		 OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Reset Camera",		0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_RSTCM]	},
{ SM_LIVE,		 OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Live",				0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_LVSTP]	},
{ SM_STREAM,	 OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Record",				0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_STRM]	},
{ SM_CAPTURE,	 OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Single Frame",		0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_CAPT]	},
{ SM_DKFRAME,	 OM_MENUBOX,	OM_BUTTON,	OM_COLD,0,			0,"Dark Frame",			0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_DARK]	},
{ SM_AUTOOFFSET, OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Set Dark Offsets",	0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_AUTOO]	},
{ SM_DARKSUB,	 OM_MENUBOX,	OM_BUTTON,	OM_COLD,0,			0,"Dark/Ref Subtract",	0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_DSUB]	},
{ SM_AUTOSCL,	 OM_MENUBOX,	OM_BUTTON,	OM_COLD,0,			0,"Auto Scale (Ctr-Drag)",	0,				0, 0,  0,  0, &colorText,		&OMOPTION[SM_AUTOS]	},
{ SM_LOCKHIST,	 OM_MENUBOX,	OM_BUTTON,	OM_COLD,0,			0,"Lock Hist Range",	0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_LOCKH]	},
{ SM_CSETTING,	 OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Camera Settings",	0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_CAMST]	},
{ SM_OVERWRT,	 OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Overwrite",			0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_OVWRT]	},
{ SM_BROWSE,	 OM_MENUBOX,	OM_BUTTON,	OM_COLD,0,			0,"Browse",				0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_BRWS]	},
{ SM_FOCUS,		 OM_MENUBOX,	OM_BUTTON,	OM_COLD,0,			0,"Focus Mode",			0,					0, 0,  0,  0, &colorText,		&OMOPTION[SM_FCS]	},
{ OM_OPTQUIT,	 OM_MENUBOX,	OM_BUTTON,  OM_COLD,0,			0,"Quit",				0,					0, 0,  0,  0, &colorText,		&OMOPTION[OM_QUIT]	},
};

//********************* Turbo-SM ************
void drawSelectedPixels(int for_movie, int for_tiff);
void draw_HLine(int x1, int x2, int y, int color, int for_tiff);
void draw_VLine(int y1, int y2, int x, int color, int for_tiff);

#define NUM_TRACES_MAX	32	
#define FILE_AV_MAX	256	
#define MAX_DATA_FRAMES 100000

int pcdFlag = 0, Bit16_flag = 0, float_flag = 0, colorTB_id = 0;
int cardio_map_sel[] = { 1, 0, 0, 0 };
char experiment[_MAX_FNAME],biopsy[_MAX_FNAME];
int runCam_flag, noCameraFlag, demoFlag, photonTFlag, photonDFlag, line_profile_flag, line_profileXY0, line_profileXY, seqFlag = FALSE, comFlag, pht_auto_scale, hotTab, oldWin_flag, fd_no_deinterleave_flag, newDarkFlag = 0, seq_zoom_x = 0;
double seq_zoom_f = 1.0;
int frameLiveFlag, noise_seq_flag, add_roi_flag, hlighted_roi=0, threshold_flag, roi_transp_level, roi_border_flag, roi_label_mode, skip_edge_rows, joe_version = 0, mark_center_flag = 1;
int pdv_chan_num, RIC_flag, RIC_mean, num_RIC_frames, seqROI_flag, edge_correct_flag, edge_correct_mode, edge_c_col1, edge_c_col2, RLI_div_flag = 0, RLI_div_flag2 = 0, live_factor;
int NDR_subtraction_flag, NDR_subtraction_frames, num_NDR_resets, NDR_cCDS_flag, movie_play_mode, movie_step, movie_rep_cnt, tiffProcFlag, NDR_trace_sub, writeToDisk_flag, multi_pix_read, multi_pix_av, TiffBit_mode = 0;
int trace_sub_mode, trace_sub_f1, trace_sub_f2, movie_scale_mode, file_bin, trace_subtracted, movie_normalized, filter_applied, BNCfile_only = 0;
int movie_bin = 1, mov_bin_used, mov_ref_sub = 1, mov_flt_flag = 1, mov_start, mov_end, mov_num_col, mov_num_row, numMovFrames, numOverlayFrames;
int actv_start, actv_end, actv_flag = 0, apd_flag = 0, cv_flag = 0, apd_mode = 0, ap_color = 0, cv_bin = 1;
int mov_min, mov_min1, mov_min2, mov_max, mov_max1, mov_max2, curMovFrame = 0, mov_cutoff = -1000, mov_multi_f1 = 0, mov_multi_num_col = 4, mov_multi_num_row = 3, max_multi_numf = 20, mov_multi_steps = 2;
int inset_x, inset_y, inset_dx, inset_dy, draw_x0;
int get_overlay_flag = 0, ov_num_col, ov_num_row, overlay_mode = 0, ovFile_num_col, ovFile_num_row, mov_multi_flag = 0, mk_omit_flag = 0;
int sp_lp_type = 0, sp_hp_type = 0, sp_lpass = 1, sp_hpass = 1;
int sp_lp_type_sv = 0, sp_hp_type_sv = 0, sp_lpass_sv = 1, sp_hpass_sv = 1;
signed short *mov_trace_scale_min = NULL, *mov_trace_scale_max = NULL;
int mov_omit_val = -32000;
double RLI_mean = 0.0;
char mov_bin_str[256], sp_lp_str[256], sp_hp_str[256];
int well96_flag = 0, well_x, well_y;
double sp_calibrate, movie_transparency = 0.5;
char file_cameraName[256];
PdvDev *pdv_p = NULL, *pdv_pt[MAXCHANNELS], *pcd_p = NULL;
unsigned int rainbow_LUT[256], regular_LUT[256];
int seqROI_ar[20][8];
int save_sm_f_type, save_sm_f_f1, save_sm_f_f2, save_sm_f_x0, save_sm_f_y0, save_sm_f_w, save_sm_f_h, trace_win_ysize, trace_win_xsize;
SYSTEMTIME lastSystemTime;
bool whiteBackgrnd = 0, logFlag = 0;
FILE *log_fp;
int arrow_w;

//unsigned char BmpLine[IMAGER_SIZE * 2 * 3];
unsigned int *MovieTiffImg = NULL;
unsigned int ImageLine[IMAGER_SIZE * 2 * 4];
float dispImage[IMAGER_SIZE][IMAGER_SIZE * 2];
short int SelectedMap[IMAGER_SIZE+1][IMAGER_SIZE*2];
signed short int *image_data = NULL;
float32 *image_data_f = NULL;
signed short int *movie_data = NULL;
signed short int *activation_fr_ar = NULL, *apd_ar = NULL, *cv_ar = NULL;
double *cv_x_ar = NULL, *cv_y_ar = NULL;

int num_PHT_sample = 0;
double PHT_noise_ar[8][1800];
double min_PHT_noise[8];

float smTrace[32][MAX_DATA_FRAMES];	//last point for dark av
float subtractionTrace[MAX_DATA_FRAMES];
float this_smTrace[MAX_DATA_FRAMES+500];
long int *avImage = NULL;
signed short int *darkImage = NULL;
signed short int *dark_data = NULL, *RIC_image = NULL;
signed short int *single_img_data = NULL, *overlay_img = NULL, *omit_ar = NULL;
float *single_img_float = NULL, *RLI_image = NULL, *ref_image = NULL;
signed short int   BNC_data[20000000];
unsigned long single_img_size;
int cam_num_col, cam_num_row, cam_num_col_last, cam_num_row_last, bit_depth, img_x0, img_y0;
short int file_BNC_ratio = 1, BNC_filter_flag;
double zoom_factor;
int config_num_col, config_num_row;
int NDR_num_f, file_frame_w, focus_frame_interval,auto_scale_w, auto_scale_w_acq;
int scale_max, scale_min, scaleLeft, scaleRight, darkExist, d_darkExist, dataExist, darkSubFlag, output_dksub_flag, showHistFlag, lockHistFlag, scaleDragFlag=FALSE, FFTlog_flag;
int f_subtract_mode, ref_frame1, ref_frame2, frame_d_mode, spatial_bin, trace_min_frame, trace_layout, NDR_pos, exp_sub_flag=0, exp_sub_f1, exp_sub_f2;
int num_av_disp, av_stt, av_end, pixel_trace_displayed, RLI_f1=5, RLI_f2=9;
int autoScaleFlag, PlayFlag, BNC_exist, file_version;
int histo_max, histo_min, histoLeft, histoRight, QuadFlag, TenFramesFlag, auto_dark_flag, splitterAlignMode, splitterMode;
float max_pix, min_pix, max_pix2, min_pix2;
int trace_glider_x;
int curConfig, curCamGain, curChipGain, chipgainSave, fixedCamGain, curNFrames, preTrigger_ms, num_out_f, num_trials, repeat_delay, save_averaged, spike_triggered;
int dataFile, streamDiskFlag, trial_cnt, overwriteFlag, singleF_flag, ExtTriggerFlag, AD_flag, NI_flag, NI_out_flag, LC_flag, TwoK_flag, half_sensor_flag;
int focusMode, TXC_TriggerFlag, MNC_flag, av_file_cnt, acquiring_flag = 0, shutter_wait, stim_delay, SFC_flag;
int spike_pos_ar[FILE_AV_MAX];
int numBNC_chan, BNC_ratio, digital_bin=1;
int pht_frames, pht_DFrame, overscan_x1, overscan_x2;
char cameraType[80], cameraStr[256];
double frame_interval;
char config_list[8][50];
char liveConfig_list[8][80];
char streamConfig_list[8][80];
double sm_lib_rates[8];
double sm_cam_lib_rates[8]; 
double sm_integration_increments[8];
int ccd_lib_bin[8], config_load_lib[8], cds_lib[8], ndr_lib[8], config_ref_list[8], config_ref_list_rev[8];
int stripes_lib[8], layers_lib[8], rotate_lib[8];
int bad_pix_lib[8], offset_chan_order[MAXOFFSETS], image_chan_order[MAXOFFSETS], cds_offset_chan[8];
int start_line_lib[8], super_frame_lib[8], NDR_start_lib[8], NDR_inc_lib[8], NDR_code_lib[8], NDR_max_lib[8];
int segment_lib[8], reserve2_lib[8], reserve3_lib[8]; //reserve1_lib[8] replaced by segment_lib
int num_traces, curDispFrame, numTraceFrames, numExposures, disp_num_col, disp_num_row, disp_num_col_old, disp_num_row_old, file_num_col, file_num_row;	//numExposures is for acquisition temp bin
int NDR_reset_ar[100];
int numDataFrames, firstDataFrame, dataReadBlock, num_seq_pnts;
char FitsHeader[36][80];
short int NP_header[2560];
char FitsEnder[36*80];
int bad_row_ar[10][5], bad_col_ar[10][5], bad_pixel_ar[300][9];
int sm_kernel_size = 1, sav_tmp_bin = 1, sav_sp_bin = 1, bad_pixel_file; // , tif_scale_flag = 0;
int trace_bdr = 5, selectPixMode, thisPixColor, traceYGain, BNC_YGain, histoGain, tracePolarity;
int scale_pix_coor[4] = { 0,0,0,0 };
int pixColorCode[NUM_TRACES_MAX];
int ROI_index[NUM_TRACES_MAX][8];
int traceColorAr[] = {	RED, GREEN, BLUE, ORANGE, PINK, YELLOW, RED2, GREEN2, BLUE2, ORANGE2, PINK2, YELLOW2,
						RED, GREEN, BLUE, ORANGE, PINK, YELLOW, RED2, GREEN2, BLUE2, ORANGE2, PINK2, YELLOW2,
						RED, GREEN, BLUE, ORANGE, PINK, YELLOW, RED2, GREEN2};
char SM_dataname[MAX_PATH], last_dataname[MAX_PATH];
struct tm *fileTime;
int gt_hour_diff;
double SM_exposure, fontSM_w;
int dragboxX0, dragboxY0, auto_skip_v1, auto_skip_v2;
int trace_frame0, num_trace_pnts, trace_exp, trace_exp_max, trace_smooth_mode = 0, trace_smooth_coeff_id = 0;
double trace_lpass = 0.0, trace_hpass = 0.0, trace_lpass_max, trace_hpass_max, left_bstop = 0.0, right_bstop = 0.0, max_zoom, min_zoom;
// Photon Transfer parameters
int num_chan_x, num_chan_y, num_chan_total, num_chan_group = 8, pdv_dev_chan, pdv_dev_unit;
int dark_chan_map[32][32];
int dark_coor_x[MAXOFFSETS], dark_coor_y[MAXOFFSETS], dark_coor_dx[MAXOFFSETS], dark_coor_dy[MAXOFFSETS];
int bright_coor_x[64], bright_coor_y[MAXOFFSETS], bright_coor_dx[MAXOFFSETS], bright_coor_dy[MAXOFFSETS];
int PHT_chan_offset=0;
long repetitions_stream;

int window_layout = 0;
int image_region_dx, image_region_dy, hist_x, hist_region_x, hist_region_y, hist_region_dy, trace_region_y, trace_region_dy;

//*** fro pdv_initcam
int     init_serial(EdtDev * edt_p, Dependent * dd_p, Edtinfo *ei_p);
int     init_serial_basler_binary(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init);
int     init_serial_duncan_binary(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init);
int     init_serial_binary(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init);
int     init_serial_ascii(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init);
char   *strip_crlf(char *str);
int     is_hex_byte_command(char *str);

int
readPref()
{
    FILE *fp;
    char buf[256];

    if (!FOPEN_S(fp,prefFname,"r"))
        return 0;
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&curConfig);
	if (!noCameraFlag) {
		while (strstr(config_list[curConfig], "!") || !strcmp(config_list[curConfig], "")) {
			curConfig++;
			if (curConfig == 8)
				curConfig = 0;
		}
	}
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&curCamGain);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&curChipGain);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&curNFrames);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&trace_win_ysize);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&NDR_num_f);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&NDR_subtraction_frames);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&file_frame_w);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%lf",&frame_interval);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&edge_correct_flag);
	if (!TwoK_flag)
		edge_correct_flag = 0;
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&edge_correct_mode);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&edge_c_col1);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&edge_c_col2);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &BNC_ratio);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &roi_transp_level);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &roi_border_flag);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &roi_label_mode);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &shutter_wait);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &stim_delay);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &window_layout);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &trace_win_xsize);
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &mark_center_flag);
exit:
	fclose(fp);
	return 1;
}

int
writePref()
{
    FILE *fp;

	if (!FOPEN_S(fp,prefFname,"w"))
		return 0;
	fprintf(fp, "%d - curConfig\n", curConfig);	//not read anymore - need to initiate from prg 0
	fprintf(fp, "%d - curCamGain\n", curCamGain);
	fprintf(fp, "%d - curChipGain\n", curChipGain);
	fprintf(fp, "%d - curNFrames\n", curNFrames);
	fprintf(fp, "%d - trace_win_ysize\n", trace_win_ysize);
	fprintf(fp, "%d - NDR_num_f\n", NDR_num_f);
	fprintf(fp, "%d - NDR_subtraction_frames\n", NDR_subtraction_frames);
	fprintf(fp, "%d - file_frame_w\n", file_frame_w);
	fprintf(fp, "%lf - frame_interval\n", frame_interval);
	fprintf(fp, "%d - edge_correct_flag\n", edge_correct_flag);
	fprintf(fp, "%d - edge_correct_mode\n", edge_correct_mode);
	fprintf(fp, "%d - edge_c_col1\n", edge_c_col1);
	fprintf(fp, "%d - edge_c_col2\n", edge_c_col2);
	fprintf(fp, "%d - BNC_ratio\n", BNC_ratio);
	fprintf(fp, "%d - roi_transp_level\n", roi_transp_level);
	fprintf(fp, "%d - roi_border_flag\n", roi_border_flag);
	fprintf(fp, "%d - roi_label_mode\n", roi_label_mode);
	fprintf(fp, "%d - shutter_wait\n", shutter_wait);
	fprintf(fp, "%d - stim_delay\n", stim_delay);
	fprintf(fp, "%d - shutter_wait\n", window_layout);
	fprintf(fp, "%d - trace_win_xsize\n", trace_win_xsize);
	fprintf(fp, "%d - mark_center_flag\n", mark_center_flag);

	fclose(fp);
	return 1;
}

int readCamConfig()
{
	char configFname[MAX_PATH] = ".\\SMSYSDAT\\sm_config.dat";
	char *libFmt = "%lf %lf %lf %lf %lf %lf %lf %lf - %s\n";
	char *libFmt2 = "%d %d %d %d %d %d %d %d - %s\n";
	char *libFmt3 = "%x %x %x %x %x %x %x %x - %s\n";
    FILE *fp;
	char tempStr[256] = "";
    int row=0, i;
    char buf[256];
	char *p;

    if (!FOPEN_S(fp,configFname,"r"))
        return 0;
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%s %s",tempStr, cameraStr);
	if (p = strstr(tempStr, "*"))
		*p = '\0';
	sprintf(cameraType, "%s-VER%d", tempStr, file_version);

	if (strstr(cameraType, "-EDT"))
		pcdFlag = 2;
	else if (strstr(cameraType, "-PCD"))
		pcdFlag = 1;
	else
		pcdFlag = 0;

	if (strstr(cameraType, "-16Bit"))
		Bit16_flag = TRUE;
	else
		Bit16_flag = FALSE;

	if (strstr(cameraType, "-LC"))		// LC = Live Control (for McCormick)
		LC_flag = TRUE;
	else
		LC_flag = FALSE;
	if (strstr(cameraType, "-AD"))
		AD_flag = TRUE;
	else {
		AD_flag = FALSE;
		SFC_flag = FALSE;
		MNC_flag = FALSE;
		NI_out_flag = FALSE;
		if (strstr(cameraType, "-SFC")) {
			SFC_flag = TRUE;
			NI_flag = 1;
		}
		else if (strstr(cameraType, "-MNC")) {
			if (strstr(cameraType, "-MNC2"))
				MNC_flag = 2;			// for 6341 - 2 analog lines
			else
				MNC_flag = 1;			//monochromater with 6343
			NI_flag = 1;
		}
		else {
			if (strstr(cameraType, "-NI")) {
				if (strstr(cameraType, "-NIO")) {
					NI_flag = 0;
					NI_out_flag = 1;
				}
				else if (strstr(cameraType, "-NIU"))
					NI_flag = 1;
				else
					NI_flag = -1;
				if (strstr(cameraType, "-NIU8"))
					numBNC_chan = 8;
			}
			else
				NI_flag = FALSE;
		}
	}
	/*****for NI acquisition (comment out)***/
#ifndef _USE_NI
	if (NI_flag) {
		MessageBox(main_window_handle, "This *.exe was not compiled with NI drivers.", "message", MB_OK);
		return FALSE;
	}
#endif

	for (i = 0; i < 8; i++)
		cds_offset_chan[i] = -1;

	pdv_chan_num = 1;
	TwoK_flag = false;
	if (strstr(cameraType, "DM2K")) {
		TwoK_flag = true;
		QuadFlag = FALSE;
		pdv_chan_num = 4;
	}
	else if (strstr(cameraType, "DM1K")) {
		TwoK_flag = true;
		QuadFlag = FALSE;
		pdv_chan_num = 2;
	}
	else if (strstr(cameraType, "DW128F")) {
		QuadFlag = FALSE;
	}
	else if (strstr(cameraType, "DW")) {
		QuadFlag = FALSE;
		for (i = 3; i < 6; i++)
			cds_offset_chan[i] = i - 3;
	}
	else if (strstr(cameraType, "128X") || strstr(cameraType, "CCD39"))
		QuadFlag = TRUE;
	else 
		QuadFlag = FALSE;

	if (strstr(cameraType, "-HF"))
		half_sensor_flag = 1;
	else
		half_sensor_flag = 0;

    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%s",tempStr);
	while (strstr(tempStr,"*"))
		*(strstr(tempStr,"*")) = ' ';
	while ((strcmp(tempStr,"End Of Config List")) && row < 8) {
		strcpy(config_list[row],tempStr);
		if (!fgets(buf,255,fp))
		    goto exit;
		sscanf(buf,"%s",tempStr);
		while (strstr(tempStr,"*"))
			*(strstr(tempStr,"*")) = ' ';
		++row;
	}

	row = 0;
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%s",tempStr);
	while ((strcmp(tempStr,"End*Of*LiveConfig*List")) && row < 8) {
		strcpy(liveConfig_list[row],tempStr);
		if (!fgets(buf,255,fp))
		    goto exit;
		sscanf(buf,"%s",tempStr);
		++row;
	}
	row = 0;
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%s",tempStr);
	while ((strcmp(tempStr,"End*Of*StreamConfig*List")) && row < 8) {
		strcpy(streamConfig_list[row],tempStr);
		if (!fgets(buf,255,fp))
		    goto exit;
		sscanf(buf,"%s",tempStr);
		++row;
	}

	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&curConfig);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&curCamGain);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&curChipGain);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&fixedCamGain);

    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%s",tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt,&sm_lib_rates[0],&sm_lib_rates[1],&sm_lib_rates[2],&sm_lib_rates[3],
		&sm_lib_rates[4],&sm_lib_rates[5],&sm_lib_rates[6],&sm_lib_rates[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt,&sm_cam_lib_rates[0],&sm_cam_lib_rates[1],&sm_cam_lib_rates[2],&sm_cam_lib_rates[3],
		&sm_cam_lib_rates[4],&sm_cam_lib_rates[5],&sm_cam_lib_rates[6],&sm_cam_lib_rates[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt,&sm_integration_increments[0],&sm_integration_increments[1],&sm_integration_increments[2],&sm_integration_increments[3],
		&sm_integration_increments[4],&sm_integration_increments[5],&sm_integration_increments[6],&sm_integration_increments[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&ccd_lib_bin[0],&ccd_lib_bin[1],&ccd_lib_bin[2],&ccd_lib_bin[3],&ccd_lib_bin[4],&ccd_lib_bin[5],&ccd_lib_bin[6],&ccd_lib_bin[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&config_load_lib[0],&config_load_lib[1],&config_load_lib[2],&config_load_lib[3],&config_load_lib[4],&config_load_lib[5],&config_load_lib[6],&config_load_lib[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&cds_lib[0],&cds_lib[1],&cds_lib[2],&cds_lib[3],&cds_lib[4],&cds_lib[5],&cds_lib[6],&cds_lib[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&ndr_lib[0],&ndr_lib[1],&ndr_lib[2],&ndr_lib[3],&ndr_lib[4],&ndr_lib[5],&ndr_lib[6],&ndr_lib[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&stripes_lib[0],&stripes_lib[1],&stripes_lib[2],&stripes_lib[3],&stripes_lib[4],&stripes_lib[5],&stripes_lib[6],&stripes_lib[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&layers_lib[0],&layers_lib[1],&layers_lib[2],&layers_lib[3],&layers_lib[4],&layers_lib[5],&layers_lib[6],&layers_lib[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&rotate_lib[0],&rotate_lib[1],&rotate_lib[2],&rotate_lib[3],&rotate_lib[4],&rotate_lib[5],&rotate_lib[6],&rotate_lib[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,libFmt2,&bad_pix_lib[0],&bad_pix_lib[1],&bad_pix_lib[2],&bad_pix_lib[3],&bad_pix_lib[4],&bad_pix_lib[5],&bad_pix_lib[6],&bad_pix_lib[7],tempStr);
    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%s",tempStr);
	if (strcmp(tempStr, "End*Of*Lib*List")) {
		sscanf(buf,libFmt2,&start_line_lib[0],&start_line_lib[1],&start_line_lib[2],&start_line_lib[3],&start_line_lib[4],&start_line_lib[5],&start_line_lib[6],&start_line_lib[7],tempStr);
		if (!fgets(buf,255,fp))
			goto exit;
		sscanf(buf,libFmt2,&super_frame_lib[0],&super_frame_lib[1],&super_frame_lib[2],&super_frame_lib[3],&super_frame_lib[4],&super_frame_lib[5],&super_frame_lib[6],&super_frame_lib[7],tempStr);
		if (!fgets(buf,255,fp))
			goto exit;
		sscanf(buf,libFmt3,&NDR_start_lib[0],&NDR_start_lib[1],&NDR_start_lib[2],&NDR_start_lib[3],&NDR_start_lib[4],&NDR_start_lib[5],&NDR_start_lib[6],&NDR_start_lib[7],tempStr);
		if (!fgets(buf,255,fp))
			goto exit;
		sscanf(buf,libFmt3,&NDR_inc_lib[0],&NDR_inc_lib[1],&NDR_inc_lib[2],&NDR_inc_lib[3],&NDR_inc_lib[4],&NDR_inc_lib[5],&NDR_inc_lib[6],&NDR_inc_lib[7],tempStr);
		if (!fgets(buf,255,fp))
			goto exit;
		sscanf(buf,libFmt3,&NDR_code_lib[0],&NDR_code_lib[1],&NDR_code_lib[2],&NDR_code_lib[3],&NDR_code_lib[4],&NDR_code_lib[5],&NDR_code_lib[6],&NDR_code_lib[7],tempStr);
		if (!fgets(buf,255,fp))
			goto exit;
			sscanf(buf,"%s",tempStr);
		if (strcmp(tempStr, "End*Of*Lib*List")) {
			sscanf(buf,libFmt2,&NDR_max_lib[0],&NDR_max_lib[1],&NDR_max_lib[2],&NDR_max_lib[3],&NDR_max_lib[4],&NDR_max_lib[5],&NDR_max_lib[6],&NDR_max_lib[7],tempStr);
			if (!fgets(buf,255,fp))
				goto exit;
			sscanf(buf,libFmt2,&segment_lib[0],&segment_lib[1],&segment_lib[2],&segment_lib[3],&segment_lib[4],&segment_lib[5],&segment_lib[6],&segment_lib[7],tempStr);
			if (!fgets(buf,255,fp))
				goto exit;
			sscanf(buf,libFmt2,&reserve2_lib[0],&reserve2_lib[1],&reserve2_lib[2],&reserve2_lib[3],&reserve2_lib[4],&reserve2_lib[5],&reserve2_lib[6],&reserve2_lib[7],tempStr);
			if (!fgets(buf,255,fp))
				goto exit;
			sscanf(buf,libFmt2,&reserve3_lib[0],&reserve3_lib[1],&reserve3_lib[2],&reserve3_lib[3],&reserve3_lib[4],&reserve3_lib[5],&reserve3_lib[6],&reserve3_lib[7],tempStr);
			if (!fgets(buf,255,fp))
				goto exit;
			sscanf(buf,"%s",tempStr);
		}
	}
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&offset_chan_order[0]);
	if (offset_chan_order[0] >= 0) {
		for (int i = 1; i < layers_lib[0]*stripes_lib[0]*pdv_chan_num; i++) {
			if (!fgets(buf,255,fp))
				goto exit;
			sscanf(buf,"%d",&offset_chan_order[i]);
		}
	}
exit:
	if (!stripes_lib[0]) {
		QuadFlag = FALSE;
	}
	if (TwoK_flag && half_sensor_flag) {
		char tmp_str[256], tmp_str2[256];
		char *p;
		int this_h, this_w;

		for (i = 0; i < sizeof(config_list) / sizeof(config_list[0]); i++) {
			strcpy(tmp_str2, config_list[i]);
			if (p = strstr(tmp_str2, "x")) {
				strcpy(tmp_str, (p + 1));
				*p = '\0';
				if (p = strstr(tmp_str, " "))
					*p = '\0';
				this_h = atoi(tmp_str);
				if (this_h > 1024 / ccd_lib_bin[i])
					sprintf(config_list[i], "!%s", config_list[i]);
				else {
					while (p = strstr(tmp_str2, " "))
						strcpy(tmp_str2, (p + 1));
					this_w = atoi(tmp_str2)/2;
					int this_str_len = (int)strlen(tmp_str2);
					strcpy(tmp_str, config_list[i]);
					if (p = strstr(tmp_str, tmp_str2)) {
						strcpy(tmp_str2, p + this_str_len);
						*p = '\0';
						sprintf(config_list[i], "%s%d%s", tmp_str, this_w, tmp_str2);
					}
				}
			}
		}
		while (strstr(config_list[curConfig], "!") || !strcmp(config_list[curConfig], "")) {
			curConfig++;

			if (curConfig == 8)
				curConfig = 0;
		}
	}

	fclose(fp);

	DWORD sectorsPerCluster;
	DWORD bytesPerSector;
	DWORD numberOfFreeClusters;
	DWORD totalNumberOfClusters;
	int totalDiskBytes;
	char cwd[MAX_PATH];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		if (p = strstr(cwd, ":\\"))
			*(p + 1) = '\0';
		if (GetDiskFreeSpace(cwd, &sectorsPerCluster, &bytesPerSector,
			&numberOfFreeClusters, &totalNumberOfClusters)) {
			totalDiskBytes = sectorsPerCluster * bytesPerSector*(numberOfFreeClusters / 1000000);
			if (strstr(cameraType, "-FASTD") && (totalDiskBytes < 200000))
				MessageBox(main_window_handle, "There is less than 200GB disk space left.", "message", MB_OK);
			else if (totalDiskBytes < 10000)
				MessageBox(main_window_handle, "There is less than 10GB disk space left.", "message", MB_OK);
		}
	}

	return 1;
}

void get_experiment_biopsy(char *str)
{
	char *p, *p_sv;
	int exp_name_len;
	char tmp_str[256];

	strcat(strcat(strcpy(tmp_str, home_dir), dirIMAGES), "\\");
	if (p_sv = strstr(str, tmp_str))
		p_sv += strlen(tmp_str);
	else
		p_sv = str;
	strcpy(experiment, p_sv);
	exp_name_len = 0;
	while (p = strstr(p_sv, "\\")) {
		strncpy(str, p_sv, (int)(p - p_sv));
		strcpy(biopsy, p + 1);
		exp_name_len += (int)(p - p_sv + 1);
		p_sv = p + 1;
	}
	*(experiment + exp_name_len - 1) = '\0';
	while (p = strstr(experiment, " "))
		*p = '_';
	strcpy(str, biopsy);
	if (p = strstr(str, ".tsm"))
		*p = '\0';
	else
		strcat(biopsy, ".tsm");
	trial_cnt = atoi(str + strlen(str) - 3);
}

void getAcq_FileName(int cnt)
{
	char *p;
	char str[MAX_PATH];

	if ((cnt < 0) && strstr(SM_dataname, ".tsm")) {
		strcpy(str, SM_dataname);
		get_experiment_biopsy(str);
	}
	strcpy(str, biopsy);
	if (p = strstr(str, ".tsm"))
		*p = '\0';
	if (cnt < 0)
		trial_cnt = atoi(str + strlen(str) - 3) + 1;
	if (atoi(str + strlen(str) - 3))
		*(p - 3) = '\0';
	sprintf(biopsy, "%s%3d%s", str, trial_cnt, ".tsm");
	while (p = strstr(biopsy, " "))
		*p = '0';
}

void setToolPanel(int refresh)
{
	int i;

	OM[SM_DATAREAD].state =  OM_NULL;
	OM[SM_NEXTREAD].state =  OM_NULL;
	OM[SM_PREVREAD].state =  OM_NULL;
	OM[SM_FIRSTFRM].state =  OM_NULL;
	if (!seqFlag && dataFile && curTab() == OM_MEASUREVIEW) {
		OM[SP_FLTPNL].state = OM_DISPLAY;
		for (i = SP_LPTYPE_UP; i <= SP_HP_DN; i++)
			OM[i].state = OM_VIDEOBUTTON;
		OM[SP_LP_LABEL].state = OM_LABEL;
		OM[SP_HP_LABEL].state = OM_LABEL;

		OM[SM_TOOLPNL].state = OM_DISPLAY;
		OM[SM_TPNLTEXT].state = OM_DISPLAY;
		for (i = SM_PLAY; i <= SM_KSIZEDN; i++)
			OM[i].state = OM_VIDEOBUTTON;
		if (numTraceFrames < numDataFrames) {
			OM[SM_DATAREAD].state =  OM_DISPLAY;
			OM[SM_NEXTREAD].state =  OM_VIDEOBUTTON;
			OM[SM_PREVREAD].state =  OM_VIDEOBUTTON;
			OM[SM_FIRSTFRM].state =  OM_VIDEOBUTTON;
		}
//		OM[SM_THRESHOLD].state = OM_NULL;
	}
	else {
		for (i = SP_FLTPNL; i <= SP_HP_LABEL; i++)
			OM[i].state = OM_NULL;

		OM[SM_TOOLPNL].state = OM_NULL;
		OM[SM_TPNLTEXT].state = OM_NULL;
		for (i = SM_PLAY; i <= SM_KSIZEDN; i++)
			OM[i].state = OM_NULL;
		if (seqFlag) {
			OM[SM_DATAREAD].x = OM[OM_MEASUREWIN].x + 10;
			OM[SM_PREVREAD].x = OM[SM_DATAREAD].x;
			OM[SM_NEXTREAD].x = OM[SM_DATAREAD].x + OM[SM_PREVREAD].dx + 10;
			OM[SM_DATAREAD].y = OM[OM_MEASUREWIN].y + OM[OM_MEASUREWIN].dy - 30;
			OM[SM_PREVREAD].y = OM[SM_DATAREAD].y;
			OM[SM_NEXTREAD].y = OM[SM_DATAREAD].y;
			OM[SM_DATAREAD].state = OM_DISPLAY;
			OM[SM_NEXTREAD].state = OM_VIDEOBUTTON;
			OM[SM_PREVREAD].state = OM_VIDEOBUTTON;
		}
	}
	if (refresh) {
		paintWin(SM_TOOLPNL, PAINTINIT);
		paintWin(SM_DATAREAD, PAINTINIT);
	}
}

void
setBGrey_Live(bool live_b_flag)
{
	if (live_b_flag)
		OM[SM_LIVE].opt->grey = true;
	OM[SM_RESET].opt->grey = true;
	OM[SM_STREAM].opt->grey = true;
	OM[SM_CAPTURE].opt->grey = true;
	OM[SM_DKFRAME].opt->grey = true;
	OM[SM_AUTOOFFSET].opt->grey = true;
	OM[SM_CSETTING].opt->grey = true;
	repaintMenubox();
}

void setBGrey_Stop(int live_b_flag)
{
	if (live_b_flag)
		OM[SM_LIVE].opt->grey = FALSE;
	OM[SM_RESET].opt->grey = FALSE;
	if (viewer_flag)
		OM[SM_STREAM].opt->grey = TRUE;
	else
		OM[SM_STREAM].opt->grey = FALSE;
	OM[SM_CAPTURE].opt->grey = FALSE;
	OM[SM_DKFRAME].opt->grey = FALSE;
	if (offset_chan_order[0] < 0 || cds_lib[curConfig] || TwoK_flag)
		OM[SM_AUTOOFFSET].opt->grey = TRUE;
	else
		OM[SM_AUTOOFFSET].opt->grey = FALSE;
	OM[SM_CSETTING].opt->grey = FALSE;
	repaintMenubox();
}

void setToolBGrey()
{
	if (PlayFlag)
		setBGrey_Live(1);
	else if (!noCameraFlag)
		setBGrey_Stop(1);
	paintWin(SM_TOOLPNL, PAINTINIT);
	paintWin(SM_DATAREAD, PAINTINIT);
}

static int ImgMenuActivated = 0;
void enableImgScl(int activate)
{
	if (activate && dataExist && !seqFlag) {
		if (window_layout < 2) {
			OM[SM_HISTGAIN].state = OM_SLIDER;
			OM[SM_HISTEXP].state = OM_DB_SLIDER;
		}
		else {
			OM[SM_HISTGAIN].state = OM_NULL;
			OM[SM_HISTEXP].state = OM_NULL;
		}
		OM[SM_IMGSCBASE].state = OM_DISPLAY;
		OM[SM_IMGSCALE].state = OM_DB_SLIDER;
		OM[SM_IMGSCLUP].state = OM_ARROW;
		OM[SM_IMGSCLDN].state = OM_ARROW;
		OM[SM_IMGSCRUP].state = OM_ARROW;
		OM[SM_IMGSCRDN].state = OM_ARROW;
	}
	else {
		OM[SM_HISTGAIN].state = OM_NULL;
		OM[SM_HISTEXP].state = OM_NULL;
		OM[SM_IMGSCBASE].state = OM_NULL;
		OM[SM_IMGSCALE].state = OM_NULL;
		OM[SM_IMGSCLUP].state = OM_NULL;
		OM[SM_IMGSCLDN].state = OM_NULL;
		OM[SM_IMGSCRUP].state = OM_NULL;
		OM[SM_IMGSCRDN].state = OM_NULL;
	}
}

void activateImgMenu() {
	if (!ImgMenuActivated) {
		OM[OM_FLDRTARR].opt->grey = FALSE;
		OM[OM_FLDLTARR].opt->grey = FALSE;
		OM[OM_FLDUPARR].opt->grey = FALSE;
		OM[OM_FLDDNARR].opt->grey = FALSE;
		setZoomGray();
		repaintMenubox();
		enableImgScl(1);
		EnableMenuItem(main_menu_handle, IDM_SAVE_IMG_FITS, MF_ENABLED);
		EnableMenuItem(main_menu_handle, IDM_SAVE_IMG_TIFF, MF_ENABLED);
		EnableMenuItem(main_menu_handle, IDM_SAVE_IMG_TXT, MF_ENABLED);
		ImgMenuActivated = 1;
	}
}

void deactivateImgMenu() {
	if (ImgMenuActivated) {
		OM[OM_OBJUPARR].opt->grey = TRUE;
		OM[OM_OBJDNARR].opt->grey = TRUE;
		OM[OM_FLDLTARR].opt->grey = TRUE;
		OM[OM_FLDRTARR].opt->grey = TRUE;
		OM[OM_FLDUPARR].opt->grey = TRUE;
		OM[OM_FLDDNARR].opt->grey = TRUE;
		repaintMenubox();
		enableImgScl(0);
		EnableMenuItem(main_menu_handle, IDM_SAVE_IMG_FITS, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(main_menu_handle, IDM_SAVE_IMG_TIFF, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(main_menu_handle, IDM_SAVE_IMG_TXT, MF_DISABLED | MF_GRAYED);
		ImgMenuActivated = 0;
	}
}

char *strip_space(char *str)
{
	char tmp_str[250];
	char *p;

	while (p = strstr(str, " ")) {
		strcpy(tmp_str, (p+1));
		*p = '\0';
		strcat(str, tmp_str);
	}
	return str;
}

void printAcqSettings(int paintFlag)
{
	int x, y, dx, dy, y0;
	char tempStr[100];
	char gainStr[][10] = {"High", "Mid-Hi", "Mid-Lo", "Low"};
	char wsStr[][10] = {"1 Me-", "12 Me-", "100 Me-"};

	x = OM[OM_MAIN].x+OM[OM_MAIN].dx+25;
	y0 = OM[OM_MAIN].y+OM[SM_CSETTING].y+15;
	y = y0;
	dx = 210;
	dy = 240;
	if (paintFlag == PAINTGRFX)
		D3DRectangle(x, y, x + dx, y + dy, colorWindow);
	else if (paintFlag == PAINTTEXT) {
		strcpy(tempStr, "Configuration:");
		drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
		y = y + 15;
		strcpy(tempStr, config_list[curConfig]);
		if (!noCameraFlag)
			drawText(0, 0, strip_space(tempStr), x + 10, y, colorText, omFontDataSm, TRUE);
		y = y + 20;
		strcpy(tempStr, "Exposure Time:");
		drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
		y = y + 15;
		sprintf(tempStr, "%3.3lfms", frame_interval);
		if (!noCameraFlag)
			drawText(0, 0, tempStr, x + 10, y, colorText, omFontDataSm, TRUE);
		y = y + 20;
		sprintf(tempStr, "# Exposures/Frame:%d", numExposures);
		if (!noCameraFlag)
			drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
		y = y + 20;
		strcpy(tempStr, "# of Frames(Stream):");
		drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
		y = y + 15;
		sprintf(tempStr, "%d(%dms)", curNFrames, (int)(curNFrames*frame_interval*numExposures));
		if (!noCameraFlag)
			drawText(0, 0, tempStr, x + 10, y, colorText, omFontDataSm, TRUE);
		y = y + 20;
		if (curCamGain >= 0 && !fixedCamGain) {
			strcpy(tempStr, "Camera Gain: ");
			strcat(tempStr, gainStr[curCamGain]);
			if (!noCameraFlag)
				drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
			y = y + 15;
		}
		if (curChipGain >= 0) {
			strcpy(tempStr, "Well Size: ");
			strcat(tempStr, wsStr[curChipGain]);
			if (!noCameraFlag)
				drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
			y = y + 15;
		}

		if (ExtTriggerFlag == 2)
			strcpy(tempStr, "Trigger: Key");
		else if (ExtTriggerFlag == 1)
			strcpy(tempStr, "Trigger: External");
		else
			strcpy(tempStr, "Trigger: None");
		if (preTrigger_ms && strcmp(tempStr, "Trigger: None"))
			sprintf(tempStr, "%s (pre %dms)", tempStr, preTrigger_ms);
		drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);

		if (NI_flag) {
			y = y + 15;
			sprintf(tempStr, "BNC Ratio: %d", BNC_ratio);
			drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
			y = y + 15;
			sprintf(tempStr, "Shutter Wait: %d ms", shutter_wait);
			drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
			y = y + 15;
			sprintf(tempStr, "Stimulus Delay: %d ms", stim_delay);
			drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
		}

		y = y0 + dy - 30;
		strcpy(tempStr, "File Name (record to):");
		drawText(0, 0, tempStr, x, y, colorText, omFontDataSm, TRUE);
		y = y + 15;
		sprintf(tempStr, "%s", biopsy);
		if (!noCameraFlag)
			drawText(0, 0, tempStr, x + 10, y, colorText, omFontDataSm, TRUE);
	}
}

static void SM_makeHeader(int images,int rows,int columns,char *camera,double exposure)
{
	char buf[81];
	int card = 0,year = 2013,month = 4, day = 21, hour = 10, minute = 20, second = 40;

	SM_exposure = exposure;

	memset(&FitsHeader,0x20,sizeof(FitsHeader));
	sprintf(buf,"SIMPLE  = %20s","T");
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"BITPIX  = %20d",16);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"NAXIS   = %20d",2);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"NAXIS1  = %20d",columns);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"NAXIS2  = %20d",rows);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"NAXIS3  = %20d",images);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"DATE    = %04d-%02d-%02dT%02d:%02d:%02d",year,month,day,hour,minute,second);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	strcpy(buf,"ORIGIN  = SciMeasure Analytical Systems, Inc.");
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	strcpy(buf,"CREATOR = Turbo-SM");
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"INSTRUME= %s",camera);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"EXPOSURE= %20.10lf",exposure*numExposures);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
 	sprintf(buf,"SATURATE= %20d",16383);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"DATAMAX = %20.0lf",16383.0);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"DATAMIN = %20.0lf",0.0);
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
	sprintf(buf,"END");
	strncpy(FitsHeader[card++],buf,(int)strlen(buf));
}

void SM_pdv_close()
{
	int i;

	for (i = 0; i < pdv_chan_num; i++) {
		if (pdv_pt[i] != NULL)
			pdv_close(pdv_pt[i]);
	}
	if (pcdFlag == 2)
		edt_close(pcd_p);
	else if (pcdFlag == 1) {
		edt_close(pcd_p);
		edt_close(pdv_p);
	}
}

int SM_pdv_open(int close_flag)
{
	int i;
	int pdv_units[MAXCHANNELS] = { 0, 0, 1, 1 };
	int pdv_channels[MAXCHANNELS] = { 0, 1, 0, 1 };

	if (close_flag)
		SM_pdv_close();

	if (pdv_chan_num > 1) {
		for (i = 0; i < pdv_chan_num; i++)
			pdv_pt[i] = pdv_open_channel(EDT_INTERFACE, pdv_units[i], pdv_channels[i]);
	}
	else
		pdv_pt[0] = pdv_open_channel(EDT_INTERFACE, pdv_dev_unit, 0);
	for (i = 0; i < pdv_chan_num; i++) {
		if (pdv_pt[i] == NULL) {
			SM_pdv_close();
			MessageBox(main_window_handle, "Cannot open pdv device.", "message", MB_OK);
			return FALSE;
		}
	}
	pdv_p = pdv_pt[0];
	if (pcdFlag == 1) {
		pcd_p = pdv_p;
		pdv_p = pdv_open_channel(EDT_INTERFACE, 1, 0);
	}
	else if (pcdFlag == 2)
		pcd_p = edt_open_channel("pcd", pdv_dev_unit, 0);
	if ((pcdFlag == 2) && (pcd_p == NULL)) {
		SM_pdv_close();
		MessageBox(main_window_handle, "Cannot open pcd device.", "message", MB_OK);
		return FALSE;
	}	
	return TRUE;
}

void SM_set_timeout(int timeout_v)
{
	int i;
	for (i = 0; i < pdv_chan_num; i++)
		pdv_set_timeout(pdv_pt[i], timeout_v);
}

typedef struct SM_CONFIGINFO {
	char *cfg_str;
	int width[8];
	int height[8];
} SM_CONFIGTYPE, *SM_CONFIG_PTR;

SM_CONFIGTYPE configLUT[] = {
{ "CCID79-PCD",				{ 128, 256, 2304, 1024, 1024, 1024 },					//width
							{ 128, 128, 912, 160, 32, 15, 128, 40 } },				//height
{ "2kx2k_NEURO_BINNED_d_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },		//width
							{ 512, 200, 50, 160, 32, 15, 128, 40 } },				//height
{ "2kx2kCARDIO_s2K_",		{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 1024, 200, 512, 320, 160, 80, 256, 128 } },
{ "2kx2k_NEURO_BINNED_s_",	{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 200, 256, 160, 80, 32, 128, 80 } },
{ "2kx2k_NEURO_BINNED_",	{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },		//Manuela used the same name, need to rename
							{ 512, 50, 160, 80, 80, 40, 30, 20 } },
{ "2kx2k_NEURO_BINNED2_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 200, 100, 256, 160, 80, 128, 112 } },
{ "2kx2k_NEURO_BINNED3_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 100, 50, 160, 80, 32, 80, 31 } },
{ "2kx2k_NEURO_BINNED4_",	{ 2048, 2048, 2048, 1024, 1024, 1024, 1024, 1024 },
							{ 1024, 100, 50, 160, 80, 80, 31, 128 } },
{ "2kx2k_NEURO_v",			{ 2048, 2048, 2048, 2048, 2048, 1024, 1024, 1024 },
							{ 1024, 512, 200, 100, 50, 320, 160, 80 } },
{ "2kx2k_marco_",			{ 2048, 2048, 2048, 2048, 1024, 1024, 1024, 512 },
							{ 512, 256, 50, 19, 32, 15, 7, 22 } },
{ "2kx2k_v4.0.vNDR",		{ 2048, 2048, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 1024, 512, 512, 460, 256, 228, 112, 90 } },
{ "2kx2k_FASTNEURO_",		{ 2048, 1024, 1024, 1024, 512, 512, 512, 2048 },
							{ 512, 160, 15, 7, 23, 10, 4, 9 } },
{ "2kx2k_FASTNEURO2_",		{ 2048, 1024, 1024, 1024, 1024, 512, 512, 512 },
							{ 512, 160, 80, 32, 15, 23, 10, 4 } },
{ "2kx2k_FASTNEURO3_",		{ 2048, 1024, 1024, 1024, 512, 512, 512, 512 },
							{ 512, 160, 80, 32, 47, 23, 10, 4 } },
{ "2k_CARDIO_BINNED_1024_",	{ 2048, 1024, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 512, 256, 160, 80, 128, 80, 64, 39 } },
{ "2k_CARDIO_BINNED_v",		{ 2048, 1024, 1024, 1024, 1024, 1024, 1024, 1024 },
							{ 640, 320, 160, 80, 160, 80, 79, 39 } },
{ "2k_NEURO_s2K_",			{ 2048, 2048, 2048, 2048, 1024, 1024, 1024, 1024 },
							{ 1024, 200, 100, 50, 320, 160, 80, 32 } },
};

int get_pcdWH(int config_id, int height) {
	if (height)
		return configLUT[config_id - 1].height[curConfig];
	else
		return configLUT[config_id - 1].width[curConfig];
}

void SM_pdv_vals()
{
	int i;

	if (pcdFlag != 1) {
		cam_num_col = pdv_get_width(pdv_p);
		cam_num_row = pdv_get_height(pdv_p);
		for (i = 0; i < pdv_chan_num; i++) {
			pdv_flush_fifo(pdv_pt[i]);
			pdv_pt[i]->dd_p->continuous = 1;
		}
		//	bit_depth = pdv_get_depth(pdv_p);
	}
	else {
		int config_id = checkCfgLUT();
		cam_num_col = get_pcdWH(config_id, 0);
		cam_num_row = get_pcdWH(config_id, 1);
		pdv_p->dd_p->continuous = 1;
	}
	config_num_col = cam_num_col;
	config_num_row = cam_num_row;
}

char *SM_pdv_query_command(char *command, char *ret_c, int max_len)
{
	int toRead;

	if (demoFlag || noCameraFlag)
		return NULL;
	pdv_serial_command(pdv_p,command);
	if (strstr(command, "OAC?") && joe_version >= 800)
		toRead = pdv_serial_wait(pdv_p, 767, max_len);
	else
		toRead = pdv_serial_wait(pdv_p, 255, max_len);
    pdv_serial_read(pdv_p,ret_c,toRead);
    return ret_c;
}

void SM_serial_command(char *command)
{
	char ret_c[256];
	int toRead;

	if (demoFlag || noCameraFlag)
		return;
	if (comFlag)
		return;
	pdv_serial_command(pdv_p, command);
	toRead = pdv_serial_wait(pdv_p, 255, 256);
	pdv_serial_read(pdv_p, ret_c, toRead);
}

void setZoomGray()
{
	if (zoom_factor >= max_zoom)
		OM[OM_OBJUPARR].opt->grey = TRUE;
	else
		OM[OM_OBJUPARR].opt->grey = FALSE;
	if (zoom_factor <= min_zoom)
		OM[OM_OBJDNARR].opt->grey = TRUE;
	else
		OM[OM_OBJDNARR].opt->grey = FALSE;
}

void resetZoom(int camFlag)
{
	int dy, num_row, num_col, num_pix;

	if (camFlag) {
		if (demoFlag) {
			num_col = cam_num_col;
			num_row = cam_num_row;
		}
		else if (TwoK_flag) {
			num_col = min(config_num_col * (pdv_chan_num >> 1) / (1 + cds_lib[curConfig]), file_frame_w) / max(1, ccd_lib_bin[curConfig] / 2);
			num_row = config_num_row * 2;
		}
		else {
			num_col = config_num_col / (1 + cds_lib[curConfig]);
			num_row = config_num_row ;
		}
		disp_num_col_old = num_col;
		disp_num_row_old = num_row;

		if (((double)num_row)/image_region_dy > ((double)num_col)/ image_region_dx) {
			num_pix = num_row;
			dy = image_region_dy;
		}
		else {
			num_pix = num_col;
			dy = image_region_dx;
		}
	}
	else {
		disp_num_col_old = disp_num_col;
		disp_num_row_old = disp_num_row;
		if (((double)disp_num_row)/ image_region_dy > ((double)disp_num_col)/ image_region_dx) {
			num_pix = disp_num_row;
			dy = image_region_dy;
		}
		else {
			num_pix = disp_num_col;
			dy = image_region_dx;
		}
	}

	zoom_factor = (dy*0.9) / num_pix;

	setZoomGray();

	dy = image_region_dy;
	img_x0 = 0;
	if (camFlag)
			img_y0 = (int)((dy - num_row *zoom_factor)/2 + 0.5);
	else
		img_y0 = (int)((dy - disp_num_row*zoom_factor)/2 + 0.5);
}

void resetBNCs()
{
	int i;
	for (i = 0; i < numBNC_chan; i++)
		OM[SM_BNC1+i].attr = OM_COLD;
}

void disableBNCs() {
	BNC_exist = FALSE;
	for (int i = SM_BNCPNL; i <= SM_BNCGNDN; i++)
		OM[i].state = OM_NULL;
}

void setBNC_true()
{
	int k, BNC5_state;

	BNC_exist = TRUE;
	OM[SM_BNCPNL].state = OM_DISPLAY;
	OM[SM_BNC_FLTR].state = OM_RADIOBUTTON;
	OM[SM_BNCGAIN].state = OM_SLIDER;
	OM[SM_BNCGNUP].state = OM_ARROW;
	OM[SM_BNCGNDN].state = OM_ARROW;
	if (numBNC_chan <= 4)
		BNC5_state = OM_NULL;
	else
		BNC5_state = OM_RADIOBUTTON;
	for (k = 0; k < 4; k++) {
		OM[SM_BNC1 + k].state = OM_RADIOBUTTON;
		OM[SM_BNC5 + k].state = BNC5_state;
	}
	if (!window_layout) {
		OM[SM_BNCPNL].dy = numBNC_chan * 25 + 200;
		OM[SM_BNC_FLTR].y = OM[SM_BNCPNL].y + OM[SM_BNCPNL].dy - 30;
	}
	else {
		OM[SM_BNCPNL].dx = numBNC_chan * 25 + 200;
		OM[SM_BNC_FLTR].x = OM[SM_BNCPNL].x + OM[SM_BNCPNL].dx - 40;
	}
	if (BNC_filter_flag)
		OM[SM_BNC_FLTR].attr = OM_HOT;
	else
		OM[SM_BNC_FLTR].attr = OM_COLD;
}

void setStream_outputs()
{
	if (AD_flag && !demoFlag) {
		char sysdat_dir[MAX_PATH];
		HANDLE handle;
		WIN32_FIND_DATA find_data;
		char command[256];
		int hi_b, lo_b; //, output_delay=100;
		int frames_add;
		int frame_inhibit_flag;

		strcat(strcpy(sysdat_dir, home_dir), "\\SMSYSDAT\\frame_inhibit_flag.txt");
		if ((handle = FindFirstFile(sysdat_dir, &find_data)) != INVALID_HANDLE_VALUE) {
			FindClose(handle);
			frame_inhibit_flag = TRUE;
		}
		else
			frame_inhibit_flag = FALSE;

		if (strstr(cameraType, "CCD67"))
			frames_add = min(80, max((int)(400*sm_lib_rates[curConfig]), 2));
		else 
			frames_add = (int)(200*sm_lib_rates[curConfig]);
//			frames_add = (int)(80*sm_lib_rates[curConfig]);
		if (win64_flag)
			frames_add = (int)(frames_add*1.2);

		if (num_out_f != curNFrames+frames_add) {
			num_out_f = curNFrames + frames_add;

			if (frame_inhibit_flag && (ExtTriggerFlag != 1)) {
				hi_b = (int)(frames_add/256);
				lo_b = (int)(frames_add % 256);
				sprintf(command,"@ICW $CC; 6; $24; 1; 0; 0; %d; %d", hi_b, lo_b);
				SM_serial_command(command);

				sprintf(command,"@ICW $CC; 6; $24; 1; 1; 7; 0; 2");
				SM_serial_command(command);

				hi_b = (int)((curNFrames)/256);
				lo_b = (int)((curNFrames) % 256);
				sprintf(command,"@ICW $CC; 6; $24; 1; 2; 3; %d; %d", hi_b, lo_b);
				SM_serial_command(command);
			}
			else {
				sprintf(command,"@ICW $CC; 6; $24; 1; 0; 0; 0; 1");
				SM_serial_command(command);

				sprintf(command,"@ICW $CC; 6; $24; 1; 1; 7; 0; 2");
				SM_serial_command(command);

				hi_b = (int)(num_out_f/256);
				lo_b = (int)(num_out_f % 256);
				sprintf(command,"@ICW $CC; 6; $24; 1; 2; 3; %d; %d", hi_b, lo_b);
				SM_serial_command(command);
			}
			sprintf(command,"@ICW $CC; 6; $24; 1; 3; 1; 0; 2");
			SM_serial_command(command);
		}
	}
}

void resetScaleB()
{
	for (int i = 0; i < 4; i++)
		scale_pix_coor[i] = 0;
}

void setFocusRep(int focusFlag)
{
	long repetitions;
	char command[256];

	if (ndr_lib[curConfig] == 1)
		repetitions = 0;
	else if (focusFlag && cds_lib[curConfig]) {
		repetitions = (long)((focus_frame_interval-1/sm_cam_lib_rates[curConfig])*1000/sm_integration_increments[curConfig] + 0.5);
		if (repetitions < 0)
			repetitions = 0;
		if (repetitions > 65534)
			repetitions = 65534;
	}
	else 
		repetitions = repetitions_stream;
	sprintf(command,"@REP %ld",repetitions);
	SM_serial_command(command);
}

char AD_rate_command[256];
static int config_loaded = 0;

int checkCfgLUT()
{
	int i;

	for (i = 0; i < sizeof(configLUT) / sizeof(configLUT[0]); i++) {
		if (strstr(cameraStr, configLUT[i].cfg_str))
			return i+1;
	}
	return 0;
}

long set_repetition()
{
	double percent_int = 1.0;
	long repetitions;

	if (NI_flag && BNC_ratio > 1)
		percent_int = 0.99;
	repetitions = (long)((frame_interval*percent_int - 1 / sm_cam_lib_rates[curConfig]) * 1000 / sm_integration_increments[curConfig] + 0.5);
	if (repetitions < 0)
		repetitions = 0;
	if (repetitions > 65534) {
		repetitions = 65534;
		frame_interval = ((double)repetitions) * sm_integration_increments[curConfig] / 1000 + 1 / sm_cam_lib_rates[curConfig];
	}
	repetitions_stream = repetitions;
	return repetitions;
}

int SM_initCamera(int load_config_flag, int close_flag, int load_pro_flag)
{
	char command[256];
	int prog;
	char *p;
	char home_drive[256];
	int config_id = 0;

	if (close_flag > 0)
		SM_pdv_close();

	char buf[256], tmp_str[256];
	int m, i;
	FILE *fp;
	char flagFname[MAX_PATH];
	//Read Image Chan_order
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\image_chan_order.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		for (int i = 0; i < layers_lib[0] * stripes_lib[0] * pdv_chan_num; i++) {
			if (!fgets(buf, 255, fp))
				goto exit;
			sscanf(buf, "%d", &image_chan_order[i]);
		}
	}

	strcpy(home_drive, home_dir);
	if (p = strstr(home_drive, ":"))
		*p = '\0';

	//read sub format for 2K
	seqROI_flag = 0;
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\draw_seqROIs.txt");
    if (FOPEN_S(fp,flagFname,"r")) {
		memset(seqROI_ar, 0, sizeof(seqROI_ar));
		if (!fgets(buf,255,fp))
			goto exit;
		for (i = 0; i < 8; i++) {
			if (!fgets(buf,255,fp))
				goto exit;
			if (!strcmp(buf, ""))
				goto exit;
			strcpy(tmp_str, buf);
			m = 0;
			while (p = strstr(tmp_str, ",")) {
				strcpy(buf, p+1);
				*p = '\0';
				sscanf(tmp_str,"%d",&seqROI_ar[m][i]);
				strcpy(tmp_str, buf);
				m++;
			}
			seqROI_flag = m;
		}
exit:
		fclose(fp);
	}

	//read Disk Data flag
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\DiskData_flag.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		writeToDisk_flag = true;
		fclose(fp);
	}
	else
		writeToDisk_flag = false;

	if (load_config_flag && !demoFlag) {
		char edt_cfg_name[_MAX_PATH];
		sprintf(edt_cfg_name, "%s:\\EDT\\pdv\\camera_config\\%s", home_drive, liveConfig_list[curConfig]);
		if (TwoK_flag) {
			config_id = checkCfgLUT();
			if (!config_loaded || !config_id) {
				if (FOPEN_S(fp, edt_cfg_name, "r"))
					fclose(fp);
				else {
					sprintf(command, "%s does not exist.", edt_cfg_name);
					MessageBox(main_window_handle, command, "message", MB_OK);
					return false;
				}
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv0_0 -f %s", home_drive, edt_cfg_name);
				system(command);
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv0_1 -f %s", home_drive, edt_cfg_name);
				system(command);
				if (pdv_chan_num > 2) {
					sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv1_0 -f %s", home_drive, edt_cfg_name);
					system(command);
					sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv1_1 -f %s", home_drive, edt_cfg_name);
					system(command);
				}
				config_loaded = 1;
			}
		}
		else {
			if (FOPEN_S(fp, edt_cfg_name, "r"))
				fclose(fp);
			else {
				sprintf(command, "%s does not exist.", edt_cfg_name);
				MessageBox(main_window_handle, command, "message", MB_OK);
				return false;
			}
			if (pcdFlag == 1)
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv%d -f %s", home_drive, 1, edt_cfg_name);
			else
				sprintf(command, "%s:\\EDT\\pdv\\initcam -u pdv%d -f %s", home_drive, pdv_dev_unit, edt_cfg_name);
			system(command);
		}
	}

	if (demoFlag) {
		if (darkImage != NULL)
			_aligned_free(darkImage);
		darkImage = (signed short int *)_aligned_malloc(cam_num_col*cam_num_row*sizeof(signed short int) , 2);
		resetZoom(1);
		return TRUE;	//	no cam
	}

//	MessageBox(main_window_handle, "Debug #3", "message", MB_OK);

	if (close_flag >= 0) {
		if (!SM_pdv_open(0))
			return FALSE;
	}

/*	if (pcdFlag == 1) {
		Edtinfo edtinfo, *ei_p;

		pdv_dep_set_default(pcd_p->dd_p);
		ei_p = &edtinfo;
		ei_p->startdma = NOT_SET;
		ei_p->enddma = NOT_SET;
		ei_p->flushdma = NOT_SET;
		ei_p->serial_init = NULL;
		ei_p->serial_init_delay = NOT_SET;
		ei_p->cl_mgtspeed = NOT_SET;
		pcd_p->dd_p->startdma = NOT_SET;
		pcd_p->dd_p->enddma = NOT_SET;
		pcd_p->dd_p->flushdma = NOT_SET;
		init_serial(pdv_p, pcd_p->dd_p, ei_p);
		//*************
		pdv_set_baud(pcd_p, 38400);
	//	edt_flush_fifo(pdv_p);         // Flush the input fifo
	//	edt_bar1_write(pdv_p, 0x19000a0, 0x600); // Sets data generator to 32bit count
		edt_bar1_write(pcd_p, 0x1900000, 0x8); //CMD_EN
		edt_bar1_write(pcd_p, 0x1900010, 0x1); //CHAN_EN
	}*/

	if (load_pro_flag) {
		// determin if needed to stream to disk
		setStream_outputs();

		long repetitions = 0L;

		if (curCamGain >= 0)
			prog = 8*curCamGain + curConfig;
		else
			prog = curConfig;
		sprintf(command,"@RCL %d",prog);
		SM_serial_command(command);

//		MessageBox(main_window_handle, "Debug #4", "message", MB_OK);

		sprintf(command,"@SEQ 0");
		SM_serial_command(command);

		if (AD_flag && !demoFlag) {
			// Set EventMode
			sprintf(command,"@ICW $CC; 3; $21; 0; 2");
			SM_serial_command(command);
			//Close shutter
			sprintf(command,"@ICW $CC; 3; $25; 1; 1");
			SM_serial_command(command);
			//Set ADAC rate
			if (strstr(cameraType, "DW")) {
				long AD_rate = (long)(50000.0*frame_interval/(8.2*BNC_ratio));
				sprintf(command,"@ICW $CC; 4; $14; $%02lx; $%02lx; $%02lx", (AD_rate&0xff0000L)>>16, (AD_rate&0xff00L)>>8, (AD_rate&0xffL));
			}
			else if (strstr(cameraType, "CCD67")) {
				long AD_rate = (long)(50000.0/8.2);
				if (win64_flag)
					BNC_ratio = min((int)frame_interval, cam_num_col/4);
				else
					BNC_ratio = 1;
				sprintf(command,"@ICW $CC; 4; $14; $%02lx; $%02lx; $%02lx", (AD_rate&0xff0000L)>>16, (AD_rate&0xff00L)>>8, (AD_rate&0xffL));
			}
			else {
				if (curConfig == 4)
					sprintf(command,"@ICW $CC; 4; $14; 0; 2; 97");
				else if (curConfig == 5)
					sprintf(command,"@ICW $CC; 4; $14; 0; 7; 216");
				else if (curConfig == 6)
					sprintf(command,"@ICW $CC; 4; $14; 0; 4; 198");
				else if (curConfig == 0 || curConfig == 7)
					sprintf(command,"@ICW $CC; 4; $14; 0; 11; 214");
				else {
					long AD_rate = (long)(50000.0*frame_interval/(8.0*BNC_ratio));
					sprintf(command,"@ICW $CC; 4; $14; $%02lx; $%02lx; $%02lx", (AD_rate&0xff0000L)>>16, (AD_rate&0xff00L)>>8, (AD_rate&0xffL));
				}
			}
			strcpy(AD_rate_command, command);
			SM_serial_command(_strupr(command));
			//set BNC 1-4 to bipolar
			sprintf(command,"@ICW $CC; 9; $13; 16; 16; 16; 16; 0; 0; 0; 0");
			SM_serial_command(command);
		}

		if (strstr(cameraType, "DW")) {
			SM_SetHourCursor();
			if (chipgainSave != curChipGain) {
				switch(curChipGain) {
					case 0: 
						sprintf(command,"@ICW $48; 9; 0;1;0; 0;1;0; 0;1;0");
						break;
					case 1: 
						sprintf(command,"@ICW $48; 9; 0;1;0; 0;1;0; 2;3;2");
						break;
					case 2: 
						sprintf(command,"@ICW $48; 9; 2;3;2; 2;3;2; 2;3;2");
						break;
				}
				for (int i = 1; i < 132; i++)
					SM_serial_command(command);
			}
			sprintf(command,"@AAM %d", curCamGain);
			SM_serial_command(command);
			SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
		}
		else if (strstr(cameraType, "CCID83")) {
			if (start_line_lib[curConfig])
				noise_seq_flag = TRUE;
			else
				noise_seq_flag = FALSE;
		}
		else if (TwoK_flag) {
			if (config_loaded && config_id) {
				for (int i = 0; i < pdv_chan_num; ++i)
					pdv_setsize(pdv_pt[i], configLUT[config_id - 1].width[curConfig], configLUT[config_id - 1].height[curConfig]);
			}

			int hbin = 0;
			if (ccd_lib_bin[curConfig] > 1)
				hbin = 1;
			int start_line;			
			if (start_line_lib[curConfig])
				start_line = start_line_lib[curConfig];
			else
				start_line = 65 + (1024-cam_num_row*ccd_lib_bin[curConfig]);

			sprintf(command,"@SPI 0; 2");
			SM_serial_command(command);
			Sleep(20);
			sprintf(command,"@SPI 0; 0");
			SM_serial_command(command);
			Sleep(20);
			sprintf(command,"@SPI 0; 4");
			SM_serial_command(command);
			Sleep(20);
			sprintf(command,"@PSR %d; %d", ccd_lib_bin[curConfig], start_line);
			SM_serial_command(command);
			Sleep(20);
			sprintf(command,"@SPI 0; %d", hbin);
			SM_serial_command(command);
			Sleep(20);

//			MessageBox(main_window_handle, "Debug #5", "message", MB_OK);

			if (pdv_chan_num > 2)
				sprintf(command, "%s:\\EDT\\pdv\\setgap2k", home_drive);
			else
				sprintf(command, "%s:\\EDT\\pdv\\setgap1k", home_drive);
			system(command);

//			MessageBox(main_window_handle, "Debug #6", "message", MB_OK);
		}
/*		else if (strstr(cameraType, "128BI")) {
			//WRITE VPIXSEL
			sprintf(command, "@ICW $58; 2; 1; 78");
			SM_serial_command(command);
			//WRITE VSHCLR
			sprintf(command, "@ICW $58; 2; 3; 239");
			SM_serial_command(command);
			//READ VPIXSEL; RANDOM; VSHCLR
			sprintf(command, "@ICW $58; 2; 0; 0");
			SM_serial_command(command);
			sprintf(command, "@ICR $58; 3");
			SM_serial_command(command);
			//SAVE VPIXSEL
			sprintf(command, "@ICW $58; 1; 91");
			SM_serial_command(command);
			//SAVE VSHCLR
			sprintf(command, "@ICW $58; 1; 93");
			SM_serial_command(command);
			//BIAS CURRENTS - 8 - BIT VALUES
			//IPIX - 2 IDENTICAL VALUES
			sprintf(command, "@ICW $56; 2; $10; 0");
			SM_serial_command(command);
			sprintf(command, "@ICW $56; 2; $12; 0");
			SM_serial_command(command);
			//ICOLBUS
			sprintf(command, "@ICW $56; 2; $11; 0");
			SM_serial_command(command);
			//IOBUF
			sprintf(command, "@ICW $56; 2; $13; 0");
			SM_serial_command(command);
			//READ IPIX1
			sprintf(command, "@ICW $56; 2; $30; 3");
			SM_serial_command(command);
			sprintf(command, "@ICR $56; 1");
			SM_serial_command(command);
			//READ IPIX2
			sprintf(command, "@ICW $56; 2; $32; 3");
			SM_serial_command(command);
			sprintf(command, "@ICR $56; 1");
			SM_serial_command(command);
			//READ ICOL
			sprintf(command, "@ICW $56; 2; $31; 3");
			SM_serial_command(command);
			sprintf(command, "@ICR $56; 1");
			SM_serial_command(command);
			//READ IOBUF
			sprintf(command, "@ICW $56; 2; $33; 3");
			SM_serial_command(command);
			sprintf(command, "@ICR $56; 1");
			SM_serial_command(command);
			//SYSTEM CLOCKS
			sprintf(command, "@LAS 50; 90; 82");
			SM_serial_command(command);
		}*/
		else if (strstr(cameraType, "128X")) {
			if (strstr(cameraType, "128X-V2")) {
				int psr_ar[] = { 1, 49, 57, 57, 1, 49, 57, 57 };
				if (start_line_lib[curConfig])
					sprintf(command, "@PSR 1; %d", start_line_lib[curConfig]);
				else
					sprintf(command, "@PSR 1; %d", psr_ar[curConfig]);
				Sleep(50);
				SM_serial_command(command);
				Sleep(20);
				sprintf(command, "@BIN %d", ccd_lib_bin[curConfig] - 1);
				SM_serial_command(command);
			}
			if (ndr_lib[curConfig] > 1)
				sprintf(command, "%s:\\EDT\\pdv\\setgap_c3", home_drive);
			else
				sprintf(command, "%s:\\EDT\\pdv\\setgap_80", home_drive);
			system(command);
		}

		if (ndr_lib[curConfig] == 1)
			repetitions = 0;
		else if (ndr_lib[curConfig] > 1) {
			if (curConfig == 7)
				repetitions = max(0, ndr_lib[curConfig] - 1);
			else
				repetitions = max(0, ndr_lib[curConfig] - 2);
		}
		else if (!ndr_lib[curConfig])
			repetitions = set_repetition();

		if (TwoK_flag && cds_lib[curConfig]) {
			setFocusRep(focusMode);
			OM[SM_FOCUSBOX].opt->grey = FALSE;
			OM[SM_FOCUS].opt->grey = FALSE;
		}
		else {
			if (pcdFlag) {		// This is to get rid of funny characters
				char ret_c[256];
				sprintf(command, "@REP?");
				SM_pdv_query_command(command, ret_c, 256);
			}

			sprintf(command,"@REP %ld",repetitions);
			SM_serial_command(command);

			OM[SM_FOCUSBOX].opt->grey = TRUE;
			OM[SM_FOCUS].opt->grey = TRUE;
		}
		if (!TwoK_flag && !strstr(cameraType, "128X"))
			loadCamOffsets();

//		MessageBox(main_window_handle, "Debug #7", "message", MB_OK);

		Sleep(50);
		sprintf(command,"@TXC 0");
		SM_serial_command(command);
		sprintf(command,"@SEQ 1");
		SM_serial_command(command);
		SM_pdv_vals();

		if (offset_chan_order[0] < 0 || cds_lib[curConfig] || TwoK_flag)
			OM[SM_AUTOOFFSET].opt->grey = TRUE;
		else 
			OM[SM_AUTOOFFSET].opt->grey = FALSE;

		cam_num_col = config_num_col;
		cam_num_row = config_num_row;
		if (TwoK_flag) {
		//	if (pdv_chan_num > 2) {
			//	if (!file_frame_w || auto_scale_w > 2 * cam_num_col / ((1 + cds_lib[curConfig])*max(1, ccd_lib_bin[curConfig] / 2)))
			if (!auto_scale_w_acq)
				auto_scale_w_acq = (pdv_chan_num/2) * cam_num_col / ((1 + cds_lib[curConfig])*max(1, ccd_lib_bin[curConfig] / 2));
		//	}
			auto_scale_w = auto_scale_w_acq;

			if (!file_frame_w)
				file_frame_w = (pdv_chan_num >> 1 )*cam_num_col/((1+cds_lib[curConfig])*max(1, ccd_lib_bin[curConfig]/2));
			else {
				if (file_frame_w < 10)
					file_frame_w = 10;
				else if (file_frame_w >(pdv_chan_num >> 1)*cam_num_col/(1+cds_lib[curConfig]))
					file_frame_w = (pdv_chan_num >> 1)*cam_num_col/(1+cds_lib[curConfig]);
			}
			if (file_frame_w < cam_num_row*(pdv_chan_num >> 1))
				cam_num_col = cam_num_row * (pdv_chan_num >> 1);
		}
		if (cam_num_col_last != cam_num_col || cam_num_row_last != cam_num_row) {
			clearTracePix();
			cam_num_row_last = cam_num_row;
			cam_num_col_last = cam_num_col;
			if (darkImage != NULL)
				_aligned_free(darkImage);
			darkImage = (signed short int *)_aligned_malloc(cam_num_col*cam_num_row*pdv_chan_num * sizeof(signed short int) / (1 + cds_lib[curConfig]), 2);
		}

		if (pcdFlag)
			live_factor = 2;
		else if (TwoK_flag) {
			live_factor = max(1, 1024 / config_num_row);
			if (ndr_lib[curConfig])
				live_factor *= 2;
		}
		else if (ndr_lib[curConfig] > 1)
			live_factor = ndr_lib[curConfig];
		else
			live_factor = (int)max(1, (1 + cds_lib[curConfig])/frame_interval);

		// for CCID79 multiple pixel read
		if (strstr(cameraType, "CCID")) {
			multi_pix_read = super_frame_lib[curConfig];
			if (multi_pix_read > 0)
				multi_pix_av = 1;
			else {
				multi_pix_read = -multi_pix_read;
				multi_pix_av = 0;
			}
		}
		else {
			multi_pix_read = 0;
			multi_pix_av = 0;
		}

//		MessageBox(main_window_handle, "Debug #8", "message", MB_OK);

		single_img_size = (unsigned long)cam_num_col*cam_num_row*(1 + cds_lib[curConfig])*(1 + ndr_lib[curConfig])*pdv_chan_num*live_factor;
		if (single_img_data != NULL)
			_aligned_free(single_img_data);
		single_img_data = (signed short int *)_aligned_malloc(single_img_size * sizeof(signed short int) * 2, 2);
		if (single_img_float != NULL)
			_aligned_free(single_img_float);
		single_img_float = (float *)_aligned_malloc(single_img_size * sizeof(float), 2);
		if (!initLive(pdv_chan_num, cds_lib[curConfig], 1, layers_lib[curConfig], stripes_lib[curConfig], cameraType, QuadFlag, rotate_lib[curConfig], bad_pix_lib[curConfig]))
			return FALSE;
		if (newDarkFlag) {
			char ref_name[MAX_PATH];
			DeleteFile(strcat(strcpy(ref_name, tmp_file_dir), "\\record_ref.tsm"));
			dataExist = FALSE;
			darkExist = FALSE;
			d_darkExist = FALSE;
			deactivateImgMenu();
			darkSubFlag = FALSE;
			if (OM[SM_DKSUBBOX].attr && !d_darkExist)
				setChckBox(SM_DKSUBBOX);
		}

		disableBNCs();
		dataFile = FALSE;
		if (selectPixMode == DRAG_ERASE)
			selectPixMode = DRAG_MODE;
		resetScaleB();
		setToolPanel(1);
		repaintMenubox();
		resetZoom(1);
		if (photonTFlag && (strstr(cameraType, "CCD") || strstr(cameraType, "CCID")))
			readPHT_config2();
	}

//	MessageBox(main_window_handle, "Debug #9", "message", MB_OK);

	GetSystemTime(&lastSystemTime);
	return TRUE;
}

void drawCross(int x, int y, int width, int color)
{
	int x0, y0, dx, dy;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	dx = image_region_dx;
	dy = image_region_dy;

	x = x + x0;
	y = y + y0;

	if (x-width>x0 && x+width<x0+dx && y-width>y0 && y+width<y0+dy) {
		Draw_Line/*6432*/(x-width,y,x+width,y,color);
		Draw_Line/*6432*/(x,y+width,x,y-width,color);
	}
}

int RainbowRGB(double ratio)
{
	double pi = 3.1415926535;

	return min(255, max(0, (int)(255.0 * cos(2.0 * pi * ratio) + 127.5)));
}

/*
0.0  -> (0, 0, 128)    (dark blue)
0.25 -> (0, 255, 0)    (green)
0.5  -> (255, 255, 0)  (yellow)
0.75 -> (255, 128, 0)  (orange)
1.0  -> (255, 0, 0)    (red)
*/
int colorTB2(int i, int RGB_index)
{
	int ctb_dark_blue[]	= { 0, 0, 128 };
	int ctb_green[]		= { 0, 255, 128 };
	int ctb_yellow[] = { 255, 255, 0 };
	int ctb_orange[] = { 255, 128, 0 };
	int ctb_red[] = { 168, 0, 0 };
	double retvel;

	if (i < 64)
		retvel = ctb_dark_blue[RGB_index] * (64-i) / 64.0 + ctb_green[RGB_index] * i / 64.0;
	else if (i < 128)
		retvel = ctb_green[RGB_index] * (128 - i) / 64.0 + ctb_yellow[RGB_index] * (i - 64) / 64.0;
	else if (i < 192)
		retvel = ctb_yellow[RGB_index] * (192 - i) / 64.0 + ctb_orange[RGB_index] * (i - 128) / 64.0;
	else
		retvel = ctb_orange[RGB_index] * (256 - i) / 64.0 + ctb_red[RGB_index] * (i - 192) / 64.0;

	return min(255, max(0, int(retvel + 0.5)));
}

void makeColorLUTs(void)
{
	double ratio;
	int i;
	int num_colors = sizeof(rainbow_LUT)/sizeof(rainbow_LUT[0]);
	if (!colorTB_id) {
		for (i = num_colors - 1; i >= 0; i--) {
			ratio = (num_colors - i)*2.0 / (3 * num_colors);
			rainbow_LUT[i] = (RainbowRGB(ratio) << 16) | (RainbowRGB(ratio + 2.0 / 3) << 8) | RainbowRGB(ratio + 1.0 / 3);
		}
	}
	else {
		for (i = num_colors - 1; i >= 0; i--)
			rainbow_LUT[i] = (colorTB2(i, 0) << 16) | (colorTB2(i, 1) << 8) | colorTB2(i, 2);
	}

	for (i = num_colors - 1; i >= 0; i--)
		regular_LUT[i] = (i << 16) | (i << 8) | i;
}

void paintBox(int x, int y, int dx, int dy, int col)
{
	HLine(x-1,x+dx-1,y-1,col);
	HLine(x-1,x+dx,y+dy,col);
	VLine(y,y+dy-1,x-1,col);
	VLine(y-1,y+dy,x+dx,col);
}

static char PHTcameraType[80], frType[256];
int readPHT_config()
{
    FILE *fp;
	char tmpStr[256];
    char buf[256];
	char PHTconfigFname[MAX_PATH];
	int i, j;
	char *p;

	strcat(strcpy(PHTconfigFname, home_dir), "\\SMSYSDAT\\photon_transfer_config.txt");
    if (!FOPEN_S(fp,PHTconfigFname,"r"))
		return FALSE;

    if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%s",PHTcameraType);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&num_chan_x);
	if (!fgets(buf,255,fp))
        goto exit;
	sscanf(buf,"%d",&num_chan_y);
	num_chan_total = num_chan_x*num_chan_y;
	for (j = 0; j < num_chan_y; j++) {
		if (!fgets(buf,255,fp))
			goto exit;
		strcpy(tmpStr, buf);
		for (i = 0; i < num_chan_x; i++) {
			if (p = strstr(tmpStr, " ")) {
				strcpy(buf, (p+1));
				*p = '\0';
				dark_chan_map[j][i] = atoi(tmpStr);
				strcpy(tmpStr, buf);
			}
		}
	}
	if (!fgets(buf, 255, fp))
		goto exit;
	sscanf(buf, "%d", &num_chan_group);
exit:
	fclose(fp);
	return TRUE;
}

int readPHT_config2()
{
    FILE *fp;
	char tmpStr[256], fr_char[50];
    char buf[256];
	char PHTconfigFname[MAX_PATH];
	int i, j, x1, x2, y1, y2, dk_x1, dk_x2, this_col, this_row, noise_col, noise_row, noise_t_chan;
	double noise_f_r;
	char *p;
	char *phtArFmt = "%s %d %d %d %lf %d %d %d %d %d %d %d %d %d %s\n";

	strcpy(tmpStr, config_list[curConfig]);
	if (p = strstr(tmpStr, " fps")) {
		strcpy(buf, (p + 7));
		*p = '\0';
	}
	strcpy(fr_char, tmpStr);
	if (p = strstr(fr_char, ",")) {
		strcpy(tmpStr, (p+1));
		*p = '\0';
		strcat(fr_char, tmpStr);
	}
	sscanf(buf,"%dx%d",&this_col, &this_row);

	if (AD_flag && !demoFlag) {
		if (strstr(cameraType, "DW"))
			cam_num_row = cam_num_row - 1;
		else 
			cam_num_row = cam_num_row - 2;
	}

	strcat(strcpy(PHTconfigFname, home_dir), "\\SMSYSDAT\\sm_ph_tr_config.txt");
    if (!FOPEN_S(fp,PHTconfigFname,"r"))
		return FALSE;
    while (fgets(buf,255,fp)) {
		if (p = strstr(buf, PHTcameraType)) {
			strcpy(tmpStr, (p+(int)strlen(PHTcameraType)));
			strcpy(frType, tmpStr);
			if (p = strstr(frType, _itoa(num_chan_total,buf,10)))
				*p = '\0';
			while (p = strstr(frType, " ")) {
				strcpy(buf, (p+1));
				*p = '\0';
				strcat(frType, buf);
			}
			sscanf(tmpStr,phtArFmt,tmpStr, &noise_t_chan, &noise_col, &noise_row, &noise_f_r, &x1, &x2, &y1, &y2, &dk_x1, &dk_x2, &pht_frames, &overscan_x1, &overscan_x2, buf);
			if ((noise_col == this_col) && (noise_row == this_row) && ((int)noise_f_r == atoi(fr_char))) {
				int m = 0;
				int div_fct = 1;
				if (multi_pix_read > 1)
					div_fct = multi_pix_read;
				for (j = 0; j < num_chan_y; j++) {
					for (i = 0; i < num_chan_x; i++) {
						bright_coor_dx[m] = x2 - x1;
						bright_coor_dy[m] = y2 - y1;
						dark_coor_dx[m] = dk_x2 - dk_x1;
						dark_coor_dy[m] = y2 - y1;
						if (strstr(frType, "SR")) {
							if (j == 0) {
								bright_coor_y[m] = y1;
								dark_coor_y[m] = y1;
							}
							else {
								bright_coor_y[m] = cam_num_row - y2;
								dark_coor_y[m] =  cam_num_row - y2;
							}
							if ((i % 2) == 0) {
								bright_coor_x[m] = x1 + i*cam_num_col/num_chan_x;
								dark_coor_x[m] = dk_x1 + i*cam_num_col/num_chan_x;
							}
							else {
								bright_coor_x[m] = (i+1)*cam_num_col/num_chan_x - x2;
								dark_coor_x[m] = (i+1)*cam_num_col/num_chan_x - dk_x2;
							}
						}
						else {
							bright_coor_x[m] = x1 + (m % num_chan_x)*cam_num_col/(num_chan_x*div_fct);
							bright_coor_y[m] = y1 + (m/num_chan_x)*cam_num_row/num_chan_y;
							dark_coor_x[m] = dk_x1 + (m % num_chan_x)*cam_num_col/(num_chan_x*div_fct);
							dark_coor_y[m] = y1 + (m/num_chan_x)*cam_num_row/num_chan_y;
						}
						m++;
					}
				}
				break;
			}
		}
	}

	fclose(fp);
	return TRUE;
}

static int pht_border = 4;
static double mean_max_sv[64], var_max_sv[64];
void drawPHT_boxes()
{
	int color, x, y, x0, y0, dx, dy, i;
	
	x0 = getMeasureWinX0() + img_x0;
	y0 = getMeasureWinY0() + img_y0;
	for (i = max(0, PHT_chan_offset); i < min(PHT_chan_offset+ num_chan_group, num_chan_total); i++) {
		color = BLUE;
		x = (int)(x0 + dark_coor_x[i] * zoom_factor + 0.5);
		y = (int)(y0 + dark_coor_y[i] * zoom_factor + 0.5);
		dx = (int)(dark_coor_dx[i] * zoom_factor + 0.5);
		dy = (int)(dark_coor_dy[i] * zoom_factor + 0.5);
		paintBox(x, y, dx, dy, color);
		color = traceColorAr[(i % 8)];
		x = (int)(x0 + bright_coor_x[i]*zoom_factor + 0.5);
		y = (int)(y0 + bright_coor_y[i]*zoom_factor + 0.5);
		dx = (int)(bright_coor_dx[i]*zoom_factor + 0.5);
		dy = (int)(bright_coor_dy[i]*zoom_factor + 0.5);
		paintBox(x, y, dx, dy, color);
	}
}

double responsivity_ar[10], e_noise_ar[10], ADU_noise_ar[10];
void drawPHT_curves()
{
	int i, m, n, k, cnt, x, y, x0, y0, color, dx, dy, dk_cnt, x1, x2, y1, y2, chan_x0, chan_y0, fit_dx, fit_dy, chan_cnt;
	double sum = 0.0, sum_sqr = 0.0, mean_max = 1.0, mean_min = 0.0, var_max = 1.0, var_min = 0.0;
	double mean_av, var_av, mean_by_var_av, mean_sqr_av, regression_beta, regression_alpha, dark_std_av, responsivity_av, e_noise_av, ADU_noise_av;
	signed short *sptr, *dk_ptr;
	char str[256];
	double *pht_mean, *pht_var;

	pht_mean = (double *)_aligned_malloc(disp_num_col*disp_num_row*sizeof(double), 2);
	pht_var = (double *)_aligned_malloc(disp_num_col*disp_num_row*sizeof(double), 2);

	x1 = getMeasureWinX0();
	y1 = getMeasureWinY0();
	x0 = x1 + img_x0;
	y0 = y1 + img_y0;
	x2 = x1 + getMeasureWinX();
	y2 = y1 + getMeasureWinY();
	color = YELLOW;
	chan_cnt = 0;
	chan_x0 = (int)(x1 + disp_num_col*zoom_factor + 10 + 0.5);
	dx = hist_x - chan_x0 - 10;
	dy = (getMeasureWinY() - 165)/min(num_chan_group, num_chan_x*num_chan_y);
	responsivity_av = 0.0;
	e_noise_av = 0.0;
	ADU_noise_av = 0.0;
	if (!darkExist)
		darkSubFlag = FALSE;

	int this_val;
	for (i = max(0, PHT_chan_offset); i < min(PHT_chan_offset+ num_chan_group, num_chan_total); i++) {
//	for (j = 0; j < num_chan_y; j++) {
//		for (i = 0; i < num_chan_x; i++) {
			cnt = 0;
			// Dark data
//			for (n = dark_coor_y[i+j*num_chan_x]; n < dark_coor_y[i+j*num_chan_x]+dark_coor_dy[i+j*num_chan_x]; n++) {
//				for (m = dark_coor_x[i+j*num_chan_x]; m < dark_coor_x[i+j*num_chan_x]+dark_coor_dx[i+j*num_chan_x]; m++) {
			for (n = dark_coor_y[i]; n < dark_coor_y[i]+dark_coor_dy[i]; n++) {
				for (m = dark_coor_x[i]; m < dark_coor_x[i]+dark_coor_dx[i]; m++) {
					sum = 0.0;
					sum_sqr = 0.0;
					sptr = (signed short *)image_data + m + n*disp_num_col;
					dk_ptr = (signed short int *)darkImage + m + n * disp_num_col;
					for (k = 0; k < pht_frames; k++) {
						this_val = *sptr;
						if (darkSubFlag)
							this_val -= *dk_ptr;
						sum += this_val;
						sum_sqr += this_val * this_val;
						sptr += disp_num_col*disp_num_row;
					}
					*pht_mean = sum/pht_frames;
					*pht_var = (sum_sqr - sum*sum/pht_frames)/(pht_frames-1);
					if (cnt == 0) {
						mean_max = *pht_mean;
						mean_min = *pht_mean;
						var_max = *pht_var;
						var_min = *pht_var;
					} 
					else {
						if (mean_max < *pht_mean)
							mean_max = *pht_mean;
						if (mean_min > *pht_mean)
							mean_min = *pht_mean;
						if (var_max < *pht_var)
							var_max = *pht_var;
						if (var_min > *pht_var)
							var_min = *pht_var;
					}
					cnt++;
					pht_mean++;
					pht_var++;
					if (cnt >= disp_num_col*disp_num_row) {
						dk_cnt = cnt;
						goto pht_exit;
					}
				}
			}
			dk_cnt = cnt;
			// Bright data
//			for (n = bright_coor_y[i+j*num_chan_x]; n < bright_coor_y[i+j*num_chan_x]+bright_coor_dy[i+j*num_chan_x]; n++) {
//				for (m = bright_coor_x[i+j*num_chan_x]; m < bright_coor_x[i+j*num_chan_x]+bright_coor_dx[i+j*num_chan_x]; m++) {
			for (n = bright_coor_y[i]; n < bright_coor_y[i]+bright_coor_dy[i]; n++) {
				for (m = bright_coor_x[i]; m < bright_coor_x[i]+bright_coor_dx[i]; m++) {
					sum = 0.0;
					sum_sqr = 0.0;
					sptr = (signed short *)image_data + m + n * disp_num_col;
					dk_ptr = (signed short int *)darkImage + m + n * disp_num_col;
					for (k = 0; k < pht_frames; k++) {
						this_val = *sptr;
						if (darkSubFlag)
							this_val -= *dk_ptr;
						sum += this_val;
						sum_sqr += this_val * this_val;
						sptr += disp_num_col * disp_num_row;
					}
					*pht_mean = sum/pht_frames;
					*pht_var = (sum_sqr - sum*sum/pht_frames)/(pht_frames-1);
					if (mean_max < *pht_mean)
						mean_max = *pht_mean;
					if (mean_min > *pht_mean)
						mean_min = *pht_mean;
					if (var_max < *pht_var)
						var_max = *pht_var;
					if (var_min > *pht_var)
						var_min = *pht_var;
					cnt++;
					pht_mean++;
					pht_var++;
					if (cnt >= disp_num_col*disp_num_row)
						goto pht_exit;
				}
			}
pht_exit:
			mean_max = mean_max*1.2;
			var_max = var_max*1.2;

			dark_std_av = 0.0;
			mean_by_var_av = 0.0;
			mean_av = 0,0;
			var_av = 0.0;
			mean_sqr_av = 0.0;

			chan_y0 = y1 + chan_cnt*dy + 10;
			if (dx <= 0) {
				free(pht_mean);
				free(pht_var);
				return;
			}
			pht_mean -= cnt;
			pht_var -= cnt;

			if (pht_auto_scale || mean_max_sv[i] <= 0.0) {
				mean_max_sv[i] = mean_max;
				var_max_sv[i] = var_max;
			}
			else {
				mean_max = mean_max_sv[i];
				var_max = var_max_sv[i];
			}
			if (mean_max > 0 && var_max > 0) {
				for (m = 0; m < cnt; m++) {
					x = (int)(chan_x0 + dx*(*pht_mean)/mean_max);
					y = (int)(chan_y0 + dy - dy*(*pht_var)/var_max);
					if (m < dk_cnt) {
						color = YELLOW;
						dark_std_av += sqrt(*pht_var);
					}
					else //{
						color = traceColorAr[(i) % num_chan_group];
					mean_by_var_av += (*pht_mean)*(*pht_var);
					mean_av += *pht_mean;
					var_av += *pht_var;
					mean_sqr_av += (*pht_mean)*(*pht_mean);
	//				}
					if (x > x1 && x < x2 && y > y1 && y < y2) {
						Draw_Line/*6432*/(x-1,y,x+1,y,color);
						Draw_Line/*6432*/(x,y-1,x,y+1,color);
					}
					pht_mean++;
					pht_var++;
				}
				pht_mean -= cnt;
				pht_var -= cnt;
				dark_std_av = dark_std_av/dk_cnt;
				mean_by_var_av = mean_by_var_av/cnt;
				mean_av = mean_av/cnt;
				var_av = var_av/cnt;
				mean_sqr_av = mean_sqr_av/cnt;
				if (mean_sqr_av != mean_av*mean_av) {
					regression_beta = (mean_by_var_av - mean_av*var_av)/(mean_sqr_av - mean_av*mean_av);
					regression_alpha = var_av - regression_beta*mean_av;

					x = max((int)(chan_x0 + dx*mean_min/mean_max), x1);
					y = max(0, min((int)(chan_y0 + dy - dy*(regression_alpha+regression_beta*mean_min)/var_max), y2));
					fit_dx = max(0, min((int)(dx*(mean_max - mean_min)/mean_max), x2-x));
					fit_dy = max(0, min((int)(dy*regression_beta*(mean_max-mean_min)/var_max), y-y1));
					if (y-fit_dy < chan_y0) {
						fit_dy = y - chan_y0;
						fit_dx = (int)(dx*fit_dy*var_max/(dy*regression_beta*mean_max));
					}
//		printf("%d, %d, %d, %d\n", x,y,x+fit_dx,y-fit_dy);
					Draw_Line/*6432*/(x,y,x+fit_dx,y-fit_dy,color);

					x = chan_x0 + 10;
					y = y1 + getMeasureWinY() - 140 + chan_cnt*12 + 17;

					int txt_color = ((color & 0xff)<<16) | (color & 0xff00) | ((color & 0xff0000)>>16);
					double responsivity = 1/regression_beta;
					double e_noise = dark_std_av/regression_beta;
					double ADU_noise = dark_std_av;						
					sprintf(str, "%5.2lf   %5.2lf   %5.2lf",responsivity,e_noise,ADU_noise);
					drawText(0, 0, str, x, y, txt_color, omFontRprt, TRUE);
					responsivity_ar[i - PHT_chan_offset] = responsivity;
					e_noise_ar[i - PHT_chan_offset] = e_noise;
					ADU_noise_ar[i - PHT_chan_offset] = ADU_noise;

					responsivity_av += responsivity;
					e_noise_av += e_noise;
					ADU_noise_av += ADU_noise;
				}
			}
			chan_cnt++;
//		}
	}

	color = YELLOW;
	int txt_color = ((color & 0xff)<<16) | (color & 0xff00) | ((color & 0xff0000)>>16);
	int pht_x_spacing = max(64, 64*(int)(mean_max/(32*64)));
	if (dx*pht_x_spacing / mean_max < 30)
		pht_x_spacing = (int)(30 * mean_max / dx);
	y = y1 + dy*chan_cnt + 10;
	x = pht_x_spacing;
	int tic_height, this_x, lbl_cnt=0;
	while (x < mean_max) {
		this_x = (int)(chan_x0 + dx*x/mean_max);
		if (!(x % (pht_x_spacing*4))) {
			tic_height = 7;
			lbl_cnt++;
			sprintf(str, "%d",x);
			drawText(0, 0, str, this_x-15, y+3, txt_color, omFontRprtTn, TRUE);
		}
		else if (!(x % (pht_x_spacing*2)))
			tic_height = 5;
		else 
			tic_height = 3;
		Draw_Line/*6432*/(this_x,y,this_x,y-tic_height,color);
		x += pht_x_spacing;
	}

	x = chan_x0 + 10;
	y = y1 + getMeasureWinY() - 140;
	sprintf(str, " RESP     e-N   ADU-N");
	drawText(0, 0, str, x, y, WHITE, omFontRprt, TRUE);

	responsivity_av = responsivity_av/chan_cnt;
	e_noise_av = e_noise_av/chan_cnt;
	ADU_noise_av = ADU_noise_av/chan_cnt;
	y += 12*chan_cnt + 21;
	sprintf(str, "%5.2lf   %5.2lf   %5.2lf",responsivity_av,e_noise_av,ADU_noise_av);
	drawText(0, 0, str, x, y, WHITE, omFontRprt, TRUE);
	responsivity_ar[min(num_chan_group, num_chan_total - PHT_chan_offset)] = responsivity_av;
	e_noise_ar[min(num_chan_group, num_chan_total - PHT_chan_offset)] = e_noise_av;
	ADU_noise_ar[min(num_chan_group, num_chan_total - PHT_chan_offset)] = ADU_noise_av;

	x = chan_x0;
	y = y1 + 10;
	paintBox(x, y, dx, dy*chan_cnt, color);

	int max_PHT_sample = OM[SM_TRACEWIN].dx - OM[SM_TRCGAIN].dx - trace_bdr * 2;
	num_chan_group = min(num_chan_x*num_chan_y, 8);
//	int fake_sample;
	for (i = 0; i < num_chan_group; i++) {
//		fake_sample = num_PHT_sample % 20;
//		PHT_noise_ar[i][num_PHT_sample] = (double)fake_sample; // e_noise_ar[i];
		PHT_noise_ar[i][num_PHT_sample] = e_noise_ar[i];
		if (num_PHT_sample == 0)
			min_PHT_noise[i] = e_noise_ar[i];
		else {
			if (abs((PHT_noise_ar[i][num_PHT_sample] - PHT_noise_ar[i][num_PHT_sample-1]) / PHT_noise_ar[i][num_PHT_sample-1]) > 0.3) {
				PHT_noise_ar[i][num_PHT_sample] = PHT_noise_ar[i][num_PHT_sample - 1];
			}
		}
	}
	num_PHT_sample++;
	if (num_PHT_sample >= max_PHT_sample) {
		for (int j = 0; j < max_PHT_sample-1; j++)
			for (i = 0; i < num_chan_group; i++)
				PHT_noise_ar[i][j] = PHT_noise_ar[i][j+1];
		num_PHT_sample--;
	}

	pht_auto_scale = 0;
	_aligned_free(pht_mean);
	_aligned_free(pht_var);
}

void savePHT_vales()
{
	char pht_v_Fname[] = "C:\\Turbo-SM\\SMDATA\\pht_values.txt";
	FILE *fp;
	int i, this_num_chan_grp, this_val=-1;
	char str[1024], command[256], ret_c[256];
	char *p;
	SYSTEMTIME SystemTime;

	this_num_chan_grp = min(num_chan_group, num_chan_total - PHT_chan_offset);
	if (!FOPEN_S(fp, pht_v_Fname, "r")) {
		if (FOPEN_S(fp, pht_v_Fname, "w")) {
			strcpy(str, "Date, Time");
			strcat(str, ", Prog");
			strcat(str, ", Filter");
			strcat(str, ", Gain");
			strcat(str, ", Clamp");
			strcat(str, ", Sample");
			for (i = 0; i < this_num_chan_grp; i++)
				sprintf(str, "%s, Responsivity%d", str, PHT_chan_offset + i + 1);
			strcat(str, ", Responsivity-Av");
			for (i = 0; i < this_num_chan_grp; i++)
				sprintf(str, "%s, Noise%d(e-)", str, PHT_chan_offset + i + 1);
			strcat(str, ", Noise-Av(e-)");
			for (i = 0; i < this_num_chan_grp; i++)
				sprintf(str, "%s, Noise%d(ADU)", str, PHT_chan_offset + i + 1);
			strcat(str, ", Noise-Av(ADU)");
			fprintf(fp, "%s\n", str);
		}
	}
	fclose(fp);

	if (FOPEN_S(fp, pht_v_Fname, "a")) {
		GetSystemTime(&SystemTime);
		sprintf(str, "%d-%d-%d, %d:%d:%d", SystemTime.wMonth, SystemTime.wDay, SystemTime.wYear, SystemTime.wHour + gt_hour_diff, SystemTime.wMinute, SystemTime.wSecond);

		sprintf(command, "@PRG?");
		SM_pdv_query_command(command, ret_c, 256);
		if (p = strstr(ret_c, "!"))
			this_val = atoi(p + 1);
		sprintf(str, "%s, %d", str, this_val);

		sprintf(command, "@FAM?");
		SM_pdv_query_command(command, ret_c, 256);
		if (p = strstr(ret_c, ":"))
			this_val = atoi(p + 1);
		sprintf(str, "%s, %d", str, this_val);

		sprintf(command, "@AAM?");
		SM_pdv_query_command(command, ret_c, 256);
		if (p = strstr(ret_c, ":"))
			this_val = atoi(p + 1);
		sprintf(str, "%s, %d", str, this_val);

		sprintf(command, "@DCA?");
		SM_pdv_query_command(command, ret_c, 256);
		if (p = strstr(ret_c, ":"))
			this_val = atoi(p + 1);
		sprintf(str, "%s, %d", str, this_val);

		sprintf(command, "@DSA?");
		SM_pdv_query_command(command, ret_c, 256);
		if (p = strstr(ret_c, ":"))
			this_val = atoi(p + 1);
		sprintf(str, "%s, %d", str, this_val);

		for (i = 0; i <= this_num_chan_grp; i++)
			sprintf(str, "%s, %lf", str, responsivity_ar[i]);
		for (i = 0; i <= this_num_chan_grp; i++)
			sprintf(str, "%s, %lf", str, e_noise_ar[i]);
		for (i = 0; i <= this_num_chan_grp; i++)
			sprintf(str, "%s, %lf", str, ADU_noise_ar[i]);

		fprintf(fp, "%s\n", str);
		fclose(fp);
	}
}

void paintBNClabels(int win)
{
	char str[2];
	int i, color;

	if (win == SM_BNCPNL) {
		if (!window_layout) {
			drawText(0, 0, "Gain", OM[SM_BNC1].x - 5, OM[SM_BNCPNL].y - 10 , colorLightShadow, omFontRprtTn, TRUE);
			drawText(0, 0, "BNC", OM[SM_BNC1].x - 3, OM[SM_BNC1].y - 20, colorLightShadow, omFontRprtTn, TRUE);
		}
		else {
			drawText(0, 0, "Gain", OM[SM_BNCPNL].x + 20, OM[SM_BNC1].y - 15, colorLightShadow, omFontRprtTn, TRUE);
			drawText(0, 0, "BNC", OM[SM_BNC1].x - 33, OM[SM_BNC1].y - 15, colorLightShadow, omFontRprtTn, TRUE);
		}
		for (i = 0; i < numBNC_chan; i++) {
			if (SelectedMap[disp_num_row][i] <= 0)
				color = colorLightShadow;
			else {
				color = traceColorAr[SelectedMap[disp_num_row][i] - 1];
				color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
			}
			sprintf(str, "%d", i+1);
			if (!window_layout)
				drawText(0, 0, str, OM[SM_BNC1].x + 20, OM[SM_BNC1].y + 20 * i, color, omFontRprtTn, TRUE);
			else
				drawText(0, 0, str, OM[SM_BNC1].x + 20 * i + 4, OM[SM_BNC1].y - 15, color, omFontRprtTn, TRUE);
		}
		drawText(0, 0, "Fltr", OM[SM_BNC_FLTR].x - 5, OM[SM_BNC_FLTR].y - 15, colorLightShadow, omFontRprtTn, TRUE);
	}
}

static unsigned int histoAr_full[32765], histoAr_full_scl[32765], selectedHistoAr_full[NUM_TRACES_MAX][32765];
static unsigned int histoAr[256], selectedHistoAr[NUM_TRACES_MAX][256];
static float histMax[NUM_TRACES_MAX + 1], histMin[NUM_TRACES_MAX + 1];
static int meanCnt[NUM_TRACES_MAX + 1];
static int hist_tic_spacing = 256, num_histo = 128;
static double histMeans[NUM_TRACES_MAX+1], histSTD[NUM_TRACES_MAX+1];
static unsigned int max_histo_cnt;
static double FFT_ar[5000];
static double FFT_max, FFT_min;
static int FFT_N, FFT_N_disp = 0;

void
putHistoClipboard(char *p)
{
	int i;
	char *q, str[128];

	if (dataExist) {
		for (i = 0; i < num_traces + 1; ++i) {
			sprintf(str, "%lf", histMeans[i]);
			for (q = str; *q; )
				if (*q == ' ') {
					while (*q == ' ')
						q++;
					*p++ = '\t';
				}
				else
					*p++ = *q++;
			*p++ = '\r';
			*p++ = '\n';
		}
	}
	*p = '\0';
}

void paintHisto(int paintFlag)
{
	static int hist_cnt, hist_dx;
	static int histo_div;
	static double mean_max, mean_min, var_av, fit_r, regression_beta;

	int color, x, y, i, j, m, k;
	int hist_y, hFrame_dy, hist_dy;
	char hist_label[50];

	hist_dy = hist_region_dy;
	hFrame_dy = hist_dy; // min(dy0 * 3 / 5, OM[SP_FLTPNL].y - OM[SM_HISTEXP].y - OM[SM_HISTEXP].dy - 40);
	hist_y = hist_region_y; // OM[SM_HISTEXP].y + hFrame_dy + 20;
	static int stt_short = 0, tic_size = 0, tic_spacing = 0, num_tic = 0;

	if (filePHT_flag) {
		int fit_dx, fit_dy;
		double sum, sum_sqr, var_max = 0.0, var_min = 0.0;
		double mean_av, mean_by_var_av, mean_sqr_av, regression_alpha;
		signed short *sptr, *dk_pt;
		double *pht_mean, *pht_var;
		int x1, y1, x2, y2;
		int dx = 350;
		int dy = hist_dy;
		int this_val;

		char var_name[] = "c:\\Turbo-SM\\SMSYSDAT\\high_var_pixels.txt";
		char meanvar_name[] = "c:\\Turbo-SM\\SMDATA\\mean_var.txt";
		FILE *fp;
		int this_pix;
		char buf[256];
		int skip_high_var = 0;

		if (paintFlag == PAINTGRFX) {
//			if (!FOPEN_S(fp, var_name, "w"))
			if (FOPEN_S(fp, var_name, "r"))
				skip_high_var = 1;
			else
				skip_high_var = 0;
			if (skip_high_var)
				if (fgets(buf, 255, fp))
					sscanf(buf, "%d", &this_pix);

			hist_cnt = 0;
			x1 = hist_x;
			x2 = x1 + dx;
			y2 = hist_y;
			y1 = y2 - dy;
			pht_mean = (double *)_aligned_malloc(256 * 256 * sizeof(double), 2);
			pht_var = (double *)_aligned_malloc(256 * 256 * sizeof(double), 2);
			for (j = ROI_index[0][3]; j < ROI_index[0][4]; j++) {
				for (i = ROI_index[0][1]; i < ROI_index[0][2]; i++) {
					if (SelectedMap[j][i] == pixColorCode[0]) {
						sum = 0.0;
						sum_sqr = 0.0;
						sptr = (signed short *)image_data;
						sptr += i + j * disp_num_col;
						dk_pt = (signed short *)dark_data;
						dk_pt += i + j * disp_num_col;
						for (k = 0; k < numTraceFrames; k++) {
							this_val = *sptr - *dk_pt;
							sum += this_val;
							sum_sqr += this_val * this_val;
							sptr += disp_num_col * disp_num_row;
						}
						*pht_mean = sum / numTraceFrames;
						*pht_var = (sum_sqr - sum * sum / numTraceFrames) / (numTraceFrames - 1);
						if (!hist_cnt) {
							mean_max = *pht_mean;
							mean_min = *pht_mean;
							var_max = *pht_var;
							var_min = *pht_var;
						}
						else {
							if (mean_max < *pht_mean)
								mean_max = *pht_mean;
							if (mean_min > *pht_mean)
								mean_min = *pht_mean;
							if (var_max < *pht_var)
								var_max = *pht_var;
							if (var_min > *pht_var)
								var_min = *pht_var;
						}

						if (skip_high_var && (j*disp_num_col + i + 1 >= this_pix)) {
							if (fgets(buf, 255, fp))
								sscanf(buf, "%d", &this_pix);
							else {
								skip_high_var = 0;
								fclose(fp);

								hist_cnt++;
								pht_mean++;
								pht_var++;
							}
						}
						else {
					//	if (*pht_var < 70) {
					/*		char buf[256];
							sprintf(buf, "**** %d: %lf %lf\n", hist_cnt, *pht_mean, *pht_var);
							OutputDebugString(buf);*/

							hist_cnt++;
							pht_mean++;
							pht_var++;
						}
					//	else {
						//	fprintf(fp, "%d\n", j*disp_num_col + i + 1);	//not read anymore - need to initiate from prg 0
					//	}
						if (hist_cnt >= 256 * 256)
							goto pht_exit;
					}
				}
			}
		pht_exit:
			if (skip_high_var)
				fclose(fp);

			mean_by_var_av = 0.0;
			mean_av = 0, 0;
			var_av = 0.0;
			mean_sqr_av = 0.0;
			pht_mean -= hist_cnt;
			pht_var -= hist_cnt;
			if (!whiteBackgrnd)
				color = YELLOW;
			else
				color = DKGRAY;
			FOPEN_S(fp, meanvar_name, "w");
			if (mean_max > 0 && var_max > 0) {
				for (m = 0; m < hist_cnt; m++) {
					x = (int)(hist_x + dx * (*pht_mean - mean_min) / (mean_max - mean_min));
					y = (int)(hist_y - dy * (*pht_var) / var_max);
					mean_by_var_av += (*pht_mean)*(*pht_var);
					mean_av += *pht_mean;
					var_av += *pht_var;
					mean_sqr_av += (*pht_mean)*(*pht_mean);
					if (x > x1 && x < x2 && y > y1 && y < y2) {
						Draw_Line(x - 1, y, x + 1, y, color);
						Draw_Line(x, y - 1, x, y + 1, color);
					}
					fprintf(fp, "%lf, %lf\n", *pht_mean, *pht_var);

					pht_mean++;
					pht_var++;
				}
				fclose(fp);

				pht_mean -= hist_cnt;
				pht_var -= hist_cnt;
				mean_by_var_av = mean_by_var_av / hist_cnt;
				mean_av = mean_av / hist_cnt;
				var_av = var_av / hist_cnt;
				mean_sqr_av = mean_sqr_av / hist_cnt;
				if (mean_sqr_av != mean_av * mean_av) {
					regression_beta = (mean_by_var_av - mean_av * var_av) / (mean_sqr_av - mean_av * mean_av);
					regression_alpha = var_av - regression_beta * mean_av;

					x = hist_x;// + dx*mean_min/mean_max);
					y = (int)(hist_y - dy * (regression_alpha + regression_beta * mean_min) / var_max);
					fit_dx = dx; //(mean_max - mean_min)/mean_max);
					fit_dy = (int)(dy*regression_beta*(mean_max - mean_min) / var_max);
					fit_r = dx * var_max / (dy*regression_beta*(mean_max - mean_min));
					if (y > hist_y) {
						x += (int)((y - hist_y)*fit_r);
						y = hist_y;
					}
					if (x + fit_dx > hist_x + dx) {
						fit_dx = hist_x + dx - x;
						fit_dy = (int)(fit_dx / fit_r);
					}
					Draw_Line(x, y, x + fit_dx, y - fit_dy, RED);

					x = hist_x + 10;
					y = hist_x + getMeasureWinY() - 140;

					hist_dx = dx;
					hist_dy = hFrame_dy;
					if (!whiteBackgrnd)
						color = WHITE;
					else
						color = BLACK;
					VLine(hist_y, hist_y - hist_dy, hist_x, color);
					VLine(hist_y, hist_y - hist_dy, hist_x + hist_dx, color);
					HLine(hist_x, hist_x + hist_dx, hist_y, color);
					HLine(hist_x, hist_x + hist_dx, hist_y - hist_dy, color);
				}
			}
			_aligned_free(pht_mean);
			_aligned_free(pht_var);
		}
		else if (paintFlag == PAINTTEXT) {
			if (!whiteBackgrnd)
				color = WHITE;
			else
				color = BLACK;
			sprintf(hist_label, "%0.1lf", mean_min);
			drawText(0, 0, hist_label, hist_x, hist_y + 5, color, omFontDataSm, TRUE);
			sprintf(hist_label, "%0.1lf", mean_max);
			drawText(0, 0, hist_label, hist_x + hist_dx - 20, hist_y + 5, color, omFontDataSm, TRUE);
			sprintf(hist_label, "Responsivity = %0.4lf", 1.0/ regression_beta);
			int this_y = hist_y - hist_dy;
			drawText(0, 0, hist_label, hist_x + 20, this_y, TEXT_RED, omFontDataSm, TRUE);
			sprintf(hist_label, "Mean Var = %0.1lf", var_av);
			drawText(0, 0, hist_label, hist_x + 20, this_y + 15, TEXT_RED, omFontDataSm, TRUE);
			sprintf(hist_label, "Count = %d", hist_cnt);
			drawText(0, 0, hist_label, hist_x + 20, this_y + 30, TEXT_RED, omFontDataSm, TRUE);
		}
	}
	else if (showHistFlag) {
		if (paintFlag == PAINTGRFX) {
			memset(histoAr, 0, sizeof(histoAr));
			memset(selectedHistoAr, 0, sizeof(selectedHistoAr));
			if (!whiteBackgrnd)
				color = WHITE;
			else
				color = BLACK;
			max_histo_cnt = 0;
			if (!lockHistFlag) {
				histo_max = 1024;
				histo_min = 0;
				while (histo_max < max_pix)
					histo_max = histo_max * 2;
				if (min_pix < 0) {
					histo_min = -hist_tic_spacing;
					while (histo_min > min_pix)
						histo_min = histo_min * 2;
				}
			}
			histoLeft = min(histo_min, histoLeft);
			scaleLeft = histo_min;
			scaleRight = histo_max;
			histo_div = (histo_max - histo_min) / num_histo;

			for (i = 0; i < num_histo; i++) {
				for (k = 0; k < histo_div; k++)
					histoAr[i] += histoAr_full[16000 + i*histo_div + k + histo_min];
				for (m = 0; m < num_traces; m++) {
					for (k = 0; k < histo_div; k++)
						selectedHistoAr[m][i] += selectedHistoAr_full[m][16000 + i*histo_div + k + histo_min];
				}
				if (max_histo_cnt < histoAr[i])
					max_histo_cnt = histoAr[i];
			}
			if (max_histo_cnt > 0) {
				double hist_f = 0.9;
				for (i = 0; i < num_histo; i++) {
					VLine(max(hist_y - hFrame_dy, hist_y - (int)(histoAr[i] * histoGain*hist_dy*hist_f / max_histo_cnt)), hist_y, hist_x + i * 3, color);
					for (m = 0; m < num_traces; m++) {
						if (selectedHistoAr[m][i])
							VLine(max(hist_y - hFrame_dy, hist_y - (int)(selectedHistoAr[m][i] * histoGain*hist_dy*hist_f / max_histo_cnt)), hist_y, hist_x + i * 3 + 1, traceColorAr[pixColorCode[m] - 1]);
					}
				}
			}
			hist_dx = 3 * num_histo;
			hist_dy = hFrame_dy;
			if (!whiteBackgrnd)
				color = YELLOW;
			else
				color = DKGRAY;
			VLine(hist_y, hist_y - hist_dy, hist_x, color);
			VLine(hist_y, hist_y - hist_dy, hist_x + hist_dx, color);
			HLine(hist_x, hist_x + hist_dx, hist_y, color);
			HLine(hist_x, hist_x + hist_dx, hist_y - hist_dy, color);

			// draw scale-max and min yellow verticle dashline on histogram
			int x1, x2;
			if (histo_max > histo_min) {
				x1 = (int)(hist_x + hist_dx * (scale_min - histo_min) / (histo_max - histo_min) + 0.5) - 1;
				x2 = (int)(hist_x + hist_dx * (scale_max - histo_min) / (histo_max - histo_min) + 0.5) + 1;
				if ((x1 >= hist_x) && (x2 >= hist_x) && (x1 <= hist_x+ hist_dx) && (x2 <= hist_x+ hist_dx)) {
					for (m = 0; m <= hist_dy / 5; m++) {
						VLine(hist_y - m * 4, hist_y - m * 4 - 1, x1, color);
						VLine(hist_y - m * 4, hist_y - m * 4 - 1, x2, color);
					}
				}
				tic_spacing = hist_tic_spacing;
				num_tic = (histo_max - histo_min) / tic_spacing;
				while (num_tic > 10) {
					num_tic = num_tic / 2;
					tic_spacing = tic_spacing * 2;
				}
				if (histo_min % 1024)
					stt_short = 1;
				else
					stt_short = 0;
				for (i = 0; i <= num_tic; i++) {
					if (stt_short) {
						if (i % 2)
							tic_size = 8;
						else
							tic_size = 5;
					}
					else {
						if (i % 2)
							tic_size = 5;
						else
							tic_size = 8;
					}
					if (num_tic > 0)
						VLine(hist_y, hist_y + tic_size, hist_x + hist_dx * i / num_tic, color);
				}
			}
			if (!lockHistFlag)
				paintWin(SM_HISTEXP, PAINTGRFX);
		}
		else if (paintFlag == PAINTTEXT) {
			if (!whiteBackgrnd)
				color = TEXT_YELLOW;
			else
				color = DKGRAY;
			for (i = 0; i <= num_tic; i++) {
				if (stt_short) {
					if (i % 2)
						tic_size = 8;
					else
						tic_size = 5;
				}
				else {
					if (i % 2)
						tic_size = 5;
					else
						tic_size = 8;
				}
				if (tic_size == 8) {
					sprintf(hist_label, "%d", histo_min + i * tic_spacing);
					int num_dig = 1;
					int this_num = histo_min + i * tic_spacing;
					double dig_w = 3.4;
					while (this_num >= 10) {
						this_num = this_num / 10;
						num_dig++;
					}
					x = hist_x + hist_dx * i / num_tic - (int)(num_dig*dig_w + 0.5);
					y = hist_y + 8;
					drawText(0, 0, hist_label, x, y, color, omFontDataSm, 0);
				}
			}
			sprintf(hist_label, "Mean    STD     Max    Min");
			x = hist_x + 160;
			y = hist_y - hFrame_dy + 10;
			drawText(0, 0, hist_label, x, y, color, omFontRprtTn, TRUE);
			if (RLI_div_flag2)
				sprintf(hist_label, "%7.2lf %6.2lf %7.2lf %6.2lf", histMeans[0]/100, histSTD[0]/100, histMax[0]/100, histMin[0]/100);
			else
				sprintf(hist_label, "%7.1lf %6.1lf %7.1lf %6.1lf", histMeans[0], histSTD[0], histMax[0], histMin[0]);
			x = x - 20;
			y = y + 12;
			if (!whiteBackgrnd)
				color = WHITE;
			else
				color = BLACK;
			drawText(0, 0, hist_label, x, y, color, omFontRprtTn, TRUE);
			for (m = 0; m < num_traces; m++) {
				if (histMax[m + 1] > -32000 && ROI_index[m][0] != BNC_SELECT) {
					if (RLI_div_flag2)
						sprintf(hist_label, "%7.2lf %6.2lf %7.2lf %6.2lf", histMeans[m + 1]/100, histSTD[m + 1]/100, histMax[m + 1]/100, histMin[m + 1]/100);
					else
						sprintf(hist_label, "%7.1lf %6.1lf %7.1lf %6.1lf", histMeans[m + 1], histSTD[m + 1], histMax[m + 1], histMin[m + 1]);
					y = y + 12;
					if (y >= hist_y - 10)
						break;
					int this_color = ((traceColorAr[pixColorCode[m]-1] & 0xff) << 16) |
						(traceColorAr[pixColorCode[m]-1] & 0xff00) |
						((traceColorAr[pixColorCode[m]-1] & 0xff0000) >> 16);
					drawText(0, 0, hist_label, x, y, this_color, omFontRprtTn, TRUE);
				}
			}
		}
	}
	else { // show FFT
		if (!num_traces)
			return;
		if (paintFlag == PAINTGRFX) {
			int x1, x2, y1, y2;
			traceFFT();
			if (!FFT_N_disp || FFT_N_disp > FFT_N)
				FFT_N_disp = FFT_N;
			color = YELLOW;
			hist_dx = 380;
			hist_dy = hist_y - OM[SM_HISTEXP].y - 40;
			for (i = 1; i < FFT_N_disp; i++) {
				x1 = hist_x + (i - 1)*hist_dx / (FFT_N_disp - 1);
				x2 = hist_x + i * hist_dx / (FFT_N_disp - 1);
				y1 = (int)(hist_y - FFT_ar[i - 1] * hist_dy / FFT_max);
				y2 = (int)(hist_y - FFT_ar[i] * hist_dy / FFT_max);
				Draw_Line(x1, y1, x2, y2, WHITE);
			}
			VLine(hist_y, hist_y - hist_dy, hist_x, color);
			VLine(hist_y, hist_y - hist_dy, hist_x + hist_dx, color);
			HLine(hist_x, hist_x + hist_dx, hist_y, color);
			HLine(hist_x, hist_x + hist_dx, hist_y - hist_dy, color);
			num_tic = 8;
			tic_spacing = FFT_N_disp / num_tic;
			while (num_tic > 10) {
				num_tic = num_tic / 2;
				tic_spacing = tic_spacing * 2;
			}
			stt_short = 0;
			for (i = 0; i <= num_tic; i++) {
				if (stt_short) {
					if (i % 2)
						tic_size = 8;
					else
						tic_size = 5;
				}
				else {
					if (i % 2)
						tic_size = 5;
					else
						tic_size = 8;
				}
				if (num_tic > 0)
					VLine(hist_y, hist_y + tic_size, hist_x + hist_dx * i / num_tic, color);
			}
			if (!lockHistFlag)
				paintWin(SM_HISTEXP, PAINTGRFX);
		}
		else if (paintFlag == PAINTTEXT) {
			color = TEXT_YELLOW;
			for (i = 0; i <= num_tic; i++) {
				if (stt_short) {
					if (i % 2)
						tic_size = 8;
					else
						tic_size = 5;
				}
				else {
					if (i % 2)
						tic_size = 5;
					else
						tic_size = 8;
				}
				if (tic_size == 8) {
					sprintf(hist_label, "%d", (int)(i * tic_spacing/(2*FFT_N*SM_exposure)));
					int num_dig = 1;
					int this_num = i * tic_spacing;
					double dig_w = 3.4;
					while (this_num >= 10) {
						this_num = this_num / 10;
						num_dig++;
					}
					if (num_tic > 0)
						x = hist_x + hist_dx * i / num_tic - (int)(num_dig*dig_w + 0.5);
					y = hist_y + 8;
					drawText(0, 0, hist_label, x, y, color, omFontDataSm, 0);
				}
			}
			if (FFTlog_flag) {
				strcpy(hist_label, "FFT-log");
				x = hist_x + hist_dx - 66;
			}
			else {
				strcpy(hist_label, "FFT");
				x = hist_x + hist_dx - 38;
			}
			y = OM[SM_HISTEXP].y + 55;
			drawText(0, 0, hist_label, x, y, color, omFontDataSm, 0);
		}
	}
}

static int sp_scale_val, bin_calib;
void drawMarks(int paintFlag)
{
	int x0, y0, dx, dy;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	dx = min(OM[SM_BNCPNL].x - OM[OM_MEASUREWIN].x - 5, (int)(disp_num_col*zoom_factor + 0.5));
	dy = OM[OM_MEASUREWIN].dy;
	if (paintFlag == PAINTGRFX) {
		int color, x, y, cross_w, x1, x2, y1, y2, i, m, n, chip_w;
		int roi_ColorAr[] = { RED, GREEN, BLUE, ORANGE };

		x = (int)(img_x0 + disp_num_col * zoom_factor / 2 + 0.5);
		y = (int)(img_y0 + disp_num_row * zoom_factor / 2 + 0.5);

		if (mark_center_flag) {
			color = YELLOW;
			cross_w = 10;
			drawCross(x, y, cross_w, color);
		}

		//	dy = min(OM[OM_MEASUREWIN].dy, cam_num_row*zoom_factor); 

		if (frameLiveFlag && seqROI_flag && disp_num_col == 2048) {
			n = 0;
			if (TwoK_flag)
				chip_w = disp_num_col;
			else
				chip_w = config_num_col / (1 + cds_lib[curConfig]);
			while (seqROI_ar[0][n]) {
				if ((chip_w > seqROI_ar[seqROI_flag - 1][n] + 1) && (cam_num_row > seqROI_ar[1][n])) {
					color = roi_ColorAr[n];
					y1 = y0 + min(max(0, (int)(img_y0 + ((disp_num_row - seqROI_ar[1][n]) / 2)*zoom_factor + 0.5)), dy);
					y2 = y0 + min(max(0, (int)(img_y0 + (disp_num_row - (disp_num_row - seqROI_ar[1][n]) / 2)*zoom_factor + 0.5)), dy);
					for (m = 0; m < (seqROI_flag - 2) / 2; m++) {
						x1 = x0 + min(max(0, (int)(img_x0 + (seqROI_ar[m * 2 + 2][n] + (disp_num_col - chip_w) / 2)*zoom_factor + 0.5)), dx);
						x2 = x0 + min(max(0, (int)(img_x0 + (seqROI_ar[m * 2 + 3][n] + (disp_num_col - chip_w) / 2)*zoom_factor + 0.5)), dx);
						if ((x1 == x0 && x2 == x0) || (x1 == x0 + dx && x2 == x0 + dx))
							continue;
						for (i = x1; i <= x2; i++) {
							if ((y1 > y0) && (y1 < y0 + dy))
								Draw_Line(i, y1, i, y1, color);
							if (y2 < y0 + dy)
								Draw_Line(i, y2, i, y2, color);
							i++;
						}
						for (i = y1; i <= y2; i++) {
							if (x1 > x0)
								Draw_Line(x1, i, x1, i, color);
							if (x2 < x0 + dx)
								Draw_Line(x2, i, x2, i, color);
							i++;
						}
					}
				}
				n++;
			}
		}
		if (scale_pix_coor[0] != scale_pix_coor[2] && scale_pix_coor[1] != scale_pix_coor[3]) {	//draw scale box
			color = PINK;
			x1 = x0 + min(max(0, (int)(img_x0 + scale_pix_coor[0] * zoom_factor + 0.5)), dx);
			y1 = y0 + min(max(0, (int)(img_y0 + scale_pix_coor[1] * zoom_factor + 0.5)), dy);
			x2 = x0 + min(max(0, (int)(img_x0 + scale_pix_coor[2] * zoom_factor + 0.5)), dx);
			y2 = x0 + min(max(0, (int)(img_y0 + scale_pix_coor[3] * zoom_factor + 0.5)), dy);
			for (i = x1; i <= x2; i++) {
				if (y1 > y0)
					Draw_Line(i, y1, i, y1, color);
				if (y2 < y0 + dy)
					Draw_Line(i, y2, i, y2, color);
				i++;
			}
			for (i = y1; i <= y2; i++) {
				if (x1 > x0)
					Draw_Line(x1, i, x1, i, color);
				if (x2 < x0 + dx)
					Draw_Line(x2, i, x2, i, color);
				i++;
			}
		}
		sp_scale_val = 0;
		if (sp_calibrate > 0.0) {	//display spatial calibration
			int sp_scale_dx, cal_factor;
			if (frameLiveFlag)
				bin_calib = ccd_lib_bin[curConfig];
			else
				bin_calib = file_bin;
			if (bin_calib) {
				color = YELLOW;
				sp_scale_dx = (int)(dx * 15 * bin_calib / (10 * zoom_factor * sp_calibrate));
				cal_factor = 1;
				while (sp_scale_dx > 10) {
					sp_scale_dx /= 10;
					cal_factor *= 10;
				}
				sp_scale_val = sp_scale_dx * cal_factor;
				sp_scale_dx = (int)(sp_scale_val*zoom_factor*sp_calibrate / (15 * bin_calib) + 0.5);
				x1 = 30;
				x2 = x1 + sp_scale_dx;
				y1 = min((int)(img_y0 + disp_num_row*0.98*zoom_factor + 0.5), y0 + dy - 60);
				HLine(x1, x2, y1, color);
				VLine(y1 - 3, y1, x1, color);
				VLine(y1 - 3, y1, x2, color);
			}
		}
	}
	else if (paintFlag == PAINTTEXT) {
		int x1, x2, y1, y2;
		char str[256];

		if (scale_pix_coor[0] != scale_pix_coor[2] && scale_pix_coor[1] != scale_pix_coor[3]) {	//draw scale box
			x1 = x0 + min(max(0, (int)(img_x0 + scale_pix_coor[0] * zoom_factor + 0.5)), dx);
			y1 = y0 + min(max(0, (int)(img_y0 + scale_pix_coor[1] * zoom_factor + 0.5)), dy);
			x2 = x0 + min(max(0, (int)(img_x0 + scale_pix_coor[2] * zoom_factor + 0.5)), dx);
			y2 = x0 + min(max(0, (int)(img_y0 + scale_pix_coor[3] * zoom_factor + 0.5)), dy);
			sprintf(str, "(%d,%d)", scale_pix_coor[0], scale_pix_coor[1]);
			drawText(0, 0, str, x1 - 5, y1 - 13, PINK, omFontRprtTn, TRUE);
			sprintf(str, "(%d,%d)", scale_pix_coor[0], scale_pix_coor[3]);
			drawText(0, 0, str, x1 - 5, y2 + 2, PINK, omFontRprtTn, TRUE);
			sprintf(str, "(%d,%d)", scale_pix_coor[2], scale_pix_coor[1]);
			drawText(0, 0, str, x2 - (int)strlen(str) * 7, y1 - 13, PINK, omFontRprtTn, TRUE);
			sprintf(str, "(%d,%d)", scale_pix_coor[2], scale_pix_coor[3]);
			drawText(0, 0, str, x2 - (int)strlen(str) * 7, y2 + 2, PINK, omFontRprtTn, TRUE);
		}
		if (sp_scale_val > 0) {
			x1 = 30;
			x2 = x1 + (int)(sp_scale_val*zoom_factor*sp_calibrate / (30 * bin_calib) + 0.5);
			y1 = min((int)(img_y0 + disp_num_row * 0.98*zoom_factor + 0.5), y0 + dy - 60);
			sprintf(str, "%dum", sp_scale_val);
			drawText(0, 0, str, x2 - (int)strlen(str) * 7 / 2, y1 - 15, TEXT_YELLOW, omFontRprtTn, TRUE);
			sprintf(str, "(%2.1lfX)", sp_calibrate);
			drawText(0, 0, str, x2 - (int)strlen(str) * 7 / 2, y1 + 2, TEXT_YELLOW, omFontRprtTn, TRUE);
		}
	}
}

static int seq_x_lb = 20;
int getLineProfileY(int ms_x1, int ms_y1, int mouseX, int mouseY)
{
	int x0, y0, x, y, x1, y1;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	x1 = (int)(((double)ms_x1 - img_x0 - x0) / zoom_factor);
	y1 = (int)(((double)ms_y1 - img_y0 - y0) / zoom_factor);
	x = (int)(((double)mouseX - img_x0 - x0) / zoom_factor);
	y = (int)(((double)mouseY - img_y0 - y0) / zoom_factor);
	if (y < 0 || y >= disp_num_row) {
		if (x < 0 || x >= disp_num_col)
			return 0;
		else {
			line_profileXY0 = (int)(x1*min(1.0, zoom_factor) + 0.5);
			line_profileXY = (int)(x*min(1.0, zoom_factor) + 0.5);
			return 2;
		}
	}
	else {
		line_profileXY0 = (int)(y1*min(1.0, zoom_factor) + 0.5);
		line_profileXY = (int)(y*min(1.0, zoom_factor) + 0.5);
		return 1;
	}
}

void getNDR_ref()
{
	long len = disp_num_col*disp_num_row;
	long this_pixel_val;
	signed short int *ptr, *brk_ptr;
	float *av_ptr;
	int i, j, m, NDR_break = 0, real_ref;

	real_ref = curDispFrame - NDR_subtraction_frames;
	real_ref = min(min(max(real_ref, trace_frame0), numTraceFrames + trace_frame0 - 1), numDataFrames - 1);
	ref_frame1 = curDispFrame - NDR_subtraction_frames - num_av_disp/2;
	ref_frame2 = ref_frame1 + num_av_disp - 1;
	ref_frame1 = min(max(ref_frame1, 0), numDataFrames - 1);
	ref_frame2 = min(max(ref_frame2, ref_frame1), numDataFrames - 1);

	for (i = 0; i < num_NDR_resets; i++) {
		if ((NDR_reset_ar[i] > ref_frame1) && (NDR_reset_ar[i] <= av_stt)) { //curDispFrame)) {
			NDR_break = NDR_reset_ar[i];
			if (ref_frame2 >= NDR_break) {
				if (NDR_break-ref_frame1 > ref_frame2-NDR_break)
					ref_frame2 = NDR_break-1;
				else {
					ref_frame1 = NDR_break;
					NDR_break = 0;
				}
			}
			break;
		}
		else
			NDR_break = 0;
	}


	av_ptr = (float *)ref_image;
	ptr = (signed short int *)image_data + ref_frame1*len;
	brk_ptr = (signed short int *)image_data + real_ref*len;
	for (j = 0; j < disp_num_row; j++) {
		for (i = 0; i < disp_num_col; i++) {
			for (m = 0, this_pixel_val = 0; m <= ref_frame2-ref_frame1; m++, ptr += len)
				this_pixel_val += *ptr;
			ptr -= len*m;
			*av_ptr = ((float)(this_pixel_val) + (m>>1))/m;
			if (NDR_break)
				*av_ptr -= *(brk_ptr+len*(NDR_break-real_ref-1)) - *(brk_ptr+len*(NDR_break-real_ref));
			av_ptr++;
			ptr++;
			brk_ptr++;
		}
	}
}

void getRefImage()
{
	long len = disp_num_col*disp_num_row;
	long this_pixel_val = 0L;
	signed short int *ptr;
	float *av_ptr;
	int i, j, m;
	char tmp_str[256];

	if (disp_setting_handle) {
		SendMessage(GetDlgItem(disp_setting_handle, FRAME_SUB1), BM_SETCHECK, 0, 0);
		SendMessage(GetDlgItem(disp_setting_handle, FRAME_SUB2), BM_SETCHECK, 1, 0);
		sprintf(tmp_str, "%d", ref_frame1 + 1);
		SetDlgItemText(disp_setting_handle, REF_SUB_FRAME1, tmp_str);
		sprintf(tmp_str, "%d", ref_frame2 + 1);
		SetDlgItemText(disp_setting_handle, REF_SUB_FRAME2, tmp_str);
	}
	OM[SM_REFSTART].attr = min((ref_frame1 + ref_frame2) * 500 / (numTraceFrames - 1), 1000);
	if (NDR_subtraction_flag) {
		darkSubFlag = TRUE;
		if (!OM[SM_DKSUBBOX].attr)
			setChckBox(SM_DKSUBBOX);
		getNDR_ref();
	}
	else {
		av_ptr = (float *)ref_image;
		ptr = (signed short int *)image_data + ref_frame1*len;
		for (j = 0; j < disp_num_row; j++) {
			for (i = 0; i < disp_num_col; i++) {
				for (m = 0; m <= ref_frame2-ref_frame1; m++) {
					if (m == 0)
						this_pixel_val = *ptr;
					else
						this_pixel_val += *ptr;
					ptr += len;
				}
				ptr -= len*m;
				*av_ptr = ((float)this_pixel_val)/m;
				av_ptr++;
				ptr++;
			}
		}
	}
	if (RLI_div_flag)
		getAllTraces();
}

void getRLIImage()
{
	long len = disp_num_col * disp_num_row;
	long this_pixel_val = 0L;
	signed short int *ptr, *dk_ptr;;
	float *av_ptr;
	int i, j, m;

	if (RLI_image)
		_aligned_free(RLI_image);
	RLI_image = (float *)_aligned_malloc(disp_num_col*disp_num_row * sizeof(float), 2);
	av_ptr = (float *)RLI_image;
	dk_ptr = dark_data;
	ptr = (signed short int *)image_data + RLI_f1 * len;
	for (j = 0; j < disp_num_row; j++) {
		for (i = 0; i < disp_num_col; i++) {
			for (m = 0; m <= RLI_f2 - RLI_f1; m++) {
				if (m == 0)
					this_pixel_val = *ptr;
				else
					this_pixel_val += *ptr;
				ptr += len;
			}
			ptr -= len * m;
			*av_ptr = ((float)this_pixel_val) / m - *dk_ptr;
			av_ptr++;
			ptr++;
			dk_ptr++;
		}
	}
}

void showROI_labels()
{
	int x, y, k, x0, y0;
	int color;
	char str[20];

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	for (k = 0; k < num_traces; k++) {
		if (ROI_index[k][0] == BNC_SELECT)	//BNCs
			continue;
		color = ((traceColorAr[pixColorCode[k] - 1] & 0xff) << 16) |
			(traceColorAr[pixColorCode[k] - 1] & 0xff00) |
			((traceColorAr[pixColorCode[k] - 1] & 0xff0000) >> 16);
		x = (int)(x0 + img_x0 + (ROI_index[k][1] + ROI_index[k][2])*zoom_factor / 2 + 0.5);
		y = (int)(y0 + img_y0 + (ROI_index[k][3] + ROI_index[k][4])*zoom_factor / 2 - 7 + 0.5);
		if (roi_label_mode == 1)
			sprintf(str, "%d", ROI_index[k][5]);
		else
			sprintf(str, "%d", k + 1);
		drawText(0, 0, str, x - (int)(strlen(str)*3.5+0.5), y, color, omFontRprt, TRUE);
	}
}

void drawPixTrace(int i, int j, int x1, int dx, int y1, int dy, int this_col) 
{
	int m, x, y, m0, y0;
	float y_base;
	float *t_pt;
	short int *image_ptr;
	long len;
	double y_gain, t_step;
	int win_y, dy0;

	win_y = getMeasureWinY0();
	dy0 = image_region_dy;
	t_pt = &this_smTrace[0];
	len = disp_num_col * disp_num_row;
	image_ptr = image_data + j* disp_num_col + i + len * trace_frame0;
	for (m = trace_frame0; m < num_trace_pnts; m++, t_pt++, image_ptr += len)
		*t_pt = (float)(*image_ptr);
//	FilterTrace();
	if (trace_lpass > 0.0)
		lpass_gauss_filter();
	y_gain = get_traceYGain(traceYGain)*dy/OM[SM_TRCGAIN].dy;
	t_step = ((double)num_trace_pnts - trace_frame0) / (dx + 1);
	y_base = this_smTrace[trace_min_frame];
	m0 = 0;
	y0 = (int)((this_smTrace[0] - y_base)*y_gain + 0.5);
	for (m = 1; m <= dx; m++) {
		x = (int)(m*t_step + 0.5);
		y = (int)((this_smTrace[x] - y_base )*y_gain + 0.5);
		Draw_Line(x1+m0, min(max(win_y,y1-y0), win_y+dy0), x1+m, min(max(win_y, y1-y), win_y + dy0), this_col);
		m0 = m;
		y0 = y;
	}
}

void getActivationMap() {
	signed short *img_ptr, *actv_ptr;
	int m, actv_fr, diff;
	long i, len;

	len = ((long)mov_num_col) * mov_num_row;
	if (activation_fr_ar != NULL)
		_aligned_free(activation_fr_ar);
	activation_fr_ar = (signed short int *)_aligned_malloc(len * sizeof(signed short int), 2);

	int omit_this = 0;
	actv_ptr = activation_fr_ar;
	for (i = 0; i < len; i++) {
		img_ptr = movie_data + actv_start * len + i;
		diff = *(img_ptr + len) - *img_ptr;
		img_ptr += len;
		actv_fr = actv_start;
		if (omit_ar)
			omit_this = *(omit_ar + i);
		if (omit_this) {
			*actv_ptr++ = mov_omit_val;
		}
		else {
			for (m = actv_start + 1; m < actv_end; m++) {
			//	if (*(img_ptr + len) - *img_ptr < 0)
				//	break;
				if (*(img_ptr + len) - *img_ptr > diff)
					actv_fr = m;
				img_ptr += len;
			}
			*actv_ptr++ = actv_fr;
		}
	}

/*	FILE *fp;
	if (!FOPEN_S(fp, "actv_test.txt", "w"))
		return;
	actv_ptr = activation_fr_ar;
	for (i = 0; i < mov_num_row; i++) {
		for (m = 0; m < mov_num_col; m++) {
			fprintf(fp, "%d, ", *actv_ptr++);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);*/
}

void getAPD_map() {
	signed short *img_ptr, *actv_ptr, *apd_ptr;
	int m, ap_max, past_peak;
	long i, len;
	double apd_percent;

	len = ((long)mov_num_col) * mov_num_row;
	if (apd_ar != NULL)
		_aligned_free(apd_ar);
	apd_ar = (signed short int *)_aligned_malloc(len * sizeof(signed short int), 2);

	if (apd_mode)
		apd_percent = 0.2;
	else
		apd_percent = 0.5;

	int omit_this = 0;
	apd_ptr = apd_ar;
	actv_ptr = activation_fr_ar;
	for (i = 0; i < len; i++) {
		img_ptr = movie_data + *actv_ptr * len + i;
		ap_max = *img_ptr++;
		if (omit_ar)
			omit_this = *(omit_ar + i);
		if (omit_this) {
			*apd_ptr = mov_omit_val;
		}
		else {
			past_peak = 0;
			for (m = *actv_ptr + 1; m < numMovFrames; m++) {
				//	if (*(img_ptr + len) - *img_ptr < 0)
				//	break;
				if (ap_max < *img_ptr)
					ap_max = *img_ptr;
				if (ap_max > *img_ptr)
					past_peak = 1;
				if (past_peak) {
					if (*img_ptr <= apd_percent * ap_max) {
						*apd_ptr = m - *actv_ptr;
						break;
					}
				}
				img_ptr += len;
			}
		}
		actv_ptr++;
		apd_ptr++;
	}
}

void getConductionMap() {
	signed short *actv_ptr, *cv_ptr;
	double *cvx_ptr, *cvy_ptr, max_cv;
	int x, y;
	long i, len;

	len = ((long)mov_num_col) * mov_num_row;
	if (cv_x_ar != NULL)
		_aligned_free(cv_x_ar);
	cv_x_ar = (double *)_aligned_malloc(len * sizeof(double), 2);
	if (cv_y_ar != NULL)
		_aligned_free(cv_y_ar);
	cv_y_ar = (double *)_aligned_malloc(len * sizeof(double), 2);
	if (cv_ar != NULL)
		_aligned_free(cv_ar);
	cv_ar = (signed short *)_aligned_malloc(len * sizeof(signed short), 2);

	int omit_this = 0;
	cvx_ptr = cv_x_ar;
	cvy_ptr = cv_y_ar;
	actv_ptr = activation_fr_ar;
	max_cv = 0.0;
	for (y = 0; y < mov_num_row; y++) {
		for (x = 0; x < mov_num_col; x++) {
			i = disp_num_col * y + x;
			if (omit_ar)
				omit_this = *(omit_ar + i);
			if (omit_this || !x || !y || x==mov_num_col-1 || y==mov_num_row-1) {
				*cvx_ptr = (double)mov_omit_val;
				*cvy_ptr = (double)mov_omit_val;
			}
			else {
				*cvx_ptr = 1.0/(*(actv_ptr + 1) - *(actv_ptr - 1));
				*cvy_ptr = 1.0/(*(actv_ptr + mov_num_col) - *(actv_ptr - mov_num_col));
				if (max_cv < sqrt(*cvx_ptr**cvx_ptr + *cvy_ptr**cvy_ptr))
					max_cv = sqrt(*cvx_ptr**cvx_ptr + *cvy_ptr**cvy_ptr);
			}
			actv_ptr++;
			cvx_ptr++;
			cvy_ptr++;
		}
	}
	cvx_ptr = cv_x_ar;
	cvy_ptr = cv_y_ar;
	cv_ptr = cv_ar;
	double cv_f = 16000 / max_cv;
	for (y = 0; y < mov_num_row; y++) {
		for (x = 0; x < mov_num_col; x++) {
			i = disp_num_col * y + x;
			if (omit_ar)
				omit_this = *(omit_ar + i);
			if (omit_this || !x || !y || x == mov_num_col - 1 || y == mov_num_row - 1)
				*cv_ptr = mov_omit_val;
			else
				*cv_ptr = (int)(cv_f*sqrt(*cvx_ptr**cvx_ptr + *cvy_ptr**cvy_ptr));
			cv_ptr++;
			cvx_ptr++;
			cvy_ptr++;
		}
	}
}

static int ov_max, ov_min, multi_dx, multi_dy, save_tiff_dx, save_tiff_dy;
static double ov_scale, mov_zoom_factor, multi_dim_pcnt = 0.9;
int mov_img_x0, mov_img_y0;
void drawCV_map() {
	int i, j, x0, y0, dx, dy, x1, y1, x2, y2, this_bin=4;

	x0 = OM[SM_MOVIEWIN].x;
	y0 = OM[SM_MOVIEWIN].y;

	dy = OM[SM_MOVIEWIN].dy;
	dx = OM[SM_MOVIEPNL].x - OM[SM_MOVIEWIN].x - 10;

	mov_zoom_factor = dx * 0.9 / (mov_num_col*3.0);

	for (j = 0; j < mov_num_row / this_bin; j++) {
		for (i = 0; i < mov_num_col/ this_bin; i++) {
			x1 = (int)(x0 + mov_img_x0 + i * mov_zoom_factor * this_bin);
			x2 = (int)(x1 + mov_zoom_factor * this_bin);
			y1 = (int)(y0 + mov_img_y0 + j * mov_zoom_factor *this_bin);
			y2 = (int)(y1 + mov_zoom_factor * this_bin);
			Draw_Line(x1, y1, x2, y2, LTGRAY);
		}
	}
}

void oneMovFrame(int frame_id, int saveTiff_flag)
{
	int x, dx, y, x0, y0, dy, i, j, ov_img_y0, mov_y0;
	double pix_scale, ov_zoom_factor;
	short int *image_ptr, *ov_img_ptr;
	long len;
	double row_skip_r = 0.05, col_skip_r = 0.05;
	int row_skip_min_mov, row_skip_max_mov, col_skip_min_mov, col_skip_max_mov;
	unsigned int *tiff_img_ptr;

	x0 = OM[SM_MOVIEWIN].x;
	y0 = OM[SM_MOVIEWIN].y;
	if (hotTab == SM_CARDIOVIEW)
		dy = OM[SM_MOVIEWIN].dy;
	else
		dy = OM[SM_MOVIEWIN].dy * 2 / 3;
	dx = OM[SM_MOVIEPNL].x - OM[SM_MOVIEWIN].x - 10;
	if (mov_multi_flag)
		mov_y0 = mov_img_y0;
	else 
		mov_y0 = 0;
	if (movie_data != NULL && overlay_mode < 2) {
		len = mov_num_col * mov_num_row * frame_id;

		if (!movie_scale_mode) {
			row_skip_min_mov = max(1, (int)(mov_num_row * row_skip_r + 0.5));
			row_skip_max_mov = min(mov_num_row - 2, (int)(mov_num_row * (1 - row_skip_r) + 0.5));
			col_skip_min_mov = max(1, (int)(mov_num_col * col_skip_r + 0.5));
			col_skip_max_mov = min(mov_num_col - 2, (int)(mov_num_col * (1 - col_skip_r) + 0.5));

			mov_max = -32765;
			mov_min = 32765;
			if (frame_id == -3)
				image_ptr = cv_ar;
			else if (frame_id == -2)
				image_ptr = apd_ar;
			else if (frame_id == -1)
				image_ptr = activation_fr_ar;
			else
				image_ptr = movie_data + len;
			for (j = 0; j < mov_num_row; j++) {
				for (i = 0; i < mov_num_col; i++) {
					if (j > row_skip_min_mov && j < row_skip_max_mov && i > col_skip_min_mov && i < col_skip_max_mov && (*image_ptr != mov_omit_val)) {
						if (mov_max < *image_ptr)
							mov_max = *image_ptr;
						else if (mov_min > *image_ptr)
							mov_min = *image_ptr;
					}
					image_ptr++;
				}
			}
			checkMovScale();
		}
		pix_scale = 255.0 / (mov_max - mov_min);
		if (!overlay_mode) {
			ov_num_col = mov_num_col;
			ov_num_row = mov_num_row;
			ov_min = mov_min;
			ov_max = mov_max;
		}
	}

	if (!mov_multi_flag) {
		if (saveTiff_flag == 1) {
			ov_zoom_factor = 1.0;
			if (!overlay_mode)
				mov_zoom_factor = ov_zoom_factor;
			else
				mov_zoom_factor = ov_zoom_factor * ov_num_col / mov_num_col;
		}
		else {
			mov_zoom_factor = zoom_factor * movie_bin;
			if (hotTab == SM_CARDIOVIEW)
				mov_zoom_factor = dx*0.9 /(mov_num_col*3.0);
			if (!overlay_mode)
				ov_zoom_factor = mov_zoom_factor;
			else if (overlay_mode == 1)
				ov_zoom_factor = mov_zoom_factor * mov_num_col / ov_num_col;
			else
				ov_zoom_factor = zoom_factor * disp_num_col / ov_num_col;
		}
	}
	else {
		mov_zoom_factor = min((dx*multi_dim_pcnt) / (mov_num_col*mov_multi_num_col), (dy*multi_dim_pcnt) / (mov_num_row*mov_multi_num_row));
		if (!overlay_mode) {
			ov_zoom_factor = mov_zoom_factor;
			mov_img_y0 += (int)((multi_dy - mov_num_row * mov_zoom_factor) / 2);
		}
		else if (overlay_mode == 1) {
			ov_zoom_factor = mov_zoom_factor * mov_num_col / ov_num_col;
			mov_img_y0 += (int)((multi_dy - mov_num_row * mov_zoom_factor) / 2);
		}
		else
			ov_zoom_factor = min((dx*multi_dim_pcnt) / (ov_num_col*mov_multi_num_col), (dy*multi_dim_pcnt) / (ov_num_row*mov_multi_num_row));
	}
	if (!movie_data) {
		if (mov_multi_flag)
			ov_img_y0 = mov_img_y0;
		else
			ov_img_y0 = 0;
	}
	else
		ov_img_y0 = (int)(mov_img_y0 - (ov_num_row * ov_zoom_factor - mov_num_row * mov_zoom_factor) / 2 + 0.5);

	int l, y_pos;
	int pix_val = 0, ov_pix_val = 0;
	unsigned char val;
	int this_x, pixel_y, ov_pix_y, pixel_x;
	unsigned int this_col, mov_col;
	int skip_this;
	int draw_x;

//	if (mov_img_y0 < ov_img_y0) {
//		MessageBox(main_window_handle, "The Overlay Image has wrong aspect ratio", "message", MB_OK);
//		return;
//	}

	if (saveTiff_flag)
		memset(MovieTiffImg, 0, save_tiff_dx*save_tiff_dy*sizeof(unsigned int));
	memset(ImageLine, 0, sizeof(ImageLine));
	tiff_img_ptr = MovieTiffImg;
	long int ov_jump;
	if (overlay_mode >= 1)
		ov_jump = ovFile_num_col * ovFile_num_row *min(curMovFrame, numOverlayFrames - 1);
	else
		ov_jump = 0;
	if ((overlay_mode == 1 && (mov_img_y0 > ov_img_y0)) || (overlay_mode == 2)) {
		for (y = 0; y < (int)(ov_num_row * min(1.0, ov_zoom_factor) + 0.5); y++) {
			ov_pix_y = (int)(y / min(1.0, ov_zoom_factor) + 0.5);
			ov_img_ptr = overlay_img + ov_jump + ov_pix_y * ov_num_col;
			draw_x = 0;
			for (x = 0; x < ov_num_col; x++) {
				ov_pix_val = *ov_img_ptr++;
				val = (unsigned char)((ov_pix_val - ov_min)*ov_scale);
				this_col = regular_LUT[val];
				this_x = (int)(draw_x*ov_zoom_factor + 0.5);
				for (l = 0; l < (int)(max(1.0, ov_zoom_factor + 1)); l++)
					ImageLine[this_x + l] = this_col;
				draw_x++;
			}
			if (saveTiff_flag == 1) {
				memcpy(tiff_img_ptr, ImageLine, ov_num_col*sizeof(unsigned int));
				tiff_img_ptr += ov_num_col;
			}
			else {
				for (int k = 0; k < (int)(max(1.0, ov_zoom_factor + 1)); k++) {
					y_pos = ov_img_y0 + (int)(y * max(1.0, ov_zoom_factor) + 0.5) + k;
					if (y_pos > mov_y0 && y_pos < dy) {
						if (!saveTiff_flag)
							putImageLine(mov_img_x0, y_pos, multi_dx, ImageLine);
						else {
							tiff_img_ptr = MovieTiffImg + (y_pos- mov_y0) * multi_dx;
							memcpy(tiff_img_ptr, ImageLine, multi_dx * sizeof(unsigned int));
						}
					}
				}
			}
		}
	}
	if (overlay_mode <= 1) {
		int ov_y_start = 0, ov_y0 = 0;
		if (mov_img_y0 > ov_img_y0) {
			ov_y_start = (int)((mov_img_y0 - ov_img_y0) / ov_zoom_factor + 0.5);
			ov_y0 = (int)(ov_y_start / min(1.0, ov_zoom_factor) + 0.5);
		}
		tiff_img_ptr = MovieTiffImg + ov_y0*ov_num_col;
		for (y = ov_y_start; y < (int)(ov_num_row * min(1.0, ov_zoom_factor) + 0.5); y++) {
			ov_pix_y = (int)(y / min(1.0, ov_zoom_factor) + 0.5);
			pixel_y = (int)((ov_pix_y - ov_y0)*ov_zoom_factor/mov_zoom_factor);
			if (pixel_y >= mov_num_row)
				break;;

			ov_img_ptr = overlay_img;
			if (overlay_mode > 0)
				ov_img_ptr += ov_jump + ov_pix_y * ov_num_col;
			if (frame_id == -3)
				image_ptr = cv_ar + pixel_y * mov_num_col;
			else if (frame_id == -2)
				image_ptr = apd_ar + pixel_y * mov_num_col;
			else if (frame_id == -1)
				image_ptr = activation_fr_ar + pixel_y * mov_num_col;
			else
				image_ptr = movie_data + len + pixel_y * mov_num_col;
			if (skip_edge_rows && ((pixel_y == 0) || (pixel_y == mov_num_row - 1)))
				skip_this = 1;
			else
				skip_this = 0;
			draw_x = 0;
			int ov_pix_cnt = 0;
			pixel_x = 0;
			for (x = 0; x < ov_num_col; x++) {
				if (!overlay_mode) {
					pix_val = *image_ptr++;
					pixel_x++;
				}
				else if (overlay_mode == 1) {
					ov_pix_val = *ov_img_ptr++;
					pix_val = *image_ptr;
					ov_pix_cnt++;
					if (ov_pix_cnt >= (int)(mov_zoom_factor / ov_zoom_factor + 0.5)) {
						image_ptr++;
						ov_pix_cnt = 0;
						pixel_x++;
					}
				}
				else
					ov_pix_val = *ov_img_ptr++;

				if (movie_scale_mode == 2) {
					mov_min = *(mov_trace_scale_min + pixel_y * mov_num_col + pixel_x);
					mov_max = *(mov_trace_scale_max + pixel_y * mov_num_col + pixel_x);
					if (mov_max == mov_omit_val)
						pix_scale = 0;
					else
						pix_scale = 255.0 / (mov_max - mov_min);
				}

				if (pix_val != mov_omit_val) {
					if (skip_this)
						pix_val = mov_min;
					if (pix_val < mov_min)
						pix_val = mov_min;
					else if (pix_val > mov_max)
						pix_val = mov_max;
				}
				if (!overlay_mode && pixel_y >= 0) {
					val = (unsigned char)((pix_val - mov_min)*pix_scale);
					if (pix_val <= mov_cutoff)
						this_col = 0;
					else {
						if (!ap_color)
							this_col = rainbow_LUT[val];
						else
							this_col = regular_LUT[val];
					}
				}
				else if (overlay_mode == 1 && pixel_y >= 0) {
					val = (unsigned char)((ov_pix_val - ov_min)*ov_scale);
					this_col = regular_LUT[val];
					val = (unsigned char)((pix_val - mov_min)*pix_scale);
					mov_col = rainbow_LUT[val];
					if (pix_val > mov_cutoff)
						this_col = (unsigned int)((this_col & 0xff)*movie_transparency + (mov_col & 0xff)*(1 - movie_transparency)) |
						((unsigned int)(((this_col & 0xff00) >> 8)*movie_transparency + ((mov_col & 0xff00) >> 8)*(1 - movie_transparency)) << 8) |
						((unsigned int)(((this_col & 0xff0000) >> 16)*movie_transparency + ((mov_col & 0xff0000) >> 16)*(1 - movie_transparency)) << 16);
				}
				else {
					val = (unsigned char)((ov_pix_val - ov_min)*ov_scale);
					this_col = regular_LUT[val];
				}
				this_x = (int)(draw_x*ov_zoom_factor + 0.5);
				for (l = 0; l < (int)(max(1.0, ov_zoom_factor + 1)); l++)
					ImageLine[this_x + l] = this_col;

				draw_x++;
			}
			if (saveTiff_flag == 1) {
				memcpy(tiff_img_ptr, ImageLine, ov_num_col * sizeof(unsigned int));
				tiff_img_ptr += ov_num_col;
			}
			else {
				for (int k = 0; k < (int)(max(1.0, ov_zoom_factor + 1)); k++) {
					y_pos = mov_img_y0 + (int)((y - ov_y_start) * max(1.0, ov_zoom_factor) + 0.5) + k;
					if (y_pos > mov_y0 && y_pos < dy) {
						if (!saveTiff_flag)
							putImageLine(mov_img_x0, y_pos, multi_dx, ImageLine);
						else {
							tiff_img_ptr = MovieTiffImg + (y_pos - mov_y0) * multi_dx;
							memcpy(tiff_img_ptr, ImageLine, multi_dx * sizeof(unsigned int));
						}
					}
				}
			}
		}
	}
	//draw color scale bar
	if (!mov_multi_flag && frame_id >= 0) {
		int num_colors = sizeof(rainbow_LUT) / sizeof(rainbow_LUT[0]);
		int x1, x2;
		y = y0 + (int)(mov_img_y0 + mov_num_row * mov_zoom_factor);
		x1 = min((int)(x0 + mov_img_x0 + mov_num_col*mov_zoom_factor + 2.5*mov_zoom_factor), OM[SM_CARDIOPNL].x);
		x2 = (int)(x1 + mov_zoom_factor*4);
		for (int i = 0; i < (int)(mov_num_row * mov_zoom_factor); i++) {
			j = (int)(i*num_colors/(mov_num_row * mov_zoom_factor) + 0.5);
			if (ap_color == 0)
				this_col = rainbow_LUT[j];
			else
				this_col = regular_LUT[j];
			HLine(x1, x2, y - i, this_col);
		}
	}
	if (roi_border_flag && (overlay_mode < 2) && (saveTiff_flag != 1) && (frame_id >= 0))
		drawSelectedPixels(1, saveTiff_flag);
}

static int inset_num_tics, inset_trace_cnt;
static double inset_tic_spacing, inset_ms_spacing;
void DisplayMovie(int paintFlag, int saveTiff_flag)
{
	int i, j, x0, y0, dx, dy;
	double row_skip_r = 0.05, col_skip_r = 0.05;
	int row_skip_min_ov, row_skip_max_ov, col_skip_min_ov, col_skip_max_ov, multi_flag_sv;
	short int *ov_img_ptr;
	unsigned int *tiff_img_ptr;
	int inset_bd = 10;

	if (!movie_data && !overlay_img)
		return;

	multi_flag_sv = mov_multi_flag;
	if (saveTiff_flag) 
		mov_multi_flag = 0;

	setOverlayButtons();
	x0 = OM[SM_MOVIEWIN].x;
	y0 = OM[SM_MOVIEWIN].y;
	if (hotTab == SM_CARDIOVIEW)
		dy = OM[SM_MOVIEWIN].dy;
	else
		dy = OM[SM_MOVIEWIN].dy * 2 / 3;
	dx = OM[SM_MOVIEPNL].x - OM[SM_MOVIEWIN].x - 10;
	if (paintFlag == PAINTGRFX) {
		if (overlay_img != NULL && overlay_mode > 0) {
			row_skip_min_ov = 0; // max(1, (int)(ov_num_row * row_skip_r + 0.5));
			row_skip_max_ov = ov_num_row - 1; // min(ov_num_row - 2, (int)(ov_num_row * (1 - row_skip_r) + 0.5));
			col_skip_min_ov = 0; // max(1, (int)(ov_num_col * col_skip_r + 0.5));
			col_skip_max_ov = ov_num_col - 1;// min(ov_num_col - 2, (int)(ov_num_col * (1 - col_skip_r) + 0.5));
			ov_max = -32765;
			ov_min = 32765;
			ov_img_ptr = overlay_img + ovFile_num_col * ovFile_num_row *min(curMovFrame, numOverlayFrames - 1);
			for (j = 0; j < ov_num_row; j++) {
				for (i = 0; i < ov_num_col; i++) {
					if (j > row_skip_min_ov && j < row_skip_max_ov && i > col_skip_min_ov && i < col_skip_max_ov) {
						if (ov_max < *ov_img_ptr)
							ov_max = *ov_img_ptr;
						else if (ov_min > *ov_img_ptr)
							ov_min = *ov_img_ptr;
					}
					ov_img_ptr++;
				}
			}
			ov_scale = 255.0 / (ov_max - ov_min);
		}
		if (!mov_multi_flag) {
			mov_img_x0 = img_x0;
			if (hotTab == SM_CARDIOVIEW)
				mov_img_y0 = img_y0 + 10;
			else
				mov_img_y0 = img_y0;
			if (saveTiff_flag == 2)
				multi_dx = save_tiff_dx;
			else
				multi_dx = dx;
			multi_dy = 0;
			oneMovFrame(curMovFrame, saveTiff_flag);
			if (hotTab == SM_CARDIOVIEW && activation_fr_ar != NULL) {
				if (actv_flag) {
					mov_img_x0 = img_x0;
					mov_img_y0 = img_y0 + OM[SM_CARDIOWIN].dy / 2;
					oneMovFrame(-1, saveTiff_flag);
				}
				if (apd_ar && apd_flag) {
					mov_img_x0 = img_x0 + actv_flag * OM[SM_CARDIOWIN].dx / 3;
					mov_img_y0 = img_y0 + OM[SM_CARDIOWIN].dy / 2;
					multi_dx = dx - mov_img_x0;
					oneMovFrame(-2, saveTiff_flag);
				}
				if (cv_ar && cv_flag) {
					mov_img_x0 = img_x0 + (actv_flag + apd_flag) * OM[SM_CARDIOWIN].dx / 3;
					mov_img_y0 = img_y0 + OM[SM_CARDIOWIN].dy / 2;
					multi_dx = dx - mov_img_x0;
					oneMovFrame(-3, saveTiff_flag);
				//	drawCV_map();
				}
			}
		}
		else {
			multi_dx = dx / mov_multi_num_col;
			multi_dy = dy / mov_multi_num_row;
			for (j = 0; j < mov_multi_num_row; j++) {
				for (i = 0; i < mov_multi_num_col; i++) {
					mov_img_x0 = multi_dx*i;
					mov_img_y0 = multi_dy*j;
					if (mov_multi_f1 + (j* mov_multi_num_col + i)*mov_multi_steps < numMovFrames)
						oneMovFrame(mov_multi_f1 + (j* mov_multi_num_col+i)*mov_multi_steps, 0);
				}
			}
		}
		if (overlay_mode < 2) {
			// MOVIE trace inset
			int x1, x2, y1, y2, k, cur_fr_x, cur_fr_y;
			int numTraceFrames_sv = numTraceFrames;
			float trace_min, trace_max;
			double y_gain = 0.0, t_dx;
			inset_trace_cnt = 0;

			inset_x = x0 + 50;
			inset_y = y0 + dy + 30;
			inset_dy = OM[SM_MOVIEWIN].dy - dy - 120;
			if (hotTab == SM_CARDIOVIEW) {
				inset_dx = (int)((dx - mov_num_col*mov_zoom_factor)*0.7);
				inset_x = (int)(OM[SM_CARDIOPNL].x - inset_dx - 50);
				inset_y = y0 + img_y0;
				inset_dy = (int)(disp_num_row*mov_zoom_factor + 0.5);
			}
			else if (!mov_multi_flag)
				inset_dx = min((int)(mov_num_col*mov_zoom_factor - 100), dx);
			else 
				inset_dx = (int)(((double)dx) *(mov_multi_num_col-1)/ mov_multi_num_col + mov_num_col*mov_zoom_factor - 100);

			numTraceFrames = numMovFrames;
			t_dx = (double)inset_dx / numTraceFrames;
			for (k = 0; k < num_traces; k++) {
			//	if (inset_trace_cnt == 1)
			//		break;
				if (ROI_index[k][0] == BNC_SELECT)
					continue;
				memcpy(this_smTrace, smTrace[k], sizeof(this_smTrace[0])*numTraceFrames);
				FilterTrace();
				trace_min = 32765;
				trace_max = -32765;
				for (i = 0; i < numTraceFrames; i++) {
					if (trace_min > this_smTrace[i])
						trace_min = this_smTrace[i];
					else if (trace_max < this_smTrace[i])
						trace_max = this_smTrace[i];
				}
				if (!inset_trace_cnt) {
					if (trace_max - trace_min > 0)
						y_gain = inset_dy / ((trace_max - trace_min)*num_traces);
				}
				int color = traceColorAr[pixColorCode[k] - 1];
				for (i = 1; i < numTraceFrames; i++) {
					x1 = (int)(inset_x + 1.0*(i - 1)*t_dx + 0.5);
					x2 = (int)(inset_x + 1.0*i*t_dx + 0.5);
					y1 = (int)(inset_y + inset_dy * (k + 1) / num_traces - (this_smTrace[i - 1] - trace_min)*y_gain + 0.5);
					y2 = (int)(inset_y + inset_dy * (k + 1) / num_traces - (this_smTrace[i] - trace_min)*y_gain + 0.5);
					if ((y1 > inset_y || y2 > inset_y) && (y1 < inset_y + inset_dy || y2 < inset_y + inset_dy)) {
						y1 = min(max(y1, inset_y), inset_y + inset_dy);
						y2 = min(max(y2, inset_y), inset_y + inset_dy);
						if (saveTiff_flag == 2) {
							x1 -= x0;
							x2 -= x0;
							y1 -= y0;
							y2 -= y0;
							if (y2 == y1) {
								for (k = x1; k <= x2; k++) {
									tiff_img_ptr = MovieTiffImg + y1 * multi_dx + k;
									*tiff_img_ptr = color;
								}
							}
							else if (y2 > y1) {
								for (k = y1; k <= y2; k++) {
									tiff_img_ptr = MovieTiffImg + k * multi_dx + x1 + (x2 - x1)*(k - y1) / (y2 - y1);
									*tiff_img_ptr = color;
								}
							}
							else {
								for (k = y1; k >= y2; k--) {
									tiff_img_ptr = MovieTiffImg + k * multi_dx + x1 + (x2 - x1)*(k - y1) / (y2 - y1);
									*tiff_img_ptr = color;
								}
							}
							x1 += x0;
							x2 += x0;
							y1 += y0;
							y2 += y0;
						}
						else
							Draw_Line(x1, y1, x2, y2, color);
					}
					if (i == curMovFrame) {
						cur_fr_x = x2;
						cur_fr_y = y2;
					}
					else if (curMovFrame == 0) {
						cur_fr_x = inset_x;
						cur_fr_y = (int)(inset_y + inset_dy - (this_smTrace[0] - trace_min)*y_gain + 0.5);
					}
				}
				inset_trace_cnt++;
			}
			numTraceFrames = numTraceFrames_sv;
			if (inset_trace_cnt) {
				if (saveTiff_flag == 2) {
					draw_HLine(inset_x - inset_bd, inset_x + inset_dx + inset_bd, inset_y - inset_bd, colorGrayText, 1);
					draw_HLine(inset_x - inset_bd, inset_x + inset_dx + inset_bd, inset_y + inset_dy + inset_bd, colorGrayText, 1);
					draw_VLine(inset_y - inset_bd, inset_y + inset_dy + inset_bd, inset_x - inset_bd, colorGrayText, 1);
					draw_VLine(inset_y - inset_bd, inset_y + inset_dy + inset_bd, inset_x + inset_dx + inset_bd, colorGrayText, 1);
				}
				else {
					paintSubFrame(inset_x - inset_bd, inset_y - inset_bd, inset_dx + inset_bd * 2, inset_dy + inset_bd * 2, colorGrayText);
					// add tics
					int tic_size, axis_x, axis_y;
					int ms_spacing_int = (int)(numMovFrames*SM_exposure * 1000);
					double tmp_f = 1.0;
					if (ms_spacing_int > 10) {
						while (ms_spacing_int > 10) {
							ms_spacing_int /= 10;
							tmp_f *= 10;
						}
					}
					else if (ms_spacing_int < 1) {
						while (ms_spacing_int < 1) {
							tmp_f /= 10;
							ms_spacing_int = (int)(numMovFrames*SM_exposure * 1000 / tmp_f);
						}
					}
					if (ms_spacing_int >= 5) 
						inset_num_tics = ms_spacing_int*2;
					else
						inset_num_tics = ms_spacing_int * 5;
					inset_ms_spacing = ms_spacing_int * tmp_f / inset_num_tics;
					inset_tic_spacing = inset_dx * inset_ms_spacing / (numMovFrames*SM_exposure * 1000);
					inset_num_tics = (int)(inset_dx / inset_tic_spacing);
					for (i = 0; i <= inset_num_tics; i++) {
						if (ms_spacing_int >= 5) {
							if (i % 2)
								tic_size = 5;
							else
								tic_size = 8;
						}
						else {
							if (i % 5)
								tic_size = 5;
							else
								tic_size = 8;
						}
						axis_y = inset_y + inset_dy + inset_bd;
						axis_x = inset_x;
						VLine(axis_y, axis_y + tic_size, (int)(axis_x + inset_tic_spacing * i), colorGrayText);
					}
				}
				if (!mov_multi_flag) {
					int for_tiff;
					if (saveTiff_flag == 2)
						for_tiff = 1;
					else
						for_tiff = 0;
					draw_VLine(inset_y, inset_y + inset_dy, cur_fr_x, WHITE, for_tiff);
					if (hotTab == SM_CARDIOVIEW) {
						x1 = (int)(inset_x + 1.0*actv_start*t_dx + 0.5);
						x2 = (int)(inset_x + 1.0*actv_end*t_dx + 0.5);
						draw_VLine(inset_y, inset_y + inset_dy, x1, BLUE, for_tiff);
						draw_VLine(inset_y, inset_y + inset_dy, x2, BLUE, for_tiff);
					}
				}
				else {
					x1 = (int)(inset_x + 1.0*mov_multi_f1*t_dx + 0.5);
					x2 = (int)(inset_x + 1.0*min((mov_multi_num_row* mov_multi_num_col*mov_multi_steps + mov_multi_f1), numMovFrames - 1)*t_dx + 0.5);
					HLine(x1, x2, inset_y + inset_dy - 1, RED);
					HLine(x1, x2, inset_y + inset_dy, RED);
					HLine(x1, x2, inset_y + inset_dy + 1, RED);
				}
			}
		}
	}
	else if (paintFlag == PAINTTEXT) {
		char mssg[200];

		if (overlay_mode < 2) {
			if (!mov_multi_flag) {
				if (hotTab == SM_CARDIOVIEW) {
					sprintf(mssg, "Frame %d", curMovFrame + 1);
					drawText(0, 0, mssg, 20, y0 + 45, TEXT_RED, omFontMedium, TRUE);
					sprintf(mssg, "Membrane Potential");
					drawText(0, 0, mssg, 15, y0 + 20, TEXT_YELLOW, omFontBig, TRUE);
					if (actv_flag) {
						sprintf(mssg, "Activation");
						drawText(0, 0, mssg, 15, y0 + dy / 2 + 5, TEXT_YELLOW, omFontBig, TRUE);
					}
					if (apd_flag) {
						if (apd_mode)
							sprintf(mssg, "APD80");
						else
							sprintf(mssg, "APD50");
						drawText(0, 0, mssg, 15 + actv_flag * OM[SM_CARDIOWIN].dx / 3, y0 + dy / 2 + 5, TEXT_YELLOW, omFontBig, TRUE);
					}
					if (cv_flag) {
						sprintf(mssg, "Conduction");
						drawText(0, 0, mssg, 15 + (actv_flag + apd_flag) * OM[SM_CARDIOWIN].dx / 3, y0 + dy / 2 + 5, TEXT_YELLOW, omFontBig, TRUE);
					}
				}
				else {
					sprintf(mssg, "Frame %d", curMovFrame + 1);
					drawText(0, 0, mssg, 30, y0 + 30, TEXT_RED, omFontMedium, TRUE);
				}
			}
			else {
				for (j = 0; j < mov_multi_num_row; j++) {
					for (i = 0; i < mov_multi_num_col; i++) {
						if (mov_multi_f1 + (j* mov_multi_num_col + i)*mov_multi_steps < numMovFrames) {
							mov_img_x0 = multi_dx * i;
							mov_img_y0 = multi_dy * j;
							sprintf(mssg, "Frame %d", mov_multi_f1 + (j* mov_multi_num_col + i)*mov_multi_steps + 1);
							drawText(0, 0, mssg, mov_img_x0 + 20, mov_img_y0 + 20, TEXT_RED, omFontMedium, TRUE);
						}
					}
				}
			}
			if (inset_trace_cnt) {
				for (i = 0; i <= inset_num_tics; i++) {
					int axis_y = inset_y + inset_dy + inset_bd;
					sprintf(mssg, "%2.1lf", inset_ms_spacing*i / 1000);
					drawText(0, 0, mssg, (int)(inset_x + i * inset_tic_spacing - 10), axis_y + 10, colorGrayText, omFontMedium, TRUE);
				}
			}
		}
	}
	mov_multi_flag = multi_flag_sv;
}

static int seq_dx, seq_stt_x, seq_stt_y, seq_y_step, seq_click_pt;
void reset_seqZoom() {
	seq_zoom_f = 1.0;
	seq_zoom_x = 0;
}

void set_seqZoom(int zoom_inc, int mouseX)
{
	double x_step, x_step0;
	double seq_zoom_f0 = seq_zoom_f;
	int seq_zoom_x0 = seq_zoom_x;

	seq_zoom_f += zoom_inc*0.1;
	seq_zoom_f = max(1.0, seq_zoom_f);

	x_step = seq_zoom_f * seq_dx / num_seq_pnts;
	x_step0 = seq_zoom_f0 * seq_dx / num_seq_pnts;
	seq_zoom_x = (int)((mouseX - seq_stt_x) / x_step0 + seq_zoom_x0 - (mouseX - seq_stt_x)/ x_step) - 1;
	seq_zoom_x = min(max(0, seq_zoom_x), int(num_seq_pnts - seq_dx / x_step));
	seq_click_x = (int)((seq_click_pt - seq_zoom_x)*x_step + seq_stt_x + 0.5);
	paintWin(curTab(), PAINTINIT);
}

void shiftSeq(int mouseX) {
	double x_step;
	x_step = seq_zoom_f * seq_dx / num_seq_pnts;
	seq_zoom_x -= (int)((mouseX - seq_click_x) / x_step + 0.5);
	seq_zoom_x = min(max(0, seq_zoom_x), int(num_seq_pnts - seq_dx/ x_step + 0.5));
	paintWin(curTab(), PAINTINIT);
}

void drawSeqThumb(int mouseX)
{
	int x0, y0, dy0;
	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	dy0 = image_region_dy;

	LockGraphics();
	if (mouseX > x0 + seq_stt_x)
		VLine(y0 + 5, y0 + dy0 - 40, mouseX, RED);
	UnlockGraphics();
}

void setSeqExp(int mouseX1, int mouseX2)
{
	double x_step;
	int x1, x2;

	x_step = seq_zoom_f * seq_dx / num_seq_pnts;
	x1 = (int)((mouseX1 - seq_stt_x) / x_step + 0.5) + seq_zoom_x;
	x2 = (int)((mouseX2 - seq_stt_x) / x_step + 0.5) + seq_zoom_x;
	if (x1 < x2) {
		seq_zoom_x = x1;
		x_step = seq_dx / (x2 - x1);
	}
	else if (x1 > x2) {
		seq_zoom_x = x2;
		x_step = -seq_dx / (x2 - x1);
	}
	seq_zoom_f = x_step * num_seq_pnts / seq_dx;
	paintWin(curTab(), PAINTINIT);
}

int edit_seq_pt, edit_saved=0;
bool seq_bitAr_save[2048][24];
int func_pat_seq_save[100];
int seq_pt_cnt_save;
void editSequence(int mouseX, int mouseY, int flag)
{
	int x, y, x0, y0, dy, i, k, x1, x2, new_vlaue, pat_k;
//	char mssg[256];

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	dy = image_region_dy;
	if (mouseX < seq_stt_x || mouseY < y0 || mouseX > seq_stt_x + seq_dx || mouseY > y0 + dy)
		return;

	memcpy(seq_bitAr_save, seq_bitAr, sizeof(seq_bitAr_save));
	memcpy(func_pat_seq_save, func_pat_seq, sizeof(func_pat_seq_save));
	seq_pt_cnt_save = seq_pt_cnt;
	edit_saved = 1;
	double x_step = seq_zoom_f * seq_dx / num_seq_pnts;

	if (mouseY < seq_stt_y)
		y = 0;
	else
		y = max(0, (mouseY - seq_stt_y) / seq_y_step + 1);
	x = (int)((mouseX - seq_stt_x) / x_step) + seq_zoom_x;
	if (flag) {
		seq_bitAr[x][y] = !seq_bitAr[x][y];
	}
	else {
		for (k = 0; k < sizeof(pat_ar) / sizeof(pat_ar[0]); k++) {
			if (x < func_pat_seq[k]) {
				break;
			}
		}

		x1 = func_pat_seq[k - 1];
		x2 = func_pat_seq[k] - 1;
		for (i = x - 1; i >= func_pat_seq[k - 1]; i--) {
			if (seq_bitAr[i][y] != seq_bitAr[x][y]) {
				x1 = i;
				break;
			}
		}
		for (i = x + 1; i < func_pat_seq[k]; i++) {
			if (seq_bitAr[i][y] != seq_bitAr[x][y]) {
				x2 = i;
				break;
			}
		}
		new_vlaue = !seq_bitAr[x][y];
		if (x - x1 < x2 - x) {
			for (i = x1; i <= x; i++)
				seq_bitAr[i][y] = new_vlaue;
		}
		else {
			for (i = x; i <= x2; i++)
				seq_bitAr[i][y] = new_vlaue;
		}
	}

	int this_click_pt = (int)((mouseX - seq_stt_x) / x_step + 0.5) + seq_zoom_x - 1;
	char this_pat[128];
	pat_k = pat_cnt - 1;
	for (k = 0; k < pat_cnt; k++) {
		if (this_click_pt < func_pat_seq[k]) {
			pat_k = k - 1;
			strcpy(this_pat, pat_ar[pat_k]);
			break;
		}
	}
	for (k = pat_k+1; k < pat_cnt; k++) {
		if (!strcmp(pat_ar[k], this_pat)) {
			for (int m = 0; m < func_pat_seq[k + 1] - func_pat_seq[k]; m++)
				seq_bitAr[func_pat_seq[k] + m][y] = seq_bitAr[func_pat_seq[pat_k] + m][y];
		}
	}
}

void ExpandSeq(int mouseX, int mouseY) {
	int i, k, y, pat_click = -1, pat_k = -1, pat_k2, pat_k_end, this_bit_cnt, added_pnt, added_pat, exp_index;
	double x_step = seq_zoom_f * seq_dx / num_seq_pnts;
	int this_pt = (int)((mouseX - seq_stt_x) / x_step + 0.5) + seq_zoom_x;

	for (k = 0; k < pat_cnt; k++) {
		if (this_pt < func_pat_seq[k]) {
			pat_click = k - 1;
			break;
		}
	}
	if (pat_click < 0)
		return;
	pat_k = pat_click;
	if (mouseY < 15) {	//Subr
		exp_index = 0;
		if (!strcmp(subr_ar[pat_k], "") || seq_reps_ar[exp_index][pat_k]==1)
			return;
		for (k = pat_k - 1; k >0; k--) {
			if (strcmp(subr_ar[k], subr_ar[k+1])) {
				pat_k = k + 1;
				break;
			}
		}
		for (k = pat_k + 1; k < pat_cnt; k++) {
			if (strcmp(subr_ar[k], subr_ar[k - 1]) || seq_reps_ar[exp_index][k] == -1) {
				pat_k_end = k;
				this_bit_cnt = func_pat_seq[pat_k_end] - func_pat_seq[pat_k];
				break;
			}
		}
	}
	else if (mouseY < 40) {	//Func
		exp_index = 1;
		if (!strcmp(func_ar[pat_k], "") || seq_reps_ar[exp_index][pat_k] == 1)
			return;
		for (k = pat_k - 1; k >0; k--) {
			if (strcmp(func_ar[k], func_ar[k + 1])) {
				pat_k = k + 1;
				break;
			}
		}
		for (k = pat_k + 1; k < pat_cnt; k++) {
			if (strcmp(func_ar[k], func_ar[k - 1]) || seq_reps_ar[exp_index][k] == -1) {
				pat_k_end = k;
				this_bit_cnt = func_pat_seq[pat_k_end] - func_pat_seq[pat_k];
				break;
			}
		}
	}
	else {	//Pat
		pat_k_end = pat_k + 1;
		this_bit_cnt = func_pat_seq[pat_k_end] - func_pat_seq[pat_k];
		exp_index = 2;
	}
	if (seq_reps_ar[exp_index][pat_click] == -1) {
		added_pnt = 0;
		added_pat = 0;
		char str1[128], str2[128];
		for (k = pat_click - 1; k > 0; k--) {
			if (seq_reps_ar[exp_index][k] != -1) {
				pat_k2 = k + 1;
				if (exp_index == 0) {
					strcpy(str1, subr_ar[pat_k2 - 1]);
					strcpy(str2, subr_ar[pat_k2]);
				}
				else if (exp_index == 1) {
					strcpy(str1, func_ar[pat_k2 - 1]);
					strcpy(str2, func_ar[pat_k2]);
				}
				else {
					strcpy(str1, pat_ar[pat_k2 - 1]);
					strcpy(str2, pat_ar[pat_k2]);
				}
				if (strcmp(str1, str2)) {
					pat_k2++;
				}
				break;
			}
		}
		for (k = pat_k2 + 1; k < pat_cnt; k++) {
			if (seq_reps_ar[exp_index][k] != -1) {
				pat_k_end = k;
				added_pnt = func_pat_seq[pat_k_end] - func_pat_seq[pat_k2];
				added_pat = pat_k_end - pat_k2;
				break;
			}
		}
		pat_cnt -= added_pat;
		for (k = pat_k2; k < pat_cnt; k++) {
			strcpy(subr_ar[k], subr_ar[k + added_pat]);
			strcpy(func_ar[k], func_ar[k + added_pat]);
			strcpy(pat_ar[k], pat_ar[k + added_pat]);
			func_pat_seq[k + 1] = func_pat_seq[k + 1 + added_pat] - added_pnt;
			for (i = 0; i < 3; i++)
				seq_reps_ar[i][k] = seq_reps_ar[i][k + added_pat];
		}
		seq_reps_ar[exp_index][pat_k2-1] = (pat_k_end - pat_k2) - seq_reps_ar[exp_index][pat_k2 - 1];
		seq_pt_cnt -= added_pnt;
		for (y = 0; y < num_bit_line; y++) {
			for (i = func_pat_seq[pat_k]; i < seq_pt_cnt; i++)
				seq_bitAr[i][y] = seq_bitAr[i + added_pnt][y];
		}
	}
	else if (abs(seq_reps_ar[exp_index][pat_k]) > 1) {
		added_pat = pat_k_end - pat_k;
		pat_cnt += added_pat;
		added_pnt = this_bit_cnt;
		for (k = pat_cnt - 1; k >= pat_k_end; k--) {
			strcpy(subr_ar[k], subr_ar[k - added_pat]);
			strcpy(func_ar[k], func_ar[k - added_pat]);
			strcpy(pat_ar[k], pat_ar[k - added_pat]);
			func_pat_seq[k] = func_pat_seq[k - added_pat] + added_pnt;
			for (i = 0; i < 3; i++)
				seq_reps_ar[i][k] = seq_reps_ar[i][k - added_pat];
		}
		for (k = pat_k; k < pat_k_end; k++) {
			seq_reps_ar[exp_index][k + added_pat] = -1;
			seq_reps_ar[exp_index][k] = 1 - abs(seq_reps_ar[exp_index][k]);
		}
		seq_pt_cnt += added_pnt;
		for (y = 0; y < num_bit_line; y++) {
			for (i = seq_pt_cnt - 1; i >= func_pat_seq[pat_k] + added_pnt; i--)
				seq_bitAr[i][y] = seq_bitAr[i - added_pnt][y];
		}
	}
}

void AddDeleteSeqPnt(int addFlag) {
	int i, y, k, pat_k=-1, m;

	memcpy(seq_bitAr_save, seq_bitAr, sizeof(seq_bitAr_save));
	memcpy(func_pat_seq_save, func_pat_seq, sizeof(func_pat_seq_save));
	seq_pt_cnt_save = seq_pt_cnt;
	edit_saved = 1;
	for (k = 0; k < pat_cnt; k++) {
		if (seq_click_pt-1 < func_pat_seq[k]) {
			pat_k = k;
			break;
		}
	}
	if (pat_k < 0)
		return;
	if (addFlag) {
		for (y = 0; y < num_bit_line; y++) {
			for (i = seq_pt_cnt; i > seq_click_pt-1; i--)
				seq_bitAr[i][y] = seq_bitAr[i - 1][y];
		}
		seq_pt_cnt++;
		for (k = pat_k; k < pat_cnt; k++) {
			if (k && func_pat_seq[k] == 0)
				break;
			func_pat_seq[k]++;
		}
		add_deleteSeqLine(addFlag, pat_ar[pat_k-1]);
	}
	else {
		seq_pt_cnt--;
		for (k = pat_k; k < sizeof(pat_ar) / sizeof(pat_ar[0]); k++) {
			if (k && func_pat_seq[k] == 0)
				break;
			func_pat_seq[k]--;
		}
		add_deleteSeqLine(addFlag, pat_ar[pat_k-1]);
		for (y = 0; y < num_bit_line; y++) {
			for (i = seq_click_pt-1; i < seq_pt_cnt; i++)
				seq_bitAr[i][y] = seq_bitAr[i + 1][y];
		}
	}
	int this_click_pt = seq_click_pt - 1;
	char this_pat[128];
	for (k = 0; k < pat_cnt; k++) {
		if (this_click_pt < func_pat_seq[k]) {
			pat_k = k - 1;
			strcpy(this_pat, pat_ar[pat_k]);
			break;
		}
	}
	for (k = pat_k + 1; k < pat_cnt; k++) {
		if (!strcmp(pat_ar[k], this_pat)) {
			for (m = 0; m < func_pat_seq[k + 1] - func_pat_seq[k]; m++)
				seq_bitAr[func_pat_seq[k] + m][y] = seq_bitAr[func_pat_seq[pat_k] + m][y];
			if (addFlag) {
				for (m = k; m < pat_cnt; m++)
					func_pat_seq[m+1]++;
			}
			else {
				for (m = k; m < pat_cnt; m++) 
					func_pat_seq[m+1]--;
			}
		}
	}

}

void undoSeqEdit() {
	if (edit_saved) {
		memcpy(seq_bitAr, seq_bitAr_save, sizeof(seq_bitAr_save));
		memcpy(func_pat_seq, func_pat_seq_save, sizeof(func_pat_seq_save));
		seq_pt_cnt = seq_pt_cnt_save;
		edit_saved = 0;
	}
}

static int newSeqRep;
bool
CALLBACK SetSeqRep(HWND hDlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	int win_dx, win_dy;
	char tmp_str[50];

	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			GetDlgItemText(hDlg, SEQ_REP_EDIT, tmp_str, sizeof(tmp_str));
			newSeqRep = max(1, atoi(tmp_str));
			EndDialog(hDlg, true);
			return true;
		}
		break;
	case WM_CREATE:
		// do initialization stuff here
		return(0);
		break;
	case WM_PAINT:
		GetWindowRect(hDlg, &rect);
		win_dx = rect.right - rect.left;
		win_dy = rect.bottom - rect.top;
		MoveWindow(hDlg, (screen_width - win_dx) / 2, (screen_height - win_dy) / 2, win_dx, win_dy, true);
		sprintf(tmp_str, "%d", newSeqRep);
		SetDlgItemText(hDlg, SEQ_REP_EDIT, tmp_str);
		break;
	}
	return FALSE;
}

int
getNewSeqRep(void)
{
	return (int)DialogBox(main_instance, TEXT("SET_SEQ_REP"), main_window_handle, (DLGPROC)SetSeqRep);
}

void setNewReps(int mouseX)
{
	int x0, y0, x;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();

	if (mouseX < seq_stt_x || mouseX > seq_stt_x + seq_dx)
		return;

	double x_step = seq_zoom_f * seq_dx / num_seq_pnts;
	x = (int)((mouseX - seq_stt_x) / x_step) + seq_zoom_x;

	for (int k = 0; k < sizeof(pat_ar) / sizeof(pat_ar[0]); k++) {
		if (x < func_pat_seq[k]) {
			newSeqRep = seq_reps_ar[2][k - 1];
			getNewSeqRep();
			seq_reps_ar[2][k - 1] = newSeqRep;
			break;
		}
	}
}

void UpdateMScale() {
	char tmp_str[256], tmp_str2[256];

	if (disp_setting_handle) {
		if (autoScaleFlag) {
			sprintf(tmp_str, "%d", auto_skip_v1);
			sprintf(tmp_str2, "%d", auto_skip_v2);
			SetDlgItemText(disp_setting_handle, AUTO_SKIP_LABEL, "Auto Scale Clipping");
			SetDlgItemText(disp_setting_handle, AUTO_SKIP_LABEL2, "(to exclude hot pixels)");
		}
		else {
			sprintf(tmp_str, "%d", scale_max);
			sprintf(tmp_str2, "%d", scale_min);
			SetDlgItemText(disp_setting_handle, AUTO_SKIP_LABEL, "Manual Scale Range");
			SetDlgItemText(disp_setting_handle, AUTO_SKIP_LABEL2, "");
		}
		SetDlgItemText(disp_setting_handle, AUTO_SKIP, tmp_str);
		SetDlgItemText(disp_setting_handle, AUTO_SKIP2, tmp_str2);
	}
}

static int lineProfileAr[2048];
static int lp_y_offset = 0, lp_x_offset = 0, lp_dy = 0, lp_dx = 0;
static int lineP_min = 0, lineP_max = 0;
static int pulse_ar[2][24];
static int live_acq_f_cnt = 0;
void DisplayFrame(int paintFlag)
{
	int x, dx, dy, y, x0, y0, dx0, dy0;

	double pix_scale;
	float *image_ptr;

	x0 = getMeasureWinX0();
	dx0 = getMeasureWinX();
	y0 = getMeasureWinY0();
	dy0 = image_region_dy;

	if (seqFlag) { // display sequence waveforms
		int i, k, m, x1, x2;
		int dx0 = getMeasureWinX(); 
		char seq_action_name[][10] = {"send SOF", "latch", "read", "100", "000", "null"};
		char seq_action_code[][5] = {"001", "011", "101", "100", "000", "111"};
		char action_str[10], action_str2[10], mssg[256];
		int seq_colorAr[] = {RED, GREEN, YELLOW, BLUE, WHITE, MIDGRAY};
		int bdr = 5, i0, color;
		unsigned int bit_char_len=0;
		double x_step;

		dataExist = 0;
		num_seq_pnts = seq_pt_cnt;
		for (k = 0; k < sizeof(bitN_ar) / sizeof(bitN_ar[0]); k++) {
			if (bit_char_len < (int)strlen(bitN_ar[k]))
				bit_char_len = (int)strlen(bitN_ar[k]);
		}
		seq_x_lb = bit_char_len * 9;
		seq_dx = (dx0 - seq_x_lb - 15);
		seq_stt_x = x0 + seq_x_lb + bdr;
		x_step = seq_zoom_f * seq_dx / num_seq_pnts;
		seq_stt_y = y0 + bdr + 150;
		seq_y_step = (dy0 - bdr * 2 - 180) / num_bit_line;

		if (paintFlag == PAINTGRFX) {
			int the_base;
			seq_click_pt = (int)((seq_click_x - seq_stt_x) / x_step + 0.5) + seq_zoom_x;
			for (k = 0; k < sizeof(pat_ar) / sizeof(pat_ar[0]); k++) {
				if (func_pat_seq[k] < seq_zoom_x)
					continue;
				if (((k > 0) && func_pat_seq[k]<func_pat_seq[k - 1]) || func_pat_seq[k] > seq_zoom_x + num_seq_pnts)
					break;
				if (k > 0) {
					if ((seq_reps_ar[2][k] <= -1 && seq_reps_ar[2][k - 1] > 0) || (seq_reps_ar[2][k] > 0 && seq_reps_ar[2][k - 1] <= -1))
						color = PURPLE;
					else if ((seq_reps_ar[1][k] <= -1 && seq_reps_ar[1][k - 1] >= 0) || (seq_reps_ar[1][k] >= 0 && seq_reps_ar[1][k - 1] <= -1))
						color = LTGRAY;
					else if (strcmp(subr_ar[k], subr_ar[k - 1]))
						color = BLUE;
					else if (strcmp(func_ar[k], func_ar[k - 1]))
						color = MIDGRAY;
					else 
						color = DKGRAY;
				}
				else
					color = DKGRAY;
				x1 = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 0.5);
				if ((x1 >= seq_stt_x) && (x1 < seq_stt_x + seq_dx))
					VLine(y0 + bdr, seq_stt_y + seq_y_step * num_bit_line, x1, color);

				// draw subr, func, pat lines
				if (k > 0) {
					if (func_pat_seq[k - 1] < seq_zoom_x) {
						x1 = seq_stt_x;
						if (func_pat_seq[k])
							x2 = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 0.5);
						else
							x2 = (int)(seq_stt_x + (num_seq_pnts - seq_zoom_x)*x_step + 0.5);
						if ((x2 > seq_stt_x) && (x1 < seq_stt_x + seq_dx)) {
							x2 = min(x2, seq_stt_x + seq_dx);
							the_base = y0 + 5;
							if (strcmp(subr_ar[k-1], "")) {
								HLine(x1, x2, the_base, BLUE);
								HLine(x1, x2, the_base + 1, BLUE);
							}
							the_base += 25;
							if (strcmp(func_ar[k-1], "")) {
								if (seq_reps_ar[1][k] < 0) {
									HLine(x1, x2, the_base, MIDGRAY);
									if (seq_reps_ar[1][k] < -1)
										HLine(x1, x2, the_base + 1, MIDGRAY);
								}
								else {
									HLine(x1, x2, the_base, LTGRAY);
									if (seq_reps_ar[1][k] > 1)
										HLine(x1, x2, the_base + 1, LTGRAY);
								}
							}
							the_base += 25;
							if (strcmp(pat_ar[k-1], "")) {
								if (seq_reps_ar[2][k] < 0) {
									HLine(x1, x2, the_base, PURPLE);
									if (seq_reps_ar[2][k] < -1)
										HLine(x1, x2, the_base + 1, PURPLE);
								}
								else {
									HLine(x1, x2, the_base, PINK);
									if (seq_reps_ar[2][k] > 1)
										HLine(x1, x2, the_base + 1, PINK);
								}
							}
						}
					}
					x1 = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 0.5);
					if (func_pat_seq[k + 1])
						x2 = (int)(seq_stt_x + (func_pat_seq[k + 1] - seq_zoom_x)*x_step + 0.5);
					else
						x2 = (int)(seq_stt_x + (num_seq_pnts - seq_zoom_x)*x_step + 0.5);
					if ((x2 > seq_stt_x) && (x1 < seq_stt_x + seq_dx)) {
						x1 = max(x1, seq_stt_x);
						x2 = min(x2, seq_stt_x + seq_dx);
						the_base = y0 + 5;
						if (strcmp(subr_ar[k], "")) {
							HLine(x1, x2, the_base, BLUE);
							HLine(x1, x2, the_base + 1, BLUE);
						}
						the_base += 25;
						if (strcmp(func_ar[k], "")) {
							if (seq_reps_ar[1][k] < 0) {
								HLine(x1, x2, the_base, MIDGRAY);
								if (seq_reps_ar[1][k] < -1)
									HLine(x1, x2, the_base + 1, MIDGRAY);
							}
							else {
								HLine(x1, x2, the_base, LTGRAY);
								if (seq_reps_ar[1][k] > 1)
									HLine(x1, x2, the_base + 1, LTGRAY);
							}
						}
						the_base += 25;
						if (strcmp(pat_ar[k], "")) {
							if (seq_reps_ar[2][k] < 0) {
								HLine(x1, x2, the_base, PURPLE);
								if (seq_reps_ar[2][k] < -1)
									HLine(x1, x2, the_base + 1, PURPLE);
							}
							else {
								HLine(x1, x2, the_base, PINK);
								if (seq_reps_ar[2][k] > 1)
									HLine(x1, x2, the_base + 1, PINK);
							}
						}
					}
				}
				else {
					x1 = seq_stt_x;
					x2 = (int)(seq_stt_x + (func_pat_seq[k + 1] - seq_zoom_x)*x_step + 0.5);
					if ((x2 > seq_stt_x) && (x1 < seq_stt_x + seq_dx)) {
						x1 = max(x1, seq_stt_x);
						x2 = min(x2, seq_stt_x + seq_dx);
						the_base = y0 + 5;
						if (strcmp(subr_ar[k], "")) {
							HLine(x1, x2, the_base, BLUE);
							HLine(x1, x2, the_base + 1, BLUE);
						}
						the_base += 25;
						if (strcmp(func_ar[k], "")) {
							HLine(x1, x2, the_base, LTGRAY);
							HLine(x1, x2, the_base + 1, LTGRAY);
						}
						the_base += 25;
						if (strcmp(pat_ar[k], "")) {
							if (seq_reps_ar[2][k] < 0)
								HLine(x1, x2, the_base, PINK2);
							else {
								HLine(x1, x2, the_base, PINK);
								if (seq_reps_ar[2][k] > 1)
									HLine(x1, x2, the_base + 1, PINK);
							}
						}
					}
				}
			}

			if ((seq_click_x >= seq_stt_x) && (seq_click_x < seq_stt_x + seq_dx))
				VLine(y0 + bdr, seq_stt_y + seq_y_step * num_bit_line -10, seq_click_x, ORANGE);
			y = seq_stt_y;
			memset(pulse_ar, 0, sizeof(pulse_ar));
			int pat_id=-1, cropped, upper_crop;
			for (i = 0; i < sizeof(pat_ar) / sizeof(pat_ar[0]); i++) {
				if (func_pat_seq[i] > seq_click_pt) {
					pat_id = i - 1;
					break;
				}
			}
			for (k = 0; k < num_bit_line; k++) {
				if (seq_click_pt > 0 && pat_id>=0) {
					cropped = 0;
					for (m = seq_click_pt; m >= 0; m--) {
						if (seq_bitAr[m][k] != seq_bitAr[seq_click_pt][k]) {
							pulse_ar[0][k] = m + 1;
							break;
						}
						if (m == 0)
							cropped = 1;
					}
					if (pulse_ar[0][k] < func_pat_seq[pat_id]) {
						pulse_ar[0][k] = func_pat_seq[pat_id];
						cropped = 1;
					}
					for (m = seq_click_pt; m < num_seq_pnts; m++)
						if (seq_bitAr[m][k] != seq_bitAr[seq_click_pt][k]) {
							pulse_ar[1][k] = m - 1;
							break;
						}
					if (!func_pat_seq[pat_id + 1])
						upper_crop = num_seq_pnts - 1;
					else
						upper_crop = func_pat_seq[pat_id + 1] - 1;
					if (pulse_ar[1][k] > upper_crop) {
						if (!cropped)
							pulse_ar[1][k] = upper_crop;
						else
							pulse_ar[1][k] = 0;
					}
				}
				color = seq_colorAr[k % 5];
				for (i = 0; i < num_seq_pnts - 1; i++) {
					if (strstr(bitN_ar[0], "CMD") && strstr(bitN_ar[1], "CMD") && strstr(bitN_ar[2], "CMD") && (k<3))
						color = DKGRAY;
					i0 = i;
					while (seq_bitAr[i + 1 + seq_zoom_x][k] == seq_bitAr[i + seq_zoom_x][k] && i < min(sizeof(seq_bitAr) / sizeof(seq_bitAr[0])-2, num_seq_pnts - 1))
						i++;
					x1 = (int)(seq_stt_x + i0 * x_step + 0.5);
					x2 = (int)(seq_stt_x + (i + 1)*x_step + 0.5);
					if ((x2 > seq_stt_x) && (x1 < seq_stt_x + seq_dx)) {
						x1 = max(x1, seq_stt_x);
						x2 = min(x2, seq_stt_x + seq_dx);
						HLine(x1, x2, (int)(y - 0.6*seq_y_step*seq_bitAr[i0 + seq_zoom_x][k] + 0.5), color);
						if (x_step > 2) {
							for (int m = 0; m <= (int)((x2 - x1) / x_step + 0.5); m++) {
								int thix_x = x1 + (int)(m*x_step + 0.5);
								HLine(thix_x, thix_x, (int)(y - 0.6*seq_y_step*(!seq_bitAr[i0 + seq_zoom_x][k]) + 0.5), MIDGRAY);
							}
						}
						if (x1 > seq_stt_x)
							VLine((int)(y - 0.6*seq_y_step*seq_bitAr[i0 + seq_zoom_x][k] + 0.5), y, x1, color);
						if (x2 < seq_stt_x + seq_dx)
							VLine((int)(y - 0.6*seq_y_step*seq_bitAr[i0 + seq_zoom_x][k] + 0.5), y, x2, color);
					}
				}
				y += seq_y_step;
			}
			y = seq_stt_y + seq_y_step;
			if (strstr(bitN_ar[0], "CMD") && strstr(bitN_ar[1], "CMD") && strstr(bitN_ar[2], "CMD")) {
				int cmd_id;
				for (i = seq_zoom_x; i < num_seq_pnts - 1; i++) {
					cmd_id = seq_bitAr[i][0] + seq_bitAr[i][1] * 2 + seq_bitAr[i][2] * 4;
					if (cmd_id > 0 && cmd_id < 7) {
						x1 = (int)(seq_stt_x + (i - seq_zoom_x) * x_step + 0.5);
						x2 = (int)(seq_stt_x + (i - seq_zoom_x + 1)*x_step + 0.5);
						if ((x2 > seq_stt_x) && (x1 < seq_stt_x + seq_dx)) {
							x1 = max(x1, seq_stt_x);
							x2 = min(x2, seq_stt_x + seq_dx);
							D3DRectangle(x1, y, x2, y + seq_y_step, seq_colorAr[cmd_id - 1]);
							VLine(y, y + seq_y_step, x1, DKGRAY);
						}
					}
				}
			}
		}
		else if (paintFlag == PAINTTEXT) {
			int this_bit_cnt, pat_bit_cnt;
			int pat_lb_cnt = 0;
			int cur_subr_cnt, cur_func_cnt;
			for (k = 0; k < sizeof(pat_ar) / sizeof(pat_ar[0]); k++) {
				if (!k) {
					this_bit_cnt = func_pat_seq[k];
					pat_bit_cnt = func_pat_seq[k+1];
					cur_subr_cnt = 0;
					cur_func_cnt = 0;
				}
				else {
					this_bit_cnt += (func_pat_seq[k] - func_pat_seq[k - 1])*abs(seq_reps_ar[2][k - 1]);
					cur_func_cnt += (func_pat_seq[k] - func_pat_seq[k - 1])*abs(seq_reps_ar[2][k - 1]);
					cur_subr_cnt += (func_pat_seq[k] - func_pat_seq[k - 1])*abs(seq_reps_ar[2][k - 1]);
					if (func_pat_seq[k + 1] > func_pat_seq[k])
						pat_bit_cnt = func_pat_seq[k+1] - func_pat_seq[k];
					else
						pat_bit_cnt = num_seq_pnts - func_pat_seq[k];
					if (strcmp(func_ar[k], func_ar[k - 1])) {
						this_bit_cnt += cur_func_cnt * max(0, (abs(seq_reps_ar[1][k - 1]) - 1));
						cur_subr_cnt += cur_func_cnt * max(0, (abs(seq_reps_ar[1][k - 1]) - 1));
						cur_func_cnt = 0;
					}
					else if (seq_reps_ar[1][k - 1] < -1 && seq_reps_ar[1][k] == -1) {
						this_bit_cnt += cur_func_cnt * max(0, (abs(seq_reps_ar[1][k - 1]) - 1));
						cur_subr_cnt += cur_func_cnt * max(0, (abs(seq_reps_ar[1][k - 1]) - 1));
						cur_func_cnt = 0;
					}
					if (strcmp(subr_ar[k], subr_ar[k - 1])) {
						this_bit_cnt += cur_subr_cnt * max(0, (abs(seq_reps_ar[0][k - 1]) - 1));
						cur_subr_cnt = 0;
					}
				}
				if (func_pat_seq[k] < seq_zoom_x)
					continue;
				if ((k > 0 && !func_pat_seq[k]) || func_pat_seq[k] > seq_zoom_x + num_seq_pnts)
					break;
				if (strstr(bitN_ar[0], "CMD") && strstr(bitN_ar[1], "CMD") && strstr(bitN_ar[2], "CMD"))
					y = seq_stt_y + bdr - 15;
				else
					y = seq_stt_y + bdr - seq_y_step - 15;
				x1 = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 0.5);
				sprintf(mssg, "%d (%d)", abs(seq_reps_ar[2][k]), pat_bit_cnt);
				if (func_pat_seq[k + 1] > func_pat_seq[k])
					x = x1 + (int)((func_pat_seq[k + 1] - func_pat_seq[k])*x_step / 2 - 3 * strlen(mssg) + 0.5);
				else
					x = x1 + (int)((num_seq_pnts - func_pat_seq[k])*x_step / 2 - 3 * strlen(mssg) + 0.5);
				if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
					drawText(0, 0, mssg, x, y, TEXT_YELLOW, omFontSmall, TRUE);
				y = seq_y_step * (num_bit_line - 1) + seq_stt_y + bdr + 5;
				sprintf(mssg, "%d", this_bit_cnt);
				x = x1 + 5;
				if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
					drawText(0, 0, mssg, x, y, WHITE, omFontSmall, TRUE);
				// subr, func names
				if (k) {
					if (func_pat_seq[k - 1] < seq_zoom_x) {
						if (strcmp(subr_ar[k - 1], "")) {
							x = seq_stt_x;
							y = y0 + 8;
							color = BLUE;
							int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
							if (seq_reps_ar[0][k - 1] > 1)
								sprintf(mssg, "%s (%d)", subr_ar[k - 1], seq_reps_ar[0][k - 1]);
							else
								sprintf(mssg, "%s", subr_ar[k - 1]);
							if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
								drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
						}
						if (strcmp(func_ar[k - 1], "")) {
							x = seq_stt_x;
							y = y0 + 33;
							int txt_color = LTGRAY2;
							if (seq_reps_ar[0][k - 1] > 1)
								sprintf(mssg, "%s (%d)", func_ar[k - 1], seq_reps_ar[1][k - 1]);
							else
								sprintf(mssg, "%s", func_ar[k - 1]);
							if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
								drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
						}
					}
					if (strcmp(subr_ar[k], subr_ar[k - 1]) && strcmp(subr_ar[k], "")) {
						x = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 5.5);
						y = y0 + 8;
						color = BLUE;
						int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
						if (seq_reps_ar[0][k] > 1)
							sprintf(mssg, "%s (%d)", subr_ar[k], seq_reps_ar[0][k]);
						else
							sprintf(mssg, "%s", subr_ar[k]);
						if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
							drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
					}
					if (strcmp(func_ar[k], func_ar[k - 1]) && strcmp(func_ar[k], "")) {
						x = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 5.5);
						y = y0 + 33;
						int txt_color = LTGRAY2;
						if (abs(seq_reps_ar[1][k]) > 1)
							sprintf(mssg, "%s (%d)", func_ar[k], abs(seq_reps_ar[1][k]));
						else
							sprintf(mssg, "%s", func_ar[k]);
						if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
							drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
					}
					if (strcmp(pat_ar[k], pat_ar[k - 1]) && strcmp(pat_ar[k], "")) {
						x = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 5.5);
						y = y0 + 58 + pat_lb_cnt*20;
						color = PINK;
						int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
						sprintf(mssg, "%s", pat_ar[k]);
						if ((x > 0) && (x < seq_stt_x + seq_dx - 7 * strlen(mssg)))
							drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
						pat_lb_cnt++;
						pat_lb_cnt = pat_lb_cnt % 4;
					}
				}
				else {
					if (strcmp(subr_ar[k], "")) {
						x = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 5.5);
						y = y0 + 8;
						color = BLUE;
						int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
						if (seq_reps_ar[0][k] > 1)
							sprintf(mssg, "%s (%d)", subr_ar[k], seq_reps_ar[0][k]);
						else
							sprintf(mssg, "%s", subr_ar[k]);
						if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
							drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
					}
					if (strcmp(func_ar[k], "")) {
						x = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 5.5);
						y = y0 + 33;
						int txt_color = LTGRAY2;
						if (abs(seq_reps_ar[1][k]) > 1)
							sprintf(mssg, "%s (%d)", func_ar[k], abs(seq_reps_ar[1][k]));
						else
							sprintf(mssg, "%s", func_ar[k]);
						if ((x > 0) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
							drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
					}
					if (strcmp(pat_ar[k], "")) {
						x = (int)(seq_stt_x + (func_pat_seq[k] - seq_zoom_x)*x_step + 5.5);
						y = y0 + 58 + pat_lb_cnt * 20;
						color = PINK;
						int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
						sprintf(mssg, "%s", pat_ar[k]);
						if ((x > 0) && (x < seq_stt_x + seq_dx - 7 * strlen(mssg)))
							drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
						pat_lb_cnt++;
						pat_lb_cnt = pat_lb_cnt % 4;
					}
				}
			}
			// Command lines
			if (strstr(bitN_ar[0], "CMD") && strstr(bitN_ar[1], "CMD") && strstr(bitN_ar[2], "CMD")) {
				char seq_cmd[][20] = { "", "SOF", "SOL", "LTCH", "SKIP", "READ", "READY", "" };
				y = seq_stt_y - 30 + seq_y_step;
				for (i = 1; i < 7; i++) {
					sprintf(mssg, "%s", seq_cmd[i]);
					x = seq_stt_x + i * 60;
					if ((x >= seq_stt_x) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg))) {
						color = seq_colorAr[i - 1];
						int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
						drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
					}
				}
			}
			//Label Click point
			for (int k = 0; k < sizeof(pat_ar) / sizeof(pat_ar[0]); k++) {
				if (seq_click_pt < func_pat_seq[k]) {
					x = seq_click_pt - func_pat_seq[k - 1] + this_bit_cnt;
					break;
				}
				if (!k)
					this_bit_cnt = func_pat_seq[k];
				else
					this_bit_cnt += (func_pat_seq[k] - func_pat_seq[k - 1])*abs(seq_reps_ar[2][k - 1]);
			}
			sprintf(mssg, "%d", x);
			x = seq_click_x + 3;
			if ((x >= seq_stt_x) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg))) {
				color = ORANGE;
				int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
				y = y0 + bdr + 3;
				drawText(0, 0, mssg, x, y, txt_color, omFontSmall, TRUE);
			}
			y = seq_stt_y + bdr - seq_y_step;
			for (k = 0; k < num_bit_line; k++) {
				color = seq_colorAr[k % 5];
				int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
				sprintf(mssg, "%s", bitN_ar[k]);
				drawText(0, 0, mssg, x0 + 3, y + seq_y_step / 2, txt_color, omFontSmall, TRUE);
				if (seq_click_pt > 0) {
					x = (int)(seq_stt_x + (pulse_ar[0][k]-seq_zoom_x) * x_step + 0.5);
					if (x > 0 && pulse_ar[1][k]) {
						sprintf(mssg, "%d", 0);
						if ((x >= seq_stt_x) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
							drawText(0, 0, mssg, x, int(y + 0.1*seq_y_step + 0.5), txt_color, omFontSmall, TRUE);
					}
					if (pulse_ar[1][k]) {
						x = (int)(seq_stt_x + (pulse_ar[1][k] - seq_zoom_x) * x_step + 0.5);
						if (x < x0 + dx0 && (pulse_ar[1][k] > pulse_ar[0][k])) {
							sprintf(mssg, "%d", pulse_ar[1][k] - pulse_ar[0][k] + 1);
							if ((x >= seq_stt_x) && (x < seq_stt_x + seq_dx - 4 * strlen(mssg)))
								drawText(0, 0, mssg, x, int(y + 0.1*seq_y_step+0.5), txt_color, omFontSmall, TRUE);
						}
					}
				}
				y += seq_y_step;
			}
			sprintf(mssg, "Mouse Wheel: Zoom, Right-Click: Reset zoom, Click: Show Pnt, Ctrl-Click: Flip to nearby edge, Shft-Click: Flip the point, Delete: Delete the point, Insert: Insert a point of same value, Ctrl-Right-Click: Change reps");
			drawText(0, 0, mssg, OM[SM_NEXTREAD].x + OM[SM_NEXTREAD].dx + 20, OM[SM_NEXTREAD].y - 25, WHITE, omFontMedium, TRUE);
			sprintf(mssg, "Program %d -- %s, Total Cycles: %6ld (=34*%5.4lf), Max Frame Rate: %4.3lf (*1.36 = %5.3lf)", curSeq + 1, seq_name, this_progCycles, (double)this_progCycles/34.0, (double)1.0e9 / (this_progCycles * 20), (double)1.0e9*1.36 / (this_progCycles * 20));
			drawText(0, 0, mssg, OM[SM_NEXTREAD].x + OM[SM_NEXTREAD].dx + 20, OM[SM_NEXTREAD].y, TEXT_YELLOW, omFontMedium, TRUE);
			y -= 10;
			for (int i = 0; i < num_seq_pnts - 1; i++) {
				i0 = i;
				sprintf(action_str, "%d%d%d", seq_bitAr[i + seq_zoom_x][21], seq_bitAr[i + seq_zoom_x][22], seq_bitAr[i + seq_zoom_x][23]);
				sprintf(action_str2, "%d%d%d", seq_bitAr[i + 1 + seq_zoom_x][21], seq_bitAr[i + 1 + seq_zoom_x][22], seq_bitAr[i + 1 + seq_zoom_x][23]);
				while (!strcmp(action_str, action_str2) && i < num_seq_pnts - 2) {
					i++;
					sprintf(action_str2, "%d%d%d", seq_bitAr[i + 1 + seq_zoom_x][21], seq_bitAr[i + 1 + seq_zoom_x][22], seq_bitAr[i + 1 + seq_zoom_x][23]);
				}
			}
		/*	for (k = 0; k < 6; k++) {
				color = seq_colorAr[k];
				int txt_color = ((color & 0xff) << 16) | (color & 0xff00) | ((color & 0xff0000) >> 16);
				sprintf(mssg, "%s - %s", seq_action_code[k], seq_action_name[k]);
				drawText(0, 0, mssg, x0 + 50 + k * 150, y + 10, txt_color, omFontSmall, TRUE);
			}*/
		}
		return;
	}

	if (!dataExist)
		return;

	int scale_max2, scale_min2;
	double pix_scale2 = 1.0;
	if (autoScaleFlag) {
		scale_max = int(max_pix+0.5);
		scale_min = int(min_pix+0.5);
		if (splitterMode) {
			scale_max2 = int(max_pix2+0.5);
			scale_min2 = int(min_pix2+0.5);
			pix_scale2 = 255.0 / (scale_max2 - scale_min2);
		}
	}
	pix_scale = 255.0 / (scale_max - scale_min);
	dx = image_region_dx;
	dy = image_region_dy;
	int num_colors = sizeof(rainbow_LUT) / sizeof(rainbow_LUT[0]);
	if (paintFlag == PAINTGRFX) {
		int l, y_pos;
		float pix_val;
		unsigned char val;
		int this_x, pixel_y;
		unsigned int this_col;
		double roi_transparency;
		int roi_color;
		int skip_this;
		double zoom_f_sv;
		int img_y0_sv;
		int chan_section, draw_x;

		if (!BNCfile_only) {
			if (hide_overscan_flag) {
				zoom_f_sv = zoom_factor;
				img_y0_sv = img_y0;
				chan_section = disp_num_col / stripes_lib[curConfig];
				zoom_factor *= chan_section * 1.0 / (chan_section - (overscan_x2 - overscan_x1 + 1));
				img_y0 = (int)(image_region_dy - disp_num_row * zoom_factor + 0.5) / 2;
			}
			memset(ImageLine, 0, sizeof(ImageLine));
			for (y = 0; y < (int)(disp_num_row * min(1.0, zoom_factor) + 0.5); y++) {
				pixel_y = (int)(y / min(1.0, zoom_factor) + 0.5);
				image_ptr = &dispImage[pixel_y][0];
				if (skip_edge_rows && ((pixel_y == 0) || (pixel_y == disp_num_row - 1)))
					skip_this = 1;
				else
					skip_this = 0;
				draw_x = 0;
				for (x = 0; x < disp_num_col; x++) {
					pix_val = *image_ptr++;
					if (hide_overscan_flag) {
						if (((x % chan_section) >= overscan_x1) && ((x % chan_section) <= overscan_x2))
							continue;
					}

					if (skip_this)
						pix_val = 0; // scale_min;
					if (splitterMode && autoScaleFlag && (x >= disp_num_col / 2)) {
						pix_val = max(min(pix_val, scale_max2), scale_min2);
						val = (unsigned char)((pix_val - scale_min2)*pix_scale2);
					}
					else {
						pix_val = max(min(pix_val, scale_max), scale_min);
						val = (unsigned char)((pix_val - scale_min)*pix_scale);
					}
					if (splitterAlignMode && frameLiveFlag) {
						this_col = regular_LUT[val];
						if (x < disp_num_col/2) {
							float pix_val2 = *(image_ptr + disp_num_col / 2 - 1);
							pix_val2 = max(min(pix_val2, scale_max), scale_min);
							unsigned char val2 = (unsigned char)((pix_val2 - scale_min)*pix_scale);
							int this_col2 = rainbow_LUT[val2];
							this_col = (unsigned int)((this_col & 0xff)*0.5 + (this_col2 & 0xff)*0.5) |
								((unsigned int)(((this_col & 0xff00) >> 8)*0.5 + ((this_col2 & 0xff00) >> 8)*0.5) << 8) |
								((unsigned int)(((this_col & 0xff0000) >> 16)*0.5 + ((this_col2 & 0xff0000) >> 16)*0.5) << 16);
						}
					}
					else {
						if (frame_d_mode == 1)
							this_col = rainbow_LUT[val];
						else
							this_col = regular_LUT[val];
						if (SelectedMap[pixel_y][x] != 0 && roi_transp_level < 100 && !hide_overscan_flag) {
							if (SelectedMap[pixel_y][x] > 0) {
								if (SelectedMap[pixel_y][x] == hlighted_roi) {
									roi_transparency = 50 / 100.0;
									roi_color = WHITE;
								}
								else {
									roi_transparency = roi_transp_level / 100.0;
									roi_color = traceColorAr[SelectedMap[pixel_y][x] - 1];
								}
							}
							else {
								roi_transparency = 0.90;
								roi_color = PURPLE;
							}
							this_col = (unsigned int)((this_col & 0xff)*roi_transparency + (roi_color & 0xff)*(1 - roi_transparency)) |
								((unsigned int)(((this_col & 0xff00) >> 8)*roi_transparency + ((roi_color & 0xff00) >> 8)*(1 - roi_transparency)) << 8) |
								((unsigned int)(((this_col & 0xff0000) >> 16)*roi_transparency + ((roi_color & 0xff0000) >> 16)*(1 - roi_transparency)) << 16);
						}
					}
					this_x = (int)(draw_x*zoom_factor + 0.5);
					for (l = 0; l < (int)(max(1.0, zoom_factor + 1)); l++)
						ImageLine[this_x + l] = this_col;

					if (line_profile_flag) {
						if (line_profile_flag == 1) {
							if (y == line_profileXY0)
								lineProfileAr[x] = (int)dispImage[y][x];
							else if (y > line_profileXY0 && y <= line_profileXY)
								lineProfileAr[x] += (int)dispImage[y][x];
							if (y == line_profileXY) {
								lineProfileAr[x] /= line_profileXY - line_profileXY0 + 1;
								if (x == 0) {
									lineP_min = lineProfileAr[x];
									lineP_max = lineProfileAr[x];
								}
								else {
									if (lineP_min > lineProfileAr[x])
										lineP_min = lineProfileAr[x];
									if (lineP_max < lineProfileAr[x])
										lineP_max = lineProfileAr[x];
								}
							}
						}
						else {
							if (x == line_profileXY0)
								lineProfileAr[y] = (int)dispImage[y][x];
							else if (x > line_profileXY0 && x <= line_profileXY)
								lineProfileAr[y] += (int)dispImage[y][x];
							if (x == line_profileXY) {
								lineProfileAr[y] /= line_profileXY - line_profileXY0 + 1;
								if (y == 0) {
									lineP_min = lineProfileAr[y];
									lineP_max = lineProfileAr[y];
								}
								else {
									if (lineP_min > lineProfileAr[y])
										lineP_min = lineProfileAr[y];
									if (lineP_max < lineProfileAr[y])
										lineP_max = lineProfileAr[y];
								}
							}
						}
					}
					draw_x++;
				}
				for (int k = 0; k < (int)(max(1.0, zoom_factor + 1)); k++) {
					y_pos = img_y0 + (int)(y * max(1.0, zoom_factor) + 0.5) + k;
					if (y_pos > 0 && y_pos < dy0)
						putImageLine(img_x0, y_pos, dx, ImageLine);
				}
			}

			if (line_profile_flag == 1) {
				int lp_step, aa;
				lp_dy = image_region_dy / 3;

				if (line_profileXY0 + line_profileXY < disp_num_row)
					lp_y_offset = y0 + image_region_dy/2 + 30;
				else
					lp_y_offset = y0 + 50;

				aa = 1;
				while (aa < (lineP_max - lineP_min) / 4) 
					aa *= 2;
				lp_step = aa;

				aa = lp_step;
				while (aa < lineP_min)
					aa += lp_step;
				lineP_min = aa - lp_step;
				aa = lp_step;
				while (aa < lineP_max)
					aa += lp_step;
				lineP_max = aa;

				for (x = 0; x < disp_num_col - 1; x++) {
					if (img_x0 + (x + 1)*zoom_factor > dx)
						break;
					if (lineP_max > lineP_min) {
						int xx1, yy1, xx2, yy2;
						xx1 = (int)(x0 + min(max(img_x0 + x * zoom_factor + 0.5, 0), image_region_dx));
						yy1 = (int)(lp_y_offset + (lineP_max - lineProfileAr[x]) * lp_dy / (lineP_max - lineP_min) + 0.5);
						xx2 = (int)(x0 + min(max(img_x0 + (x + 1)*zoom_factor + 0.5, 0), image_region_dx));
						yy2 = (int)(lp_y_offset + (lineP_max - lineProfileAr[x + 1]) * lp_dy / (lineP_max - lineP_min) + 0.5);
						HLine(xx1, xx2, yy1, RED);
						VLine(yy1, yy2, xx2, RED);
					}
				}
				HLine(x0 + max(img_x0, 0), x0 + min(img_x0 + (int)((disp_num_col)*zoom_factor + 0.5), dx), lp_y_offset, YELLOW);
				HLine(x0 + max(img_x0, 0), x0 + min(img_x0 + (int)((disp_num_col)*zoom_factor + 0.5), dx), lp_y_offset + lp_dy, YELLOW);
				HLine(x0 + max(img_x0, 0), x0 + min(img_x0 + (int)((disp_num_col)*zoom_factor + 0.5), dx),
					y0 + img_y0 + (int)(line_profileXY0*max(1.0, zoom_factor) + 0.5), RED);
				HLine(x0 + max(img_x0, 0), x0 + min(img_x0 + (int)((disp_num_col)*zoom_factor + 0.5), dx),
					y0 + img_y0 + (int)(line_profileXY*max(1.0, zoom_factor) + 0.5), RED);
			}
			else if (line_profile_flag == 2) {
				int lp_step, aa;
				lp_dx = min(img_x0 + (int)((disp_num_col)*zoom_factor + 0.5), dx) / 3;

				if (line_profileXY0 + line_profileXY > disp_num_col)
					lp_x_offset = x0 + min(img_x0 + (int)((disp_num_col)*zoom_factor + 0.5), dx) / 2 - 20;
				else
					lp_x_offset = x0 + min(img_x0 + (int)((disp_num_col)*zoom_factor + 0.5), dx) - 20;

				aa = 1;
				while (aa < (lineP_max - lineP_min) / 4)
					aa *= 2;
				lp_step = aa;

				aa = lp_step;
				while (aa < lineP_min)
					aa += lp_step;
				lineP_min = aa - lp_step;
				aa = lp_step;
				while (aa < lineP_max)
					aa += lp_step;
				lineP_max = aa;

				for (y = 0; y < disp_num_row - 1; y++) {
					if (img_y0 + (y + 1)*zoom_factor > dy)
						break;
					if (lineP_max > lineP_min) {
						int xx1, yy1, xx2, yy2;
						xx1 = (int)(lp_x_offset - (lineP_max - lineProfileAr[y]) * lp_dx / (lineP_max - lineP_min) + 0.5);
						yy1 = (int)(y0 + min(max(img_y0 + y * zoom_factor + 0.5, 0), image_region_dy));
						xx2 = (int)(lp_x_offset - (lineP_max - lineProfileAr[y + 1]) * lp_dx / (lineP_max - lineP_min) + 0.5);
						yy2 = (int)(y0 + min(max(img_y0 + (y + 1)*zoom_factor + 0.5, 0), image_region_dy));
						VLine(yy1, yy2, xx1, RED);
						HLine(xx1, xx2, yy2  , RED);
					}
				}
				VLine(y0 + max(img_y0, 0), y0 + min(img_y0 + (int)((disp_num_row)*zoom_factor + 0.5), dy), lp_x_offset, YELLOW);
				VLine(y0 + max(img_y0, 0), y0 + min(img_y0 + (int)((disp_num_row)*zoom_factor + 0.5), dy), lp_x_offset - lp_dx, YELLOW);
				VLine(y0 + max(img_y0, 0), y0 + min(img_y0 + (int)((disp_num_row)*zoom_factor + 0.5), dy),
					x0 + img_x0 + (int)(line_profileXY0*max(1.0, zoom_factor) + 0.5), RED);
				VLine(y0 + max(img_y0, 0), y0 + min(img_y0 + (int)((disp_num_row)*zoom_factor + 0.5), dy),
					x0 + img_x0 + (int)(line_profileXY*max(1.0, zoom_factor) + 0.5), RED);
			}

			if (hide_overscan_flag) {
				zoom_factor = zoom_f_sv;
				img_y0 = img_y0_sv;
			}

			//draw color scale bar
			int y1, y2;
			x = x0 + 45;
			y2 = max(img_y0 - 5, y0 + 12);
			y1 = y2 - 12;
			for (int i = 0; i < num_colors; i++) {
				if (frame_d_mode == 1)
					this_col = rainbow_LUT[i];
				else
					this_col = regular_LUT[i];
				VLine(y1, y2, x + i, this_col);
			}

			drawMarks(PAINTGRFX);
			if (!window_layout)
				paintHisto(PAINTGRFX);

			if (frame_d_mode == 2 && zoom_factor >= 16.0 && dataFile && !frameLiveFlag) {	//draw overlay traces
				int first_pix_x, first_pix_y, x_pos;

				first_pix_x = (int)max(0, -img_x0 / zoom_factor + 0.5);
				first_pix_y = (int)max(0, -img_y0 / zoom_factor + 0.5);
				dx = (int)(zoom_factor + 0.5) - 4;
				for (y = first_pix_y; y < disp_num_row; y++) {
					y_pos = (int)(y * zoom_factor + 0.5) + img_y0;
					if (y_pos > dy0)
						break;
					for (x = first_pix_x; x < disp_num_col; x++) {
						x_pos = (int)(x * zoom_factor + 0.5) + img_x0 + 2;
						if (x_pos + dx > x0 + image_region_dx - 10)
							break;
						if (SelectedMap[y][x] > 0)
							this_col = traceColorAr[SelectedMap[y][x] - 1];
						else
							this_col = MIDGRAY;
						if (x_pos > 0 && y_pos > 0)
							drawPixTrace(x, y, x0 + x_pos, dx, y0 + y_pos + dx * 2 / 3, dx, this_col);
					}
				}
				pixel_trace_displayed = 1;
			}
			else {
				pixel_trace_displayed = 0;
				if (roi_border_flag)
					drawSelectedPixels(0, 0);
			}

			if (photonDFlag) {
				if (!hide_overscan_flag)
					drawPHT_boxes();
				drawPHT_curves();
			}
		}
	}
	else if (paintFlag == PAINTTEXT) {
		char mssg[200];

		if (!BNCfile_only) {
			drawMarks(PAINTTEXT);
			if (!window_layout)
				paintHisto(PAINTTEXT);
		}

	/*	if (dataExist) {
			strcpy(mssg, "Use Mouse Wheel or +/- Buttons to Zoom, Ctrl-Z to undo last ROI selection");
			drawText(0, 0, mssg, 30, y0 + 20, TEXT_RED, omFontMedium, TRUE);
		}*/
		if (acquiring_flag && num_trials > 1) {
			if (spike_triggered || (av_file_cnt < num_trials - 1)) {
				sprintf(mssg, "Trials Acquired: %d", av_file_cnt + 1);
				drawText(0, 0, mssg, dx / 3, dy0 / 3, TEXT_YELLOW, omFontBig, TRUE);
				if (!spike_triggered) {
					strcpy(mssg, "Hold down Shift-C to Cancel repeated acqusitions");
					drawText(0, 0, mssg, dx / 3, dy0 / 3 + 25, TEXT_YELLOW, omFontBig, TRUE);
				}
			}
		}
		if (line_profile_flag) {
			sprintf(mssg, "%d", lineP_max);
			if (line_profile_flag == 1)
				drawText(0, 0, mssg, min(x0 + max(img_x0, 0) + (int)((disp_num_col)*zoom_factor + 0.5), dx) + 3, lp_y_offset - 8, TEXT_YELLOW, omFontRprtTn, TRUE);
			else
				drawText(0, 0, mssg, (int)(lp_x_offset - (fontSM_w + 0.8)*strlen(mssg) / 2 + 0.5), y0 + min(img_y0 + (int)((disp_num_row)*zoom_factor + 0.5), dy) + 1, TEXT_YELLOW, omFontRprtTn, TRUE);
			sprintf(mssg, "%d", lineP_min);
			if (line_profile_flag == 1)
				drawText(0, 0, mssg, min(x0 + max(img_x0, 0) + (int)((disp_num_col)*zoom_factor + 0.5), dx) + 3, lp_y_offset+lp_dy - 8, TEXT_YELLOW, omFontRprtTn, TRUE);
			else
				drawText(0, 0, mssg, (int)(lp_x_offset-lp_dx - (fontSM_w + 0.8)*strlen(mssg) / 2 + 0.5), y0 + min(img_y0 + (int)((disp_num_row)*zoom_factor + 0.5), dy) + 1, TEXT_YELLOW, omFontRprtTn, TRUE);
		}
		if (photonDFlag)
			strcpy(mssg, "Photon Transfer");
		if (frameLiveFlag) {
			strcpy(mssg, "Live");
			if (NDR_subtraction_flag && ndr_lib[curConfig])
				sprintf(mssg, "%s (NDR sub %d)", mssg, NDR_subtraction_frames);
			else if (darkSubFlag)
				strcat(mssg, " - Dark");
		}
		else if (dataFile) {
			char tmp_str[256];
			char *p;
			int min_text_x, max_text_x;

			sprintf(mssg, "%s", SM_dataname);
			if (!window_layout) {
				y = y0 + 10;
				min_text_x = image_region_dx;
				max_text_x = OM[SM_HISTEXP].x;
			}
			else {
				y = OM[SM_IMGSCBASE].y;
				min_text_x = OM[SM_IMGSCBASE].x + OM[SM_IMGSCBASE].dx + 20;
				max_text_x = getMeasureWinX() - 300;
			}
			x = getMeasureWinX() - (int)((fontSM_w + 0.8)*strlen(mssg) + 5);
			x = min(x, max_text_x);
			if (x < min_text_x) {
				int num_char_skip = (int)(strlen(mssg) - (getMeasureWinX() - min_text_x - 5) / (fontSM_w + 0.8));
				strcpy(mssg, mssg + num_char_skip);
				x = min_text_x;
			}
			drawText(0, 0, mssg, x, y, colorInfoBkgrnd, omFontSmall, TRUE);
			y += 15;
			if (fileTime != NULL) {
				if (fileTime->tm_min < 10)
					sprintf(tmp_str, "0%d", fileTime->tm_min);
				else
					sprintf(tmp_str, "%d", fileTime->tm_min);
				sprintf(mssg, "Time: %d/%d/%d %d:%s", fileTime->tm_mon + 1, fileTime->tm_mday, fileTime->tm_year + 1900, fileTime->tm_hour + gt_hour_diff, tmp_str);
			}
			drawText(0, 0, mssg, x, y, colorInfoBkgrnd, omFontSmall, TRUE);
			y += 15;
			if (p = strstr(file_cameraName, "VER1,")) {
				strcpy(tmp_str, p + 6);
				sprintf(mssg, "Config: %s, Exposure: %3.3lfms", tmp_str, SM_exposure * 1000);
			}
			else
				sprintf(mssg, "Exposure: %3.3lfms", SM_exposure * 1000);
			drawText(0, 0, mssg, x, y, colorInfoBkgrnd, omFontSmall, TRUE);
			y += 15;
			sprintf(mssg, "Frames: %d-%d of %d", firstDataFrame + 1, firstDataFrame + numTraceFrames, numDataFrames);
			drawText(0, 0, mssg, x, y, colorInfoBkgrnd, omFontSmall, TRUE);

			if (num_av_disp > 1)
				sprintf(mssg, "Frame %d/%d", firstDataFrame + av_stt + 1, firstDataFrame + av_end + 1);
			else
				sprintf(mssg, "Frame %d", firstDataFrame + curDispFrame + 1);
			if (darkSubFlag) {
				if (f_subtract_mode) {
					sprintf(mssg, "%s - %d/%d", mssg, firstDataFrame + ref_frame1 + 1, firstDataFrame + ref_frame2 + 1);
					if (NDR_subtraction_flag && NDR_trace_sub)
						strcat(mssg, " [NDR subtracted]");
					else if (f_subtract_mode)
						strcat(mssg, " [Ref subtracted]");
				}
				else
					sprintf(mssg, "%s - Dark", mssg);
			}
			if (trace_subtracted)
				sprintf(mssg, "%s [Data with Scaled Trace Sutraction]", mssg);
			if (movie_normalized)
				sprintf(mssg, "%s [Data Normalized]", mssg);
		}
		else
			strcpy(mssg, "");
		if (strcmp(mssg, "") && edge_correct_flag)
			sprintf(mssg, "%s [Row Corrected]", mssg);

		drawText(0, 0, mssg, 30, dy0 - 20, TEXT_RED, omFontMedium, TRUE);

		//draw color scale bar
		x = x0 + 15;
		y = max(img_y0 - 17, y0);
		sprintf(mssg, "%d", scale_min);
		drawText(0, 0, mssg, (int)(x - (fontSM_w+0.8)*strlen(mssg) / 2), y, LTGRAY, omFontRprtTn, TRUE);
		x += num_colors + 60;
		sprintf(mssg, "%d", scale_max);
		drawText(0, 0, mssg, (int)(x - (fontSM_w+0.8)*strlen(mssg) / 2), y, LTGRAY, omFontRprtTn, TRUE);

		if (roi_label_mode)
			showROI_labels();
	}
}

static float edge_c_intar[2048];
static float edge_c_ar[2048];
static int DoNotPaint = 0;
static FILE *fp_txt_img;
static bool save_txt_img_flag = 0;
static int row_skip_min, row_skip_max, col_skip_min, col_skip_max;
static bool scaleROI;

void resetHisto()
{
	int i;
	memset(histoAr_full, 0, sizeof(histoAr_full));
	memset(histoAr_full_scl, 0, sizeof(histoAr_full_scl));
	memset(selectedHistoAr_full, 0, sizeof(selectedHistoAr_full));
	memset(meanCnt, 0, sizeof(meanCnt));
	for (i = 0; i < sizeof(histMeans) / sizeof(histMeans[0]); i++) {
		histMax[i] = -32765;
		histMin[i] = 32765;
		histMeans[i] = 0.0;
		histSTD[i] = 0.0;
	}
}

void setScaleDim()
{
	scaleROI = scale_pix_coor[0] != scale_pix_coor[2] && scale_pix_coor[1] != scale_pix_coor[3];
	double row_skip_r = 0.05, col_skip_r = 0.05;
	if (TwoK_flag && (pdv_chan_num > 2)) {
		if (auto_scale_w < disp_num_col)
			col_skip_r = max(col_skip_r, (((double)disp_num_col) - auto_scale_w) / (2.0*disp_num_col));
		if (auto_scale_w < disp_num_row)
			row_skip_r = max(row_skip_r, (((double)disp_num_row) - auto_scale_w) / (2.0*disp_num_row));
	}
	row_skip_min = max(1, (int)(disp_num_row * row_skip_r + 0.5));
	row_skip_max = min(disp_num_row - 2, (int)(disp_num_row * (1 - row_skip_r) + 0.5));
	col_skip_min = max(1, (int)(disp_num_col * col_skip_r + 0.5));
	col_skip_max = min(disp_num_col - 2, (int)(disp_num_col * (1 - col_skip_r) + 0.5));
}

void getHistoMinMax(float pixel, int x, int y)
{
	float min_pix_sv = min_pix, max_pix_sv = max_pix;

	if ((pixel < auto_skip_v1) && (pixel > auto_skip_v2)) {
		if (splitterMode && (x >= disp_num_col / 2)) {
			min_pix = min_pix2;
			max_pix = max_pix2;
		}
		if (hide_overscan_flag) {
			int chan_section = disp_num_col / stripes_lib[curConfig];
			if (((x % chan_section) < overscan_x1) || ((x % chan_section) > overscan_x2)) {
				if (pixel > max_pix)
					max_pix = pixel;
				else if (pixel < min_pix)
					min_pix = pixel;
				histoAr_full_scl[int(pixel+0.5) + 16000]++;
			}
		}
		else if (scaleROI) {
			if (x >= scale_pix_coor[0] && x <= scale_pix_coor[2]) {
				if (y > scale_pix_coor[1] && y < scale_pix_coor[3]) {
					if (pixel > max_pix)
						max_pix = pixel;
					else if (pixel < min_pix)
						min_pix = pixel;
					histoAr_full_scl[int(pixel + 0.5) + 16000]++;
				}
			}
		}
		else {
			if (x > col_skip_min && x < col_skip_max) {
				if (y > row_skip_min && y < row_skip_max) {

					if (pixel > max_pix)
						max_pix = pixel;
					else if (pixel < min_pix)
						min_pix = pixel;
					histoAr_full_scl[int(pixel + 0.5) + 16000]++;
				}
			}
		}
		if (splitterMode && (x >= disp_num_col / 2)) {
			min_pix2 = min_pix;
			min_pix = min_pix_sv;
			max_pix2 = max_pix;
			max_pix = max_pix_sv;
		}
	}
}

void sumHistoPix(float pixel, int x, int y)
{
	float pixel2;
	int m;

	//get histogram info
	if (histMax[0] < pixel)
		histMax[0] = pixel;
	if (histMin[0] > pixel)
		histMin[0] = pixel;
	histMeans[0] += pixel;
	pixel2 = pixel * pixel;
	histSTD[0] += pixel2;
	meanCnt[0]++;
	histoAr_full[int(pixel+0.5) + 16000]++;
	if (SelectedMap[y][x] > 0) {
		for (m = 0; m < num_traces; m++) {
			if (SelectedMap[y][x] == pixColorCode[m]) {
				if (histMax[m + 1] < pixel)
					histMax[m + 1] = pixel;
				if (histMin[m + 1] > pixel)
					histMin[m + 1] = pixel;
				histMeans[m + 1] += pixel;
				histSTD[m + 1] += pixel2;
				meanCnt[m + 1]++;
				selectedHistoAr_full[m][int(pixel + 0.5) + 16000]++;
			}
			ROI_index[m][5] = meanCnt[m + 1];
		}
	}
}

void getDispImage(int HistoOnly)
{
	int x, y, cnt, cnt_x;
	float edge_c_mean, edge_c_val;
	float *ptr, *ptr_sv, *RLI_ptr, *ref_ptr, *ref_ptr_sv;
	signed short int *dk_ptr, *RIC_ptr;
	signed short int *dk_ptr_sv, *RIC_ptr_sv;
	unsigned int ref_av=0;

	if (!dataExist)
		return;

	if (!DoNotPaint && BNCfile_only) {
		paintWin(OM_MEASUREWIN, PAINTINIT);
		return;
	}

	ptr = (float *)single_img_float;
	RIC_ptr = (signed short int *)RIC_image;
	dk_ptr = (signed short int *)darkImage;
	ref_ptr = (float *)ref_image;
	if (!HistoOnly) {
		if (!singleF_flag) {
			if (dataFile && NDR_subtraction_flag)
				getNDR_ref();
			else if (dataFile && RLI_div_flag2) {
				getRLIImage();
				getRefImage();
			}
		}

		max_pix = -32765;
		min_pix = 32765;
		max_pix2 = -32765;
		min_pix2 = 32765;
		if (singleF_flag && !get_overlay_flag) {
			dk_ptr = (signed short int *)darkImage;
			if (!darkExist)
				darkSubFlag = FALSE;
		}
		else if (dataFile) {
			ref_ptr = (float *)ref_image;
			dk_ptr = (signed short int *)dark_data;
			if (!d_darkExist)
				darkSubFlag = FALSE;
		}
		if (!darkSubFlag) {
			if (OM[SM_DKSUBBOX].attr) {
				setChckBox(SM_DKSUBBOX);
				paintWin(SM_DKSUBBOX, PAINTGRFX);
			}
		}

		//**** get edge_correction values
		if (edge_correct_flag) {
			ptr_sv = ptr;
			dk_ptr_sv = dk_ptr;
			ref_ptr_sv = ref_ptr;
			RIC_ptr_sv = RIC_ptr;
			edge_c_mean = 0;
			cnt = 0;
			for (y = 0; y < disp_num_row; y++) {
//				edge_c_ar[y] = 0;
				edge_c_val = 0;
				cnt_x = 0;

				if (edge_correct_mode == 1) {
					ptr += disp_num_col - edge_c_col2;
					dk_ptr += disp_num_col - edge_c_col2;
					ref_ptr += disp_num_col - edge_c_col2;
					RIC_ptr += disp_num_col - edge_c_col2;
				}
				else {
					ptr += edge_c_col1;
					dk_ptr += edge_c_col1;
					ref_ptr += edge_c_col1;
					RIC_ptr += edge_c_col1;
					for (x = edge_c_col1; x <= edge_c_col2; x++) {
						if (darkSubFlag) {
							if ((f_subtract_mode && !get_overlay_flag) || RLI_div_flag2)
								dispImage[y][x] = (float)(*ptr++ - *ref_ptr++);
							else
								dispImage[y][x] = (float)(*ptr++ - *dk_ptr++);
							if (RIC_flag && *RIC_ptr)
								dispImage[y][x] = dispImage[y][x] * RIC_mean / (*RIC_ptr);
							RIC_ptr++;
						}
						else
							dispImage[y][x] = *ptr++;
						if ((dispImage[y][x] < auto_skip_v1) && (dispImage[y][x] > auto_skip_v2)) {
//							edge_c_ar[y] += dispImage[y][x];
							edge_c_val += dispImage[y][x];
							cnt_x++;
						}
					}
					ptr += disp_num_col - edge_c_col2 * 2 - 1;
					dk_ptr += disp_num_col - edge_c_col2 * 2 - 1;
					ref_ptr += disp_num_col - edge_c_col2 * 2 - 1;
					RIC_ptr += disp_num_col - edge_c_col2 * 2 - 1;
				}
				if (edge_correct_mode >= 1) {
					for (x = disp_num_col - 1 - edge_c_col2; x <= disp_num_col - 1 - edge_c_col1; x++) {
						if (darkSubFlag) {
							if ((f_subtract_mode && !get_overlay_flag) || RLI_div_flag2)
								dispImage[y][x] = (float)(*ptr++ - *ref_ptr++);
							else
								dispImage[y][x] = (float)(*ptr++ - *dk_ptr++);
							if (RIC_flag && *RIC_ptr)
								dispImage[y][x] =dispImage[y][x] * RIC_mean / (*RIC_ptr);
							RIC_ptr++;
						}
						else
							dispImage[y][x] = *ptr++;
						if ((dispImage[y][x] < auto_skip_v1) && (dispImage[y][x] > auto_skip_v2)) {
//							edge_c_ar[y] += dispImage[y][x];
							edge_c_val += dispImage[y][x];
							cnt_x++;
						}
					}
					ptr += edge_c_col1 - 1;
					dk_ptr += edge_c_col1 - 1;
					ref_ptr += edge_c_col1 - 1;
					RIC_ptr += edge_c_col1 - 1;
				}
				else {
					ptr += edge_c_col2;
					dk_ptr += edge_c_col2;
					ref_ptr += edge_c_col2;
					RIC_ptr += edge_c_col2;
				}
				if (cnt_x)
					edge_c_val = edge_c_val/cnt_x;
				edge_c_intar[y] = edge_c_val;
				if ((y > 1) && (y < disp_num_row - 2)) {
					edge_c_mean += edge_c_val;
					cnt++;
				}
			}
			edge_c_mean = edge_c_mean / cnt;
			for (y = 0; y < disp_num_row; y++)
				edge_c_ar[y] = edge_c_intar[y] - edge_c_mean;
			ptr = ptr_sv;
			dk_ptr = dk_ptr_sv;
			ref_ptr = ref_ptr_sv;
			RIC_ptr = RIC_ptr_sv;
		}
	}
	/****/

	short int RIC_val, *ov_img_ptr, *omit_pt;
	float pixel, *image_ptr = &dispImage[0][0];
	int i, m;
	double pixel_div;
	RLI_ptr = (float *)RLI_image;

	resetHisto();
	setScaleDim();
	ov_img_ptr = overlay_img;
	omit_pt = omit_ar;
	int omit_this = 0;
	for (y = 0; y < disp_num_row; y++) {
		image_ptr = &dispImage[y][0];
		for (x = 0; x < disp_num_col; x++) {
			if (omit_ar && !frameLiveFlag)
				omit_this = *omit_pt++;
			if (!HistoOnly) {
				pixel = *ptr++;
				if (darkSubFlag || RLI_div_flag2) {
					if ((f_subtract_mode && !get_overlay_flag) || RLI_div_flag2)
						pixel -= *ref_ptr;
					else
						pixel -= *dk_ptr;
					if (RIC_flag) {
						if (RIC_val = *RIC_ptr++)
							pixel = pixel*RIC_mean / RIC_val;
					}
					if (RLI_div_flag2 && !singleF_flag) {
						if (*RLI_ptr != 0)
							pixel_div = max(min((double)pixel / (*RLI_ptr), 3.2767), -3.2767);
						else
							pixel_div = 3.2767;
						if (save_txt_img_flag)
							fprintf(fp_txt_img, "%lf, ", pixel_div);
						pixel = (float)(pixel_div * 10000);
					}
					else if (save_txt_img_flag)
						fprintf(fp_txt_img, "%lf, ", pixel);
					dk_ptr++;
					ref_ptr++;
					RLI_ptr++;
				}
				if (edge_correct_flag)
					pixel -= edge_c_ar[y];
				*image_ptr++ = pixel;
				if (!omit_this)
					getHistoMinMax(pixel, x, y);
			}
			else 
				pixel = *image_ptr++;
			if (!omit_this)
				sumHistoPix(pixel, x, y);
		}
		if (get_overlay_flag) {
			memcpy(ov_img_ptr, &dispImage[y][0], disp_num_col*2);
			ov_img_ptr += disp_num_col;
		}
		if (save_txt_img_flag)
			fprintf(fp_txt_img, "\n");
	}
	// get rid of outlier from scaling
	if (!HistoOnly) {
		spatial_filter(0);
		if (omit_ar && !frameLiveFlag) {
			omit_pt = omit_ar;
			for (y = 0; y < disp_num_row; y++) {
				image_ptr = &dispImage[y][0];
				for (x = 0; x < disp_num_col; x++) {
					if (*omit_pt)
						*image_ptr = -32765;
					image_ptr++;
					omit_pt++;
				}
			}
		}
		// to get rid of outlier pixels from scaling
		if ((histoAr_full_scl[int(min_pix + 0.5) + 16000] < (unsigned int)min(10, disp_num_col*disp_num_row*0.01)) && (histoAr_full_scl[int(min_pix + 0.5) + 16000 + 1] == 0)) {
			min_pix++;
			while (histoAr_full_scl[int(min_pix + 0.5) + 16000] == 0)
				min_pix++;
		}
		if ((histoAr_full_scl[int(max_pix + 0.5) + 16000] < (unsigned int)min(10, disp_num_col*disp_num_row*0.01)) && (histoAr_full_scl[int(max_pix + 0.5) + 16000 - 1] == 0)) {
			max_pix--;
			while (histoAr_full_scl[int(min_pix + 0.5) + 16000] == 0)
				max_pix--;
		}
	}
	if (BNC_exist) {
		for (x = 0; x < disp_num_col; x++) {
			if (SelectedMap[disp_num_row][x] > 0) {
				for (m = 0; m < num_traces; m++) {
					if (SelectedMap[y][x] == pixColorCode[m]) {
						histMeans[m + 1] = 0;
						histSTD[m + 1] = 0;
						meanCnt[m + 1] = 1;
					}
				}
			}
		}
	}
	for (i = 0; i < num_traces + 1; i++) {
		if (meanCnt[i] <= 1) {
			histMeans[i] = histMeans[i];
			histSTD[i] = 0.0;
		}
		else {
			histMeans[i] = histMeans[i] / meanCnt[i];
			histSTD[i] = sqrt(histSTD[i] / meanCnt[i] - histMeans[i] * histMeans[i]);
		}
	}
	if (!DoNotPaint && !get_overlay_flag)
		paintWin(curTab(), PAINTINIT);
}

void SM_saveTxt_image(char *filename)
{
	if (!FOPEN_S(fp_txt_img, filename, "w"))
		return;

	save_txt_img_flag = 1;
	getDispImage(0);
	fclose(fp_txt_img);
	save_txt_img_flag = 0;
}

void setLiveFlag(int flag)
{
	if (frameLiveFlag != flag) {
		if (dataFile && ((disp_num_col != cam_num_col) || (disp_num_row != cam_num_row))) {
			clearTracePix();
			resetZoom(flag);
		}
		if (flag)
			paintWin(SM_TRACEWIN, PAINTINIT);
	}
	frameLiveFlag = flag;
}

extern void frameRepair(unsigned short int *image_buffer, int bad_pix_index, int twoKFlag, unsigned int image_width, unsigned int file_width, unsigned int file_height, int horizontal_bin, int superFrame, int file_img_w, int binFlag, int segmented);

void getStreamFrame()
{
	signed short int *sptr, *dptr, *ptr;
	disp_num_col = file_num_col;
	disp_num_row = file_num_row;
	singleF_flag = FALSE;
	double val;
	int i;
	long m, len;

	if (!dataFile) 
		return;
	if (!BNCfile_only) {
		auto_scale_w = disp_num_col;
		ptr = (signed short int *)image_data;
		len = disp_num_col * disp_num_row;

		float *f_ptr = single_img_float;
		if (num_av_disp > 1) {
			dptr = single_img_data;
			av_stt = max(0, (int)(curDispFrame - num_av_disp / 2.0 + 0.5));
			av_end = min(av_stt + num_av_disp - 1, numTraceFrames - 1);
			if (NDR_subtraction_flag) {
				for (i = 0; i < num_NDR_resets; i++) {
					if ((NDR_reset_ar[i] > av_stt) && (NDR_reset_ar[i] <= av_end)) {
						int NDR_break = NDR_reset_ar[i];
						if (av_end >= NDR_break) {
							if (NDR_break - av_stt > av_end - NDR_break)
								av_end = NDR_break - 1;
							else {
								av_stt = NDR_break;
							}
						}
						break;
					}
				}
			}
			sptr = ptr + len * av_stt;
			for (m = 0; m < len; m++) {
				ptr = sptr + m;
				val = *ptr;
				for (i = av_stt + 1; i <= av_end; i++) {
					ptr += len;
					val += *ptr;
				}
				*f_ptr = (float)(val / (av_end - av_stt + 1));
				*dptr = (signed short int)(*f_ptr + 0.5);
				f_ptr++;
				dptr++;
			}
		}
		else {
			av_stt = min(curDispFrame, numTraceFrames - 1);
			ptr += len * curDispFrame;
			memcpy(single_img_data, ptr, len * 2);
			ptr = single_img_data;
			for (m = 0; m < len; m++)
				*f_ptr++ = (float)(*ptr++);
		}

		if (bad_pixel_file && bad_pixel_ar[0][0]) {
			frameRepair((unsigned short *)single_img_data, 1, 0, disp_num_col, disp_num_col, disp_num_row, 0, 0, disp_num_col, 0, 0);
			ptr = single_img_data;
			f_ptr = single_img_float;
			for (m = 0; m < len; m++)
				*f_ptr++ = (float)(*ptr++);
		}
	}
	setLiveFlag(false);
	getDispImage(0);
}

/*
void drawEdgePix(int x, int y, int color, int x_min, int x_max, int y_min, int y_max)
{
	int neighbor_ar[][2] = { {0, -1 }, {1, 0 }, {0, 1}, { -1, 0 } };
	int i, j, m, vector;

	if (SelectedMap[y - 1][x] != color) {

	}
}*/

void draw_HLine(int x1, int x2, int y, int color, int for_tiff)
{
	unsigned int *tiff_img_ptr;
	int i, x0, y0;

	if (for_tiff) {
		x0 = getMeasureWinX0();
		y0 = getMeasureWinY0();
		tiff_img_ptr = MovieTiffImg + (y-y0) * save_tiff_dx + x1 - x0;
		for (i = x1; i <= x2; i++)
			*tiff_img_ptr++ = color;
	}
	else
		HLine(x1, x2, y, color);
}

void draw_VLine(int y1, int y2, int x, int color, int for_tiff)
{
	unsigned int *tiff_img_ptr;
	int i, x0, y0;

	if (for_tiff) {
		x0 = getMeasureWinX0();
		y0 = getMeasureWinY0();
		tiff_img_ptr = MovieTiffImg + (y1-y0) * save_tiff_dx + x-x0;
		for (i = y1; i <= y2; i++) {
			*tiff_img_ptr = color;
			tiff_img_ptr += save_tiff_dx;
		}
	}
	else
		VLine(y1, y2, x, color);
}

void drawSelectedPixels(int for_movie, int for_tiff)
{
	int i, j, k, color, x0, y0, dx, dy, this_img_x0, this_img_y0, cnt = 0;
	int x1, x2, y1, y2;
	double this_zoom;

	if (for_movie) {
		this_zoom = mov_zoom_factor/mov_bin_used;
		this_img_x0 = mov_img_x0;
		this_img_y0 = mov_img_y0;
	}
	else {
		this_zoom = zoom_factor;
		this_img_x0 = img_x0;
		this_img_y0 = img_y0;
	}
	if (num_traces) {
		x0 = getMeasureWinX0();
		y0 = getMeasureWinY0();
		dx = image_region_dx;
		dy = image_region_dy;
		for (k = 0; k < num_traces; k++) {
			if (ROI_index[k][0] == BNC_SELECT)	//BNCs
				continue; 
			color = traceColorAr[pixColorCode[k]-1];
			for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++) {
				for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++) {
					if (SelectedMap[j][i] == pixColorCode[k]) {
						x1 = max(x0, min(x0 + dx, (int)(i*this_zoom + 0.5) + this_img_x0 + x0));
						x2 = max(x0, min(x0 + dx, (int)((i + 1)*this_zoom + 0.5) + this_img_x0 + x0));
						y1 = max(y0, min(y0 + dy, (int)(j*this_zoom + 0.5) + this_img_y0 + y0));
						y2 = max(y0, min(y0 + dy, (int)((j + 1)*this_zoom + 0.5) + this_img_y0 + y0));
						if (j > 0) {
							if (SelectedMap[j - 1][i] != SelectedMap[j][i])
								draw_HLine(x1, x2, y1, color, for_tiff);
						}
						else
							draw_HLine(x1, x2, y1, color, for_tiff);
						if (j < disp_num_row - 1) {
							if (SelectedMap[j + 1][i] != SelectedMap[j][i])
								draw_HLine(x1, x2, y2, color, for_tiff);
						}
						else
							draw_HLine(x1, x2, y2, color, for_tiff);
						if (i > 0) {
							if (SelectedMap[j][i - 1] != SelectedMap[j][i])
								draw_VLine(y1, y2, x1, color, for_tiff);
						}
						else
							draw_VLine(y1, y2, x1, color, for_tiff);
						if (i < disp_num_col - 1) {
							if (SelectedMap[j][i + 1] != SelectedMap[j][i])
								draw_VLine(y1, y2, x2, color, for_tiff);
						}
						else
							draw_VLine(y1, y2, x2, color, for_tiff);
					}
				}
			}
			cnt++;
		}
	}
}

void checkOmitwithROI()
{
	int k, i, j, m, n;
	signed short *omit_pt;
	int keep_roi[32];

	for (k = 0; k < num_traces; k++) {
		keep_roi[k] = 0;
		for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++) {
			omit_pt = omit_ar + j * disp_num_col + ROI_index[k][1];
			for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++) {
				if (SelectedMap[j][i] == pixColorCode[k]) {
					if (*omit_pt)
						SelectedMap[j][i] = 0;
					else
						keep_roi[k] = 1;
				}
				omit_pt++;
			}
		}
	}
	int roi_cnt = 0;
	int num_traces_sv = num_traces;
	for (k = 0; k < num_traces_sv; k++) {
		if (!keep_roi[roi_cnt]) {
			for (m = k; m < num_traces - 1; m++) {
				for (n = 0; n <= 5; n++)
					ROI_index[m][n] = ROI_index[m + 1][n];
				pixColorCode[m] = pixColorCode[m + 1];
			}
			num_traces--;
			k--;
		}
		roi_cnt++;
	}
}

void saveOmit(char *fileName)
{
	int handle;

	if ((handle = _open(fileName, O_BINARY | O_CREAT | O_WRONLY, S_IWRITE)) == -1)
		return;
	_write(handle, (signed short *)omit_ar, disp_num_col*disp_num_row * sizeof(signed short int));
	_close(handle);
}

void readOmit(char *fileName)
{
	int handle;

	if ((handle = _open(fileName, O_BINARY | O_RDONLY)) == -1)
		return;
	if (!omit_ar)
		omit_ar = (signed short int *)_aligned_malloc((unsigned long)disp_num_col*disp_num_row * sizeof(signed short int), 2);
	_read(handle, (signed short *)omit_ar, disp_num_col*disp_num_row * sizeof(signed short int));
	_close(handle);
	if (num_traces)
		checkOmitwithROI();
}

void ROI_Omit()
{
	int i, j;
	signed short *omt_pt;

	if (!num_traces) {
		MessageBox(main_window_handle, "Please select a ROI before using this function.", "message", MB_OK);
		return;
	}
	if (!omit_ar) {
		omit_ar = (signed short int *)_aligned_malloc((unsigned long)disp_num_col*disp_num_row * sizeof(signed short int), 2);
		memset(omit_ar, 0, disp_num_col*disp_num_row * sizeof(signed short int));
	}
	omt_pt = omit_ar;
	for (j = 0; j < disp_num_row; j++) {
		for (i = 0; i < disp_num_col; i++) {
			if (!SelectedMap[j][i])
				*omt_pt = 1;
			else
				*omt_pt = 0;
			omt_pt++;
		}
	}
}

int highlightROI(int mouseX, int mouseY) {
	int x, y, x0, y0;

	if (!dataExist || BNCfile_only)
		return 0;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	x = (int)(((double)mouseX - img_x0 - x0) / zoom_factor);	//+ 0.5); not needed 
	y = (int)(((double)mouseY - img_y0 - y0) / zoom_factor);
	if ((x < 0 || x >= disp_num_col || y < 0 || y >= disp_num_row) || SelectedMap[y][x] == hlighted_roi)
		return 0;
	else
		return SelectedMap[y][x];
}

void shiftROIs(int shift_direction) {
	int x, y, k;
	short int *ptr, *ptr0;

	for (k = 0; k < num_traces; k++) {
		if (!hlighted_roi || pixColorCode[k] == hlighted_roi) {
			if (shift_direction == 0) {
				if (ROI_index[k][3] > 0)
					ROI_index[k][3]--;
				if (ROI_index[k][4] > 0)
					ROI_index[k][4]--;
			}
			else if (shift_direction == 1) {
				if (ROI_index[k][1] > 0)
					ROI_index[k][1]--;
				if (ROI_index[k][2] > 0)
					ROI_index[k][2]--;
			}
			else if (shift_direction == 2) {
				if (ROI_index[k][4] < disp_num_row)
					ROI_index[k][4]++;
				if (ROI_index[k][3] < disp_num_row - 1)
					ROI_index[k][3]++;
			}
			else if (shift_direction == 3) {
				if (ROI_index[k][2] < disp_num_col)
					ROI_index[k][2]++;
				if (ROI_index[k][1] < disp_num_col - 1)
					ROI_index[k][1]++;
			}
		}
	}

	if (hlighted_roi) {
		for (k = 0; k < num_traces; k++) {
			if (pixColorCode[k] == hlighted_roi) {
				if (shift_direction == 0) {
					for (y = ROI_index[k][3]; y <= ROI_index[k][4]; y++) {
						for (x = ROI_index[k][1]; x <= ROI_index[k][2]; x++) {
							if ((SelectedMap[y][x] == 0 || SelectedMap[y][x] == hlighted_roi) && (SelectedMap[y + 1][x] == 0 || SelectedMap[y + 1][x] == hlighted_roi))
								SelectedMap[y][x] = SelectedMap[y + 1][x];
						}
					}
				}
				else if (shift_direction == 1) {
					for (y = ROI_index[k][3]; y <= ROI_index[k][4]; y++) {
						for (x = ROI_index[k][1]; x <= ROI_index[k][2]; x++) {
							if ((SelectedMap[y][x] == 0 || SelectedMap[y][x] == hlighted_roi) && (SelectedMap[y][x + 1] == 0 || SelectedMap[y][x + 1] == hlighted_roi))
								SelectedMap[y][x] = SelectedMap[y][x + 1];
						}
					}
				}
				else if (shift_direction == 2) {
					for (y = ROI_index[k][4]-1; y >= ROI_index[k][3]-1; y--) {
						for (x = ROI_index[k][1]; x <= ROI_index[k][2]; x++) {
							if ((SelectedMap[y][x] == 0 || SelectedMap[y][x] == hlighted_roi) && (SelectedMap[y - 1][x] == 0 || SelectedMap[y - 1][x] == hlighted_roi))
								SelectedMap[y][x] = SelectedMap[y - 1][x];
						}
					}
				}
				else {
					for (y = ROI_index[k][3]; y <= ROI_index[k][4]; y++) {
						for (x = ROI_index[k][2]-1; x >= ROI_index[k][1]-1; x--) {
							if ((SelectedMap[y][x] == 0 || SelectedMap[y][x] == hlighted_roi) && (SelectedMap[y][x - 1] == 0 || SelectedMap[y][x - 1] == hlighted_roi))
								SelectedMap[y][x] = SelectedMap[y][x - 1];
						}
					}
				}
				break;
			}
		}
	}
	else {
		if (shift_direction == 0) {
			for (y = 1; y < disp_num_row; y++) {
				ptr = &SelectedMap[y - 1][0];
				ptr0 = &SelectedMap[y][0];
				memcpy(ptr, ptr0, disp_num_col * sizeof(short int));
			}
			ptr = &SelectedMap[disp_num_row - 1][0];
			memset(ptr, 0, disp_num_col * sizeof(short int));
		}
		else if (shift_direction == 1) {
			for (y = 0; y < disp_num_row; y++) {
				ptr = &SelectedMap[y][0];
				for (x = 1; x < disp_num_col; x++)
					*ptr++ = *(ptr + 1);
				*ptr = 0;
			}
		}
		else if (shift_direction == 2) {
			for (y = disp_num_row - 1; y > 0; y--) {
				ptr = &SelectedMap[y][0];
				ptr0 = &SelectedMap[y - 1][0];
				memcpy(ptr, ptr0, disp_num_col * sizeof(short int));
			}
			ptr = &SelectedMap[0][0];
			memset(ptr, 0, disp_num_col * sizeof(short int));
		}
		else if (shift_direction == 3) {
			for (y = 0; y < disp_num_row; y++) {
				ptr = &SelectedMap[y][0] + disp_num_col - 1;
				for (x = 1; x < disp_num_col; x++)
					*ptr-- = *(ptr - 1);
				*ptr = 0;
			}
		}
	}
	getDispImage(0);
	getAllTraces();
}

void saveROIs(char *txtName)
{
	int i, j, k;
	FILE *fp;

	if (!FOPEN_S(fp, txtName, "w"))
		return;
	for (k = 0; k < num_traces; k++) {
		for (i = 0; i < 7; i++)
			fprintf(fp, "%d, ", ROI_index[k][i]);
		fprintf(fp, "\n");
		for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++) {
			for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++) {
				if (SelectedMap[j][i] == pixColorCode[k])
					fprintf(fp, "%d, %d\n", j, i);
			}
		}
	}
	fclose(fp);
}

void readROIs(char *txtName)
{
	int i, j, k, m, addROI_flag;
	FILE *fp;
	char buf[256], tmp_str[256];
	char *p;

	if (!FOPEN_S(fp, txtName, "r"))
		return;

	if (num_traces) {
		addROI_flag = !(MessageBox(main_window_handle,
			(LPCTSTR) "Do you want to remove exisiting ROIs?",
			(LPCTSTR) "Add ROIs",
			MB_YESNO |
			MB_ICONQUESTION |
			MB_SYSTEMMODAL) != IDYES);

		if (addROI_flag)
			clearTracePix();
	}

	k = num_traces;
	while (fgets(buf, 255, fp)) {
		for (i = 0; i < 7; i++) {
			if (p = strstr(buf, ",")) {
				strcpy(tmp_str, p + 1);
				*p = '\0';
				ROI_index[k][i] = atoi(buf);
				strcpy(buf, tmp_str);
			}
		}
		if (!ROI_index[k][5])
			break;
		for (m = 0; m < ROI_index[k][5]; m++) {
			if (fgets(buf, 255, fp)) {
				if (p = strstr(buf, ",")) {
					strcpy(tmp_str, p + 1);
					*p = '\0';
					j = atoi(buf);
					i = atoi(tmp_str);
					SelectedMap[j][i] = k+1;
					pixColorCode[k] = k + 1;
				}
			}
		}
		k++;
	}
	fclose(fp);
	num_traces = k;
	if (omit_ar)
		checkOmitwithROI();
	if (dataFile && (!frameLiveFlag || (disp_num_col == file_num_col && disp_num_row == file_num_row)))
		getAllTraces();
}

void eraseROI(int ROI_id, int save_erased)
{
	int i, j;

	if (num_traces) {
		for (j = ROI_index[ROI_id][3]; j < ROI_index[ROI_id][4]; j++)
			for (i = ROI_index[ROI_id][1]; i < ROI_index[ROI_id][2]; i++) {
				if (save_erased)
					SelectedMap[j][i] = -100;
				else
					SelectedMap[j][i] = 0;
			}
		for (int l = ROI_id; l < NUM_TRACES_MAX - 1; l++) {
			pixColorCode[l] = pixColorCode[l + 1];
			for (int m = 0; m < 5; m++)
				ROI_index[l][m] = ROI_index[l + 1][m];
			memcpy(smTrace[l], smTrace[l + 1], sizeof(smTrace[l]));
		}
		num_traces--;
	}
}

void undoLastROI()
{
	eraseROI(num_traces - 1, 1);
}

void setROI_index(int x1, int x2, int y1, int y2, int BNC_pix)
{
	pixColorCode[num_traces] = thisPixColor;
	if (BNC_pix > 0) {
		ROI_index[num_traces][0] = BNC_SELECT;
		ROI_index[num_traces][1] = BNC_pix - 1;
		ROI_index[num_traces][2] = BNC_pix - 1;
		ROI_index[num_traces][3] = disp_num_row;
		ROI_index[num_traces][4] = disp_num_row;
		ROI_index[num_traces][5] = 1;
	}
	else {
		if (add_roi_flag && num_traces) {
			num_traces--;
			ROI_index[num_traces][1] = min(ROI_index[num_traces][1], max(0, x1));
			ROI_index[num_traces][2] = max(ROI_index[num_traces][2], min(disp_num_col, x2));
			ROI_index[num_traces][3] = min(ROI_index[num_traces][3], max(0, y1));
			ROI_index[num_traces][4] = max(ROI_index[num_traces][4], min(disp_num_row, y2));
		}
		else {
			ROI_index[num_traces][0] = selectPixMode;
			ROI_index[num_traces][1] = max(0, x1);
			ROI_index[num_traces][2] = min(disp_num_col, x2);
			ROI_index[num_traces][3] = max(0, y1);
			ROI_index[num_traces][4] = min(disp_num_row, y2);
		}
	}
	num_traces++;
}

int showPixelVals(int mouseX, int mouseY)
{
	int x, y, x0, y0, dy;
	char mssg[256], str[256];

	if ((!dataExist && !seqFlag) || (hotTab == SM_MOVIEVIEW) || (hotTab == SM_CARDIOVIEW))
		return FALSE;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	dy = image_region_dy;

	if (seqFlag) {
		if (mouseX < seq_stt_x || mouseY < y0 || mouseX > seq_stt_x+seq_dx || mouseY > y0 + dy)
			return FALSE;

		double x_step = seq_zoom_f * seq_dx / num_seq_pnts;
		if (mouseY < seq_stt_y)
			y = 0;
		else
			y = max(0, (mouseY - seq_stt_y)/seq_y_step + 1);
		x = (int)((mouseX - seq_stt_x) / x_step + 0.5) + seq_zoom_x;
		int this_bit_cnt;
		for (int k = 0; k < sizeof(pat_ar) / sizeof(pat_ar[0]); k++) {
			if (x < func_pat_seq[k]) {
				x = x - func_pat_seq[k - 1] + this_bit_cnt;
				if (strcmp(func_ar[k - 1], ""))
					sprintf(str, "F - %s", func_ar[k - 1]);
				else if (strcmp(pat_ar[k - 1], ""))
					sprintf(str, "P - %s", pat_ar[k - 1]);
				else
					strcpy(str, "");
				break;
			}
			else
				strcpy(str, "");

			if (!k)
				this_bit_cnt = func_pat_seq[k];
			else
				this_bit_cnt += (func_pat_seq[k] - func_pat_seq[k - 1])*abs(seq_reps_ar[2][k - 1]);
		}

		D3DRectangle(x0 + getMeasureWinX() - 350, dy - 20, x0 + getMeasureWinX() - 20, dy - 5, colorBlack);
		UnlockGraphics();
		sprintf(mssg, "%s", str);
//		sprintf(mssg, "%s: %d (%s)", str, x, bitN_ar[y]);
		drawText(0, 0, mssg, x0 + getMeasureWinX() - 340, dy - 20, WHITE, omFontSmall, TRUE);
	}
	else {
		if (mouseX < x0 || mouseY < y0 || mouseX > x0 + image_region_dx || mouseY > y0 + dy)
			return FALSE;

		x = (int)(((double)mouseX - img_x0 - x0) / zoom_factor);	//+ 0.5); not needed 
		y = (int)(((double)mouseY - img_y0 - y0) / zoom_factor);
		if (x < 0 || x >= disp_num_col || y < 0 || y >= disp_num_row)
			return FALSE;

		D3DRectangle(image_region_dx - 310, dy - 23, image_region_dx - 200, dy - 5, colorBlack);
		UnlockGraphics();
		sprintf(mssg, "%.2lf (%d, %d)", dispImage[y][x], x + 1, y + 1);
		drawText(0, 0, mssg, image_region_dx - 300, dy - 20, WHITE, omFontSmall, TRUE);
	}

	return TRUE;
}

void setSelectedOmit(int y, int x, int color)
{
	int setIt = 0;
	if (!mk_omit_flag) {
		if (splitterMode) {
			if (draw_x0 < disp_num_col / 2) {
				if (x < disp_num_col / 2)
					setIt = 1;
			}
			else {
				if (x >= disp_num_col / 2)
					setIt = 1;
			}
		}
		else
			setIt = 1;
		if (setIt)
			SelectedMap[y][x] = color;
	}
	else
		*(omit_ar + y * disp_num_col + x) = (color > 0);
}

int getThresholdPix(int mouseX, int mouseY)
{
	int x0, y0, x, y, x1, y1, x2, y2;
	float *image_ptr;
	float thr_val;
	int i, j, k;
	double threshold_percent = 0.9;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	x = (int)(((double)mouseX - img_x0 - x0) / zoom_factor);	//+ 0.5); not needed 
	y = (int)(((double)mouseY - img_y0 - y0) / zoom_factor);
	if ((x < 0 || x >= disp_num_col || y < 0 || y >= disp_num_row))
		return FALSE;

	image_ptr = &dispImage[y][0] + x;
	thr_val = *image_ptr;
	if (!mk_omit_flag) {
		if (SelectedMap[y][x]) {
			k = SelectedMap[y][x] - 1;
			x1 = ROI_index[k][1];
			y1 = ROI_index[k][3];
			x2 = ROI_index[k][2];
			y2 = ROI_index[k][4];
		}
		else {
			if (num_traces) {
				for (k = 0; k < num_traces; k++) {
					if ((x > ROI_index[k][1]) && (x < ROI_index[k][2]) && (y > ROI_index[k][3]) && (y < ROI_index[k][4])) {
						x1 = ROI_index[k][1];
						y1 = ROI_index[k][3];
						x2 = ROI_index[k][2];
						y2 = ROI_index[k][4];
						goto thr_out;
					}
				}
			}
			clearTracePix();
			k = 0;
			pixColorCode[k] = 1;
			x1 = 1;
			y1 = 1;
			x2 = disp_num_col - 1;
			y2 = disp_num_row - 1;
			ROI_index[k][1] = x1;
			ROI_index[k][3] = y1;
			ROI_index[k][2] = x2;
			ROI_index[k][4] = y2;
			num_traces++;
		}
	thr_out:
		ROI_index[num_traces][0] = 0;

		for (j = y1; j < y2; j++) {
			image_ptr = &dispImage[j][0] + x1;
			for (i = x1; i < x2; i++) {
				if ((*image_ptr) >(short int)(threshold_percent*thr_val))
					setSelectedOmit(j, i, pixColorCode[k]);
				else
					setSelectedOmit(j, i, 0);
				image_ptr++;
			}
		}
		getTrace(k);
		if (trace_sub_mode) {
			if (trace_sub_mode == 1 && num_traces == 1)
				memcpy(subtractionTrace, smTrace[0], sizeof(subtractionTrace));
			scaledTraceSubOne(k);
		}
		paintWin(SM_TRACEWIN, PAINTINIT);
		getDispImage(1);
	}
	else {
		x1 = 1;
		y1 = 1;
		x2 = disp_num_col - 1;
		y2 = disp_num_row - 1;
		for (j = y1; j < y2; j++) {
			image_ptr = &dispImage[j][0] + x1;
			for (i = x1; i < x2; i++) {
				if ((*image_ptr) < (short int)(threshold_percent*thr_val))
					setSelectedOmit(j, i, 1);
				else
					setSelectedOmit(j, i, 0);
				image_ptr++;
			}
		}
		getDispImage(0);
	}
	return TRUE;
}

int getTracePix(int mouseX, int mouseY, int BNC_pix)
{
	int x, y, i, j, k, x0, y0, ii, jj;
	int min_x = 0, max_x = 0, min_y = 0, max_y = 0;

	if (well96_flag)
		clearTracePix();

	if (BNC_pix == 0) {
		x0 = getMeasureWinX0();
		y0 = getMeasureWinY0();
		x = (int)(((double)mouseX - img_x0 - x0) / zoom_factor);	//+ 0.5); not needed 
		y = (int)(((double)mouseY - img_y0 - y0) / zoom_factor);
		if ((x < 0 || x >= disp_num_col || y < 0 || y >= disp_num_row) && selectPixMode == KERNEL_MODE)
			return FALSE;
	}

	if (!add_roi_flag || BNC_pix > 0 || !num_traces) {
		if ((num_traces >= NUM_TRACES_MAX) && ((selectPixMode < DRAG_ERASE && BNC_pix == 0) || BNC_pix > 0)) {
			int isBNC = FALSE;
			for (i = 0; i < disp_num_col; i++) {
				if (SelectedMap[disp_num_row][i] == pixColorCode[0]) {
					SelectedMap[disp_num_row][i] = 0;
					OM[SM_BNC1 + i].attr = OM_COLD;
					isBNC = TRUE;
				}
			}
			if (!isBNC) {
				for (j = ROI_index[0][3]; j < ROI_index[0][4]; j++) {
					for (i = ROI_index[0][1]; i < ROI_index[0][2]; i++) {
						if (SelectedMap[j][i] == pixColorCode[0])
							SelectedMap[j][i] = 0;
					}
				}
			}
			eraseROI(0, 0);
		}

		int col_found;
		for (k = 0; k < NUM_TRACES_MAX; k++) {
			col_found = FALSE;
			for (i = 0; i < num_traces; i++) {
				if (pixColorCode[i] == k + 1) {
					col_found = TRUE;
					break;
				}
			}
			if (!col_found) {
				thisPixColor = k + 1;
				break;
			}
		}
	}

	if (BNC_pix > 0)
		SelectedMap[disp_num_row][BNC_pix-1] = thisPixColor;
	else if (BNC_pix < 0) {
		for (k = 0; k < num_traces; k++) {
			if (SelectedMap[disp_num_row][-BNC_pix-1] == pixColorCode[k]) {
				SelectedMap[disp_num_row][-BNC_pix-1] = 0;
				break;
			}
		}
		eraseROI(k, 0);
	}
	else if (selectPixMode == DRAW_MODE) { //Draw
		int last_x, last_y, added_y_cnt=0;
		int this_min_x, this_max_x;
		min_y = disp_num_row;
		max_y = 0;
		for (i = 0; i < d.ptr; i++) {
			traceX[i] = (int)(((double)traceX[i] - img_x0 - x0)/zoom_factor + 0.5);
			traceY[i] = (int)(((double)traceY[i] - img_y0 - y0)/zoom_factor + 0.5);
			if (min_y > traceY[i])
				min_y = traceY[i];
			if (max_y < traceY[i])
				max_y = traceY[i];
			if (i > 0) {
				if (abs(traceY[i]-last_y) > 1) {
					for (j = 1; j < abs(traceY[i]-last_y); j++) {
						if (traceY[i] > last_y) {
							traceY[d.ptr+added_y_cnt] = last_y + j;
							traceX[d.ptr+added_y_cnt] = last_x + j*(traceX[i]-last_x)/(traceY[i]-last_y);
						}
						else {
							traceY[d.ptr+added_y_cnt] = traceY[i] + j;
							traceX[d.ptr+added_y_cnt] = traceX[i] + j*(traceX[i]-last_x)/(traceY[i]-last_y);
						}
						added_y_cnt++;
					}
				}
			}
			last_x = traceX[i];
			last_y = traceY[i];
		}
		min_x = disp_num_col;
		max_x = 0;
		for (y = min_y; y <= min(max_y, disp_num_row-1); y++) {
			this_min_x = disp_num_col;
			this_max_x = 0;
			for (i = 0; i < d.ptr+added_y_cnt; i++) {
				if (traceY[i] == y) {
					if (this_min_x > traceX[i])
						this_min_x = traceX[i];
					if (this_max_x < traceX[i])
						this_max_x = traceX[i];
				}
			}
			for (x = this_min_x; x <= min(this_max_x, disp_num_col - 1); x++)
				setSelectedOmit(y, x, thisPixColor);
			//	SelectedMap[y][x] = thisPixColor;
			if (min_x > this_min_x)
				min_x = this_min_x;
			if (max_x < this_max_x)
				max_x = this_max_x;
		}
	}
	else if (selectPixMode == DRAG_MODE || selectPixMode == DRAG_ERASE) { //Drag Box
		int boxX0, boxY0;
		boxX0 = (int)(((double)dragboxX0 - img_x0 - x0)/zoom_factor);
		boxY0 = (int)(((double)dragboxY0 - img_y0 - y0)/zoom_factor);
		for (j = min(boxY0, y); j <= min(max(boxY0, y),disp_num_row-1); j++) {
			for (i = min(boxX0, x); i <= min(max(boxX0, x), disp_num_col - 1); i++) {
				if (selectPixMode == DRAG_MODE)
					setSelectedOmit(j, i, thisPixColor);
				//		SelectedMap[j][i] = thisPixColor;
				else if (selectPixMode == DRAG_ERASE) {
					setSelectedOmit(j, i, 0);
				//		SelectedMap[j][i] = 0;
				}
			}
		}
		if (selectPixMode == DRAG_ERASE) {	//erase
			if (!mk_omit_flag) {
				int selectFound;
				for (k = 0; k < num_traces; k++) {
					selectFound = FALSE;
					for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++) {
						for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++) {
							if (SelectedMap[j][i] == pixColorCode[k]) {
								selectFound = TRUE;
								break;
							}
						}
						if (selectFound) {
							getTrace(k);
							if (trace_sub_mode)
								scaledTraceSubOne(k);
							break;
						}
					}
					if (!selectFound) {
						eraseROI(k, 0);
						k--;
					}
					else {
						// find max min x y
						int this_min_x = ROI_index[k][2], this_max_x = ROI_index[k][1], this_min_y = ROI_index[k][4], this_max_y = ROI_index[k][3];
						for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++) {
							for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++) {
								if (SelectedMap[j][i] == pixColorCode[k]) {
									if (this_min_x > i)
										this_min_x = i;
									if (this_max_x < i)
										this_max_x = i;
									if (this_min_y > j)
										this_min_y = j;
									if (this_max_y < j)
										this_max_y = j;
								}
							}
						}
						ROI_index[k][1] = this_min_x;
						ROI_index[k][2] = this_max_x + 1;
						ROI_index[k][3] = this_min_y;
						ROI_index[k][4] = this_max_y + 1;
					}
				}
			}
		}
		else {
			min_x = min(boxX0, x);
			max_x = max(boxX0, x);
			min_y = min(boxY0, y);
			max_y = max(boxY0, y);
		}
	}	
	else if (selectPixMode == KERNEL_MODE) { //Kernel Select
		// get circle
		int r = sm_kernel_size/2;
		for (j = 0; j <= r; j++) {
			for (i = 0; i <= r; i++) {
				if ((int)(sqrt((j-sm_kernel_size/2.0)*(j-sm_kernel_size/2.0)+i*i)+0.5) > (int)(sm_kernel_size/2.0+0.5)) 
					break;
			}
			i--;

			jj = min(max(0, j+y-r), disp_num_row-1);
			for (ii = min(max(0, x - i), disp_num_col - 1); ii <= min(max(0, x + i), disp_num_col - 1); ii++)
				setSelectedOmit(jj, ii, thisPixColor);
			jj = min(max(0, y+r-j), disp_num_row-1);
			for (ii = min(max(0, x-i), disp_num_col-1); ii <= min(max(0, x+i), disp_num_col-1); ii++) 
				setSelectedOmit(jj, ii, thisPixColor);
		}
		if (sm_kernel_size % 2) {
			jj = min(max(0, y), disp_num_row-1);
			for (ii = min(max(0, x-r), disp_num_col-1); ii <= min(max(0, x+r), disp_num_col-1); ii++) 
				setSelectedOmit(jj, ii, thisPixColor);
		}
		min_x = x - r;
		max_x = x + r;
		min_y = y - r;
		max_y = y + r;
	}
	max_x++;
	max_y++;
	if (!mk_omit_flag) {
		int new_roi;
		if (omit_ar && !frameLiveFlag) {
			signed short *omit_ptr;
			new_roi = 0;
			for (j = min_y; j < max_y; j++) {
				omit_ptr = omit_ar + j * disp_num_col + min_x;
				for (i = min_x; i < max_x; i++) {
					if (SelectedMap[j][i] && !(*omit_ptr))
						new_roi = 1;
					if (*omit_ptr)
						SelectedMap[j][i] = 0;
					omit_ptr++;
				}
			}
		}
		else
			new_roi = 1;
		if (((selectPixMode < DRAG_ERASE && BNC_pix == 0) || BNC_pix > 0) && new_roi) {
			setROI_index(min_x, max_x, min_y, max_y, BNC_pix);
		}

		if (selectPixMode != DRAG_ERASE) {
			getTrace(num_traces - 1);
			if (trace_sub_mode) {
				if (trace_sub_mode == 1 && num_traces == 1)
					memcpy(subtractionTrace, smTrace[0], sizeof(subtractionTrace));
				scaledTraceSubOne(num_traces - 1);
			}

			if (splitterMode && !BNC_pix) {
				thisPixColor = num_traces + 1;
				for (j = min_y; j < max_y; j++)
					for (i = min_x; i < max_x; i++) {
						if (SelectedMap[j][i] > 0) {
							if (draw_x0 < disp_num_col / 2)
								SelectedMap[j][i + disp_num_col / 2] = thisPixColor;
							else
								SelectedMap[j][i - disp_num_col / 2] = thisPixColor;
						}
					}
				if (draw_x0 < disp_num_col / 2)
					setROI_index(min_x + disp_num_col / 2, max_x + disp_num_col / 2, min_y, max_y, BNC_pix);
				else
					setROI_index(min_x - disp_num_col / 2, max_x - disp_num_col / 2, min_y, max_y, BNC_pix);
				getTrace(num_traces - 1);
				if (trace_sub_mode) {
					if (trace_sub_mode == 1 && num_traces == 1)
						memcpy(subtractionTrace, smTrace[0], sizeof(subtractionTrace));
					scaledTraceSubOne(num_traces - 1);
				}
			}
		}

		if (well96_flag) {
			int this_max_x, this_min_x, this_max_y, this_min_y;
			int well_spacing = well96_flag;
			for (j = 0; j < well_y; j++)
				for (i = 0; i < well_x; i++) {
					if (!i && !j)
						continue;
					this_max_x = min(disp_num_col, max_x + well_spacing * i);
					this_min_x = min_x + well_spacing * i;
					this_max_y = min(disp_num_row, max_y + well_spacing * j);
					this_min_y = min_y + well_spacing * j;
					if ((this_min_x < disp_num_col) && (this_min_y < disp_num_row)) {
						thisPixColor++;
						for (jj = this_min_y; jj < this_max_y; jj++)
							for (ii = this_min_x; ii < this_max_x; ii++)
								if (SelectedMap[jj - well_spacing * j][ii - well_spacing * i] > 0)
									SelectedMap[jj][ii] = thisPixColor;
						setROI_index(this_min_x, this_max_x, this_min_y, this_max_y, 0);
						getTrace(num_traces - 1);
						if (trace_sub_mode) {
							if (trace_sub_mode == 1 && num_traces == 1)
								memcpy(subtractionTrace, smTrace[0], sizeof(subtractionTrace));
							scaledTraceSubOne(num_traces - 1);
						}
					}
				}
		}
		paintWin(SM_TRACEWIN, PAINTINIT);
		getDispImage(1);
	}
	else
		getDispImage(0);
	return TRUE;
}

void clearTracePix()
{
	memset(SelectedMap, 0, sizeof(SelectedMap));
	memset(ROI_index, 0, sizeof(ROI_index));
	resetBNCs();
	num_traces = 0;
	thisPixColor = 1;
}

void getAllTraces()
{
	setLiveFlag(0);
	for (int k = 0; k < num_traces; k++)
		getTrace(k);
}

void scaledTraceSubOne(int k)
{
	float scale_factor;

	scale_factor = (smTrace[k][trace_sub_f1] - smTrace[k][trace_sub_f2])/(subtractionTrace[trace_sub_f1] - subtractionTrace[trace_sub_f2]);
	for (int i = 0; i < numTraceFrames; i++) {
		smTrace[k][i] = smTrace[k][i] - (subtractionTrace[i] - subtractionTrace[trace_sub_f2]) * scale_factor;
	}
}

void resetDataProc()
{
	trace_sub_mode = 0;
	SendMessage(GetDlgItem(tools_window_handle, TRACE_SUB1), BM_SETCHECK, 1, 0);
	SendMessage(GetDlgItem(tools_window_handle, TRACE_SUB2), BM_SETCHECK, 0, 0);
	SendMessage(GetDlgItem(tools_window_handle, TRACE_SUB3), BM_SETCHECK, 0, 0);
	movie_scale_mode = 0;
	SendMessage(GetDlgItem(tools_window_handle, MOVIE_SCALE1), BM_SETCHECK, 1, 0);
	SendMessage(GetDlgItem(tools_window_handle, MOVIE_SCALE2), BM_SETCHECK, 0, 0);
	SendMessage(GetDlgItem(tools_window_handle, MOVIE_SCALE3), BM_SETCHECK, 0, 0);
	SendMessage(GetDlgItem(tools_window_handle, MOVIE_SCALE4), BM_SETCHECK, 0, 0);
}

static int num_steps = 30;
HWND createProgBar()
{
	// making a progress bar
	RECT rcClient;  // Client area of parent window.
	int cyVScroll = 50;  // Height of scroll bar arrow.
	GetClientRect(main_window_handle, &rcClient);
	HWND hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE | WS_CAPTION, rcClient.left + rcClient.right / 6,
		rcClient.top + (rcClient.bottom - cyVScroll) * 2 / 5,
		rcClient.right / 4, cyVScroll,
		main_window_handle, NULL, NULL, NULL);
	SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, num_steps));
	SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
	return hwndPB;
}

int scaledTraceSub(int applyFlag, char *fName)
{
	FILE *fp;
	char buf[256], tmp_str[512], tmp_str2[512];
	char *p;
	int frm_cnt;

	if (!num_traces && trace_sub_mode == 1) {
		MessageBox(main_window_handle, "Please select a ROI, and then click apply.", "message", MB_OK);
		return false;
	}
	getAllTraces();
	if (trace_sub_mode) {
		if (trace_sub_mode == 1)
			memcpy(subtractionTrace, smTrace[0], sizeof(subtractionTrace));
		else {
			if (!FOPEN_S(fp, fName, "r"))
				return false;
			strcpy(tmp_str, "");
			frm_cnt = 0;
			while (fgets(buf, 255, fp)) {
				strcat(tmp_str, buf);
				while (p = strstr(tmp_str, ",")) {
					strcpy(tmp_str2, p+1);
					*p = '\0';
					subtractionTrace[frm_cnt] = (float)atof(tmp_str);
					frm_cnt++;
					strcpy(tmp_str, tmp_str2);
				}
			}
			if (strcmp(tmp_str, "")) {
				subtractionTrace[frm_cnt] = (float)atof(tmp_str);
				frm_cnt++;
			}
			fclose(fp);
			if (frm_cnt != numTraceFrames) {
				MessageBox(main_window_handle, "This trace file does not have the same number of frames as current file.", "message", MB_OK);
				return false;
			}
		}
		if (applyFlag == 0) {
			for (int k = 0; k < num_traces; k++)
				scaledTraceSubOne(k);
		}
		else if (applyFlag == 1) {
			signed short int *ptr, *dk_ptr;
			long len = disp_num_col * disp_num_row;

			HWND hwndPB = createProgBar();
			int step_cnt = 0;
			double step_size = ((double)num_steps) / len;
			char str[256];
			sprintf(str, "Scaled Subtraction for pixel %d of %d", 0, len);
			SetWindowText(hwndPB, str);

			for (long i = 0; i < len; i++) {
				ptr = image_data + i;
				for (int m = 0; m < numTraceFrames; m++) {
					smTrace[0][m] = *ptr;
					ptr += len;
				}
				scaledTraceSubOne(0);
				ptr = image_data + i;
				dk_ptr = dark_data + i;
				for (int m = 0; m < numTraceFrames; m++) {
					*ptr = (short int)smTrace[0][m] + *dk_ptr;
					ptr += len;
				}
				sprintf(str, "Scaled Subtraction for pixel %d of %d", i, len);
				SetWindowText(hwndPB, str);
				while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
			}
			DestroyWindow(hwndPB);

			getAllTraces();
			resetDataProc();
			trace_subtracted = 1;
			EnableTools(tools_window_handle);
			getStreamFrame();
		}
	}
	paintWin(SM_TRACEWIN, PAINTINIT);
	return true;
}

int FilterWholeData()
{
	int m, l, j, k; 
	long bin_val;

	signed short int *ptr, *dk_ptr, *b_ptr, *k_ptr, *fr_ptr;
	long len;

	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size;
	char str[256];
//	sprintf(str, "Scaled Subtraction for pixel %d of %d", 0, len);
//	SetWindowText(hwndPB, str);

/*	if (NDR_subtraction_frames) {
		step_cnt = 0;
		step_size = ((double)num_steps) / numTraceFrames;
		SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
		ptr = image_data;
		for (m = 0; m < numTraceFrames-4; m++) {
			for (long i = 0; i < len; i++) {
			//	*ptr = (*ptr)*4 + (*(ptr + len))*2 + *(ptr + 2 * len);
				*ptr = (*ptr)*max(max(1, NDR_subtraction_frames - m), (NDR_subtraction_frames - (numTraceFrames - m - 1)));
				for (k = 0; k < min(min(NDR_subtraction_frames, m), (numTraceFrames - m - 1)); k++)
					*ptr += *(ptr + len * k);
				ptr++;
			}
			sprintf(str, "Rolling summing for frame %d of %d", m, numTraceFrames);
			SetWindowText(hwndPB, str);
			while ((int)(m*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
		}
		ptr = dark_data;
		for (long i = 0; i < len; i++)
			*ptr++ *= NDR_subtraction_frames;
	}*/
	
	if (spatial_bin > 1) {
		step_cnt = 0;
		step_size = ((double)num_steps) / numTraceFrames;
		SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
		len = disp_num_col * disp_num_row;
		fr_ptr = image_data;
		b_ptr = image_data;
		for (int i = 0; i < numTraceFrames; i++) {
			ptr = fr_ptr;
			for (m = 0; m < disp_num_row / spatial_bin; m++) {
				for (l = 0; l < disp_num_col / spatial_bin; l++) {
					bin_val = 0;
					k_ptr = ptr;
					for (j = 0; j < spatial_bin; j++) {
						for (k = 0; k < spatial_bin; k++)
							bin_val += *k_ptr++;
						k_ptr += disp_num_col - spatial_bin;
					}
					*b_ptr++ = (short)(bin_val / (spatial_bin*spatial_bin));
					ptr += spatial_bin;
				}
				ptr += disp_num_col * (spatial_bin - 1);
			}
			fr_ptr += len;
			sprintf(str, "Spatial binning frame %d of %d", i, numTraceFrames);
			SetWindowText(hwndPB, str);
			while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
		}
		ptr = dark_data;
		b_ptr = dark_data;
		for (m = 0; m < disp_num_row / spatial_bin; m++) {
			for (l = 0; l < disp_num_col / spatial_bin; l++) {
				bin_val = 0;
				k_ptr = ptr;
				for (j = 0; j < spatial_bin; j++) {
					for (k = 0; k < spatial_bin; k++)
						bin_val += *k_ptr++;
					k_ptr += disp_num_col - spatial_bin;
				}
				*b_ptr++ = (short)(bin_val / (spatial_bin*spatial_bin));
				ptr += spatial_bin;
			}
			ptr += disp_num_col * (spatial_bin - 1);
		}
		disp_num_col /= spatial_bin;
		disp_num_row /= spatial_bin;
		file_num_col = disp_num_col;
		file_num_row = disp_num_row;
		spatial_bin = 1;
		sprintf(str, "%d", spatial_bin);
		SetDlgItemText(tools_window_handle, SP_BIN_NUM, str);
		resetDispSettings();
	}

	if ((trace_lpass > 0.0) || (trace_hpass > 0.5) || exp_sub_flag) {
		len = disp_num_col * disp_num_row;
		step_cnt = 0;
		step_size = ((double)num_steps) / len;
		SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
		for (long i = 0; i < len; i++) {
			ptr = image_data + i;
			for (m = 0; m < numTraceFrames; m++) {
				this_smTrace[m] = *ptr;
				if (tracePolarity < 0)
					this_smTrace[m] = this_smTrace[0] * 2 - this_smTrace[m];
				ptr += len;
			}
			FilterTrace();
			ptr = image_data + i;
			dk_ptr = dark_data + i;
			for (m = 0; m < numTraceFrames; m++) {
				*ptr = (short int)this_smTrace[m];
				ptr += len;
			}
			sprintf(str, "Filter whole dataset for pixel %d of %d", i, len);
			SetWindowText(hwndPB, str);
			while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
		}
	}

	DestroyWindow(hwndPB);

	trace_lpass = 0.0;
	trace_hpass = 0.0;
	exp_sub_flag = 0;
	if (disp_setting_handle)
		SendMessage(GetDlgItem(disp_setting_handle, EXP_SUB_ID), BM_SETCHECK, 0, 0);
	OM[SM_LOWPASS].attr = 0;
	OM[SM_HIGHPASS].attr = 0;
	if (tracePolarity < 0) {
		tracePolarity = 1;
		paintWin(SM_TPOLARITY, PAINTINIT);
	}

	getAllTraces();
//	resetDataProc();
	filter_applied = 1;
	EnableTools(tools_window_handle);
	getStreamFrame();
	paintWin(SM_TRACEWIN, PAINTINIT);
	return true;
}

static int trace_factor;
int getTrace(int k)
{
	int i, j, len, num_av=0, ii, isBNC=FALSE;
	signed short int *pt, *dk_pt;
	float this_av, RLI_val, this_float_val, rlisub_ref_val;
	float *fpt;
	long this_dark=0;

//	if (frameLiveFlag)
	if (!dataFile)
		return 0;
	for (i = 0; i < disp_num_col; i++) {
		if (SelectedMap[disp_num_row][i] == pixColorCode[k]) {
			pt = (signed short *)BNC_data;
			pt += i*numTraceFrames*file_BNC_ratio;
			ii = 0;
			while (ii < numTraceFrames*file_BNC_ratio) {
				smTrace[k][ii] = ((float)(*pt))/2;
				pt++;
				ii++;
			}
			isBNC = TRUE;
			num_av = 1;
			break;
		}
	}
	if (!isBNC) {
		len = disp_num_col*disp_num_row;
		for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++) {
			for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++) {
				if (SelectedMap[j][i] == pixColorCode[k]) {
					fpt = (float32 *)image_data_f;
					pt = (signed short *)image_data;
					if (float_flag) {
						fpt += i + j * disp_num_col;
						this_float_val = *fpt;
					}
					else {
						pt += i + j * disp_num_col;
						this_float_val = (float32)(*pt);
					}
					ii = 0;
					dk_pt = (signed short *)dark_data;
					if (d_darkExist) {
						long int dk_inc = (long int)disp_num_col*j + i;
						dk_pt += dk_inc;
						this_dark += *dk_pt;
					}
					while (ii < numTraceFrames) {
						if (!num_av)
							smTrace[k][ii] = this_float_val;
						else
							smTrace[k][ii] = smTrace[k][ii] + this_float_val;
						if (d_darkExist)
							smTrace[k][ii] -= *dk_pt;
						ii++;
						if (ii == numTraceFrames)
							break;
						if (float_flag) {
							fpt += len;
							this_float_val = *fpt;
						}
						else {
							pt += len;
							this_float_val = (float32)(*pt);
						}
					}
					num_av++;
				}
			}
		}
		if (NDR_trace_sub && NDR_subtraction_flag) {
			int this_NDR_break, break_on = 0;

			j = 0;
			this_NDR_break = NDR_reset_ar[j];
			for (i = 0; i < numTraceFrames - NDR_subtraction_frames; i++) {
				if ((this_NDR_break > i) && (this_NDR_break <= i+NDR_subtraction_frames)) {
					smTrace[k][i] = smTrace[k][i+NDR_subtraction_frames] - smTrace[k][this_NDR_break] + smTrace[k][this_NDR_break-1] - smTrace[k][i];
					break_on = 1;
				}
				else {
					smTrace[k][i] = smTrace[k][i+NDR_subtraction_frames] - smTrace[k][i];
					if (break_on) {
						j++;
						this_NDR_break = NDR_reset_ar[j];
						break_on = 0;
					}
				}
			}
			for (i = numTraceFrames - 1; i >= NDR_subtraction_frames; i--)
				smTrace[k][i] = smTrace[k][i-NDR_subtraction_frames];
			for (; i >= 0; i--)
				smTrace[k][i] = smTrace[k][NDR_subtraction_frames];
		}
	}
	//********** for RLI div
	if (RLI_div_flag) {
		this_av = 0;
		for (i = RLI_f1; i <= RLI_f2; i++ )
			this_av += smTrace[k][i];
		RLI_val = this_av/(RLI_f2 - RLI_f1 + 1);
		this_av = 0;
		for (i = ref_frame1; i <= ref_frame2; i++)
			this_av += smTrace[k][i];
		rlisub_ref_val = this_av / (ref_frame2 - ref_frame1 + 1);
	}
	//************
	float trace_max = -32000;
	if (isBNC)
		trace_factor = file_BNC_ratio;
	else
		trace_factor = 1;
	for (i = 0; i < numTraceFrames*trace_factor; i++) {
		if (RLI_div_flag)
			smTrace[k][i] = (smTrace[k][i]- rlisub_ref_val)/RLI_val;
		else if (num_av)
			smTrace[k][i] = smTrace[k][i]/num_av;
		if (tracePolarity < 0)
			smTrace[k][i] = smTrace[k][0]*2 - smTrace[k][i];
		if (acquiring_flag && spike_triggered) {
			if (trace_max < smTrace[k][i]) {
				trace_max = smTrace[k][i];
				spike_pos_ar[av_file_cnt] = i/ trace_factor;
				curDispFrame = i/ trace_factor;
			}
		}
	}
	if (num_av)
		this_dark /= num_av;
	ROI_index[k][6] = (int)this_dark;
	return isBNC;
}

double get_traceYGain(int gain)
{
	double val;

	val = pow(2,gain/50.0)/500.0;
	if (RLI_div_flag && RLI_mean > 0.0)
		val *= RLI_mean;
	return val;
}

void kernel_spatial_filter(int movie_fr, int hpass)
{
	int x, y, num_sum_pix, kernel_x, kernel_y, i, j, kernel_total, this_x, this_y, k;
	signed short *image_ptr_m, *kernel_ptr_m, *flt_ptr_m, *mov_ptr;
	signed short *filtered_img_m;
	float *image_ptr, *kernel_ptr, *flt_ptr;
	float *filtered_img;
	double *sp_kernel, *sp_k_ptr;
	double mean_kernel[][3] = { { 1.0 / 9, 1.0 / 9, 1.0 / 9 },{ 1.0 / 9, 1.0 / 9, 1.0 / 9 },{ 1.0 / 9, 1.0 / 9, 1.0 / 9 } };
	double lp_gaussian_kernel[][3] = { { 1.0 / 16, 1.0 / 8, 1.0 / 16 },{ 1.0 / 8, 1.0 / 4, 1.0 / 8 },{ 1.0 / 16, 1.0 / 8, 1.0 / 16 } };
	double hp_laplacian_kernel3[][3] = { { 0.0, 1.0, 0.0 },{ 1.0, -4.0,  1.0 },{ 0.0, 1.0, 0.0 } };
	double hp_laplacian_kernel5[][5] = {	{ 0.0, 0.0, 1.0,  0.0, 0.0 },
											{ 0.0, 1.0, 2.0,  1.0, 0.0 },
											{ 1.0, 2.0,-16.0, 2.0, 1.0 },
											{ 0.0, 1.0, 2.0,  1.0, 0.0 },
											{ 0.0, 0.0, 1.0,  0.0, 0.0 } };
	double hp_laplacian_kernel7[][7] = {	{ 0.0, 1.0,  1.0,  2.0,  1.0, 1.0, 0.0 },
											{ 1.0, 2.0,  3.0,  4.0,  3.0, 2.0, 1.0 },
											{ 1.0, 3.0,  4.0,-12.0,  4.0, 3.0, 1.0 },
											{ 2.0, 4.0,-12.0,-40.0,-12.0, 4.0, 2.0 },
											{ 1.0, 3.0,  4.0,-12.0,  4.0, 3.0, 1.0 },
											{ 1.0, 2.0,  3.0,  4.0,  3.0, 2.0, 1.0 },
											{ 0.0, 1.0,  1.0,  2.0,  1.0, 1.0, 0.0 }};
	double hp_laplacian_kernel9[][9] = {	{ 0.0, 1.0, 1.0,  2.0,  2.0,  2.0, 1.0, 1.0, 0.0 },
											{ 1.0, 2.0, 4.0,  5.0,  5.0,  5.0, 4.0, 2.0, 1.0 },
											{ 1.0, 4.0, 5.0,  3.0,  0.0,  3.0, 5.0, 4.0, 1.0 },
											{ 2.0, 5.0, 3.0,-12.0,-24.0,-12.0, 3.0, 5.0, 2.0 },
											{ 2.0, 5.0, 0.0,-24.0,-40.0,-24.0, 0.0, 5.0, 2.0 },
											{ 2.0, 5.0, 3.0,-12.0,-24.0,-12.0, 3.0, 5.0, 2.0 },
											{ 1.0, 4.0, 5.0,  3.0,  0.0,  3.0, 5.0, 4.0, 1.0 },
											{ 1.0, 2.0, 4.0,  5.0,  5.0,  5.0, 4.0, 2.0, 1.0 },
											{ 0.0, 1.0, 1.0,  2.0,  2.0,  2.0, 1.0, 1.0, 0.0 }};
	double knl_sum;
	float pixel;
	int this_num_col, this_num_row, iteration;
	double pi = 3.1415926535;

	if (hpass) {
		kernel_x = sp_hpass * 2 +1;
		kernel_y = sp_hpass * 2 + 1;
	}
	else if (sp_lp_type == 1) {
		kernel_x = sizeof(mean_kernel[0]) / sizeof(mean_kernel[0][0]);
		kernel_y = sizeof(mean_kernel) / sizeof(mean_kernel[0]);
	}
	else if (sp_lp_type == 2) {
		kernel_x = sizeof(lp_gaussian_kernel[0]) / sizeof(lp_gaussian_kernel[0][0]);
		kernel_y = sizeof(lp_gaussian_kernel) / sizeof(lp_gaussian_kernel[0]);
	}
	kernel_total = kernel_x * kernel_y;
	sp_kernel = (double *)_aligned_malloc(kernel_total * sizeof(double), 2);

	iteration = sp_lpass;
	if (hpass) {
		if (sp_hpass == 1)
			memcpy(sp_kernel, hp_laplacian_kernel3, kernel_total * sizeof(double));
		else if (sp_hpass == 2)
			memcpy(sp_kernel, hp_laplacian_kernel5, kernel_total * sizeof(double));
		else if (sp_hpass == 3)
			memcpy(sp_kernel, hp_laplacian_kernel7, kernel_total * sizeof(double));
		else 
			memcpy(sp_kernel, hp_laplacian_kernel9, kernel_total * sizeof(double));
		iteration = 1;
	}
	else if (sp_lp_type == 1)
		memcpy(sp_kernel, mean_kernel, kernel_total * sizeof(double));
	else if (sp_lp_type == 2)
		memcpy(sp_kernel, lp_gaussian_kernel, kernel_total * sizeof(double));

	if (!movie_fr) {
		resetHisto();
		setScaleDim();
		max_pix = -32765;
		min_pix = 32765;
		max_pix2 = -32765;
		min_pix2 = 32765;
		this_num_col = disp_num_col;
		this_num_row = disp_num_row;
		filtered_img = (float *)_aligned_malloc((unsigned long)this_num_col*this_num_row * sizeof(float), 2);
		for (k = 0; k < iteration; k++) {
			flt_ptr = filtered_img;
			for (y = 0; y < this_num_row; y++) {
				for (x = 0; x < this_num_col; x++) {
					num_sum_pix = 0;
					knl_sum = 0.0;
					sp_k_ptr = sp_kernel;
					for (j = 0; j < kernel_y; j++) {
						this_y = y - kernel_y / 2 + j;
						if ((this_y >= 0) && (this_y < this_num_row)) {
							kernel_ptr = &dispImage[this_y][0];
							for (i = 0; i < kernel_x; i++) {
								this_x = x - kernel_x / 2 + i;
								if ((this_x >= 0) && (this_x < this_num_col)) {
									knl_sum += (*(kernel_ptr + this_x))*(*sp_k_ptr++);
									num_sum_pix++;
								}
								else
									sp_k_ptr++;
							}
						}
						else
							sp_k_ptr += kernel_x;
					}
					if (num_sum_pix < kernel_total)
						*flt_ptr = (signed short)(knl_sum * kernel_total / num_sum_pix + 0.5);
					else
						*flt_ptr = (signed short)(knl_sum + 0.5);
					flt_ptr++;
				}
			}
			flt_ptr = filtered_img;
			for (y = 0; y < this_num_row; y++) {
				image_ptr = &dispImage[y][0];
				for (x = 0; x < this_num_col; x++) {
					*image_ptr++ = *flt_ptr++;
					if (k == sp_lpass - 1) {
						pixel = *(image_ptr - 1);
						getHistoMinMax(pixel, x, y);
						sumHistoPix(pixel, x, y);
					}
				}
			}
		}
		_aligned_free(filtered_img);
	}
	else {
		mov_ptr = movie_data;
		this_num_col = mov_num_col;
		this_num_row = mov_num_row;
		mov_ptr += (movie_fr - 1) * this_num_col*this_num_row;
		filtered_img_m = (signed short int *)_aligned_malloc((unsigned long)this_num_col*this_num_row * sizeof(signed short int), 2);
		for (k = 0; k < iteration; k++) {
			flt_ptr_m = filtered_img_m;
			for (y = 0; y < this_num_row; y++) {
				for (x = 0; x < this_num_col; x++) {
					num_sum_pix = 0;
					knl_sum = 0.0;
					sp_k_ptr = sp_kernel;
					for (j = 0; j < kernel_y; j++) {
						this_y = y - kernel_y / 2 + j;
						if ((this_y >= 0) && (this_y < this_num_row)) {
							kernel_ptr_m = mov_ptr + this_y * this_num_col;
							for (i = 0; i < kernel_x; i++) {
								this_x = x - kernel_x / 2 + i;
								if ((this_x >= 0) && (this_x < this_num_col)) {
									knl_sum += (*(kernel_ptr_m + this_x))*(*sp_k_ptr++);
									num_sum_pix++;
								}
								else
									sp_k_ptr++;
							}
						}
						else
							sp_k_ptr += kernel_x;
					}
					if (num_sum_pix < kernel_total)
						*flt_ptr_m = (signed short)(knl_sum * kernel_total / num_sum_pix + 0.5);
					else
						*flt_ptr_m = (signed short)(knl_sum + 0.5);
					flt_ptr_m++;
				}
			}
			flt_ptr_m = filtered_img_m;
			for (y = 0; y < this_num_row; y++) {
				image_ptr_m = mov_ptr + y * this_num_col;
				for (x = 0; x < this_num_col; x++) {
					*image_ptr_m++ = *flt_ptr_m++;
				}
			}
		}
		_aligned_free(filtered_img_m);
	}
	_aligned_free(sp_kernel);
}

void spatial_filter(int movie_fr)
{
	if (sp_lp_type)
		kernel_spatial_filter(movie_fr, 0);
	if (sp_hp_type)
		kernel_spatial_filter(movie_fr, 1);
}

float trace_sv[MAX_DATA_FRAMES+500], trace_sv2[MAX_DATA_FRAMES+500];
void lpass_gauss_filter()
{
	int i, m, numcoef;
	double coef[1000];
	double t0, test, sum;
	double cutoff = trace_lpass*SM_exposure;
	double sigma = 0.1325/cutoff;
	double this_val;

	if (sigma < 0.61) {
		numcoef = 1;
		coef[0] = sigma*sigma/2.0;
		coef[1] = 1.0-2.0*coef[0];
		coef[2] = coef[0];
	}
	else {
		numcoef = min((long)(4*sigma),sizeof(coef)/(2*sizeof(coef[0]))-1);
		t0 = -1.0/(2.0*sigma*sigma);
		coef[numcoef] = 1.0;
		sum = 1.0;
		for (i = 1; i <= numcoef; i++) {
			test = i*i*t0;
			coef[numcoef-i] = exp (test);
			coef[numcoef+i] = coef[numcoef-i];
			sum = sum + 2.0*coef[numcoef-i];
		}
		for (i = 1; i <= numcoef*2; i++) 
			coef[i] = coef[i]/sum;
	}

	int add_on = 100;
	long new_length = 1;
	while (new_length < max(numcoef*4, numTraceFrames+2*add_on)) 
		new_length = new_length*2;
	add_on = (new_length - numTraceFrames)/2;
	for (i = numTraceFrames-1; i >= 0; i--)
		this_smTrace[i+add_on] = this_smTrace[i];
	for (i = 0; i < add_on; i++)
		this_smTrace[i] = this_smTrace[add_on];
	for (i = numTraceFrames+add_on; i < new_length; i++)
		this_smTrace[i] = this_smTrace[numTraceFrames + add_on - 1];

	memcpy(trace_sv, this_smTrace, sizeof(this_smTrace));
    for(i = numcoef; i <= new_length-numcoef; i++){
        this_val = 0.0;
        for(m = 0; m <= numcoef*2; m++)
            this_val += trace_sv[i+m-numcoef] * coef[m];
		this_smTrace[i] = (float)this_val;
    }
    for(i = 0; i < numTraceFrames; i++)
		this_smTrace[i] = this_smTrace[i+add_on];
}

void hpass_gauss_filter()
{
	int i, j, m, numcoef;
	float coef[2000];
	float t0, test, sum;
	float cutoff = (float)(trace_hpass * SM_exposure);
	float sigma = 0.1325f / cutoff;
	float this_val;

	if (sigma < 0.61f) {
		numcoef = 1;
		coef[0] = sigma*sigma/2.0f;
		coef[1] = 1.0f-2.0f*coef[0];
		coef[2] = coef[0];
	}
	else {
		numcoef = min((long)(4*sigma),(sizeof(coef)/sizeof(coef[0]))/2-1);
		t0 = -1.0f/(2.0f*sigma*sigma);
		coef[numcoef] = 1.0f;
		sum = 1.0f;
		for (i = 1; i <= numcoef; i++) {
			test = i*i*t0;
			coef[numcoef-i] = exp (test);
			coef[numcoef+i] = coef[numcoef-i];
			sum = sum + 2.0f*coef[numcoef-i];
		}
		for (i = 0; i <= numcoef*2; i++) 
			coef[i] = coef[i]/sum;
	}

	float base_ln = this_smTrace[0];
	memcpy(trace_sv2, this_smTrace, sizeof(this_smTrace));

	int add_on = 100;
	long new_length = 1;
	while (new_length < max(numcoef*4, numTraceFrames+2*add_on)) 
		new_length = new_length*2;
	add_on = (new_length - numTraceFrames)/2;
	for (i = numTraceFrames - 1; i >= 0; i--)
		this_smTrace[i + add_on] = this_smTrace[i];
	for (i = 0; i < add_on; i++)
		this_smTrace[i] = this_smTrace[add_on];
	for (i = numTraceFrames + add_on; i < new_length; i++)
		this_smTrace[i] = this_smTrace[numTraceFrames + add_on - 1];

	memcpy(trace_sv, this_smTrace, sizeof(this_smTrace));
	for(i = numcoef; i <= new_length-numcoef; i++){
        this_val = 0.0f;
		for (m = 0, j = i - numcoef; m <= numcoef * 2; m++, j++)
			this_val += trace_sv[j] * coef[m];
		this_smTrace[i] = this_val;
    }

	for (i = 0, j = add_on; i < numTraceFrames; i++,j++)
		this_smTrace[i] = trace_sv2[i] - this_smTrace[j] + base_ln;
}


//FFT
typedef float fft_real;
typedef struct { fft_real Re; fft_real Im; } fft_complex;

#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif

/*
// Print a vector of complexes as ordered pairs. 
static void
print_vector(
	const char *title,
	fft_complex *x,
	int n)
{
	int i;
	printf("%s (dim=%d):", title, n);
	for (i = 0; i<n; i++) printf(" %5.2f,%5.2f ", x[i].Re, x[i].Im);
	putchar('\n');
	return;
}*/

void
fft(fft_complex *v, int n, fft_complex *tmp)
{
	if (n>1) {			// otherwise, do nothing and return 
		int k, m;    fft_complex z, w, *vo, *ve;
		ve = tmp; vo = tmp + n / 2;
		for (k = 0; k<n / 2; k++) {
			ve[k] = v[2 * k];
			vo[k] = v[2 * k + 1];
		}
		fft(ve, n / 2, v);		// FFT on even-indexed elements of v[] 
		fft(vo, n / 2, v);		// FFT on odd-indexed elements of v[] 
		for (m = 0; m<n / 2; m++) {
			w.Re = (fft_real)cos(2 * PI*m / (double)n);
			w.Im = -(fft_real)sin(2 * PI*m / (double)n);
			z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	// Re(w*vo[m]) 
			z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	// Im(w*vo[m]) 
			v[m].Re = ve[m].Re + z.Re;
			v[m].Im = ve[m].Im + z.Im;
			v[m + n / 2].Re = ve[m].Re - z.Re;
			v[m + n / 2].Im = ve[m].Im - z.Im;
		}
	}
	return;
}

void
ifft(fft_complex *v, int n, fft_complex *tmp)
{
	if (n>1) {			// otherwise, do nothing and return 
		int k, m;    fft_complex z, w, *vo, *ve;
		ve = tmp; vo = tmp + n / 2;
		for (k = 0; k<n / 2; k++) {
			ve[k] = v[2 * k];
			vo[k] = v[2 * k + 1];
		}
		ifft(ve, n / 2, v);		// FFT on even-indexed elements of v[] 
		ifft(vo, n / 2, v);		// FFT on odd-indexed elements of v[] 
		for (m = 0; m<n / 2; m++) {
			w.Re = (fft_real)cos(2 * PI*m / (double)n);
			w.Im = (fft_real)sin(2 * PI*m / (double)n);
			z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	// Re(w*vo[m])
			z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	// Im(w*vo[m])
			v[m].Re = ve[m].Re + z.Re;
			v[m].Im = ve[m].Im + z.Im;
			v[m + n / 2].Re = ve[m].Re - z.Re;
			v[m + n / 2].Im = ve[m].Im - z.Im;
		}
	}
	return;
}

void traceFFT()
{
	int i;
	fft_complex *v, *scratch;

	FFT_N = 2;
	while (FFT_N <= numTraceFrames)
		FFT_N *= 2;
	FFT_N /= 2;

	v = (fft_complex *)_aligned_malloc(FFT_N * sizeof(fft_complex), 2);
	scratch = (fft_complex *)_aligned_malloc(FFT_N * sizeof(fft_complex), 2);

	for (i = 0; i < FFT_N; i++) {
		v[i].Re = this_smTrace[i];
		v[i].Im = 0;
	}

	
/*		SM_exposure = 0.0005;
	for (i = 0; i < FFT_N; i++) {
		v[i].Re = sin(2 * PI *SM_exposure * i/ 0.0167) + 0.5 * sin(2 * PI *SM_exposure * i/0.0125) + 5;
		v[i].Im = 0; // 100 * sin(16 * PI*i / (double)FFT_N) + 200;
	}*/

	fft(v, FFT_N, scratch);
	FFT_N /= 2;
	FFT_max = 0.0;
	FFT_min = 0.0;
	for (i = 1; i < FFT_N; i++) {
		FFT_ar[i] = sqrt(v[i].Re*v[i].Re + v[i].Im*v[i].Im);
		if (FFTlog_flag) {
			FFT_ar[i] = log(FFT_ar[i]);
			if (FFT_ar[i] < 0.0)
				FFT_ar[i] = 0.0;
		}
		if (FFT_max < FFT_ar[i])
			FFT_max = FFT_ar[i];
	}
	FFT_ar[0] = FFT_ar[1];
/*	for (i = 0; i < FFT_N; i++)
		this_smTrace[i] = sqrt(v[i].Re*v[i].Re + v[i].Im*v[i].Im);
	for (; i < numTraceFrames; i++)
		this_smTrace[i] = 0;
	this_smTrace[0] = 0;*/

	_aligned_free(v);
	_aligned_free(scratch);
}

/*
static double bessi0(double x)
//------------------------------------------------------------
// PURPOSE: Evaluate modified Bessel function In(x) and n=0.  
//------------------------------------------------------------
{
	double ax, ans;
	double y;


	if ((ax = fabs(x)) < 3.75) {
		y = x / 3.75, y = y * y;
		ans = 1.0 + y * (3.5156229 + y * (3.0899424 + y * (1.2067492
			+ y * (0.2659732 + y * (0.360768e-1 + y * 0.45813e-2)))));
	}
	else {
		y = 3.75 / ax;
		ans = (exp(ax) / sqrt(ax))*(0.39894228 + y * (0.1328592e-1
			+ y * (0.225319e-2 + y * (-0.157565e-2 + y * (0.916281e-2
				+ y * (-0.2057706e-1 + y * (0.2635537e-1 + y * (-0.1647633e-1
					+ y * 0.392377e-2))))))));
	}
	return ans;
}

void band_stop_filter()
{
	float base_line = this_smTrace[0];
	float Fhi = (float)(right_bstop * SM_exposure * 2);
	float Flo = (float)(left_bstop * SM_exposure * 2);
	int ripple = 20, order = 30, block_len, num_coef;
	int fStop;
	float coef[100];
	float T; // alpha, div_f;
	int    i;

	if (Fhi < Flo)
		fStop = true;
	else
		fStop = false;
/*	if (ripple <= 21)	// ripple is fixed at 20 for now
		alpha = 0.0;
	else if (ripple >= 50)
		alpha = 0.1102*(ripple - 8.7);
	else 
		alpha = 0.5842*pow(0.4, ripple - 21.0) + 0.07886*(ripple - 21.0);
	div_f = bessi0(alpha);/
	for (i = 0; i < order; i++) {
//		coef[i] = bessi0(alpha*sqrt(1.0 - (i + 1.0)*(i + 1.0) / (order*order))) / div_f;
		T = (i + 1) * PI;
		coef[i] = (sin(T*Fhi) - sin(T*Flo)) / T;
	}
	for (i = 0; i < order; i++)
		coef[order + 1 + i] = coef[i];
	coef[order] = Fhi - Flo + fStop;
	for (i = 0; i < order; i++)
		coef[i] = coef[order*2 - i];
	num_coef = order * 2 + 1;
	if (num_coef <= 17)
		block_len = 64;
	else if (num_coef <= 29)
		block_len = 128;
	else if (num_coef <= 52)
		block_len = 256;
	else if (num_coef <= 94)
		block_len = 512;
	else if (num_coef <= 171)
		block_len = 1024;
	else //if (num_coef <= 377)
		block_len = 2048;

	if (num_coef > block_len)
		return;
	if (block_len > numTraceFrames)
		return;
	
	int add_on = 100;
	long new_length = 1;
	while (new_length < numTraceFrames + 2 * add_on)
		new_length = new_length * 2;
	add_on = (new_length - numTraceFrames) / 2;
	for (i = numTraceFrames - 1; i >= 0; i--)
		this_smTrace[i + add_on] = this_smTrace[i];
	for (i = 0; i < add_on; i++)
		this_smTrace[i] = this_smTrace[add_on];
	for (i = numTraceFrames + add_on; i < new_length; i++)
		this_smTrace[i] = this_smTrace[numTraceFrames + add_on - 1];
//	memcpy(trace_sv, this_smTrace, sizeof(this_smTrace));
	for (i = 0; i < new_length; i++)
		trace_sv[i*2] = this_smTrace[i];

	for (i = 0; i < new_length; i++) {
		trace_sv[i++] = 2000.0f + 1000.0f*sinf((float)2.0f*3.14159f*120.0f*(float)i/20000.0);
		trace_sv[i++] = 0.0f;
	}

	fft_complex v[fft_N], v1[fft_N], scratch[fft_N];
	int k;

	// Fill v[] with a function of known FFT: 
	for (k = 0; k<fft_N; k++) {
		v[k].Re = 0.125*cos(2 * PI*k / (double)fft_N);
		v[k].Im = 0.125*sin(2 * PI*k / (double)fft_N);
		v1[k].Re = 0.3*cos(2 * PI*k / (double)fft_N);
		v1[k].Im = -0.3*sin(2 * PI*k / (double)fft_N);
	}

	// FFT, iFFT of v[]:
	print_vector("Orig", v, fft_N);
	fft(v, fft_N, scratch);
	print_vector(" FFT", v, fft_N);

	// FFT, iFFT of v1[]:
	print_vector("Orig", v1, fft_N);
	fft(v1, fft_N, scratch);
	print_vector(" FFT", v1, fft_N);
}
*/

void smoothTrace()
{
	int i, m, trace_smooth_coeff, add_on, norm_f;
	long new_length;
	float this_sum;
	int coeff_ar[9];
	int coeff_ar1[] = { -3, 12, 17, 12, -3,  0,  0,  0,  0 };
	int coeff_ar2[] = { -2,  3,  6,  7,  6,  3, -2,  0,  0 };
	int coeff_ar3[] = {-21, 14, 39, 54, 59, 54, 39, 14,-21 };
	int coeff_ar4[] = {  5,-30, 75,131, 75,-30,  5,  0,  0 };
	int coeff_ar5[] = { 15,-55, 30,135,179,135, 30,-55, 15 };

	if (trace_smooth_mode == 1)
		trace_smooth_coeff = trace_smooth_coeff_id + 2;
	else {
		switch (trace_smooth_coeff_id)
		{
		case 0: 
			trace_smooth_coeff = 5;
			norm_f = 35;
			memcpy(coeff_ar, coeff_ar1, sizeof(coeff_ar));
			break;
		case 1:
			trace_smooth_coeff = 7;
			norm_f = 21;
			memcpy(coeff_ar, coeff_ar2, sizeof(coeff_ar));
			break;
		case 2:
			trace_smooth_coeff = 9;
			norm_f = 231;
			memcpy(coeff_ar, coeff_ar3, sizeof(coeff_ar));
			break;
		case 3:
			trace_smooth_coeff = 7;
			norm_f = 231;
			memcpy(coeff_ar, coeff_ar4, sizeof(coeff_ar));
			break;
		case 4:
			trace_smooth_coeff = 9;
			norm_f = 429;
			memcpy(coeff_ar, coeff_ar5, sizeof(coeff_ar));
			break;
		}
	}

	add_on = trace_smooth_coeff;
	new_length = numTraceFrames + 2 * add_on;

	for (i = numTraceFrames - 1; i >= 0; i--)
		this_smTrace[i + add_on] = this_smTrace[i];
	for (i = 0; i < add_on; i++)
		this_smTrace[i] = this_smTrace[add_on];
	for (i = numTraceFrames + add_on; i < new_length; i++)
		this_smTrace[i] = this_smTrace[numTraceFrames + add_on - 1];

	if (trace_smooth_mode == 1) {	//Moving average
		for (i = 0; i < numTraceFrames; i++) {
			this_sum = 0.0;
			for (m = 0; m < trace_smooth_coeff; m++)
				this_sum += this_smTrace[i+m];
			this_smTrace[i] = this_sum/ trace_smooth_coeff;
		}
	}
	else {		//Savitzky-Golay
		for (i = 0; i < numTraceFrames; i++) {
			this_sum = 0.0;
			for (m = 0; m < trace_smooth_coeff; m++)
				this_sum += this_smTrace[i + m]*coeff_ar[m];
			this_smTrace[i] = this_sum / norm_f;
		}
	}
}

void exponential_sub(int fit_x1, int fit_x2) {
	int i, num_fit, exp_x_shift=60;
	double A, m, c, x, sumx = 0.0, sumx2 = 0.0, sumy = 0.0, sumxy = 0.0;
	double y1, y2, y_exp_offset, y_fit;

	fit_x1 = max(1, fit_x1);
	fit_x2 = min(max(fit_x1 + exp_x_shift + 10, fit_x2), numTraceFrames);
	num_fit = fit_x2 - fit_x1 + 1;
	for (i = fit_x1; i <= fit_x2; i++)
	{
		x = i * 1.0;
		sumx = sumx + this_smTrace[i];
		sumx2 = sumx2 + this_smTrace[i]* this_smTrace[i];
		sumy = sumy + log(x);
		sumxy = sumxy + log(x) * this_smTrace[i];

	}
	c = (sumx2*sumy - sumx * sumxy) / (sumx2 - sumx * sumx);
	m = (sumxy - sumx * sumy) / (sumx2 - sumx * sumx);
	A = exp(c);

	exp_x_shift = 0;
	for (i = 0; i < numTraceFrames; i++) {
		exp_x_shift++;
		y1 = (log((fit_x1 + exp_x_shift) / A)) / m;
		y2 = (log((fit_x2 + exp_x_shift) / A)) / m;
		if (this_smTrace[fit_x1] - this_smTrace[fit_x2] >= y2 - y1)
			break;
	}
	y_exp_offset = y1;

	for (i = 0; i < numTraceFrames; i++) {
		if (A != 0.0 && m != 0.0) {
			y_fit = (log((i+ exp_x_shift) / A)) / m;
			this_smTrace[i] += (float)((y_fit - y_exp_offset));
		}
	}
}

void FilterTrace()
{
	// this_smTrace is the current trace being processed. Global parameter

	if (trace_smooth_mode)
		smoothTrace();
	if (trace_lpass > 0.0)
		lpass_gauss_filter();
	if (trace_hpass > 0.5)
		hpass_gauss_filter();
	if (exp_sub_flag)
		exponential_sub(exp_sub_f1, exp_sub_f2);
	//traceFFT();
/*	right_bstop = 1000;
	if (left_bstop > 0.0 || right_bstop > 0.0)
		band_stop_filter();*/
}

void saveWholeTraces(char *txtName)
{
	int k, i, m, isBNC, numTraceFrames_sv, firstDataFrame_sv;
    FILE *fp;
	if (!FOPEN_S(fp,txtName,"w"))
		return;

	numTraceFrames_sv = numTraceFrames;
	firstDataFrame_sv = firstDataFrame;

	int total_cnt = num_traces * numDataFrames / numTraceFrames;
	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / total_cnt;
	char str[256];
	sprintf(str, "Saving Traces: %d%%", 0);
	SetWindowText(hwndPB, str);

	for (k = 0; k < num_traces; k++) {
		firstDataFrame = 0;
		for (m = 0; m < numDataFrames/numTraceFrames; m++) {
			SM_readData(SM_dataname);
			getAllTraces();
			if (ROI_index[k][0] == BNC_SELECT)
				isBNC = 1;
			else
				isBNC = 0;
			memcpy(this_smTrace, smTrace[k], sizeof(smTrace[k]));
			if (!isBNC || BNC_filter_flag)
				FilterTrace();

			for (i = 0; i < numTraceFrames; i++)
				fprintf(fp,"%lf,",this_smTrace[i]);

			int this_percent = 100*(m + k*numDataFrames / numTraceFrames)/total_cnt;
			sprintf(str, "Saving Traces: %d%%", this_percent);
			SetWindowText(hwndPB, str);
			while ((int)((m + k * numDataFrames / numTraceFrames)*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}

			firstDataFrame += numTraceFrames;
		}
		if (numDataFrames % numTraceFrames) {
			SM_readData(SM_dataname);
			getAllTraces();
			if (ROI_index[k][0] == BNC_SELECT)
				isBNC = 1;
			else
				isBNC = 0;
			memcpy(this_smTrace, smTrace[k], sizeof(smTrace[k]));
			if (!isBNC || BNC_filter_flag)
				FilterTrace();

			for (i = 0; i < numTraceFrames; i++)
				fprintf(fp,"%lf,",this_smTrace[i]);

			int this_percent = 100 * (m + k * numDataFrames / numTraceFrames_sv) / total_cnt;
			sprintf(str, "Saving Traces: %d%%", this_percent);
			SetWindowText(hwndPB, str);
			while ((int)((m + k * numDataFrames / numTraceFrames_sv)*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
		}
		fprintf(fp,"\n");
		numTraceFrames = numTraceFrames_sv;
	}
	char tmp_str[256];
	if (fileTime->tm_min < 10)
		sprintf(tmp_str, "0%d", fileTime->tm_min);
	else
		sprintf(tmp_str, "%d", fileTime->tm_min);
	sprintf(str, "%s (%d/%d/%d %d:%s)", SM_dataname, fileTime->tm_mon + 1, fileTime->tm_mday, fileTime->tm_year + 1900, fileTime->tm_hour + gt_hour_diff, tmp_str);
	strcpy(tmp_str, str);
	sprintf(str, "%s, Configuration: %s, Exposure: %lfs, Number of Frames: %d", tmp_str, file_cameraName, SM_exposure, numDataFrames);
	if (BNC_exist)
		sprintf(str, "%s, BNC Ratio: %d", str, file_BNC_ratio);
	for (k = 0; k < num_traces; k++)
		sprintf(str, "%s, Trace%d: %d pixels", str, k+1, ROI_index[k][5]);
	fprintf(fp, str);
	fclose(fp);
	DestroyWindow(hwndPB);

	firstDataFrame = firstDataFrame_sv;
	SM_readData(SM_dataname);
}

void saveWholeROIdata(char *txtName)
{
	int k, i, j, m, ii, numTraceFrames_sv;
	int handle;
	signed short int *pt;
	long len, this_fr_num;
	unsigned _int64 sect_stt;
	FILE *fp;

	if (!FOPEN_S(fp, txtName, "w"))
		return;

	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / numDataFrames;
	char str[256];
	sprintf(str, "Saving images: %d of %d", 0, numDataFrames);
	SetWindowText(hwndPB, str);

	numTraceFrames_sv = numTraceFrames;
	if ((handle = _open(SM_dataname, O_BINARY | O_RDONLY)) == -1)
		return;
	for (k = 0; k < num_traces; k++) {
		if (ROI_index[k][0] == BNC_SELECT)
			continue;
		for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++)
			for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++)
				if (SelectedMap[j][i] == pixColorCode[k])
					fprintf(fp, "%d,", i);
		fprintf(fp, "	");
	}
	fprintf(fp, "X\n");
	for (k = 0; k < num_traces; k++) {
		if (ROI_index[k][0] == BNC_SELECT)
			continue;
		for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++)
			for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++)
				if (SelectedMap[j][i] == pixColorCode[k])
					fprintf(fp, "%d,", j);
		fprintf(fp, "	");
	}
	fprintf(fp, "Y\n");
	len = disp_num_col * disp_num_row;
	for (m = 0; m < numDataFrames / numTraceFrames; m++) {
		sect_stt = 2880L + ((unsigned _int64)numTraceFrames)*m*len * 2;
		_lseeki64(handle, sect_stt, SEEK_SET);
		_read(handle, (signed short *)image_data, len*numTraceFrames * 2);
		pt = image_data;
		for (ii = 0; ii < numTraceFrames; ii++) {
			for (k = 0; k < num_traces; k++) {
				if (ROI_index[k][0] == BNC_SELECT)
					continue;
				for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++)
					for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++)
						if (SelectedMap[j][i] == pixColorCode[k])
							fprintf(fp, "%d,", *(pt + j * disp_num_col + i));
				fprintf(fp, "	");
			}
			this_fr_num = ii + m * numTraceFrames + 1;
			fprintf(fp, "%d\n", this_fr_num);
			sprintf(str, "Saving images: %d of %d", this_fr_num, numDataFrames);
			SetWindowText(hwndPB, str);
			while ((int)(ii*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
			pt += len;
		}
	}
	if (numDataFrames % numTraceFrames) {
		sect_stt = 2880L + ((unsigned _int64)numTraceFrames)*m*len * 2;
		numTraceFrames = numDataFrames % numTraceFrames;
		_lseeki64(handle, sect_stt, SEEK_SET);
		_read(handle, (signed short *)image_data, len*numTraceFrames * 2);
		pt = image_data;
		for (ii = 0; ii < numTraceFrames; ii++) {
			for (k = 0; k < num_traces; k++) {
				if (ROI_index[k][0] == BNC_SELECT)
					continue;
				for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++)
					for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++)
						if (SelectedMap[j][i] == pixColorCode[k])
							fprintf(fp, "%d,", *(pt + j * disp_num_col + i));
				fprintf(fp, "	");
			}
			this_fr_num = ii + m * numTraceFrames + 1;
			fprintf(fp, "%d\n", this_fr_num);
			sprintf(str, "Saving images: %d of %d", this_fr_num, numDataFrames);
			SetWindowText(hwndPB, str);
			while ((int)(ii*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
			pt += len;
		}
	}
	pt = dark_data;
	for (k = 0; k < num_traces; k++) {
		if (ROI_index[k][0] == BNC_SELECT)
			continue;
		for (j = ROI_index[k][3]; j < ROI_index[k][4]; j++)
			for (i = ROI_index[k][1]; i < ROI_index[k][2]; i++)
				if (SelectedMap[j][i] == pixColorCode[k])
					fprintf(fp, "%d,", *(pt + j * disp_num_col + i));
		fprintf(fp, "	");
	}
	fprintf(fp, "Dark Value\n");
	numTraceFrames = numTraceFrames_sv;
	_close(handle);
	fclose(fp);
	DestroyWindow(hwndPB);

	SM_readData(SM_dataname);
}

float thumb_vals[NUM_TRACES_MAX];
void DisplayTrace(char *txtName, int paintFlag)
{
	int num_grid, superinpose_intersect=0;

	if ((!num_traces && !photonDFlag) || frameLiveFlag)
		return;
	if (!window_layout && trace_win_ysize < 100)
		return;

	if (paintFlag == PAINTINIT) {
		LockGraphics();
		DisplayTrace(txtName, PAINTGRFX);
		UnlockGraphics();
		DisplayTrace(txtName, PAINTTEXT);
	}
	else if (paintFlag == PAINTGRFX) {
		FILE *fp = nullptr;
		int i, k, saveTraceFlag, grid_color, isBNC;
		int x, x0, y0, x1, x2, y1, y2, y_max, y_min, xx0, dx_pt;
		float this_av = 0;
		double dx, dy, y_gain, grid_w;

		if (!strcmp(txtName, ""))
			saveTraceFlag = 0;
		else
			saveTraceFlag = 1;
		if (saveTraceFlag) {
			if (!FOPEN_S(fp, txtName, "w"))
				return;
		}
		x0 = OM[SM_TRACEWIN].x + trace_bdr;
		dx = ((double)OM[SM_TRACEWIN].dx - OM[SM_TRCGAIN].dx - trace_bdr * 2) / num_trace_pnts;
		double dy_portion = 0.85;
		dy = ((double)trace_region_dy*dy_portion - 2 * trace_bdr) / (num_traces+1);
		y_max = OM[SM_TRCEXP].y - trace_bdr;
		y_min = trace_region_y + trace_bdr;

		if (photonDFlag) {
			int y_base, this_min, this_max;
			double this_hgt;
			y_gain = get_traceYGain(traceYGain);
			y0 = (int)(trace_region_y + (trace_region_dy - trace_bdr)*0.9 + 0.5);
			this_hgt = 0.8*(trace_region_dy - 2 * trace_bdr) / min(num_chan_group, num_chan_total - PHT_chan_offset);
			for (k = 0; k < min(num_chan_group, num_chan_total - PHT_chan_offset); k++) {
				this_max = (int)(y0 - this_hgt*k + 0.5);
				this_min = (int)(this_max - this_hgt + 0.5);
				y_base = (int)(this_max - this_hgt/2 + 0.5);
				for (i = 1; i < num_PHT_sample; i++) {
					x1 = x0 + i - 1;
					x2 = x0 + i;
					y1 = (int)(y_base - (PHT_noise_ar[k][i - 1] - min_PHT_noise[k])*y_gain);
					y2 = (int)(y_base - (PHT_noise_ar[k][i] - min_PHT_noise[k])*y_gain);
					if ((y1 >= this_min && y2 >= this_min) && (y1 <= this_max && y2 <= this_max)) {
						Draw_Line(x1, y1, x2, y2, traceColorAr[(k % 8)]);
					}
					else {
						y2 = min(max(y2, this_min+5), this_max-5);
						Draw_Line(x2, y2, x2, y2, WHITE);
					}
				}
			}
		}
		else {
			grid_color = DKGRAY;
			if (!window_layout)
				num_grid = 10;
			else
				num_grid = 5;
			grid_w = dx * num_trace_pnts / num_grid;
			y1 = y_min;
			y2 = OM[SM_TRCEXP].y - 5;
			if (darkSubFlag && f_subtract_mode && ref_frame1 != ref_frame2) {
				dx_pt = (int)(dx*(ref_frame2 - ref_frame1 + 1));
				xx0 = (int)(x0 + (ref_frame1 - trace_frame0)*dx + 0.5);
				if ((xx0 >= x0) && (xx0 <= x0 + OM[SM_TRACEWIN].dx - 2 * trace_bdr))
					D3DRectangle(xx0, y1, min(x0 + OM[SM_TRACEWIN].dx - 2 * trace_bdr, xx0 + dx_pt), y2, BLUE);
			}
			for (i = 0; i <= 10; i++) {
				x = max(x0, (int)(x0 + grid_w * i + 0.5));
				if (x < x0 + OM[SM_TRACEWIN].dx)
					VLine(y1, y2, x, grid_color);
			}
			HLine(x0, (int)(x0 + grid_w + 0.5), y1 + 10, grid_color);
			if (darkSubFlag && f_subtract_mode && ref_frame1 == ref_frame2) {
				i = (int)(x0 + dx * (ref_frame1 - trace_frame0) + 0.5);
				if ((i > x0) && (i < x0 + OM[SM_TRACEWIN].dx - OM[SM_TRCGAIN].dx - trace_bdr * 2))
					VLine(y1, y2, i, BLUE);
			}
			y_gain = get_traceYGain(traceYGain)*trace_region_dy/300;
			if (num_av_disp > 1) {
				dx_pt = (int)(dx*(av_end - av_stt + 1));
				xx0 = trace_glider_x - (int)((curDispFrame - av_stt)*dx);
				if ((xx0 >= x0) && (xx0 <= x0 + OM[SM_TRACEWIN].dx - 2 * trace_bdr))
					D3DRectangle(xx0, y1, min(x0 + OM[SM_TRACEWIN].dx - 2 * trace_bdr, xx0 + dx_pt), y2, MIDGRAY);
			}
			if (trace_layout)
				superinpose_intersect = (ref_frame1 + ref_frame2) / 2;
			double this_y_gain;
			float trace_min, trace_val0, trace_base_shift;
			for (k = 0; k < num_traces; k++) {
				if (ROI_index[k][0] == BNC_SELECT) {
					isBNC = 1;
					trace_factor = file_BNC_ratio;
					this_y_gain = pow(2, BNC_YGain/50.0)/500.0;
				}
				else {
					isBNC = 0;
					trace_factor = 1;
					this_y_gain = y_gain;
				}
				memcpy(this_smTrace, smTrace[k], sizeof(smTrace[k]));
				if (!isBNC || BNC_filter_flag)
					FilterTrace();
				trace_min_frame = max(trace_min_frame, trace_frame0);
				if (k == 0 || !trace_layout) {
					trace_min = this_smTrace[trace_min_frame];
					trace_val0 = this_smTrace[superinpose_intersect];
				}
				if (trace_layout) {
					trace_base_shift = this_smTrace[superinpose_intersect] - trace_val0;
					y0 = (int)(trace_region_y + trace_bdr * 2 + (double)trace_region_dy * 3 / 4 + 0.5) - 15;
				}
				else {
					trace_base_shift = 0;
					y0 = (int)(trace_region_y + trace_bdr * 2 + (double)trace_region_dy*(1 - dy_portion) + dy * (k + 1) + 0.5) - 15;
				}
				for (i = 1; i < num_trace_pnts * trace_factor; i++) {
					if (saveTraceFlag)
						fprintf(fp, "%lf,", this_smTrace[trace_frame0*trace_factor + i - 1]);
					x1 = (int)(x0 + 1.0*(i - 1)*dx / trace_factor + 0.5);
					x2 = (int)(x0 + 1.0*i*dx / trace_factor + 0.5);
					y1 = (int)(y0 - (this_smTrace[trace_frame0*trace_factor + i - 1] - trace_min - trace_base_shift)*this_y_gain);
					y2 = (int)(y0 - (this_smTrace[trace_frame0*trace_factor + i] - trace_min - trace_base_shift)*this_y_gain);
					if ((y1 > y_min || y2 > y_min) && (y1 < y_max || y2 < y_max)) {
						y1 = min(max(y1, y_min), y_max);
						y2 = min(max(y2, y_min), y_max);
						Draw_Line(x1, y1, x2, y2, traceColorAr[pixColorCode[k] - 1]);
					}
				}
				if (saveTraceFlag)
					fprintf(fp, "%lf\n", this_smTrace[trace_frame0*trace_factor + i - 1]);
				if (num_av_disp > 1) {
					this_av = 0.0;
					for (i = av_stt; i <= av_end; i++)
						this_av += this_smTrace[i*trace_factor];
					thumb_vals[k] = this_av / (av_end - av_stt + 1);
				}
				else {
					thumb_vals[k] = this_smTrace[curDispFrame*trace_factor];
				}
				if (isBNC)
					thumb_vals[k] *= 2;

				if (darkSubFlag && f_subtract_mode && !NDR_trace_sub && !RLI_div_flag) {
					this_av = 0;
					for (i = ref_frame1; i <= ref_frame2; i++)
						this_av += this_smTrace[i];
					thumb_vals[k] -= this_av / (ref_frame2 - ref_frame1 + 1);
				}
				else if (!darkSubFlag)
					thumb_vals[k] += ROI_index[k][6];
			}
			if (!saveTraceFlag)
				drawTraceThumb(-1);
			else {
				char str[1024], tmp_str[1024];
				if (fileTime->tm_min < 10)
					sprintf(tmp_str, "0%d", fileTime->tm_min);
				else
					sprintf(tmp_str, "%d", fileTime->tm_min);
				sprintf(str, "%s (%d/%d/%d %d:%s)", SM_dataname, fileTime->tm_mon + 1, fileTime->tm_mday, fileTime->tm_year + 1900, fileTime->tm_hour + gt_hour_diff, tmp_str);
				strcpy(tmp_str, str);
				sprintf(str, "%s, Configuration: %s, Exposure: %lfs, Number of Frames: %d", tmp_str, file_cameraName, SM_exposure, numTraceFrames);
				if (BNC_exist)
					sprintf(str, "%s, BNC Ratio: %d", str, file_BNC_ratio);
				for (k = 0; k < num_traces; k++)
					sprintf(str, "%s, Trace%d: %d pixels", str, k + 1, ROI_index[k][5]);
				fprintf(fp, str);
				fclose(fp);
			}
		}
	}
	else if (paintFlag == PAINTTEXT) {
		if (photonDFlag)
			return;

		int x0, y_min, y1, grid_w_ms;
		double dx, grid_w;
		char str[100];

		if (!window_layout)
			num_grid = 10;
		else
			num_grid = 5;
		x0 = OM[SM_TRACEWIN].x + trace_bdr;
		dx = ((double)OM[SM_TRACEWIN].dx - OM[SM_TRCGAIN].dx - trace_bdr * 2) / num_trace_pnts;
		y_min = trace_region_y + trace_bdr;
		grid_w = dx * num_trace_pnts / num_grid;
		y1 = y_min;
		grid_w_ms = (int)(num_trace_pnts*SM_exposure * 1000/ num_grid + 0.5);
		if (grid_w_ms >= 10000) {
			sprintf(str, "%5.1lf s", (double)grid_w_ms / 1000.0);
		}
		else
			sprintf(str, "%d ms", grid_w_ms);
		drawText(0, 0, str, (int)(x0 + (grid_w - (int)strlen(str) * 7) / 2), y1 - 4, MIDGRAY, omFontSmall, FALSE);
	}
}

void savePS_traces(char *ps_name)
{
	FILE *fp;
	unsigned int this_col;
	double ps_x_scale, this_y_gain, y_gain, av10;
	int ps_step = 20, isBNC;

	if (!(fp = fopen(ps_name, "w")))
		return;

	fprintf(fp, "%%!PS-Adobe-3.0\n");
//	fprintf(fp, "%%%%BoundingBox: ?????\n");
	fprintf(fp, "%%%%Creator: Turbo-SM64\n");
	fprintf(fp, "%%%%CreationDate: \n");
	fprintf(fp, "%%%%Requirement: color\n");
	fprintf(fp, "%%%%EndComments\n");
	fprintf(fp, "%%%%BeginProlog\n");
	fprintf(fp, "save\n");
	fprintf(fp, "%%+ prolog.ps - Prolog for Turbo-SM64\n");
	fprintf(fp, "%%+ Copyright (c) SciMeasure/RedShirtImaging. All Rights Reserved\n");
	fprintf(fp, "/$TB_DICT 40 dict def $TB_DICT begin /bdef { bind def } bind def /D {currentpoint stroke moveto} bdef "); 
	fprintf(fp, "/M {moveto} bdef /R {rlineto} bdef /L0 {[] 0 setdash} bdef /STDFONT { findfont exch scalefont setfont } bdef "); 
	fprintf(fp, "/ISOFONT { findfont dup length dict begin { 1 index /FID ne {def} {pop pop} ifelse } forall "); 
	fprintf(fp, "/Encoding ISOLatin1Encoding def currentdict end /idltmpfont exch definefont exch scalefont setfont } bdef end 2.5 setmiterlimit\n");
	fprintf(fp, "%%%%EndProlog\n");
	fprintf(fp, "%%%%Page: 0 1\n");
	fprintf(fp, "%%%%PageRequirement: color\n");
	fprintf(fp, "%%%%PageOrientation: Portrait\n");
	fprintf(fp, "%%%%BeginPageSetup\n");
	fprintf(fp, "save\n");
	ps_x_scale = 6.5 * 72 / (ps_step*num_trace_pnts);
	fprintf(fp, "$TB_DICT begin 72 100 translate %lf dup scale\n", ps_x_scale);
	fprintf(fp, "%%%%IncludeResource: font Helvetica\n");
	fprintf(fp, "423.333 /Helvetica STDFONT\n");
	fprintf(fp, "%%%%EndPageSetup\n");
	fprintf(fp, "36 setlinewidth L0 ");
	y_gain = get_traceYGain(traceYGain);
	for (int k = 0; k < num_traces; k++) {
		if (ROI_index[k][0] == BNC_SELECT) {
			isBNC = 1;
			trace_factor = file_BNC_ratio;
			this_y_gain = pow(2, BNC_YGain / 50.0) / 500.0;
		}
		else {
			isBNC = 0;
			trace_factor = 1;
			this_y_gain = y_gain;
		}
		memcpy(this_smTrace, smTrace[k], sizeof(smTrace[k]));
		if (!isBNC || BNC_filter_flag)
			FilterTrace();
		this_col = traceColorAr[pixColorCode[k] - 1];
		fprintf(fp, "%lf ", ((this_col & 0xff0000) >> 16) / 255.0);
		fprintf(fp, "%lf ", ((this_col & 0xff00) >> 8) / 255.0);
		fprintf(fp, "%lf ", (this_col & 0xff) / 255.0);
		av10 = 0.0;
		for (int i = 0; i < 10; i++)
			av10 += this_smTrace[i];
		av10 /= 10;
		fprintf(fp, "setrgbcolor 0 %d M ", (int)((700 - (k+1)*600/ num_traces - (av10 - this_smTrace[0])*this_y_gain)/ ps_x_scale));
		for (int i = 1; i < num_trace_pnts*trace_factor; i++)
			fprintf(fp, "%d %d R ", ps_step/ trace_factor, (int)((this_smTrace[i] - this_smTrace[i-1])*this_y_gain / ps_x_scale));
		fprintf(fp, "D ");
	}
	fprintf(fp, "0.0 0.0 0.0 setrgbcolor 0 %d M 0 %d R %d 0 R 0 %d R D ", (int)(50 / ps_x_scale), -(int)(2/ ps_x_scale), (int)(ps_step*num_trace_pnts / 10), (int)(2 / ps_x_scale));
	fprintf(fp, "\n%%%%PageTrailer\n");
	fprintf(fp, "end\n");
	fprintf(fp, "restore\n");
	fprintf(fp, "showpage\n");
	fprintf(fp, "%%%%PageResources: font Helvetica\n");
	fprintf(fp, "%%%%Trailer\n");
	fprintf(fp, "restore\n");
	fprintf(fp, "%%%%Pages: 1\n");
	fprintf(fp, "%%%%DocumentNeededResources : font Helvetica\n");
	fprintf(fp, "%%%%EOF\n");

	fclose(fp);
}

int resetDispFrame(int movieFlag)
{
	int ret_val = FALSE;

	if (curDispFrame >= trace_frame0 + num_trace_pnts) {
		if (movieFlag) 
			curDispFrame = trace_frame0;
		else
			curDispFrame = trace_frame0 + num_trace_pnts - 1;
		ret_val = TRUE;
	}
	else if (curDispFrame < trace_frame0) {
		if (movieFlag) 
			curDispFrame = trace_frame0 + num_trace_pnts - 1;
		else
			curDispFrame = trace_frame0;
		ret_val = TRUE;
	}
	if (curDispFrame > numTraceFrames - 1) {
		double dx = ((double)OM[SM_TRACEWIN].dx - OM[SM_TRCGAIN].dx - trace_bdr * 2) / num_trace_pnts;
		int x0 = OM[SM_TRACEWIN].x + trace_bdr;

		curDispFrame = numTraceFrames - 1;
		trace_glider_x = (int)((double)(curDispFrame - trace_frame0)*dx + x0 + 0.5);
	}
	return ret_val;
}

void setTraceThumb(int mouseX, int ref_flag)
{
	int trace_glider_x_sv, f_diff;
	double dx = ((double)OM[SM_TRACEWIN].dx-OM[SM_TRCGAIN].dx-trace_bdr*2)/num_trace_pnts;
	int x0 = OM[SM_TRACEWIN].x + trace_bdr;

	if (ref_flag == 4 && !f_subtract_mode)
		return;

	if (ref_flag != 0)
		trace_glider_x_sv = trace_glider_x;

	if (mouseX < 0)
		trace_glider_x = (int)((double)(curDispFrame-trace_frame0)*dx + x0 + 0.5);
	else {
		trace_glider_x = min(max(mouseX, OM[SM_TRACEWIN].x+trace_bdr), OM[SM_TRACEWIN].x+OM[SM_TRACEWIN].dx-trace_bdr);
		if (trace_glider_x > OM[SM_TRCGAIN].x-trace_bdr)
			trace_glider_x = OM[SM_TRCGAIN].x - trace_bdr;
		if (trace_glider_x < (int)(x0 - dx + 0.5))
			trace_glider_x = (int)(x0 - dx + 0.5);

		if (ref_flag == 4) {
			f_diff = ref_frame2 - ref_frame1;
			ref_frame1 = (int)((trace_glider_x - x0) / dx + 0.5 + trace_frame0 - f_diff/2.0);
			ref_frame2 = ref_frame1 + f_diff;
			ref_frame1 = max(ref_frame1, 0);
			ref_frame2 = min(ref_frame2, num_trace_pnts);
		}
		else if (ref_flag < 0)
			trace_min_frame = (int)((trace_glider_x - x0)/dx + 0.5) + trace_frame0;
		else if (ref_flag > 1) {
			ref_frame2 = (int)((trace_glider_x - x0) / dx + 0.5) + trace_frame0;
			if ((ref_frame1 > ref_frame2) && (ref_flag == 3)) {
				int ref_f_sv = ref_frame2;
				ref_frame2 = ref_frame1;
				ref_frame1 = ref_f_sv;
			}
		}
		else if (ref_flag == 1) {
			ref_frame1 = (int)((trace_glider_x - x0)/dx + 0.5) + trace_frame0;
			ref_frame2 = ref_frame1;
			f_subtract_mode = 1;
		}
		else {
			curDispFrame = (int)((trace_glider_x - x0) / dx + 0.5) + trace_frame0;
			resetDispFrame(0);
		}
		if (trace_glider_x < x0)
			trace_glider_x = x0;
	}
	if (ref_flag != 0) {
		trace_glider_x = trace_glider_x_sv;
		if (ref_flag > 2) {
			if (!darkSubFlag) {
				darkSubFlag = TRUE;
				setChckBox(SM_DKSUBBOX);
				repaintMenubox();
			}
			dispSettingCheck();
		}
	}
	if (ref_flag >= 0)
		getStreamFrame();
	else
		paintWin(SM_TRACEWIN, PAINTINIT);
}

void drawTraceThumb(int mouseX)
{
	int y1, y2, x_pos, y_pos, m;
	char str[20];

	LockGraphics();
	y1 = trace_region_y + 15;
	y2 = OM[SM_TRCEXP].y - 10;
	if (mouseX < 0)
		VLine(y1,y2,trace_glider_x,WHITE);
	else {
		if (trace_exp == trace_exp_max)
			return;
		VLine(y1, y2, mouseX,RED);
	}
	UnlockGraphics();
	if (mouseX < 0) {
		sprintf(str, "%d", firstDataFrame+curDispFrame+1);
		if (num_av_disp > 1)
			sprintf(str, "%s(%d/%d)", str, firstDataFrame+av_stt+1, firstDataFrame+av_end+1);
		if (trace_glider_x < OM[SM_TRACEWIN].dx/2)
			x_pos = trace_glider_x+5;
		else
			x_pos = trace_glider_x-(int)strlen(str)*7-5;
		y_pos = y1;
		drawText(0, 0, str, x_pos, y_pos, WHITE, omFontRprtTn, TRUE);
		for (m = 0; m < num_traces; m++) {
			y_pos += 10;
			if (y_pos >= OM[SM_TRACEWIN].y + OM[SM_TRACEWIN].dy - 10)
				break;
			int this_color = ((traceColorAr[pixColorCode[m]-1] & 0xff)<<16) |
							 (traceColorAr[pixColorCode[m]-1] & 0xff00) |
							 ((traceColorAr[pixColorCode[m]-1] & 0xff0000)>>16);
			if (RLI_div_flag)
				sprintf(str, "%0.2lf%%", thumb_vals[m]*100);
			else
				sprintf(str, "%.1lf", thumb_vals[m]);
			drawText(0, 0, str, x_pos, y_pos, this_color, omFontRprtTn, TRUE);
		}
	}
}

void drawTraceWin(int paintFlag)
{
	if ((window_layout == 1 || (window_layout == 2 && frameLiveFlag)) && dataExist)
		paintHisto(paintFlag);

	if (dataFile && (num_traces || photonDFlag) && !runCam_flag && !frameLiveFlag) {
		DisplayTrace("", paintFlag);		
	}
}

void setHistoRangeWheel(int inc, int mouseX)
{
	if (showHistFlag) {
		if (!lockHistFlag) {
			lockHistFlag = TRUE;
			setChckBox(SM_HLOCKBOX);
			repaintMenubox();
		}

		if ((mouseX - OM[SM_HISTEXP].x) > OM[SM_HISTEXP].dx / 2) {
			if (inc > 0)
				histo_max -= 64;
			else
				histo_max += 64;
		}
		else {
			if (inc > 0)
				histo_min += 64;
			else
				histo_min -= 64;
		}
		histo_min = min(max(histo_min, histoLeft), histoRight);
		histo_max = min(max(histo_min, histo_max), histoRight);
		if (!window_layout)
			paintWin(OM_MEASUREWIN, PAINTINIT);
		else
			paintWin(SM_TRACEWIN, PAINTINIT);
		paintWin(SM_IMGSCBASE, PAINTINIT);
	}
	else {
		if (inc > 0)
			FFT_N_disp -= 8;
		else
			FFT_N_disp += 8;
		FFT_N_disp = min(max(0, FFT_N_disp), FFT_N);
		if (!window_layout)
			paintWin(OM_MEASUREWIN, PAINTINIT);
		else
			paintWin(SM_TRACEWIN, PAINTINIT);
	}
}

void setHistoRange(int mouseX)
{
	int x, x1, x2;
	int hmin_sv, hmax_sv, FFT_N_D_sv;

	if (showHistFlag) {
		if (!lockHistFlag) {
			lockHistFlag = TRUE;
			setChckBox(SM_HLOCKBOX);
			repaintMenubox();
		}

		hmin_sv = histo_min;
		hmax_sv = histo_max;

		x = (mouseX - OM[SM_HISTEXP].x)*(histoRight - histoLeft) / OM[SM_HISTEXP].dx + histoLeft;

		x1 = OM[SM_HISTEXP].x + (int)((double)OM[SM_HISTEXP].dx*(histo_min - histoLeft) / (histoRight - histoLeft) + 0.5);
		x2 = OM[SM_HISTEXP].x + (int)((double)OM[SM_HISTEXP].dx*(histo_max - histoLeft) / (histoRight - histoLeft) + 0.5);
		if (abs(mouseX - x1) < abs(mouseX - x2)) {
			histo_min = min(max((int)(x / hist_tic_spacing)*hist_tic_spacing, histoLeft), histoRight);
			if (histo_min >= histo_max)
				histo_min = histo_max - hist_tic_spacing;
		}
		else {
			histo_max = min(max((int)(x / hist_tic_spacing)*hist_tic_spacing, histoLeft), histoRight);
			if (histo_max <= histo_min)
				histo_max = histo_min + hist_tic_spacing;
		}

		if (hmin_sv != histo_min || hmax_sv != histo_max) {
			if (!window_layout)
				paintWin(OM_MEASUREWIN, PAINTINIT);
			else
				paintWin(SM_TRACEWIN, PAINTINIT);
			paintWin(SM_IMGSCBASE, PAINTINIT);
		}
	}
	else {
		FFT_N_D_sv = FFT_N_disp;
		FFT_N_disp = min(max(0, (mouseX - OM[SM_HISTEXP].x)*FFT_N / OM[SM_HISTEXP].dx), FFT_N);
		if (FFT_N_D_sv != FFT_N_disp) {
			if (!window_layout)
				paintWin(OM_MEASUREWIN, PAINTINIT);
			else
				paintWin(SM_TRACEWIN, PAINTINIT);
		}
	}
}

void setHistoGain(int mouseY)
{
	int y;

	y = (mouseY - OM[SM_HISTGAIN].y)*1000/OM[SM_HISTGAIN].dy;
	OM[SM_HISTGAIN].attr = min(max(0, y), 1000);
	histoGain = (1000 - OM[SM_HISTGAIN].attr)/10 + 1;
	if (!window_layout)
		paintWin(OM_MEASUREWIN, PAINTINIT);
	else
		paintWin(SM_TRACEWIN, PAINTINIT);
}

void setTraceGain(int mouseY)
{
	int y;

	y = (mouseY - OM[SM_TRCGAIN].y) * 1000 / OM[SM_TRCGAIN].dy;
	OM[SM_TRCGAIN].attr = min(max(0, y), 1000);
	traceYGain = 1000 - OM[SM_TRCGAIN].attr;
	paintWin(SM_TRACEWIN, PAINTINIT);
	if (pixel_trace_displayed)
		paintWin(OM_MEASUREWIN, PAINTINIT);
}

void setTraceGainWheel(int inc)
{
	traceYGain += 10*inc;
	traceYGain = min(1000, max(0, traceYGain));
	OM[SM_TRCGAIN].attr = 1000 - traceYGain;
	paintWin(SM_TRACEWIN, PAINTINIT);
	if (pixel_trace_displayed)
		paintWin(OM_MEASUREWIN, PAINTINIT);
}

void setBNCGain(int mouseX, int mouseY)
{
	int x, y;

	if (!window_layout) {
		y = (mouseY - OM[SM_BNCGAIN].y) * 1000 / OM[SM_BNCGAIN].dy;
		OM[SM_BNCGAIN].attr = min(max(0, y), 1000);
		BNC_YGain = 1000 - OM[SM_BNCGAIN].attr;
	}
	else {
		x = (mouseX - OM[SM_BNCGAIN].x) * 1000 / OM[SM_BNCGAIN].dx;
		OM[SM_BNCGAIN].attr = min(max(0, x), 1000);
		BNC_YGain = OM[SM_BNCGAIN].attr;
	}
	paintWin(SM_BNCPNL, PAINTINIT);
	paintWin(SM_TRACEWIN, PAINTINIT);
	if (pixel_trace_displayed)
		paintWin(OM_MEASUREWIN, PAINTINIT);
}

void setTraceClickExp(int mouseX1, int mouseX2)
{
	int x1, x2, frame0_sv, frame2;

	if (dataFile && (trace_exp_max > trace_exp)) {
		frame0_sv = trace_frame0;
		x1 = min(max(mouseX1, OM[SM_TRACEWIN].x+trace_bdr), OM[SM_TRACEWIN].x+OM[SM_TRACEWIN].dx-trace_bdr);
		if (x1 > OM[SM_TRCGAIN].x-trace_bdr)
			x1 = OM[SM_TRCGAIN].x - trace_bdr;
		trace_frame0 = (int)((double)num_trace_pnts*(x1 - (OM[SM_TRACEWIN].x+trace_bdr))/(OM[SM_TRACEWIN].dx-OM[SM_TRCGAIN].dx-trace_bdr*2) + 0.5) + frame0_sv;
		x2 = min(max(mouseX2, OM[SM_TRACEWIN].x+trace_bdr), OM[SM_TRACEWIN].x+OM[SM_TRACEWIN].dx-trace_bdr);
		if (x2 > OM[SM_TRCGAIN].x-trace_bdr)
			x2 = OM[SM_TRCGAIN].x - trace_bdr;
		if (x2 <= x1)
			x2 = x1 + 1;
		frame2 = (int)((double)num_trace_pnts*(x2 - (OM[SM_TRACEWIN].x+trace_bdr))/(OM[SM_TRACEWIN].dx-OM[SM_TRCGAIN].dx-trace_bdr*2) + 0.5) + frame0_sv;
		trace_exp = numTraceFrames/(frame2-trace_frame0+1);
		if (trace_exp > trace_exp_max)
			trace_exp = trace_exp_max;
		num_trace_pnts = numTraceFrames/trace_exp;
		OM[SM_TRCEXP].attr = min(max(0, trace_exp*1000/(trace_exp_max-1)), 1000);
		if (trace_frame0 > numTraceFrames - num_trace_pnts)
			trace_frame0 = numTraceFrames - num_trace_pnts;
		OM[SM_TRCSTART].attr = min(trace_frame0*1000/(numTraceFrames-1), 1000);
		if (resetDispFrame(0))
			paintWin(OM_MEASUREWIN, PAINTINIT);
		setTraceThumb(-1, 0);
		paintWin(SM_TRACEWIN, PAINTINIT);
	}
}

void setTraceExpWheel(int inc)
{
	int exp_sv; 

	exp_sv = trace_exp;
	if (inc > 0)
		trace_exp++;
	else
		trace_exp--;
	trace_exp = min(trace_exp_max, max(1, trace_exp));
	OM[SM_TRCEXP].attr = min(max(0, trace_exp * 1000 / (trace_exp_max - 1)), 1000);
	if (exp_sv != trace_exp) {
		num_trace_pnts = numTraceFrames / trace_exp;
		trace_frame0 = curDispFrame - (curDispFrame - trace_frame0)* exp_sv / trace_exp;
		trace_frame0 = min(numTraceFrames - num_trace_pnts, max(0, trace_frame0));
		OM[SM_TRCSTART].attr = min(trace_frame0 * 1000 / (numTraceFrames - 1), 1000);
		if (resetDispFrame(0))
			paintWin(OM_MEASUREWIN, PAINTINIT);
		setTraceThumb(-1, 0);
		paintWin(SM_TRACEWIN, PAINTINIT);
	}
}

void setTraceExp(int mouseX)
{
	int exp_x, exp_sv;

	if (!dataFile)
		return;
	if (mouseX >= 0) {
		exp_sv = trace_exp;
		exp_x = (mouseX - OM[SM_TRCEXP].x) * 1000 / OM[SM_TRCEXP].dx;
		OM[SM_TRCEXP].attr = min(max(0, exp_x), 1000);

		trace_exp = min((int)(1 + OM[SM_TRCEXP].attr*(trace_exp_max-1) / 1000.0 + 0.5), trace_exp_max);
	}
	else {
		exp_sv = 0;
		OM[SM_TRCEXP].attr = min(max(0, (trace_exp-1) * 1000 / (trace_exp_max-1)), 1000);
	}

	if (trace_exp != exp_sv) {
		num_trace_pnts = numTraceFrames/trace_exp;
		if (trace_frame0 > numTraceFrames - num_trace_pnts) {
			trace_frame0 = numTraceFrames - num_trace_pnts;
			OM[SM_TRCSTART].attr = min(trace_frame0*1000/(numTraceFrames-1), 1000);
		}
		if (resetDispFrame(0))
			paintWin(OM_MEASUREWIN, PAINTINIT);
		setTraceThumb(-1, 0);
		paintWin(SM_TRACEWIN, PAINTINIT);
	}
	else 
		paintWin(SM_TRCEXP, PAINTINIT);
}

void setTraceLowPass(int mouseX, int drawTraceFlag)
{
	int lpass_x;

	if (!dataFile)
		return;
	if (mouseX >= 0) {
		lpass_x = (mouseX - OM[SM_LOWPASS].x) * 1000 / OM[SM_LOWPASS].dx;
		OM[SM_LOWPASS].attr = min(max(0, lpass_x), 1000);
		trace_lpass = OM[SM_LOWPASS].attr*trace_lpass_max / 1000.0;
	}
	else
		OM[SM_LOWPASS].attr = (int)min(trace_lpass*1000/ trace_lpass_max, 1000);
		
	if (drawTraceFlag)
		paintWin(SM_TRACEWIN, PAINTINIT);
	else 
		paintWin(SM_LOWPASS, PAINTINIT);
	if (pixel_trace_displayed)
		paintWin(OM_MEASUREWIN, PAINTINIT);
}

void setTraceHighPass(int mouseX, int drawTraceFlag)
{
	int hpass_x;

	if (!dataFile)
		return;
	if (mouseX >= 0) {
		hpass_x = (mouseX - OM[SM_HIGHPASS].x)*1000/OM[SM_HIGHPASS].dx;
		OM[SM_HIGHPASS].attr = min(max(0, hpass_x), 1000);
		trace_hpass = OM[SM_HIGHPASS].attr*trace_hpass_max/1000.0;
	}
	else
		OM[SM_HIGHPASS].attr = (int)min(trace_hpass * 1000 / trace_hpass_max, 1000);
	if (drawTraceFlag)
		paintWin(SM_TRACEWIN, PAINTINIT);
	else 
		paintWin(SM_HIGHPASS, PAINTINIT);
}

void setRefStart(int mouseX)
{
	int x, start_sv, ref_stt;

	if (!dataFile)
		return;

	start_sv = (ref_frame1 + ref_frame2)/2;
	x = (mouseX - OM[SM_REFSTART].x) * 1000 / OM[SM_REFSTART].dx;
	OM[SM_REFSTART].attr = min(max(0, x), 1000);
	ref_stt = min((int)(OM[SM_REFSTART].attr*numTraceFrames / 1000.0 + 0.5), numTraceFrames - 1);
	if (ref_stt != start_sv) {
		ref_frame1 += ref_stt - start_sv;
		ref_frame2 += ref_stt - start_sv;
		ref_frame1 = min(max(ref_frame1, 0), numDataFrames - 1);
		ref_frame2 = min(max(ref_frame2, ref_frame1), numDataFrames - 1);
		if (f_subtract_mode) {
			getRefImage();
			getStreamFrame();
		}
		paintWin(SM_TRACEWIN, PAINTINIT);
	}
}

void setTraceStart(int mouseX)
{
	int x, start_sv, exp_sv;

	if (!dataFile)
		return;

	start_sv = trace_frame0;
	exp_sv = trace_exp;
	x = (mouseX - OM[SM_TRCSTART].x)*1000/OM[SM_TRCSTART].dx;
	OM[SM_TRCSTART].attr = min(max(0, x), 1000);
	trace_frame0 = min((int)(OM[SM_TRCSTART].attr*numTraceFrames/1000.0 + 0.5), numTraceFrames-1);
	if (trace_frame0 > numTraceFrames - num_trace_pnts) {
		if (trace_exp > 1) {
			if (trace_frame0 > numTraceFrames - numTraceFrames/trace_exp_max) {
				trace_frame0 = numTraceFrames - numTraceFrames/trace_exp_max;
				OM[SM_TRCSTART].attr = min(trace_frame0*1000/(numTraceFrames-1), 1000);
			}
			num_trace_pnts = numTraceFrames - trace_frame0;
			trace_exp = numTraceFrames/num_trace_pnts;
			if (trace_exp != exp_sv)
				OM[SM_TRCEXP].attr = min(max(0, (trace_exp-1)*1000/(trace_exp_max-1)), 1000);
		}
	}
	if (trace_frame0 != start_sv) {
		if (resetDispFrame(0))
			paintWin(OM_MEASUREWIN, PAINTINIT);
		setTraceThumb(-1, 0);
		paintWin(SM_TRACEWIN, PAINTINIT);
	}
}

void printImgScale(int win)
{
	char str[50];

	if (OM[win].anim_state == OM_WARM && (win == SM_IMGSCBASE || OM[win].parent == SM_IMGSCBASE)) {
		sprintf(str, "Image Scale (%d to %d)", scale_min, scale_max);
		drawText(0, 0, str, OM[SM_IMGSCBASE].x + 90, OM[SM_IMGSCBASE].y + OM[SM_IMGSCBASE].dy - 18, colorRegular, omFontRprtTn, TRUE);
	}
}

void setImgScale(int mouseX)
{
	int x, x1, x2;
	int smin_sv, smax_sv;

	if (autoScaleFlag) {
		autoScaleFlag = 0;
		setChckBox(SM_AUTOSBOX);
		repaintMenubox();
	}

	smin_sv = scale_min;
	smax_sv = scale_max;

	x = (mouseX - OM[SM_IMGSCALE].x)*(scaleRight-scaleLeft)/OM[SM_IMGSCALE].dx + scaleLeft;

	x1 = OM[SM_IMGSCALE].x + (int)((double)OM[SM_IMGSCALE].dx*(scale_min-scaleLeft)/(scaleRight-scaleLeft) + 0.5);
	x2 = OM[SM_IMGSCALE].x + (int)((double)OM[SM_IMGSCALE].dx*(scale_max-scaleLeft)/(scaleRight-scaleLeft) + 0.5);
	if (abs(mouseX-x1) < abs(mouseX-x2)) {
		scale_min = min(max(x, scaleLeft), scaleRight);
		if (scale_min >= scale_max)
			scale_min = scale_max - 1;
	}
	else {
		scale_max = min(max(x, scaleLeft), scaleRight);
		if (scale_max <= scale_min)
			scale_max = scale_min + 1;
	}

	if (smin_sv != scale_min || smax_sv != scale_max) {
		paintWin(OM_MEASUREWIN, PAINTINIT);
		UpdateMScale();
	}
}

int isImgScButton(int win)
{
	return ((OM[win].id == SM_IMGSCBASE || OM[win].parent == SM_IMGSCBASE) && dataExist && (win != SM_IMGSCALE));
}

int isImgScSlider(int win)
{
	return (OM[win].id == SM_IMGSCALE && dataExist);
}

int isHistoExp(int win)
{
	return (OM[win].id == SM_HISTEXP && dataExist);
}

int isHistoGain(int win)
{
	return (OM[win].id == SM_HISTGAIN && dataExist);
}

int isTraceGain(int win)
{
	return (OM[win].id == SM_TRCGAIN);
}

int isBNCGain(int win)
{
	return (OM[win].id == SM_BNCGAIN);
}

int isTraceExp(int win)
{
	return (OM[win].id == SM_TRCEXP);
}

int isTraceButton(int win)
{
	return ((OM[win].state == OM_ARROW && OM[win].parent == SM_TRACEWIN));
}

int isTraceLowPass(int win)
{
	return (OM[win].id == SM_LOWPASS);
}

int isTraceHighPass(int win)
{
	return (OM[win].id == SM_HIGHPASS);
}

int isTraceStart(int win)
{
	return (OM[win].id == SM_TRCSTART);
}

int isRefStart(int win)
{
	return (OM[win].id == SM_REFSTART);
}

int isTrace(int win)
{
	return (OM[win].id == SM_TRACEWIN);
}

int isTraceThumb(int mouseX)
{
	if (abs(trace_glider_x-mouseX) <= 15 || (mouseX < 0))
		return TRUE;
	else
		return FALSE;
}

int isMovSlider(int win)
{
	return ((OM[win].parent == SM_MOVIEPNL || OM[win].parent == SM_CARDIOPNL) && OM[win].state == OM_SLIDER);
}

void PhotonTransferAcq()
{
	signed short int *pt;
	if ((cam_num_row <= 0) || (cam_num_col <= 0))
		return;
 
	if (image_data != NULL)
		_aligned_free(image_data);
	image_data = (signed short *)_aligned_malloc(pht_frames*cam_num_col*cam_num_row * 2, 2);
	SM_take_Dark(image_data, pht_frames, pdv_chan_num, cds_lib[curConfig], layers_lib[curConfig], stripes_lib[curConfig], frame_interval, cameraType, file_frame_w, ccd_lib_bin[curConfig] / 2, QuadFlag, rotate_lib[curConfig],
		bad_pix_lib[curConfig], ndr_lib[curConfig], NDR_subtraction_frames, segment_lib[curConfig]);

	pt = (signed short *)image_data;
	pt += cam_num_col*cam_num_row*pht_DFrame;
	memcpy(single_img_data, pt, cam_num_col*cam_num_row*2);

	signed short *ptr = single_img_data;
	float *f_ptr = single_img_float;
	for (int m = 0; m < cam_num_col*cam_num_row; m++)
		*f_ptr++ = (float)(*ptr++);

	dataExist = TRUE;
	dataFile = TRUE;
	disp_num_col = cam_num_col;
	disp_num_row = cam_num_row;
	numTraceFrames = pht_frames;
	numDataFrames = numTraceFrames;
	num_trace_pnts = numTraceFrames;
	curDispFrame = pht_DFrame - 1;
	singleF_flag = TRUE;
	activateImgMenu();

	getDispImage(0);
	paintWin(SM_TRACEWIN, PAINTINIT);
}

static int demo_start;
extern unsigned int file_width, file_height;
void AcquireOneFrame(int dispFlag)
{
/*	ULONGLONG starttime = GetTickCount64();
	ULONGLONG endtime;
	char buf[300];

	endtime = GetTickCount64();
	sprintf(buf, "Start time = %d\n", (int)(endtime - starttime));
	OutputDebugString(buf);
	starttime = endtime;*/

	if ((cam_num_row <= 0) || (cam_num_col <= 0))
		return;

	if (demoFlag) {
		signed short *ptr;
		int demo_val;

		config_num_col = cam_num_col;
		config_num_row = cam_num_row;
		ptr = (signed short *)single_img_data;
		for (int j = 0; j < cam_num_row; j++) {
			demo_val = abs(j - demo_start) + 1;
			for (int i = 0; i < cam_num_col; i++)
				*ptr++ = demo_val + 1024;
		}
		demo_start += 1;
		if (demo_start >= cam_num_col)
			demo_start = 0;
	}
	else {
		SM_take_Live(single_img_data, pdv_chan_num, cds_lib[curConfig], 1, layers_lib[curConfig], stripes_lib[curConfig], live_factor, cameraType, file_frame_w, ccd_lib_bin[curConfig] / 2, QuadFlag, rotate_lib[curConfig], 
			bad_pix_lib[curConfig], ndr_lib[curConfig], NDR_subtraction_frames, segment_lib[curConfig]);
		if (TwoK_flag) {
			cam_num_col =  min(config_num_col * (pdv_chan_num >> 1) / (1 + cds_lib[curConfig]), file_frame_w) / max(1, ccd_lib_bin[curConfig]/2);
			cam_num_row = 2 * config_num_row;
		}
		else {
			cam_num_col = file_width;
			cam_num_row = file_height;
		}
		signed short *ptr = single_img_data;
		float *f_ptr = single_img_float;
		for (int m = 0; m < cam_num_col*cam_num_row; m++)
			*f_ptr++ = (float)(*ptr++);

	}
	setLiveFlag(true);

	if (!dataExist) {
		dataExist = TRUE;
		OM[SM_TOOLPNL].state = OM_DISPLAY;
		OM[SM_TPNLTEXT].state = OM_DISPLAY;
		for (int i = SM_DRAW; i <= SM_KSIZEDN; i++)
			OM[i].state = OM_VIDEOBUTTON;
		if (!dataFile)
			OM[SM_TPOLARITY].state = OM_NULL;
	}
	disp_num_col = cam_num_col;
	disp_num_row = cam_num_row;
	singleF_flag = TRUE;
	auto_scale_w = auto_scale_w_acq;
	activateImgMenu();

	if (dispFlag)
		getDispImage(0);
}

/*__inline void ByteSwap(WORD *wX) { 
	_asm {
		mov     ax, word ptr [wX]
		xchg	al,ah
		mov     word ptr [wX], ax
	}
} */

void SM_swapFitsData(unsigned long count, int darkFlag)
{
	unsigned char *p, *q,v;
	unsigned long i = count;
	
	if (darkFlag)
		p = (unsigned char *)dark_data;
	else
		p = (unsigned char *)image_data;
	q = p + 1;
	while (i-- > 0) {
		v = *p;
		*p = *q;
		*q = v;
		p+=2;
		q+=2;
	}

	//	ByteSwap(p++);
}

char *
stripSpaces(char *buf)
{
	char *p;
	int buflen;

	if ((buflen = (int)strlen(buf)) > 0) {
		for (p = buf + buflen - 1; p >= buf; --p)
			if (*p == ' ')
				*p = '\0';
			else
				break;
	}
	return buf;
}

#define fitsCARDS		36
#define fitsCARDLEN		80
#define fitsCARDBUFLEN (fitsCARDLEN+1)
#define fitsCARDOFFSET	10
#define fitsCARDVALLEN (fitsCARDLEN - fitsCARDOFFSET)
#define fitsSIMPLE		0
#define fitsBITPIX		1
#define fitsNAXIS		2
#define fitsNAXIS1		3
#define fitsNAXIS2		4
#define fitsNAXIS3		5
#define fitsDATE		6
#define fitsORIGIN		7
#define fitsCREATOR		8
#define fitsINSTRUME	9
#define fitsEXPOSURE	10
#define fitsSATURATE	11
#define fitsDATAMAX		12
#define fitsDATAMIN		13
#define fitsEND			14
#define fitsTAGS		15
#define fitsTAGLEN		8

char *fitsTags[] = {
	"SIMPLE  ","BITPIX  ","NAXIS   ","NAXIS1  ","NAXIS2  ","NAXIS3  ","DATE    ","ORIGIN  ","CREATOR ","INSTRUME","EXPOSURE","SATURATE","DATAMAX ","DATAMIN ","END     " };

int SM_readHeader(int *images,int *rows,int *columns,char *camera,double *exposure)
{
	char cardBuf[fitsCARDBUFLEN],scanBuf[fitsCARDBUFLEN],*cardValue;
	int fitsCard, fitsTag, done = FALSE;

	for (fitsCard = 0; fitsCard < fitsCARDS && !done; ++fitsCard) {
		strncpy(cardBuf, FitsHeader[fitsCard],fitsCARDLEN);
		cardBuf[fitsCARDLEN] = '\0';
		for (fitsTag = 0;fitsTag < fitsTAGS; ++fitsTag)
			if (strncmp(fitsTags[fitsTag],cardBuf,fitsTAGLEN) == 0)
				break;
		if (fitsTag < fitsTAGS) {
			cardValue = cardBuf + fitsCARDOFFSET;
			switch (fitsTag) {
			case fitsSIMPLE:
				sscanf(cardValue,"%s",scanBuf);
				if (scanBuf[0] != 'T')
					return FALSE;
				break;
			case fitsNAXIS1:
				sscanf(cardValue,"%d",columns);
				break;
			case fitsNAXIS2:
				sscanf(cardValue,"%d",rows);
				break;
			case fitsNAXIS3:
				sscanf(cardValue,"%d",images);
				break;
			case fitsINSTRUME:
				sprintf(scanBuf,"%-*.*s",fitsCARDVALLEN,fitsCARDVALLEN,cardValue);
				stripSpaces(scanBuf);
				strcpy(camera,scanBuf);
				break;
			case fitsEXPOSURE:
				sscanf(cardValue,"%lf",exposure);
				break;
			case fitsEND:
				done = TRUE;
				break;
			}
		}
	}
	return TRUE;
}

char nextName[MAX_PATH];
char *getPrevNextName(char *filename, int prev_flag)
{
	char *p;
	char str[MAX_PATH];
	int fileNum;

	strcpy(str, filename);
	if (p = strstr(str, ".tsm")) {
		*p = '\0';
		fileNum = atoi((p-3));
		if (fileNum)
			*(p-3) = '\0';
		if (prev_flag) {
			if (fileNum > 1) {
				sprintf(nextName, "%s%3d%s", str, fileNum-1, ".tsm");
				while (p = strstr(nextName, " "))
					*p = '0';
			}
			else 
				strcpy(nextName, "");
		}
		else {
			if (fileNum < 999) {
				sprintf(nextName, "%s%3d%s", str, fileNum+1, ".tsm");
				while (p = strstr(nextName, " "))
					*p = '0';
			}
			else 
				strcpy(nextName, "");
		}
	}
	else
		strcpy(nextName, "");
	return nextName;
}

void setSmDataName(char *filename)
{
	char nextFile[MAX_PATH], prevFile[MAX_PATH];
	int nextGray, prevGray;
	int handle;

	strcpy(SM_dataname, filename);
	if (strstr(SM_dataname, ".tsm")) {
		strcpy(prevFile, getPrevNextName(filename, 1));
		if (strcmp(prevFile, "")) {
			if ((handle = _open(prevFile, O_BINARY | O_RDONLY)) == -1)
				prevGray = TRUE;
			else {
				_close(handle);
				prevGray = FALSE;
			}
		}
		else
			prevGray = TRUE;
		strcpy(filename, SM_dataname);
		strcpy(nextFile, getPrevNextName(filename, 0));
		if (strcmp(nextFile, "")) {
			if ((handle = _open(nextFile, O_BINARY | O_RDONLY)) == -1)
				nextGray = TRUE;
			else {
				_close(handle);
				nextGray = FALSE;
			}
		}
		else
			nextGray = TRUE;
		setPrevNextGray(prevGray, nextGray);
	}
}

void setOmitButtons()
{
	if (dataFile) {
		EnableMenuItem(OMIT_menu, IDC_OMIT_ROI, MF_ENABLED);
		EnableMenuItem(OMIT_menu, IDC_OMIT_READ, MF_ENABLED);
		if (mk_omit_flag) {
			EnableMenuItem(OMIT_menu, IDC_OMIT_ON, MF_DISABLED | MF_GRAYED);
			EnableMenuItem(OMIT_menu, IDC_OMIT_OFF, MF_ENABLED);
		}
		else {
			EnableMenuItem(OMIT_menu, IDC_OMIT_OFF, MF_DISABLED | MF_GRAYED);
			EnableMenuItem(OMIT_menu, IDC_OMIT_ON, MF_ENABLED);
		}
		if (omit_ar) {
			EnableMenuItem(OMIT_menu, IDC_OMIT_SAVE, MF_ENABLED);
			EnableMenuItem(OMIT_menu, IDC_OMIT_RMV, MF_ENABLED);
		}
		else {
			EnableMenuItem(OMIT_menu, IDC_OMIT_SAVE, MF_DISABLED | MF_GRAYED);
			EnableMenuItem(OMIT_menu, IDC_OMIT_RMV, MF_DISABLED | MF_GRAYED);
		}
	}
}

void resetDispSettings()
{
	mk_omit_flag = 0;
	trace_subtracted = 0;
	movie_normalized = 0;
	filter_applied = 0;
	if (tools_window_handle) {
		resetDataProc();
		EnableTools(tools_window_handle);
	}
	trace_frame0 = 0;
	trace_exp = 1;
	trace_exp_max = max(2 ,numTraceFrames*40/(OM[SM_TRACEWIN].dx-2*trace_bdr));
	num_trace_pnts = numTraceFrames;
	trace_lpass_max = 0.5/SM_exposure;
	trace_hpass_max = 30.0;
	resetDispFrame(0);
	OM[SM_TRCEXP].attr = 0;
	OM[SM_TRCSTART].attr = 0;

	if (disp_num_col_old != disp_num_col || disp_num_row_old != disp_num_row) {
		trace_min_frame = 20;
		clearTracePix();
		resetZoom(0);
		imageShiftControl();
		repaintMenubox();
		if (overlay_img != NULL) {
			_aligned_free(overlay_img);
			overlay_img = NULL;
		}
		if (omit_ar != NULL) {
			_aligned_free(omit_ar);
			omit_ar = NULL;
		}
	}
	if (movie_data != NULL) {
		_aligned_free(movie_data);
		movie_data = NULL;
		EnableMenuItem(main_menu_handle, IDM_SAVE_MOVIE, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(main_menu_handle, IDM_SAVE_MOVIE2, MF_DISABLED | MF_GRAYED);
	}
	getAllTraces();
	FFT_N_disp = 0;
	mov_start = 0;
	mov_end = numTraceFrames - 1;
	actv_start = 0;
	actv_end = mov_end;
//	setTabOn(OM_MEASUREVIEW);
	setOmitButtons();
}

void alloc_dark_data(int num_col, int num_row)
{
	if (dark_data != NULL)
		_aligned_free(dark_data);
	dark_data = (signed short int *)_aligned_malloc(num_col*num_row*sizeof(signed short int), 2);
	memset(dark_data, 0, num_col*num_row*sizeof(signed short int));
	if (ref_image != NULL)
		_aligned_free(ref_image);
	ref_image = (float *)_aligned_malloc(num_col*num_row * sizeof(float), 2);
	memset(ref_image, 0.0, num_col*num_row * sizeof(float));
}

void get_saveName()
{
	char filename[MAX_PATH], dir[MAX_PATH];
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;

	strcat(strcat(strcat(strcpy(dir, home_dir), dirIMAGES), "\\"), experiment);
	if (!existDir(dir))
		_mkdir(dir);
	strcat(strcat(strcpy(filename, dir), "\\"), biopsy);
	if (!overwriteFlag) {
		while ((findhandle = FindFirstFile(filename, &find_data)) != INVALID_HANDLE_VALUE) {
			FindClose(findhandle);
			strcpy(filename, getPrevNextName(filename, 0));
		}
	}
	else {
		if ((findhandle = FindFirstFile(filename, &find_data)) != INVALID_HANDLE_VALUE) {
			FindClose(findhandle);
			DeleteFile(filename);
		}
	}
	setSmDataName(filename);
}

static char av_filename_ar[FILE_AV_MAX][MAX_PATH];
static int spike_trg_av_ar[FILE_AV_MAX];
void SM_averageData(char *filenames, char *av_Fname, int num_av_acq)
{
	char filename[MAX_PATH], file_dir[MAX_PATH], tmp_str[MAX_PATH];
	char *p, *p_file, *q;
	int file_cnt, av_NFrames, av_num_row, av_num_col, this_fileV;
	int handles[FILE_AV_MAX], av_handle;
	char cameraName[256], str[256];
	long *av_file = NULL;
	int i, k, n, space_flag; // , BNC_av_flag = 0;
	signed short *img_pt;

	if (!num_av_acq) {
		if (!strcmp(filenames, ""))
			return;
		if (strstr(filenames, " "))
			space_flag = 1;
		else
			space_flag = 0;

		if (p = strstr(filenames, ".tsm")) {
			strncpy(file_dir, filenames, (int)(p - filenames));
			*(file_dir + (int)(p - filenames)) = '\0';
			strcpy(tmp_str, file_dir);
			while (p = strstr(tmp_str, "\\"))
				strcpy(tmp_str, (p + 1));
			if (p = strstr(file_dir, tmp_str))
				*p = '\0';
		}
		else
			return;

		file_cnt = 0;
		p_file = filenames + strlen(file_dir) + space_flag;
		while ((p = strstr(p_file, ".tsm")) && (file_cnt < sizeof(handles) / sizeof(handles[0]))) {
			strncpy(filename, p_file, (int)(p - p_file) + 4);
			*(filename + (int)(p - p_file) + 4) = '\0';
			sprintf(tmp_str, "%s%s", file_dir, filename);
			strcpy(av_filename_ar[file_cnt], tmp_str);
			p_file = p + 4 + space_flag;
			file_cnt++;
		}
	}
	else
		file_cnt = num_av_acq;

	int min_spike_pos = spike_pos_ar[0];
	int max_spike_pos = spike_pos_ar[0];
	for (n = 0; n < file_cnt; n++) {
		strcpy(filename, av_filename_ar[n]);
		if ((handles[n] = _open(filename, O_BINARY | O_RDONLY)) == -1) {
			for (i = 0; i < n; i++)
				_close(handles[i]);
			return;
		}
		_lseek(handles[n], 0L, SEEK_SET);
		_read(handles[n], (char *)FitsHeader, sizeof(FitsHeader));
		SM_readHeader(&numDataFrames, &disp_num_row, &disp_num_col, cameraName, &SM_exposure);
		if (!n) {
			av_NFrames = numDataFrames;
			av_num_row = disp_num_row;
			av_num_col = disp_num_col;
		}
		else {
			if ((av_NFrames != numDataFrames) || (av_num_row != disp_num_row) || (av_num_col != disp_num_col)) {
				sprintf(tmp_str, "%s has different image size or file length. Averaging cannot be completed", filename);
				MessageBox(main_window_handle, tmp_str, "message", MB_OK);
				for (i = 0; i <= n; i++)
					_close(handles[i]);
				return;
			}
		}
		if (spike_triggered && min_spike_pos) {
			if (min_spike_pos > spike_pos_ar[n])
				min_spike_pos = spike_pos_ar[n];
			if (max_spike_pos < spike_pos_ar[n])
				max_spike_pos = spike_pos_ar[n];
		}

		if (q = strstr(cameraName, "-VER")) {
			strcpy(str, (q + 4));
			if (q = strstr(str, ","))
				*q = '\0';
			this_fileV = atoi(str);
		}
		else
			this_fileV = 0;
	}

	if ((av_handle = _open(av_Fname, O_BINARY | O_CREAT | O_WRONLY, S_IWRITE)) == -1) {
		for (i = 0; i < file_cnt; i++)
			_close(handles[i]);
		return;
	}
	SM_makeHeader(numDataFrames, disp_num_row, disp_num_col, cameraName, (double)SM_exposure);
	_write(av_handle, (char *)FitsHeader, sizeof(FitsHeader));

	if (image_data != NULL)
		_aligned_free(image_data);
	image_data = (signed short *)_aligned_malloc(curNFrames*cam_num_col*cam_num_row * 2, 2);

	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / numDataFrames;
	sprintf(str, "File Averaging: %d of %d", 0, numDataFrames);
	SetWindowText(hwndPB, str);

	int av_frame_total;
	if (spike_triggered && min_spike_pos) {
		av_frame_total = numDataFrames - (max_spike_pos - min_spike_pos);
		for (i = 0; i < file_cnt; i++)
			_lseek(handles[i], 2880L + ((long)(spike_pos_ar[i] - min_spike_pos))*disp_num_col*disp_num_row * 2, SEEK_SET);
	}
	else
		av_frame_total = numDataFrames;

	av_file = (long *)_aligned_malloc(disp_num_col*disp_num_row * sizeof(long), 2);
	for (n = 0; n < av_frame_total; n++) {
		for (i = 0; i < file_cnt; i++) {
			_read(handles[i], (signed short *)image_data, disp_num_col*disp_num_row * 2);
			img_pt = (signed short *)image_data;
			for (k = 0; k < disp_num_col*disp_num_row; k++) {
				if (i == 0)
					*av_file++ = (long)*img_pt++;
				else
					*av_file++ += (long)*img_pt++;
			}
			av_file -= disp_num_col*disp_num_row;
		}
		img_pt = (signed short *)image_data;
		for (k = 0; k < disp_num_col*disp_num_row; k++)
			*img_pt++ = (short)((*av_file++) / file_cnt);
		av_file -= disp_num_col*disp_num_row;
		_write(av_handle, (signed short *)image_data, disp_num_col*disp_num_row * 2);
		sprintf(str, "File Averaging: %d of %d", n, numDataFrames);
		SetWindowText(hwndPB, str);
		while ((int)(n*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}
	if (spike_triggered && min_spike_pos) {
		for (; n < numDataFrames; n++) {
			_write(av_handle, (signed short *)image_data, disp_num_col*disp_num_row * 2);
			while ((int)(n*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
		}
	}
	//dark image
	_lseek(handles[0], 2880L + ((long)numDataFrames)*disp_num_col*disp_num_row * 2, SEEK_SET);
	_read(handles[0], (signed short *)image_data, disp_num_col*disp_num_row * 2);	
	_lseek(av_handle, 2880L + ((long)numDataFrames)*disp_num_col*disp_num_row * 2, SEEK_SET);
	_write(av_handle, (signed short *)image_data, disp_num_col*disp_num_row * 2);

	for (i = 0; i < file_cnt; i++)
		_close(handles[i]);
	_close(av_handle);

	if (av_file != NULL)
		_aligned_free(av_file);
	av_file = NULL;

	signed short *BNC_pt = NULL;
	unsigned long BNC_Length, BNC_Length_int;
	int handle;
	int NI_file_flag;
	short int file_BNC_ratio = 0, file_numBNC_chan = 0;
	char filenames_BNC[MAX_PATH];
	float64 *NI_spt = NULL, *NI_file_data = NULL;
	float64 *av_BNC_pt = NULL, *av_BNC_data = NULL;

	for (i = 0; i < file_cnt; i++) {
		strcpy(filenames_BNC, av_filename_ar[i]);
		if (p = strstr(filenames_BNC, ".tsm"))
			*p = '\0';
		sprintf(filename, "%s.tbn", filenames_BNC);
		if ((handle = _open(filename, O_BINARY | O_RDONLY)) != -1) {
			_read(handle, (void *)&file_numBNC_chan, sizeof(file_numBNC_chan));
			_read(handle, (void *)&file_BNC_ratio, sizeof(file_BNC_ratio));
			// correct an error in old BNC files
			if (!this_fileV) {
				file_numBNC_chan = 4;
				file_BNC_ratio = 1;
			}
			if (file_numBNC_chan < 0) {
				file_numBNC_chan = -file_numBNC_chan;
				NI_file_flag = 1;
			}
			BNC_Length = numDataFrames * file_BNC_ratio * file_numBNC_chan * sizeof(float64);
			if (i == 0)
				av_BNC_data = (float64 *)_aligned_malloc(BNC_Length, 2);
			av_BNC_pt = av_BNC_data;
			if (NI_file_flag) {
				if (i == 0)
					NI_file_data = (float64 *)_aligned_malloc(BNC_Length, 2);
				NI_spt = NI_file_data;
				_read(handle, NI_spt, BNC_Length);
				_close(handle);
				for (k = 0l; k < ((long)numDataFrames)*file_numBNC_chan*file_BNC_ratio; k++) {
					if (i == 0)
						*av_BNC_pt++ = (float64)*NI_spt++;
					else
						*av_BNC_pt++ += (float64)*NI_spt++;
				}
			}
			else {
				BNC_Length_int = numDataFrames * file_BNC_ratio*file_numBNC_chan * 2;
				BNC_pt = (signed short *)BNC_data;
				_read(handle, BNC_pt, BNC_Length_int);
				_close(handle);
				for (k = 0l; k < ((long)numDataFrames)*file_numBNC_chan*file_BNC_ratio; k++) {
					if (i == 0)
						*av_BNC_pt++ = (float64)*BNC_pt++;
					else
						*av_BNC_pt++ += (float64)*BNC_pt++;
				}
			}
		}
		else
			goto av_exit;
		strcpy(filenames_BNC, filenames);
	}
	av_BNC_pt = av_BNC_data;
	BNC_pt = (signed short *)BNC_data;
	for (k = 0; k < numDataFrames*file_numBNC_chan*file_BNC_ratio; k++, av_BNC_pt++, BNC_pt++) {
		*av_BNC_pt = (*av_BNC_pt) / file_cnt;
		*BNC_pt = (signed short) *av_BNC_pt;
	}
	strcpy(filename, av_Fname);
	p = strstr(filename, ".tsm");
	*p = '\0';
	strcat(filename, ".tbn");
	if ((handle = _open(filename, O_BINARY | O_CREAT | O_WRONLY, S_IWRITE)) == -1)
		goto av_exit;

	if (NI_file_flag)
		file_numBNC_chan = -file_numBNC_chan;
	_write(handle, (void *)&file_numBNC_chan, sizeof(file_numBNC_chan));
	_write(handle, (void *)&BNC_ratio, sizeof(file_BNC_ratio));
	if (NI_file_flag)
		_write(handle, av_BNC_data, BNC_Length);
	else
		_write(handle, BNC_data, BNC_Length_int);
	_commit(handle);
	_close(handle);

av_exit:

	DestroyWindow(hwndPB);
	SM_readData(av_Fname);
}

void readNPheader(int *images, int *rows, int *columns, char *camera, double *exposure)
{
	char NPcam_gain_ar[][4] = {"30X", "10X", "3X", "1X"};

	if (NP_header[389] > 1 && NP_header[392] == 1)
		*exposure = 1000.0 / NP_header[399];
	else
		*exposure = NP_header[388] / 1000.0;
	*exposure /= 1000;
	int div_factor = NP_header[390];
	if (div_factor >= 10)
		*exposure *= div_factor;
	*images = NP_header[4];
	*columns = NP_header[384];
	*rows = NP_header[385];
	file_BNC_ratio = NP_header[391];
	sprintf(camera, "NPfile-Gain%s-Bin%d", NPcam_gain_ar[NP_header[389]-2], NP_header[386]);
}

void SetNewFileRead(int BNC_f_flag)
{
	dataExist = TRUE;
	dataFile = TRUE;
	setToolPanel(0);
	d_darkExist = TRUE;
//	f_subtract_mode = 0;
	activateImgMenu();

	if (!BNC_f_flag) {
		darkSubFlag = TRUE;
		if (!OM[SM_DKSUBBOX].attr)
			setChckBox(SM_DKSUBBOX);
		enableDataButtons();
	}
	else {
		EnableMenuItem(main_menu_handle, IDM_TRACE_PS, MF_ENABLED);
		EnableMenuItem(main_menu_handle, IDM_TRACE_TXT, MF_ENABLED);
		EnableMenuItem(main_menu_handle, IDM_TRACE_TXT2, MF_ENABLED);
	}
	resetDispSettings();
	dispSettingCheck();
	setTraceThumb(-1, 0);

/*	RLI_mean = histMeans[0];
	traceYGain = (int)(2000 / log(RLI_mean));
	traceYGain = min(1000, max(0, traceYGain));
	OM[SM_TRCGAIN].attr = 1000 - traceYGain;*/

	paintWin(SM_TRACEWIN, PAINTINIT);
	paintWin(SM_IMGSCBASE, PAINTINIT);
	save_sm_f_f2 = numTraceFrames - 1;
	save_sm_f_w = disp_num_col;
	save_sm_f_h = disp_num_row;
	UpdateMScale();
}

void SM_readNPdata(char *filename)
{
	int handle, i, k;

	if (strstr(filename, ".tif")) {
		SM_readTIFF_stack(filename, 0);
		return;
	}

	SM_SetHourCursor();

	setSmDataName(filename);
    if ((handle = _open(SM_dataname,O_BINARY | O_RDONLY)) == -1)
		return;

	if (strcmp(last_dataname, SM_dataname))
		firstDataFrame = 0;
	strcpy(last_dataname, SM_dataname);

	_lseek(handle,0L,SEEK_SET);
	_read(handle,(char *)NP_header,sizeof(NP_header));
	readNPheader(&numDataFrames, &disp_num_row, &disp_num_col, file_cameraName, &SM_exposure);
	file_num_col = disp_num_col;
	file_num_row = disp_num_row;

	firstDataFrame = 0;
	numTraceFrames = numDataFrames;
	int len = disp_num_col * disp_num_row + 8 * file_BNC_ratio;

	if (image_data != NULL)
		_aligned_free(image_data);
	image_data = (signed short int *)_aligned_malloc(numTraceFrames*len * sizeof(signed short int), 2);

	if (firstDataFrame > 0)
		_lseeki64(handle,2560L+((unsigned _int64)firstDataFrame)*len *2,SEEK_SET);
	_read(handle,(signed short *)image_data, len*numTraceFrames*2);
	if (numTraceFrames < numDataFrames-firstDataFrame)
		_lseeki64(handle,2880L+((unsigned _int64)numDataFrames)*len *2,SEEK_SET);
	alloc_dark_data(disp_num_col,disp_num_row);
	if (dark_data != NULL)
		_read(handle,(signed short *)dark_data,disp_num_col*disp_num_row*2);
	_close(handle);
	addFileList();

	//rearrange camera and BNC data
	signed short int *tmp_data = (signed short int *)_aligned_malloc(numTraceFrames*len * sizeof(signed short int), 2);
	memcpy(tmp_data, image_data, numTraceFrames*len * sizeof(signed short int));
	signed short int *src_pt, *dst_pt;
	src_pt = tmp_data;
	dst_pt = image_data;
	for (k = 0; k < numTraceFrames; k++) {
		src_pt = tmp_data + k;
		for (i = 0; i < disp_num_col*disp_num_row; i++) {
			*dst_pt++ = *src_pt;
			src_pt += numTraceFrames;
		}
	}
	int BNC_Length = numTraceFrames * file_BNC_ratio * 8 * 2;
	src_pt = tmp_data + disp_num_col * disp_num_row*numTraceFrames;
	memcpy(BNC_data, src_pt, BNC_Length);
	_aligned_free(tmp_data);
	signed short *BNC_pt = BNC_data;
	for (i = 0; i < BNC_Length / 2; i++)
		*BNC_pt++ = (signed short)(*BNC_pt*10 / 32.767);
	numBNC_chan = 8;
	setBNC_true();

	//Get file time
	struct stat attrib;
	stat(SM_dataname, &attrib);
	fileTime = gmtime(&(attrib.st_mtime));

	if (single_img_size < (unsigned long)disp_num_col*disp_num_row) {
		if (single_img_data != NULL)
			_aligned_free(single_img_data);
		single_img_size = (unsigned long)disp_num_col*disp_num_row;
		single_img_data = (signed short int *)_aligned_malloc(single_img_size * sizeof(signed short int), 2);
		if (single_img_float != NULL)
			_aligned_free(single_img_float);
		single_img_float = (float *)_aligned_malloc(single_img_size * sizeof(float), 2);
	}

	SetNewFileRead(0);
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

static int BNC_only_header_size = 512;
void SM_saveBNC_only(char *filename)
{
	int handle, handle0;
	short int file_numBNC_chan;
	int BNC_num_frames;
	float64 *NI_file_data = NULL;
	unsigned long BNC_Length;
	char BNCname[MAX_PATH];

	if ((handle = _open(filename, O_MODE, S_MODE)) == -1)
		return;

	strcpy(BNCname, SM_dataname);
	char *p = strstr(BNCname, ".tsm");
	*p = '\0';
	strcat(BNCname, ".tbn");
	if ((handle0 = _open(BNCname, O_BINARY | O_RDONLY)) != -1) {
		_read(handle0, (void *)&file_numBNC_chan, sizeof(file_numBNC_chan));
		_read(handle0, (void *)&file_BNC_ratio, sizeof(file_BNC_ratio));
		BNC_num_frames = file_BNC_ratio * numDataFrames;
		BNC_Length = BNC_num_frames * numBNC_chan * sizeof(float64);
		NI_file_data = (float64 *)_aligned_malloc(BNC_Length, 2);
		_read(handle0, NI_file_data, BNC_Length);
		_close(handle0);

		_write(handle, (void *)&numBNC_chan, sizeof(numBNC_chan));
		_write(handle, (void *)&BNC_num_frames, sizeof(BNC_num_frames));
		_write(handle, (void *)&SM_exposure, sizeof(SM_exposure));
		_lseek(handle, BNC_only_header_size, SEEK_SET);
		_write(handle, NI_file_data, BNC_Length);
		_commit(handle);
		_close(handle);
		if (NI_file_data)
			_aligned_free(NI_file_data);
	}
}

void SM_readBNC_only(char *filename)
{
	int handle, i, k;
	signed short *trace_dpt;
	float64 *NI_spt = NULL, *NI_file_data = NULL;
	unsigned long BNC_Length;

	if ((handle = _open(filename, O_BINARY | O_RDONLY)) == -1)
		return;

	strcpy(SM_dataname, filename);
	_read(handle, (void *)&numBNC_chan, sizeof(numBNC_chan));
	_read(handle, (void *)&numDataFrames, sizeof(numDataFrames));
	_read(handle, (void *)&SM_exposure, sizeof(SM_exposure));
	dataReadBlock = 10000;
	numTraceFrames = min(dataReadBlock, numDataFrames - firstDataFrame);
	BNC_Length = numTraceFrames * numBNC_chan * sizeof(float64);
	NI_file_data = (float64 *)_aligned_malloc(BNC_Length, 2);
	_lseek(handle, BNC_only_header_size, SEEK_SET);
	_read(handle, NI_file_data, BNC_Length);
	NI_spt = NI_file_data;
	trace_dpt = (signed short *)BNC_data;
	for (i = 0; i < numBNC_chan; i++) {
		_lseeki64(handle, ((unsigned _int64)firstDataFrame) * sizeof(float64) + ((unsigned _int64)numDataFrames)*i * sizeof(float64) + BNC_only_header_size, SEEK_SET);
		_read(handle, NI_file_data, BNC_Length);
		NI_spt = NI_file_data;
		for (k = 0; k < numTraceFrames; k++) {
			*trace_dpt++ = (signed short)(*NI_spt * 1000); //16384/10.0);
			NI_spt++; // += file_BNC_ratio;
		}
	}

	if (NI_file_data)
		_aligned_free(NI_file_data);

	//Get file time
	strcpy(SM_dataname, filename);
	struct stat attrib;
	stat(SM_dataname, &attrib);
	fileTime = gmtime(&(attrib.st_mtime));

	BNCfile_only = 1;
	BNC_ratio = 1;
	disp_num_row = 10;
	disp_num_col = 10;
	file_num_col = disp_num_col;
	file_num_row = disp_num_row;
	setBNC_true();

	SetNewFileRead(1);
}

void SM_readData(char *filename)
{
	int handle, i, k, this_fileV;
	char *q;
	char str[256];
	char BNCname[MAX_PATH];
	int data_pt_size;

	setTabOn(OM_MEASUREVIEW);

	BNCfile_only = 0;
	if (strstr(filename, ".bnc")) {
		SM_readBNC_only(filename);
		return;
	}
	else if (strstr(filename, ".tif") || strstr(filename, ".TIF") || strstr(filename, ".Tif")) {
		SM_readTIFF_stack(filename, 0);
		return;
	}
	else if (strstr(filename, ".da")) {
		SM_readNPdata(filename);
		return;
	}

	SM_SetHourCursor();

	setSmDataName(filename);
	if ((handle = _open(SM_dataname, O_BINARY | O_RDONLY)) == -1)
		return;

	if (strcmp(last_dataname, SM_dataname))
		firstDataFrame = 0;
	strcpy(last_dataname, SM_dataname);

	_lseek(handle, 0L, SEEK_SET);
	_read(handle, (char *)FitsHeader, sizeof(FitsHeader));
	SM_readHeader(&numDataFrames, &disp_num_row, &disp_num_col, file_cameraName, &SM_exposure);
	file_num_col = disp_num_col;
	file_num_row = disp_num_row;

	if (q = strstr(file_cameraName, "-VER")) {
		strcpy(str, (q + 4));
		if (q = strstr(str, ","))
			*q = '\0';
		this_fileV = atoi(str);
		if (this_fileV > file_version)
			MessageBox(main_window_handle, "You may need a new version of software to read this data file", "message", MB_OK);
	}
	else
		this_fileV = 0;
	if (q = strstr(file_cameraName, "VER1,")) {
		strcpy(str, q + 5);
		if (q = strstr(str, "BIN")) {
			strcpy(str, q + 3);
			if (q = strstr(str, "S"))
				*q = '\0';
			if (strlen(str) > 0)
				sscanf(str, "%d", &file_bin);
			else
				file_bin = 2;
		}
		else
			file_bin = 1;
	}
	else
		file_bin = 0;

	if (q = strstr(file_cameraName, "-float")) {
		*q = '\0';
		float_flag = 1;
		data_pt_size = sizeof(float32);
	}
	else {
		float_flag = 0;
		data_pt_size = sizeof(signed short int);
	}

	dataReadBlock = (int)(min(1000000000L / (disp_num_col*disp_num_row), MAX_DATA_FRAMES));
	if (dataReadBlock > MAX_DATA_FRAMES / 2)
		dataReadBlock = (dataReadBlock / 1000) * 1000;
	else if (dataReadBlock > MAX_DATA_FRAMES / 20)
		dataReadBlock = (dataReadBlock / 100) * 100;
	else
		dataReadBlock = (dataReadBlock / 50) * 50;

	if (!dataReadBlock)
		dataReadBlock = 10;

	numTraceFrames = min(numDataFrames - firstDataFrame, dataReadBlock);

	if (firstDataFrame > 0)
		_lseeki64(handle, 2880L + ((unsigned _int64)firstDataFrame)*disp_num_col*disp_num_row * data_pt_size, SEEK_SET);

	if (image_data != NULL)
		_aligned_free(image_data);
	image_data = (signed short int *)_aligned_malloc(numTraceFrames*disp_num_col*disp_num_row * sizeof(signed short int), 2);
	if (float_flag) {
		if (image_data_f != NULL)
			_aligned_free(image_data_f);
		image_data_f = (float32 *)_aligned_malloc(numTraceFrames*disp_num_col*disp_num_row * data_pt_size, 2);
		_read(handle, (float32 *)image_data_f, disp_num_col*disp_num_row*numTraceFrames * data_pt_size);
		signed short int *dpt = image_data;
		float32 *fdpt = image_data_f;
		for (int i = 0; i < numTraceFrames*disp_num_col*disp_num_row; i++)
			*dpt++ = (signed short int)(*fdpt++);
	}
	else
		_read(handle, (signed short *)image_data, disp_num_col*disp_num_row*numTraceFrames * 2);
	if (numTraceFrames < numDataFrames - firstDataFrame)
		_lseeki64(handle, 2880L + ((unsigned _int64)numDataFrames)*disp_num_col*disp_num_row * data_pt_size, SEEK_SET);
	alloc_dark_data(disp_num_col, disp_num_row);
	if (dark_data != NULL)
		_read(handle, (signed short *)dark_data, disp_num_col*disp_num_row * 2);
	_close(handle);
	addFileList();

	//Get file time
	struct stat attrib;
	stat(SM_dataname, &attrib);
	fileTime = gmtime(&(attrib.st_mtime));

	EnableMenuItem(main_menu_handle, IDM_BNC_TXT, MF_DISABLED | MF_GRAYED);
	strcpy(BNCname, SM_dataname);
	char *p;
	if (p = strstr(BNCname, ".tsm")) {
		*p = '\0';
		strcat(BNCname, ".tbn");
		if ((handle = _open(BNCname, O_BINARY | O_RDONLY)) != -1) {
			short int file_numBNC_chan = 0, NI_file_flag = 0;
				_read(handle, (void *)&file_numBNC_chan, sizeof(file_numBNC_chan));
				_read(handle, (void *)&file_BNC_ratio, sizeof(file_BNC_ratio));
				// correct an error in old BNC files
				if (!this_fileV) {
					file_numBNC_chan = 4;
						file_BNC_ratio = 1;
				}
			if (file_numBNC_chan < 0) {
				file_numBNC_chan = -file_numBNC_chan;
				NI_file_flag = 1;
			}
			if (NI_file_flag)
				EnableMenuItem(main_menu_handle, IDM_BNC_ONLY_OUT, MF_ENABLED);
			if (file_BNC_ratio > 1)
				EnableMenuItem(main_menu_handle, IDM_BNC_TXT, MF_ENABLED);

			signed short *trace_spt, *trace_dpt;
			float64 *NI_spt = NULL, *NI_file_data = NULL;
			unsigned long BNC_Length;
			if (NI_file_flag) {
				BNC_Length = numTraceFrames * file_BNC_ratio * sizeof(float64);
				NI_file_data = (float64 *)_aligned_malloc(BNC_Length, 2);
			}
			else
				BNC_Length = numTraceFrames * file_BNC_ratio * 2;
			trace_dpt = (signed short *)BNC_data;
			for (i = 0; i < file_numBNC_chan; i++) {
				trace_spt = (signed short *)BNC_data + numTraceFrames * i*file_BNC_ratio;
				if (NI_file_flag) {
					_lseeki64(handle, ((unsigned _int64)firstDataFrame)*file_BNC_ratio * sizeof(float64) + ((unsigned _int64)numDataFrames)*file_BNC_ratio*i * sizeof(float64) + 4, SEEK_SET);
					_read(handle, NI_file_data, BNC_Length);
					NI_spt = NI_file_data;
				}
				else {
					_lseeki64(handle, ((unsigned _int64)firstDataFrame)*file_BNC_ratio * 2 + ((unsigned _int64)numDataFrames)*file_BNC_ratio*i * 2 + 4, SEEK_SET);
					_read(handle, trace_spt, BNC_Length);
				}
				for (k = 0; k < numTraceFrames * file_BNC_ratio; k++) {
					if (NI_file_flag) {
						*trace_dpt = (signed short)(*NI_spt * 1000); //16384/10.0);
						NI_spt++; // += file_BNC_ratio;
					}
					else {
						memcpy(trace_dpt, trace_spt, sizeof(signed short));
						trace_spt++; // += file_BNC_ratio;
					}
					trace_dpt++;
				}
			}
			if (NI_file_data)
				_aligned_free(NI_file_data);
			_close(handle);
			numBNC_chan = file_numBNC_chan;
			setBNC_true();
		}
		else {
			if (BNC_exist) {
				clearTracePix();
				disableBNCs();
			}
		}
	}
	else {
		if (BNC_exist) {
			clearTracePix();
			disableBNCs();
		}
	}

	if (single_img_size < (unsigned long)disp_num_col*disp_num_row) {
		if (single_img_data != NULL)
			_aligned_free(single_img_data);
		single_img_size = (unsigned long)disp_num_col*disp_num_row;
		single_img_data = (signed short int *)_aligned_malloc(single_img_size * sizeof(signed short int), 2);
		if (single_img_float != NULL)
			_aligned_free(single_img_float);
		single_img_float = (float *)_aligned_malloc(single_img_size * sizeof(float), 2);
	}

	SetNewFileRead(0);
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

void saveBNCtext(char *txtName)
{
	char BNCname[MAX_PATH];
	int handle, i;
	unsigned long k;
	FILE *fp;
	char *p;

	strcpy(BNCname, SM_dataname);
	p = strstr(BNCname,".tsm");
	*p = '\0';
	strcat(BNCname, ".tbn");
    if ((handle = _open(BNCname,O_BINARY | O_RDONLY)) != -1) {
		short int file_BNC_ratio = 0, file_numBNC_chan = 0;
		_read(handle,(void *)&file_numBNC_chan,sizeof(file_numBNC_chan));
		_read(handle,(void *)&file_BNC_ratio,sizeof(file_BNC_ratio));

		if (FOPEN_S(fp,txtName,"w")) {
			unsigned long BNC_Length = numTraceFrames*file_BNC_ratio*2;
			unsigned long BNC_block = sizeof(BNC_data);
			signed short *BNC_pt;
			for (i = 0; i < file_numBNC_chan; i++) {
				BNC_pt = (signed short *)BNC_data;
				_lseeki64(handle,((unsigned _int64)numDataFrames)*file_BNC_ratio*i*2+4,SEEK_SET);
				if (BNC_Length < BNC_block) {
					_read(handle,BNC_pt,BNC_Length);
					for (k = 0; k < BNC_Length/2-1; k++) {
						fprintf(fp,"%ld,",*BNC_pt);
						BNC_pt++;
					}
					fprintf(fp,"%ld\n",*BNC_pt);
				}
				else {
					while (BNC_Length > 0) {
						_read(handle,BNC_pt,BNC_block);
						for (k = 0; k < BNC_block/2; k++) {
							fprintf(fp,"%ld,",*BNC_pt);
							BNC_pt++;
						}
						BNC_Length -= BNC_block;
					}
					BNC_Length += BNC_block;
					_read(handle,BNC_pt,BNC_Length);
					for (k = 0; k < BNC_Length/2-1; k++) {
						fprintf(fp,"%ld,",*BNC_pt);
						BNC_pt++;
					}
					fprintf(fp,"%ld\n",*BNC_pt);
				}
			}
			fclose(fp);
		}
		_close(handle);
	}
}

void subDark()
{
	signed short int *dtpt, *dkpt;
	int i, y, x;

	dtpt = (signed short int *)image_data;
	for (i = 0; i < numTraceFrames; i++) {
		dkpt = (signed short int *)dark_data;
		for (y = 0; y < disp_num_row; y++)
			for (x = 0; x < disp_num_col; x++)
				*dtpt = *dtpt++ - *dkpt++;
	}
}

void addDark()
{
	signed short int *dtpt, *dkpt;
	int i, y, x;

	dtpt = (signed short int *)image_data;
	for (i = 0; i < numTraceFrames; i++) {
		dkpt = (signed short int *)dark_data;
		for (y = 0; y < disp_num_row; y++)
			for (x = 0; x < disp_num_col; x++)
				*dtpt = *dtpt++ + *dkpt++;
	}
}

int SM_saveDark(char *filename, int bin)
{
    int handle;
	char cameraName[256];
	unsigned long darkLength;

	if ((handle = _open(filename,O_BINARY | O_RDONLY)) == -1)
		return false;
	_close(handle);
	if (darkExist) { // && (disp_num_col == cam_num_col) && (disp_num_row == cam_num_row)) {
		setSmDataName(filename);

		int num_col_sv = disp_num_col;
		int num_row_sv = disp_num_row;

		if ((handle = _open(SM_dataname,O_MODE,S_MODE)) == -1)
			return false;
		_lseek(handle,0L,SEEK_SET);
		_read(handle,(char *)FitsHeader,sizeof(FitsHeader));
		SM_readHeader(&numTraceFrames, &disp_num_row, &disp_num_col, cameraName, &SM_exposure);

		if (num_col_sv/bin == disp_num_col && num_row_sv/bin == disp_num_row) {
			darkLength = (unsigned long)num_col_sv*num_row_sv *2;
			alloc_dark_data(num_col_sv, num_row_sv);
			memcpy(dark_data, darkImage, darkLength);
			if (bin > 1) {
				dig_binning(dark_data, num_col_sv, bin);
				darkLength /= bin * bin;
			}
			_lseeki64(handle,2880L + ((unsigned _int64)numTraceFrames)*disp_num_col*disp_num_row*2,SEEK_SET);
			_write(handle,(signed short *)dark_data,darkLength);
		}
		else 
			MessageBox(main_window_handle, "This data file has a different image format from the current dark frame.", "message", MB_OK);
		_close(handle);
	}
	else {
		if (!ndr_lib[curConfig])
			MessageBox(main_window_handle,"No dark frame has been taken in the configuration of the data file. You may take a dark frame and go to File>>Resave Dark Frame", "message", MB_OK);
	}
	return true;
}

typedef struct OM_TIFHEADER {
	char order[2];
	short int version;
	unsigned int offset;
} OMTIFHEADERTYPE, *OMTIFHEADER_PTR;

typedef struct OM_TIFENTRY {
	short int tag,type;
	unsigned int length,value_offset;
} OMTIFENTRYTYPE, *OMTIFENTRY_PTR;

// for BigTiff
typedef struct OM_TIFHEADER2 {
	char order[2];
	short int version, OS_byteSize, add2;
	unsigned _int64 offset;
} OMTIFHEADERTYPE2, *OMTIFHEADER_PTR2;

typedef struct OM_TIFENTRY2 {
	short int tag, type;
	unsigned _int64 length, value_offset;
} OMTIFENTRYTYPE2, *OMTIFENTRY_PTR2;


#define TAGMINVAL						254
#define TAGNEWSUBFILETYPE				254
#define TAGSUBFILETYPE					255
#define TAGIMAGEWIDTH					256
#define TAGIMAGELENGTH					257
#define TAGBITSPERSAMPLE				258
#define TAGCOMPRESSION					259
#define TAGPHOTOMETRICINTERPRETATION	262
#define TAGTHRESHOLDING					263
#define TAGCELLWIDTH					264
#define TAGCELLLENGTH					265
#define TAGFILLORDER					266
#define TAGDOCUMENTNAME					269
#define TAGIMAGEDESCRIPTION				270
#define	TAGMAKE							271
#define TAGMODEL						272
#define TAGSTRIPOFFSETS					273
#define TAGORIENTATION					274
#define TAGSAMPLESPERPIXEL				277
#define TAGROWSPERSTRIP					278
#define TAGSTRIPBYTECOUNTS				279
#define TAGMINSAMPLEVALUE				280
#define TAGMAXSAMPLEVALUE				281
#define TAGXRESOLUTION					282
#define TAGYRESOLUTION					283
#define TAGPLANARCONFIGURATION			284
#define TAGPAGENAME						285
#define TAGXPOSITION					286
#define TAGYPOSITION					287
#define TAGFREEOFFSETS					288
#define TAGFREEBYTECOUNTS				289
#define TAGGRAYRESPONSEUNIT				290
#define TAGGRAYRESPONSECURVE			291
#define TAGGROUP3OPTIONS				292
#define TAGGROUP4OPTIONS				293
#define TAGRESOLUTIONUNIT				296
#define TAGPAGENUMBER					297
#define TAGCOLORRESPONSECURVE			301
#define TAGSOFTWARE						305
#define TAGDATETIME						306
#define TAGARTIST						315
#define TAGHOSTCOMPUTER					316
#define TAGPREDICTOR					317
#define TAGWHITEPOINT					318
#define TAGPRIMARYCHROMATICITIES		319
#define TAGCOLORMAP						320
#define TAGMAXVAL						320

#define TAGTYPEBYTE		 1
#define TAGTYPEASCII	 2
#define TAGTYPESHORT	 3
#define TAGTYPELONG		 4
#define TAGTYPERATIONAL	 5
#define TAGTYPESBYTE	 6
#define TAGTYPEUNDEFINED 7
#define TAGTYPESSHORT	 8
#define TAGTYPESLONG	 9
#define TAGTYPESRATIONAL 10
#define	TAGTYPEFLOAT	 11
#define TAGTYPEDOUBLE	 12
#define TAGTYPES		 12
#define TAGTYPELONG8	 16

void SM_saveTIFF_image(char *tiffName)
{
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;
	int tif_handle;
	char description_str[256];
	OMTIFHEADERTYPE hdr_new;
	OMTIFENTRYTYPE entry;
	short int this_entries;
	int dscrpt_str_len, i, j, entry_len, dscrpt_str_offset;
	unsigned long img_stt, img_byte_size, entry_offset, line_byte_size;
	signed short int *p;

	if ((findhandle = FindFirstFile(tiffName, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(tiffName);
	}

	if ((tif_handle = _open(tiffName,O_BINARY|_O_WRONLY|O_CREAT,S_IWRITE)) == -1) 
		return;
	SM_SetHourCursor();

	if (frame_d_mode == 1 || TiffBit_mode) {
		img_byte_size = disp_num_col * disp_num_row * 3;
		line_byte_size = disp_num_col * 3;
	}
	else {
		img_byte_size = disp_num_col * disp_num_row * 2;
		line_byte_size = 0;
	}

	// for  pseudo color or 24bit tiff
	int bitsPerSampleNumber = 3;
	int this_bitsPerSample = 8;
	int add_len;
	unsigned char *line_buffer = (unsigned char *)_aligned_malloc(line_byte_size, 2);

	memcpy(hdr_new.order, "II", sizeof(hdr_new.order));	//Intel byte order "II", Motorola "MM"
	hdr_new.version = 42;
	entry_offset = sizeof(hdr_new);
	hdr_new.offset = entry_offset;
	this_entries = 10;
	sprintf(description_str, "Turbo-SM Camera=%s Images=%d Exposure=%lfs",cameraType,1,SM_exposure);
	dscrpt_str_len = (int)strlen(description_str) + 1;
	entry_len = (int)(sizeof(entry)*this_entries + sizeof(this_entries) + sizeof(entry_offset));
	if (frame_d_mode == 1 || TiffBit_mode)
		add_len = bitsPerSampleNumber * sizeof(short int);
	else
		add_len = 0;
	dscrpt_str_offset = entry_offset + entry_len + add_len;
	img_stt = dscrpt_str_offset + 256;

	_lseek(tif_handle,0,SEEK_SET);
	_write(tif_handle,(void *)&hdr_new,sizeof(hdr_new));
	_write(tif_handle,(void *)&this_entries,sizeof(this_entries));
	entry.tag = TAGNEWSUBFILETYPE;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = 0;
	_write(tif_handle,(void *)&entry,sizeof(entry));
	entry.tag = TAGIMAGEWIDTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = disp_num_col;
	_write(tif_handle,(void *)&entry,sizeof(entry));
	entry.tag = TAGIMAGELENGTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = disp_num_row;
	_write(tif_handle,(void *)&entry,sizeof(entry));
	if (frame_d_mode == 1 || TiffBit_mode) {	// pseudo color
		entry.tag = TAGBITSPERSAMPLE;	entry.type = TAGTYPESHORT;	entry.length = bitsPerSampleNumber;	entry.value_offset = dscrpt_str_offset - add_len;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGPHOTOMETRICINTERPRETATION;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 2; // color
		_write(tif_handle, (void *)&entry, sizeof(entry));
	}
	else {	// Monochrome
		entry.tag = TAGBITSPERSAMPLE;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 16;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGPHOTOMETRICINTERPRETATION;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 1; // GRAY
		_write(tif_handle, (void *)&entry, sizeof(entry));
	}
	//	entry.tag = TAGBITSPERSAMPLE;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 16;
	_write(tif_handle,(void *)&entry,sizeof(entry));
	entry.tag = TAGPHOTOMETRICINTERPRETATION;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 1; // GRAY
	_write(tif_handle,(void *)&entry,sizeof(entry));
	entry.tag = TAGIMAGEDESCRIPTION;	entry.type = TAGTYPEASCII;	entry.length = dscrpt_str_len;	entry.value_offset = dscrpt_str_offset; 
	_write(tif_handle,(void *)&entry,sizeof(entry));
	entry.tag = TAGSTRIPOFFSETS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = img_stt;
	_write(tif_handle,(void *)&entry,sizeof(entry));
	if (frame_d_mode == 1 || TiffBit_mode) {
		entry.tag = TAGSAMPLESPERPIXEL;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = bitsPerSampleNumber;
	}
	else {
		entry.tag = TAGSAMPLESPERPIXEL;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 1;
	}
	_write(tif_handle,(void *)&entry,sizeof(entry));
	//below is causing trouble for large image
/*	entry.tag = TAGROWSPERSTRIP;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = disp_num_row; 
	_write(tif_handle,(void *)&entry,sizeof(entry));
	entry.tag = TAGSTRIPBYTECOUNTS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = img_byte_size;
	_write(tif_handle,(void *)&entry,sizeof(entry));*/

	if (frame_d_mode == 1 || TiffBit_mode) {
		_lseek(tif_handle, dscrpt_str_offset - add_len, SEEK_SET);
		for (int m = 0; m < bitsPerSampleNumber; ++m)
			_write(tif_handle, (void *)&this_bitsPerSample, sizeof(short int));
	}
	_lseek(tif_handle,dscrpt_str_offset,SEEK_SET);
	_write(tif_handle,(void *)&description_str,dscrpt_str_len);

	_lseeki64(tif_handle, img_stt, SEEK_SET);
	if (frame_d_mode == 1 || TiffBit_mode) {
		double pix_scale = 255.0 / (scale_max - scale_min);
		for (j = 0; j < disp_num_row; j++) {
			for (i = 0; i < disp_num_col; i++) {
				float this_val = min(max(dispImage[j][i], scale_min), scale_max);
				unsigned char val = (unsigned char)((this_val - scale_min)*pix_scale);
				if (frame_d_mode == 1) {
					line_buffer[i * 3] = rainbow_LUT[val] >> 16;
					line_buffer[i * 3 + 1] = rainbow_LUT[val] >> 8;
					line_buffer[i * 3 + 2] = rainbow_LUT[val];
				}
				else {
					line_buffer[i * 3] = (int)val;
					line_buffer[i * 3 + 1] = (int)val;
					line_buffer[i * 3 + 2] = (int)val;
				}
			}
			_write(tif_handle, line_buffer, line_byte_size);
		}
	}
	else {
		p = (signed short int *)single_img_data;
		for (j = 0; j < disp_num_row; j++)
			for (i = 0; i < disp_num_col; i++) {
				if (!darkSubFlag)
					*p++ = (short int)(dispImage[j][i]+0.5);
				else if (!f_subtract_mode)
					*p++ = (short int)(dispImage[j][i]+0.5) + 100;
				else
					*p++ = (short int)(dispImage[j][i]+0.5) + 16384;
			}
		_write(tif_handle, (signed short int *)single_img_data, disp_num_col*disp_num_row * 2);
	}

	_commit(tif_handle);
	_close(tif_handle);
}	

void SM_saveTIFF(char *tiffName)
{
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;
	int handle0, tif_handle;
	char description_str[256];
	OMTIFHEADERTYPE hdr_new;
	OMTIFHEADERTYPE2 hdr_new2;
	OMTIFENTRYTYPE entry;
	OMTIFENTRYTYPE2 entry2;
	short int this_entries;
	unsigned int i, dscrpt_str_len, entry_len, dscrpt_str_offset, numFrame_to_save;
	int m, k;
	unsigned long img_stt0, img_byte_size, entry_offset;
	int len0;
	unsigned long long img_stt;
	signed short int *ptr, *dk_ptr;
	int bigTiff_flag = 0;

	if ((findhandle = FindFirstFile(tiffName, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(tiffName);
	}

	if ((handle0 = _open(SM_dataname,O_BINARY | O_RDONLY)) == -1)
		return;
	numFrame_to_save = save_sm_f_f2 - save_sm_f_f1 + 1;
	img_byte_size = save_sm_f_w*save_sm_f_h*2;
	len0 = disp_num_row * disp_num_col * 2;

	if ((tif_handle = _open(tiffName,O_BINARY|_O_WRONLY|O_CREAT,S_IWRITE)) == -1) 
		return;

	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / numFrame_to_save;
	char str[256];
	sprintf(str, "Saving images: %d of %d", 0, numFrame_to_save);
	SetWindowText(hwndPB, str);

//	if ((((unsigned long long)numFrame_to_save) + 1)*save_sm_f_w*save_sm_f_h * 2 > 4000000000)
//		bigTiff_flag = 1;

	if (bigTiff_flag) {
		memcpy(hdr_new2.order, "II", sizeof(hdr_new.order));	//Intel byte order "II", Motorola "MM"
		hdr_new2.version = 43;
		hdr_new2.OS_byteSize = 8;
		hdr_new2.add2 = 0;
		entry_offset = sizeof(hdr_new2);
		hdr_new2.offset = entry_offset;
		_lseek(tif_handle, 0, SEEK_SET);
		_write(tif_handle, (void *)&hdr_new2, sizeof(hdr_new2));
	}
	else {
		memcpy(hdr_new.order, "II", sizeof(hdr_new.order));	//Intel byte order "II", Motorola "MM"
		hdr_new.version = 42;
		entry_offset = sizeof(hdr_new);
		hdr_new.offset = entry_offset;
		_lseek(tif_handle, 0, SEEK_SET);
		_write(tif_handle, (void *)&hdr_new, sizeof(hdr_new));
	}

	this_entries = 10;
	sprintf(description_str, "Turbo-SM Camera=%s Images=%d Exposure=%lfs", file_cameraName,numFrame_to_save+1,SM_exposure);
	dscrpt_str_len = (int)strlen(description_str) + 1;
	entry_len = sizeof(entry)*this_entries + sizeof(this_entries) + sizeof(entry_offset);
	dscrpt_str_offset = entry_offset + entry_len*(numFrame_to_save+1);
	img_stt0 = dscrpt_str_offset + 256;
	img_stt = img_stt0;

	if (bigTiff_flag) {
		for (i = 0; i < numFrame_to_save + 1; i++) {
			_write(tif_handle, (void *)&this_entries, sizeof(((long long)this_entries)));
			entry2.tag = TAGNEWSUBFILETYPE;	entry2.type = TAGTYPELONG;	entry2.length = 1;	entry2.value_offset = 0;
			_write(tif_handle, (void *)&entry2, sizeof(entry2));
			entry2.tag = TAGIMAGEWIDTH;		entry2.type = TAGTYPELONG;	entry2.length = 1;	entry2.value_offset = save_sm_f_w;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGIMAGELENGTH;		entry2.type = TAGTYPELONG;	entry2.length = 1;	entry2.value_offset = save_sm_f_h;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGBITSPERSAMPLE;	entry2.type = TAGTYPESHORT;	entry2.length = 1;	entry2.value_offset = 16;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGPHOTOMETRICINTERPRETATION;	entry2.type = TAGTYPESHORT;	entry2.length = 1;	entry2.value_offset = 1; // GRAY
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGIMAGEDESCRIPTION;	entry2.type = TAGTYPEASCII;	entry2.length = dscrpt_str_len;	entry2.value_offset = dscrpt_str_offset;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGSTRIPOFFSETS;	entry2.type = TAGTYPELONG8;	entry2.length = 1;	entry2.value_offset = img_stt;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGSAMPLESPERPIXEL;	entry2.type = TAGTYPESHORT;	entry2.length = 1;	entry2.value_offset = 1;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGROWSPERSTRIP;	entry2.type = TAGTYPELONG;	entry2.length = 1;	entry2.value_offset = save_sm_f_h;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry2.tag = TAGSTRIPBYTECOUNTS;	entry2.type = TAGTYPELONG;	entry2.length = 1;	entry2.value_offset = img_byte_size;
			_write(tif_handle, (void *)&entry, sizeof(entry));

			if (i < numFrame_to_save) {
				img_stt += img_byte_size;
				entry_offset += entry_len;
				_write(tif_handle, (void *)&entry_offset, sizeof(entry_offset));
				_lseek(tif_handle, entry_offset, SEEK_SET);
			}
		}
	}
	else {
		for (i = 0; i < numFrame_to_save + 1; i++) {
			_write(tif_handle, (void *)&this_entries, sizeof(this_entries));
			entry.tag = TAGNEWSUBFILETYPE;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = 0;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGIMAGEWIDTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_sm_f_w;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGIMAGELENGTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_sm_f_h;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGBITSPERSAMPLE;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 16;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGPHOTOMETRICINTERPRETATION;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 1; // GRAY
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGIMAGEDESCRIPTION;	entry.type = TAGTYPEASCII;	entry.length = dscrpt_str_len;	entry.value_offset = dscrpt_str_offset;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGSTRIPOFFSETS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = (unsigned long)img_stt;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGSAMPLESPERPIXEL;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 1;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGROWSPERSTRIP;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_sm_f_h;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGSTRIPBYTECOUNTS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = img_byte_size;
			_write(tif_handle, (void *)&entry, sizeof(entry));

			if (i < numFrame_to_save) {
				img_stt += img_byte_size;
				entry_offset += entry_len;
				_write(tif_handle, (void *)&entry_offset, sizeof(entry_offset));
				_lseek(tif_handle, entry_offset, SEEK_SET);
			}
		}
	}
	_lseek(tif_handle,dscrpt_str_offset,SEEK_SET);
	_write(tif_handle,(void *)&description_str,dscrpt_str_len);

	img_stt = img_stt0;
	_lseeki64(handle0, 2880L+save_sm_f_f1* len0, SEEK_SET);
	_lseeki64(tif_handle,img_stt,SEEK_SET);
	for (i = 0; i < numFrame_to_save; i++) {
		_read(handle0,(signed short *)image_data, len0);
		if (output_dksub_flag) {
			ptr = image_data;
			dk_ptr = dark_data;
			for (k = 0; k < len0 >> 1; k++, ptr++, dk_ptr++)
				*ptr = *ptr + 100 - *dk_ptr;
		}
		ptr = image_data + disp_num_col*save_sm_f_y0 + save_sm_f_x0;
		for (m = 0; m < save_sm_f_h; m++) {
			_write(tif_handle,ptr,save_sm_f_w*2);
			ptr += disp_num_col;
		}
		sprintf(str, "Saving images: %d of %d", i, numFrame_to_save);
		SetWindowText(hwndPB, str);
		while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}
	sprintf(str, "Saving dark image");
	SetWindowText(hwndPB, str);
	if (output_dksub_flag)
		memset(image_data, 0, len0);
	else
		memcpy(image_data, dark_data, len0);
	ptr = image_data + disp_num_col*save_sm_f_y0 + save_sm_f_x0;
	for (m = 0; m < save_sm_f_h; m++) {
		_write(tif_handle,ptr,save_sm_f_w*2);
		ptr += disp_num_col;
	}

	sprintf(str, "Finishing writing");
	SetWindowText(hwndPB, str);
	_commit(tif_handle);
	_close(tif_handle);
	_close(handle0);
	DestroyWindow(hwndPB);
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

void SM_saveMovie(char *tiffName, int screenFlag)
{
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;
	int tif_handle;
	char description_str[256];
	OMTIFHEADERTYPE hdr_new;
	OMTIFENTRYTYPE entry;
	short int this_entries;
	int dscrpt_str_len, i, entry_len, add_len, dscrpt_str_offset, numFrame_to_save, m, k;
	unsigned long img_stt, img_stt0, line_byte_size = 0, img_byte_size, entry_offset;
	unsigned int *ptr;

	if ((findhandle = FindFirstFile(tiffName, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(tiffName);
	}

	if (screenFlag) {
		save_tiff_dx = OM[SM_MOVIEPNL].x - OM[SM_MOVIEWIN].x - 10;
		save_tiff_dx = min(save_tiff_dx, (int)(mov_num_col*mov_zoom_factor + 0.5));
		if (!num_traces)
			save_tiff_dy = OM[SM_MOVIEWIN].dy * 2 / 3;
		else
			save_tiff_dy = OM[SM_MOVIEWIN].dy;
	}
	else {
		save_tiff_dx = ov_num_col;
		save_tiff_dy = ov_num_row;
	}

	img_byte_size = (int)(save_tiff_dx * save_tiff_dy * 3);
	line_byte_size = save_tiff_dx * 3;
	numFrame_to_save = numMovFrames;

	if ((tif_handle = _open(tiffName, O_BINARY | _O_WRONLY | O_CREAT, S_IWRITE)) == -1)
		return;

	SM_SetHourCursor();
	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / numFrame_to_save;
	char str[256];
	sprintf(str, "Saving images: %d of %d", 0, numFrame_to_save);
	SetWindowText(hwndPB, str);

	// for  pseudo color
	int bitsPerSampleNumber = 3;
	int this_bitsPerSample = 8;
	unsigned char *line_buffer = (unsigned char *)_aligned_malloc(line_byte_size, 2);

	memcpy(hdr_new.order, "II", sizeof(hdr_new.order));	//Intel byte order "II", Motorola "MM"
	hdr_new.version = 42;
	entry_offset = sizeof(hdr_new);
	hdr_new.offset = entry_offset;
	this_entries = 10;
	sprintf(description_str, "Turbo-SM Camera=%s Images=%d Exposure=%lfs", file_cameraName, numFrame_to_save, SM_exposure);
	dscrpt_str_len = (int)strlen(description_str) + 1;
	entry_len = sizeof(entry)*this_entries + sizeof(this_entries) + sizeof(entry_offset);
	add_len = bitsPerSampleNumber * sizeof(short int);
	dscrpt_str_offset = entry_offset + entry_len * (numFrame_to_save + 1) + add_len;
	img_stt0 = dscrpt_str_offset + 256;
	img_stt = img_stt0;

	_lseek(tif_handle, 0, SEEK_SET);
	_write(tif_handle, (void *)&hdr_new, sizeof(hdr_new));
	for (i = 0; i < numFrame_to_save; i++) {
		_write(tif_handle, (void *)&this_entries, sizeof(this_entries));
		entry.tag = TAGNEWSUBFILETYPE;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = 0;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGIMAGEWIDTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_tiff_dx;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGIMAGELENGTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_tiff_dy;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGBITSPERSAMPLE;	entry.type = TAGTYPESHORT;	entry.length = bitsPerSampleNumber;	entry.value_offset = dscrpt_str_offset - add_len;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGPHOTOMETRICINTERPRETATION;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 2; // color
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGIMAGEDESCRIPTION;	entry.type = TAGTYPEASCII;	entry.length = dscrpt_str_len;	entry.value_offset = dscrpt_str_offset;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGSTRIPOFFSETS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = img_stt;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGSAMPLESPERPIXEL;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = bitsPerSampleNumber;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGROWSPERSTRIP;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_sm_f_h;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		entry.tag = TAGSTRIPBYTECOUNTS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = img_byte_size;
		_write(tif_handle, (void *)&entry, sizeof(entry));
		if (i < numFrame_to_save - 1) {
			img_stt += img_byte_size;
			entry_offset += entry_len;
			_write(tif_handle, (void *)&entry_offset, sizeof(entry_offset));
			_lseek(tif_handle, entry_offset, SEEK_SET);
		}
	}
	_lseek(tif_handle, dscrpt_str_offset - add_len, SEEK_SET);
	for (m = 0; m < bitsPerSampleNumber; ++m)
		_write(tif_handle, (void *)&this_bitsPerSample, sizeof(short int));
	_lseek(tif_handle, dscrpt_str_offset, SEEK_SET);
	_write(tif_handle, (void *)&description_str, dscrpt_str_len);

	int mov_multi_flag_sv, overlay_mode_sv, curMovFrame_sv;
	mov_multi_flag_sv = mov_multi_flag;
	overlay_mode_sv = overlay_mode;
	curMovFrame_sv = curMovFrame;

	mov_multi_flag = 0;
	if (overlay_mode == 2)
		overlay_mode = 1;
	MovieTiffImg = (unsigned int *)_aligned_malloc((unsigned long)save_tiff_dx*save_tiff_dy * sizeof(unsigned int), 2);
	img_stt = img_stt0;
	_lseeki64(tif_handle, img_stt, SEEK_SET);
	for (i = 0; i < numFrame_to_save; i++) {
		curMovFrame = i;
		DisplayMovie(PAINTGRFX, 1 + screenFlag);
		ptr = MovieTiffImg;
		for (m = 0; m < save_tiff_dy; m++) {
			for (k = 0; k < save_tiff_dx; k++) {
				unsigned int this_val = *ptr++;
				line_buffer[k * 3] = this_val >> 16;
				line_buffer[k * 3 + 1] = this_val >> 8;
				line_buffer[k * 3 + 2] = this_val;
			}
			_write(tif_handle, line_buffer, line_byte_size);
		}
		sprintf(str, "Saving images: %d of %d", i, numFrame_to_save);
		SetWindowText(hwndPB, str);
		while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}
	sprintf(str, "Finishing writing");
	SetWindowText(hwndPB, str);
	_aligned_free(line_buffer);
	_commit(tif_handle);
	_close(tif_handle);
	_aligned_free(MovieTiffImg);

	mov_multi_flag = mov_multi_flag_sv;
	overlay_mode = overlay_mode_sv;
	curMovFrame = curMovFrame_sv;
	DestroyWindow(hwndPB);
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

void SM_saveTIFF_movie(char *tiffName)
{
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;
	int tif_handle;
	char description_str[256];
	OMTIFHEADERTYPE hdr_new;
	OMTIFENTRYTYPE entry;
	short int this_entries;
	int dscrpt_str_len, i, entry_len, add_len, dscrpt_str_offset, numFrame_to_save, m, k, curFrame_sv, ref1_sv, ref2_sv;
	unsigned long img_stt, img_stt0, line_byte_size = 0, img_byte_size, entry_offset;
	signed short int *ptr;

//	printf("\nSM_saveTIFF_movie %d %d %d %d\n", firstDataFrame, numTraceFrames, save_sm_f_f1, save_sm_f_f2);
	if ((save_sm_f_f1 < firstDataFrame) || (save_sm_f_f2 >= numTraceFrames+firstDataFrame)) {
		MessageBox(main_window_handle, "The range defined is greater than the current movie range. Only the movie range will be saved.", "message", MB_OK);
		save_sm_f_f1 = max(firstDataFrame, save_sm_f_f1);
		save_sm_f_f2 = min(numTraceFrames+firstDataFrame - 1, save_sm_f_f2);
	}

	if ((findhandle = FindFirstFile(tiffName, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(tiffName);
	}

	numFrame_to_save = save_sm_f_f2 - save_sm_f_f1 + 1;
	if (NDR_subtraction_flag && NDR_cCDS_flag)
		numFrame_to_save /= NDR_subtraction_frames;
	if (frame_d_mode == 1) {
		img_byte_size = save_sm_f_w * save_sm_f_h * 3;
		line_byte_size = save_sm_f_w * 3;
	}
	else
		img_byte_size = save_sm_f_w * save_sm_f_h * 2;

	if ((tif_handle = _open(tiffName, O_BINARY | _O_WRONLY | O_CREAT, S_IWRITE)) == -1)
		return;

	SM_SetHourCursor();
	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / numFrame_to_save;
	char str[256];
	sprintf(str, "Saving images: %d of %d", 0, numFrame_to_save);
	SetWindowText(hwndPB, str);

	// for  pseudo color
	int bitsPerSampleNumber = 3;
	int this_bitsPerSample = 8;
	unsigned char *line_buffer = (unsigned char *)_aligned_malloc(line_byte_size, 2);

	memcpy(hdr_new.order, "II", sizeof(hdr_new.order));	//Intel byte order "II", Motorola "MM"
	hdr_new.version = 42;
	entry_offset = sizeof(hdr_new);
	hdr_new.offset = entry_offset;
	this_entries = 10;
	sprintf(description_str, "Turbo-SM Camera=%s Images=%d Exposure=%lfs", file_cameraName,numFrame_to_save,SM_exposure);
	dscrpt_str_len = (int)strlen(description_str) + 1;
	entry_len = sizeof(entry)*this_entries + sizeof(this_entries) + sizeof(entry_offset);
	if (frame_d_mode == 1)
		add_len = bitsPerSampleNumber * sizeof(short int);
	else
		add_len = 0;
	dscrpt_str_offset = entry_offset + entry_len*(numFrame_to_save+1) + add_len;
	img_stt0 = dscrpt_str_offset + 256;
	img_stt = img_stt0;


	_lseek(tif_handle,0,SEEK_SET);
	_write(tif_handle,(void *)&hdr_new,sizeof(hdr_new));
	for (i = 0; i < numFrame_to_save; i++) {
		_write(tif_handle,(void *)&this_entries,sizeof(this_entries));
		entry.tag = TAGNEWSUBFILETYPE;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = 0;
		_write(tif_handle,(void *)&entry,sizeof(entry));
		entry.tag = TAGIMAGEWIDTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_sm_f_w;
		_write(tif_handle,(void *)&entry,sizeof(entry));
		entry.tag = TAGIMAGELENGTH;		entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_sm_f_h;
		_write(tif_handle,(void *)&entry,sizeof(entry));
		if (frame_d_mode == 1) {	// pseudo color
			entry.tag = TAGBITSPERSAMPLE;	entry.type = TAGTYPESHORT;	entry.length = bitsPerSampleNumber;	entry.value_offset = dscrpt_str_offset - add_len;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGPHOTOMETRICINTERPRETATION;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 2; // color
			_write(tif_handle, (void *)&entry, sizeof(entry));
		}
		else {	// Monochrome
			entry.tag = TAGBITSPERSAMPLE;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 16;
			_write(tif_handle, (void *)&entry, sizeof(entry));
			entry.tag = TAGPHOTOMETRICINTERPRETATION;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 1; // GRAY
			_write(tif_handle, (void *)&entry, sizeof(entry));
		}
		entry.tag = TAGIMAGEDESCRIPTION;	entry.type = TAGTYPEASCII;	entry.length = dscrpt_str_len;	entry.value_offset = dscrpt_str_offset; 
		_write(tif_handle,(void *)&entry,sizeof(entry));
		entry.tag = TAGSTRIPOFFSETS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = img_stt;
		_write(tif_handle,(void *)&entry,sizeof(entry));
		if (frame_d_mode == 1) {
			entry.tag = TAGSAMPLESPERPIXEL;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = bitsPerSampleNumber;
		}
		else {
			entry.tag = TAGSAMPLESPERPIXEL;	entry.type = TAGTYPESHORT;	entry.length = 1;	entry.value_offset = 1;
		}
		_write(tif_handle,(void *)&entry,sizeof(entry));
		entry.tag = TAGROWSPERSTRIP;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = save_sm_f_h; 
		_write(tif_handle,(void *)&entry,sizeof(entry));
		entry.tag = TAGSTRIPBYTECOUNTS;	entry.type = TAGTYPELONG;	entry.length = 1;	entry.value_offset = img_byte_size;
		_write(tif_handle,(void *)&entry,sizeof(entry));
		if (i < numFrame_to_save-1) {
			img_stt += img_byte_size;
			entry_offset += entry_len;
			_write(tif_handle,(void *)&entry_offset,sizeof(entry_offset));
			_lseek(tif_handle,entry_offset,SEEK_SET);
		}
	}
	if (frame_d_mode == 1) {
		_lseek(tif_handle, dscrpt_str_offset - add_len, SEEK_SET);
		for (m = 0; m < bitsPerSampleNumber; ++m)
			_write(tif_handle, (void *)&this_bitsPerSample, sizeof(short int));
	}
	_lseek(tif_handle,dscrpt_str_offset,SEEK_SET);
	_write(tif_handle,(void *)&description_str,dscrpt_str_len);

	if (frame_d_mode == 1)
		DoNotPaint = 1;
	curFrame_sv = curDispFrame;
	ref1_sv = ref_frame1;
	ref2_sv = ref_frame2;
	curDispFrame = save_sm_f_f1 - firstDataFrame; // -1;
	img_stt = img_stt0;
	_lseeki64(tif_handle,img_stt,SEEK_SET);
	movie_rep_cnt = 0;
	double pix_scale = 255.0 / (scale_max - scale_min);
	for (i = 0; i < numFrame_to_save; i++) {
		getStreamFrame();
		ptr = (signed short int *)single_img_data;
		for (m = save_sm_f_y0; m < save_sm_f_y0+save_sm_f_h; m++) {
			for (k = save_sm_f_x0; k < save_sm_f_x0 + save_sm_f_w; k++) {
				int this_val = (int)(dispImage[m][k]+0.5);
				if (frame_d_mode == 1) {
					this_val = min(max((int)(dispImage[m][k]+0.5), scale_min), scale_max);
					unsigned char val = (unsigned char)((this_val - scale_min)*pix_scale);
					line_buffer[k * 3] = rainbow_LUT[val] >> 16;
					line_buffer[k * 3 + 1] = rainbow_LUT[val] >> 8;
					line_buffer[k * 3 + 2] = rainbow_LUT[val];
				}
				else {
					if (darkSubFlag) {
						if (!f_subtract_mode)
							this_val += 100;
						else
							this_val += 16384;
					}
					*ptr++ = this_val;
				}
			}
			if (frame_d_mode == 1)
				_write(tif_handle, line_buffer, line_byte_size);
		}
		if (frame_d_mode != 1)
			_write(tif_handle,single_img_data,img_byte_size);
		if ((movie_play_mode == 1) && movie_step)
			curDispFrame += movie_step;
		else
			curDispFrame++;
		sprintf(str, "Saving images: %d of %d", i, numFrame_to_save);
		SetWindowText(hwndPB, str);
		while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}
	sprintf(str, "Finishing writing");
	SetWindowText(hwndPB, str);
	_aligned_free(line_buffer);
	_commit(tif_handle);
	_close(tif_handle);
	DestroyWindow(hwndPB);
	DoNotPaint = 0;
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
	curDispFrame = curFrame_sv;
	ref_frame1 = ref1_sv;
	ref_frame2 = ref2_sv;
}	

short int tagList[] = { TAGNEWSUBFILETYPE,TAGSUBFILETYPE,TAGIMAGEWIDTH,TAGIMAGELENGTH,TAGBITSPERSAMPLE,TAGCOMPRESSION,TAGPHOTOMETRICINTERPRETATION,
						TAGTHRESHOLDING,TAGCELLWIDTH,TAGCELLLENGTH,TAGFILLORDER,TAGDOCUMENTNAME,TAGIMAGEDESCRIPTION,TAGMAKE,TAGMODEL,TAGSTRIPOFFSETS,
						TAGORIENTATION,TAGSAMPLESPERPIXEL,TAGROWSPERSTRIP,TAGSTRIPBYTECOUNTS,TAGMINSAMPLEVALUE,TAGMAXSAMPLEVALUE,TAGXRESOLUTION,
						TAGYRESOLUTION,TAGPLANARCONFIGURATION,TAGPAGENAME,TAGXPOSITION,TAGFREEOFFSETS,TAGFREEBYTECOUNTS,TAGGRAYRESPONSEUNIT,
						TAGGRAYRESPONSECURVE,TAGGROUP3OPTIONS,TAGGROUP4OPTIONS,TAGRESOLUTIONUNIT,TAGPAGENUMBER,TAGCOLORRESPONSECURVE,TAGSOFTWARE,
						TAGDATETIME,TAGARTIST,TAGHOSTCOMPUTER,TAGPREDICTOR,TAGWHITEPOINT,TAGPRIMARYCHROMATICITIES,TAGCOLORMAP };

char  *tagListStr[] = { "NEW SUBFILE TYPE","SUBFILE TYPE","IMAGE WIDTH","IMAGE LENGTH","BITS PER SAMPLE","COMPRESSION","PHOTOMETRIC INTERPRETATION",
						"THRESHOLDING","CELL WIDTH","CELL LENGTH","FILL ORDER","DOCUMENT NAME","IMAGE DESCRIPTION","MAKE","MODEL","STRIP OFFSETS",
						"ORIENTATION","SAMPLES PER PIXEL","ROWS PER STRIP","STRIP BYTE COUNTS","MIN SAMPLE VALUE","MAX SAMPLE VALUE","X RESOLUTION",
						"Y RESOLUTION","PLANAR CONFIGURATION","PAGE NAME","X POSITION","FREE OFFSETS","FREE BYTE COUNTS","GRAY RESPONSE UNIT",
						"GRAY RESPONSE CURVE","GROUP 3 OPTIONS","GROUP 4 OPTIONS","RESOLUTION UNIT","PAGE NUMBER","COLOR RESPONSE CURVE","SOFTWARE",
						"DATE TIME","ARTIST","HOST COMPUTER","PREDICTOR","WHITE POINT","PRIMARY CHROMATICITIES","COLOR MAP" };

short int tags[] = {	TAGNEWSUBFILETYPE,
						TAGIMAGEWIDTH,
						TAGIMAGELENGTH,
						TAGBITSPERSAMPLE,
						TAGCOMPRESSION,
						TAGPHOTOMETRICINTERPRETATION,
						TAGSTRIPOFFSETS,
						TAGSAMPLESPERPIXEL,
						TAGROWSPERSTRIP,
						TAGSTRIPBYTECOUNTS,
						TAGXRESOLUTION,
						TAGYRESOLUTION,
						TAGRESOLUTIONUNIT };

#define MAXTAGS			(sizeof(tags)/sizeof(short int))
#define ALLTAGS			(sizeof(tagList)/sizeof(short int))

OMTIFENTRYTYPE tag[MAXTAGS];

#define TIFFILEOPENERROR		-1
#define	TIFHEADERERROR			-2
#define	TIFINTELERROR			-3
#define TIFDIRECTORYERROR		-4
#define TIFENTRYERROR			-5
#define TIFCOMPRESSIONERROR		-6
#define TIFPHOTOINTERPERROR		-7
#define TIFBITSPERPIXELERROR1	-8
#define TIFBITSPERPIXELERROR2	-9
#define TIFSAMPLESPERPIXELERROR	-10
#define TIFNONMONOCHROME		-11

char *tifErrorList[] = {	"Error opening TIF file",
							"Error reading TIF file header",
							"Error reading Motorola TIF file format. Use Intel format.",
							"Error reading TIF file directory",
							"Error reading TIF file directory entries",
							"Error - Cannot read compressed TIF files!",
							"Error - Can only read Gray or RGB TIF files!",
							"Error - Number of bits per pixel entries in TIF file must be 1 or 3",
							"Error - Number of bits per pixel in TIF file must be the same for each color",
							"Error - Number of samples per pixel in TIF file must be 1 or 3",
							"Error - This software handles open monochrome images" };

void ShortSwap(unsigned char *ptr)
{
	unsigned char c;

	c = *ptr;
	*ptr = *(ptr+1);
	*ptr = c;
}

void LongSwap(unsigned char *ptr)
{
	unsigned char c;

	c = *ptr;
	*ptr = *(ptr+3);
	*ptr = c;
	++ptr;
	c = *ptr;
	*ptr = *(ptr+1);
	*ptr = c;
}

unsigned short int *tiff_data = NULL;
int SM_readTIFF_stack(char *tiffName, int for_overlay)
{
	int samplesPerPixel = 0,bitsPerSampleNumber = 0,bitsPerSampleOffset = 0,bitsPerSample[3] = { 0, 0, 0 },stripNumber = 0,
	stripOffset = 0,stripOffsets = 0,stripOffsetValue = 0, stripByteCountValue = 0,rowsPerStrip = 0,
	compression = -1,color = 0,rgb = 0,stripByteCount = 0,stripByteCounts = 0,stripByteCountOffset = 0,stripByteCountOffsets = 0,
	stripByteCountOffsetValues[3] = { 0,0,0 },planarConfiguration = 1, tif_ResUnit = 0;
	long tif_xRes = 0, tif_yRes = 0;

	int invert,error = 0, handle, i, j, img_stt, img_len;
//	char description_str[256];
	OMTIFHEADERTYPE hdr;
	OMTIFENTRYTYPE entry;
	short int entries;
	long offset;
	signed short int *ptr;
	unsigned short int *u_ptr;

	setTabOn(OM_MEASUREVIEW);

    if ((handle = _open(tiffName,O_BINARY|O_RDONLY,S_IREAD)) == -1)
         return FALSE;
	if ((_read(handle,(void *)&hdr,sizeof(hdr)) == sizeof(hdr)) && (hdr.version == 0x002a || hdr.version == 0x2a00)) {
		invert = hdr.order[0] == 'M' && hdr.order[1] == 'M';
		if (invert) {
			error = TIFINTELERROR;
			goto end;
		}
		if (invert)
			LongSwap((unsigned char *)&hdr.offset);
		if ((offset = _lseek(handle,(long)hdr.offset,SEEK_SET)) != offset || (_read(handle,(void *)&entries,sizeof(entries)) != sizeof(entries))) {
			error = TIFDIRECTORYERROR;
			goto end;
		}
		if (invert)
			ShortSwap((unsigned char *)&entries);
		if (invert) {
			return FALSE;
		}
		for (i = 0; i < entries; ++i) {
			if (_read(handle,(void *)&entry,sizeof(entry)) == sizeof(entry)) {
				if ((entry.tag > TAGMAXVAL) || (entry.tag < TAGMINVAL))
					continue;
				//	break;
				for (j = 0; j < ALLTAGS; ++j) {
					if (tagList[j] == entry.tag)
						break;
					else {
						switch (entry.tag) {
						case TAGIMAGEWIDTH:			if (for_overlay)
														ov_num_col = entry.value_offset;
													else
														disp_num_col = entry.value_offset;
							break;
						case TAGIMAGELENGTH:		if (for_overlay)
														ov_num_row = entry.value_offset;
													else
														disp_num_row = entry.value_offset;
													break;
						case TAGCOMPRESSION:		if (entry.value_offset == 1)
														compression = FALSE;
													else {
														compression = TRUE;
														error = TIFCOMPRESSIONERROR;
														goto end;
													}
													break;
						case TAGPHOTOMETRICINTERPRETATION:
													if (entry.value_offset == 0 || entry.value_offset == 1)
														color = FALSE;
													else if (entry.value_offset == 2)
														color = TRUE;
													else {
														color = FALSE;
														error = TIFPHOTOINTERPERROR;
														goto end;
													}
													break;
						case TAGSTRIPOFFSETS:		if ((stripNumber = entry.length) == 1)
														stripOffset = entry.value_offset;
													else
														stripOffsets = entry.value_offset;		
													break;
						case TAGBITSPERSAMPLE:		if ((bitsPerSampleNumber = entry.length) == 1)
														bitsPerSample[0] = entry.value_offset;
													else if (bitsPerSampleNumber != 3) {
														error = TIFBITSPERPIXELERROR1;
														goto end;
													}
													else
														bitsPerSampleOffset = entry.value_offset;		
													break;
						case TAGSAMPLESPERPIXEL:	if ((samplesPerPixel = entry.value_offset) == 1)
														rgb = FALSE;	
													else if (samplesPerPixel == 3)
														rgb = TRUE;
													else {
														rgb = FALSE;	
														error = TIFSAMPLESPERPIXELERROR;
														goto end;
													}
													break;
						case TAGROWSPERSTRIP:		rowsPerStrip = entry.value_offset;
													break;
						case TAGSTRIPBYTECOUNTS:	if ((stripByteCounts = entry.length) == 1)
														stripByteCountOffset = entry.value_offset;
													else
														stripByteCountOffsets = entry.value_offset;
													break;
						case TAGPLANARCONFIGURATION:planarConfiguration = entry.value_offset;
													break;
						case TAGXRESOLUTION:		tif_xRes = entry.value_offset;
													break;
						case TAGYRESOLUTION:		tif_yRes = entry.value_offset;
													break;
						case TAGRESOLUTIONUNIT:		tif_ResUnit = entry.value_offset;
													break;
						}
					}
				}
			}
			else {
				error = TIFENTRYERROR;
				goto end;
			}
		}
		if (bitsPerSampleNumber == 3) {
			error = TIFNONMONOCHROME;
			goto end;
		}

		img_stt = stripOffset;

		if (!for_overlay) {
			file_num_col = disp_num_col;
			file_num_row = disp_num_row;
		}

		int num_fs = 0, next_entry;
		if (_read(handle,(void *)&next_entry,sizeof(next_entry)) == sizeof(next_entry)) {
			while (next_entry) {
				num_fs++;
				if (num_fs >= 10000)
					break;

				_lseeki64(handle,next_entry,SEEK_SET);
				_read(handle,(void *)&entries,sizeof(entries));
				for (i = 0; i < entries; ++i)
					_read(handle,(void *)&entry,sizeof(entry));
				_read(handle,(void *)&next_entry,sizeof(next_entry));
			}
		}
		if (for_overlay) {
			numOverlayFrames = num_fs + 1;
		}
		else {
			numDataFrames = num_fs + 1;
		//	numTraceFrames = numDataFrames;

			dataReadBlock = (int)(min(1000000000L / (disp_num_col*disp_num_row), MAX_DATA_FRAMES));
			if (dataReadBlock > MAX_DATA_FRAMES / 2)
				dataReadBlock = (dataReadBlock / 1000) * 1000;
			else if (dataReadBlock > MAX_DATA_FRAMES / 20)
				dataReadBlock = (dataReadBlock / 100) * 100;
			else
				dataReadBlock = (dataReadBlock / 50) * 50;

			if (!dataReadBlock)
				dataReadBlock = 10;

			firstDataFrame = 0;
			numTraceFrames = min(numDataFrames - firstDataFrame, dataReadBlock);
		}
	}
	else {
		error = TIFHEADERERROR;
		goto end;
	}

	if (bitsPerSample[0] == 32)
		float_flag = 1;
	else
		float_flag = 0;

	SM_SetHourCursor();
	if (for_overlay) {
		if (overlay_img != NULL)
			_aligned_free(overlay_img);
		overlay_img = (signed short int *)_aligned_malloc((unsigned long)ov_num_col*ov_num_row*numOverlayFrames * sizeof(signed short int), 2);
		ptr = overlay_img;
		img_len = ov_num_col * ov_num_row * 2;
		_lseeki64(handle, img_stt, SEEK_SET);
		for (i = 0; i < numOverlayFrames; ++i) {
			_read(handle, (void *)ptr, img_len);
			ptr += img_len >> 1;
		}

		ovFile_num_col = ov_num_col;
		ovFile_num_row = ov_num_row;
		if (movie_data != NULL)
			overlay_mode = 1;
		else
			overlay_mode = 2;
	}
	else {
		if (numDataFrames <= 1) {
			MessageBox(main_window_handle, "Turbo-SM does not display single frame tiff file.", "message", MB_OK);
			return FALSE;
		}

		if (SM_exposure <= 0.0)
			SM_exposure = 0.0005; //for filter and time scale
		getTiffExpTime();

		if (image_data != NULL)
			_aligned_free(image_data);
		image_data = (signed short int *)_aligned_malloc(numTraceFrames*disp_num_col*disp_num_row * sizeof(signed short int), 2);
		if (float_flag) {
			if (image_data_f != NULL)
				_aligned_free(image_data_f);
			image_data_f = (float32 *)_aligned_malloc(numTraceFrames*disp_num_col*disp_num_row * sizeof(float32), 2);
			float32 *u_ptr_f = image_data_f;
			img_len = disp_num_col * disp_num_row * sizeof(float32);
			_lseeki64(handle, img_stt, SEEK_SET);
			for (i = 0; i < numTraceFrames; ++i) {
				_read(handle, (void *)u_ptr_f, img_len);
				u_ptr_f += disp_num_col * disp_num_row;
			}
			u_ptr_f = image_data_f;
			ptr = image_data;
			for (i = 0; i < numTraceFrames*disp_num_col * disp_num_row; ++i)
				*ptr++ = (signed short int)((*u_ptr_f++)*10+1000);
		}
		else {
			tiff_data = (unsigned short int *)_aligned_malloc(numTraceFrames*disp_num_col*disp_num_row * sizeof(unsigned short int), 2);
			u_ptr = tiff_data;
			img_len = disp_num_col * disp_num_row * 2;
			_lseeki64(handle, img_stt, SEEK_SET);
			for (i = 0; i < numTraceFrames; ++i) {
				_read(handle, (void *)u_ptr, img_len);
				u_ptr += img_len >> 1;
			}
			int tif_scale_flag = 0;
			u_ptr = tiff_data;
			for (i = 0; i < numTraceFrames*disp_num_col * disp_num_row; ++i) {
				if (*u_ptr++ > 20000) {
					tif_scale_flag = 1;
					break;
				}
			}
			if (tif_scale_flag) {
				u_ptr = tiff_data;
				ptr = image_data;
				for (i = 0; i < numTraceFrames*disp_num_col * disp_num_row; ++i)
					*ptr++ = (signed short int)((*u_ptr++) >> 1);
			}
			else
				memcpy(image_data, tiff_data, img_len*numTraceFrames);
		}

		alloc_dark_data(disp_num_col, disp_num_row);

		if (single_img_size < (unsigned long)disp_num_col*disp_num_row) {
			if (single_img_data != NULL)
				_aligned_free(single_img_data);
			single_img_size = (unsigned long)disp_num_col*disp_num_row;
			single_img_data = (signed short int *)_aligned_malloc(single_img_size * sizeof(signed short int), 2);
			if (single_img_float != NULL)
				_aligned_free(single_img_float);
			single_img_float = (float *)_aligned_malloc(single_img_size * sizeof(float), 2);
		}

		if (tiff_data != NULL)
			_aligned_free(tiff_data);

		setSmDataName(tiffName);
		//Get file time
		struct stat attrib;
		stat(tiffName, &attrib);
		fileTime = gmtime(&(attrib.st_mtime));

		SetNewFileRead(0);
	}

end:
	_close(handle);
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
	if (error) {
		MessageBox(main_window_handle, tifErrorList[-error-1], "message", MB_OK);
		return FALSE;
	}
	return TRUE;
}	

void save_fits_image(char *fitsName)
{
    int handle;
	int i, j;
 	unsigned long dataLength,dataLeft;
	signed short int *ptr, *sptr;
	unsigned char *p, *q, v;
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;

	SM_SetHourCursor();

	if ((findhandle = FindFirstFile(fitsName, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(fitsName);
	}

	SM_makeHeader(1,disp_num_row,disp_num_col,cameraType,(double)SM_exposure);
	dataLength = (unsigned long)disp_num_row*disp_num_col*2;
	dataLeft = sizeof(FitsHeader) - (dataLength)%sizeof(FitsHeader);

	if ((handle = _open(fitsName,O_BINARY|O_CREAT|O_WRONLY,S_IWRITE)) == -1)
		return;
	_write(handle,(char *)FitsHeader,sizeof(FitsHeader));

	ptr = (signed short *)single_img_data;
	sptr = (signed short *)dispImage;
	for (j = 0; j < disp_num_row; j++) {
		memcpy(ptr, sptr, disp_num_col*2);
		p = (unsigned char *)ptr;
		i = disp_num_col*2;
		q = p + 1;
		while (i-- > 0) {
			v = *p;
			*p = *q;
			*q = v;
			p+=2;
			q+=2;
		}
		ptr += disp_num_col;
		sptr += IMAGER_SIZE*2;
	}

	_write(handle,single_img_data,dataLength);
	_write(handle,(char *)FitsEnder,dataLeft);
	_commit(handle);
	_close(handle);

	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

void SM_saveData(char *filename)
{
	int handle;
	unsigned long dataLength, dataLeft, darkLength;
	signed short int *ptr;

	SM_SetHourCursor();

	SM_makeHeader(numTraceFrames, disp_num_row, disp_num_col, cameraType, (double)frame_interval / 1000.0);
	dataLength = (unsigned long)numTraceFrames*disp_num_row*disp_num_col * 2;
	darkLength = (unsigned long)disp_num_row*disp_num_col * 2;
	dataLeft = sizeof(FitsHeader) - (dataLength + darkLength) % sizeof(FitsHeader);

	if ((handle = _open(filename, O_BINARY | O_CREAT | O_WRONLY, S_IWRITE)) == -1)
		return;
	_write(handle, (char *)FitsHeader, sizeof(FitsHeader));
	if (numTraceFrames) {
		ptr = (signed short *)image_data;
		_write(handle, ptr, dataLength);
	}
	alloc_dark_data(disp_num_col, disp_num_row);
	d_darkExist = TRUE;
	if (darkExist)
		memcpy(dark_data, darkImage, darkLength);
	_write(handle, (signed short *)dark_data, darkLength);
	_write(handle, (char *)FitsEnder, dataLeft);
	_commit(handle);
	_close(handle);

	getAcq_FileName(-1);
	repaintMenubox();
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

extern int lut_size;
extern unsigned int *lut;
static unsigned short int *image_buffer = (unsigned short int *)NULL;

void convertTmpDiskFile(char *tmpD_name, char *newFileName)
{
	int i, j, k, stt_byte, disk_chunk_size, numChannels;
	int read_len, images, loops, horizontalBin, superFrame = 1, file_img_w, binFlag, cdsFlag, segmented, image_stripes, image_layers;
	unsigned int file_length_sv, image_width, image_height, image_length, file_width, file_height, file_length;
	u_char *data_pt;
	int diskF_handle[4], tsmhandle;
	char disk_tmp_file[MAXCHANNELS][_MAX_FNAME] = { "", "", "","" };
	char str[256];
	char *p;
	unsigned long dataLeft;

	strcpy(str, tmpD_name);
	if (p = strstr(str, "file_ch"))
		*(p + 7) = '\0';
	else 
		return;

	for (i = 0; i < 4; i++)
		sprintf(disk_tmp_file[i], "%s%d.dat", str, i+1);
	if ((diskF_handle[0] = _open(disk_tmp_file[0], O_BINARY | O_RDONLY)) == -1) {
		MessageBox(main_window_handle, "Can't open the *.dat file.", "message", MB_OK);
		return;
	}
	int this_offset = 10 * sizeof(int) + 5 * sizeof(unsigned int);
	_lseeki64(diskF_handle[0], -this_offset, SEEK_END);
	_read(diskF_handle[0], &images, sizeof(int));
	_read(diskF_handle[0], &image_width, sizeof(unsigned int));
	_read(diskF_handle[0], &image_height, sizeof(unsigned int));
	_read(diskF_handle[0], &image_length, sizeof(unsigned int));
	_read(diskF_handle[0], &file_width, sizeof(unsigned int));
	_read(diskF_handle[0], &file_height, sizeof(unsigned int));
	_read(diskF_handle[0], &image_stripes, sizeof(int));
	_read(diskF_handle[0], &image_layers, sizeof(int));
	_read(diskF_handle[0], &stt_byte, sizeof(int));
	_read(diskF_handle[0], &cdsFlag, sizeof(int));
	_read(diskF_handle[0], &numChannels, sizeof(int));
	_read(diskF_handle[0], &horizontalBin, sizeof(int));
	_read(diskF_handle[0], &file_img_w, sizeof(int));
	_read(diskF_handle[0], &binFlag, sizeof(int));
	_read(diskF_handle[0], &segmented, sizeof(int));
	_close(diskF_handle[0]);

	file_length = file_width * file_height * 2;
	loops = (images + cdsFlag);
	disk_chunk_size = 1024 * 1000 / file_height;
	disk_chunk_size = min(disk_chunk_size, loops);
	read_len = image_length / numChannels;
//	lut_sv = (unsigned int *)_aligned_malloc(lut_size, 2);
	lut = (unsigned int *)_aligned_malloc(file_length * sizeof(lut[0]), 2);
	image_buffer = (unsigned short *)_aligned_malloc(file_length, 2);
	if (image_data != NULL)
		_aligned_free(image_data);
	image_data = (signed short *)_aligned_malloc(image_length*(disk_chunk_size + 1), 2);
	makeLookUpTable(lut, image_width, image_height, file_width, file_height, image_stripes, image_layers, superFrame, false, cdsFlag, 0, 1);

	HWND hwndPB = createProgBar();
	SendMessage(hwndPB, PBM_SETPOS, (WPARAM)0, 0);
	int step_cnt = 0;
	double step_size = ((double)num_steps) / loops;
	long this_img;

	tsmhandle = _open(newFileName, O_MODE, S_MODE);

	if (((file_img_w != file_width) || (horizontalBin > 1)))
		SM_makeHeader(images, file_height, min(file_img_w, (signed int)file_width) / horizontalBin, cameraType, (double)frame_interval / 1000.0);
	else
		SM_makeHeader(images,file_height, file_width, cameraType, (double)frame_interval / 1000.0);
	unsigned long long fileDataLength = (unsigned long long)(images + 1)*(unsigned long long)file_length*superFrame;
	dataLeft = sizeof(FitsHeader) - (fileDataLength) % sizeof(FitsHeader);
	memset(&FitsEnder, 0, min(sizeof(FitsEnder), dataLeft));
	_write(tsmhandle, (char *)FitsHeader, sizeof(FitsHeader));

	signed short *last_frame = (signed short *)_aligned_malloc(image_length, 2);
	data_pt = (u_char *)image_data;
	for (i = 0; i < numChannels; i++) {
		diskF_handle[i] = _open(disk_tmp_file[i], O_BINARY | O_RDONLY);
		_lseeki64(diskF_handle[i], stt_byte, SEEK_SET);
		if (cdsFlag) {
			_read(diskF_handle[i], (u_char *)data_pt, read_len);
			data_pt += image_length / numChannels;
		}
	}
	this_img = 1;
	file_length_sv = file_length;
	for (j = 0; j < loops / disk_chunk_size; ++j) {
		for (k = 0; k < disk_chunk_size; k++) {
			for (i = 0; i < numChannels; i++) {
				_read(diskF_handle[i], (u_char *)data_pt, read_len);
				data_pt += image_length / numChannels;
			}
		}
		if (cdsFlag) {
			memcpy(last_frame, (data_pt - image_length), image_length);
			subtractCDS((unsigned short *)image_data, disk_chunk_size + 1, image_width, image_height, image_length, 1, false, true, numChannels);
		}
		data_pt = (u_char *)image_data;
		for (k = 0; k < disk_chunk_size; k++) {
			file_length = file_length_sv;
			frame_deInterleave(file_length >> 1, lut, (unsigned short *)data_pt, image_buffer);
			frameRepair(image_buffer, 0, true, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
			file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
			_write(tsmhandle, (void *)image_buffer, file_length);
			data_pt += image_length;

			sprintf(str, "Deinterleaving %d of %d images", this_img, images);
			SetWindowText(hwndPB, str);
			while ((int)(this_img*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
			this_img++;
		}

		if (cdsFlag) {
			data_pt = (u_char *)image_data;
			memcpy(data_pt, last_frame, image_length);
		}
		data_pt += image_length;
	}
	if (loops % disk_chunk_size) {
		for (k = 0; k < (loops % disk_chunk_size); k++) {
			for (i = 0; i < numChannels; i++) {
				_read(diskF_handle[i], (u_char *)data_pt, read_len);
				data_pt += image_length / numChannels;
			}
		}
		if (cdsFlag) {
			memcpy(last_frame, (data_pt - image_length), image_length);
			subtractCDS((unsigned short *)image_data, disk_chunk_size + 1, image_width, image_height, image_length, 1, false, true, numChannels);
		}
		data_pt = (u_char *)image_data;
		for (k = 0; k < (loops % disk_chunk_size); k++) {
			file_length = file_length_sv;
			frame_deInterleave(superFrame*file_length >> 1, lut, (unsigned short *)data_pt, image_buffer);
			frameRepair(image_buffer, 0, true, image_width, file_width, file_height, horizontalBin, superFrame, file_img_w, binFlag, segmented);
			file_length = file_height * min(file_img_w, (signed int)file_width) * 2 / horizontalBin;
			_write(tsmhandle, (void *)image_buffer, file_length);
			data_pt += image_length;

			sprintf(str, "Deinterleaving %d of %d images", this_img, images);
			SetWindowText(hwndPB, str);
			while ((int)(this_img*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
			this_img++;
		}
	}
	memset(data_pt, 0, file_length);
	_lseeki64(tsmhandle, 2880L + (unsigned long long)images*(unsigned long long)file_length, SEEK_SET);
	_write(tsmhandle, (void *)data_pt, file_length);
	_write(tsmhandle, (char *)FitsEnder, dataLeft);
	_close(tsmhandle);
	for (i = 0; i < numChannels; i++) {
		_close(diskF_handle[i]);
		DeleteFile(disk_tmp_file[i]);
	}
//	_aligned_free(lut_sv);
	_aligned_free(image_buffer);
	DestroyWindow(hwndPB);
	SM_saveDark(newFileName, 1);
	if (!noCameraFlag)
		initLive(pdv_chan_num, cds_lib[curConfig], 1, layers_lib[curConfig], stripes_lib[curConfig], cameraType, QuadFlag, rotate_lib[curConfig], bad_pix_lib[curConfig]);
	SM_readData(newFileName);
}

static int byteSwapLUT[32767];
void SM_saveFITS(char *fitsName, int batchFlag)
{
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;
	int handle_fit, handle, flip_flag, x, y;
	long m, i;
	char cameraName[256], flagFname[MAX_PATH];
	signed short int *pt, *pt2, k;
	unsigned long dataLength,dataLeft,darkLength;
	FILE *fp;

	if ((findhandle = FindFirstFile(fitsName, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(fitsName);
	}
	if ((handle_fit = _open(fitsName,O_BINARY|O_CREAT|O_WRONLY,S_IWRITE)) == -1)
		return;
    if ((handle = _open(SM_dataname,O_BINARY | O_RDONLY)) == -1)
		return;

	SM_SetHourCursor();
	//make LUT table
	unsigned char *p, *q, v;
	for (i = 0; i < sizeof(byteSwapLUT)/sizeof(byteSwapLUT[0]); i++) {
		k = (signed short int)i;
		p = (unsigned char *)&k;
		q = p + 1;
		v = *p;
		*p = *q;
		*q = v;
		byteSwapLUT[i] = k;
	}

	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\flip_fits_flag.txt");
	if (FOPEN_S(fp,flagFname,"r")) {
		flip_flag = TRUE;
		fclose(fp);
	}
	else
		flip_flag = FALSE;

	_read(handle,(char *)FitsHeader,sizeof(FitsHeader));
	SM_readHeader(&numDataFrames, &disp_num_row, &disp_num_col, cameraName, &SM_exposure);
	dataLength = (unsigned long)numDataFrames*disp_num_row*disp_num_col*2;
	darkLength = (unsigned long)disp_num_row*disp_num_col*2;
	dataLeft = sizeof(FitsHeader) - (dataLength+darkLength)%sizeof(FitsHeader);

	if (single_img_size < (unsigned long)disp_num_col*disp_num_row) {
		if (single_img_data != NULL)
			_aligned_free(single_img_data);
		single_img_size = (unsigned long)disp_num_col*disp_num_row;
		single_img_data = (signed short int *)_aligned_malloc(single_img_size * sizeof(signed short int), 2);
		if (single_img_float != NULL)
			_aligned_free(single_img_float);
		single_img_float = (float *)_aligned_malloc(single_img_size * sizeof(float), 2);
	}

	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / numDataFrames;
	char str[256];
	sprintf(str, "Saving images: %d of %d", 0, numDataFrames);
	SetWindowText(hwndPB, str);

	SM_makeHeader(numDataFrames+1,disp_num_row,disp_num_col,cameraName,(double)SM_exposure);
	_write(handle_fit,(char *)FitsHeader,sizeof(FitsHeader));
	dataLength = (unsigned long)disp_num_row*disp_num_col*2;
	for (i = 0; i < numDataFrames; i++) {
		_read(handle,(signed short int *)image_data,dataLength);
		pt = (signed short int *)image_data;
		if (flip_flag) {
			pt2 = (signed short int *)single_img_data;
			memcpy(pt2, pt, darkLength);
			for (y = 0; y < disp_num_row/2; y++)
				for (x = 0; x < disp_num_col; x++) {
					*pt = byteSwapLUT[*(pt2+(disp_num_row-y*2-1)*disp_num_col)];
					*(pt+(disp_num_row-y*2-1)*disp_num_col) = byteSwapLUT[*pt2];
					pt++; pt2++;
				}
			pt += (darkLength>>2);
		}
		else {
			for (m = (dataLength>>1); m > 0; m--)
				*pt++ = byteSwapLUT[*pt];
		}
		_write(handle_fit,(signed short int *)image_data,dataLength);
		sprintf(str, "Saving images: %d of %d", i, numDataFrames);
		SetWindowText(hwndPB, str);
		while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}
	if (!dark_data)
		alloc_dark_data(disp_num_col,disp_num_row);
	_read(handle,(signed short int *)dark_data,darkLength);
	pt = (signed short int *)dark_data;
	if (flip_flag) {
		pt2 = (signed short int *)single_img_data;
		memcpy(pt2, pt, darkLength);
		for (y = 0; y < disp_num_row/2; y++)
			for (x = 0; x < disp_num_col; x++) {
				*pt = byteSwapLUT[*(pt2+(disp_num_row-y*2-1)*disp_num_col)];
				*(pt+(disp_num_row-y*2-1)*disp_num_col) = byteSwapLUT[*pt2];
				pt++; pt2++;
			}
	}
	else {
		for (m = (darkLength>>1); m > 0; m--)
			*pt++ = byteSwapLUT[*pt];
	}
	_write(handle_fit,(signed short int *)dark_data,darkLength);
	_write(handle_fit,(char *)FitsEnder,dataLeft);
	sprintf(str, "Finishing writing");
	SetWindowText(hwndPB, str);

	_commit(handle_fit);
	_close(handle_fit);
	DestroyWindow(hwndPB);
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
	if (!batchFlag)
		SM_readData(SM_dataname);
}


void SM_saveSm_tsm(char *fileName)
{
    int handle0, handle;
	unsigned long len, len0, dataLeft;
	signed short int *ptr, *b_ptr, *k_ptr, *dk_ptr;
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;
	int numFrame_to_save, i, m, l, k, j;
	long bin_val;

	if ((findhandle = FindFirstFile(fileName, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(fileName);
	}

	if ((handle0 = _open(SM_dataname, O_BINARY | O_RDONLY)) == -1)
		return;

	numFrame_to_save = save_sm_f_f2 - save_sm_f_f1 + 1;
	SM_makeHeader(numFrame_to_save,save_sm_f_h/sav_sp_bin,save_sm_f_w/sav_sp_bin, file_cameraName,(double)SM_exposure);
	len0 = (unsigned long)disp_num_col*disp_num_row*2;
	len = (unsigned long)save_sm_f_h*save_sm_f_w*2;
	dataLeft = sizeof(FitsHeader) - len*(numFrame_to_save+1)%sizeof(FitsHeader);

	if ((handle = _open(fileName,O_BINARY|O_CREAT|O_WRONLY,S_IWRITE)) == -1)
		return;
	_write(handle,(char *)FitsHeader,sizeof(FitsHeader));

	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size = ((double)num_steps) / numFrame_to_save;
	char str[256];
	sprintf(str, "Saving images: %d of %d", 0, numFrame_to_save);
	SetWindowText(hwndPB, str);

	_lseeki64(handle0, 2880L+save_sm_f_f1*len0, SEEK_SET);
	for (i = 0; i < numFrame_to_save; i++) {
		_read(handle0,(signed short *)image_data,len0);
		if (output_dksub_flag) {
			ptr = image_data;
			dk_ptr = dark_data;
			for (k = 0; k < (int)(len0 >> 1); k++, ptr++, dk_ptr++)
				*ptr = *ptr + 100 - *dk_ptr;
		}
		if (sav_sp_bin > 1) {
			ptr = image_data;
			b_ptr = image_data;
			for (m = 0; m < save_sm_f_h/sav_sp_bin; m++) {
				for (l = 0; l < save_sm_f_w/sav_sp_bin; l++) {
					bin_val = 0;
					k_ptr = ptr;
					for (j = 0; j < sav_sp_bin; j++) {
						for (k = 0; k < sav_sp_bin; k++)
							bin_val += *k_ptr++;
						k_ptr += disp_num_col - sav_sp_bin;
					}
					*b_ptr++ = (short)(bin_val/(sav_sp_bin*sav_sp_bin));
					ptr += sav_sp_bin;
				}
				ptr += disp_num_col*(sav_sp_bin-1);
			}
		}
		ptr = image_data + disp_num_col*save_sm_f_y0/sav_sp_bin + save_sm_f_x0/sav_sp_bin;
		for (m = 0; m < save_sm_f_h/sav_sp_bin; m++) {
			_write(handle,ptr,save_sm_f_w*2/sav_sp_bin);
			ptr += disp_num_col/sav_sp_bin;
		}
		sprintf(str, "Saving images: %d of %d", i, numFrame_to_save);
		SetWindowText(hwndPB, str);
		while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}
	if (output_dksub_flag)
		memset(image_data, 0, len0);
	else
		memcpy(image_data, dark_data, len0);
	if (sav_sp_bin > 1) {	//dark frame
		ptr = image_data;
		b_ptr = image_data;
		for (m = 0; m < save_sm_f_h/sav_sp_bin; m++) {
			for (l = 0; l < save_sm_f_w/sav_sp_bin; l++) {
				bin_val = 0;
				k_ptr = ptr;
				for (j = 0; j < sav_sp_bin; j++) {
					for (k = 0; k < sav_sp_bin; k++)
						bin_val += *k_ptr++;
					k_ptr += disp_num_col - sav_sp_bin;
				}
				*b_ptr++ = (short)(bin_val/(sav_sp_bin*sav_sp_bin));
				ptr += sav_sp_bin;
			}
			ptr += disp_num_col*(sav_sp_bin-1);
		}
	}
	ptr = image_data + disp_num_col*save_sm_f_y0/sav_sp_bin + save_sm_f_x0/sav_sp_bin;
	for (m = 0; m < save_sm_f_h/sav_sp_bin; m++) {
		_write(handle,ptr,save_sm_f_w*2/sav_sp_bin);
		ptr += disp_num_col/sav_sp_bin;
	}
	sprintf(str, "Finishing writing");
	SetWindowText(hwndPB, str);

	_write(handle,(char *)FitsEnder,dataLeft);
	_commit(handle);
	_close(handle);
	_close(handle0);
	DestroyWindow(hwndPB);
}

void SM_saveSmFile(char *fileName)
{
	int numFrame_to_save = save_sm_f_f2 - save_sm_f_f1 + 1;
	if ((numFrame_to_save < 1) || (save_sm_f_w <= 0) || (save_sm_f_h <= 0)) {
		MessageBox(main_window_handle, "There is a problem with save setting.", "message", MB_OK);
		return;
	}

	SM_SetHourCursor();
	if (strstr(fileName, ".tif"))
		SM_saveTIFF(fileName);
	else 
		SM_saveSm_tsm(fileName);
	SM_readData(SM_dataname);
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

void Save_RMS_pix(char *txtName)
{
    FILE *fp;
	int i, j, k, m;
	signed short int *pt;
	long trace_mean, len;
	int p_id;
	double trace_stv;
	double RMS_ar[128*128];
	int rms_ids[128*128];

	memset(rms_ids, 0, sizeof(rms_ids));
	if (FOPEN_S(fp,txtName,"w")) {
		len = disp_num_row*disp_num_col;
		for (j = 0; j < disp_num_row; j++) {
			for (i = 0; i < disp_num_col; i++) {
				p_id = i + j*disp_num_col;

				pt = (signed short *)image_data;
				pt += p_id;
				trace_mean = 0;
				for (k = 0; k < numTraceFrames; k++) {
					trace_mean += *pt;
					pt += len;
				}
				trace_mean /= numTraceFrames;

				pt = (signed short *)image_data;
				pt += i + j*disp_num_col;
				trace_stv = 0.0;
				for (k = 0; k < numTraceFrames; k++) {
					trace_stv += (*pt-trace_mean) * (*pt-trace_mean);
					pt += len;
				}
				trace_stv = trace_stv/numTraceFrames;

				for (k = p_id-1; k >= 0; k--) {
					if (trace_stv < RMS_ar[k])
						break;
				}
				m = k+1;
				for (k = p_id; k > m; k--) {
					RMS_ar[k] = RMS_ar[k-1];
					rms_ids[k] = rms_ids[k-1];
				}
				RMS_ar[m] = trace_stv;
				rms_ids[m] = p_id;
			}
		}
		for (i = 0; i < 500; i++) {
			fprintf(fp,"%d\n",rms_ids[i]+1);
	//		fprintf(fp,"%d, %.4lf\n",rms_ids[i]+1, RMS_ar[i]);
		}
		fclose(fp);
	}
}

void Batch_saveFITS(char *fName)
{
	char *p;
	char dir_str[MAX_PATH], tmp_str[MAX_PATH], dName_sv[MAX_PATH];
	int str_len;
	HANDLE handle;
	WIN32_FIND_DATA find_data;

	strcpy(dir_str, fName);
	strcpy(tmp_str, fName);
	str_len = (int)strlen(dir_str);
	while (p = strstr(tmp_str, "\\")) {
		strcpy(tmp_str, (p+1));
	}
	str_len = str_len - (int)strlen(tmp_str);
	p = dir_str;
	*(p+str_len) = '\0';

	if (dataFile)
		strcpy(dName_sv, SM_dataname);
	strcpy(tmp_str, dir_str);
	if ((handle = FindFirstFile(strcat(tmp_str,"\\*.tsm"),&find_data)) != INVALID_HANDLE_VALUE) {
		strcat(strcat(strcpy(SM_dataname, dir_str), "\\"), find_data.cFileName);
		strcpy(tmp_str, SM_dataname);
		p = strstr(tmp_str, ".tsm");
		*p = '\0';
		strcat(tmp_str, ".fit");
		SM_saveFITS(tmp_str, 1);
		while ((int)FindNextFile(handle,&find_data) > 0) {
			strcat(strcat(strcpy(tmp_str, dir_str), "\\"), find_data.cFileName);
			strcat(strcat(strcpy(SM_dataname, dir_str), "\\"), find_data.cFileName);
			strcpy(tmp_str, SM_dataname);
			p = strstr(tmp_str, ".tsm");
			*p = '\0';
			strcat(tmp_str, ".fit");
			SM_saveFITS(tmp_str, 1);
		}
	}
	if (dataFile) {
		strcpy(SM_dataname, dName_sv);
		SM_readData(SM_dataname);
	}
}

void SM_startAD()
{
	char command[256];

	if (!demoFlag) {
		SM_set_timeout(0);
		if (TXC_TriggerFlag || strstr(cameraType, "-SLVU") || (NI_flag == 1)) {
			if (ExtTriggerFlag != 2) {
				sprintf(command,"@TXC 1");
				SM_serial_command(command);
			}
		}
		else if (ExtTriggerFlag != 2 || LC_flag) {
			sprintf(command,"@FVI 0002");	// gated trigger
			SM_serial_command(command);
			if (!LC_flag) {
				sprintf(command,"@FVI 0004");	//for new version of firmware, trigger the start
				SM_serial_command(command);
			}
		}
//		NI_pulses();		//Old USB out
//		NI_stim_down();		//Old USB out
		if (MNC_flag)
			monochromater_conversion(0);
		if (AD_flag) {
			SM_serial_command(AD_rate_command);
			// Turn on Frame-inhibit
			sprintf(command,"@ICW $CC; 3; $25; 1; 0");
			SM_serial_command(command);
			// Turn interactive mode off
			sprintf(command,"@ICW $CC; 3; $25; 0; 0");
			SM_serial_command(command);
			if ((ExtTriggerFlag == 1)) { 
				//stop a/d to wait for trigger
				sprintf(command,"@ICW $CC; 2; $11; 0");
				SM_serial_command(command);
			} 
			else {
				//start a/d
				sprintf(command,"@ICW $CC; 2; $11; 1");
				SM_serial_command(command);
				if (sm_lib_rates[curConfig] >= 10.0) {
					sprintf(command,"@ICW $CC; 4; $14; 0; 2; 97");
					SM_serial_command(command);
				}
			}
		}
	}
}

void SM_stopAD()
{
	char command[256];

	if (!demoFlag) {
		if (AD_flag) {	//stop a/d
			// Turn interactive mode on
			sprintf(command,"@ICW $CC; 3; $25; 1; 1");
			SM_serial_command(command);
		}
		else {
			/*****for NI acquisition***/
#ifdef _USE_NI
			if (NI_flag)
				NI_setOutZero();
#endif
		/****/
		}
		sprintf(command,"@TXC 0");
		SM_serial_command(command);
		sprintf(command,"@FVI 0000");
		SM_serial_command(command);
		if (MNC_flag)
			monochromater_conversion(1);
	}
	SM_set_timeout(500);
}

void setNDR_pos(int the_cfg)
{
	if (ndr_lib[the_cfg] == 1) {
		char command[256];
		if (!NDR_pos) {
			NDR_pos = NDR_start_lib[the_cfg]+NDR_inc_lib[the_cfg]*(NDR_num_f-1);
			sprintf(command,"@MCR $%X; 1; %X", NDR_pos, (NDR_code_lib[the_cfg] | 0x80));
		}
		else {
			sprintf(command,"@MCR $%X; 1; %X", NDR_pos, NDR_code_lib[the_cfg]);
			NDR_pos = 0;
		}
		SM_serial_command(command);
		Sleep(20);
		sprintf(command,"@SEQ 1");
		SM_serial_command(command);
	}
}

void windows_system(const char *cmd)
{
	PROCESS_INFORMATION p_info;
	STARTUPINFO s_info;
//	LPSTR cmdline, programpath;

	memset(&s_info, 0, sizeof(s_info));
	memset(&p_info, 0, sizeof(p_info));
	s_info.cb = sizeof(s_info);

//	cmdline     = _tcsdup(TEXT(cmd));
//	programpath = _tcsdup(TEXT(cmd));

	if (CreateProcess(NULL, (char *)cmd, NULL, NULL, 0, 0, NULL, NULL, &s_info, &p_info))
	{
		WaitForSingleObject(p_info.hProcess, INFINITE);
		CloseHandle(p_info.hProcess);
		CloseHandle(p_info.hThread);
	}
}

void postAcqAct(int dark_f_flag, int cancelled_flag) {
	char command[256];

	if (TwoK_flag) {
		setFocusRep(focusMode);
	}

	GetSystemTime(&lastSystemTime);
	if (dark_f_flag != 1 && !cancelled_flag) {
		if (fd_no_deinterleave_flag && num_trials > 1) {
			getAcq_FileName(-1);
			D3DRectangle(image_region_dx / 3, image_region_dy / 3 - 20, image_region_dx / 3 + 200, image_region_dy / 3 + 30, colorBlack);
			char mssg[256];
			sprintf(mssg, "Trials Acquired: %d", av_file_cnt + 1);
			drawText(0, 0, mssg, image_region_dx / 3, image_region_dy / 3, TEXT_YELLOW, omFontBig, TRUE);
		}
		else {
			if (auto_dark_flag)
				acqDarkFrame(1);
			if (SM_saveDark(SM_dataname, digital_bin)) {
				if (!dark_f_flag) {
					getAcq_FileName(-1);
					Sleep(50);
					if (NDR_subtraction_flag)
						f_subtract_mode = 1;
					SM_readData(SM_dataname);
				}
			}
		}
	}
	sprintf(command, "@SEQ 1");
	SM_serial_command(command);
	setBGrey_Stop(1);
	if (cancelled_flag) {
		MessageBox(main_window_handle, "Acquisition cancelled", "message", MB_OK);
		MessageBox(main_window_handle, "Acquisition cancelled", "message", MB_OK);
	}
}

void StreamDataSM_64bit(int dark_f_flag)
{
	char command[256], file_name[MAX_PATH];
	char home_drive[256];
	int numbufs, cancelled_flag = 0;
	int f_size_ratio;
	char *p;

	strcpy(home_drive, home_dir);
	if (p = strstr(home_drive, ":"))
		*p = '\0';
	
	//	checkEDTtime();
	if (dark_f_flag == 2) {
		strcat(strcpy(SM_dataname, home_dir), "\\RIC_images.tsm");
		strcpy(file_name, SM_dataname);
	}
	else {
		get_saveName();
		strcpy(file_name, SM_dataname);
	}

	if (fd_no_deinterleave_flag && (av_file_cnt < sizeof(multi_file_names) / sizeof(multi_file_names[0])))
		strcpy(multi_file_names[av_file_cnt], file_name);

	if (demoFlag) {
		signed short *ptr;
		int demo_val;

		config_num_col = cam_num_col;
		config_num_row = cam_num_row;
		if (image_data != NULL)
			_aligned_free(image_data);
		image_data = (signed short *)_aligned_malloc(curNFrames*cam_num_col*cam_num_row*2, 2);

		// making a progress bar
		RECT rcClient;  // Client area of parent window.
		int cyVScroll = 30;  // Height of scroll bar arrow.
		HWND hwndPB;    // Handle of progress bar.
		int cur_pb_pos = 0, num_steps = 20;
		int pb_block_size = curNFrames/ num_steps;

		GetClientRect(main_window_handle, &rcClient);
		hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
			WS_CHILD | WS_VISIBLE, rcClient.left + rcClient.right / 6,
			rcClient.top + (rcClient.bottom - cyVScroll) * 2 / 5,
			rcClient.right / 4, cyVScroll,
			main_window_handle, (HMENU)0, NULL, NULL);
		SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, num_steps + 1));
		SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
		SendMessage(hwndPB, PBM_STEPIT, 0, 0);
		ptr = (signed short *)image_data;
		for (int k = 0; k < curNFrames; k++) {
			for (int j = 0; j < cam_num_row; j++) {
				demo_val = abs(j - demo_start) + 1;
				for (int i = 0; i < cam_num_col; i++)
					*ptr++ = demo_val + 1024;
			}
			demo_start += 1;
			if (demo_start >= cam_num_col)
				demo_start = 0;
			if ((k / pb_block_size > cur_pb_pos) && (cur_pb_pos + 1 < num_steps)) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				cur_pb_pos = k / pb_block_size;
			}
			/*			ptr = (signed short *)BNC_data;
			for (i = 0; i < numBNC_chan; i++) {
				for (k = 0; k < num_frames; k++) {
					demo_val = (int)(k);
					*ptr++ = demo_val + 1024;
				}
			}*/
		}
		DestroyWindow(hwndPB);
		numTraceFrames = curNFrames;
		disp_num_col = cam_num_col;
		disp_num_row = cam_num_row;
		SM_saveData(file_name);
		SM_readData(SM_dataname);
		return;
	}

	if (TwoK_flag) {
		if (super_frame_lib[curConfig] || ndr_lib[curConfig]) {
			sprintf(command,"@REP 0");
			SM_serial_command(command);
		}
		else {
			sprintf(command,"@REP %ld",repetitions_stream);
			SM_serial_command(command);
		}
		numbufs = 4;
	}
	else {
		numbufs = 64;
	}
	if (pcdFlag)
		f_size_ratio = 20;
	else if (TwoK_flag) {
		if (super_frame_lib[curConfig])
			f_size_ratio = (int)(1024/(config_num_row));		// 10 will be the fixed super-glued-frame factor
		else 
			f_size_ratio = 10*(int)(1024/config_num_row);
		if (ExtTriggerFlag == 2)
			curNFrames = max(curNFrames, f_size_ratio*2);
	}
	else if (ndr_lib[curConfig]>1)
		f_size_ratio = ndr_lib[curConfig];
	else
		f_size_ratio = (int)max(10, 10 * (1 + cds_lib[curConfig]) / frame_interval);

	if (ExtTriggerFlag != 2 || LC_flag) 
		SM_startAD();

/*	if (num_traces) {
		initRecord(pdv_chan_num, file_name, cds_lib[curConfig], ndr_lib[curConfig], curNFrames, layers_lib[curConfig], stripes_lib[curConfig], frame_interval, f_size_ratio, cameraType, file_frame_w, ccd_lib_bin[curConfig] / 2, BNC_ratio,
			ExtTriggerFlag, 0, dark_f_flag, QuadFlag, rotate_lib[curConfig], bad_pix_lib[curConfig], segment_lib[curConfig]);
		return;
	}*/

	if (ExtTriggerFlag < 2) {
		int preTrigger_frames = 0;
		if (ExtTriggerFlag)
			int preTrigger_frames = (int)(preTrigger_ms / frame_interval);
		if (writeToDisk_flag)
			SM_take_disk(pdv_chan_num, file_name, cds_lib[curConfig], curNFrames, layers_lib[curConfig], stripes_lib[curConfig], frame_interval, f_size_ratio, cameraType, file_frame_w, ccd_lib_bin[curConfig] / 2, BNC_ratio,
				ExtTriggerFlag, preTrigger_frames, QuadFlag, rotate_lib[curConfig], bad_pix_lib[curConfig], segment_lib[curConfig]);
		else
			cancelled_flag = !SM_take_tb(pdv_chan_num, file_name, cds_lib[curConfig], ndr_lib[curConfig], curNFrames, layers_lib[curConfig], stripes_lib[curConfig], frame_interval, f_size_ratio, cameraType, file_frame_w, ccd_lib_bin[curConfig] / 2, BNC_ratio,
				ExtTriggerFlag, preTrigger_frames, dark_f_flag, QuadFlag, rotate_lib[curConfig], bad_pix_lib[curConfig], segment_lib[curConfig]);
	}
	else {
		for (int i = 0; i < pdv_chan_num; ++i)
			pdv_setsize(pdv_pt[i], config_num_col, config_num_row);
		SM_pdv_close();

		int x1, y1, x2, y2;
		RECT cur_rect;
		GetWindowRect(main_window_handle, &cur_rect);
		x1 = getMeasureWinX0() + window_client_x0 + cur_rect.left;
		y1 = getMeasureWinY0() + window_client_y0 + cur_rect.top;
		x2 = getMeasureWinX0() + getMeasureWinX() + window_client_x0 + cur_rect.left;
		y2 = getMeasureWinY0() + getMeasureWinY() + window_client_y0 + cur_rect.top;

	/*	if (NI_flag)
			sprintf(command, "%s:\\EDT\\pdv\\SM_Record_NI", home_drive);
		else*/
			sprintf(command, "%s:\\EDT\\pdv\\SM_Record", home_drive);

		if (TwoK_flag) {
			if (pdv_chan_num == 4)
				sprintf(command, "%s -p 0_0 -p 0_1 -p 1_0 -p 1_1 -l 2 -s 8 -2", command);
			else
				sprintf(command, "%s -p 0_0 -p 0_1 -l 2 -s 4 -2", command);
			sprintf(command, "%s -t %s %s -n %d -b %d -i %lf -f %d -y %s -w %d -h %d -o %d", command,
				file_name, (cds_lib[curConfig] ? "-c " : ""), curNFrames, numbufs, frame_interval, f_size_ratio, cameraType, file_frame_w,
				ccd_lib_bin[curConfig] / 2, BNC_ratio);
		}
		else {
			if (ndr_lib[curConfig] > 1)
				f_size_ratio = ndr_lib[curConfig];
			sprintf(command, "%s -t %s %s -l %d -s %d -p %d_%d -n %d -b %d -i %lf -f %d -y %s -o %d", command, file_name,
				(cds_lib[curConfig] ? "-c " : ""), layers_lib[curConfig], stripes_lib[curConfig], pdv_dev_unit, pdv_dev_chan,
				curNFrames, numbufs, frame_interval, f_size_ratio, cameraType, BNC_ratio);
		}

			sprintf(command, "%s -M %d %d -W %d %d %d %d -P %s", command, auto_skip_v2, auto_skip_v1, x1, y1, x2, y2, tmp_file_dir);
			if (auto_scale_w)
				sprintf(command, "%s -A %d", command, auto_scale_w);
			if (RIC_flag)
				strcat(command, " -x");
			if (!autoScaleFlag)
				sprintf(command, "%s -m %d %d", command, scale_min, scale_max);
			if (darkSubFlag)
				strcat(command, " -D");
			if (edge_correct_flag)// && (!file_frame_w || (file_frame_w == 2*cam_num_col/(1+cds_lib[curConfig]))))
				sprintf(command, "%s -R %d %d %d", command, edge_correct_mode, edge_c_col1, edge_c_col2);
			if (NDR_subtraction_flag && ndr_lib[curConfig])
				sprintf(command, "%s -N %d", command, NDR_subtraction_frames);
/*		}
		else if (MNC_flag) {
			extern double MNC_in1_vol, MNC_in2_vol, MNC_out1_vol, MNC_out2_vol, MNC_w1_vol, MNC_w2_vol;
			extern int ratio_type, frame_per_ratio;
			monochromater_conversion(0);
			sprintf(command, "%s -M %d %d, %lf %lf %lf %lf %lf %lf", command, ratio_type, frame_per_ratio,
				MNC_in1_vol, MNC_in2_vol, MNC_out1_vol, MNC_out2_vol, MNC_w1_vol, MNC_w2_vol);
		}*/
		if (strstr(cameraType, "CCD39") || strstr(cameraType, "128X"))
			strcat(command, " -q");
		if (ExtTriggerFlag) {
			int preTrigger_frames = (int)(preTrigger_ms / frame_interval);
			sprintf(command, "%s -g %d", command, preTrigger_frames);
		}
		if (bad_pix_lib[curConfig])
			sprintf(command, "%s -a %d", command, bad_pix_lib[curConfig]);

		windows_system(command);

		int open_cnt = 0;
		while (!SM_pdv_open(0)) {
			Sleep(500);
			open_cnt++;
			if (open_cnt == 3) {
				MessageBox(main_window_handle, "SM_pdv_open Failed. Restart Turbo-SM", "message", MB_OK);
				break;
			}
		}
		if (pcdFlag != 1) {
			for (int i = 0; i < pdv_chan_num; ++i)
				pdv_setsize(pdv_pt[i], config_num_col, config_num_row*live_factor);
		}
	}

	postAcqAct(dark_f_flag, cancelled_flag);
}

void rearrange_MultiFiles()
{
	for (int i = 0; i < num_trials; i++) {
		rearrange_File(curNFrames, i, pdv_chan_num, QuadFlag, file_frame_w, segment_lib[curConfig]);
		SM_saveDark(multi_file_names[i], digital_bin);
	}
	SM_readData(multi_file_names[0]);
}

void openShutter()
{
	char command[256];

	if (AD_flag && !demoFlag) {
		sprintf(command, "@ICW $CC; 3; $25; 1; 3");
		SM_serial_command(command);
	}
	else {
		/*****for NI acquisition***/
#ifdef _USE_NI
		if (NI_flag || NI_out_flag) {
			NI_openShutter();
			Sleep(shutter_wait);
		}
#endif
		/****/
	}
}

void closeShutter()
{
	char command[256];

	if (AD_flag && !demoFlag) {
		sprintf(command, "@ICW $CC; 3; $25; 1; 1");
		SM_serial_command(command);
	}
	else {
		/*****for NI acquisition***/
#ifdef _USE_NI
		if (NI_flag || NI_out_flag)
			NI_setOutZero();
#endif
		/****/
	}
}

extern SYSTEMTIME acq_end_sysTime;
void StreamDataSM()
{
	SYSTEMTIME cur_systime;
	unsigned long ms_diff;

	OM[SM_STREAM].opt->grey = TRUE;
	if ((repeat_delay > 0) && (av_file_cnt > 0) && !spike_triggered) {
		GetSystemTime(&cur_systime);
		ms_diff = (cur_systime.wMinute - acq_end_sysTime.wMinute) * 60000 + (cur_systime.wMilliseconds - acq_end_sysTime.wSecond) * 1000 + cur_systime.wMilliseconds - acq_end_sysTime.wMilliseconds;
		if ((unsigned long)repeat_delay > ms_diff)
			Sleep(repeat_delay- ms_diff);
	}
	if (NI_out_flag)
		openShutter();
	StreamDataSM_64bit(0);
	if (NI_out_flag)
		closeShutter();
	OM[SM_STREAM].opt->grey = FALSE;
	if (num_trials > 1) {
		if (save_averaged)
			strcpy(av_filename_ar[av_file_cnt], SM_dataname);
		av_file_cnt++;
	}
}

void save_record_ref()
{
    int handle;
	int n_ref_frame;
    char filename[MAX_PATH];
	unsigned long dataLength,dataLeft,darkLength;
	signed short int *ptr;
	HANDLE findhandle;
	WIN32_FIND_DATA find_data;

	SM_SetHourCursor();
	strcat(strcpy(filename, tmp_file_dir), "\\record_ref.tsm");
	if ((findhandle = FindFirstFile(filename, &find_data)) != INVALID_HANDLE_VALUE) {
		FindClose(findhandle);
		DeleteFile(filename);
	}
	if (RIC_flag)
		n_ref_frame = 1;
	else
		n_ref_frame = 0;
	dataLength = (unsigned long)n_ref_frame*disp_num_row*disp_num_col*2;
	darkLength = (unsigned long)disp_num_row*disp_num_col*2;
	dataLeft = sizeof(FitsHeader) - (dataLength+darkLength)%sizeof(FitsHeader);

	SM_makeHeader(n_ref_frame,disp_num_row,disp_num_col,cameraType,(double)frame_interval/1000.0);
	if ((handle = _open(filename,O_BINARY|O_CREAT|O_WRONLY,S_IWRITE)) == -1)
		return;
	_write(handle,(char *)FitsHeader,sizeof(FitsHeader));
	if (n_ref_frame) {
		ptr = (signed short *)RIC_image;
		_write(handle,ptr,dataLength);
	}
	alloc_dark_data(disp_num_col,disp_num_row);
	if (darkExist)
		memcpy(dark_data, darkImage, darkLength);
	_write(handle,(signed short *)dark_data,darkLength);
	_write(handle,(char *)FitsEnder,dataLeft);
	_commit(handle);
	_close(handle);

	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

void makeRICframe()
{
	signed short int *ptr, *av_ptr, *dk_ptr;
	long this_pixel_val = 0L;
	long len = disp_num_col*disp_num_row;
	double RIC_total;
	int i, j, m;

	if (RIC_image != NULL)
		_aligned_free(RIC_image);
	RIC_image = (signed short int *)_aligned_malloc(len*sizeof(signed short int), 2);
	memset(RIC_image, 0, sizeof(RIC_image));
	av_ptr = (signed short int *)RIC_image;
	dk_ptr = (signed short int *)dark_data;
	ptr = (signed short int *)image_data;
	RIC_total = 0;
	for (j = 0; j < disp_num_row; j++) {
		for (i = 0; i < disp_num_col; i++) {
			for (m = 0; m < numTraceFrames; m++) {
				if (m == 0)
					this_pixel_val = *ptr;
				else
					this_pixel_val += *ptr;
				ptr += len;
			}
			ptr -= len*m;
			*av_ptr = (short)(this_pixel_val/m - *dk_ptr);
			RIC_total += *av_ptr;

			av_ptr++;
			dk_ptr++;
			ptr++;
		}
	}
	RIC_mean = (int)(RIC_total/len + 0.5);
	RIC_flag = 1;
	EnableMenuItem(main_menu_handle, RIC_REMOVE, MF_ENABLED);
	save_record_ref();
}

void recordRICdata()
{
	int dataFile_sv, nFrameSave, trg_sv;
	char f_name_sv[MAX_PATH];

	if (!darkExist) {
		MessageBox(main_window_handle, "Please take Dark Frame first", "message", MB_OK);
		return;
	}

	trg_sv = ExtTriggerFlag;
	dataFile_sv = dataFile;
	nFrameSave = curNFrames;
	if (dataFile)
		strcpy(f_name_sv, SM_dataname);

	curNFrames = num_RIC_frames; 
	ExtTriggerFlag = 0;
	StreamDataSM_64bit(2);
	makeRICframe();

	dataFile = dataFile_sv;
	curNFrames = nFrameSave;
	ExtTriggerFlag = trg_sv;
	if (dataFile)
		SM_readData(f_name_sv);	
}

void loadRICdata(char *filename)
{
	int dataFile_sv;
	char f_name_sv[MAX_PATH];

	dataFile_sv = dataFile;
	if (dataFile)
		strcpy(f_name_sv, SM_dataname);
	SM_readData(filename);
	makeRICframe();
	dataFile = dataFile_sv;
	if (dataFile)
		SM_readData(f_name_sv);	
}

void playMovie()
{
	curMovFrame++;
	if (curMovFrame >= numMovFrames)
		curMovFrame = 0;
	setMovieSlider(-1, SM_CURMOVF);
	paintWin(curTab(), PAINTINIT);
}

void playMovieFrame(int reverseFlag)
{
	if (NDR_subtraction_flag && NDR_cCDS_flag) {
		if (movie_rep_cnt < movie_step)
			movie_rep_cnt ++;
		else {
			movie_rep_cnt = 0;
			curDispFrame += NDR_subtraction_frames;
		}
	}
	else if (reverseFlag)
		curDispFrame--;
	else if (!movie_play_mode || !movie_step)
		curDispFrame++;
	else if (movie_play_mode == 1)
		curDispFrame += movie_step;
	else {
		if (movie_rep_cnt < movie_step)
			movie_rep_cnt ++;
		else {
			movie_rep_cnt = 0;
			curDispFrame++;
		}
	}
	resetDispFrame(1);
	setTraceThumb(-1, 0);
	paintWin(SM_TRACEWIN, PAINTINIT);
	paintWin(SM_IMGSCBASE, PAINTINIT);
}

int
readColors(int prf_flag)
{
	int handle,color, endfileFlag=FALSE, filelength;
	char col[OMCOLORS];
	int col2[170];

	if (!prf_flag) {
		if ( (handle = _open( colorsFname, _O_BINARY | _O_RDONLY )) == -1 ) {
			if( (handle = _open( colorsFnameOld, _O_BINARY | _O_RDONLY )) == -1 )
				return FALSE;
			_read(handle,(void *)col,sizeof(col));
			for (color = 0; color < OMCOLORS; ++color) {
				colors[color] = colorMap[(int)col[color]];
				if (!colors[color])
					endfileFlag = TRUE;
				if (endfileFlag)
					colors[color] = colorMap[COLOROFF];
			}
			_read(handle,(void *)fillPercent,sizeof(fillPercent));
			_read(handle,(void *)genLabels,sizeof(genLabels));
			_read(handle,(void *)genAbbrvs,sizeof(genAbbrvs));
			_close(handle);
			writeColors(0);
			return TRUE;
		}
		filelength = _lseek(handle,0L,SEEK_END);
		_lseek(handle,0L,SEEK_SET);
		if (filelength <= 898) {
			_read(handle,(void *)col2,sizeof(col2));
			for (color = 0; color < OMCOLORS; ++color) {
				colors[color] = col2[color];
				if (colors[color] <= 0)
					colors[color] = colorMap[COLOROFF];
			}
			_read(handle,(void *)fillPercent,sizeof(fillPercent));
			_read(handle,(void *)genLabels,sizeof(genLabels));
			_read(handle,(void *)genAbbrvs,sizeof(genAbbrvs));
			_close(handle);
			writeColors(0);
		}
		else {
			_read(handle,(void *)colors,sizeof(colors));
			_read(handle,(void *)fillPercent,sizeof(fillPercent));
			_read(handle,(void *)genLabels,sizeof(genLabels));
			_read(handle,(void *)genAbbrvs,sizeof(genAbbrvs));
			_close(handle);
		}
	}
	else {
		if ( (handle = _open( colorsFnamePrf, _O_BINARY | _O_RDONLY )) != -1 ) {
			_read(handle,(void *)colors,sizeof(colors));
			_read(handle,(void *)fillPercent,sizeof(fillPercent));
			_read(handle,(void *)genLabels,sizeof(genLabels));
			_read(handle,(void *)genAbbrvs,sizeof(genAbbrvs));
			_close(handle);
		}
	}
	return TRUE;
}

void
writeColors(int prf_flag)
{
    int handle;
	char CFname[MAX_PATH];

	if (!prf_flag)
		strcpy(CFname, colorsFname);
	else
		strcpy(CFname, colorsFnamePrf);
    if ((handle = _open(CFname,O_MODE,S_MODE)) != -1) {
		_write(handle,(void *)colors,sizeof(colors));
		_write(handle,(void *)fillPercent,sizeof(fillPercent));
		_write(handle,(void *)genLabels,sizeof(genLabels));
		_write(handle,(void *)genAbbrvs,sizeof(genAbbrvs));
		_close(handle);
	}
}

void CreateFonts()
{
	/*6432*/
//	if (screen_height > 750) {
		omFontBig = CreateFont((int)(19 * max(1.0, sqrt(screen_height / 1200.0))), 0, 0, 0, FW_MEDIUM, 0, 0, 0, ANSI_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman");
		omFontBigFat = CreateFont((int)(19 * max(1.0, sqrt(screen_height / 1200.0))),0,0,0,FW_MEDIUM,    0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Times New Roman");
		omFontMedium = CreateFont((int)(17 * max(1.0, sqrt(screen_height / 1200.0))), 0, 0, 0, FW_MEDIUM, 0, 0, 0, ANSI_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman");
		omFontSmall = CreateFont((int)(12 * max(1.0, sqrt(screen_height / 1200.0))),0,0,0,FW_EXTRALIGHT,0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"MS Sans Serif");
		omFontData = CreateFont((int)(16 * max(1.0, sqrt(screen_height / 1200.0))),0,0,0,FW_MEDIUM,    0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Courier New");
		omFontDataSm = CreateFont((int)(15 * max(1.0, sqrt(screen_height / 1200.0))),0,0,0,FW_MEDIUM,    0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Courier New");
		omFontRprt = CreateFont((int)(16 * max(1.0, sqrt(screen_height / 1200.0))),0,0,0,FW_HEAVY,     0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Courier New");
		omFontRprtTn = CreateFont((int)(14 * max(1.0, sqrt(screen_height / 1200.0))), 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New");
		/*	}
	else {
		omFontBig = CreateFont(19,0,0,0,FW_MEDIUM,    0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Times New Roman");
		omFontBigFat = CreateFont(19,0,0,0,FW_MEDIUM,    0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Times New Roman");
		omFontMedium = CreateFont(14, 0, 0, 0, FW_MEDIUM, 0, 0, 0, ANSI_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman");
		omFontSmall = CreateFont(10,0,0,0,FW_EXTRALIGHT,0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"MS Sans Serif");
		omFontData = CreateFont(14,0,0,0,FW_MEDIUM,    0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Courier New");
		omFontDataSm = CreateFont(12,0,0,0,FW_MEDIUM,    0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Courier New");
		omFontRprt = CreateFont(14,0,0,0,FW_HEAVY,     0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Courier New");
		omFontRprtTn = CreateFont(12,0,0,0,FW_HEAVY,     0,0,0,ANSI_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Courier New");
	}*/
	CourierFont =CreateFont(0, (int)(7 * max(1.0, sqrt(screen_height / 1200.0))),0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier New"));
}

void DeleteFonts(void)
{
	/*6432*/
	if (omFontRprtTn != NULL) DeleteObject(omFontRprtTn);
	if (omFontRprt != NULL) DeleteObject(omFontRprt);
	if (omFontDataSm != NULL) DeleteObject(omFontDataSm);
	if (omFontData != NULL) DeleteObject(omFontData);
	if (omFontSmall != NULL) DeleteObject(omFontSmall);
	if (omFontMedium != NULL) DeleteObject(omFontMedium);
	if (omFontBigFat != NULL) DeleteObject(omFontBigFat);
	if (omFontBig != NULL) DeleteObject(omFontBig);
}

/*
void initColors()
{
	if (!readColors(0))
		MessageBox(main_window_handle,
		(LPCTSTR) "Using default color set",(LPCTSTR) "Error reading colors file!",MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}*/

void FillCamConfigBox (HWND hwndList) 
{
	int i, k;

	if (!SendMessage (hwndList, LB_GETCOUNT, 0, 0)) {
		k = 0;
		for (i = 0; i < sizeof(config_list)/sizeof(config_list[0]); i++) {
			if (!strstr(config_list[i], "!") && (strcmp(config_list[i], ""))) {
				SendMessage (hwndList, LB_ADDSTRING, 0, (LPARAM) config_list[i]) ;
				config_ref_list[k] = i;
				config_ref_list_rev[i] = k;
				k++;
			}
		}
	}
}

bool
getFI_enableFlag()
{
	bool enableFlag;

	if (strstr(cameraType, "CCD39-AD"))
		enableFlag = (frame_interval < 2.0) ? false : true;
	else if (strstr(cameraType, "DW128"))
		enableFlag = (curConfig == 0 || curConfig == 3) ? true : false;
	else
		enableFlag = true;
	return enableFlag;
}

static int config_sv;
static double f_interval_sv;
static double f_interval_ar[8];
static int img_w_ar[8], scl_w_ar[8];

bool
CALLBACK CSettingProc(HWND hDlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HWND hwndList;
	RECT rect;
	int win_dx, win_dy;
	char tmp_str[256], tmp_str2[256], flagFname[256];
	char *p;
	int BNC_R_list[] = { 1, 2, 4, 8, 16, 32 };
	int digbin_list[] = { 1, 2, 4 };
	int cur_ratio_id = 0;
	int cur_digbin_id = 0;
	int tmp_curConfig, tmp_BNC_ratio, tmp_digbin;
	double tmp_frame_interval;
	FILE *fp;

	switch(msg) {
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case DIGBIN_TX:
			cur_digbin_id = (int)SendMessage(GetDlgItem(hDlg, DIGBIN_ID), UDM_GETPOS, 0, 0);
			if (cur_digbin_id < sizeof(digbin_list) / sizeof(digbin_list[0])) {
				tmp_digbin = digbin_list[cur_digbin_id];
				sprintf(tmp_str, "Digital Bin:   %d", tmp_digbin);
				SetDlgItemText(hDlg, DIGBIN_LABEL, tmp_str);
				SetFocus(GetDlgItem(hDlg, 1));
			}
			break;
		case BNC_RATIO_TX:
			cur_ratio_id = (int)SendMessage(GetDlgItem(hDlg, BNC_RATIO_ID), UDM_GETPOS, 0, 0);
			if (cur_ratio_id < sizeof(BNC_R_list) / sizeof(BNC_R_list[0])) {
				tmp_BNC_ratio = BNC_R_list[cur_ratio_id];
				sprintf(tmp_str, "BNC Ratio: %d", tmp_BNC_ratio);
				SetDlgItemText(hDlg, BNC_R_LABEL, tmp_str);
				SetFocus(GetDlgItem(hDlg, 1));
			}
			break;
		case CAM_CONFIGLIST:
			if (HIWORD(wparam) == 1) {
				hwndList = GetDlgItem(hDlg, CAM_CONFIGLIST);
				tmp_curConfig = config_ref_list[SendMessage(hwndList, LB_GETCURSEL, 0, 0)];
				tmp_frame_interval = 1.0 / sm_lib_rates[tmp_curConfig];
				if (tmp_frame_interval < f_interval_ar[tmp_curConfig])
					tmp_frame_interval = f_interval_ar[tmp_curConfig];
				EnableWindow(GetDlgItem(hDlg, CAM_FRM_INTERVAL), getFI_enableFlag());
				sprintf(tmp_str, "%lf", tmp_frame_interval);
				SetDlgItemText(hDlg, CAM_FRM_INTERVAL, tmp_str);
				if (TwoK_flag) {
					if (!img_w_ar[tmp_curConfig]) {
						strcpy(tmp_str2, config_list[tmp_curConfig]);
						GetDlgItemText(hDlg, FILE_IMG_WID, tmp_str, sizeof(tmp_str2));
						int this_w = atoi(tmp_str);
						if (p = strstr(tmp_str2, "x")) {
							strcpy(tmp_str, (p + 1));
							*p = '\0';
							while (p = strstr(tmp_str2, " "))
								strcpy(tmp_str2, (p + 1));
					//		if (this_w > atoi(tmp_str2))
								SetDlgItemText(hDlg, FILE_IMG_WID, tmp_str2);
					/*		else if (p = strstr(tmp_str, " ")) {
								*p = '\0';
								if (this_w < atoi(tmp_str))
									SetDlgItemText(hDlg, FILE_IMG_WID, tmp_str);
							}*/
						}
					}
					else {
						sprintf(tmp_str, "%d", img_w_ar[tmp_curConfig]);
						SetDlgItemText(hDlg, FILE_IMG_WID, tmp_str);
					}
					if (scl_w_ar[tmp_curConfig]) {
						sprintf(tmp_str, "%d", auto_scale_w);
						SetDlgItemText(hDlg, AUTO_S_W, tmp_str);
					}
				}
				if (ndr_lib[tmp_curConfig]) {
					EnableWindow(GetDlgItem(hDlg, NDR_FRAMES), true);
					EnableWindow(GetDlgItem(hDlg, NDR_F_LABEL), true);
				}
				else {
					EnableWindow(GetDlgItem(hDlg, NDR_FRAMES), false);
					EnableWindow(GetDlgItem(hDlg, NDR_F_LABEL), false);
				}
			}
			break;
		case TRIGGER0:
		case TRIGGER1:
			EnableWindow(GetDlgItem(hDlg, PRE_TRIGGER_ID), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_TRG_LABEL), FALSE);
			break;
		case TRIGGER2:
			EnableWindow(GetDlgItem(hDlg, PRE_TRIGGER_ID), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_TRG_LABEL), TRUE);
			break;
		case CAM_WS1M:
			break;
		case CAM_WS12M:
			break;
		case CAM_WS100M:
			break;
		case SPLITTER_MODE_BX:
			splitterAlignMode = !splitterAlignMode;
			if (splitterAlignMode)
				SendMessage (GetDlgItem (hDlg, SPLITTER_MODE_BX), BM_SETCHECK, 1, 0);
			else
				SendMessage (GetDlgItem (hDlg, SPLITTER_MODE_BX), BM_SETCHECK, 0, 0);
//			EnableWindow(GetDlgItem(hDlg, SPLITTER_MODE_BX), TRUE);
			break;
		case IDC_SAVE_AV:
			save_averaged = !save_averaged;
			if (!save_averaged)
				EnableWindow(GetDlgItem(hDlg, IDC_SPIKE_TRG), false);
			else
				EnableWindow(GetDlgItem(hDlg, IDC_SPIKE_TRG), true);
			break;
		case CAM_SETTING_OK:
			hwndList = GetDlgItem(hDlg, CAM_CONFIGLIST);
			curConfig = config_ref_list[SendMessage(hwndList, LB_GETCURSEL, 0, 0)];

			cur_digbin_id = (int)SendMessage(GetDlgItem(hDlg, DIGBIN_ID), UDM_GETPOS, 0, 0);
			digital_bin = digbin_list[cur_digbin_id];

			cur_ratio_id = (int)SendMessage(GetDlgItem(hDlg, BNC_RATIO_ID), UDM_GETPOS, 0, 0);
			BNC_ratio = BNC_R_list[cur_ratio_id];
			while ((BNC_ratio*sm_lib_rates[curConfig] * numBNC_chan > 500) && (BNC_ratio > 1))
				BNC_ratio /= 2;
			GetDlgItemText(hDlg, SHUTTER_DELAY_ID, tmp_str, sizeof(tmp_str));
			shutter_wait = atoi(tmp_str);
			GetDlgItemText(hDlg, STIM_DELAY_ID, tmp_str, sizeof(tmp_str));
			stim_delay = atoi(tmp_str);

			if (strstr(cameraType, "DW")) {
				GetDlgItemText(hDlg, NDR_FRAMES, tmp_str, sizeof(tmp_str));
				num_RIC_frames = max(10, atoi(tmp_str));
			}
			else if (TwoK_flag) {
				if (ndr_lib[curConfig] == 1) {
					GetDlgItemText(hDlg, NDR_FRAMES, tmp_str, sizeof(tmp_str));
					NDR_num_f = min(max(1, atoi(tmp_str)), NDR_max_lib[curConfig]);
				}
				GetDlgItemText(hDlg, FILE_IMG_WID, tmp_str, sizeof(tmp_str));
				file_frame_w = atoi(tmp_str);
				img_w_ar[curConfig] = file_frame_w;
				file_frame_w = file_frame_w*max(1, ccd_lib_bin[curConfig]/2);
				GetDlgItemText(hDlg, FOCUS_F_INT, tmp_str, sizeof(tmp_str));
				focus_frame_interval = max(atoi(tmp_str), (int)(1/sm_cam_lib_rates[curConfig]));
				GetDlgItemText(hDlg, AUTO_S_W, tmp_str, sizeof(tmp_str));
				auto_scale_w = atoi(tmp_str);
				auto_scale_w_acq = auto_scale_w;
				scl_w_ar[curConfig] = auto_scale_w;
			}

			if (curCamGain >= 0) {
				if (SendMessage (GetDlgItem (hDlg, CAM_G30X), BM_GETCHECK, 0, 0)) 
					curCamGain = 0;
				else if (SendMessage (GetDlgItem (hDlg, CAM_G10X), BM_GETCHECK, 0, 0)) 
					curCamGain = 1;
				else if (SendMessage (GetDlgItem (hDlg, CAM_G3X), BM_GETCHECK, 0, 0)) 
					curCamGain = 2;
				else if (SendMessage (GetDlgItem (hDlg, CAM_G1X), BM_GETCHECK, 0, 0)) 
					curCamGain = 3;
			}
			if (curChipGain >= 0) {
				if (SendMessage (GetDlgItem (hDlg, CAM_WS1M), BM_GETCHECK, 0, 0)) 
					curChipGain = 0;
				else if (SendMessage (GetDlgItem (hDlg, CAM_WS12M), BM_GETCHECK, 0, 0)) 
					curChipGain = 1;
				else if (SendMessage (GetDlgItem (hDlg, CAM_WS100M), BM_GETCHECK, 0, 0)) 
					curChipGain = 2;
			}
			GetDlgItemText(hDlg, CAM_FRM_INTERVAL, tmp_str, sizeof(tmp_str));
			frame_interval = max(1.0/sm_lib_rates[curConfig], (double)atof(tmp_str));
			f_interval_ar[curConfig] = frame_interval;
			GetDlgItemText(hDlg, CAM_TMP_BIN, tmp_str, sizeof(tmp_str));
			numExposures = atoi(tmp_str);
			GetDlgItemText(hDlg, CAM_NUM_FRAMES, tmp_str, sizeof(tmp_str));
			curNFrames = ((atoi(tmp_str)+5)/10)*10;
			curNFrames = max(10, curNFrames);

			GetDlgItemText(hDlg, CAM_EXP_NAME, tmp_str, sizeof(tmp_str));
			get_experiment_biopsy(tmp_str);

			// get Trigger Mode
			ExtTriggerFlag = 0;
			TXC_TriggerFlag = 0;
			if (SendMessage (GetDlgItem (hDlg, TRIGGER1), BM_GETCHECK, 0, 0)) {
				if (AD_flag || NI_flag)
					ExtTriggerFlag = 1;
				else if (strstr(cameraType, "CCD"))
					TXC_TriggerFlag = 1;
			}
			else if (SendMessage (GetDlgItem (hDlg, TRIGGER2), BM_GETCHECK, 0, 0))
				ExtTriggerFlag = 2;
			GetDlgItemText(hDlg, PRE_TRIGGER_ID, tmp_str, sizeof(tmp_str));
			preTrigger_ms = atoi(tmp_str);
			if (preTrigger_ms/frame_interval >= curNFrames && ExtTriggerFlag) {
				MessageBox(main_window_handle, "The Pre-trigger time should be within acquisition duration.", "message", MB_OK);
				return false;
			}
			GetDlgItemText(hDlg, TRIAL_REPEAT_ID, tmp_str, sizeof(tmp_str));
			num_trials = atoi(tmp_str);
			if (num_trials < 1)
				num_trials = 1;
			GetDlgItemText(hDlg, REPEAT_DELAY_ID, tmp_str, sizeof(tmp_str));
			repeat_delay = atoi(tmp_str);
			if (repeat_delay < 0)
				repeat_delay = 1000;
			if (SendMessage(GetDlgItem(hDlg, IDC_SAVE_AV), BM_GETCHECK, 0, 0))
				save_averaged = 1;
			else
				save_averaged = 0;
			if (SendMessage(GetDlgItem(hDlg, IDC_SPIKE_TRG), BM_GETCHECK, 0, 0))
				spike_triggered = 1;
			else
				spike_triggered = 0;
			if (TXC_TriggerFlag)
				MessageBox(main_window_handle, "This External Trigger mode involves stopping the camera while waiting for the trigger. The dark offsets may be different. We recommend turning on the light source a little late or turning it off a little early to have a dark period within the recorded data stream.", "message", MB_OK);
			if (streamDiskFlag && ExtTriggerFlag) {
				MessageBox(main_window_handle, "Streaming to disk cannot use External Trigger mode. Please reduce number of frames or deselect External Trigger", "message", MB_OK);
				return false;
			}
			EndDialog(hDlg, TRUE);
			return TRUE;
		case CAM_SETTING_CANCEL:
			curConfig = config_sv;
			frame_interval = f_interval_sv;
			EndDialog(hDlg, FALSE);
			return FALSE;
		}
		break;

	case WM_CREATE: 
		// do initialization stuff here
		return(0);
		break;
	case WM_PAINT:
		GetWindowRect (hDlg, &rect);
		win_dx = rect.right-rect.left;
		win_dy = rect.bottom-rect.top;
		MoveWindow (hDlg, (screen_width-win_dx)/2, (screen_height-win_dy)/2, win_dx, win_dy, TRUE);
		hwndList = GetDlgItem (hDlg, CAM_CONFIGLIST);
		SendMessage(hwndList,WM_SETFONT,(WPARAM)CourierFont,0);
		FillCamConfigBox(hwndList);
		SendMessage(hwndList,LB_SETCURSEL,config_ref_list_rev[curConfig],0);
		if (TwoK_flag || strstr(cameraType, "DW")) {
			if (TwoK_flag)
				sprintf(tmp_str, "%d", NDR_num_f);
			else
				sprintf(tmp_str, "%d", num_RIC_frames);
			SetDlgItemText(hDlg, NDR_FRAMES, tmp_str);
			if ((ndr_lib[curConfig] &&TwoK_flag) || strstr(cameraType, "DW")) {
				EnableWindow(GetDlgItem(hDlg, NDR_FRAMES), true);
				EnableWindow(GetDlgItem(hDlg, NDR_F_LABEL), true);
				if (strstr(cameraType, "DW")) 
					SetDlgItemText(hDlg, NDR_F_LABEL, "RIC Frames");
			}
			else {
				EnableWindow(GetDlgItem(hDlg, NDR_FRAMES), false);
				EnableWindow(GetDlgItem(hDlg, NDR_F_LABEL), false);
			}
			if (TwoK_flag) {
				sprintf(tmp_str, "%d", file_frame_w/max(1, ccd_lib_bin[curConfig]/2));
				SetDlgItemText(hDlg, FILE_IMG_WID, tmp_str);
				EnableWindow(GetDlgItem(hDlg, FILE_IMG_WID), true);
				EnableWindow(GetDlgItem(hDlg, FILE_WIDTH_SAVE), true);
				if (splitterAlignMode)
					SendMessage (GetDlgItem (hDlg, SPLITTER_MODE_BX), BM_SETCHECK, 1, 0);
				else
					SendMessage (GetDlgItem (hDlg, SPLITTER_MODE_BX), BM_SETCHECK, 0, 0);
				if (splitterMode)
					EnableWindow(GetDlgItem(hDlg, SPLITTER_MODE_BX), TRUE);
				else
					EnableWindow(GetDlgItem(hDlg, SPLITTER_MODE_BX), false);
				sprintf(tmp_str, "%d", focus_frame_interval);
				SetDlgItemText(hDlg, FOCUS_F_INT, tmp_str);
				EnableWindow(GetDlgItem(hDlg, FOCUS_F_INT), true);
				EnableWindow(GetDlgItem(hDlg, FOCUS_LB), true);
				sprintf(tmp_str, "%d", auto_scale_w_acq);
				SetDlgItemText(hDlg, AUTO_S_W, tmp_str);
				EnableWindow(GetDlgItem(hDlg, AUTO_S_W), TRUE);
				EnableWindow(GetDlgItem(hDlg, AUTO_S_LB), TRUE);
			}
			else {
				EnableWindow(GetDlgItem(hDlg, FILE_IMG_WID), false);
				EnableWindow(GetDlgItem(hDlg, FILE_WIDTH_SAVE), false);
				EnableWindow(GetDlgItem(hDlg, SPLITTER_MODE_BX), false);
				EnableWindow(GetDlgItem(hDlg, FOCUS_F_INT), false);
				EnableWindow(GetDlgItem(hDlg, FOCUS_LB), false);
				EnableWindow(GetDlgItem(hDlg, AUTO_S_W), false);
				EnableWindow(GetDlgItem(hDlg, AUTO_S_LB), false);
			}
		}
		else {
			EnableWindow(GetDlgItem(hDlg, NDR_FRAMES), false);
			EnableWindow(GetDlgItem(hDlg, NDR_F_LABEL), false);
			EnableWindow(GetDlgItem(hDlg, FILE_IMG_WID), false);
			EnableWindow(GetDlgItem(hDlg, FILE_WIDTH_SAVE), false);
			EnableWindow(GetDlgItem(hDlg, SPLITTER_MODE_BX), false);
			EnableWindow(GetDlgItem(hDlg, FOCUS_F_INT), false);
			EnableWindow(GetDlgItem(hDlg, FOCUS_LB), false);
			EnableWindow(GetDlgItem(hDlg, AUTO_S_W), false);
			EnableWindow(GetDlgItem(hDlg, AUTO_S_LB), false);
		}
		if (curCamGain >= 0) {
			int camGain[] = { CAM_G30X,CAM_G10X,CAM_G3X,CAM_G1X };
			SendMessage(GetDlgItem(hDlg, camGain[curCamGain]), BM_SETCHECK, 1, 0);
		}
		if (fixedCamGain || curCamGain < 0) {
			EnableWindow(GetDlgItem(hDlg, CAM_GAIN_LABEL), false);
			EnableWindow(GetDlgItem(hDlg, CAM_G30X), false);
			EnableWindow(GetDlgItem(hDlg, CAM_G10X), false);
			EnableWindow(GetDlgItem(hDlg, CAM_G3X), false);
			EnableWindow(GetDlgItem(hDlg, CAM_G1X), false);
		}
		if (curChipGain >= 0) {
			int chipGain[] = { CAM_WS1M,CAM_WS12M,CAM_WS100M };
			if (curChipGain == 0)
				SendMessage(GetDlgItem (hDlg, chipGain[curChipGain]), BM_SETCHECK, 1, 0);
		}
		else {
			EnableWindow(GetDlgItem(hDlg, CAM_WELLSIZE_LABEL), false);
			EnableWindow(GetDlgItem(hDlg, CAM_WS1M), false);
			EnableWindow(GetDlgItem(hDlg, CAM_WS12M), false);
			EnableWindow(GetDlgItem(hDlg, CAM_WS100M), false);
		}
		sprintf(tmp_str, "%lf", frame_interval);
		SetDlgItemText(hDlg, CAM_FRM_INTERVAL, tmp_str);
		EnableWindow(GetDlgItem (hDlg, CAM_FRM_INTERVAL), getFI_enableFlag()); 
		sprintf(tmp_str, "%d", numExposures);
		SetDlgItemText(hDlg, CAM_TMP_BIN, tmp_str);
		sprintf(tmp_str, "%d", curNFrames);
		SetDlgItemText(hDlg, CAM_NUM_FRAMES, tmp_str);

		if (!AD_flag && !NI_flag && !strstr(cameraType, "CCD"))
			EnableWindow(GetDlgItem(hDlg, TRIGGER1), FALSE);
		if (ExtTriggerFlag == 1 || TXC_TriggerFlag)
			SendMessage (GetDlgItem (hDlg, TRIGGER1), BM_SETCHECK, 1, 0);
		else if (ExtTriggerFlag == 2)
			SendMessage (GetDlgItem (hDlg, TRIGGER2), BM_SETCHECK, 1, 0);
		else
			SendMessage (GetDlgItem (hDlg, TRIGGER0), BM_SETCHECK, 1, 0);
		sprintf(tmp_str, "%d", preTrigger_ms);
		SetDlgItemText(hDlg, PRE_TRIGGER_ID, tmp_str);
		if (ExtTriggerFlag == 2) {
			EnableWindow(GetDlgItem(hDlg, PRE_TRIGGER_ID), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_TRG_LABEL), TRUE);
		}
		else {
			EnableWindow(GetDlgItem(hDlg, PRE_TRIGGER_ID), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_TRG_LABEL), FALSE);
		}

		sprintf(tmp_str, "%d", num_trials);
		SetDlgItemText(hDlg, TRIAL_REPEAT_ID, tmp_str);
		sprintf(tmp_str, "%d", repeat_delay);
		SetDlgItemText(hDlg, REPEAT_DELAY_ID, tmp_str);
		if (save_averaged)
			SendMessage(GetDlgItem(hDlg, IDC_SAVE_AV), BM_SETCHECK, 1, 0);
		else
			SendMessage(GetDlgItem(hDlg, IDC_SAVE_AV), BM_SETCHECK, 0, 0);
		if (spike_triggered)
			SendMessage(GetDlgItem(hDlg, IDC_SPIKE_TRG), BM_SETCHECK, 1, 0);
		else
			SendMessage(GetDlgItem(hDlg, IDC_SPIKE_TRG), BM_SETCHECK, 0, 0);
		if (!save_averaged)
			EnableWindow(GetDlgItem(hDlg, IDC_SPIKE_TRG), false);
		else
			EnableWindow(GetDlgItem(hDlg, IDC_SPIKE_TRG), true);

		HWND this_hwnd, buddyHwnd;
		this_hwnd = GetDlgItem (hDlg, BNC_RATIO_TX);
		buddyHwnd = this_hwnd;
		this_hwnd = GetDlgItem (hDlg, BNC_RATIO_ID);
		SendMessage(this_hwnd, UDM_SETBUDDY, (WPARAM)buddyHwnd, 0);
		SendMessage(this_hwnd, UDM_SETRANGE, 0, MAKELPARAM(5, 0));
		for (int i = 0; i < sizeof(BNC_R_list)/sizeof(BNC_R_list[0]); i++) {
			if (BNC_R_list[i] == BNC_ratio)
				cur_ratio_id = i;
		}
		SendMessage(this_hwnd, UDM_SETPOS, 0, cur_ratio_id);
		sprintf(tmp_str, "BNC Ratio: %d", BNC_ratio);
		SetDlgItemText(hDlg, BNC_R_LABEL, tmp_str);

		this_hwnd = GetDlgItem (hDlg, DIGBIN_TX);
		buddyHwnd = this_hwnd;
		this_hwnd = GetDlgItem (hDlg, DIGBIN_ID);
		SendMessage(this_hwnd, UDM_SETBUDDY, (WPARAM)buddyHwnd, 0);
		SendMessage(this_hwnd, UDM_SETRANGE, 0, MAKELPARAM(5, 0));
		for (int i = 0; i < sizeof(digbin_list)/sizeof(digbin_list[0]); i++) {
			if (digbin_list[i] == digital_bin)
				cur_digbin_id = i;
		}
		SendMessage(this_hwnd, UDM_SETPOS, 0, cur_digbin_id);
		sprintf(tmp_str, "Digital Bin:   %d", digital_bin);
		SetDlgItemText(hDlg, DIGBIN_LABEL, tmp_str);

		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\DiskData_flag.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			EnableWindow(GetDlgItem(hDlg, DIGBIN_TX), false);
			EnableWindow(GetDlgItem(hDlg, DIGBIN_ID), false);
			EnableWindow(GetDlgItem(hDlg, DIGBIN_LABEL), false);
			fclose(fp);
		}
		else if (p = strstr(cameraType, "-FASTD")) {
			EnableWindow(GetDlgItem(hDlg, DIGBIN_TX), false);
			EnableWindow(GetDlgItem(hDlg, DIGBIN_ID), false);
			EnableWindow(GetDlgItem(hDlg, DIGBIN_LABEL), false);
		}

		sprintf(tmp_str, "%d", shutter_wait);
		SetDlgItemText(hDlg, SHUTTER_DELAY_ID, tmp_str);
		sprintf(tmp_str, "%d", stim_delay);
		SetDlgItemText(hDlg, STIM_DELAY_ID, tmp_str);
		if (!NI_flag) {
			EnableWindow(GetDlgItem(hDlg, BNC_RATIO_TX), false);
			EnableWindow(GetDlgItem(hDlg, BNC_RATIO_ID), false);
			EnableWindow(GetDlgItem(hDlg, BNC_R_LABEL), false);
//			EnableWindow(GetDlgItem(hDlg, SHUTTER_D_LABEL), false);
			EnableWindow(GetDlgItem(hDlg, SHUTTER_DELAY_ID), false);
			EnableWindow(GetDlgItem(hDlg, STIM_DELAY_ID), false);
		}
		sprintf(tmp_str, "%s\\%s", experiment, biopsy);
		SetDlgItemText(hDlg, CAM_EXP_NAME, tmp_str);
		config_sv = curConfig;
		f_interval_sv = frame_interval;
		break;
	}
	return FALSE;
}

int 
getCameraSetting(void)
{
	return (int)DialogBox(main_instance, TEXT("CAM_SETTING_DLG"),main_window_handle, (DLGPROC)CSettingProc);
}

BOOL 
CALLBACK SaveSettingProc(HWND hDlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	int win_dx, win_dy;
	char tmp_str[50];

	switch(msg) {	
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case INPUT_ROI:
			if (num_traces) {
				sprintf(tmp_str, "%d", ROI_index[0][1]);
				SetDlgItemText(hDlg, SM_F_X0, tmp_str);
				sprintf(tmp_str, "%d", ROI_index[0][3]);
				SetDlgItemText(hDlg, SM_F_Y0, tmp_str);
				sprintf(tmp_str, "%d", ROI_index[0][2] - ROI_index[0][1]);
				SetDlgItemText(hDlg, SM_F_WIDTH, tmp_str);
				sprintf(tmp_str, "%d", ROI_index[0][4] - ROI_index[0][3]);
				SetDlgItemText(hDlg, SM_F_HEIGHT, tmp_str);
			}
			else 
				MessageBox(main_window_handle, "You need to drag on image to select a ROI.", "message", MB_OK);
			break;
		case INPUT_FULL:
			sprintf(tmp_str, "%d", 0);
			SetDlgItemText(hDlg, SM_F_X0, tmp_str);
			sprintf(tmp_str, "%d", 0);
			SetDlgItemText(hDlg, SM_F_Y0, tmp_str);
			sprintf(tmp_str, "%d", disp_num_col);
			SetDlgItemText(hDlg, SM_F_WIDTH, tmp_str);
			sprintf(tmp_str, "%d", disp_num_row);
			SetDlgItemText(hDlg, SM_F_HEIGHT, tmp_str);
			break;
		case SAVE_F_TYPE1:
			EnableWindow(GetDlgItem (hDlg, SM_SP_BIN), TRUE);
			break;
		case SAVE_F_TYPE2:
			EnableWindow(GetDlgItem (hDlg, SM_SP_BIN), FALSE);
			break;
		case IDOK:
			if (tiffProcFlag) {
				GetDlgItemText(hDlg, SM_F_FRAME1, tmp_str, sizeof(tmp_str));
				save_sm_f_f1 = atoi(tmp_str)-1;
				GetDlgItemText(hDlg, SM_F_FRAME2, tmp_str, sizeof(tmp_str));
				save_sm_f_f2 = atoi(tmp_str)-1;
				if ((save_sm_f_f1 < firstDataFrame) || (save_sm_f_f1 >= numTraceFrames+firstDataFrame) || 
					(save_sm_f_f2 < firstDataFrame) || (save_sm_f_f1 >= numTraceFrames+firstDataFrame) ||
					(save_sm_f_f2 < save_sm_f_f1)) {
					MessageBox(main_window_handle, "The frame 1 or 2 is out of range of current data.", "message", MB_OK);
					return FALSE;
				}
			}
			else {
				save_sm_f_type = SendMessage(GetDlgItem(hDlg, SAVE_F_TYPE1), BM_GETCHECK, 0, 0)? 0 : 1;
				GetDlgItemText(hDlg, SM_F_FRAME1, tmp_str, sizeof(tmp_str));
				save_sm_f_f1 = min(max(atoi(tmp_str)-1, 0), numDataFrames-1);
				GetDlgItemText(hDlg, SM_F_FRAME2, tmp_str, sizeof(tmp_str));
				save_sm_f_f2 = min(max(atoi(tmp_str)-1, save_sm_f_f1), numDataFrames-1);
				if (SendMessage(GetDlgItem(hDlg, OUTPUT_DARK_SUB), BM_GETCHECK, 0, 0))
					output_dksub_flag = 1;
				else
					output_dksub_flag = 0;
			}
			GetDlgItemText(hDlg, SM_F_X0, tmp_str, sizeof(tmp_str));
			save_sm_f_x0 = max(0, min(atoi(tmp_str), disp_num_col-1));
			GetDlgItemText(hDlg, SM_F_Y0, tmp_str, sizeof(tmp_str));
			save_sm_f_y0 = max(0, min(atoi(tmp_str), disp_num_row-1));
			GetDlgItemText(hDlg, SM_F_WIDTH, tmp_str, sizeof(tmp_str));
			save_sm_f_w = min(atoi(tmp_str), disp_num_col-save_sm_f_x0);
			GetDlgItemText(hDlg, SM_F_HEIGHT, tmp_str, sizeof(tmp_str));
			save_sm_f_h = min(atoi(tmp_str), disp_num_row-save_sm_f_y0);
			GetDlgItemText(hDlg, SM_SP_BIN, tmp_str, sizeof(tmp_str));
			sav_sp_bin = min(max(atoi(tmp_str), 1), 20);
			GetDlgItemText(hDlg, SM_TMP_BIN, tmp_str, sizeof(tmp_str));
			sav_tmp_bin = min(max(atoi(tmp_str), 1), 10);
			EndDialog(hDlg, true);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, false);
			return FALSE;
		}
		break;
	case WM_CREATE: 
		// do initialization stuff here
		return(0);
		break;
	case WM_PAINT:
		GetWindowRect (hDlg, &rect);
		win_dx = rect.right-rect.left;
		win_dy = rect.bottom-rect.top;
		MoveWindow (hDlg, (screen_width-win_dx)/2, (screen_height-win_dy)/2, win_dx, win_dy, true);
		if (tiffProcFlag) {
			SendMessage (GetDlgItem (hDlg, SAVE_F_TYPE2), BM_SETCHECK, 1, 0);
			EnableWindow(GetDlgItem (hDlg, SAVE_F_TYPE1), false);
			EnableWindow(GetDlgItem(hDlg, SAVE_F_TYPE2), false);
			EnableWindow(GetDlgItem(hDlg, SM_SP_BIN), false);
			EnableWindow(GetDlgItem(hDlg, OUTPUT_DARK_SUB), false);
			EnableWindow(GetDlgItem(hDlg, OUTPUT_EDGE_CORRECTION), false);
			save_sm_f_f1 = min(max(save_sm_f_f1, firstDataFrame), numTraceFrames-1-firstDataFrame);
			save_sm_f_f2 = min(max(save_sm_f_f2, save_sm_f_f1), numTraceFrames-1-firstDataFrame);
		}
		else {
			SendMessage (GetDlgItem (hDlg, (save_sm_f_type == 0)? SAVE_F_TYPE1: SAVE_F_TYPE2), BM_SETCHECK, 1, 0);
			EnableWindow(GetDlgItem (hDlg, SAVE_F_TYPE1), true);
			EnableWindow(GetDlgItem (hDlg, SAVE_F_TYPE2), true);
			EnableWindow(GetDlgItem (hDlg, SM_SP_BIN), true);
			EnableWindow(GetDlgItem(hDlg, OUTPUT_DARK_SUB), true);
			EnableWindow(GetDlgItem(hDlg, OUTPUT_EDGE_CORRECTION), false);
			save_sm_f_f1 = min(max(save_sm_f_f1, 0), numDataFrames-1);
			save_sm_f_f2 = min(max(save_sm_f_f2, save_sm_f_f1), numDataFrames-1);
		}
		if (output_dksub_flag) 
			SendMessage(GetDlgItem(hDlg, OUTPUT_DARK_SUB), BM_SETCHECK, 1, 0);
		else 
			SendMessage(GetDlgItem(hDlg, OUTPUT_DARK_SUB), BM_SETCHECK, 0, 0);

		sprintf(tmp_str, "%d", save_sm_f_f1+1);
		SetDlgItemText(hDlg, SM_F_FRAME1, tmp_str);
		sprintf(tmp_str, "%d", save_sm_f_f2+1);
		SetDlgItemText(hDlg, SM_F_FRAME2, tmp_str);
		save_sm_f_x0 = min(save_sm_f_x0, disp_num_col-1);
		save_sm_f_y0 = min(save_sm_f_y0, disp_num_row-1);
		sprintf(tmp_str, "%d", save_sm_f_x0);
		SetDlgItemText(hDlg, SM_F_X0, tmp_str);
		sprintf(tmp_str, "%d", save_sm_f_y0);
		SetDlgItemText(hDlg, SM_F_Y0, tmp_str);
		save_sm_f_w = min(save_sm_f_w, disp_num_col-save_sm_f_x0);
		save_sm_f_h = min(save_sm_f_h, disp_num_row-save_sm_f_y0);
		sprintf(tmp_str, "%d", save_sm_f_w);
		SetDlgItemText(hDlg, SM_F_WIDTH, tmp_str);
		sprintf(tmp_str, "%d", save_sm_f_h);
		SetDlgItemText(hDlg, SM_F_HEIGHT, tmp_str);
		sprintf(tmp_str, "%d", sav_sp_bin);
		SetDlgItemText(hDlg, SM_SP_BIN, tmp_str);
		sprintf(tmp_str, "%d", sav_tmp_bin);
		SetDlgItemText(hDlg, SM_TMP_BIN, tmp_str);
		break;
	}
	return FALSE;
}

int
getSaveSmallSetting(void)
{
	return (int)DialogBox(main_instance, TEXT("SMALL_FILE_SAVE"),main_window_handle, (DLGPROC)SaveSettingProc);
}

bool
CALLBACK TiffBitProc(HWND hDlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	int win_dx, win_dy;

	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			TiffBit_mode = SendMessage(GetDlgItem(hDlg, TIFF_BIT1), BM_GETCHECK, 0, 0) ? 0 : 1;
			EndDialog(hDlg, true);
			return true;
		case IDCANCEL:
			EndDialog(hDlg, false);
			return FALSE;
		}
		break;
	case WM_CREATE:
		// do initialization stuff here
		return(0);
		break;
	case WM_PAINT:
		GetWindowRect(hDlg, &rect);
		win_dx = rect.right - rect.left;
		win_dy = rect.bottom - rect.top;
		MoveWindow(hDlg, (screen_width - win_dx) / 2, (screen_height - win_dy) / 2, win_dx, win_dy, true);
		SendMessage(GetDlgItem(hDlg, (TiffBit_mode == 0) ? TIFF_BIT1 : TIFF_BIT1), BM_SETCHECK, 1, 0);
		break;
	}
	return FALSE;
}

int
getTiffBit(void)
{
	return (int)DialogBox(main_instance, TEXT("SINGLE_TIFF_SAVE"), main_window_handle, (DLGPROC)TiffBitProc);
}

bool
CALLBACK SetFirstFProc(HWND hDlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	int win_dx, win_dy;
	char tmp_str[50];

	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			GetDlgItemText(hDlg, FIRST_F_EDIT, tmp_str, sizeof(tmp_str));
			firstDataFrame = max(0, min(atoi(tmp_str) - 1, numDataFrames - 50));
			EndDialog(hDlg, true);
			return true;
		}
		break;
	case WM_CREATE:
		// do initialization stuff here
		return(0);
		break;
	case WM_PAINT:
		GetWindowRect(hDlg, &rect);
		win_dx = rect.right - rect.left;
		win_dy = rect.bottom - rect.top;
		MoveWindow(hDlg, (screen_width - win_dx) / 2, (screen_height - win_dy) / 2, win_dx, win_dy, true);
		sprintf(tmp_str, "%d", firstDataFrame + 1);
		SetDlgItemText(hDlg, FIRST_F_EDIT, tmp_str);
		break;
	}
	return FALSE;
}

int
getFirstDataFrame(void)
{
	return (int)DialogBox(main_instance, TEXT("SET_FIRST_F"), main_window_handle, (DLGPROC)SetFirstFProc);
}

bool
CALLBACK getTiffExpProc(HWND hDlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	int win_dx, win_dy;
	char tmp_str[50];
	double this_val;

	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDOK:
			GetDlgItemText(hDlg, TIFF_EXP_EDIT, tmp_str, sizeof(tmp_str));
			if (strcmp(tmp_str, "")) {
				sscanf(tmp_str, "%lf", &this_val);
				if (this_val > 0.000000001)
					SM_exposure = this_val/1000.0;
			}
	/*		if (SendMessage(GetDlgItem(hDlg, TIFF_SCL_CHCK), BM_GETCHECK, 0, 0))
				tif_scale_flag = 1;
			else
				tif_scale_flag = 0;*/
			EndDialog(hDlg, true);
			return true;
		}
		break;
	case WM_CREATE:
		// do initialization stuff here
		return(0);
		break;
	case WM_PAINT:
		GetWindowRect(hDlg, &rect);
		win_dx = rect.right - rect.left;
		win_dy = rect.bottom - rect.top;
		MoveWindow(hDlg, (screen_width - win_dx) / 2, (screen_height - win_dy) / 2, win_dx, win_dy, true);
		sprintf(tmp_str, "%lf", SM_exposure*1000);
		SetDlgItemText(hDlg, TIFF_EXP_EDIT, tmp_str);
	/*	if (tif_scale_flag)
			SendMessage(GetDlgItem(hDlg, TIFF_SCL_CHCK), BM_SETCHECK, 1, 0);
		else
			SendMessage(GetDlgItem(hDlg, TIFF_SCL_CHCK), BM_SETCHECK, 0, 0);*/
		break;
	}
	return FALSE;
}

int
getTiffExpTime(void)
{
	return (int)DialogBox(main_instance, TEXT("GET_TIFF_EXP"), main_window_handle, (DLGPROC)getTiffExpProc);
}

void
getNDR_resets()
{
	int i, j, ii, num_av;
	long len, this_val;
	signed short *pt;
	
	memset(NDR_reset_ar, 0, sizeof(NDR_reset_ar));
	len = disp_num_col*disp_num_row;
	for (ii = 0; ii < numTraceFrames; ii++) {
		num_av = 0;
		this_val = 0;
		pt = (signed short *)image_data + len*ii + 2 + 2*disp_num_col;
		for (j = 2; j < min(20, disp_num_row/4); j++) {
			for (i = 2; i < min(10, disp_num_col/4); i++) {
				if (!num_av)
					this_val = *pt;
				else
					this_val += *pt;
				pt++;
				num_av++;
			}
			pt += disp_num_col - i + 2;
		}
		trace_sv[ii] = (float)this_val/num_av;
	}

	j = 0;
	for (ii = 1; ii < numTraceFrames; ii++) {
		if (trace_sv[ii] < trace_sv[ii-1] - 10) {
			NDR_reset_ar[j] = ii;
			j++;
			if (j >= sizeof(NDR_reset_ar)/sizeof(NDR_reset_ar[0])-1)
				break;
		}
	}
	num_NDR_resets = j;
	if ((num_NDR_resets > 2) && (NDR_reset_ar[1] - NDR_reset_ar[0] < 10)) {
		MessageBox(main_window_handle, "This may not be a NDR data file.", "message", MB_OK);
		NDR_subtraction_flag = 0;
		f_subtract_mode = 0;
	}
}

void
dispSettingCheck()
{
	if (dataFile && f_subtract_mode) {
		if (NDR_subtraction_flag)
			getNDR_resets();

		ref_frame1 = min(max(ref_frame1, 0), numDataFrames - 1);
		ref_frame2 = min(max(ref_frame2, ref_frame1), numDataFrames - 1);
		getRefImage();
	}
}

static double meanOffsets[64];
static int darkOffsetAr[1024]; //32x4x8

void
readCamoffsets()
{
	char offsetFname[MAX_PATH] = ".\\SMSYSDAT\\sm_offsets.dat";
    int handle;

    if ((handle = _open(offsetFname,O_BINARY | O_RDONLY)) == -1)
        return;
	_read(handle,(int *)darkOffsetAr,sizeof(darkOffsetAr));
	_close(handle);
}

void
loadCamOffsets()
{
	char command[256], flagFname[MAX_PATH];
	int sm_test_offset[MAXOFFSETS];
	int configToUse;
	FILE *fp;

	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\DoNotLoadOffsets.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		fclose(fp);
		return;
	}
		
	if (comFlag)
		return;
	int num_cam_channels = stripes_lib[curConfig] * layers_lib[curConfig];
	if (cds_offset_chan[curConfig] >= 0)
		configToUse = cds_offset_chan[curConfig];
	else
		configToUse = curConfig;
	for (int i = 0; i < num_cam_channels; i++) {
		sm_test_offset[i] = darkOffsetAr[configToUse * 8 * 4 + curCamGain * 4 + i];
		if (sm_test_offset[i] > 0) {
			sprintf(command, "@OIC %d: %d", offset_chan_order[i], sm_test_offset[i]);
			SM_serial_command(command);
		}
	}
}

void
getCamOffsets(int PHT_flag)
{
	signed short *pt;
	double channel_sum;
	int i, j, m, n, chan, layers, stripes;

	AcquireOneFrame(0);
	chan = 0;
	if (PHT_flag) {
		for (i = 0; i < num_chan_total; i++) {
			channel_sum = 0.0;
			pt = (signed short *)single_img_data + dark_coor_y[i]*cam_num_col + dark_coor_x[i];
			for (n = dark_coor_y[i]; n < dark_coor_y[i]+dark_coor_dy[i]; n++) {
				for (m = dark_coor_x[i]; m < dark_coor_x[i]+dark_coor_dx[i]; m++)
					channel_sum += *pt++;
				pt += cam_num_col - dark_coor_dx[i];
			}
			meanOffsets[chan] = channel_sum/(dark_coor_dy[i]*dark_coor_dx[i]);
			chan++;
		}
	}
	else {
		layers = layers_lib[curConfig];
		stripes = stripes_lib[curConfig];
		for (j = 0; j < layers; j++) {
			for (i = 0; i < stripes; i++) {
				channel_sum = 0.0;
				pt = (signed short *)single_img_data + cam_num_col*j*cam_num_row/layers + i*cam_num_col/stripes;
				for (n = 0; n < cam_num_row/layers; n++) {
					for (m = 0; m < cam_num_col/stripes; m++)
						channel_sum += *pt++;
	//					channel_sum += frameAr[j*cam_num_row/layers+n][i*cam_num_col/stripes+m];
					pt += cam_num_col - cam_num_col/stripes;
				}
				meanOffsets[chan] = channel_sum*layers*stripes/(cam_num_row*cam_num_col);
				chan++;
			}
		}
	}
}

void AutoSetOffset(int PHT_flag)
{
	FILE *fp;
	int i, handle, chan_id, num_cam_channels, autoSetDone = false, sm_test_offset_sv;
	int sm_test_offset0[MAXOFFSETS], sm_test_offset1[MAXOFFSETS], sm_test_offset[MAXOFFSETS];
	char *p, *q, command[256], buf[256];
	char mean_set_Fname[MAX_PATH] = ".\\SMSYSDAT\\offset_mean_set.txt", offset_inc_Fname[MAX_PATH] = ".\\SMSYSDAT\\offset_increment.txt", offsetFname[MAX_PATH] = ".\\SMSYSDAT\\sm_offsets.dat";
	double standard_offset_mean;
	double meanOffsets0[64], meanOffsets1[64];

	num_cam_channels = stripes_lib[curConfig] * layers_lib[curConfig];

	char tmp_str[1024], ret_c[1024], str[256];
	HWND hwndPB = NULL;
	int step_cnt = 0;
	double step_size = ((double)num_steps) / num_cam_channels;
	if (num_cam_channels > 10) {
		hwndPB = createProgBar();
		sprintf(str, "Auto Set Offsets");
		SetWindowText(hwndPB, str);
	}
	else
		SM_SetHourCursor();

	standard_offset_mean = (p = strstr(cameraType, "DW"))? 1000.0 : 500.0;
	if (FOPEN_S(fp,mean_set_Fname,"r")) {
		if (fgets(buf,255,fp))
			standard_offset_mean = atoi(buf);
		fclose(fp);
	}
	for (i = 0; i < MAXOFFSETS; i++) {
		sm_test_offset0[i] = 50;
		if (joe_version >= 800)
			sm_test_offset1[i] = 4095;
		else
			sm_test_offset1[i] = 1023;
	}
	if (num_cam_channels > 10) {
		double meanOffsets_sv[64];

		PHT_flag = 0;
		if (hwndPB)
			SendMessage(hwndPB, PBM_SETPOS, (WPARAM)0, 0);
		step_cnt = 0;
		getCamOffsets(PHT_flag);
		memcpy(meanOffsets_sv, meanOffsets, sizeof(double)*64);
		sprintf(command, "@OAC?");
		SM_pdv_query_command(command, ret_c, 1024);
		int this_chan, hex_flag = 0, num_offset_chan = 0, chan_seq_id, test_offset_diff = 100;
		if (FOPEN_S(fp, offset_inc_Fname, "r")) {
			if (fgets(buf, 255, fp))
				test_offset_diff = atoi(buf);
			fclose(fp);
		}
		while (p = strstr(ret_c, "#")) {
			strcpy(tmp_str, (p + 1));
			if (q = strstr(tmp_str, ":")) {
				strcpy(ret_c, (q + 1));
				*q = '\0';
				this_chan = atoi(tmp_str);
				strcpy(tmp_str, ret_c);
				if (q = strstr(tmp_str, "$")) {
					hex_flag = 1;
					strcpy(tmp_str, (q + 1));
				}
				if (q = strstr(tmp_str, ";")) {
					strcpy(ret_c, (q + 1));
					*q = '\0';
				}
				for (i = 0; i < num_cam_channels; i++) {
					if (PHT_flag || noise_seq_flag) 
						chan_id = dark_chan_map[i / num_chan_x][i%num_chan_x] - 1;
					else
						chan_id = offset_chan_order[i];
					if (chan_id == this_chan) {
						chan_seq_id = i;
						break;
					}
				}
				if (hex_flag)
					sscanf(tmp_str, "%06x", &sm_test_offset[chan_seq_id]);
				else
					sscanf(tmp_str, "%d", &sm_test_offset[chan_seq_id]);
				num_offset_chan++;
			}
		}
	/*	sprintf(str, "%d, %d, %d, %d,%d, %d, %d, %d,%d, %d, %d, %d,%d, %d, %d, %d", sm_test_offset[0], sm_test_offset[1], sm_test_offset[2], sm_test_offset[3], sm_test_offset[4], sm_test_offset[5], sm_test_offset[6], sm_test_offset[7], sm_test_offset[8], sm_test_offset[9], sm_test_offset[10], sm_test_offset[11], sm_test_offset[12], sm_test_offset[13], sm_test_offset[14], sm_test_offset[15]);
		MessageBox(main_window_handle, str, "message", MB_OK);*/
		for (i = 0; i < num_cam_channels; i++) {
			if (PHT_flag || noise_seq_flag) //&& strstr(cameraType, "CCID83"))
				chan_id = dark_chan_map[i / num_chan_x][i%num_chan_x] - 1;
			else
				chan_id = offset_chan_order[i];
			if (sm_test_offset[i] < 0) {
				sprintf(str, "%d, %d", sm_test_offset[i], i);
				MessageBox(main_window_handle, str, "message", MB_OK);
			}
			else {
				sprintf(command, "@OIC %d: %d", chan_id, sm_test_offset[i] + test_offset_diff);
				SM_serial_command(command);
			}
			if (hwndPB) {
				sprintf(str, "Auto Set Offsets for channel %d of %d (offset: %d)", chan_id, num_cam_channels, sm_test_offset[i]);
				SetWindowText(hwndPB, str);
				while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
			}
		}
		getCamOffsets(PHT_flag);
		if (hwndPB)
			SendMessage(hwndPB, PBM_SETPOS, (WPARAM)0, 0);
		step_cnt = 0;
		for (i = 0; i < num_cam_channels; i++) {
			if (PHT_flag || noise_seq_flag) //&& strstr(cameraType, "CCID83"))
				chan_id = dark_chan_map[i / num_chan_x][i%num_chan_x] - 1;
			else
				chan_id = offset_chan_order[i];
			if (meanOffsets[i] > 0) {
				sm_test_offset[i] = (int)(sm_test_offset[i] + (meanOffsets_sv[i] - standard_offset_mean) * test_offset_diff / (meanOffsets_sv[i] - meanOffsets[i]));
				sprintf(command, "@OIC %d: %d", chan_id, sm_test_offset[i]);
				SM_serial_command(command);
			}
			if (hwndPB) {
				sprintf(str, "Auto Set Offsets for channel %d of %d (offset: %d)", chan_id, num_cam_channels, sm_test_offset[i]);
				SetWindowText(hwndPB, str);
				while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
					SendMessage(hwndPB, PBM_STEPIT, 0, 0);
					step_cnt++;
				}
			}
		}
	}
	else {
		sprintf(command, "@OAC %d", sm_test_offset0[0]);
		SM_serial_command(command);
		Sleep(30);
		getCamOffsets(PHT_flag);
		while (!autoSetDone) {
			if (hwndPB)
				SendMessage(hwndPB, PBM_SETPOS, (WPARAM)0, 0);
			step_cnt = 0;
			for (i = 0; i < num_cam_channels; i++) {
				if (meanOffsets[i] == 0)
					sm_test_offset0[i] = 10;
				sm_test_offset_sv = sm_test_offset[i];
				sm_test_offset[i] = (sm_test_offset0[i] + sm_test_offset1[i]) / 2;
				if (sm_test_offset[i] != sm_test_offset_sv) {
					if (PHT_flag || noise_seq_flag) //&& strstr(cameraType, "CCID83"))
						chan_id = dark_chan_map[i / num_chan_x][i%num_chan_x] - 1;
					else
						chan_id = offset_chan_order[i];
					sprintf(command, "@OIC %d: %d", chan_id, sm_test_offset[i]);
					SM_serial_command(command);
				}
				if (num_cam_channels >= num_steps && hwndPB) {
					sprintf(str, "Auto Set Offsets for channel %d of %d (offset: %d)", chan_id, num_cam_channels, sm_test_offset[i]);
					SetWindowText(hwndPB, str);
					while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
						SendMessage(hwndPB, PBM_STEPIT, 0, 0);
						step_cnt++;
					}
				}
			}
			Sleep(30);
			getCamOffsets(PHT_flag);
			int max_diff = 0;
			for (i = 0; i < num_cam_channels; i++) {
				if (meanOffsets[i] > standard_offset_mean)
					sm_test_offset0[i] = sm_test_offset[i];
				else
					sm_test_offset1[i] = sm_test_offset[i];
				if ((i == 0) || sm_test_offset1[i] - sm_test_offset0[i] > max_diff)
					max_diff = sm_test_offset1[i] - sm_test_offset0[i];
			}
			if (max_diff < 2) {
				autoSetDone = TRUE;
				for (i = 0; i < num_cam_channels; i++) {
					if (PHT_flag || noise_seq_flag) //&& strstr(cameraType, "CCID83"))
						chan_id = dark_chan_map[i / num_chan_x][i%num_chan_x] - 1;
					else
						chan_id = offset_chan_order[i];
					sprintf(command, "@OIC %d: %d", chan_id, sm_test_offset0[i]);
					SM_serial_command(command);
				}
				Sleep(30);
				getCamOffsets(PHT_flag);
				memcpy(meanOffsets0, meanOffsets, sizeof(meanOffsets));
				for (i = 0; i < num_cam_channels; i++) {
					if (PHT_flag || noise_seq_flag) //&& strstr(cameraType, "CCID83"))
						chan_id = dark_chan_map[i / num_chan_x][i%num_chan_x] - 1;
					else
						chan_id = offset_chan_order[i];
					sprintf(command, "@OIC %d: %d", chan_id, sm_test_offset1[i]);
					SM_serial_command(command);
				}
				Sleep(30);
				getCamOffsets(PHT_flag);
				memcpy(meanOffsets1, meanOffsets, sizeof(meanOffsets));
				for (i = 0; i < num_cam_channels; i++) {
					if (abs((int)(meanOffsets0[i] - standard_offset_mean)) > abs((int)(meanOffsets1[i] - standard_offset_mean)))
						sm_test_offset[i] = sm_test_offset1[i];
					else
						sm_test_offset[i] = sm_test_offset0[i];
					if (PHT_flag || noise_seq_flag) //&& strstr(cameraType, "CCID83"))
						chan_id = dark_chan_map[i / num_chan_x][i%num_chan_x] - 1;
					else
						chan_id = offset_chan_order[i];
					sprintf(command, "@OIC %d: %d", chan_id, sm_test_offset[i]);
					SM_serial_command(command);
				}
			}
		}
	}
	if ((handle = _open(offsetFname, O_MODE, S_MODE)) != -1) {
		for (i = 0; i < num_cam_channels; i++)
			darkOffsetAr[curConfig * 8 * 4 + curCamGain * 4 + i] = sm_test_offset[i];
		_write(handle, (int *)darkOffsetAr, sizeof(darkOffsetAr));
		_close(handle);
	}
	if (num_cam_channels >= num_steps && hwndPB)
		DestroyWindow(hwndPB);
	else
		SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
}

bool
IsWin64()
{
	char dir64bit[MAX_PATH], sm_dir[MAX_PATH];

	strcpy(dir64bit, "C:\\Program Files (x86)");
	if (_chdir(dir64bit) == 0) {
		strcpy(sm_dir, home_dir);
		_chdir(sm_dir);
		return true;
	}
	else
		return false;
}

int
initSystem(void)
{	
	FILE *fp;
	char flagFname[MAX_PATH],command[256];

	time_t myTime = time(&myTime);
	struct tm *gtm = gmtime(&myTime);
	int gt_h = gtm->tm_hour;
	struct tm *ltm = localtime(&myTime);
	int lt_h = ltm->tm_hour;
	gt_hour_diff = lt_h - gt_h;
	fileTime = NULL;
	hotTab = OM_MEASUREVIEW;

	win64_flag = IsWin64();
	file_version = 1;
	file_bin = 0;
	num_seq_pnts = 1200;
	filePHT_flag = 0;
	trace_win_ysize = (screen_height/30)*10;
	trace_win_xsize = screen_width / 3;
	movie_scale_mode = 0;
	trace_sub_mode = 0;
	trace_sub_f1 = 0;
	trace_sub_f2 = 0;
	movie_play_mode = 0;
	movie_step = 0;
	save_sm_f_type = 0;
	save_sm_f_f1 = 0;
	save_sm_f_f2 = 200;
	save_sm_f_x0 = 0;
	save_sm_f_y0 = 0;
	save_sm_f_w = 256;
	save_sm_f_h = 256;
	NDR_subtraction_flag = 0;
	NDR_cCDS_flag = 0;
	NDR_trace_sub = 0;
	NDR_subtraction_frames = 4;
	edge_correct_flag = 0;
	edge_correct_mode = 2;
	edge_c_col1 = 1; 
	edge_c_col2 = 29;
	auto_skip_v1 = 32765;
	auto_skip_v2 = -1024;
	demo_start = 0;
	cam_num_row = 128;
	cam_num_col = 128;
	file_frame_w = 0;
	auto_scale_w = 0;
	NDR_num_f = 500;
	cam_num_row_last = 0;
	cam_num_col_last = 0;
	disp_num_row = 0;
	disp_num_col = 0;
	disp_num_col_old = 0;
	disp_num_row_old = 0;
	skip_edge_rows = 0;
	add_roi_flag = 0;
	threshold_flag = 0;
	roi_transp_level = 95; 
	roi_border_flag = 1; 
	roi_label_mode = 0;
	trace_glider_x = 15;
	numExposures = 1;
	firstDataFrame = 0;
	traceYGain = 500;
	BNC_YGain = 500;
	tracePolarity = 1;
	histoGain = 1;
	scale_max = 16384;
	scale_min = 0;
	scaleLeft = -8192;
	scaleRight = 16383;
	histo_max = scale_max; 
	histo_min = 0;
	histoLeft = scaleLeft;
	histoRight = scaleRight;
	num_traces = 0;
	thisPixColor = 1;
	memset(&FitsEnder,0,sizeof(FitsEnder));
	strcpy(experiment, "Untitled");
	strcpy(biopsy, "Untitled001.tsm");
	trial_cnt = 1;
	overwriteFlag = FALSE;
	streamDiskFlag = FALSE;
	strcpy(last_dataname, "");
	ExtTriggerFlag = FALSE;
	TXC_TriggerFlag = FALSE;
	preTrigger_ms = 0;
	num_trials = 1; 
	repeat_delay = 1000;
	save_averaged = 0;
	spike_triggered = 0;
	numBNC_chan = 4;
	BNC_ratio = 1;
	BNC_filter_flag = 1;
	max_zoom = 30.0;
	min_zoom = 0.1;
	chipgainSave = -1;
	curDispFrame = 99;
	curNFrames = 1000;
	num_av_disp = 1;
	focus_frame_interval = 300;
	frame_interval = 1.0;
	focusMode = false;
	splitterAlignMode = false;
	makeColorLUTs();
	live_factor = 1;
	pdv_chan_num = 1;
	single_img_size = (unsigned long)cam_num_col*cam_num_row*(1 + cds_lib[curConfig])*(1 + ndr_lib[curConfig])*pdv_chan_num*live_factor;
	single_img_data = (signed short int *)_aligned_malloc(single_img_size * sizeof(signed short int) * 2, 2);
	single_img_float = (float *)_aligned_malloc(single_img_size * sizeof(float), 2);
	darkImage = NULL;
	ref_image = NULL;
	dark_data = NULL;
	num_out_f = 0;
	pht_frames = 100;
	RIC_flag = 0;
	num_RIC_frames = 100;
	NDR_pos = 0;
	bad_pixel_file = 0;
	shutter_wait = 0;
	stim_delay = 0;
	acq_gain = 300;
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\bad_pix_file_flag.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		bad_pixel_file = true;
		fclose(fp);
	}

	if (!noCameraFlag) {
		readCamoffsets();
		if (!readCamConfig())
			return FALSE;
		if (TwoK_flag) {
			curDispFrame = 50;
			curNFrames = 200;
		}
		else if (strstr(cameraType, "CCID83")) {
			curDispFrame = 4;
			curNFrames = 10;
			pht_frames = 5;
		}
	}
	readPref();
	initScreen();

	char buf[256], str[256];
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\plugins.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		if (fgets(buf, 255, fp)) {
			if (strstr(buf, "XNZNG"))
				OM[SM_CARDIOVIEW].state = OM_TAB;
		}
		fclose(fp);
	}

	if (!noCameraFlag && !demoFlag) {
		MessageBox(main_window_handle,"Make sure the camera is turned on.", "message", MB_OK);
		frame_interval = 1.0/sm_lib_rates[curConfig];
		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\com_flag.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			comFlag = true;
			fclose(fp);
		}
		else
			comFlag = false;

		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\old_win_flag.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			oldWin_flag = true;
			fclose(fp);
		}
		else
			oldWin_flag = false;

		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\no_deinterleave_flag.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			fd_no_deinterleave_flag = true;
			fclose(fp);
		}
		else
			fd_no_deinterleave_flag = false;

		if (!SM_initCamera(1,0,1)) {
			MessageBox(main_window_handle, "Cannot initialize the camera.", "message", MB_OK);
			return false;
		}

		char *str_fmt = "%d.%d.%d", *p;
		char ret_c[256];
		int a, b, c;
		sprintf(command, "@JOE?");
		SM_pdv_query_command(command, ret_c, 256);
		if ((p = strstr(ret_c, "!")))
			strcpy(ret_c, p + 1);
		sscanf(ret_c, str_fmt, &a, &b, &c);
		joe_version = a * 100 + b * 10 + c;

		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\makelog_flag.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			logFlag = true;
			fclose(fp);
			char logFname[MAX_PATH];
			strcat(strcpy(logFname, home_dir), "\\SM_log.txt");
			if (FOPEN_S(fp, logFname, "r")) {
				fclose(fp);
				MessageBox(main_window_handle, "SM_log.txt in C:\\Turbo-SM\\ already exists. Click OK to overwrite it or rename it before clicking OK", "message", MB_OK);
			}
			FOPEN_S(log_fp, logFname, "w");
		}
		else
			logFlag = false;

		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\auto_dark_flag.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			auto_dark_flag = true;
			fclose(fp);
		}
		else
			auto_dark_flag = false;

		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\sp_calib_flag.txt");
		if (FOPEN_S(fp, flagFname, "r")) {
			if (fgets(buf, 255, fp))
				sscanf(buf, "%lfX", &sp_calibrate);
			fclose(fp);
		}
		else
			sp_calibrate = 0.0;

		strcpy(tmp_file_dir, home_dir);
		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\sm_tmp_file_path.txt");
		if (FOPEN_S(fp,flagFname,"r")) {
			if (fgets(buf,255,fp)) {
				sscanf(buf,"%s",tmp_file_dir);
				if (!existDir(tmp_file_dir))
					_mkdir(tmp_file_dir);
				if (!existDir(tmp_file_dir)) {
					sprintf(str, "Cannot allocate direcotry: %s. Please check sm_tmp_file_path.txt in SMSYSDAT\\", tmp_file_dir);
					MessageBox(main_window_handle, str, "message", MB_OK);
					strcpy(tmp_file_dir, home_dir);
				}
			}
			fclose(fp);
		}
	}
	else {
		OM[SM_FOCUSBOX].opt->grey = true;
		OM[SM_FOCUS].opt->grey = true;
		OM[SM_OVWRTBOX].opt->grey = true;
		OM[SM_OVERWRT].opt->grey = true;
		OM[SM_BROWSE].opt->grey = true;
		OM[SM_FNAMEUPARR].opt->grey = true;
		OM[SM_FNAMEDNARR].opt->grey = true;
		setBGrey_Live(true);
	}
	OM[OM_OBJUPARR].opt->grey = true;
	OM[OM_OBJDNARR].opt->grey = true;
	OM[OM_FLDLTARR].opt->grey = true;
	OM[OM_FLDRTARR].opt->grey = true;
	OM[OM_FLDUPARR].opt->grey = true;
	OM[OM_FLDDNARR].opt->grey = true;
	if (!TwoK_flag) {
		OM[SM_FOCUSBOX].state = OM_NULL;
		OM[SM_FOCUS].state = OM_NULL;
	}
	PlayFlag = false;
	setToolBGrey();
	selectPixMode = DRAG_MODE;
	dataExist = false;
	dataFile = false;
	BNC_exist = false;
	darkSubFlag = false;
	output_dksub_flag = true;
	f_subtract_mode = 0;
	ref_frame1 = 0;
	ref_frame2 = 0;
	frame_d_mode = 0;
	trace_layout = 0;
	spatial_bin = 1;
	showHistFlag = true;
	lockHistFlag = false;
	autoScaleFlag = true;
	setChckBox(SM_AUTOSBOX);
	if (!demoFlag && !noCameraFlag) {
		SM_stopAD();
		strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\seq_debug_flag.txt");
		if (!FOPEN_S(fp,flagFname,"r")) {
			for (int i = 0; i < 8; i++) {
				if (NDR_code_lib[i]) {
					sprintf(command,"@SEQ 0");
					SM_serial_command(command);
					sprintf(command,"@FTC");
					SM_serial_command(command);
					Sleep(500);
					SM_initCamera(1,1,1);
					break;
				}
			}
		}
		else
			fclose(fp);
		sprintf(command,"@SEQ 1");	//redundancy to make sure the camera is running
		SM_serial_command(command);
	}
	else {
		if (darkImage != NULL)
			_aligned_free(darkImage);
		darkImage = (signed short int *)_aligned_malloc(cam_num_col*cam_num_row * sizeof(signed short int), 2);
		resetZoom(1);
	}

	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\splitter_flag.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		splitterMode = true;
		fclose(fp);
	}
	else
		splitterMode = false;

	return TRUE;
}

void exitSystem(void)
{
	if (!noCameraFlag && !demoFlag)
		SM_pdv_close();
	freeAll_ar_pts();
}

int getOMSize(void)
{
	int i;
	for (i = 1; i < sizeof(OM)/sizeof(OM[0]); ++i)
		if (OM[i].id == 0) break;
	return i;
}

unsigned int
makeDarker(unsigned int val,int factor)
{
	int i;
	union {
		unsigned int newvalint;
		unsigned char newbyte[4];
	} newval;

	if (!factor)
		return val;
	newval.newvalint = val;
	for (i = 0; i < 4; ++i)
		newval.newbyte[i] = newval.newbyte[i]>>factor;
	return newval.newvalint;
}

void enableTraceTools(int flag)
{
	if (flag) {
		OM[SM_TRCEXP].state = OM_SLIDER;
		OM[SM_TRCEXP_UP].state = OM_ARROW;
		OM[SM_TRCEXP_DN].state = OM_ARROW;
		OM[SM_LOWPASS].state = OM_SLIDER;
		OM[SM_LOWPASS_UP].state = OM_ARROW;
		OM[SM_LOWPASS_DN].state = OM_ARROW;
		OM[SM_HIGHPASS].state = OM_SLIDER;
		OM[SM_HIGHPASS_UP].state = OM_ARROW;
		OM[SM_HIGHPASS_DN].state = OM_ARROW;
		OM[SM_TRCSTART].state = OM_SLIDER;
		OM[SM_TRCSTT_UP].state = OM_ARROW;
		OM[SM_TRCSTT_DN].state = OM_ARROW;
		OM[SM_REFSTART].state = OM_SLIDER;
		OM[SM_REFSTT_UP].state = OM_ARROW;
		OM[SM_REFSTT_DN].state = OM_ARROW;
	}
	else {
		OM[SM_TRCEXP].state = OM_NULL;
		OM[SM_TRCEXP_UP].state = OM_NULL;
		OM[SM_TRCEXP_DN].state = OM_NULL;
		OM[SM_LOWPASS].state = OM_NULL;
		OM[SM_LOWPASS_UP].state = OM_NULL;
		OM[SM_LOWPASS_DN].state = OM_NULL;
		OM[SM_HIGHPASS].state = OM_NULL;
		OM[SM_HIGHPASS_UP].state = OM_NULL;
		OM[SM_HIGHPASS_DN].state = OM_NULL;
		OM[SM_TRCSTART].state = OM_NULL;
		OM[SM_TRCSTT_UP].state = OM_NULL;
		OM[SM_TRCSTT_DN].state = OM_NULL;
		OM[SM_REFSTART].state = OM_NULL;
		OM[SM_REFSTT_UP].state = OM_NULL;
		OM[SM_REFSTT_DN].state = OM_NULL;
	}
}

void initScreen(void)
{
	SIZE dxdy;
	int i,maxX,hgt = 0, dk_factor = 0;
	FILE *fp;
    char buf[256], str[256];

	char flagFname[MAX_PATH];
	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\menu_dim_flag.txt");
	if (FOPEN_S(fp,flagFname,"r")) {
	    if (fgets(buf,255,fp))
			sscanf(buf,"%d",&dk_factor);
		fclose(fp);
	}
	
	buttonWidth = GetSystemMetrics(SM_CXVSCROLL);
	buttonHeight = GetSystemMetrics(SM_CYVSCROLL);

	colorDarkShadow  = makeDarker(GetSysColor(COLOR_3DDKSHADOW), dk_factor);
	colorLightShadow = makeDarker(GetSysColor(COLOR_3DSHADOW), dk_factor);
	colorRegular     = makeDarker(GetSysColor(COLOR_3DFACE), dk_factor);
	colorTab         = makeDarker(GetSysColor(COLOR_3DFACE), dk_factor);
	colorLight       = makeDarker(GetSysColor(COLOR_3DLIGHT), dk_factor);
	colorWindow      = makeDarker(GetSysColor(COLOR_3DFACE), dk_factor);
	colorCheckBox    = makeDarker(GetSysColor(COLOR_BTNFACE), dk_factor);
	colorArrow       = makeDarker(GetSysColor(COLOR_BTNFACE), dk_factor);
	colorGrayText	 = makeDarker(GetSysColor(COLOR_GRAYTEXT), dk_factor);
	colorText	     = makeDarker(GetSysColor(COLOR_MENUTEXT), dk_factor);
	colorInfoBkgrnd  = makeDarker(GetSysColor(COLOR_INFOBK), dk_factor);

// default settings
	for (i = OM_MAIN; i <= OM_OPTQUIT; i++)
		memcpy(&OM[i], &OM_BASIC[i], sizeof(OM[0]));
	
//****
	if (screen_height > 750) 
		OM[OM_MAIN].dx = screen_width - 260;
	else {
		if (screen_width > 1.6*screen_height) 
			OM[OM_MAIN].dx = (int)(1.8*OM[OM_MAIN].dy);
	}
	OM[OM_MAIN].dy = screen_height - OM[OM_MAIN].y - OM[OM_MEASUREVIEW].dy - OM[OM_MAIN].x;
	if (!window_layout) {
		OM[OM_MEASUREWIN].dx = OM[OM_MAIN].dx;
		OM[OM_MEASUREWIN].dy = OM[OM_MAIN].dy - trace_win_ysize;
	}
	else {
		OM[OM_MEASUREWIN].dx = (OM[OM_MAIN].dx - trace_win_xsize);
		OM[OM_MEASUREWIN].dy = OM[OM_MAIN].dy;
	}
	if (whiteBackgrnd)
		*OM[OM_MEASUREWIN].color = WHITE;

	OM[OM_MENUBOX].x = 2*OM[OM_MAIN].x + OM[OM_MAIN].dx;
	OM[OM_MENUBOX].dx = screen_width - OM[OM_MENUBOX].x - OM[OM_MAIN].x;
	OM[OM_MENUBOX].y = OM[OM_MAIN].y;
	OM[OM_MENUBOX].dy = screen_height - OM[OM_MENUBOX].y - OM[OM_MEASUREVIEW].dy - OM[OM_MAIN].x;
	maxX = OM[OM_MENUBOX].x + OM[OM_MENUBOX].dx - 5;

	OM[OM_OBJBOX].x = OM[OM_MENUBOX].x + 40;
	OM[OM_OBJBOX].y = OM[OM_MENUBOX].y + 25;
	OM[OM_OBJBOX].dx = 50;
	OM[OM_OBJBOX].dy = 30;

	OM[OM_OBJUPARR].dx = buttonWidth-1;
	OM[OM_OBJUPARR].x = OM[OM_OBJBOX].x;
	OM[OM_OBJUPARR].dy = buttonHeight;
	OM[OM_OBJUPARR].y = OM[OM_OBJBOX].y;

	OM[OM_OBJDNARR].dx = buttonWidth-1;
	OM[OM_OBJDNARR].x = OM[OM_OBJBOX].x + OM[OM_OBJBOX].dx/2;
	OM[OM_OBJDNARR].dy = buttonHeight;
	OM[OM_OBJDNARR].y = OM[OM_OBJBOX].y;

	OM[OM_NAVIBOX].x = OM[OM_MENUBOX].x + 120;
	OM[OM_NAVIBOX].y = OM[OM_MENUBOX].y + 10;
	OM[OM_NAVIBOX].dx = 32;
	OM[OM_NAVIBOX].dy = 32;

	OM[OM_FLDLTARR].dx = buttonWidth;
	OM[OM_FLDLTARR].x = OM[OM_NAVIBOX].x;
	OM[OM_FLDLTARR].dy = buttonHeight;
	OM[OM_FLDLTARR].y = OM[OM_NAVIBOX].y + OM[OM_NAVIBOX].dy/2;

	OM[OM_FLDRTARR].dx = buttonWidth;
	OM[OM_FLDRTARR].x = OM[OM_NAVIBOX].x + OM[OM_NAVIBOX].dx;
	OM[OM_FLDRTARR].dy = buttonHeight;
	OM[OM_FLDRTARR].y = OM[OM_NAVIBOX].y + OM[OM_NAVIBOX].dy/2;

	OM[OM_FLDUPARR].dx = buttonWidth;
	OM[OM_FLDUPARR].x = OM[OM_NAVIBOX].x + OM[OM_NAVIBOX].dx/2;
	OM[OM_FLDUPARR].dy = buttonHeight;
	OM[OM_FLDUPARR].y = OM[OM_NAVIBOX].y;

	OM[OM_FLDDNARR].dx = buttonWidth;
	OM[OM_FLDDNARR].x = OM[OM_NAVIBOX].x + OM[OM_NAVIBOX].dx/2;
	OM[OM_FLDDNARR].dy = buttonHeight;
	OM[OM_FLDDNARR].y = OM[OM_NAVIBOX].y + OM[OM_NAVIBOX].dy;

	for (i = OM_MEASUREVIEW; i <= OM_HELPVIEW; ++i) {
		OM[i].x = OM[OM_MEASUREVIEW].x + (i-OM_MEASUREVIEW)*(OM[OM_MEASUREVIEW].dx+5);
		OM[i].y = OM[OM_MENUBOX].y + OM[OM_MENUBOX].dy + 5;
	}

	if (!window_layout) {
		OM[SM_DATAREAD].x = OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx - 120;
		OM[SM_DATAREAD].y = OM[OM_MEASUREWIN].y + 55;
	}
	else {
		OM[SM_DATAREAD].x = OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx - 120;
		OM[SM_DATAREAD].y = OM[OM_MEASUREWIN].y + OM[OM_MEASUREWIN].dy - 65;
	}
	OM[SM_DATAREAD].dx = 50;
	OM[SM_DATAREAD].dy = 16;
	OM[SM_PREVREAD].dx = 18;
	OM[SM_PREVREAD].x = OM[SM_DATAREAD].x;
	OM[SM_PREVREAD].dy = OM[SM_DATAREAD].dy;
	OM[SM_PREVREAD].y = OM[SM_DATAREAD].y;
	OM[SM_NEXTREAD].dx = 18;
	OM[SM_NEXTREAD].x = OM[SM_PREVREAD].x + OM[SM_PREVREAD].dx + 10;
	OM[SM_NEXTREAD].dy = OM[SM_DATAREAD].dy;
	OM[SM_NEXTREAD].y = OM[SM_DATAREAD].y;
	for (i = SM_DATAREAD; i <= SM_PREVREAD; i++)
		OM[i].color = BLACK;
	OM[SM_FIRSTFRM].x = OM[SM_NEXTREAD].x + OM[SM_NEXTREAD].dx + 10;
	OM[SM_FIRSTFRM].dy = OM[SM_DATAREAD].dy;
	OM[SM_FIRSTFRM].dx = OM[SM_FIRSTFRM].dy;
	OM[SM_FIRSTFRM].y = OM[SM_DATAREAD].y;

	OM[SM_IMGSCBASE].dx = 360;
	OM[SM_IMGSCBASE].dy = 40;
	if (!window_layout)
		OM[SM_IMGSCBASE].x = OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx - 400;
	else
		OM[SM_IMGSCBASE].x = OM[OM_MEASUREWIN].x + 20;
	OM[SM_IMGSCBASE].y = (int)(OM[OM_MEASUREWIN].y + OM[OM_MEASUREWIN].dy*0.92 - OM[SM_IMGSCBASE].dy);

	if (screen_height > 1200)
		arrow_w = 14;
	else
		arrow_w = 12;
	for (i = SM_IMGSCLUP; i <= SM_IMGSCRDN; i++) {
		OM[i].dx = arrow_w;
		OM[i].dy = arrow_w;
	}
	OM[SM_IMGSCALE].x = OM[SM_IMGSCBASE].x + OM[SM_IMGSCLUP].dx + arrow_w;
	OM[SM_IMGSCALE].y = OM[SM_IMGSCBASE].y + 8;
	OM[SM_IMGSCALE].dx = OM[SM_IMGSCBASE].dx - OM[SM_IMGSCLUP].dx*2 - arrow_w*2;
	OM[SM_IMGSCALE].dy = arrow_w;
	int y = OM[SM_IMGSCALE].y + OM[SM_IMGSCALE].dy/2;
	OM[SM_IMGSCLUP].x = OM[SM_IMGSCBASE].x;
	OM[SM_IMGSCLUP].y = y - arrow_w*7/6;
	OM[SM_IMGSCLDN].x = OM[SM_IMGSCLUP].x;
	OM[SM_IMGSCLDN].y = y + arrow_w/6;
	OM[SM_IMGSCRUP].x = OM[SM_IMGSCALE].x + OM[SM_IMGSCALE].dx + arrow_w;
	OM[SM_IMGSCRUP].y = y - arrow_w*7/6;
	OM[SM_IMGSCRDN].x = OM[SM_IMGSCRUP].x;
	OM[SM_IMGSCRDN].y = y + arrow_w /6;
	for (i = SM_IMGSCBASE; i <= SM_IMGSCRDN; i++)
		OM[i].color = OM[OM_MEASUREWIN].color;

	OM[SP_FLTPNL].x = OM[SM_IMGSCBASE].x;
	OM[SP_FLTPNL].dx = OM[SM_IMGSCBASE].dx + 25; // OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx - OM[SP_FLTPNL].x;
	OM[SP_FLTPNL].dy = 48;
	OM[SP_FLTPNL].y = (int)(OM[SM_IMGSCBASE].y - OM[SP_FLTPNL].dy - OM[OM_MEASUREWIN].dy*0.004);
	OM[SP_FLTPNL].color = OM[OM_MEASUREWIN].color;
	y = OM[SP_FLTPNL].y + OM[SP_FLTPNL].dy / 2;
	for (i = SP_LPTYPE_UP; i <= SP_LP_DN; i++) {
		OM[i].dx = arrow_w;
		OM[i].dy = arrow_w;
		if ((i - SP_LPTYPE_UP) % 2) {
			OM[i].x = OM[SP_FLTPNL].x + 25 * (int)((i - SP_LPTYPE_UP) / 2);
			OM[i].y = y + arrow_w / 6;
		}
		else {
			OM[i].x = OM[SP_FLTPNL].x + 25 * (i - SP_LPTYPE_UP) / 2;
			OM[i].y = y - arrow_w*7 / 6;
		}
		OM[i].color = OM[OM_MEASUREWIN].color;
	}
	OM[SP_LP_LABEL].x = OM[SP_LP_DN].x + 20;
	OM[SP_LP_LABEL].dx = OM[SP_FLTPNL].dx/2 - 50;
	OM[SP_LP_LABEL].dy = 20;
	OM[SP_LP_LABEL].y = OM[SP_LPTYPE_UP].y + arrow_w/2;
	OM[SP_LP_LABEL].color = OM[OM_MEASUREWIN].color;
	OM[SP_LP_LABEL].title = "No LPass Filter";
	OM[SP_LP_LABEL].title_x = OM[SP_LP_LABEL].x + 1;
	OM[SP_LP_LABEL].title_y = OM[SP_LP_LABEL].y + 1;
	for (i = SP_HPTYPE_UP; i <= SP_HP_DN; i++) {
		OM[i].dx = arrow_w;
		OM[i].dy = arrow_w;
		if ((i - SP_HPTYPE_UP) % 2) {
			OM[i].x = OM[SP_FLTPNL].x + 25 * (int)((i - SP_HPTYPE_UP) / 2) + OM[SP_FLTPNL].dx/2;
			OM[i].y = y + arrow_w / 6;
		}
		else {
			OM[i].x = OM[SP_FLTPNL].x + 25 * (i - SP_HPTYPE_UP) / 2 + OM[SP_FLTPNL].dx/2;
			OM[i].y = y - arrow_w*7 / 6;
		}
		OM[i].color = OM[OM_MEASUREWIN].color;
	}
	OM[SP_HP_LABEL].x = OM[SP_HP_DN].x + 20;
	OM[SP_HP_LABEL].dx = OM[SP_FLTPNL].dx/2 - 50;
	OM[SP_HP_LABEL].dy = 20;
	OM[SP_HP_LABEL].y = OM[SP_HPTYPE_UP].y + arrow_w / 2;
	OM[SP_HP_LABEL].color = OM[OM_MEASUREWIN].color;
	OM[SP_HP_LABEL].title = "No HPass Filter";
	OM[SP_HP_LABEL].title_x = OM[SP_HP_LABEL].x + 1;
	OM[SP_HP_LABEL].title_y = OM[SP_HP_LABEL].y + 1;

	OM[SM_TOOLPNL].x = OM[SM_IMGSCBASE].x;
	OM[SM_TOOLPNL].dx = OM[SP_FLTPNL].dx;
	OM[SM_TOOLPNL].dy = 40;
	OM[SM_TOOLPNL].y = (int)(OM[SM_IMGSCBASE].y + OM[OM_MEASUREWIN].dy*0.06);
	OM[SM_TPNLTEXT].dx = OM[SM_TOOLPNL].dx;
	OM[SM_TPNLTEXT].x = OM[SM_TOOLPNL].x;
	OM[SM_TPNLTEXT].dy = 15;
	OM[SM_TPNLTEXT].y = OM[SM_TOOLPNL].y;

	int dy = 15;
	y = OM[SM_TOOLPNL].y + OM[SM_TPNLTEXT].dy + 1;
	OM[SM_PREVFRAME].dx = 18;
	OM[SM_PREVFRAME].x = OM[SM_TOOLPNL].x;
	OM[SM_PREVFRAME].dy = dy;
	OM[SM_PREVFRAME].y = y;
	OM[SM_PLAY].dx = 15;
	OM[SM_PLAY].x = OM[SM_PREVFRAME].x + OM[SM_PREVFRAME].dx + 15;
	OM[SM_PLAY].dy = dy;
	OM[SM_PLAY].y = y;
	OM[SM_PAUSE].dx = 12;
	OM[SM_PAUSE].x = OM[SM_PLAY].x + OM[SM_PLAY].dx + 15;
	OM[SM_PAUSE].dy = dy;
	OM[SM_PAUSE].y = y;
	OM[SM_NEXTFRAME].dx = 18;
	OM[SM_NEXTFRAME].x = OM[SM_PAUSE].x + OM[SM_PAUSE].dx + 15;
	OM[SM_NEXTFRAME].dy = dy;
	OM[SM_NEXTFRAME].y = y;

	OM[SM_THRESHOLD].dx = 16;
	OM[SM_THRESHOLD].x = OM[SM_NEXTFRAME].x + OM[SM_NEXTFRAME].dx + 40;
	OM[SM_THRESHOLD].dy = OM[SM_THRESHOLD].dx;
	OM[SM_THRESHOLD].y = y;
	OM[SM_ADD_CUR_ROI].dx = 16;
	OM[SM_ADD_CUR_ROI].x = OM[SM_THRESHOLD].x + OM[SM_THRESHOLD].dx + 10;
	OM[SM_ADD_CUR_ROI].dy = OM[SM_ADD_CUR_ROI].dx;
	OM[SM_ADD_CUR_ROI].y = y;
	OM[SM_REMOVE].dx = 16;
	OM[SM_REMOVE].x = OM[SM_ADD_CUR_ROI].x + OM[SM_ADD_CUR_ROI].dx + 10;
	OM[SM_REMOVE].dy = OM[SM_REMOVE].dx;
	OM[SM_REMOVE].y = y;
	OM[SM_DRAW].dx = 16;
	OM[SM_DRAW].x = OM[SM_REMOVE].x + OM[SM_REMOVE].dx + 10;
	OM[SM_DRAW].dy = OM[SM_DRAW].dx;
	OM[SM_DRAW].y = y;
	OM[SM_DRAGBOX].dx = 16;
	OM[SM_DRAGBOX].x = OM[SM_DRAW].x + OM[SM_DRAW].dx + 10;
	OM[SM_DRAGBOX].dy = OM[SM_DRAGBOX].dx;
	OM[SM_DRAGBOX].y = y;
	OM[SM_KERNEL].dx = 16;
	OM[SM_KERNEL].x = OM[SM_DRAGBOX].x + OM[SM_DRAGBOX].dx + 10;
	OM[SM_KERNEL].dy = OM[SM_KERNEL].dx;
	OM[SM_KERNEL].y = y;
	OM[SM_KSIZEUP].dx = 12;
	OM[SM_KSIZEUP].x = OM[SM_KERNEL].x + OM[SM_KERNEL].dx + 5;
	OM[SM_KSIZEUP].dy = OM[SM_KSIZEUP].dx;
	OM[SM_KSIZEUP].y = y - 6;
	OM[SM_KSIZEDN].dx = OM[SM_KSIZEUP].dx;
	OM[SM_KSIZEDN].x = OM[SM_KSIZEUP].x;
	OM[SM_KSIZEDN].dy = OM[SM_KSIZEUP].dx;
	OM[SM_KSIZEDN].y = OM[SM_KSIZEUP].y + OM[SM_KSIZEUP].dy + 4;
	OM[SM_TPOLARITY].dx = 13;
	OM[SM_TPOLARITY].x = OM[SM_TOOLPNL].x + OM[SM_TOOLPNL].dx - 55;
	OM[SM_TPOLARITY].dy = OM[SM_TPOLARITY].dx;
	OM[SM_TPOLARITY].y = y + 2;
	for (i = SM_TOOLPNL; i <= SM_KSIZEDN; i++)
		OM[i].color = OM[OM_MEASUREWIN].color;

	// BNC buttons
	for (i = SM_BNCGAIN; i <= SM_BNCGNDN; i++) {
		OM[i].dx = arrow_w;
		OM[i].dy = arrow_w;
		OM[i].color = &colorBlack;
	}
	if (!window_layout) {
		OM[SM_BNCPNL].dx = 30;
		OM[SM_BNCPNL].x = OM[SM_TOOLPNL].x - OM[SM_BNCPNL].dx - 15;
		OM[SM_BNCPNL].dy = 300;
		OM[SM_BNCPNL].y = OM[OM_MEASUREWIN].y + OM[OM_MEASUREWIN].dy / 4;
		OM[SM_BNCGAIN].x = OM[SM_BNCPNL].x;
		OM[SM_BNCGAIN].y = OM[SM_BNCPNL].y + arrow_w / 2;
		OM[SM_BNCGAIN].dy = 140 - arrow_w*2 - 5;
		y = OM[SM_BNCGAIN].y + OM[SM_BNCGAIN].dy + arrow_w + 5;
		OM[SM_BNCGNUP].x = OM[SM_BNCGAIN].x;
		if (!OM[SM_BNCGAIN].attr)
			OM[SM_BNCGAIN].attr = 1000 - BNC_YGain;
	}
	else {
		OM[SM_BNCPNL].dx = 300;
		OM[SM_BNCPNL].x = OM[OM_MEASUREWIN].x + (OM[OM_MEASUREWIN].dx - 300)/3 - 50;
		OM[SM_BNCPNL].dy = 30;
		OM[SM_BNCPNL].y = OM[SP_FLTPNL].y - 50;
		OM[SM_BNCGAIN].x = (int)(OM[SM_BNCPNL].x + arrow_w*1.5);
		OM[SM_BNCGAIN].y = OM[SM_BNCPNL].y;
		OM[SM_BNCGAIN].dx = 140 - arrow_w;
		y = OM[SM_BNCGAIN].y + OM[SM_BNCGAIN].dy/2;
		OM[SM_BNCGNUP].x = OM[SM_BNCPNL].x;
		if (!OM[SM_BNCGAIN].attr)
			OM[SM_BNCGAIN].attr = BNC_YGain;
	}
	OM[SM_BNCGNDN].x = OM[SM_BNCGNUP].x;
	OM[SM_BNCGNUP].y = y - arrow_w * 7 / 6;
	OM[SM_BNCGNDN].y = y + arrow_w / 6;
	OM[SM_BNCGNUP].attr = OM_UPARR;
	OM[SM_BNCGNDN].attr = OM_DNARR;
	if (BNC_exist) {
		OM[SM_BNCPNL].state = OM_DISPLAY;
		OM[SM_BNCGAIN].state = OM_SLIDER;
		OM[SM_BNCGNUP].state = OM_ARROW;
		OM[SM_BNCGNDN].state = OM_ARROW;
	}
	else {
		OM[SM_BNCPNL].state = OM_NULL;
		OM[SM_BNCGAIN].state = OM_NULL;
		OM[SM_BNCGNUP].state = OM_NULL;
		OM[SM_BNCGNDN].state = OM_NULL;
	}
	OM[SM_BNCPNL].color = OM[OM_MEASUREWIN].color;
	for (i = SM_BNC1; i <= SM_BNC_FLTR; ++i) {
		OM[i].dx = 20;
		OM[i].dy = 20;
		if (!window_layout) {
			OM[i].x = OM[SM_BNCPNL].x;
			OM[i].y = OM[SM_BNCPNL].y + 170 + 20 * (i - SM_BNC1);
		}
		else {
			OM[i].x = OM[SM_BNCPNL].x + 170 + 20 * (i - SM_BNC1);
			OM[i].y = OM[SM_BNCPNL].y;
		}
		OM[i].state = OM_NULL;
		OM[i].color = OM[OM_MEASUREWIN].color;
	}

	// TRACEWIN
	if (!window_layout) {
		OM[SM_TRACEWIN].x = OM[OM_MEASUREWIN].x;
		OM[SM_TRACEWIN].y = OM[OM_MEASUREWIN].y + OM[OM_MEASUREWIN].dy + 2;
		OM[SM_TRACEWIN].dx = OM[OM_MEASUREWIN].dx;
		OM[SM_TRACEWIN].dy = OM[OM_MAIN].dy - OM[OM_MEASUREWIN].dy - 2;
	}
	else {
		OM[SM_TRACEWIN].x = OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx + 2;
		OM[SM_TRACEWIN].y = OM[OM_MEASUREWIN].y;
		OM[SM_TRACEWIN].dx = OM[OM_MAIN].dx - OM[OM_MEASUREWIN].dx - 2;
		OM[SM_TRACEWIN].dy = OM[OM_MAIN].dy;
	}
	OM[SM_TRACEWIN].color = &colorBlack;

	OM[SM_QUESTION].x = OM[SM_TRACEWIN].x + OM[SM_TRACEWIN].dx - 38;
	OM[SM_QUESTION].y = OM[SM_TRACEWIN].y + 10;
	OM[SM_QUESTION].dx = 18;
	OM[SM_QUESTION].dy = 18;

	for (i = SM_TRCSTART; i <= SM_TRCSTT_DN; i++) {
		OM[i].dx = arrow_w;
		OM[i].dy = arrow_w;
		OM[i].color = &colorBlack;
	}
	OM[SM_TRCSTT_UP].x = OM[SM_TRACEWIN].x + 10;
	OM[SM_TRCSTT_DN].x = OM[SM_TRCSTT_UP].x;
	OM[SM_TRCSTART].x = OM[SM_TRCSTT_UP].x + OM[SM_TRCSTT_UP].dx + 76;
	if (!window_layout)
		OM[SM_TRCSTART].dx = OM[SM_TRACEWIN].dx / 2 - (OM[SM_TRCSTART].x - OM[SM_TRACEWIN].x) * 2;
	else
		OM[SM_TRCSTART].dx = OM[SM_TRACEWIN].dx - (OM[SM_TRCSTART].x - OM[SM_TRACEWIN].x) * 2;
	OM[SM_TRCSTART].y = OM[SM_TRACEWIN].y + OM[SM_TRACEWIN].dy - OM[SM_TRCSTART].dy - arrow_w / 2 - 3;
	y = OM[SM_TRCSTART].y + OM[SM_TRCSTART].dy / 2;
	OM[SM_TRCSTT_UP].y = y - arrow_w*7/6;
	OM[SM_TRCSTT_DN].y = y + arrow_w / 6;
	OM[SM_TRCSTT_UP].attr = OM_UPARR;
	OM[SM_TRCSTT_DN].attr = OM_DNARR;

	for (i = SM_REFSTART; i <= SM_REFSTT_DN; i++) {
		OM[i].dx = arrow_w;
		OM[i].dy = arrow_w;
		OM[i].color = &colorBlack;
	}
	if (!window_layout) {
		OM[SM_REFSTT_UP].x = OM[SM_TRACEWIN].x + OM[SM_TRACEWIN].dx / 2 + 10;
		OM[SM_REFSTART].y = OM[SM_TRCSTART].y;
	}
	else {
		OM[SM_REFSTT_UP].x = OM[SM_TRACEWIN].x + 10;
		OM[SM_REFSTART].y = OM[SM_TRCSTART].y - 25;
	}
	OM[SM_REFSTT_DN].x = OM[SM_REFSTT_UP].x;
	OM[SM_REFSTART].x = OM[SM_REFSTT_UP].x + OM[SM_REFSTT_UP].dx + 76;
	OM[SM_REFSTART].dx = OM[SM_TRCSTART].dx;
	y = OM[SM_REFSTART].y + OM[SM_REFSTART].dy / 2;
	OM[SM_REFSTT_UP].y = y - arrow_w * 7 / 6;
	OM[SM_REFSTT_DN].y = y + arrow_w / 6;
	OM[SM_REFSTT_UP].attr = OM_UPARR;
	OM[SM_REFSTT_DN].attr = OM_DNARR;

	if (!window_layout) {
		OM[SM_TRCEXP].dx = 80;
		if (trace_win_ysize < 80)
			enableTraceTools(0);
		else
			enableTraceTools(1);
	}
	else
		OM[SM_TRCEXP].dx = 120;
	OM[SM_TRCEXP].dy = arrow_w;
	OM[SM_LOWPASS].dx = OM[SM_TRCEXP].dx;
	OM[SM_LOWPASS].dy = OM[SM_TRCEXP].dy;
	OM[SM_HIGHPASS].dx = OM[SM_TRCEXP].dx;
	OM[SM_HIGHPASS].dy = OM[SM_TRCEXP].dy;
	if (!window_layout) {
		OM[SM_TRCEXP].x = OM[SM_TRACEWIN].x + OM[SM_TRACEWIN].dx / 2 - OM[SM_TRCEXP].dx / 2;
		OM[SM_TRCEXP].y = (int)(OM[SM_TRCSTART].y - OM[SM_TRCEXP].dy*arrow_w*0.18);
		OM[SM_LOWPASS].x = OM[SM_TRCEXP].x - OM[SM_LOWPASS].dx - 200;
		OM[SM_LOWPASS].y = OM[SM_TRCEXP].y;
		OM[SM_HIGHPASS].x = OM[SM_TRCEXP].x + OM[SM_TRCEXP].dx + 200;
		OM[SM_HIGHPASS].y = OM[SM_TRCEXP].y;
	}
	else {
		OM[SM_TRCEXP].x = OM[SM_TRACEWIN].x + (OM[SM_TRACEWIN].dx - OM[SM_TRCEXP].dx )/2 + 30;
		OM[SM_TRCEXP].y = OM[SM_TRACEWIN].y + OM[SM_TRACEWIN].dy - 120;
		OM[SM_LOWPASS].x = OM[SM_TRCEXP].x;
		OM[SM_LOWPASS].y = OM[SM_TRCEXP].y + 25;
		OM[SM_HIGHPASS].x = OM[SM_TRCEXP].x;
		OM[SM_HIGHPASS].y = OM[SM_LOWPASS].y + 25;
	}

	y = OM[SM_TRCEXP].y + OM[SM_TRCEXP].dy / 2;
	OM[SM_TRCEXP].color = &colorBlack;
	OM[SM_TRCEXP_UP].dx = arrow_w;
	OM[SM_TRCEXP_UP].x = OM[SM_TRCEXP].x - 60 - arrow_w;
	OM[SM_TRCEXP_UP].dy = arrow_w;
	OM[SM_TRCEXP_UP].y = y - arrow_w * 7 / 6;
	OM[SM_TRCEXP_UP].color = OM[SM_TRACEWIN].color;
	OM[SM_TRCEXP_UP].attr = OM_UPARR;
	OM[SM_TRCEXP_DN].dx = OM[SM_TRCEXP_UP].dx;
	OM[SM_TRCEXP_DN].x = OM[SM_TRCEXP_UP].x;
	OM[SM_TRCEXP_DN].dy = OM[SM_TRCEXP_UP].dy;
	OM[SM_TRCEXP_DN].y = y + arrow_w / 6;
	OM[SM_TRCEXP_DN].color = OM[SM_TRACEWIN].color;
	OM[SM_TRCEXP_DN].attr = OM_DNARR;

	y = OM[SM_LOWPASS].y + OM[SM_LOWPASS].dy / 2;
	OM[SM_LOWPASS].color = &colorBlack;
	OM[SM_LOWPASS_UP].dx = arrow_w;
	OM[SM_LOWPASS_UP].x = OM[SM_LOWPASS].x - 82 - arrow_w;
	OM[SM_LOWPASS_UP].dy = arrow_w;
	OM[SM_LOWPASS_UP].y = y - arrow_w * 7 / 6;
	OM[SM_LOWPASS_UP].color = OM[SM_TRACEWIN].color;
	OM[SM_LOWPASS_UP].attr = OM_UPARR;
	OM[SM_LOWPASS_DN].dx = OM[SM_LOWPASS_UP].dx;
	OM[SM_LOWPASS_DN].x = OM[SM_LOWPASS_UP].x;
	OM[SM_LOWPASS_DN].dy = OM[SM_TRCEXP_UP].dy;
	OM[SM_LOWPASS_DN].y = y + arrow_w / 6;
	OM[SM_LOWPASS_DN].color = OM[SM_TRACEWIN].color;
	OM[SM_LOWPASS_DN].attr = OM_DNARR;

	y = OM[SM_HIGHPASS].y + OM[SM_HIGHPASS].dy / 2;
	OM[SM_HIGHPASS].color = &colorBlack;
	OM[SM_HIGHPASS_UP].dx = arrow_w;
	OM[SM_HIGHPASS_UP].x = OM[SM_HIGHPASS].x - 90 - arrow_w;
	OM[SM_HIGHPASS_UP].dy = arrow_w;
	OM[SM_HIGHPASS_UP].y = y - arrow_w * 7 / 6;
	OM[SM_HIGHPASS_UP].color = OM[SM_TRACEWIN].color;
	OM[SM_HIGHPASS_UP].attr = OM_UPARR;
	OM[SM_HIGHPASS_DN].dx = OM[SM_HIGHPASS_UP].dx;
	OM[SM_HIGHPASS_DN].x = OM[SM_HIGHPASS_UP].x;
	OM[SM_HIGHPASS_DN].dy = OM[SM_HIGHPASS_UP].dy;
	OM[SM_HIGHPASS_DN].y = y + arrow_w / 6;
	OM[SM_HIGHPASS_DN].color = OM[SM_TRACEWIN].color;
	OM[SM_HIGHPASS_DN].attr = OM_DNARR;

	for (i = SM_TRCGAIN; i <= SM_TRCGNDN; i++) {
		OM[i].dx = arrow_w;
		OM[i].dy = arrow_w;
		OM[i].color = &colorBlack;
	}
	OM[SM_TRCGAIN].x = OM[SM_TRACEWIN].x + OM[SM_TRACEWIN].dx - OM[SM_TRCGAIN].dx - 3;
	OM[SM_TRCGAIN].y = OM[SM_TRACEWIN].y + trace_bdr;
	OM[SM_TRCGAIN].dy = OM[SM_TRACEWIN].dy - trace_bdr*2 - OM[SM_TRCGNDN].dy*2 - 5;
	if (!OM[SM_TRCGAIN].attr)
		OM[SM_TRCGAIN].attr = 1000 - traceYGain;
	y = OM[SM_TRCGAIN].y + OM[SM_TRCGAIN].dy + OM[SM_TRCGNUP].dy + 5;
	OM[SM_TRCGNUP].x = OM[SM_TRCGAIN].x;
	OM[SM_TRCGNDN].x = OM[SM_TRCGAIN].x;
	OM[SM_TRCGNUP].y = y - arrow_w * 7 / 6;
	OM[SM_TRCGNDN].y = y + arrow_w / 6;
	OM[SM_TRCGNUP].attr = OM_UPARR;
	OM[SM_TRCGNDN].attr = OM_DNARR;

	OM[SM_HISTEXP].dx = 300;
	OM[SM_HISTEXP].dy = arrow_w;
	OM[SM_HISTGAIN].dx = arrow_w;
	OM[SM_HISTEXP].color = OM[OM_MEASUREWIN].color;
	OM[SM_HISTGAIN].color = OM[OM_MEASUREWIN].color;
	if (!window_layout) {
		OM[SM_HISTEXP].x = OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx - 365;
		OM[SM_HISTEXP].y = (int)(OM[SM_DATAREAD].y + OM[SM_DATAREAD].dy + OM[OM_MEASUREWIN].dy*0.02);
		OM[SM_HISTGAIN].x = OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx - OM[SM_HISTGAIN].dx - 6;
		OM[SM_HISTGAIN].y = OM[SM_HISTEXP].y + 120;
		OM[SM_HISTGAIN].dy = OM[OM_MEASUREWIN].dy / 4;
	}
	else {
		OM[SM_HISTEXP].x = OM[SM_TRACEWIN].x + (OM[SM_TRACEWIN].dx - OM[SM_HISTEXP].dx)/2;
		OM[SM_HISTEXP].y = OM[SM_TRACEWIN].y + arrow_w + 3;
		OM[SM_HISTGAIN].x = OM[SM_TRACEWIN].x + OM[SM_TRACEWIN].dx / 2 + 220;
		OM[SM_HISTGAIN].y = OM[SM_HISTEXP].y + 50;
		OM[SM_HISTGAIN].dy = 200;
	}
	if (!OM[SM_HISTGAIN].attr)
		OM[SM_HISTGAIN].attr = 1000-(histoGain-1)*10;

	if (viewer_flag) {
		OM[SM_MOVIEVIEW].state = OM_NULL;
		OM[SM_CARDIOVIEW].state = OM_NULL;
	}
	// Cardio window
	int cardio_menu_w = 340, cardio_w_dy0 = 1000, slider_spacing;
	OM[SM_CARDIOWIN].x = OM[OM_MAIN].x;
	OM[SM_CARDIOWIN].y = OM[OM_MAIN].y;
	OM[SM_CARDIOWIN].dx = OM[OM_MAIN].dx - cardio_menu_w;
	OM[SM_CARDIOWIN].dy = OM[OM_MAIN].dy;
	OM[SM_CARDIOWIN].color = &colorBlack;
	OM[SM_CARDIOPNL].x = OM[SM_CARDIOWIN].x + OM[SM_CARDIOWIN].dx - 2;
	OM[SM_CARDIOPNL].y = OM[SM_CARDIOWIN].y;
	OM[SM_CARDIOPNL].dx = cardio_menu_w;
	OM[SM_CARDIOPNL].dy = OM[SM_CARDIOWIN].dy;
	OM[SM_CARDIOPNL].color = &colorTab;

	for (i = SM_MKMAP; i <= SM_MKMAP; ++i) {
		OM[i].x = OM[SM_CARDIOPNL].x + 20;
		OM[i].y = OM[SM_CARDIOPNL].y + max(17, (OM[SM_CARDIOPNL].dy - cardio_w_dy0) / 2) + (i - SM_MKMAP) * 25 + 40;
		OM[i].title_x = OM[i].x + 6;
	}
	OM[SM_MKMAP].title = "Construct Cardio Data (filtering etc)";

	OM[SM_CABINLB].x = OM[SM_MKMAP].x + 20;
	OM[SM_CABINLB].y = OM[SM_MKMAP].y + 35;
	OM[SM_CABINLB].title_x = OM[SM_CABINLB].x;
	OM[SM_CABINLB].title_y = OM[SM_CABINLB].y;
	OM[SM_CABINLB].dx = 100;
	OM[SM_CABINLB].dy = 12;
	OM[SM_CABINLB].color = &colorTab;
	OM[SM_CABINLB].title = "Digital Bin";
	for (i = SM_CABINUP; i <= SM_CABINDN; ++i) {
		OM[i].dx = 12;
		OM[i].dy = 6;
		OM[i].x = OM[SM_MKMAP].x;
		OM[i].y = OM[SM_MKMAP].y + 35 + (i - SM_CABINUP)*(OM[i].dy + 3);
	}

	OM[SM_MAPBOX1].title = "Membrane Potential";
	OM[SM_MAPBOX2].title = "Activation";
	OM[SM_MAPBOX3].title = "APD50";
	OM[SM_MAPBOX4].title = "Conduction";
	for (i = SM_MAPBOX1; i <= SM_MAPBOX4; ++i) {
		OM[i].x = OM[SM_CARDIOPNL].x + 25;
		if (i == SM_MAPBOX1)
			OM[i].y = OM[SM_MKMAP].y + 160;
		else
			OM[i].y = OM[SM_MKMAP].y + 310 + (i - SM_MAPBOX1) * 25;
		OM[i].dx = 12;
		OM[i].dy = 12;
		OM[i].title_x = OM[i].x + OM[i].dx + 6;
		OM[i].title_y = OM[i].y - 2;
	}

	for (i = MAP_CALC2; i <= MAP_CALC4; ++i) {
		OM[i].x = OM[SM_CARDIOPNL].x + 230;
		OM[i].y = OM[SM_MAPBOX1+1 + i - MAP_CALC2].y - 5;
		OM[i].dx = 80;
		OM[i].dy = 25;
		OM[i].title_x = OM[i].x + 10;
		OM[i].title_y = OM[i].y + 4;
		OM[i].title = "Calculate";
	}

	if (OM[SM_CARDIOPNL].dy > cardio_w_dy0)
		slider_spacing = 38;
	else
		slider_spacing = 30;
	int slider_set_y0 = OM[SM_MKMAP].y + 80;
	for (i = 0; i < 5; ++i) {
		if (SM_CASTART + i * 3 == CUR_CA_FRM)
			slider_set_y0 = OM[SM_MAPBOX1].y + 50;
		if (SM_CASTART + i * 3 == ACTV_START)
			slider_set_y0 = OM[SM_MAPBOX4].y + 40;
		OM[SM_CASTART + i * 3].x = OM[SM_CARDIOPNL].x + 25;
		OM[SM_CASTART + i * 3].y = slider_set_y0 + slider_spacing * i;
		OM[SM_CASTART + i * 3].dx = cardio_menu_w - 60;
		OM[SM_CASTART + i * 3].dy = 12;
		OM[SM_CASTART + i * 3].title_x = OM[SM_CASTART + i * 3].x;
		OM[SM_CASTART + i * 3].title_y = OM[SM_CASTART + i * 3].y - 15;
		OM[SM_CASTART + i * 3 + 1].x = OM[SM_CASTART + i * 3].x + OM[SM_CASTART + i * 3].dx + 10;
		OM[SM_CASTART + i * 3 + 1].y = slider_set_y0 + slider_spacing * i - 1;
		OM[SM_CASTART + i * 3 + 1].dx = 10;
		OM[SM_CASTART + i * 3 + 1].dy = 5;
		OM[SM_CASTART + i * 3 + 2].x = OM[SM_CASTART + i * 3].x + OM[SM_CASTART + i * 3].dx + 10;
		OM[SM_CASTART + i * 3 + 2].y = slider_set_y0 + slider_spacing * i + 7;
		OM[SM_CASTART + i * 3 + 2].dx = 10;
		OM[SM_CASTART + i * 3 + 2].dy = 5;
	}

	OM[CV_BINLB].x = OM[SM_MAPBOX4].x + 20;
	OM[CV_BINLB].y = OM[SM_MAPBOX4].y + 35;
	OM[CV_BINLB].title_x = OM[CV_BINLB].x;
	OM[CV_BINLB].title_y = OM[CV_BINLB].y;
	OM[CV_BINLB].dx = 100;
	OM[CV_BINLB].dy = 12;
	OM[CV_BINLB].color = &colorTab;
	OM[CV_BINLB].title = "Binning for Conduction";
	for (i = CV_BINUP; i <= CV_BINDN; ++i) {
		OM[i].dx = 12;
		OM[i].dy = 6;
		OM[i].x = OM[SM_MAPBOX4].x;
		OM[i].y = OM[SM_MAPBOX4].y + 35 + (i - CV_BINUP)*(OM[i].dy + 3);
	}

	for (i = CARDIO_COLOR; i <= CARDIO_BW; ++i) {
		OM[i].x = OM[SM_CARDIOPNL].x + 50 + (i - CARDIO_COLOR) * 120;
		OM[i].y = OM[SM_MAPBOX1].y + 30;
		OM[i].dx = 20;
		OM[i].dy = 15;
		OM[i].state = OM_RADIOBUTTON;
		OM[i].title_x = OM[i].x + 20;
		OM[i].title_y = OM[i].y - 1;
	}
	OM[CARDIO_COLOR].title = "Rainbow Color";
	OM[CARDIO_BW].title = "Gray level";

	for (i = CARDIO_PLAY; i <= CARDIO_PAUSE; i++) {
		OM[i].x = OM[SM_CARDIOPNL].x + 120 + (i - CARDIO_PLAY) * 50;
		OM[i].y = OM[SM_MAPBOX1].y + 60;
		OM[i].dx = 15;
		OM[i].dy = 15;
	}

	for (i = SM_APD50; i <= SM_APD80; ++i) {
		OM[i].x = OM[SM_CARDIOPNL].x + 50 + (i - SM_APD50) * 100;
		OM[i].y = OM[ACTV_START].y + 65;
		OM[i].dx = 20;
		OM[i].dy = 15;
		OM[i].state = OM_RADIOBUTTON;
		OM[i].title_x = OM[i].x + 20;
		OM[i].title_y = OM[i].y - 1;
	}
	OM[SM_APD50].title = "APD50";
	OM[SM_APD80].title = "APD80";

	int mv_mene_w = 300, mv_w_dy0 = 1000;
	OM[SM_MOVIEWIN].x = OM[OM_MAIN].x;
	OM[SM_MOVIEWIN].y = OM[OM_MAIN].y;
	OM[SM_MOVIEWIN].dx = OM[OM_MAIN].dx - mv_mene_w;
	OM[SM_MOVIEWIN].dy = OM[OM_MAIN].dy;
	OM[SM_MOVIEWIN].color = &colorBlack;
	OM[SM_MOVIEPNL].x = OM[SM_MOVIEWIN].x + OM[SM_MOVIEWIN].dx - 2;
	OM[SM_MOVIEPNL].y = OM[SM_MOVIEWIN].y;
	OM[SM_MOVIEPNL].dx = mv_mene_w;
	OM[SM_MOVIEPNL].dy = OM[SM_MOVIEWIN].dy;
	OM[SM_MOVIEPNL].color = &colorTab;

	if (OM[SM_MOVIEPNL].dy > mv_w_dy0)
		slider_spacing = 38;
	else
		slider_spacing = 30;
	for (i = SM_MKMOVIE; i <= SM_DATAOVLY; ++i) {
		OM[i].x = OM[SM_MOVIEPNL].x + 20;
		OM[i].y = OM[SM_MOVIEPNL].y + max(17, (OM[SM_MOVIEPNL].dy- mv_w_dy0) /2) + (i - SM_MKMOVIE )*25;
		OM[i].title_x = OM[i].x + 6;
	}
	OM[SM_MKMOVIE].title = "Make / Update Movie";
	OM[SM_RDOVRLAY].title = "Read Tiff for Overlay";
	OM[SM_DATAOVLY].title = "Use Unbinned for Overlay";

	for (i = SM_MVSUBBOX; i <= SM_MVFLTBOX; ++i) {
		OM[i].x = OM[SM_MOVIEPNL].x + 25;
		OM[i].y = OM[SM_DATAOVLY].y + 60 + (i - SM_MVFLTBOX) * 25;
		OM[i].dx = 12;
		OM[i].dy = 12;
		OM[i].title_x = OM[i].x + OM[i].dx + 6;
		OM[i].title_y = OM[i].y - 2;
	}
	OM[SM_MVSUBBOX].title = "Subtract Movie Frame 1";
	OM[SM_MVFLTBOX].title = "Apply Filters / Polarity";
	OM[SM_MVBINLB].x = OM[SM_MVFLTBOX].title_x;
	OM[SM_MVBINLB].y = OM[SM_MVFLTBOX].y + 23;
	OM[SM_MVBINLB].title_x = OM[SM_MVBINLB].x;
	OM[SM_MVBINLB].title_y = OM[SM_MVBINLB].y;
	OM[SM_MVBINLB].dx = 100;
	OM[SM_MVBINLB].dy = 12;
	OM[SM_MVBINLB].color = &colorTab;
	OM[SM_MVBINLB].title = "Digital Bin";
	for (i = SM_MVBINUP; i <= SM_MVBINDN; ++i) {
		OM[i].dx = 12;
		OM[i].dy = 6;
		OM[i].x = OM[SM_MVFLTBOX].x;
		OM[i].y = OM[SM_MVFLTBOX].y + 24 + (i - SM_MVBINUP)*(OM[i].dy + 3);
	}

	OM[OV_MODE_LB].x = OM[SM_MOVIEPNL].x + 20;
	OM[OV_MODE_LB].y = OM[SM_MVBINLB].y + slider_spacing*2 + 35;
	OM[OV_MODE_LB].dx = 150;
	OM[OV_MODE_LB].dy = 12;
	OM[OV_MODE_LB].title_x = OM[OV_MODE_LB].x;
	OM[OV_MODE_LB].title_y = OM[OV_MODE_LB].y - 2;
	OM[OV_MODE_LB].title = "Overlay Mode";
	for (i = SM_OV_MODE1; i <= SM_OV_MODE3; ++i) {
		OM[i].x = OM[SM_MOVIEPNL].x + (i - SM_OV_MODE1) * 90 + 25;
		if (i == SM_OV_MODE2)
			OM[i].x -= 9;
		OM[i].y = OM[OV_MODE_LB].y + 25;
		OM[i].dx = 20;
		OM[i].dy = 15;
		OM[i].state = OM_RADIOBUTTON;
		OM[i].title_x = OM[i].x + 15;
		OM[i].title_y = OM[i].y - 1;
	}
	OM[SM_OV_MODE1].title = "Movie Only";
	OM[SM_OV_MODE2].title = "Movie Overlay";
	OM[SM_OV_MODE3].title = "Image Only";

	OM[MOV_SCALE_LB].x = OM[SM_MOVIEPNL].x + 20;
	OM[MOV_SCALE_LB].y = OM[SM_OV_MODE1].y + 55;
	OM[MOV_SCALE_LB].dx = 150;
	OM[MOV_SCALE_LB].dy = 20;
	OM[MOV_SCALE_LB].title_x = OM[MOV_SCALE_LB].x;
	OM[MOV_SCALE_LB].title_y = OM[MOV_SCALE_LB].y - 2;
	OM[MOV_SCALE_LB].title = "Movie Scaling Mode";
	for (i = SM_MV_SCALE1; i <= SM_MV_SCALE3; ++i) {
		OM[i].x = OM[SM_MOVIEPNL].x + 25;
		OM[i].y = OM[MOV_SCALE_LB].y + (i - SM_MV_SCALE1) * 25 + 25;
		OM[i].dx = 20;
		OM[i].dy = 15;
		OM[i].state = OM_RADIOBUTTON;
		OM[i].title_x = OM[i].x + 15;
		OM[i].title_y = OM[i].y - 1;
	}
	OM[SM_MV_SCALE1].title = "Auto Max/Min within Frame (excluding 5% border)";
	OM[SM_MV_SCALE2].title = "Manual Max/Min from Scale Slider";
	OM[SM_MV_SCALE3].title = "Auto Max/Min within Pixel (Trace)";
//	OM[SM_MV_SCALE3].state = OM_NULL;

	slider_set_y0 = OM[SM_MVBINLB].y + 50;
	for (i = 0; i < 13; ++i) {
		if ((SM_MVSTART + i * 3 == MOV_SCALEMIN2) || (SM_MVSTART + i * 3 == MOV_SCALEMAX2))
			slider_set_y0 = OM[SM_MV_SCALE2].y - slider_spacing*2 + 55;
		else if (SM_MVSTART + i * 3 > SM_CURMOVF)
			slider_set_y0 = OM[SM_MV_SCALE2].y - slider_spacing * 2 + 200;
		else if (SM_MVSTART + i * 3 == SM_CURMOVF)
			slider_set_y0 = OM[SM_MV_SCALE2].y - slider_spacing * 2 + 145;
		else if (SM_MVSTART + i * 3 > MOV_SCALEMAX2)
			slider_set_y0 = OM[SM_MV_SCALE2].y - slider_spacing * 2 + 115;
		else if (SM_MVSTART + i * 3 > SM_MVEND)
			slider_set_y0 = OM[SM_MV_SCALE2].y - slider_spacing * 2 + 70;
		else
			slider_set_y0 = OM[SM_MVBINLB].y + 45;
		OM[SM_MVSTART + i * 3].x = OM[SM_MOVIEPNL].x + 25;
		OM[SM_MVSTART + i * 3].y = slider_set_y0 + slider_spacing * i;
		OM[SM_MVSTART + i * 3].dx = mv_mene_w - 60;
		OM[SM_MVSTART + i * 3].dy = 12;
		OM[SM_MVSTART + i * 3].title_x = OM[SM_MVSTART + i * 3].x;
		OM[SM_MVSTART + i * 3].title_y = OM[SM_MVSTART + i * 3].y - 15;
		OM[SM_MVSTART + i * 3 + 1].x = OM[SM_MVSTART + i * 3].x + OM[SM_MVSTART + i * 3].dx + 10;
		OM[SM_MVSTART + i * 3 + 1].y = slider_set_y0 + slider_spacing * i - 1;
		OM[SM_MVSTART + i * 3 + 1].dx = 10;
		OM[SM_MVSTART + i * 3 + 1].dy = 5;
		OM[SM_MVSTART + i * 3 + 2].x = OM[SM_MVSTART + i * 3].x + OM[SM_MVSTART + i * 3].dx + 10;
		OM[SM_MVSTART + i * 3 + 2].y = slider_set_y0 + slider_spacing * i + 7;
		OM[SM_MVSTART + i * 3 + 2].dx = 10;
		OM[SM_MVSTART + i * 3 + 2].dy = 5;
	}
	OM[MOV_SCALEMIN2].title_y = OM[MOV_SCALEMIN].title_y;
	OM[MOV_SCALEMAX2].title_y = OM[MOV_SCALEMAX].title_y;

	for (i = MOV_PLAY; i <= MOV_PAUSE; i++) {
		OM[i].x = OM[SM_MOVIEPNL].x + 120 + (i-MOV_PLAY)*50;
		OM[i].y = OM[SM_CURMOVF].y + 20;
		OM[i].dx = 15;
		OM[i].dy = 15;
	}

	OM[MOV_MODE_LB].x = OM[SM_MOVIEPNL].x + 20;
	OM[MOV_MODE_LB].y = OM[MOV_SCALEMAX2].y + 55;
	OM[MOV_MODE_LB].dx = 150;
	OM[MOV_MODE_LB].dy = 12;
	OM[MOV_MODE_LB].title_x = OM[MOV_MODE_LB].x;
	OM[MOV_MODE_LB].title_y = OM[MOV_MODE_LB].y - 2;
	OM[MOV_MODE_LB].title = "Movie Display";
	for (i = MOV_MOVIE; i <= MOV_MULTI; ++i) {
		OM[i].x = OM[SM_MOVIEPNL].x + 25;
		if (i == MOV_MOVIE)
			OM[i].y = OM[SM_CURMOVF].y - 35;
		else
			OM[i].y = OM[MULTI_FRAME1].y - 35;
		OM[i].dx = 20;
		OM[i].dy = 15;
		OM[i].state = OM_RADIOBUTTON;
		OM[i].title_x = OM[i].x + 15;
		OM[i].title_y = OM[i].y - 1;
	}
	OM[MOV_MOVIE].title = "Movie";
	OM[MOV_MULTI].title = "Multiple Frames";

	//****** Misc Window *********
	omSize = getOMSize();
	omSizeSave = omSize;

	if (startText()) {
		oldFont = setTextFont(omFontBig,2);
		for (i = SM_RESET; i <= OM_OPTQUIT; ++i) {
			char *s = OM[i].title;

			if (OM[i].opt->grey)
				OM[i].color = &colorGrayText; //was LTGRAY2;
			OM[i].title_x = OM[OM_MENUBOX].x + 5 * (OM[i].opt->col + 3);
			OM[i].title_y = OM[OM_MENUBOX].y + 18 * (OM[i].opt->row - 3) + 9;
			OM[i].x = OM[i].title_x - 2;
			OM[i].y = OM[i].title_y;
			getTextSize(s, (int)strlen(s), &dxdy);
			OM[i].dx = dxdy.cx + 2;
			OM[i].dy = dxdy.cy + 1;
		}
		setTextFont(omFontMedium, 2);
		for (i = SM_DARKSUB; i <= SM_LOCKHIST; ++i) {
			char *s = OM[i].title;

			if (OM[i].opt->grey)
				OM[i].color = &colorGrayText; //was LTGRAY2;
			OM[i].title_x = OM[OM_MENUBOX].x + 5 * (OM[i].opt->col + 3);
			OM[i].title_y = OM[OM_MENUBOX].y + 18 * (OM[i].opt->row - 3) + 9;
			OM[i].x = OM[i].title_x - 2;
			OM[i].y = OM[i].title_y;
			getTextSize(s, (int)strlen(s), &dxdy);
			OM[i].dx = dxdy.cx + 2;
			OM[i].dy = dxdy.cy + 1;
		}

		for (i = SM_MKMAP; i <= SM_MKMAP; ++i) {
			char *s = OM[i].title;
			getTextSize(s, (int)strlen(s), &dxdy);
			OM[i].title_y = OM[i].y + 3;
			OM[i].dx = dxdy.cx + 2;
			OM[i].dy = dxdy.cy + 5;
		}

		for (i = SM_MKMOVIE; i <= SM_DATAOVLY; ++i) {
			char *s = OM[i].title;
			getTextSize(s, (int)strlen(s), &dxdy);
			OM[i].title_y = OM[i].y + 3;
			OM[i].dx = dxdy.cx + 2;
			OM[i].dy = dxdy.cy + 5;
		}

		setTextFont(omFontSmall, 0);
		getTextSize(dataString, (int)strlen(dataString), &dxdy);
		fontSM_w = (double)dxdy.cx / (int)strlen(dataString);
		if (dxdy.cy > hgt)
			hgt = dxdy.cy;
		for (i = SM_RESET; i <= OM_OPTQUIT; ++i) {
			getTextSize(OM[i].opt->keystr, (int)strlen(OM[i].opt->keystr), &dxdy);
			OM[i].opt->keystr_x = maxX - dxdy.cx;
			OM[i].opt->keystr_y = OM[i].title_y + (hgt - dxdy.cy) / 2;
		}
		for (i = OM_MEASUREVIEW; i <= OM_HELPVIEW; ++i) {
			char *s = OM[i].title;
			getTextSize(s, (int)strlen(s), &dxdy);
			OM[i].title_x = OM[i].x + (OM[i].dx - dxdy.cx) / 2;
			OM[i].title_y = OM[i].y + (OM[i].dy - dxdy.cy) / 2;
		/*	if (0 == OM_TEXT_CHINESE) {
				if (i == OM_MEASUREVIEW || i == SM_MOVIEVIEW)
					OM[i].title_x = OM[i].x + 11;
			}*/
		}
		stopText();
	}

	OM[SM_DKSUBBOX].dx = buttonWidth - 2;
	OM[SM_DKSUBBOX].x = OM[SM_DKFRAME].x;
	OM[SM_DKSUBBOX].dy = buttonHeight - 2;
	OM[SM_DKSUBBOX].y = OM[SM_DARKSUB].y + 1;
	OM[SM_AUTOSBOX].dx = buttonWidth - 2;
	OM[SM_AUTOSBOX].x = OM[SM_DKFRAME].x;
	OM[SM_AUTOSBOX].dy = buttonHeight - 2;
	OM[SM_AUTOSBOX].y = OM[SM_AUTOSCL].y + 1;
	OM[SM_HLOCKBOX].dx = buttonWidth - 2;
	OM[SM_HLOCKBOX].x = OM[SM_DKFRAME].x;
	OM[SM_HLOCKBOX].dy = buttonHeight - 2;
	OM[SM_HLOCKBOX].y = OM[SM_LOCKHIST].y + 1;
	OM[SM_FOCUSBOX].dx = buttonWidth - 2;
	OM[SM_FOCUSBOX].x = OM[SM_LIVE].x + 90;
	OM[SM_FOCUSBOX].dy = buttonHeight - 2;
	OM[SM_FOCUSBOX].y = OM[SM_LIVE].y - 1;
	OM[SM_OVWRTBOX].dx = buttonWidth - 2;
	OM[SM_OVWRTBOX].x = OM[SM_DKFRAME].x + 40;
	OM[SM_OVWRTBOX].dy = buttonHeight - 2;
	OM[SM_OVWRTBOX].y = OM[SM_OVERWRT].y + 2;

	OM[SM_FNAMEBOX].dx = buttonWidth;
	OM[SM_FNAMEBOX].x = OM[SM_DKFRAME].x + 2;
	OM[SM_FNAMEBOX].dy = buttonHeight;
	OM[SM_FNAMEBOX].y = OM[SM_OVWRTBOX].y;
	OM[SM_FNAMEUPARR].dx = buttonWidth - 2;
	OM[SM_FNAMEUPARR].x = OM[SM_FNAMEBOX].x;
	OM[SM_FNAMEUPARR].dy = buttonHeight/2;
	OM[SM_FNAMEUPARR].y = OM[SM_FNAMEBOX].y - 2;
	OM[SM_FNAMEDNARR].dx = buttonWidth - 2;
	OM[SM_FNAMEDNARR].x = OM[SM_FNAMEBOX].x;
	OM[SM_FNAMEDNARR].dy = buttonHeight/2;
	OM[SM_FNAMEDNARR].y = OM[SM_FNAMEBOX].y + buttonHeight/ 2 + 2;

	if (darkSubFlag && (darkExist || d_darkExist))
		setChckBox(SM_DKSUBBOX);
	if (autoScaleFlag)
		setChckBox(SM_AUTOSBOX);
	if (lockHistFlag)
		setChckBox(SM_HLOCKBOX);
	if (focusMode)
		setChckBox(SM_FOCUSBOX);
	if (overwriteFlag)
		setChckBox(SM_OVWRTBOX);

	if (!window_layout) {
		OM[SM_HISTGAIN].parent = OM_MEASUREWIN;
		OM[SM_HISTEXP].parent = OM_MEASUREWIN;
		image_region_dx = OM[SM_BNCPNL].x - OM[OM_MEASUREWIN].x - 25;
		image_region_dy = OM[OM_MEASUREWIN].dy;
		hist_region_x = OM[OM_MEASUREWIN].dx - 400;
		hist_region_dy = min(OM[OM_MEASUREWIN].dy * 3 / 5, OM[SP_FLTPNL].y - OM[SM_HISTEXP].y - OM[SM_HISTEXP].dy - 40);
		hist_region_y = OM[SM_HISTEXP].y + 20 + hist_region_dy;
		trace_region_y = OM[SM_TRACEWIN].y;
	}
	else {
		OM[SM_HISTGAIN].parent = SM_TRACEWIN;
		OM[SM_HISTEXP].parent = SM_TRACEWIN;
		image_region_dx = OM[OM_MEASUREWIN].dx - 20;
		image_region_dy = OM[SM_BNCPNL].y - OM[OM_MEASUREWIN].y - 20;
		hist_region_x = OM[SM_HISTGAIN].x - 400;
		hist_region_dy = 300;
		hist_region_y = OM[SM_HISTEXP].y + 20 + hist_region_dy;
		if (window_layout > 1)
			trace_region_y = OM[SM_TRACEWIN].y;
		else {
			trace_region_y = hist_region_y + 30;
			OM[SM_TRCGAIN].y = trace_region_y + trace_bdr;
			OM[SM_TRCGAIN].dy = OM[SM_TRCGNUP].y - trace_region_y - trace_bdr*2;
		}
	}
	trace_region_dy = OM[SM_TRCEXP].y - trace_region_y;
	hist_x = hist_region_x;

	strcat(strcpy(flagFname, home_dir), "\\SMSYSDAT\\96well_flag.txt");
	if (FOPEN_S(fp, flagFname, "r")) {
		int well_bin;
		if (fgets(buf, 255, fp)) {
			sscanf(buf, "%d %d%s", &well96_flag, &well_bin, str);
			well96_flag /= well_bin;
		}
		if (fgets(buf, 255, fp)) {
			sscanf(buf, "%d %d%s", &well_x, &well_y, str);
			if (well_x*well_y > 32)
				well_y = 32 / well_x;
		}
		fclose(fp);
	}
	else
		well96_flag = 0;

}

void paintScreen(int flag)
{
	setToolPanel(0);
	if (dataExist) {
		ImgMenuActivated = 0;
		activateImgMenu();
	}
	if (dataFile && flag)
		SM_readData(SM_dataname);
	paintWin(-2, PAINTINIT);
}

void paintTab(int win, int x, int y, int dx, int dy, int ontop)
{
	if (ontop) {
		HLine(x-2,x+dx+1,y-2,colorRegular);//LTGRAY);
		HLine(x-2,x+dx+1,y-1,colorRegular);//LTGRAY);
		HLine(x-2,x+dx+2,y+dy,colorRegular);//LTGRAY);
		HLine(x-1,x+dx-1,y+dy+1,colorLightShadow);//MIDGRAY);
		HLine(x,x+dx-1,y+dy+2,colorBlack);//BLACK);
		VLine(y-2,y+dy-1,x-1,colorRegular);//LTGRAY);
		DrawPixelSlow(x-1,y+dy,colorLightShadow);//MIDGRAY);
		VLine(y-2,y+dy,x-2,colorLight);//WHITE);
		VLine(y-2,y+dy,x+dx,colorLightShadow);//MIDGRAY);
		DrawPixelSlow(x+dx+1,y-2,colorLightShadow);//MIDGRAY);
		DrawPixelSlow(x+dx,y+dy+1,colorBlack);//BLACK);
		DrawPixelSlow(x+dx-1,y+dy+1,colorLightShadow);//MIDGRAY);
		VLine(y-1,y+dy,x+dx+1,colorBlack);//BLACK);
	}
	else {
		HLine(x-2,x+dx+1,y-2,colorLightShadow);//MIDGRAY);
		HLine(x-2,x+dx+1,y-1,colorBlack);//BLACK);
		HLine(x-2,x+dx+2,y+dy,colorRegular);//LTGRAY);
		HLine(x-1,x+dx-1,y+dy+1,colorLightShadow);//MIDGRAY);
		HLine(x,x+dx-1,y+dy+2,colorBlack);//BLACK);
		VLine(y,y+dy-1,x-1,colorRegular);//LTGRAY);
		DrawPixelSlow(x-1,y+dy,colorLightShadow);//MIDGRAY);
		VLine(y,y+dy,x-2,colorLight);//WHITE);
		VLine(y,y+dy,x+dx,colorLightShadow);//MIDGRAY);
		DrawPixelSlow(x+dx,y+dy+1,colorBlack);//BLACK);
		DrawPixelSlow(x+dx-1,y+dy+1,colorLightShadow);//MIDGRAY);
		VLine(y,y+dy,x+dx+1,colorBlack);//BLACK);
	}
}

void paintButtonFrame(int x, int y, int dx, int dy, int down)
{
	if (down) {
		HLine(x+1,x+dx-2,y+1,colorLightShadow);//MIDGRAY);
		HLine(x+1,x+dx-2,y+dy-2,colorLight);//WHITE);
		VLine(y+1,y+dy-2,x+1,colorLightShadow);//MIDGRAY);
		VLine(y+1,y+dy-2,x+dx-2,colorLight);//WHITE);
	}
	else {
		HLine(x+1,x+dx-2,y+1,colorLight);//WHITE);
		HLine(x+1,x+dx-2,y+dy-1,colorLightShadow);//MIDGRAY);
		VLine(y+1,y+dy-3,x+1,colorLight);//WHITE);
		VLine(y,y+dy-2,x+dx-1,colorLightShadow);//MIDGRAY);
	}
}

	int radiobutton_map[][13] =	{	{-1,	 -1,	 -1,	 -1,	 MIDGRAY,MIDGRAY,MIDGRAY,MIDGRAY,-1,	 -1,	 -1,	 -1,	 -1},
									{-1,	 -1,	 MIDGRAY,BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 MIDGRAY,-1,	 -1,	 -1},
									{-1,	 MIDGRAY,BLACK,	 MIDGRAY,WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,MIDGRAY,WHITE,	 -1},
									{-1,	 BLACK,	 MIDGRAY,WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,-1,	 -1},
									{MIDGRAY,BLACK,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,WHITE},
									{MIDGRAY,BLACK,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,WHITE},
									{MIDGRAY,BLACK,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,WHITE},
									{MIDGRAY,BLACK,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,WHITE},
									{-1,	 BLACK,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,WHITE},
									{-1,	 MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,WHITE,	 -1},
									{-1,	 -1,	 MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,-1,	 WHITE,	 -1},
									{-1,	 -1,	 WHITE,	 -1,	 MIDGRAY,MIDGRAY,MIDGRAY,MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 -1,	 -1},
									{-1,	 -1,	 -1,	 -1,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 -1,	 -1,	 -1,	 -1}};
	
	int radiobutton_map2[][13] ={	{-1,	 -1,	 -1,	 -1,	 MIDGRAY,MIDGRAY,MIDGRAY,MIDGRAY,-1,	 -1,	 -1,	 -1,	 -1},
									{-1,	 -1,	 MIDGRAY,BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 MIDGRAY,-1,	 -1,	 -1},
									{-1,	 MIDGRAY,BLACK,	 MIDGRAY,WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,MIDGRAY,WHITE,	 -1},
									{-1,	 BLACK,	 MIDGRAY,WHITE,	 WHITE,	 MIDGRAY,MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 MIDGRAY,-1,	 -1},
									{MIDGRAY,BLACK,	 WHITE,	 WHITE,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 WHITE,	 WHITE,	 MIDGRAY,WHITE},
									{MIDGRAY,BLACK,	 WHITE,	 MIDGRAY,BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 MIDGRAY,WHITE,	 MIDGRAY,WHITE},
									{MIDGRAY,BLACK,	 WHITE,	 MIDGRAY,BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 MIDGRAY,WHITE,	 MIDGRAY,WHITE},
									{MIDGRAY,BLACK,	 WHITE,	 MIDGRAY,BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 MIDGRAY,WHITE,	 MIDGRAY,WHITE},
									{-1,	 BLACK,	 WHITE,	 WHITE,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 BLACK,	 WHITE,	 WHITE,	 MIDGRAY,WHITE},
									{-1,	 MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 MIDGRAY,MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 MIDGRAY,WHITE,	 -1},
									{-1,	 -1,	 MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 MIDGRAY,-1,	 WHITE,	 -1},
									{-1,	 -1,	 WHITE,	 -1,	 MIDGRAY,MIDGRAY,MIDGRAY,MIDGRAY,MIDGRAY,WHITE,	 WHITE,	 -1,	 -1},
									{-1,	 -1,	 -1,	 -1,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 WHITE,	 -1,	 -1,	 -1,	 -1}};

void paintRadioButton(int win)
{
	int i,j;
		

	for (j = 0; j < sizeof(radiobutton_map)/sizeof(radiobutton_map[0]); j++)
		for (i = 0; i < sizeof(radiobutton_map[0])/sizeof(radiobutton_map[0][0]); i++)
			if (radiobutton_map[i][j] > -1) {
				if (OM[win].attr == OM_HOT) 
					HLine(OM[win].x+i,OM[win].x+i,OM[win].y+j,radiobutton_map2[i][j]);
				else
					HLine(OM[win].x+i,OM[win].x+i,OM[win].y+j,radiobutton_map[i][j]);
			}
}

void paintSmArrow(int x, int y, int dx, int dy, int attr, int color)
{
	int i;

	for (i = 0; i <= dx/2-5; ++i) {
		if (attr == OM_UPARR)
			HLine(x+5+i,x+dx-5-i,y+dy-(dy-dx/2+2)/2-2-i,color);
		else if (attr == OM_DNARR) 
			HLine(x+5+i,x+dx-5-i,y+(dy-dx/2+2)/2+2+i,color);
		else if (attr == OM_LTARR)
			VLine(y+5+i,y+dy-5-i,x+dx-(dx-dy/2+2)/2-2-i,color);
		else if (attr == OM_RTARR) 
			VLine(y+5+i,y+dy-5-i,x+(dx-dy/2+2)/2+2+i,color);
	}
}

void paintNarrowArrow(int x, int y, int dx, int dy, int attr, int color)
{
	int i;

	for (i = 0; i <= dx/2; ++i) {
		if (attr == OM_UPARR)
			HLine(x+i,x+dx-i,y+dy-i,color);
		else if (attr == OM_DNARR) 
			HLine(x+i,x+dx-i,y+i,color);
	}
}

int videobutton_draw[][16] =   {{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	  0,	 -1, 	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	  0,	 -1,	  0,	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	  0, 	 -1,	  0,	  0,	  0,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	  0,	 -1, 	  0,	  0,	  0,	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	  0,	 -1,	  0, 	  0,	  0,	 -1, 	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	  0,	 -1,	  0,	  0, 	  0,	 -1,	 -1, 	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	  0,	 -1,	  0,	  0,	  0, 	 -1,	 -1,	 -1, 	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	  0,	 -1,	  0,	  0,	  0,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	  0,	 -1,	  0,	  0,	  0,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 -1},
								{-1,	 -1,	 -1,	  0,	 -1,	  0,	  0,	  0,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 -1},
								{-1,	 -1,	  0,	 -1,	  0,	  0,	  0,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 -1},
								{-1,	 -1,	  0,	  0,	  0,	  0,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 -1},
								{-1,	  0,	 -1,	 -1,	  0,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1},
								{-1,	  0,	  0,	  0,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1},
								{ 0,	  0,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1}};

int videobutton_dragbox[][16] = {{DKGRAY,DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 DKGRAY},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 DKGRAY},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,		 -1, DKGRAY, DKGRAY},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY}};

int videobutton_rmvbox[][16] = {{DKGRAY,DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1,	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1, 	 -1,	 -1},
								{DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 DKGRAY},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 DKGRAY},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,		 -1, DKGRAY, DKGRAY},
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY}};

int videobutton_addbox[][16] = {{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{-1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{DKGRAY, DKGRAY,DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY,  DKGRAY,  -1, 	 -1,	 -1, 	 -1,	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1 } };

int videobutton_addbox2[][16] ={{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ DKGRAY, DKGRAY,DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY,  DKGRAY,  -1, 	 -1,	 -1, 	 -1,	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1 } };

int videobutton_thr[][16] = { { -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ DKGRAY, DKGRAY,DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY,  DKGRAY,  -1, 	 -1,	 -1, 	 -1,	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1 } };

int videobutton_thr2[][16] = { { -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY - 1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1,	 -1 },
								{ DKGRAY, DKGRAY,DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY,  DKGRAY,  -1, 	 -1,	 -1, 	 -1,	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, -1,	 -1, 	 -1,	 -1, 	 -1,	 -1 },
								{ DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1, 	 -1,	 -1,	 -1, 	 -1,	 -1 },
								{ -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1 } };

int videobutton_kernel[][16] = {{-1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1,	 -1,	 -1,	 -1,	 -1},
								{-1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1},
								{-1,	 -1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 DKGRAY, -1,	 -1},
								{-1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1,	 DKGRAY, -1},
								{-1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 DKGRAY, -1},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1, 	 DKGRAY},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 DKGRAY},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 DKGRAY},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 DKGRAY},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 DKGRAY},
								{DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 -1,	 DKGRAY},
								{-1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1,	 DKGRAY, -1},
								{-1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 -1, 	 DKGRAY, -1},
								{-1,	 -1,	 DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 -1, 	 -1,	 -1,	 DKGRAY, -1,	 -1},
								{-1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, -1,	 -1,	 -1},
								{-1,	 -1,	 -1,	 -1,	 -1,	 DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, DKGRAY, -1,	 -1,	 -1,	 -1,	 -1}};

void paintIconButton(int win, int color)
{
	int i, j;
	int iconbutton_map[16][16];

	switch (win) {
		case SM_DRAW:
			memcpy(iconbutton_map, videobutton_draw, sizeof(iconbutton_map));
			break;
		case SM_DRAGBOX:
			memcpy(iconbutton_map, videobutton_dragbox, sizeof(iconbutton_map));
			break;
		case SM_KERNEL:
		case SM_QUESTION:
			memcpy(iconbutton_map, videobutton_kernel, sizeof(iconbutton_map));
			break;
		case SM_REMOVE:
			memcpy(iconbutton_map, videobutton_rmvbox, sizeof(iconbutton_map));
			break;
		case SM_ADD_CUR_ROI:
			if (add_roi_flag)
				memcpy(iconbutton_map, videobutton_addbox2, sizeof(iconbutton_map));
			else
				memcpy(iconbutton_map, videobutton_addbox, sizeof(iconbutton_map));
			break;
		case SM_THRESHOLD:
			if (threshold_flag)
				memcpy(iconbutton_map, videobutton_thr2, sizeof(iconbutton_map));
			else
				memcpy(iconbutton_map, videobutton_thr, sizeof(iconbutton_map));
			break;
	}
	for (j = 0; j < sizeof(iconbutton_map)/sizeof(iconbutton_map[0]); j++)
		for (i = 0; i < sizeof(iconbutton_map[0])/sizeof(iconbutton_map[0][0]); i++) {
			if (iconbutton_map[j][i] != -1) {
				HLine(OM[win].x+i,OM[win].x+i,OM[win].y+j,color);
			}
		}
}

void paintVideoButton(int win, int paintFlag)
{
	int color = 0, state = OM[win].anim_state;

	if (paintFlag == PAINTGRFX) {
		int i;

		if (hotTab == OM_MEASUREVIEW) {
			if (win != SM_QUESTION)
				paintWin(SM_TPNLTEXT, paintFlag);
			if (seqFlag) {
				if ((win == SM_PREVREAD && curSeq == 0) || (win == SM_NEXTREAD && curSeq == 7))
					color = colorDarkShadow;
				else if (state == OM_WARM) color = colorRegular;
				else if (state == OM_COLD) color = colorLightShadow;
			}
			else {
				if ((PlayFlag && win != SM_PAUSE) || (!PlayFlag && win == SM_PAUSE) || (firstDataFrame == 0 && win == SM_PREVREAD)
					|| (firstDataFrame + numTraceFrames == numDataFrames && win == SM_NEXTREAD))
					color = colorDarkShadow;
				else if (win - SM_DRAW == selectPixMode || state == OM_WARM) color = colorRegular;
				else if (state == OM_COLD) color = colorLightShadow;
			}
			if (win == SM_FIRSTFRM)
				D3DRectangle(OM[win].x, OM[win].y, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy, colorDarkShadow);
		}
		else {
			if ((!PlayFlag && win == MOV_PLAY) || (PlayFlag && win == MOV_PAUSE) || (!PlayFlag && win == CARDIO_PLAY) || (PlayFlag && win == CARDIO_PAUSE))
					color = colorDarkShadow;
			else color = colorLightShadow;
		}
		if (win == SM_PLAY || win == MOV_PLAY || win == CARDIO_PLAY) {
			for (i = 0; i <= OM[win].dy; i++)
				VLine(OM[win].y + OM[win].dy / 2 + i / 2, OM[win].y + OM[win].dy / 2 - i / 2, OM[win].x + OM[win].dy - i, color);
			Draw_Line(OM[win].x, OM[win].y + OM[win].dy, OM[win].x + OM[win].dy, OM[win].y + OM[win].dy / 2, colorLightShadow);
		}
		else if (win == SM_PAUSE || win == MOV_PAUSE || win == CARDIO_PAUSE) {
			int inc = 0;
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, colorLightShadow);
			inc += 2;
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + inc++, colorLightShadow);
		}
		else if (win == SM_PREVFRAME || win == SM_PREVREAD) {
			for (i = 0; i <= OM[win].dx / 2; i++)
				VLine(OM[win].y + OM[win].dy / 2 + i * 2 / 3, OM[win].y + OM[win].dy / 2 - i * 2 / 3, OM[win].x + i, color);
			VLine(OM[win].y + OM[win].dy / 2 + i * 2 / 3, OM[win].y + OM[win].dy / 2 - i * 2 / 3, OM[win].x + OM[win].dx / 2 + 1, colorLightShadow);
			for (i = OM[win].dx / 2 + 1; i <= OM[win].dx; i++)
				VLine(OM[win].y + OM[win].dy * 3 / 8, OM[win].y + OM[win].dy * 5 / 8, OM[win].x + i, color);
			VLine(OM[win].y + OM[win].dy * 3 / 8, OM[win].y + OM[win].dy * 5 / 8, OM[win].x + OM[win].dx, colorLightShadow);
			HLine(OM[win].x + OM[win].dx / 2 + 1, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy * 5 / 8, colorLightShadow);
		}
		else if (win == SM_NEXTFRAME || win == SM_NEXTREAD) {
			for (i = 0; i <= OM[win].dx / 2; i++)
				VLine(OM[win].y + OM[win].dy * 3 / 8, OM[win].y + OM[win].dy * 5 / 8, OM[win].x + i, color);
			HLine(OM[win].x, OM[win].x + OM[win].dx / 2, OM[win].y + OM[win].dy * 5 / 8, colorLightShadow);
			for (i = 0; i <= OM[win].dx / 2; i++)
				VLine(OM[win].y + OM[win].dy / 2 + i * 2 / 3, OM[win].y + OM[win].dy / 2 - i * 2 / 3, OM[win].x + OM[win].dx - i, color);
			Draw_Line(OM[win].x + OM[win].dx / 2, OM[win].y + OM[win].dy / 2 + OM[win].dx / 3, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy / 2, colorLightShadow);
		}
		else if (win == SM_DRAW || win == SM_DRAGBOX || win == SM_KERNEL || win == SM_REMOVE || win == SM_ADD_CUR_ROI || win == SM_THRESHOLD || win == SM_QUESTION) {
			paintIconButton(win, color);
		}
		else if ((win == SM_KSIZEUP) || (win == SP_LPTYPE_UP) || (win == SP_LP_UP) || (win == SP_HPTYPE_UP) || (win == SP_HP_UP)) {
			for (i = 0; i <= OM[win].dx / 2; ++i)
				HLine(OM[win].x + i, OM[win].x + OM[win].dx - i, OM[win].y + OM[win].dy - i, color);
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy, colorLightShadow);
		}
		else if ((win == SM_KSIZEDN) || (win == SP_LPTYPE_DN) || (win == SP_LP_DN) || (win == SP_HPTYPE_DN) || (win == SP_HP_DN)) {
			for (i = 0; i <= OM[win].dx / 2; ++i)
				HLine(OM[win].x + i, OM[win].x + OM[win].dx - i, OM[win].y + i, color);
			Draw_Line/*6432*/(OM[win].x + OM[win].dx / 2, OM[win].y + OM[win].dx / 2, OM[win].x + OM[win].dx, OM[win].y, colorLightShadow);
		}
		else if (win == SM_TPOLARITY) {
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy / 2, color);
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy / 2 + 1, color);
			if (tracePolarity == 1) {
				VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + OM[win].dx / 2, color);
				VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + OM[win].dx / 2 + 1, color);
			}
		}
		else if (win == SM_FIRSTFRM) {
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y, color);
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y + 1, color);
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy, color);
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy - 1, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + 1, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + OM[win].dx, color);
			VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + OM[win].dx - 1, color);
		}
	}
	else if (paintFlag == PAINTTEXT) {
		char str[30];

		strcpy(str, "");
		if (win == SM_QUESTION) {
			strcpy(str, "?");
			drawText(0, 0, str, OM[win].x + 5, OM[win].y + 2, TEXT_RED, omFontSmall, TRUE);
		}
		else if (win == SM_KERNEL) {
			sprintf(str, "%d", sm_kernel_size);
			if (sm_kernel_size < 10)
				drawText(0, 0, str, OM[win].x + 5, OM[win].y + 2, TEXT_RED, omFontSmall, TRUE);
			else
				drawText(0, 0, str, OM[win].x + 1, OM[win].y + 2, TEXT_RED, omFontSmall, TRUE);
		}
		if ((state == OM_WARM) && (win != SM_KSIZEUP) && (win != SM_KSIZEDN)) {
			int x = 0, y = OM[SM_TPNLTEXT].y;
			switch (win) {
				/*		case SM_FIRSTFRM:
							strcpy(str, "Set Start Frame");
							x = OM[win].x - 60;
							y = OM[win].y + OM[win].dy + 3;
							break;*/
			case SM_PLAY:
				strcpy(str, "Play");
				x = OM[win].x - 6;
				break;
			case SM_PAUSE:
				strcpy(str, "Pause");
				x = OM[win].x - 8;
				break;
			case SM_PREVFRAME:
				strcpy(str, "Previous Frame");
				x = OM[win].x;
				break;
			case SM_NEXTFRAME:
				strcpy(str, "Next Frame");
				x = OM[win].x - 30;
				break;
			case SM_DRAW:
				strcpy(str, "Draw Area");
				x = OM[win].x - 20;
				break;
			case SM_DRAGBOX:
				strcpy(str, "Drag a Box");
				x = OM[win].x - 30;
				break;
			case SM_KERNEL:
				strcpy(str, "Click Kernel");
				x = OM[win].x - 50;
				break;
			case SM_REMOVE:
				strcpy(str, "Remove");
				x = OM[win].x - 20;
				break;
			case SM_ADD_CUR_ROI:
				if (add_roi_flag)
					strcpy(str, "Add to ROI (active)");
				else
					strcpy(str, "Add to ROI (inactive)");
				x = OM[win].x - 30;
				break;
			case SM_THRESHOLD:
				if (threshold_flag)
					strcpy(str, "Threshold (active)");
				else
					strcpy(str, "Threshold (inactive)");
				x = OM[win].x - 30;
				break;
			case SM_TPOLARITY:
				strcpy(str, "Trace Polarity");
				x = OM[win].x - 60;
				break;
			}
			if (win - SM_DRAW == selectPixMode)
				color = TEXT_RED;
			else
				color = LTGRAY;
			drawText(0, 0, str, x, y, color, omFontRprtTn, TRUE);
		}
	}
}

void paintZoom(int win, int x, int y, int dx, int dy, int state, int attr)
{
	int color = 0;

	if (OM[win].opt->grey)
		color = colorGrayText;
	else if (state == OM_HOT) color = DKRED;
	else if (state == OM_WARM) color = colorLightShadow;
	else if (state == OM_COLD) color = colorText;
	HLine(x,x+dx,y+dy/2,color);
	HLine(x,x+dx,y+dy/2+1,color);
	HLine(x,x+dx,y+dy/2-1,color);
	if (attr == OM_ZOOMIN) {
		VLine(y,y+dy,x+dx/2,color);
		VLine(y,y+dy,x+dx/2+1,color);
		VLine(y,y+dy,x+dx/2-1,color);
	}
}

void paintArrowOrSliderFrame(int win, int x, int y, int dx, int dy, int state, int attr)
{
	int i, color;

	if (OM[OM[win].parent].state == OM_NAVIGTR || OM[win].parent == SM_MOVIEPNL || OM[win].parent == SM_CARDIOPNL) {
		if (OM[win].parent == SM_MOVIEPNL) {
			if ((!movie_data && win > SM_MVEND_DN) || (movie_scale_mode != 1 && (win <= MOV_SCMAX_DN2 && win >= MOV_SCMIN_UP))
				|| (!mov_multi_flag && (win <= MULTI_STEP_DN && win >= MULTI_F1_UP)) || (mov_multi_flag && (win <= SM_CURMOVF_DN && win >= SM_CURMOVF_UP)))
				color = colorLightShadow;
			else if (state == OM_HOT) color = DKRED;
			else if (state == OM_WARM) color = colorLightShadow;
			else if (state == OM_COLD) color = colorGrayText;
			else color = WHITE;
		}
		else if (OM[win].parent == SM_CARDIOPNL) {
			if ((!movie_data && win > SM_CAEND_DN))
				color = colorLightShadow;
			else if (state == OM_HOT) color = DKRED;
			else if (state == OM_WARM) color = colorLightShadow;
			else if (state == OM_COLD) color = colorGrayText;
			else color = WHITE;
		}
		else if (OM[win].opt->grey)
			color = colorLightShadow;
		else if (state == OM_HOT) color = DKRED;
		else if (state == OM_WARM) color = colorLightShadow;
		else if (state == OM_COLD) color = colorText;
		else color = WHITE;
		for (i = 0; i <= dx/2; ++i) {
			if (attr == OM_UPARR) {
				if (OM[win].parent != SM_FNAMEBOX && OM[win].parent != SM_MOVIEPNL && OM[win].parent != SM_CARDIOPNL) {
					HLine(x + i, x + dx - i, y + dy / 2 - i, color);
					HLine(x + dx / 4 + 1, x + dx * 3 / 4, y + dy - i, color);
				}
				else 
					HLine(x + i, x + dx - i, y + dy - i, color);
			}
			else if (attr == OM_DNARR) {
				if (OM[win].parent != SM_FNAMEBOX && OM[win].parent != SM_MOVIEPNL && OM[win].parent != SM_CARDIOPNL) {
					HLine(x + i, x + dx - i, y + dy / 2 + i, color);
					HLine(x + dx / 4 + 1, x + dx * 3 / 4, y + i, color);
				}
				else 
					HLine(x + i, x + dx - i, y + i, color);
			}
			else if (attr == OM_LTARR) {
				VLine(y+i,y+dy-i,x+dx/2-i,color);
				VLine(y+dy/4+1,y+dy*3/4,x+dx-i,color);
			}
			else if (attr == OM_RTARR) {
				VLine(y+i,y+dy-i,x+dx/2+i,color);
				VLine(y+dy/4+1,y+dy*3/4,x+i,color);
			}
		}
	}
	else if (OM[win].parent == SM_IMGSCBASE || OM[win].parent == SM_TRACEWIN || OM[win].parent == SM_BNCPNL) {
	//	x++;
	//	dx -= 2;
		if (state == OM_HOT || state == OM_WARM)
			paintNarrowArrow(x, y, dx, dy, attr, colorRegular);
		else
			paintNarrowArrow(x, y, dx, dy, attr, colorLightShadow);
	}
	else {
		if (state == OM_HOT) {
			HLine(x+1,x+dx-1,y+1,colorLightShadow );
			HLine(x+1,x+dx-1,y+dy-1,colorLight );
			VLine(y+1,y+dy-2,x+1,colorLightShadow );
			VLine(y+1,y+dy-2,x+dx-1,colorLight );
			paintSmArrow(x, y, dx, dy, attr, colorText);
		}
		else if (state == OM_WARM) {
			HLine(x+1,x+dx-1,y+1,colorLight );
			HLine(x+1,x+dx-1,y+dy-1,colorLightShadow );
			VLine(y+1,y+dy-2,x+1,colorLight );
			VLine(y+1,y+dy-2,x+dx-1,colorLightShadow );
			paintSmArrow(x, y, dx, dy, attr, colorText);
		}
		else {
			HLine(x+1,x+dx-1,y+1,colorRegular );
			HLine(x+1,x+dx-1,y+dy-1,colorRegular );
			VLine(y+1,y+dy-2,x+1,colorRegular );
			VLine(y+1,y+dy-2,x+dx-1,colorRegular );
			paintSmArrow(x, y, dx, dy, attr, colorText);
		}
	}
	//UnlockGraphics();
}

void paintFrame(int x, int y, int dx, int dy)
{
	HLine(x-1,x+dx-1,y-1,colorDarkShadow);
	HLine(x-2,x+dx,y-2,colorLightShadow);
	HLine(x-3,x+dx+2,y-3,colorRegular);
	HLine(x-4,x+dx+3,y-4,colorLight);
	
	HLine(x-1,x+dx,y+dy,colorRegular);
	HLine(x-2,x+dx+1,y+dy+1,colorLight);
	HLine(x-3,x+dx+2,y+dy+2,colorRegular);
	HLine(x-3,x+dx+3,y+dy+3,colorLightShadow);
	HLine(x-4,x+dx+4,y+dy+4,colorBlack);
	
	VLine(y,y+dy-1,x-1,colorDarkShadow);
	VLine(y-1,y+dy,x-2,colorLightShadow);
	VLine(y-2,y+dy+2,x-3,colorRegular);
	VLine(y-3,y+dy+3,x-4,colorLight);			
	
	VLine(y-1,y+dy,x+dx,colorRegular);
	VLine(y-2,y+dy+1,x+dx+1,colorLight);
	VLine(y-3,y+dy+2,x+dx+2,colorRegular);
	VLine(y-3,y+dy+3,x+dx+3,colorLightShadow);
	VLine(y-4,y+dy+4,x+dx+4,colorBlack);
}

void paintSubFrame(int x, int y, int dx, int dy, int &color)
{
	HLine(x-1,x+dx-1,y-1,color);
	HLine(x-1,x+dx,y+dy, color);
	VLine(y,y+dy-1,x-1, color);
	VLine(y-1,y+dy,x+dx, color);
}

static bool min_drawn = 0, max_drawn = 0;
void paintSlider(int win, int paintFlag)
{
	if (paintFlag == PAINTGRFX) {
		int x, y, dx, dy;

		if (win == SM_TRCGAIN || win == SM_HISTGAIN || (win == SM_BNCGAIN && !window_layout )) {	//vertical slider
			dx = OM[win].dx;
			dy = dx;
			x = OM[win].x;
			y = OM[win].y + (int)(((double)OM[win].dy*OM[win].attr) / 1000.0 + 0.5) - dy / 2;
			if (OM[win].color == BLACK) {
				VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + dx / 2, colorLightShadow);
				D3DRectangle(x, y, x + dx, y + dy, colorLightShadow);
				HLine(x + 1, x + dx - 2, y + 1, colorRegular);
				HLine(x + 1, x + dx - 2, y + dy - 1, colorDarkShadow);
				VLine(y + 1, y + dy - 3, x + 1, colorRegular);
				VLine(y, y + dy - 2, x + dx - 1, colorDarkShadow);
			}
			else {
				VLine(OM[win].y, OM[win].y + OM[win].dy, OM[win].x + dx / 2, colorLightShadow);
				D3DRectangle(x, y, x + dx, y + dy, colorRegular);
				HLine(x + 1, x + dx - 2, y + 1, colorLight);
				HLine(x + 1, x + dx - 2, y + dy - 1, colorLightShadow);
				VLine(y + 1, y + dy - 3, x + 1, colorLight);
				VLine(y, y + dy - 2, x + dx - 1, colorLightShadow);
			}
		}
		else {
			int slider_spacing = 20;
			dy = OM[win].dy;
			dx = dy;
			x = OM[win].x + (int)(((double)OM[win].dx*OM[win].attr) / 1000.0 + 0.5) - dx / 2;
			y = OM[win].y;
			if (OM[win].parent == SM_MOVIEPNL || OM[win].parent == SM_CARDIOPNL) {
				min_drawn = 0;
				max_drawn = 0;
				if (win == MOV_SCALEMIN2 || win == MOV_SCALEMAX2) {
					D3DRectangle(OM[win].x - dx / 2, OM[win-3].y-dy, OM[win].x + OM[win].dx + dx / 2, OM[win-3].y, *OM[win].color);
					D3DRectangle(OM[win].x - dx / 2, OM[win].y, OM[win].x + OM[win].dx + dx / 2, OM[win].y + dy, *OM[win].color);
				}
				else
					D3DRectangle(OM[win].x - dx / 2, OM[win].y - slider_spacing + 2, OM[win].x + OM[win].dx + dx / 2, OM[win].y + slider_spacing, *OM[win].color);
			}
			if (win == SM_LOWPASS || win == SM_HIGHPASS || win == SM_TRCEXP)
				D3DRectangle(OM[win].x - dx / 2, OM[win].y, OM[win].x + OM[win].dx + dx / 2 + 95, OM[win].y + dy, *OM[win].color);
			else
				D3DRectangle(OM[win].x - dx / 2, OM[win].y, OM[win].x + OM[win].dx + dx / 2, OM[win].y + dy, *OM[win].color);
			HLine(OM[win].x, OM[win].x + OM[win].dx, OM[win].y + dy / 2, colorLightShadow);
			D3DRectangle(x, y, x + dx, y + dy, colorRegular);
			HLine(x + 1, x + dx - 2, y + 1, colorLight);
			HLine(x + 1, x + dx - 2, y + dy - 1, colorLightShadow);
			VLine(y + 1, y + dy - 3, x + 1, colorLight);
			VLine(y, y + dy - 2, x + dx - 1, colorLightShadow);
		}
	}
	else if (paintFlag == PAINTTEXT) {
		char str[100];

		if (OM[win].parent == SM_MOVIEPNL || OM[win].parent == SM_CARDIOPNL) {
			if (win == SM_CASTART)
				sprintf(str, "Start Frame %d", mov_start + 1);
			else if (win == SM_CAEND)
				sprintf(str, "End Frame %d", mov_end + 1);
			else if (win == CUR_CA_FRM)
				sprintf(str, "Movie Frame %d", curMovFrame + 1);
			else if (win == ACTV_START)
				sprintf(str, "Activation Start: %4.2lf ms", actv_start*SM_exposure * 1000);
			else if (win == ACTV_END)
				sprintf(str, "Activation End: %4.2lf ms", actv_end*SM_exposure*1000);
			else if (win == MULTI_FRAME1)
				sprintf(str, "First Frame %d", mov_multi_f1 + 1);
			else if (win == MULTI_NUMCOL)
				sprintf(str, "Number of Columns %d", mov_multi_num_col);
			else if (win == MULTI_NUMROW)
				sprintf(str, "Number of Rows %d", mov_multi_num_row);
			else if (win == MULTI_STEP)
				sprintf(str, "Number of Frames per Step %d", mov_multi_steps);
			else if (win == SM_CURMOVF)
				sprintf(str, "Movie Frame %d", curMovFrame + 1);
			else if (win == SM_MVSTART)
				sprintf(str, "Movie Start Frame %d", mov_start + 1);
			else if (win == SM_MVEND)
				sprintf(str, "Movie End Frame %d", mov_end + 1);
			else if (win == MOV_SCALEMIN || win == MOV_SCALEMIN2)
				sprintf(str, "Scale Min (coarse/fine) %d", mov_min);
			else if (win == MOV_SCALEMAX || win == MOV_SCALEMAX2)
				sprintf(str, "Scale Max (coarse/fine) %d", mov_max);
			else if (win == MOV_DISP_CUT) 
				sprintf(str, "Pixel Display Cutoff %d", mov_cutoff);
			else if (win == MV_TRANSPARENC) 
				sprintf(str, "Transparency %d%%", (int)(movie_transparency*100 + 0.5));
			if (hotTab == SM_CARDIOVIEW) {
				if (!movie_data && win > SM_CAEND_DN)
					drawText(0, 0, str, OM[win].title_x, OM[win].title_y, colorGrayText, omFontDataSm, TRUE);
				else
					drawText(0, 0, str, OM[win].title_x, OM[win].title_y, colorBlack, omFontDataSm, TRUE);
			}
			else {
				if (win == MOV_SCALEMIN || win == MOV_SCALEMIN2) {
					if (!min_drawn) {
						drawText(0, 0, str, OM[win].title_x, OM[win].title_y, colorBlack, omFontDataSm, TRUE);
						min_drawn = 1;
					}
				}
				else if (win == MOV_SCALEMAX || win == MOV_SCALEMAX2) {
					if (!max_drawn) {
						drawText(0, 0, str, OM[win].title_x, OM[win].title_y, colorBlack, omFontDataSm, TRUE);
						max_drawn = 1;
					}
				}
				else
					drawText(0, 0, str, OM[win].title_x, OM[win].title_y, colorBlack, omFontDataSm, TRUE);
			}
		}
		else if (win == SM_TRCEXP) {
			drawText(0, 0, "X Exp", OM[win].x - 53, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
			sprintf(str, "%dX", trace_exp);
			drawText(0, 0, str, OM[win].x + OM[win].dx + 8, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
		}
		else if (win == SM_LOWPASS) {
			drawText(0, 0, "Low Pass", OM[win].x - 76, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
			sprintf(str, "%5.1lf", trace_lpass);
			while (isspace(*str))
				strcpy(str, (str+1));
			drawText(0, 0, str, OM[win].x + OM[win].dx + 7, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
		}
		else if (win == SM_HIGHPASS) {
			drawText(0, 0, "High Pass", OM[win].x - 83, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
			sprintf(str, "%5.1lf", trace_hpass);
			while (isspace(*str))
				strcpy(str, (str + 1));
			drawText(0, 0, str, OM[win].x + OM[win].dx + 7, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
		}
		else if (win == SM_TRCSTART) {
			drawText(0, 0, "X Shift", OM[win].x - 68, OM[win].y - 3, colorLightShadow, omFontDataSm, TRUE);
			sprintf(str, "%d", trace_frame0);
			drawText(0, 0, str, OM[win].x + OM[win].dx + 7, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
		}
		else if (win == SM_REFSTART) {
			drawText(0, 0, "Ref Frm", OM[win].x - 68, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
			sprintf(str, "%d", (ref_frame1 + ref_frame2)/2);
			drawText(0, 0, str, OM[win].x + OM[win].dx + 7, OM[win].y - arrow_w / 5, colorLightShadow, omFontDataSm, TRUE);
		}
	}
}

void paintDBSlider(int win)
{
	int x1, x2, y, dx, dy;

	dy = OM[win].dy;
	dx = dy;
	if (win == SM_HISTEXP) {
		if (showHistFlag) {
			x1 = OM[win].x + min(max(0, (int)((double)OM[win].dx*(histo_min - histoLeft) / (histoRight - histoLeft) + 0.5)), OM[win].dx) - dx / 2;
			x2 = OM[win].x + min(max(0, (int)((double)OM[win].dx*(histo_max - histoLeft) / (histoRight - histoLeft) + 0.5)), OM[win].dx) - dx / 2;
		}
		else {
			x1 = OM[win].x + min(max(0, (int)((double)OM[win].dx*0 / FFT_N + 0.5)), OM[win].dx) - dx / 2;
			x2 = OM[win].x + min(max(0, (int)((double)OM[win].dx*FFT_N_disp / FFT_N + 0.5)), OM[win].dx) - dx / 2;
		}
	}
	else if (win == SM_IMGSCALE) {
		x1 = OM[win].x + min(max(0, (int)((double)OM[win].dx*(scale_min-scaleLeft)/(scaleRight-scaleLeft) + 0.5)), OM[win].dx) - dx/2;
		x2 = OM[win].x + min(max(0, (int)((double)OM[win].dx*(scale_max-scaleLeft)/(scaleRight-scaleLeft) + 0.5)), OM[win].dx) - dx/2;
	}
	else {
		x1 = OM[win].x + (int)(((double)OM[win].dx*OM[win].attr)/1000.0 + 0.5) - dx/2;
		x2 = OM[win].x + (int)(((double)OM[win].dx*OM[win].attr)/1000.0 + 0.5) - dx/2;
	}
	y = OM[win].y;
//	if (OM[win].color == BLACK) {
		D3DRectangle(OM[win].x-dx/2, OM[win].y, OM[win].x+OM[win].dx+dx/2, OM[win].y+dy, *OM[win].color);
		HLine(OM[win].x, OM[win].x+OM[win].dx, OM[win].y+dy/2, colorLightShadow );
		D3DRectangle(x1,y, x1+dx,y+dy,colorLightShadow);
		D3DRectangle(x2,y, x2+dx,y+dy,colorLightShadow);
		HLine(x1+1,x1+dx-2,y+1,colorRegular );
		HLine(x1+1,x1+dx-2,y+dy-1,colorDarkShadow );
		VLine(y+1,y+dy-3,x1+1,colorRegular );
		VLine(y,y+dy-2,x1+dx-1,colorDarkShadow );
		HLine(x2+1,x2+dx-2,y+1,colorRegular );
		HLine(x2+1,x2+dx-2,y+dy-1,colorDarkShadow );
		VLine(y+1,y+dy-3,x2+1,colorRegular );
		VLine(y,y+dy-2,x2+dx-1,colorDarkShadow );
//	}
}

void paintCheck(int x, int y, int dx, int dy)
{
	int i;

	for (i = 0; i < dy/4; i++) {
		Draw_Line/*6432*/(x+2,y+dy/2,x+dx/3+1,y+dy-dy/4+i-2,GREEN);
		Draw_Line/*6432*/(x+dx/3+1,y+dy-dy/4+i-2,x+dx-3,y+2,GREEN);
	}
	Draw_Line/*6432*/(x+2,y+dy/2+1,x+dx/3+1,y+dy-2,GREEN);
	Draw_Line/*6432*/(x+dx/3+1,y+dy-2,x+dx-3,y+3,GREEN);
}

void paintColorOff(int x, int y, int dx, int dy)
{
	/*64Draw_*/D3DRectangle(x, y, x + dx, y + dy, colorWindow);
	Draw_Line/*6432*/(x,y+1,x+dx-1,y+dy,RED);
	Draw_Line/*6432*/(x,y,x+dx,y+dy,RED);
	Draw_Line/*6432*/(x+1,y,x+dx,y+dy-1,RED);
	Draw_Line/*6432*/(x,y+dy-1,x+dx-1,y,RED);
	Draw_Line/*6432*/(x,y+dy,x+dx,y,RED);
	Draw_Line/*6432*/(x+1,y+dy,x+dx,y+1,RED);
}

void paintWin(int win, int paintFlag)
{
	int i, color;

	if (paintFlag == PAINTINIT) {
		LockGraphics();
		paintWin(win, PAINTGRFX);
		UnlockGraphics();
		paintWin(win, PAINTTEXT);
		return;
	}
	if ((win >= 0) && (OM[win].state != OM_NULL) && (OM[OM[win].parent].state != OM_NULL)) {
		if (OM[win].state == OM_BUTTON)
			color = *((OM[win].anim_state == OM_COLD) ? OM[OM[win].parent].color : &colorWindow); // was LTGRAY;
		else if (OM[win].color)
			color = *(OM[win].color);
		else
			color = 0;
		if (paintFlag == PAINTGRFX) {
			if (OM[win].dx != 0 && (win <= OM_OPTQUIT) && OM[win].state < OM_NULL && (win != SM_QUESTION)) {
				if (color != colorMap[COLOROFF])
					D3DRectangle(OM[win].x, OM[win].y, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy, color); // grfx
				else
					paintColorOff(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy); // grfx
			}
			switch (OM[win].state) {
			case OM_LABEL:
				D3DRectangle(OM[win].x, OM[win].y, OM[win].x + OM[win].dx, OM[win].y + OM[win].dy, color);
				break;
			case OM_DISPLAY:
				if ((hotTab == SM_CARDIOVIEW) && (win == SM_CARDIOPNL)) {
					paintSubFrame(OM[SM_CARDIOPNL].x + 10, OM[SM_MKMAP].y - 10, OM[SM_CARDIOPNL].dx - 14, (int)(OM[SM_CARDIOPNL].dy*0.7), colorGrayText);
				}
				else if ((hotTab == SM_MOVIEVIEW) && (win == SM_MOVIEPNL)) {
					paintSubFrame(OM[SM_MOVIEPNL].x + 8, OM[SM_MKMOVIE].y - 10, OM[SM_MOVIEPNL].dx - 13, OM[SM_OV_MODE1].y - OM[SM_MKMOVIE].y + 40, colorGrayText);
					paintSubFrame(OM[SM_MOVIEPNL].x + 8, OM[MOV_SCALE_LB].y - 15, OM[SM_MOVIEPNL].dx - 13, OM[MOV_SCALEMAX2].y - OM[MOV_SCALE_LB].y + 45, colorGrayText);
					paintSubFrame(OM[SM_MOVIEPNL].x + 8, OM[MOV_MODE_LB].y - 15, OM[SM_MOVIEPNL].dx - 13, OM[MULTI_STEP].y - OM[MOV_MODE_LB].y + 45, colorGrayText);
				}
				else if (OM[win].parent == OM_NONE)
					paintFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy); // grfx
				break;
			case OM_TAB:
				paintTab(win, OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, OM[win].anim_state == OM_WARM); // grfx
				break;
			case OM_MENU:
				if (OM[win].attr == 1)
					paintSubFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, colorText); // grfx
				break;
			case OM_SCRMENU:
				paintFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy); // grfx
				break;
			case OM_SCRTRCK:
				paintSubFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, colorText); // grfx
				break;
			case OM_ZOOM:
				if (OM[win].dx != 0)
					paintZoom(win, OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, OM[win].anim_state, OM[win].attr); // grfx
				break;
			case OM_ARROW:
			case OM_THUMB:
				if (OM[win].dx != 0) {
				//	if (OM[OM[win].parent].state != OM_NAVIGTR)
				//		paintSubFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy); // grfx
					paintArrowOrSliderFrame(win, OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, OM[win].anim_state, OM[win].attr); // grfx
				}
				break;
			case OM_BUTTON:
			case OM_BUTTON2:
				if (OM[win].anim_state >= OM_WARM) {
					if (hotTab <= SM_MOVIEVIEW || (hotTab == SM_CARDIOVIEW && (win == SM_MKMAP || movie_data)))
						paintButtonFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, OM[win].anim_state != OM_WARM); 
				}
				break;
			case OM_CHCKBOX:
			case OM_CHCKBX2:
				drawChckBox(win); // grfx
				break;
			case OM_RADIOBUTTON:
				paintRadioButton(win); // grfx
				break;
			case OM_VIDEOBUTTON:
				paintVideoButton(win, PAINTGRFX); // grfx & text
				break;
			case OM_SLIDER:
				paintSlider(win, PAINTGRFX); // grft & text
				break;
			case OM_DB_SLIDER:
				paintDBSlider(win); // grfx
				break;
			}
		}
		else if (paintFlag == PAINTTEXT) {
			switch (OM[win].state) {
			case OM_TAB:
			case OM_LABEL:
			case OM_BUTTON:
			case OM_BUTTON2:
			case OM_TEXT:
			case OM_CHCKBOX:
			case OM_CHCKBX2:
			case OM_RADIOBUTTON:
				paintText(win); // text
				break;
			case OM_VIDEOBUTTON:
				paintVideoButton(win, paintFlag); // grfx & text
				break;
			case OM_SLIDER:
				paintSlider(win, PAINTTEXT); // grft & text
				break;
			}
		}
	}
	else if (win == -1)
		return;

	for (i = win + 1; i < omSizeSave; ++i)
		if (OM[i].parent == win)
			switch (OM[i].state) {
			case OM_DISPLAY:
				if (OM[i].parent == OM_NONE || OM[OM[i].parent].anim_state)
					paintWin(i, paintFlag);
				break;
			case OM_MENU:
			case OM_SCRMENU:
			case OM_SCRTRCK:
			case OM_NAVIGTR:
			case OM_ZOOM:
			case OM_ARROW:
			case OM_THUMB:
			case OM_PARA:
			case OM_TAB:
			case OM_CHCKBOX:
			case OM_CHCKBX2:
			case OM_RADIOBUTTON:
			case OM_VIDEOBUTTON:
			case OM_ICONBUTTON:
			case OM_TEXT:
			case OM_LABEL:
			case OM_BUTTON:
			case OM_BUTTON2:
			case OM_SLIDER:
			case OM_DB_SLIDER:
				paintWin(i, paintFlag);
				break;
			}
	if (win == OM_MENUBOX)
		printAcqSettings(paintFlag);
	if (OM[win].state == OM_DISPLAY && (win == SM_MOVIEWIN || win == SM_CARDIOWIN))
		DisplayMovie(paintFlag, 0);
	else if (OM[win].state == OM_DISPLAY && win == OM_MEASUREWIN)
		DisplayFrame(paintFlag);
	else if (OM[win].state == OM_DISPLAY && win == SM_TRACEWIN)
		drawTraceWin(paintFlag);
	else if (paintFlag == PAINTTEXT) {
		printImgScale(win);
		if (BNC_exist)
			paintBNClabels(win);
	}
}

int
clipX(int x)
{
	int newx;
	if (splitterMode) {
		if (draw_x0 < disp_num_col/2)
			newx = min(max(x, img_x0 + OM[OM_MEASUREWIN].x), img_x0 + OM[OM_MEASUREWIN].x + (int)((disp_num_col / 2)*zoom_factor + 0.5) - 1);
		else
			newx = min(max(x, img_x0 + OM[OM_MEASUREWIN].x + (int)((disp_num_col / 2)*zoom_factor + 0.5)), img_x0 + OM[OM_MEASUREWIN].x + (int)(disp_num_col*zoom_factor + 0.5) - 1);
	}
	else
		newx = min(max(x, img_x0 + OM[OM_MEASUREWIN].x), img_x0 + OM[OM_MEASUREWIN].x + (int)(disp_num_col*zoom_factor + 0.5) - 1);
	return newx;
}

int
clipY(int y)
{
	int newy = min(max(y,img_y0 + OM[OM_MEASUREWIN].y), img_y0 + OM[OM_MEASUREWIN].y + (int)(disp_num_row*zoom_factor + 0.5) - 1);
	return newy;
}

void
firstSegment(int x,int y,int doneFlag)
{
	traceX[d.ptr] = x;
	traceY[d.ptr] = y;
	d.x[d.ptr] = (float)x;
	d.y[d.ptr] = (float)y;
	if (!doneFlag)
		Draw_Line/*6432*/(traceX[d.ptr],traceY[d.ptr],traceX[d.ptr],traceY[d.ptr],drawColor);
	++d.ptr;
}

void
addSegment(int x,int y,int mindel,int draw)
{
	if (mindel)
		if (abs(x-traceX[d.ptr]) < mindel && abs(y - traceY[d.ptr]) < mindel)
			return;
	traceX[d.ptr] = x;
	traceY[d.ptr] = y;
	d.x[d.ptr] = (float)x;
	d.y[d.ptr] = (float)y;
	if (draw)
		Draw_Line/*6432*/(traceX[d.ptr-1],traceY[d.ptr-1],traceX[d.ptr],traceY[d.ptr],drawColor);
	++d.ptr;
}

void setScaleBox(int x, int y)
{
	int x0, y0;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	scale_pix_coor[0] = (int)(((double)dragboxX0 - img_x0 - x0)/zoom_factor);
	scale_pix_coor[1] = (int)(((double)dragboxY0 - img_y0 - y0)/zoom_factor);
	scale_pix_coor[2] = (int)(((double)x - img_x0 - x0)/zoom_factor);
	scale_pix_coor[3] = (int)(((double)y - img_y0 - y0)/zoom_factor);

	getDispImage(0);
}

int
initDraw(int mouseX, int mouseY)
{
	int x, y, x0, y0, color;

	if (!dataExist || BNCfile_only)
		return FALSE;

	if (threshold_flag)
		return TRUE;

	x0 = getMeasureWinX0();
	y0 = getMeasureWinY0();
	x = (int)(((double)mouseX - img_x0 - x0)/zoom_factor);
	y = (int)(((double)mouseY - img_y0 - y0)/zoom_factor);
	draw_x0 = x;
	if (x < 0 || x >= disp_num_col || y < 0 || y >= disp_num_row)
		return FALSE;

	x = clipX(mouseX);
	y = clipY(mouseY);
	drawColor = drawColor0;

	/*DDraw_64*/LockGraphics();
	if (selectPixMode == 1 || selectPixMode == 3 || scaleDragFlag) {
		dragboxX0 = x;
		dragboxY0 = y;
		if (scaleDragFlag)
			color = PINK;
		else if (selectPixMode == 1)
			color = drawColor;
		else
			color = colorLightShadow;
		HLine(x, x, y, color);
	}
	else {
		d.ptr = 0;
		firstSegment(x,y,FALSE);
	}
	/*DDraw_64*/UnlockGraphics();
	return TRUE;
}

void
updateDraw(int mouseX, int mouseY)
{
	int x, y, color;

	if (!dataExist || BNCfile_only || threshold_flag)
		return;
	x = clipX(mouseX);
	y = clipY(mouseY);
	if (selectPixMode == 1 || selectPixMode == 3 || scaleDragFlag) {
		LockGraphics();
		DisplayFrame(PAINTGRFX);
		if (scaleDragFlag)
			color = PINK;
		else if (selectPixMode == 1)
			color = drawColor;
		else
			color = colorLightShadow;
		HLine(dragboxX0, x, dragboxY0, color);
		HLine(dragboxX0, x, y, color);
		VLine(dragboxY0, y, dragboxX0, color);
		VLine(dragboxY0, y, x, color);
		UnlockGraphics();
		DisplayFrame(PAINTTEXT);
	}
	else {
		LockGraphics();
		addSegment(x,y,3,TRUE);
		UnlockGraphics();
	}
}

int
exitDraw(int mouseX, int mouseY)
{
	int x,y;

	if (!dataExist || BNCfile_only)
		return TRUE;
	if ((!dataFile || frameLiveFlag) && mk_omit_flag) {
		MessageBox(main_window_handle, "You need a data file display in order to make Omit Array", "message", MB_OK);
		return TRUE;
	}

	x = clipX(mouseX);
	y = clipY(mouseY);
	if (threshold_flag)
		getThresholdPix(x, y);
	else if (scaleDragFlag) {
		setScaleBox(x, y);
	}
	else if (selectPixMode == 1 || selectPixMode == 3) {
		getTracePix(x, y, 0);
	}
	else {
		/*DDraw_64*/LockGraphics();
		addSegment(x,y,3,TRUE);
		x = traceX[0];
		y = traceY[0];
		addSegment(x,y,0,TRUE);
		/*DDraw_64*/UnlockGraphics();
		getTracePix(x, y, 0);
	}
	return TRUE;
}

void
putImageLine(int x, int y, int dx, unsigned int *lineBuffer)
{
	if (x < 0)
		HImageLine(OM[OM_MEASUREWIN].x, OM[OM_MEASUREWIN].y + y, dx, lineBuffer - x);
	else
		HImageLine(OM[OM_MEASUREWIN].x + x, OM[OM_MEASUREWIN].y + y, dx, lineBuffer);
}

int
getMeasureWinX0(void)
{
	return OM[OM_MEASUREWIN].x;
}

int
getMeasureWinY0(void)
{
	return OM[OM_MEASUREWIN].y;
}

int
getMeasureWinX(void)
{
	return OM[OM_MEASUREWIN].dx;
}

int
getMeasureWinY(void)
{
	return OM[OM_MEASUREWIN].dy;
}

void
repaintMenubox()
{
	paintWin(OM_MENUBOX, PAINTINIT);
}

void imageShiftControl()
{
	int dx, dy;

	if (!dataExist)
		return;

	dx = image_region_dx;
	dy = image_region_dy;
	if ((int)(disp_num_row*zoom_factor + 0.5) <= dy)
		img_y0 = (int)((dy - disp_num_row*zoom_factor)/2 + 0.5);
	else {
		if (img_y0 > 0)
			img_y0 = 0;
		else if ((int)(img_y0+disp_num_row*zoom_factor + 0.5) < dy)
			img_y0 = (int)(dy - disp_num_row*zoom_factor + 0.5);
	}
	if ((int)(disp_num_col*zoom_factor + 0.5) <= dx)
		img_x0 = 0;
	else {
		if (img_x0 > 0)
			img_x0 = 0;
		else if ((int)(img_x0+disp_num_col*zoom_factor + 0.5) < dx)
			img_x0 = (int)(dx - disp_num_col*zoom_factor + 0.5);
	}
}

void
moveImage(int mouseX, int mouseY, int mouseX0, int mouseY0)
{
	img_x0 += mouseX - mouseX0;
	img_y0 += mouseY - mouseY0;
	paintWin(OM_MEASUREWIN, PAINTINIT);
}

void
centerZoom(double zoom_old)
{
	int curCtrX, curCtrY, dx;

	dx = min(image_region_dx, (int)(disp_num_col*zoom_old + 0.5));
	curCtrX = (int)((dx/2 - img_x0) / zoom_old + 0.5);
	img_x0 = min(0, image_region_dx / 2 - (int)(curCtrX*zoom_factor + 0.5));
	curCtrY = (int)((image_region_dy / 2 - img_y0)/ zoom_old + 0.5);
	img_y0 = (int)(image_region_dy / 2 - curCtrY*zoom_factor + 0.5);
}

void
acqDarkFrame(int autoFlag)
{
	signed short int *image_ptr,*dark_ptr;
	long int *av_ptr;
	int num_dark_av; 
	int i, x, y, len; 
	int dark_w, dark_h;

	if (demoFlag)
		return;
	SM_SetHourCursor();
/*	SM_take_Dark(darkImage, 16, pdv_chan_num, cds_lib[curConfig], layers_lib[curConfig], stripes_lib[curConfig], frame_interval, cameraType, file_frame_w, ccd_lib_bin[curConfig] / 2, QuadFlag, rotate_lib[curConfig],
		bad_pix_lib[curConfig], ndr_lib[curConfig], NDR_subtraction_frames, segment_lib[curConfig]);
	if (TwoK_flag) {
		cam_num_col = min(config_num_col * (pdv_chan_num >> 1) / (1 + cds_lib[curConfig]), file_frame_w) / max(1, ccd_lib_bin[curConfig] / 2);
		cam_num_row = 2 * config_num_row;
	}
	else {
		cam_num_col = file_width;
		cam_num_row = file_height;
	}
	
	disp_num_col = cam_num_col;
	disp_num_row = cam_num_row;

	*/

	if (TwoK_flag) {
		dark_w = min(config_num_col * (pdv_chan_num >> 1) / (1 + cds_lib[curConfig]), file_frame_w) / max(1, ccd_lib_bin[curConfig] / 2);
		dark_h = 2 * config_num_row;
	}
	else {
		dark_w = config_num_col / (cds_lib[curConfig] + 1);
		dark_h = config_num_row;
	}

	len = dark_w * dark_h * sizeof(long int);
	avImage = (long int *)_aligned_malloc(len, sizeof(long int));
	if (avImage != NULL) {
		memset(avImage, 0, len);
		num_dark_av = 16; // shift right by 2!
		for (i = 0; i < num_dark_av; i++) {
			AcquireOneFrame(0);
			for (y = 0, av_ptr = avImage, image_ptr = single_img_data; y < dark_h; y++)
				for (x = 0; x < dark_w; x++,av_ptr++,image_ptr++)
					*av_ptr = *av_ptr + *image_ptr;
		}
		for (y = 0, av_ptr = avImage, dark_ptr = darkImage; y < dark_h; y++)
			for (x = 0; x < dark_w; x++, av_ptr++, dark_ptr++)
				*dark_ptr = (short int)(*av_ptr / num_dark_av);
		_aligned_free(avImage);
		avImage = NULL;
	}
	SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
	darkExist = TRUE;
	darkSubFlag = TRUE;

//	if (!autoFlag)
		save_record_ref();

	EnableMenuItem(main_menu_handle, IDM_FILE_DARK, MF_ENABLED);
	if (!OM[SM_DKSUBBOX].attr)
		setChckBox(SM_DKSUBBOX);
	repaintMenubox();
}

void resetNDR()
{
	if (TwoK_flag)
		setNDR_pos(curConfig);	//0 to set NDR wrap position
	if (ndr_lib[curConfig])
		NDR_subtraction_flag = 1;
	else
		NDR_subtraction_flag = 0;
	if (!OM[SM_DKSUBBOX].attr && NDR_subtraction_flag)
		setChckBox(SM_DKSUBBOX);
}

void reset2KSettings()
{
	char ref_name[MAX_PATH];
	DeleteFile(strcat(strcpy(ref_name, tmp_file_dir), "\\record_ref.tsm"));
	dataExist = FALSE;
	darkExist = FALSE;
	d_darkExist = FALSE;
	deactivateImgMenu();
	darkSubFlag = NDR_subtraction_flag;
	if (OM[SM_DKSUBBOX].attr && !darkSubFlag)
		setChckBox(SM_DKSUBBOX);
	resetZoom(1);
}

void changeZoom(int zoom_in_out)
{
	int drawMenuFlag = FALSE;
	double zoom_old;

	zoom_old = zoom_factor;
	zoom_factor += 0.25*zoom_in_out;
	if (zoom_factor < min_zoom)
		zoom_factor = min_zoom;
	if (zoom_factor > max_zoom)
		zoom_factor = max_zoom;
	if (zoom_factor != zoom_old) {
		centerZoom(zoom_old);
		if (zoom_in_out > 0) {
			if ((int)(img_x0 + disp_num_col * zoom_factor + 0.5) > image_region_dx)
				drawMenuFlag = TRUE;
			if ((int)(img_y0 + disp_num_row * zoom_factor + 0.5) > image_region_dy)
				drawMenuFlag = TRUE;
			if (zoom_factor >= max_zoom || OM[OM_OBJDNARR].opt->grey == TRUE) {
				setZoomGray();
				drawMenuFlag = TRUE;
			}
		}
		else {
			if ((int)(img_x0 + disp_num_col * zoom_factor + 0.5) <= image_region_dx) {
				img_x0 = min(0, (int)(image_region_dx - disp_num_row * zoom_factor + 0.5));
				drawMenuFlag = TRUE;
			}
			if ((int)(img_y0 + disp_num_row * zoom_factor + 0.5) <= image_region_dy) {
				img_y0 = (int)((image_region_dy - disp_num_col * zoom_factor) / 2 + 0.5);
				drawMenuFlag = TRUE;
			}
			if (zoom_factor == min_zoom || OM[OM_OBJUPARR].opt->grey == TRUE) {
				setZoomGray();
				drawMenuFlag = TRUE;
			}
		}

		if (drawMenuFlag) {
			imageShiftControl();
			repaintMenubox();
		}
		paintWin(curTab(), PAINTINIT);
	}
}

void getOverlayImage(int tif_flag)
{
	ov_num_col = disp_num_col;
	ov_num_row = disp_num_row;
	ovFile_num_col = disp_num_col;
	ovFile_num_row = disp_num_row;
	if (overlay_img != NULL)
		_aligned_free(overlay_img);
	overlay_img = (signed short int *)_aligned_malloc((unsigned long)ov_num_col*ov_num_row * sizeof(signed short int), 2);
	get_overlay_flag = 1;
	getDispImage(0);
	get_overlay_flag = 0;
	if (movie_data != NULL)
		overlay_mode = 1;
	else
		overlay_mode = 2;
}

void MakeMovie()
{
	int m, l, j, k, numTraceFrames_sv;
	long bin_val;
	float ref_val;

	signed short int *ptr, *mptr, *b_ptr, *k_ptr, *fr_ptr;
	long len = disp_num_col * disp_num_row;
	long mov_pix_stt = len * mov_start;

	numTraceFrames_sv = numTraceFrames;
	numTraceFrames = mov_end - mov_start + 1;
	numMovFrames = numTraceFrames;
	if (movie_data != NULL)
		_aligned_free(movie_data);
	movie_data = (signed short int *)_aligned_malloc(numTraceFrames*(disp_num_col / movie_bin)*(disp_num_row / movie_bin) * sizeof(signed short int), 2);
	if (mov_trace_scale_min != NULL)
		_aligned_free(mov_trace_scale_min);
	mov_trace_scale_min = (signed short int *)_aligned_malloc((disp_num_col / movie_bin)*(disp_num_row / movie_bin) * sizeof(signed short int), 2);
	if (mov_trace_scale_max != NULL)
		_aligned_free(mov_trace_scale_max);
	mov_trace_scale_max = (signed short int *)_aligned_malloc((disp_num_col / movie_bin)*(disp_num_row / movie_bin) * sizeof(signed short int), 2);
	EnableMenuItem(main_menu_handle, IDM_SAVE_MOVIE, MF_ENABLED);
	EnableMenuItem(main_menu_handle, IDM_SAVE_MOVIE2, MF_ENABLED);

	HWND hwndPB = createProgBar();
	int step_cnt = 0;
	double step_size;
	char str[256];
//	sprintf(str, "Scaled Subtraction for pixel %d of %d", 0, len);
//	SetWindowText(hwndPB, str);

	mov_bin_used = movie_bin;
	if (movie_bin > 1) {
		step_cnt = 0;
		step_size = ((double)num_steps) / numTraceFrames;
		SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
		fr_ptr = image_data + mov_pix_stt;
		b_ptr = movie_data;
		for (int i = 0; i < numTraceFrames; i++) {
			ptr = fr_ptr;
			for (m = 0; m < disp_num_row / movie_bin; m++) {
				for (l = 0; l < disp_num_col / movie_bin; l++) {
					bin_val = 0;
					k_ptr = ptr;
					for (j = 0; j < movie_bin; j++) {
						for (k = 0; k < movie_bin; k++)
							bin_val += *k_ptr++;
						k_ptr += disp_num_col - movie_bin;
					}
					*b_ptr++ = (short)(bin_val / (movie_bin*movie_bin));
					ptr += movie_bin;
				}
				ptr += disp_num_col * (movie_bin - 1);
			}
			fr_ptr += len;
			sprintf(str, "Spatial binning frame %d of %d", i, numTraceFrames);
			SetWindowText(hwndPB, str);
			while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
				step_cnt++;
			}
		}
		mov_num_col = disp_num_col / movie_bin;
		mov_num_row = disp_num_row / movie_bin;
	}
	else {
		mov_num_col = disp_num_col;
		mov_num_row = disp_num_row;
		fr_ptr = image_data + mov_pix_stt;
		memcpy(movie_data, fr_ptr, len*numTraceFrames * 2);
	}

	len = mov_num_col * mov_num_row;
	step_cnt = 0;
	step_size = ((double)num_steps) / len;
	SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
	signed short *min_pt, *max_pt;
	min_pt = mov_trace_scale_min;
	max_pt = mov_trace_scale_max;
	int omit_this = 0;
	for (long i = 0; i < len; i++) {
		mptr = movie_data + i;
		ptr = movie_data + i;
		if (omit_ar) {
			if (movie_bin > 1) {
				omit_this = 0;
				int omit_y = (i / mov_num_col)*movie_bin;
				int omit_x = (i % mov_num_col)*movie_bin;
				for (m = omit_y; m < omit_y + movie_bin; m++)
					for (k = omit_x; k < omit_x + movie_bin; k++)
						omit_this += *(omit_ar + m* mov_num_col*movie_bin + k);
				omit_this /= movie_bin * movie_bin;
			}
			else
				omit_this = *(omit_ar + i);
		}
		if (omit_this) {
			for (m = 0; m < numTraceFrames; m++) {
				*mptr = mov_omit_val;
				mptr += len;
			}
			*min_pt++ = mov_omit_val;
			*max_pt++ = mov_omit_val;
		}
		else {
			for (m = 0; m < numTraceFrames; m++) {
				this_smTrace[m] = *ptr;
				if (tracePolarity < 0 && mov_flt_flag)
					this_smTrace[m] = this_smTrace[0] * 2 - this_smTrace[m];
				ptr += len;
			}
			if (mov_flt_flag)
				FilterTrace();
			ref_val = this_smTrace[0];
			signed short this_min = 32765, this_max = -32765;
			for (m = 0; m < numTraceFrames; m++) {
				if (mov_ref_sub)
					*mptr = (short int)(this_smTrace[m] - ref_val);
				else
					*mptr = (short int)(this_smTrace[m]);
				if (this_min > *mptr)
					this_min = *mptr;
				if (this_max < *mptr)
					this_max = *mptr;
				mptr += len;
			}
			*min_pt++ = this_min;
			*max_pt++ = this_max;
		}
		sprintf(str, "Filter dataset for pixel %d of %d", i, len);
		SetWindowText(hwndPB, str);
		while ((int)(i*step_size + 0.5) >= step_cnt && step_cnt < num_steps) {
			SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
		}
	}

	if (sp_lp_type || sp_hp_type) {
		ptr = movie_data;
		for (m = 0; m < numTraceFrames; m++) {
			spatial_filter(m+1);
		}
	}

	if (overlay_img != NULL)
		overlay_mode = 1;
	else
		overlay_mode = 0;
	mov_cutoff = -1000;
	DestroyWindow(hwndPB);
	numTraceFrames = numTraceFrames_sv;
	numMovFrames = mov_end - mov_start + 1;
	actv_start = min(actv_start, numMovFrames - 1);
	setMovieSlider(-1, ACTV_START);
	actv_end = min(actv_end, numMovFrames - 1);
	setMovieSlider(-1, ACTV_END);
	if (activation_fr_ar != NULL)
		_aligned_free(activation_fr_ar);
	activation_fr_ar = NULL;
	actv_flag = false;
	OM[SM_MAPBOX2].attr = actv_flag;
	if (apd_ar != NULL)
		_aligned_free(apd_ar);
	apd_ar = NULL;
	apd_flag = false;
	OM[SM_MAPBOX3].attr = apd_flag;
	if (cv_x_ar != NULL)
		_aligned_free(cv_x_ar);
	cv_x_ar = NULL;
	if (cv_y_ar != NULL)
		_aligned_free(cv_y_ar);
	cv_y_ar = NULL;
	if (cv_ar != NULL)
		_aligned_free(cv_ar);
	cv_ar = NULL;
	cv_flag = false;
	OM[SM_MAPBOX4].attr = cv_flag;
}

static int mov_coarse_scl_unit = 200, cutoff_ext = 100;
void checkMovScale()
{
//	int mov_cutoff_sv = mov_cutoff;

	if (mov_min > mov_max)
		mov_max = mov_min;
	if (mov_min < 0) {
		mov_min1 = mov_min / mov_coarse_scl_unit - 1;
		mov_min2 = mov_min % mov_coarse_scl_unit + mov_coarse_scl_unit;
	}
	else {
		mov_min1 = mov_min / mov_coarse_scl_unit;
		mov_min2 = mov_min % mov_coarse_scl_unit;
	}
	if (mov_max < 0) {
		mov_max1 = mov_max / mov_coarse_scl_unit - 1;
		mov_max2 = mov_max % mov_coarse_scl_unit + mov_coarse_scl_unit;
	}
	else {
		mov_max1 = mov_max / mov_coarse_scl_unit;
		mov_max2 = mov_max % mov_coarse_scl_unit;
	}
	setMovieSlider(-1, MOV_SCALEMIN);
	setMovieSlider(-1, MOV_SCALEMIN2);
	setMovieSlider(-1, MOV_SCALEMAX);
	setMovieSlider(-1, MOV_SCALEMAX2);

//	mov_cutoff = max(min(mov_cutoff, mov_max - 1), mov_min - cutoff_ext);
	setMovieSlider(-1, MOV_DISP_CUT);
}

void setMovieSlider(int mouseX, int win)
{
	int slider_val;
	int hf_num_unit = 16383 / mov_coarse_scl_unit;

	if (mouseX >= 0) {
		if (hotTab == SM_MOVIEVIEW) {
			if ((!movie_data && win > SM_MVEND) || (movie_scale_mode != 1 && win >= MOV_SCALEMIN && win <= MOV_SCALEMAX2)
				|| (!mov_multi_flag && (win <= MULTI_STEP && win >= MULTI_FRAME1)) || (mov_multi_flag && (win == SM_CURMOVF)))
				return;
		}
		else if (hotTab == SM_CARDIOVIEW) {
			if ((!movie_data && win > SM_CAEND))
				return;
		}
		slider_val = (mouseX - OM[win].x) * 1000 / OM[win].dx;
		OM[win].attr = min(max(0, slider_val), 1000);
		switch (win)
		{
		case MULTI_FRAME1:
			mov_multi_f1 = max(0, min(OM[win].attr*(numMovFrames) / 1000, numMovFrames - 1));
			break;
		case MULTI_NUMCOL:
			mov_multi_num_col = max(1, min(OM[win].attr*(max_multi_numf + 1) / 1000, max_multi_numf));
			break;
		case MULTI_NUMROW:
			mov_multi_num_row = max(1, min(OM[win].attr*(max_multi_numf + 1) / 1000, max_multi_numf));
			break;
		case MULTI_STEP:
			mov_multi_steps = max(1, min(OM[win].attr*51 / 1000, 50));
			break;
		case SM_CURMOVF:
			curMovFrame = max(0, min(OM[win].attr*(numMovFrames) / 1000, numMovFrames - 1));
			break;
		case CUR_CA_FRM:
			curMovFrame = max(0, min(OM[win].attr*(numMovFrames) / 1000, numMovFrames - 1));
			break;
		case ACTV_START:
			actv_start = min(OM[win].attr*numTraceFrames / 1000, actv_end - 1);
			OM[win].attr = (int)min(actv_start * 1000 / numTraceFrames, 1000);
			paintWin(curTab(), PAINTINIT);
			break;
		case ACTV_END:
			actv_end = min(max(OM[win].attr*numTraceFrames / 1000, actv_start + 1), numMovFrames - 1);
			OM[win].attr = (int)min(actv_end * 1000 / numTraceFrames, 1000);
			paintWin(curTab(), PAINTINIT);			
			break;
		case SM_CASTART:
		case SM_MVSTART:
			mov_start = min(OM[win].attr*numTraceFrames / 1000, mov_end - 1);
			OM[win].attr = (int)min(mov_start * 1000 / numTraceFrames, 1000);
			break;
		case SM_CAEND:
		case SM_MVEND:
			mov_end = min(max(OM[win].attr*numTraceFrames / 1000, mov_start + 1), numTraceFrames-1);
			OM[win].attr = (int)min(mov_end * 1000 / numTraceFrames, 1000);
			break;
		case MOV_SCALEMIN:
			mov_min1 = min(OM[win].attr * (2* hf_num_unit) / 1000 - hf_num_unit, mov_max1);
			mov_min = mov_min1 * mov_coarse_scl_unit + mov_min2;
			OM[win].attr = (int)min((mov_min1 + hf_num_unit) * 1000 / (2 * hf_num_unit), 1000);
			checkMovScale();
			break;
		case MOV_SCALEMIN2:
			mov_min2 = OM[win].attr * mov_coarse_scl_unit / 1000;
			mov_min = mov_min1 * mov_coarse_scl_unit + mov_min2;
			checkMovScale();
			break;
		case MOV_SCALEMAX:
			mov_max1 = max(OM[win].attr* (2* hf_num_unit) / 1000 - hf_num_unit, mov_min1);
			mov_max = mov_max1 * mov_coarse_scl_unit + mov_max2;
			OM[win].attr = (int)min((mov_max1 + hf_num_unit) * 1000 / (2 * hf_num_unit), 1000);
			checkMovScale();
			break;
		case MOV_SCALEMAX2:
			mov_max2 = OM[win].attr* mov_coarse_scl_unit / 1000;
			mov_max = mov_max1 * mov_coarse_scl_unit + mov_max2;
			checkMovScale();
			break;
		case MOV_DISP_CUT:
			mov_cutoff = OM[win].attr*(mov_max-mov_min+cutoff_ext) / 1000 + mov_min - cutoff_ext;
			break;
		case MV_TRANSPARENC:
			movie_transparency = (double)OM[win].attr / 1000;
			break;
		}
		if (win > SM_MVEND || win == CUR_CA_FRM)
			paintWin(curTab(), PAINTINIT);
		else
			paintWin(win, PAINTINIT);
	}
	else {
		switch (win)
		{
		case MULTI_FRAME1:
			OM[win].attr = (int)min(mov_multi_f1 * 1000 / numMovFrames, 1000);
			break;
		case MULTI_NUMCOL:
			OM[win].attr = (int)min(mov_multi_num_col * 1000 / max_multi_numf, 1000);
			break;
		case MULTI_NUMROW:
			OM[win].attr = (int)min(mov_multi_num_row * 1000 / max_multi_numf, 1000);
			break;
		case MULTI_STEP:
			OM[win].attr = (int)min(mov_multi_steps * 1000 / 50, 1000);
			break;
		case SM_CURMOVF:
			OM[win].attr = (int)min(curMovFrame * 1000 / numMovFrames, 1000);
			break;
		case CUR_CA_FRM:
			OM[win].attr = (int)min(curMovFrame * 1000 / numMovFrames, 1000);
			break;
		case ACTV_START:
			OM[win].attr = (int)min(actv_start * 1000 / numTraceFrames, 1000);
			break;
		case ACTV_END:
			OM[win].attr = (int)min(actv_end * 1000 / numTraceFrames, 1000);
			break;
		case SM_CASTART:
		case SM_MVSTART:
			OM[win].attr = (int)min(mov_start * 1000 / numTraceFrames, 1000);
			break;
		case SM_CAEND:
		case SM_MVEND:
			OM[win].attr = (int)min(mov_end * 1000 / numTraceFrames, 1000);
			break;
		case MOV_SCALEMIN:
			OM[win].attr = (int)min((mov_min1 + hf_num_unit) * 1000 / (2* hf_num_unit), 1000);
			break;
		case MOV_SCALEMIN2:
			OM[win].attr = (int)min(mov_min2 * 1000 / mov_coarse_scl_unit, 1000);
			break;
		case MOV_SCALEMAX:
			OM[win].attr = (int)min((mov_max1 + hf_num_unit) * 1000 / (2* hf_num_unit), 1000);
			break;
		case MOV_SCALEMAX2:
			OM[win].attr = (int)min(mov_max2 * 1000 / mov_coarse_scl_unit, 1000);
			break;
		case MOV_DISP_CUT:
			OM[win].attr = (int)min(max(0,(mov_cutoff-mov_min+ cutoff_ext))  * 1000 / (mov_max-mov_min+ cutoff_ext), 1000);
			break;
		case MV_TRANSPARENC:
			OM[win].attr = (int)min(movie_transparency * 1000, 1000);
			break;
		}
	}
}

void updateSpatialFiler(int hpass)
{
	char *lp_types[] = { "No LPass Filter", "3x3 Mean", "3x3 Gaussian" };
	char *hp_types[] = { "No HPass Filter", "Laplacian" };
	if (!hpass) {	//lpass
		if ((sp_lp_type != sp_lp_type_sv) || ((sp_lpass != sp_lpass_sv) && sp_lp_type)) {
			if (sp_lp_type)
				sprintf(sp_lp_str, "%s %d", lp_types[sp_lp_type], sp_lpass);
			else
				sprintf(sp_lp_str, "%s", lp_types[sp_lp_type]);
			OM[SP_LP_LABEL].title = sp_lp_str;
			paintWin(SP_LP_LABEL, PAINTINIT);
			getDispImage(0);
		}
		sp_lp_type_sv = sp_lp_type;
		sp_lpass_sv = sp_lpass;
	}
	else {
		if ((sp_hp_type != sp_hp_type_sv) || ((sp_hpass != sp_hpass_sv) && sp_hp_type)) {
			if (sp_hp_type)
				sprintf(sp_hp_str, "%s %dx%d", hp_types[sp_hp_type], sp_hpass*2+1, sp_hpass*2+1);
			else
				sprintf(sp_hp_str, "%s", hp_types[sp_hp_type]);
			OM[SP_HP_LABEL].title = sp_hp_str;
			if (sp_hp_type)
				auto_skip_v2 = -32765;
			paintWin(SP_HP_LABEL, PAINTINIT);
			getDispImage(0);
		}
		sp_hp_type_sv = sp_hp_type;
		sp_hpass_sv = sp_hpass;
	}
}

int procWin(int win)
{
	BOOL retval;
	char new_biopsy[_MAX_FNAME], data_dir[_MAX_PATH];
	char *p;
	int configSave, gainSave, nFrameSave, file_frame_w_save, splitterAlignMode_save, NDR_num_f_sv;
	int firstDFrame_old;
	int mov_min_sv, mov_max_sv;
	char tmp_str[256];
	double f_intervalSave;
	int showImageFlag=FALSE, switch_flag=FALSE;
    static char *noneLeft  =   "Nothing to erase",
                *sureMsg[] = { "Erase Parameter?",
                               "Erase All?"        };

	if (!isRadioButton(win) && !isVideoButton(win) && !isIconButton(win) && !isImgScButton(win) && !isTraceButton(win) && (OM[win].parent != SM_BNCPNL)) {
		if (OM[win].parent == SM_MOVIEPNL || OM[win].parent == SM_CARDIOPNL) {
			if (isThumb(win))
				return TRUE;
			if (OM[win].parent == SM_CARDIOPNL && !movie_data && win > SM_CAEND_DN)
				return FALSE;
		}
		else if (isThumb(win) || OM[win].opt->grey)
			return TRUE;
	}
	else if (isIconButton(win) && (OM[win].attr == OM_GRAY))
		return TRUE;
	else if (hotTab == SM_MOVIEVIEW) {
		if (!movie_data)
			return TRUE;
	}
	else if (!seqFlag) {
		if (hotTab == OM_MEASUREVIEW) {
			if ((PlayFlag && win != SM_PAUSE) || (!PlayFlag && win == SM_PAUSE) || (win - SM_DRAW == selectPixMode))
				return TRUE;
			else if ((firstDataFrame == 0 && win == SM_PREVREAD) || (firstDataFrame + numTraceFrames == numDataFrames && win == SM_NEXTREAD))
				return TRUE;
		}
		else {
			if (PlayFlag && win != MOV_PAUSE && win != CARDIO_PAUSE)
				return TRUE;
		}
	}

	if (logFlag && log_fp)
		fprintf(log_fp, "In Procwin(), win = %d\n", win);

    switch (win) {
	case SM_MAPBOX2:
		if (activation_fr_ar) {
			actv_flag = !actv_flag;
			OM[SM_MAPBOX2].attr = actv_flag;
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case MAP_CALC2:
		if (movie_data) {
			getActivationMap();
			actv_flag = true;
			OM[SM_MAPBOX2].attr = actv_flag;
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_MAPBOX3:
		if (apd_ar) {
			apd_flag = !apd_flag;
			OM[SM_MAPBOX3].attr = apd_flag;
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case MAP_CALC3:
		if (movie_data && activation_fr_ar) {
			getAPD_map();
			apd_flag = true;
			OM[SM_MAPBOX3].attr = apd_flag;
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_MAPBOX4:
		if (cv_ar) {
			cv_flag = !cv_flag;
			OM[SM_MAPBOX4].attr = cv_flag;
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case MAP_CALC4:
		if (movie_data && activation_fr_ar) {
			getConductionMap();
			cv_flag = true;
			OM[SM_MAPBOX4].attr = cv_flag;
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_MKMAP:
	case SM_MKMOVIE:
		MakeMovie();
		if (win == SM_MKMOVIE)
			setMovieSlider(-1, SM_CURMOVF);
		else
			setMovieSlider(-1, CUR_CA_FRM);
		paintWin(curTab(), PAINTINIT);
		break;
	case SM_RDOVRLAY:
		popup_flag = TRUE;
		PopFileInitializeTiff(main_window_handle);
		strcat(strcat(strcpy(data_dir, home_dir), dirIMAGES), "\\");
		strcat(strcpy(new_biopsy, data_dir), experiment);
		retval = PopFileOpenDlg(main_window_handle, new_biopsy, new_biopsy);
		if (retval) {
			SM_readTIFF_stack(new_biopsy, 1);
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_DATAOVLY:
		getOverlayImage(0);
		paintWin(curTab(), PAINTINIT);
		break;
	case CARDIO_COLOR:
	case CARDIO_BW:
		if ((ap_color != win - CARDIO_COLOR) && (movie_data != NULL)) {
			ap_color = win - CARDIO_COLOR;
			if (ap_color) {
				OM[CARDIO_COLOR].attr = OM_COLD;
				OM[CARDIO_BW].attr = OM_HOT;
			}
			else {
				OM[CARDIO_COLOR].attr = OM_HOT;
				OM[CARDIO_BW].attr = OM_COLD;
			}
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_APD50:
	case SM_APD80:
		if ((apd_mode != win - SM_APD50) && (movie_data != NULL)) {
			apd_mode = win - SM_APD50;
			if (apd_mode) {
				OM[SM_APD50].attr = OM_COLD;
				OM[SM_APD80].attr = OM_HOT;
				OM[SM_MAPBOX3].title = "APD80";
			}
			else {
				OM[SM_APD50].attr = OM_HOT;
				OM[SM_APD80].attr = OM_COLD; 
				OM[SM_MAPBOX3].title = "APD50";
			}
			paintWin(SM_MAPBOX3, PAINTINIT);
			paintWin(SM_APD50, PAINTINIT);
			paintWin(SM_APD80, PAINTINIT);
		}
		break;
	case SM_OV_MODE1:
	case SM_OV_MODE2:
	case SM_OV_MODE3:
		if ((overlay_mode != win - SM_OV_MODE1) && (movie_data != NULL) && (overlay_img != NULL)) {
			overlay_mode = win - SM_OV_MODE1;
			if (overlay_mode > 0) {
				ov_num_col = ovFile_num_col;
				ov_num_row = ovFile_num_row;
			}
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_MV_SCALE1:
	case SM_MV_SCALE2:
	case SM_MV_SCALE3:
		if ((movie_scale_mode != win - SM_MV_SCALE1) && (movie_data != NULL)) {
			movie_scale_mode = win - SM_MV_SCALE1;
			setMvScButtons();
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case MOV_MOVIE:
	case MOV_MULTI:
		if ((mov_multi_flag != win - MOV_MOVIE) && (movie_data != NULL)) {
			mov_multi_flag = win - MOV_MOVIE;
			setMvMdButtons();
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_MVSUBBOX:
		mov_ref_sub = !mov_ref_sub;
		setChckBox(win);
		paintWin(win, PAINTINIT);
		break;
	case CV_BINUP:
		if (cv_bin * 2 < min(mov_num_col, mov_num_row) / 4) {
			cv_bin = cv_bin * 2;
			sprintf(mov_bin_str, "Binning for Conduction %d", cv_bin);
			OM[CV_BINLB].title = mov_bin_str;
			paintWin(CV_BINLB, PAINTINIT);
		}
		break;
	case CV_BINDN:
		if (cv_bin / 2 >= 1) {
			cv_bin = cv_bin / 2;
			sprintf(mov_bin_str, "Binning for Conduction %d", cv_bin);
			OM[CV_BINLB].title = mov_bin_str;
			paintWin(CV_BINLB, PAINTINIT);
		}
		break;
	case SM_CABINUP:
	case SM_MVBINUP:
		if (movie_bin * 2 < min(disp_num_col, disp_num_row) / 4) {
			movie_bin = movie_bin * 2;
			sprintf(mov_bin_str, "Digital Bin %d", movie_bin);
			if (hotTab == SM_CARDIOVIEW) {
				OM[SM_CABINLB].title = mov_bin_str;
				paintWin(SM_CABINLB, PAINTINIT);
			}
			else {
				OM[SM_MVBINLB].title = mov_bin_str;
				paintWin(SM_MVBINLB, PAINTINIT);
			}
		}
		break;
	case SM_CABINDN:
	case SM_MVBINDN:
		if (movie_bin / 2 >= 1) {
			movie_bin = movie_bin / 2;
			sprintf(mov_bin_str, "Digital Bin %d", movie_bin);
			if (hotTab == SM_CARDIOVIEW) {
				OM[SM_CABINLB].title = mov_bin_str;
				paintWin(SM_CABINLB, PAINTINIT);
			}
			else {
				OM[SM_MVBINLB].title = mov_bin_str;
				paintWin(SM_MVBINLB, PAINTINIT);
			}
		}
		break;
	case SM_MVFLTBOX:
		mov_flt_flag = !mov_flt_flag;
		setChckBox(win);
		paintWin(win, PAINTINIT);
		break;
	case SM_CURMOVF_UP:
	case CUR_CA_FRM_UP:
		curMovFrame = max(0, min(curMovFrame + 1, numMovFrames - 1));
		paintWin(curTab(), PAINTINIT);
		break;
	case SM_CURMOVF_DN:
	case CUR_CA_FRM_DN:
		curMovFrame = max(curMovFrame - 1, 0);
		paintWin(curTab(), PAINTINIT);
		break;
	case SM_MVSTART_UP:
	case SM_CASTART_UP:
		mov_start = min(mov_start + 1, mov_end - 1);
		if (hotTab == SM_CARDIOVIEW) {
			setMovieSlider(-1, SM_CASTART);
			paintWin(SM_CASTART, PAINTINIT);
		}
		else {
			setMovieSlider(-1, SM_MVSTART);
			paintWin(SM_MVSTART, PAINTINIT);
		}
		break;
	case SM_CASTART_DN:
	case SM_MVSTART_DN:
		mov_start = max(mov_start - 1, 0);
		if (hotTab == SM_CARDIOVIEW) {
			setMovieSlider(-1, SM_CASTART);
			paintWin(SM_CASTART, PAINTINIT);
		}
		else {
			setMovieSlider(-1, SM_MVSTART);
			paintWin(SM_MVSTART, PAINTINIT);
		}
		break;
	case SM_CAEND_UP:
	case SM_MVEND_UP:
		mov_end = min(mov_end + 1, numTraceFrames - 1);
		if (hotTab == SM_CARDIOVIEW) {
			setMovieSlider(-1, SM_CAEND);
			paintWin(SM_CAEND, PAINTINIT);
		}
		else {
			setMovieSlider(-1, SM_MVEND);
			paintWin(SM_MVEND, PAINTINIT);
		}
		break;
	case SM_CAEND_DN:
	case SM_MVEND_DN:
		mov_end = max(mov_end - 1, mov_start + 1);
		if (hotTab == SM_CARDIOVIEW) {
			setMovieSlider(-1, SM_CAEND);
			paintWin(SM_CAEND, PAINTINIT);
		}
		else {
			setMovieSlider(-1, SM_MVEND);
			paintWin(SM_MVEND, PAINTINIT);
		}
		break;
	case ACTV_START_UP:
		actv_start = min(actv_start + 1, actv_end - 1);
		setMovieSlider(-1, ACTV_START);
		paintWin(curTab(), PAINTINIT);
		break;
	case ACTV_START_DN:
		actv_start = max(actv_start - 1, 0);
		setMovieSlider(-1, ACTV_START);
		paintWin(curTab(), PAINTINIT);
		break;
	case ACTV_END_UP:
		actv_end = min(actv_end + 1, numMovFrames - 1);
		setMovieSlider(-1, ACTV_END);
		paintWin(curTab(), PAINTINIT);
		break;
	case ACTV_END_DN:
		actv_end = max(actv_end - 1, actv_start + 1);
		setMovieSlider(-1, ACTV_END);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_F1_UP:
		mov_multi_f1 = min(mov_multi_f1 + 1, numMovFrames - 1);
		setMovieSlider(-1, MULTI_FRAME1);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_F1_DN:
		mov_multi_f1 = max(mov_multi_f1 - 1, 0);
		setMovieSlider(-1, MULTI_FRAME1);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_NUMC_UP:
		mov_multi_num_col = min(mov_multi_num_col + 1, max_multi_numf);
		setMovieSlider(-1, MULTI_NUMCOL);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_NUMC_DN:
		mov_multi_num_col = max(mov_multi_num_col - 1, 1);
		setMovieSlider(-1, MULTI_NUMCOL);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_NUMR_UP:
		mov_multi_num_row = min(mov_multi_num_row + 1, max_multi_numf);
		setMovieSlider(-1, MULTI_NUMROW);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_NUMR_DN:
		mov_multi_num_row = max(mov_multi_num_row - 1, 1);
		setMovieSlider(-1, MULTI_NUMROW);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_STEP_UP:
		mov_multi_steps = min(mov_multi_steps + 1, 50);
		setMovieSlider(-1, MULTI_STEP);
		paintWin(curTab(), PAINTINIT);
		break;
	case MULTI_STEP_DN:
		mov_multi_steps = max(mov_multi_steps - 1, 1);
		setMovieSlider(-1, MULTI_STEP);
		paintWin(curTab(), PAINTINIT);
		break;
	case MOV_SCMIN_DN:
	case MOV_SCMIN_UP:
	case MOV_SCMIN_DN2:
	case MOV_SCMIN_UP2:
	case MOV_SCMAX_DN:
	case MOV_SCMAX_UP:
	case MOV_SCMAX_DN2:
	case MOV_SCMAX_UP2:
		if (movie_scale_mode == 1) {
			mov_min_sv = mov_min;
			mov_max_sv = mov_max;
			if (win == MOV_SCMIN_DN)
				mov_min = max(mov_min - mov_coarse_scl_unit, -16383);
			else if (win == MOV_SCMIN_DN2)
				mov_min = max(mov_min - 1, -16383);
			else if (win == MOV_SCMAX_DN)
				mov_max = max(mov_max - mov_coarse_scl_unit, mov_min);
			else if (win == MOV_SCMAX_DN2)
				mov_max = max(mov_max - 1, mov_min);
			else if (win == MOV_SCMIN_UP)
				mov_min = min(mov_min + mov_coarse_scl_unit, mov_max);
			else if (win == MOV_SCMIN_UP2)
				mov_min = min(mov_min + 1, mov_max);
			else if (win == MOV_SCMAX_UP)
				mov_max = min(mov_max + mov_coarse_scl_unit, 16383);
			else if (win == MOV_SCMAX_UP2)
				mov_max = min(mov_max + 1, 16383);
			checkMovScale();
			if (mov_min != mov_min_sv || mov_max != mov_max_sv)
				paintWin(curTab(), PAINTINIT);
		}
		break;
	case MOV_DISPCUT_UP:
		mov_cutoff++;
		checkMovScale();
		paintWin(curTab(), PAINTINIT);
		break;
	case MOV_DISPCUT_DN:
		mov_cutoff--;
		checkMovScale();
		paintWin(curTab(), PAINTINIT);
		break;
	case MV_TRANSP_UP:
		movie_transparency = min((movie_transparency*100 + 1.0)/100, 1.0);
		if (overlay_mode == 1)
			paintWin(curTab(), PAINTINIT);
		else
			setMovieSlider(-1, MV_TRANSPARENC); {
			paintWin(MV_TRANSPARENC, PAINTINIT);
		}
		break;
	case MV_TRANSP_DN:
		movie_transparency = max((movie_transparency * 100 - 1.0) / 100, 0.0);
		if (overlay_mode == 1)
			paintWin(curTab(), PAINTINIT);
		else
			setMovieSlider(-1, MV_TRANSPARENC); {
			paintWin(MV_TRANSPARENC, PAINTINIT);
		}
		break;
	case OM_OBJUPARR:
		changeZoom(1);
		break;
	case OM_OBJDNARR:
		changeZoom(-1);
		break;
	case OM_FLDLTARR:
	case OM_FLDRTARR:
	case OM_FLDUPARR:
	case OM_FLDDNARR: 
		MessageBox(main_window_handle, "Shift-drag to move the image", "message", MB_OK);
		break;
	case SM_BNC1: 
	case SM_BNC2: 
	case SM_BNC3: 
	case SM_BNC4:
	case SM_BNC5: 
	case SM_BNC6: 
	case SM_BNC7: 
	case SM_BNC8:
		if (OM[win].attr == OM_COLD) {
			OM[win].attr = OM_HOT;
			getTracePix(0, 0, win-SM_BNC1+1);
		}
		else {
			OM[win].attr = OM_COLD;
			getTracePix(0, 0, -(win-SM_BNC1+1));
		}
		break;
	case SM_BNC_FLTR:
		BNC_filter_flag = !BNC_filter_flag;
		if (BNC_filter_flag)
			OM[win].attr = OM_HOT;
		else
			OM[win].attr = OM_COLD;
		paintWin(win, PAINTINIT);
		getAllTraces();
		paintWin(SM_TRACEWIN, PAINTINIT);
		break;
	case SM_RESET:
	//	initDDbuffer();
		chipgainSave = -1;
		config_loaded = 0;
		SM_initCamera(1,1,1);
		paintWin(OM_MEASUREWIN, PAINTINIT);
		paintWin(SM_TRACEWIN, PAINTINIT);
		break;
	case SM_CSETTING:
		popup_flag = TRUE;
		nFrameSave = curNFrames;
		configSave = curConfig;
		gainSave = curCamGain;
		chipgainSave = curChipGain;
		f_intervalSave = frame_interval;
		file_frame_w_save = file_frame_w;
		splitterAlignMode_save = splitterAlignMode;
		NDR_num_f_sv = NDR_num_f;
		newDarkFlag = 0;
		if (getCameraSetting()) {
			SM_SetHourCursor();
			if ((configSave != curConfig) || (gainSave != curCamGain) || (chipgainSave != curChipGain) || 
				((float)f_intervalSave != (float)frame_interval) || (nFrameSave != curNFrames) || (NDR_num_f_sv != NDR_num_f)) {
				if ((configSave != curConfig) || (gainSave != curCamGain) || (chipgainSave != curChipGain) ||
					((float)f_intervalSave != (float)frame_interval))
					newDarkFlag = 1;
				if (TwoK_flag && NDR_pos)
					setNDR_pos(configSave);	//to restore NDR seq
				SM_initCamera(1,1,1);
				resetNDR();
			}
			if (TwoK_flag && file_frame_w_save != file_frame_w) {
				if (file_frame_w < 10)
					file_frame_w = 10;
				else if (file_frame_w >(pdv_chan_num >> 1)*config_num_col / (1 + cds_lib[curConfig]))
					file_frame_w = (pdv_chan_num >> 1)*config_num_col / (1 + cds_lib[curConfig]);
				reset2KSettings();
			}

			repaintMenubox();
			paintWin(OM_MEASUREWIN, PAINTINIT);
			paintWin(SM_TRACEWIN, PAINTINIT);
			SM_SetNonHourCursor(crossCursorOn ? crossCursor : arrowCursor);
		}
		else
			splitterAlignMode = splitterAlignMode_save;
		break;
	case SM_CAPTURE:
		filePHT_flag = 0;
		if (auto_dark_flag)
			acqDarkFrame(1);
		openShutter();
		AcquireOneFrame(1);
		closeShutter();
		break;
    case SM_STREAM: 
		if (num_trials > 1) {
			if (spike_triggered && !num_traces) {
				MessageBox(main_window_handle, "Please select a BNC or a ROI for Spike Triggered Average", "message", MB_OK);
				break;
			}
			acquiring_flag = 1;
			memset(spike_pos_ar, 0, sizeof(spike_pos_ar));
		}
		av_file_cnt = 0;
		StreamDataSM();
		break;
    case SM_LIVE: 
		filePHT_flag = 0;
		runCam_flag = !runCam_flag;
		if (SFC_flag)
			SFC_scan_control(runCam_flag);
		if (runCam_flag) {
			if (auto_dark_flag)
				acqDarkFrame(1);
			openShutter();
			OM[SM_LIVE].title = "Stop";
			setBGrey_Live(0);
		}
		else {
			closeShutter();
			OM[SM_LIVE].title = "Live";
			setBGrey_Stop(0);
		}
        break;
    case SM_DKFRAME: 
		if (ndr_lib[curConfig])
			MessageBox(main_window_handle,"Single dark frame in NDR mode is meaningless. Use Record.", "message", MB_OK);
		else {
			MessageBox(main_window_handle,"16 Frames will be acquired and averaged for the Dark Frame. Make sure there is no light going to the camera sensor.", "message", MB_OK);
			acqDarkFrame(0);
			AcquireOneFrame(1);
		}
		break;
	case SM_AUTOOFFSET:
		MessageBox(main_window_handle,"Make sure there is no light going to the camera sensor.", "message", MB_OK);
		AutoSetOffset(0);
		break;
    case SM_DKSUBBOX:
	case SM_DARKSUB:
		darkSubFlag = !darkSubFlag;
		setChckBox(SM_DKSUBBOX);
		repaintMenubox();
		getDispImage(0);
		paintWin(SM_TRACEWIN, PAINTINIT);
		break;
    case SM_AUTOSBOX:
	case SM_AUTOSCL:
		autoScaleFlag = !autoScaleFlag;
		setChckBox(SM_AUTOSBOX);
		repaintMenubox();
		UpdateMScale();
		getDispImage(0);
		break;
    case SM_HLOCKBOX:
	case SM_LOCKHIST:
		lockHistFlag = !lockHistFlag;
		setChckBox(SM_HLOCKBOX);
		repaintMenubox();
		paintWin(OM_MEASUREWIN, PAINTINIT);
		break;
	case SM_FOCUSBOX:
    case SM_FOCUS:
		focusMode = !focusMode;
		setFocusRep(focusMode);
		setChckBox(SM_FOCUSBOX);
		repaintMenubox();
		break;	
	case SM_OVWRTBOX:
    case SM_OVERWRT:
		overwriteFlag = !overwriteFlag;
		setChckBox(SM_OVWRTBOX);
		repaintMenubox();
		break;	
    case SM_BROWSE:
		popup_flag = TRUE;
		strcat(strcat(strcpy(data_dir, home_dir), dirIMAGES), "\\");
		strcat(strcat(strcat(strcpy(new_biopsy, data_dir), experiment), "\\"), biopsy);
		retval = PopFileOpenDlg(main_window_handle, new_biopsy, new_biopsy);
		if (retval) {
			if (p = strstr(new_biopsy, data_dir)) {
	//			strcpy(str, (p+(int)strlen(data_dir)));
				get_experiment_biopsy(new_biopsy);
				repaintMenubox();
			}
			else
				MessageBox(main_window_handle, "Please keep the data in SMDATA\\", "message", MB_OK);
		}
		break;	
	case SM_FNAMEUPARR:
		trial_cnt++;
		getAcq_FileName(trial_cnt);
		repaintMenubox();
		break;	
	case SM_FNAMEDNARR:
		trial_cnt--;
		trial_cnt = max(trial_cnt, 1);
		getAcq_FileName(trial_cnt);
		repaintMenubox();
		break;	
	case MOV_PLAY:
	case CARDIO_PLAY:
		if (movie_data)
			PlayFlag = TRUE;
		break;
	case MOV_PAUSE:
	case CARDIO_PAUSE:
		if (movie_data && PlayFlag) {
			PlayFlag = FALSE;
			paintWin(curTab(), PAINTINIT);
		}
		break;
	case SM_PLAY:
		if (dataFile) {
			movie_rep_cnt = 0;
			PlayFlag = TRUE;
			setToolBGrey();
		}
		break;
	case SM_PAUSE:
		if (dataFile && PlayFlag) {
			PlayFlag = FALSE;
			setToolBGrey();
		}
		break;
	case SM_PREVFRAME:
		playMovieFrame(1);
		break;
	case SM_NEXTFRAME:
		playMovieFrame(0);
		break;
	case SM_DRAW:
		selectPixMode = DRAW_MODE;
		paintWin(SM_TOOLPNL, PAINTINIT);
		break;
	case SM_DRAGBOX:
		selectPixMode = DRAG_MODE;
		paintWin(SM_TOOLPNL, PAINTINIT);
		break;
	case SM_KERNEL:
		selectPixMode = KERNEL_MODE;
		paintWin(SM_TOOLPNL, PAINTINIT);
		break;
	case SM_REMOVE:
		selectPixMode = DRAG_ERASE;
		paintWin(SM_TOOLPNL, PAINTINIT);
		break;
	case SM_ADD_CUR_ROI:
		add_roi_flag = !add_roi_flag;
		paintWin(SM_TOOLPNL, PAINTINIT);
		break;
	case SM_THRESHOLD:
		threshold_flag = !threshold_flag;
		paintWin(SM_TOOLPNL, PAINTINIT);
		break;
	case SM_TPOLARITY:
		tracePolarity = -tracePolarity;
		getAllTraces();
		paintWin(SM_TOOLPNL, PAINTINIT);
		paintWin(SM_TRACEWIN, PAINTINIT);
		break;
	case SM_KSIZEUP:
		sm_kernel_size++;
		sm_kernel_size = min(sm_kernel_size, 99);
		paintWin(SM_KERNEL, PAINTINIT);
		if (disp_setting_handle) {
			sprintf(tmp_str, "%d", sm_kernel_size);
			SetDlgItemText(disp_setting_handle, KERNEL_SIZE_TX, tmp_str);
		}
		break;
	case SM_KSIZEDN:
		sm_kernel_size--;
		sm_kernel_size = max(sm_kernel_size, 1);
		paintWin(SM_KERNEL, PAINTINIT);
		if (disp_setting_handle) {
			sprintf(tmp_str, "%d", sm_kernel_size);
			SetDlgItemText(disp_setting_handle, KERNEL_SIZE_TX, tmp_str);
		}
		break;
	case SM_FIRSTFRM:
		firstDFrame_old = firstDataFrame;
		getFirstDataFrame();
		if (firstDFrame_old != firstDataFrame) {
			SM_readData(SM_dataname);
			paintWin(OM_MEASUREWIN, PAINTINIT);
		}
		break;
	case SM_PREVREAD:
		if (seqFlag) {
			curSeq--;
			curSeq = max(0, curSeq);
			setCurSequence();
		}
		else {
			firstDataFrame = max(0, firstDataFrame - dataReadBlock / 2);
			SM_readData(SM_dataname);
		}
		paintWin(OM_MEASUREWIN, PAINTINIT);
		break;
	case SM_NEXTREAD:
		if (seqFlag) {
			curSeq++;
			curSeq = min(7, curSeq);
			setCurSequence();
		}
		else {
			firstDataFrame = min(numDataFrames - dataReadBlock, firstDataFrame + dataReadBlock / 2);
			SM_readData(SM_dataname);
		}
		paintWin(OM_MEASUREWIN, PAINTINIT);
		break;
	case SM_TRCEXP_UP:
	case SM_TRCEXP_DN:
		if (dataFile) {
			if (win == SM_TRCEXP_UP)
				trace_exp++;
			else
				trace_exp--;
			if (trace_exp < 1)
				trace_exp = 1;
			if (trace_exp > trace_exp_max)
				trace_exp = trace_exp_max;
			setTraceExp(-1);
		}
		break;
	case SM_TRCSTT_UP:
	case SM_TRCSTT_DN:
		if (dataFile) {
			if (win == SM_TRCSTT_UP)
				trace_frame0++;
			else
				trace_frame0--;
			trace_frame0 = max(0, min(trace_frame0, numTraceFrames - 1));
			OM[SM_TRCSTART].attr = min(trace_frame0 * 1000 / (numTraceFrames - 1), 1000);
			paintWin(SM_TRACEWIN, PAINTINIT);
		}
		break;
	case SM_REFSTT_UP:
	case SM_REFSTT_DN:
		if (dataFile) {
			if (win == SM_REFSTT_UP) {
				ref_frame1++;
				ref_frame2++;
			}
			else {
				ref_frame1--;
				ref_frame2--;
			}
			ref_frame1 = min(max(ref_frame1, 0), numDataFrames - 1);
			ref_frame2 = min(max(ref_frame1, ref_frame2), numDataFrames - 1);
			OM[SM_REFSTART].attr = min((ref_frame1 + ref_frame2) * 500 / (numTraceFrames - 1), 1000);
			if (f_subtract_mode) {
				getRefImage();
				getStreamFrame();
			}
			paintWin(SM_TRACEWIN, PAINTINIT);
		}
		break;
	case SM_TRCGNUP:
	case SM_TRCGNDN:
		if (dataFile) {
			if (win == SM_TRCGNUP)
				traceYGain += 10;
			else
				traceYGain -= 10;
			traceYGain = min(1000, max(0, traceYGain));
			OM[SM_TRCGAIN].attr = 1000 - traceYGain;
			paintWin(SM_TRACEWIN, PAINTINIT);
		}
		break;
	case SM_BNCGNUP:
	case SM_BNCGNDN:
		if (dataFile) {
			if (win == SM_BNCGNUP)
				BNC_YGain += 10;
			else
				BNC_YGain -= 10;
			BNC_YGain = min(1000, max(0, BNC_YGain));
			if (!window_layout)
				OM[SM_BNCGAIN].attr = 1000 - BNC_YGain;
			else
				OM[SM_BNCGAIN].attr = BNC_YGain;
			paintWin(SM_BNCPNL, PAINTINIT);
			paintWin(SM_TRACEWIN, PAINTINIT);
		}
		break;
	case SM_LOWPASS_UP:
	case SM_LOWPASS_DN:
		if (dataFile) {
			if (win == SM_LOWPASS_UP)
				trace_lpass += trace_lpass_max * 0.01;
			else
				trace_lpass -= trace_lpass_max * 0.01;
			if (trace_lpass < 0.0)
				trace_lpass = 0.0;
			if (trace_lpass > trace_lpass_max)
				trace_lpass = trace_lpass_max;
			setTraceLowPass(-1, 1);;
		}
		break;
	case SM_HIGHPASS_UP:
	case SM_HIGHPASS_DN:
		if (dataFile) {
			if (win == SM_HIGHPASS_UP)
				trace_hpass += trace_hpass_max * 0.01;
			else
				trace_hpass -= trace_hpass_max * 0.01;
			if (trace_hpass < 0.0)
				trace_hpass = 0.0;
			if (trace_hpass > trace_hpass_max)
				trace_hpass = trace_hpass_max;
			setTraceHighPass(-1, 1);;
		}
		break;
	case SP_LPTYPE_DN:
		sp_lp_type--;
		sp_lp_type = max(sp_lp_type, 0);
		updateSpatialFiler(0);
		break;
	case SP_LPTYPE_UP:
		sp_lp_type++;
		sp_lp_type = min(sp_lp_type, 2);
		updateSpatialFiler(0);
		break;
	case SP_LP_DN:
		sp_lpass--;
		sp_lpass = max(sp_lpass, 1);
		updateSpatialFiler(0);
		break;
	case SP_LP_UP:
		sp_lpass++;
		sp_lpass = min(sp_lpass, 10);
		updateSpatialFiler(0);
		break;
	case SP_HPTYPE_DN:
		sp_hp_type--;
		sp_hp_type = max(sp_hp_type, 0);
		updateSpatialFiler(1);
		break;
	case SP_HPTYPE_UP:
		sp_hp_type++;
		sp_hp_type = min(sp_hp_type, 1);
		updateSpatialFiler(1);
		break;
	case SP_HP_DN:
		sp_hpass--;
		sp_hpass = max(sp_hpass, 1);
		updateSpatialFiler(1);
		break;
	case SP_HP_UP:
		sp_hpass++;
		sp_hpass = min(sp_hpass, 4);
		updateSpatialFiler(1);
		break;
	case SM_IMGSCLUP:
	case SM_IMGSCLDN:
	case SM_IMGSCRUP:
	case SM_IMGSCRDN:
		if (autoScaleFlag) {
			autoScaleFlag = 0;
			setChckBox(SM_AUTOSBOX);
			repaintMenubox();
		}
		switch (win) {
		case SM_IMGSCLUP:
			scale_min++;
			scale_min = min(scale_max-1, scale_min);
			break;
		case SM_IMGSCLDN:
			scale_min--;
			scale_min = max(scaleLeft, scale_min);
			break;
		case SM_IMGSCRUP:
			scale_max++;
			scale_max = min(scaleRight, scale_max);
			break;
		case SM_IMGSCRDN:
			scale_max--;
			scale_max = max(scale_min+1, scale_max);
			break;
		}
		paintWin(OM_MEASUREWIN, PAINTINIT);
		UpdateMScale();
		break;
	case SM_QUESTION:
		displayHelp();
		break;
	case OM_OPTQUIT:return (MessageBox(main_window_handle,
									(LPCTSTR) "Do you really want to quit?",
									(LPCTSTR) "Quit Turbo-SM",
									MB_YESNO |
									MB_ICONQUESTION |
									MB_SYSTEMMODAL) != IDYES);
		break;
    }
	return TRUE;
}



void paintText(int win)
{
	int color;

	if (hotTab == SM_CARDIOVIEW && !movie_data && win > SM_MKMAP)
		color = colorGrayText;
	else
		color = colorText;
	if (OM[win].state == OM_RADIOBUTTON)
		drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x, OM[win].title_y, color, omFontSmall, TRUE);
	else if (OM[win].state == OM_BUTTON2)
		drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x, OM[win].title_y, color, omFontMedium, TRUE);
	else if (OM[win].state == OM_CHCKBX2)
		drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x, OM[win].title_y, color, omFontMedium, TRUE);
	else if (OM[win].state == OM_TEXT)
		drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x,OM[win].title_y,*OM[win].color, omFontMedium, TRUE);
	else if (OM[win].state == OM_BUTTON) {
		char buf[2] = " ";

		if (win == SM_FOCUS)
			drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x,OM[win].title_y,OM[win].opt->grey? colorGrayText:OM[win].color?*OM[win].color:colorBlack, omFontSmall, TRUE);
		else if (win >= SM_DARKSUB && win <= SM_LOCKHIST)
			drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x,OM[win].title_y,OM[win].opt->grey? colorGrayText:OM[win].color?*OM[win].color:colorBlack, omFontMedium, TRUE);
		else
			drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x,OM[win].title_y,OM[win].opt->grey? colorGrayText:OM[win].color?*OM[win].color:colorBlack, omFontBig, TRUE);
//		if (win <= OM_OPTQUIT)
//			drawText(0, 0, OM[win].opt->keystr,OM[win].opt->keystr_x,OM[win].opt->keystr_y,TEXT_RED, omFontSmall, FALSE);
		if (!OM[win].opt->grey) {
			buf[0] = OM[win].title[OM[win].opt->hilite];
			if (OM[win].opt->hilite_x)
				drawText(0, -1, buf, OM[win].opt->hilite_x,OM[win].title_y,TEXT_RED, omFontBigFat, TRUE);
		}
	}
	else if (OM[win].state == OM_LABEL) {
		if (hotTab == SM_CARDIOVIEW) 
			drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x, OM[win].title_y, color, omFontMedium, TRUE);
		else if (OM[win].parent == SP_FLTPNL)
			drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x, OM[win].title_y, colorLightShadow, omFontSmall, TRUE);
		else
			drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x, OM[win].title_y, colorBlack, omFontMedium, TRUE);
	//	if (OM[win].opt->draw_fn)
	//		(*OM[win].opt->draw_fn)();
	} 
	else
		drawText(0, OM[win].bitmap_index, OM[win].title, OM[win].title_x, OM[win].title_y,colorText, omFontSmall, FALSE);
}

static bool
winTest(OMTYPE *p, int x, int y)
{
	return (x >= p->x && x <= p->x + p->dx - 1 && y >= p->y && y <= p->y + p->dy - 1);
}

static bool
vSliderTest(OMTYPE *p, int x, int y)
{
	return (x >= p->x && x <= p->x + p->dx - 1 && y >= p->y - p->dx/2 && y <= p->y + p->dy + p->dx/2 - 1);
}

static bool
hSliderTest(OMTYPE *p, int x, int y)
{
	return (x >= p->x - p->dy/2 && x <= p->x + p->dx + p->dy/2 - 1 && y >= p->y && y <= p->y + p->dy - 1);
}

int windowTest(int x, int y)
{
	int win, foundWin = -1, foundTop = -2, insideTop = FALSE;

	for (win = 0; win < omSize; ++win)
		if (OM[win].state != OM_NULL && OM[OM[win].parent].state != OM_NULL) {
			if (winTest(&OM[win],x,y)) {
				if (dataFile && (hotTab == SM_CARDIOVIEW)) {
					if (win == SM_CARDIOPNL) {
						for (int i = SM_CARDIOPNL + 1; i < SM_MOVIEPNL; i++)
							if (winTest(&OM[i], x, y)) return i;
						return SM_CARDIOPNL;
					}
					else if (win == SM_CARDIOWIN)
						return SM_CARDIOWIN;
				}
				else if (dataFile && (hotTab == SM_MOVIEVIEW)) {
					if (win == SM_MOVIEPNL) {
						for (int i = SM_MOVIEPNL + 1; i <= MOV_MULTI; i++)
							if (winTest(&OM[i], x, y)) 
								return i;
						return SM_MOVIEPNL;
					}
					else if (win == SM_MOVIEWIN)
						return SM_MOVIEWIN;
				}
				else if (hotTab == OM_MEASUREVIEW) {
					if (win == OM_MEASUREWIN) {
						if (dataExist) {
							if (winTest(&OM[SM_FIRSTFRM], x, y)) return SM_FIRSTFRM;
							else if (winTest(&OM[SM_NEXTREAD], x, y)) return SM_NEXTREAD;
							else if (winTest(&OM[SM_PREVREAD], x, y)) return SM_PREVREAD;
							else if (vSliderTest(&OM[SM_HISTGAIN], x, y) && !window_layout) return SM_HISTGAIN;
							else if (hSliderTest(&OM[SM_HISTEXP], x, y) && !window_layout) return SM_HISTEXP;
							else if (winTest(&OM[SM_IMGSCBASE], x, y)) {
								if (hSliderTest(&OM[SM_IMGSCALE], x, y)) return SM_IMGSCALE;
								else if (winTest(&OM[SM_IMGSCLUP], x, y)) return SM_IMGSCLUP;
								else if (winTest(&OM[SM_IMGSCLDN], x, y)) return SM_IMGSCLDN;
								else if (winTest(&OM[SM_IMGSCRUP], x, y)) return SM_IMGSCRUP;
								else if (winTest(&OM[SM_IMGSCRDN], x, y)) return SM_IMGSCRDN;
								else return SM_IMGSCBASE;
							}
							else if (winTest(&OM[SP_FLTPNL], x, y)) {
								for (int i = SP_FLTPNL + 1; i <= SP_HP_DN; i++)
									if (winTest(&OM[i], x, y)) return i;
								return SP_FLTPNL;
							}
							else if (winTest(&OM[SM_TOOLPNL], x, y)) {
								for (int i = SM_TOOLPNL + 1; i <= SM_KSIZEDN; i++)
									if (winTest(&OM[i], x, y)) return i;
								return SM_TOOLPNL;
							}
							else if (winTest(&OM[SM_BNCPNL], x, y)) {
								if (vSliderTest(&OM[SM_BNCGAIN], x, y)) return SM_BNCGAIN;
								else if (vSliderTest(&OM[SM_BNCGNUP], x, y)) return SM_BNCGNUP;
								else if (vSliderTest(&OM[SM_BNCGNDN], x, y)) return SM_BNCGNDN;
								else {
									for (int i = SM_BNC1; i <= SM_BNC_FLTR; i++)
										if (winTest(&OM[i], x, y) && (OM[i].state != OM_NULL)) return i;
								}
								return SM_BNCPNL;
							}
						}
						else if (seqFlag) {
							if (winTest(&OM[SM_NEXTREAD], x, y)) return SM_NEXTREAD;
							else if (winTest(&OM[SM_PREVREAD], x, y)) return SM_PREVREAD;
						}
					}
					else if (win == SM_TRACEWIN) {
						if (winTest(&OM[SM_QUESTION], x, y)) return SM_QUESTION;
						else if (vSliderTest(&OM[SM_TRCGAIN], x, y)) return SM_TRCGAIN;
						else if (vSliderTest(&OM[SM_TRCGNUP], x, y)) return SM_TRCGNUP;
						else if (vSliderTest(&OM[SM_TRCGNDN], x, y)) return SM_TRCGNDN;
						else if (hSliderTest(&OM[SM_TRCEXP], x, y)) return SM_TRCEXP;
						else if (hSliderTest(&OM[SM_TRCEXP_UP], x, y)) return SM_TRCEXP_UP;
						else if (hSliderTest(&OM[SM_TRCEXP_DN], x, y)) return SM_TRCEXP_DN;
						else if (hSliderTest(&OM[SM_LOWPASS], x, y)) return SM_LOWPASS;
						else if (hSliderTest(&OM[SM_LOWPASS_UP], x, y)) return SM_LOWPASS_UP;
						else if (hSliderTest(&OM[SM_LOWPASS_DN], x, y)) return SM_LOWPASS_DN;
						else if (hSliderTest(&OM[SM_HIGHPASS], x, y)) return SM_HIGHPASS;
						else if (hSliderTest(&OM[SM_HIGHPASS_UP], x, y)) return SM_HIGHPASS_UP;
						else if (hSliderTest(&OM[SM_HIGHPASS_DN], x, y)) return SM_HIGHPASS_DN;
						else if (hSliderTest(&OM[SM_TRCSTART], x, y)) return SM_TRCSTART;
						else if (hSliderTest(&OM[SM_TRCSTT_UP], x, y)) return SM_TRCSTT_UP;
						else if (hSliderTest(&OM[SM_TRCSTT_DN], x, y)) return SM_TRCSTT_DN;
						else if (hSliderTest(&OM[SM_REFSTART], x, y)) return SM_REFSTART;
						else if (hSliderTest(&OM[SM_REFSTT_UP], x, y)) return SM_REFSTT_UP;
						else if (hSliderTest(&OM[SM_REFSTT_DN], x, y)) return SM_REFSTT_DN;
						else if (vSliderTest(&OM[SM_HISTGAIN], x, y) && (window_layout == 1)) return SM_HISTGAIN;
						else if (hSliderTest(&OM[SM_HISTEXP], x, y) && (window_layout == 1)) return SM_HISTEXP;
						else return SM_TRACEWIN;
					}
				}
				if (OM[win].state == OM_TAB || OM[win].state == OM_BUTTON || OM[win].state == OM_ARROW || OM[win].state == OM_DRAWFIELD || OM[win].state == OM_BUTTON2 ||
					OM[win].state == OM_THUMB || OM[win].state == OM_PARA || OM[win].state == OM_CHCKBOX || OM[win].state == OM_CHCKBX2 || OM[win].state == OM_ZOOM ||
					OM[win].state == OM_RADIOBUTTON || OM[win].state == OM_VIDEOBUTTON || OM[win].state == OM_ICONBUTTON ||
					(OM[win].state == OM_DISPLAY && OM[win].anim_state == OM_WARM && OM[OM[win].parent].anim_state == OM_WARM && OM[win].parent != OM_NONE)) return win;
			}
		}
	return foundWin;
}

void disableSidePnl(bool flag)
{
	OM[SM_DARKSUB].opt->grey = flag;
	OM[SM_AUTOSCL].opt->grey = flag;
	OM[SM_LOCKHIST].opt->grey = flag;
	OM[SM_DKSUBBOX].opt->grey = flag;
	OM[SM_AUTOSBOX].opt->grey = flag;
	OM[SM_HLOCKBOX].opt->grey = flag;

	if (noCameraFlag)
		flag = TRUE;
	OM[SM_RESET].opt->grey = flag;
	OM[SM_CSETTING].opt->grey = flag;
	OM[SM_CAPTURE].opt->grey = flag;
	OM[SM_LIVE].opt->grey = flag;
	OM[SM_STREAM].opt->grey = flag;
	OM[SM_DKFRAME].opt->grey = flag;
	OM[SM_FOCUS].opt->grey = flag;
	OM[SM_FOCUSBOX].opt->grey = flag;
	OM[SM_OVERWRT].opt->grey = flag;
	OM[SM_OVWRTBOX].opt->grey = flag;
	OM[SM_BROWSE].opt->grey = flag;
	OM[SM_FNAMEUPARR].opt->grey = flag;
	OM[SM_FNAMEDNARR].opt->grey = flag;

	if (!flag) {
		if (offset_chan_order[0] < 0 || cds_lib[curConfig] || TwoK_flag)
			OM[SM_AUTOOFFSET].opt->grey = TRUE;
		else
			OM[SM_AUTOOFFSET].opt->grey = FALSE;
	}
	else 
		OM[SM_AUTOOFFSET].opt->grey = TRUE;
}

void setMPColorButtons()
{
	for (int i = CARDIO_COLOR; i <= CARDIO_BW; ++i) {
		if (i - CARDIO_COLOR == ap_color)
			OM[i].attr = OM_HOT;
		else
			OM[i].attr = OM_COLD;
	}

}

void setAPDButtons()
{
	for (int i = SM_APD50; i <= SM_APD80; ++i) {
		if (i - SM_APD50 == apd_mode)
			OM[i].attr = OM_HOT;
		else
			OM[i].attr = OM_COLD;
	}

}

void setOverlayButtons()
{
	if (!overlay_img)
		overlay_mode = 0;
	else if (!movie_data)
		overlay_mode = 2;
	for (int i = SM_OV_MODE1; i <= SM_OV_MODE3; ++i) {
		if (i - SM_OV_MODE1 == overlay_mode)
			OM[i].attr = OM_HOT;
		else
			OM[i].attr = OM_COLD;
	}

}

void setMvScButtons()
{
	for (int i = SM_MV_SCALE1; i <= SM_MV_SCALE3; ++i) {
		if (i - SM_MV_SCALE1 == movie_scale_mode)
			OM[i].attr = OM_HOT;
		else
			OM[i].attr = OM_COLD;
	}

}

void setMvMdButtons()
{
	for (int i = MOV_MOVIE; i <= MOV_MULTI; ++i) {
		if (i - MOV_MOVIE == mov_multi_flag)
			OM[i].attr = OM_HOT;
		else
			OM[i].attr = OM_COLD;
	}

}

int setTabOn(int win)
{
	if (BNCfile_only && win == SM_MOVIEVIEW) {
		MessageBox(main_window_handle, "Movie Display is not available for BNC only files", "message", MB_OK);
		return 0;
	}
	if (BNCfile_only && win == SM_CARDIOVIEW) {
		MessageBox(main_window_handle, "Cardio Analysis is not available for BNC only files", "message", MB_OK);
		return 0;
	}
	if (win != hotTab) {
		if (hotTab != -1)
			OM[hotTab].anim_state = OM_COLD;
		OM[win].anim_state = OM_WARM;
		hotTab = win;
		setToolPanel(0);
		if (hotTab == OM_MEASUREVIEW) {
			OM[OM_MEASUREWIN].state = OM_DISPLAY;
			OM[SM_TRACEWIN].state = OM_DISPLAY;
			OM[SM_QUESTION].state = OM_VIDEOBUTTON;
			OM[SM_CARDIOWIN].state = OM_NULL;
			OM[SM_CARDIOPNL].state = OM_NULL;
			OM[SM_MOVIEWIN].state = OM_NULL;
			OM[SM_MOVIEPNL].state = OM_NULL;
			OM[MOV_PLAY].state = OM_NULL;
			OM[MOV_PAUSE].state = OM_NULL;
			OM[CARDIO_PLAY].state = OM_NULL;
			OM[CARDIO_PAUSE].state = OM_NULL;
			enableImgScl(1);
			disableSidePnl(0);
			getDispImage(0);
		}
		else if (hotTab == SM_MOVIEVIEW) {
			overlay_mode = 0;
			OM[OM_MEASUREWIN].state = OM_NULL;
			OM[SM_TRACEWIN].state = OM_NULL;
			OM[SM_QUESTION].state = OM_NULL;
			OM[SM_CARDIOWIN].state = OM_NULL;
			OM[SM_CARDIOPNL].state = OM_NULL;
			OM[SM_MOVIEWIN].state = OM_DISPLAY;
			enableImgScl(0);
			if (dataFile) {
				OM[SM_MOVIEPNL].state = OM_DISPLAY;
				OM[MOV_PLAY].state = OM_VIDEOBUTTON;
				OM[MOV_PAUSE].state = OM_VIDEOBUTTON;
				OM[SM_MVSUBBOX].attr = mov_ref_sub;
				OM[SM_MVFLTBOX].attr = mov_flt_flag;
				sprintf(mov_bin_str, "Digital Bin %d", movie_bin);
				OM[SM_MVBINLB].title = mov_bin_str;

				setOverlayButtons();
				setMvScButtons();
				setMvMdButtons();

				numMovFrames = mov_end - mov_start + 1;
				if (!curMovFrame)
					curMovFrame = curDispFrame - mov_start;
				curMovFrame = max(0, min(curMovFrame, numMovFrames - 1));
				for (int i = SM_MVSTART; i <= MULTI_STEP; i += 3)
					setMovieSlider(-1, i);
			}
			disableSidePnl(1);
		}
		else {
			OM[OM_MEASUREWIN].state = OM_NULL;
			OM[SM_TRACEWIN].state = OM_NULL;
			OM[SM_QUESTION].state = OM_NULL;
			OM[SM_MOVIEWIN].state = OM_NULL;
			OM[SM_MOVIEPNL].state = OM_NULL;
			OM[SM_CARDIOWIN].state = OM_DISPLAY;
			resetZoom(0);
			enableImgScl(0);
			if (dataFile) {
				mov_flt_flag = 1;
				OM[SM_CARDIOPNL].state = OM_DISPLAY;
				OM[CARDIO_PLAY].state = OM_VIDEOBUTTON;
				OM[CARDIO_PAUSE].state = OM_VIDEOBUTTON;
				OM[SM_MAPBOX1].attr = true;
				OM[SM_MAPBOX2].attr = actv_flag;
				OM[SM_MAPBOX3].attr = apd_flag;
				OM[SM_MAPBOX4].attr = cv_flag;
				sprintf(mov_bin_str, "Digital Bin %d", movie_bin);
				OM[SM_CABINLB].title = mov_bin_str;
				sprintf(mov_bin_str, "Binning for Conduction %d", cv_bin);
				OM[CV_BINLB].title = mov_bin_str;

				setMPColorButtons();
				setAPDButtons();

				numMovFrames = mov_end - mov_start + 1;
				if (!curMovFrame)
					curMovFrame = curDispFrame - mov_start;
				curMovFrame = max(0, min(curMovFrame, numMovFrames - 1));
				for (int i = SM_CASTART; i <= ACTV_END; i += 3)
					setMovieSlider(-1, i);
			}
			disableSidePnl(1);
		}
		setToolPanel(0);
		paintWin(OM_MAIN, PAINTINIT);
		paintWin(OM_MENUBOX, PAINTINIT);
		return 1;
	}
	return 0;
}

int
isTab(int win)
{
	return (OM[win].state == OM_TAB);
}

int
curTab(void)
{
	return hotTab;
}

int
getMain(void)
{
	return OM_MAIN;
}

int
isButton(int win)
{
	return ((OM[win].state == OM_BUTTON && !OM[win].opt->grey) || OM[win].state == OM_ARROW || OM[win].state == OM_ZOOM || OM[win].state == OM_THUMB || OM[win].state == OM_PARA || OM[win].state == OM_BUTTON2);
}

int
isRadioButton(int win)
{
	return (OM[win].state == OM_RADIOBUTTON);
}

int
isVideoButton(int win)
{
	return (OM[win].state == OM_VIDEOBUTTON);
}

int
isIconButton(int win)
{
	return (OM[win].state == OM_ICONBUTTON);
}

int
isArrow(int win)
{
	return (OM[win].state == OM_ARROW);
}

int
isThumb(int win)
{
	return (OM[win].state == OM_THUMB);
}

int
isChckBox(int win)
{
	return ((OM[win].state == OM_CHCKBOX && !OM[win].opt->grey) || OM[win].state == OM_CHCKBX2);
}

int
curChckBox(void)
{
	return hotChckBox;
}

int
setChckBox(int win)
{
	if (win != -1) {
		if (OM[win].state == OM_CHCKBOX || OM[win].state == OM_CHCKBX2)
			OM[win].attr =! OM[win].attr;
	//	if (win == SM_DKSUBBOX)
	//		paintWin(SM_DKSUBBOX, PAINTGRFX);
	}
	return win;
}

void
drawChckBox(int win)
{
	if (OM[win].state == OM_CHCKBOX) {
		paintButtonFrame(OM[win].x,OM[win].y,OM[win].dx,OM[win].dy,OM[win].anim_state != OM_WARM);
	}
	else if (OM[win].state == OM_CHCKBX2 && OM[win].color && *OM[win].color > 0) {
		D3DRectangle(OM[win].x, OM[win].y, OM[win].x + 200, OM[win].y + OM[win].dy, colorTab);	//redraw background for text
		if (!movie_data && (win >= SM_MAPBOX1 && win <= SM_MAPBOX4)) {
			paintSubFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, colorGrayText);
			return;
		}
		paintSubFrame(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy, colorText);
		if (OM[win].anim_state == OM_WARM)
			paintSubFrame(OM[win].x+1,OM[win].y+1,OM[win].dx-2,OM[win].dy-2, colorText);
	}
	if (OM[win].attr >= 1)
		paintCheck(OM[win].x, OM[win].y, OM[win].dx, OM[win].dy);
}

int
setChckBoxOn(int win)
{
	if (win != -1)
		OM[hotChckBox = win].anim_state = OM_WARM;
	return win;
}

int
setChckBoxOff(int win)
{
	if (win != -1) {
		OM[win].anim_state = OM_COLD;
		hotChckBox = -1;
	}
	return win;
}

int
isButtonHot(void)
{
	return hotButtonHot;
}

int
curButton(void)
{
	return hotButton;
}

int
setButtonHot(int win)
{
	if (win != -1) {
		OM[hotButton = win].anim_state = OM_HOT;
		hotButtonHot = true;
	}
	return win;
}

int
setButtonOn(int win)
{
	if (win != -1) {
		OM[hotButton = win].anim_state = OM_WARM;
		hotButtonHot = false;
	}
	return win;
}

int
setButtonOff(int win)
{
	if (win != -1) {
		OM[win].anim_state = OM_COLD;
		hotButton = -1;
		hotButtonHot = FALSE;
	}
	return win;
}

int
getBackgrndColor(void)
{
	return *OM[OM_MEASUREWIN].color;
}

int
isMeasure(int win)
{
	return (OM[win].id == OM_MEASUREWIN);
}

int
isMovieWin(int win)
{
	return (OM[win].id == SM_MOVIEWIN);
}

int
isMovieTrace(int win, int mouseX, int mouseY)
{
	if ((hotTab == SM_MOVIEVIEW || hotTab == SM_CARDIOVIEW) && num_traces) {
		if ((mouseX > inset_x) && (mouseX < inset_x + inset_dx) && (mouseY > inset_y) && (mouseY < inset_y + inset_dy))
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void setMoiveFrame(int mouseX)
{
	curMovFrame = min(numMovFrames-1, max(0, (numMovFrames * (mouseX - inset_x) / inset_dx)));
}

void
setWinLock(int win)
{
	OM[OM_MAIN].attr = (win != -1)? win : 0;
}

int
isWinLocked()
{
	return OM[OM_MAIN].attr;
}

void
lockCursor(void)
{
	RECT winRect, rectpos = {NULL, NULL, NULL, NULL};
	int leftborder, topborder;

	GetWindowRect(main_window_handle, (LPRECT)&winRect);
	topborder = getWinTopBorder();
	leftborder = getWinLeftBorder();
	rectpos.left = OM[OM_MEASUREWIN].x + winRect.left + leftborder;
	rectpos.right = OM[OM_MEASUREWIN].x + OM[OM_MEASUREWIN].dx + winRect.left + leftborder;
	rectpos.top = OM[OM_MEASUREWIN].y + winRect.top + topborder;
	rectpos.bottom = OM[OM_MEASUREWIN].y + OM[OM_MEASUREWIN].dy + winRect.top + topborder;
	ClipCursor(&rectpos);
}

void
unlockCursor(void)
{
	ClipCursor(NULL);
}

void
resizeScreen(void)
{
	initScreen();
	if (!dataExist)
		resetZoom(1);
	else
		resetZoom(frameLiveFlag);
	paintScreen(0);
}

void Draw_seq(char *seqFname)
{
	extern HMENU	hFileMenu, hOutputMenu, DispSettingsMenu;
	dataExist = 0;
	disableSidePnl(1);

	proc_seqFile(seqFname);
	seqFlag = TRUE;
	setToolPanel(0);
	paintWin(OM_MEASUREWIN, PAINTINIT);
}

void
freeAll_ar_pts(void)
{
	if (darkImage != NULL) {
		_aligned_free(darkImage);
		darkImage = NULL;
	}
	if (ref_image != NULL) {
		_aligned_free(ref_image);
		ref_image = NULL;
	}
	if (dark_data != NULL) {
		_aligned_free(dark_data);
		dark_data = NULL;
	}
	if (single_img_data != NULL) {
		_aligned_free(single_img_data);
		single_img_data = NULL;
	}
	if (single_img_float != NULL) {
		_aligned_free(single_img_float);
		single_img_float = NULL;
	}
	if (omit_ar != NULL) {
		_aligned_free(omit_ar);
		omit_ar = NULL;
	}
	if (overlay_img != NULL) {
		_aligned_free(overlay_img);
		overlay_img = NULL;
	}
	if (image_data != NULL) {
		_aligned_free(image_data);
		image_data = NULL;
	}
	if (movie_data != NULL) {
		_aligned_free(movie_data);
		movie_data = NULL;
	}
}

void
displaySysTime(int systime_y)
{
	char str[256];
	SYSTEMTIME SystemTime;

	GetSystemTime(&SystemTime);
	sprintf(str, "%d, %d, -%d", SystemTime.wSecond, SystemTime.wMilliseconds, systime_y);
	drawText(0, 0, str, 20, systime_y, BLACK, omFontRprtTn, TRUE);
}

//********* from pdv_initcam **********

int
init_serial(EdtDev * edt_p, Dependent * dd_p, Edtinfo *ei_p)
{
	SerialInitNode *lp = ei_p->serial_init;

	edt_msg(DEBUG1, "sending serial init....\n");


	while (lp)
	{
		if (lp->tag == serial_tag_binary)
			init_serial_binary(edt_p, dd_p, ei_p->serial_init_delay, lp->data);

		else if (lp->tag == serial_tag_baslerf)
			init_serial_basler_binary(edt_p, dd_p, ei_p->serial_init_delay, lp->data);

		else if (lp->tag == serial_tag_duncanf)
			init_serial_duncan_binary(edt_p, dd_p, ei_p->serial_init_delay, lp->data);

		else init_serial_ascii(edt_p, dd_p, ei_p->serial_init_delay, lp->data);

		lp = lp->next;
	}
	return 0;

}

int
init_serial_ascii(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init)
{
	char    resp[257];
	int     skip_init = 0;
	int     hamamatsu = 0;
	int     ret = 0;

	//	if (grepit(dd_p->cameratype, "Hamamatsu") != NULL)
	//		hamamatsu = 1;

	/* first flush any pending/garbage serial */
	resp[0] = '\0';
	ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 256);
	pdv_serial_read(edt_p, resp, 256);

	if (edt_p->dd_p->startup_delay)
		edt_msleep(edt_p->dd_p->startup_delay);

	edt_msg(DEBUG1, "sending serial init commands to camera....\n");

	/*
	* if kodak 'i' camera, send a couple of "TRM?"s just to flush out the
	* serial, and send the IDN string to get the camera firmware ID
	*/
	if (pdv_is_kodak_i(edt_p))
	{
		/* first send a couple of TRM?s to flush and sync */
		pdv_serial_command(edt_p, "TRM?");
		pdv_serial_wait(edt_p, 500, 40);
		pdv_serial_command(edt_p, "TRM?");
		pdv_serial_wait(edt_p, 500, 40);
		ret = pdv_serial_read(edt_p, resp, 256);

		pdv_serial_command(edt_p, "IDN?");
		edt_msg(DEBUG1, "IDN? ");
		pdv_serial_wait(edt_p, 1000, 60);
		ret = pdv_serial_read(edt_p, resp, 256);
		if (ret > 20)
			edt_msg(DEBUG1, "%s\n", resp);
		else if (ret > 0)
			edt_msg(DEBUG1, "%s (unexpected response!)\n", resp);
		else
		{
			edt_msg(DEBUG1, "<no response from camera -- check cables and connections>\n");
			skip_init = 1;
		}
	}

	if (!skip_init)
	{
		int    i = 0, ms;
		char   *lastp = NULL;
		char   *p = serial_init;
		char    cmdstr[32];

		if (delay == NOT_SET)
			ms = 500;
		else ms = delay;

		/*
		* send serial init string
		*/
		cmdstr[0] = '\0';
		while (*p)
		{
			if (i > 31)
			{
				edt_msg(EDT_MSG_FATAL, "ERROR: serial command too long\n");
				return -1;
			}
			if (*p == ':' && lastp && *lastp != '\\')
			{
				cmdstr[i] = '\0';
				i = 0;

				memset(resp, '\0', 257);

				if (cmdstr[0])
				{
					if (is_hex_byte_command(cmdstr))
					{
						if (delay == NOT_SET)
							ms = 10;
						edt_msg(DEBUG2, "%s\n", cmdstr);
						pdv_serial_command_hex(edt_p, cmdstr, 1);
						ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 128);
						pdv_serial_read(edt_p, resp, ret);
					}
					else			/* FUTURE: expand and test serial_tx_rx and replace
									* pdv_serial_command/wait/read with that for all (?)
									*/
					{
						/* edt_msg(DEBUG1, ".", cmdstr); */
						edt_msg(DEBUG2, "%s ", cmdstr);
						pdv_serial_command(edt_p, cmdstr);
						ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 16);
						pdv_serial_read(edt_p, resp, 256);
						edt_msg(DEBUG2, " <%s>", strip_crlf(resp));
						edt_msg(DEBUG2, "\n");
					}
					/* edt_msg(DEBUG1, ".", cmdstr); */
					edt_msleep(ms);
				}
			}
			else if (*p != '\\')
				cmdstr[i++] = *p;
			lastp = p;
			++p;
		}
	}

	return 0;
}

u_char
atoxdigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return (ch - '0');

	if (ch >= 'a' && ch <= 'f')
		return (ch - 'a') + 10;

	if (ch >= 'A' && ch <= 'F')
		return (ch - 'A') + 10;

	return 0;
}

/*
* binary initialization: serial_binit specified, interpret as string of
* hex bytes only
* update 7/10/07: now sends adjacent bytes as a single command (string of
* bytes); does a * wait/read on whitespace only
*/
int
init_serial_binary(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init)
{
	int ret;
	u_int i;
	size_t buflen = strlen(serial_init);
	int cmdlen = 0, nibble_index = 0;
	u_char hbuf[1024];
	char    resp[1025];
	u_char ch;


	/* flush junk */
	ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 1024);

	hbuf[0] = 0;
	nibble_index = 0;
	for (i = 0; i < buflen + 1; i++)
	{
		if (i < buflen)
			ch = serial_init[i];
		else ch = '\0';

		if ((i == buflen) || (ch == '\t' || ch == ' '))
		{
			cmdlen = (nibble_index + 1) / 2;

			if (cmdlen)
			{
				/* send command, wait for response */
				pdv_serial_binary_command(edt_p, (char *)hbuf, cmdlen);
				if ((ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 1024)))
					pdv_serial_read(edt_p, resp, ret);
			}

			nibble_index = 0;
			hbuf[0] = 0;
		}
		else if (isxdigit(ch))
		{
			hbuf[nibble_index / 2] = (hbuf[nibble_index / 2] << 4) | atoxdigit(ch);
			if ((nibble_index++) % 2)
				hbuf[nibble_index / 2] = 0;
		}
		else
		{
			edt_msg(PDVLIB_MSG_WARNING, "serial_binit: hex string format error on buf[%d] ('%c')\n", i, serial_init[i]);
			return -1;
		}
	}

	edt_set_dependent(edt_p, dd_p);

	return 0;
}

/*
* basler A202k binary only initialization -- expects serial_init to contain
* colon-separated strings of hex bytes WITHOUT framing information; add
* basler A202k style framing and send the commands to the camera
*/
int
init_serial_basler_binary(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init)
{
	u_int  i, j;
	char   *p;
	int    ms = 50;
	char   *nextp;
	size_t len;
	int     ret;
	char    cmdstr[32];
	char    bytestr[3];
	char    resp[1024];

	if (delay != NOT_SET)
		ms = delay;

	/* first flush any pending/garbage serial */
	resp[0] = '\0';
	ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 1023);
	pdv_serial_read(edt_p, resp, 1023);

	edt_msg(DEBUG1, "sending Basler A202k framed commands to camera:\n");

	/*
	* send serial init string (first stick on a trailing ':' for parser)
	*/
	len = strlen(serial_init);
	if (serial_init[len - 1] != ':')
	{
		serial_init[len] = ':';    /* safe because data is */
		serial_init[len + 1] = '\0'; /* malloc'd to arg len + 2 */
	}

	p = serial_init;

	while ((nextp = strchr(p, ':')))
	{
		int     ms = 50;
		u_char bytebuf[32];
		u_char *bp = bytebuf;

		len = (size_t)(nextp - p);
		if (len > 31)
		{
			edt_msg(EDT_MSG_FATAL, "ERROR: serial command too long\n");
			return -1;
		}

		strncpy(cmdstr, p, len);
		cmdstr[len] = 0;
		memset(resp, '\0', 1023);

		if (len % 2)
		{
			edt_msg(EDT_MSG_FATAL, "serial_binary format string error: odd nibble count\n");
			return -1;
		}

		for (i = 0; i<len; i++)
		{
			if (!isxdigit(cmdstr[i]))
			{
				edt_msg(EDT_MSG_FATAL, "serial_binrary format string error: odd nibble count\n");
				return -1;
			}
		}

		for (i = 0, j = 0; i<len; i += 2, j++)
		{
			u_int bint;
			bytestr[0] = cmdstr[i];
			bytestr[1] = cmdstr[i + 1];
			bytestr[2] = '\0';
			sscanf(bytestr, "%x", &bint);
			*(bp++) = (u_char)(bint & 0xff);
		}
		pdv_send_basler_frame(edt_p, bytebuf, (int)len / 2);

		ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 1023);
		pdv_serial_read(edt_p, resp, ret);

		p = nextp + 1;
		edt_msleep(ms);
	}

	edt_set_dependent(edt_p, dd_p);

	return 0;
}

/*
* duncantech MS2100, 2150, 3100, 4100 binary only initialization -- expects
* serial_init to contain colon-separated strings of hex bytes WITHOUT
* framing information; add duncan framing and send the commands to the
* camera
*/
int
init_serial_duncan_binary(EdtDev * edt_p, Dependent * dd_p, int delay, char *serial_init)
{
	int    i, j;
	char   *p;
	int    ms = 50;
	char   *nextp;
	int     len;
	int     ret;
	char    cmdstr[32];
	char    bytestr[3];
	char    resp[1024];

	if (delay != NOT_SET)
		ms = delay;

	/* first flush any pending/garbage serial */
	resp[0] = '\0';
	ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 1023);
	pdv_serial_read(edt_p, resp, 1023);

	edt_msg(DEBUG1, "sending DuncanTech framed commands to camera:\n");

	/*
	* send serial init string (first stick on a trailing ':' for parser)
	*/
	len = (int)strlen(serial_init);
	if (serial_init[len - 1] != ':')
	{
		serial_init[len] = ':';    /* safe because data is */
		serial_init[len + 1] = '\0'; /* malloc'd to arg len + 2 */
	}

	p = serial_init;

	while ((nextp = strchr(p, ':')))
	{
		int     ms = 50;
		u_char bytebuf[32];
		u_char *bp = bytebuf;

		len = (int)(nextp - p);
		if (len > 31)
		{
			edt_msg(EDT_MSG_FATAL, "ERROR: serial command too long\n");
			return -1;
		}

		strncpy(cmdstr, p, len);
		cmdstr[len] = 0;
		memset(resp, '\0', 1024);

		if (len % 2)
		{
			edt_msg(EDT_MSG_FATAL, "serial_binrary format string error: odd nibble count\n");
			return -1;
		}

		for (i = 0; i<len; i++)
		{
			if (!isxdigit(cmdstr[i]))
			{
				edt_msg(EDT_MSG_FATAL, "serial_binrary format string error: odd nibble count\n");
				return -1;
			}
		}

		for (i = 0, j = 0; i<len; i += 2, j++)
		{
			u_int bint;
			bytestr[0] = cmdstr[i];
			bytestr[1] = cmdstr[i + 1];
			bytestr[2] = '\0';
			sscanf(bytestr, "%x", &bint);
			*(bp++) = (u_char)(bint & 0xff);
		}
		pdv_send_duncan_frame(edt_p, bytebuf, len / 2);

		/* flush out junk */
		ret = pdv_serial_wait(edt_p, dd_p->serial_timeout, 1023);
		pdv_serial_read(edt_p, resp, ret);

		p = nextp + 1;
		edt_msleep(ms);
	}


	edt_set_dependent(edt_p, dd_p);

	return 0;
}

char    scRetStr[256];
char   *
strip_crlf(char *str)
{
	char   *p = str;

	scRetStr[0] = '\0';

	while (*p)
	{
		if (*p == '\r')
			sprintf(&scRetStr[strlen(scRetStr)], "\\r");
		else if (*p == '\n')
			sprintf(&scRetStr[strlen(scRetStr)], "\\n");
		else
			sprintf(&scRetStr[strlen(scRetStr)], "%c", *p);
		++p;
	}

	return scRetStr;
}

/*
* looking for "0xNN". If so, return 1, else return 0
*/
int
is_hex_byte_command(char *str)
{
	int     i;

	if (strlen(str) != 4)
		return 0;

	if ((strncmp(str, "0x", 2) != 0)
		&& (strncmp(str, "0X", 2) != 0))
		return 0;

	for (i = 2; i < (int)strlen(str); i++)
	{
		if ((str[i] < '0') && (str[i] > '9')
			&& (str[i] < 'a') && (str[i] > 'z')
			&& (str[i] < 'A') && (str[i] > 'Z'))
			return 0;
	}
	return 1;
}


//Run Script
static HWND script_hwndPB = NULL;
static int script_steps;
void createScriptProgBar()
{
	// making a progress bar
	RECT rcClient;  // Client area of parent window.
	int cyVScroll = 50;  // Height of scroll bar arrow.
	GetClientRect(main_window_handle, &rcClient);
	script_hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE | WS_CAPTION, rcClient.left + rcClient.right / 6,
		rcClient.top + (rcClient.bottom - cyVScroll) * 2 / 5 - 50,
		rcClient.right / 4, cyVScroll,
		main_window_handle, NULL, NULL, NULL);
	SendMessage(script_hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, script_steps + 1));
	SendMessage(script_hwndPB, PBM_SETSTEP, (WPARAM)1, 0);
	SendMessage(script_hwndPB, PBM_STEPIT, 0, 0);
}

void getParaFromStr(char *str)
{
	char this_str[256], command[256];
	char *p;
	long repetitions;

	if (strstr(str, "Number of Frames")) {
		if (p = strstr(str, "=")) {
			strcpy(this_str, p + 1);
			curNFrames = atoi(this_str);
		}
		return;
	}
	else if (strstr(str, "Exposure Time")) {
		if (p = strstr(str, "=")) {
			strcpy(this_str, p + 1);
			if (strstr(str, "*="))
				frame_interval *= atoi(this_str);
			else if (strstr(str, "+="))
				frame_interval += atof(this_str);
			else
				frame_interval = atof(this_str);
			repetitions = set_repetition();
			sprintf(command, "@REP %ld", repetitions);
			SM_serial_command(command);
		}
		return;
	}
}

void runFunctionFromStr(char *str)
{
	if (strstr(str, "Record"))
		StreamDataSM();
	else if (strstr(str, "Dark Frame"))
		acqDarkFrame(1);
}

void SendSCMDFromStr(char *str, int val)
{
	char command[256];
	sprintf(command, str, val);
	SM_serial_command(command);
}

static char script_ar[100][256];
void RunScript()
{
	char Fname[MAX_PATH];
	FILE *fp;
	char buf[256], str[256], str2[256];
	char *p;
	int i, loops, added_cnt, cnt, inloop_cnt, num_lines, rep_startline=0;

	strcat(strcpy(Fname, home_dir), "\\SMSYSDAT\\Turbo_Script.txt");
	if (FOPEN_S(fp, Fname, "r")) {
		cnt = 0;
		added_cnt = 0;
		inloop_cnt = -1;
		while (fgets(buf, 255, fp)) {
			strcpy(str, buf);
			*(str+2) = '\0';
			if (!strcmp(str, "//"))
				continue;
			strcpy(script_ar[cnt], buf);
			if (p = strstr(buf, "REPEAT-")) {
				strcpy(str, p + 7);
				loops = atoi(str);
				inloop_cnt = 0;
			}
			else if (p = strstr(buf, "REPEATEND")) {
				added_cnt += inloop_cnt * (loops - 1);
				inloop_cnt = -1;
			}
			cnt++;
			if (inloop_cnt >= 0)
				inloop_cnt++;
		}
		fclose(fp);

		num_lines = cnt;
		script_steps = cnt + added_cnt;
		createScriptProgBar();
		int step_cnt = 0;
		double step_size = 1;
		char bar_title[256];
		sprintf(bar_title, "Turbo Script");
		SetWindowText(script_hwndPB, bar_title);
		loops = 0;
		cnt = 0;
		for (i = 0; i < num_lines; i++) {
			strcpy(buf, script_ar[i]);
			if (p = strstr(buf, "\n"))
				*p = '\0';
			sprintf(bar_title, "Line %d: %s", i+1, buf);
			if (loops)
				sprintf(bar_title, "%s, - Repeat %d", bar_title, cnt+1);
			SetWindowText(script_hwndPB, bar_title);
			if (step_cnt < script_steps)
				SendMessage(script_hwndPB, PBM_STEPIT, 0, 0);
			step_cnt++;
			if (p = strstr(buf, "REPEAT-")) {
				strcpy(str, p + 7);
				loops = atoi(str);
				cnt = 0;
				rep_startline = i;
			}
			else if (p = strstr(buf, "REPEATEND")) {
				cnt++;
				if (cnt < loops)
					i = rep_startline;
				else
					loops = 0;
				continue;
			}
			else if (strstr(buf, "="))
				getParaFromStr(buf);
			else if (strstr(buf, "Function:"))
				runFunctionFromStr(buf);
			else if (strstr(buf, "Serial Command:")) {
				if (p = strstr(buf, "@")) {
					int val = 0, base_val, inc_val;
					strcpy(str, p);
					if (p = strstr(str, "_")) {
						strcpy(buf, p+1);
						*p = '\0';
						if (p = strstr(buf, "_")) {
							strcpy(str2, p + 1);
							*p = '\0';
							inc_val = atoi(str2);
							base_val = atoi(buf);
							val = base_val + inc_val * cnt;
						}
					}
					SendSCMDFromStr(str, val);
				}
			}
		}
		DestroyWindow(script_hwndPB);
	}

}

