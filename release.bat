@echo off
setlocal
chcp 65001 >nul

set "ROOT_DIR=%~dp0"
for %%I in ("%ROOT_DIR%.") do set "ROOT_DIR=%%~fI"

set "DIST_DIR=%ROOT_DIR%\dist"
set "RELEASE_EXE=%DIST_DIR%\Mine3D-windows-x64.exe"

call "%ROOT_DIR%\build.bat"
if errorlevel 1 exit /b %errorlevel%

if not exist "%DIST_DIR%" mkdir "%DIST_DIR%"
if errorlevel 1 exit /b 1

copy /y "%ROOT_DIR%\Mine3D.exe" "%RELEASE_EXE%" >nul
if errorlevel 1 exit /b 1

echo [OK] Created %RELEASE_EXE%
exit /b 0
