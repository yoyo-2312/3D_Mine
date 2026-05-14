@echo off
setlocal

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [ERROR] vswhere.exe was not found.
    echo Install Visual Studio 2022 first.
    exit /b 1
)

for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALLDIR=%%I"
if not defined VSINSTALLDIR (
    echo [ERROR] Visual Studio with C++ tools was not found.
    exit /b 1
)

where cl.exe >nul 2>nul
if errorlevel 1 (
    call "%VSINSTALLDIR%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul
    if errorlevel 1 (
        echo [ERROR] Failed to initialize the Visual Studio build environment.
        exit /b 1
    )
)

set "EASYX_LIB=%VSINSTALLDIR%\VC\Auxiliary\VS\lib\x64"
if not exist "%EASYX_LIB%\EasyXa.lib" (
    echo [ERROR] EasyXa.lib was not found in:
    echo         %EASYX_LIB%
    echo Install EasyX for this Visual Studio instance first.
    exit /b 1
)

powershell -NoProfile -Command "if (Get-Process -Name Mine3D -ErrorAction SilentlyContinue) { exit 0 } else { exit 1 }" >nul
if %errorlevel% equ 0 (
    echo [ERROR] Mine3D.exe is currently running.
    echo Close the game before rebuilding.
    exit /b 1
)

cl.exe /Zi /EHsc /nologo /utf-8 /Fe:Mine3D.exe ^
    "Main Func.cpp" ^
    "Game_Mine.cpp" ^
    "algorithm functions.cpp" ^
    "draw functions.cpp" ^
    /link /LIBPATH:"%EASYX_LIB%" user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

exit /b %errorlevel%
