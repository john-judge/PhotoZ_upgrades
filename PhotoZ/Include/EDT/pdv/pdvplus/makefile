
!include ../makefile.def


CC = cl

OPTIM = -O2

HEADER_DIR = ..

CFLAGS = $(OPTIM) -MD -DWIN32 -nologo -I$(HEADER_DIR) 
CCFLAGS = $(OPTIM) -MD -DWIN32 -nologo -I$(HEADER_DIR) 

LIBS = pdvlib.lib edt_mmx.lib

LINK32 = link
DLLFLAGS = /dll /nologo /link50compat /map

LIB32 = link -lib

LIBOBJECTS = \
	$(CFGNAME)/PdvInput.obj \
	$(CFGNAME)/PdvConfig.obj \
	$(CFGNAME)/PdvConfigSet.obj 


LIBDIR=../lib/$(ARCH)
PDVPLUSLIB=$(LIBDIR)/pdvplus.lib

$(PDVPLUSLIB): dir $(LIBOBJECTS) $(INCLUDES) 
	 link /lib /out:$(PDVPLUSLIB) $(LIBOBJECTS)


dir:
	-mkdir $(CFGNAME)

objclean:
	-del  *.obj *.lib *.dll core *~ 




#{}.c{$(CFGNAME)}.obj:
#	$(CC) -c $(CFLAGS) -Fo$(CFGNAME)/ $<

{}.cpp{$(CFGNAME)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(CFGNAME)/ $<









