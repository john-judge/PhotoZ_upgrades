
!include ../makefile.def



CC = cl

OPTIM = -O2

HEADER_DIR = ..

CFLAGS = /EHsc $(OPTIM) -MD -DWIN32 -nologo -I$(HEADER_DIR) -DUSE_MMX 
CCFLAGS = $(OPTIM) -MD -DWIN32 -nologo -I$(HEADER_DIR) -DUSE_MMX

LIBS = pdvlib.lib edt_mmx.lib

LINK32 = link
DLLFLAGS = /dll /nologo /link50compat /map

LIB32 = link -lib

LIBOBJECTS = \
	$(CFGNAME)/BufferTimerData.obj \
	$(CFGNAME)/ColorBalance.obj \
	$(CFGNAME)/EdtBGSubtractor.obj \
	$(CFGNAME)/EdtBufHeader.obj \
	$(CFGNAME)/EdtDataHeader.obj \
	$(CFGNAME)/EdtHistogram.obj \
	$(CFGNAME)/EdtImage.obj \
	$(CFGNAME)/EdtImageData.obj \
	$(CFGNAME)/EdtImgArith.obj \
	$(CFGNAME)/EdtImgDyadOp.obj \
	$(CFGNAME)/EdtImgMinMax.obj \
	$(CFGNAME)/EdtImgOp.obj \
	$(CFGNAME)/EdtImgProfile.obj \
	$(CFGNAME)/EdtLut.obj \
	$(CFGNAME)/EdtScaledLut.obj \
	$(CFGNAME)/ErrorHandler.obj \
	$(CFGNAME)/SocketConnection.obj \
	$(CFGNAME)/StringUtils.obj 

TARGET=../lib/$(ARCH)/edtimage.lib
$(TARGET): dir $(LIBOBJECTS) $(INCLUDES) 
	 link /lib /out:$(TARGET) $(LIBOBJECTS)


dir:
	-mkdir $(CFGNAME)

objclean:
	-del  *.obj *.lib *.dll core *~ 




#{}.c{$(CFGNAME)}.obj:
#	$(CC) -c $(CFLAGS) -Fo$(CFGNAME)/ $<

{}.cpp{$(CFGNAME)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(CFGNAME)/ $<









