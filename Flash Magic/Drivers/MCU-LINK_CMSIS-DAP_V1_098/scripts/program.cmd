@echo off
REM Script to program CMSIS-DAP image onto MCU-Link board
REM Arguments: Null, Path to binary, Help

setlocal
setlocal enabledelayedexpansion

set version=v0.0.4 August 2020

set ScriptHome=%~d0%~p0
set BinHome=%ScriptHome%\..\bin
set ImageHome=%ScriptHome%\..\probe_firmware
set PATH=%BinHome%;%PATH%
set ScryptLog=%temp%\MCULinkScrypt.log

if /i "%1" == "Help" (
    goto :Usage
)

REM Opening Banner
CLS
echo CMSIS-DAP firmware for MCU-Link programming script %version%.
echo 
echo "Place the board in ISP USB mode using the appropriate jumper on your MCU-Link."
echo "Refer to the board documentation for more information."
echo "Connect board via USB then press Space."

:while1
echo.
pause
echo.

if /i "%1" == "" (
    set MCULinkImageWild=MCU-LINK_CMSIS-DAP_V*.s19
)
call :getMCULinkImage

REM if no match, %1 may contain path to image

if "%MCULinkImage%" == "" set MCULinkImage=%1
if "%MCULinkImage%" == "" goto :NoMCULinkImage

REM echo %MCULinkImage%

for %%i in (%MCULinkImage%) do set ShortMCULinkImage="%%~nxi"

call :CheckReady
echo.

echo Programming %ShortMCULinkImage%
call :ProgramPart

echo Connect Next Board then press Space (or CTRL-C to Quit)
goto :while1

:Usage
    echo Usage:
    echo "Connect a MCU-Link probe configured for ISP USB mode."
    echo This script will program the probe with a CMSIS-DAP image.
    echo The following arguments are accepted:
    echo                 [Programs CMSIS-DAP Image]
    echo "path to bin"   [Programs the chosen binary]
    goto :end

:end
    endlocal
    pause
    goto :eof

:NoMCULinkImage
    echo No Probe image found
    goto :Usage

:getMCULinkImage
    for /r "%ImageHome%" %%f in (%MCULinkImageWild%) do (
        set MCULinkImage=%%f
    )
    goto :eof

:CheckReady
    timeout /T 1 >NUL
    echo | set /p none=
    blhost --usb 0x1fc9,0x0021 get-property 1 2>&1 | findstr /r "Success\." >NUL
    REM echo %errorlevel%  
    if %errorlevel% neq 0 (
        echo Configure board for ISP mode and connect via USB then press Space.
        pause
        goto :CheckReady
    )
    goto :eof
	
:ProgramPart
    blhost --usb 0x1fc9,0x0021 flash-image "%MCULinkImage%" erase 0 >NUL 2>&1 >%ScryptLog%
    if %errorlevel% neq 0 goto :progloopend
    findstr /r "Success\." %ScryptLog% >NUL
    if %errorlevel% equ 0 goto :progsuccess
    goto :progloopend
	
:progloopend
    type %ScryptLog%
    echo.
    echo Programming %MCULinkImage% has failed!
    goto :eof

:progsuccess
    echo.
    <NUL set /p =Programmed successfully 
    echo - To use: remove ISP jumper and reboot.
    echo.
    goto :eof

:eof
