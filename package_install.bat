@echo off
setlocal
rem --- Locate and load the MSVC build environment via vswhere (any VS 2019+) ---
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" set "VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -prerelease -find VC\Auxiliary\Build\vcvars64.bat`) do set "VCVARS=%%i"
if not defined VCVARS ( echo ERROR: could not locate vcvars64.bat via vswhere. & exit /b 1 )
rem Put the VS Installer dir on PATH so vcvars' own internal vswhere calls resolve quietly
for %%d in ("%VSWHERE%") do set "PATH=%%~dpd;%PATH%"
call "%VCVARS%"

rem --- Use the MSVC-built Ruby (adjust this path for your environment) ---
if exist C:\RubyMSVC34\bin set "PATH=C:\RubyMSVC34\bin;%PATH%"

rem --- Derive the version so the gem filename is never hardcoded ---
for /f "delims=" %%v in ('ruby -Ilib -e "require 'oj_windows/version'; print Oj::VERSION"') do set "OJ_VER=%%v"
if not defined OJ_VER ( echo ERROR: could not read Oj::VERSION. & exit /b 1 )
echo Building oj_windows %OJ_VER% ...

call gem build oj_windows.gemspec
if %errorlevel% neq 0 exit /b %errorlevel%
call gem install "oj_windows-%OJ_VER%.gem"
if %errorlevel% neq 0 exit /b %errorlevel%
echo Success: installed oj_windows %OJ_VER%
endlocal
