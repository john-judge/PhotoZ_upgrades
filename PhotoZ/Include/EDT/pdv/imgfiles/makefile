!include ../makefile.def

CFG=Release
PDVHOME=..

CC = cl

OPTIM = -O2 -DNDEBUG -D_MBCS -MD


CFLAGS = $(OPTIM) -DWIN32 -nologo $(CFLAGS) -I$(PDVHOME)/pdvplus -I$(PDVHOME)/tiff/libtiff


LIBOBJS = \
$(CFGNAME)/Imagefiletiff.obj \
$(CFGNAME)/imagefile.obj \
$(CFGNAME)/imagefilebmp.obj \
$(CFGNAME)/imagefilemgr.obj \
$(CFGNAME)/imagefileraw.obj \
$(CFGNAME)/imagefiletype.obj 

LIB = ../lib/$(ARCH)/imgfiles.lib

all : $(LIB)

dir : $(CFGNAME)

$(CFGNAME):
	if not exist $(CFGNAME)/ mkdir $(CFGNAME)

$(LIB) : dir $(LIBOBJS)
	$(LIB32) $(LIBFLAGS) /OUT:$(LIB) $(LIBOBJS) 

{}.c{$(CFGNAME)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(CFGNAME)\ $<

{}.cpp{$(CFGNAME)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(CFGNAME)\ $<

