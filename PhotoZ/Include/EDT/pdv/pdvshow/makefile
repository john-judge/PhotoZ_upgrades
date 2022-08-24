#makefile for pdv executables
# Copyright EDT 2002


!include ../makefile.def

HEADER_DIR =.

INCLUDES_FLAGS = -I.. -I../edtimage -I../pdvplus -I../imgfiles -I../edtcamdll -I../tifflib -I../imgfiles

OUTDIR = ..\pdv

#can be overridden on the command line

CFG=Release

INTDIR=$(CFG)$(ARCH)

OPTIM = -O2

!IFNDEF BUILD_ID

BUILD_ID=""

!ENDIF

LIBS = edtimage.lib pdvplus.lib pdvlib.lib libtiff.lib imgfiles.lib 

# .SILENT:

!IF "$(CFG)" == "Release"


CFLAGS = -W3 -O2 -MD /EHsc -nologo $(INCLUDES_FLAGS) -DBUILD_ID=$(BUILD_ID)  /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

LINK32_FLAGS=$(LIBS) /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\pdvshow.pdb" /out:"../bin/$(ARCH)/pdvshow.exe" /libpath:".." /libpath:"../lib/$(ARCH)" 

!ELSE


CFLAGS = -W3 -Od -MD /EHsc -nologo $(INCLUDES_FLAGS)  -DUSE_MMX -DBUILD_ID=$(BUILD_ID) /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXEXT" /D "_NT_" /D "_WINDLL" /D "_AFXDLL" /FD /c 


LINK32_FLAGS=$(LIBS) /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\pdvshow.pdb" /debug /machine:I386 /out:"../pdvshow.exe" /pdbtype:sept /libpath:".." /libpath:"../lib/debug" 


!ENDIF

RSC_FLAGS= /l 0x409 /d "_DEBUG" /d "_AFXDLL" 

CCFLAGS = $(CFLAGS)

LINK32_OBJS= \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ConfigDataSet.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\pdvshow.obj" \
	"$(INTDIR)\pdvshowDoc.obj" \
	"$(INTDIR)\pdvshowView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\pdvshow.res" \
	"$(INTDIR)\BinningDlg.obj" \
	"$(INTDIR)\camcontroldlg.obj" \
	"$(INTDIR)\camprogramdlg.obj" \
	"$(INTDIR)\CamSelectDialog.obj" \
	"$(INTDIR)\contrastdlg.obj" \
	"$(INTDIR)\DalsaCameraControl.obj" \
	"$(INTDIR)\DataPanelDlg.obj" \
	"$(INTDIR)\dirname.obj" \
	"$(INTDIR)\DisplaySkipDlg.obj" \
	"$(INTDIR)\EdtImageFrame.obj" \
	"$(INTDIR)\edtimagetext.obj" \
	"$(INTDIR)\edtimagetextdlg.obj" \
	"$(INTDIR)\EdtImageWin.obj" \
	"$(INTDIR)\EdtLiveFrame.obj" \
	"$(INTDIR)\edtplotwindow.obj" \
	"$(INTDIR)\gammadialog.obj" \
	"$(INTDIR)\histdialog.obj" \
	"$(INTDIR)\KeyboardInput.obj" \
	"$(INTDIR)\LedButton.obj" \
	"$(INTDIR)\profiledialog.obj" \
	"$(INTDIR)\RecordLogDlg.obj" \
	"$(INTDIR)\RoiDlg.obj" \
	"$(INTDIR)\SoftwareRGBDlg.obj" \
	"$(INTDIR)\WhiteBalanceSetup.obj" \
	"$(INTDIR)\WindowPaint.obj" \
	"$(INTDIR)\MsWindowPaint.obj" \
	"$(INTDIR)\LiveDisplay.obj" \
	"$(INTDIR)\ImageWindowData.obj" \
	"$(INTDIR)\ZoomDialog.obj"



LINK32=link.exe

TARGETS = ..\bin\$(ARCH)\pdvshow.exe

all: pdvplus imgfiles edtimage dir $(TARGETS)


$(TARGETS): $(INTDIR) $(LINK32_OBJS) 
	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)
	if exist $@.manifest mt -manifest $@.manifest -outputresource:$@
	if exist $@.manifest del $@.manifest

{}.c{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<

{}.cpp{$(INTDIR)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(INTDIR)\ $<

{}.rc{$(INTDIR)}.res:
	rc $(RSC_FLAGS) -fo$*.res $<

ntclean: dir
	-del $(INTDIR)\*.*  $(TARGETS)

dir : $(INTDIR)

$(INTDIR):
	if not exist $(INTDIR)/ mkdir $(INTDIR)

clean: ntclean

pdvplus:
	cd ..\pdvplus
	nmake 
	cd ..\pdvshow

imgfiles:
	cd ..\imgfiles
	nmake 
	cd ..\pdvshow
	
edtimage:
	cd ..\edtimage
	nmake 
	cd ..\pdvshow
	
	
