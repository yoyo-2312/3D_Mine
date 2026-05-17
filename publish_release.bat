@echo off
setlocal
chcp 65001 >nul

if "%~1"=="" (
    echo Usage: publish_release.bat ^<tag^> ["release title"]
    exit /b 1
)

set "ROOT_DIR=%~dp0"
for %%I in ("%ROOT_DIR%.") do set "ROOT_DIR=%%~fI"

set "TAG=%~1"
set "TITLE=%~2"
if not defined TITLE set "TITLE=%TAG%"

set "RELEASE_EXE=%ROOT_DIR%\dist\Mine3D-windows-x64.exe"
if not exist "%RELEASE_EXE%" (
    call "%ROOT_DIR%\release.bat"
    if errorlevel 1 exit /b %errorlevel%
)

for /f "delims=" %%I in ('where gh 2^>nul') do (
    if not defined GH_EXE set "GH_EXE=%%I"
)

if not defined GH_EXE if exist "%LOCALAPPDATA%\Programs\GitHubCLI\bin\gh.exe" set "GH_EXE=%LOCALAPPDATA%\Programs\GitHubCLI\bin\gh.exe"

if not defined GH_EXE (
    echo [ERROR] GitHub CLI was not found.
    exit /b 1
)

"%GH_EXE%" auth status >nul 2>nul
if errorlevel 1 (
    echo [ERROR] GitHub CLI is not authenticated.
    echo Run: "%GH_EXE%" auth login --web --git-protocol ssh --skip-ssh-key
    exit /b 1
)

"%GH_EXE%" release create "%TAG%" "%RELEASE_EXE%" --title "%TITLE%" --generate-notes
if errorlevel 1 exit /b %errorlevel%

echo [OK] Published release %TAG%
exit /b 0
