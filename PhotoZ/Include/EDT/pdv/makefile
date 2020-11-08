#makefile for pdv executables
# Copyright EDT 1998

PDVHOME=.

!include makefile.def

!include includes.mk 

# _CRT_SECURE_NO_DEPRECATE gets rid of warnings about strcpy 
# being unsafe (we don't want to use proprietary M$ strcpy_s)
# It is useful if using Vis. Studio 2005 or its 'cl' compiler

TARGETLIB=pdvlib.lib
TARGETDLL=pdvlib.dll

ARCHLIB=$(LIBDIR)\$(TARGETLIB)
ARCHDLL=$(BINDIR)\$(TARGETDLL)

all: $(LIBS) $(TARGETS) pciload.exe

$(TARGETS) $(EXTRATARGETS): $(CFGNAME) $(CFGNAME)/$*.obj $(INCLUDES) $(LIBS)
	$(CC) $(CFLAGS) $(CFGNAME)/$*.obj /Fe$(BINDIR)/$*.exe $(LIBS) $(WINLIBS) /link $(LDFLAGS)
	@if exist "$(BINDIR)\$@.manifest" mt.exe /nologo -manifest $(BINDIR)\$*.exe.manifest -outputresource:$(BINDIR)\$*.exe
	@if exist "$(BINDIR)\$@.manifest" del $(BINDIR)\$*.exe.manifest


#static linking

$(ARCHLIB): dir $(LIBOBJS) $(INCLUDES) 
	@$(LINK32) $(DLLFLAGS) /out:$(ARCHDLL) /implib:$(ARCHLIB) $(LIBOBJS) $(MMX_LIB)
	@if exist "$(ARCHDLL).manifest" mt.exe /nologo -manifest $(ARCHDLL).manifest -outputresource:$(ARCHDLL);2
	@if exist "$(ARCHDLL).manifest" del $(ARCHDLL).manifest

dir: $(CFGNAME) $(BINDIR) $(LIBDIR)

$(CFGNAME):
	if not exist "$(CFGNAME)/" mkdir $(CFGNAME)

$(BINDIR):
	if not exist "$(BINDIR)/" mkdir $(BINDIR)

$(LIBDIR):
	if not exist "$(LIBDIR)/" mkdir $(LIBDIR)

$(LIBDIR)/libtiff.lib:
	cd tiff\libtiff
	del *.obj
	$(MAKE) -f makefile.vc
	cd ..\..
	move libtiff.lib $(LIBDIR)
	
PCILOADOBJS = $(CFGNAME)/pciload_main.obj $(CFGNAME)/pciload_fn.obj \
	$(CFGNAME)/pciload_4013e.obj $(CFGNAME)/pciload_4013xla.obj $(CFGNAME)/pciload_4028xla.obj $(CFGNAME)/pciload_spi.obj

pciload.exe : $(PCILOADOBJS) $(INCLUDES) 
	$(LINK32) /out:$(BINDIR)/$@ $(PCILOADOBJS) $(LIBS)
	if exist "$(BINDIR)\$@.manifest" mt.exe -manifest $(BINDIR)\$*.exe.manifest -outputresource:$(BINDIR)\$*.exe
	if exist "$(BINDIR)\$@.manifest" del $(BINDIR)\$*.exe.manifest

pdvshow.exe : pdvplus.lib imgfiles.lib
	cd pdvshow
	nmake -C -f makefile
	cd ..
    copy bin\$(ARCH)\pdvshow.exe .
	

pdvplus.lib:
	cd pdvplus
	nmake -C -f makefile
	cd ..

$(LIBDIR)/edtimage.lib:
	cd edtimage
	nmake -C -f makefile
	cd ..

imgfiles.lib:
	cd imgfiles
	nmake -C -f makefile
	cd ..

$(LIBDIR)/clseredt.lib:
	cd edt_camlink
	nmake -C -f makefile.nt
	cd ..

objclean: 
	-del *.exe.manifest $(CFGNAME)\*.obj *.ilk *.pch 

clean: objclean
	-del $(TARGETS) $(TARGETLIB) $(ARCHLIB) $(TARGETDLL) $(ARCHDLL) pciload.exe
    -cd pdvshow; nmake -C -f makefile ntclean

{}.c{$(CFGNAME)}.obj:
	$(CC) -c $(CFLAGS) -Fo$(CFGNAME)\ $<

{}.cpp{$(CFGNAME)}.obj:
	(CC) -c $(CFLAGS) -Fo$(CFGNAME)\ $<

