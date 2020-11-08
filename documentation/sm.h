#define PAINTINIT		0
#define PAINTGRFX		1
#define PAINTTEXT		2

#define DRAW_MODE		0
#define DRAG_MODE		1
#define KERNEL_MODE		2
#define DRAG_ERASE		3
#define BNC_SELECT		4

#define BKGRND			0x00f0e9de
#define BLACK			0x00000000
#define DKGRAY			0x00404040
#define MIDGRAY			0x00808080
#define LTGRAY2			0x00c0c0c0
#define LTGRAY			0x00e4dfdf
#define BLUE			0x0000AFFF 
#define GREEN			0x0000D800
#define DKRED			0x00b0201f
#define TEXT_RED		0x005040df
#define TEXT_LTRED		0x00209aef
#define WHITE			0x00ffffff
#define RED				0x00FF0000
#define YELLOW			0x00FFFF00	
#define TEXT_YELLOW		0x0000FFFF	
#define ORANGE			0x00FFA444	
#define PINK			0x00FF33FF 
#define RED2			0x00EE6000 
#define GREEN2			0x0000EEBB  
#define BLUE2			0x0044BBEE 
#define ORANGE2			0x00EEA588	
#define PINK2			0x00BE4AEE 
#define YELLOW2			0x00DDDD88	
#define PURPLE			0x006622FF 

#define MAXOFFSETS 64

#define OM_DISPLAY		0
#define OM_TAB			1	
#define OM_MENU			2
#define OM_SCRMENU		3
#define OM_SCRTRCK		4
#define OM_ZOOM			5
#define OM_ARROW		6
#define OM_THUMB		7
#define OM_LABEL		8
#define OM_BUTTON		9
#define OM_TEXT			10
#define OM_PARA			OM_TEXT+1
#define OM_NAVIGTR		OM_TEXT+2
#define OM_CHCKBOX		OM_TEXT+3
#define OM_CHCKBX2		OM_TEXT+4
#define OM_TAB2			OM_TEXT+5
#define OM_RADIOBUTTON	OM_TEXT+6
#define OM_DRAWFIELD	OM_TEXT+7
#define OM_VIDEOBUTTON	OM_TEXT+8
#define OM_ICONBUTTON	OM_TEXT+9
#define OM_SLIDER		OM_TEXT+10
#define OM_DB_SLIDER	OM_TEXT+11
#define OM_BUTTON2		OM_TEXT+12
#define OM_NULL			OM_TEXT+13

#define OM_NONE			-2
#define OM_ALONE		-1
#define OM_MAIN			0
#define OM_MEASUREVIEW	1
#define SM_MOVIEVIEW	2
#define SM_MOVIEWIN		3
#define OM_SUMMARYDATA	4
#define OM_DERIVEDDATA	5
#define OM_ALLBIOPSY	6
#define OM_OPTIONSVIEW	7
#define OM_HELPVIEW		8
#define OM_MEASUREWIN	9
#define SM_TRACEWIN		10
#define SM_QUESTION		11
#define SM_MOVIEPNL		(SM_QUESTION+1)	//12
#define SM_MKMOVIE		(SM_MOVIEPNL+1)	
#define SM_RDOVRLAY		(SM_MOVIEPNL+2)	
#define SM_DATAOVLY		(SM_MOVIEPNL+3)	
#define SM_MVSUBBOX		(SM_MOVIEPNL+4)	
#define SM_MVFLTBOX		(SM_MOVIEPNL+5)	
#define SM_MVBINUP		(SM_MOVIEPNL+6)	
#define SM_MVBINDN		(SM_MOVIEPNL+7)	
#define SM_MVBINLB		(SM_MOVIEPNL+8)	
#define OV_MODE_LB		(SM_MOVIEPNL+9)	
#define SM_OV_MODE1		(SM_MOVIEPNL+10)	
#define SM_OV_MODE2		(SM_MOVIEPNL+11)	
#define SM_OV_MODE3		(SM_MOVIEPNL+12)	
#define MOV_SCALE_LB	(SM_MOVIEPNL+13)	
#define SM_MV_SCALE1	(SM_MOVIEPNL+14)	
#define SM_MV_SCALE2	(SM_MOVIEPNL+15)	
#define SM_MV_SCALE3	(SM_MOVIEPNL+16)	
#define SM_MVSTART		(SM_MOVIEPNL+17)	//29
#define SM_MVSTART_UP	(SM_MVSTART+1)	
#define SM_MVSTART_DN	(SM_MVSTART+2)	
#define SM_MVEND		(SM_MVSTART+3)	
#define SM_MVEND_UP		(SM_MVSTART+4)	
#define SM_MVEND_DN		(SM_MVSTART+5)	
#define MOV_SCALEMIN	(SM_MVSTART+6)	
#define MOV_SCMIN_UP	(SM_MVSTART+7)	
#define MOV_SCMIN_DN	(SM_MVSTART+8)	
#define MOV_SCALEMIN2	(SM_MVSTART+9)	
#define MOV_SCMIN_UP2	(SM_MVSTART+10)	
#define MOV_SCMIN_DN2	(SM_MVSTART+11)	
#define MOV_SCALEMAX	(SM_MVSTART+12)	
#define MOV_SCMAX_UP	(SM_MVSTART+13)	
#define MOV_SCMAX_DN	(SM_MVSTART+14)	
#define MOV_SCALEMAX2	(SM_MVSTART+15)	
#define MOV_SCMAX_UP2	(SM_MVSTART+16)	
#define MOV_SCMAX_DN2	(SM_MVSTART+17)	
#define MOV_DISP_CUT	(SM_MVSTART+18)	
#define MOV_DISPCUT_UP	(SM_MVSTART+19)	
#define MOV_DISPCUT_DN	(SM_MVSTART+20)	
#define MV_TRANSPARENC	(SM_MVSTART+21)	
#define MV_TRANSP_UP	(SM_MVSTART+22)	
#define MV_TRANSP_DN	(SM_MVSTART+23)	
#define SM_CURMOVF		(SM_MVSTART+24)	//53
#define SM_CURMOVF_UP	(SM_CURMOVF+1)	
#define SM_CURMOVF_DN	(SM_CURMOVF+2)	//55
#define MULTI_FRAME1	(SM_CURMOVF+3)	
#define MULTI_F1_UP		(SM_CURMOVF+4)	
#define MULTI_F1_DN		(SM_CURMOVF+5)	
#define MULTI_NUMCOL	(SM_CURMOVF+6)	
#define MULTI_NUMC_UP	(SM_CURMOVF+7)
#define MULTI_NUMC_DN	(SM_CURMOVF+8)	
#define MULTI_NUMROW	(SM_CURMOVF+9)	
#define MULTI_NUMR_UP	(SM_CURMOVF+10)
#define MULTI_NUMR_DN	(SM_CURMOVF+11)	
#define MULTI_STEP		(SM_CURMOVF+12)	
#define MULTI_STEP_UP	(SM_CURMOVF+13)	
#define MULTI_STEP_DN	(SM_CURMOVF+14)	//67
#define MOV_PLAY		(MULTI_STEP_DN+1)
#define MOV_PAUSE		(MULTI_STEP_DN+2)
#define MOV_MODE_LB		(MULTI_STEP_DN+3)
#define MOV_MOVIE		(MULTI_STEP_DN+4)
#define MOV_MULTI		(MULTI_STEP_DN+5)
#define SM_IMGSCBASE	(MOV_MULTI+1) 
#define SM_IMGSCALE		(SM_IMGSCBASE+1)
#define SM_IMGSCLUP		(SM_IMGSCBASE+2)
#define SM_IMGSCLDN		(SM_IMGSCBASE+3)
#define SM_IMGSCRUP		(SM_IMGSCBASE+4)
#define SM_IMGSCRDN		(SM_IMGSCBASE+5)	//78
#define SM_DATAREAD		(SM_IMGSCRDN+1)		
#define SM_NEXTREAD		(SM_DATAREAD+1)
#define SM_PREVREAD		(SM_DATAREAD+2)
#define SM_FIRSTFRM		(SM_DATAREAD+3)		
#define SM_BNCPNL		(SM_FIRSTFRM+1)		
#define SM_BNC1			(SM_BNCPNL+1)
#define SM_BNC2			(SM_BNCPNL+2)
#define SM_BNC3			(SM_BNCPNL+3)
#define SM_BNC4			(SM_BNCPNL+4)
#define SM_BNC5			(SM_BNCPNL+5)
#define SM_BNC6			(SM_BNCPNL+6)
#define SM_BNC7			(SM_BNCPNL+7)
#define SM_BNC8			(SM_BNCPNL+8)		
#define SM_BNC_FLTR		(SM_BNCPNL+9)		
#define SM_BNCGAIN		(SM_BNC_FLTR+1)		
#define SM_BNCGNUP		(SM_BNC_FLTR+2)		
#define SM_BNCGNDN		(SM_BNC_FLTR+3)		
#define SP_FLTPNL		(SM_BNC_FLTR+4)			
#define SP_LPTYPE_UP	(SP_FLTPNL+1)			
#define SP_LPTYPE_DN	(SP_FLTPNL+2)			
#define SP_LP_UP		(SP_FLTPNL+3)			
#define SP_LP_DN		(SP_FLTPNL+4)			
#define SP_LP_LABEL		(SP_FLTPNL+5)			
#define SP_HPTYPE_UP	(SP_FLTPNL+6)			
#define SP_HPTYPE_DN	(SP_FLTPNL+7)			
#define SP_HP_UP		(SP_FLTPNL+8)			
#define SP_HP_DN		(SP_FLTPNL+9)			
#define SP_HP_LABEL		(SP_FLTPNL+10)			
#define SM_TOOLPNL		(SP_HP_LABEL+1)			
#define SM_TPNLTEXT		(SM_TOOLPNL+1)
#define SM_PLAY			(SM_TOOLPNL+2)
#define SM_PAUSE		(SM_TOOLPNL+3)
#define SM_NEXTFRAME	(SM_TOOLPNL+4)
#define SM_PREVFRAME	(SM_TOOLPNL+5)		
#define SM_DRAW			(SM_PREVFRAME+1)	
#define SM_DRAGBOX		(SM_DRAW+1)	
#define SM_KERNEL		(SM_DRAW+2)	
#define SM_REMOVE		(SM_DRAW+3)	
#define SM_ADD_CUR_ROI	(SM_DRAW+4)	
#define SM_THRESHOLD	(SM_DRAW+5)	
#define SM_TPOLARITY	(SM_DRAW+6)
#define SM_KSIZEUP		(SM_DRAW+7)	
#define SM_KSIZEDN		(SM_DRAW+8)			
#define SM_TRCEXP	    (SM_KSIZEDN+1)	
#define SM_TRCEXP_UP	(SM_KSIZEDN+2)	
#define SM_TRCEXP_DN	(SM_KSIZEDN+3)	
#define SM_LOWPASS	    (SM_KSIZEDN+4)	
#define SM_LOWPASS_UP	(SM_KSIZEDN+5)	
#define SM_LOWPASS_DN	(SM_KSIZEDN+6)	
#define SM_HIGHPASS		(SM_KSIZEDN+7)		
#define SM_HIGHPASS_UP	(SM_KSIZEDN+8)		
#define SM_HIGHPASS_DN	(SM_KSIZEDN+9)		
#define SM_TRCSTART	    (SM_HIGHPASS_DN+1)	
#define SM_TRCSTT_UP	(SM_TRCSTART+1)	
#define SM_TRCSTT_DN	(SM_TRCSTART+2)	
#define SM_REFSTART	    (SM_TRCSTART+3)	
#define SM_REFSTT_UP	(SM_REFSTART+1)	
#define SM_REFSTT_DN	(SM_REFSTART+2)	
#define SM_TRCGAIN		(SM_REFSTART+3)		
#define SM_TRCGNUP		(SM_TRCGAIN+1)		
#define SM_TRCGNDN		(SM_TRCGAIN+2)		
#define SM_HISTGAIN		(SM_TRCGAIN+3)	
#define SM_HISTEXP		(SM_TRCGAIN+4)		
#define OM_MENUBOX		(SM_HISTEXP+1)		
#define SM_FNAMEBOX		(OM_MENUBOX+1)		
#define SM_FNAMEUPARR	(OM_MENUBOX+2)	
#define SM_FNAMEDNARR	(OM_MENUBOX+3)	//38
#define OM_OBJBOX		(OM_MENUBOX+4)		//54
#define OM_OBJUPARR		(OM_OBJBOX+1)	//41
#define OM_OBJDNARR		(OM_OBJBOX+2)	//42
#define OM_NAVIBOX		(OM_OBJBOX+3)		//57
#define OM_FLDLTARR		(OM_NAVIBOX+1)	//44
#define OM_FLDRTARR		(OM_NAVIBOX+2)	//45
#define OM_FLDUPARR		(OM_NAVIBOX+3)	//46
#define OM_FLDDNARR		(OM_NAVIBOX+4)	//47
#define SM_HISTOBOX		(OM_NAVIBOX+5)	//48
#define SM_DKSUBBOX		(OM_NAVIBOX+6)		//63
#define SM_HLOCKBOX		(OM_NAVIBOX+7)	//48
#define SM_AUTOSBOX		(OM_NAVIBOX+8)	//49
#define SM_FOCUSBOX		(OM_NAVIBOX+9)
#define SM_OVWRTBOX		(OM_NAVIBOX+10)		//67
#define SM_RESET		(SM_OVWRTBOX+1)	
#define SM_LIVE			(SM_OVWRTBOX+2)
#define SM_STREAM		(SM_OVWRTBOX+3)	
#define SM_CAPTURE		(SM_OVWRTBOX+4)	
#define SM_DKFRAME		(SM_OVWRTBOX+5)	
#define SM_AUTOOFFSET	(SM_OVWRTBOX+6)
#define SM_DARKSUB		(SM_OVWRTBOX+7)	
#define SM_AUTOSCL		(SM_OVWRTBOX+8)	
#define SM_LOCKHIST		(SM_OVWRTBOX+9)
#define SM_CSETTING		(SM_OVWRTBOX+10)
#define SM_OVERWRT		(SM_OVWRTBOX+11)		
#define SM_BROWSE		(SM_OVWRTBOX+12)		
#define SM_FOCUS		(SM_OVWRTBOX+13)
#define OM_OPTQUIT		(SM_OVWRTBOX+14)	

#define OM_DISPLAYMAIN		0
#define OM_DISPLAYMEASURE	1

#define OM_TEXTHEAD		0

#define OM_MENUMAIN		0

#define OM_OBJ			0
#define OM_FIELD		1
#define OM_X			2
#define OM_Y			3
#define SM_RSTCM		4
#define SM_LVSTP		5
#define SM_STRM			6
#define SM_CAPT			7
#define SM_DARK			8
#define SM_AUTOO		9
#define SM_DSUB			10
#define SM_AUTOS		11
#define SM_LOCKH		12
#define SM_CAMST		13
#define SM_OVWRT		14
#define SM_BRWS			15
#define SM_FCS			16
#define OM_QUIT			17
#define OM_DEMO			(OM_QUIT+1)
#define OM_NGLTARR		(OM_QUIT+2)
#define OM_NGRTARR		(OM_QUIT+3)
#define OM_NGUPARR		(OM_QUIT+4)
#define OM_NGDNARR		(OM_QUIT+5)
#define OM_SCRUPARR		(OM_QUIT+6)
#define OM_SCRDNARR		(OM_QUIT+7)
#define OM_OBUPARR		(OM_QUIT+8)
#define OM_OBDNARR		(OM_QUIT+9)
#define OM_VWUPARR		(OM_QUIT+10)
#define OM_VWDNARR		(OM_QUIT+11)
#define SM_HISTCHCK		(OM_VWDNARR+1)
#define SM_DSUBCHCK		(OM_VWDNARR+2)
#define SM_AUTOCHCK		(OM_VWDNARR+3)
#define SM_HLCKCHCK		(OM_VWDNARR+4)
#define OM_DRWMDLB		(OM_VWDNARR+5)
#define OM_ERSMDLB		(OM_VWDNARR+6)
#define OM_RULERLB		(OM_VWDNARR+7)
#define OM_CCOK			(OM_VWDNARR+8)
#define OM_CCCNCL		(OM_VWDNARR+9)
#define SM_FCSCHCK		(OM_VWDNARR+10)

#define OM_COLD			0
#define OM_WARM			1
#define OM_HOT			2
#define OM_GRAY			3

#define OM_UPARR		1
#define OM_DNARR		2
#define OM_LTARR		3
#define OM_RTARR		4
#define OM_THMVERT		5
#define OM_THMHORZ		6
#define OM_ZOOMIN		7
#define OM_ZOOMOUT		8

#define OM_TEXT_ENGLISH	0
#define OM_TEXT_CHINESE	1

#define MAXCHANNELS     4

#define FILES       6
#define FINDEX      0
#define FINDICES    1
#define FIMAGES     2
#define FDATA       3
#define FSUMMARY    4
#define FSUMMARIES  5

#define XYBUFSIZE 32000

typedef struct OM_DATA{
short int ptr,aux1,aux2,max;
    float *x,*y;
} OMDATATYPE, *OMDATA_PTR;

typedef struct OM_TISSUEAR
{
		 int x_ul, y_ul, x_ur, y_ur, x_ll, y_ll, x_lr, y_lr;
         int fieldX, fieldY;
         int objective; 
         int left_type, right_type, top_type, bottom_type;
} OMTISSUETYPE, *OMTISSUE_PTR;

typedef struct OM_OPTION
{
	int id;
	int row;
	int col;
	int hilite;
	int grey;
	int scan;
	int avkey;
	int adikey;
	int vkey;
	int dikey;
	int tabcode;
	char *keystr;
	void (*draw_fn)();
	int hilite_x;
	int keystr_x;
	int keystr_y;
} OMOPTIONTYPE, *OMOPTION_PTR;

typedef struct OM_PAR
{
	char parameter[21];
	int index;
	int type;
	int *edge,*fill,*wdth;
	int selected;
	int bitmap_index;
	int tabbutton;
	int order;
	int revindex;
} OMPARTYPE, *OMPAR_PTR;

typedef struct OM_PARLIST
{
	int parms;
	int first;
	int num;
	int hot;
	OMPAR_PTR list;
} OMPARLISTTYPE, *OMPARLIST_PTR;

// the blitter object structure OM
typedef struct OM_TYP
{
		int id;				// ID of the entity
		int	parent;			// ID of the parent
        int state;          // type of window - display, tab, menu, submenu, text, button
        int anim_state;     // an animation state variable, up to you
        int attr;           // attributes pertaining to the object (general)
		int border;
		char *title;
		int bitmap_index;
		int x;
		int y;
		int dx;
		int dy;
		int *color;
		OMOPTION_PTR opt;			// pointer to option
		int title_x;
		int title_y;
		int first_child;
		int num_children;
//        int x,y;            // position bitmap will be displayed at
//        int width, height;  // the width and height of the bob
//        int width_fill;     // internal, used to force 8*x wide surfaces
//		int color;			// color
//		int parent_color;	// parent color
//		int border;			// border type - could be used for width
//		void *title;		// if text, title
//		int title_x;		// x coord of title
//		int title_y;		// y coord of title	
//        int bpp;            // bits per pixel
//        int counter_1;      // general counters
//        int counter_2;
//        int max_count_1;    // general threshold values;
//        int max_count_2;
//        int varsI[16];      // stack of 16 integers
//        float varsF[16];    // stack of 16 floats
//        int curr_frame;     // current animation frame
//        int num_frames;     // total number of animation frames
//        int curr_animation; // index of current animation
//        int anim_counter;   // used to time animation transitions
//        int anim_index;     // animation element index
//        int anim_count_max; // number of cycles before animation
//        int *animations[MAX_BOB_ANIMATIONS]; // animation sequences
//        LPDIRECTDRAWSURFACE4 images[MAX_BOB_FRAMES]; // the bitmap images DD surfaces 
} OMTYPE, *OM_PTR;

void writeColors(int prf_flag);
int readColors(int prf_flag);
void CreateFonts(void);
void DeleteFonts(void);
int initSystem(void);
void exitSystem(void);
void initScreen(void);
void paintScreen(void);
void paintWin(int win, int flag);
void paintText(int win);
int procWin(int win);
//void drawText(int text_mode, int bitmap_index, char *text, int x, int y, int color, LPD3DXFONT font, int extra);
void drawText(int text_mode, int bitmap_index, char *text, int x, int y, int color, HFONT font, int extra);
int windowTest(int x, int y);
int isButton(int win);
int isArrow(int win);
int isThumb(int win);
int isButtonHot(void);
int isTab(int win);
int curTab(void);
int curButton(void);
int setButtonOn(int win);
int setButtonHot(int win);
int setButtonOff(int win);
int isChckBox(int win);
int curChckBox(void);
int setChckBox(int win);
void drawChckBox(int win);
int setChckBoxOn(int win);
int setChckBoxOff(int win);
int isMeasure(int win);
int isTrace(int win);
int isTraceThumb(int mouseX);
void setTraceThumb(int mouseX, int ref_flag);
void setWinLock(int win);
int isWinLocked(void);
int initDraw(int mouseX, int mouseY);
void updateDraw(int mouseX, int mouseY);
int exitDraw(int mouseX, int mouseY);
void putImageLine(int x, int y, int dx, unsigned int *lineBuffer);
void repaintMenubox(void);
void paintRadioButton(int win);
int isRadioButton(int win);
int isVideoButton(int win);
int isIconButton(int win);
void lockCursor(void);
void unlockCursor(void);
int getWinTopBorder(void);
int getWinLeftBorder(void);
int getBackgrndColor(void);
int getMeasureWinX0(void);
int getMeasureWinY0(void);
int getMeasureWinX(void);
int getMeasureWinY(void);
void displaySysTime(int systime_y);
int clipX(int x);
int clipY(int y);
void AcquireOneFrame(int dispFlag);
int getTrace(int k);
int getTracePix(int mouseX, int mouseY, int BNC_pix);
void setHistoRange(int mouseX);
void setHistoRangeWheel(int inc, int mouseX);
void setHistoGain(int mouseY);
void setTraceGain(int mouseY);
void setBNCGain(int mouseX, int mouseY);
void setTraceExp(int mouseX);
void setTraceLowPass(int mouseX, int drawTraceFlag);
void setTraceHighPass(int mouseX, int drawTraceFlag);
void setTraceStart(int mouseX);
void setRefStart(int mouseX);
int isHistoExp(int win);
int isHistoGain(int win);
int isTraceGain(int win);
int isBNCGain(int win);
int isTraceExp(int win);
int isTraceLowPass(int win);
int isTraceHighPass(int win);
int isTraceStart(int win);
int isRefStart(int win);
void clearTracePix(void);
void SM_averageData(char *filename, char *av_Fname, int num_av_acq);
void SM_readData(char *filename);
int SM_saveDark(char *filename);
void playMovieFrame(int reverseFlag);
void playMovie();
void SM_saveFITS(char *fitsName, int batchFlag);
void SM_saveTIFF(char *tiffName);
void SM_saveSmFile(char *fileName);
void SM_saveTIFF_image(char *tiffName);
void Batch_saveFITS(char *fName);
void save_fits_image(char *fitsName);
void setPrevNextGray(int prevGFlag, int nextGFlag);
char *getPrevNextName(char *filename, int prev_flag);
void addFileList(void);
void drawTraceThumb(int mouseX);
void setTraceClickExp(int mouseX1, int mouseX2);
int isImgScSlider(int win);
int isImgScButton(int win);
int isTraceButton(int win);
void setImgScale(int mouseX);
void loadCamOffsets(void);
void paintBNClabels(int win);
int readPref(void);
int writePref(void);
int SM_initCamera(int load_config_flag, int close_flag, int load_pro_flag);
void DisplayTrace(char *txtName, int paintFlag);
void StreamDataSM();
void getDispImage(int HistoOnly);
void PhotonTransferAcq();
int readPHT_config();
char *SM_pdv_query_command(char *command, char *ret_c, int max_len);
void AutoSetOffset(int PHT_flag);
void SM_serial_command(char *command);
int readPHT_config2(void);
void setZoomGray();
int getLineProfileY(int ms_x1, int _ms_y1, int mouseX, int mouseY);
void proc_seqFile(char *Fname);
void save_seqFile(char *Fname);
BOOL PopFileOpenDlg(HWND, PTSTR, PTSTR);
void getStreamFrame();
void freeAll_ar_pts();
void imageShiftControl();
void dispSettingCheck();
void moveImage(int mouseX, int mouseY, int mouseX0, int mouseY0);
void saveBNCtext(char *txtName);
void saveWholeTraces(char *txtName);
void saveWholeROIdata(char *txtName);
void acqDarkFrame(int autoFlag);
void recordRICdata();
void loadRICdata(char *filename);
void Save_RMS_pix(char *fName);
int getSaveSmallSetting(void);
int getTiffBit(void);
void SM_saveTIFF_movie(char *tiffName);
void SM_saveMovie(char *tiffName, int screenFlag);
int getMain(void);
void resizeScreen(void);
//void checkEDTtime(void);
void StreamDataSM_64bit(int dark_f_flag);
void initDDbuffer(void);
int SM_readTIFF_stack(char *tiffName, int for_overlay);
void monochromater_conversion(int offline_flag);
void resetNDR();
void reset2KSettings();
void changeZoom(int zoom_in_out);
void getAllTraces(void);
void getAcq_FileName(int cnt);
void resetScaleB();
void undoLastROI();
int scaledTraceSub(int applyFlag, char *fName);
void scaledTraceSubOne(int k);
void EnableTools(HWND hDlg);
int showPixelVals(int mouseX, int mouseY);
void resetDataProc();
//void Movie_scaling();
void SM_SetHourCursor();
void SM_SetNonHourCursor(HCURSOR cursor);
void saveROIs(char *txtName);
void readROIs(char *txtName);
void saveOmit(char *fileName);
void readOmit(char *fileName);
void getAllTraces();
int SM_readHeader(int *images, int *rows, int *columns, char *camera, double *exposure);
void SM_stopAD();
double get_traceYGain(int gain);
void lpass_gauss_filter();
void enableDataButtons();
void FilterTrace();
int FilterWholeData();
void resetDispSettings();
void savePHT_vales();
int getTiffExpTime(void);
void SM_readNPdata(char *filename);
void SM_readBNC_only(char *filename);
void SM_saveBNC_only(char *filename);
void SM_saveTxt_image(char *filename);
int setTabOn(int win);
int isMovSlider(int win);
void setMovieSlider(int mouseX, int win);
void setOverlayButtons();
void setMvScButtons();
void setMvMdButtons();
void checkMovScale();
void PopFileInitializeTiff(HWND hwnd);
void paintSubFrame(int x, int y, int dx, int dy, int &color);
void setOmitButtons();
void spatial_filter(int movie_fr);
void ROI_Omit();
int isMovieTrace(int win, int mouseX, int mouseY);
void setMoiveFrame(int mouseX);
void setTraceGainWheel(int inc);
void setTraceExpWheel(int inc);
int displayHelp(void);
void traceFFT();
void Draw_seq(char *seqFname);
void set_seqZoom(int zoom_inc, int mouseX);
void setCurSequence();
void editSequence(int mouseX, int mouseY, int flag);
void convertTmpDiskFile(char *tmpD_name, char *newFileName);
void subtractCDS(unsigned short int *image_data, int n, unsigned int width, unsigned int height, unsigned int length, int factor, int QUAD, int TWOK, int num_pdvChan);
void frame_deInterleave(int length, unsigned *lookuptable, unsigned short *old_images, unsigned short *new_images);
void frameRepair(unsigned short int *image_buffer, int bad_pix_index, int twoKFlag, unsigned int image_width, unsigned int file_width, unsigned int file_height, int horizontal_bin, int superFrame, int file_img_w, int binFlag, int segmented);
int	makeLookUpTable(unsigned int *lut, int image_width, int image_height, int file_width, int file_height, int stripes, int layers, int factor, int QUAD, int CDS, int ROTATE, int TWOK);
void setNewReps(int mouseX);
void drawLittleZhou(void);
void savePS_traces(char *ps_name);
void zhouEnter(void);
void rearrange_MultiFiles(void);
void rearrange_File(int images, int file_id, int numChannels, int quadFlag, int file_img_w, int segmented);
void makeColorLUTs(void);
int checkCfgLUT(void);
int get_pcdWH(int config_id, int height);
void SFC_scan_control(int flag);
void startSFC_scan(int SFC_trigger);
void RunScript();


