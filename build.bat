@echo off
setlocal
chcp 65001 >nul

set "TARGET=Mine3D.exe"
set "EASYX_VERSION=25.9.10"
set "EASYX_URL=https://easyx.cn/download/easyx4mingw_%EASYX_VERSION%.zip"
set "DEPS_DIR=%~dp0.deps"
set "AUTO_EASYX_DIR=%DEPS_DIR%\easyx4mingw_%EASYX_VERSION%"

for /f "delims=" %%I in ('where g++ 2^>nul') do (
    if not defined CXX_EXE set "CXX_EXE=%%I"
)

if not defined CXX_EXE (
    echo [ERROR] g++.exe was not found in PATH.
    echo Install MinGW-w64 and add its bin directory to PATH.
    exit /b 1
)

for %%I in ("%CXX_EXE%") do set "MINGW_BIN=%%~dpI"
for %%I in ("%MINGW_BIN%..") do set "MINGW_ROOT=%%~fI"
for /f "delims=" %%I in ('"%CXX_EXE%" -dumpmachine') do set "GCC_MACHINE=%%I"

set "EASYX_LIB_DIR=lib32"
echo %GCC_MACHINE% | findstr /i "x86_64 amd64" >nul
if %errorlevel% equ 0 set "EASYX_LIB_DIR=lib64"

set "EASYX_CFLAGS="
set "EASYX_LDFLAGS="

if not defined EASYX_DIR set "EASYX_DIR=%AUTO_EASYX_DIR%"

if not exist "%EASYX_DIR%\include\graphics.h" (
    if /i not "%EASYX_DIR%"=="%AUTO_EASYX_DIR%" (
        echo [ERROR] graphics.h was not found in:
        echo         %EASYX_DIR%\include
        exit /b 1
    )

    if not exist "%DEPS_DIR%" mkdir "%DEPS_DIR%"
    if errorlevel 1 exit /b 1

    echo [INFO] MinGW-compatible EasyX was not found.
    echo [INFO] Downloading %EASYX_URL%

    powershell -NoProfile -ExecutionPolicy Bypass -Command ^
        "$ErrorActionPreference='Stop';" ^
        "$zip=Join-Path $env:DEPS_DIR 'easyx4mingw_%EASYX_VERSION%.zip';" ^
        "Invoke-WebRequest -Uri $env:EASYX_URL -OutFile $zip;" ^
        "if (Test-Path $env:AUTO_EASYX_DIR) { Remove-Item -Recurse -Force $env:AUTO_EASYX_DIR };" ^
        "New-Item -ItemType Directory -Force -Path $env:AUTO_EASYX_DIR | Out-Null;" ^
        "Expand-Archive -LiteralPath $zip -DestinationPath $env:AUTO_EASYX_DIR -Force;" ^
        "$root=Get-ChildItem -Path $env:AUTO_EASYX_DIR -Recurse -Filter graphics.h | Select-Object -First 1 -ExpandProperty DirectoryName;" ^
        "if (-not $root) { throw 'graphics.h was not found in the EasyX archive.' };" ^
        "if ((Split-Path $root -Leaf) -eq 'include') { $pkg=Split-Path $root -Parent } else { $pkg=$env:AUTO_EASYX_DIR };" ^
        "if ($pkg -ne $env:AUTO_EASYX_DIR) { Get-ChildItem -LiteralPath $pkg | Move-Item -Destination $env:AUTO_EASYX_DIR -Force }"
    if errorlevel 1 (
        echo [ERROR] Failed to download or extract EasyX for MinGW.
        exit /b 1
    )
)

if not exist "%EASYX_DIR%\include\graphics.h" (
    echo [ERROR] graphics.h was not found in:
    echo         %EASYX_DIR%\include
    exit /b 1
)

if not exist "%EASYX_DIR%\%EASYX_LIB_DIR%\libeasyx.a" (
    echo [ERROR] EasyX MinGW library was not found in:
    echo         %EASYX_DIR%\%EASYX_LIB_DIR%
    echo Expected libeasyx.a.
    exit /b 1
)

set "EASYX_CFLAGS=-I%EASYX_DIR%\include"
set "EASYX_LDFLAGS=-L%EASYX_DIR%\%EASYX_LIB_DIR% -leasyx"

powershell -NoProfile -Command "if (Get-Process -Name Mine3D -ErrorAction SilentlyContinue) { exit 0 } else { exit 1 }" >nul
if %errorlevel% equ 0 (
    echo [INFO] %TARGET% is still running.
    echo [INFO] Closing it before rebuilding...

    powershell -NoProfile -Command "Get-Process -Name Mine3D -ErrorAction SilentlyContinue | Stop-Process -Force"
    if errorlevel 1 (
        echo [ERROR] Failed to close %TARGET%. Please close it from Task Manager and try again.
        exit /b 1
    )
)

"%CXX_EXE%" -std=c++17 -g -O0 -w -finput-charset=UTF-8 -fexec-charset=UTF-8 %EASYX_CFLAGS% ^
    -o "%TARGET%" ^
    "Main Func.cpp" ^
    "Game_Mine.cpp" ^
    "algorithm functions.cpp" ^
    "draw functions.cpp" ^
    "mingw_compat.cpp" ^
    %EASYX_LDFLAGS% -lwinmm -lgdi32 -luser32 -lole32 -luuid

if errorlevel 1 exit /b %errorlevel%

echo [OK] Built %TARGET%
exit /b 0
