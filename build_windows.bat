@echo off
setlocal EnableExtensions

set "CONFIG_PRESET=windows-x64"
set "BUILD_PRESET=windows-x64-release"
set "BUILD_DIR=out\build\windows-x64"
set "JOBS=%NUMBER_OF_PROCESSORS%"

if not defined JOBS set "JOBS=4"

echo ==========================================
echo Building Fallout 2 CE (Windows x64 Release)
echo ==========================================

if /I "%~1"=="--reconfigure" goto :configure

if exist "%BUILD_DIR%\CMakeCache.txt" (
    echo Configure step skipped. Use --reconfigure to force it.
    goto :build
)

:configure
echo Running configure preset: %CONFIG_PRESET%
cmake --preset %CONFIG_PRESET%
if errorlevel 1 goto :failed

:build
echo Building with preset: %BUILD_PRESET% (jobs=%JOBS%)
powershell -NoProfile -Command "$sw = [Diagnostics.Stopwatch]::StartNew(); cmake --build --preset '%BUILD_PRESET%' --parallel %JOBS%; $code = $LASTEXITCODE; $sw.Stop(); Write-Host ('Build completed in: {0}' -f $sw.Elapsed); exit $code"
if errorlevel 1 goto :failed

echo Build success.
exit /b 0

:failed
echo Build failed with error code %ERRORLEVEL%
exit /b %ERRORLEVEL%
