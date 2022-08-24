
@setlocal

@set board=PDV
@set LOG=CHANGELOG_PDV.txt
@set /P VER=< version

@call findstr %VER% %LOG%

@if %errorlevel% EQU 0 goto changelog_ok
@echo.
@echo ************************************************************
@echo *****   No entry for ver. %VER% found in changelog.  *****
@echo *****   Update shared/%LOG% then re-run    *****
@echo *****   getsource -s, and this script.                 *****
@echo ************************************************************
endlocal
@exit /b 1
:changelog_ok

set BUILD_ID=1001
call "c:\Program Files (x86)\Microsoft Visual Studio 9.0\vc\bin\vcvars32.bat"

@if %errorlevel% EQU 0 goto continue0
@echo "failed vcvars32.bat -- check your build environment!"
endlocal
@exit /b 1
:continue0

which cl | findstr 64

if errorlevel 1 (set ARCH=x86) else set ARCH=amd64

@echo "ARCH = %ARCH%"

nmake -C -f makefile.pkg %1%

@if %errorlevel% EQU 0 goto continue1
@echo "%0%: nmake returned ERROR %errorlevel%, exiting"
endlocal
@exit /b 1
:continue1

endlocal

setlocal

call "c:\Program Files (x86)\Microsoft Visual Studio 9.0\vc\bin\x86_amd64\vcvarsx86_amd64.bat"

@if %errorlevel% EQU 0 goto continue2
@echo "%0%: failed vcvars64.bat -- check your build environment!"
@exit /b 1
:continue2

which cl | findstr 64

if errorlevel 1 (set ARCH=x86) else set ARCH=amd64

@echo "ARCH = %ARCH%"

nmake -C -f makefile.pkg %1%

@if %errorlevel% EQU 0 goto end
@echo "%0%: nmake %1% returned ERROR %errorlevel%, exiting"
endlocal
@exit /b 1

:end
endlocal
exit /b 0

