@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by SIMUL.HPJ. >"hlp\Simul.hm"
echo. >>"hlp\Simul.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Simul.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Simul.hm"
echo. >>"hlp\Simul.hm"
echo // Prompts (IDP_*) >>"hlp\Simul.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\Simul.hm"
echo. >>"hlp\Simul.hm"
echo // Resources (IDR_*) >>"hlp\Simul.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\Simul.hm"
echo. >>"hlp\Simul.hm"
echo // Dialogs (IDD_*) >>"hlp\Simul.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Simul.hm"
echo. >>"hlp\Simul.hm"
echo // Frame Controls (IDW_*) >>"hlp\Simul.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\Simul.hm"
REM -- Make help for Project SIMUL


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\Simul.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Simul.hlp" goto :Error
if not exist "hlp\Simul.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Simul.hlp" Debug
if exist Debug\nul copy "hlp\Simul.cnt" Debug
if exist Release\nul copy "hlp\Simul.hlp" Release
if exist Release\nul copy "hlp\Simul.cnt" Release
echo.
goto :done

:Error
echo hlp\Simul.hpj(1) : error: Problem encountered creating help file

:done
echo.
