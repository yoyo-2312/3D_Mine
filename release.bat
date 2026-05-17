@echo off
setlocal
chcp 65001 >nul

set "ROOT_DIR=%~dp0"
for %%I in ("%ROOT_DIR%.") do set "ROOT_DIR=%%~fI"

set "DIST_DIR=%ROOT_DIR%\dist"
set "STAGE_DIR=%DIST_DIR%\Mine3D-windows-x64"
set "ZIP_PATH=%DIST_DIR%\Mine3D-windows-x64.zip"

call "%ROOT_DIR%\build.bat"
if errorlevel 1 exit /b %errorlevel%

if exist "%STAGE_DIR%" rmdir /s /q "%STAGE_DIR%"
if exist "%ZIP_PATH%" del /q "%ZIP_PATH%"

mkdir "%STAGE_DIR%"
if errorlevel 1 exit /b 1

copy /y "%ROOT_DIR%\Mine3D.exe" "%STAGE_DIR%\" >nul
if errorlevel 1 exit /b 1

xcopy "%ROOT_DIR%\Sweep_IMG" "%STAGE_DIR%\Sweep_IMG\" /e /i /y >nul
if errorlevel 1 exit /b 1

xcopy "%ROOT_DIR%\Sweep_MP3" "%STAGE_DIR%\Sweep_MP3\" /e /i /y >nul
if errorlevel 1 exit /b 1

powershell -NoProfile -Command ^
    "$ErrorActionPreference='Stop';" ^
    "Compress-Archive -Path '%STAGE_DIR%\\*' -DestinationPath '%ZIP_PATH%' -Force"
if errorlevel 1 exit /b 1

echo [OK] Created %ZIP_PATH%
exit /b 0
